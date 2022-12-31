/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include <Eeprom_class_exc.hpp>
#include "main.h"
#include "HAL_init.h"
/* Private includes ----------------------------------------------------------*/
#include "u8g2/clib/u8g2.h"
#include <stdio.h>
#include <stdlib.h>
#include "Audio_256x64.h"
#include "BD37534F/bd37534f.hpp"
#include "RDA5807.hpp"
//#include "rds.h"
#include "./TEA6321/tea6321.hpp"
#include "ir.h"
#include "CFPushButton/CFPushButton.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


#define TSA_MIX

#ifdef DB37XX
#define VOL_MIN -80
#define VOL_MAX 15
#define BASS_MIN -20
#define BASS_MAX 20
#define TRBL_MIN -20
#define TRBL_MAX 20
#endif

#ifdef TSA_MIX
#define VOL_MIN -31
#define VOL_MAX 20
#define BASS_MIN -18
#define BASS_MAX 18
#define TRBL_MIN -12
#define TRBL_MAX 12
#endif

uint16_t currProgFm=0;


struct adc_t
{
    uint16_t bass;
    uint16_t trbl;
    uint16_t balc;
    uint16_t volume;
    uint16_t lm_in;
};

enum InputSrcT
{
    INPUT_CD=0, INPUT_DVD, INPUT_FM, INPUT_TV, INPUT_BT
};

volatile struct adc_t adcVal;
Eeprom eeprom(35);

/* Private function prototypes -----------------------------------------------*/
#ifdef __cplusplus
extern "C"{
#endif

    extern uint8_t u8x8_stm32_gpio_and_delay( u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr );
    extern uint8_t u8x8_byte_stm32_hw_spi( u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr );
    extern uint8_t u8x8_byte_stm32_hw_i2c( u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr );

    inline int get_enc_val(){
        return TIM2->CNT;
    }
    inline InputSrcT get_input_src(){
        return (InputSrcT) ((TIM2->CNT>>1)%5);
    }
    inline void set_input_src( InputSrcT src ){
        TIM2->CNT=src<<1;
    }

#ifdef __cplusplus
}
#endif
/* Private user code ---------------------------------------------------------*/
int prescaler( long min1, long max1, long min2, long max2, int val ){
    return min2+ (val-min1)* (max2-min2)/ (max1-min1);
}
#ifdef DB37XX
class MixerDRV: public Bd37534f
#elif defined TSA_MIX
class MixerDRV: public TEA6321
#endif
{
public:
    MixerDRV( void *drv ) :
#ifdef TSA_MIX
        TEA6321(drv){};
#elif defined DB37XX
        Bd37534f(drv){};
#endif

private:
    virtual int write_drv( const uint8_t mem_addr, const uint8_t val ){
        uint8_t data=val;
        HAL_StatusTypeDef ret=HAL_I2C_Mem_Write((I2C_HandleTypeDef*)drv, addr, mem_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 200);
        return ret;
    }
};

void HAL_TIM_IC_CaptureCallback( TIM_HandleTypeDef *htim ){
    if(htim==&htim3){
        switch ( HAL_TIM_GetActiveChannel(&htim3) )
        {
            case HAL_TIM_ACTIVE_CHANNEL_2:
                ir_tim_interrupt(htim);
            break;
            default:
            break;
        }
    }
}

void swHighPressed(){
    currProgFm++;
    eeprom.writeVariable(CURR_FM_PROG_ADR, currProgFm);
}
void swLowPressed(){
    currProgFm--;
    eeprom.writeVariable(CURR_FM_PROG_ADR, currProgFm);
}

void updateInputEeprom( Eeprom eeprom, uint16_t inpSrc, InputSrcT InputSrc ){
//    eeprom.readVariable(CURR_INP_PROG_ADR, &inpSrc);
//    if(inpSrc!=InputSrc)
        eeprom.writeVariable(CURR_INP_PROG_ADR, InputSrc);
}

/**
 * @brief  The application entry point.
 * @retval int
 */
int main( void ){
    /* MCU Configuration--------------------------------------------------------*/
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    RTC_TimeTypeDef sTime={ 0 };
//  RTC_DateTypeDef sDate = {0};
    char data[20];
    bool fm_avability=true;
    uint16_t programFm[32]={ 10020, 8980, 10320, 9950, 9584, 9533, 10645, 9406, 8822, 9897, 8945, 8814, 10586, 10135, 9647, 9237, 10708, 9077, 10422, 10328, 9286, 9946, 9832, 9599,
            8813, 10627, 9143, 9539, 8823, 10169, 10179, 8838 };


    int Volume=prescaler(0, 2800, VOL_MIN, VOL_MAX, adcVal.volume);
    InputSrcT InputSrc=INPUT_CD;


    HAL_Init_Peryf();
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adcVal, sizeof (adcVal)/sizeof(uint16_t));

    u8g2_t u8g2;
    u8g2_Setup_ssd1322_nhd_256x64_f(&u8g2, U8G2_R0, u8x8_byte_stm32_hw_spi, u8x8_stm32_gpio_and_delay);

    //check if POR
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST)){
        u8g2_InitDisplay(&u8g2);
        u8g2_ClearDisplay(&u8g2);
        u8g2_SetPowerSave(&u8g2, 0);
    }else
        u8g2_InitInterface(&u8g2);

    HAL_NVIC_EnableIRQ(TIM3_IRQn);
    ir_init(&htim3);

    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    htim2.Instance->CNT=0;

    MixerDRV mixer(&hi2c2);
    mixer.init();
    RDA5807 radio(&hi2c2);

    CFPushButton btnHigh(SW_HIGH_GPIO_Port, SW_HIGH_Pin, 10, 4000);  //Arduino way of defining pins
    CFPushButton btnLow(SW_LOW_GPIO_Port, SW_LOW_Pin, 10, 4000);



    if(radio.init()<0)
        fm_avability=false;

    btnHigh.setPulledUp();
    btnLow.setPulledUp();
    btnHigh.setOnShortPressReachedCallback(swHighPressed);
    btnLow.setOnShortPressReachedCallback(swLowPressed);

    HAL_FLASH_Unlock();
    /* EEPROM Init */

    if(FLASH_OPER_OK==eeprom.init()){
        uint16_t eeStatus;
        eeprom.readVariable( INIT_EE_PROG_ADR, &eeStatus);
        if(eeStatus!=INIT_EEPROM_VAL){
            int i=0;
            //EEPROM not initialized
            for( i=0; i<33; ++i ){
                eeprom.writeVariable(i, programFm[i]);
            }
            eeprom.writeVariable(CURR_INP_PROG_ADR, get_input_src());
            eeprom.writeVariable( INIT_EE_PROG_ADR, INIT_EEPROM_VAL);
        }else{
            for( int i=0; i<33; ++i )
                eeprom.readVariable(i, &programFm[i]);
            uint16_t tmp;
            eeprom.readVariable(CURR_INP_PROG_ADR, &tmp);
            if(tmp>INPUT_BT)
                tmp=0;
            set_input_src((InputSrcT)tmp);
            eeprom.readVariable(CURR_FM_PROG_ADR, &tmp);
            if(tmp>32)
                tmp=0;
            currProgFm=tmp;
        }
    }
    /* Infinite loop */
    while ( 1 ){
        u8g2_FirstPage(&u8g2);
        do{
            u8g2_ClearBuffer(&u8g2);
            u8g2_SetBitmapMode(&u8g2, 2);
            u8g2_SetFont(&u8g2, u8g2_font_VCR_OSD_tf);
            //u8g2_SetDrawColor(&u8g2, 1);

            //Draw picture if button pressed
            if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin)){
                u8g2_DrawXBM(&u8g2, 0, 0, Audio_256x64_width, Audio_256x64_height, (const uint8_t*)&Audio_256x64_bits);
            }

            if(int val=prescaler(995, 3100, VOL_MIN, VOL_MAX, adcVal.volume)!=Volume){
                Volume=val;
                mixer.set_vol(Volume);
            }


            if(get_input_src()!=InputSrc){
                mixer.set_input(get_input_src());
                InputSrc=get_input_src();
                updateInputEeprom(eeprom, 0, InputSrc);
            }
            Volume=prescaler(995, 3100, VOL_MIN, VOL_MAX, adcVal.volume);
            sprintf(data, "VOL=%+ddB", Volume);
            u8g2_DrawStr(&u8g2, 28, 64, data);
            int Bass = BASS_MAX- prescaler(300, 3800, BASS_MAX, BASS_MIN, adcVal.bass);
            sprintf(data, "BASS=%+ddB", Bass);
            u8g2_DrawStr(&u8g2, 0, 48, data);
            int Treble = TRBL_MAX - prescaler(327, 3780, TRBL_MAX, TRBL_MIN, adcVal.trbl);
            sprintf(data, "TRBL=%+ddB", Treble);
            u8g2_DrawStr(&u8g2, 128, 48, data);

//            sprintf(date, "ENC=%d", (int)TIM2->CNT);
//            u8g2_DrawStr(&u8g2, 0, 32, date);

            HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
            sprintf(data, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
            u8g2_DrawStr(&u8g2, 150, 64, data);

            int ir=ir_read();
            if(ir>=0){
                switch ( ir )
                {
                    case IR_CODE_VOL_UP:
                        HAL_GPIO_WritePin(DRV_R_GPIO_Port, DRV_R_Pin, GPIO_PIN_RESET);
                        HAL_GPIO_WritePin(DRV_L_GPIO_Port, DRV_L_Pin, GPIO_PIN_SET);
                        HAL_Delay(100);
                    break;
                    case IR_CODE_VOL_DW:
                        HAL_GPIO_WritePin(DRV_R_GPIO_Port, DRV_R_Pin, GPIO_PIN_SET);
                        HAL_GPIO_WritePin(DRV_L_GPIO_Port, DRV_L_Pin, GPIO_PIN_RESET);
                        HAL_Delay(100);
                    break;
                    default:
                        HAL_GPIO_WritePin(DRV_R_GPIO_Port, DRV_R_Pin, GPIO_PIN_RESET);
                        HAL_GPIO_WritePin(DRV_L_GPIO_Port, DRV_L_Pin, GPIO_PIN_RESET);
                    break;

                }
                HAL_GPIO_WritePin(DRV_R_GPIO_Port, DRV_R_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(DRV_L_GPIO_Port, DRV_L_Pin, GPIO_PIN_RESET);
                sprintf(data, "Ir=%x", ir);
                u8g2_DrawStr(&u8g2, 100, 32, data);
            }
            uint16_t freq;
            switch ( InputSrc )
            {
                case INPUT_CD:

                    u8g2_DrawStr(&u8g2, 0, 16, "CD");
                break;
                case INPUT_DVD:
                    u8g2_DrawStr(&u8g2, 0, 16, "DVD");
                break;
                case INPUT_TV:
                    u8g2_DrawStr(&u8g2, 0, 16, "TV");
                break;
                case INPUT_BT:
                    u8g2_SetFont(&u8g2, u8g2_font_open_iconic_embedded_2x_t);
                    u8g2_DrawGlyph(&u8g2, 0, 16, 'J');
                    u8g2_SetFont(&u8g2, u8g2_font_VCR_OSD_tf);
                    u8g2_DrawStr(&u8g2, 16, 16, "T PLAY");
                break;
                case INPUT_FM:
//			        u8g2_SetFont(&u8g2, u8g2_font_tenthinnerguys_tf);
                    if(!fm_avability){
                        u8g2_DrawStr(&u8g2, 0, 16, "FM err");
                        break;
                    }

                    u8g2_DrawStr(&u8g2, 0, 16, "FM");
                    radio.setFrequency(programFm[currProgFm%32]);
                    freq=radio.getFrequency();
                    sprintf(data, "Pr%02d: %3u.%02uMHz", currProgFm%32, freq/100, freq%100);
                    u8g2_DrawStr(&u8g2, 30, 16, data);
                    if(radio.isStereo())
                        u8g2_DrawStr(&u8g2, 240, 16, "S");
//                    if(radio.getRdsReady()){
//                        u8g2_DrawStr(&u8g2, 24, 16, radio.getRdsText());
//                        u8g2_DrawStr(&u8g2, 32, 16, radio.getRdsTime());
//                        u8g2_DrawStr(&u8g2, 38, 16, radio.getRdsText2B());
//
//                    }
                break;
                default:
                break;
            }
        }while ( u8g2_NextPage(&u8g2) );
        btnLow.loop();
//        HAL_GPIO_WritePin(DRV_L_GPIO_Port, DRV_L_Pin, (GPIO_PinState)!HAL_GPIO_ReadPin(SW_LOW_GPIO_Port, SW_LOW_Pin));
//        HAL_GPIO_WritePin(DRV_R_GPIO_Port, DRV_R_Pin, (GPIO_PinState)!HAL_GPIO_ReadPin(SW_HIGH_GPIO_Port, SW_HIGH_Pin));
		if ( GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW_HIGH_GPIO_Port, SW_HIGH_Pin))
		    swHighPressed();
        if ( GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW_LOW_GPIO_Port, SW_LOW_Pin))
            swLowPressed();
        btnHigh.loop();
        HAL_Delay(1);
    }
}
