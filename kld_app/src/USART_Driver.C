
#include "public.h"


u32 g_canbox_baudrate = 38400;


//不使用中断发送
ext void USART_TX(void)
{
	uint16_t timeout;
	USART_Tx_Ptr=USART_Tx_Buff;
	USART_Tx_Length_Bak=USART_Tx_Length;
	while(USART_Tx_Length>0)
	{
		for (timeout=400; timeout>0; timeout--) {
			CLEAR_WATCHDOG;
			if (RESET != USART_GetFlagStatus(CAN_COMM_UART, USART_FLAG_TXC)) {
				break;
			}
		}
		USART_SendData(CAN_COMM_UART, *USART_Tx_Ptr);

		USART_Tx_Ptr++;
		USART_Tx_Length--;
	}

	//Do not resend here. we let upper layer to decide when to resend
	//Usart_Resend_Timer=T150MS_4;
}

//host to can adapter
//ext void USART_ACK(uchar ack_type)
//{
//	USART_SendData(CAN_COMM_UART, ack_type);
//	while(RESET == USART_GetFlagStatus(CAN_COMM_UART, USART_FLAG_TXC));
//}


ext void Usart_Resend_Time(void)	// 4ms 时基
{
	if(Usart_Resend_Timer!=0)
	{
		Usart_Resend_Timer--;
		if(Usart_Resend_Timer==0)
		{
			Usart_Resend_Counter++;
			if(Usart_Resend_Counter>3)//已重发3次
			{	//重发3次后，丢弃
				Usart_Resend_Counter=0;
				Usart_Resend_Timer = 0;
				F_Usart_Tx_Full=0;
			}
			else
			{	
				USART_Tx_Ptr=USART_Tx_Buff;
				USART_Tx_Length=USART_Tx_Length_Bak;
				if(USART_Tx_Length<=MAX_USART_TX_BUFFER_LENGTH)
				{
					USART_TX();
				}
			}
		
		}
	}
}

void USART_RxTx_Init(bool enable)
{
	USART_InitType USART_InitStructure;
	GPIO_InitType  GPIO_InitStructure;

	USART_DeInit(CAN_COMM_UART);
	USART_InitStructure.BaudRate            = g_canbox_baudrate;
	USART_InitStructure.WordLength          = USART_WL_8B;
	USART_InitStructure.StopBits            = USART_STPB_1;
	USART_InitStructure.Parity              = USART_PE_NO;
	USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
	USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

	GPIO_InitStruct(&GPIO_InitStructure);

	if (enable) {
		TIM_ConfigInt(TIMER_IR_RX, TIM_INT_CC4, DISABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
		GPIO_InitStructure.Pin = GPIO_CAN_UART_RX_PIN;
		GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_USART2;
		GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;
		GPIO_InitPeripheral(GPIO_CAN_UART_RX_GRP, &GPIO_InitStructure);

		USART_Init(CAN_COMM_UART, &USART_InitStructure);
		USART_ConfigInt(CAN_COMM_UART, USART_INT_RXDNE, ENABLE);
		USART_Enable(CAN_COMM_UART, ENABLE);
		g_canbox_use_uart = TRUE;
	} else {
		USART_Init(CAN_COMM_UART, &USART_InitStructure);
		USART_ConfigInt(CAN_COMM_UART, USART_INT_RXDNE, DISABLE);
		USART_Enable(CAN_COMM_UART, DISABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Input;
		GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
		GPIO_InitStructure.Pin = GPIO_CAN_UART_RX_PIN;
		GPIO_InitStructure.GPIO_Alternate = GPIO_AF7_TIM5;
		GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;
		GPIO_InitPeripheral(GPIO_CAN_UART_RX_GRP, &GPIO_InitStructure);

		TIM_ConfigInt(TIMER_IR_RX, TIM_INT_CC4, ENABLE);
		g_canbox_use_uart = FALSE;
	}

	canbox_init();

	USART_Tx_Ptr=USART_Tx_Buff;
	F_Usart_Rx_Data_Ready=0;
}

