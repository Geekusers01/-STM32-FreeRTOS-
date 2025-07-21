/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#define VL53LX1_SHUT_Pin GPIO_PIN_5
#define VL53LX1_SHUT_GPIO_Port GPIOA
#define LED_LEFT_TOP_Pin GPIO_PIN_1
#define LED_LEFT_TOP_GPIO_Port GPIOB
#define Si24R1_IRQ_Pin GPIO_PIN_2
#define Si24R1_IRQ_GPIO_Port GPIOB
#define Si24R1_CS_Pin GPIO_PIN_12
#define Si24R1_CS_GPIO_Port GPIOB
#define Si24R1_CE_Pin GPIO_PIN_8
#define Si24R1_CE_GPIO_Port GPIOA
#define LED_RIGHT_TOP_Pin GPIO_PIN_3
#define LED_RIGHT_TOP_GPIO_Port GPIOB
#define LED_RIGHT_BOTTOM_Pin GPIO_PIN_8
#define LED_RIGHT_BOTTOM_GPIO_Port GPIOB
#define LED_LEFT_BOTTOM_Pin GPIO_PIN_9
#define LED_LEFT_BOTTOM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
