
#ifndef _VIDEOCHIP_H_
#define _VIDEOCHIP_H_

typedef enum
{
	EVT_VIDEO_NONE,
	EVT_VIDEO_SOURCE,
	EVT_VIDEO_INFO,
	EVT_CAMERA_ENTER,
	EVT_CAMERA_EXIT,
	EVT_RGB_ENTER,
	EVT_RGB_EXIT,
	EVT_TV_ENTER,
	EVT_VIDEO_DETECT,
	EVT_VID_EMERGENCY_OFF,
	EVT_VID_MUTE_OFF,
	EVT_VID_MUTE_ON,
	EVT_VID_FRONT_SOURCE_SET,
	EVT_VID_REAR_L_SOURCE_SET,
	EVT_VID_REAR_R_SOURCE_SET,
	EVT_VID_REAR_LR_SOURCE_SET,
	EVT_VID_FRONT_REAR_SOURCE_SET
}VIDEO_EVT;


extern void SetFrontVideo(SOURCE);
extern void SetRearLVideo(SOURCE);
extern void SetRearRVideo(SOURCE prm);
extern SOURCE video_get_rear_video_source(Def_FR_Source type);
extern void Video_Main(void);

ext s8 g_brightness;



void vcom_update_level(void);
void vcom_set_level(u8 level);

void video_set_pwr_ctrl(bool on);

#endif

