
#define ROOT
#include "public.h"

typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress = 0;


static u8 g_first_love;


void RTCCalibration(void)
{
	NVIC_InitType NVIC_InitStructure;
	TIM_ICInitType TIM_ICInitStructure;
	RTC_InitType RTC_InitStructure;

	g_rtc_calib_timer_freq = 0;
	
	/* System Clocks Configuration */
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM9, ENABLE);

	/* NVIC configuration */

	/* Enable the TIM3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel                   = TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ICInitStructure.Channel     = TIM_CH_3;
	TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
	TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
	TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
	TIM_ICInitStructure.IcFilter    = 0x0;

	TIM_ICInit(TIM9, &TIM_ICInitStructure);
	TIM9->CTRL1 |= TIM_CTRL1_C3SEL;

	/* TIM enable counter */
	TIM_Enable(TIM9, ENABLE);

	/* Enable the CC3 Interrupt Request */
	TIM_ConfigInt(TIM9, TIM_INT_CC3, ENABLE);

	/* wait calibration finished */
	for(;;)
	{
		if (0!=g_rtc_calib_timer_freq) {
			/* TIM enable counter */
			TIM_Enable(TIM9, DISABLE);
			/* Enable the CC3 Interrupt Request */
			TIM_ConfigInt(TIM9, TIM_INT_CC3, DISABLE);

			g_rtc_async_div = 0x7F;  // value range: 0-7F
			g_rtc_sync_div  = g_rtc_calib_timer_freq / g_rtc_async_div; // TIM3Freq
			RTC_InitStructure.RTC_AsynchPrediv = g_rtc_async_div;
			RTC_InitStructure.RTC_SynchPrediv  = g_rtc_sync_div;
			RTC_InitStructure.RTC_HourFormat   = RTC_24HOUR_FORMAT;
			RTC_Init(&RTC_InitStructure);

			NVIC_InitStructure.NVIC_IRQChannel                   = TIM9_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelCmd                = DISABLE;
			NVIC_Init(&NVIC_InitStructure);

			break;
		}
	}

}

static void rtc_config(void)
{
	g_rtc_async_div = 0x7F;
	g_rtc_sync_div = 0x14A;
	
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
	PWR_BackupAccessEnable(ENABLE);

	RCC_EnableRtcClk(DISABLE);
	RCC_EnableLsi(ENABLE);
	while (RCC_GetFlagStatus(RCC_CTRLSTS_FLAG_LSIRD) == RESET) {}
	RCC_ConfigRtcClk(RCC_RTCCLK_SRC_LSI);
	RCC_EnableRtcClk(ENABLE);
	RTC_WaitForSynchro();

	rtc_reset();

	RTCCalibration();
}

static void rcu_config(void)
{
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC, ENABLE);
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD, ENABLE);
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);

	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_USART1, ENABLE);
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART2, ENABLE);
	
	RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_ADC, ENABLE);
	ADC_ConfigClk(ADC_CTRL3_CKMOD_AHB, RCC_ADCHCLK_DIV16);
	RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSE, RCC_ADC1MCLK_DIV8);

	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM1, ENABLE);
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM4, ENABLE);
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM8, ENABLE);
}
static void gpio_config(void)
{
	GPIO_InitType gpio_init_output;
	GPIO_InitType  gpio_init_output_af;
	GPIO_InitType  gpio_init_input_pull_up;
	GPIO_InitType  gpio_init_input_float;
	GPIO_InitType  gpio_init_ain;

	GPIO_InitStruct(&gpio_init_output);
	gpio_init_output.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init_output.GPIO_Current = GPIO_DC_4mA;
	GPIO_InitStruct(&gpio_init_output_af);
	gpio_init_output_af.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_init_output_af.GPIO_Current = GPIO_DC_4mA;
	GPIO_InitStruct(&gpio_init_input_float);
	gpio_init_input_float.GPIO_Mode = GPIO_Mode_Input;
	gpio_init_input_float.GPIO_Pull = GPIO_No_Pull;
	GPIO_InitStruct(&gpio_init_input_pull_up);
	gpio_init_input_pull_up.GPIO_Mode = GPIO_Mode_Input;
	gpio_init_input_pull_up.GPIO_Pull = GPIO_Pull_Up;
	GPIO_InitStruct(&gpio_init_ain);
	gpio_init_ain.GPIO_Mode = GPIO_Mode_Analog;

	/* MUTE control */
	gpio_init_output.Pin = GPIO_MUTE_PIN;
	GPIO_InitPeripheral(GPIO_MUTE_GRP, &gpio_init_output);
	GPIO_SetBits(GPIO_MUTE_GRP, GPIO_MUTE_PIN);

	if (0!=g_first_love) {

		/* System power control */
		gpio_init_output.Pin = GPIO_REAL_SYS_PWR_PIN;
		GPIO_InitPeripheral(GPIO_REAL_SYS_PWR_GRP, &gpio_init_output);
		GPIO_ResetBits(GPIO_REAL_SYS_PWR_GRP, GPIO_REAL_SYS_PWR_PIN);
		
		/* Devices 5V power control */
		gpio_init_output.Pin = GPIO_DEVICE_5V_PWR_PIN;
		GPIO_InitPeripheral(GPIO_DEVICE_5V_PWR_GRP, &gpio_init_output);
		GPIO_ResetBits(GPIO_DEVICE_5V_PWR_GRP, GPIO_DEVICE_5V_PWR_PIN);

		/* Audio power control */
		gpio_init_output.Pin = GPIO_AUDIO_PWR_PIN;
		GPIO_InitPeripheral(GPIO_AUDIO_PWR_GRP, &gpio_init_output);
		GPIO_ResetBits(GPIO_AUDIO_PWR_GRP, GPIO_AUDIO_PWR_PIN);

		/* Fcam power control */
		gpio_init_output.Pin = GPIO_FCAM_PWR_PIN;
		GPIO_InitPeripheral(GPIO_FCAM_PWR_GRP, &gpio_init_output);
		GPIO_ResetBits(GPIO_FCAM_PWR_GRP, GPIO_FCAM_PWR_PIN);
	}


	/* UART HOST */
	gpio_init_output_af.Pin = GPIO_HOST_UART_TX_PIN;
	gpio_init_output_af.GPIO_Alternate = GPIO_AF4_USART1;
	gpio_init_output_af.GPIO_Pull = GPIO_No_Pull;
	GPIO_InitPeripheral(GPIO_HOST_UART_TX_GRP, &gpio_init_output_af);
	gpio_init_output_af.Pin = GPIO_HOST_UART_RX_PIN;
	gpio_init_output_af.GPIO_Alternate = GPIO_AF4_USART1;
	gpio_init_output_af.GPIO_Pull = GPIO_Pull_Up;
	GPIO_InitPeripheral(GPIO_HOST_UART_RX_GRP, &gpio_init_output_af);

	/* UART CAN */
	gpio_init_output_af.Pin = GPIO_CAN_UART_TX_PIN;
	gpio_init_output_af.GPIO_Alternate = GPIO_AF4_USART2;
	gpio_init_output_af.GPIO_Pull = GPIO_No_Pull;
	GPIO_InitPeripheral(GPIO_CAN_UART_TX_GRP, &gpio_init_output_af);
	gpio_init_output_af.Pin = GPIO_CAN_UART_RX_PIN;
	gpio_init_output_af.GPIO_Alternate = GPIO_AF4_USART2;
	gpio_init_output_af.GPIO_Pull = GPIO_Pull_Up;
	GPIO_InitPeripheral(GPIO_CAN_UART_RX_GRP, &gpio_init_output_af);

	
	/* ACC */
	gpio_init_input_pull_up.Pin = GPIO_ACC_PIN;
	GPIO_InitPeripheral(GPIO_ACC_GRP, &gpio_init_input_pull_up);

	/* Bake */
	gpio_init_input_pull_up.Pin = GPIO_BRAKE_PIN;
	GPIO_InitPeripheral(GPIO_BRAKE_GRP, &gpio_init_input_pull_up);

	/* Ill detect */
	gpio_init_input_pull_up.Pin = GPIO_ILL_PIN;
	GPIO_InitPeripheral(GPIO_ILL_GRP, &gpio_init_input_pull_up);

	/* Reverse detect */
	gpio_init_input_pull_up.Pin = GPIO_REVERSE_PIN;
	GPIO_InitPeripheral(GPIO_REVERSE_GRP, &gpio_init_input_pull_up);

	/* Telephone mute detect */
	gpio_init_input_pull_up.Pin = GPIO_TEL_MUTE_DET_PIN;
	GPIO_InitPeripheral(GPIO_TEL_MUTE_DET_GRP, &gpio_init_input_pull_up);

	/* IR rx */
	gpio_init_input_float.Pin = GPIO_IR_RX_PIN;
	GPIO_InitPeripheral(GPIO_IR_RX_GRP, &gpio_init_input_float);

	/* ANT control */
	gpio_init_output.Pin = GPIO_ANT_CTRL_PIN;
	GPIO_InitPeripheral(GPIO_ANT_CTRL_GRP, &gpio_init_output);
	GPIO_ResetBits(GPIO_ANT_CTRL_GRP, GPIO_ANT_CTRL_PIN);

	/* DTV IR TX */
	gpio_init_output.Pin = GPIO_DTV_IR_TX_PIN;
	GPIO_InitPeripheral(GPIO_DTV_IR_TX_GRP, &gpio_init_output);
	GPIO_ResetBits(GPIO_DTV_IR_TX_GRP, GPIO_DTV_IR_TX_PIN);


	/* LCD enable */
	gpio_init_output.Pin = GPIO_LCD_EN_PIN;
	GPIO_InitPeripheral(GPIO_LCD_EN_GRP, &gpio_init_output);
	GPIO_ResetBits(GPIO_LCD_EN_GRP, GPIO_LCD_EN_PIN);

	/* Backlight enable */
	gpio_init_output.Pin = GPIO_BKL_EN_PIN;
	GPIO_InitPeripheral(GPIO_BKL_EN_GRP, &gpio_init_output);
	GPIO_ResetBits(GPIO_BKL_EN_GRP, GPIO_BKL_EN_PIN);


	/* DVD reset */
	gpio_init_output.Pin = GPIO_DVD_RST_PIN;
	GPIO_InitPeripheral(GPIO_DVD_RST_GRP, &gpio_init_output);
	GPIO_ResetBits(GPIO_DVD_RST_GRP, GPIO_DVD_RST_PIN);
	
	/* DVD power control */
	gpio_init_output.Pin = GPIO_DVD_PWR_CTRL_PIN;
	GPIO_InitPeripheral(GPIO_DVD_PWR_CTRL_GRP, &gpio_init_output);
	GPIO_ResetBits(GPIO_DVD_PWR_CTRL_GRP, GPIO_DVD_PWR_CTRL_PIN);
	
	/* DVD disc detect */
	gpio_init_input_pull_up.Pin = GPIO_DVD_DISC_DET_PIN;
	GPIO_InitPeripheral(GPIO_DVD_DISC_DET_GRP, &gpio_init_input_pull_up);
	gpio_init_input_pull_up.Pin = GPIO_DVD_TRAY_OUT_PIN;
	GPIO_InitPeripheral(GPIO_DVD_TRAY_OUT_GRP, &gpio_init_input_pull_up);
	gpio_init_input_pull_up.Pin = GPIO_DVD_TRAY_IN_PIN;
	GPIO_InitPeripheral(GPIO_DVD_TRAY_IN_GRP, &gpio_init_input_pull_up);
	
	/* DVD Load P/N */
	gpio_init_output.Pin = GPIO_DVD_LOAD_N_PIN;
	GPIO_InitPeripheral(GPIO_DVD_LOAD_N_GRP, &gpio_init_output);
	GPIO_ResetBits(GPIO_DVD_LOAD_N_GRP, GPIO_DVD_LOAD_N_PIN);
	gpio_init_output.Pin = GPIO_DVD_LOAD_P_PIN;
	GPIO_InitPeripheral(GPIO_DVD_LOAD_P_GRP, &gpio_init_output);
	GPIO_ResetBits(GPIO_DVD_LOAD_P_GRP, GPIO_DVD_LOAD_P_PIN);
	

	/* SWC key pull-up control */
	gpio_init_output.Pin = GPIO_SWC_KEY_CTL_2K2_PIN;
	GPIO_InitPeripheral(GPIO_SWC_KEY_CTL_2K2_GRP, &gpio_init_output);
	gpio_init_output.Pin = GPIO_SWC_KEY_CTL_470R_PIN;
	GPIO_InitPeripheral(GPIO_SWC_KEY_CTL_470R_GRP, &gpio_init_output);
	

	/* ADC */
	gpio_init_ain.Pin = GPIO_AD_BATT_DET_PIN;
	GPIO_InitPeripheral(GPIO_AD_BATT_DET_GRP, &gpio_init_ain);
	gpio_init_ain.Pin = GPIO_AD_VOL_PIN;
	GPIO_InitPeripheral(GPIO_AD_VOL_GRP, &gpio_init_ain);
	gpio_init_ain.Pin = GPIO_AD_PANEL_KEY_DET_1_PIN|GPIO_AD_PANEL_KEY_DET_2_PIN;
	GPIO_InitPeripheral(GPIO_AD_PANEL_KEY_DET_GRP, &gpio_init_ain);
	gpio_init_ain.Pin = GPIO_AD_SWC_KEY_DET_1_PIN|GPIO_AD_SWC_KEY_DET_2_PIN;
	GPIO_InitPeripheral(GPIO_AD_SWC_KEY_DET_GRP, &gpio_init_ain);


	/* LED control */
	gpio_init_output_af.Pin = GPIO_LED_B_PIN|GPIO_LED_G_PIN|GPIO_LED_R_PIN;
	gpio_init_output_af.GPIO_Alternate = GPIO_AF6_TIM8;
	gpio_init_output_af.GPIO_Pull = GPIO_No_Pull;
	GPIO_InitPeripheral(GPIO_LED_GRP, &gpio_init_output_af);
	gpio_init_output.Pin = GPIO_LED_EN_PIN;
	GPIO_InitPeripheral(GPIO_LED_GRP, &gpio_init_output);
	GPIO_ResetBits(GPIO_LED_GRP, GPIO_LED_EN_PIN);


	/* HOST recovery key */
	gpio_init_output.Pin = GPIO_HOST_REC_KEY_PIN;
	GPIO_InitPeripheral(GPIO_HOST_REC_KEY_GRP, &gpio_init_output);
	GPIO_SetBits(GPIO_HOST_REC_KEY_GRP, GPIO_HOST_REC_KEY_PIN);

	/* HOST power key */
	if (0!=g_first_love) {
		gpio_init_output.Pin = GPIO_HOST_PWR_KEY_PIN;
		GPIO_InitPeripheral(GPIO_HOST_PWR_KEY_GRP, &gpio_init_output);
		GPIO_ResetBits(GPIO_HOST_PWR_KEY_GRP, GPIO_HOST_PWR_KEY_PIN);
	}


	/* VCOM */
	gpio_init_output_af.Pin = GPIO_VCOM_PIN;
	gpio_init_output_af.GPIO_Alternate = GPIO_AF2_TIM1;
	gpio_init_output_af.GPIO_Pull = GPIO_No_Pull;
	GPIO_InitPeripheral(GPIO_VCOM_GRP, &gpio_init_output_af);


	/* BEEP */
	gpio_init_output_af.Pin = GPIO_BEEP_PIN;
	gpio_init_output_af.GPIO_Alternate = GPIO_AF2_TIM4;
	gpio_init_output_af.GPIO_Pull = GPIO_No_Pull;
	GPIO_InitPeripheral(GPIO_BEEP_GRP, &gpio_init_output_af);


}
static void uart_config(void)
{
	USART_InitType USART_InitStructure;
	NVIC_InitType NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* UART for HOST comm */
	NVIC_InitStructure.NVIC_IRQChannel = HOST_COMM_UART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	USART_DeInit(HOST_COMM_UART);
	USART_InitStructure.BaudRate            = 115200;
	USART_InitStructure.WordLength          = USART_WL_8B;
	USART_InitStructure.StopBits            = USART_STPB_1;
	USART_InitStructure.Parity              = USART_PE_NO;
	USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
	USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
	USART_Init(HOST_COMM_UART, &USART_InitStructure);
	USART_ConfigInt(HOST_COMM_UART, USART_INT_RXDNE, ENABLE);
	USART_Enable(HOST_COMM_UART, ENABLE);


	/* UART for CAN comm */
	NVIC_InitStructure.NVIC_IRQChannel = CAN_COMM_UART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	USART_Init(CAN_COMM_UART, &USART_InitStructure);
	USART_ConfigInt(HOST_COMM_UART, USART_INT_RXDNE, ENABLE);
	USART_Enable(HOST_COMM_UART, ENABLE);

}
static void adc_config(void)
{
	ADC_InitType  ADC_InitStructure;

	ADC_DeInit(ADC);
	ADC_InitStructure.MultiChEn      = DISABLE;
	ADC_InitStructure.ContinueConvEn = DISABLE;
	ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_NONE;
	ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;
	ADC_InitStructure.ChsNumber      = 1;
	ADC_Init(ADC, &ADC_InitStructure);

	ADC_Enable(ADC, ENABLE);
	while(ADC_GetFlagStatusNew(ADC, ADC_FLAG_RDY) == RESET);
	ADC_StartCalibration(ADC);
	while (ADC_GetCalibrationStatus(ADC));

}

static void timer_config(void)
{
	TIM_TimeBaseInitType  TIM_TimeBaseStructure;
	OCInitType TIM_OCInitStructure;

	/* VCOM */
	TIM_InitTimBaseStruct(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.Period    = 999;
	TIM_TimeBaseStructure.Prescaler = 3;
	TIM_TimeBaseStructure.ClkDiv    = TIM_CLK_DIV1;
	TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
	TIM_InitTimeBase(TIMER_VCOM, &TIM_TimeBaseStructure);

	TIM_InitOcStruct(&TIM_OCInitStructure);
	TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
	TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
	TIM_OCInitStructure.Pulse       = 115;
	TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_HIGH;
	TIM_InitOc1(TIMER_VCOM, &TIM_OCInitStructure);

	TIM_ConfigOc1Preload(TIMER_VCOM, TIM_OC_PRE_LOAD_ENABLE);
	TIM_ConfigArPreload(TIMER_VCOM, ENABLE);
	TIM_EnableCtrlPwmOutputs(TIMER_VCOM, ENABLE);
	TIM_Enable(TIMER_VCOM, ENABLE);

	/* Beep */
	TIM_TimeBaseStructure.Period    = 63;
	TIM_TimeBaseStructure.Prescaler = 999;
	TIM_InitTimeBase(TIMER_BEEP, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.Pulse       = 0;
	TIM_InitOc3(TIMER_BEEP, &TIM_OCInitStructure);

	TIM_ConfigOc3Preload(TIMER_BEEP, TIM_OC_PRE_LOAD_ENABLE);
	TIM_ConfigArPreload(TIMER_BEEP, ENABLE);
	TIM_Enable(TIMER_BEEP, ENABLE);

	/* LED color */
	TIM_TimeBaseStructure.Period    = 256;
	TIM_TimeBaseStructure.Prescaler = 999;
	TIM_InitTimeBase(TIMER_LED, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.Pulse       = 0;
	TIM_InitOc1(TIMER_LED, &TIM_OCInitStructure);
	TIM_InitOc2(TIMER_LED, &TIM_OCInitStructure);
	TIM_InitOc3(TIMER_LED, &TIM_OCInitStructure);

	TIM_ConfigOc1Preload(TIMER_LED, TIM_OC_PRE_LOAD_ENABLE);
	TIM_ConfigOc2Preload(TIMER_LED, TIM_OC_PRE_LOAD_ENABLE);
	TIM_ConfigOc3Preload(TIMER_LED, TIM_OC_PRE_LOAD_ENABLE);
	TIM_ConfigArPreload(TIMER_LED, ENABLE);

	TIM_EnableCtrlPwmOutputs(TIMER_LED, ENABLE);
	TIM_Enable(TIMER_LED, ENABLE);
}

static void mcu_init(void)
{
	g_counter_ms = 0;
	g_trigger_4ms = 0;
	g_trigger_12ms = 0;
	g_trigger_40ms = 0;
	g_trigger_100ms = 0;
	g_trigger_1000ms = 0;

	systick_config();

	rcu_config();
	rtc_config();
	gpio_config();
	adc_config();
	timer_config();
	uart_config();
	
	i2c_init();
	hal_init();
}

void main_goto_iap(void)
{
	if (((*(__IO uint32_t*)FLASH_ADDR_UPDATER) & 0x2FFE0000 ) == 0x20000000)        //ApplicationAddress为新程序的起始地址，检查栈顶地址是否合法，即栈顶地址是否为0x2000xxxx（内置SRAM）
	{

		SysTick->CTRL  = 0;

		// Mute
		AUDIO_HW_MUTE;

		// turn off BEEP
		TIM_SelectOcMode(TIMER_BEEP, TIMER_CH_BEEP, TIM_OCMODE_INACTIVE);
		TIM_Enable(TIMER_BEEP, DISABLE);
		TIM_DeInit(TIMER_BEEP);

		USART_ConfigInt(HOST_COMM_UART, USART_INT_RXDNE, DISABLE);
		USART_Enable(HOST_COMM_UART, DISABLE);

		USART_ConfigInt(CAN_COMM_UART, USART_INT_RXDNE, DISABLE);
		USART_Enable(CAN_COMM_UART, DISABLE);

		// todo : disable timer interrupt for IR rx

		JumpAddress = *(__IO uint32_t*) (FLASH_ADDR_UPDATER + 4);               //用户代码区第二个字存储为新程序起始地址（新程序复位向量指针）
		Jump_To_Application = (pFunction) JumpAddress;                          
		__set_MSP(*(__IO uint32_t*) FLASH_ADDR_UPDATER);                        //初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		Jump_To_Application();                                                  //设置PC指针为新程序复位中断函数的地址
	}
}

static void Task4msPro()
{
	g_trigger_4ms=0;
	LinUart_Resend_Time();	
	Usart_Resend_Time();

	encoder_key_main();
//	encoder2_key_main();
}
static void Task12msPro()
{
	g_trigger_12ms=0;
	CLEAR_WATCHDOG;
	radio_main();
#ifdef DVD_FUNCTION_ENABLE
	dvd_main();
#endif
	channel_main();
	Video_Main();
	Evt_Lifetime_Manage();			
	audio_main();
	Illumi_Detect();

	Voltage_Error_Detect(); 	
	ACC_On_Detect(N_SAMPLING_ACC);

	panel_key_main();

	CLEAR_WATCHDOG;
 	ACC_Off_Detect();
	PowerManage();
	LinTxWince_Service();
	Navi_Audio_Detect();	
	Game_Audio_Detect();
	beep_main();
	if(F_SystemOff==0)
	{
		Generate_CheckSum();
	}

	Canbox_Main();

	Reverse_Detect();

// todo 	ir_rx_main();
// todo	can_ir_main();

}
static void Task40msPro()
{
	g_trigger_40ms=0;		
	MMI();
	Parking_Pro();
	ABRT_Driver();
}
static void Task100msPro()
{
	
	g_trigger_100ms=0;

	led_main();

	app_wd_main();
}
static void Task1sPro()
{	
	g_trigger_1000ms=0;
	batt_main();
	if (g_startup_cntr<250) {
		++g_startup_cntr;
	}

	if (g_super_watchdog_timer>0) {
		--g_super_watchdog_timer;
		if ( (g_startup_cntr>10) && (0==g_super_watchdog_timer) ) {
			NVIC_SystemReset();	// force reset 
		}
	}
}


void VariableInit(void)
{
	nPowerState=POWER_SYSTEM_RESET;

	g_fake_pwr_off = FALSE;

	g_app_in_charge = FALSE;

	g_bt_type = 0;

	g_startup_cntr = 0;

	g_super_watchdog_timer = 0;

	audio_init();
	radio_init();
#ifdef DVD_FUNCTION_ENABLE
	dvd_init();
#endif
	led_init();
	beep_init();
// todo	ir_rx_init();
// todo	ir_tx_init();
// todo	can_ir_init();
	panel_key_init();
	if(Check_CheckSum())
	{		
	}
	else
	{
		Init_Nochange_Section();
	}

	Clr_Machine_Power();
	InitEvent();

	PowerOffReason=POWER_STANDBY;

	g_brightness = -1;

// todo	g_rtc_h = 0;
// todo	g_rtc_l = 0;
// todo	g_rtc_cntr = 0;
}

static void mcu_stay_in_sleep(void)
{
	GPIO_InitType  gpio_init_input_float;
	u16 acc_on = 0;
	u16 acc_off = 0;
	NVIC_InitType NVIC_InitStructure;
	EXTI_InitType EXTI_InitStructure;

	/* shutdown unused mcu pin & clock & power */
	GPIO_InitStruct(&gpio_init_input_float);
	gpio_init_input_float.GPIO_Mode = GPIO_Mode_Analog;
	gpio_init_input_float.GPIO_Pull = GPIO_No_Pull;

	/* UART HOST */
	gpio_init_input_float.Pin = GPIO_HOST_UART_TX_PIN;
	GPIO_InitPeripheral(GPIO_HOST_UART_TX_GRP, &gpio_init_input_float);
	gpio_init_input_float.Pin = GPIO_HOST_UART_RX_PIN;
	GPIO_InitPeripheral(GPIO_HOST_UART_RX_GRP, &gpio_init_input_float);
	/* UART CAN */
	gpio_init_input_float.Pin = GPIO_CAN_UART_TX_PIN;
	GPIO_InitPeripheral(GPIO_CAN_UART_TX_GRP, &gpio_init_input_float);
	gpio_init_input_float.Pin = GPIO_CAN_UART_RX_PIN;
	GPIO_InitPeripheral(GPIO_CAN_UART_RX_GRP, &gpio_init_input_float);
	/* Bake */
	gpio_init_input_float.Pin = GPIO_BRAKE_PIN;
	GPIO_InitPeripheral(GPIO_BRAKE_GRP, &gpio_init_input_float);
	/* Ill detect */
	gpio_init_input_float.Pin = GPIO_ILL_PIN;
	GPIO_InitPeripheral(GPIO_ILL_GRP, &gpio_init_input_float);
	/* Reverse detect */
	gpio_init_input_float.Pin = GPIO_REVERSE_PIN;
	GPIO_InitPeripheral(GPIO_REVERSE_GRP, &gpio_init_input_float);
	/* Telephone mute detect */
	gpio_init_input_float.Pin = GPIO_TEL_MUTE_DET_PIN;
	GPIO_InitPeripheral(GPIO_TEL_MUTE_DET_GRP, &gpio_init_input_float);
	/* IR rx */
	gpio_init_input_float.Pin = GPIO_IR_RX_PIN;
	GPIO_InitPeripheral(GPIO_IR_RX_GRP, &gpio_init_input_float);
	/* MUTE control */
	AUDIO_HW_UNMUTE;	// save power

	/* ANT control */
	gpio_init_input_float.Pin = GPIO_ANT_CTRL_PIN;
//	GPIO_InitPeripheral(GPIO_ANT_CTRL_GRP, &gpio_init_input_float);
	/* DTV IR TX */
	gpio_init_input_float.Pin = GPIO_DTV_IR_TX_PIN;
	GPIO_InitPeripheral(GPIO_DTV_IR_TX_GRP, &gpio_init_input_float);
	/* LCD enable */
	gpio_init_input_float.Pin = GPIO_LCD_EN_PIN;
	GPIO_InitPeripheral(GPIO_LCD_EN_GRP, &gpio_init_input_float);
	/* Backlight enable */
	gpio_init_input_float.Pin = GPIO_BKL_EN_PIN;
	GPIO_InitPeripheral(GPIO_BKL_EN_GRP, &gpio_init_input_float);
	/* DVD reset */
	gpio_init_input_float.Pin = GPIO_DVD_RST_PIN;
	GPIO_InitPeripheral(GPIO_DVD_RST_GRP, &gpio_init_input_float);
	GPIO_ResetBits(GPIO_DVD_RST_GRP, GPIO_DVD_RST_PIN);
	/* DVD power control */
	gpio_init_input_float.Pin = GPIO_DVD_PWR_CTRL_PIN;
	GPIO_InitPeripheral(GPIO_DVD_PWR_CTRL_GRP, &gpio_init_input_float);
	/* DVD disc detect */
	gpio_init_input_float.Pin = GPIO_DVD_DISC_DET_PIN;
	GPIO_InitPeripheral(GPIO_DVD_DISC_DET_GRP, &gpio_init_input_float);
	gpio_init_input_float.Pin = GPIO_DVD_TRAY_OUT_PIN;
	GPIO_InitPeripheral(GPIO_DVD_TRAY_OUT_GRP, &gpio_init_input_float);
	gpio_init_input_float.Pin = GPIO_DVD_TRAY_IN_PIN;
	GPIO_InitPeripheral(GPIO_DVD_TRAY_IN_GRP, &gpio_init_input_float);
	/* DVD Load P/N */
	gpio_init_input_float.Pin = GPIO_DVD_LOAD_N_PIN;
	GPIO_InitPeripheral(GPIO_DVD_LOAD_N_GRP, &gpio_init_input_float);
	gpio_init_input_float.Pin = GPIO_DVD_LOAD_P_PIN;
	GPIO_InitPeripheral(GPIO_DVD_LOAD_P_GRP, &gpio_init_input_float);
	/* SWC key pull-up control */
	gpio_init_input_float.Pin = GPIO_SWC_KEY_CTL_2K2_PIN;
	GPIO_InitPeripheral(GPIO_SWC_KEY_CTL_2K2_GRP, &gpio_init_input_float);
	gpio_init_input_float.Pin = GPIO_SWC_KEY_CTL_470R_PIN;
	GPIO_InitPeripheral(GPIO_SWC_KEY_CTL_470R_GRP, &gpio_init_input_float);
	/* ADC */
	gpio_init_input_float.Pin = GPIO_AD_BATT_DET_PIN;
	GPIO_InitPeripheral(GPIO_AD_BATT_DET_GRP, &gpio_init_input_float);
	gpio_init_input_float.Pin = GPIO_AD_VOL_PIN;
	GPIO_InitPeripheral(GPIO_AD_VOL_GRP, &gpio_init_input_float);
	gpio_init_input_float.Pin = GPIO_AD_PANEL_KEY_DET_1_PIN|GPIO_AD_PANEL_KEY_DET_2_PIN;
	GPIO_InitPeripheral(GPIO_AD_PANEL_KEY_DET_GRP, &gpio_init_input_float);
	/* LED control */
	gpio_init_input_float.Pin = GPIO_LED_B_PIN|GPIO_LED_G_PIN|GPIO_LED_R_PIN;
	GPIO_InitPeripheral(GPIO_LED_GRP, &gpio_init_input_float);
//	gpio_init_input_float.Pin = GPIO_LED_EN_PIN;
//	GPIO_InitPeripheral(GPIO_LED_GRP, &gpio_init_input_float);

	/* HOST recovery key */
	gpio_init_input_float.Pin = GPIO_HOST_REC_KEY_PIN;
	GPIO_InitPeripheral(GPIO_HOST_REC_KEY_GRP, &gpio_init_input_float);
	/* VCOM */
	gpio_init_input_float.Pin = GPIO_VCOM_PIN;
	GPIO_InitPeripheral(GPIO_VCOM_GRP, &gpio_init_input_float);
	/* BEEP */
	gpio_init_input_float.Pin = GPIO_BEEP_PIN;
	GPIO_InitPeripheral(GPIO_BEEP_GRP, &gpio_init_input_float);

	/* I2C */
	gpio_init_input_float.Pin = GPIO_I2C_SCL_PIN;
	GPIO_InitPeripheral(GPIO_I2C_GRP, &gpio_init_input_float);
	gpio_init_input_float.Pin = GPIO_I2C_SDA_PIN;
	GPIO_InitPeripheral(GPIO_I2C_GRP, &gpio_init_input_float);

	/* Uart */
	USART_Enable(HOST_COMM_UART, DISABLE);
	USART_Enable(CAN_COMM_UART, DISABLE);

	/* ADC */
	ADC_Enable(ADC, DISABLE);

	/* Timer */
	TIM_Enable(TIMER_VCOM, DISABLE);
	TIM_Enable(TIMER_BEEP, DISABLE);
	TIM_Enable(TIMER_LED, DISABLE);

	/* Clock */
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_USART1, DISABLE);
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART2, DISABLE);
	RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_ADC, DISABLE);
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM1, DISABLE);
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM4, DISABLE);
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM8, DISABLE);

	while(1) {
		acc_on = 0;
		acc_off = 0;
		for(;;) {
			delay_us(2);
			if (Bit_RESET==GPIO_ReadInputDataBit(GPIO_ACC_GRP,GPIO_ACC_PIN)) {
				acc_on++;
				acc_off=0;
			} else {
				acc_on=0;
				acc_off++;
			}
			if (acc_on>10000) {
				break;
			}
			if (acc_off>10000) {
				break;
			}
		}
		if (acc_on>10000) {
			break;
		}

		GPIO_ConfigEXTILine(GPIOA_PORT_SOURCE, GPIO_PIN_SOURCE0);
		EXTI_InitStructure.EXTI_Line = EXTI_LINE0;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_InitPeripheral(&EXTI_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

//		DBG_ConfigPeriph(DBG_STOP, ENABLE);

		PWR_EnterSTOP2Mode(PWR_STOPENTRY_WFI, PWR_CTRL3_RAM1RET);
//		SystemInit();

		// we already exit the sleep mode
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_InitPeripheral(&EXTI_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
		NVIC_Init(&NVIC_InitStructure);
	}

	AUDIO_HW_MUTE;

	systick_config();

	rcu_config();
	gpio_config();
	adc_config();
	timer_config();
	uart_config();
	
	i2c_init();
}


int main(void)
{
	g_first_love = 1;
	mcu_init();

	REAL_SYS_PWR_OFF;

	PowerInit();	
	app_wd_init();

	USART_RxTx_Init(FALSE);
	
	VariableInit();
	delay_1ms(500);
	REAL_SYS_PWR_ON;


	BEGIN_WATCHDOG;//Watch Dog is enabled

	g_first_love = 0;

	for(;;)
	{
		CLEAR_WATCHDOG;

		if (g_mcu_in_sleep) {
			mcu_stay_in_sleep();
			g_mcu_in_sleep = 0;
		}

		USART_Data_Analyse();
		LinUart_Data_Analyse();

 		TimeManage();
		if(g_trigger_4ms)
		{
			g_trigger_4ms = 0;
			Task4msPro();
		}
		
		 if(g_trigger_12ms)
		{
			g_trigger_12ms=0;
			Task12msPro();
		}
		 
		if(g_trigger_40ms)
		{
			g_trigger_40ms=0;
			Task40msPro();
		}
		
		 if(g_trigger_100ms)
		{
			g_trigger_100ms=0;
			Task100msPro();
		}
		 if(g_trigger_1000ms)
		 {
		 	g_trigger_1000ms=0;
			Task1sPro();
		 }
	}
}
