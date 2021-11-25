
#include "public.h"


#if ASP_MODEL==ASP_BU32107

const u8 g_eq_preset_level[EQ_MODE_NUMS-1][EQ_FREQ_NUMS]={
                /*  80HZ    125HZ  200HZ_   200HZ   315HZ  500HZ    800HZ   1.25K_     1.25K     2K     3.15K    5K     8K_    8K    12.5K  */
/*FLAT*/    {    10,      10,       10,        10,        10,      10,       10,       10,        10,        10,      10,     10,    10,    10,    10    },
/*JAZZ*/    {    11,      12,       10,        8,        8,       8,        10,       11,        13,        12,      11,     8,    4,    7,    9    },
/*POP*/     {    11,      11,       12,        13,        14,      14,       15,        15,          14,       12,      11,     12,    11,    10,    11    },
/*CLSC*/    {    13,      13,       14,        15,        15,       16,       16,       15,        13,        11,      10,    9,    9,    9,    9    },
/*ROCK*/   {    13,      14,       14,         13,       12,       10,        5,         4,          6,       8,      11,     13,    13,    16,    16    },
/*DBB*/     {    13,      13,       12,         11,       11,       10,        10,      10,       11,       13,      16,     16,    12,    10,    10    },
};

#else

const u8 g_eq_preset_level[EQ_MODE_NUMS-1][EQ_FREQ_NUMS]={
                /*  60HZ    100HZ  120HZ   0.5KHZ  1KHZ    1.5KHZ  10KHZ   12.5KHZ 15KHZ	*/
/*FLAT*/    {    10,      10,       10,        10,      10,       10,       10,        10,        10    },
/*JAZZ*/    {    20,      16,       12,        10,       6,         2,       14,         17,       20    },
/*POP*/     {     9,       11,       13,        17,      20,       17,       11,          9,         7    },
/*CLSC*/    {    20,      18,       16,         8,        2,        8,        16,        18,       20    },
/*ROCK*/    {    20,      18,       16,         9,        6,        9,       16,         18,       20    },
/*NEWS*/   {    5,        8,        11,        14,       17,      14,       11,          8,        5    },
/*URBN*/    {    18,      16,       14,        10,        9,       10,       14,        16,       18    },
/*TECH*/    {    20,      18,       16,        10,        9,       10,       12,        14,       16    },
/*FILM*/     {     7,       8,        11,        14,       18,       14,       11,         8,        7    },
/*MIDI*/     {    20,      18,       16,        10,        9,       10,       12,        14,       16    }
};

#endif

void audio_eq_init(void)
{
	u8 cnt;
	g_audio_info.eq_mode = EQ_MODE_FLAT;

	for (cnt=0; cnt<EQ_FREQ_NUMS; cnt++) {
		g_audio_info.eq_visible_level[cnt] = DEFAULT_EQ_LEVEL;
		g_audio_info.eq_custom_level[cnt] = DEFAULT_EQ_LEVEL;
	}

#if ASP_MODEL==ASP_BU32107
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].eq_mode = g_audio_info.eq_mode;
	for (cnt=0; cnt<EQ_FREQ_NUMS; cnt++) {
		g_dsp_set_v2[g_audio_info.dsp_set_user_id].eq_custom_level[cnt] = g_audio_info.eq_custom_level[cnt];
	}
#endif

}

static void _audio_eq_update_value(void)
{
	u8 cnt;

#if ASP_MODEL==ASP_BU32107
	if (EQ_MODE_CUSTOM == g_audio_info.eq_mode) {
		for (cnt=0; cnt<EQ_FREQ_NUMS; cnt++) {
			g_audio_info.eq_visible_level[cnt] = g_audio_info.eq_custom_level[cnt];
		}		
	} else {
		for (cnt=0; cnt<EQ_FREQ_NUMS; cnt++) {
			g_audio_info.eq_visible_level[cnt] = g_eq_preset_level[g_audio_info.eq_mode-1][cnt];
		}		
	}
	for (cnt=0; cnt<EQ_FREQ_NUMS; cnt++) {
		g_dsp_set_v2[g_audio_info.dsp_set_user_id].eq_custom_level[cnt] = g_audio_info.eq_custom_level[cnt];
	}
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].eq_mode = g_audio_info.eq_mode;
#else
	if (EQ_MODE_CUSTOM == g_audio_info.eq_mode) {
		for (cnt=0; cnt<EQ_FREQ_NUMS; cnt++) {
			g_audio_info.eq_visible_level[cnt] = g_audio_info.eq_custom_level[cnt];
		}		
	} else {
		for (cnt=0; cnt<EQ_FREQ_NUMS; cnt++) {
			g_audio_info.eq_visible_level[cnt] = g_eq_preset_level[g_audio_info.eq_mode-1][cnt];
		}		
	}
#endif
}

void audio_eq_update_mode(EQ_MODE mode)
{
	g_audio_info.eq_mode = mode;
	_audio_eq_update_value();
}

void audio_eq_update_level(EQ_FREQ freq, u8 level)
{
	u8 cnt;

	// update the custom level from current visible level, because
	// maybe we are adjusting custom level from the preset levels.
	for (cnt=0; cnt<EQ_FREQ_NUMS; cnt++) {
		g_audio_info.eq_custom_level[cnt] = g_audio_info.eq_visible_level[cnt];
	}		
	g_audio_info.eq_mode = EQ_MODE_CUSTOM;
	g_audio_info.eq_custom_level[freq] = level;

	_audio_eq_update_value();
}


