/*
 * CommRequest.h
 *
 *  Created on: 17 apr 2020
 *      Author: Massimo WS
 */

#ifndef COMMREQUEST_H_
#define COMMREQUEST_H_


typedef enum    {
    FW_VERSION          = 1,
    BOOT_VERSION,

    FLASH_TYPE,
    FLASH_READ,
    FLASH_WRITE,
    FLASH_CRC,
    FLASH_ERASE_SECTOR_4k,
    FLASH_JEDEC_ID,
    FLASH_RBPR,
    FLASH_WBPR,
    FLASH_STATUS,
    FLASH_WREN,
    FLASH_WRDI,
    FLASH_ERASE_BLOCK,
    FLASH_ERASED_CHECK,
    FLASH_READ_UNIQUE_ID,
    FLASH_READ_MDBADDR,
    FLASH_WRITE_MDBADDR,



    SET_MDB_ADDR        = 20,

    SET_BACKLIGHT       = 21,
    BACKLIGHT_BLINK_Hz,
    CLEARSCREEN,

    VENTILAZIO          = 25,

    SCREEN_SETFONT      = 35,
    WRITE_TEXT,
    WRITE_CENTER,

    SYSTEM_REBOOT       = 100,

} _COMM_CMD;




uint16_t HandleCommMsg(uint8_t *msg, int len);


#endif /* COMMREQUEST_H_ */
