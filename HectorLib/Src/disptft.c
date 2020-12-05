/*
 * File:   disptft.c
 * Author: http://pic18fxx.blogspot.com 
 * 
 * Graphical display TFT. 
 * Driver: ILI9341, ST7735, ST7789, SSD1331.
 * Interface: SPI.
 * Version 3.0
 */
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "spi.h"
#include "gpio.h"
#include "disptft.h"

//==============================================================================
// Declaration of global variables.
//==============================================================================
const uint8_t *font, *font2;
uint8_t width, height, letter_spacing, dot_size = 0, frame_memory = 0, scale_font = 3; 
uint16_t tft_x = 0, tft_y = 0;

//==============================================================================
// This function initializes the display TFT
//==============================================================================
void TFT_Init(uint8_t frame_memory_, uint16_t color)
{
    LL_SPI_Enable(SPI2);
    TFT_CS_1(); TFT_DC_0(); TFT_RES_0();
    TFT_Reset();
    TFT_Driver();
    TFT_MemoryAccessControl(frame_memory_);
    TFT_FillScreen(color);
}

//==============================================================================
// This function resets the display TFT.
//==============================================================================
void TFT_Reset(void)
{
    TFT_CS_1();
    TFT_RES_0(); LL_mDelay(50);
    TFT_RES_1(); LL_mDelay(150);
}

//==============================================================================
// This function writes a command.
//==============================================================================
void TFT_WriteCommand(uint8_t command)
{
    TFT_CS_0();
    TFT_DC_0(); // When DCX = ’0’, command is selected.
    SPI_WR8(command);
    TFT_CS_1();
}

//==============================================================================
// This function writes a Parameter.
//==============================================================================
void TFT_WriteParameter(uint8_t parameter)
{
    TFT_CS_0();
    TFT_DC_1(); // When DCX = ’1’, data is selected.
    SPI_WR8(parameter);
    TFT_CS_1();
}

//==============================================================================
// This function is used to define area of frame memory where MCU can access.
// x1: Set start column address. 
// x2: Set end column address. 
// y1: Set start page address.
// y2: Set end page address.
//==============================================================================
void TFT_ColumnPage(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2)
{
    TFT_CS_0();
    TFT_DC_0(); SPI_WR8(0x2A);
    TFT_DC_1(); SPI_WR8(x1 >> 8); SPI_WR8(x1 & 0xFF); SPI_WR8(x2 >> 8); SPI_WR8(x2 & 0xFF);
    TFT_DC_0(); SPI_WR8(0x2B);
    TFT_DC_1(); SPI_WR8(y1 >> 8); SPI_WR8(y1 & 0xFF); SPI_WR8(y2 >> 8); SPI_WR8(y2 & 0xFF);
    TFT_DC_0(); SPI_WR8(0x2C);
    TFT_CS_1();
}

//==============================================================================
// This function sets the memory access control. 
//==============================================================================
void TFT_MemoryAccessControl(uint8_t frame_memory_)
{
    TFT_WriteCommand(0x36);
    TFT_WriteParameter(frame_memory_);
    switch(frame_memory_)
    {
        case TFT_VERTICAL1:
        case TFT_VERTICAL2:
        case TFT_VERTICAL1_BMP:
        case TFT_VERTICAL2_BMP:
            tft_x = TFT_W - 1; tft_y = TFT_H - 1;
            break;
        case TFT_HORIZONTAL1:
        case TFT_HORIZONTAL2:
        case TFT_HORIZONTAL1_BMP:
        case TFT_HORIZONTAL2_BMP:
            tft_x = TFT_H - 1; tft_y = TFT_W - 1;
            break;
    }

    frame_memory = frame_memory_;
}

//==============================================================================
// This function returns the memory access control. 
//==============================================================================
uint8_t TFT_ReturnMemoryAccessControl(void)
{
    return frame_memory;
}

//==============================================================================
// This function is used to convert 24 bpp color data to 5-6-5 RGB format.
// 16 bit/pixel color order (R:5-bit, G:6-bit, B:5-bit), 65,536 colors.
// 8-8-8 to to 5-6-5 conversion.
//==============================================================================
uint16_t TFT_RGBConvert(uint16_t red, uint16_t green, uint16_t blue)
{
    uint16_t color = 0;
    red = (red & 0xF8) << 8;
    green = (green & 0xFC) << 3;
    blue = (blue & 0xF8) >> 3;
    color = red | green | blue;
    return color;
}

//==============================================================================
// This function draws a pixel on TFT.
// x: x position. Valid values: 0..240 
// y: y position. Valid values: 0..320 
// color: color parameter.
//==============================================================================
void TFT_Pixel(uint16_t x, uint16_t y, uint16_t color)
{

    TFT_CS_0();
    TFT_DC_0(); SPI_WR8(0x2A);
    TFT_DC_1(); SPI_WR8(x >> 8); SPI_WR8(x & 0xFF); SPI_WR8(x >> 8); SPI_WR8(x & 0xFF);
    TFT_DC_0(); SPI_WR8(0x2B);
    TFT_DC_1(); SPI_WR8(y >> 8); SPI_WR8(y & 0xFF); SPI_WR8(y >> 8); SPI_WR8(y & 0xFF);
    TFT_DC_0(); SPI_WR8(0x2C);
    TFT_DC_1(); SPI_WR8(color >> 8); SPI_WR8(color & 0xFF);
    TFT_CS_1();

}

//==============================================================================
// This function sets the size of dot.
// size_: Size of dot. Valid values: 0, 1, 2, 3, 4, 5.
//==============================================================================
void TFT_SetDotSize(uint8_t size_)
{
    dot_size = size_;
}

//==============================================================================
// This function draws a dot on TFT.
// x: x position. Valid values: 0..240 
// y: y position. Valid values: 0..320 
// color: color parameter.
//==============================================================================
void TFT_Dot(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t i;
    switch(dot_size)
    {
        case 0:  i = 1;   break;
        case 1:  i = 9;   break;
        case 2:  i = 25;  break;
        case 3:  i = 49;  break;
        case 4:  i = 81;  break;
        case 5:  i = 121; break;
        default: i = 121; break;
    }
    TFT_ColumnPage(x - dot_size, x + dot_size, y - dot_size, y + dot_size);
    TFT_CS_0(); TFT_DC_1();
    while(i)
    {
        SPI_WR8(color >> 8);
        SPI_WR8(color & 0xFF);
        i--;
    }
    TFT_CS_1();
}

//==============================================================================
// This function sets the type of font.
// letterspacing: Letter spacing. Valid values: 1, 2, 3...
//==============================================================================
void TFT_SetFont(const uint8_t *font_, uint8_t letterspacing)
{
    font2 = font_;
    letter_spacing = letterspacing;
    height = TFT_CharHeight();
}

//==============================================================================
// This function returns the height of character. The size is determined in pixels.
//==============================================================================
uint8_t TFT_CharHeight(void)
{
    font = font2;
    font += 6;
    return *font;
}

//==============================================================================
// This function returns the width of text. The size is determined in pixels.
//==============================================================================
uint16_t TFT_TextWidth(const char *buffer)
{
    uint16_t p, text_width = 0;
    while(*buffer)
    {
        p = *buffer - 32; p = p * 4; p = p + 8;
        font = font2;
        font += p;
        text_width += *font;
        text_width += letter_spacing;
        buffer++;
    }
    text_width -= letter_spacing;
    return text_width;
}


uint16_t TFT_TextScaledWidth(const char *buffer)
{
    uint16_t p, text_width = 0;
    while(*buffer)
    {
        p = *buffer - 32; p = p * 4; p = p + 8;
        font = font2;
        font += p;
        text_width += (*font) * scale_font;
        text_width += letter_spacing;
        buffer++;
    }
    text_width -= letter_spacing;
    return text_width;
}


//==============================================================================
// This function sets the scale.
//==============================================================================
void TFT_SetScale(uint8_t sc)
{
	scale_font = sc;
}

//==============================================================================
// This function draws a character on the TFT.
// c: character to be written. 
// x: x position. Valid values: 0..240 
// y: y position. Valid values: 0..320 
// ink: Top color.
// paper: Bottom color.
//==============================================================================
void TFT_WriteChar8(uint8_t c, uint16_t x, uint16_t y, uint16_t ink, uint16_t paper)
{
    uint8_t i, j, k;
    uint16_t p;
    p = c - 32; p = p * 4; p = p + 8;
    font = font2;
    font += p;
    width = *font;
    font += 2; p = *font;
    font -= 1; p = (p << 8) | *font;
    font = font2;
    font += p;
    i = height;
    TFT_ColumnPage(x, x + width + letter_spacing - 1, y, y + height);
    TFT_CS_0(); TFT_DC_1();
    while(i)
    {
        j = width;
        while(j)
        {
            if(j>0){if(*font&0x01){SPI_WR8(ink>>8);SPI_WR8(ink&0xFF);}else{SPI_WR8(paper>>8);SPI_WR8(paper&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x02){SPI_WR8(ink>>8);SPI_WR8(ink&0xFF);}else{SPI_WR8(paper>>8);SPI_WR8(paper&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x04){SPI_WR8(ink>>8);SPI_WR8(ink&0xFF);}else{SPI_WR8(paper>>8);SPI_WR8(paper&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x08){SPI_WR8(ink>>8);SPI_WR8(ink&0xFF);}else{SPI_WR8(paper>>8);SPI_WR8(paper&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x10){SPI_WR8(ink>>8);SPI_WR8(ink&0xFF);}else{SPI_WR8(paper>>8);SPI_WR8(paper&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x20){SPI_WR8(ink>>8);SPI_WR8(ink&0xFF);}else{SPI_WR8(paper>>8);SPI_WR8(paper&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x40){SPI_WR8(ink>>8);SPI_WR8(ink&0xFF);}else{SPI_WR8(paper>>8);SPI_WR8(paper&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x80){SPI_WR8(ink>>8);SPI_WR8(ink&0xFF);}else{SPI_WR8(paper>>8);SPI_WR8(paper&0xFF);}j--;}else{font++;break;}
            font++;
        }
        for(k = 0; k < letter_spacing; k++)
        {
            SPI_WR8(paper >> 8); SPI_WR8(paper & 0xFF);
        }
        i--;
    }
    TFT_CS_1();
}

void TFT_WriteChar(uint8_t c, uint16_t x, uint16_t y, uint16_t ink, uint16_t paper)
{
    uint8_t i, j, k;
    uint16_t p;
    p = c - 32; p = p * 4; p = p + 8;
    font = font2;
    font += p;
    width = *font;
    font += 2; p = *font;
    font -= 1; p = (p << 8) | *font;
    font = font2;
    font += p;
    i = height;
    TFT_ColumnPage(x, x + width + letter_spacing - 1, y, y + height);
    LL_SPI_SetDataWidth(SPI2,LL_SPI_DATAWIDTH_16BIT );
    TFT_CS_0(); TFT_DC_1();
    while(i)
    {
        j = width;
        while(j)
        {
            if(j>0){if(*font&0x01){SPI_WR16(ink);}else{SPI_WR16(paper);}j--;}else{font++;break;}
            if(j>0){if(*font&0x02){SPI_WR16(ink);}else{SPI_WR16(paper);}j--;}else{font++;break;}
            if(j>0){if(*font&0x04){SPI_WR16(ink);}else{SPI_WR16(paper);}j--;}else{font++;break;}
            if(j>0){if(*font&0x08){SPI_WR16(ink);}else{SPI_WR16(paper);}j--;}else{font++;break;}
            if(j>0){if(*font&0x10){SPI_WR16(ink);}else{SPI_WR16(paper);}j--;}else{font++;break;}
            if(j>0){if(*font&0x20){SPI_WR16(ink);}else{SPI_WR16(paper);}j--;}else{font++;break;}
            if(j>0){if(*font&0x40){SPI_WR16(ink);}else{SPI_WR16(paper);}j--;}else{font++;break;}
            if(j>0){if(*font&0x80){SPI_WR16(ink);}else{SPI_WR16(paper);}j--;}else{font++;break;}
            font++;
        }
        for(k = 0; k < letter_spacing; k++)
        {
            SPI_WR16(paper);
        }
        i--;
    }
    TFT_CS_1();
    LL_SPI_SetDataWidth(SPI2,LL_SPI_DATAWIDTH_8BIT );
}


//==============================================================================
// This function draws a character on the TFT.
// This function does a scale depending on the function TFT_SetScale.
// c: character to be written.
// x: x position. Valid values: 0..240
// y: y position. Valid values: 0..320
// color1: Top color.
// color2: Bottom color.
//==============================================================================
void TFT_WriteCharScale(uint8_t c, uint16_t x, uint16_t y, uint16_t ink, uint16_t paper)
{

    if (scale_font == 1)
    {
        TFT_WriteChar(c, x, y, ink, paper);
        return;
    }

    uint8_t i, j, k;
    uint16_t p, tx = x;
    p = c - 32; p = p * 4; p = p + 8;
    font = font2;
    font += p;
    width = *font;
    font += 2; p = *font;
    font -= 1; p = (p << 8) | *font;
    font = font2;
    font += p;
    i = height;
    while(i)
    {
        j = width;
        while(j)
        {
            if(j>0){if(*font&0x01){TFT_Box(x,y,x+scale_font,y+scale_font,ink);}else{TFT_Box(x,y,x+scale_font,y+scale_font,paper);}j--;x+=scale_font;}else{font++;break;}
            if(j>0){if(*font&0x02){TFT_Box(x,y,x+scale_font,y+scale_font,ink);}else{TFT_Box(x,y,x+scale_font,y+scale_font,paper);}j--;x+=scale_font;}else{font++;break;}
            if(j>0){if(*font&0x04){TFT_Box(x,y,x+scale_font,y+scale_font,ink);}else{TFT_Box(x,y,x+scale_font,y+scale_font,paper);}j--;x+=scale_font;}else{font++;break;}
            if(j>0){if(*font&0x08){TFT_Box(x,y,x+scale_font,y+scale_font,ink);}else{TFT_Box(x,y,x+scale_font,y+scale_font,paper);}j--;x+=scale_font;}else{font++;break;}
            if(j>0){if(*font&0x10){TFT_Box(x,y,x+scale_font,y+scale_font,ink);}else{TFT_Box(x,y,x+scale_font,y+scale_font,paper);}j--;x+=scale_font;}else{font++;break;}
            if(j>0){if(*font&0x20){TFT_Box(x,y,x+scale_font,y+scale_font,ink);}else{TFT_Box(x,y,x+scale_font,y+scale_font,paper);}j--;x+=scale_font;}else{font++;break;}
            if(j>0){if(*font&0x40){TFT_Box(x,y,x+scale_font,y+scale_font,ink);}else{TFT_Box(x,y,x+scale_font,y+scale_font,paper);}j--;x+=scale_font;}else{font++;break;}
            if(j>0){if(*font&0x80){TFT_Box(x,y,x+scale_font,y+scale_font,ink);}else{TFT_Box(x,y,x+scale_font,y+scale_font,paper);}j--;x+=scale_font;}else{font++;break;}
            font++;
        }
        for(k = 0; k < letter_spacing; k++)
        {
            TFT_Box(x, y, x+scale_font, y+scale_font, paper);
            x += scale_font;
        }
        y += scale_font;
        x = tx;
        i--;
    }
}


//==============================================================================
// This function writes text constant on TFT.
// buffer: Pointer to read all the array.
// x: x position. Valid values: 0..240 
// y: y position. Valid values: 0..320 
// ink: Top color.
// paper: Bottom color.
//==============================================================================
void TFT_ConstText(const char* buffer, uint16_t x, uint16_t y, uint16_t ink, uint16_t paper)
{
    while(*buffer)
    {
        TFT_WriteChar(*buffer, x, y, ink, paper);
        x += width + letter_spacing;
        buffer++;
    }
}

//==============================================================================
// These functions write text using a scale.
// buffer: Pointer to read all the array.
// x: x position. Valid values: 0..240
// y: y position. Valid values: 0..320
// color1: Top color.
// color2: Bottom color.
//==============================================================================
void TFT_ConstTextScale(const char* buffer, uint16_t x, uint16_t y, uint16_t ink, uint16_t paper)
{
    while(*buffer)
    {
        TFT_WriteCharScale(*buffer, x, y, ink, paper);
        x += (width * scale_font) + (letter_spacing * scale_font);
        buffer++;
    }
}

//==============================================================================
// This function writes text variable on TFT.
// buffer: Pointer to read all the array.
// x: x position. Valid values: 0..240 
// y: y position. Valid values: 0..320 
// ink: Top color.
// paper: Bottom color.
//==============================================================================
void TFT_Text(const char* buffer, uint16_t x, uint16_t y, uint16_t ink, uint16_t paper)
{
    while(*buffer)
    {
        TFT_WriteChar(*buffer, x, y, ink, paper);
        x += width + letter_spacing;
        buffer++;
    }
}

//==============================================================================
// These functions write text with alignment. 
// buffer: Pointer to read all the array.
// y: y position. Valid values: 0..320 
// ink: Top color.
// paper: Bottom color.
//==============================================================================
void TFT_TextLEFT(int8_t *buffer, uint16_t y, uint16_t ink, uint16_t paper)
{
    uint16_t x = 0;
    while(*buffer)
    {
        TFT_WriteChar(*buffer, x, y, ink, paper);
        x += width + letter_spacing;
        buffer++;
    }
    TFT_Box(x, y, tft_x, y + height - 1, paper);
}

void TFT_TextCENTER(const char* buffer, uint16_t y, uint16_t ink, uint16_t paper)
{
    uint16_t x;
    x = TFT_TextWidth(buffer);
    x = tft_x - x;
    x = x / 2;
    TFT_Box(0, y, x, y + height - 1, paper);
    x++;
    while(*buffer)
    {
        TFT_WriteChar(*buffer, x, y, ink, paper);
        x += width + letter_spacing;
        buffer++;
    }
    TFT_Box(x, y, tft_x, y + height - 1, paper);
}


void TFT_TextScaledCENTER(const char* buffer, uint16_t y, uint16_t ink, uint16_t paper)
{
    uint16_t x;
    x = TFT_TextScaledWidth(buffer);
    x = tft_x - x;
    x = x / 2;
    TFT_Box(0, y, x, y + height - 1, paper);
    x++;
    while(*buffer)
    {
        TFT_WriteCharScale(*buffer, x, y, ink, paper);
        x += width + letter_spacing;
        buffer++;
    }
    TFT_Box(x, y, tft_x, y + height - 1, paper);
}

void TFT_TextRIGHT(const char *buffer, uint16_t y, uint16_t ink, uint16_t paper)
{
    uint16_t x;
    x = TFT_TextWidth(buffer);
    x = tft_x - x;
    TFT_Box(0, y, x, y + height - 1, paper);
    x++;
    while(*buffer)
    {
        TFT_WriteChar(*buffer, x, y, ink, paper);
        x += width + letter_spacing;
        buffer++;
    }
}

void TFT_TextLEFT2(int8_t *buffer, uint16_t y, uint16_t ink, uint16_t paper, uint16_t color3)
{
    uint16_t x = 0;
    while(*buffer)
    {
        TFT_WriteChar(*buffer, x, y, ink, paper);
        x += width + letter_spacing;
        buffer++;
    }
    TFT_Box(x, y, tft_x, y + height - 1, color3);
}

//==============================================================================
// This function writes a text in a determined area.
// x1: x coordinate of the upper left text corner.  
// y1: y coordinate of the upper left text corner.  
// x2: x coordinate of the upper left text corner. 
// y2: y coordinate of the upper left text corner.
// ink: Top color.
// paper: Bottom color.
//==============================================================================
void TFT_TextCENTER2(const char *buffer, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t ink, uint16_t paper)
{
    uint16_t x, y;
    x = x2 - x1;
    x = x - TFT_TextWidth(buffer);
    x = x / 2;
    x = x + x1;
    y = y2 - y1;
    y = y - height;
    y = y / 2;
    y = y + y1;
    y = y + 1;
    TFT_Box(x1, y, x, y + height - 1, paper);
    x++;
    while(*buffer)
    {
        TFT_WriteChar(*buffer, x, y, ink, paper);
        x += width + letter_spacing;
        buffer++;
    }
    TFT_Box(x, y, x2, y + height - 1, paper);
}

//==============================================================================
// This function writes encrypt text used for passwords.
// x: x position. Valid values: 0..240 
// y: y position. Valid values: 0..320
// encr: Encrypted enable. Valid values: 1 or 0. 
// ndigits: Total of digits. Valid values: 1....20
// ink: Top color.
// paper: Bottom color.
//==============================================================================
void TFT_TextEncrypted(int8_t *buffer, uint16_t x, uint16_t y, int8_t encr, int8_t ndigits, uint16_t ink, uint16_t paper)
{
    int8_t i = 0, j = ndigits;
    while(*buffer)
    {
        if(encr == 1)
        {
            i = '*';
        }else
        {
            i = *buffer;
        }
        TFT_WriteChar(i, x, y, ink, paper);
        x += width + letter_spacing;
        i++;
        j--;
        buffer++;
    }
    for(i = 0; i < j; i++)
    {
        TFT_WriteChar(' ', x, y, ink, paper);
        x += width + letter_spacing;
    }
}

//==============================================================================
// This function fills screen with given color.  
// color: color parameter.
//==============================================================================
void TFT_FillScreen(uint16_t color)
{
    uint8_t DH, DL;
    uint16_t i, j;
    DH = color >> 8;
    DL = color & 0xFF;
    TFT_ColumnPage(0, tft_x, 0, tft_y);
    TFT_CS_0(); TFT_DC_1();
    for(i = 0; i < TFT_H; i++)
    {
        for (j = 0; j < TFT_W; j++)
        {
            SPI_WR8(DH);
            SPI_WR8(DL);
        }
    }
    TFT_CS_1();
}


void TFT_FillScreenDMA(uint16_t color)
{
    TFT_ColumnPage(0, tft_x, 0, tft_y);
    //LL_mDelay(500);
    TFT_CS_0(); TFT_DC_1();
    SPI2_DMA_WriteConst((uint8_t*)&color, TFT_H*TFT_W);
    TFT_CS_1();
}

//==============================================================================
// This function draws a box on TFT.
// x1: x coordinate of the upper left rectangle corner. Valid values: 0..240  
// y1: y coordinate of the upper left rectangle corner. Valid values: 0..320 
// x2: x coordinate of the lower right rectangle corner. Valid values: 0..240 
// y2: y coordinate of the lower right rectangle corner. Valid values: 0..320 
// color: color parameter. 
//==============================================================================
void TFT_Box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint32_t j;

    TFT_ColumnPage(x1, x2, y1, y2);
    TFT_CS_0(); TFT_DC_1();
    LL_SPI_SetDataWidth(SPI2,LL_SPI_DATAWIDTH_16BIT );

    j  = x2 - x1 +1;
    j *= y2 - y1 +1;

    while (j--)
        SPI_WR16(color);

    TFT_CS_1();
    LL_SPI_SetDataWidth(SPI2,LL_SPI_DATAWIDTH_8BIT );
}



void TFT_Box8(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint8_t DH, DL;
    uint16_t i, j;
    DH = color >> 8;
    DL = color & 0xFF;
    TFT_ColumnPage(x1, x2, y1, y2);
    TFT_CS_0(); TFT_DC_1();
    for(i = y1; i <= y2; i++)
    {
        for (j = x1; j <= x2; j++)
        {
            SPI_WR8(DH);
            SPI_WR8(DL);
        }
    }
    TFT_CS_1();
}

//==============================================================================
// This function draws a line on TFT (Bresenham algorithm). 
// x1: x coordinate of the line start. Valid values: 0..240
// y1: y coordinate of the line start. Valid values: 0..320 
// x2: x coordinate of the line end. Valid values: 0..240 
// y2: y coordinate of the line end. Valid values: 0..320 
// color: color parameter.
//==============================================================================
void TFT_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int16_t dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int16_t dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int16_t err = (dx > dy ? dx : -dy) / 2, e2;
    for(;;)
    {
        TFT_Dot(x1, y1, color);
        if (x1 == x2 && y1 == y2)
            break;
        e2 = err;
        if(e2 >-dx)
        {
            err -= dy;
            x1 += sx;
        }
        if(e2 < dy)
        {
            err += dx;
            y1 += sy;
        }
    }
}

//==============================================================================
// This function draws a rectangle on TFT.
// x1: x coordinate of the upper left rectangle corner. Valid values: 0..240  
// y1: y coordinate of the upper left rectangle corner. Valid values: 0..320 
// x2: x coordinate of the lower right rectangle corner. Valid values: 0..240 
// y2: y coordinate of the lower right rectangle corner. Valid values: 0..320 
// color: color parameter.
//==============================================================================
void TFT_Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    TFT_Line(x1, y1, x2, y1, color);
    TFT_Line(x1, y2, x2, y2, color);
    TFT_Line(x1, y1, x1, y2, color);
    TFT_Line(x2, y1, x2, y2, color);
}

//==============================================================================
// This function draws a rounded edge rectangle on TFT.
// radius: radius of the rounded edge.
//==============================================================================
void TFT_RectangleRound(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, uint16_t color)
{
    TFT_Line(x1 + radius, y1, x2 - radius, y1, color);
    TFT_Line(x1 + radius, y2, x2 - radius, y2, color);
    TFT_Line(x1, y1 + radius, x1, y2 - radius, color);
    TFT_Line(x2, y1 + radius, x2, y2 - radius, color);
    TFT_RoundEdge(x1 + radius, y1 + radius, 3, radius, color);
    TFT_RoundEdge(x2 - radius, y1 + radius, 4, radius, color);
    TFT_RoundEdge(x1 + radius, y2 - radius, 2, radius, color);
    TFT_RoundEdge(x2 - radius, y2 - radius, 1, radius, color);
}

//==============================================================================
// This function draws a filled rounded edge rectangle on TFT.
//==============================================================================
void TFT_RectangleRoundFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, uint16_t color)
{
    TFT_Box(x1 + radius, y1, x2 - radius, y2, color);
    TFT_Box(x1, y1 + radius, x2, y2 - radius, color);
    TFT_CircleFill(x1 + radius, y1 + radius, radius, color);
    TFT_CircleFill(x2 - radius, y1 + radius, radius, color);
    TFT_CircleFill(x1 + radius, y2 - radius, radius, color);
    TFT_CircleFill(x2 - radius, y2 - radius, radius, color);
}

//==============================================================================
// This function draws a circle on TFT (Midpoint circle algorithm).
// x1: x coordinate of the circle center. Valid values: 0..240
// y1: y coordinate of the circle center. Valid values: 0..320
// radius: radius size
// color: color parameter.
//==============================================================================
void TFT_Circle(uint16_t x1, uint16_t y1, uint16_t radius, uint16_t color)
{
    int16_t x = radius, y = 0;
    int16_t radiusError = 1 - x;
    while(x >= y)
    {
        TFT_Dot(x + x1, y + y1, color);
        TFT_Dot(y + x1, x + y1, color);
        TFT_Dot(-x + x1, y + y1, color);
        TFT_Dot(-y + x1, x + y1, color);
        TFT_Dot(-x + x1, -y + y1, color);
        TFT_Dot(-y + x1, -x + y1, color);
        TFT_Dot(x + x1, -y + y1, color);
        TFT_Dot(y + x1, -x + y1, color);
        y++;
        if(radiusError<0)
        {
            radiusError += 2 * y + 1;
        }else
        {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    }
}

//==============================================================================
// This function draws a filled circle on TFT.
//==============================================================================
void TFT_CircleFill(uint16_t x1, uint16_t y1, uint16_t radius, uint16_t color)
{
    uint8_t i = dot_size;
    int16_t x = radius, y = 0;
    int16_t radiusError = 1 - x;
    dot_size = 0;
    while(x >= y)
    {
        TFT_Line(x1-x, y1+y, x1+x, y1+y, color);
        TFT_Line(x1-x, y1-y, x1+x, y1-y, color);
        TFT_Line(x1-y, y1+x, x1+y, y1+x, color);
        TFT_Line(x1-y, y1-x, x1+y, y1-x, color);
        TFT_Dot(x + x1, y + y1, color);
        TFT_Dot(y + x1, x + y1, color);
        TFT_Dot(-x + x1, y + y1, color);
        TFT_Dot(-y + x1, x + y1, color);
        TFT_Dot(-x + x1, -y + y1, color);
        TFT_Dot(-y + x1, -x + y1, color);
        TFT_Dot(x + x1, -y + y1, color);
        TFT_Dot(y + x1, -x + y1, color);
        y++;
        if(radiusError<0)
        {
            radiusError += 2 * y + 1;
        }else
        {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    }
    dot_size = i;
}

//==============================================================================
// This function is used to draw a round edge.
//==============================================================================
void TFT_RoundEdge(uint16_t x1, uint16_t y1, uint16_t alignment, uint16_t radius, uint16_t color)
{
    int16_t x = radius, y = 0;
    int16_t radiusError = 1 - x;
    while(x >= y)
    {
        switch(alignment)
        {
        case 1:
            TFT_Dot(x + x1, y + y1, color);
            TFT_Dot(y + x1, x + y1, color);
            break;
        case 2:
            TFT_Dot(-x + x1, y + y1, color);
            TFT_Dot(-y + x1, x + y1, color);
            break;
        case 3:
            TFT_Dot(-x + x1, -y + y1, color);
            TFT_Dot(-y + x1, -x + y1, color);
            break;
        case 4:
            TFT_Dot(x + x1, -y + y1, color);
            TFT_Dot(y + x1, -x + y1, color);
            break;
        }      
        y++;
        if(radiusError<0)
        {
            radiusError += 2 * y + 1;
        }else
        {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    }
}

//==============================================================================
// This function draws a image on a desired location.
// x: x position. 
// y: y position. 
// width: width of the image in pixels.
// height: height of the image in pixels.
//==============================================================================
void TFT_Icon(const uint16_t *buffer, uint16_t x, uint16_t y, uint16_t width_, uint16_t height_)
{
    uint16_t i, j;
    TFT_ColumnPage(x, x + (width_ - 1), y, y + (height_ - 1));
    TFT_CS_0(); TFT_DC_1();
    for(i = 0; i < height_; i++)
    {
        for(j = 0; j < width_; j++)
        {
            SPI_WR8(*buffer >> 8);
            SPI_WR8(*buffer & 0xFF);
            buffer++;
        }
    }
    TFT_CS_1();
}


//   work in progress!
void TFT_Icon_prepare(uint16_t x, uint16_t y, uint16_t width_, uint16_t height_)
{
    TFT_ColumnPage(x, x + (width_ - 1), y, y + (height_ - 1));
    LL_SPI_SetDataWidth(SPI2,LL_SPI_DATAWIDTH_16BIT );
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_4, LL_DMA_MEMORY_INCREMENT);
    LL_SPI_EnableDMAReq_TX(SPI2);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_4);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_4);

    TFT_CS_0(); TFT_DC_1();
}

void TFT_Icon_copy(const uint16_t *buffer, uint32_t width_)
{
    SPI2_DMA_Copy((uint8_t*)buffer, width_);
}

void TFT_Icon_End()
{
    TFT_CS_1();
    LL_SPI_SetDataWidth(SPI2,LL_SPI_DATAWIDTH_8BIT );
}

//end WIP!

void TFT_IconDMA(const uint16_t *buffer, uint16_t x, uint16_t y, uint16_t width_, uint16_t height_)
{
    TFT_ColumnPage(x, x + (width_ - 1), y, y + (height_ - 1));
    TFT_CS_0(); TFT_DC_1();
    SPI2_DMA_Copy((uint8_t*)buffer, height_ * width_);
    TFT_CS_1();
}

void TFT_IconTransparent(const uint16_t *buffer, uint16_t x, uint16_t y, uint8_t width_, uint8_t height_, uint16_t color)
{
    uint16_t i, j, color1;
    TFT_ColumnPage(x, x + (width_ - 1), y, y + (height_ - 1));
    LL_SPI_SetDataWidth(SPI2,LL_SPI_DATAWIDTH_16BIT );
    TFT_CS_0(); TFT_DC_1();
    for(i = 0; i < height_; i++)
    {
        for(j = 0; j < width_; j++)
        {
            color1 = *(buffer++);
            color1 &= color;
            SPI_WR16(color1);
        }
    }
    TFT_CS_1();
    LL_SPI_SetDataWidth(SPI2,LL_SPI_DATAWIDTH_8BIT );
}


//==============================================================================
// This function selects the driver.
//==============================================================================
void TFT_Driver(void)
{

    TFT_WriteCommand(0xCB);   // Power control A (CBh)
    TFT_WriteParameter(0x39);
    TFT_WriteParameter(0x2C);
    TFT_WriteParameter(0x00);
    TFT_WriteParameter(0x34);
    TFT_WriteParameter(0x02);

    TFT_WriteCommand(0xCF);   // Power control B (CFh)
    TFT_WriteParameter(0x00);
    TFT_WriteParameter(0XC1);
    TFT_WriteParameter(0X30);

    TFT_WriteCommand(0xE8);   // Driver timing control A (E8h)
    TFT_WriteParameter(0x85);
    TFT_WriteParameter(0x00);
    TFT_WriteParameter(0x78);

    TFT_WriteCommand(0xEA);   // Driver timing control B (EAh)
    TFT_WriteParameter(0x00);
    TFT_WriteParameter(0x00);

    TFT_WriteCommand(0xED);   // Power on sequence control (EDh)
    TFT_WriteParameter(0x64);
    TFT_WriteParameter(0x03);
    TFT_WriteParameter(0X12);
    TFT_WriteParameter(0X81);

    TFT_WriteCommand(0xF7);   // Pump ratio control (F7h)
    TFT_WriteParameter(0x20);

    TFT_WriteCommand(0xC0);   // Power Control 1 (C0h)
    TFT_WriteParameter(0x23); // VRH[5:0]

    TFT_WriteCommand(0xC1);   // Power Control 2 (C1h)
    TFT_WriteParameter(0x10); // SAP[2:0]; BT[3:0]

    TFT_WriteCommand(0xC5);   // Set the VCOMH voltage.
    TFT_WriteParameter(0x3E);
    TFT_WriteParameter(0x28);

    TFT_WriteCommand(0xC7);   // VCOM Control 2(C7h)
    TFT_WriteParameter(0x86);

    TFT_WriteCommand(0x36);   // Memory Access Control (36h)
    TFT_WriteParameter(TFT_VERTICAL1);

    TFT_WriteCommand(0x3A);   // COLMOD: Pixel Format Set (3Ah)
    TFT_WriteParameter(0x55);

    TFT_WriteCommand(0xB1);   // Frame Rate Control (In Normal Mode/Full Colors) (B1h)
    TFT_WriteParameter(0x00);
    TFT_WriteParameter(0x18);

    TFT_WriteCommand(0xB6);   // Display Function Control (B6h)
    TFT_WriteParameter(0x08);
    TFT_WriteParameter(0x82);
    TFT_WriteParameter(0x27);

    TFT_WriteCommand(0xF2);   // Enable 3G (F2h)
    TFT_WriteParameter(0x00);

    TFT_WriteCommand(0x26);   // Gamma Set (26h)
    TFT_WriteParameter(0x01);

    TFT_WriteCommand(0xE0);   //Positive Gamma Correction (E0h)
    TFT_WriteParameter(0x0F);
    TFT_WriteParameter(0x31);
    TFT_WriteParameter(0x2B);
    TFT_WriteParameter(0x0C);
    TFT_WriteParameter(0x0E);
    TFT_WriteParameter(0x08);
    TFT_WriteParameter(0x4E);
    TFT_WriteParameter(0xF1);
    TFT_WriteParameter(0x37);
    TFT_WriteParameter(0x07);
    TFT_WriteParameter(0x10);
    TFT_WriteParameter(0x03);
    TFT_WriteParameter(0x0E);
    TFT_WriteParameter(0x09);
    TFT_WriteParameter(0x00);

    TFT_WriteCommand(0XE1);   // Negative Gamma Correction (E1h)
    TFT_WriteParameter(0x00);
    TFT_WriteParameter(0x0E);
    TFT_WriteParameter(0x14);
    TFT_WriteParameter(0x03);
    TFT_WriteParameter(0x11);
    TFT_WriteParameter(0x07);
    TFT_WriteParameter(0x31);
    TFT_WriteParameter(0xC1);
    TFT_WriteParameter(0x48);
    TFT_WriteParameter(0x08);
    TFT_WriteParameter(0x0F);
    TFT_WriteParameter(0x0C);
    TFT_WriteParameter(0x31);
    TFT_WriteParameter(0x36);
    TFT_WriteParameter(0x0F);

    TFT_WriteCommand(0x11);   // Exit Sleep
    LL_mDelay(150);           // Delay of 150ms
    TFT_WriteCommand(0x29);   // Display ON (29h)

}
