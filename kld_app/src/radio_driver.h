
#ifndef _RADIO_DRIVER_H_
#define _RADIO_DRIVER_H_


#define FM_PRESET_NUM       30
#define AM_PRESET_NUM       12

typedef enum
{
    RADIO_BAND_FM = 0,
    RADIO_BAND_AM,
    RADIO_BAND_NUMS
}RADIO_BAND;

typedef enum
{
    PRESET_BAND_FM1 = 0,
    PRESET_BAND_FM2,
    PRESET_BAND_FM3,
    PRESET_BAND_AM1,
    PRESET_BAND_AM2,
    PRESET_BAND_NUMS
}PRESET_BAND;

typedef enum
{
    RADIO_AREA_MIN = 0,
    RADIO_AREA_USA = 0,
    RADIO_AREA_LATIN,
    RADIO_AREA_EUROPE,
    RADIO_AREA_OIRT,
    RADIO_AREA_JAPAN,
    RADIO_AREA_CHN,
    RADIO_AREA_KOREA,
    RADIO_AREA_SOUTH_AMERICA,
    RADIO_AREA_AUSTRALIA,
    RADIO_AREA_ASIA_PACIFIC,
    RADIO_AREA_RUSSIA,
    RADIO_AREA_JAPAN_2,
    RADIO_AREA_NUMS
}RADIO_AREA;

typedef enum  
{
    RADIO_STATE_UNKNOWN=0,
    RADIO_STATE_PWR_OFF_ING,
    RADIO_STATE_PWR_ON_ING,
    RADIO_STATE_INIT_ING,
    RADIO_STATE_IDLE,
    RADIO_STATE_FREQ_SEEKING,
    RADIO_STATE_PRESET_SEEKING,
    RADIO_STATE_FREQ_SETTING,
    RADIO_STATE_PARAM_SETTING,
    RADIO_STATE_NUMS
}RADIO_STATE;

typedef enum  
{
    RFS_STATE_UNKNOWN=0,
    RFS_STATE_BEGIN,
    RFS_STATE_SET_FREQ,
    RFS_STATE_SET_FREQ_ING,
    RFS_STATE_SET_FREQ_DONE,
    RFS_STATE_WAIT_TUNE_STATUS,
    RFS_STATE_GET_SIGNAL_LEVEL,
    RFS_STATE_WAIT_RDS_PI_MATCH,
    RFS_STATE_WAIT_RDS_TP_MATCH,
    RFS_STATE_WAIT_RDS_PTY_MATCH,
    RFS_STATE_PLAY_ING,
    RFS_STATE_GOTO_NEXT_FREQ,
    RFS_STATE_STOP,
    RFS_STATE_FINISHED,
    RFS_STATE_NUMS
}RADIO_FREQ_SEEK_STATE;
typedef enum  
{
    RFS_CMD_UNKNOWN=0,
    RFS_CMD_AUTO_UP,
    RFS_CMD_AUTO_DN,
    RFS_CMD_STEP_UP,
    RFS_CMD_STEP_DN,
    RFS_CMD_AUTO_SAVE,
    RFS_CMD_SCAN,
    RFS_CMD_AF_SEEK_LIST,
    RFS_CMD_AF_SEEK_PI,
    RFS_CMD_NUMS
}RADIO_FREQ_SEEK_CMD;
typedef struct
{
    RADIO_FREQ_SEEK_STATE state;
    RADIO_FREQ_SEEK_CMD cmd;
    PRESET_BAND band;
    u16 start_freq;
    u16 cur_freq;
    bool stopped;
    u16 timer;
}RADIO_FREQ_SEEK_INFO;

typedef enum  
{
    RPS_STATE_UNKNOWN=0,
    RPS_STATE_BEGIN,
    RPS_STATE_SET_FREQ,
    RPS_STATE_SET_FREQ_ING,
    RPS_STATE_SET_FREQ_DONE,
    RPS_STATE_PLAY_ING,
    RPS_STATE_FINISHED,
    RPS_STATE_NUMS
}RADIO_PRESET_SEEK_STATE;
typedef enum
{
    RPS_CMD_UNKNOWN=0,
    RPS_CMD_SCAN,
    RPS_CMD_STOP,
    RPS_CMD_NUMS
}RADIO_PRESET_SEEK_CMD;
typedef struct
{
    RADIO_PRESET_SEEK_STATE state;
    RADIO_PRESET_SEEK_CMD cmd;
    PRESET_BAND band;
    s8 start_id;
    s8 cur_id;
    u16 timer;
}RADIO_PRESET_SEEK_INFO;

typedef enum  
{
    RFSET_STATE_UNKNOWN=0,
    RFSET_STATE_BEGIN,
    RFSET_STATE_SET_ING,
    RFSET_STATE_SET_DONE,
    RFSET_STATE_FINISHED,
#ifdef FIX_RADIO_BAND_SWITCH_POP
    RFSET_STATE_PREPARING,
#endif
    RFSET_STATE_NUMS
}RADIO_FREQ_SET_STATE;
typedef enum
{
    RFSET_CMD_UNKNOWN=0,
    RFSET_CMD_STEP_UP,
    RFSET_CMD_STEP_DN,
    RFSET_CMD_SET_TO,
    RFSET_CMD_SWITCH_BAND,
    RFSET_CMD_STOP,
    RFSET_CMD_NUMS
}RADIO_FREQ_SET_CMD;
typedef struct
{
    RADIO_FREQ_SET_STATE state;
    RADIO_FREQ_SET_CMD cmd;
    PRESET_BAND band;
    u16 freq;
    u8 timer;
}RADIO_FREQ_SET_INFO;

typedef enum  
{
    RPMSET_STATE_UNKNOWN=0,
    RPMSET_STATE_BEGIN,
    RPMSET_STATE_FINISHED,
    RPMSET_STATE_NUMS
}RADIO_PARAM_SET_STATE;
typedef enum
{
    RPMSET_CMD_UNKNOWN=0,
    RPMSET_CMD_LOC_DX,
    RPMSET_CMD_RDS_TA_SWITCH,
    RPMSET_CMD_RDS_AF_SWITCH,
    RPMSET_CMD_RDS_PTY_SEL,
    RPMSET_CMD_ST,
    RPMSET_CMD_STOP,
    RPMSET_CMD_NUMS
}RADIO_PARAM_SET_CMD;
typedef struct
{
    RADIO_PARAM_SET_STATE state;
    RADIO_PARAM_SET_CMD cmd;
    u16 value;
}RADIO_PARAM_SET_INFO;


typedef union 
{
    RADIO_FREQ_SEEK_INFO freq_seek;
    RADIO_PRESET_SEEK_INFO preset_seek;
    RADIO_FREQ_SET_INFO freq_set;
    RADIO_PARAM_SET_INFO param_set;
}RADIO_EVENT_INFO;

typedef enum
{
    RSL_NONE=0,
    RSL_POOR=1,
    RSL_BAD=2,
    RSL_NORMAL=3,
    RSL_GOOD=4,
    RSL_EXCELLENT=5,
    RSL_NUMS
}RADIO_SIGNAL_LEVEL;

typedef enum
{
    RADIO_EVT_NONE,
    RADIO_EVT_BAND,   /*change band*/
    RADIO_EVT_SEEK,  /*EVT in tuner*/ /** prm=0: dowm ;prm>0: up**/
    RADIO_EVT_PRESET_SELECT,
    RADIO_EVT_PRESET_SAVE,
    RADIO_EVT_FREQ_SET,
    RADIO_EVT_LOC_DX,               /** DX mode**/
    RADIO_EVT_AREA_SET,
    RADIO_EVT_STOP_SEEK,
    RADIO_EVT_RDS_SWITCH,
    RADIO_EVT_RDS_PTY_SET,
    RADIO_EVT_ST,
    RADIO_EVT_NUMS
}RADIO_EVENT;

typedef struct
{
    unsigned          :1;
    unsigned F_RADIO_ST_SW        :1;
    unsigned F_RADIO_PS       :1;   
    unsigned    F_RADIO_LOC_DX :1;
    unsigned F_RADIO_ST_MONO          :1;
    unsigned F_RADIO_SCAN          :1;
    unsigned F_RADIO_AMS           :1;
    unsigned F_RADIO_SEEK          :1;  
}_RADIO_FLAG;
typedef union
{
    uchar   byte;
    _RADIO_FLAG field;
}RADIO_FLAG;

typedef struct
{
    RADIO_STATE state;
    PRESET_BAND cur_band;
    PRESET_BAND another_band;
    u16 max_freq[RADIO_BAND_NUMS];
    u16 min_freq[RADIO_BAND_NUMS];
    u8 step[RADIO_BAND_NUMS];
    u16 cur_freq[PRESET_BAND_NUMS];
    s8 cur_idx[PRESET_BAND_NUMS];
    u16 preset_fm[3][FM_PRESET_NUM];
    u16 preset_am[2][AM_PRESET_NUM];
    RADIO_EVENT_INFO evt_info;
    RADIO_FLAG flag;
    RADIO_SIGNAL_LEVEL signal_level;
    u8 signal_loss_timer;
    u16 tp_loss_timer;
    u8 st_det_timer;
    u8 af_check_timer;
    u8 pwr_timer;
}RADIO_INFO;

/*******************************************************
*   interface for system
*******************************************************/
void radio_init(void);
void radio_main(void);
void radio_set_pwr_ctrl(bool on);


/*******************************************************
*   interface for mcu-host communication
*    return: actually fill length
*******************************************************/
u8 radio_comm_fill_total_info(u8 *pbuf);
u8 radio_comm_fill_cur_info(u8 *pbuf);
u8 radio_comm_fill_preset_info(u8 *pbuf);


/***************************************
* legacy varible
**************************************/
ext u8 g_radio_area;


/***************************************
* our varible
**************************************/
ext RADIO_INFO g_radio_info;    // do not write any member outside radio_driver.c, just read only.
ext u16 g_tea668x_version;

ext u8 g_7708_detune;
ext u8 g_7708_fs;
ext u8 g_7708_snr;
ext u8 g_7708_mpx;
ext u8 g_7708_mp;
ext u8 g_7708_dev;
ext u8 g_7708_adj;

/*******************************************************
*   interface for physical radio device
*******************************************************/

// return value: true, if radio device finish initialization.
//                   false, if radio device still in initialization.
bool radio_dev_init(void);

// return value: true, if radio device finish de-initialization.
//                   false, if radio device still in de-initialization.
bool radio_dev_deinit(void);

// return value: true, if radio device finish setting frequency .
//                   false, if radio device still setting frequency.
bool radio_dev_is_set_freq_done(void);

void radio_dev_set_freq(RADIO_BAND band, u16 freq);

void radio_dev_set_freq_tune(RADIO_BAND band, u16 freq);

// return value: true, if radio device received a qualify station .
//                   false, if the current station is not good, due to fault at 
//                   RSSI/SNR/IF offset, etc.
bool radio_dev_is_tune_ok(bool strict);

// return value: true, if radio device 's tune status (RSSI/SNR/IF offset, etc) is available .
//                   false, if radio device 's tune status still un-available.
bool radio_dev_is_tune_status_ready(void);

// return value: 0: error occur
//                   1: mono 
//                   2: stereo
u8 radio_dev_get_stereo_status(void);


typedef enum
{
    RDSM_DX = 0,    /* seek more station, but lower quality */
    RDSM_LOC = 1,   /* seek less station, but higher quality */
    RDSM_NUMS
}RADIO_DEV_SEEK_MODE;
bool radio_dev_set_seek_mode(RADIO_DEV_SEEK_MODE mode);

// param on :  FALSE ---- Force Mono
//                 TRUE ---- Stereo on
bool radio_dev_set_st_mode(bool on);


bool radio_dev_set_mute(bool mute);


void radio_init_area(u8 area);

#endif  /* _RADIO_DRIVER_H_ */

