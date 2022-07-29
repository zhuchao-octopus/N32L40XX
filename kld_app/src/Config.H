

#ifndef _CONFIG_H_
#define _CONFIG_H_

//#define DEBUG

//#define KD2000

//#define CUSTOM_887
//#define CUSTOM_S217

/*****************************
******************************
	options 
******************************
******************************/
// Tuner device 
#define TUNER_NONE				0
#define TUNER_NXP_TEF6606		1
#define TUNER_NXP_TEA6851		2
#define TUNER_SILICONLAB_SI476X		3
#define TUNER_NXP_TEA668X		4
#define TUNER_ST_TDA7708		5
#define TUNER_SILICONLAB_SI475X    6

//DVD device
#define DVD_DVS				0
#define DVD_XINWA			1

//AUXIN MODE
#define AUXIN_FRONT			0
#define AUXIN_REAR			1
#define AUXIN_FRONT_REAR	2

//FAN_START_TEMP 
#define FAN_START_ALL_TIME		0
#define FAN_START_45_C			1
#define FAN_START_46			2
#define FAN_START_47			3
#define FAN_START_48			4
#define FAN_START_49		5
#define FAN_START_50		6
#define FAN_START_51		7
#define FAN_START_52		8
#define FAN_START_53		9
#define FAN_START_54		10
#define FAN_START_55		11
#define FAN_START_56		12
#define FAN_START_57		13
#define FAN_START_58		14
#define FAN_CLOSE			15

//NAVI_CH
#define NAVI_AUDIO_LEFT_CH        		0
#define NAVI_AUDIO_RIGHT_CH			1
#define NAVI_AUDIO_LEFT_RIGHT_CH         2
#define NAVI_AUDIO_ALL_CH        		      3

//PARKING_MODE
#define PARKING_OFF_MODEL					0	//不检测
#define PARKING_LEVEL_MODEL				1	//电平检测
#define PARKING_SERIAL_MODEL				2	//序列检测

//ILLUMIN_MODE
#define ILLUMIN_AUTO_MODE	0
#define ILLUMIN_NIGHT_MODE	1
#define ILLUMIN_DAY_MODE	2

// ILL detect mode
#define ILLUMIN_DETCET_MODE_LEVEL	0
#define ILLUMIN_DETCET_MODE_PULSE	1

//ASP device define 
#define ASP_NONE	0
#define ASP_TDA7415		1
#define ASP_TDA7719		2
#define ASP_BD37534		3
#define ASP_BU32107		4

//电压异常检测，AD查寻
#define	VOLT_5V		64
#define	VOLT_6V		78
#define	VOLT_7V		90
#define	VOLT_8V		103
#define	VOLT_8V5	109
#define	VOLT_9V		116
#define	VOLT_9V5	123
#define	VOLT_10V	130
#define	VOLT_10V5	136
#define	VOLT_12V	156
#define	VOLT_15V	193
#define	VOLT_16V	206
#define	VOLT_17V	219

// Encoder trigger type
#define RISE_LEVEL					1
#define FALL_LEVEL				2
#define RISE_FALL_LEVEL			3	


/*****************************
******************************
	Configs 
******************************
******************************/
/*---- Audio ----*/
//#define ASP_MODEL		ASP_BU32107		// DSP
#define ASP_MODEL		ASP_BD37534		// non DSP
#define VOLUME_CUT_WHEN_NAVI_MIX		9
#define DFT_NAVI_AUDIO_CH	NAVI_AUDIO_LEFT_CH//导航发声时默认喇叭
#ifdef CUSTOM_S217
#define A_SRC_RADIO_EXTRA_GAIN_ATTEN		1
#define A_SRC_HOST_EXTRA_GAIN_ATTEN		7
#define A_SRC_BT_MODULE_EXTRA_GAIN_ATTEN		10
#else
#define A_SRC_RADIO_EXTRA_GAIN_ATTEN		0
#define A_SRC_HOST_EXTRA_GAIN_ATTEN		4
#define A_SRC_BT_MODULE_EXTRA_GAIN_ATTEN		-3
#endif
#define A_SRC_DVD_EXTRA_GAIN_ATTEN		-4
#define A_SRC_AUXIN_EXTRA_GAIN_ATTEN		-7
#define A_SRC_TV_EXTRA_GAIN_ATTEN		0
#define A_NAVI_MIX_EXTRA_DB	9
#define A_BT_PHONE_EXTRA_DB	12
#define A_ALL_EXTRA_DB			3
#define CONFIG_LOUDNESS_DB	6

/*---- Radio ----*/
#define TUNER_MODEL  TUNER_NXP_TEA668X
#define RADIO_DEFAULT_REGION   	RADIO_AREA_EUROPE//默认收音区域选择
#define RDS_FUNCTION		1//RDS开关
//#define TEA668X_V205
#ifndef TEA668X_V205
#define TEA668X_V102
#endif

#if TUNER_MODEL==TUNER_SILICONLAB_SI475X
#define FIX_RADIO_BAND_SWITCH_POP
#endif

/*---- DVD ----*/
#ifndef CUSTOM_S217
#define DVD_FUNCTION_ENABLE
#endif
#define DFT_DVD_MODE		DVD_XINWA//机芯选择
//#define NO_DISC_LED

/*---- AUX In ----*/
#define DFT_AUXIN_MODE		AUXIN_REAR//aux 模式选择

/*---- Car Signal ----*/
#define DFT_PARKING_MODE 	PARKING_LEVEL_MODEL//默认刹车检测方式
#define DFT_ILLUMIN_MODE 	ILLUMIN_AUTO_MODE//默认大灯显示模式
#define DEF_ILLUMIN_DETECT_MODE ILLUMIN_DETCET_MODE_LEVEL//大灯检测模式

/*---- Power ----*/
#define	HIGH_VOLT_PROTECT_OFF		VOLT_17V//高压关机电压值
#define	HIGH_VOLT_PROTECT_ON		VOLT_16V	//高压恢复电压值
#define	LOW_VOLT_PROTECT_OFF		VOLT_8V5	//低压关机电压值
#define	LOW_VOLT_PROTECT_ON		VOLT_9V5//低压恢复电压值
//#define FAKE_POWER_OFF							1//假关机



/*---- Misc ----*/
#define DEF_BEEP_ONOFF	ON	//默认蜂鸣器是否打开
#define DFT_FAN_START_TEMPERATURE FAN_START_50//默认风扇启动温度


/*****************************
******************************
	Version 
******************************
******************************/
//#define MCU_FIXED_VERSION "CSN2_04282021_173906_V308"

#define MCU_VERSION_BASE   __DATE__ "_" __TIME__

#if !defined(MCU_VERSION_BASE)
#error "MCU_VERSION_BASE must be defined!"
#endif

#if ASP_MODEL==ASP_BU32107
#define MCU_DSP		"D_"
#else
#define MCU_DSP		""
#endif

#if TUNER_MODEL==TUNER_ST_TDA7708
#define MCU_RADIO		"R7708_"
#else
#define MCU_RADIO		""
#endif

#ifdef KD2000
#define MCU_VERSION	"CSM4_KD2000_" MCU_DSP MCU_RADIO MCU_VERSION_BASE 
#elif defined (CUSTOM_887)
#define MCU_VERSION	"CSM4_6_" MCU_DSP MCU_RADIO MCU_VERSION_BASE 
#elif defined (CUSTOM_S217)
#define MCU_VERSION	"CSM4_S217_" MCU_DSP MCU_RADIO MCU_VERSION_BASE 
#else
#define MCU_VERSION	"CSM4_" MCU_DSP MCU_RADIO MCU_VERSION_BASE 
#endif


#endif //_CONFIG_H




