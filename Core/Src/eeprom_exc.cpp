/**
  ******************************************************************************
  * @file    EEPROM_Emulation/src/eeprom.c 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   This file provides all the EEPROM emulation firmware functions.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 
/** @addtogroup EEPROM_Emulation
  * @{
  */ 

/* Includes ------------------------------------------------------------------*/
#include <eeprom_exc.h>
#include <stdexcept>

uint16_t DataVar = 0;   /* Global variable used to store variable value in read sequence */

/* Virtual address defined by the user: 0xFFFF value is prohibited */

/* Private functions ---------------------------------------------------------*/
void EE_Format(void);
uint16_t EE_FindValidPage(operationValidPageT Operation);
void EE_VerifyPageFullWriteVariable(uint16_t VirtAddress, uint16_t Data);
FlashStatusTypeDef EE_PageTransfer(uint16_t VirtAddress, uint16_t Data);


void FLASH_Program_HalfWord(uint32_t Address, uint16_t Data)
{
    if ( HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Address, Data))
            throw std::exception();
}

void FLASH_ErasePage(const uint32_t address)
{
    FLASH_EraseInitTypeDef eraseCfg = {FLASH_TYPEERASE_PAGES, FLASH_BANK_1, address, 1};
    uint32_t PageError;
    if (HAL_OK !=  HAL_FLASHEx_Erase(&eraseCfg, &PageError))
        throw std::exception();
}


void FLASH_transfer_page(const uint32_t page_addrFrom, const uint32_t page_addrTo)
{
    int16_t x = -1;
    /* Transfer data from Page0 to Page1 */
    for (int VarIdx = 0; VarIdx < NumbOfVar; VarIdx++)
    {
      if ((*(__IO uint16_t*)(page_addrTo + 6)) == VarIdx)
        x = VarIdx;
      if (VarIdx != x)
      {

      if(FLASH_OPER_ERR == EE_ReadVariable(VarIdx, &DataVar)) /* Read the last variables' updates */
        /* In case variable corresponding to the virtual address was found */
          /* Transfer the variable to the Page1 */
          EE_VerifyPageFullWriteVariable(VarIdx, DataVar);
      else
          throw std::exception(); //variable not found
      }
    }
    FLASH_Program_HalfWord(PAGE1_BASE_ADDRESS, VALID_PAGE); /* Mark Page1 as valid */
    FLASH_ErasePage(page_addrFrom);
}


/**
  * @brief  Restore the pages to a known good state in case of page's status
  *   corruption after a power loss.
  * @param  None.
  * @retval - Flash error code: on write Flash error
  *         - FLASH_OPER_ERR: on success
  */
FlashStatusTypeDef EE_Init( void )
{
    uint16_t PageStatus0= (*(__IO uint16_t*)PAGE0_BASE_ADDRESS); /* Get Page0 status */
    uint16_t PageStatus1= (*(__IO uint16_t*)PAGE1_BASE_ADDRESS); /* Get Page1 status */
    try{
        /* Check for invalid header states and repair if necessary */
        switch ( PageStatus0 )
        {
            case ERASED:
                switch ( PageStatus1 )
                {
                    case VALID_PAGE: /* Page0 erased, Page1 valid */
                        FLASH_ErasePage(PAGE0_BASE_ADDRESS);
                        break;
                    case RECEIVE_DATA: /* Page0 erased, Page1 receive */
                        FLASH_ErasePage(PAGE0_BASE_ADDRESS);
                        FLASH_Program_HalfWord(PAGE1_BASE_ADDRESS, VALID_PAGE);
                        break;
                    default: /* First EEPROM access (Page0&1 are erased) or invalid state -> format EEPROM */
                        EE_Format(); /* Erase both Page0 and Page1 and set Page0 as valid page */
                        break;
                }
                break;
            case RECEIVE_DATA:
                switch ( PageStatus1 )
                {
                    case VALID_PAGE: /* Page0 receive, Page1 valid */
                        FLASH_transfer_page(PAGE1_BASE_ADDRESS,
                                PAGE0_BASE_ADDRESS);
                        break;
                    case ERASED:
                        FLASH_ErasePage(PAGE1_BASE_ADDRESS);
                        FLASH_Program_HalfWord(PAGE0_BASE_ADDRESS, VALID_PAGE);
                        break;
                    default: /*  invalid state -> format EEPROM */
                        EE_Format(); /* Erase both Page0 and Page1 and set Page0 as valid page */
                        break;
                }
                break;
            case VALID_PAGE:
                switch ( PageStatus1 )
                {
                    case VALID_PAGE:
                        EE_Format();/* Invalid state -> format eeprom */
                        break;
                    case ERASED:
                        FLASH_ErasePage(PAGE1_BASE_ADDRESS);
                        break;
                    default: /* Page0 valid, Page1 receive */
                        FLASH_transfer_page(PAGE0_BASE_ADDRESS,
                                PAGE1_BASE_ADDRESS);
                        break;
                }
                break;

            default: /* Any other state -> format eeprom */
                /* Erase both Page0 and Page1 and set Page0 as valid page */
                EE_Format();
                break;
        }
    } catch ( const std::exception &e ){
        return FLASH_OPER_ERR;
    }

    return FLASH_OPER_ERR;
}

/**
  * @brief  Returns the last stored variable data, if found, which correspond to
  *   the passed virtual address
  * @param  VirtAddress: Variable virtual address
  * @param  Data: Global variable contains the read variable value
  * @retval Success or error status:
  *           - 0: if variable was found
  *           - 1: if the variable was not found
  *           - NO_VALID_PAGE: if no valid page was found.
  */
FlashStatusTypeDef EE_ReadVariable(uint16_t VirtAddress, uint16_t* Data)
{
    uint16_t ValidPage= PAGE0;
    uint16_t AddressValue=0x5555;
    uint32_t CurrentAddress=0x08010000;
    uint32_t PageStartAddress=0x08010000;

    try{
        ValidPage=EE_FindValidPage(READ_FROM_VALID_PAGE); /* Get active Page for read operation */
    } catch ( const std::exception &e ){
        return FLASH_OPER_NO_VALID_PAGE;
    }

    PageStartAddress=(uint32_t) (EEPROM_START_ADDRESS + (uint32_t) (ValidPage*PAGE_SIZE )); /* Get the valid Page start Address */
    CurrentAddress=(uint32_t) ( (EEPROM_START_ADDRESS-2) + (uint32_t) ( (1 + ValidPage) * PAGE_SIZE )); /* Get the valid Page end Address */

    /* Check each active page address starting from end */
    while ( CurrentAddress > (PageStartAddress+2) ){
        AddressValue= (*(__IO uint16_t*)CurrentAddress); /* Get the current location content to be compared with virtual address */
        /* Compare the read address with the virtual address */
        if(AddressValue==VirtAddress){
            *Data = (*(__IO uint16_t*) (CurrentAddress-2)); /* Get content of Address-2 which is variable value */
            return FLASH_OPER_ERR; /* In case variable value is read */
        }else
            CurrentAddress=CurrentAddress-4; /* Next address location */
    }
    return FLASH_OPER_ERR;
}

/**
  * @brief  Writes/upadtes variable data in EEPROM.
  * @param  VirtAddress: Variable virtual address
  * @param  Data: 16 bit data to be written
  * @retval Success or error status:
  *           - FLASH_OPER_ERR: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
FlashStatusTypeDef EE_WriteVariable( uint16_t VirtAddress, uint16_t Data )
{
    FlashStatusTypeDef Status=FLASH_OPER_OK;
    try{
        /* Write the variable virtual address and value in the EEPROM */
        EE_VerifyPageFullWriteVariable(VirtAddress, Data);
    }catch ( const std::range_error &e){ //page full
        try{
            return EE_PageTransfer(VirtAddress, Data); /* Perform Page transfer */
        }catch ( const std::exception &e ){
            return FLASH_OPER_ERR;
        }
    }catch ( const std::exception &e ){
        return FLASH_OPER_ERR;
    }

    return Status;
}




/**
  * @brief  Erases PAGE0 and PAGE1 and writes VALID_PAGE header to PAGE0
  * @param  None
  * @throw  in case of error std:exception
  */
void EE_Format(void)
{
  FLASH_ErasePage(PAGE0_BASE_ADDRESS);
  FLASH_Program_HalfWord(PAGE0_BASE_ADDRESS, VALID_PAGE);   /* Set Page0 as valid page: Write VALID_PAGE at Page0 base address */
  FLASH_ErasePage(PAGE1_BASE_ADDRESS);
}

/**
  * @brief  Find valid Page for write or read operation
  * @param  Operation: operation to achieve on the valid page.
  *   This parameter can be one of the following values:
  *     @arg READ_FROM_VALID_PAGE: read operation from valid page
  *     @arg WRITE_IN_VALID_PAGE: write operation from valid page
  * @retval Valid page number (PAGE0 or PAGE1) or NO_VALID_PAGE in case
  *   of no valid page was found
  */
uint16_t EE_FindValidPage( operationValidPageT Operation )
{
    uint16_t PageStatus0=6, PageStatus1=6;
    PageStatus0= (*(__IO uint16_t*)PAGE0_BASE_ADDRESS); /* Get Page0 actual status */
    PageStatus1= (*(__IO uint16_t*)PAGE1_BASE_ADDRESS); /* Get Page1 actual status */

    /* Write or read operation */
    switch ( Operation )
    {
        case WRITE_IN_VALID_PAGE: /* ---- Write operation ---- */
            if(PageStatus1==VALID_PAGE)
                return (PageStatus0==RECEIVE_DATA) ? PAGE0 : PAGE1; /* Page0 receiving data */
            else if(PageStatus0==VALID_PAGE)
                return (PageStatus0==RECEIVE_DATA) ? PAGE1 : PAGE0; /* Page1 receiving data */
            else
                throw std::exception(); /* No valid Page */

        case READ_FROM_VALID_PAGE: /* ---- Read operation ---- */
            if(PageStatus0==VALID_PAGE)
                return PAGE0; /* Page0 valid */
            else if(PageStatus1==VALID_PAGE)
                return PAGE1; /* Page1 valid */
            else
                throw std::exception(); /* No valid Page */
        default:
            throw std::exception();     //wrong operation
    }
}

/**
  * @brief  Verify if active page is full and Writes variable in EEPROM.
  * @param  VirtAddress: 16 bit virtual address of the variable
  * @param  Data: 16 bit data to be written as variable value
  * @retval Success or error status:
  *           - FLASH_OPER_ERR: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
void EE_VerifyPageFullWriteVariable(const uint16_t VirtAddress, const uint16_t Data)
{
  uint16_t ValidPage = PAGE0;
  uint32_t Address = 0x08010000, PageEndAddress = 0x080107FF;

  /* Get valid Page for write operation */
  ValidPage = EE_FindValidPage(WRITE_IN_VALID_PAGE);
  Address = (uint32_t)(EEPROM_START_ADDRESS + (uint32_t)(ValidPage * PAGE_SIZE));   /* Get the valid Page start Address */
  PageEndAddress = (uint32_t)((EEPROM_START_ADDRESS - 2) + (uint32_t)((1 + ValidPage) * PAGE_SIZE));    /* Get the valid Page end Address */

  /* Check each active page address starting from begining */
  while (Address < PageEndAddress)
  {
    if ((*(__IO uint32_t*)Address) == FLASH_EMPTY_VALUE) /* Verify if Address and Address+2 contents are 0xFFFF */
    {
        FLASH_Program_HalfWord(Address, Data);              /* Set variable data */
        FLASH_Program_HalfWord(Address + 2, VirtAddress); /* Set variable virtual address */
        return;
    }
    else
      Address = Address + 4;    /* Next address location */
  }
  throw std::range_error("EE_VerifyPageFullWriteVariable:PAGE FULL");   // PAGE_FULL; /* Return PAGE_FULL in case the valid page is full */
}

/**
  * @brief  Transfers last updated variables data from the full Page to
  *   an empty one.
  * @param  VirtAddress: 16 bit virtual address of the variable
  * @param  Data: 16 bit data to be written as variable value
  * @retval Success or error status:
  *           - FLASH_OPER_ERR: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
FlashStatusTypeDef EE_PageTransfer(uint16_t VirtAddress, uint16_t Data)
{
  uint32_t NewPageAddress = 0;
  uint32_t  OldPageAddress = 0;
  uint16_t ValidPage = PAGE0, VarIdx = 0;

  /* Get active Page for read operation */
  ValidPage = EE_FindValidPage(READ_FROM_VALID_PAGE);

  if (ValidPage == PAGE1)       /* Page1 valid */
  {
    NewPageAddress = PAGE0_BASE_ADDRESS;
    OldPageAddress = PAGE1_BASE_ADDRESS;
  }
  else if (ValidPage == PAGE0)  /* Page0 valid */
  {
    NewPageAddress = PAGE1_BASE_ADDRESS;
    OldPageAddress = PAGE0_BASE_ADDRESS;
  }
  else
    return FLASH_OPER_NO_VALID_PAGE;       /* No valid Page */

  FLASH_Program_HalfWord(NewPageAddress, RECEIVE_DATA); /* Set the new Page status to RECEIVE_DATA status */
  EE_VerifyPageFullWriteVariable(VirtAddress, Data);    /* Write the variable passed as parameter in the new active page */
  /* Transfer process: transfer variables from old to the new active page */
  for (VarIdx = 0; VarIdx < NumbOfVar; VarIdx++)
    if (VarIdx != VirtAddress)  /* Check each variable except the one passed as parameter */
      if( FLASH_OPER_ERR == EE_ReadVariable(VarIdx, &DataVar))   /* Read the other last variable updates */
        EE_VerifyPageFullWriteVariable(VarIdx, DataVar); /* Transfer the variable to the new active page */

  FLASH_ErasePage(OldPageAddress); /* Erase the old Page: Set old Page status to ERASED status */
  FLASH_Program_HalfWord(NewPageAddress, VALID_PAGE);   /* Set new Page status to VALID_PAGE status */
  return FLASH_OPER_ERR;
}

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
