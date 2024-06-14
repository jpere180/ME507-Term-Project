/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Ultrasonic.h"
#include "DFPLAYER_MINI.h"
#include "motor_driver2.h"
#include "radio4brie.h"
#include "reflectance_array.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

//Ultrasonic Variables
#define TRIG_PIN GPIO_PIN_9
#define TRIG_PORT GPIOC
#define ECHO_PIN GPIO_PIN_8
#define ECHO_PORT GPIOA

Ultrasonic ultrasonic;

//Motor Variables
//Encoder_Driver Enc_Driver;

int pulse = 0;
int CCW = 0;
int CCW1 = 0;
int BREAK = 0;
int CW = 0;
int CW1 = 0;
int CCW_HM = 0;
int CW_HM = 0;
int CCW_DL = 0;
int CW_DL = 0;
int CW_AR = 0;
int CCW_AR = 0;
int left_counts = 0;
int right_counts = 0;
uint32_t startTick = 0;
uint32_t finalTick = 0;
int trig_enable = 0;
int trig_disable = 0;
int total_cal=0;

// Flags
int FWD = 0;
int CRAB = 0;
int NE = 0;
int NW = 0;
int SE = 0;
int SW = 0;
int DONUT = 0;
int PIVOT = 0;
int cali=1;
int cal_end=0;
uint32_t state1 = 1;
uint32_t state2 = 0;
int state1st=1;
int state2nd=1;
int DNT_ST = 1;


//
uint32_t ADCValue=0;
float Volts = 0;
float dir=0;
//Line Follower Variables

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
radio radio1 = {.timer = &htim1, .channel = TIM_CHANNEL_2};	//initialize radio input capture struct
uint32_t trig = 1500;	// initialize to neutral level
// ----------[STATE 0: INIT]----------

  //GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5

  // Make Motor 1 Objects
  Motor_Driver MOT1 ={
		.htim = &htim3,
		.channel = TIM_CHANNEL_1,
		.gpio_port_en = GPIOC,
		.gpio_pin_en = GPIO_PIN_0};

// Make Motor 2 Objects
  Motor_Driver MOT2 = {
		.htim = &htim3,
		.channel = TIM_CHANNEL_2,
		.gpio_port_en = GPIOC,
		.gpio_pin_en = GPIO_PIN_1};

// Make Motor 3 Objects
  Motor_Driver MOT3 = {
		.htim = &htim3,
		.channel = TIM_CHANNEL_3,
		.gpio_port_en = GPIOC,
		.gpio_pin_en = GPIO_PIN_4};

 // Make Motor 4 Objects
  Motor_Driver MOT4 = {
		.htim = &htim3,
		.channel = TIM_CHANNEL_4,
		.gpio_port_en = GPIOC,
		.gpio_pin_en = GPIO_PIN_5};
  reflect reflect1 = {.portB = GPIOB,.portC = GPIOC,.pin1= GPIO_PIN_12,.pin2 = GPIO_PIN_13,.pin3 = GPIO_PIN_14,.pin4 = GPIO_PIN_15,.pin5 = GPIO_PIN_6,.pin6 = GPIO_PIN_7,.pin7 = GPIO_PIN_8,.pin8 = GPIO_PIN_11,.timer = &htim1};
int cali_track=0;


/*
  enable_motor(&MOT1);
  enable_motor(&MOT2);
  enable_motor(&MOT3);
  enable_motor(&MOT4);
*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  //MP3 and Ultrasonic Initialization
  DF_Init(10);
  Ultrasonic_Init(&htim1);
  HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);

  //Encoder Initialization
  int encoder_velocity;
  int encoder_position;
  int timer_counter;

  //Radio Init
  HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_2);	// starts pwm input capture in interrupt mode




 // Variables for motor direction
  BREAK = 2400;
  CCW = 3250;
  CCW1 = 3250;
  CW = 1550;
  CW1 = 1550;
  CCW_HM = CCW+1050;
  CW_HM = CW-1050;
  CCW_DL = CCW+900;
  CW_DL = CW-900;
  CCW_AR = CCW+600;
  CW_AR = CW-600;

/*
  set_duty(&MOT1, CCW);
  set_duty(&MOT2, CCW1);
  set_duty(&MOT3, CCW1);
  set_duty(&MOT4, CCW);
*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //ADC Set Up
	  HAL_ADC_Start(&hadc1);
	  if (HAL_ADC_PollForConversion(&hadc1,10)==  HAL_OK){
		  ADCValue = HAL_ADC_GetValue(&hadc1);
	  }
	  HAL_ADC_Stop(&hadc1);
	  Volts = ((float)ADCValue/0xFFF)*3.3*(402+110)/110;
	  if (Volts<10){
			disable_motor(&MOT1);
			disable_motor(&MOT2);
			disable_motor(&MOT3);
			disable_motor(&MOT4);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);

	  }
/*
	  //Crab Calibration
	  while (cal_end==0){
		  calibrate(&reflect1);
	  }
*/
	  //State 1 Code

	  if (state1 == 1){
		  FWD = 1;
		  if (state1st ==1){
			  state1st = 0;
			  startTick = HAL_GetTick();
		  }
		  	if (HAL_GetTick()-startTick >5000){
		  		state1 = 0;
		  		state2 = 1;
		  		state1st=1;
		  				  	}

	  }

	  //State 2 Code
	  if (state2 == 1){
	  	  FWD = -1;

	  if (state2nd ==1){
	  	  startTick = HAL_GetTick();
	  	  state2nd=0;
	  }
	  		if (HAL_GetTick()-startTick > 5000){
				state1 = 1;
				state2 = 0;
				state2nd=1;
			}}



	  //Ultrasonic set up
	  Ultrasonic_Trigger(&htim1, TRIG_PORT, TRIG_PIN);
	  Ultrasonic_Measure(&ultrasonic, &htim1, ECHO_PORT, ECHO_PIN);

	  if (ultrasonic.Distance >=20 && ultrasonic.Distance <=30)
	  {
	  	DF_PlayFromStart();
	  	DONUT = -1;
	  	DNT_ST = 1;
	  }

	  //HAL_Delay(50);

	  //if (ultrasonic.Distance > 20)
	  //{
	  //	FWD = 1;
	  //}

	  // ----------[STATE 1 - FORWARD/BACKWARD]----------
	  // Forward
	  if (FWD == 1){
		  set_duty(&MOT1, CW_AR);
		  set_duty(&MOT2, CW_HM);
		  set_duty(&MOT3, CCW1);
		  set_duty(&MOT4, CCW_DL);
	  }

	  // Backward
	  else if (FWD == -1){
		  set_duty(&MOT1, CCW_AR);
		  set_duty(&MOT2, CCW_HM);
		  set_duty(&MOT3, CW1);
		  set_duty(&MOT4, CW_DL);
	  }

	  // ----------[STATE 2 - CRAB]----------
	  // CRAB LEFT
	  if (CRAB == 1){
		  set_duty(&MOT1, CCW_AR);
		  set_duty(&MOT2, CW_HM);
		  set_duty(&MOT3, CCW1);
		  set_duty(&MOT4, CW_DL);
	  }
	  // CRAB RIGHT
	  else if (CRAB == -1){
		  set_duty(&MOT1, CW_AR);
		  set_duty(&MOT2, CCW_HM);
		  set_duty(&MOT3, CW1);
		  set_duty(&MOT4, CCW_DL);
	  }

	  // ----------[STATE 3 - DIAGONAL]----------
	  if (NE == 1){
		  disable_motor(&MOT1);
		  set_duty(&MOT2, CW_HM);
		  set_duty(&MOT3, CCW1);
		  disable_motor(&MOT4);
	  }

	  if (NW == 1){
		  set_duty(&MOT1, CW_AR);
		  disable_motor(&MOT2);
		  disable_motor(&MOT3);
		  set_duty(&MOT4, CCW_DL);
	  }

	  if (SE == 1){
		  set_duty(&MOT1, CCW_AR);
		  disable_motor(&MOT2);
		  disable_motor(&MOT3);
		  set_duty(&MOT4, CW_DL);
	  }

	  if (SW == 1){
		  disable_motor(&MOT1);
		  set_duty(&MOT2, CCW_HM);
		  set_duty(&MOT3, CW1);
		  disable_motor(&MOT4);
	  }

	  // ----------[STATE 4 - DONUT]----------
	  // Clock wise Donut
	  if (DONUT == 1){
		  set_duty(&MOT1, CCW_AR);
		  set_duty(&MOT2, CCW_HM);
		  set_duty(&MOT3, CCW1);
		  set_duty(&MOT4, CCW_DL);
		  if (DNT_ST == 1){
			  startTick = HAL_GetTick();
			  DNT_ST = 0;
		  }
		  if (HAL_GetTick()-startTick > 5000){
			  DONUT = 0;
		  }

	  }

	  // Counter clock wise Donut
	  else if (DONUT == -1){
		  set_duty(&MOT1, CW_AR);
		  set_duty(&MOT2, CW_HM);
		  set_duty(&MOT3, CW1);
		  set_duty(&MOT4, CW_DL);
		  if (DNT_ST == 1){
		  			  startTick = HAL_GetTick();
		  			  DNT_ST = 0;
		  		  }
		  		  if (HAL_GetTick()-startTick > 5000){
		  			  DONUT = 0;
		  		  }
	  }

	  // ----------[STATE 5 - PIVOT]----------
	  // RIGHT
	  if (PIVOT == 1){
		  disable_motor(&MOT1);
		  disable_motor(&MOT2);
		  set_duty(&MOT3, CCW1);
		  set_duty(&MOT4, CCW_DL);
	  }

	  // LEFT
	  else if (PIVOT == -1){
		  set_duty(&MOT1, CW_AR);
		  set_duty(&MOT2, CW_HM);
		  disable_motor(&MOT3);
		  disable_motor(&MOT4);
	  }
  }
  // ---- [closed loop control]------------
  //
 // dir = direction(&reflect1);
  //if (dir>4400 && dir<4600){
//	  FWD=1;
//	  NW=0;
	//  NE=0;
  //}else if (dir<4400 || dir>4600){
//	  FWD=0;
//	  if (4600<dir){
//		  NW = 1;
//	  }else if (4400>dir){
//		  NE=1;
//	  }
 // }
}
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 95;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 4799;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 2400;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_11, GPIO_PIN_SET);

  /*Configure GPIO pins : PC13 PC0 PC1 PC4
                           PC5 PC6 PC7 PC8
                           PC9 PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
trig = get(&radio1);	// stores value from radio receiver
/*
if  (trig<1100){
	cal_end = 1;
}
*/
if (trig>1750 && trig_enable==0){			// if trigger is mostly pulled do thing
	/*
	 * changes the code below to motor enables and disables depending on the trigger,
	 * it is set the current way for demo purposes only
	 */
	  enable_motor(&MOT1);
	  enable_motor(&MOT2);
	  enable_motor(&MOT3);
	  enable_motor(&MOT4);
	  trig_enable = 1;
	  trig_disable = 0;

}else if (trig<=1750 && trig_disable==0){
	disable_motor(&MOT1);
	disable_motor(&MOT2);
	disable_motor(&MOT3);
	disable_motor(&MOT4);
	trig_enable = 0;
	trig_disable = 1;
}

}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
