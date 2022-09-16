
#include "public.h"

#define OUT_PUT1 0x00   
#define OUT_PUT2 0x01
#define OUT_PUT3 0x02
#define OUT_PUT4 0x03
#define OUT_PUT5 0x04
#define OUT_PUT6 0x05

/*chip address*/
#define FMS6502_ADDR    0x06  

/*offset address*/
#define OUT_PUT1_ADDR 0x00   
#define OUT_PUT2_ADDR 0x00
#define OUT_PUT3_ADDR 0x01
#define OUT_PUT4_ADDR 0x01
#define OUT_PUT5_ADDR 0x02
#define OUT_PUT6_ADDR 0x02
#define FMS6502_CLAMP 0x03
#define FMS6502_GAIN 	0x04

#define IN_PUTOFF 0x00
#define IN_PUT01   0x01
#define IN_PUT02   0x02
#define IN_PUT03   0x03
#define IN_PUT04   0x04
#define IN_PUT05   0x05
#define IN_PUT06   0x06
#define IN_PUT07   0x07
#define IN_PUT08   0x08

#define FRONT_CVBS_CHANNEL	OUT_PUT1
#define REAR1_CVBS_CHANNEL	OUT_PUT6
#define REAR2_CVBS_CHANNEL	OUT_PUT5

#define CHANNEL_GAIN_0db	1
#define CHANNEL_GAIN_6db	0

#define NAV_CVBS_SOURCE 			IN_PUT01

#define DVD_CVBS_SOURCE 			IN_PUT02
#define RIGHT_CAM_SOURCE				IN_PUT04
#define CAMERA_CVBS_SOURCE 			IN_PUT03
#define DTV_CVBS_SOURCE				IN_PUT05
#define AUX_CVBS_SOURCE				IN_PUT06
#define AUX_FRONT_CVBS_SOURCE		IN_PUT07
#define LEFT_CAM_SOURCE			IN_PUT08
#define NO_SOURCE					IN_PUTOFF

#define DVD_SYNC_SOURCE 			DVD_CVBS_SOURCE
static void Select_Video_Source(uchar output_addr,uchar input_number);

static u8 g_wait_panel_pwr_timer = 0;

static bool g_fms6502_need_config = FALSE;
static u16 g_fms6502_config_timer = 0;
static SOURCE g_rear1_source = SOURCE_SD;
static SOURCE g_rear2_source = SOURCE_SD;
static bool g_fms6502_shutdown = FALSE;

const uchar RearVideoPath[NUM_OF_SOURCE+1]=
{
	NAV_CVBS_SOURCE,	/*TUNER*/
	DVD_CVBS_SOURCE,	/*DVD*/
	DVD_CVBS_SOURCE,		/*DVDC*/
	DTV_CVBS_SOURCE,		/*TV*/
	NAV_CVBS_SOURCE,	/*NAVI*/
	AUX_CVBS_SOURCE,	/*AUX*/
	DTV_CVBS_SOURCE,	/*DTV*/
	NAV_CVBS_SOURCE,	/*SD*/
	NAV_CVBS_SOURCE,	/*XM*/
	NO_SOURCE,//DVD_CVBS_SOURCE,		/*IPOD video*/
	NAV_CVBS_SOURCE,	/*USB*/
	NAV_CVBS_SOURCE,		/*CAMERA*/
	AUX_FRONT_CVBS_SOURCE,		/*AUX 2 */
	NAV_CVBS_SOURCE,					/*BT*/
	NAV_CVBS_SOURCE,			/*WB*/
	NAV_CVBS_SOURCE,
	NO_SOURCE,		//source off
	NAV_CVBS_SOURCE,
	NAV_CVBS_SOURCE,
	NO_SOURCE	
};

static void fms6502_write(u8 addr, u8 value)
{
	i2c_start();
	i2c_send_byte(FMS6502_ADDR);
	i2c_wait_ack();
	i2c_send_byte(addr);
	i2c_wait_ack();
	i2c_send_byte(value);
	i2c_wait_ack();
	i2c_stop();
}

static void Select_Video_Source(uchar output_number,uchar input_number)
{
	uchar data;	
	uchar addr;
	bool input_right=TRUE;
	static u8 InputMap[3];
//	static u8 OutputGain  = 0;
	switch(output_number)
	{
		case OUT_PUT1:
			data = (InputMap[0] & 0xF0) | input_number;
			InputMap[0] = data;
			addr = OUT_PUT1_ADDR;
			break;
		case OUT_PUT2:
			data = (InputMap[0] & 0x0F) | (input_number<<4);
			InputMap[0] = data;
			addr = OUT_PUT2_ADDR;
			break;
		case OUT_PUT3:
			data = (InputMap[1] & 0xF0) | input_number;
			InputMap[1] = data;
			addr = OUT_PUT3_ADDR;
			break;
		case OUT_PUT4:
			data = (InputMap[1] & 0x0F) | (input_number<<4);
			InputMap[1] = data;
			addr = OUT_PUT4_ADDR;
			break;
		case OUT_PUT5:
			data = (InputMap[2] & 0xF0) | input_number;
			InputMap[2] = data;
			addr = OUT_PUT5_ADDR;
			break;
		case OUT_PUT6:
			data = (InputMap[2] & 0x0F) | (input_number<<4);
			InputMap[2] = data;
			addr = OUT_PUT6_ADDR;
			break;
		default:
			input_right =FALSE;
			break;
	}
	if(input_right == TRUE)
	{
		fms6502_write(addr, data);
		fms6502_write(FMS6502_CLAMP, 0xFF);
		fms6502_write(FMS6502_GAIN, 0x00);	////all 6db Output
	}
}
/*--------------------------------------------------------------------------
Routine :SetFrontVideo
Input   : 
Output  :
Description 
        -  video channel functions
--------------------------------------------------------------------------*/ 
extern void SetFrontVideo(u8 prm)
{
	u8 input = NO_SOURCE;
	if (SOURCE_FRONT_AUX == prm) {
		GPIO_SetBits(GPIO_FCAM_PWR_GRP, GPIO_FCAM_PWR_PIN);
	}
	switch (prm) {
		case SOURCE_CAMERA:
			input = CAMERA_CVBS_SOURCE;
			break;
		case SOURCE_TV:
		case SOURCE_DTV:
			input = DTV_CVBS_SOURCE;
			break;
		case SOURCE_AUX:
			input = AUX_CVBS_SOURCE;
			break;
		case SOURCE_FRONT_AUX:
			input = AUX_FRONT_CVBS_SOURCE;
			break;
		case SOURCE_DVD:
			input = DVD_CVBS_SOURCE;
			break;
		default:
			input = NO_SOURCE;
			break;
	}
	Select_Video_Source(FRONT_CVBS_CHANNEL, input);
	if (SOURCE_FRONT_AUX != prm) {
		GPIO_ResetBits(GPIO_FCAM_PWR_GRP, GPIO_FCAM_PWR_PIN);
	}
}

/*--------------------------------------------------------------------------
Routine :SetRearVideo
Input   : 
Output  :
Description 
        -  video channel functions
--------------------------------------------------------------------------*/ 

extern void SetRearLVideo(SOURCE prm)
{
	if(prm>NUM_OF_SOURCE)
		prm=NUM_OF_SOURCE;
	Select_Video_Source(REAR1_CVBS_CHANNEL,RearVideoPath[prm]);
	g_rear1_source = prm;
}
extern void SetRearRVideo(SOURCE prm)
{
	if(prm>NUM_OF_SOURCE)
		prm=NUM_OF_SOURCE;
	Select_Video_Source(REAR2_CVBS_CHANNEL,RearVideoPath[prm]);
	g_rear2_source = prm;
}

SOURCE video_get_rear_video_source(Def_FR_Source type)
{
	if (REAR_L == type) {
		return g_rear1_source;
	} else if (REAR_R == type) {
		return g_rear2_source;
	}
	return NUM_OF_SOURCE;
}

static void video_fms6502_config(void)
{
	Select_Video_Source(REAR1_CVBS_CHANNEL,RearVideoPath[g_rear1_source]);
	Select_Video_Source(REAR2_CVBS_CHANNEL,RearVideoPath[g_rear2_source]);
}

void video_set_pwr_ctrl(bool on)
{
	if (on) {
		g_fms6502_need_config = TRUE;
		g_fms6502_config_timer = 0;
	} else {
		g_wait_panel_pwr_timer = 0;
	}
}

void Video_Main(void)
{
	EVENT *nEvt;
	uchar tmp_Lprm;
	static u8 s_front_ch=SOURCE_CAMERA;

	if(!Is_Machine_Power) {
		g_wait_panel_pwr_timer = 0;
		return;
	}

	if (!IS_PANEL_PWR_ON) {
		g_wait_panel_pwr_timer = 0;
		return;
	}

	if (g_wait_panel_pwr_timer < T100MS_12) {
		++g_wait_panel_pwr_timer;
		vcom_update_level();
		return;
	}

	nEvt=GetEvent(VIDEO_MODULE);
	tmp_Lprm=LSB(nEvt->prm);

	if (g_fms6502_need_config) {
		if (g_fms6502_config_timer < T50MS_12) {
			SetFrontVideo(s_front_ch);
		}
	}


	if (g_fake_pwr_off) {
		g_fms6502_need_config = TRUE;
		if (!g_fms6502_shutdown) {
			g_fms6502_shutdown = TRUE;
			Select_Video_Source(REAR1_CVBS_CHANNEL,NO_SOURCE);
			Select_Video_Source(REAR2_CVBS_CHANNEL,NO_SOURCE);
		}
		if (EVT_VID_FRONT_SOURCE_SET == (nEvt->ID)) {
			SetFrontVideo(tmp_Lprm);
			s_front_ch = tmp_Lprm;
		}
		return;
	} else {
		g_fms6502_shutdown = FALSE;
	}

	if (g_fms6502_need_config) {
		if (++g_fms6502_config_timer > T6S_12) {
			video_fms6502_config();
			g_fms6502_need_config = FALSE;
		}
	}

	switch(nEvt->ID)
	{
		case EVT_VID_REAR_R_SOURCE_SET:
			SetRearRVideo((SOURCE)tmp_Lprm);
			PostEvent(WINCE_MODULE, TX_TO_GUI_REAR_R_SOURCE_INFO,  g_rear2_source);
			break;
		case EVT_VID_REAR_L_SOURCE_SET:
//			if (SOURCE_AVOFF!=tmp_Lprm) {
//				tmp_Lprm = SOURCE_TUNER;
//			}
			SetRearLVideo((SOURCE)tmp_Lprm);
			SetRearRVideo((SOURCE)tmp_Lprm);
			PostEvent(WINCE_MODULE, TX_TO_GUI_REAR_L_SOURCE_INFO,  g_rear1_source);
			break;
		case EVT_VID_FRONT_SOURCE_SET:
			SetFrontVideo(tmp_Lprm);
			s_front_ch = tmp_Lprm;
			break;
//		case EVT_VID_REAR_2_SOURCE_SET:
//			if (SOURCE_AVOFF!=g_rear1_source) {
//				switch (tmp_Lprm) {
//					case SOURCE_HDMI:
//						tmp_Lprm = SOURCE_FRONT_AUX;
//						break;
//					case SOURCE_FRONT_AUX:
//						tmp_Lprm = SOURCE_AUX;
//						break;
//				}
//				SetRearLVideo((SOURCE)tmp_Lprm);
//				SetRearRVideo((SOURCE)tmp_Lprm);
//			}
//			break;
		default:
			break;
	}

}


/*++++++++++++++++++++++++++++++++++++++++++++*/
/***************** VCOM control *****************/
void vcom_update_level(void)
{
	TIM_SetCmp1(TIMER_VCOM, g_sys_info_store.vcom_value*10);
}
void vcom_set_level(u8 level)
{
	if (level>99) {
		level=99;
	} else if (level<=0) {
		level=1;
	}
	
	if (level!=g_sys_info_store.vcom_value) {
		g_sys_info_store.vcom_value = level;
		ak_flash_save_info();
	}
	vcom_update_level();
}
/*-------------------------------------------------*/

