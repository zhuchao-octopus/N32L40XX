
#ifndef _DVD_H_
#define _DVD_H_

typedef enum
{
    DISC_STATE_UNKNOWN = 0,
    DISC_STATE_READY,
    DISC_STATE_OUTSIDE,
    DISC_STATE_INJECTING,
    DISC_STATE_EJECTING,
    DISC_STATE_NODISC,
    DISC_STATE_NUMS
}DISC_STATE;

typedef enum
{
    DISC_CMD_NONE = 0,
    DISC_CMD_INJECT,
    DISC_CMD_EJECT,
    DISC_CMD_NUMS
}DISC_CMD;

typedef struct
{
    bool dvd_pwr_up;
    DISC_STATE disc_state;
    DISC_CMD disc_cmd;
    u16 auto_inject_timer;
    u8 pwr_timer;
    u8 timer;
    u16 force_eject_timer;
}DVD_INFO;

typedef enum
{
    EVT_DVD_NONE,
    EVT_DVD_PWR_CTRL,
    EVT_DVD_DISC_CTRL,
    EVT_DVD_FORCE_DISC_EJECT,
    EVT_DVD_NUMS
}DVD_EVENT;


ext DVD_INFO g_dvd_info;    // do not write any member outside dvd.c, just read only.

extern void dvd_init(void);
extern void dvd_poweroff(void);
extern void dvd_main(void);
extern bool dvd_is_audio_ready(void);

#endif  /* _DVD_H_ */

