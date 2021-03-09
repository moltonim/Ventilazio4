/**
  ******************************************************************************
  * File Name          : SPI.c
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

/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */
volatile char SPI2_TransmitComplete;
volatile char SPI1_ReceiveComplete;
/* USER CODE END 0 */

/* SPI1 init function */
void MX_SPI1_Init(void)
{
  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**SPI1 GPIO Configuration
  PA5   ------> SPI1_SCK
  PA6   ------> SPI1_MISO
  PA7   ------> SPI1_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* SPI1 DMA Init */

  /* SPI1_RX Init */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_2, LL_DMA_CHANNEL_3);

  LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_2, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA2, LL_DMA_STREAM_2, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_2, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_2, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_2, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_2, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_DisableFifoMode(DMA2, LL_DMA_STREAM_2);

  /* SPI1_TX Init */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_3, LL_DMA_CHANNEL_3);

  LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_3, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA2, LL_DMA_STREAM_3, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_3, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_3, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_3, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_3, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_DisableFifoMode(DMA2, LL_DMA_STREAM_3);

  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);

}
/* SPI2 init function */
void MX_SPI2_Init(void)
{
  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**SPI2 GPIO Configuration
  PB13   ------> SPI2_SCK
  PB14   ------> SPI2_MISO
  PB15   ------> SPI2_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_14;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* SPI2 DMA Init */

  /* SPI2_TX Init */
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_4, LL_DMA_CHANNEL_0);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_4, LL_DMA_PRIORITY_MEDIUM);

  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_4, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_4, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_4, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_4, LL_DMA_PDATAALIGN_HALFWORD);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_4, LL_DMA_MDATAALIGN_HALFWORD);

  LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_4);

  SPI_InitStruct.TransferDirection = LL_SPI_HALF_DUPLEX_TX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI2, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI2, LL_SPI_PROTOCOL_MOTOROLA);

}
/* SPI3 init function */
void MX_SPI3_Init(void)
{
  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  /**SPI3 GPIO Configuration
  PC10   ------> SPI3_SCK
  PC11   ------> SPI3_MISO
  PC12   ------> SPI3_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI3, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI3, LL_SPI_PROTOCOL_MOTOROLA);

}

/* USER CODE BEGIN 1 */


void Prepare_SPI1_DMA(void)
{
    /* DMA2 used for SPI1 Transmission
     * DMA2 used for SPI1 Reception
     */

    //Disable both stream!
    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_3);

    /* (3) Configure the DMA2_Stream2 functional parameters */
    // READ!
    LL_DMA_ConfigTransfer(DMA2,
                          LL_DMA_STREAM_2,
                          LL_DMA_DIRECTION_PERIPH_TO_MEMORY | LL_DMA_PRIORITY_HIGH | LL_DMA_MODE_NORMAL |
                          LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_INCREMENT |
                          LL_DMA_PDATAALIGN_BYTE | LL_DMA_MDATAALIGN_BYTE);

    /* (4) Configure the DMA2_Stream3 functional parameters */
    // WRITE!
    LL_DMA_ConfigTransfer(DMA2,
                          LL_DMA_STREAM_3,
                          LL_DMA_DIRECTION_MEMORY_TO_PERIPH | LL_DMA_PRIORITY_HIGH | LL_DMA_MODE_NORMAL |
                          LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_NOINCREMENT |
                          LL_DMA_PDATAALIGN_BYTE | LL_DMA_MDATAALIGN_BYTE);

    /* (5) Enable DMA interrupts complete/error */
    LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_2);
    LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_2);
    LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_3);
    LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_3);

    LL_SPI_EnableDMAReq_RX(SPI1);
    LL_SPI_EnableDMAReq_TX(SPI1);
}


void Configure_SPI1_DMA(uint32_t* buf, uint32_t len, char read)
{
    // read == true >> lettura      da SPI.
    //      == false>> Scrittura    SU spi!

    uint32_t c = 0;


    /* DMA2 used for SPI1 Transmission
     * DMA2 used for SPI1 Reception
     */

    //DIsable both stream!
//    LL_SPI_Disable(SPI1);
    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_3);


    /* (3) Configure the DMA2_Stream2 functional parameters */
    // READ!

    LL_DMA_ConfigAddresses(DMA2,
                           LL_DMA_STREAM_2,
                           LL_SPI_DMA_GetRegAddr(SPI1), (uint32_t)buf,
                           LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_2));
    LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, len);


    /* (4) Configure the DMA2_Stream3 functional parameters */
    // WRITE!

    LL_DMA_ConfigAddresses(DMA2,
                           LL_DMA_STREAM_3,
                           (uint32_t)(&c), LL_SPI_DMA_GetRegAddr(SPI1),
                           LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_3));
    LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_3, len);


    //start DMA:
    SPI1_ReceiveComplete = 0;

    /* Enable DMA Channels */
    LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);
    LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_3);

    if (read)
    {
        while (SPI1_ReceiveComplete == 0)      {}
    }
    //while (LL_SPI_IsActiveFlag_BSY(SPI1))   {}
}



void Configure_SPI1_DMA_oldandwork(uint32_t* buf, uint32_t len, char read)
{
    // read == true >> lettura      da SPI.
    //      == false>> Scrittura    SU spi!

    uint8_t c = 0;


    /* DMA2 used for SPI1 Transmission
     * DMA2 used for SPI1 Reception
     */

    //DIsable both stream!
//    LL_SPI_Disable(SPI1);
    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_3);


    /* (3) Configure the DMA2_Stream2 functional parameters */
    // READ!
    LL_DMA_ConfigTransfer(DMA2,
                          LL_DMA_STREAM_2,
                          LL_DMA_DIRECTION_PERIPH_TO_MEMORY | LL_DMA_PRIORITY_HIGH | LL_DMA_MODE_NORMAL |
                          LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_INCREMENT |
                          LL_DMA_PDATAALIGN_BYTE | LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_ConfigAddresses(DMA2,
                           LL_DMA_STREAM_2,
                           LL_SPI_DMA_GetRegAddr(SPI1), (uint32_t)buf,
                           LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_2));
    LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, len);


    /* (4) Configure the DMA2_Stream3 functional parameters */
    // WRITE!
    LL_DMA_ConfigTransfer(DMA2,
                          LL_DMA_STREAM_3,
                          LL_DMA_DIRECTION_MEMORY_TO_PERIPH | LL_DMA_PRIORITY_HIGH | LL_DMA_MODE_NORMAL |
                          LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_NOINCREMENT |
                          LL_DMA_PDATAALIGN_BYTE | LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_3, (uint32_t)(&c), LL_SPI_DMA_GetRegAddr(SPI1),
                           LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_3));
    LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_3, len);


    /* (5) Enable DMA interrupts complete/error */
    LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_2);
    LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_2);
    LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_3);
    LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_3);

    /* Enable DMA RX Interrupt */
    LL_SPI_EnableDMAReq_RX(SPI1);
    /* Enable DMA TX Interrupt */
    LL_SPI_EnableDMAReq_TX(SPI1);

    //start DMA:
    SPI1_ReceiveComplete = 0;

    /* Enable SPI1 */
//    LL_SPI_Enable(SPI1);

    /* Enable DMA Channels */
    LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);
    LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_3);

    if (read)
    {
        while (SPI1_ReceiveComplete == 0)      {}
    }
    //while (LL_SPI_IsActiveFlag_BSY(SPI1))   {}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////SPI 2 - TFT with DMA//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


void SPI_WR8(uint8_t data)
{
    while(!LL_SPI_IsActiveFlag_TXE(SPI2))    {}
        LL_SPI_TransmitData8 (SPI2, data);
        //while(!LL_SPI_IsActiveFlag_RXNE(SPI2)) {}
        (void) SPI2->DR; //fake Rx read;
    while (LL_SPI_IsActiveFlag_BSY(SPI2))    {}
}

void SPI_WR16(uint16_t data)
{
    while(!LL_SPI_IsActiveFlag_TXE(SPI2))    {}
        LL_SPI_TransmitData16 (SPI2, data);
        //while(!LL_SPI_IsActiveFlag_RXNE(SPI2)) {}
        (void) SPI2->DR; //fake Rx read;
    while (LL_SPI_IsActiveFlag_BSY(SPI2))    {}
}


void SPI2_DMA_WriteConst(const uint8_t* buf, uint32_t len)
{
    //note: SPI2 using DMA1, LL_DMA_STREAM_4, LL_DMA_CHANNEL_0
    uint32_t n = len;

    //Disable stream, or wait until is finish...
    while(!LL_SPI_IsActiveFlag_TXE(SPI2))    {}
//    LL_SPI_Disable(SPI2);
    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_4);

    LL_SPI_SetDataWidth(SPI2, LL_SPI_DATAWIDTH_16BIT);
    LL_DMA_ClearFlag_TC4(DMA1);
    LL_DMA_ClearFlag_TE4(DMA1);

//    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_4, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_4, LL_DMA_MEMORY_NOINCREMENT);

//    LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_4, LL_DMA_PDATAALIGN_HALFWORD);
//    LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_4, LL_DMA_MDATAALIGN_HALFWORD);

    LL_SPI_EnableDMAReq_TX(SPI2);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_4);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_4);

    if (len > 0xFFFF)
        n = 0xFFFF;
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_4, n);

    //set buffer start
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_4,
                             (uint32_t)buf, LL_SPI_DMA_GetRegAddr(SPI2),
                             LL_DMA_DIRECTION_MEMORY_TO_PERIPH
                             );

    SPI2_TransmitComplete = 0;

    // ** Activate SPI2 !
    // Enable SPI2
//    LL_SPI_Enable(SPI2);
    // Enable DMA Stream Tx
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);

    if (len > 0xFFFF)
    {
        n = len - 0xFFFF;
        while (SPI2_TransmitComplete == 0)      {}
        SPI2_TransmitComplete = 0;
        while (LL_SPI_IsActiveFlag_BSY(SPI2))   {}

        LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_4, n);
        //LL_SPI_Enable(SPI2);
        LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);
    }

    while (SPI2_TransmitComplete == 0)      {}
    while (LL_SPI_IsActiveFlag_BSY(SPI2))   {}

    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_4);
    LL_SPI_SetDataWidth(SPI2, LL_SPI_DATAWIDTH_8BIT);

}


void SPI2_DMA_Copy(const uint8_t* buf, uint32_t len)
{
    //note: SPI2 using DMA1, LL_DMA_STREAM_4, LL_DMA_CHANNEL_0
    uint32_t n = len;

    //Disable stream, or wait until is finish...
    while(!LL_SPI_IsActiveFlag_TXE(SPI2))    {}
//    LL_SPI_Disable(SPI2);
    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_4);

    LL_SPI_SetDataWidth(SPI2, LL_SPI_DATAWIDTH_16BIT);
    LL_DMA_ClearFlag_TC4(DMA1);
    LL_DMA_ClearFlag_TE4(DMA1);

    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_4, LL_DMA_MEMORY_INCREMENT);

    LL_SPI_EnableDMAReq_TX(SPI2);

    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_4);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_4);

    //
    if (len > 0xFFFF)
        n = 0xFFFF;
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_4, n);

    //set buffer start
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_4,
                             (uint32_t)buf, LL_SPI_DMA_GetRegAddr(SPI2),
                             LL_DMA_DIRECTION_MEMORY_TO_PERIPH
                             );

    SPI2_TransmitComplete = 0;

    //LL_SPI_Enable(SPI2);
    // Enable DMA Stream Tx
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);

    if (len > 0xFFFF)
    {
        n = len - 0xFFFF;
        while (SPI2_TransmitComplete == 0)      {}
        SPI2_TransmitComplete = 0;
        LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_4,
                                     (uint32_t)buf + 0x1FFFE, LL_SPI_DMA_GetRegAddr(SPI2),
                                     LL_DMA_DIRECTION_MEMORY_TO_PERIPH
                                     );
        while (LL_SPI_IsActiveFlag_BSY(SPI2))   {}

        LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_4, n);
        LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);
    }

    while (SPI2_TransmitComplete == 0)      {}
    while (LL_SPI_IsActiveFlag_BSY(SPI2))   {}

    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_4);
    LL_SPI_SetDataWidth(SPI2, LL_SPI_DATAWIDTH_8BIT);

}






void SPI2_DMA_CopyContinue(const uint8_t* buf, uint32_t len)
{
    //note: SPI2 using DMA1, LL_DMA_STREAM_4, LL_DMA_CHANNEL_0
    uint32_t n = len;

    //Disable stream, or wait until is finish...

//    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_4);

    LL_DMA_ClearFlag_TC4(DMA1);
    LL_DMA_ClearFlag_TE4(DMA1);

    //security
    if (len > 0xFFFF)
        n = 0xFFFF;

    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_4, n);

    //set buffer start
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_4,
                             (uint32_t)buf, LL_SPI_DMA_GetRegAddr(SPI2),
                             LL_DMA_DIRECTION_MEMORY_TO_PERIPH
                             );

    SPI2_TransmitComplete = 0;

    // Enable DMA Stream Tx
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);
}




/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
