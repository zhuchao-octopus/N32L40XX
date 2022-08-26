
#include "public.h"


const uword Tft_Brightness_100_Percent_Tab[MAX_BRIGHTNESS_LEVEL]=
{
	0,
	1,
	9,
	17,
	24,
	32,
	39,
	47,
	54,
	62,
	69,
	77,
	84,
	92,
	99,
	107,
	114,
	122,
	129,
	137,
	144,
};
const uword Tft_Brightness_60_Percent_Tab[MAX_BRIGHTNESS_LEVEL]=
{	
	0,
	2,
	6,
	10,
	15,
	19,
	24,
	28,
	33,
	37,
	42,
	46,
	51,
	55,
	60,
	64,
	69,
	73,
	78,
	82,
	87,
};


//在使用LED和CCFL作为TFT背光时，为了保持代码一致，上电完成后，
//CCFL_EN一直有效。视频MUTE时，只是把PWM的占空比调低。
extern void enable_backlight(void)
{
	if (!IS_PANEL_PWR_ON) {
		return;
	}
	GPIO_SetBits(GPIO_BKL_EN_GRP, GPIO_BKL_EN_PIN);
} 
extern void disable_backlight(void)
{
	GPIO_ResetBits(GPIO_BKL_EN_GRP, GPIO_BKL_EN_PIN);
}

u16 g_pre_tft_backlight_level = 0;
void ABRT_Driver(void)
{
	uword tft_backlight_level;
	u8 level;

	if (g_brightness<0) {
		level = 0;
	} else {
		level = g_brightness;
	}

	if(F_ILLUMIN_CONTORL_MODE == ILLUMIN_AUTO_MODE)
	{
		if(Det_Flag.field.F_Lighting_Det==1)
		{
			tft_backlight_level=Tft_Brightness_60_Percent_Tab [level];	
		}
		else
		{
			tft_backlight_level=Tft_Brightness_100_Percent_Tab [level];				
		}  	
	}
	else if(F_ILLUMIN_CONTORL_MODE == ILLUMIN_NIGHT_MODE)
	{
		tft_backlight_level=Tft_Brightness_60_Percent_Tab [level];
	}
	else
	{
		tft_backlight_level=Tft_Brightness_100_Percent_Tab [level];
	}

	//所有屏亮度控制真正动作在此处完成
	if ( F_TFT_ON == OFF )
	{
		disable_backlight();
		if (0!=g_pre_tft_backlight_level) {
			// notify HOST. so they can use it to control their backllight
			PostEvent(WINCE_MODULE, TX_TO_GUI_BRIGHTNESS_INFO,0 );
		}
		g_pre_tft_backlight_level = 0;
	}
	else
	{
		if (g_pre_tft_backlight_level!=tft_backlight_level) {
			if (0 == tft_backlight_level) {
				disable_backlight();
			} else {
				enable_backlight();
			}
			g_pre_tft_backlight_level = tft_backlight_level;

			// notify HOST. so they can use it to control their backllight
			PostEvent(WINCE_MODULE, TX_TO_GUI_BRIGHTNESS_INFO,level );
		}
	}
}

void Illumi_Detect(void)
{
	bool port;
	static uchar Light_Sampling_Counter=1;
	if(SYSTEM_POWER_IS_ON()==0)	//20070907 ILLUMI先置高后开机，TFT亮度错误
		return;
	port=(bool)(Bit_RESET==GPIO_ReadInputDataBit(GPIO_ILL_GRP, GPIO_ILL_PIN));
	Light_Sampling_Counter--;
	if (Light_Sampling_Counter==0)
	{
		//多次采样确认
		if(port)
		{
			Det_Flag.field.F_Lighting_Det=1;
		}
		else
		{
			Det_Flag.field.F_Lighting_Det=0;
		}
	}
	if ((port&&Det_Flag.field.F_Lighting_Det)||(!port&&(!Det_Flag.field.F_Lighting_Det)))
	{
		//端口电平与标志位一致 :=1
		Light_Sampling_Counter=N_SAMPLING_LIGHTOFF;
	}
}

// 倒车检测
// 检测到倒车信号后，视频切换到后视镜模式
#define N_SAMPLING_REVERSE	 3
void Reverse_Detect(void)	//12ms执行一次
{

	bool port;
	static uchar Reverse_Sampling_Counter=1;

	if(Get_ACC_Det_Flag==0)
	{
		Reverse_Sampling_Counter=N_SAMPLING_REVERSE;
		Clr_Reverse_Det_Flag;	
		return;
	}
	port=(bool)(Bit_RESET==GPIO_ReadInputDataBit(GPIO_REVERSE_GRP, GPIO_REVERSE_PIN));
	Reverse_Sampling_Counter--;

	if(Reverse_Sampling_Counter==0)
	{
		//多次采样确认
		if(port)
		{
			Set_Reverse_Det_Flag;
		}
		else 
		{	
			Clr_Reverse_Det_Flag;
		}
	}
	if ((port&&Get_Reverse_Det_Flag) ||(!port &&(!Get_Reverse_Det_Flag)))
	{
		//端口电平与标志位一致 :=1
		Reverse_Sampling_Counter=N_SAMPLING_REVERSE;
	}
}




#define 	N_SAMPLING_PARK	3
static uchar Park_Sampling_Counter;

void Parking_Pro(void)
{
	u8 port;

	//复位上电时PARKING_MODE 还未初始化，导致F_Stop_Car 被置1
	if(!Is_Machine_Power)
	{
		Park_Sampling_Counter=1;
		F_PARKING=0; //已开车
		return;
	}

	if (g_canbox_use_uart) {
		port=GPIO_ReadInputDataBit(GPIO_BRAKE_GRP, GPIO_BRAKE_PIN);
	} else {
//	fix this later	port=( (0==GPIO_CAR_BRAKE_DET_2) || (1==GPIO_CAR_BRAKE_DET) );  //采样
		port=GPIO_ReadInputDataBit(GPIO_BRAKE_GRP, GPIO_BRAKE_PIN);
	}
	Park_Sampling_Counter--;
	if (Park_Sampling_Counter==0)
	{
		//多次采样确认
		if (port)
		{
			F_PARKING=1; //已刹车
		}
		else
		{
			F_PARKING=0; //已开车
		}
		PostEvent(WINCE_MODULE, TX_TO_GUI_CAR_PARKING_STATUS,F_PARKING);
	}

	if ((port&&F_PARKING==1) ||(!port&&F_PARKING==0) )
	{
		//端口电平与标志位一致 :=1
		Park_Sampling_Counter=N_SAMPLING_PARK;
	}	
}

void Navi_Audio_Detect(void)
{
	//return;
	if(!Is_Machine_Power
		||Wince_Start_Flag == 0
		||(g_fake_pwr_off)
	)  
	{
		g_navi_audio_det = 0;
		audio_set_navi_break(FALSE);
		return;
	}

	if (0==g_navi_audio_det) {
		audio_set_navi_break(FALSE);
	} else {
		audio_set_navi_break(TRUE);
	}
}
void Game_Audio_Detect(void)
{
	if(!Is_Machine_Power
		||Wince_Start_Flag == 0
	)  
	{
		g_app_audio_det = 0;
		audio_set_app_3rd_break(FALSE);
		return;
	}  

	if (0==g_app_audio_det) {
		audio_set_app_3rd_break(FALSE);
	} else {
		audio_set_app_3rd_break(TRUE);
	}
}


/************************关闭或者打开PowerCtrl脚****************************/
void SystemPowerContrl(ONOFF On_Off)
{
	if(On_Off==ON)
	{
//		bool do_delay = FALSE;
//		if (!REAL_SYS_PWR_IS_ON()) {
			// do delay before power up the panel, or the touch
			// may be malfunction
//			do_delay = TRUE;
//		}
		REAL_SYS_PWR_ON;
		SYSTEM_POWER_CTRL_ON;

#if 1
		// do not delay, or some LCD will be burned, if VGH/VGL enabled early then DVDD
		// T_T !!!!
		// what should we do later, if some TP can not work after ACC 30S off... T_T!!!
#else
		if (do_delay) {
			Delay_ms(500);
		}
#endif
		GPIO_SetBits(GPIO_LCD_EN_GRP, GPIO_LCD_EN_PIN);
		dvd_poweron();
//		GPIO_SetBits(GPIO_HDMI_PWR_GRP, GPIO_HDMI_PWR_PIN);
//		GPIO_SetBits(GPIO_TV_PWR_GRP, GPIO_TV_PWR_PIN);
	}
	else
	{
		GPIO_ResetBits(GPIO_HDMI_PWR_GRP, GPIO_HDMI_PWR_PIN);
		GPIO_ResetBits(GPIO_TV_PWR_GRP, GPIO_TV_PWR_PIN);
		GPIO_ResetBits(GPIO_ANT_CTRL_GRP, GPIO_ANT_CTRL_PIN);
		GPIO_ResetBits(GPIO_LCD_EN_GRP, GPIO_LCD_EN_PIN);
		GPIO_ResetBits(GPIO_FCAM_PWR_GRP, GPIO_FCAM_PWR_PIN);
		dvd_poweroff();
		SYSTEM_POWER_CTRL_OFF;
		if (g_kill_host) {
			REAL_SYS_PWR_OFF;
			g_kill_host = FALSE;
		}
	}
}

