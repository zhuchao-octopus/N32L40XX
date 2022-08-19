#ifndef _CANBOX_H_
#define _CANBOX_H_

// About what following enum mean, pls refence to mcu-host communication protocol.docx
typedef enum
{
	CAN_HEAD_0 = 0,
	CAN_HEAD_1,
	CAN_HEAD_2,
	CAN_HEAD_3,
	CAN_HEAD_4,
	CAN_HEAD_5,
	CAN_HEAD_NUM
} CAN_HEAD_TYPE;

typedef enum
{
	CAN_LEN_0 = 0,
	CAN_LEN_1,
	CAN_LEN_2,
	CAN_LEN_3,
	CAN_LEN_4,
	CAN_LEN_NUM
} CAN_LEN_TYPE;

typedef enum
{
	CAN_PARITY_0 = 0,
	CAN_PARITY_1,
	CAN_PARITY_2,
	CAN_PARITY_3,
	CAN_PARITY_4,
	CAN_PARITY_5,
	CAN_PARITY_NUM
} CAN_PARITY_TYPE;

typedef enum
{
	CAN_ACK_0 = 0,
	CAN_ACK_1,
	CAN_ACK_2,
	CAN_ACK_NUM
} CAN_ACK_TYPE;

#define CAN_ACK_1_OK	0xFF
#define CAN_ACK_1_CHECKSUM_ERROR	0xF0
#define CAN_ACK_1_NOT_SUPPORT	0xF3
#define CAN_ACK_1_BUSY	0xFC

#define CAN_ACK_2_OK	0xFF
#define CAN_ACK_2_NACK	0xFE

typedef enum
{
	CAN_RX_HEAD,
	CAN_RX_HEAD2,
	CAN_RX_DATATYPE,
	CAN_RX_DATATYPE2,
	CAN_RX_LEN,
	CAN_RX_LEN2,
	CAN_RX_DATA,
	CAN_RX_PARITY,
	CAN_RX_PARITY2
}CAN_RX_STATE;

typedef enum
{
	CAN_NO_EVENT=0,
	CAN_EVENT_TX_DATA
}CAN_EVENT;

void canbox_init(void);
void Canbox_Main(void);
void canbox_set_protocol(
		CAN_HEAD_TYPE head_type, 
		CAN_LEN_TYPE len_type, 
		CAN_PARITY_TYPE parity_type, 
		CAN_ACK_TYPE ack_type);
void canbox_rx(uint8_t data);
void can_send_tv_code(uint8_t data);


#endif

