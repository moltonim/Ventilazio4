/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f2xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f2xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */
extern volatile char SPI2_TransmitComplete;
extern volatile char SPI1_ReceiveComplete;
extern volatile uint32_t stime;
extern volatile uint8_t USART_stime;
extern uint8_t LedBoard_toggle;
extern uint32_t Backlight_toggle;


/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
    stime++;
    if (!(stime%2500))
        LedBoard_toggle = 1;

    if(!(stime%Backlight_toggle))
    {
        LL_GPIO_TogglePin(TFT_LIGHT_GPIO_Port, TFT_LIGHT_Pin);
    }


  /* USER CODE END SysTick_IRQn 0 */
  /* USER CODE BEGIN SysTick_IRQn 1 */


  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F2xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f2xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */

  /* USER CODE END EXTI0_IRQn 0 */
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
    /* USER CODE BEGIN LL_EXTI_LINE_0 */
    bttn[0] = 1;
    /* USER CODE END LL_EXTI_LINE_0 */
  }
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */

  /* USER CODE END EXTI1_IRQn 0 */
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
    /* USER CODE BEGIN LL_EXTI_LINE_1 */
    bttn[1] = 1;
    /* USER CODE END LL_EXTI_LINE_1 */
  }
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */

  /* USER CODE END EXTI2_IRQn 0 */
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
    /* USER CODE BEGIN LL_EXTI_LINE_2 */
    bttn[2] = 1;
    /* USER CODE END LL_EXTI_LINE_2 */
  }
  /* USER CODE BEGIN EXTI2_IRQn 1 */

  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
    /* USER CODE BEGIN LL_EXTI_LINE_3 */
    bttn[3] = 1;
    /* USER CODE END LL_EXTI_LINE_3 */
  }
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */

  /* USER CODE END EXTI4_IRQn 0 */
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_4) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_4);
    /* USER CODE BEGIN LL_EXTI_LINE_4 */
    bttn[4] = 1;
    /* USER CODE END LL_EXTI_LINE_4 */
  }
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream4 global interrupt.
  */
void DMA1_Stream4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream4_IRQn 0 */

    if(LL_DMA_IsActiveFlag_TC4(DMA1))
      {
        LL_DMA_ClearFlag_TC4(DMA1);
        /* Call function Tranmission complete Callback */
        SPI2_TransmitComplete = 1;
      }
      else if(LL_DMA_IsActiveFlag_TE4(DMA1))
      {
        /* Call Error function */
          LL_DMA_ClearFlag_TE4(DMA1);
        //SPI2_TransferError_Callback();
      }

  /* USER CODE END DMA1_Stream4_IRQn 0 */

  /* USER CODE BEGIN DMA1_Stream4_IRQn 1 */

  /* USER CODE END DMA1_Stream4_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
    if(LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1)
    {
        LL_TIM_ClearFlag_UPDATE(TIM2);
    }
    //LL_GPIO_TogglePin(TESTPOINT_1_GPIO_Port, TESTPOINT_1_Pin);
  /* USER CODE END TIM2_IRQn 0 */
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
    static uint32_t val = 0;

    if (LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1)) // Received characters added to fifo
    {
        CommBuf[rx_pointer++] = LL_USART_ReceiveData8(USART1); // Receive the character
        //rx_pointer %= sizeof(CommBuf);

        //Lanciare timer per fine trasmissione!

//        if (LL_TIM_IsEnabledCounter(TIM5))
        {
            LL_TIM_DisableCounter(TIM5);
            val = LL_TIM_GetAutoReload(TIM5);
            LL_TIM_SetCounter(TIM5, val);
//            LL_GPIO_SetOutputPin(TESTPOINT_1_GPIO_Port, TESTPOINT_1_Pin);
//            LL_GPIO_TogglePin(TESTPOINT_1_GPIO_Port, TESTPOINT_1_Pin);
        }
        LL_TIM_EnableCounter(TIM5);

        return;
    }


    //Quest aparte da rivedere con ilTMA in TX!
    if(LL_USART_IsEnabledIT_TXE(USART1) && LL_USART_IsActiveFlag_TXE(USART1))
    {
        if (tx_pointer == (tx_lenbuf-1) )
        {
            // Disable TX interrupt
            LL_USART_DisableIT_TXE(USART1);
            // Enable TC interrupt
            LL_USART_EnableIT_TC(USART1);
        }
        LL_USART_TransmitData8(USART1, CommBuf[tx_pointer++]);
    }

    if (LL_USART_IsActiveFlag_TC(USART1) && LL_USART_IsEnabledIT_TC(USART1))
    {

            LL_USART_DisableIT_TC(USART1); // Suppress interrupt when empty
            // >>> set 485 direction to receive
            rx_pointer = 0;
            USART_stime = 1;
            COMM485_RX;
    }


  /* USER CODE END USART1_IRQn 0 */
  /* USER CODE BEGIN USART1_IRQn 1 */
  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM5_IRQn 0 */
    if(LL_TIM_IsActiveFlag_UPDATE(TIM5) == 1)
    {
        LL_TIM_ClearFlag_UPDATE(TIM5);
    }

    //LL_GPIO_TogglePin(TESTPOINT_1_GPIO_Port, TESTPOINT_1_Pin);
//    LL_GPIO_ResetOutputPin(TESTPOINT_1_GPIO_Port, TESTPOINT_1_Pin);
    //    LL_GPIO_TogglePin(TESTPOINT_1_GPIO_Port, TESTPOINT_1_Pin);

    //detect EOT of 485
    rx_lenbuf = rx_pointer;
    USART_stime = 0;
    //LL_TIM_DisableCounter(TIM5);

  /* USER CODE END TIM5_IRQn 0 */
  /* USER CODE BEGIN TIM5_IRQn 1 */

  /* USER CODE END TIM5_IRQn 1 */
}

/**
  * @brief This function handles DMA2 Stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */
    if(LL_DMA_IsActiveFlag_TC2(DMA2))
      {
        LL_DMA_ClearFlag_TC2(DMA2);
        /* Call function Transmission complete Callback */
        SPI1_ReceiveComplete = 1;
      }
      else if(LL_DMA_IsActiveFlag_TE2(DMA2))
      {
        /* Call Error function */
        LL_DMA_ClearFlag_TE2(DMA2);
      }
  /* USER CODE END DMA2_Stream2_IRQn 0 */

  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA2 Stream3 global interrupt.
  */
void DMA2_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream3_IRQn 0 */
    if(LL_DMA_IsActiveFlag_TC3(DMA2))
      {
        LL_DMA_ClearFlag_TC3(DMA2);

        /* Call function Transmission complete Callback */
        ////// TRANSMIT FINISHED (NOT USED YET)!!!
        //SPI1_ReceiveComplete = 1;
        ////// TRANSMIT FINISHED (NOT USED YET)!!!

      }
      else if(LL_DMA_IsActiveFlag_TE3(DMA2))
      {
        LL_DMA_ClearFlag_TE3(DMA2);
        /* Call Error function */
        //SPI1_TransferError_Callback();
      }
  /* USER CODE END DMA2_Stream3_IRQn 0 */

  /* USER CODE BEGIN DMA2_Stream3_IRQn 1 */

  /* USER CODE END DMA2_Stream3_IRQn 1 */
}

/* USER CODE BEGIN 1 */


/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
