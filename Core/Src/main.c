/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body - LED Blinking + UART + CMSIS-RTOS v2
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "cmsis_os.h"
#include "master_task.h"

/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;     // <-- Added UART handle
uint8_t txBuffer[7];
uint8_t rxBuffer[7];

/* RTOS thread handles */
osThreadId_t defaultTaskHandle;
osThreadId_t Task1Handle;
osThreadId_t Task2Handle;

/* Function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);   // <-- Added
void StartDefaultTask(void *argument);
void StartTask1(void *argument);
void StartTask2(void *argument);

/* MAIN FUNCTION */
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  Protocol_Init();
  MX_GPIO_Init();
  MX_USART1_UART_Init();   // <-- Added

  Protocol_GetNextTxFrame(txBuffer);
  //HAL_Delay(100);  // Short delay to ensure everything is initialized


  Protocol_Init2();
  Protocol_GetNextTxFrame(txBuffer);
   //HAL_Delay(10);



  /* Test UART message before RTOS starts */
  //char msg[] = "STM32L412 UART2 via ST-LINK VCP OK!\r\n";

  /* RTOS Init */


  osKernelInitialize();

  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, NULL);
  Task1Handle = osThreadNew(StartTask1, NULL, NULL);
  Task2Handle = osThreadNew(StartTask2, NULL, NULL);


  osKernelStart();

  while (1)
  {
  }
}

/* CLOCK CONFIG -------------------------------------------------------------*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

/* GPIO INIT ---------------------------------------------------------------*/
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* PA5 LED */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART2 PA2 (TX), PA3 (RX) */
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USART2 INIT -------------------------------------------------------------*/
static void MX_USART1_UART_Init(void)
{
  __HAL_RCC_USART1_CLK_ENABLE();

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
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
}

/* DEFAULT TASK ------------------------------------------------------------*/
void StartDefaultTask(void *argument)
{

  for(;;)
  {
    
	  HAL_UART_Receive(&huart1, rxBuffer, FRAME_SIZE, HAL_MAX_DELAY);
    HAL_Delay(100);

    osDelay(10);
  }
}

/* TASK1 -------------------------------------------------------------------*/
void StartTask1(void *argument)
{
  for(;;)
  {
	 // HAL_UART_Transmit(&huart2, txBuffer, FRAME_SIZE, HAL_MAX_DELAY);
   // HAL_Delay(1000);
   HAL_UART_Transmit(&huart1, rxBuffer, FRAME_SIZE, HAL_MAX_DELAY);
    osDelay(1000);
	  /* Blink once before RTOS */


  }
}

/* TASK2 -------------------------------------------------------------------*/
void StartTask2(void *argument)
{
	  //char msg[] = "Task2 running...\r\n";
  for(;;)
  {
   // HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    HAL_GPIO_TogglePin(GPIOC, Open_Led_Pin);
    osDelay(1000);
  }
}

/* ERROR HANDLER -----------------------------------------------------------*/
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
