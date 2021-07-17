/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file n32l40x_it.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32l40x_it.h"
#include "public.h"

/** @addtogroup N32L40x_StdPeriph_Template
 * @{
 */

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles Memory Manage exception.
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles Bus Fault exception.
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles Usage Fault exception.
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles SVCall exception.
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 */
void SysTick_Handler(void)
{
	delay_decrement();
}

/******************************************************************************/
/*                 N32L40x Peripherals Interrupt Handlers                     */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_n32l40x.s).                                                 */
/******************************************************************************/


void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_LINE0) != RESET)
    {
        /* Clear the Key Button EXTI line pending bit */
        EXTI_ClrITPendBit(EXTI_LINE0);
    }
}

void USART1_IRQHandler(void)
{
	if (USART_GetIntStatus(HOST_COMM_UART, USART_INT_RXDNE) != RESET)
	{
		host_rx_data(USART_ReceiveData(HOST_COMM_UART)&0xFF);
	}
	if (USART_GetIntStatus(HOST_COMM_UART, USART_INT_OREF) != RESET)
	{
		USART_ReceiveData(HOST_COMM_UART);
	}
}

void USART2_IRQHandler(void)
{
	if (USART_GetIntStatus(CAN_COMM_UART, USART_INT_RXDNE) != RESET)
	{
		canbox_rx(USART_ReceiveData(CAN_COMM_UART)&0xFF);
	}
	if (USART_GetIntStatus(CAN_COMM_UART, USART_INT_OREF) != RESET)
	{
		USART_ReceiveData(CAN_COMM_UART);
	}
}


__IO uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
__IO uint16_t CaptureNumber   = 0;
__IO uint32_t Capture         = 0;
__IO uint32_t g_rtc_calib_timer_freq        = 0;

void TIM9_IRQHandler(void)
{
	if (TIM_GetIntStatus(TIM9, TIM_INT_CC3) == SET)
	{
		/* Clear TIM3 Capture compare interrupt pending bit */
		TIM_ClrIntPendingBit(TIM9, TIM_INT_CC3);
		if (CaptureNumber == 0)
		{
			/* Get the Input Capture value */
			IC3ReadValue1 = TIM_GetCap3(TIM9);
			if (IC3ReadValue1>0) {
				CaptureNumber = 1;
			}
		}
		else if (CaptureNumber == 1)
		{
			/* Get the Input Capture value */
			IC3ReadValue2 = TIM_GetCap3(TIM9);

			/* Capture computation */
			if (IC3ReadValue2 > IC3ReadValue1)
			{
				Capture = (IC3ReadValue2 - IC3ReadValue1);
			}
			else
			{
				Capture = ((0xFFFF - IC3ReadValue1) + IC3ReadValue2);
			}
			/* Frequency computation */
			g_rtc_calib_timer_freq      = (uint32_t)(SystemCoreClock / 2) / Capture;

			CaptureNumber = 2;
		}
	}
}

void TIM5_IRQHandler(void)
{
	if (TIM_GetIntStatus(TIM2, TIM_INT_CC2) == SET) {
		TIM_ClrIntPendingBit(TIM2, TIM_INT_CC2);
		ir_rx_handler();
	} else if (TIM_GetIntStatus(TIM2, TIM_INT_CC4) == SET) {
		TIM_ClrIntPendingBit(TIM2, TIM_INT_CC4);
		can_ir_handler();
 	} else if (TIM_GetIntStatus(TIM2, TIM_INT_CC1) == SET) {
		TIM_ClrIntPendingBit(TIM2, TIM_INT_CC1);
		Rem_Isr();
	}
}

/**
 * @brief  This function handles PPP interrupt request.
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */
