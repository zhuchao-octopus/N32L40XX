
#include "public.h"

static void do_beep(void)
{
	switch (g_beep_info.mode) {
		case BEEP_MODE_SHORT:
			if (0 == g_beep_info.timer) {
				TIM_SetCmp3(TIMER_BEEP, 32);
			} else if (g_beep_info.timer > T36MS_12) {
				g_beep_info.mode = BEEP_MODE_NONE;
			}
			break;
		case BEEP_MODE_DOUBLE:
			if (0 == g_beep_info.timer) {
				TIM_SetCmp3(TIMER_BEEP, 32);
			} else if ( T50MS_12 == g_beep_info.timer) {
				TIM_SetCmp3(TIMER_BEEP, 0);
			} else if ( T100MS_12 == g_beep_info.timer) {
				TIM_SetCmp3(TIMER_BEEP, 32);
			} else if (g_beep_info.timer > T120MS_12) {
				g_beep_info.mode = BEEP_MODE_NONE;
			}
			break;
		case BEEP_MODE_LONG:
			if (0 == g_beep_info.timer) {
				TIM_SetCmp3(TIMER_BEEP, 32);
			} else if (g_beep_info.timer > T500MS_12) {
				g_beep_info.mode = BEEP_MODE_NONE;
			}
			break;
		case BEEP_MODE_NONE:
			TIM_SetCmp3(TIMER_BEEP, 0);
			g_beep_info.state = BEEP_STATE_IDLE;
			break;
	}

	++g_beep_info.timer;
}

void beep_init(void)
{
	g_beep_info.state = BEEP_STATE_PWR_DOWN;
	g_beep_info.mode = BEEP_MODE_NONE;
	g_beep_info.timer = 0;
	TIM_SetCmp3(TIMER_BEEP, 0);
}

void beep_main(void)
{
	if (!Is_Machine_Power) {
		beep_init();
		return;
	}

	if(!g_sys_info_store.beep_onoff) {
		beep_init();
		return;
	}
	
	switch (g_beep_info.state) {
		case BEEP_STATE_PWR_DOWN:
			g_beep_info.state = BEEP_STATE_IDLE;
			break;
		case BEEP_STATE_IDLE:
			break;
		case BEEP_STATE_BEEP_ING:
			audio_set_mute(AUDIO_MUTE_DRIVER, FALSE);
			do_beep();
			break;
		default:
			break;
	}
}

void beep_short_mode(void)
{
	if (BEEP_STATE_IDLE!=g_beep_info.state) {
		return;
	}
	if (0==audio_get_volume()) {
		return;
	}
	g_beep_info.mode = BEEP_MODE_SHORT;
	g_beep_info.state = BEEP_STATE_BEEP_ING;
	g_beep_info.timer = 0;
}

void beep_double_mode(void)
{
	if (BEEP_STATE_IDLE!=g_beep_info.state) {
		return;
	}
	if (0==audio_get_volume()) {
		return;
	}
	g_beep_info.mode = BEEP_MODE_DOUBLE;
	g_beep_info.state = BEEP_STATE_BEEP_ING;
	g_beep_info.timer = 0;
}

void beep_long_mode(void)
{
	if (BEEP_STATE_IDLE!=g_beep_info.state) {
		return;
	}
	if (0==audio_get_volume()) {
		return;
	}
	g_beep_info.mode = BEEP_MODE_LONG;
	g_beep_info.state = BEEP_STATE_BEEP_ING;
	g_beep_info.timer = 0;
}

