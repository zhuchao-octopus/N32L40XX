
#ifndef _IAP_H_
#define _IAP_H_


typedef   signed          char s8;
typedef   signed short     int s16;
typedef   signed           int s32;

typedef unsigned          char u8;
typedef unsigned short     int u16;
typedef unsigned           int u32;

#define IAP_RX_STATE_HOST_ADDR		0
#define IAP_RX_STATE_MCU_ADDR		1
#define IAP_RX_STATE_LENGTH		2
#define IAP_RX_STATE_SEQ		3
#define IAP_RX_STATE_GROUP_ID		4
#define IAP_RX_STATE_SUB_ID		5
#define IAP_RX_STATE_DATA		6
#define IAP_RX_STATE_CHECKSUM		7

#define FMC_PAGE_SIZE		0x800
#define TOTAL_ERASE_PAGE_NUM	28	// (64-8)*1024 / 2048
#define FMC_START_ADDR		0x08000000

#define SIZE_OF_PACKAGE	64
#define IAP_RX_BUFFER_SIZE		128
#define IAP_DATA_BLOCK_NUM				896	// (64-8)*1024 / 64
#define IAP_TX_BUFFER_SIZE		11
#define LINUART_RX_COMPLETE_FLAG		0x20
#define LINUART_RX_ERROR_FLAG			0x08
#define IAP_HOST_ADDR				0xaa
#define IAP_MCU_ADDR					0x55
#define IAP_MIN_RX_LENGTH			0x07

#define IAP_STATE_START			0
#define IAP_STATE_REQ_UNLOCK_KEY	1
#define IAP_STATE_WAIT_UNLOCK_KEY	2
#define IAP_STATE_REQ_FLASH_DATA		3
#define IAP_STATE_WAIT_FLASH_DATA		4
#define IAP_STATE_WAIT_CHECKSUM			5
#define IAP_STATE_WAIT_RESET				6


#define IAP_RX_CMD_START_LEN		7
#define IAP_RX_CMD_UNLOCK_LEN		9
#define IAP_RX_CMD_DATA_LEN	73
#define IAP_RX_CMD_CHECKSUM_LEN	9
#define IAP_RX_CMD_RESET_LEN		7

#define IAP_TX_CMD_CONFIRM_LEN		7
#define IAP_TX_CMD_REQ_KEY_LEN		7
#define IAP_TX_CMD_REQ_DATA_LEN		9
#define IAP_TX_CMD_REQ_CHECKSUM_LEN		7
#define IAP_TX_CMD_REQ_RESET_LEN	7



#define MCU_TX_GROUPID_IAP_INFO 	0x07
	#define MCU_TX_SUBID_IAP_CONFIRM					0x01
	#define MCU_TX_SUBID_IAP_REQUEST_KEY			0x02
	#define MCU_TX_SUBID_IAP_REQUEST_DATA			0x03
	#define MCU_TX_SUBID_IAP_REQUEST_CHECKSUM	0x04
	#define MCU_TX_SUBID_IAP_REQUEST_RESET		0x05

#define MCU_RX_GROUPID_IAP_EVENT			0x06
	#define MCU_RX_SUBID_IAP_START				0x01
	#define MCU_RX_SUBID_IAP_UNLOCK_KEY		0x02
	#define MCU_RX_SUBID_IAP_DATA					0x03
	#define MCU_RX_SUBID_IAP_CHECKSUM			0x04
	#define MCU_RX_SUBID_IAP_RESET				0x05



void iap_init(void);
void iap_rx_handler(void);
void iap_tx_handler(void);
void iap_rx_data(u8 data);



#endif // _IAP_H_
