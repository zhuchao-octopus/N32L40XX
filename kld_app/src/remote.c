
#include "public.h"

//#define SUPPORT_IR_RX_01FE
typedef enum {
     REM_IDLE,
     REM_START_H,
     REM_START_L,
     REM_SYSTEM_CUSTEM_CODE_H,
     REM_SYSTEM_CUSTEM_CODE_L,
     REM_STOP
}Def_REM;
Def_REM RemState;
uchar Rem_Data[4];

void Rem_Isr(void)
{
	volatile u16 temp;
	static u8 bitIndex;
	static u8 byteIndex;

	temp = TIM_GetCap1(TIMER_IR_RX);

	switch(RemState)
	{    
		case REM_IDLE:
			GPIO_SetBits(GPIO_DTV_IR_TX_GRP, GPIO_DTV_IR_TX_PIN);
			break;
		case REM_START_H:
			temp = TIM_GetCnt(TIMER_IR_RX);
			temp += 4500;
			GPIO_ResetBits(GPIO_DTV_IR_TX_GRP, GPIO_DTV_IR_TX_PIN);
			RemState = REM_START_L;	   
			break;
		case REM_START_L:
			temp += 2250;
			GPIO_SetBits(GPIO_DTV_IR_TX_GRP, GPIO_DTV_IR_TX_PIN);
			bitIndex = 0x01;
			byteIndex = 0;
			RemState = REM_SYSTEM_CUSTEM_CODE_H;
			break;
		case REM_SYSTEM_CUSTEM_CODE_H:
			GPIO_ResetBits(GPIO_DTV_IR_TX_GRP, GPIO_DTV_IR_TX_PIN);
			temp+=280;
			RemState = REM_SYSTEM_CUSTEM_CODE_L;
			break;
		case REM_SYSTEM_CUSTEM_CODE_L:
			GPIO_SetBits(GPIO_DTV_IR_TX_GRP, GPIO_DTV_IR_TX_PIN);
			if(bitIndex > 0 && byteIndex <= 3)
			{
				if(Rem_Data[byteIndex] & bitIndex)
					temp+=845;
				else
					temp+=282;
				bitIndex = bitIndex<<1;
				if(bitIndex > 0)
				{
					RemState = REM_SYSTEM_CUSTEM_CODE_H;    
				}
				if(bitIndex == 0)
				{
					if(byteIndex<3)
					{
						byteIndex++;
						bitIndex = 0x01;
						RemState = REM_SYSTEM_CUSTEM_CODE_H;    
					}
					else
					{
						RemState = REM_STOP;
					}                   
				}
			}
			else
			{
				temp+=282;
				RemState = REM_STOP;
			}
			break;         
		case REM_STOP:
			GPIO_ResetBits(GPIO_DTV_IR_TX_GRP, GPIO_DTV_IR_TX_PIN);
			temp+=280;
			RemState = REM_IDLE;
			break;
		default:
			break;
	}
	TIM_SetCmp1(TIMER_IR_RX, temp);
}

#define DTV_CUSTOM_CODE	0x55AA
uchar Rem_SendDTVTouch(u8 x, u8 y)
{
	if(RemState != REM_IDLE)
		 return 0;
	else
	{
		 Rem_Data[0] = LSB(DTV_CUSTOM_CODE);
		 Rem_Data[1] = MSB(DTV_CUSTOM_CODE);
		 Rem_Data[2] = x;
		 Rem_Data[3] = y;
		 RemState = REM_START_H;
		TIM_GenerateEvent(TIMER_IR_RX, TIM_EVT_SRC_CC1);
		 return 1;
	}
}

u8 ir_send_tv_code(u8 custom1, u8 custom2, u8 cmd1, u8 cmd2)
{
	if(RemState != REM_IDLE)
		 return 0;
	else
	{
		 Rem_Data[0] = custom1;
		 Rem_Data[1] = custom2;
		 Rem_Data[2] = cmd1;
		 Rem_Data[3] = cmd2;
		 RemState = REM_START_H;		  
		TIM_GenerateEvent(TIMER_IR_RX, TIM_EVT_SRC_CC1);
		 return 1;
	}
}

void ir_tx_init(void)
{
	RemState = REM_IDLE;
	GPIO_SetBits(GPIO_DTV_IR_TX_GRP, GPIO_DTV_IR_TX_PIN);
}

static const IR_RX_KEY_MAP g_ir_rx_key_map_f708[] = {
	{0x0B, UICC_FAKE_POWER_OFF},
	{0x0C, UICC_SOURCE},
	{0x54, UICC_PLAY_PAUSE},
	{0x48, UICC_EJECT},
	{0x41, UICC_RADIO},
	{0x01, UICC_UP},//UICC_DVD_UP},
	{0x02, UICC_TITLE},
	{0x03, UICC_MUTE},
	{0x4A, UICC_LEFT},//UICC_DVD_LEFT},
	{0x04, UICC_ENTER},//UICC_DVD_ENTER},
	{0x05, UICC_RIGHT},//UICC_DVD_RIGHT},
	{0x06, UICC_SETUP},
	{0x14, UICC_STOP},
	{0x07, UICC_DOWN},//UICC_DVD_DOWN},
	{0x08, UICC_ANGLE},
	{0x09, UICC_SUB_TITLE},
	{0x15, UICC_AMS_RPT},
	{0x51, UICC_ST_PROG},
	{0x52, UICC_VOLUME_UP},
	{0x00, UICC_ZOOM},
	{0x1F, UICC_LOC_RDM},
	{0x1C, UICC_PREV},
	{0x1D, UICC_EQ},
	{0x53, UICC_NEXT},
	{0x1A, UICC_PBC},
	{0x59, UICC_OSD},
	{0x40, UICC_VOLUME_DOWN},
	{0x16, UICC_MUSIC},
	{0x46, UICC_1},
	{0x19, UICC_2},
	{0x0D, UICC_3},
	{0x4B, UICC_4},
	{0x5F, UICC_5},
	{0x5B, UICC_6},
	{0x4F, UICC_7},
	{0x57, UICC_8},
	{0x5E, UICC_9},
	{0x5A, UICC_0},
	{0x4E, UICC_NUM_10_PLUS},
	{0x56, UICC_GOTO},
	{0x5D, UICC_NAVI},
	{0x55, UICC_LOUDNESS},
};

static const IR_RX_KEY_MAP g_ir_rx_key_map_00ff[] = {
	{0x00, UICC_FAKE_POWER_OFF},
	{0x01, UICC_SOURCE},
	{0x05, UICC_NEXT},
	{0x0D, UICC_PREV},
	{0x08, UICC_STEP_UP},
	{0x0A, UICC_STEP_DOWN},
	{0x15, UICC_RADIO},
	{0x09, UICC_ENTER},
	{0x04, UICC_VOLUME_UP},
	{0x06, UICC_VOLUME_DOWN},
	{0x0C, UICC_PREV},
	{0x0E, UICC_NEXT},
	{0x11, UICC_HOME},
	{0x12, UICC_BACK},
	{0x47, UICC_1},
	{0x07, UICC_2},
	{0x40, UICC_3},
	{0x57, UICC_4},
	{0x44, UICC_5},
	{0x0F, UICC_6},
	{0x54, UICC_7},
	{0x1E, UICC_8},
	{0x1F, UICC_9},
	{0x1B, UICC_0},
	{0x1A, UICC_EQ},
	{0x10, UICC_LOUDNESS},
	{0x18, UICC_MUTE},
	{0x19, UICC_NAVI},
};

#ifdef SUPPORT_IR_RX_01FE
static const IR_RX_KEY_MAP g_ir_rx_key_map_01fe[] = {
	{0x40, UICC_FAKE_POWER_OFF},
	{0x00, UICC_MUTE},
	{0x55, UICC_PREV},
	{0x5A, UICC_PLAY_PAUSE},
	{0x54, UICC_NEXT},
	{0x16, UICC_DVD_UP},
	{0x51, UICC_DVD_LEFT},
	{0x13, UICC_DVD_ENTER},
	{0x50, UICC_DVD_RIGHT},
	{0x1A, UICC_DVD_DOWN},
	{0x11, UICC_SOURCE},
	{0x4C, UICC_SETUP},
	{0x19, UICC_NAVI},
	{0x18, UICC_VOLUME_UP},
	{0x10, UICC_VOLUME_DOWN},
};
#endif

void ir_rx_init(void)
{
	g_ir_rx_info.prev_timer=0;
	g_ir_rx_info.state=IR_RX_STATE_IDLE;
	g_ir_rx_info.timeout = 0;
	g_ir_rx_info.repeat_timer = 0;
	g_ir_rx_info.bit_cntr = 0;
	g_ir_rx_info.key_code = 0;
	g_ir_rx_info.sys_code = 0;
	g_ir_rx_info.key_sent = TRUE;
}

void ir_rx_handler(void)
{
	u32 tmp_timer;

	if (!Is_Machine_Power) {
		ir_rx_init();
		return;
	}
	
	tmp_timer=g_ir_rx_info.prev_timer;
	g_ir_rx_info.prev_timer=TIM_GetCap2(TIMER_IR_RX);
	if (g_ir_rx_info.prev_timer > tmp_timer) {
		tmp_timer=g_ir_rx_info.prev_timer-tmp_timer;
	} else {
		tmp_timer = ((0xFFFF - tmp_timer) + g_ir_rx_info.prev_timer);
	}

	switch (g_ir_rx_info.state)
	{
		case IR_RX_STATE_IDLE:
			
			if (Bit_RESET == GPIO_ReadInputDataBit(GPIO_IR_RX_GRP, GPIO_IR_RX_PIN)) {
				g_ir_rx_info.timeout = T600MS_12;
				g_ir_rx_info.bit_cntr = 0;
				g_ir_rx_info.state = IR_RX_STATE_START;
			}
			break;
		case IR_RX_STATE_START:
			if (Bit_RESET == GPIO_ReadInputDataBit(GPIO_IR_RX_GRP, GPIO_IR_RX_PIN)) {
				if ( (tmp_timer>IR_RX_REPEAT_PERIOD_MIN)&&(tmp_timer<IR_RX_REPEAT_PERIOD_MAX) ) {
					bool repeat = FALSE;
					if ( 
						(g_ir_rx_info.sys_code==IR_SYS_CODE_1)	/* system code 1 */
						&&( (0xAD52 == g_ir_rx_info.key_code) ||	/* Volume + */
							(0xBF40 == g_ir_rx_info.key_code) ) 	/* Volume - */
					  )
					{
						repeat = TRUE;
					}

					if ( 
						(g_ir_rx_info.sys_code==IR_SYS_CODE_2)	/* system code 2 */
						&&( (0xFB04 == g_ir_rx_info.key_code) ||	/* Volume + */
							(0xF906 == g_ir_rx_info.key_code) ) 	/* Volume - */
					  )
					{
						repeat = TRUE;
					}

#ifdef SUPPORT_IR_RX_01FE
					if ( 
						(g_ir_rx_info.sys_code==IR_SYS_CODE_3)	/* system code 3 */
						&&( (0x18E7 == g_ir_rx_info.key_code) ||	/* Volume + */
							(0x10EF == g_ir_rx_info.key_code) ) 	/* Volume - */
					  )
					{
						repeat = TRUE;
					}
#endif

					if (repeat) {
						if (g_ir_rx_info.repeat_timer==0) {
							g_ir_rx_info.key_sent = FALSE;
							g_ir_rx_info.repeat_timer = T100MS_12;
						}
						g_ir_rx_info.timeout = T300MS_12;
					}

 				} else if(tmp_timer>=IR_RX_START_PERIOD_MIN&&tmp_timer<=IR_RX_START_PERIOD_MAX) {
					g_ir_rx_info.key_code = 0;
					g_ir_rx_info.sys_code = 0;
					g_ir_rx_info.bit_cntr = 0;
					g_ir_rx_info.state = IR_RX_STATE_CUSTOM;
					g_ir_rx_info.key_sent = TRUE;
				}
			}
			break;
		case IR_RX_STATE_CUSTOM:
			if ( (tmp_timer<IR_RX_ZERO_PERIOD_MIN)||(tmp_timer>IR_RX_ONE_PERIOD_MAX) ) {
				g_ir_rx_info.state = IR_RX_STATE_IDLE;
				break;
			} else if ( (tmp_timer>IR_RX_ZERO_PERIOD_MIN)&&(tmp_timer<IR_RX_ZERO_PERIOD_MAX) ) {
				g_ir_rx_info.key_code>>=1;
			}
			else if ( (tmp_timer>IR_RX_ONE_PERIOD_MIN)&&(tmp_timer<IR_RX_ONE_PERIOD_MAX) )
			{
				g_ir_rx_info.key_code>>=1;
				g_ir_rx_info.key_code|=0x8000;
			}
			g_ir_rx_info.bit_cntr++;
			if ( 16 == g_ir_rx_info.bit_cntr ) {
				g_ir_rx_info.sys_code = g_ir_rx_info.key_code ;
			} else if ( 32 == g_ir_rx_info.bit_cntr ) {
				g_ir_rx_info.key_sent = FALSE;
				g_ir_rx_info.bit_cntr = 0;
				g_ir_rx_info.state = IR_RX_STATE_IDLE;
				g_ir_rx_info.repeat_timer = T100MS_12;
				g_ir_rx_info.timeout = T600MS_12;
			}
			break;

		default:
			break;
			
	}
}

void ir_rx_main(void)
{
	u8 i,code;
	u8 len;

	if (!Is_Machine_Power) {
		ir_rx_init();
		return;
	}

	if (g_ir_rx_info.repeat_timer) {
		g_ir_rx_info.repeat_timer--;
	}
	
	if (0!=g_ir_rx_info.timeout) {
		g_ir_rx_info.timeout--;
		if ( 0==g_ir_rx_info.timeout) {
			ir_rx_init();
			return;
		}
	}

	if (g_ir_rx_info.key_sent) {
		return;
	}
	
	g_ir_rx_info.key_sent=TRUE;
	
	code=LSB(g_ir_rx_info.key_code);
	
	if(0xff==(code+MSB(g_ir_rx_info.key_code))) {	// check sum OK
		if (g_ir_rx_info.sys_code==IR_SYS_CODE_1) {
			len = sizeof(g_ir_rx_key_map_f708)/sizeof(IR_RX_KEY_MAP);
			for(i=0;i<len;i++) {
				if(code==g_ir_rx_key_map_f708[i].ir_key_code) {
					PostEvent(MMI_MODULE,g_ir_rx_key_map_f708[i].uicc_key_code,0);		
					break;
				}
			}
		} else if (g_ir_rx_info.sys_code==IR_SYS_CODE_2) {
			len = sizeof(g_ir_rx_key_map_00ff)/sizeof(IR_RX_KEY_MAP);
			for(i=0;i<len;i++) {
				if(code==g_ir_rx_key_map_00ff[i].ir_key_code) {
					PostEvent(MMI_MODULE,g_ir_rx_key_map_00ff[i].uicc_key_code,0);		
					break;
				}
			}
#ifdef SUPPORT_IR_RX_01FE
		} else if (g_ir_rx_info.sys_code==IR_SYS_CODE_3) {
			len = sizeof(g_ir_rx_key_map_01fe)/sizeof(IR_RX_KEY_MAP);
			for(i=0;i<len;i++) {
				if(code==g_ir_rx_key_map_01fe[i].ir_key_code) {
					PostEvent(MMI_MODULE,g_ir_rx_key_map_01fe[i].uicc_key_code,0);		
					break;
				}
			}
#endif

		} else if (g_ir_rx_info.sys_code == IR_SYS_CODE_ASUKA_TV) {
			ir_send_tv_code(LSB(g_ir_rx_info.sys_code), 
							MSB(g_ir_rx_info.sys_code), 
							LSB(g_ir_rx_info.key_code),
							MSB(g_ir_rx_info.key_code));
		}
	}
}


static const IR_RX_KEY_MAP g_can_ir_key_map[] = {
	{0x0C, UICC_SOURCE},
	{0x54, UICC_PLAY_PAUSE},
	{0x41, UICC_RADIO},
	{0x03, UICC_MUTE},
	{0x52, UICC_VOLUME_UP},
	{0x01, UICC_PREV},
	{0x07, UICC_NEXT},
	{0x40, UICC_VOLUME_DOWN},
	{0x5c, UICC_BT},
	{0x4A, UICC_PREV},
	{0x05, UICC_NEXT},
	{0x1C, UICC_PREV},
	{0x53, UICC_NEXT},
	{0x4D, UICC_HANG},
	{0x11, UICC_SPEECH},
};
void can_ir_init(void)
{
	g_can_ir_info.prev_timer=0;
	g_can_ir_info.state=IR_RX_STATE_IDLE;
	g_can_ir_info.timeout = 0;
	g_can_ir_info.repeat_timer = 0;
	g_can_ir_info.bit_cntr = 0;
	g_can_ir_info.key_code = 0;
	g_can_ir_info.sys_code = 0;
	g_can_ir_info.key_sent = TRUE;
}

void can_ir_handler(void)
{
	u32 tmp_timer;

	if (!Is_Machine_Power) {
		can_ir_init();
		return;
	}
	
	tmp_timer=g_can_ir_info.prev_timer;
	g_can_ir_info.prev_timer=TIM_GetCap4(TIMER_IR_RX);
	if (g_can_ir_info.prev_timer > tmp_timer) {
		tmp_timer=g_can_ir_info.prev_timer-tmp_timer;
	} else {
		tmp_timer = ((0xFFFF - tmp_timer) + g_can_ir_info.prev_timer);
	}

	switch (g_can_ir_info.state)
	{
		case IR_RX_STATE_IDLE:
			if (Bit_RESET == GPIO_ReadInputDataBit(GPIO_CAN_UART_RX_GRP, GPIO_CAN_UART_RX_PIN)) {
				g_can_ir_info.timeout = T600MS_12;
				g_can_ir_info.bit_cntr = 0;
				g_can_ir_info.state = IR_RX_STATE_START;
			}
			break;
		case IR_RX_STATE_START:
			if (Bit_RESET == GPIO_ReadInputDataBit(GPIO_CAN_UART_RX_GRP, GPIO_CAN_UART_RX_PIN)) {
				if ( (tmp_timer>IR_RX_REPEAT_PERIOD_MIN)&&(tmp_timer<IR_RX_REPEAT_PERIOD_MAX) ) {
					if ( 
						(g_can_ir_info.sys_code==IR_SYS_CODE_1)	/* system code 1 */
						&&( (0xAD52 == g_can_ir_info.key_code) ||	/* Volume + */
							(0xBF40 == g_can_ir_info.key_code) ) 	/* Volume - */
					  ) 
					{
						if (g_can_ir_info.repeat_timer==0) {
							g_can_ir_info.key_sent = FALSE;
							g_can_ir_info.repeat_timer = T50MS_12;
						}
						g_can_ir_info.timeout = T300MS_12;
					}
 				} else if(tmp_timer>=IR_RX_START_PERIOD_MIN&&tmp_timer<=IR_RX_START_PERIOD_MAX) {
					g_can_ir_info.key_code = 0;
					g_can_ir_info.sys_code = 0;
					g_can_ir_info.bit_cntr = 0;
					g_can_ir_info.state = IR_RX_STATE_CUSTOM;
					g_can_ir_info.key_sent = TRUE;
				}
			}
			break;
		case IR_RX_STATE_CUSTOM:
			if ( (tmp_timer<IR_RX_ZERO_PERIOD_MIN)||(tmp_timer>IR_RX_ONE_PERIOD_MAX) ) {
				g_can_ir_info.state = IR_RX_STATE_IDLE;
				break;
			} else if ( (tmp_timer>IR_RX_ZERO_PERIOD_MIN)&&(tmp_timer<IR_RX_ZERO_PERIOD_MAX) ) {
				g_can_ir_info.key_code>>=1;
			}
			else if ( (tmp_timer>IR_RX_ONE_PERIOD_MIN)&&(tmp_timer<IR_RX_ONE_PERIOD_MAX) )
			{
				g_can_ir_info.key_code>>=1;
				g_can_ir_info.key_code|=0x8000;
			}
			g_can_ir_info.bit_cntr++;
			if ( 16 == g_can_ir_info.bit_cntr ) {
				g_can_ir_info.sys_code = g_can_ir_info.key_code ;
			} else if ( 32 == g_can_ir_info.bit_cntr ) {
				g_can_ir_info.key_sent = FALSE;
				g_can_ir_info.bit_cntr = 0;
				g_can_ir_info.state = IR_RX_STATE_IDLE;
				g_can_ir_info.repeat_timer = T500MS_12;
				g_can_ir_info.timeout = T600MS_12;
			}
			break;

		default:
			break;
			
	}
}

void can_ir_main(void)
{
	u8 i,code;
	u8 len;

	if (!Is_Machine_Power) {
		can_ir_init();
		return;
	}

	if (g_can_ir_info.repeat_timer) {
		g_can_ir_info.repeat_timer--;
	}
	
	if (0!=g_can_ir_info.timeout) {
		g_can_ir_info.timeout--;
		if ( 0==g_can_ir_info.timeout) {
			can_ir_init();
			return;
		}
	}

	if (g_can_ir_info.key_sent) {
		return;
	}
	
	g_can_ir_info.key_sent=TRUE;
	
	code=LSB(g_can_ir_info.key_code);
	
	if(0xff==(code+MSB(g_can_ir_info.key_code))) {	// check sum OK
		if (g_can_ir_info.sys_code==IR_SYS_CODE_1) {
			len = sizeof(g_can_ir_key_map)/sizeof(IR_RX_KEY_MAP);
			for(i=0;i<len;i++) {
				if(code==g_can_ir_key_map[i].ir_key_code) {
					PostEvent(MMI_MODULE,g_can_ir_key_map[i].uicc_key_code,0);		
					break;
				}
			}
		}
	}
}

