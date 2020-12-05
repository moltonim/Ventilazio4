/*
 * EEProm.h
 *
 *  Created on: 07 apr 2020
 *      Author: Massimo WS
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "main.h"

uint32_t GetJedec(void);
uint32_t GetStatusRegister(void);
uint32_t GetConfigRegister(void);
uint32_t SetConfigRegister(uint8_t val);
uint8_t  ReadOneByte(uint32_t addr);
void     FlashReadBlock(uint8_t *buf, uint32_t addr, uint32_t len);
void     FlashReadBlock16(uint16_t *buf, uint32_t addr,  uint32_t byteLen);
void     FlashReadSecureID(uint8_t *buf, uint32_t addr,  uint32_t len);
void     FlashWriteSecureID(const uint8_t *buf, uint32_t addr, uint32_t len);
uint16_t FlashCalcCRC16(uint32_t addr,  uint32_t len);
uint32_t FlashEraseControl(uint32_t addr,  uint32_t len);

void     FlashWriteEnable(void);
void     FlashWriteDisable(void);
void     SectorErase(uint32_t addr, char block);
void     SectorEraseAndWait(uint32_t addr);
void     FlashPageProgram(const uint8_t *buf, uint32_t addr, uint32_t len);
uint64_t ReadBlockProtection(void);
void     WriteALLBlockProtection(void);
void     GlobalWriteProtectionUnlock(void);


void FlashReadBlockInit(uint32_t addr);
void FlashReadContinue(uint8_t *buf, uint32_t byteLen);
void FlashReadEnd();


#define EEPROM_DISABLE  LL_GPIO_SetOutputPin(FLASH_CS_GPIO_Port, FLASH_CS_Pin)
#define EEPROM_ENABLE   LL_GPIO_ResetOutputPin(FLASH_CS_GPIO_Port, FLASH_CS_Pin)


#endif /* INC_EEPROM_H_ */
