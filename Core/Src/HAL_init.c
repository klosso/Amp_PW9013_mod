/*
 * HAL_init.c
 *
 *  Created on: Mar 13, 2022
 *      Author: smikulsk
 */

#include "HAL_init.h"
#include "usb_device.h"

void SystemClock_Config( void );
void MX_GPIO_Init( void );
void MX_ADC1_Init( void );
void MX_I2C2_Init( void );
void MX_SPI2_Init( void );
void MX_TIM2_Init( void );
void MX_TIM3_Init( void );
void MX_RTC_Init( void );
void MX_USART1_UART_Init( void );
void MX_DMA_Init(void);

ADC_HandleTypeDef hadc1;
I2C_HandleTypeDef hi2c2;
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi2;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_adc1;


void HAL_Init_Peryf()
{
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_I2C2_Init();
    MX_SPI2_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_RTC_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_USB_DEVICE_Init();
}

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_adc1);
    __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);

}
/**
 * @brief System Clock Configuration
 * @retval None
 */
/*void SystemClock_Config( void ){
    RCC_OscInitTypeDef RCC_OscInitStruct={ 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct={ 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit={ 0 };
    * Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.

    RCC_OscInitStruct.OscillatorType= RCC_OSCILLATORTYPE_HSE
            |RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState= RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue= RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.LSEState= RCC_LSE_ON;
    RCC_OscInitStruct.HSIState= RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState= RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource= RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL= RCC_PLL_MUL6;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct)!=HAL_OK)
    {
        Error_Handler();
    }
    * Initializes the CPU, AHB and APB buses clocks

    RCC_ClkInitStruct.ClockType= RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource= RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider= RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider= RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider= RCC_HCLK_DIV1;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!=HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection= RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC
            |RCC_PERIPHCLK_USB;
    PeriphClkInit.RTCClockSelection= RCC_RTCCLKSOURCE_LSE;
    PeriphClkInit.AdcClockSelection= RCC_ADCPCLK2_DIV6;
    PeriphClkInit.UsbClockSelection= RCC_USBCLKSOURCE_PLL;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit)!=HAL_OK)
    {
        Error_Handler();
    }
}*/



void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /* Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initializes the CPU, AHB and APB buses clocks */

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC
                              |RCC_PERIPHCLK_USB;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}
/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 5;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
void MX_I2C2_Init( void ){
    hi2c2.Instance= I2C2;
    hi2c2.Init.ClockSpeed=100000;
    hi2c2.Init.DutyCycle= I2C_DUTYCYCLE_2;
    hi2c2.Init.OwnAddress1=0;
    hi2c2.Init.AddressingMode= I2C_ADDRESSINGMODE_7BIT;
    hi2c2.Init.DualAddressMode= I2C_DUALADDRESS_DISABLE;
    hi2c2.Init.OwnAddress2=0;
    hi2c2.Init.GeneralCallMode= I2C_GENERALCALL_DISABLE;
    hi2c2.Init.NoStretchMode= I2C_NOSTRETCH_DISABLE;
    if(HAL_I2C_Init(&hi2c2)!=HAL_OK)
    {
        Error_Handler();
    }
}
/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
void MX_RTC_Init( void ){
    RTC_TimeTypeDef sTime={ 0 };
    RTC_DateTypeDef DateToUpdate={ 0 };
    /** Initialize RTC Only
     */
    hrtc.Instance= RTC;
    hrtc.Init.AsynchPrediv= RTC_AUTO_1_SECOND;
    hrtc.Init.OutPut= RTC_OUTPUTSOURCE_ALARM;
    if(HAL_RTC_Init(&hrtc)!=HAL_OK)
    {
        Error_Handler();
    }
    /** Initialize RTC and set the Time and Date
     */
    sTime.Hours=15;
    sTime.Minutes=0;
    sTime.Seconds=0;
    if(HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN)!=HAL_OK)
    {
        Error_Handler();
    }
    DateToUpdate.WeekDay= RTC_WEEKDAY_MONDAY;
    DateToUpdate.Month= RTC_MONTH_JANUARY;
    DateToUpdate.Date=1;
    DateToUpdate.Year=0;
    if(HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN)!=HAL_OK)
    {
        Error_Handler();
    }
}
/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
void MX_SPI2_Init( void ){
    /* SPI2 parameter configuration*/
    hspi2.Instance= SPI2;
    hspi2.Init.Mode= SPI_MODE_MASTER;
    hspi2.Init.Direction= SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize= SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity= SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase= SPI_PHASE_1EDGE;
    hspi2.Init.NSS= SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler= SPI_BAUDRATEPRESCALER_2;
    hspi2.Init.FirstBit= SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode= SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation= SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial=10;
    if(HAL_SPI_Init(&hspi2)!=HAL_OK)
    {
        Error_Handler();
    }
}
/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
void MX_TIM2_Init( void ){
    TIM_Encoder_InitTypeDef sConfig={ 0 };
    TIM_MasterConfigTypeDef sMasterConfig={ 0 };
    htim2.Instance= TIM2;
    htim2.Init.Prescaler=0;
    htim2.Init.CounterMode= TIM_COUNTERMODE_UP;
    htim2.Init.Period=65535;
    htim2.Init.ClockDivision= TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload= TIM_AUTORELOAD_PRELOAD_DISABLE;
    sConfig.EncoderMode= TIM_ENCODERMODE_TI1;
    sConfig.IC1Polarity= TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection= TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler= TIM_ICPSC_DIV2;
    sConfig.IC1Filter=10;
    sConfig.IC2Polarity= TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection= TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler= TIM_ICPSC_DIV2;
    sConfig.IC2Filter=10;
    if(HAL_TIM_Encoder_Init(&htim2, &sConfig)!=HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger= TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode= TIM_MASTERSLAVEMODE_DISABLE;
    if(HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)!=HAL_OK)
    {
        Error_Handler();
    }
}
/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
void MX_TIM3_Init( void ){
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};

    /* USER CODE BEGIN TIM3_Init 1 */

    /* USER CODE END TIM3_Init 1 */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 71;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 65535;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
      Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
      Error_Handler();
    }
    if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
    {
      Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
      Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
    {
      Error_Handler();
    }
}

/* static void MX_TIM3_Init(void)
{

   USER CODE BEGIN TIM3_Init 0

   USER CODE END TIM3_Init 0

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

   USER CODE BEGIN TIM3_Init 1

   USER CODE END TIM3_Init 1
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_TI1F_ED;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim3, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV8;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
   USER CODE BEGIN TIM3_Init 2

   USER CODE END TIM3_Init 2

}*/
/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
void MX_USART1_UART_Init( void ){
    huart1.Instance= USART1;
    huart1.Init.BaudRate=115200;
    huart1.Init.WordLength= UART_WORDLENGTH_8B;
    huart1.Init.StopBits= UART_STOPBITS_1;
    huart1.Init.Parity= UART_PARITY_NONE;
    huart1.Init.Mode= UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl= UART_HWCONTROL_NONE;
    huart1.Init.OverSampling= UART_OVERSAMPLING_16;
    if(HAL_UART_Init(&huart1)!=HAL_OK)
    {
        Error_Handler();
    }
}
/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
 void MX_GPIO_Init( void ){

    GPIO_InitTypeDef GPIO_InitStruct={ 0 };

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB,OLED_DC_Pin|DRV_R_Pin|DRV_L_Pin, GPIO_PIN_RESET);    /*Configure GPIO pin : ENC_BTN_Pin */
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, OLED_CS_Pin, GPIO_PIN_RESET);    /*Configure GPIO pin : ENC_BTN_Pin OLED_CS*/

    GPIO_InitStruct.Pin= ENC_BTN_Pin;
    GPIO_InitStruct.Mode= GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull= GPIO_NOPULL;
    HAL_GPIO_Init(ENC_BTN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin= OLED_CS_Pin;
    GPIO_InitStruct.Mode= GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull= GPIO_NOPULL;
    GPIO_InitStruct.Speed= GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_CS_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : OLED_DC_Pin SW_LOW_Pin SW_HIGH_Pin DRV_R_Pin DRV_L_Pin */
    GPIO_InitStruct.Pin= OLED_DC_Pin|DRV_R_Pin|DRV_L_Pin;
    GPIO_InitStruct.Mode= GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull= GPIO_NOPULL;
    GPIO_InitStruct.Speed= GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : SW_LOW_Pin SW_HIGH_Pin */
    GPIO_InitStruct.Pin = SW_LOW_Pin|SW_HIGH_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler( void ){
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while ( 1 )
    {
    }
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
