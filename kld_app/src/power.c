
#include "public.h"

static u16 g_app_watchdog_cntr;
static u8 g_notify_app_awake_cntr;
static u16 g_power_timer;

// notify APP that we are wakeup.
// **** NOTICE that ******
// We SHOULD notify app every ACC OFF/ON circle!!!!  If APP
// don't send app_wd cmd to us later, we should find another method.
// ********************
static void _notify_app_awake(void)
{
	if ( (nPowerState!=POWER_ACCOFF2) && 
		(nPowerState!=POWER_ACCOFF_WAIT) &&
		(nPowerState!=POWER_ACCOFF_CHECK_HOST) &&
		(nPowerState!=POWER_ACC_OFF) ) {
		PostEvent(WINCE_MODULE, TX_TO_GUI_POWER_CMD, WCE_POWER_EXIT_ACC_WAIT);
	}
}

void app_wd_feed_dog(void)
{
	g_host_is_sleep = FALSE;

	g_app_watchdog_cntr = 1;

	if (g_notify_app_awake_cntr>0) {
		--g_notify_app_awake_cntr;
		_notify_app_awake();
	}
}

void app_wd_stop_dog(void)
{
	g_app_watchdog_cntr = 0;

	if (g_notify_app_awake_cntr>0) {
		--g_notify_app_awake_cntr;
		_notify_app_awake();
	}
}

void app_wd_init(void)
{
	g_notify_app_awake_cntr = 0;
	g_host_is_sleep = TRUE;
	g_app_watchdog_cntr = 0;
}


void app_wd_main(void)
{
	if (0 == Get_ACC_Det_Flag) {
		g_app_watchdog_cntr = 0;
		return;
	}

	if (g_app_watchdog_cntr > 0) {
		++g_app_watchdog_cntr;
		if (g_app_watchdog_cntr >= T30S_100) {
			// timeout after last feed dog
			// kill ourself
			NVIC_SystemReset();	// force reset 
		}
	}
}

void PowerInit(void)
{
	g_acc_wait_timeout = 0;         //ACC wait default off
	g_kill_host = FALSE;
	g_saved_batt_adc_val = 0;
	g_mcu_in_sleep = 0;
}

static void do_wakeup_host(void)
{
	GPIO_SetBits(GPIO_HOST_PWR_KEY_GRP, GPIO_HOST_PWR_KEY_PIN);
	delay_1ms(400);
	GPIO_ResetBits(GPIO_HOST_PWR_KEY_GRP, GPIO_HOST_PWR_KEY_PIN);

	g_notify_app_awake_cntr = 5;
}

extern void backlight_power_ctrl(bool on)
{
	if(on)
	{
		F_TFT_ON = ON;
			
	}
	else
	{
		disable_backlight();
		F_TFT_ON = OFF;
	}
}

static void System_Power_OnOff(ONOFF SW)
{
	if(SW==ON)
	{
		SystemPowerContrl(ON);
		GPIO_SetBits(GPIO_AUDIO_PWR_GRP, GPIO_AUDIO_PWR_PIN);
	}
	else
	{
		Clr_Machine_Power();
		SystemPowerContrl(OFF);
		GPIO_ResetBits(GPIO_AUDIO_PWR_GRP, GPIO_AUDIO_PWR_PIN);
		Wince_Start_Flag=0;
	}
}



 #define BU_CONFIRM_TIMER	30
 
extern void Voltage_Error_Detect(void)
{	
	static uchar BUTimerOut;
	uchar ADC_current;	
	static u16 s_volt_det_timer = 0;
	static u8 s_low_volt_cntr = 0;

	if (s_volt_det_timer < 60000) {
		++s_volt_det_timer;
	}
	if (s_low_volt_cntr < 250) {
		++s_low_volt_cntr;
	}
	if(BUTimerOut!=0)
	{
		--BUTimerOut;
	}
	ADC_current = adc_channel_sample(AD_BATT_DET);
	g_batt_adc_val = ADC_current;

	if (ADC_current<LOW_VOLT_PROTECT_OFF)
	{
		F_ErrorPower=1;
		if (s_low_volt_cntr>T500MS_12) {
			audio_set_mute(AUDIO_MUTE_BATT, TRUE);
		}
//		backlight_power_ctrl(OFF);
		if(ADC_current < VOLT_6V && F_System_need_reset==0)
		{
			audio_set_pwr_ctrl(TRUE);

			if (ADC_current < VOLT_5V) {
				F_System_need_reset=1;
				EmergencyPowerDown();
			}
		}
		if ( (POWER_NORMAL_RUN == nPowerState) && (T6S_12 <= s_volt_det_timer) ) {
			PowerOffReason=POWEROFF_FROM_VOLTAGE;
			PostEvent(POWER_MODULE,EVT_POWER_OFF,PowerOffReason);
		}
		BUTimerOut=BU_CONFIRM_TIMER;
	}
	else if(ADC_current<LOW_VOLT_PROTECT_ON)
	{	
		BUTimerOut=BU_CONFIRM_TIMER;
		s_volt_det_timer = 0;
		s_low_volt_cntr = 0;
	}
	else if(ADC_current<HIGH_VOLT_PROTECT_ON)
	{
		if(BUTimerOut==0)
		{
			BUTimerOut=0;
			F_ErrorPower=0;
			audio_set_mute(AUDIO_MUTE_BATT, FALSE);
		}
		s_volt_det_timer = 0;
		s_low_volt_cntr = 0;
	}
	else if(ADC_current<HIGH_VOLT_PROTECT_OFF)
	{	
		BUTimerOut=BU_CONFIRM_TIMER;
		s_volt_det_timer = 0;
		s_low_volt_cntr = 0;
	}
	else
	{
//		if(VoltageState!=HIGHT_ERROR||F_ErrorPower==0)
		{
			F_ErrorPower=1;
			F_System_need_reset=1;
						
			audio_set_mute(AUDIO_MUTE_BATT, TRUE);
			backlight_power_ctrl(FALSE);
			EmergencyPowerDown();
			if ( (POWER_NORMAL_RUN == nPowerState) && (T6S_12 <= s_volt_det_timer) ) {
				PowerOffReason=POWEROFF_FROM_VOLTAGE;
				PostEvent(POWER_MODULE,EVT_POWER_OFF,PowerOffReason);
			}
		}
		BUTimerOut=BU_CONFIRM_TIMER;
		s_low_volt_cntr = 0;
	}
}


extern void ACC_On_Detect(uchar cnt_Acc_On )	
{ 

	if((0==Get_ACC_Det_Flag)&&(LOW_LEVEL==GPIO_ReadInputDataBit(GPIO_ACC_GRP,GPIO_ACC_PIN)))
	{
		if(nPowerState==POWER_ACC_OFF)		
			AccOffTimer=T6S_100;
	
		if(ACC_Sampling_Counter)
		{
			ACC_Sampling_Counter--;
		}
		else
		{
			Set_ACC_Det_Flag; 
		}
	}
	else
	{
		ACC_Sampling_Counter=cnt_Acc_On;
	}
 }


extern void ACC_Off_Detect(void)
{
	static uword Sample;
	
	if(Get_ACC_Det_Flag==1
		||(SYSTEM_POWER_IS_ON() && nPowerState==POWER_NORMAL_RUN ))
	{
		for(Sample=1000;;)          //消抖处理
		{
			if(GPIO_ReadInputDataBit(GPIO_ACC_GRP,GPIO_ACC_PIN)==LOW_LEVEL)
			{
				return;
			}
			Sample--;
			if(Sample==0)
			{
				break;
			}
		}

		Clr_ACC_Det_Flag;
		Clr_Machine_Power();
		if(nPowerState==POWER_SYSTEM_STANDBY)
		{//先POWER OFF  后ACC OFF 的处理
			audio_set_mute(AUDIO_MUTE_SYSTEM, FALSE);
			F_SystemStandBy=1;
			return;
		}
		backlight_power_ctrl(FALSE);
		audio_set_mute(AUDIO_MUTE_SYSTEM, TRUE);
		if(PowerOffReason<=POWER_ON)
		{
			PowerOffReason=POWEROFF_FROM_ACC;
		}
		PostEvent(POWER_MODULE,EVT_POWER_OFF,PowerOffReason);
	}
}

static void SystemPowerUp(void)
{
	System_Power_OnOff(ON);
	backlight_power_ctrl(FALSE);
}

static void EnterPowerOff(void)
{
	F_SystemInitEnd=0;
	System_Power_OnOff(OFF);
	ClearEvent(POWER_MODULE);
	nPowerState=POWER_SYSTEM_STANDBY;
	F_SystemOff=1;
	F_SystemStandBy=0;	
	Power_Timer=0;
}

extern void EmergencyPowerDown(void)
{
	Generate_CheckSum();

	//Shut off B+ quickly
	Power_Timer=0;
	Clr_Machine_Power();
	SystemPowerContrl(OFF);
	F_ErrorPower=1;
	F_System_need_reset=1;
	PowerOffReason=POWEROFF_FROM_VOLTAGE;
	PostEvent(POWER_MODULE,EVT_POWER_OFF,PowerOffReason);
}

static void TFT_Power_Ctrl(void)
{
	static uchar Tft_PowerTimer;
	if(Tft_PowerTimer++ >= T3S_12)
		Tft_PowerTimer = T3S_12;
	switch(TFT_Power_State)
	{
		case TFT_POWER_OFF_START:
		default:
			backlight_power_ctrl(FALSE);	// 关闭背光	
			TFT_Power_State=TFT_POWER_OFF;
			Tft_PowerTimer = 0;
			break;
		case TFT_POWER_OFF:
			if(Tft_PowerTimer >= T100MS_12)	
			{
				TFT_Power_State=TFT_POWER_ON_WAITING;
				Tft_PowerTimer = 0;
			}
			break;
 
		case TFT_POWER_ON_WAITING:
			if (!Is_Machine_Power)
			{
				break;
			}
			if (Tft_PowerTimer++ >= T2S_12) {
				Tft_PowerTimer=0;
				TFT_Power_State++;
			}
			break;

		case TFT_POWER_ON:
			if (Tft_PowerTimer++ >= T1S_12) {
				Tft_PowerTimer=0;
				TFT_Power_State++;
			}
			break;

		case TFT_DEVICE_INIT:
			{
				Tft_PowerTimer=0;
				TFT_Power_State++;
			} 
			break;

		case TFT_CCFL_ON:	
			{
				TFT_Power_State++;
				Tft_PowerTimer=0;
			}
			break;
		case TFT_ALL_ON:
			if (Tft_PowerTimer++ >= T3S_12) {
				if (g_brightness<0) {
					g_brightness=1;
				}
			}
			F_TFT_ON = ON;
			break;

		case TFT_IDLE:
			break;
	}
}
extern void PowerManage(void)           //12ms执行一次
{
	EVENT *nEvt;
	nEvt=GetEvent(POWER_MODULE);	
	switch (nEvt->ID)
	{
		case EVT_POWER_ON:
			if (Get_ACC_Det_Flag==0||IS_POWER_OUT_OF_RANGE)
				break;
			nPowerState=POWER_ON_DELAY;
			Power_Timer=0;
			PowerOffReason=POWER_STANDBY;
			audio_set_mute(AUDIO_MUTE_SYSTEM, TRUE);

			break;
		case EVT_POWER_OFF:
			if ( (POWER_ACCOFF2 == nPowerState) ||
				(POWER_ACCOFF_CHECK_HOST == nPowerState) ) {
				// we are handling the ACC off, so ignore this event. 
				// do nothing here, just break
				break;
			}
			PowerOffReason=(POWEROFFREASON)(nEvt->prm);
			if(PowerOffReason==POWEROFF_FROM_VOLTAGE)
			{
				F_SystemInitEnd=0;
				nPowerState=POWER_BU_OFF_DELAY;
			}
			else if (PowerOffReason==POWEROFF_FROM_ACC)
			{
				AccOffTimer=T2S_100;	//20*100ms =2s
				nPowerState=POWER_ACC_OFF;
			}
			audio_set_mute(AUDIO_MUTE_SYSTEM, TRUE);

			Power_Timer=0;
			Clr_Machine_Power();
			TFT_Power_State=TFT_POWER_OFF_START;
			TFT_Power_Ctrl();
			break;
			
		default:
			break;
	}
	if (nPowerState>POWER_VOLTAGE_RISE
		&&nPowerState!=POWER_ACCOFF2
		&&nPowerState!=POWER_ACCOFF_CHECK_HOST
		&& Get_ACC_Det_Flag==1
		&&PowerOffReason>=POWEROFF_FROM_ACCOFF
		&&!IS_POWER_OUT_OF_RANGE)
	{
		if(F_SystemInitEnd==0)
			nPowerState=POWER_ON_DELAY;
		else
			nPowerState=POWER_ACC_ON_DELAY;
		
		Power_Timer=0;
		PowerOffReason=POWER_ON;
		audio_set_mute(AUDIO_MUTE_SYSTEM, TRUE);
	}	
	F_SystemOff=0;
	Power_Timer++;
	if(Power_Timer>=T10S_12)
		Power_Timer=T10S_12;

	switch(nPowerState)
	{
		case POWER_SYSTEM_RESET:
			nPowerState=POWER_ON_DELAY;
			PowerOffReason=POWER_STANDBY;
			Power_Timer=0;			
			audio_set_mute(AUDIO_MUTE_SYSTEM, TRUE);
			F_System_need_reset=0;
			System_Power_OnOff(OFF);
			delay_1ms(10);			
			break;

		case POWER_ON_DELAY:
			F_SystemInitEnd=0;
			if(Power_Timer>=30)	
			{
				nPowerState = POWER_MMI_INITIAL;
				Power_Timer=0;
				 
//				RearSourceL=SOURCE_FOLLOW_FRONT;
//				RearSourceR=SOURCE_FOLLOW_FRONT;

				if( F_System_need_reset==1)
				{
					F_System_need_reset=0;
					g_kill_host = TRUE;
					g_startup_cntr = 0;
					System_Power_OnOff(OFF);
					delay_1ms(10);
				}
				SystemPowerUp();
			} 
			break;

		case POWER_MMI_INITIAL:
			if(Power_Timer>=20)//T12MS_12)
			{
#ifdef DVD_FUNCTION_ENABLE
				GPIO_SetBits(GPIO_DVD_RST_GRP, GPIO_DVD_RST_PIN);
#endif
				nPowerState=POWER_VAR_RECOVER;
			}
			break;

		case POWER_VAR_RECOVER:
			if(Power_Timer>=10)//T12MS_12)	
			{
				nPowerState=POWER_INIT_FINISH;		

				// follow the previous CAN setting
				if (g_canbox_use_uart) {
					USART_RxTx_Init(TRUE);
				} else {
					USART_RxTx_Init(FALSE);
				}
				
				Set_Machine_Power();

				Generate_CheckSum();
			}
			break;
		case POWER_INIT_FINISH:
			Power_Timer=0;
			PowerOffReason=POWER_ON;
			PostEvent(MAIN_MODULE,EVT_SRC_POWER_ON,NONE); 		
			nPowerState=POWER_WAIT_WINCE_SOURCE;
			TFT_Power_State=TFT_POWER_ON_WAITING;
			do_wakeup_host();
			break;
			
		case POWER_WAIT_WINCE_SOURCE:
			if (Power_Timer>T24MS_12) {
				nPowerState=POWER_NORMAL_RUN;
				Power_Timer =0;
				TFT_Power_Ctrl();

				g_super_watchdog_timer = T5S_1000;
			}
			break;
			
		case POWER_NORMAL_RUN:
			F_SystemInitEnd=1;
			F_SystemStandBy=0;
			if (!IS_POWER_OUT_OF_RANGE) {
				TFT_Power_Ctrl();
				if (Power_Timer>T3S_12) {
					audio_set_mute(AUDIO_MUTE_SYSTEM, FALSE);
				}
				if (Power_Timer < T10S_12) {
					if (0 == Power_Timer%T1S_12) {
						if (g_startup_cntr>15) {
							do_wakeup_host();
						}
					}
				}
			}
			break;

		case POWER_CLOSE_SCREEN:
			break;

		case POWER_MOVE_OVER:
			break;

		case POWER_BU_OFF_DELAY:
			AccOffTimer=T3S_100;		
			nPowerState=POWER_VOLTAGE_RISE;
			if (PowerOffReason==POWEROFF_FROM_VOLTAGE)
			{
				// We already check voltage debounce, so we kill everything
				// if we are here
				AccOffTimer=0;		
				radio_set_pwr_ctrl(FALSE);
				EnterPowerOff();
				REAL_SYS_PWR_OFF;
				delay_1ms(500);
			}
			break;
		case POWER_ACC_ON_DELAY:
			if(Power_Timer>=T300MS_12)	//delay 300ms
			{
				Set_Machine_Power();
				Power_Timer=0;
				PowerOffReason=POWER_ON;
				nPowerState=POWER_NORMAL_RUN;

				do_wakeup_host();
				backlight_power_ctrl(TRUE);
			}
			break;
		case POWER_VOLTAGE_RISE:
			if (PowerOffReason==POWEROFF_FROM_VOLTAGE
			&&!IS_POWER_OUT_OF_RANGE
			&&Get_ACC_Det_Flag==1)
			{
				REAL_SYS_PWR_ON;
				PowerOffReason=POWER_ON;
				nPowerState=POWER_ON_DELAY;
				Power_Timer=0;
				audio_set_mute(AUDIO_MUTE_SYSTEM, TRUE);
				break;
			}
			if(AccOffTimer==0)
			{
				radio_set_pwr_ctrl(FALSE);
				PostEvent(WINCE_MODULE,TX_TO_GUI_POWER_CMD,WCE_POWER_OFF);
				EnterPowerOff();
				REAL_SYS_PWR_OFF;
			}
			break;

		case POWER_SYSTEM_STANDBY:
			F_SystemOff=1;

			if((PowerOffReason==POWEROFF_FROM_VOLTAGE||PowerOffReason==POWEROFF_FROM_ACC)
				&&Get_ACC_Det_Flag==1
				&&!IS_POWER_OUT_OF_RANGE
				&& Power_Timer > T2S_12)
			{
				nPowerState=POWER_ON_DELAY;
				Power_Timer=0;
				audio_set_mute(AUDIO_MUTE_SYSTEM, TRUE);
				PowerOffReason=POWER_STANDBY;
			}
			if(Get_ACC_Det_Flag==0&&Power_Timer>=T3S_12)
			{
				audio_set_mute(AUDIO_MUTE_SYSTEM, FALSE);
				F_SystemStandBy=1;
				nPowerState=POWER_SUPER_SAVE;
			}
			break;
		case POWER_SUPER_SAVE:
			if(Get_ACC_Det_Flag==1 && !IS_POWER_OUT_OF_RANGE)
			{
				nPowerState=POWER_SYSTEM_STANDBY;
			} else {
				g_mcu_in_sleep = 1;
			}
			break;
		case POWER_ACC_OFF:
			if(AccOffTimer==0)
			{
				// notify APP to enter sleep
				PostEvent(WINCE_MODULE, TX_TO_GUI_POWER_CMD, WCE_POWER_ENTER_ACC_WAIT);
				g_app_slept = FALSE;
				g_host_is_sleep = TRUE;	// we assume it's asleep
				g_power_timer = 0;
				nPowerState=POWER_ACCOFF_CHECK_HOST;
			}
			break;
		case POWER_ACCOFF_CHECK_HOST:
			++g_power_timer;
			if (0 == g_power_timer%T300MS_12) {
				if (g_power_timer > T30S_12) {
					// if we are here, the HOST can not sleep, so go ahead and kill it
					g_kill_host = TRUE;
					g_startup_cntr = 0;
					g_host_is_sleep = TRUE;
				}

				if (g_host_is_sleep) {
					// reset rtc
					rtc_reset();

					// reset backlight
					g_brightness = 18;

					AccOffTimer=T500MS_100;
					F_SystemInitEnd=0;
					PostEvent(MAIN_MODULE,EVT_SRC_POWER_OFF,PowerOffReason);
					if (0==g_acc_wait_timeout) {
						nPowerState = POWER_ACCOFF2;
					} else {
						g_acc_wait_timer = g_acc_wait_timeout*T1S_100;
						audio_set_mute(AUDIO_MUTE_SYSTEM, TRUE); //make a final mute to assure mute action is done
						EnterPowerOff();
						delay_1ms(500);
						nPowerState = POWER_ACCOFF_WAIT;
					}
				} else {
					g_host_is_sleep = TRUE;	// we assume it's asleep

					if (1==Get_ACC_Det_Flag) {
						nPowerState = POWER_ON_DELAY;
					} else {
						// notify APP to enter sleep again
						PostEvent(WINCE_MODULE, TX_TO_GUI_POWER_CMD, WCE_POWER_ENTER_ACC_WAIT);
					}

					
				}
			}
			break;
		case POWER_ACCOFF_WAIT:
			if (0==g_acc_wait_timer) {
				g_brightness = -1;
				nPowerState = POWER_ACCOFF2;
			}
			break;
		case POWER_ACCOFF2:
			if (AccOffTimer==0)
			{
				if (!g_app_slept) {
					// We did not recv app's ack about our sleep notify, maybe something 
					// wrong, so we kill the host.
					g_kill_host = TRUE;
					g_startup_cntr = 0;
				}
				if (0!=g_acc_wait_timeout) {
					// user set ACC off delay, so we kill the host when timeout
					g_kill_host = TRUE;
					g_startup_cntr = 0;
				}
				audio_set_mute(AUDIO_MUTE_SYSTEM, TRUE); //make a final mute to assure mute action is done
				EnterPowerOff();
				delay_1ms(500);
				Power_Timer=0;
				F_PARKING=0;
			}
			break;
		case	POWER_OFF_HOLD://wait Wince off
			break;
		default:
			EnterPowerOff();
			break;
	}


}

void batt_main(void)
{
	u8 delta;

	if (POWER_NORMAL_RUN != nPowerState) {
		g_batt_notify_cntr = 0;
		return;
	}

	if (g_saved_batt_adc_val > g_batt_adc_val) {
		delta = g_saved_batt_adc_val - g_batt_adc_val;
	} else {
		delta = g_batt_adc_val - g_saved_batt_adc_val;
	}

	if (delta >= 5) {
		g_saved_batt_adc_val = g_batt_adc_val;
		g_batt_notify_cntr = 10;
		PostEvent(WINCE_MODULE, TX_TO_GUI_POWER_CMD, WCE_POWER_BATT_ADC_VAL);
	}

	if (g_batt_notify_cntr>0) {
		--g_batt_notify_cntr;
		PostEvent(WINCE_MODULE, TX_TO_GUI_POWER_CMD, WCE_POWER_BATT_ADC_VAL);
	}
}

