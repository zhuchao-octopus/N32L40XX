/*!
    \file    systick.c
    \brief   the systick configuration file
    
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

#include "public.h"


volatile static uint32_t delay;

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U)){
        /* capture error */
        while (1){
        }
    }
}

/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t count)
{
    delay = count;

    while(0U != delay){
    }
}

void delay_decrement(void)
{
	++g_counter_ms;
	if(g_counter_ms%4==0)
	{
		g_trigger_4ms=1;
		Counter_4ms++;
		if (0!=g_vol_key_info.debounce_timer)
			g_vol_key_info.debounce_timer--;
		if (0!=g_tune_key_info.debounce_timer)
			g_tune_key_info.debounce_timer--;
	}
	if(g_counter_ms%12==0)
	{
		g_trigger_12ms=1;
	}
	if(g_counter_ms%40==0)
	{
		g_trigger_40ms=1;
	}
	if(g_counter_ms%100==0)
	{
		g_trigger_100ms=1;

		if (AccOffTimer != 0) {
			--AccOffTimer;
		}

		if (g_acc_wait_timer != 0) {
			--g_acc_wait_timer;
		}

	}
	if(g_counter_ms==1000)
	{
		g_trigger_1000ms=1;
		g_counter_ms=0;
	}

	if (0U != delay){
		CLEAR_WATCHDOG;
		delay--;
	}
}


void delay_us(u8 us)
{
	uint8_t i;
	for (;us>0;us--) {
		for (i=0; i<10; i++) {
		}
	}
}

GPIO_InitType  g_gpio_i2c_output;
GPIO_InitType  g_gpio_i2c_input;

#define iic_scl_high()	GPIO_SetBits(GPIO_I2C_GRP, GPIO_I2C_SCL_PIN)
#define iic_scl_low()	  GPIO_ResetBits(GPIO_I2C_GRP, GPIO_I2C_SCL_PIN)

#define iic_sda_high()	 GPIO_SetBits(GPIO_I2C_GRP, GPIO_I2C_SDA_PIN)
#define iic_sda_low()	  GPIO_ResetBits(GPIO_I2C_GRP, GPIO_I2C_SDA_PIN)
	
#define iic_sda_out()  { g_gpio_i2c_output.Pin = GPIO_I2C_SDA_PIN; GPIO_InitPeripheral(GPIO_I2C_GRP, &g_gpio_i2c_output); }
#define iic_scl_out()  { ; }
#define iic_sda_in() { g_gpio_i2c_input.Pin = GPIO_I2C_SDA_PIN; GPIO_InitPeripheral(GPIO_I2C_GRP, &g_gpio_i2c_input); }
#define read_sda_() GPIO_ReadInputDataBit(GPIO_I2C_GRP,GPIO_I2C_SDA_PIN)


void i2c_init(void){
	GPIO_InitStruct(&g_gpio_i2c_output);
	g_gpio_i2c_output.GPIO_Mode      = GPIO_Mode_Out_PP;
	g_gpio_i2c_output.GPIO_Current      = GPIO_DC_4mA;
	GPIO_InitStruct(&g_gpio_i2c_input);
	g_gpio_i2c_input.GPIO_Mode      = GPIO_Mode_Input;
	g_gpio_i2c_input.GPIO_Pull = GPIO_Pull_Up;

	iic_sda_high();  
	iic_scl_high();

	g_gpio_i2c_output.Pin = GPIO_I2C_SCL_PIN|GPIO_I2C_SDA_PIN;
	GPIO_InitPeripheral(GPIO_I2C_GRP, &g_gpio_i2c_output);
	
	iic_sda_out();
	iic_scl_out();
}

void i2c_start(void)
{
  
	iic_sda_high();
  	delay_us(5);
	iic_scl_high();
	iic_sda_out();
  	delay_us(5);
 	iic_sda_low();
  	delay_us(5);
	iic_scl_low();
  	delay_us(5);
}

void i2c_stop(void)
{	
	iic_scl_low();
  	delay_us(5);
	iic_sda_low();
	iic_sda_out();	
  	delay_us(5);
	iic_scl_high();
  	delay_us(5);
	iic_sda_high();
  	delay_us(5);
}

uint8_t i2c_wait_ack(void)
{
	uint16_t ucErrTime=0;
	CLEAR_WATCHDOG;
	iic_sda_high();
	iic_sda_in();     
  	delay_us(1);
	iic_scl_high();
  	delay_us(1);
	while(read_sda_())
	{
		ucErrTime++;
		if (0==(ucErrTime%600)) {
			CLEAR_WATCHDOG;
		}
		if(ucErrTime>12000)
		{
			i2c_stop();
			return 1;
		}
	}
	iic_scl_low();	   
	return 0;  
}

void IIC_Ack(void)
{
	iic_scl_low();
	iic_sda_out();
  	delay_us(2);
	iic_sda_low();
  	delay_us(2);
	iic_scl_high();
  	delay_us(2);
	iic_scl_low();
  	delay_us(2);
}
void IIC_NAck(void)
{
	iic_scl_low();
	iic_sda_out();
  	delay_us(2);
	iic_sda_high();
  	delay_us(2);
	iic_scl_high();
  	delay_us(2);
	iic_scl_low();
  	delay_us(2);
}

void i2c_send_byte(uint8_t data)
{                        
	uint8_t t;    
	CLEAR_WATCHDOG;
	iic_scl_low();  
	iic_sda_out();	    
	delay_us(2);
	for(t=0;t<8;t++)
	{       
		if((data&0x80)>>7)
			iic_sda_high();
		else
			iic_sda_low();

		data<<=1; 	  
		delay_us(2);
		iic_scl_high();
		delay_us(2);
		iic_scl_low();
		delay_us(2);
	}	 
}
uint8_t i2c_read_byte(unsigned char ack)
{
	unsigned char i,receive=0;
	CLEAR_WATCHDOG;
	iic_sda_in();
	for(i=0;i<8;i++ )
	{
		iic_scl_low();
		delay_us(2);
		iic_scl_high();
		receive<<=1;
		if(read_sda_()) receive++;   
		delay_us(1);
	}					 
	if (!ack)
		IIC_NAck();//
	else
		IIC_Ack(); //
	return receive;
}

uint8_t adc_channel_sample(uint8_t channel)
{
	u16 adc_val_12b;
	uint16_t timeout;

	CLEAR_WATCHDOG;
	
	ADC_ConfigRegularChannel(ADC, channel, 1, ADC_SAMP_TIME_41CYCLES5); 

	ADC_EnableSoftwareStartConv(ADC, ENABLE);

	timeout = 400;
	while( (ADC_GetFlagStatus(ADC, ADC_FLAG_ENDC)==0) && (timeout-->0) ) {
		CLEAR_WATCHDOG;
	}
	ADC_ClearFlag(ADC,ADC_FLAG_ENDC);
	ADC_ClearFlag(ADC,ADC_FLAG_STR);
	adc_val_12b = ADC_GetDat(ADC);
	
	return ( (adc_val_12b>>4) & 0xFF);
}


void ak_flash_save_info(void)
{

	uint32_t cnt;
	uint32_t len;
	uint32_t page_address;
	uint32_t *data;

	if(FLASH_HSICLOCK_DISABLE == FLASH_ClockInit()) {
		while(1);
	}

	FLASH_Unlock();
	
	/* erase all the area */
	len = FLASH_SIZE_SAVE_INFO/FMC_PAGE_SIZE;
	page_address = FLASH_ADDR_SAVE_INFO;
	for(cnt = 0; cnt < len; cnt++){
		CLEAR_WATCHDOG;
		FLASH_EraseOnePage(page_address);
		page_address = page_address + FMC_PAGE_SIZE;
	}

	/* save key store info */
	len = (sizeof(KEY_INFO_STORE)-1)/4 + 1;		// 1 flash word == 4Byte, align it
	page_address = FLASH_ADDR_KEY_STORE;
	data = (uint32_t *)&g_key_info_store;
	for (cnt=0; cnt<len; cnt++) {
		CLEAR_WATCHDOG;
		FLASH_ProgramWord(page_address, data[cnt]);
		page_address = page_address + 4; 
	}

	/* save system info */
	len = (sizeof(SYSTEM_INFO_STORE)-1)/4 + 1;		// 1 flash word == 4Byte, align it
	page_address = FLASH_ADDR_SYSTEM_INFO;
	data = (uint32_t *)&g_sys_info_store;
	for (cnt=0; cnt<len; cnt++) {
		CLEAR_WATCHDOG;
		FLASH_ProgramWord(page_address, data[cnt]);
		page_address = page_address + 4; 
	}

	FLASH_Lock();

}


void ak_memcpy(uint8_t *to, uint8_t *from, uint16_t length)
{
	uint16_t i;
	for(i=0;i<length;i++)
	{
		*(to+i)=*(from+i);
	}
}

void hal_init(void)
{
	ak_memcpy((uint8_t *)&g_sys_info_store, (uint8_t *)FLASH_ADDR_SYSTEM_INFO, sizeof(SYSTEM_INFO_STORE));

	if ( ('S' != g_sys_info_store.magic_num0) ||
		('Y' != g_sys_info_store.magic_num1) ||
		('S' != g_sys_info_store.magic_num2) ) {
		g_sys_info_store.magic_num0 = 'S';
		g_sys_info_store.magic_num1 = 'Y';
		g_sys_info_store.magic_num2 = 'S';
		g_sys_info_store.brake_mode = PARKING_LEVEL_MODEL;
		g_sys_info_store.audio_navi_mix_extra_gain = 0x80;
		g_sys_info_store.radio_area = RADIO_AREA_JAPAN;
		g_sys_info_store.vol_ctrl_when_reverse = 0;
		g_sys_info_store.beep_onoff = 1;
		g_sys_info_store.output_type = AUDIO_OUTPUT_NORMAL;
		g_sys_info_store.led_always_on = 0;
		g_sys_info_store.led_r_level = 255;
		g_sys_info_store.led_g_level = 1;
		g_sys_info_store.led_b_level = 1;
		g_sys_info_store.vcom_value = 20;
	}
	g_audio_info.output_type = g_sys_info_store.output_type;
	g_radio_area = RADIO_AREA_JAPAN;//g_sys_info_store.radio_area;
	g_audio_info.navi_mix_extra_gain = g_sys_info_store.audio_navi_mix_extra_gain;

}


void rtc_reset(void)
{
	RTC_DateType date;
	RTC_TimeType time;

	date.Year = 0x00;
	date.Month = RTC_MONTH_JANUARY;
	date.WeekDay = RTC_WEEKDAY_SATURDAY;
	date.Date = 0x01;
	RTC_SetDate(RTC_FORMAT_BCD, &date);

	time.H12 = RTC_AM_H12;
	time.Hours = 0x00;
	time.Minutes = 0x00;
	time.Seconds = 0x00;
	RTC_ConfigTime(RTC_FORMAT_BCD, &time);

}

#define BCD_TO_DEC(bcd) (((bcd)>>4)*10+((bcd)&0x0F))

uint32_t rtc_get_seconds(void)
{
	RTC_DateType date;
	RTC_TimeType time;
	uint32_t seconds=0;
	uint32_t tmp;

	RTC_GetDate(RTC_FORMAT_BCD, &date);
	RTC_GetTime(RTC_FORMAT_BCD, &time);

	// calc date
	tmp = BCD_TO_DEC(date.Date)-1;
	tmp = tmp*86400;
	seconds += tmp;

	// calc hour
	tmp = BCD_TO_DEC(time.Hours);
	tmp = tmp*3600;
	seconds += tmp;

	// calc minute
	tmp = BCD_TO_DEC(time.Minutes);
	tmp = tmp*60;
	seconds += tmp;

	// calc second
	tmp = BCD_TO_DEC(time.Seconds);
	seconds += tmp;

	return seconds;
}

