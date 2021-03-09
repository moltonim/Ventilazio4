/*
 * XPT2046.h
 *
 *  Created on: 05 apr 2020
 *      Author: Massimo WS
 */

#ifndef INC_XPT2046_H_
#define INC_XPT2046_H_


#define TOUCH_DESEL()   LL_GPIO_SetOutputPin(TFT_RST_GPIO_Port, TFT_RST_Pin)
#define TOUCH_SEL()     LL_GPIO_ResetOutputPin(TFT_RST_GPIO_Port, TFT_RST_Pin)


void XPT2046_init(void);




#endif /* INC_XPT2046_H_ */
