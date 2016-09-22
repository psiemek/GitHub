/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>	/*rtt*/
#include <stdlib.h>	/*rtt*/
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "lwip.h"

/* USER CODE BEGIN Includes */
#include "freemodbus.h"
#include "shell.h"

#include "ftp_server.h"
#include "http_server.h"
#include "cfs-coffee-arch.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim3;

osThreadId defaultTaskHandle;
osThreadId PWMTaskHandle;
osThreadId LEDTaskHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern volatile unsigned short  usRegHoldingBuf[REG_HOLDING_NREGS];
extern volatile uint8_t modbus_conn_status;
struct _settings
{
	uint8_t ip[4];
	uint8_t netmask[4];
	uint8_t gateway[4];
	uint8_t macaddress[6];
	uint16_t modbusTCPport;
	uint8_t modbusaddress;
	uint8_t checksum;
}settings;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_RTC_Init(void);
void StartDefaultTask(void const * argument);
void PWMTaskBody(void const * argument);
void LEDTaskBody(void const * argument);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
extern void initialise_monitor_handles(void);	/*rtt*/
void WriteSettings(void);
void RestoreSettings(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	//initialise_monitor_handles();	/*rtt*/
	//printf("System initialize. \n");	/*rtt Dodac -specs=rdimon.specs -lc -lrdimon */
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_RTC_Init();

  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32 *)(&usRegHoldingBuf[6]), 7);	//Start pomiarów ADC

  cfs_coffee_arch_init();
  //coffee_file_test();
  RestoreSettings();

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of PWMTask */
  osThreadDef(PWMTask, PWMTaskBody, osPriorityLow, 0, 128);
  //PWMTaskHandle = osThreadCreate(osThread(PWMTask), NULL);

  /* definition and creation of LEDTask */
  osThreadDef(LEDTask, LEDTaskBody, osPriorityLow, 0, 128);
  //LEDTaskHandle = osThreadCreate(osThread(LEDTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */


  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV5;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_PLL2;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL2_ON;
  RCC_OscInitStruct.PLL2.PLL2MUL = RCC_PLL2_MUL8;
  RCC_OscInitStruct.PLL2.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV5;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.PLLI2S.PLLI2SMUL = RCC_PLLI2S_MUL10;
  PeriphClkInit.PLLI2S.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV5;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLL3CLK, RCC_MCODIV_1);

  HAL_RCC_EnableCSS();

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  __HAL_RCC_HSE_PREDIV2_CONFIG(RCC_HSE_PREDIV2_DIV5);

  __HAL_RCC_PLLI2S_CONFIG(RCC_PLLI2S_MUL10);

  __HAL_RCC_PLLI2S_ENABLE();

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 7;
  HAL_ADC_Init(&hadc1);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 2;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = 3;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_15;
  sConfig.Rank = 4;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 6;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = 7;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

}

/* RTC init function */
void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef DateToUpdate;

    /**Initialize RTC and set the Time and Date 
    */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  HAL_RTC_Init(&hrtc);

  sTime.Hours = 0x12;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;

  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 0x1;
  DateToUpdate.Year = 0x16;

  HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD);

}

/* TIM3 init function */
void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 21;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 64000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim3);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

  HAL_TIM_PWM_Init(&htim3);

  HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_SINGLE);

  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_TRIGGER;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_FALLING;
  sSlaveConfig.TriggerFilter = 0;
  HAL_TIM_SlaveConfigSynchronization(&htim3, &sSlaveConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 64001;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);

  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);

  HAL_TIM_MspPostInit(&htim3);

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
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PA8   ------> RCC_MCO
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, FLASH_WP_Pin|FLASH_RE_Pin|FLASH_WE_Pin|FLASH_CE_Pin 
                          |LED_230V_Pin|LED_24V_Pin|LED_RUN_Pin|LED_CH1_Pin 
                          |LED_CH2_Pin|LED_MODBUS_Pin|FLASH_ALE_Pin|FLASH_CLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : FLASH_WP_Pin FLASH_RE_Pin FLASH_WE_Pin FLASH_CE_Pin 
                           FLASH_ALE_Pin FLASH_CLE_Pin */
  GPIO_InitStruct.Pin = FLASH_WP_Pin|FLASH_RE_Pin|FLASH_WE_Pin|FLASH_CE_Pin 
                          |FLASH_ALE_Pin|FLASH_CLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : FLASH_R_B_Pin */
  GPIO_InitStruct.Pin = FLASH_R_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(FLASH_R_B_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_230V_Pin LED_24V_Pin LED_RUN_Pin LED_CH1_Pin 
                           LED_CH2_Pin LED_MODBUS_Pin */
  GPIO_InitStruct.Pin = LED_230V_Pin|LED_24V_Pin|LED_RUN_Pin|LED_CH1_Pin 
                          |LED_CH2_Pin|LED_MODBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : DEFAULT_SWITCH_Pin */
  GPIO_InitStruct.Pin = DEFAULT_SWITCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DEFAULT_SWITCH_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : FLASH_IO0_Pin FLASH_IO1_Pin FLASH_IO3_Pin FLASH_IO4_Pin 
                           FLASH_IO5_Pin FLASH_IO6_Pin FLASH_IO7_Pin */
  GPIO_InitStruct.Pin = FLASH_IO0_Pin|FLASH_IO1_Pin|FLASH_IO3_Pin|FLASH_IO4_Pin 
                          |FLASH_IO5_Pin|FLASH_IO6_Pin|FLASH_IO7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : FLASH_IO2_Pin */
  GPIO_InitStruct.Pin = FLASH_IO2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(FLASH_IO2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void WriteSettings(void)
{
	uint8_t *pointer = &settings.ip[0];
	uint32_t *pointer2 = &settings.ip[0];
	uint8_t i;
	settings.checksum = 0;
	for (i=0; i<(sizeof(settings)-1);i++)
	{
		settings.checksum += *pointer;
		pointer++;
	}
	pointer = settings.ip;
	HAL_FLASH_Unlock();
	//FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPERR);
	FLASH_PageErase(0x0803F800);
	FLASH->CR &= !(FLASH_CR_PER);
	for (i=0; i<(sizeof(settings)/4); i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x0803F800+i*4, *pointer2 );
		pointer2++;
	}
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x0803F800+i*4, *pointer2 );
	HAL_FLASH_Lock();
}

void RestoreDefaultSettings(void)
{
	uint8_t i;
	unsigned long uid[3];
	uid[0] = *((unsigned long *)0x1FFFF7E8);
	//uid[1] = *((unsigned long *)0x1FFFF7EC);
	//uid[2] = *((unsigned long *)0x1FFFF7F0);

	settings.ip[0] = 192;
	settings.ip[1] = 168;
	settings.ip[2] = 1;
	settings.ip[3] = 30;
	settings.netmask[0] = 255;
	settings.netmask[1] = 255;
	settings.netmask[2] = 255;
	settings.netmask[3] = 0;
	settings.gateway[0] = 192;
	settings.gateway[1] = 168;
	settings.gateway[2] = 1;
	settings.gateway[3] = 1;
	settings.macaddress[5] = (uint8_t)uid[0];
	settings.macaddress[4] = (uint8_t)(uid[0]>>8);
	settings.macaddress[3] = (uint8_t)(uid[0]>>16);
	settings.macaddress[2] = (uint8_t)(uid[0]>>24);
	settings.macaddress[1] = 0x80; //(uint8_t)(uid[1]);
	settings.macaddress[0] = 0x00; //(uint8_t)(uid[1]>>8);
	settings.modbusTCPport = 502;
	settings.modbusaddress = 0;
	settings.checksum = 0;
	WriteSettings();
}

void RestoreSettings(void)
{
	uint8_t i, check=0;
	uint8_t *pointer = &settings.ip[0];

	memcpy(settings.ip, 0x0803F800, sizeof(settings));
	for (i=0; i<(sizeof(settings)-1);i++)
	{
		check += *pointer;
		pointer++;
	}
	if (check != settings.checksum)	RestoreDefaultSettings();	//Wrong checksum back to default settings
}
/* USER CODE END 4 */

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();

  /* USER CODE BEGIN 5 */
  sys_thread_new( "FreeModbus", vMBServerTask, NULL, DEFAULT_THREAD_STACKSIZE, 5/*mainMB_TASK_PRIORITY*/ );
  //http_server_netconn_init();
  //shell_init();
  //FTP_Server_init();
  HTTP_Server_init();


  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/* PWMTaskBody function */
void PWMTaskBody(void const * argument)
{
  /* USER CODE BEGIN PWMTaskBody */

	htim3.Instance->CCER |= TIM_CCER_CC3E;
	HAL_TIM_OnePulse_Start(&htim3, TIM_CHANNEL_2 );	//Za³¹czenie wyjsc wibracona


  /* Infinite loop */
  for(;;)
  {
	switch (usRegHoldingBuf[0]) //Kanal 1
	{
		case 0x00:	//Wyjscie wylaczone
			htim3.Instance->CCR2 = 64001;
		break;
		case 0x01:	//Wyjscie sterowane Modbus
			if (usRegHoldingBuf[1] == 0) // Praca na dwóch po³ówkach sinusoidy
			{
				if (usRegHoldingBuf[2] > 64001) usRegHoldingBuf[2] = 64001;
				htim3.Instance->CCR2 = 64001 - usRegHoldingBuf[2];
			}
			else 	// Praca jednopolowkowo
			{
				if (usRegHoldingBuf[2] > 64001) usRegHoldingBuf[2] = 64001;
				htim3.Instance->CCR2 = 64001 - usRegHoldingBuf[2]/2;
			}
		break;
		case 0x04:	//Wyjscie sterowane potencjometrem
			htim3.Instance->CCR2 = 64001 - (usRegHoldingBuf[6]*16-1600);
		break;
		case 0x05:	//Wyjscie sterowane wejœciem napiêciowym
			htim3.Instance->CCR2 = 64001 - (usRegHoldingBuf[8]*16-1600);
		break;
		case 0x06:	//Wyjscie sterowane wejœciem pradowym
			htim3.Instance->CCR2 = 64001 - (usRegHoldingBuf[10]*16-1600);
		break;
		default:
			htim3.Instance->CCR2 = 64001;
		break;

	}

	switch (usRegHoldingBuf[3]) //Kanal 2
	{
		case 0x00:	//Wyjscie wylaczone
			htim3.Instance->CCR3 = 64001;
		break;
		case 0x01:	//Wyjscie sterowane Modbus
			if (usRegHoldingBuf[4] == 0) // Praca na dwóch po³ówkach sinusoidy
			{
				if (usRegHoldingBuf[5] > 64001) usRegHoldingBuf[5] = 64001;
				htim3.Instance->CCR3 = 64001 - usRegHoldingBuf[5];
			}
			else 	// Praca jednopolowkowo
			{
				if (usRegHoldingBuf[2] > 64001) usRegHoldingBuf[5] = 64001;
				htim3.Instance->CCR3 = 64001 - usRegHoldingBuf[5]/2;
			}
		break;
		case 0x04:	//Wyjscie sterowane potencjometrem
			htim3.Instance->CCR3 = 64001 - (usRegHoldingBuf[7]*16-1600);
		break;
		case 0x05:	//Wyjscie sterowane wejœciem napiêciowym
			htim3.Instance->CCR3 = 64001 - (usRegHoldingBuf[9]*16-1600);
		break;
		case 0x06:	//Wyjscie sterowane wejœciem pradowym
			htim3.Instance->CCR2 = 64001 - (usRegHoldingBuf[11]*16-1600);
		break;
		default:
			htim3.Instance->CCR3 = 64001;
		break;

	}

    osDelay(1);
  }
  /* USER CODE END PWMTaskBody */
}

/* LEDTaskBody function */
void LEDTaskBody(void const * argument)
{
  /* USER CODE BEGIN LEDTaskBody */
	HAL_GPIO_WritePin(LED_230V_GPIO_Port, LED_230V_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_24V_GPIO_Port, LED_24V_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RUN_GPIO_Port, LED_RUN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_CH1_GPIO_Port, LED_CH1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_CH2_GPIO_Port, LED_CH2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_MODBUS_GPIO_Port, LED_MODBUS_Pin, GPIO_PIN_SET);

  /* Infinite loop */
  for(;;)
  {
	  usRegHoldingBuf[13] = (float)(((1.43-(usRegHoldingBuf[12]*3.3/4096))/4.3)+25);
	  if ((htim3.Instance->SR & TIM_SR_CC1IF) != 0)		// Dioda 230VAC
	  {
		  htim3.Instance->SR &= !TIM_SR_CC1IF;
		  HAL_GPIO_WritePin(LED_230V_GPIO_Port, LED_230V_Pin, GPIO_PIN_RESET);
	  }
	  else
		  HAL_GPIO_WritePin(LED_230V_GPIO_Port, LED_230V_Pin, GPIO_PIN_SET);

	  if((usRegHoldingBuf[0] != 0) || (usRegHoldingBuf[3] != 0))	//Dioda RUN
		  HAL_GPIO_TogglePin(LED_RUN_GPIO_Port, LED_RUN_Pin);
	  else
		  HAL_GPIO_WritePin(LED_RUN_GPIO_Port, LED_RUN_Pin, GPIO_PIN_SET);
	  if(usRegHoldingBuf[0] != 0)									// Dioda CH1
		  HAL_GPIO_TogglePin(LED_CH1_GPIO_Port, LED_CH1_Pin);
	  else
		  HAL_GPIO_WritePin(LED_CH1_GPIO_Port, LED_CH1_Pin, GPIO_PIN_SET);
	  if(usRegHoldingBuf[3] != 0)									// Dioda CH2
		  HAL_GPIO_TogglePin(LED_CH2_GPIO_Port, LED_CH2_Pin);
	  else
		  HAL_GPIO_WritePin(LED_CH2_GPIO_Port, LED_CH2_Pin, GPIO_PIN_SET);

	  if (modbus_conn_status != 0)									// Dioda Modbus
		  HAL_GPIO_WritePin(LED_MODBUS_GPIO_Port, LED_MODBUS_Pin, GPIO_PIN_RESET);
	  else
		  HAL_GPIO_WritePin(LED_MODBUS_GPIO_Port, LED_MODBUS_Pin, GPIO_PIN_SET);

	  if (HAL_GPIO_ReadPin(DEFAULT_SWITCH_GPIO_Port, DEFAULT_SWITCH_Pin) == 0)
	  {
		  HAL_GPIO_WritePin(LED_24V_GPIO_Port, LED_24V_Pin, GPIO_PIN_SET);
		  osDelay(3000);
		  HAL_GPIO_WritePin(LED_24V_GPIO_Port, LED_24V_Pin, GPIO_PIN_RESET);
		  if (HAL_GPIO_ReadPin(DEFAULT_SWITCH_GPIO_Port, DEFAULT_SWITCH_Pin) == 0)
		  {
			  //powrót do ustawien default
			  NVIC_SystemReset();
		  }
	  }
	  osDelay(500);
  }
  /* USER CODE END LEDTaskBody */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
