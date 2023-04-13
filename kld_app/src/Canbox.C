#include"public.h"

static CAN_HEAD_TYPE g_head_type = CAN_HEAD_0;
static CAN_LEN_TYPE g_len_type = CAN_LEN_0;
static CAN_PARITY_TYPE g_parity_type = CAN_PARITY_0;
static CAN_ACK_TYPE g_ack_type = CAN_ACK_1;
static CAN_RX_STATE g_rx_state = CAN_RX_HEAD;
static uchar can_packet_len = 0;
#ifdef CAN_RX_DOUBLE_BUF
static uchar can_packet_len_2 = 0;
#endif
static uchar can_data_len = 0;
static bool can_passthrough = FALSE;

#define CAN_RX_PT_BUF_LEN  16
static u8 g_can_rx_pt_buf[CAN_RX_PT_BUF_LEN];
volatile u8 *g_can_rx_pt_rd;
volatile u8 *g_can_rx_pt_wr;

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
#ifdef CAN_RX_DOUBLE_BUF
static void __canbox_send_ack2(uchar ack, uchar using_buf2)
#else
static void __canbox_send_ack2(uchar ack)
#endif
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

#ifdef CAN_RX_DOUBLE_BUF
	if (0!=using_buf2) {
		buf[2] = __canbox_get_datatype(USART_Rx_Buff_2, can_packet_len_2);
	} else {
		buf[2] = __canbox_get_datatype(USART_Rx_Buff, can_packet_len);
	}
#else
	buf[2] = __canbox_get_datatype(USART_Rx_Buff, can_packet_len);
#endif
	__canbox_tx_tlv(buf, 3);
}

void canbox_init(void)
{
	g_rx_state = CAN_RX_HEAD;
	F_Usart_Rx_Data_Ready = 0;
#ifdef CAN_RX_DOUBLE_BUF
	F_Usart_Rx_Data_Ready_2 = 0;
	g_canbox_rx_using_buf2 = 0;
#endif
}

void canbox_set_protocol(
		CAN_HEAD_TYPE head_type, 
		CAN_LEN_TYPE len_type, 
		CAN_PARITY_TYPE parity_type, 
		CAN_ACK_TYPE ack_type)
{
	can_passthrough = FALSE;
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
#ifdef CAN_RX_DOUBLE_BUF
		F_Usart_Rx_Data_Ready_2 = 0;
		g_canbox_rx_using_buf2 = 0;
#endif
	}
}

void canbox_passthrough(bool en)
{
	can_passthrough = en;
	g_rx_state = CAN_RX_HEAD;
	F_Usart_Rx_Data_Ready = 0;
	g_can_rx_pt_rd = g_can_rx_pt_buf;
	g_can_rx_pt_wr = g_can_rx_pt_buf;
}

void canbox_rx(uint8_t data)
{

	
#ifdef CAN_RX_DOUBLE_BUF
	uchar *buf = USART_Rx_Buff;
	uchar *len = &can_packet_len;
	if ( (0!=F_Usart_Rx_Data_Ready) || (0!=g_canbox_rx_using_buf2) ) {
		buf = USART_Rx_Buff_2;
		len = &can_packet_len_2;
		g_canbox_rx_using_buf2 = 1;
	}
#endif
	
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
#ifdef CAN_RX_DOUBLE_BUF
			*len = 0;
			buf[*len] = data;
#else
			can_packet_len = 0;
			USART_Rx_Buff[can_packet_len] = data;
#endif
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
#ifdef CAN_RX_DOUBLE_BUF
				*len = *len + 1;
#else
				++can_packet_len;
#endif
			}
			break;
		case CAN_RX_HEAD2:
#ifdef CAN_RX_DOUBLE_BUF
			buf[*len] = data;
#else
			USART_Rx_Buff[can_packet_len] = data;
#endif
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
#ifdef CAN_RX_DOUBLE_BUF
				*len = *len + 1;
#else
				++can_packet_len;
#endif
			} else {
				g_rx_state = CAN_RX_HEAD;
#ifdef CAN_RX_DOUBLE_BUF
				g_canbox_rx_using_buf2 = 0;
#endif
			}
			break;
		case CAN_RX_DATATYPE:
#ifdef CAN_RX_DOUBLE_BUF
			buf[*len] = data;
#else
			USART_Rx_Buff[can_packet_len] = data;
#endif
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
#ifdef CAN_RX_DOUBLE_BUF
				*len = *len + 1;
#else
				++can_packet_len;
#endif
			} else {
				g_rx_state = CAN_RX_HEAD;
#ifdef CAN_RX_DOUBLE_BUF
				g_canbox_rx_using_buf2 = 0;
#endif
			}
			break;
		case CAN_RX_DATATYPE2:
			break;
		case CAN_RX_LEN:
			if ( (CAN_LEN_4 != g_len_type) && (data>(MAX_USART_RX_BUFFER_LENGTH-8)) ) {
				// we will overrun buffer, ignore this packet
				g_rx_state = CAN_RX_HEAD;
#ifdef CAN_RX_DOUBLE_BUF
				g_canbox_rx_using_buf2 = 0;
#endif
				break;
			}
#ifdef CAN_RX_DOUBLE_BUF
			buf[*len] = data;
#else
			USART_Rx_Buff[can_packet_len] = data;
#endif
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
#ifdef CAN_RX_DOUBLE_BUF
				*len = *len + 1;
#else
				++can_packet_len;
#endif
			} else {
				g_rx_state = CAN_RX_HEAD;
#ifdef CAN_RX_DOUBLE_BUF
				g_canbox_rx_using_buf2 = 0;
#endif
			}
			break;
		case CAN_RX_LEN2:
			break;
		case CAN_RX_DATA:
#ifdef CAN_RX_DOUBLE_BUF
			buf[*len] = data;
			*len = *len + 1;
#else
			USART_Rx_Buff[can_packet_len] = data;
			++can_packet_len;
#endif
			--can_data_len;
			if (0==can_data_len) {
				g_rx_state = CAN_RX_PARITY;
			}
#ifdef CAN_RX_DOUBLE_BUF
			if (*len>(MAX_USART_RX_BUFFER_LENGTH-8)){
#else
			if (can_packet_len>(MAX_USART_RX_BUFFER_LENGTH-8)){
#endif
				// we will overrun buffer, ignore this packet
				g_rx_state = CAN_RX_HEAD;
#ifdef CAN_RX_DOUBLE_BUF
				g_canbox_rx_using_buf2 = 0;
#endif
				break;
			}			
			break;
		case CAN_RX_PARITY:
#ifdef CAN_RX_DOUBLE_BUF
			buf[*len] = data;
			*len = *len + 1;
#else
			USART_Rx_Buff[can_packet_len] = data;
			++can_packet_len;
#endif
			if (IS_2BYTES_PARITY) {
				g_rx_state = CAN_RX_PARITY2;
			} else {
				// we recevied one whole packet
#ifdef CAN_RX_DOUBLE_BUF
				if (0!=g_canbox_rx_using_buf2) {
					F_Usart_Rx_Data_Ready_2=1;
				} else {
					F_Usart_Rx_Data_Ready=1;
				}
#else
				F_Usart_Rx_Data_Ready=1;
#endif
				g_rx_state = CAN_RX_HEAD;
#ifdef CAN_RX_DOUBLE_BUF
				g_canbox_rx_using_buf2 = 0;
#endif
			}
			break;
		case CAN_RX_PARITY2:
#ifdef CAN_RX_DOUBLE_BUF
			buf[*len] = data;
			*len = *len + 1;
#else
			USART_Rx_Buff[can_packet_len] = data;
			++can_packet_len;
#endif
			if (IS_2BYTES_PARITY) {
				// we recevied one whole packet
#ifdef CAN_RX_DOUBLE_BUF
				if (0!=g_canbox_rx_using_buf2) {
					F_Usart_Rx_Data_Ready_2=1;
				} else {
					F_Usart_Rx_Data_Ready=1;
				}
#else
				F_Usart_Rx_Data_Ready=1;
#endif
			}
			g_rx_state = CAN_RX_HEAD;
#ifdef CAN_RX_DOUBLE_BUF
			g_canbox_rx_using_buf2 = 0;
#endif
			break;
	}
}

ext void USART_Data_Analyse(void)
{
	u16 checksum;
	u8 idx, parity_pos;
	u8 checksum_ok;
	
	u8 data[1];

	u8 *buf;
	u8 len;
	
	if (can_passthrough) {
		if (g_can_rx_pt_wr == g_can_rx_pt_rd) {
			// rx data buffer empty
			return;
		}

		if (0!=USART_Transmit_Rx_Buff_full) {
			return;
		}

		if (g_can_rx_pt_wr != g_can_rx_pt_rd) {

			data[0] = *g_can_rx_pt_rd;
			g_can_rx_pt_rd++;
			if ( (g_can_rx_pt_rd - g_can_rx_pt_buf) >= CAN_RX_PT_BUF_LEN ) {
				g_can_rx_pt_rd = g_can_rx_pt_buf;
			}

			// Transmit the whole packet to HOST
			memcpy(USART_Transmit_Rx_Buff, data, 1);
			USART_Transmit_Rx_Buff_full = 1;
			PostEvent(WINCE_MODULE, TX_TO_GUI_TRANSMIT_CAN_INFO,1);

		}

		return;
	}
	
#ifdef CAN_RX_DOUBLE_BUF

	if ( (0==F_Usart_Rx_Data_Ready) && (0==F_Usart_Rx_Data_Ready_2) )
#else
	if (0==F_Usart_Rx_Data_Ready)
#endif
		return;
	if (0!=USART_Transmit_Rx_Buff_full)
		return;

#ifdef CAN_RX_DOUBLE_BUF
	if (0!=F_Usart_Rx_Data_Ready_2) {
		buf = USART_Rx_Buff_2;
		len = can_packet_len_2;
	} else {
		buf = USART_Rx_Buff;
		len = can_packet_len;
	}
#endif

	// check the parity
	if (IS_1BYTES_HEAD) {
		idx = 1;
	} else if (IS_2BYTES_HEAD) {
		idx = 2;
	} else {
		return;
	}
	if (IS_2BYTES_PARITY) {
#ifdef CAN_RX_DOUBLE_BUF
		parity_pos = len-2;
#else
		parity_pos = can_packet_len-2;
#endif
	} else {
#ifdef CAN_RX_DOUBLE_BUF
		parity_pos = len-1;
#else
		parity_pos = can_packet_len-1;
#endif
	}
	checksum = 0;
	for (;idx<parity_pos;idx++) {
#ifdef CAN_RX_DOUBLE_BUF
		checksum += buf[idx];
#else
		checksum += USART_Rx_Buff[idx];
#endif
	}
	checksum_ok = 0;
	switch(g_parity_type) {
		case CAN_PARITY_0:
#ifdef CAN_RX_DOUBLE_BUF
			if (buf[parity_pos]==((checksum&0xFF)^0xFF))
#else
			if (USART_Rx_Buff[parity_pos]==((checksum&0xFF)^0xFF))
#endif
				checksum_ok = 1;
			break;
		case CAN_PARITY_1:
#ifdef CAN_RX_DOUBLE_BUF
			if ( (buf[parity_pos]==((checksum>>8)&0xFF)) &&
				 (buf[parity_pos+1]==((checksum)&0xFF)) )
#else
			if ( (USART_Rx_Buff[parity_pos]==((checksum>>8)&0xFF)) &&
				 (USART_Rx_Buff[parity_pos+1]==((checksum)&0xFF)) )
#endif
				checksum_ok = 1;
			break;
		case CAN_PARITY_2:
#ifdef CAN_RX_DOUBLE_BUF
			if (buf[parity_pos]==(checksum&0xFF))
#else
			if (USART_Rx_Buff[parity_pos]==(checksum&0xFF))
#endif
				checksum_ok = 1;
			break;
		case CAN_PARITY_3:
#ifdef CAN_RX_DOUBLE_BUF
			if (buf[parity_pos]==(((checksum&0xFF)-1)&0xFF))
#else
			if (USART_Rx_Buff[parity_pos]==(((checksum&0xFF)-1)&0xFF))
#endif
				checksum_ok = 1;
			break;
		case CAN_PARITY_4:
#ifdef CAN_RX_DOUBLE_BUF
			if (buf[parity_pos]==((checksum-1)&0xFF))
#else
			if (USART_Rx_Buff[parity_pos]==((checksum-1)&0xFF))
#endif
				checksum_ok = 1;
			break;
		case CAN_PARITY_5:
#ifdef CAN_RX_DOUBLE_BUF
			if (buf[parity_pos]==0xAA)
#else
			if (USART_Rx_Buff[parity_pos]==0xAA)
#endif
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
#ifdef CAN_RX_DOUBLE_BUF
				__canbox_send_ack2(CAN_ACK_2_OK, (buf == USART_Rx_Buff_2));
#else
				__canbox_send_ack2(CAN_ACK_2_OK);
#endif
				break;
		}
	} else {
		switch(g_ack_type) {
			case CAN_ACK_1:
				__canbox_send_ack1(CAN_ACK_1_CHECKSUM_ERROR);
				break;
			case CAN_ACK_2:
#ifdef CAN_RX_DOUBLE_BUF
				__canbox_send_ack2(CAN_ACK_2_NACK, (buf == USART_Rx_Buff_2));
#else
				__canbox_send_ack2(CAN_ACK_2_NACK);
#endif
				break;
		}
#ifdef CAN_RX_DOUBLE_BUF
		if (buf == USART_Rx_Buff_2) {
			F_Usart_Rx_Data_Ready_2 = 0;
		} else {
			F_Usart_Rx_Data_Ready = 0;
		}
#else
		F_Usart_Rx_Data_Ready = 0;
#endif
		return;
	}

	// Transmit the whole packet to HOST
#ifdef CAN_RX_DOUBLE_BUF
	ak_memcpy(USART_Transmit_Rx_Buff, buf, len);
	USART_Transmit_Rx_Buff_full = 1;
	PostEvent(WINCE_MODULE, TX_TO_GUI_TRANSMIT_CAN_INFO,len);
	if (buf == USART_Rx_Buff_2) {
		F_Usart_Rx_Data_Ready_2 = 0;
	} else {
		F_Usart_Rx_Data_Ready = 0;
	}
#else
	ak_memcpy(USART_Transmit_Rx_Buff, USART_Rx_Buff, can_packet_len);
	USART_Transmit_Rx_Buff_full = 1;
	PostEvent(WINCE_MODULE, TX_TO_GUI_TRANSMIT_CAN_INFO,can_packet_len);

	F_Usart_Rx_Data_Ready = 0;	
#endif
}
void Canbox_Main(void)
{
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
}


