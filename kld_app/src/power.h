#ifndef	_POWER_H_
#define _POWER_H_

#define Watchdog_Reset		0	//Watchdog引起RESET
#define B_Power_Reset		1	//B+上电引起RESET
#define Key_Reset		2	//外部按键引起RESET
#define B_Power_Blinking_Reset	3	//瞬检引起RESET

ext uint8_t	g_mcu_in_sleep;

extern void PowerInit(void);



#define POWER_SYSTEM_RESET		0
#define POWER_ON_DELAY		1
#define POWER_MMI_INITIAL		2
#define POWER_VAR_RECOVER		3
#define POWER_MOTOR_INIT		4
#define POWER_INIT_FINISH		5
#define POWER_WAIT_WINCE_SOURCE		6
#define POWER_NORMAL_RUN		8
#define POWER_CLOSE_SCREEN		9
#define POWER_MOVE_OVER			10
#define POWER_BU_OFF_DELAY			11
#define POWER_ACC_ON_DELAY		12
#define POWER_VOLTAGE_RISE		13
#define POWER_SYSTEM_STANDBY	14
#define POWER_ACC_OFF			15
#define POWER_ACCOFF1			16
#define POWER_ACCOFF2			17
#define POWER_OFF_EJECT			20
#define POWER_PANEL_OFF			21
#define POWER_SUPER_SAVE		22
#define POWER_OFF_HOLD			23
#define POWER_ACCOFF_WAIT              24
#define POWER_ACCOFF_CHECK_HOST        25

typedef enum
{
	TFT_POWER_OFF_START,
	TFT_POWER_OFF,
	TFT_POWER_ON_WAITING,
	TFT_POWER_ON,
	TFT_DEVICE_INIT,
	TFT_CCFL_ON,
	
	TFT_ALL_ON,
	TFT_STANDBY,
	TFT_IDLE
}TFT_POWER_STATE;


typedef enum
{
	NO_EVT,
	EVT_POWER_ON,
	EVT_POWER_OFF,
	EVT_POWER_SWITCH_OFF,
	EVT_MOTOR_SWITCH_OFF,
	EVT_POWER_TEMP_ON,
	EVT_POWER_TEMP_OFF,
	EVT_SWITCH_MPEG_ON,
	EVT_SAVE_STATUS,
	EVT_BEEP_OFF,
	EVT_WINCE_POWER_CMD
}POWER_EVT;

typedef enum 
{
	POWER_STANDBY,
	POWER_ON,
	POWEROFF_FROM_MANNED,
	POWEROFF_FROM_ACCOFF,
	POWEROFF_FROM_ACC_AFTER_MANNED,	
	POWEROFF_FROM_VOLTAGE,
	POWEROFF_FROM_ACC,
	POWEROFF_IN_MOVING,
	POWEROFF_FROM_MCU5V,
	POWEROFF_FROM_PANEL
}POWEROFFREASON;
ext POWEROFFREASON PowerOffReason;


ext uchar ACC_Sampling_Counter;
ext u16  Power_Timer;
ext uchar  nPowerState;
ext TFT_POWER_STATE TFT_Power_State;

extern void PowerManage(void);
extern void Voltage_Error_Detect(void);
extern void ACC_On_Detect(uchar cnt_Acc_On);
extern void ACC_Off_Detect(void);
extern void EmergencyPowerDown(void);
extern void backlight_power_ctrl(bool bPower);

ext u16 g_acc_wait_timer;
ext u16 g_acc_wait_timeout;

ext bool g_host_is_sleep;
ext bool g_kill_host;
ext void app_wd_init(void);
ext void app_wd_main(void);
ext void app_wd_feed_dog(void);
ext void app_wd_stop_dog(void);


ext bool g_app_slept;

ext u8 g_batt_adc_val;
ext u8 g_saved_batt_adc_val;
ext u8 g_batt_notify_cntr;
ext void batt_main(void);


ext u8 g_4g_pwrkey_cnt;
ext void host_pwrkey_main(void);

#endif

