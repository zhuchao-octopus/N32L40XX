
#ifndef _LINUART_WINCE_H_
#define _LINUART_WINCE_H_

#define WCE_POWER_OFF                   0x00
#define WCE_POWER_REQ                   0x01
#define WCE_POWER_ENTER_ACC_WAIT    0x02
#define WCE_POWER_EXIT_ACC_WAIT     0x03
#define WCE_POWER_ACC_WAIT_TIMEOUT  0x04
#define WCE_POWER_BATT_ADC_VAL      0x20

#define LIN_ADDRESS_MCU 0x55
#define LIN_ADDRESS_WINCE   0xAA

typedef enum
{
    LIN_RX_HEAD_CODE1=0,
    LIN_RX_HEAD_CODE2,
    LIN_RX_LENGTH,
    LIN_RX_SEQUENCE_NUM,
    LIN_RX_GROUP_ID,    
    LIN_RX_SUB_ID,  
    LIN_RX_DATA,
    LIN_RX_ACK,
    LIN_RX_CHECKSUM
}LIN_RX_STATE;
typedef enum
{
    TX_TO_GUI_SHORT_KEY_EVENT=1,        
    //refer Uicc_def.h  
    TX_TO_GUI_FRONT_SOURCE_INFO,    
    //SOURCE
    TX_TO_GUI_REAR_L_SOURCE_INFO,
    //SOURCE
    TX_TO_GUI_BRIGHTNESS_INFO,
    //MIN_Bringtness~MAX_Brightness
    TX_TO_GUI_LONG_KEY_EVENT,
    //refer Uicc_def.h  
    TX_TO_GUI_RADIO_TOTAL_INFO, //5
    //
    TX_TO_GUI_POWER_CMD,

    TX_TO_GUI_REAR_R_SOURCE_INFO,
    //SOURCE
    //SOURCE
    TX_TO_GUI_WINCE_WORK_STATUS,

    TX_TO_GUI_CAR_PARKING_STATUS,
    TX_TO_GUI_TRANSMIT_CAN_INFO,
    TX_TO_GUI_TINY_RAM_INFO,
    TX_TO_GUI_RTC_INFO,
    TX_TO_GUI_AUTO_TESTER_ACC_INFO,

    TX_TO_GUI_RADIO_CURRENT_INFO,
    TX_TO_GUI_RADIO_FREQENCY_LIST_INFO,
    TX_TO_GUI_RADIO_SET_INFO    ,
    TX_TO_GUI_RADIO_PARAMETER_INFO,
    TX_TO_GUI_RADIO_TEST_PARAMETER_INFO,
    TX_TO_GUI_RDS_TOTAL_INFO,
    TX_TO_GUI_RDS_FLAG_INFO,
    TX_TO_GUI_RDS_PTY_INFO,
    TX_TO_GUI_RDS_PS_INFO,
    TX_TO_GUI_RDS_RT_INFO,
    TX_TO_GUI_RDS_PSLIST_INFO,//10
    
    TX_TO_GUI_AUDIO_TOTAL_AUDIO_INFO,       
    TX_TO_GUI_AUDIO_ASP_INFO,
    TX_TO_GUI_AUDIO_EQUALIZER_INFO,
    TX_TO_GUI_AUDIO_EQ_INFO2,
    TX_TO_GUI_AUDIO_VOLUME_INFO,
    TX_TO_GUI_AUDIO_FLAG_INFO,//16
    TX_TO_GUI_AUDIO_SA_INFO,
    TX_TO_GUI_AUDIO_VOL_CTRL_INFO_WHEN_REVERSE,
    TX_TO_GUI_AUDIO_CUR_MAX_OUTPUT_VOL,
    TX_TO_GUI_AUDIO_CHANNEL_DEF_GAIN,
    TX_TO_GUI_AUDIO_PROCESSOR_TYPE,
    TX_TO_GUI_AUDIO_DSP_SETTINGS,
    TX_TO_GUI_AUDIO_DSP_SETTINGS_2,

    TX_TO_GUI_SYSTEM_TIME_INFO,
    TX_TO_GUI_SYSTEM_SET_INFO,
    TX_TO_GUI_SYSTEM_DET_INFO,
    TX_TO_GUI_SYSTEM_BEEP_INFO,
    TX_TO_GUI_SYSTEM_RADIO_REGION_INFO,
    TX_TO_GUI_SYSTEM_BREAK_SET_INFO,
    TX_TO_GUI_SYSTEM_ILLUMIN_SET_INFO,
    TX_TO_GUI_SYSTEM_NAVI_MUSIC_SET_INFO,
    TX_TO_GUI_SYSTEM_PANEL_LED_COLOR,
    TX_TO_GUI_SYSTEM_ALL_KEY_INFOS,
    TX_TO_GUI_SYSTEM_NAVI_MIX_INFO,
    TX_TO_GUI_SYSTEM_LED_ONOFF_INFO,
    TX_TO_GUI_SYSTEM_VCOM_INFO,
    
    TX_TO_GUI_IAP_START_CONFIRM_KEY,
    TX_TO_GUI_IAP_REQUEST_UNLOCK_KEY,//21
    
    TX_TO_GUI_IAP_REQUEST_DATA,
    TX_TO_GUI_IAP_REQUEST_CHECKSUM,
    TX_TO_GUI_IAP_REQUEST_RESET,
    TX_TO_GUI_MCU_VERSION,
    
    TX_TO_GUI_SWC_MODE_INFO,
    TX_TO_GUI_SWC_CUR_KEY_INFO,
    TX_TO_GUI_SWC_REPEAT_KEY_INFO,
    TX_TO_GUI_SWC_KEY_CODES,
    TX_TO_GUI_SWC_KEY_TEST_INFO,

    TX_TO_GUI_TV_CHANNEL_INFO,
    TX_TO_GUI_TV_STATUS_INFO,
    TX_TO_GUI_TV_SYSTEM_INFO,
    TX_TO_GUI_TV_CHANNEL_SEEK_INFO,

    TX_TO_GUI_PANEL_MODE_INFO,
    TX_TO_GUI_PANEL_CUR_KEY_INFO,
    TX_TO_GUI_PANEL_REPEAT_KEY_INFO,
    TX_TO_GUI_PANEL_KEY_CODES,
    TX_TO_GUI_PANEL_KEY_TEST_INFO,

    TX_TO_GUI_SECRET_SETTING_TOTAL_INFO,
    TX_TO_GUI_SECRET_SETTING_AUXIN_SET_INFO,
    TX_TO_GUI_SECRET_SETTING_DVD_SET_INFO,
    TX_TO_GUI_SECRET_SETTING_BREAK_DET_SET_INFO,
    TX_TO_GUI_SECRET_SETTING_ILLUMIN_DET_SET_INFO,
    TX_TO_GUI_SECRET_SETTING_RADIO_PARA_INFO,
    TX_TO_GUI_SECRET_SETTING_FAN_START_TEMP_INFO,
    TX_TO_GUI_SECRET_SETTING_RADIO_REGION,
    TX_TO_GUI_SECRET_SETTING_CAR_SUPPPRT_INFO,  

    TX_TO_GUI_DVD_ALL_INFO,
    TX_TO_GUI_DVD_PWR_INFO,
    TX_TO_GUI_DVD_DISC_INFO,

    TX_TO_GUI_SEND_AUTH_SIGNATURE,
    
    TX_TO_GUI_MAX_CMD
}TX_TO_GUI_CMD;

#define UART_RX_ACK     0xff
#define UART_NACK_NG        0xf0
#define UART_NACK_NO_SUPPORT    0xf3
#define UART_NACK_BUSY      0xfc
    
ext void LinUart_Data_Analyse(void);
ext void LinUart_Resend_Time(void);
ext void LinTxWince_Service(void);
ext void LinUart_Ack_Rx(void);
ext void LinUart_Ack_Tx(uchar ackchar,uchar Seqence_num);
ext void LinUart_Tx(void);//ѭ���ȴ�����
ext void host_rx_data(uint8_t Lin_Data);



typedef struct 
{
    unsigned    :   1;
    unsigned Lin_Rx_Data_Ready    : 1;
    unsigned Lin_RxBuff_Full    :   1;
    unsigned Lin_RxBuff2_Full   :   1;
    unsigned Lin_TxBuff_Full    :   1;
    unsigned Lin_TxBuff2_Full     : 1;
    unsigned            :   1;
    unsigned            :   1;
}_LINUART_FLAG;
typedef union 
{
      unsigned char byte;
      _LINUART_FLAG  field;
}LINUART_FLAG;

ext LINUART_FLAG LinUart_Flag;

#define F_Lin_Rx_Data_Ready     LinUart_Flag.field.Lin_Rx_Data_Ready
#define F_Lin_RxBuff_Full           LinUart_Flag.field.Lin_RxBuff_Full
#define F_Lin_RxBuff2_Full      LinUart_Flag.field.Lin_RxBuff2_Full

#define F_Lin_TxBuff_Full           LinUart_Flag.field.Lin_TxBuff_Full
#define F_Lin_TxBuff2_Full      LinUart_Flag.field.Lin_TxBuff2_Full

#endif
