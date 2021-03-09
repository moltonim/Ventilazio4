/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <core_cm3.h>
#include "disptft.h"
#include "XPT2046.h"
#include "font.h"
#include "bitmap.h"
#include "EEProm.h"
#include "Algorithm.h"
#include "CommRequest.h"
#include "Flash2TFT_transf.h"
#include "Ventilo.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint32_t stime = 0;
volatile uint8_t USART_stime = 1;

uint8_t CommBuf[COMBUF_DIM];
int rx_pointer;
int tx_pointer;
int tx_lenbuf, rx_lenbuf;
uint8_t LedBoard_toggle = 0;
uint32_t Backlight_toggle = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//extern const unsigned char Neolo_Icon_map[];
//extern const unsigned char galaxy_320x240_2_map[];
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char        bttn[5] = {0, 0, 0, 0, 0};
uint16_t    Player[6] = {};

//extern const uint8_t img_bubble_pattern_map[];

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    int i;
    char buffer1[40];
    char reboot_ask = 0;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */
  memset(bttn, 0, sizeof(bttn));
  memset(buffer1, 0, sizeof(buffer1));
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  SysTick_Config(SystemCoreClock / 10000);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */

  LL_TIM_EnableIT_UPDATE(TIM2);
  LL_TIM_EnableCounter(TIM2);
  //LL_TIM_GenerateEvent_UPDATE(TIM2);

  LL_TIM_ClearFlag_UPDATE(TIM5);
  LL_TIM_EnableIT_UPDATE(TIM5);

//  LL_TIM_EnableCounter(TIM7);

  LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

  //RS485 direction = read
  USART1_Interrupts_Enable();
  //LL_GPIO_ResetOutputPin(RS485_DIR_GPIO_Port, RS485_DIR_Pin);
  COMM485_RX;

  // test for SPI EEPROM on SPI1
  EEPROM_DISABLE;
  LL_SPI_Enable(SPI1);
  LL_mDelay(10);

  FlashToTFT_transf_Init();

  uint8_t       data[4] = {0, 0, 0, 0};
  unsigned int  answ = 0, n = 0;

  Player[0] = 987;
  Player[1] = 100;
  Player[2] = 123;
  Player[3] = 77;
  Player[4] = 321;
  Player[5] = 456;

  //LL_SPI_TransmitData8 (SPI1, 0x9F);
  //while (1)
  i = 0;
  {
      // JEDEC-ID Read
      EEPROM_ENABLE;
      //while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
      LL_SPI_TransmitData8 (SPI1, 0x9F);
      while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
      LL_SPI_ReceiveData8 (SPI1);
      //while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
      LL_SPI_TransmitData8 (SPI1, 0x00);
      while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
      data[i++] = LL_SPI_ReceiveData8 (SPI1);
      //while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
      LL_SPI_TransmitData8 (SPI1, 0x00);
      while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
      data[i++] = LL_SPI_ReceiveData8 (SPI1);
      //while(!LL_SPI_IsActiveFlag_TXE(SPI1))    {}
      LL_SPI_TransmitData8 (SPI1, 0x00);
      while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
      data[i++] = LL_SPI_ReceiveData8 (SPI1);

      LL_mDelay(10);
      EEPROM_DISABLE;

      while (LL_SPI_IsActiveFlag_BSY(SPI1)) {}  //test!

      answ   = data[0];
      answ <<= 8;
      answ  |= data[1];
      answ <<= 8;
      answ  |= data[2];
//      answ <<= 8;      answ  |= data[3];

  }     // end JEDEC-ID Read

  //LL_SPI_Enable(SPI2);
  //XPT2046_init();

  TFT_Init(TFT_HORIZONTAL1, BLUE);
  TFT_SetFont(Courier_New_Bold_20, 1);
//  TFT_SetFont(Terminal6x8, 1);
  TFT_ConstText("Hello World!", 0, 0, BLACK, WHITE);
//  TFT_TextCENTER((char*)"Center Text :)", 40, LIGHTGREEN, BLUE);
  TFT_TextCENTER((char*)" !# ", 40, LIGHTGREEN, BLUE);

  TFT_Icon(icon1, 50, 20, 28, 36);
  for(i = 41; i > 2; i--)
  {
      sprintf(buffer1, "Version 0x%X ", answ);
      TFT_Text(buffer1, 20, 82, BLACK, WHITE);
      LL_mDelay(100);
  }

  TFT_FillScreen(BLUE);
  answ = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  int b = 20;
  uint8_t buf[100];
  memset(buf, 0, sizeof(buf));

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if(LedBoard_toggle)
    {
        LedBoard_toggle = 0;
        LL_GPIO_TogglePin(BOARD_GLED_GPIO_Port, BOARD_GLED_Pin);
    }

    n = !n;
    if (bttn[0])
    {
        //          answ = GetJedec();
        //          answ = GetStatusRegister();
        uint64_t bp = ReadBlockProtection();
        TFT_SetFont(Courier_New_Bold_20, 1);
        n = 4;
        TFT_SetScale(n);
//        sprintf(buffer1, "EEP=0x%02X%08X !", (unsigned int)(bp>>32)&0xFF, (unsigned int)bp);
        sprintf(buffer1, "123 456 789", n);
//        TFT_Text(buffer1, 20, 100, LIGHTGREEN, BLUE);
        TFT_ConstTextScale(buffer1, 10, 120, LIGHTRED, WHITE);
        TFT_SetFont(Courier_New_Bold_20, 1);
        FlashWriteDisable();
        bttn[0] = 0;
    }

    if (bttn[1])
    {
        answ = GetStatusRegister();
        TFT_FillScreenDMA(LIGHTBLUE);
        n = 2;
        TFT_SetScale(2);
//        TFT_SetFont(Terminal6x8, 0);

        n = GetConfigRegister();
        sprintf(buffer1, "S&C=0x%02X.%02X", answ, n);
//        TFT_Text(buffer1, 20, 82, BLACK, WHITE);
        TFT_ConstTextScale(buffer1, 10, 82, LIGHTRED, WHITE);
        TFT_SetFont(Courier_New_Bold_20, 1);
        LL_mDelay(10);
        bttn[1] = 0;
    }

    if (bttn[2])
    {
        memset(buf, 0, sizeof(buf));
        //          LL_GPIO_TogglePin(TFT_LIGHT_GPIO_Port, TFT_LIGHT_Pin);
        FlashReadBlock(buf, 0, 16);
        sprintf(buffer1, "Delay = %d ", answ);
//        TFT_Text(buffer1, 20, 82, RED, WHITE);


        Draw_Ventilazio(Player);

        bttn[2] = 0;
    }

    if (bttn[3])
    {
        n = 0;
        n = answ & 0x02;
        if (answ == 0 || n)
        {
            //scrivi!
            for(int n = 0; n < 20; n++)
                buf[n] =0x20+n;
            FlashWriteEnable();
            answ = GetStatusRegister();
            n = GetConfigRegister();

            //WriteALLBlockProtection();
            //              GlobalWriteProtectionUnlock();
            //SectorErase(0, 1);
            //Devo aspettare finchè non azzera, leggendone lo stato!

            answ = GetStatusRegister();
            n = GetConfigRegister();
            LL_mDelay(10);
            //FlashWriteEnable();    //is it necessary?

            //FlashPageProgram(buf, 0, 17);
            LL_mDelay(10);
            //FlashWriteDisable();
            LL_mDelay(10);
        }
        answ = GetStatusRegister();
        sprintf(buffer1, "Status = %02X - ", answ);
        TFT_Text(buffer1, 20, 82, RED, WHITE);
        bttn[3] = 0;
    }

    if (bttn[4])
    {
        TFT_FillScreenDMA(BLACK);

        FlashToTFT_Server();

        answ = GetJedec();
        sprintf(buffer1, "Jedec = %06X", answ);
        TFT_TextCENTER(buffer1, 220, WHITE, BLACK);
        bttn[4] = 0;

//        SetConfigRegister(0);
    }


    /////////////////////SERIAL COMMUNICATIONS/////////////////////////////////////////

    if (USART_stime == 0 && rx_pointer != 0)
    {
        USART_stime = 1;
        //Elaborazione messaggio ricevuto!
        if (  !CRC161(CommBuf, rx_lenbuf))
        {
            COMM485_TX;

            //gestisco il comando:

            //Solo in caso != da indirizzo interno!!!

            tx_lenbuf = HandleCommMsg(CommBuf, rx_lenbuf);
            tx_pointer = 0;

            //Trasmetto qualcosa:
            //              tx_lenbuf = 20;
            //              for (int n = 0; n < 20; n++)                  CommBuf[n] = 'A' + n;
            uint16_t crc =CRC161(CommBuf, tx_lenbuf);
            CommBuf[tx_lenbuf++] = crc>>8;
            CommBuf[tx_lenbuf++] = crc;
            LL_USART_TransmitData8(USART1, CommBuf[tx_pointer++]);
            LL_USART_EnableIT_TXE(USART1);
            if (CommBuf[2] == SYSTEM_REBOOT)
                reboot_ask = 1;
        }
        rx_pointer = 0;
        rx_lenbuf = 0;
    }

    ///////////////////////SYSTEM REBOOT///////////////////////////////////////////////

    if (reboot_ask && tx_pointer == tx_lenbuf )
    {
        LL_mDelay(30);
        __NVIC_SystemReset();
    }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_13, 195, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(120000000);
  LL_SetSystemCoreClock(120000000);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
