#ifndef _CHANNEL_H_
#define _CHANNEL_H_

typedef enum
{
    EVT_SRC_NONE,
    EVT_SRC_EMERGENCY_OFF,
    EVT_SRC_POWER_OFF,
    EVT_SRC_POWER_ON,
    EVT_SRC_REAR_L_SOURCE_SET,
    EVT_SRC_REAR_R_SOURCE_SET,
    EVT_SRC_REAR_LR_SOURCE_SET,
    EVT_SRC_FRONT_SOURCE_SET,
    EVT_SRC_FRONT_REAR_CHG,
    EVT_SRC_VIDEO_CHANGE    
}SOURCE_EVT;


ext  void channel_main(void);

#endif

