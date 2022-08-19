#include"public.h"

static CAN_HEAD_TYPE g_head_type = CAN_HEAD_0;
static CAN_LEN_TYPE g_len_type = CAN_LEN_0;
static CAN_PARITY_TYPE g_parity_type = CAN_PARITY_0;
static CAN_ACK_TYPE g_ack_type = CAN_ACK_1;
static CAN_RX_STATE g_rx_state = CAN_RX_HEAD;
static uchar can_packet_len = 0;
static uchar can_data_len = 0;

static uint8_t g_tv_sent = 1;
static uint8_t g_tv_data;

// some utility
#define IS_1BYTES_HEAD	((CAN_HEAD_0==g_head_type)||(CAN_HEAD_1==g_head_type)|| \
							(CAN_HEAD_4==g_head_type)||(CAN_HEAD_5==g_head_type))
#define IS_2BYTES_HEAD	((CAN_HEAD_2==g_head_type)||(CAN_HEAD_3==g_head_type))
#define IS_OFFSET1_LEN	((CAN_LEN_1==g_len_type)||(CAN_LEN_2==g_len_type))
#define IS_OFFSET2_LEN	((CAN_LEN_0==g_len_type)||(CAN_LEN_4==g_len_type))
#define IS_2BYTES_PARITY	(CAN_PARITY_1==g_parity_type)
static uchar __canbox_get_datatype(uchar *pBuf, uchar len)
{
	uchar datatype = 0xFF;
	
	if ((IS_OFFSET1_LEN)&&(IS_1BYTES_HEAD))
		datatype = *(pBuf+2);
	else if ((IS_OFFSET2_LEN)&&(IS_1BYTES_HEAD))
		datatype = *(pBuf+1);
	else if ((IS_OFFSET2_LEN)&&(IS_2BYTES_HEAD))
		datatype = *(pBuf+3);
		
	return datatype;
}

static void __canbox_tx_tlv(uchar *pBuf, uchar len)
{
	u16 checksum;
	u8 cnt;
	USART_Tx_Length = 0;
	switch(g_head_type) {
		case CAN_HEAD_0:
			USART_Tx_Buff[USART_Tx_Length++]=0x2E;
			break;
		case CAN_HEAD_1:
			USART_Tx_Buff[USART_Tx_Length++]=0xFD;
			break;
		case CAN_HEAD_2:
			USART_Tx_Buff[USART_Tx_Length++]=0x5A;
			USART_Tx_Buff[USART_Tx_Length++]=0xA5;
			break;
		case CAN_HEAD_3:
			USART_Tx_Buff[USART_Tx_Length++]=0xAA;
			USART_Tx_Buff[USART_Tx_Length++]=0x55;
			break;
	}
	checksum = 0;
	for(cnt=0;cnt<len;cnt++) {
		USART_Tx_Buff[USART_Tx_Length++] = *(pBuf+cnt);
		checksum += *(pBuf+cnt);
	}

	switch (g_parity_type) {
		case CAN_PARITY_0:
			USART_Tx_Buff[USART_Tx_Length++]=((checksum&0xFF)^0xFF);
			break;
		case CAN_PARITY_1:
			USART_Tx_Buff[USART_Tx_Length++]=((checksum>>8)&0xFF);
			USART_Tx_Buff[USART_Tx_Length++]=((checksum)&0xFF);
			break;
		case CAN_PARITY_2:
			USART_Tx_Buff[USART_Tx_Length++]=(checksum&0xFF);
			break;
		case CAN_PARITY_3:
			USART_Tx_Buff[USART_Tx_Length++]=((checksum&0xFF)-1);
			break;
		case CAN_PARITY_4:
			USART_Tx_Buff[USART_Tx_Length++]=((checksum-1)&0xFF);
			break;
	}
	USART_TX();
}
static void __canbox_tx_packet(uchar *pBuf, uchar len)
{
	ak_memcpy(USART_Tx_Buff, pBuf, len);
	USART_Tx_Length = len;
	USART_TX();
}
static void __canbox_send_ack1(uchar ack)
{
	USART_Tx_Buff[0] = ack;
	USART_Tx_Length = 1;
	USART_TX();
}
static void __canbox_send_ack2(uchar ack)
{
	u8 buf[16];

	switch(g_len_type) {
		case CAN_LEN_0:
			if (IS_1BYTES_HEAD) {
				buf[0] = ack;
				buf[1] = 1;
			} else if (IS_2BYTES_HEAD) {
				buf[0] = 1;
				buf[1] = ack;
			} else {
				return;
			}
			break;
		case CAN_LEN_1:
			if (IS_1BYTES_HEAD) {
				buf[0]=3+(IS_2BYTES_PARITY?2:1);
				buf[1]=ack;
			} else {
				return;
			}
			break;
		default:
			return;
	}

	buf[2] = __canbox_get_datatype(USART_Rx_Buff, can_packet_len);
	__canbox_tx_tlv(buf, 3);
}

void canbox_init(void)
{
	g_rx_state = CAN_RX_HEAD;
	F_Usart_Rx_Data_Ready = 0;
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

void canbox_rx(uint8_t data)
{
#if 0
	switch(g_rx_state) {
		case CAN_RX_HEAD:
			can_packet_len = 0;
			USART_Rx_Buff[can_packet_len] = data;
			if (CAN_ACK_1 == g_ack_type) {
				// we just know a ACK came, but do nothing
				switch(data) {
					case CAN_ACK_1_OK:
					case CAN_ACK_1_CHECKSUM_ERROR:
					case CAN_ACK_1_NOT_SUPPORT:
					case CAN_ACK_1_BUSY:
						break;
					default:
						break;
				}
			}
			switch (g_head_type) {
				case CAN_HEAD_0:
					if (0x2E==data) {
						g_rx_state = (IS_OFFSET1_LEN)?CAN_RX_LEN:CAN_RX_DATATYPE;
					}
					break;
				case CAN_HEAD_1:
					if (0xFD==data) {
						g_rx_state = (IS_OFFSET1_LEN)?CAN_RX_LEN:CAN_RX_DATATYPE;
					}
					break;
				case CAN_HEAD_2:
					if (0x5A==data)
						g_rx_state = CAN_RX_HEAD2;
					break;
				case CAN_HEAD_3:
					if (0xAA==data)
						g_rx_state = CAN_RX_HEAD2;
					break;
				case CAN_HEAD_4:
					if (0xFD==data) {
						g_rx_state = CAN_RX_DATA;
						can_data_len = 4;
					} else if (0xFC==data) {
						g_rx_state = CAN_RX_LEN;
					}
					break;
				case CAN_HEAD_5:
					if (0x55==data) {
						g_rx_state = CAN_RX_DATA;
						can_data_len = 6;
					}
					break;
				default:
					break;
			}
			if (CAN_RX_HEAD!=g_rx_state) {
				++can_packet_len;
			}
			break;
		case CAN_RX_HEAD2:
			USART_Rx_Buff[can_packet_len] = data;
			switch(g_head_type) {
				case CAN_HEAD_2:
					if (0xA5==data) {
						if (IS_OFFSET2_LEN) {
							g_rx_state = CAN_RX_LEN;
						}
					}
					break;
				case CAN_HEAD_3:
					if (0x55==data) {
						if (IS_OFFSET2_LEN) {
							g_rx_state = CAN_RX_LEN;
						}
					}
					break;
				default:
					break;
			}
			if (CAN_RX_HEAD2!=g_rx_state) {
				++can_packet_len;
			} else {
				g_rx_state = CAN_RX_HEAD;
			}
			break;
		case CAN_RX_DATATYPE:
			USART_Rx_Buff[can_packet_len] = data;
			if ((IS_1BYTES_HEAD)&&(IS_OFFSET2_LEN)) {
				g_rx_state = CAN_RX_LEN;
			}
			if ((IS_1BYTES_HEAD)&&(IS_OFFSET1_LEN)) {
				g_rx_state = CAN_RX_DATA;
			}
			if ((IS_2BYTES_HEAD)&&(IS_OFFSET2_LEN)) {
				g_rx_state = CAN_RX_DATA;
			}
			if (CAN_RX_DATATYPE!=g_rx_state) {
				++can_packet_len;
			} else {
				g_rx_state = CAN_RX_HEAD;
			}
			break;
		case CAN_RX_DATATYPE2:
			break;
		case CAN_RX_LEN:
			if ( (CAN_LEN_4 != g_len_type) && (data>(MAX_USART_RX_BUFFER_LENGTH-8)) ) {
				// we will overrun buffer, ignore this packet
				g_rx_state = CAN_RX_HEAD;
				break;
			}
			USART_Rx_Buff[can_packet_len] = data;
			switch (g_len_type) {
				case CAN_LEN_0:
					can_data_len = data;
					if (IS_1BYTES_HEAD) {
						g_rx_state = CAN_RX_DATA;
					} else if (IS_2BYTES_HEAD) {
						g_rx_state = CAN_RX_DATATYPE;
					}
					break;
				case CAN_LEN_1:
					can_data_len = data-3;//exclude the datatype & length & parity
					if (IS_2BYTES_PARITY) --can_data_len;
					if (IS_1BYTES_HEAD) g_rx_state = CAN_RX_DATATYPE;
					break;
				case CAN_LEN_2:
					can_data_len = data-3;//exclude the datatype & length & parity
					g_rx_state = CAN_RX_DATATYPE;
					break;
				case CAN_LEN_4:
					can_data_len = data&0x1F;
					g_rx_state = CAN_RX_DATA;
					break;
			}
			
			if (CAN_RX_LEN!=g_rx_state) {
				++can_packet_len;
			} else {
				g_rx_state = CAN_RX_HEAD;
			}
			break;
		case CAN_RX_LEN2:
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
			if (IS_2BYTES_PARITY) {
				g_rx_state = CAN_RX_PARITY2;
			} else {
				// we recevied one whole packet
				F_Usart_Rx_Data_Ready=1;
				g_rx_state = CAN_RX_HEAD;
			}
			break;
		case CAN_RX_PARITY2:
			USART_Rx_Buff[can_packet_len] = data;
			++can_packet_len;
			if (IS_2BYTES_PARITY) {
				// we recevied one whole packet
				F_Usart_Rx_Data_Ready=1;
			}
			g_rx_state = CAN_RX_HEAD;
			break;
	}
#endif
}

ext void USART_Data_Analyse(void)
{
	u16 checksum;
	u8 idx, parity_pos;
	u8 checksum_ok;
	if (0==F_Usart_Rx_Data_Ready)
		return;
	if (0!=USART_Transmit_Rx_Buff_full)
		return;

	// check the parity
	if (IS_1BYTES_HEAD) {
		idx = 1;
	} else if (IS_2BYTES_HEAD) {
		idx = 2;
	} else {
		return;
	}
	if (IS_2BYTES_PARITY) {
		parity_pos = can_packet_len-2;
	} else {
		parity_pos = can_packet_len-1;
	}
	checksum = 0;
	for (;idx<parity_pos;idx++) {
		checksum += USART_Rx_Buff[idx];
	}
	checksum_ok = 0;
	switch(g_parity_type) {
		case CAN_PARITY_0:
			if (USART_Rx_Buff[parity_pos]==((checksum&0xFF)^0xFF))
				checksum_ok = 1;
			break;
		case CAN_PARITY_1:
			if ( (USART_Rx_Buff[parity_pos]==((checksum>>8)&0xFF)) &&
				 (USART_Rx_Buff[parity_pos+1]==((checksum)&0xFF)) )
				checksum_ok = 1;
			break;
		case CAN_PARITY_2:
			if (USART_Rx_Buff[parity_pos]==(checksum&0xFF))
				checksum_ok = 1;
			break;
		case CAN_PARITY_3:
			if (USART_Rx_Buff[parity_pos]==(((checksum&0xFF)-1)&0xFF))
				checksum_ok = 1;
			break;
		case CAN_PARITY_4:
			if (USART_Rx_Buff[parity_pos]==((checksum-1)&0xFF))
				checksum_ok = 1;
			break;
		case CAN_PARITY_5:
			if (USART_Rx_Buff[parity_pos]==0xAA)
				checksum_ok = 1;
			break;
	}

	// Do ACK
	if (checksum_ok) {
		switch(g_ack_type) {
			case CAN_ACK_1:
				__canbox_send_ack1(CAN_ACK_1_OK);
				break;
			case CAN_ACK_2:
				__canbox_send_ack2(CAN_ACK_2_OK);
				break;
		}
	} else {
		switch(g_ack_type) {
			case CAN_ACK_1:
				__canbox_send_ack1(CAN_ACK_1_CHECKSUM_ERROR);
				break;
			case CAN_ACK_2:
				__canbox_send_ack2(CAN_ACK_2_NACK);
				break;
		}
		F_Usart_Rx_Data_Ready = 0;
		return;
	}

	// Transmit the whole packet to HOST
	ak_memcpy(USART_Transmit_Rx_Buff, USART_Rx_Buff, can_packet_len);
	USART_Transmit_Rx_Buff_full = 1;
	PostEvent(WINCE_MODULE, TX_TO_GUI_TRANSMIT_CAN_INFO,can_packet_len);

	F_Usart_Rx_Data_Ready = 0;	
}

void Canbox_Main(void)
{
#if 1
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

