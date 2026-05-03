/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define Push_Button_Pin GPIO_PIN_1
#define Push_Button_GPIO_Port GPIOC
#define KCE_RS485_TX_Pin GPIO_PIN_2
#define KCE_RS485_TX_GPIO_Port GPIOA
#define KCE_RS485_RX_Pin GPIO_PIN_3
#define KCE_RS485_RX_GPIO_Port GPIOA
#define PW_24V_Pin GPIO_PIN_0
#define PW_24V_GPIO_Port GPIOB
#define Close_End_Pin GPIO_PIN_6
#define Close_End_GPIO_Port GPIOC
#define Open_Led_Pin GPIO_PIN_7
#define Open_Led_GPIO_Port GPIOC
#define KCE_DE_Pin GPIO_PIN_11
#define KCE_DE_GPIO_Port GPIOA
#define RS232_TX_Pin GPIO_PIN_6
#define RS232_TX_GPIO_Port GPIOB
#define RS232_RX_Pin GPIO_PIN_7
#define RS232_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
