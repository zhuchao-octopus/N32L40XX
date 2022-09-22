
#ifndef _CUSTOMER_H_
#define _CUSTOMER_H_


typedef enum
{
	SOURCE_TUNER=0,	//HD Radio
	SOURCE_DVD=1,		
	SOURCE_DVDC=2,	//CDC
	SOURCE_TV=3,
	SOURCE_NAVI=4,
	SOURCE_AUX=5,		//AUX IN 1
	SOURCE_DTV=6,
	SOURCE_SD=7,
	SOURCE_XM=8,		//SERIUS
	SOURCE_IPOD=9,
	SOURCE_USB=10,
	SOURCE_CAMERA=11,
	SOURCE_FRONT_AUX=12,	//AUX IN 2
	SOURCE_BT=13,
	SOURCE_HDMI=14,
	SOURCE_HDRADIO=15,
	SOURCE_AVOFF=16,		//source off
	SOURCE_3G=17,	
	SOURCE_IPOD_USB=18,		
	NUM_OF_SOURCE=19,
	SOURCE_FOLLOW_FRONT = 0xFE
}SOURCE;

typedef enum
{
	FV_SRC_AUXIN=0x05,
	FV_SRC_BACKCAR=0x0B,
	FV_SRC_F_CAM=0x0C,
	FV_SRC_L_CAM=0x20,
	FV_SRC_R_CAM=0x21
} FRONT_VIDEO_SRC;


#define MAX_BRIGHTNESS_LEVEL	21

extern void MMI(void);
extern void hdmi_main(void);

/* Interface functions ------------------------------------------------------ */

typedef enum
{
	FRONT,
	REAR_L,
	REAR_R,
	REAR_LR,
	FRONT_REAR_LR
}Def_FR_Source;

EXT bool g_fake_pwr_off;

ext SOURCE FrontSource;



extern void SwitchSource(SOURCE TargetSRC);


#endif



