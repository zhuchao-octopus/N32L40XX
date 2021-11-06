#include "public.h"
#define MCU_CMD_INDEX	0
#define GROUPID_INDEX		1
#define SUBID_INDEX			2
#define LENGTH_INDEX			3
//----------------------TX-----------------------------------------
#define MCU_TX_COMMON_INFO  	0x01
	#define SHORT_KEY_EVENT				0x01
	#define FRONT_SOURCE_INFO		0x02
	#define REAR_L_SOURCE_INFO		0x03
	#define BRIGHTNESS_INFO			0x04
	#define LONG_KEY_EVENT				0x05
	#define TX_MCU_VERSION				0x06
	#define TX_CANBOX_VERSION			0x07
	#define TX_POWER_CMD				0x08
	#define REAR_R_SOURCE_INFO		0x09
	#define REAR_LR_SOURCE_INFO		0x0A
	#define WINCE_WORK_STATUS		0x0B
	#define CAR_STATUS		0x0C
	#define TX_TINY_RAM_INFO	0x0D
	#define TX_RTC_INFO	0x0E
	#define TX_MAIN_DISP_INFO	0x0F
	#define TX_AUTO_TESTER_INFO	0x10

#define MCU_TX_RADIO_INFO		0x02	
	#define TOTAL_RADIO_INFO		0x01
	#define CURRENT_RADIO_INFO		0x02
	#define FREQENCY_LIST_INFO		0x03
	#define RADIO_SET_INFO			0x04
	#define RADIO_PARAMETER_INFO 	0x05

#define MCU_TX_RDS_INFO			0x03
	#define TX_RDS_TOTAL_INFO		0x01
	#define TX_RDS_FLAG_INFO		0x02
	#define TX_RDS_PTY_INFO			0x03
	#define TX_RDS_PS_INFO			0x04
	#define TX_RDS_RT_INFO			0x05		
	#define TX_RDS_PSLIST_INFO		0x06
	
#define MCU_TX_AUDIO_INFO 		0x04
	#define TX_AUDIO_TOTAL_AUDIO_INFO	0x01
	#define TX_AUDIO_ASP_INFO			0x02
	#define TX_AUDIO_EQUALIZER_INFO 	0x03
	#define TX_AUDIO_EQ_INFO2	0x04
	#define TX_AUDIO_VOLUME_INFO		0x05
	#define TX_AUDIO_FLAG_INFO			0x06
	#define TX_AUDIO_FREQUENCY_SPECTRUM_INFO			0x07
	#define TX_AUDIO_VOL_CTRL_INFO_WHEN_REVERSE		0x08
	#define TX_AUDIO_CUR_MAX_OUTPUT_VOL		0x09
	#define TX_AUDIO_CHANNEL_DEF_GAIN		0x0A
	#define TX_AUDIO_PROCESSOR_TYPE		0x0B
	#define TX_AUDIO_DSP_SETTINGS		0x0C
	#define TX_AUDIO_SA_INFO			0x0D
	#define TX_AUDIO_DSP_SETTINGS_2		0x0E

#define MCU_TX_CAN_INFO	0x05
	#define TX_CAN_DATA	0x01

#define MCU_TX_SYSTEM_INFO 		0x06
	#define TOTAL_SYSTEM_INFO		0x01
	#define SYSTEM_TIME_INFO		0x02
	#define SYSTEM_SET_INFO			0x03
	#define SYSTEM_DET_INFO			0x04
	#define SYSTEM_NAVI_AUDIO_CHANNEL_INFO			0x05
	#define SYSTEM_BEEP_CONTROL_INFO			0x06
	#define SYSTEM_RADIO_REGION_INFO			0x07
	#define SYSTEM_BREAK_DET_SET_INFO			0x09
	#define SYSTEM_ILLUMIN_SET_INFO			0x0A
	#define SYSTEM_NAVI_MUSIC_SET_INFO			0x0B
	#define SYSTEM_ILLUMIN_DET_SET_INFO			0x0C
	#define SYSTEM_PANEL_LED_COLOR			0x0D
	#define SYSTEM_ALL_KEY_INFOS			0x0E
	#define SYSTEM_NAVI_MIX_INFO		0x0F
	#define SYSTEM_LED_ONOFF_INFO		0x10
	#define SYSTEM_VCOM_INFO		0x11
	
#define MCU_TX_REFLASH_INFO 	0x07	
	#define IAP_START_CONFIRM_KEY	0x01
	#define IAP_REQUEST_UNLOCK_KEY	0x02
	#define IAP_REQUEST_DATA		0x03
	#define IAP_REQUEST_CHECKSUM	0x04
	#define IAP_REQUEST_RESET		0x05

#define MCU_TX_SWC_INFO		0x09
	#define SWC_MODE_INFO			0x01
	#define SWC_KEY_CUR_KEY_INFO		0x02
	#define SWC_REPEAT_KEY_INFO	0x03
	#define SWC_KEY_INFO			0x04
	#define SWC_KEY_TEST_INFO			0x05

#define MCU_TX_PANEL_INFO		0x0B

	#define PANEL_MODE_INFO			0x01
	#define PANEL_KEY_CUR_KEY_INFO		0x02
	#define PANEL_REPEAT_KEY_INFO	0x03
	#define PANEL_KEY_INFO			0x04
	#define PANEL_KEY_TEST_INFO			0x05

#define MCU_TX_SECRET_SETTING_INFO 0x0C
	#define SECRET_TOTAL_INFO	0x01
	#define SECRET_AUXIN_MODE_INFO	0x02
	#define SECRET_DVD_MODE_INFO	0x03
	#define SECRET_BREAK_DET_SET_INFO	0x04
	#define SECRET_ILLUMIN_DET_SET_INFO	0x05
	#define SECRET_RADIO_PARAMETER_INFO	0x06
	#define SECRET_FAN_START_TEMP_INFO	0x07
	#define SECRET_RADIO_RAGION_INFO	0x08
	#define SECRET_CAR_SURPPORT_INFO	0x09	

#define MCU_TX_DVD		0x0D
	#define MCU_TX_DVD_PWR_INFO		0x01
	#define MCU_TX_DVD_DISC_INFO		0x02

#define MCU_TX_AUTH_INFO	0x0F
	#define AUTH_SEND_SIGNATRUE		0x00
//--------------------------RX----------------------------------------
#define MCU_RX_GROUPID_COMMON_CMD		0x01
	#define SUBID_POWER_CMD	 		0x01
	#define SUBID_FRONT_SOURCE_CMD	 0x02
	#define SUBID_REAR_SOURCE_L_CMD		0x03
	#define SUBID_BRIGHTNESS_CMD		0x04
	#define SUBID_WINCE_STATUS_CMD		0x05
	#define SUBID_BLUETOOTH_CMD		0x06
	#define SUBID_PHONETIC_RECOGNITION_CMD 0x07
	#define SUBID_BEEP_CONTROL_CMD		0x08
	#define SUBID_SYSTEM_SET_INFO_REQ	0x09
	#define SUBID_SYSTEM_VERSION_INFO_REQ	0x0A
	#define SUBID_SYSTEM_LOAD_FACTORY_SET	0x0B
	#define SUBID_DISC_LED_SET		0x0C
	#define SUBID_3G_CMD			0x0D
	#define SUBID_DVD_POWER_CMD			0x0E
	#define SUBID_REAR_SOURCE_R_CMD		0x0F
	#define SUBID_MCU_WORK_STAUS_CMD		0x10
	#define SUBID_REAR_SOURCE_LR_CMD		0x11
	#define SUBID_CAR_STATUS_REQ		0x12
	#define SUBID_TINY_RAM_CTRL		0x13
	#define SUBID_SET_CUSTOM_CONFIG		0x14
	#define SUBID_RTC_INFO_REQ		0x15
	#define SUBID_PWR_CTRL_CMD		0x16
	#define SUBID_MAIN_DISP_CTRL_CMD		0x17
	#define SUBID_MAIN_DISP_RESOLUTION_CTRL_CMD		0x18
	#define SUBID_FRONT_VIDEO_SRC_CMD		0x19
	#define SUBID_AUTO_TESTER_CMD		0x1B
	#define SUBID_FCAM_PWR_CMD		0x1C
	
#define MCU_RX_GROUPID_RADIO_EVENT		0x02
	#define SUBID_RADIO_OPERATE			 0x01
	#define SUBID_RADIO_FREQUENCY_SET 	0x02
	#define SUBID_RADIO_PARAMETER_SET		0x03
	#define SUBID_RADIO_INFO_REQ			0x04

#define MCU_RX_GROUPID_RDS_EVENT	0x03
	#define SUBID_RDS_OPERATE			 	0x01
	#define SUBID_RDS_PARAMETER_SET 		0x02
	#define SUBID_RDS_INFO_REQ				0x03
	
#define MCU_RX_GROUP_AUDIO_EVENT	0x04
	#define SUBID_ASP_SET			 		0x01
	#define SUBID_EQ_SET			 			0x02
	#define SUBID_EQ_SET2 					0x03
	#define SUBID_VOLUME_SET				0x04
	#define SUBID_AUDIO_OPERATE				0x05
	#define SUBID_AUDIO_INFO_REQ			0x06
	#define SUBID_FREQUENCY_SPECTRUM_SET		0x07
	#define SUBID_NAVI_AUDIO_NOTIFY		0x08
	#define SUBID_APP_AUDIO_NOTIFY		0x09
	#define SUBID_VOL_CTRL_WHEN_REVERSE	0x0A
	#define SUBID_MAX_OUTPUT_GAIN		0x0B
	#define SUBID_AUDIO_CH_VOL_ADJ	0x0C
	#define SUBID_DSP_SETTINGS		0x0D
	#define SUBID_SA_CTRL		0x0F

#define MCU_RX_GROUP_CAN_EVENT		0x05
	#define SUBID_CAN_COMM_SET		0x01
	#define SUBID_CAN_PROTOCOL_SET	0x02
	#define SUBID_TRANSMIT_DATA_TO_CAN	0x03
	
#define MCU_RX_GROUP_IAP_EVENT		0x06
	#define SUBID_REFLASH_CMD			 	0x01
	#define SUBID_REFLASH_KEY			 	0x02
	#define SUBID_REFLASH_DATA				0x03
	#define SUBID_REFLASH_CHECKSUM			0x04
	#define SUBID_REFLASH_RESET				0x05	
	
#define MCU_RX_GROUP_SWC_EVENT	0x07
	#define SUBID_SWC_MODE_SELECT			0x01
	#define SUBID_SWC_SEND_UER_KEY		0x02
	#define SUBID_SWC_OVERLAP_CMD			0x03
	#define SUBID_SWC_INFO_REQ				0x04
	
#define MCU_RX_GROUP_TV_EVENT	0x09
	#define SUBID_TV_KEY	0x01
	#define SUBID_TV_TOUCH	0x02
	#define SUBID_TV_IR_CODE	0x05

#define MCU_RX_SYSTEM_SETTING_EVENT	0x0A
	#define SUBID_RADIO_REGION	0x01
	#define SUBID_BEEP_CONTROL	0x02
	#define SUBID_BRIGHTNESS	0x03
	#define SUBID_NAVI_AUDIO	0x04
	#define SUBID_LOAD_FACTORY	0x05
	#define SUBID_SETTING_INFO_REQ 0x06
	#define SUBID_BREAK_DET_SET 0x07
	#define SUBID_ILLUMIN_CONRTOL_SET 0x08
	#define SUBID_NAVI_MUSC_SET 0x09	
	#define SUBID_ILLUMIN_DET_SET 0x0A
	#define SUBID_PANEL_LED_COLOR_CTRL 0x0B
	#define SUBID_QUERY_KEY_INFO 0x0C
	#define SUBID_SET_KEY_INFO 0x0D
	#define SUBID_NAVI_MIX_CTRL 0x0E
	#define SUBID_LED_ONOFF_CTRL 0x0F
	#define SUBID_VCOM_CTRL 0x10
	#define SUBID_BT_TYPE		0x11

#define MCU_RX_GROUP_PANEL_EVENT	0x0B
	#define SUBID_PANEL_MODE_SELECT			0x01
	#define SUBID_PANEL_SEND_UER_KEY		0x02
	#define SUBID_PANEL_OVERLAP_CMD			0x03
	#define SUBID_PANEL_INFO_REQ				0x04
	#define SUBID_PANEL_DEFCONFIG_SET			0x05

#define  MCU_RX_SYSTEM_SECRET_SETTING_ENENT	0x0C
	#define SUBID_SECRET_SETTING_INFO_REQ		0x01
	#define SUBID_SECRET_SETTING_AUXIN_MODE	0x02
	#define SUBID_SECRET_SETTING_DVD_MODE	0x03
	#define SUBID_SECRET_SETTING_BREAK_DET_SET 0x04
	#define SUBID_SECRET_SETTING_ILLUMIN_DET_SET 0x05
	#define SUBID_SECRET_SETTING_RADIO_PARA_SET 0x06
	#define SUBID_SECRET_SETTING_FAN_START_TEMPERATURE_SET 0x07
	#define SUBID_SECRET_SETTING_RADIO_RAFION_SET 0x08

#define MCU_RX_GROUP_DVD	0x0D
	#define SUBID_DVD_OPS		0x01
	#define SUBID_DVD_QUERY	0x02

#define MCU_RX_AUTH_EVENT	0x0F
	#define SUBID_AUTH_REQ_SIGN		0x00

#define MAX_LIN_RX_LENGTH	144
static LIN_RX_STATE  Lin_Rx_State;
volatile uchar *PtrToLinBuffRx;
uchar * Lin_Tx_Ptr;
uchar Lin_Tx_Length,Lin_Tx_Length_bak;
uchar Lin_Tx_Length2,Lin_Tx_Length2_bak;
//ext uchar ReSend_Timer;
#define MAX_LIN_TX_LENGTH	98
static uchar Lin_Tx_Buff[MAX_LIN_TX_LENGTH];		//86+8 ????
static uchar Lin_Tx_Buff2[MAX_LIN_TX_LENGTH];	//86+8 ????
static uchar Lin_Rx_Len;

static uchar Lin_Rx_Buff[MAX_LIN_RX_LENGTH];
static uchar Lin_Rx_Buff2[MAX_LIN_RX_LENGTH];


static uchar Lin_Rx_Seq_Num;

//记录当前序列包由哪个buff发出，指针记录该buff
typedef struct
{
	uchar Lin_Tx_Seq_Num;	//save the latest seq num
	uchar * Ptr_Send_Buff;		//指向最新发送buff的开头
}LIN_SEND_TABLE;
static  LIN_SEND_TABLE Lin_Send_Table;

static uchar Lin_Rx_Checksum;
static uchar Lin_Tx_Checksum;

static uchar Lin_ReSend_Counter;	//重发计数器
static uchar LinUart_ReSend_Timer;	//重发间隔定时器

static uchar Lin_ReSend_Counter2;	
static uchar LinUart_ReSend_Timer2;	
static uchar GRoup_ID;
static uchar buff[MAX_LIN_TX_LENGTH];


static void Lin_Command_Check(uchar *Read_Lin_Ptr);


void host_rx_data(uint8_t Lin_Data)
{
	switch (Lin_Rx_State)
	{
		case LIN_RX_HEAD_CODE1:
			if(Lin_Data == LIN_ADDRESS_WINCE)	
			{
				Lin_Rx_Checksum=Lin_Data;
				if(F_Lin_RxBuff_Full==0)
				{
					PtrToLinBuffRx=Lin_Rx_Buff;
					Lin_Rx_State = LIN_RX_HEAD_CODE2;
					*PtrToLinBuffRx=Lin_Data;
					PtrToLinBuffRx++;
				}
				else if(F_Lin_RxBuff2_Full==0)
				{
					PtrToLinBuffRx=Lin_Rx_Buff2;
					Lin_Rx_State = LIN_RX_HEAD_CODE2;
					*PtrToLinBuffRx=Lin_Data;
					PtrToLinBuffRx++;
				}
					
			}
			break;
		case LIN_RX_HEAD_CODE2:
			if(Lin_Data == LIN_ADDRESS_MCU)
			{
				Lin_Rx_Checksum^=Lin_Data;
				Lin_Rx_State = LIN_RX_LENGTH;
				*PtrToLinBuffRx=Lin_Data;
				PtrToLinBuffRx++;
			}
			else if (Lin_Data == LIN_ADDRESS_WINCE)
			{
				Lin_Rx_Checksum=Lin_Data;
				Lin_Rx_State = LIN_RX_HEAD_CODE2;
			}
			else
			{
				Lin_Rx_State = LIN_RX_HEAD_CODE1;
			}
			break;
		case LIN_RX_LENGTH:
			Lin_Rx_Len=Lin_Data;
			*PtrToLinBuffRx = Lin_Data;
			PtrToLinBuffRx++;
			Lin_Rx_Checksum^=Lin_Data;
			if(Lin_Rx_Len<=MAX_LIN_RX_LENGTH)
				Lin_Rx_State=LIN_RX_SEQUENCE_NUM;
			else
				Lin_Rx_State=LIN_RX_HEAD_CODE1;
			break;
		case LIN_RX_SEQUENCE_NUM:
			Lin_Rx_Seq_Num= Lin_Data;
			Lin_Rx_State = LIN_RX_GROUP_ID;
			Lin_Rx_Checksum^=Lin_Rx_Seq_Num;
			*PtrToLinBuffRx = Lin_Rx_Seq_Num;
			PtrToLinBuffRx++;
			break;
		case LIN_RX_GROUP_ID:
			GRoup_ID=Lin_Data;
			if(Lin_Rx_Len<6)
				Lin_Rx_State = LIN_RX_HEAD_CODE1;
			else if(Lin_Rx_Len == 6)
				Lin_Rx_State = LIN_RX_CHECKSUM;
			else
				Lin_Rx_State = LIN_RX_SUB_ID;
			Lin_Rx_Checksum^=Lin_Data;
			*PtrToLinBuffRx = Lin_Data;
			PtrToLinBuffRx++;
			break;
		case LIN_RX_SUB_ID:
			Lin_Rx_Len=Lin_Rx_Len-7;
			if(Lin_Rx_Len>0)
				Lin_Rx_State = LIN_RX_DATA;
			else
				Lin_Rx_State = LIN_RX_CHECKSUM;
			Lin_Rx_Checksum^=Lin_Data;
			*PtrToLinBuffRx = Lin_Data;
			PtrToLinBuffRx++;
			break;
		case LIN_RX_DATA:
			*PtrToLinBuffRx = Lin_Data;
			PtrToLinBuffRx++;
			Lin_Rx_Checksum^=Lin_Data;
			Lin_Rx_Len--;  
			if(Lin_Rx_Len == 0)
			{
				Lin_Rx_State = LIN_RX_CHECKSUM;	
			}
			break;
		case  LIN_RX_CHECKSUM:
			g_host_is_sleep = FALSE;

			*PtrToLinBuffRx = Lin_Data;				
			//Lin_Rx_Checksum^=Lin_Data;
			Lin_Rx_State = LIN_RX_HEAD_CODE1;
			if( Lin_Rx_Checksum==Lin_Data)
			{ 					
				if(GRoup_ID==UART_RX_ACK)
				{
					LinUart_Ack_Rx();
				}
				else if(GRoup_ID<UART_NACK_NG)//仅对命令做应答，对ACK不应答
				{
					F_Lin_Rx_Data_Ready=1;

					if(PtrToLinBuffRx>=&Lin_Rx_Buff[0]&&PtrToLinBuffRx<=&Lin_Rx_Buff[MAX_LIN_RX_LENGTH])
					{
						F_Lin_RxBuff_Full=1;
					}
					else
					{
						F_Lin_RxBuff2_Full=1;
					}
				}
				else if(GRoup_ID==UART_NACK_NO_SUPPORT)
				{
					LinUart_Ack_Rx();
				}
				else if(GRoup_ID==UART_NACK_BUSY)
				{
					LinUart_Ack_Rx();
				}
				else 
				{
					//Nack;
				}
			}   
			else
			{
				if(GRoup_ID<UART_NACK_NG)
				{
					LinUart_Ack_Tx(UART_NACK_NG,Lin_Rx_Seq_Num);
				}
			}
			break;
		default:
			break;
	}

}

static void Lin_Command_Check(uchar *Read_Lin_Ptr)
{
	uchar tmp;
	uchar tmp_Seq_Num;
	uchar Group_ID_temp;
	uchar Sub_ID_temp;
	uchar length;
	length=*Read_Lin_Ptr++ - 7;
	tmp_Seq_Num=*Read_Lin_Ptr++;
	Group_ID_temp=*Read_Lin_Ptr++;
	Sub_ID_temp=*Read_Lin_Ptr++;
	if(Group_ID_temp<UART_NACK_NG)//仅对命令做应答，对ACK不应答
	{
		LinUart_Ack_Tx(UART_RX_ACK,tmp_Seq_Num);
		if((Wince_Start_Flag ==0))
			Wince_Start_Flag =1;
	}

	g_super_watchdog_timer = 0;

	switch (Group_ID_temp)
	{
		case MCU_RX_GROUPID_COMMON_CMD:
			switch(Sub_ID_temp)
			{
				case SUBID_POWER_CMD:
					if (6 == *Read_Lin_Ptr) {
						g_acc_wait_timeout = WORD(*(Read_Lin_Ptr+1), *(Read_Lin_Ptr+2));
						PostEvent(WINCE_MODULE, TX_TO_GUI_POWER_CMD, WCE_POWER_ACC_WAIT_TIMEOUT);
					} else if (7 == *Read_Lin_Ptr) {
						PostEvent(WINCE_MODULE, TX_TO_GUI_POWER_CMD, WCE_POWER_ACC_WAIT_TIMEOUT);
					} else if (0xFE == *Read_Lin_Ptr) {
						app_wd_stop_dog();
						g_disabed_encoder = TRUE;
						if (AUDIO_SRC_RADIO == g_audio_info.cur_source) {
							// we should not in radio channel, when u-boot start
							// re-init audio
							g_audio_info.cur_source = AUDIO_SRC_NONE;
							g_audio_info.sys_source = AUDIO_SRC_NONE;
							audio_set_pwr_ctrl(TRUE);
						}
					} else if (0xFF == *Read_Lin_Ptr) {
						g_disabed_encoder = FALSE;
						app_wd_feed_dog();
					} else if (0x20 == *Read_Lin_Ptr) {
						PostEvent(WINCE_MODULE, TX_TO_GUI_POWER_CMD, WCE_POWER_BATT_ADC_VAL);
					}
					break;
				case SUBID_FRONT_SOURCE_CMD: 
					PostEvent(WINCE_MODULE, TX_TO_GUI_FRONT_SOURCE_INFO,  *Read_Lin_Ptr);
					if(*Read_Lin_Ptr>NUM_OF_SOURCE)
						*Read_Lin_Ptr=NUM_OF_SOURCE;
					if(*Read_Lin_Ptr!=SOURCE_NAVI)
					{
						SwitchSource((SOURCE)(*Read_Lin_Ptr));
					}
					if(*Read_Lin_Ptr != SOURCE_TUNER)
					{
						if(g_radio_info.flag.field.F_RADIO_SEEK) {
							PostEvent(TUNER_MODULE,RADIO_EVT_STOP_SEEK, NONE);
						}
					}
					
					break;
					
				case SUBID_REAR_SOURCE_L_CMD:
					PostEvent(VIDEO_MODULE,EVT_VID_REAR_L_SOURCE_SET, *Read_Lin_Ptr);
					break;
					
				case SUBID_REAR_SOURCE_R_CMD:
					PostEvent(VIDEO_MODULE,EVT_VID_REAR_R_SOURCE_SET, *Read_Lin_Ptr);
					break;

				case SUBID_WINCE_STATUS_CMD:
					if(*Read_Lin_Ptr>NUM_OF_SOURCE)
						*Read_Lin_Ptr=NUM_OF_SOURCE;
					if(*Read_Lin_Ptr!=SOURCE_NAVI)
					{
						SwitchSource((SOURCE)(*Read_Lin_Ptr));
					}
					break;
				case SUBID_BRIGHTNESS_CMD:
					if(*Read_Lin_Ptr >= MAX_BRIGHTNESS_LEVEL-1)
						*Read_Lin_Ptr = MAX_BRIGHTNESS_LEVEL-1;
					g_brightness = *Read_Lin_Ptr;
					PostEvent(WINCE_MODULE, TX_TO_GUI_BRIGHTNESS_INFO,g_brightness );
					break;
				case SUBID_BLUETOOTH_CMD:
					if(*Read_Lin_Ptr==0)
					{
						if (g_audio_info.bt_phone_on) {
							if (!g_audio_info.carplay_phone_on) {
								audio_set_mute_temporary(360);
							} else {
								audio_set_mute_temporary(2000);
							}
						}
						audio_set_bt_phone(FALSE);
						audio_set_carplay_phone(FALSE);
					}
					else if(*Read_Lin_Ptr==1)
					{
						if (!g_audio_info.bt_phone_on && !g_audio_info.carplay_phone_on) {
							audio_set_mute_temporary(800);
						}
						audio_set_bt_phone(TRUE);
					}
					else if(*Read_Lin_Ptr==2)
					{
						SwitchSource(SOURCE_BT);
					}
					else if(*Read_Lin_Ptr==3)
					{
					}
					else if (*Read_Lin_Ptr==4)
					{
						audio_set_mute_temporary(360);
					}
					else if (*Read_Lin_Ptr==0x10)
					{
					}
					else if (*Read_Lin_Ptr==0x11)
					{
					}
					else if (*Read_Lin_Ptr==0x20)
					{
						if (g_audio_info.bt_voice_on) {
							audio_set_mute_temporary(360);
						}
						audio_set_bt_voice(FALSE);
					}
					else if(*Read_Lin_Ptr==0x21)
					{
						audio_set_bt_voice(TRUE);
					}
					else if (*Read_Lin_Ptr==0x30)
					{
//						audio_set_carplay_phone(FALSE);
						audio_set_mute_temporary(2000);
					}
					else if(*Read_Lin_Ptr==0x31)
					{
						audio_set_mute_temporary(3000);
						audio_set_carplay_phone(TRUE);
					}
					break;

				case SUBID_BEEP_CONTROL_CMD:
					if ( (*Read_Lin_Ptr==0) || (*Read_Lin_Ptr==1) )
					{
						if ( *Read_Lin_Ptr != g_sys_info_store.beep_onoff) {
							g_sys_info_store.beep_onoff=*Read_Lin_Ptr;
							ak_flash_save_info();
						}
						PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_BEEP_INFO, g_sys_info_store.beep_onoff);
					}
					else if(*Read_Lin_Ptr==2)
					{
						beep_short_mode();
					}
					else if(*Read_Lin_Ptr==3)
					{
						beep_long_mode();
					}
					else if(*Read_Lin_Ptr==4)
					{
						beep_double_mode();
					}
					break;
				case SUBID_SYSTEM_SET_INFO_REQ:
					switch(*Read_Lin_Ptr)
					{
						case 0x00:	
							PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_SET_INFO,NONE );
							break;
						case 0x02:
							PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_BEEP_INFO, g_sys_info_store.beep_onoff);
							break;
						case 0x03:
							PostEvent(WINCE_MODULE, TX_TO_GUI_BRIGHTNESS_INFO,g_brightness );	
							break;
						case  0x05:
							PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_BREAK_SET_INFO,g_sys_info_store.brake_mode );
							break;
						case  0x06:
							PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_ILLUMIN_SET_INFO,F_ILLUMIN_CONTORL_MODE);
							break;	
						default:
							break;
						
					}
					break;
				case SUBID_SYSTEM_VERSION_INFO_REQ:
					switch(*Read_Lin_Ptr)
					{
						case 0x00:	
							PostEvent(WINCE_MODULE, TX_TO_GUI_MCU_VERSION, 0);
							break;
						case 0x66:
							PostEvent(WINCE_MODULE, TX_TO_GUI_MCU_VERSION, 0x66);
							break;
						default:
							break;						
					}
					break;
				case SUBID_MCU_WORK_STAUS_CMD:
					switch(*Read_Lin_Ptr)
					{
						case 0x00:	
							PostEvent(WINCE_MODULE, TX_TO_GUI_FRONT_SOURCE_INFO,  FrontSource);
							break;
						case 0x01:
							PostEvent(WINCE_MODULE, TX_TO_GUI_REAR_L_SOURCE_INFO,  video_get_rear_video_source(REAR_L));
							break;
						case 0x02:
							PostEvent(WINCE_MODULE, TX_TO_GUI_REAR_R_SOURCE_INFO,  video_get_rear_video_source(REAR_R));						
							break;
						case 0x03:
							PostEvent(WINCE_MODULE, TX_TO_GUI_REAR_L_SOURCE_INFO,  video_get_rear_video_source(REAR_L));
							PostEvent(WINCE_MODULE, TX_TO_GUI_REAR_R_SOURCE_INFO,  video_get_rear_video_source(REAR_R));	
							break;
						default:
							break;
					}
					break;
				case SUBID_CAR_STATUS_REQ:
					switch(*Read_Lin_Ptr)
					{
						case 0x00:	//parking status
							PostEvent(WINCE_MODULE, TX_TO_GUI_CAR_PARKING_STATUS,  F_PARKING);
							break;
						default:
							break;
					}
					break;
				case SUBID_TINY_RAM_CTRL:
					switch (*Read_Lin_Ptr)
					{
						case 0x00:	//set tiny ram
							system_set_tiny_ram(Read_Lin_Ptr+1, length-1);
							break;
						case 0x01:	//get tiny ram
							PostEvent(WINCE_MODULE, TX_TO_GUI_TINY_RAM_INFO,  NONE);
							break;
						default:
							break;
					}
					break;

				case SUBID_RTC_INFO_REQ:
					PostEvent(WINCE_MODULE, TX_TO_GUI_RTC_INFO,  NONE);					
					break;

				case SUBID_PWR_CTRL_CMD:
					switch(*Read_Lin_Ptr) {
						case 0x00:
							g_app_in_charge = TRUE;
							if (g_fake_pwr_off) {
								g_fake_pwr_off = FALSE;
								audio_set_mute(AUDIO_MUTE_SYSTEM, FALSE);
								g_app_slept = FALSE;
								PostEvent(WINCE_MODULE, TX_TO_GUI_BRIGHTNESS_INFO,g_brightness );	
							}
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_PROCESSOR_TYPE, 1);	
							break;
						case 0x01:
							g_app_in_charge = FALSE;
							if (!g_fake_pwr_off) {
								g_fake_pwr_off = TRUE;
								audio_set_mute(AUDIO_MUTE_SYSTEM, TRUE);
							}
							g_app_slept = TRUE;
							break;
						default:
							break;
					}
					break;

				case SUBID_FRONT_VIDEO_SRC_CMD:
					PostEvent(VIDEO_MODULE,EVT_VID_FRONT_SOURCE_SET, *Read_Lin_Ptr);
					break;

				case SUBID_FCAM_PWR_CMD:
					switch (*Read_Lin_Ptr++) {
						case 0x00:
							GPIO_ResetBits(GPIO_FCAM_PWR_GRP, GPIO_FCAM_PWR_PIN);
							break;
						case 0x01:
							GPIO_SetBits(GPIO_FCAM_PWR_GRP, GPIO_FCAM_PWR_PIN);
							break;
					}
					break;
					
				default:
					break;
			}
			break;
		case MCU_RX_GROUPID_RADIO_EVENT:
			switch(Sub_ID_temp)
			{
				case SUBID_RADIO_OPERATE:					
					switch(*Read_Lin_Ptr++)
					{
						case 0x00:	//波段切换
							PostEvent(TUNER_MODULE, RADIO_EVT_BAND,*Read_Lin_Ptr );
							break;
						case 0x01:	//搜索电台
							PostEvent(TUNER_MODULE, RADIO_EVT_SEEK,*Read_Lin_Ptr );
							break;
						case 0x02:	//近程远程切换
							PostEvent(TUNER_MODULE, RADIO_EVT_LOC_DX,*Read_Lin_Ptr );
							break;
						case 0x03:	//选择预置电台播放
							PostEvent(TUNER_MODULE, RADIO_EVT_PRESET_SELECT,*Read_Lin_Ptr );
							break;
						case 0x04:	//保存当前频率到指定电台号
							PostEvent(TUNER_MODULE, RADIO_EVT_PRESET_SAVE,*Read_Lin_Ptr );
							break;
						case 0x05:
							PostEvent(TUNER_MODULE, RADIO_EVT_ST, 0x00 );	// close ST
							break;
						case 0x06:
							PostEvent(TUNER_MODULE, RADIO_EVT_ST, 0x01 );	// open ST
							break;
						default:
							break;
					}
					break;
				case SUBID_RADIO_FREQUENCY_SET:
					PostEvent(TUNER_MODULE, RADIO_EVT_FREQ_SET,WORD(*Read_Lin_Ptr++, *Read_Lin_Ptr));
					break;
				case SUBID_RADIO_INFO_REQ:
					switch(*Read_Lin_Ptr++)
					{
						case 0x00:	
							//PostEvent(WINCE_MODULE, TX_TO_GUI_RADIO_TOTAL_INFO,NONE );
							PostEvent(WINCE_MODULE, TX_TO_GUI_RADIO_CURRENT_INFO,NONE );
							PostEvent(WINCE_MODULE, TX_TO_GUI_RADIO_FREQENCY_LIST_INFO,NONE );
							break;
						case 0x01:
							PostEvent(WINCE_MODULE, TX_TO_GUI_RADIO_CURRENT_INFO,NONE );
							break;
						case 0x02:
							PostEvent(WINCE_MODULE, TX_TO_GUI_RADIO_FREQENCY_LIST_INFO,NONE );
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}//end MCU_RX_GROUPID_RADIO_EVENT
			break;
			
		case  MCU_RX_GROUPID_RDS_EVENT:
			switch(Sub_ID_temp)
			{
				case SUBID_RDS_OPERATE: 
					switch(*Read_Lin_Ptr++)
					{
						case 0x00:	
							PostEvent(TUNER_MODULE, RADIO_EVT_RDS_SWITCH,*Read_Lin_Ptr );
							break;
						case 0x01:	
							PostEvent(TUNER_MODULE, RADIO_EVT_RDS_PTY_SET,*Read_Lin_Ptr );
							break;
						default:
							break;
					}
					break;
				case SUBID_RDS_INFO_REQ:
					switch(*Read_Lin_Ptr++)
					{
						case 0x00:	
							PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_FLAG_INFO,NONE );
							PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PTY_INFO, NONE);
							PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PS_INFO, NONE);
							break;
						case 0x01:	
							PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_FLAG_INFO,NONE );
							break;
						case 0x02:
							PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PTY_INFO, NONE);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			break;
		case  MCU_RX_GROUP_AUDIO_EVENT:
			switch(Sub_ID_temp)
			{
				case SUBID_EQ_SET:
					tmp = *Read_Lin_Ptr++;
					switch (tmp) {
						case 0x00:
							audio_set_eq_mode((EQ_MODE)(*Read_Lin_Ptr));
							break;
						case 0x03:
							audio_set_subwoofer(*Read_Lin_Ptr);
							break;
						case 0x04:
							audio_set_loudness(*Read_Lin_Ptr);
							break;
						case 0x05:
							audio_set_fader(*Read_Lin_Ptr);
							break;
						case 0x06:
							audio_set_balance(*Read_Lin_Ptr);
							break;

						case 0x20:
						case 0x21:
						case 0x22:
						case 0x23:
						case 0x24:
						case 0x25:
						case 0x26:
						case 0x27:
						case 0x28:
#if ASP_MODEL==ASP_BU32107
						case 0x29:
						case 0x2A:
						case 0x2B:
						case 0x2C:
						case 0x2D:
						case 0x2E:
#endif
 							audio_set_eq_custom_level((EQ_FREQ)(tmp-0x20), *Read_Lin_Ptr);
							break;
						case 0x40:
							audio_save_eq_user_mode(*Read_Lin_Ptr);
							break;
						case 0x50:
							audio_soundfield_ctrl(*Read_Lin_Ptr);
							break;
						default:
							break;
							
					}
					break;
				case SUBID_VOLUME_SET:
					switch(*Read_Lin_Ptr++)
					{
						case 0x00:
							audio_set_volume(*Read_Lin_Ptr);
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_VOLUME_INFO, NONE);
							break;
						case 0x01:
 							if (0 == *Read_Lin_Ptr) {
								audio_set_mute(AUDIO_MUTE_USER, TRUE);
								PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_VOLUME_INFO, NONE);
								PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_FLAG_INFO, NONE);
							} else if (1 == *Read_Lin_Ptr) {
								audio_set_mute(AUDIO_MUTE_USER, FALSE);
								PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_VOLUME_INFO, NONE);
								PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_FLAG_INFO, NONE);
							}
							break;
						case 0x02:
							audio_volume_up();
							if (length==3) {
								++Read_Lin_Ptr;
								PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_VOLUME_INFO, *Read_Lin_Ptr);
							} else {
								PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_VOLUME_INFO, NONE);
							}
							break;
						case 0x03:
							audio_volume_down();
							if (length==3) {
								++Read_Lin_Ptr;
								PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_VOLUME_INFO, *Read_Lin_Ptr);
							} else {
								PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_VOLUME_INFO, NONE);
							}
							break;
						case 0x04:
							{
								u16 ms = 0;
								ms = (u16)(*Read_Lin_Ptr)*(u16)100;
								audio_set_mute_temporary(ms);
							}
							break;
						default:
							break;
					}
					
					break;
				case SUBID_AUDIO_INFO_REQ:
					switch(*Read_Lin_Ptr++)
					{
						case 0x02:	
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_EQUALIZER_INFO, NONE);
#if ASP_MODEL==ASP_BU32107
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS, NONE);
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS_2, NONE);
#endif
							break;
						case 0x03:	
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_EQ_INFO2, NONE);
							break;
						case 0x04:	
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_VOLUME_INFO, NONE);
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_FLAG_INFO, NONE);
							break;
						default:
							break;
					}
					break;
				case SUBID_NAVI_AUDIO_NOTIFY:
					switch (*Read_Lin_Ptr) {
						case 0:
							g_navi_audio_det = 0;
							break;
						case 1:
							g_navi_audio_det = 1;
							break;
						default:
							break;
					}
					break;
				case SUBID_APP_AUDIO_NOTIFY:
					if (0x66 == *Read_Lin_Ptr) {
						audio_set_android_sound_on(TRUE);
					} else if (0x44 == *Read_Lin_Ptr) {
						audio_set_android_sound_on(FALSE);
					} else {
						g_app_audio_det = (*Read_Lin_Ptr++!=0)?1:0;
					}
					break;
				case SUBID_VOL_CTRL_WHEN_REVERSE:
					if (*Read_Lin_Ptr <= 100) {
						if (*Read_Lin_Ptr != g_sys_info_store.vol_ctrl_when_reverse) {
							audio_set_vol_ctrl_when_reverse(*Read_Lin_Ptr);
							ak_flash_save_info();
						}
					}
					PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_VOL_CTRL_INFO_WHEN_REVERSE, NONE);
					break;
				case SUBID_MAX_OUTPUT_GAIN:
					PostEvent(AUDIO_MODULE, AUDIO_EVT_OUTPUT_TYPE, WORD(0,*Read_Lin_Ptr));
					if (0xFF == *Read_Lin_Ptr) {
						PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_CUR_MAX_OUTPUT_VOL, NONE);
					}
					break;
				case SUBID_AUDIO_CH_VOL_ADJ:
					switch (*Read_Lin_Ptr++) {
						case 1:
							g_audio_info.extra_input_gain_factory[AUDIO_SRC_HOST] = (s8)(*Read_Lin_Ptr++ - 10);
							g_audio_info.extra_input_gain_factory[AUDIO_SRC_RADIO] = (s8)(*Read_Lin_Ptr++ - 10);
							g_audio_info.extra_input_gain_factory[AUDIO_SRC_DVD] = (s8)(*Read_Lin_Ptr++ - 10);
							g_audio_info.extra_input_gain_factory[AUDIO_SRC_BT_MODULE] = (s8)(*Read_Lin_Ptr++ - 10);
							g_audio_info.extra_input_gain_factory[AUDIO_SRC_AUXIN] = (s8)(*Read_Lin_Ptr++ - 10);
							g_audio_info.extra_input_gain_factory[AUDIO_SRC_TV] = (s8)(*Read_Lin_Ptr++ - 10);
							PostEvent(AUDIO_MODULE, AUDIO_EVT_ADJ_CH_VOL, NONE);
							break;
						case 2:
							g_audio_info.extra_input_gain_user[AUDIO_SRC_HOST] = (s8)(*Read_Lin_Ptr++ - 10);
							g_audio_info.extra_input_gain_user[AUDIO_SRC_RADIO] = (s8)(*Read_Lin_Ptr++ - 10);
							g_audio_info.extra_input_gain_user[AUDIO_SRC_DVD] = (s8)(*Read_Lin_Ptr++ - 10);
							g_audio_info.extra_input_gain_user[AUDIO_SRC_BT_MODULE] = (s8)(*Read_Lin_Ptr++ - 10);
							g_audio_info.extra_input_gain_user[AUDIO_SRC_AUXIN] = (s8)(*Read_Lin_Ptr++ - 10);
							g_audio_info.extra_input_gain_user[AUDIO_SRC_TV] = (s8)(*Read_Lin_Ptr++ - 10);
							PostEvent(AUDIO_MODULE, AUDIO_EVT_ADJ_CH_VOL, NONE);
							break;
						case 3:
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_CHANNEL_DEF_GAIN, NONE);
							break;
					}
					break;
				case SUBID_DSP_SETTINGS:
#if ASP_MODEL==ASP_BU32107
					switch (*Read_Lin_Ptr++) {
						case 0x00:
							audio_set_loud_on(*Read_Lin_Ptr);
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS, 1);
							break;
						case 0x01:
							audio_set_hpf(*Read_Lin_Ptr, *(Read_Lin_Ptr+1));
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS, 1);
							break;
						case 0x02:
							audio_set_dsp_phat(*Read_Lin_Ptr, *(Read_Lin_Ptr+1));
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS, 1);
							break;
						case 0x03:
							audio_set_dsp_bass(*Read_Lin_Ptr, *(Read_Lin_Ptr+1), *(Read_Lin_Ptr+2));
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS, 1);
							break;
						case 0x04:
							audio_set_dsp_core(*Read_Lin_Ptr, *(Read_Lin_Ptr+1));
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS, 1);
							break;
						case 0x05:
							audio_set_dsp_space(*Read_Lin_Ptr, *(Read_Lin_Ptr+1));
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS, 1);
							break;
						case 0x06:
							audio_set_subwoofer_on(*Read_Lin_Ptr);
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS, 1);
							break;
						case 0x07:
							audio_set_dsp_soundfield(*Read_Lin_Ptr);
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS_2, 1);
							break;
						case 0x08:
							audio_set_dsp_speaker(AUDIO_SPK_FL, *Read_Lin_Ptr, *(Read_Lin_Ptr+1), *(Read_Lin_Ptr+2));
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS_2, 1);
							break;
						case 0x09:
							audio_set_dsp_speaker(AUDIO_SPK_FR, *Read_Lin_Ptr, *(Read_Lin_Ptr+1), *(Read_Lin_Ptr+2));
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS_2, 1);
							break;
						case 0x0A:
							audio_set_dsp_speaker(AUDIO_SPK_RL, *Read_Lin_Ptr, *(Read_Lin_Ptr+1), *(Read_Lin_Ptr+2));
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS_2, 1);
							break;
						case 0x0B:
							audio_set_dsp_speaker(AUDIO_SPK_RR, *Read_Lin_Ptr, *(Read_Lin_Ptr+1), *(Read_Lin_Ptr+2));
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS_2, 1);
							break;
						case 0x0C:
							audio_set_dsp_surround(*Read_Lin_Ptr);
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS_2, 1);
							break;
						case 0x0D:
							audio_set_dsp_sf_expert_mode(*Read_Lin_Ptr);
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS_2, 1);
							break;
						default:
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS, 1);
							PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_SETTINGS_2, 1);
							break;
					}
#endif
					break;
				case SUBID_SA_CTRL:
					audio_set_spectrum_analyzer((bool)(*Read_Lin_Ptr));
					break;
				default:
					break;
			}
			break;

		case MCU_RX_GROUP_CAN_EVENT:
			switch(Sub_ID_temp) {
				case SUBID_CAN_COMM_SET:
					switch (*Read_Lin_Ptr) {
						case 0:
							g_canbox_baudrate = 9600;
							USART_RxTx_Init(TRUE);	// reinit USART
							break;
						case 1:
							g_canbox_baudrate = 19200;
							USART_RxTx_Init(TRUE);	// reinit USART
							break;
						case 2:
							g_canbox_baudrate = 38400;
							USART_RxTx_Init(TRUE);	// reinit USART
							break;
						case 3:
							g_canbox_baudrate = 57600;
							USART_RxTx_Init(TRUE);	// reinit USART
							break;
						case 4:
							g_canbox_baudrate = 115200;
							USART_RxTx_Init(TRUE);	// reinit USART
							break;
						case 0xFF:
							USART_RxTx_Init(FALSE);	// reinit USART
							break;
					}
					break;
				case SUBID_CAN_PROTOCOL_SET:
					canbox_set_protocol(
						(CAN_HEAD_TYPE)(*(Read_Lin_Ptr+0)), 
						(CAN_LEN_TYPE)(*(Read_Lin_Ptr+1)),
						(CAN_PARITY_TYPE)(*(Read_Lin_Ptr+2)),
						(CAN_ACK_TYPE)(*(Read_Lin_Ptr+3))
						);
					break;
				case SUBID_TRANSMIT_DATA_TO_CAN:
					if (0==g_usart_tx_len) {
						ak_memcpy(USART_Transmit_Tx_Buff, Read_Lin_Ptr, length);
						g_usart_tx_len = length;
						PostEvent(CAN_MODULE, CAN_EVENT_TX_DATA, 1);
					} else if (0==g_usart_tx_len2) {
						ak_memcpy(USART_Transmit_Tx_Buff2, Read_Lin_Ptr, length);
						g_usart_tx_len2 = length;
						PostEvent(CAN_MODULE, CAN_EVENT_TX_DATA, 2);
					} else {
						ak_memcpy(USART_Transmit_Tx_Buff2, Read_Lin_Ptr, length);
						g_usart_tx_len2 = length;
						PostEvent(CAN_MODULE, CAN_EVENT_TX_DATA, 3);	// override tx buffer 2
					}
					break;
			}
			break;
			
		case MCU_RX_GROUP_IAP_EVENT:
			if(Sub_ID_temp == SUBID_REFLASH_CMD)
			{
				main_goto_iap();
			} else if (0x05==Sub_ID_temp) {
				// app send the wrong cmd, fix it later
				USART_RxTx_Init(TRUE);	// reinit USART
			}
			break;

		case MCU_RX_GROUP_SWC_EVENT:
			switch(Sub_ID_temp)
			{				
				case SUBID_SWC_MODE_SELECT:
					switch(*Read_Lin_Ptr) {
						case 0:
							PostEvent(PANEL_MODULE, KEY_EVT_ENTER_STUDY, KEY_STUDY_SWC);
							break;
						case 1:
						case 2:
							PostEvent(PANEL_MODULE, KEY_EVT_EXIT_STUDY, *Read_Lin_Ptr);
							break;
						default:
							break;
					}
					break;

				case SUBID_SWC_SEND_UER_KEY:
					{
						u8 key_code_short;
						u8 key_code_long;
						key_code_short = *Read_Lin_Ptr;
						++Read_Lin_Ptr;
						key_code_long = *Read_Lin_Ptr;
						PostEvent(PANEL_MODULE, KEY_EVT_STUDY_KEY_CODE, WORD(key_code_long, key_code_short));
					}
					break;	

				case SUBID_SWC_OVERLAP_CMD:
					if (2==(*Read_Lin_Ptr)) {
						PostEvent(PANEL_MODULE, KEY_EVT_CLEAR_KEYS, KEY_STUDY_SWC);
					}
					break;

				case SUBID_SWC_INFO_REQ:
					switch (*Read_Lin_Ptr) {
						case 0:
							PostEvent(PANEL_MODULE, KEY_EVT_QUERY_KEY_CODES, KEY_STUDY_SWC);
							break;
					}
					break;	

				default:
					break;	
			}
			break;

		case MCU_RX_GROUP_TV_EVENT:
			switch(Sub_ID_temp)
			{
				case SUBID_TV_KEY:
					break;
				case SUBID_TV_TOUCH:
					Rem_SendDTVTouch(*Read_Lin_Ptr, *(Read_Lin_Ptr+1));
					break;
				case SUBID_TV_IR_CODE:
					ir_send_tv_code(*Read_Lin_Ptr, *(Read_Lin_Ptr+1), *(Read_Lin_Ptr+2), *(Read_Lin_Ptr+3));
					break;
			}
			break;
			
		case MCU_RX_SYSTEM_SETTING_EVENT:
			switch(Sub_ID_temp)
			{
				case  SUBID_RADIO_REGION	:
					if(*Read_Lin_Ptr!= g_radio_area)
					{
						g_radio_area = *Read_Lin_Ptr;
						PostEvent(TUNER_MODULE, RADIO_EVT_AREA_SET,*Read_Lin_Ptr );
						g_sys_info_store.radio_area=g_radio_area;
						ak_flash_save_info();
					}
					PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_RADIO_REGION_INFO, g_radio_area);
					break;

				case SUBID_BRIGHTNESS	:
					if(*Read_Lin_Ptr >= MAX_BRIGHTNESS_LEVEL-1)
						*Read_Lin_Ptr = MAX_BRIGHTNESS_LEVEL-1;
					g_brightness = *Read_Lin_Ptr;
					PostEvent(WINCE_MODULE, TX_TO_GUI_BRIGHTNESS_INFO,g_brightness );
					break;
				case SUBID_LOAD_FACTORY:
					if(*Read_Lin_Ptr == 0)
					{
						audio_set_mute(AUDIO_MUTE_SYSTEM, TRUE);
						audio_init();
						Init_Nochange_Section();

						REAL_SYS_PWR_OFF;
						delay_1ms(1000);
						NVIC_SystemReset();	// force reset 
					}
					break;		
				case SUBID_SETTING_INFO_REQ:
				{
					switch(*Read_Lin_Ptr)
					{
						case 0x00:	
							PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_SET_INFO,NONE );
							break;
						case 0x01:
							PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_RADIO_REGION_INFO, g_radio_area);
							break;
						case 0x03:
							PostEvent(WINCE_MODULE, TX_TO_GUI_BRIGHTNESS_INFO,g_brightness );	
							break;
						case 0x05:
							PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_BREAK_SET_INFO,g_sys_info_store.brake_mode );
							break;
						case 0x06:
							PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_ILLUMIN_SET_INFO,F_ILLUMIN_CONTORL_MODE);
							break;
						case 0x09:
							PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_NAVI_MIX_INFO, g_audio_info.navi_mix_extra_gain);
							break;
						default:
							break;
					}
				}
				break;
			case  SUBID_BREAK_DET_SET:
				if((*Read_Lin_Ptr!= g_sys_info_store.brake_mode) && (*Read_Lin_Ptr <=PARKING_SERIAL_MODEL))
				{
					g_sys_info_store.brake_mode = *Read_Lin_Ptr;
					F_PARKING=0;//重新检测
				}
				PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_BREAK_SET_INFO,g_sys_info_store.brake_mode );
				break;
			case SUBID_ILLUMIN_CONRTOL_SET:	
				if(*Read_Lin_Ptr <=2)
				{
					F_ILLUMIN_CONTORL_MODE = *Read_Lin_Ptr;					
				}
				PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_ILLUMIN_SET_INFO,F_ILLUMIN_CONTORL_MODE );
				break;
			case SUBID_PANEL_LED_COLOR_CTRL:
				switch (*Read_Lin_Ptr) {
					case 0x04:
						led_set_color(*(Read_Lin_Ptr+1), *(Read_Lin_Ptr+2), *(Read_Lin_Ptr+3));
						PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_PANEL_LED_COLOR, NONE);
						break;
					case 0x05:
						PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_PANEL_LED_COLOR, NONE);
						break;
				}
				break;
			case SUBID_QUERY_KEY_INFO:
				PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_ALL_KEY_INFOS, NONE);
				break;
			case SUBID_NAVI_MIX_CTRL:
				PostEvent(AUDIO_MODULE, AUDIO_EVT_NAVI_EXTRA_GAIN, WORD(0,*Read_Lin_Ptr));
				break;
				
			case SUBID_LED_ONOFF_CTRL:
				switch (*Read_Lin_Ptr) {
					case 0x00:
					case 0x01:
					case 0x02:
						led_set_always_on(*Read_Lin_Ptr);
						break;
					default:
						break;
				}
				PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_LED_ONOFF_INFO, NONE);
				break;
			case SUBID_VCOM_CTRL:
				switch (*Read_Lin_Ptr) {
					case 0x00:
						vcom_set_level(*(Read_Lin_Ptr+1));
						PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_VCOM_INFO, NONE);
						break;
					case 0x01:
						PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_VCOM_INFO, NONE);
						break;
					default:
						break;
				}
				break;
			case SUBID_BT_TYPE:
				g_bt_type = *Read_Lin_Ptr;
				break;
			default:
				break;
			}
			break;
		case MCU_RX_GROUP_PANEL_EVENT:
			switch(Sub_ID_temp)
			{				
				case SUBID_PANEL_MODE_SELECT:
					switch(*Read_Lin_Ptr) {
						case 0:
							PostEvent(PANEL_MODULE, KEY_EVT_ENTER_STUDY, KEY_STUDY_PANEL);
							break;
						case 1:
						case 2:
							PostEvent(PANEL_MODULE, KEY_EVT_EXIT_STUDY, *Read_Lin_Ptr);
							break;
						default:
							break;
					}
					break;


				case SUBID_PANEL_SEND_UER_KEY:
					{
						u8 key_code_short;
						u8 key_code_long;
						key_code_short = *Read_Lin_Ptr;
						++Read_Lin_Ptr;
						key_code_long = *Read_Lin_Ptr;
						PostEvent(PANEL_MODULE, KEY_EVT_STUDY_KEY_CODE, WORD(key_code_long, key_code_short));
					}
					break;	

				case SUBID_PANEL_OVERLAP_CMD:
					if (2==(*Read_Lin_Ptr)) {
						PostEvent(PANEL_MODULE, KEY_EVT_CLEAR_KEYS, KEY_STUDY_PANEL);
					}
					break;


				case SUBID_PANEL_INFO_REQ:
					switch (*Read_Lin_Ptr) {
						case 0:
							PostEvent(PANEL_MODULE, KEY_EVT_QUERY_KEY_CODES, KEY_STUDY_PANEL);
							break;
					}
					break;	

				case SUBID_PANEL_DEFCONFIG_SET:
					panel_key_set_defconfig(*Read_Lin_Ptr);
					break;	

				default:
					break;	
			}
			break;
		case MCU_RX_SYSTEM_SECRET_SETTING_ENENT:
			switch(Sub_ID_temp)
			{
				case SUBID_SECRET_SETTING_INFO_REQ:
					switch(*Read_Lin_Ptr)
					{
						case 0x1:
							PostEvent(WINCE_MODULE, TX_TO_GUI_SECRET_SETTING_TOTAL_INFO,NONE);
							break;
						case 0x04:
							PostEvent(WINCE_MODULE, TX_TO_GUI_SECRET_SETTING_BREAK_DET_SET_INFO,g_sys_info_store.brake_mode);	
							break;
						case 0xFF:
							audio_set_mute(AUDIO_MUTE_SYSTEM, TRUE);
							NVIC_SystemReset();	// force reset 
							//PostEvent(WINCE_MODULE, TX_TO_GUI_SECRET_SETTING_TOTAL_INFO,NONE);
							break;
					}
					break;

				case SUBID_SECRET_SETTING_BREAK_DET_SET:
					if((*Read_Lin_Ptr!= g_sys_info_store.brake_mode) && (*Read_Lin_Ptr<=PARKING_SERIAL_MODEL))
					{
						g_sys_info_store.brake_mode = *Read_Lin_Ptr;
						//F_PARKING=0;//重新检测
						ak_flash_save_info();
					}
					PostEvent(WINCE_MODULE, TX_TO_GUI_SECRET_SETTING_BREAK_DET_SET_INFO,g_sys_info_store.brake_mode);					
					break;
					
				default:
					break;					
			}
			break;
		case MCU_RX_GROUP_DVD:
#ifdef DVD_FUNCTION_ENABLE
			switch (Sub_ID_temp) {
				case SUBID_DVD_OPS:
					switch (*Read_Lin_Ptr) {
						case 1:
							PostEvent(DVDC_MODULE, EVT_DVD_PWR_CTRL, 1);
							break;
						case 2:
							PostEvent(DVDC_MODULE, EVT_DVD_PWR_CTRL, 0);
							break;
						case 3:
							PostEvent(DVDC_MODULE, EVT_DVD_DISC_CTRL, 1);
							break;
						case 4:
							PostEvent(DVDC_MODULE, EVT_DVD_DISC_CTRL, 0);
							break;
						case 0x0F:
							PostEvent(DVDC_MODULE, EVT_DVD_FORCE_DISC_EJECT, 0);
							break;
						default:
							break;
					}
					break;
				case SUBID_DVD_QUERY:
					switch (*Read_Lin_Ptr) {
						case 0:
							PostEvent(WINCE_MODULE, TX_TO_GUI_DVD_PWR_INFO, NONE);
							PostEvent(WINCE_MODULE, TX_TO_GUI_DVD_DISC_INFO, NONE);
							break;
						case 1:
							PostEvent(WINCE_MODULE, TX_TO_GUI_DVD_PWR_INFO, NONE);
							break;
						case 2:
							PostEvent(WINCE_MODULE, TX_TO_GUI_DVD_DISC_INFO, NONE);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
#endif
			break;
		case MCU_RX_AUTH_EVENT:
			switch (Sub_ID_temp) {
				case SUBID_AUTH_REQ_SIGN:
					System_SetAuthChallenge(Read_Lin_Ptr,length);
					PostEvent(WINCE_MODULE, TX_TO_GUI_SEND_AUTH_SIGNATURE,NONE);					
					break;
				default:
					break;
			}
			break;
		default:
		break;
	}
}



void LinUart_Ack_Rx(void)
{
	if(Lin_Send_Table.Lin_Tx_Seq_Num==Lin_Rx_Seq_Num)
	{
		if(Lin_Send_Table.Ptr_Send_Buff==Lin_Tx_Buff)
		{
			F_Lin_TxBuff_Full=0;
			LinUart_ReSend_Timer=0;
		}
		else if(Lin_Send_Table.Ptr_Send_Buff==Lin_Tx_Buff2)
		{
			F_Lin_TxBuff2_Full=0;
			LinUart_ReSend_Timer2=0;												
		}	
	}
	else//Lin_Rx_Seq_Num is the early packet
	{
		if(Lin_Send_Table.Ptr_Send_Buff==Lin_Tx_Buff)
		{
			F_Lin_TxBuff2_Full=0;
			LinUart_ReSend_Timer2=0;					
		}
		else if(Lin_Send_Table.Ptr_Send_Buff==Lin_Tx_Buff2)
		{
			F_Lin_TxBuff_Full=0;
			LinUart_ReSend_Timer=0;												
		}
	}
}

void LinUart_Tx(void)
{
	uint16_t timeout;
	Lin_Tx_Ptr=Lin_Send_Table.Ptr_Send_Buff;
	if(Lin_Tx_Ptr==Lin_Tx_Buff)
	{
		while(Lin_Tx_Length>0)
		{
			USART_SendData(HOST_COMM_UART, *Lin_Tx_Ptr);
			for (timeout=400; timeout>0; timeout--) {
				CLEAR_WATCHDOG;
				if (RESET != USART_GetFlagStatus(HOST_COMM_UART, USART_FLAG_TXC)) {
					break;
				}
			}
			Lin_Tx_Ptr++;
			Lin_Tx_Length--;
		}
		LinUart_ReSend_Timer=T200MS_4;
	}
	else if(Lin_Tx_Ptr==Lin_Tx_Buff2)
	{
		while(Lin_Tx_Length2>0)
		{	
			USART_SendData(HOST_COMM_UART, *Lin_Tx_Ptr);
			for (timeout=400; timeout>0; timeout--) {
				CLEAR_WATCHDOG;
				if (RESET != USART_GetFlagStatus(HOST_COMM_UART, USART_FLAG_TXC)) {
					break;
				}
			}
			Lin_Tx_Ptr++;
			Lin_Tx_Length2--;
		}
		LinUart_ReSend_Timer2=T200MS_4;
	}
}

void LinUart_Ack_Tx(uchar ackchar,uchar Seqence_num)
{
	uchar checksum;
	uint16_t timeout;
	
	USART_SendData(HOST_COMM_UART, LIN_ADDRESS_MCU);
	checksum=LIN_ADDRESS_MCU;
	for (timeout=400; timeout>0; timeout--) {
		CLEAR_WATCHDOG;
		if (RESET != USART_GetFlagStatus(HOST_COMM_UART, USART_FLAG_TXC)) {
			break;
		}
	}

	USART_SendData(HOST_COMM_UART, LIN_ADDRESS_WINCE);
	checksum^=LIN_ADDRESS_WINCE;
	for (timeout=400; timeout>0; timeout--) {
		CLEAR_WATCHDOG;
		if (RESET != USART_GetFlagStatus(HOST_COMM_UART, USART_FLAG_TXC)) {
			break;
		}
	}
	
	USART_SendData(HOST_COMM_UART, 6);//length
	checksum^=6;
	for (timeout=400; timeout>0; timeout--) {
		CLEAR_WATCHDOG;
		if (RESET != USART_GetFlagStatus(HOST_COMM_UART, USART_FLAG_TXC)) {
			break;
		}
	}
	
	USART_SendData(HOST_COMM_UART, Seqence_num);
	checksum^=Seqence_num;
	for (timeout=400; timeout>0; timeout--) {
		CLEAR_WATCHDOG;
		if (RESET != USART_GetFlagStatus(HOST_COMM_UART, USART_FLAG_TXC)) {
			break;
		}
	}

	USART_SendData(HOST_COMM_UART, ackchar);
	checksum^=ackchar;
	for (timeout=400; timeout>0; timeout--) {
		CLEAR_WATCHDOG;
		if (RESET != USART_GetFlagStatus(HOST_COMM_UART, USART_FLAG_TXC)) {
			break;
		}
	}
	
	USART_SendData(HOST_COMM_UART, checksum);
	for (timeout=400; timeout>0; timeout--) {
		CLEAR_WATCHDOG;
		if (RESET != USART_GetFlagStatus(HOST_COMM_UART, USART_FLAG_TXC)) {
			break;
		}
	}

}
 
ext void LinUart_Data_Analyse(void)
{
	if(F_Lin_Rx_Data_Ready==1)
	{
		uchar *Read_Ptr;
		F_Lin_Rx_Data_Ready=0;
		if(F_Lin_RxBuff_Full==1)
		{
			Read_Ptr=Lin_Rx_Buff+2;
			Lin_Command_Check(Read_Ptr);	
			F_Lin_RxBuff_Full=0;
		}
		if(F_Lin_RxBuff2_Full==1)
		{
			Read_Ptr=Lin_Rx_Buff2+2;
			Lin_Command_Check(Read_Ptr);			
			F_Lin_RxBuff2_Full=0;
		}
	}
  }

ext void LinUart_Resend_Time(void)	// 4ms 时基
{

	if(LinUart_ReSend_Timer!=0)
	{
		LinUart_ReSend_Timer--;
		if(LinUart_ReSend_Timer==0)
		{
			Lin_ReSend_Counter++;
			if(Lin_ReSend_Counter>3)//已重发3次
			{	//重发3次后，丢弃
				F_Lin_TxBuff_Full=0;
				Lin_ReSend_Counter=0;
			}
			else
			{	//重发一包数据
				Lin_Send_Table.Ptr_Send_Buff=Lin_Tx_Buff;
				LinUart_Tx();
				Lin_Tx_Length=Lin_Tx_Length_bak;
			}
		
		}
	}
	if(LinUart_ReSend_Timer2!=0)
	{
		LinUart_ReSend_Timer2--;
		if(LinUart_ReSend_Timer2==0)
		{
			Lin_ReSend_Counter2++;
			if(Lin_ReSend_Counter2>3)
			{	//重发3次后，丢弃
				F_Lin_TxBuff2_Full=0;
				Lin_ReSend_Counter2=0;
			}
			else
			{	//重发一包数据
				Lin_Send_Table.Ptr_Send_Buff=Lin_Tx_Buff2;
				LinUart_Tx();
				Lin_Tx_Length2=Lin_Tx_Length2_bak;
			}
		
		}
	}	
	
}

const uchar Mcu_Tx_Comd_Matrix[][4]=
{
	// cmd							GroupID					SubID						Datalength
	NO_EVT,							0,						0,							0,
	TX_TO_GUI_SHORT_KEY_EVENT,		MCU_TX_COMMON_INFO,	SHORT_KEY_EVENT,				1,	
	TX_TO_GUI_FRONT_SOURCE_INFO,		MCU_TX_COMMON_INFO,	FRONT_SOURCE_INFO,			1,
	TX_TO_GUI_REAR_L_SOURCE_INFO,		MCU_TX_COMMON_INFO,	REAR_L_SOURCE_INFO,			1,
	TX_TO_GUI_BRIGHTNESS_INFO,		MCU_TX_COMMON_INFO,	BRIGHTNESS_INFO,				1,
	TX_TO_GUI_LONG_KEY_EVENT,		MCU_TX_COMMON_INFO,	LONG_KEY_EVENT,				1,
	TX_TO_GUI_POWER_CMD,			MCU_TX_COMMON_INFO,	TX_POWER_CMD,				1,
	TX_TO_GUI_REAR_R_SOURCE_INFO,   MCU_TX_COMMON_INFO,    REAR_R_SOURCE_INFO,			1,
	TX_TO_GUI_WINCE_WORK_STATUS,   MCU_TX_COMMON_INFO,		WINCE_WORK_STATUS,		1,
	TX_TO_GUI_CAR_PARKING_STATUS,			MCU_TX_COMMON_INFO,	CAR_STATUS,					1,
	TX_TO_GUI_TRANSMIT_CAN_INFO,			MCU_TX_CAN_INFO,	TX_CAN_DATA,		1,
	TX_TO_GUI_TINY_RAM_INFO,			MCU_TX_COMMON_INFO,		TX_TINY_RAM_INFO,		4,
	TX_TO_GUI_RTC_INFO,			MCU_TX_COMMON_INFO,		TX_RTC_INFO,		4,
	
	TX_TO_GUI_RADIO_TOTAL_INFO,		MCU_TX_RADIO_INFO,		TOTAL_RADIO_INFO,			1,	
	TX_TO_GUI_RADIO_CURRENT_INFO,	MCU_TX_RADIO_INFO,		CURRENT_RADIO_INFO,			7,
	TX_TO_GUI_RADIO_FREQENCY_LIST_INFO,	MCU_TX_RADIO_INFO,	FREQENCY_LIST_INFO,			1,
	TX_TO_GUI_RADIO_SET_INFO	,		MCU_TX_RADIO_INFO,		RADIO_SET_INFO,				1,

#if RDS_FUNCTION==1
	TX_TO_GUI_RDS_TOTAL_INFO,		MCU_TX_RDS_INFO,			TX_RDS_TOTAL_INFO,			75,
	TX_TO_GUI_RDS_FLAG_INFO,			MCU_TX_RDS_INFO,			TX_RDS_FLAG_INFO,			2,
	TX_TO_GUI_RDS_PTY_INFO,			MCU_TX_RDS_INFO,			TX_RDS_PTY_INFO,				2,
	TX_TO_GUI_RDS_PS_INFO,			MCU_TX_RDS_INFO,			TX_RDS_PS_INFO,				8,
	TX_TO_GUI_RDS_RT_INFO,			MCU_TX_RDS_INFO,			TX_RDS_RT_INFO,				64,
	TX_TO_GUI_RDS_PSLIST_INFO,		MCU_TX_RDS_INFO,			TX_RDS_PSLIST_INFO,			1,
#endif

	TX_TO_GUI_AUDIO_TOTAL_AUDIO_INFO,MCU_TX_AUDIO_INFO,		TX_AUDIO_TOTAL_AUDIO_INFO,	7+7+16+1+2,
	TX_TO_GUI_AUDIO_ASP_INFO,		MCU_TX_AUDIO_INFO,		TX_AUDIO_ASP_INFO,			7,
	TX_TO_GUI_AUDIO_EQUALIZER_INFO,	MCU_TX_AUDIO_INFO,		TX_AUDIO_EQUALIZER_INFO,		7,
	TX_TO_GUI_AUDIO_EQ_INFO2,	MCU_TX_AUDIO_INFO,		TX_AUDIO_EQ_INFO2,	6,
	TX_TO_GUI_AUDIO_VOLUME_INFO,		MCU_TX_AUDIO_INFO,		TX_AUDIO_VOLUME_INFO,		2,
	TX_TO_GUI_AUDIO_FLAG_INFO,		MCU_TX_AUDIO_INFO,		TX_AUDIO_FLAG_INFO,			2,
	TX_TO_GUI_AUDIO_VOL_CTRL_INFO_WHEN_REVERSE,		MCU_TX_AUDIO_INFO,		TX_AUDIO_VOL_CTRL_INFO_WHEN_REVERSE,			1,
	TX_TO_GUI_AUDIO_CUR_MAX_OUTPUT_VOL,		MCU_TX_AUDIO_INFO,		TX_AUDIO_CUR_MAX_OUTPUT_VOL,			1,
	TX_TO_GUI_AUDIO_CHANNEL_DEF_GAIN,		MCU_TX_AUDIO_INFO,		TX_AUDIO_CHANNEL_DEF_GAIN,			1,
	TX_TO_GUI_AUDIO_PROCESSOR_TYPE,		MCU_TX_AUDIO_INFO,		TX_AUDIO_PROCESSOR_TYPE,			1,
	TX_TO_GUI_AUDIO_DSP_SETTINGS,		MCU_TX_AUDIO_INFO,		TX_AUDIO_DSP_SETTINGS,			1,
	TX_TO_GUI_AUDIO_DSP_SETTINGS_2,		MCU_TX_AUDIO_INFO,		TX_AUDIO_DSP_SETTINGS_2,			1,
	TX_TO_GUI_AUDIO_SA_INFO,			MCU_TX_AUDIO_INFO,		TX_AUDIO_SA_INFO,		1,
	TX_TO_GUI_SYSTEM_TIME_INFO,		MCU_TX_SYSTEM_INFO,		SYSTEM_TIME_INFO,			3,
	TX_TO_GUI_SYSTEM_SET_INFO,		MCU_TX_SYSTEM_INFO,		SYSTEM_SET_INFO,				7,
	TX_TO_GUI_SYSTEM_DET_INFO,		MCU_TX_SYSTEM_INFO,		SYSTEM_DET_INFO,				1,
	TX_TO_GUI_SYSTEM_BEEP_INFO,	MCU_TX_SYSTEM_INFO,		SYSTEM_BEEP_CONTROL_INFO,		1,
	TX_TO_GUI_SYSTEM_RADIO_REGION_INFO,MCU_TX_SYSTEM_INFO,	SYSTEM_RADIO_REGION_INFO,		1,	
	TX_TO_GUI_SYSTEM_BREAK_SET_INFO,MCU_TX_SYSTEM_INFO,		SYSTEM_BREAK_DET_SET_INFO,		1,	
	TX_TO_GUI_SYSTEM_ILLUMIN_SET_INFO,MCU_TX_SYSTEM_INFO,		SYSTEM_ILLUMIN_SET_INFO,		1,
	TX_TO_GUI_SYSTEM_NAVI_MUSIC_SET_INFO,MCU_TX_SYSTEM_INFO, SYSTEM_NAVI_MUSIC_SET_INFO,		1,
	TX_TO_GUI_SYSTEM_PANEL_LED_COLOR, MCU_TX_SYSTEM_INFO,	SYSTEM_PANEL_LED_COLOR,		1,
	TX_TO_GUI_SYSTEM_ALL_KEY_INFOS, MCU_TX_SYSTEM_INFO,	SYSTEM_ALL_KEY_INFOS,		1,
	TX_TO_GUI_SYSTEM_NAVI_MIX_INFO, MCU_TX_SYSTEM_INFO,	SYSTEM_NAVI_MIX_INFO,		1,
	TX_TO_GUI_SYSTEM_LED_ONOFF_INFO, MCU_TX_SYSTEM_INFO,	SYSTEM_LED_ONOFF_INFO,		1,
	TX_TO_GUI_SYSTEM_VCOM_INFO, MCU_TX_SYSTEM_INFO,	SYSTEM_VCOM_INFO,		1,
	TX_TO_GUI_IAP_START_CONFIRM_KEY,	MCU_TX_REFLASH_INFO,		IAP_START_CONFIRM_KEY,		0,
	TX_TO_GUI_IAP_REQUEST_UNLOCK_KEY,MCU_TX_REFLASH_INFO,	IAP_REQUEST_UNLOCK_KEY,		0,	
	TX_TO_GUI_IAP_REQUEST_DATA,		MCU_TX_REFLASH_INFO,		IAP_REQUEST_DATA,			2,
	TX_TO_GUI_IAP_REQUEST_CHECKSUM,	MCU_TX_REFLASH_INFO,		IAP_REQUEST_CHECKSUM,		0,
	TX_TO_GUI_IAP_REQUEST_RESET,		MCU_TX_REFLASH_INFO,		IAP_REQUEST_RESET,			0,
	
	TX_TO_GUI_MCU_VERSION,			MCU_TX_COMMON_INFO,	TX_MCU_VERSION,				sizeof(MCU_VERSION),
	
	TX_TO_GUI_SWC_MODE_INFO,		MCU_TX_SWC_INFO,		SWC_MODE_INFO,				1,
	TX_TO_GUI_SWC_CUR_KEY_INFO,		MCU_TX_SWC_INFO,		SWC_KEY_CUR_KEY_INFO,			1,
	TX_TO_GUI_SWC_REPEAT_KEY_INFO,	MCU_TX_SWC_INFO,		SWC_REPEAT_KEY_INFO,			1,
	TX_TO_GUI_SWC_KEY_CODES,			MCU_TX_SWC_INFO,		SWC_KEY_INFO,				1,
	TX_TO_GUI_SWC_KEY_TEST_INFO,	MCU_TX_SWC_INFO,		SWC_KEY_TEST_INFO,			1,	

	TX_TO_GUI_PANEL_MODE_INFO,		MCU_TX_PANEL_INFO,		PANEL_MODE_INFO,			1,
	TX_TO_GUI_PANEL_CUR_KEY_INFO,	MCU_TX_PANEL_INFO,		PANEL_KEY_CUR_KEY_INFO,			1,	
	TX_TO_GUI_PANEL_REPEAT_KEY_INFO,MCU_TX_PANEL_INFO,		PANEL_REPEAT_KEY_INFO,		1,
	TX_TO_GUI_PANEL_KEY_CODES,		MCU_TX_PANEL_INFO,		PANEL_KEY_INFO,				1,
	TX_TO_GUI_PANEL_KEY_TEST_INFO,	MCU_TX_PANEL_INFO,		PANEL_KEY_TEST_INFO,		1,

	TX_TO_GUI_SECRET_SETTING_TOTAL_INFO,		MCU_TX_SECRET_SETTING_INFO,SECRET_TOTAL_INFO	,			9,
	TX_TO_GUI_SECRET_SETTING_BREAK_DET_SET_INFO,MCU_TX_SECRET_SETTING_INFO,SECRET_BREAK_DET_SET_INFO	,	1,	
	TX_TO_GUI_SECRET_SETTING_RADIO_REGION,MCU_TX_SECRET_SETTING_INFO,SECRET_RADIO_RAGION_INFO,			1,
	TX_TO_GUI_SECRET_SETTING_CAR_SUPPPRT_INFO,MCU_TX_SECRET_SETTING_INFO,SECRET_CAR_SURPPORT_INFO,	1,
	TX_TO_GUI_DVD_PWR_INFO, MCU_TX_DVD, MCU_TX_DVD_PWR_INFO, 1,
	TX_TO_GUI_DVD_DISC_INFO, MCU_TX_DVD, MCU_TX_DVD_DISC_INFO, 1,
	TX_TO_GUI_SEND_AUTH_SIGNATURE, MCU_TX_AUTH_INFO, AUTH_SEND_SIGNATRUE,	0,
};
ext void LinTxWince_Service(void)
{
	uchar i;

	uchar *ptr2;
	static uchar length=0;
	EVENT *nEvt;
	uchar GroupID,SubID;
	u32 tmp;
	bool invalid_rds_ps;
	u8 rds_ps_alpha_upper;
	u8 rds_ps_alpha_lower;
	u8 rds_ps_digital;
	u8 rds_ps_symbol;

	if(!SYSTEM_POWER_IS_ON())
	{
		return;
	}

	if(g_radio_info.flag.field.F_RADIO_SEEK)
	{	
		F_Lin_TxBuff_Full=0;
		F_Lin_TxBuff2_Full=0;
		LinUart_ReSend_Timer=0;
		Lin_ReSend_Counter=0;	//重发计数器
		LinUart_ReSend_Timer2=0;
		Lin_ReSend_Counter2=0;	//重发计数器
	}
	if(F_Lin_TxBuff_Full==1&&F_Lin_TxBuff2_Full==1)
		return;
	nEvt=GetEvent(WINCE_MODULE);

	if(nEvt->ID==NO_EVT)
	{
		USART_Transmit_Rx_Buff_full = 0;
		return;
	}		
	
	for(i=0;i<TX_TO_GUI_MAX_CMD;i++)
	{
		if(nEvt->ID==Mcu_Tx_Comd_Matrix[i][MCU_CMD_INDEX])
			break;
	}
	if(i<TX_TO_GUI_MAX_CMD)
	{
		length =Mcu_Tx_Comd_Matrix[i][LENGTH_INDEX];
		GroupID=Mcu_Tx_Comd_Matrix[i][GROUPID_INDEX];
		SubID=Mcu_Tx_Comd_Matrix[i][SUBID_INDEX];
		ptr2=buff;
	}
	else
	{
		return;
	}
	switch(nEvt->ID)
	{
		case TX_TO_GUI_SHORT_KEY_EVENT:
			{
				u8 index = MSB(nEvt->prm);
				buff[0]=LSB(nEvt->prm);
				if ( (index != 0xFF) && (index<g_key_info_store.key_num) ) {
					buff[1] = g_key_info_store.key[index].adc_channel;
					buff[2] = g_key_info_store.key[index].adc_value;
					buff[3] = g_key_info_store.key[index].swc_pu_type;
					length = 4;
				}
			}
			break;
		case TX_TO_GUI_LONG_KEY_EVENT:	
		case TX_TO_GUI_WINCE_WORK_STATUS:	
		case TX_TO_GUI_CAR_PARKING_STATUS:	
		case TX_TO_GUI_SYSTEM_RADIO_REGION_INFO:
		case TX_TO_GUI_SYSTEM_BEEP_INFO:
		case TX_TO_GUI_SYSTEM_BREAK_SET_INFO:
		case TX_TO_GUI_SYSTEM_ILLUMIN_SET_INFO:
		case TX_TO_GUI_SYSTEM_NAVI_MUSIC_SET_INFO:
			buff[0]=nEvt->prm;			
			break;

		case TX_TO_GUI_POWER_CMD:
			if (WCE_POWER_ACC_WAIT_TIMEOUT == nEvt->prm) {
				buff[0] = WCE_POWER_ACC_WAIT_TIMEOUT;
				buff[1] = MSB(g_acc_wait_timeout);
				buff[2] = LSB(g_acc_wait_timeout);
				length = 3;
			} else if (WCE_POWER_BATT_ADC_VAL == nEvt->prm) {
				buff[0] = WCE_POWER_BATT_ADC_VAL;
				buff[1] = g_saved_batt_adc_val;
				length = 2;
			} else {
				buff[0]=nEvt->prm;
				length = 1;
			}
			break;

		case TX_TO_GUI_BRIGHTNESS_INFO:	
			buff[0]=nEvt->prm;
			if (buff[0]>127) {
				buff[0] = 0;
			}
			buff[1]=0;
			if (F_ILLUMIN_CONTORL_MODE == ILLUMIN_NIGHT_MODE) {
				buff[1]=1;
			} else if (F_ILLUMIN_CONTORL_MODE == ILLUMIN_DAY_MODE) {
				buff[1]=2;
			} else if (F_ILLUMIN_CONTORL_MODE == ILLUMIN_AUTO_MODE) {
				if (Det_Flag.field.F_Lighting_Det==1) {
					buff[1]=4;
				} else {
					buff[1]=3;
				}
			}
			length = 2;
			break;
			
		case TX_TO_GUI_FRONT_SOURCE_INFO:				
		case TX_TO_GUI_REAR_L_SOURCE_INFO:	
		case TX_TO_GUI_REAR_R_SOURCE_INFO:
			buff[0]=nEvt->prm;			
			break;	

		case TX_TO_GUI_TINY_RAM_INFO:
			length = system_get_tiny_ram_size();
			system_get_tiny_ram(buff,length);
			break;

		case TX_TO_GUI_RTC_INFO:
			length = 4;
			tmp = rtc_get_seconds();
			buff[0] = (tmp>>24)&0xFF;
			buff[1] = (tmp>>16)&0xFF;
			buff[2] = (tmp>>8)&0xFF;
			buff[3] = (tmp>>0)&0xFF;
			break;
			
		case TX_TO_GUI_RADIO_TOTAL_INFO:		//5
			length = radio_comm_fill_total_info(buff);
			break;
		case TX_TO_GUI_RADIO_CURRENT_INFO:
			length = radio_comm_fill_cur_info(buff);
			break;
		case TX_TO_GUI_RADIO_FREQENCY_LIST_INFO:
			length = radio_comm_fill_preset_info(buff);
			break;
		case TX_TO_GUI_RADIO_SET_INFO:	
			buff[0]=g_radio_area;
			break;
#if RDS_FUNCTION==1		
		case TX_TO_GUI_RDS_FLAG_INFO:
			buff[0]=0x00;
			if (g_rds_info.tp_on)
				buff[0] |= (1<<7);
			if (g_rds_info.ta_on)
				buff[0] |= (1<<1);
			if (g_rds_info.af_switch_on)
				buff[0] |= (1<<6);
			if (g_rds_info.ta_switch_on)
				buff[0] |= (1<<5);
			if (0!=g_rds_info.pty_select)
				buff[0] |= (1<<4);
			length=1;
			break;
		case	TX_TO_GUI_RDS_PTY_INFO:
			buff[0]=g_rds_info.pty_select;
			buff[1]=g_rds_info.pty;
			length=2;
			break;
		case TX_TO_GUI_RDS_PS_INFO:
			invalid_rds_ps = FALSE;
			rds_ps_alpha_upper = 0;
			rds_ps_alpha_lower = 0;
			rds_ps_digital = 0;
			rds_ps_symbol = 0;
			for (i=0; i<RDS_PS_CHAR_NR; i++) {
				buff[i] = g_rds_info.ps[i];
				/* check PS content */
				if ((buff[i]>='0')&&(buff[i]<='9')) {
					++rds_ps_digital;
				} else if ((buff[i]>='a')&&(buff[i]<='z')) {
					++rds_ps_alpha_lower;
				} else if ((buff[i]>='A')&&(buff[i]<='Z')) {
					++rds_ps_alpha_upper;
				} else if (buff[i]==0x20) {     /* ' ' */
					++rds_ps_symbol;
				} else if (buff[i]==0x27) {     /* ' */
					++rds_ps_symbol;
				} else if ((buff[i]>=0x2C)&&(buff[i]<=0x2F)) {          /* ,-./ */
					++rds_ps_symbol;
				} else {
					invalid_rds_ps = TRUE;
				}
			}
			if ((rds_ps_digital+rds_ps_alpha_upper)<3) {
				invalid_rds_ps = TRUE;
			}
			if (invalid_rds_ps) {
				// clear invalid PS
				for (i=0; i<RDS_PS_CHAR_NR; i++) {
					buff[i] = 0;
				}
			}
			length = RDS_PS_CHAR_NR;
			break;
		case	TX_TO_GUI_RDS_RT_INFO:
			if (0==nEvt->prm) {
				length = rds_fill_rt_info(buff, TRUE);
			} else {
				length = rds_fill_rt_info(buff, FALSE);
			}
			break;
//		case	TX_TO_GUI_RDS_PSLIST_INFO:
//			break;
#endif
		case TX_TO_GUI_AUDIO_ASP_INFO:
			length = 0;
			break;
		case TX_TO_GUI_AUDIO_EQUALIZER_INFO:
			buff[0] = g_audio_info.eq_mode;
			buff[1] = g_audio_info.subwoofer;
			buff[2] = 0x07;	// for legacy
			buff[3] = 0x07;	// for legacy
			buff[4] = g_audio_info.loudness;
			buff[5] = g_audio_info.fader;
			buff[6] = g_audio_info.balance;
			for (i=0; i<EQ_FREQ_NUMS; i++) {
				buff[7+i] = g_audio_info.eq_visible_level[i];
			}
			length =7+EQ_FREQ_NUMS;				
			break;	
		case TX_TO_GUI_AUDIO_EQ_INFO2:	
			length = 0;
			break;

		case TX_TO_GUI_AUDIO_VOLUME_INFO:					
			buff[0]=audio_get_volume();
			buff[1]=MAX_VOLUME;
			buff[2]=nEvt->prm;
			length = 3;
			break;

		case TX_TO_GUI_AUDIO_FLAG_INFO:	//15
			buff[0] = 0; 
			if (g_audio_info.mute & AUDIO_MUTE_USER) {
				buff[0] |= (1<<4);
			}
			buff[1] = 0;
			break;

		case TX_TO_GUI_AUDIO_VOL_CTRL_INFO_WHEN_REVERSE:
			buff[0] = g_sys_info_store.vol_ctrl_when_reverse;
			length = 1;
			break;
			
		case TX_TO_GUI_AUDIO_CUR_MAX_OUTPUT_VOL:
			buff[0] = g_audio_info.output_type;
			length = 1;
			break;

		case TX_TO_GUI_AUDIO_CHANNEL_DEF_GAIN:
			buff[0] = A_SRC_HOST_EXTRA_GAIN_ATTEN+10;
			buff[1] = A_SRC_RADIO_EXTRA_GAIN_ATTEN+10;
			buff[2] = A_SRC_DVD_EXTRA_GAIN_ATTEN+10;
			buff[3] = A_SRC_BT_MODULE_EXTRA_GAIN_ATTEN+10;
			buff[4] = A_SRC_AUXIN_EXTRA_GAIN_ATTEN+10;
			buff[5] = A_SRC_TV_EXTRA_GAIN_ATTEN+10;
			length = 6;
			break;

		case TX_TO_GUI_AUDIO_PROCESSOR_TYPE:
#if ASP_MODEL==ASP_BU32107
			buff[0] = 1;
#else
			buff[0] = 0;
#endif
			length = 1;
			break;

		case TX_TO_GUI_AUDIO_DSP_SETTINGS:
#if ASP_MODEL==ASP_BU32107
			buff[0] = g_audio_info.loud_on;
			buff[1] = g_audio_info.hpf_on;
			buff[2] = g_audio_info.hpf_freq;
			buff[3] = g_audio_info.phat_en;
			buff[4] = g_audio_info.phat_gain;
			buff[5] = g_audio_info.dsp_bass_on;
			buff[6] = g_audio_info.dsp_bass_freq;
			buff[7] = g_audio_info.dsp_bass_gain;
			buff[8] = g_audio_info.core_en;
			buff[9] = g_audio_info.core_gain;
			buff[10] = g_audio_info.space_en;
			buff[11] = g_audio_info.space_gain;
			buff[12] = g_audio_info.subwoofer_on;
			length = 13;
#endif
			break;

		case TX_TO_GUI_AUDIO_SA_INFO:
#if ASP_MODEL==ASP_BU32107
			for (i=0; i<SPECTRUM_NUMS; i++) {
				buff[i] = g_audio_info.sa_data[i];
			}
			length = SPECTRUM_NUMS;
#endif
			break;

		case TX_TO_GUI_AUDIO_DSP_SETTINGS_2:
#if ASP_MODEL==ASP_BU32107
			buff[0] = g_audio_info.sf_mode;
			for (i=0; i<AUDIO_SPK_NUMS; i++) {
				buff[1+3*i] = g_audio_info.spk_on[i];
				buff[2+3*i] = g_audio_info.spk_gain[i];
				buff[3+3*i] = g_audio_info.spk_delay[i];
			}
			buff[13] = g_audio_info.surround_mode;
			buff[14] = g_audio_info.soundfield_expert_mode;
			length = 15;
#endif
			break;

		case TX_TO_GUI_SYSTEM_TIME_INFO:	
			buff[0]=0;
			buff[1]=0;
			buff[2]=0;
			break;
		case TX_TO_GUI_SYSTEM_SET_INFO:
			buff[0]=0;
			buff[1]=g_radio_area;
			buff[2]=g_brightness;
			buff[3]=g_sys_info_store.brake_mode;
			buff[4]=F_ILLUMIN_CONTORL_MODE;
			buff[5]=0;
			buff[6]=0;
			break;
		case TX_TO_GUI_SYSTEM_DET_INFO:
			buff[0]=F_PARKING<<7;
			break;

		case TX_TO_GUI_SYSTEM_PANEL_LED_COLOR:
			buff[0] = 0x01;
			buff[1] = g_sys_info_store.led_r_level;
			buff[2] = g_sys_info_store.led_g_level;
			buff[3] = g_sys_info_store.led_b_level;
			length = 4;
			break;

		case TX_TO_GUI_SYSTEM_ALL_KEY_INFOS:
			buff[0] = 0xFF;
			break;

		case TX_TO_GUI_SYSTEM_NAVI_MIX_INFO:
			if (NAVI_BREAK_DIRECT == g_audio_info.navi_mix_extra_gain) {
				buff[0] = 0;
			} else {
				buff[0] = g_audio_info.navi_mix_extra_gain;
			}
			length = 1;
			break;

		case TX_TO_GUI_SYSTEM_LED_ONOFF_INFO:
			buff[0] = g_sys_info_store.led_always_on;
			length = 1;
			break;

		case TX_TO_GUI_SYSTEM_VCOM_INFO:
			buff[0] = g_sys_info_store.vcom_value;
			length = 1;
			break;


		case TX_TO_GUI_MCU_VERSION:
			{
				u8 cnt, len;
				char* p=MCU_VERSION;
				len = sizeof(MCU_VERSION);
#ifdef MCU_FIXED_VERSION
				if (0==nEvt->prm) {
					p = MCU_FIXED_VERSION;
					len = sizeof(MCU_FIXED_VERSION);
				}
#endif
				length = 0;
				for (cnt=0; cnt<len; cnt++) {
					if (' '==p[cnt]) {
					} else if (':'==p[cnt]) {
					} else {
						buff[length++]=p[cnt];
					}
				}
#if TUNER_MODEL==TUNER_NXP_TEA668X
#ifndef MCU_FIXED_VERSION
#ifdef TEA668X_V205
				length--;
				buff[length++]='_';
				buff[length++]='V';
				buff[length++]='0'+(g_tea668x_version>>8);
				buff[length++]='0';
				buff[length++]='0'+(g_tea668x_version&0xFF);
				if (0x0205 != g_tea668x_version) {
					buff[length++]='x';
				}
				buff[length++]='\0';
#elif defined(TEA668X_V102)
				length--;
				buff[length++]='_';
				buff[length++]='V';
				buff[length++]='0'+(g_tea668x_version>>8);
				buff[length++]='0';
				buff[length++]='0'+(g_tea668x_version&0xFF);
				if (0x0102 != g_tea668x_version) {
					buff[length++]='x';
				}
				buff[length++]='\0';
#endif
#endif
#endif
			}
			break;
		case  TX_TO_GUI_SWC_MODE_INFO:
			buff[0] = (uchar)nEvt->prm;
			break;
		case TX_TO_GUI_SWC_CUR_KEY_INFO:
			length = panel_key_get_cur_key_info(KEY_STUDY_SWC, (uchar)nEvt->prm, buff);
			break;
		case TX_TO_GUI_SWC_REPEAT_KEY_INFO:
			buff[0] = (uchar)nEvt->prm;
			break;
		case TX_TO_GUI_SWC_KEY_CODES:
			length = panel_key_get_all_key_codes(KEY_STUDY_SWC, buff);
			break;
		case TX_TO_GUI_SWC_KEY_TEST_INFO:
			buff[0] = (uchar)nEvt->prm;
			break;
		case  TX_TO_GUI_PANEL_MODE_INFO:
			buff[0] = (uchar)nEvt->prm;
			break;
		case TX_TO_GUI_PANEL_CUR_KEY_INFO:
			length = panel_key_get_cur_key_info(KEY_STUDY_PANEL, (uchar)nEvt->prm, buff);
			break;
		case TX_TO_GUI_PANEL_REPEAT_KEY_INFO:
			buff[0] = (uchar)nEvt->prm;
			break;
		case TX_TO_GUI_PANEL_KEY_CODES:
			length = panel_key_get_all_key_codes(KEY_STUDY_PANEL, buff);
			break;
		case TX_TO_GUI_PANEL_KEY_TEST_INFO:
			buff[0] = (uchar)nEvt->prm;
			break;			
		case TX_TO_GUI_SECRET_SETTING_TOTAL_INFO:
			buff[0]=0;
			buff[1]=0;
			buff[2]=g_sys_info_store.brake_mode;
			buff[3]=0;
			buff[4]=0;
			buff[5]=0;
			buff[6]=0;
			buff[7]=0;
			buff[8]=0;
			buff[9]=g_radio_area;
			buff[10]=0x00;			
			length = 11;
			break;	
		case TX_TO_GUI_SECRET_SETTING_BREAK_DET_SET_INFO:
		case TX_TO_GUI_SECRET_SETTING_RADIO_REGION:
		case TX_TO_GUI_SECRET_SETTING_CAR_SUPPPRT_INFO:
			buff[0]= (uchar)nEvt->prm;
			length = 1;
			break;	
		case TX_TO_GUI_TRANSMIT_CAN_INFO:
			length = (uchar)nEvt->prm;
			if(length < sizeof(buff)-7)
			{
				ak_memcpy(buff, USART_Transmit_Rx_Buff, length);
				USART_Transmit_Rx_Buff_full = 0;
			}
			break;
#ifdef DVD_FUNCTION_ENABLE
		case TX_TO_GUI_DVD_PWR_INFO:
			buff[0] = g_dvd_info.dvd_pwr_up;
			length = 1;
			break;
		case TX_TO_GUI_DVD_DISC_INFO:
			buff[0] = g_dvd_info.disc_state;
			length = 1;
			break;
#endif
		case TX_TO_GUI_SEND_AUTH_SIGNATURE:
			length = System_GetAuthSignature(buff,MAX_LIN_TX_LENGTH);
			break;
		default:
			break;
	}

	{
		uchar *LinBuf_Ptr;
		uchar *ptr2_bak;
		if(F_Lin_TxBuff_Full==0)
		{
			LinBuf_Ptr=Lin_Tx_Buff;
			F_Lin_TxBuff_Full=1;
			Lin_Tx_Length=(uchar)(length+7);	//数据包的长度
			Lin_Tx_Length_bak=Lin_Tx_Length;
		}
		else
		{
			LinBuf_Ptr=Lin_Tx_Buff2;
			F_Lin_TxBuff2_Full=1;
			Lin_Tx_Length2=(uchar)(length+7);	//数据包的长度
			Lin_Tx_Length2_bak=Lin_Tx_Length2;
		}
		Lin_Send_Table.Ptr_Send_Buff=LinBuf_Ptr;
		*LinBuf_Ptr=LIN_ADDRESS_MCU;
		Lin_Tx_Checksum=*LinBuf_Ptr;
		LinBuf_Ptr++;
		*LinBuf_Ptr=LIN_ADDRESS_WINCE;
		Lin_Tx_Checksum^=*LinBuf_Ptr;
		
		LinBuf_Ptr++;
		*LinBuf_Ptr=(uchar)(length+7);
		Lin_Tx_Checksum^=*LinBuf_Ptr;

		LinBuf_Ptr++;		
		Lin_Send_Table.Lin_Tx_Seq_Num++;
		*LinBuf_Ptr=Lin_Send_Table.Lin_Tx_Seq_Num;		
		Lin_Tx_Checksum^=*LinBuf_Ptr;
		
		LinBuf_Ptr++;
		*LinBuf_Ptr=GroupID;
		Lin_Tx_Checksum^=*LinBuf_Ptr;

		LinBuf_Ptr++;
		*LinBuf_Ptr=SubID;
		Lin_Tx_Checksum^=*LinBuf_Ptr;
		
		ptr2_bak=ptr2;
		for(i=0;i<length;i++)
		{
			LinBuf_Ptr++;		
			*LinBuf_Ptr=*ptr2_bak; 
			Lin_Tx_Checksum^=*ptr2_bak;
			ptr2_bak++;			
		}
		
		LinBuf_Ptr++;
		*LinBuf_Ptr=Lin_Tx_Checksum;
		
		if(Lin_Send_Table.Ptr_Send_Buff==Lin_Tx_Buff)
			Lin_ReSend_Counter=0;
		else
			Lin_ReSend_Counter2=0;
		
		LinUart_Tx();
		if(nEvt->ID == TX_TO_GUI_TRANSMIT_CAN_INFO)
		{
			F_Lin_TxBuff_Full=0;
			F_Lin_TxBuff2_Full=0;
			LinUart_ReSend_Timer=0;
			LinUart_ReSend_Timer2=0;
		}
	}
}

