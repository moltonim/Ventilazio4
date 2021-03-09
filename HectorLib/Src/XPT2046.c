/*
 * XPT2046.c
 *
 *  Created on: 05 apr 2020
 *      Author: Massimo WS
 */

#include "main.h"
#include "spi.h"
#include "gpio.h"

#include "XPT2046.h"


static uint8_t SendReceiveTouch_8(uint8_t data)
{
    while(!LL_SPI_IsActiveFlag_TXE(SPI3)) {}
    LL_SPI_TransmitData8 (SPI3, data);
    while(!LL_SPI_IsActiveFlag_RXNE(SPI3)) {}
    // (void) SPIx->DR; //fake Rx read;
    while (LL_SPI_IsActiveFlag_BSY(SPI3));
    return LL_SPI_ReceiveData8(SPI3);
}

uint16_t XPT2046_GetTouch(uint8_t address)
{
    uint16_t data = 0;

    TOUCH_SEL();
    SendReceiveTouch_8(address);
    data = SendReceiveTouch_8(0X00);
    data <<= 8;
    data |= SendReceiveTouch_8(0X00);
    data >>= 3;

    TOUCH_DESEL();
    return data;
}

/*
void XPT2046_GetTouch_XY(volatile uint16_t* x_kor, volatile uint16_t* y_kor, uint8_t count_read)
{
    uint8_t i = 0;
    uint16_t tmpx, tmpy, touch_x, touch_y = 0;

    touch_x = XPT2046_GetTouch(X);
    //delay_us(100);
    touch_y = XPT2046_GetTouch(Y);
    for (i = 0; i < count_read; i++) {
        tmpx = XPT2046_GetTouch(X);
        //delay_us(100);
        tmpy = XPT2046_GetTouch(Y);

        if (tmpx == 0)  tmpy = 0;
        else if (tmpy == 0)  tmpx = 0;
    else
    {
            touch_x = (touch_x + tmpx) / 2;
            touch_y = (touch_y + tmpy) / 2;
    }

    }
        *x_kor = touch_x;
        *y_kor = touch_y;

}
*/

/**
 * Get the current position and state of the touchpad
 * @param data store the read data here
 * @return false: because no ore data to be read
 */

/*
bool XPT2046_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    static uint16_t last_x = 0;
    static uint16_t last_y = 0;
    uint16_t x = 0;
    uint16_t y = 0;
    uint8_t irq = LV_DRV_INDEV_IRQ_READ;

    if (irq == 0) {
        XPT2046_GetTouch_XY(&x, &y, 1);
        xpt2046_corr(&x, &y);
        // xpt2046_avg(&x, &y);

        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PR;
    }
    else
        data->state = LV_INDEV_STATE_REL;
   // printf("X=%d  Y= %d  m_sec=%d count=%d\n\r", x, y, millis(), count);
    return false;
}

*/

//Touchscreen
void XPT2046_init(void)
{
    uint8_t d;
    LL_SPI_Enable(SPI3);
    TOUCH_SEL();
    d = SendReceiveTouch_8(0x80);
    d = SendReceiveTouch_8(0x00);
    d = SendReceiveTouch_8(0x00);
    LL_mDelay(2);
    d++;
    TOUCH_DESEL();
}
