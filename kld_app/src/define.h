
#ifndef _DEFINE_H_
#define _DEFINE_H_


typedef unsigned char BYTE;

typedef unsigned char uchar;
typedef unsigned int uword;
typedef uchar * puchar;
typedef uword * puword;
typedef uchar EVENT_ID ;
typedef void (* pfunction) (uchar);            



#define SetBit(Var,Place) (Var|=(uchar)((1<<(Place%8))))
#define ClrBit(Var,Place) (Var&=(uchar)((1<<(Place%8))^255))
#define GetBit(Var,Place) ((Var& (1<<(Place%8)))==0?0:1)
#define CplBit(Var,Place) (Var^=(1<<(Place%8)))
#define LSB(WORD)		  (uchar)WORD
#define MSB(WORD)		  (uchar)(WORD>>8)
#define WORD(MSB,LSB)	  (uword)(((uword) MSB << 8) + LSB)

#define HighNibble(var)  (var>>4)
#define LowNibble(var)   (var&0x0f)
#define _BYTE(NibbleH,NibbleL)  ((NibbleH<<4)|NibbleL)

#define BIT_0			0x01
#define BIT_1			0x02
#define BIT_2			0x04
#define BIT_3			0x08
#define BIT_4			0x10
#define BIT_5			0x20
#define BIT_6			0x40
#define BIT_7			0x80
#define		Cpl_Bit(Val,Place)		((Val)^=(Place))
#define		Get_Bit(Val,Place)		((Val)&(Place))
#define		Set_Bit(Val,Place)		((Val)|=(Place))
//#define		Clr_Bit(Val,Place)		((Val)&=((Place)^0xff))
#define		Clr_Bit(Val,Place)		((Val)&=(~(Place)))

#define AREA 0x00
#define BitClr(BIT)  ( *((volatile char *) (AREA+BIT/8)) &= (~(1<<(7-BIT%8))) )
#define BitSet(BIT)  ( *((volatile char *) (AREA+BIT/8)) |=   (1<<(7-BIT%8))  )
#define BitVal(BIT)  ( *((volatile char *) (AREA+BIT/8)) & (1<<(7-BIT%8)) ) 


#define GetPinOut(BIT)  ((*(BIT/8+&Bak_Port)  &  (1<<(BIT%8)) )==0?0:1)
#define GetPinIn(BIT)   ((*(BIT/8*3+&PA_IDR)  &  (1<<(BIT%8)) )==0?0:1)

#define MAX(x,y)	( ((x) > (y)) ? (x) : (y) )
#define MIN(x,y)	( ((x) < (y)) ? (x) : (y) )
typedef union 
{
	uchar	b_var[2]; 
	uword	w_var;
	//b_var[0] is MSB byte of w_var
	//b_var[1] is LSB byte of w_var
}WORDVAR;

typedef enum
{
	OFF=0,
	ON=1
}ONOFF;

#define Opening                 1
#define Closing                   0

//#define ENABLE_WWDG

#define BEGIN_WATCHDOG		
#ifdef ENABLE_WWDG
#define CLEAR_WATCHDOG  WWDG_SetCnt(127);
#else
#define CLEAR_WATCHDOG	 	
#endif
#define DI
#define EI

#define NONE  0

#define HIGH_LEVEL	1
#define LOW_LEVEL	0

#define IO_AS_INPUT			0	//DDR
#define IO_AS_OUTPUT			1	//DDR

#define FLOATING_INPUT			0	//CR1
#define PULL_UP_INPUT			1	//CR1

#define PSEUDO_OPEN_DRAIN_OUT	0	//CR1
#define PUSH_PULL_OUT			1	//CR1

#define INPUT_INTERRUPT_DISANBLED	0	//CR2
#define INPUT_INTERRUPT_ENANBLED	1	//CR2

#define OUTPUT_LIMIT_2MHZ		0	//CR2 
#define OUTPUT_LIMIT_10MHZ		1	//CR2


#define FLASH_UNLOCK_KEY1  0X56
#define FLASH_UNLOCK_KEY2  0XAE

#define RESET_BATT_FAIL     0XC3
#define RESET_BATT_OK       0X3C


#define MCCSR_CTRL_WORD         0B00000010;//  Fcpu=Fosc2=4Mhz
// 4ms 时基
#define T4MS_4		1	
#define T12MS_4		3
#define T32MS_4		8
#define T60MS_4		15
#define T72MS_4		18
#define T100MS_4	25
#define T200MS_4	50
#define T300MS_4	75
#define T400MS_4	100
#define T500MS_4	125
#define T800MS_4	200
#define T2000MS_4	500

// 12ms 时基
#define T12MS_12		1
#define T24MS_12	2
#define T36MS_12	3
#define T50MS_12	4
#define T60MS_12	5
#define T72MS_12	6
#define T84MS_12	7
#define T100MS_12	8
#define T108MS_12	9
#define T120MS_12	10
#define T150MS_12	14
#define T200MS_12	16
#define T290MS_12	25
#define T300MS_12	25
#define T400MS_12	33
#define T500MS_12	42
#define T600MS_12	50
#define T1S_12		83
#define T1S5_12		125
#define T2S_12		167
#define T2S5_12		208
#define T3S_12		250
#define T6S_12		500
#define T8S_12		666
#define T10S_12		833
#define T30S_12		2500

// 40ms 时基
#define T40MS_40	1
#define T80MS_40	2
#define T200MS_40	5
#define T320MS_40	8
#define T600MS_40	15
#define T800MS_40	20
#define T1S_40		25
#define T1S5_40		38
#define T2S_40		50
#define T3S_40		75
#define T5S_40  		125
#define T7S_40		175
#define T8S_40		200
#define T10S_40		250

// 100ms 时基
#define T100MS_100	1
#define T200MS_100	2
#define T300MS_100	3
#define T500MS_100	5
#define T1S_100		10
#define T2S_100		20
#define T3S_100		30
#define T4S_100		40
#define T5S_100		50
#define T6S_100		60
#define T8S_100		80
#define T9S_100		90
#define T10S_100		100
#define T20S_100		200
#define T25S_100		250
#define T30S_100		300

#define T5S_1000		5
#define T10S_1000	10
#define T20S_1000	20
#define T30S_1000	30
#define T40S_1000	40
#define T50S_1000	50
#define T60S_1000	60


#define N_SAMPLING_IPOD         3 
#define N_SAMPLING_PARK         3
#define N_SAMPLING_LIGHTON    1
#define N_SAMPLING_LIGHTOFF   9
#define N_SAMPLING_VOLTAGE    4	// 3*100MS
#define N_SAMPLING_ACC           42
#define N_SAMPLING_WAKEUP     8


#endif  //_DEFINE_H_

