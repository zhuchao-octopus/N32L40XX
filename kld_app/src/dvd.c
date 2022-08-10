#include "public.h"

#ifdef DVD_FUNCTION_ENABLE

static void dvd_pwr_handler(void)
{
	
	if (g_dvd_info.dvd_pwr_up) {
		if (g_dvd_info.pwr_timer<=T3S_12) {
			++g_dvd_info.pwr_timer;
			if (g_dvd_info.pwr_timer==T3S_12) {
				GPIO_SetBits(GPIO_DVD_PWR_CTRL_GRP, GPIO_DVD_PWR_CTRL_PIN);
				GPIO_SetBits(GPIO_DVD_RST_GRP, GPIO_DVD_RST_PIN);
			} else if (g_dvd_info.pwr_timer>T1S_12) {
				GPIO_SetBits(GPIO_DVD_PWR_CTRL_GRP, GPIO_DVD_PWR_CTRL_PIN);
				GPIO_ResetBits(GPIO_DVD_RST_GRP, GPIO_DVD_RST_PIN);
			} else {
				GPIO_ResetBits(GPIO_DVD_PWR_CTRL_GRP, GPIO_DVD_PWR_CTRL_PIN);
				GPIO_ResetBits(GPIO_DVD_RST_GRP, GPIO_DVD_RST_PIN);
			}
		}
	}

}

void dvd_init(void)
{
	g_dvd_info.pwr_timer = 0;
	g_dvd_info.dvd_pwr_up = FALSE;
}

void dvd_poweron(void)
{
	g_dvd_info.dvd_pwr_up = TRUE;
	g_dvd_info.pwr_timer = 0;
}

void dvd_poweroff(void)
{
	g_dvd_info.dvd_pwr_up = FALSE;
	GPIO_ResetBits(GPIO_DVD_PWR_CTRL_GRP, GPIO_DVD_PWR_CTRL_PIN);
	GPIO_ResetBits(GPIO_DVD_RST_GRP, GPIO_DVD_RST_PIN);
}

void dvd_main(void)
{
	if (Is_Machine_Power) {
		dvd_pwr_handler();
	}
}

bool dvd_is_audio_ready(void)
{
	return TRUE;
}

#endif

