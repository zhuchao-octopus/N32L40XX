#ifndef _USART_DRIVER_H_
#define _USART_DRIVER_H_

#define USART_HEAD_CODE			0xaa

//Head code definition
#define UART_HEAD_CODE			0x2E


//Ack type definition

#define RX_ACK						0xFF
#define RX_NACK_ERR_CHECKSUM	0xF0
#define RX_NACK_NO_SUPPORT		0xF3
#define RX_NACK_BUSY				0xFC


typedef enum
{
	RX_HEAD_CODE=0,
	RX_DATA_TYPE=1,
	RX_LENGTH=2,
	RX_DATA=3,
	RX_CHECKSUM=4
}USART_RX_STATE;

typedef enum{  			/*Parameters for Control Register1*/
	USART_DEFAULT_PARAM1      =  0x00,     
	USART_DISABLE                     	=  0x20,
	USART_WORDLENGTH_SELECT 	=  0x10,
	USART_WAKEUP_MODE         	=  0x08,
	USART_PARITY_CEN          	=  0x04,
	USART_PARITY_SEL		=  0x02,
	USART_PARITY_INT_ENABLE	=  0x01
}USART_CR1_TYPE;

typedef enum {                             /*Parameters for Control Register2*/
	USART_TRANSMITTER_INT_ENABLE	       	=  0x80, 
	USART_TRANSMITTER_COMPLETE_INT_ENABLE 	=  0x40, 
	USART_RECEIVE_INT_ENABLE         		=  0x20, 
	USART_IDLE_INT_ENABLE 		  	=  0x10, 
	USART_TX_ENABLE				=  0x08,
	USART_RX_ENABLE				=  0x04,
	USART_RX_MUTE					=  0x02,
	USART_SEND_BREAK				=  0x01,
	USART_DEFAULT_PARAM2     			=  0x00
}USART_CR2_TYPE;

typedef enum {                             /*Parameters for Control Register3*/
	USART_DEFAULT_PARAM3     			=  0x00
}USART_CR3_TYPE;

/**************************Status Register bits*******************************/
#define USART_ErrValue 0x0f                         /*Error bits in SR register*/

typedef enum {                             /*Parameters for Status Register*/
	USART_TXE	       	=  0x80, 
	USART_TC 		=  0x40, 
	USART_RXNE         	=  0x20, 
	USART_IDLE 		=  0x10, 
	USART_OR		=  0x08,
	USART_NF		=  0x04,
	USART_FE		=  0x02,
	USART_PE		=  0x01
}USART_SR_TYPE;

#define CAN_BOX_HEAD		0xaa

#define USART_CR1_R8     ((u8)0x80) /*!< Receive Data bit 8 */
#define USART_CR1_T8     ((u8)0x40) /*!< Transmit data bit 8 */
#define USART_CR1_USARTD ((u8)0x20) /*!< USART Disable (for low power consumption) */
#define USART_CR1_M      ((u8)0x10) /*!< Word length mask */
#define USART_CR1_WAKE   ((u8)0x08) /*!< Wake-up method mask */
#define USART_CR1_PCEN   ((u8)0x04) /*!< Parity Control Enable mask */
#define USART_CR1_PS     ((u8)0x02) /*!< USART LINBreakLength mask */
#define USART_CR1_PIEN   ((u8)0x01) /*!< USART Parity Interrupt Enable mask */
#define USART_CR2_TIEN  ((u8)0x80) /*!< Transmitter Interrupt Enable mask */
#define USART_CR2_TCIEN ((u8)0x40) /*!< TransmissionComplete Interrupt Enable mask */
#define USART_CR2_RIEN  ((u8)0x20) /*!< Receiver Interrupt Enable mask */
#define USART_CR2_ILIEN ((u8)0x10) /*!< IDLE Line Interrupt Enable mask */
#define USART_CR2_TEN   ((u8)0x08) /*!< Transmitter Enable mask */
#define USART_CR2_REN   ((u8)0x04) /*!< Receiver Enable mask */
#define USART_CR2_RWU   ((u8)0x02) /*!< Receiver Wake-Up mask */
#define USART_CR2_SBK   ((u8)0x01) /*!< Send Break mask */


#define MAX_USART_RX_BUFFER_LENGTH		80
#define MAX_USART_TX_BUFFER_LENGTH		144

typedef struct 
{
	unsigned Usart_Rx_Data_Ready	:	1;
	unsigned Usart_Tx_Full	:	1;
	unsigned USART_Transmit_Rx_Buff_full	:	1;
	unsigned USART_Transmit_Rx_Buff2_full	:	1;
	unsigned :4;
}_UART_FLAG;
typedef union 
{
	  unsigned char byte;
	  _UART_FLAG  field;
}UART_FLAG;

ext UART_FLAG Uart_Flag;

#define F_Usart_Rx_Data_Ready	Uart_Flag.field.Usart_Rx_Data_Ready
#define F_Usart_Tx_Full	Uart_Flag.field.Usart_Tx_Full
#define USART_Transmit_Rx_Buff_full Uart_Flag.field.USART_Transmit_Rx_Buff_full
#define USART_Transmit_Rx_Buff2_full Uart_Flag.field.USART_Transmit_Rx_Buff2_full

ext uchar * USART_Tx_Ptr;
ext uchar USART_Tx_Length;
ext uchar USART_Tx_Buff[MAX_USART_TX_BUFFER_LENGTH];
ext uchar USART_Rx_Buff[MAX_USART_RX_BUFFER_LENGTH];


ext uchar USART_Transmit_Rx_Buff[MAX_USART_RX_BUFFER_LENGTH];
ext uchar USART_Transmit_Rx_Buff2[MAX_USART_RX_BUFFER_LENGTH];
ext uchar USART_Transmit_Tx_Buff[MAX_USART_TX_BUFFER_LENGTH];
ext uchar g_usart_tx_len;
ext uchar USART_Transmit_Tx_Buff2[MAX_USART_TX_BUFFER_LENGTH];
ext uchar g_usart_tx_len2;


ext volatile USART_RX_STATE Usart_Rx_State;

ext uchar Usart_Resend_Counter;
ext uchar Usart_Resend_Timer;
ext uchar USART_Tx_Length_Bak;
ext uchar Bak_media_type;
extern u32 g_canbox_baudrate;
ext bool g_canbox_use_uart;

#define T150MS_4	35

ext void USART_TX(void);
//ext void USART_ACK(uchar ack_type);
ext void Usart_Resend_Time(void);	// 4ms Ê±»ù

ext void USART_RxTx_Init(bool enable);
ext void USART_Data_Analyse(void);
#endif	//_USART_H
