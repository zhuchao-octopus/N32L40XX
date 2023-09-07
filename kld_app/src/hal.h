/*!
    \file    systick.h
    \brief   the header file of systick
    
    \version 2018-03-26, V1.0.0, demo for GD32E103
    \version 2020-09-30, V1.1.0, demo for GD32E103
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef HAL_H
#define HAL_H

#include <stdint.h>


ext uint16_t g_counter_ms;
ext uint8_t g_trigger_4ms;
ext uint8_t g_trigger_12ms;
ext uint8_t g_trigger_40ms;
ext uint8_t g_trigger_100ms;
ext uint8_t g_trigger_1000ms;


typedef struct
{
    uint8_t magic_num0;
    uint8_t magic_num1;
    uint8_t magic_num2;
    uint8_t brake_mode;
    uint8_t audio_navi_mix_extra_gain;
    uint8_t radio_area;
    uint8_t vol_ctrl_when_reverse;
    uint8_t beep_onoff;
    uint8_t output_type;
    uint8_t led_always_on;
    uint8_t led_r_level;
    uint8_t led_g_level;
    uint8_t led_b_level;
    uint8_t vcom_value;
} __attribute__((aligned(4))) SYSTEM_INFO_STORE;

ext SYSTEM_INFO_STORE g_sys_info_store;


ext  UCharField Sys_Flag2;

#define Wince_Start_Flag        Sys_Flag2.field.B1
#define F_System_need_reset     Sys_Flag2.field.B2

typedef struct 
{
    unsigned F_Initialize           :   1;  
    unsigned F_Reverse_Det          :   1;  
    unsigned F_Slot_In          :   1;  
    unsigned F_Lighting_Det     :   1;  
    unsigned F_ACC_DetFlag          :   1;  
    unsigned F_Line_MuteFlag        :   1;

    unsigned F_TFT_ON_Flag          :   1;  //
    unsigned F_ErrorPower_Flag      :   1;  //B+超出范围标记,1:有效
    unsigned F_MachineState     :   1;  
    unsigned F_Parking      :   1;  
    unsigned F_SysLowPower_Flag     :   1;
    unsigned F_Can_Box_Flag     :   1;
    unsigned    F_Power_On_Course   :   1;  //在上电过程置1，上电结束后置0
    unsigned F_OnStar_Active        :   1;
    unsigned F_ILL_ctrl_mode        :   2;
}_DET_FLAG;
typedef union 
{
      uword word;
      _DET_FLAG field;
}DET_FLAG;

ext  DET_FLAG   Det_Flag;


#define Set_Reverse_Det_Flag    Det_Flag.field.F_Reverse_Det=1;
#define Clr_Reverse_Det_Flag    Det_Flag.field.F_Reverse_Det=0;
#define Get_Reverse_Det_Flag    Det_Flag.field.F_Reverse_Det    

#define Set_ACC_Det_Flag    Det_Flag.field.F_ACC_DetFlag=1;  
#define Clr_ACC_Det_Flag        Det_Flag.field.F_ACC_DetFlag=0;
#define Get_ACC_Det_Flag    Det_Flag.field.F_ACC_DetFlag             


#define F_TFT_ON    Det_Flag.field.F_TFT_ON_Flag

#define Is_Machine_Power    (Det_Flag.field.F_MachineState==1)
#define Set_Machine_Power()     Det_Flag.field.F_MachineState=1;
#define Clr_Machine_Power()     Det_Flag.field.F_MachineState=0;


#define F_ErrorPower            Det_Flag.field.F_ErrorPower_Flag
#define IS_POWER_OUT_OF_RANGE   (F_ErrorPower==1)
#define F_PARKING       Det_Flag.field.F_Parking 

#define F_ILLUMIN_CONTORL_MODE  Det_Flag.field.F_ILL_ctrl_mode

/* configure systick */
void systick_config(void);
/* delay a time in milliseconds */
void delay_1ms(uint32_t count);
void delay_us(u8 us);
/* delay decrement */
void delay_decrement(void);


extern void delay_3us(void);

extern void i2c_init(void);
extern void i2c_start(void);
extern void i2c_stop(void);
extern uint8_t i2c_wait_ack(void);
extern void i2c_send_byte(uint8_t data);
extern uint8_t i2c_read_byte(unsigned char ack);

extern uint8_t adc_channel_sample(uint8_t channel);

extern void ak_memcpy(uint8_t *to, uint8_t *from, uint16_t length);
extern void ak_flash_save_info(void);

extern void hal_init(void);

ext uint32_t g_rtc_async_div;
ext uint32_t g_rtc_sync_div;
extern __IO uint32_t g_rtc_calib_timer_freq;

void rtc_reset(void);
uint32_t rtc_get_seconds(void);


#define FLASH_ADDR_UPDATER    0x0801D800
#define FLASH_ADDR_SAVE_INFO    0x0801F800
#define FLASH_ADDR_KEY_STORE    0x0801F800      /* offset 0 */
#define FLASH_ADDR_SYSTEM_INFO  0x0801FC00  /* offset 1024 */
#define FLASH_SIZE_SAVE_INFO        0x800       /* 2KB */
#define FMC_PAGE_SIZE                       0x800       /* 1page == 2KB */

#define RTC_CALIB_TIME      2000    // in ms
ext uint32_t g_rtc_calib_cntr;
ext uint32_t g_rtc_prescaler;


#endif /* HAL_H */
