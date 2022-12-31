#pragma once
/*
 * HAL_init.h
 *
 *  Created on: Mar 13, 2022
 *      Author: Sebastian Mikulski
 */

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C"{
#endif


extern  ADC_HandleTypeDef hadc1;
extern  I2C_HandleTypeDef hi2c2;
extern  RTC_HandleTypeDef hrtc;
extern  SPI_HandleTypeDef hspi2;
extern  TIM_HandleTypeDef htim2;
extern  TIM_HandleTypeDef htim3;
extern  UART_HandleTypeDef huart1;
extern  DMA_HandleTypeDef hdma_adc1;

void HAL_Init_Peryf( void );


#ifdef __cplusplus
}
#endif
