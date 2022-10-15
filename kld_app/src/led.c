
#include "public.h"

#define WHITE_LED_COLOR_LEVEL_HIGH

static void led_power_handler(void)
{
	bool pwr_up = FALSE;
	
	if (Is_Machine_Power) {
		if (g_led_info.notify_reset) {
			++g_led_info.notify_timer;
			if (g_led_info.notify_timer > T500MS_100) {
				g_led_info.notify_timer = 0;
				pwr_up = (g_led_info.is_power_up)?FALSE:TRUE;
			}
		} else if (2==g_sys_info_store.led_always_on) {
			pwr_up = TRUE;
		} else if (1==1) {//g_sys_info_store.led_always_on) {
			if (g_fake_pwr_off) {
				pwr_up = FALSE;
			} else {
				pwr_up = TRUE;
			}
		} else if (F_ILLUMIN_CONTORL_MODE == ILLUMIN_NIGHT_MODE) {
			pwr_up = TRUE;
		} else if (F_ILLUMIN_CONTORL_MODE == ILLUMIN_DAY_MODE) {
			pwr_up = FALSE;
		} else if (F_ILLUMIN_CONTORL_MODE == ILLUMIN_AUTO_MODE) {
			if (Det_Flag.field.F_Lighting_Det==1) {
				pwr_up = TRUE;
			} else {
				pwr_up = FALSE;
			}
		}
	} else {
		pwr_up = FALSE;
	}

	if (g_led_info.temp_on_timer>0) {
		--g_led_info.temp_on_timer;
		pwr_up = TRUE;
	}

	if (pwr_up != g_led_info.is_power_up) {
		g_led_info.is_power_up = pwr_up;
		if (g_led_info.is_power_up) {
			GPIO_SetBits(GPIO_LED_GRP, GPIO_LED_EN_PIN);
		} else {
			GPIO_ResetBits(GPIO_LED_GRP, GPIO_LED_EN_PIN);
		}
	}
}

void led_init(void)
{
	g_led_info.is_power_up = FALSE;
	g_led_info.color_changed = FALSE;
	g_led_info.temp_on_timer = 0;
	g_led_info.save_color_timer = 0;
	g_led_info.notify_reset = FALSE;
	g_led_info.notify_timer = 0;
	g_led_info.auto_color_timer = 0;
}

static void led_auto_color()
{
	static u8 r=0;
	static u8 g=0;
	static u8 b=0;
	
#define AUTO_COLOR_STEP 	25

	++g_led_info.auto_color_timer;
	if (g_led_info.auto_color_timer>T6S_100) {
		g_led_info.auto_color_timer = 0;
		r = 0;
		g = 0;
		b = LED_LEVEL_MAX;
		return;
	} else if (g_led_info.auto_color_timer>T5S_100) {
		r = 0;
		g -= AUTO_COLOR_STEP;
		b = LED_LEVEL_MAX;
	} else if (g_led_info.auto_color_timer>T4S_100) {
		r = 0;
		g = LED_LEVEL_MAX;
		b += AUTO_COLOR_STEP;
	} else if (g_led_info.auto_color_timer>T3S_100) {
		r -= AUTO_COLOR_STEP;
		g = LED_LEVEL_MAX;
		b = 0;
	} else if (g_led_info.auto_color_timer>T2S_100) {
		r = LED_LEVEL_MAX;
		g += AUTO_COLOR_STEP;
		b = 0;
	} else if (g_led_info.auto_color_timer>T1S_100) {
		r = LED_LEVEL_MAX;
		g = 0;
		b -= AUTO_COLOR_STEP;
	} else {
		r += AUTO_COLOR_STEP;
		g = 0;
		b = LED_LEVEL_MAX;
	}

//	TIM_SetCmp1(TIMER_LED, b);
//	TIM_SetCmp2(TIMER_LED, g);
//	TIM_SetCmp3(TIMER_LED, r);

}

void led_main(void)
{
	led_power_handler();
	
	if (Is_Machine_Power) {
		if ((LED_LEVEL_MAX==g_sys_info_store.led_r_level) && (LED_LEVEL_MAX==g_sys_info_store.led_g_level) && (LED_LEVEL_MAX==g_sys_info_store.led_b_level)) {
			if (g_led_info.is_power_up) {
				led_auto_color();
			}
		} else if (g_led_info.color_changed) {
			if ((0xFE==g_sys_info_store.led_r_level) && (0xFE==g_sys_info_store.led_g_level) && (0xFE==g_sys_info_store.led_b_level)) {
#ifdef WHITE_LED_COLOR_LEVEL_HIGH
//				TIM_SetCmp1(TIMER_LED, g_sys_info_store.led_b_level);
//				TIM_SetCmp2(TIMER_LED, g_sys_info_store.led_g_level);
//				TIM_SetCmp3(TIMER_LED, g_sys_info_store.led_r_level);
#else
//				TIM_SetCmp1(TIMER_LED, 0);
//				TIM_SetCmp2(TIMER_LED, 0);
//				TIM_SetCmp3(TIMER_LED, 0);
#endif
			} else {
//				TIM_SetCmp1(TIMER_LED, g_sys_info_store.led_b_level);
//				TIM_SetCmp2(TIMER_LED, g_sys_info_store.led_g_level);
//				TIM_SetCmp3(TIMER_LED, g_sys_info_store.led_r_level);
			}
			g_led_info.color_changed = FALSE;
		}
	} else {
		// make sure next power up will config the led color
		g_led_info.color_changed = TRUE;
	}

	if (g_led_info.save_color_timer>0) {
		--g_led_info.save_color_timer;
		if (0==g_led_info.save_color_timer) {
			ak_flash_save_info();
		}
	}

}

void led_set_color(u8 r_lvl, u8 g_lvl, u8 b_lvl)
{
	if (r_lvl<LED_LEVEL_MIN)
		r_lvl = LED_LEVEL_MIN;
	if (g_lvl<LED_LEVEL_MIN)
		g_lvl = LED_LEVEL_MIN;
	if (b_lvl<LED_LEVEL_MIN)
		b_lvl = LED_LEVEL_MIN;
	
	g_led_info.color_changed = TRUE;
	g_led_info.temp_on_timer = T5S_100;
	if ((r_lvl!=g_sys_info_store.led_r_level) ||
		(g_lvl!=g_sys_info_store.led_g_level) ||
		(b_lvl!=g_sys_info_store.led_b_level)) {
		g_sys_info_store.led_r_level = r_lvl;
		g_sys_info_store.led_g_level = g_lvl;
		g_sys_info_store.led_b_level = b_lvl;
		g_led_info.save_color_timer = T2S_100;
	}
}

void led_set_always_on(u8 on)
{
	if (on!=g_sys_info_store.led_always_on) {
		g_sys_info_store.led_always_on = on;
		ak_flash_save_info();
	}
}

void led_notify_reset(void)
{
	g_led_info.notify_reset = TRUE;
}

