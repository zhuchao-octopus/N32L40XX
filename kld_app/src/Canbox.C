#include"public.h"

static CAN_HEAD_TYPE g_head_type = CAN_HEAD_0;
static CAN_LEN_TYPE g_len_type = CAN_LEN_0;
static CAN_PARITY_TYPE g_parity_type = CAN_PARITY_0;
static CAN_ACK_TYPE g_ack_type = CAN_ACK_1;
static CAN_RX_STATE g_rx_state = CAN_RX_HEAD;
static uchar can_packet_len = 0;
static uchar can_data_len = 0;

static u8 can_passthrough = 0;
#define CAN_RX_PT_BUF_LEN  64
static u8 g_can_rx_pt_buf[CAN_RX_PT_BUF_LEN];
volatile u8 *g_can_rx_pt_rd;
volatile u8 *g_can_rx_pt_wr;

static uint8_t g_tv_sent = 1;
static uint8_t g_tv_data;

// some utility
#define IS_1BYTES_HEAD	((CAN_HEAD_0==g_head_type)||(CAN_HEAD_1==g_head_type)|| \
							(CAN_HEAD_4==g_head_type)||(CAN_HEAD_5==g_head_type))
#define IS_2BYTES_HEAD	((CAN_HEAD_2==g_head_type)||(CAN_HEAD_3==g_head_type))
#define IS_OFFSET1_LEN	((CAN_LEN_1==g_len_type)||(CAN_LEN_2==g_len_type))
#define IS_OFFSET2_LEN	((CAN_LEN_0==g_len_type)||(CAN_LEN_4==g_len_type))
#define IS_2BYTES_PARITY	(CAN_PARITY_1==g_parity_type)

static void __canbox_tx_packet(uchar *pBuf, uchar len)
{
	ak_memcpy(USART_Tx_Buff, pBuf, len);
	USART_Tx_Length = len;
	USART_TX();
}
void canbox_init(void)
{
	g_rx_state = CAN_RX_HEAD;
	F_Usart_Rx_Data_Ready = 0;
	canbox_passthrough(0);
}

void canbox_set_protocol(
		CAN_HEAD_TYPE head_type, 
		CAN_LEN_TYPE len_type, 
		CAN_PARITY_TYPE parity_type, 
		CAN_ACK_TYPE ack_type)
{
	if ((head_type != g_head_type)||
		(len_type != g_len_type)||
		(parity_type != g_parity_type)||
		(ack_type != g_ack_type)) {
		
		g_head_type = (head_type<CAN_HEAD_NUM)?head_type:g_head_type;
		g_len_type = (len_type<CAN_LEN_NUM)?len_type:g_len_type;
		g_parity_type = (parity_type<CAN_PARITY_NUM)?parity_type:g_parity_type;
		g_ack_type = (ack_type<CAN_ACK_NUM)?ack_type:g_ack_type;

		g_rx_state = CAN_RX_HEAD;
		F_Usart_Rx_Data_Ready = 0;
	}
}

void canbox_passthrough(u8 en)
{
	can_passthrough = en;
	g_rx_state = CAN_RX_HEAD;
	F_Usart_Rx_Data_Ready = 0;
	g_can_rx_pt_rd = g_can_rx_pt_buf;
	g_can_rx_pt_wr = g_can_rx_pt_buf;
	if (1==en) {
		GPIO_ResetBits(GPIO_TV_PWR_GRP, GPIO_TV_PWR_PIN);
		delay_1ms(500);
		GPIO_SetBits(GPIO_TV_PWR_GRP, GPIO_TV_PWR_PIN);
	}
}

void canbox_rx(uint8_t data)
{
	if (can_passthrough) {
		*g_can_rx_pt_wr = data;
		g_can_rx_pt_wr++;
		if ( (g_can_rx_pt_wr - g_can_rx_pt_buf) >= CAN_RX_PT_BUF_LEN ) {
			g_can_rx_pt_wr = g_can_rx_pt_buf;
		}
		return;
	}

	switch(g_rx_state) {
		case CAN_RX_HEAD:
			can_packet_len = 0;
			USART_Rx_Buff[can_packet_len] = data;
			if (0x01==data) {
				g_rx_state = CAN_RX_HEAD2;
			}
			if (CAN_RX_HEAD!=g_rx_state) {
				++can_packet_len;
			}
			break;
		case CAN_RX_HEAD2:
			USART_Rx_Buff[can_packet_len] = data;
			if (0x88==data) {
				g_rx_state = CAN_RX_DATATYPE;
			}
			if (CAN_RX_HEAD2!=g_rx_state) {
				++can_packet_len;
			} else {
				g_rx_state = CAN_RX_HEAD;
			}
			break;
		case CAN_RX_DATATYPE:
			USART_Rx_Buff[can_packet_len] = data;
			g_rx_state = CAN_RX_LEN;
			if (CAN_RX_DATATYPE!=g_rx_state) {
				++can_packet_len;
			} else {
				g_rx_state = CAN_RX_HEAD;
			}
			break;
		case CAN_RX_LEN:
			if ( data>(MAX_USART_RX_BUFFER_LENGTH-8) ) {
				// we will overrun buffer, ignore this packet
				g_rx_state = CAN_RX_HEAD;
				break;
			}
			USART_Rx_Buff[can_packet_len] = data;
			can_data_len = data;
			g_rx_state = CAN_RX_DATA;

			if (CAN_RX_LEN!=g_rx_state) {
				++can_packet_len;
			} else {
				g_rx_state = CAN_RX_HEAD;
			}
			break;
		case CAN_RX_DATA:
			USART_Rx_Buff[can_packet_len] = data;
			++can_packet_len;
			--can_data_len;
			if (0==can_data_len) {
				g_rx_state = CAN_RX_PARITY;
			}
			if (can_packet_len>(MAX_USART_RX_BUFFER_LENGTH-8)){
				// we will overrun buffer, ignore this packet
				g_rx_state = CAN_RX_HEAD;
				break;
			}			
			break;
		case CAN_RX_PARITY:
			USART_Rx_Buff[can_packet_len] = data;
			++can_packet_len;
			// we recevied one whole packet
			F_Usart_Rx_Data_Ready=1;
			g_rx_state = CAN_RX_HEAD;
			break;
	}
}

ext void USART_Data_Analyse(void)
{
	u8 data;
	u8 finish = 0;
	static u16 state = 0;

	if (can_passthrough) {
		if (g_can_rx_pt_wr == g_can_rx_pt_rd) {
			// rx data buffer empty
			return;
		}
		if (0!=USART_Transmit_Rx_Buff_full) {
			return;
		}
		while (g_can_rx_pt_wr != g_can_rx_pt_rd) {
			data = *g_can_rx_pt_rd;
			g_can_rx_pt_rd++;
			if ( (g_can_rx_pt_rd - g_can_rx_pt_buf) >= CAN_RX_PT_BUF_LEN ) {
				g_can_rx_pt_rd = g_can_rx_pt_buf;
			}
			if (0==state) {
				switch (data) {
					case 'B':
						state = 0x100;
						break;
					case 'A':
						state = 0x200;
						break;
					case 'N':
						state = 0x300;
						break;
					case 'e':
						state = 0x400;
						break;
					case 'p':
						state = 0x500;
						break;
					case 'v':
						state = 0x600;
						break;
					case 's':
						state = 0x700;
						break;
				}
				continue;
			}
			switch (state & 0xF00) {
				case 0x100:
					switch (state & 0xFF) {
						case 0:
							if ('O'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 1:
							if ('O'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 2:
							if ('T'==data) {
								state++;
								finish = 1;
							} else {
								state=0;
							}
							break;
					}
					break;
				case 0x200:
					switch (state & 0xFF) {
						case 0:
							if ('C'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 1:
							if ('K'==data) {
								state++;
								finish = 1;
							} else {
								state=0;
							}
							break;
					}
					break;
				case 0x300:
					switch (state & 0xFF) {
						case 0:
							if ('A'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 1:
							if ('K'==data) {
								state++;
								finish = 1;
							} else {
								state=0;
							}
							break;
					}
					break;
				case 0x400:
					switch (state & 0xFF) {
						case 0:
							if ('r'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 1:
							if ('r'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 2:
							if ('o'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 3:
							if ('r'==data) {
								state++;
								finish = 1;
							} else {
								state=0;
							}
							break;
					}
					break;
				case 0x500:
					switch (state & 0xFF) {
						case 0:
							if ('r'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 1:
							if ('o'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 2:
							if ('g'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 3:
							if ('r'==data) {
								state++;
								finish = 1;
							} else {
								state=0;
							}
							break;
					}
					break;
				case 0x600:
					switch (state & 0xFF) {
						case 0:
							if ('e'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 1:
							if ('r'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 2:
							if ('i'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 3:
							if ('f'==data) {
								state++;
								finish = 1;
							} else {
								state=0;
							}
							break;
					}
					break;
				case 0x700:
					switch (state & 0xFF) {
						case 0:
							if ('u'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 1:
							if ('c'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 2:
							if ('c'==data) {
								state++;
							} else {
								state=0;
							}
							break;
						case 3:
							if ('e'==data) {
								state++;
								finish = 1;
							} else {
								state=0;
							}
							break;
					}
					break;

			}
			if (0!=finish) {
				break;
			}
		}
		if (0!=finish) {
			// Transmit the whole packet to HOST
			switch (state & 0xF00) {
				case 0x100:
					ak_memcpy(USART_Transmit_Rx_Buff, "BOOT", 4);
					PostEvent(WINCE_MODULE, TX_TO_GUI_TRANSMIT_CAN_INFO,4);
					break;
				case 0x200:
					ak_memcpy(USART_Transmit_Rx_Buff, "ACK", 3);
					PostEvent(WINCE_MODULE, TX_TO_GUI_TRANSMIT_CAN_INFO,3);
					break;
				case 0x300:
					ak_memcpy(USART_Transmit_Rx_Buff, "NAK", 3);
					PostEvent(WINCE_MODULE, TX_TO_GUI_TRANSMIT_CAN_INFO,3);
					break;
				case 0x400:
					ak_memcpy(USART_Transmit_Rx_Buff, "error", 5);
					PostEvent(WINCE_MODULE, TX_TO_GUI_TRANSMIT_CAN_INFO,5);
					break;
				case 0x500:
					ak_memcpy(USART_Transmit_Rx_Buff, "program", 7);
					PostEvent(WINCE_MODULE, TX_TO_GUI_TRANSMIT_CAN_INFO,7);
					break;
				case 0x600:
					ak_memcpy(USART_Transmit_Rx_Buff, "verify", 6);
					PostEvent(WINCE_MODULE, TX_TO_GUI_TRANSMIT_CAN_INFO,6);
					break;
				case 0x700:
					ak_memcpy(USART_Transmit_Rx_Buff, "succeed", 7);
					PostEvent(WINCE_MODULE, TX_TO_GUI_TRANSMIT_CAN_INFO,7);
					break;
			}
			state=0;
		}
		USART_Transmit_Rx_Buff_full = 1;
		g_can_rx_pt_rd = g_can_rx_pt_buf;
		g_can_rx_pt_wr = g_can_rx_pt_buf;
		return;
	}


	if (0==F_Usart_Rx_Data_Ready)
		return;
	if (0!=USART_Transmit_Rx_Buff_full)
		return;

	// Transmit the whole packet to HOST
	ak_memcpy(USART_Transmit_Rx_Buff, USART_Rx_Buff, can_packet_len);
	USART_Transmit_Rx_Buff_full = 1;
	PostEvent(WINCE_MODULE, TX_TO_GUI_TRANSMIT_CAN_INFO,can_packet_len);

	F_Usart_Rx_Data_Ready = 0;	
}

void Canbox_Main(void)
{
#if 0
	if (1==g_tv_sent)
		return;
	g_tv_sent=1;

	USART_Tx_Buff[0]=0x01;
	USART_Tx_Buff[1]=0x88;
	USART_Tx_Buff[2]=0x00;
	USART_Tx_Buff[3]=0x01;
	USART_Tx_Buff[4]=g_tv_data;
	USART_Tx_Buff[5]=0x03;
	USART_Tx_Buff[6]=0x00;
	USART_Tx_Buff[7]=0x00;
	USART_Tx_Length = 8;
	USART_TX();
#else
	EVENT *nEvt;
	nEvt=GetEvent(CAN_MODULE);

	switch(nEvt->ID) {
		case CAN_EVENT_TX_DATA:
			switch (nEvt->prm) {
				case 1:
					__canbox_tx_packet(USART_Transmit_Tx_Buff,g_usart_tx_len);
					g_usart_tx_len = 0;
					break;
				case 2:
				case 3:
					__canbox_tx_packet(USART_Transmit_Tx_Buff2,g_usart_tx_len2);
					g_usart_tx_len2 = 0;
					break;
			}
			break;
			
		case CAN_NO_EVENT:
		default:
			break;
	}
#endif
}

void can_send_tv_code(uint8_t data)
{
	g_tv_sent = 0;
	g_tv_data = data;
	if (0x1E==g_tv_data) {
		g_tv_data = 0x17;
	}
}

