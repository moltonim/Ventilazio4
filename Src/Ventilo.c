/*
 * Ventilo.c
 *
 *  Created on: 23 mag 2020
 *      Author: Massimo WS
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "font.h"
#include "disptft.h"
#include "Ventilo.h"

/************************************************************************************
 *
 *  6 (3x2) = [x, y]        Courier_New_Bold_20, scale 4 > altezza 23*4 = 92
 *  p1 | p2 | p3
 *  p4 | p5 | p6
 *
 *  4 (2x2) = [ax, ay]        Arial_36,  scale 2 > altezza 43*2 = 86
 *  p1 | p2
 *  p3 | p4
 *
 *  2 (2x1) = [px, py]        Arial_36,  scale 2 > altezza 43*2 = 86
 *  p1 | p2
 *
 *  1 [xx, yy]              Arial_36,  scale 4 > altezza 43*2 = 172
 *
 */

//Altezza arial_36              = 43
//Altezza Courier_New_Bold_20   = 23


static int yy = 20;

static int px[2] = {10, 170};
static int py[2] = {70, 70};


static int ax[4] = {0, 150, 0, 150};
static int ay[4] = {20, 20, 140, 140};

static int x[6] = {3, 106, 209, 3, 106, 209};
static int y[6] = {40, 40, 40, 140, 140, 140};

static uint16_t PlColor[6] = {  WHITE,
                                LIGHTRED,
                                LIGHTGREEN,
                                BRIGHTYELLOW,
                                BRIGHTBLUE,
                                LIGHTMAGENTA,
};

void Draw_Ventilazio(uint16_t* player)
{
    int n;
    int how;
    int pl[6] = {};
    char buf[40] = {};

    unsigned int t = 0;

    uint16_t wh;

    TFT_FillScreenDMA(BLACK);

    //calc how many != 0
    for (how = 0, n = 0; n < 6; n++)
        if (player[n])
        {
            pl[how++] = n;
        }

    switch (how)
    {
        case 0:
            //nothing to do... ?
            break;

        case 1:
            TFT_SetFont(Arial_36, 1);
            TFT_SetScale(4);
            sprintf(buf, "%d", player[pl[0]]);
            //TFT_TextScaledCENTER(buf, yy, PlColor[pl[0]], BLACK); //non va?
            wh = TFT_TextScaledWidth(buf);
            n = (320 - wh)/2;
            //n += px[0];
            TFT_ConstTextScale(buf, n, yy, PlColor[pl[0]], BLACK);
            break;

        case 2:
            TFT_SetFont(Arial_36, 1);
            TFT_SetScale(2);
            sprintf(buf, "%d", player[pl[0]]);
            wh = TFT_TextScaledWidth(buf);
            n = ((320/2) - wh)/2;
            n += px[0];
            TFT_ConstTextScale(buf, n, py[0], PlColor[pl[0]], BLACK);
            sprintf(buf, "%d", player[pl[1]]);
            wh = TFT_TextScaledWidth(buf);
            n = ((320/2) - wh)/2;
            n += px[1];
            TFT_ConstTextScale(buf, n, py[1], PlColor[pl[1]], BLACK);
            break;

        case 3:
        case 4:
            TFT_SetFont(Arial_36, 1);
            TFT_SetScale(2);
            for (n = 0; n < how; n++)
            {
                if (player[pl[n]] == 0)
                    continue;
                sprintf(buf, "%d", player[pl[n]]);
                wh = TFT_TextScaledWidth(buf);
                t = ((320/2) - wh)/2;
                t += ax[n];
                TFT_ConstTextScale(buf, t, ay[n], PlColor[pl[n]], BLACK);
            }
            break;

        case 5:
        case 6:
            TFT_SetFont(Courier_New_Bold_20, 0);
            TFT_SetScale(3);
            for (n = 0; n < how; n++)
            {
                if (player[pl[n]] == 0)
                    continue;
                sprintf(buf, "%d", player[pl[n]]);
                wh = TFT_TextScaledWidth(buf);
                t = ((320/3) - wh)/2;
                if ( t < 0)
                    t = 0;
                t += x[n];
                TFT_ConstTextScale(buf, t, y[n], PlColor[pl[n]], BLACK);
            }
            break;

        default:
            break;
    }

    TFT_SetScale(1);
    TFT_SetFont(Courier_New_Bold_20, 1);
}
