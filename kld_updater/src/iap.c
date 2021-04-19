
#include "n32l40x.h"
#include <stdio.h>
#include "iap.h"

static u8 g_iap_rx_state;
static u8 g_iap_rx_checksum;
static u8 g_iap_rx_buffer[IAP_RX_BUFFER_SIZE];
static u8 g_iap_rx_idx;
static u8 g_iap_rx_packet_len;
static u8 g_rx_grp_id;
//static u8 g_rx_sub_id;
static u8 g_iap_rx_ready;
static u8 g_iap_tx_sub_id;
static u8 g_iap_tx_buffer[16];
static u8 g_iap_tx_seq;
static u16 g_flash_block_id;
static u8 g_iap_state;
static u16 g_iap_image_checksum;

static uint32_t g_flash_addr;

void iap_uart_ctrl(u8 on)
{
	if (on) {
		USART_ConfigInt(USART1, USART_INT_RXDNE, ENABLE);
		USART_Enable(USART1, ENABLE);
		g_iap_rx_state = IAP_RX_STATE_HOST_ADDR;
		g_iap_rx_checksum = 0;
		g_iap_rx_idx = 0;
		g_iap_rx_ready = 0;
	} else {
		USART_Enable(USART1, DISABLE);
		USART_ConfigInt(USART1, USART_INT_RXDNE, DISABLE);
	}
}

void iap_do_erase(void)
{
	uint32_t i;
	uint32_t base_address = FMC_START_ADDR;
	uint32_t page_address = base_address;

	iap_uart_ctrl(0);

	if(FLASH_HSICLOCK_DISABLE == FLASH_ClockInit()) {
		while(1);
	}

	FLASH_Unlock();
	for(i = 0; i < TOTAL_ERASE_PAGE_NUM; i++){
		FLASH_EraseOnePage(page_address);
		page_address = page_address + FMC_PAGE_SIZE;
	}
	FLASH_Lock();

	iap_uart_ctrl(1);

	g_flash_addr = FMC_START_ADDR;
}

void iap_do_program(uint32_t addr, uint16_t len, uint8_t *data)
{
	uint32_t Address;
	uint32_t i;
	uint32_t value;

	iap_uart_ctrl(0);

	/* Unlock the Flash Bank1 Program Erase controller */
	FLASH_Unlock();

	/* Program Flash Bank1 */
	Address = addr;
	i = 0;
	while(Address < (addr + len))
	{
		value = (data[3]<<24)|(data[2]<<16)|(data[1]<<8)|(data[0]<<0);
		FLASH_ProgramWord(Address, value);
		i++;
		Address = Address + 4;
		data = data + 4;
	}

	FLASH_Lock();

	iap_uart_ctrl(1);
}

void iap_do_reset(void)
{
	NVIC_SystemReset();
}

void iap_init(void)
{
	GPIO_InitType GPIO_InitStructure_AFPP;
	NVIC_InitType NVIC_InitStructure;
	USART_InitType USART_InitStructure;

	/* USART configure */
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_USART1, ENABLE);

	GPIO_InitStruct(&GPIO_InitStructure_AFPP);
	GPIO_InitStructure_AFPP.GPIO_Mode      = GPIO_Mode_AF_PP;
	GPIO_InitStructure_AFPP.GPIO_Current = GPIO_DC_4mA;

	GPIO_InitStructure_AFPP.Pin = GPIO_PIN_9;
	GPIO_InitStructure_AFPP.GPIO_Alternate = GPIO_AF4_USART1;
	GPIO_InitStructure_AFPP.GPIO_Pull = GPIO_No_Pull;
	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure_AFPP);
	GPIO_InitStructure_AFPP.Pin = GPIO_PIN_10;
	GPIO_InitStructure_AFPP.GPIO_Alternate = GPIO_AF4_USART1;
	GPIO_InitStructure_AFPP.GPIO_Pull = GPIO_Pull_Up;
	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure_AFPP);
	
	NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_DeInit(USART1);
	USART_InitStructure.BaudRate            = 115200;
	USART_InitStructure.WordLength          = USART_WL_8B;
	USART_InitStructure.StopBits            = USART_STPB_1;
	USART_InitStructure.Parity              = USART_PE_NO;
	USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
	USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
	USART_Init(USART1, &USART_InitStructure);
	USART_ConfigInt(USART1, USART_INT_RXDNE, ENABLE);

	iap_uart_ctrl(1);
	
	/* var init */
	g_iap_rx_state = IAP_RX_STATE_HOST_ADDR;
	g_iap_rx_checksum = 0;
	g_iap_rx_idx = 0;
	g_iap_rx_ready = 0;
	g_iap_tx_sub_id = 0;
	g_iap_tx_seq = 0;
	g_flash_block_id = 0;
	g_iap_state = IAP_STATE_START;
}

void iap_rx_handler(void)
{
	u8 cnt;
	switch (g_iap_state) {
		case IAP_STATE_START:
//			g_iap_tx_sub_id = MCU_TX_SUBID_IAP_CONFIRM;
			g_iap_state = IAP_STATE_REQ_UNLOCK_KEY;
			break;
		case IAP_STATE_REQ_UNLOCK_KEY:
			g_iap_tx_sub_id = MCU_TX_SUBID_IAP_REQUEST_KEY;
			g_iap_state = IAP_STATE_WAIT_UNLOCK_KEY;
			break;
		case IAP_STATE_WAIT_UNLOCK_KEY:
			if (g_iap_rx_ready) {
				g_iap_rx_ready = 0;
				if ( MCU_RX_SUBID_IAP_UNLOCK_KEY != g_iap_rx_buffer[5] ) {
					break;
				}
				if ( 0x59 != g_iap_rx_buffer[6] ) {
					break;
				}
				if ( 0xB1 != g_iap_rx_buffer[7] ) {
					break;
				}
				// do IAP flash erasing
				iap_do_erase();
				g_iap_image_checksum = 0;
				g_iap_state = IAP_STATE_REQ_FLASH_DATA;
			}
			break;
		case IAP_STATE_REQ_FLASH_DATA:
			g_iap_tx_sub_id = MCU_TX_SUBID_IAP_REQUEST_DATA;
			g_iap_state = IAP_STATE_WAIT_FLASH_DATA;
			break;
		case IAP_STATE_WAIT_FLASH_DATA:
			if (g_iap_rx_ready) {
				g_iap_rx_ready = 0;
				if ( MCU_RX_SUBID_IAP_DATA != g_iap_rx_buffer[5] ) {
					break;
				}
				if ( (g_flash_block_id>>8) != g_iap_rx_buffer[6] ) {
					break;
				}
				if ( (g_flash_block_id&0xFF) != g_iap_rx_buffer[7] ) {
					break;
				}
				// do IAM flash programming
				iap_do_program(g_flash_addr, SIZE_OF_PACKAGE, (g_iap_rx_buffer+8));
				g_flash_addr += SIZE_OF_PACKAGE;
				
				for (cnt=0; cnt<SIZE_OF_PACKAGE; cnt++) {
					g_iap_image_checksum += g_iap_rx_buffer[8+cnt];
				}
				++g_flash_block_id;
				if (g_flash_block_id >= IAP_DATA_BLOCK_NUM) {
					g_iap_state = IAP_STATE_WAIT_CHECKSUM;
					g_iap_tx_sub_id = MCU_TX_SUBID_IAP_REQUEST_CHECKSUM;
				} else {
					g_iap_state = IAP_STATE_REQ_FLASH_DATA;
				}
			}
			break;
		case IAP_STATE_WAIT_CHECKSUM:
			if (g_iap_rx_ready) {
				g_iap_rx_ready = 0;
				if ( MCU_RX_SUBID_IAP_CHECKSUM != g_iap_rx_buffer[5] ) {
					break;
				}
				
				if (g_iap_image_checksum != ((g_iap_rx_buffer[7]<<8)|g_iap_rx_buffer[6])) {
					// checksum error, try again

					// do IAP flash erasing
					iap_do_erase();

					g_iap_image_checksum = 0;
					g_iap_state = IAP_STATE_REQ_FLASH_DATA;
					g_flash_block_id = 0;
				} else {
					g_iap_state = IAP_STATE_WAIT_RESET;
					g_iap_tx_sub_id = MCU_TX_SUBID_IAP_REQUEST_RESET;
				}
			}
			break;
		case IAP_STATE_WAIT_RESET:
			if (g_iap_rx_ready) {
				g_iap_rx_ready = 0;
				if ( MCU_RX_SUBID_IAP_RESET != g_iap_rx_buffer[5] ) {
					break;
				}
				
				// do reset
				iap_do_reset();
			}
			break;
	}
}

void iap_tx_handler(void)
{
	u8 len;
	u8 i;
	u8 checksum;
	
	if (0==g_iap_tx_sub_id) {
		return;
	}
	
	if (MCU_TX_SUBID_IAP_REQUEST_DATA==g_iap_tx_sub_id) {
		len = 9;
	} else {
		len = 7;
	}
	checksum = 0;
	
	g_iap_tx_buffer[0]=IAP_MCU_ADDR;
	g_iap_tx_buffer[1]=IAP_HOST_ADDR;
	g_iap_tx_buffer[2]=len;		
	g_iap_tx_buffer[3]=g_iap_tx_seq++;
	g_iap_tx_buffer[4]=MCU_TX_GROUPID_IAP_INFO;
	g_iap_tx_buffer[5]=g_iap_tx_sub_id;
	if (MCU_TX_SUBID_IAP_REQUEST_DATA==g_iap_tx_sub_id) {
		g_iap_tx_buffer[6] = (g_flash_block_id>>8);
		g_iap_tx_buffer[7] = (g_flash_block_id&0xFF);
	}
	for(i=0;i<len-1;i++)
	{
		checksum^=g_iap_tx_buffer[i];
	}
	g_iap_tx_buffer[len-1] = checksum;

	while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXC));
	for(i=0;i<len;i++)
	{
		USART_SendData(USART1, g_iap_tx_buffer[i]);
		while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXC));
	}
	
	g_iap_tx_sub_id = 0;
}

void iap_rx_data(u8 data)
{

	switch (g_iap_rx_state)
	{
		case IAP_RX_STATE_HOST_ADDR:
			if(data == IAP_HOST_ADDR)	
			{
				g_iap_rx_checksum=data;
				g_iap_rx_state = IAP_RX_STATE_MCU_ADDR;
				g_iap_rx_idx = 0;
				g_iap_rx_buffer[g_iap_rx_idx] = data;
				++g_iap_rx_idx;
			}
			break;
		case IAP_RX_STATE_MCU_ADDR:
			if(data == IAP_MCU_ADDR)
			{
				g_iap_rx_checksum ^= data;
				g_iap_rx_state = IAP_RX_STATE_LENGTH;
				g_iap_rx_buffer[g_iap_rx_idx] = data;
				++g_iap_rx_idx;
			}
			else if (data == IAP_HOST_ADDR)
			{
				g_iap_rx_checksum=data;
				g_iap_rx_state = IAP_RX_STATE_MCU_ADDR;
			}
			else
			{
				g_iap_rx_state = IAP_RX_STATE_HOST_ADDR;
			}
			break;
		case IAP_RX_STATE_LENGTH:
			g_iap_rx_packet_len=data;
			g_iap_rx_checksum ^= data;
			g_iap_rx_buffer[g_iap_rx_idx] = data;
			++g_iap_rx_idx;
		
			if(g_iap_rx_packet_len<=IAP_RX_BUFFER_SIZE)
				g_iap_rx_state=IAP_RX_STATE_SEQ;
			else
				g_iap_rx_state=IAP_RX_STATE_HOST_ADDR;
			break;
		case IAP_RX_STATE_SEQ:
			g_iap_rx_state = IAP_RX_STATE_GROUP_ID;
			g_iap_rx_checksum ^= data;
			g_iap_rx_buffer[g_iap_rx_idx] = data;
			++g_iap_rx_idx;
			break;
		case IAP_RX_STATE_GROUP_ID:
			g_rx_grp_id=data;
			if(g_iap_rx_packet_len<6)
				g_iap_rx_state = IAP_RX_STATE_HOST_ADDR;
			else if(g_iap_rx_packet_len == 6)
				g_iap_rx_state = IAP_RX_STATE_CHECKSUM;
			else
				g_iap_rx_state = IAP_RX_STATE_SUB_ID;
			g_iap_rx_checksum ^= data;
			g_iap_rx_buffer[g_iap_rx_idx] = data;
			++g_iap_rx_idx;
			break;
		case IAP_RX_STATE_SUB_ID:
//			g_rx_sub_id = data;
			g_iap_rx_packet_len=g_iap_rx_packet_len-7;
			if(g_iap_rx_packet_len>0)
				g_iap_rx_state = IAP_RX_STATE_DATA;
			else
				g_iap_rx_state = IAP_RX_STATE_CHECKSUM;
			g_iap_rx_checksum ^= data;
			g_iap_rx_buffer[g_iap_rx_idx] = data;
			++g_iap_rx_idx;
			break;
		case IAP_RX_STATE_DATA:
			g_iap_rx_checksum ^= data;
			g_iap_rx_buffer[g_iap_rx_idx] = data;
			++g_iap_rx_idx;
			g_iap_rx_packet_len--;  
			if(g_iap_rx_packet_len == 0)
			{
				g_iap_rx_state = IAP_RX_STATE_CHECKSUM;
			}
			break;
		case IAP_RX_STATE_CHECKSUM:
			g_iap_rx_buffer[g_iap_rx_idx] = data;
			g_iap_rx_state = IAP_RX_STATE_HOST_ADDR;
			if( g_iap_rx_checksum==data)
			{ 					
				if(g_rx_grp_id==MCU_RX_GROUPID_IAP_EVENT)
				{
					g_iap_rx_ready=1;
				} else {
					// ignore ACK or other group
					g_iap_rx_ready = 0;
				}
			} else {
				// checksum error
				g_iap_rx_ready = 0;
			}
			break;
		default:
			break;
	}
}
