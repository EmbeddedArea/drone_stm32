/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "circular_buffers.h"
#define SERIAL_DEBUG 0
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void send_message_to_pc(const char * buffer );
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PC_TX_Pin GPIO_PIN_2
#define PC_TX_GPIO_Port GPIOA
#define PC_RX_Pin GPIO_PIN_3
#define PC_RX_GPIO_Port GPIOA
#define GPS_TX_Pin GPIO_PIN_10
#define GPS_TX_GPIO_Port GPIOB
#define GPS_RX_Pin GPIO_PIN_11
#define GPS_RX_GPIO_Port GPIOB
#define DEBUG_LED_Pin GPIO_PIN_13
#define DEBUG_LED_GPIO_Port GPIOA
#define LORA_AUX_Pin GPIO_PIN_3
#define LORA_AUX_GPIO_Port GPIOB
#define LORA_M1_Pin GPIO_PIN_4
#define LORA_M1_GPIO_Port GPIOB
#define LORA_M0_Pin GPIO_PIN_5
#define LORA_M0_GPIO_Port GPIOB
#define LORA_TX_Pin GPIO_PIN_6
#define LORA_TX_GPIO_Port GPIOB
#define LORA_RX_Pin GPIO_PIN_7
#define LORA_RX_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/**
 * Struct type that is sent to the queue
 */
typedef struct {
	//circular_buffers_t *circular_buffer_addr;	/** address of the related circular buffer */
	uint32_t start_index;						/** starting index of the circular buffer */
	uint32_t length;							/** message length */
} uart_data_t;

/**
 * Size of circular buffer
 */
#define PC_CIRCULAR_UART_BUFFER_SIZE (100)

/**
 * PC UART_HandleTypeDef
 */
#define HUART_PC				(huart2)
/**
 * PC DMA_HandleTypeDef
 */
#define HDMA_UART_PC			(hdma_usart2_rx)
/**
 * PC DMA Buffer length for receiver
 */
#define PC_RX_DMA_BUFFER_LEN	(100)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
