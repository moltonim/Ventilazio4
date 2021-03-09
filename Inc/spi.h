/**
  ******************************************************************************
  * File Name          : SPI.h
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __spi_H
#define __spi_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_SPI1_Init(void);
void MX_SPI2_Init(void);
void MX_SPI3_Init(void);

/* USER CODE BEGIN Prototypes */
//SPI1 -> FLASH
void SPI1_DMA_Read_FlashToRam(uint8_t* buf, uint32_t len);
void Prepare_SPI1_DMA(void);
void Configure_SPI1_DMA(uint32_t* buf, uint32_t len, char read);

//SPI2 -> TFT
void SPI_WR8(uint8_t data);
void SPI_WR16(uint16_t data);

void SPI2_DMA_WriteConst(const uint8_t* buf, uint32_t len);
void SPI2_DMA_Copy(const uint8_t* buf, uint32_t len);
void SPI2_DMA_CopyContinue(const uint8_t* buf, uint32_t len);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
