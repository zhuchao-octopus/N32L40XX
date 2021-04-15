
#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#include "n32l40x.h"

#ifdef	ROOT
#define	EXT 
#define	ext 
#else
#define	EXT	extern
#define	ext	extern
#endif

#include <stdio.h>

#define bool _Bool
#define TRUE 1
#define FALSE 0


typedef struct BitChar {
	unsigned char B0:1;
	unsigned char B1:1;
	unsigned char B2:1;
	unsigned char B3:1;
	unsigned char B4:1;
	unsigned char B5:1;
	unsigned char B6:1;
	unsigned char B7:1;
} SBitChar;

typedef union  CharField{
	unsigned char	byte;
	SBitChar	field;
} UCharField;

typedef union  CharInt{
	unsigned char	byte[2];// byte[1]=L, byte[0]=H in hiware
	unsigned int Dbyte;
} UCharInt;


typedef union  CharLong{
	unsigned char	byte[4];// byte[0]=H
	unsigned long int Lbyte;
} UCharLong;
#include "config.h"
#include "define.h"
#include "hal.h"
#include "uicc_def.h"
#include "event.h"
#include "system.h"
#include "customer.h"

#include "channel.h"

#include "power.h"

#include "audio_driver.h"

#include "radio_driver.h"

#include "rds.h"

#include "clock.h"
#include "videochip.h"
#include "io.h"

#include "remote.h"


#include "usart_driver.h"
#include "LinUart_Wince.h"
#include "Canbox.h"


#include "led.h"

#include "beep.h"

#ifdef DVD_FUNCTION_ENABLE
#include "dvd.h"
#endif

#include "key.h"

#endif

