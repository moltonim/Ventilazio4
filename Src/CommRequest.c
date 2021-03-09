/*
 * CommRequest.c
 *
 *  Created on: 17 apr 2020
 *      Author: Massimo WS
 */

// gestisco i vari comandi che mi arrivano dalla seriale

#include <string.h>

#include "main.h"
#include "EEProm.h"
#include "disptft.h"
#include "font.h"
#include "bitmap.h"
#include "Ventilo.h"
#include "CommRequest.h"


extern uint32_t Backlight_toggle;

/*          FORMATO:
 *
 * <AddrH><AddrL><Command>[3*ADDR]<Len><Data0><Data1>...<DataLen-1><CRC_H><CRC_L>
 *Lunghezza dati max = 256
 *Lunghezza max telegram = 256+2+1+1+2+3 =265 [Buffer length = 270]]
 *ADDR: indirizzo scrittura FLASH (3 byte)
 *
 *Risposta:
 *
 *<AddrH | 0x80><AddrL><Command><Len><Data1>...<DataLen-1><CRC_H><CRC_L>
 *Lunghezza =unknow, buffer == ricezione
 *
 *
 *
 *
 *
 */


//static char buffer1[50];

uint16_t HandleCommMsg(uint8_t *msg, int len)
{

    uint16_t    carToSend = 0;
    int n;
    uint16_t    x, y, ink, paper;

    msg[0] |= 0x80;
    uint8_t cmd =  msg[2];
    uint8_t msglen =  msg[6];
    uint8_t *data = &msg[3];
//    char* c;

    uint32_t addr =msg[3];
    addr <<= 8;    addr |= msg[4];
    addr <<= 8;    addr |= msg[5];

    carToSend = 3;


    switch (cmd)
    {
        case FW_VERSION:
            *(data++) = 2 ;
            *(data++) = 0 ;
            *(data++) = 1 ;
            carToSend += 3;
            break;

        case FLASH_JEDEC_ID:
            n = GetJedec();
            *(data++) = 3;
            *(data++) = n>>16 ;
            *(data++) = n>>8 ;
            *(data++) = n&0xFF ;
            carToSend += 4;
            break;

        case FLASH_CRC:
            n =  msg[6]; n <<= 8;
            n |= msg[7]; n <<= 8;
            n |= msg[8];            //calc len
            n = FlashCalcCRC16(addr, n);
            *(data++) = 2;
            *(data++) = n>>8 ;
            *(data++) = n&0xFF ;
            carToSend += 3;
            break;

        case FLASH_ERASED_CHECK:
            n =  msg[6]; n <<= 8;
            n |= msg[7]; n <<= 8;
            n |= msg[8];            //calc len
            n = FlashEraseControl(addr, n);
            *(data++) = 3;
            *(data++) = n>>16 ;
            *(data++) = n>>8 ;
            *(data++) = n&0xFF ;
            carToSend += 4;
            break;

        case FLASH_STATUS:
            n = GetStatusRegister();
            *(data++) = n&0xFF ;
            carToSend += 1;
            break;

        case FLASH_WREN:
            FlashWriteEnable();
            break;

        case FLASH_WRDI:
            FlashWriteDisable();
            break;


        case FLASH_READ:
            *(data++) = msglen ;
            FlashReadBlock(data, addr, msglen+1);
            carToSend += msglen+1+1;
            break;

        case FLASH_READ_UNIQUE_ID:
            FlashReadSecureID(data, 0, 8);
            carToSend += 8 + 1;
            break;

        case FLASH_READ_MDBADDR:
            // deve essere >= 8!!!
            FlashReadSecureID(data, 20, 2);
            carToSend += 2;
            break;

        case FLASH_WRITE_MDBADDR:
            // deve essere >= 8!!!
            FlashWriteEnable();
            FlashWriteSecureID(data, 20, 2);
//            carToSend += 2 + 1;
            break;

        case FLASH_WRITE:
            n = addr%256;
            n +=  msglen;
            if (n < 256)
            {
                FlashWriteEnable();
                data += 4;  //3 address 1 length
                FlashPageProgram(data, addr, msglen+1);
                n = 1;      //meant OK
            }
            else
            {
                n = 0;      //NOK
            }
            msg[3] = 1;
            msg[4] = n;
            carToSend += 2;
            break;


        case FLASH_ERASE_SECTOR_4k:
            FlashWriteEnable();             //A carico dell'USER?
            //*(data++) = addr & (~0x2000) ;
            addr &= 0x2000;     //force 2k
            SectorErase(addr, 0);
            //carToSend += 1;
            break;


        case FLASH_ERASE_BLOCK:
            FlashWriteEnable();             //A carico dell'USER?
            //*(data++) = addr & (~0x2000) ;
            //addr &= 0x2000;     //force 2k
            SectorErase(addr, 1);
            //carToSend += 1;
            break;


        case FLASH_RBPR:
        {
            uint64_t bp = ReadBlockProtection();
            *(data++) = 6;
            *(data++) = bp>>(5*8) ;
            *(data++) = bp>>(4*8) ;
            *(data++) = bp>>(3*8) ;
            *(data++) = bp>>(2*8) ;
            *(data++) = bp>>(1*8) ;
            *(data++) = bp;
            carToSend += 7;
            break;
        }

        case FLASH_WBPR:
            FlashWriteEnable();
            WriteALLBlockProtection();
            break;

        case SET_BACKLIGHT:
            if(!(*data))
            {
                LL_GPIO_SetOutputPin(TFT_LIGHT_GPIO_Port, TFT_LIGHT_Pin);
            }
            else
            {
                LL_GPIO_ResetOutputPin(TFT_LIGHT_GPIO_Port, TFT_LIGHT_Pin);
            }
            msg[3] = 1;
            carToSend += 1;
            break;

        case BACKLIGHT_BLINK_Hz:
            n = *data;
            carToSend += 1;
            if (n > 20)
            {
                msg[3] = 0;
                break;
            }
            if (n == 0)
            {
                LL_GPIO_ResetOutputPin(TFT_LIGHT_GPIO_Port, TFT_LIGHT_Pin);
            }
            else
            {
                //Blink freq, 1 to 20 Hz
                Backlight_toggle = (10000)/n;
            }
            msg[3] = 1;
            break;

        case CLEARSCREEN:
            n = *(data++);
            n <<= 8;
            n |= *(data);
            TFT_FillScreenDMA(n);
            msg[3] = 1;
            carToSend += 1;
            break;

////////////////////////////////////////////////////////////////
        case VENTILAZIO:
            for(x = 0; x < 6; x++)
            {
                n = *(data++);
                n <<= 8;
                n |= *(data++);
                Player[x] = n;
            }
            Draw_Ventilazio(Player);
            break;
////////////////////////////////////////////////////////////////

        case SCREEN_SETFONT:
            n = *(data++);
            //Aggiungere altre font, mettere un puntatore alla struttura che dipende da n!
            TFT_SetFont(Courier_New_Bold_20, *data);
            msg[3] = 1;
            carToSend += 1;
            break;

        case WRITE_TEXT:
            x = *(data++);
            x <<= 8;
            x |= *(data++);
            y = *(data++);
            y <<= 8;
            y |= *(data++);
            ink = *(data++);
            ink <<= 8;
            ink |= *(data++);
            paper = *(data++);
            paper <<= 8;
            paper |= *(data++);
            msg[len-2] = 0;
            n = strlen((char*)data);
            //strcpy(buffer1, data);
            TFT_ConstText((char*)data, x, y, ink, paper);
            msg[3] = n;
            carToSend += 1;
            break;


        case WRITE_CENTER:
            y = *(data++);
            y <<= 8;
            y |= *(data++);
            ink = *(data++);
            ink <<= 8;
            ink |= *(data++);
            paper = *(data++);
            paper <<= 8;
            paper |= *(data++);
            msg[len-2] = 0;
            n = strlen((char*)data);
            //strcpy(buffer1, data);
            TFT_TextCENTER((char*)data, y, ink, paper);
            msg[3] = n;
            carToSend += 1;
            break;


        case SYSTEM_REBOOT:
            // nothinig to do!
            break;

        default:
            data++;
            strcpy((char*)data, "Unknow CMD");
            n = strlen((char*)data);
            msg[3] = n;
            carToSend += n+1;
            break;

    }

    return  carToSend;

}
