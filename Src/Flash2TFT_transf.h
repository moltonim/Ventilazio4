/*
 * Flash2TFT_transf.h
 *
 *  Created on: 03 mag 2020
 *      Author: Massimo WS
 */

/*
 * Struttura per il doppio buffer, trasferimento immagine - font da flash a TFT
 * Tecnica del doppio buffer: quando uno è pieno uso l'altro
 *
 * Non so quale sto usando, cerco in entrambe!
 *
 */


#ifndef FLASH2TFT_TRANSF_H_
#define FLASH2TFT_TRANSF_H_





#define TWOBUF_SIZE            0x1000       // =  8192 8k



typedef struct {

    uint8_t busy;

    uint32_t    Buf[TWOBUF_SIZE/4];
    int         GlobalDimension;
    int         volte, resto;
    uint32_t    flashAddr_start;

} DBuffType;


extern DBuffType   TwobufTransf;

void FlashToTFT_transf_Init(void);
void FlashToTFT_transf_Setup(uint32_t flashAddr, uint32_t Len);
void FlashToTFT_Server(void);



#endif /* FLASH2TFT_TRANSF_H_ */
