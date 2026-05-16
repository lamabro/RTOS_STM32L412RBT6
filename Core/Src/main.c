/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : UART Interrupt + FreeRTOS Task Sync Example
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "cmsis_os.h"
#include "master_task.h"
#include "string.h"

/* UART + Buffers ------------------------------------------------------------*/
UART_HandleTypeDef huart2;

#define RX_BUFFER_SIZE 7
#define TX_BUFFER_SIZE 7

uint8_t rxBuffer[RX_BUFFER_SIZE];
uint8_t txBuffer[TX_BUFFER_SIZE];

/* RTOS Handles --------------------------------------------------------------*/
osThreadId_t uartTaskHandle;
osSemaphoreId_t uartRxSemaphore;

/* Function Prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartUartTask(void *argument);

/* MAIN ----------------------------------------------------------------------*/
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    Protocol_Init();
    Protocol_GetNextTxFrame(txBuffer);

    /* Create semaphore */
    const osSemaphoreAttr_t uartSemAttr = { .name = "uartRxSem" };
    uartRxSemaphore = osSemaphoreNew(1, 0, &uartSemAttr);

    /* Start UART interrupt reception */
    HAL_UART_Receive_IT(&huart2, rxBuffer, RX_BUFFER_SIZE);

    /* RTOS Init */
    osKernelInitialize();
    uartTaskHandle = osThreadNew(StartUartTask, NULL, NULL);
    osKernelStart();

    while (1) {}
}

/* UART RX COMPLETE CALLBACK -------------------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t msg[] = "From Slave";
    HAL_UART_Transmit(&huart2, msg, sizeof(msg)-1, 5);

        HAL_UART_Transmit(&huart2, rxBuffer, sizeof(rxBuffer)-1, 5);


    if (huart->Instance == USART2)
    {
        osSemaphoreRelease(uartRxSemaphore);   // Wake task
        HAL_UART_Receive_IT(&huart2, rxBuffer, RX_BUFFER_SIZE); // Re-arm
    }
}

/* UART TASK -----------------------------------------------------------------*/
void StartUartTask(void *argument)
{
    for(;;)
    {
        /* Wait until ISR signals data received */
        if (osSemaphoreAcquire(uartRxSemaphore, osWaitForever) == osOK)



        {

        	 uint8_t msg[] = "INSIDE TASK";
        	    HAL_UART_Transmit(&huart2, msg, sizeof(msg)-1, 5);

        	        HAL_UART_Transmit(&huart2, rxBuffer, sizeof(rxBuffer)-1, 5);


            /* Process received frame */
            Protocol_HandleRxFrame(rxBuffer, RX_BUFFER_SIZE);
            osDelay(3);

            /* Prepare next TX frame */
            Protocol_GetNextTxFrame(txBuffer);
            osDelay(3);

            /* Transmit response */
            HAL_UART_Transmit(&huart2, txBuffer, TX_BUFFER_SIZE, 100);
        }
    }
}

/* GPIO INIT -----------------------------------------------------------------*/
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* PA5 LED */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* USART2 INIT ---------------------------------------------------------------*/
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
        Error_Handler();
}

void USART2_IRQHandler(void)
{
    /* Base interrupt handling */
    HAL_UART_IRQHandler(&huart2);
}


/* CLOCK CONFIG --------------------------------------------------------------*/
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

/* ERROR HANDLER -------------------------------------------------------------*/
void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}
