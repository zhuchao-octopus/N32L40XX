
#ifndef _CLOCK_H_
#define _CLOCK_H_

extern void TimeManage(void);


ext u8 AccOffTimer;

#define RTC_PSCR            7
#define RTC_COUNTER         25000//¶¨Ê±50MS(HSE)
ext uchar Counter_4ms;

typedef struct 
{
    unsigned F_HalfSecond_Flag      :   1;    
    unsigned F_HalfSecondBackup_Flag    :   1;
    unsigned F_HalfSecondOut_Flag       :   1;    // 1:CVBS, 0:RGB
    unsigned F_4MS_Flag         :   1; 
    unsigned F_12MS_Flag            :   1;
    unsigned F_40MS_Flag            :   1;
    unsigned F_100MS_Flag       :   1;
    unsigned F_Clock_Display_Flag       :   1;
    unsigned F_IR_HasKey_Flag       :   1;
    unsigned    F_1S_FLAG   :   1;
    unsigned        :   6;
}_CLOCK_FLAG    ;
typedef union 
{
      unsigned char byte;
      _CLOCK_FLAG field;
}CLOCK_FLAG;
ext CLOCK_FLAG ClockFlag;

#define F_4MS           ClockFlag.field.F_4MS_Flag
#define F_12MS          ClockFlag.field.F_12MS_Flag
#define F_40MS          ClockFlag.field.F_40MS_Flag
#define F_100MS         ClockFlag.field.F_100MS_Flag
#define F_1S                ClockFlag.field.F_1S_FLAG   


#endif

