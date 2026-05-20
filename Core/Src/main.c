
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : UART Interrupt + CMSIS-RTOS v2 Queue Example
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "cmsis_os.h"
#include "string.h"
#include "master_task.h"

/* UART + Buffers ------------------------------------------------------------*/
UART_HandleTypeDef huart2;

#define FRAME_SIZE    7
#define QUEUE_LENGTH  10   // can store 10 frames of 7 bytes each

/* RX buffer used ONLY by ISR */
/* RTOS Handles --------------------------------------------------------------*/
static uint8_t rxBuffer[FRAME_SIZE];
osThreadId_t uartTaskHandle;
osThreadId_t sendframeHandle;
osMessageQueueId_t uartRxQueue;

/* Function Prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartUartTask(void *argument);
void StartSendFrameTask(void *argument);

void Error_Handler(void);

/* MAIN ----------------------------------------------------------------------*/
int main(void)
{
    /* HAL Init */
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    /* Protocol init (your existing code) */
    Protocol_Init();

    /* Create RX queue: 10 frames, each 7 bytes (CMSIS-RTOS v2) */
    uartRxQueue = osMessageQueueNew(QUEUE_LENGTH, FRAME_SIZE, NULL);
    if (uartRxQueue == NULL)
    {
        Error_Handler();
    }

    /* Start UART interrupt reception into rxBuffer */
    HAL_UART_Receive_IT(&huart2, rxBuffer, FRAME_SIZE);

    /* Initialize CMSIS-RTOS kernel */
    osKernelInitialize();

    /* Create UART task */
    const osThreadAttr_t uartTaskAttr = {
        .name = "uartTask",
        .priority = osPriorityNormal,
        .stack_size = 1024
    };

    /* Create SendFrame task */
    const osThreadAttr_t sendframeAttr = {
        .name = "sendframeTask",
        .priority = osPriorityBelowNormal,  // Lower priority than uartTask
        .stack_size = 1024
    };

    uartTaskHandle = osThreadNew(StartUartTask, NULL, &uartTaskAttr);
    sendframeHandle = osThreadNew(StartSendFrameTask, NULL, &sendframeAttr);

    if (uartTaskHandle == NULL || sendframeHandle == NULL)
    {
        Error_Handler();
    }

    /* Start scheduler (does not return under normal operation) */
    osKernelStart();

    /* Should never get here */
    while (1) {}
}

/* UART RX COMPLETE CALLBACK -------------------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)

{



    if (huart->Instance == USART2)
    {
        /* Push a COPY of the received frame into the queue (from ISR) */
        osMessageQueuePut(uartRxQueue, rxBuffer, 0, 0);

        /* Re-arm UART for next frame */
        HAL_UART_Receive_IT(&huart2, rxBuffer, FRAME_SIZE);
    }
}

/* UART TASK -----------------------------------------------------------------*/
void StartUartTask(void *argument)
{
    uint8_t frame[FRAME_SIZE];
    uint8_t txBuffer[FRAME_SIZE];

    char startMsg[] = "[UART TASK] Started\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)startMsg, strlen(startMsg), 100);

    for (;;)
    {
        /* Wait for next frame from queue (FIFO, blocking) */
        if (osMessageQueueGet(uartRxQueue, frame, NULL, osWaitForever) == osOK)
        {
            /* Process received frame (safe copy) */
            Protocol_HandleRxFrame(frame, FRAME_SIZE);

            char dbg[] = "[UART TASK] Frame received and processed\r\n";
            HAL_UART_Transmit(&huart2, (uint8_t*)dbg, strlen(dbg), 10);

            /* Prepare next TX frame (if needed) */
            // Protocol_GetNextTxFrame(txBuffer);

            /* Transmit response (if needed) */
            // HAL_UART_Transmit(&huart2, txBuffer, FRAME_SIZE, 10);
        }

        else
        {
            /* Timeout occurred - queue empty for 1 second */
            char err[] = "[UART TASK] Queue timeout!\r\n";
            HAL_UART_Transmit(&huart2, (uint8_t*)err, strlen(err), 10);
        }
    }
}

/* SEND FRAME TASK -----------------------------------------------------------*/
void StartSendFrameTask(void *argument)
{
    uint8_t txBuffer[FRAME_SIZE];

    char dbgStart[] = "[TX TASK] Started periodic TX every 2s\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)dbgStart, strlen(dbgStart), 10);

    for (;;)
    {
        /* Protocol_GetNextTxFrame decides what to send based on FSM state */
        Protocol_GetNextTxFrame(txBuffer);
        HAL_UART_Transmit(&huart2, rxBuffer, FRAME_SIZE, 10);

        char dbg[] = "[TX TASK] TX frame sent\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t*)dbg, strlen(dbg), 10);

        osDelay(pdMS_TO_TICKS(2000));  // CMSIS-RTOS v2 delay
    }
}

/* USART2 IRQ HANDLER --------------------------------------------------------*/
void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart2);
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
    {
        Error_Handler();
    }

    /* NVIC config for USART2 - Ensure priority is within FreeRTOS limits */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
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
