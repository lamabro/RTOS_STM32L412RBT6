/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : UART Interrupt RX/TX with CMSIS-RTOS V2
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "cmsis_os.h"
#include "string.h"

/* UART Handles ------------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Buffer Definitions ------------------------------------------------------*/
#define RX_BUFFER_SIZE 7
#define TX_BUFFER_SIZE 7

uint8_t rxBuffer[RX_BUFFER_SIZE];
uint8_t txBuffer[TX_BUFFER_SIZE];

/* RTOS Handles ------------------------------------------------------------*/
osThreadId_t uartTaskHandle;
osMessageQueueId_t uartRxQueue;
osSemaphoreId_t uartTxSemaphore;

/* Function Prototypes -----------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartUartTask(void *argument);
void Error_Handler(void);

/* Interrupt Handlers ------------------------------------------------------*/
/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart2);
}

/* UART Callbacks ----------------------------------------------------------*/
/**
  * @brief Rx Transfer completed callback (called from ISR context).
  * @param huart UART handle
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        /* Copy received data to a temporary buffer */
        uint8_t tempBuffer[RX_BUFFER_SIZE];
        memcpy(tempBuffer, rxBuffer, RX_BUFFER_SIZE);
        
        /* Send received data to queue (from ISR context) */
        osMessageQueuePut(uartRxQueue, tempBuffer, 0, 0);
        
        /* Re-arm reception for next data */
        HAL_UART_Receive_IT(&huart2, rxBuffer, RX_BUFFER_SIZE);
    }
}

/**
  * @brief Tx Transfer completed callback.
  * @param huart UART handle
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        /* Signal that transmission is complete */
        osSemaphoreRelease(uartTxSemaphore);
    }
}

/**
  * @brief UART error callback.
  * @param huart UART handle
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        /* Clear error flags */
        __HAL_UART_CLEAR_OREFLAG(huart);
        __HAL_UART_CLEAR_NEFLAG(huart);
        __HAL_UART_CLEAR_PEFLAG(huart);
        __HAL_UART_CLEAR_FEFLAG(huart);
        
        /* Re-arm reception */
        HAL_UART_Receive_IT(&huart2, rxBuffer, RX_BUFFER_SIZE);
    }
}

/* RTOS Task ---------------------------------------------------------------*/
/**
  * @brief UART Task: Processes received data and echoes it back
  * @param argument Not used
  */
void StartUartTask(void *argument)
{
    uint8_t receivedData[RX_BUFFER_SIZE];
    osStatus_t status;
    
    /* Initial delay to ensure system is ready */
    osDelay(100);
    
    for (;;)
    {
        /* Wait for data from queue (block until data arrives) */
        status = osMessageQueueGet(uartRxQueue, receivedData, NULL, osWaitForever);
        
        if (status == osOK)
        {
            /* Data received - transmit it back */
            /* Use blocking transmit with semaphore for synchronization */
            if (HAL_UART_Transmit_IT(&huart2, receivedData, RX_BUFFER_SIZE) == HAL_OK)
            {
                /* Wait for transmission to complete (with timeout) */
                osSemaphoreAcquire(uartTxSemaphore, 100);
            }
            
            /* Optional: Blink LED to indicate activity */
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        }
        
        /* Allow other tasks to run */
        osDelay(1);
    }
}

/* MAIN FUNCTION -----------------------------------------------------------*/
int main(void)
{
    /* MCU Configuration */
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    
    /* Create RTOS objects before kernel start */
    
    /* Create message queue for UART RX data (holds up to 10 messages) */
    uartRxQueue = osMessageQueueNew(10, RX_BUFFER_SIZE, NULL);
    
    /* Create semaphore for TX complete synchronization */
    uartTxSemaphore = osSemaphoreNew(1, 0, NULL);
    
    /* Create UART task */
    const osThreadAttr_t uartTaskAttr = {
        .name = "UartTask",
        .stack_size = 256 * 4,
        .priority = osPriorityNormal
    };
    uartTaskHandle = osThreadNew(StartUartTask, NULL, &uartTaskAttr);
    
    /* NVIC Configuration for RTOS compatibility */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);  /* Priority must be < configMAX_SYSCALL_INTERRUPT_PRIORITY */
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    
    /* Start first UART interrupt reception */
    HAL_UART_Receive_IT(&huart2, rxBuffer, RX_BUFFER_SIZE);
    
    /* Start RTOS kernel */
    osKernelInitialize();
    osKernelStart();
    
    /* Should never reach here */
    while (1)
    {
    }
}

/* USART2 Initialization ---------------------------------------------------*/
static void MX_USART2_UART_Init(void)
{
    /* Enable USART2 clock */
    __HAL_RCC_USART2_CLK_ENABLE();
    
    /* Configure UART parameters */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    
    /* Initialize UART */
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* GPIO Initialization -----------------------------------------------------*/
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Enable GPIOA clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    /* Configure PA5 as LED output */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* Configure UART pins: PA2 (TX), PA3 (RX) */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;  /* Check your MCU's alternate function */
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* System Clock Configuration ----------------------------------------------*/
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    /* Configure oscillator */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    
    /* Configure clock dividers */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

/* Error Handler -----------------------------------------------------------*/
void Error_Handler(void)
{
    /* Disable all interrupts */
    __disable_irq();
    
    /* Infinite loop */
    while (1)
    {
        /* Toggle LED fast to indicate error */
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(100);
    }
}

/* Assert handler (for debugging) -----------------------------------------*/
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add their own implementation to report file name and line number */
    while (1) {}
}
#endif /* USE_FULL_ASSERT */