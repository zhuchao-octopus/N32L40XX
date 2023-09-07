
#ifndef _BEEP_H_
#define _BEEP_H_

typedef enum
{
    BEEP_STATE_PWR_DOWN = 0,
    BEEP_STATE_IDLE,
    BEEP_STATE_BEEP_ING,    
    BEEP_STATE_NUMS
}BEEP_STATE;

typedef enum
{
    BEEP_MODE_NONE,
    BEEP_MODE_SHORT,
    BEEP_MODE_DOUBLE,
    BEEP_MODE_LONG,
    BEEP_MODE_NUMS
}BEEP_MODE;

typedef struct
{
    BEEP_STATE state;
    BEEP_MODE mode;
    u8 timer;
}BEEP_INFO;

ext BEEP_INFO g_beep_info;

extern void beep_init(void);
extern void beep_main(void);
extern void beep_short_mode(void);
extern void beep_double_mode(void);
extern void beep_long_mode(void);


#endif  /* _BEEP_H_ */

