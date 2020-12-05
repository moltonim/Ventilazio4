/*
 * Flash2TFT_transf.c
 *
 *  Created on: 03 mag 2020
 *      Author: Massimo WS
 */


#include <string.h>

#include "main.h"
#include "spi.h"
#include "EEProm.h"
#include "disptft.h"
#include "font.h"
#include "bitmap.h"
#include "CommRequest.h"
#include "disptft.h"

#include "Flash2TFT_transf.h"


DBuffType   TwobufTransf;

//extern const unsigned char Neolo_Icon_map[];
//extern const unsigned char galaxy_320x240_2_map[];


extern volatile char SPI2_TransmitComplete;
extern volatile char SPI1_ReceiveComplete;



void FlashToTFT_transf_Init(void)
{
    memset((char*)&TwobufTransf, 0, sizeof(TwobufTransf));
}

//Who call the setup?
void FlashToTFT_transf_Setup(uint32_t flashAddr, uint32_t Len)
{
     if (TwobufTransf.busy)
         return;                //it will be up to user take care of this!

     TwobufTransf.busy  = 1;
     TwobufTransf.GlobalDimension = Len;
     TwobufTransf.flashAddr_start = flashAddr;
     TwobufTransf.volte = Len/TWOBUF_SIZE;
     TwobufTransf.resto = Len%TWOBUF_SIZE;
}


#define BITMAP_FLASH_ADDR           0x010000
#define BITMAP_FLASH_DIM            153604     //decimale: dimensione del file "galaxy_320x240.bin"


//Call this in the main loop!
void FlashToTFT_Server(void)
{
//    if (!TwobufTransf.busy)             //nothing to serve, can go away!
//        return;

    //Test 1: tutto a mano: da flash a buffer, dabuffer a TFT (senza dma ALCUNO!)
    // - sospensiva!

    int n;

    TwobufTransf.GlobalDimension = BITMAP_FLASH_DIM;
    TwobufTransf.flashAddr_start = BITMAP_FLASH_ADDR;
    TwobufTransf.volte = BITMAP_FLASH_DIM/TWOBUF_SIZE;
    TwobufTransf.resto = BITMAP_FLASH_DIM%TWOBUF_SIZE;

    TFT_Icon_prepare(0, 0, 320, 240);
    FlashReadBlockInit(TwobufTransf.flashAddr_start);
    Prepare_SPI1_DMA();

    LL_GPIO_SetOutputPin(TESTPOINT_1_GPIO_Port, TESTPOINT_1_Pin);

//    SPI1_ReceiveComplete = 1;

    int len;
    for (n = 0; n < TwobufTransf.volte+1; n++)
    {
        len = (n == TwobufTransf.volte)? TwobufTransf.resto: TWOBUF_SIZE;
        Configure_SPI1_DMA((uint32_t*)TwobufTransf.Buf, len, 0);
        SPI2_DMA_CopyContinue((uint8_t*)TwobufTransf.Buf, len/2);
        while (SPI2_TransmitComplete == 0)  { }
    }

    FlashReadEnd();
    TFT_Icon_End();
    LL_GPIO_ResetOutputPin(TESTPOINT_1_GPIO_Port, TESTPOINT_1_Pin);
}

