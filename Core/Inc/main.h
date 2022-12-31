/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENC_L_Pin GPIO_PIN_0
#define ENC_L_GPIO_Port GPIOA
#define ENC_R_Pin GPIO_PIN_1
#define ENC_R_GPIO_Port GPIOA
#define ENC_BTN_Pin GPIO_PIN_2
#define ENC_BTN_GPIO_Port GPIOA
#define BASS_Pin GPIO_PIN_3
#define BASS_GPIO_Port GPIOA
#define TRBL_Pin GPIO_PIN_4
#define TRBL_GPIO_Port GPIOA
#define BLNC_Pin GPIO_PIN_5
#define BLNC_GPIO_Port GPIOA
#define VOL_Pin GPIO_PIN_6
#define VOL_GPIO_Port GPIOA
#define LM_IN_Pin GPIO_PIN_1
#define LM_IN_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_14
#define OLED_DC_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_8
#define OLED_CS_GPIO_Port GPIOA
#define DRV_L_Pin GPIO_PIN_6
#define DRV_L_GPIO_Port GPIOB
#define DRV_R_Pin GPIO_PIN_7
#define DRV_R_GPIO_Port GPIOB
#define SW_HIGH_Pin GPIO_PIN_8
#define SW_HIGH_GPIO_Port GPIOB
#define SW_LOW_Pin GPIO_PIN_9
#define SW_LOW_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define INIT_EE_PROG_ADR 33
#define CURR_INP_PROG_ADR 34
#define CURR_FM_PROG_ADR 35

#define INIT_EEPROM_VAL 0x55

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
