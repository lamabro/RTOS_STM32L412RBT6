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
UART_HandleTypeDef huart2;     // UART handle
#define RX_BUFFER_SIZE 7      // Changed from 6 to 7 for 7 bytes
#define TX_BUFFER_SIZE 7       // Changed from 6 to 7 for 7 bytes

uint8_t txBuffer[TX_BUFFER_SIZE];
uint8_t rxBuffer[RX_BUFFER_SIZE];

/* RTOS thread handles */
osThreadId_t defaultTaskHandle;
osThreadId_t Task1Handle;
osThreadId_t Task2Handle;

/* Function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);
void StartTask1(void *argument);
//void StartTask2(void *argument);

/* MAIN FUNCTION */
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  Protocol_Init();
  MX_GPIO_Init();
  MX_USART2_UART_Init();   // Initialize UART

  Protocol_GetNextTxFrame(txBuffer);
  //HAL_Delay(100);  // Short delay to ensure everything is initialized

 // Protocol_Init2();
  Protocol_GetNextTxFrame(txBuffer);




  /* RTOS Init */
  osKernelInitialize();

  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, NULL);
  Task1Handle = osThreadNew(StartTask1, NULL, NULL);
 // Task2Handle = osThreadNew(StartTask2, NULL, NULL);

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
  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* USART2 INIT -------------------------------------------------------------*/
static void MX_USART2_UART_Init(void)
{
  __HAL_RCC_USART2_CLK_ENABLE();

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* DEFAULT TASK - Receives UART data */
void StartDefaultTask(void *argument)
{
  for(;;)
  {
    // Clear the buffer before receiving
    memset(rxBuffer, 0, RX_BUFFER_SIZE);

    // Receive exactly 7 bytes with timeout
    if(HAL_UART_Receive(&huart2, rxBuffer, RX_BUFFER_SIZE, 7000) == HAL_OK)
    {
      // Optional: Echo received data for debugging
       HAL_UART_Transmit(&huart2, rxBuffer, RX_BUFFER_SIZE, 100);
       Protocol_HandleRxFrame(rxBuffer,  RX_BUFFER_SIZE);
       Protocol_GetNextTxFrame(txBuffer);
    }

    osDelay(10);  // Small delay to allow other tasks to run
  }
}

/* TASK1 - Transmits received data back */
void StartTask1(void *argument)
{
  for(;;)
  {
    // Check if there's valid data in rxBuffer (non-zero)
	  HAL_UART_Transmit(&huart2, txBuffer, RX_BUFFER_SIZE, 100);
	  osDelay(1000);

      // Check every 100ms for new data
  }
}

/* TASK2 - LED Blinking Task */


/* ERROR HANDLER -----------------------------------------------------------*/
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
