/*
 * EEProm.c
 *
 *  Created on: 07 apr 2020
 *      Author: Massimo WS
 */

#include "EEProm.h"
#include "algorithm.h"

#include <string.h>

unsigned char  block_protection_6[6];


uint32_t GetJedec(void)
{
    uint32_t answ = 0;
    EEPROM_ENABLE;
    LL_SPI_ReceiveData8 (SPI1); //dummy read

    LL_SPI_TransmitData8 (SPI1, 0x9F);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);
    //while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x00);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    answ = LL_SPI_ReceiveData8 (SPI1);
    answ <<= 8;
    //while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x00);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    answ |= LL_SPI_ReceiveData8 (SPI1);
    answ <<= 8;
    //while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x00);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    answ |= LL_SPI_ReceiveData8 (SPI1);
    while(LL_SPI_IsActiveFlag_RXNE(SPI1)) {}

    EEPROM_DISABLE;

    return answ;
}


uint32_t GetStatusRegister(void)
{
    uint16_t answ = 0;

    EEPROM_ENABLE;
    LL_mDelay(10);
    answ = LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, 0x05);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    answ = LL_SPI_ReceiveData8 (SPI1);
    //answ <<= 8;   dummy read!
    LL_SPI_TransmitData8 (SPI1, 0x00);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    answ = LL_SPI_ReceiveData8 (SPI1);
    LL_mDelay(1);
    EEPROM_DISABLE;

    return answ;
}


void SectorEraseAndWait(uint32_t addr)
{
//    uint8_t answ = 0;
    EEPROM_ENABLE;
//    LL_mDelay(1);
    LL_SPI_ReceiveData8 (SPI1); //dummy read

    LL_SPI_TransmitData8 (SPI1, 0xD8);  //Anche 0x20!
//    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, addr>>16);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
//    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_TransmitData8 (SPI1, addr>>8);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
//    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_TransmitData8 (SPI1, addr);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1); //dummy read

    // Wait foir status register == not busy

    uint16_t answ = 0;
    do {
        LL_SPI_TransmitData8 (SPI1, 0x05);
        while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
        answ = LL_SPI_ReceiveData8 (SPI1);
        //answ <<= 8;   dummy read!

        LL_SPI_TransmitData8 (SPI1, 0x00);
        while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
        answ = LL_SPI_ReceiveData8 (SPI1);
    } while (answ&0x01 );

    //

//    LL_mDelay(1);
    EEPROM_DISABLE;
}


uint32_t GetConfigRegister(void)
{
    uint16_t answ = 0;
    EEPROM_ENABLE;
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, 0x35);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    answ = LL_SPI_ReceiveData8 (SPI1);
    //answ <<= 8;   dummy read?
    LL_SPI_TransmitData8 (SPI1, 0x00);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    answ = LL_SPI_ReceiveData8 (SPI1);
    EEPROM_DISABLE;

    return answ;
}


uint32_t SetConfigRegister(uint8_t val)
{
    uint16_t answ = 0;

    FlashWriteEnable();
    LL_SPI_ReceiveData8(SPI1);
    EEPROM_ENABLE;
    LL_mDelay(1);
    LL_SPI_TransmitData8 (SPI1, 0x00);      //status register?
    LL_SPI_ReceiveData8(SPI1);  //dummy read
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_TransmitData8 (SPI1, val);      //config register!
    LL_SPI_ReceiveData8(SPI1);  //dummy read
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_mDelay(1);
    EEPROM_DISABLE;
    return answ;       //for now!
}



uint8_t ReadOneByte(uint32_t addr)
{
    uint8_t answ = 0;

    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    EEPROM_ENABLE;

    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x03);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, addr>>16);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, addr>>8);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, addr);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    answ = LL_SPI_ReceiveData8 (SPI1);

    EEPROM_DISABLE;

    return answ;
}


void FlashReadBlock(uint8_t *buf, uint32_t addr,  uint32_t byteLen)
{
    EEPROM_ENABLE;

    LL_SPI_ReceiveData8 (SPI1);     //Dummy

    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x03);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr>>16);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr>>8);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr & 0xFF);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    for (int n = 0; n < byteLen; n++)
    {
        LL_SPI_TransmitData8 (SPI1, 0);
        while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
        buf[n] = LL_SPI_ReceiveData8 (SPI1);
    }

    EEPROM_DISABLE;
}


void FlashReadBlockInit(uint32_t addr)
{
    EEPROM_ENABLE;

    LL_SPI_ReceiveData8 (SPI1);     //Dummy

    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x03);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr>>16);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr>>8);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr & 0xFF);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);
}



void FlashReadContinue(uint8_t *buf, uint32_t byteLen)
{
    while (byteLen--)
    {
        LL_SPI_TransmitData8 (SPI1, 0);
        while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
        *(buf++) = LL_SPI_ReceiveData8 (SPI1);
    }
}

void FlashReadEnd()
{
    EEPROM_DISABLE;
}




//Non va???
void FlashReadBlock16(uint16_t *buf, uint32_t addr,  uint32_t len)
{
    EEPROM_ENABLE;

    LL_SPI_ReceiveData8 (SPI1);


    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x03);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr>>16);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr>>8);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr & 0xFF);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_mDelay(1);
    LL_SPI_SetDataWidth(SPI1,LL_SPI_DATAWIDTH_16BIT );


    for (int n = 0; n < len/2; n++)
    {
        LL_SPI_TransmitData16 (SPI1, 0);
        while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
        buf[n] = LL_SPI_ReceiveData16 (SPI1);
    }
    EEPROM_DISABLE;
    LL_SPI_SetDataWidth(SPI2,LL_SPI_DATAWIDTH_8BIT );
}


void FlashReadSecureID(uint8_t *buf, uint32_t addr,  uint32_t len)
{
    EEPROM_ENABLE;

    LL_SPI_ReceiveData8 (SPI1);

    uint8_t c;
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x88);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr>>8);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr & 0xFF);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, 0);     //dummy cycle
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    for (int n = 0; n < len; n++)
    {
        LL_SPI_TransmitData8 (SPI1, 0);
        while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
        c= LL_SPI_ReceiveData8 (SPI1);
        buf[n] = c;
    }

    EEPROM_DISABLE;
}


void FlashWriteSecureID(const uint8_t *buf, uint32_t addr, uint32_t len)
{
    EEPROM_ENABLE;
    //    LL_mDelay(1);
    uint8_t c;
    LL_SPI_ReceiveData8 (SPI1); //dummy read

    LL_SPI_TransmitData8 (SPI1, 0xA5);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_TransmitData8 (SPI1, addr>>8);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_TransmitData8 (SPI1, addr & 0xFF);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}

    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1); //dummy read

    while (len--)
    {
        c = *(buf++);
        LL_SPI_TransmitData8 (SPI1, c);
        while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
        //        while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
        //        LL_mDelay(30);
    }
    //    LL_mDelay(1);
    EEPROM_DISABLE;
}


uint32_t FlashEraseControl(uint32_t addr,  uint32_t len)
{
    EEPROM_ENABLE;

    LL_SPI_ReceiveData8 (SPI1);


    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x03);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr>>16);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr>>8);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    LL_SPI_TransmitData8 (SPI1, addr & 0xFF);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);

    uint8_t c;
    uint32_t n;
    for (n = 0; n < len; n++)
    {
        LL_SPI_TransmitData8 (SPI1, 0);
        while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
        c = LL_SPI_ReceiveData8 (SPI1);
        if (c != 0xFF)
            break;
    }

    if (c == 0xFF)
        n = 0xFFFFFF;
    EEPROM_DISABLE;

    return n;
}


uint16_t FlashCalcCRC16(uint32_t addr,  uint32_t len)
{
    uint16_t crc = 0;

    int whole_cycle, rest;
    int ind;
    whole_cycle = len/256;
    rest        = len%256;
    uint8_t buf[256];

    CRC162(NULL, 0, 1);      //init CRC

    for(int n = 0; n < whole_cycle; n++)
    {
        ind = addr + n*256;
        FlashReadBlock(buf, ind, 256);
        crc = CRC162(buf, 256, 0);
    }

    if (rest != 0)
    {
        ind = addr + 256*whole_cycle;
        FlashReadBlock(buf, ind, rest);
        crc = CRC162(buf, rest, 0);
    }

    return crc;
}



void FlashWriteEnable(void)
{
//    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    EEPROM_ENABLE;
    LL_SPI_ReceiveData8 (SPI1);
//    LL_mDelay(1);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x06);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {} //?
//    LL_mDelay(1);
    EEPROM_DISABLE;
}


void FlashWriteDisable(void)
{
//    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    EEPROM_ENABLE;
//    LL_mDelay(1);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x04);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
//    LL_mDelay(1);
    EEPROM_DISABLE;
}





void SectorErase(uint32_t addr, char block)
{
//    uint8_t answ = 0;
    EEPROM_ENABLE;
    //LL_mDelay(1);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_ReceiveData8 (SPI1); //dummy read

    LL_SPI_TransmitData8 (SPI1, block? 0xD8:0x20);  //se blocco D8, altrim 0x20 = 4k
//    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_ReceiveData8 (SPI1); //dummy read
//    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, addr>>16);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_ReceiveData8 (SPI1); //dummy read
//    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
//    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_TransmitData8 (SPI1, addr>>8);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_ReceiveData8 (SPI1); //dummy read
//    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
//    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_TransmitData8 (SPI1, addr);
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
//    LL_SPI_ReceiveData8 (SPI1); //dummy read
//    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1); //dummy read

    //LL_mDelay(2);
    EEPROM_DISABLE;
}


void FlashPageProgram(const uint8_t *buf, uint32_t addr, uint32_t len)
{
    EEPROM_ENABLE;
//    LL_mDelay(1);
    LL_SPI_ReceiveData8 (SPI1); //dummy read

    LL_SPI_TransmitData8 (SPI1, 0x02);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_TransmitData8 (SPI1, addr>>16);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_TransmitData8 (SPI1, addr>>8);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_TransmitData8 (SPI1, addr & 0xFF);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}

    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1); //dummy read

    while (len--)
    {
        LL_SPI_TransmitData8 (SPI1, *(buf++));
        while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
//        while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
//        LL_mDelay(30);
    }
//    LL_mDelay(1);
    EEPROM_DISABLE;
}


uint64_t ReadBlockProtection(void)
{
    uint64_t answ = 0;

    EEPROM_ENABLE;
    LL_SPI_ReceiveData8 (SPI1); //dummy read

    LL_SPI_TransmitData8 (SPI1, 0x72);
    memset(block_protection_6, 0, sizeof(block_protection_6));
//    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    while(LL_SPI_IsActiveFlag_BSY(SPI1)) {}
    LL_SPI_ReceiveData8 (SPI1);  //dummy read
    int len = 6;
    while (len--)
    {
//        while(!LL_SPI_IsActiveFlag_TXE(SPI1)) {}
        while(LL_SPI_IsActiveFlag_BSY(SPI1)) {}
        LL_SPI_TransmitData8 (SPI1, 0);
        while(LL_SPI_IsActiveFlag_BSY(SPI1)) {}
        while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
        block_protection_6[len] = LL_SPI_ReceiveData8 (SPI1);
        while(LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    }
    LL_mDelay(10);
    EEPROM_DISABLE;
    for (int n = 0; n < 6; n++)
    {
        answ <<= 8;
        answ |= block_protection_6[n];
    }
    return answ;
}


void WriteALLBlockProtection(void)
{
//    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    EEPROM_ENABLE;
//    LL_mDelay(1);

    LL_SPI_ReceiveData8 (SPI1);  //dummy read
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x42);
    for(int n = 0; n < 6; n++)
    {
        //while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
        while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
        LL_SPI_TransmitData8 (SPI1, 0x00);
        LL_mDelay(1);
        LL_SPI_ReceiveData8 (SPI1); //dummy read
    }

    //while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
//    LL_mDelay(1);
    EEPROM_DISABLE;

}



void GlobalWriteProtectionUnlock(void)
{
//    uint8_t answ = 0;
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    EEPROM_ENABLE;

    while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
    LL_SPI_TransmitData8 (SPI1, 0x98);
    while (LL_SPI_IsActiveFlag_BSY(SPI1))    {}
    while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}

    EEPROM_DISABLE;
}

