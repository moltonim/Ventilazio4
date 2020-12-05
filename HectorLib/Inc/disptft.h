/*
 * File:   disptft.h
 * Author: http://pic18fxx.blogspot.com 
 * 
 * Graphical display TFT. 
 * Driver: ILI9341, ST7735, ST7789, SSD1331.
 * Interface: SPI.
 * Version 3.0
 */
#ifndef DISPTFT_H
#define	DISPTFT_H
//#include "datatypes.h"
#include "stm32f2xx.h"
//==============================================================================
// Connection of module TFT.
//
// VCC      <<<>>> 3.3 V.
// GND      <<<>>> GND.
// TFT_CS   <<<>>> You can change this pin.
// TFT_REST <<<>>> You can change this pin.
// TFT_D/C  <<<>>> You can change this pin.
// TFT_SDI  <<<>>> Connect to SDO MCU.
// TFT_SCK  <<<>>> Connect to SCK MCU.
// TFT_SDO  <<<>>> Connect to SDI MCU.
// LED      <<<>>> 3.3 V (Use a 10-ohm resistor).
//==============================================================================
//#define TFT_CS_1()	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET)
//#define TFT_CS_0()	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET)
//
//#define TFT_RES_1()	HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET)
//#define TFT_RES_0()	HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_RESET)
//
//#define TFT_DC_1()	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET)
//#define TFT_DC_0()	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET)

#define TFT_CS_1()  LL_GPIO_SetOutputPin(TFT_CS_GPIO_Port, TFT_CS_Pin)
#define TFT_CS_0()  LL_GPIO_ResetOutputPin(TFT_CS_GPIO_Port, TFT_CS_Pin)

#define TFT_RES_1() LL_GPIO_SetOutputPin(TFT_RST_GPIO_Port, TFT_RST_Pin)
#define TFT_RES_0() LL_GPIO_ResetOutputPin(TFT_RST_GPIO_Port, TFT_RST_Pin)

#define TFT_DC_1()  LL_GPIO_SetOutputPin(TFT_DC_GPIO_Port, TFT_DC_Pin)
#define TFT_DC_0()  LL_GPIO_ResetOutputPin(TFT_DC_GPIO_Port, TFT_DC_Pin)

//==============================================================================
// Define your driver.
//==============================================================================
#define DRIVER_ILI9341_240x320
//#define DRIVER_ST7735_80x160
//#define DRIVER_ST7789_240x240
//#define DRIVER_SSD1331_96x64

//==============================================================================
// These are some colors that you can use for fonts and graphics.
//==============================================================================
#define BLACK              0x0000	    //0b0000000000000000
#define BRIGHTBLUE         0x001F	    //0b0000000000011111
#define BRIGHTGREEN        0x3F		    //0b0000011111100000
#define BRIGHTCYAN         0x3F1f 	    //0b0000011111111111
#define BRIGHTRED          0xF800	    //0b1111100000000000
#define BRIGHTMAGENTA      0xF81F	    //0b1111100000011111
#define BRIGHTYELLOW       0xFFE0       //0b1111111111100000
#define BLUE               0x0010 	    //0b0000000000010000
#define GREEN              0x0400	    //0b0000010000000000
#define CYAN               0x0410       //0b0000010000010000
#define RED                0x8000 	    //0b1000000000000000
#define MAGENTA            0x8010       //0b1000000000010000
#define BROWN              0xFC00 	    //0b1111110000000000
#define LIGHTGRAY          0x8410	    //0b1000010000010000
#define DARKGRAY           0x4208       //0b0100001000001000
#define LIGHTBLUE          0x841F       //0b1000010000011111
#define LIGHTGREEN         0x87F0       //0b1000011111110000
#define LIGHTCYAN          0x87FF       //0b1000011111111111
#define LIGHTRED           0xFC10       //0b1111110000010000
#define LIGHTMAGENTA       0xFC1F       //0b1111110000011111
#define YELLOW             0xFFF0       //0b1111111111110000
#define WHITE              0xFFFF       //0b1111111111111111

#define GRAY0       	   0xE71C       //0b1110011100011100
#define GRAY1         	   0xC618       //0b1100011000011000
#define GRAY2              0xA514       //0b1010010100010100
#define GRAY3              0x8410       //0b1000010000010000
#define GRAY4              0x630C       //0b0110001100001100
#define GRAY5              0x4208       //0b0100001000001000
#define GRAY6	           0x2104       //0b0010000100000100



//==============================================================================
// These are the defines to select the memory access control.
//==============================================================================
#define TFT_W               240
#define TFT_H               320
#define TFT_VERTICAL1       0x88
#define TFT_VERTICAL2       0x48
#define TFT_HORIZONTAL1     0xE8
#define TFT_HORIZONTAL2     0x28
#define TFT_VERTICAL1_BMP   0x08
#define TFT_VERTICAL2_BMP   0xC8
#define TFT_HORIZONTAL1_BMP 0xA8
#define TFT_HORIZONTAL2_BMP 0x69

//==============================================================================
// Function Prototypes.
//==============================================================================
void TFT_Init(uint8_t frame_memory_, uint16_t color);
void TFT_Driver(void);
void TFT_Reset(void);
void TFT_WriteCommand(uint8_t command);
void TFT_WriteParameter(uint8_t parameter);
void TFT_ColumnPage(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
void TFT_MemoryAccessControl(uint8_t frame_memory_);
uint8_t   TFT_ReturnMemoryAccessControl(void);
uint16_t  TFT_RGBConvert(uint16_t red, uint16_t green, uint16_t blue);
//==============================================================================
//  wip per grafina & flash

void TFT_Icon_prepare(uint16_t x, uint16_t y, uint16_t width_, uint16_t height_);
void TFT_Icon_copy(const uint16_t *buffer,  uint32_t width_);
void TFT_Icon_End();

//==============================================================================
void TFT_Pixel(uint16_t x, uint16_t y, uint16_t color);
void TFT_SetDotSize(uint8_t size_);
void TFT_Dot(uint16_t x, uint16_t y, uint16_t color);
void TFT_SetFont(const uint8_t *font_, uint8_t letterspacing);
uint8_t   TFT_CharHeight(void);
uint16_t  TFT_TextWidth(const char *buffer);
uint16_t  TFT_TextScaledWidth(const char *buffer);
void TFT_SetScale(uint8_t sc);
void TFT_WriteChar  (uint8_t c, uint16_t x, uint16_t y, uint16_t ink, uint16_t paper);
void TFT_WriteChar16(uint8_t c, uint16_t x, uint16_t y, uint16_t ink, uint16_t paper);
void TFT_WriteCharScale(uint8_t c, uint16_t x, uint16_t y, uint16_t ink, uint16_t paper);
void TFT_ConstText(const char* buffer, uint16_t x, uint16_t y, uint16_t ink, uint16_t paper);
void TFT_ConstTextScale(const char* buffer, uint16_t x, uint16_t y, uint16_t ink, uint16_t paper);
void TFT_Text(const char* buffer, uint16_t x, uint16_t y, uint16_t ink, uint16_t paper);
void TFT_TextLEFT(int8_t *buffer, uint16_t y, uint16_t ink, uint16_t paper);
void TFT_TextCENTER(const char* buffer, uint16_t y, uint16_t ink, uint16_t paper);
void TFT_TextScaledCENTER(const char* buffer, uint16_t y, uint16_t ink, uint16_t paper);
void TFT_TextRIGHT(const char *buffer, uint16_t y, uint16_t ink, uint16_t paper);
void TFT_TextLEFT2(int8_t *buffer, uint16_t y, uint16_t ink, uint16_t paper, uint16_t color3);
void TFT_TextCENTER2(const char *buffer, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t ink, uint16_t paper);
void TFT_TextEncrypted(int8_t *buffer, uint16_t x, uint16_t y, int8_t encr, int8_t ndigits, uint16_t ink, uint16_t paper);
void TFT_FillScreen   (uint16_t color);
void TFT_FillScreenDMA(uint16_t color);
void TFT_Box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT_Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT_RectangleRound(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, uint16_t color);
void TFT_RectangleRoundFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, uint16_t color);
void TFT_Circle(uint16_t x1, uint16_t y1, uint16_t radius, uint16_t color);
void TFT_CircleFill(uint16_t x1, uint16_t y1, uint16_t radius, uint16_t color);
void TFT_RoundEdge(uint16_t x1, uint16_t y1, uint16_t alignment, uint16_t radius, uint16_t color);
void TFT_Icon   (const uint16_t *buffer, uint16_t x, uint16_t y, uint16_t width_, uint16_t height_);
void TFT_IconDMA(const uint16_t *buffer, uint16_t x, uint16_t y, uint16_t width_, uint16_t height_);
void TFT_IconTransparent(const uint16_t *buffer, uint16_t x, uint16_t y, uint8_t width_, uint8_t height_, uint16_t color);
#endif	/* DISPTFT_H */
