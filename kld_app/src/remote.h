
#ifndef _REMOTE_H_
#define _REMOTE_H_

/***********************************************************************/
//Global Variables and functions defined in REMOTE module

void ir_tx_init(void);
void Rem_Isr(void);
uchar Rem_SendDTVTouch(u8 x, u8 y);
u8 ir_send_tv_code(u8 custom1, u8 custom2, u8 cmd1, u8 cmd2);


/*
2250            // “1”数字脉冲标准宽度（2250微秒）。
250             // “1”数字脉宽误差容限。
1125            // “0”数字脉冲标准宽度（1125微秒）
250             // “0”数字脉宽误差容限。
13500           // “Start”信号脉冲标准宽度。(13500微秒)
500             // “Start”信号脉宽误差容限。
15000           // “Repeat”信号脉冲标准宽度。(11250微秒)
500             // “Repeat”信号脉宽误差容限。
10800		// 一帧遥控器信号间隔标准宽度。   (108毫秒)
*/

//########## The following define base 2uS timer ################

// “0”数字脉冲标准宽度。（1125微秒）(0.56ms:0.56ms)
#define IR_RX_ZERO_PERIOD_MIN		281	//562-281
#define IR_RX_ZERO_PERIOD_MAX		843	//562+281

// “1”数字脉冲标准宽度。（2250微秒）(0.56ms:1.68ms)
#define IR_RX_ONE_PERIOD_MIN          788	//1125-337
#define IR_RX_ONE_PERIOD_MAX         1462 //1125+337

// “Repeat”信号脉冲标准宽度。(11250微秒)(9ms:2.25ms)
#define IR_RX_REPEAT_PERIOD_MIN          5000 //5625-625
#define IR_RX_REPEAT_PERIOD_MAX         6250 //5625+625


// “Start”信号脉冲标准宽度。(13500微秒)(9ms:4.5ms)
#define IR_RX_START_PERIOD_MIN         5750 //6750-1000
#define IR_RX_START_PERIOD_MAX        7750 // 6750+1000


#define IR_SYS_CODE_1		0xF708
#define IR_SYS_CODE_2		0xFF00
#define IR_SYS_CODE_3		0x01FE

#define IR_SYS_CODE_ASUKA_TV	0xEF00

typedef enum
{
	IR_RX_STATE_IDLE,
	IR_RX_STATE_START,
	IR_RX_STATE_CUSTOM,
	IR_RX_STATE_REPEAT,
	IR_RX_STATE_NUMS
}IR_RX_STATE;

typedef struct
{
	IR_RX_STATE state;
	u16 prev_timer;
	u8 timeout;
	u8 repeat_timer;
	u8 bit_cntr;
	u16 sys_code;
	u16 key_code;
	bool key_sent;
}IR_RX_INFO;
ext IR_RX_INFO g_ir_rx_info;

typedef struct
{
	u8 ir_key_code;
	u8 uicc_key_code;
}IR_RX_KEY_MAP;

void ir_rx_handler(void);
void ir_rx_init(void);
void ir_rx_main(void);

ext IR_RX_INFO g_can_ir_info;
void can_ir_init(void);
void can_ir_handler(void);
void can_ir_main(void);

#endif	//_REMOTE_H
