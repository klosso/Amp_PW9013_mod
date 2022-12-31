/**
  ******************************************************************************
  * @file    EEPROM_Emulation/inc/eeprom.h 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   This file contains all the functions prototypes for the EEPROM 
  *          emulation firmware library.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#pragma once
#include<stdint.h>

/* Exported constants --------------------------------------------------------*/
/* Define the STM32F10Xxx Flash page size depending on the used STM32 device */
#if defined (STM32F10X_LD) || defined (STM32F10X_MD)
  #define PAGE_SIZE  (uint16_t)0x400  /* Page size = 1KByte */
#elif defined (STM32F10X_HD) || defined (STM32F10X_CL)
  #define PAGE_SIZE  (uint16_t)0x800  /* Page size = 2KByte */
#elif defined (STM32F103xB)
  #define PAGE_SIZE  (uint16_t)0x800  /* Page size = 2KByte */
#endif
#define ADDR_FLASH_PAGE_0     ((uint32_t)0x08000000) /* Base @ of Page 126, 1 Kbytes */
#define ADDR_FLASH_PAGE_124   ((uint32_t)0x0801F000) /* Base @ of Page 126, 1 Kbytes */
#define ADDR_FLASH_PAGE_125   ((uint32_t)0x0801F400) /* Base @ of Page 127, 1 Kbytes */
#define ADDR_FLASH_PAGE_126   ((uint32_t)0x0801F800) /* Base @ of Page 126, 1 Kbytes */
#define ADDR_FLASH_PAGE_127   ((uint32_t)0x0801FC00) /* Base @ of Page 127, 1 Kbytes */



/* Used Flash pages for EEPROM emulation */
#define MAX_NUMB_OF_VAR               33
#define PAGE_FULL               ((uint8_t)0x80)
#define PAGE0                   ((uint16_t)0x0000)
#define PAGE1                   ((uint16_t)0x0001)

//#ifdef __cplusplus
//extern "C" {
//#endif


enum FlashStatusTypeDef { FLASH_OPER_OK =0 ,FLASH_OPER_ERR, FLASH_OPER_NO_VALID_PAGE = 0x00AB};

/* Page status definitions */
enum FlashStateDefT {
        ERASED = 0xFFFF,     /* PAGE is empty */
        RECEIVE_DATA = 0xEEEE,     /* PAGE is marked to receive data */
        VALID_PAGE = 0x0000     /* PAGE containing valid data */
        };
/* Valid pages in read and write defines */
enum operationValidPageT {READ_FROM_VALID_PAGE =0, WRITE_IN_VALID_PAGE};

/* Page full define */



class FlashHandlers
{
public:
    FlashHandlers(const uint32_t page0, const uint32_t page1 );
    void programHalfWord(uint32_t Address, uint16_t Data);
    void erasePage(const uint32_t address);
    void format(void);
private:
    uint32_t PAGE0_BASE_ADDRESS;
    uint32_t PAGE1_BASE_ADDRESS;
};


class Eeprom{
public:
    Eeprom(const uint8_t);
    FlashStatusTypeDef init(void);
    FlashStatusTypeDef readVariable(uint16_t VirtAddress, uint16_t* Data);
    FlashStatusTypeDef writeVariable(uint16_t VirtAddress, uint16_t Data);
private:
    uint16_t findValidPage(operationValidPageT Operation);
    void verifyPageFullWriteVariable(uint16_t VirtAddress, uint16_t Data);
    FlashStatusTypeDef pageTransfer(uint16_t VirtAddress, uint16_t Data);
    void transferPage(const uint32_t page_addrFrom, const uint32_t page_addrTo);


    /* Pages 0 and 1 base and end addresses */
    const uint32_t EEPROM_START_ADDRESS = (ADDR_FLASH_PAGE_124); /* EEPROM emulation start address:
                                                  after 64KByte of used Flash memory */
    const uint32_t FlashEmptyValue = 0xFFFFFFFF;
    const uint32_t PAGE0_BASE_ADDRESS = EEPROM_START_ADDRESS;
    const uint32_t PAGE0_END_ADDRESS = EEPROM_START_ADDRESS + (PAGE_SIZE - 1);
    const uint32_t PAGE1_BASE_ADDRESS = (EEPROM_START_ADDRESS + PAGE_SIZE);
    const uint32_t PAGE1_END_ADDRESS = (EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1));
    FlashHandlers flash;
    uint16_t DataVar;
    uint8_t NumbOfVar;
};

//#ifdef __cplusplus
//}
//#endif


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
