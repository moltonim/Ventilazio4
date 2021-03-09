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
#include "stm32f2xx_ll_dma.h"
#include "stm32f2xx_ll_rcc.h"
#include "stm32f2xx_ll_bus.h"
#include "stm32f2xx_ll_system.h"
#include "stm32f2xx_ll_exti.h"
#include "stm32f2xx_ll_cortex.h"
#include "stm32f2xx_ll_utils.h"
#include "stm32f2xx_ll_pwr.h"
#include "stm32f2xx_ll_spi.h"
#include "stm32f2xx_ll_tim.h"
#include "stm32f2xx_ll_usart.h"
#include "stm32f2xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "algorithm.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define COMBUF_DIM  270


extern char bttn[];
extern uint16_t    Player[6];

extern uint8_t CommBuf[COMBUF_DIM];
extern int rx_pointer;
extern int tx_pointer;
extern int tx_lenbuf, rx_lenbuf;
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
#define USER_Btn_Pin LL_GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define MCO_Pin LL_GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define RMII_MDC_Pin LL_GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define USER_BTN1_Pin LL_GPIO_PIN_0
#define USER_BTN1_GPIO_Port GPIOA
#define USER_BTN1_EXTI_IRQn EXTI0_IRQn
#define USER_BTN2_Pin LL_GPIO_PIN_1
#define USER_BTN2_GPIO_Port GPIOA
#define USER_BTN2_EXTI_IRQn EXTI1_IRQn
#define USER_BTN3_Pin LL_GPIO_PIN_2
#define USER_BTN3_GPIO_Port GPIOA
#define USER_BTN3_EXTI_IRQn EXTI2_IRQn
#define USER_BTN4_Pin LL_GPIO_PIN_3
#define USER_BTN4_GPIO_Port GPIOA
#define USER_BTN4_EXTI_IRQn EXTI3_IRQn
#define USER_BTN5_Pin LL_GPIO_PIN_4
#define USER_BTN5_GPIO_Port GPIOA
#define USER_BTN5_EXTI_IRQn EXTI4_IRQn
#define FLASH_CS_Pin LL_GPIO_PIN_4
#define FLASH_CS_GPIO_Port GPIOC
#define USER_BTN6_Pin LL_GPIO_PIN_5
#define USER_BTN6_GPIO_Port GPIOC
#define LD1_Pin LL_GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define TESTPOINT_1_Pin LL_GPIO_PIN_0
#define TESTPOINT_1_GPIO_Port GPIOG
#define TFT_RST_Pin LL_GPIO_PIN_10
#define TFT_RST_GPIO_Port GPIOB
#define TFT_DC_Pin LL_GPIO_PIN_11
#define TFT_DC_GPIO_Port GPIOB
#define TFT_CS_Pin LL_GPIO_PIN_12
#define TFT_CS_GPIO_Port GPIOB
#define STLK_RX_Pin LL_GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin LL_GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define TFT_LIGHT_Pin LL_GPIO_PIN_6
#define TFT_LIGHT_GPIO_Port GPIOC
#define RS485_DIR_Pin LL_GPIO_PIN_8
#define RS485_DIR_GPIO_Port GPIOA
#define USB_DM_Pin LL_GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin LL_GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin LL_GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin LL_GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define TOUCH_CS_Pin LL_GPIO_PIN_2
#define TOUCH_CS_GPIO_Port GPIOD
#define RMII_TX_EN_Pin LL_GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin LL_GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define SWO_Pin LL_GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define JUMP_1_Pin LL_GPIO_PIN_5
#define JUMP_1_GPIO_Port GPIOB
#define JUMP_2_Pin LL_GPIO_PIN_6
#define JUMP_2_GPIO_Port GPIOB
#define LD2_Pin LL_GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
#define TOUCH_IRQ_Pin LL_GPIO_PIN_8
#define TOUCH_IRQ_GPIO_Port GPIOB
#define BOARD_GLED_Pin LL_GPIO_PIN_9
#define BOARD_GLED_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
