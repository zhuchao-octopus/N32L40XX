
#include "public.h"

//#define AUDIO_INIT_SRC_FOR_TEST AUDIO_SRC_HOST


static u8 _audio_change_volume(u8 start_vol, u8 target_vol)
{
	u8 vol;
	if (start_vol < target_vol) {
		vol = start_vol + 1;
	} else if (start_vol > target_vol) {
		vol = start_vol - 1;
	} else {
		vol = start_vol;
	}
	return vol;
}

static AUDIO_SOURCE _audio_select_source(SOURCE av_src)
{
	AUDIO_SOURCE src = AUDIO_SRC_NONE;
	switch (av_src) {
		case SOURCE_TUNER:
		case SOURCE_XM:
		case SOURCE_SIRIUS:
			src = AUDIO_SRC_RADIO;
			break;
		case SOURCE_DVD:
		case SOURCE_DVDC:
			src = AUDIO_SRC_DVD;
			break;
		case SOURCE_TV:
		case SOURCE_DTV:
		case SOURCE_HDRADIO:
			src = AUDIO_SRC_TV;
			break;
		case SOURCE_AUX:
		case SOURCE_FRONT_AUX:
			src = AUDIO_SRC_AUXIN;
			break;
		case SOURCE_SD:
		case SOURCE_USB:
			src = AUDIO_SRC_HOST;
			break;
		case SOURCE_BT:
			if (1==g_bt_type) {
				src = AUDIO_SRC_HOST;
			} else if (2==g_bt_type) {
				src = AUDIO_SRC_HOST;
			} else {
				src = AUDIO_SRC_BT_MODULE;
			}
			break;
		case SOURCE_AVOFF:
		case NUM_OF_SOURCE:
		default:
			src = AUDIO_SRC_NONE;
			break;
	}
	return src;
}

static void _audio_do_set_navi_break(bool on)
{
	if (g_audio_info.navi_break_on!= on) {
		g_audio_info.navi_break_on = on;
		audio_dev_update_navi_mix(g_audio_info.navi_break_on);
		audio_dev_update_navi_mix_vol(g_audio_info.navi_mix_vol);
		audio_dev_update_fader_balance(g_audio_info.fader, g_audio_info.balance);
	}

	if (g_audio_info.navi_break_on) {
		audio_set_mute(AUDIO_MUTE_DRIVER, FALSE);
	}	
}

static void audio_reverse_handler(void)
{
	bool should_update_navi_mix_vol = FALSE;
	if ( (!Get_Reverse_Det_Flag) && (g_audio_info.reverse_on) ) {
		should_update_navi_mix_vol = TRUE;
	} else if ( (Get_Reverse_Det_Flag) && (!g_audio_info.reverse_on) ) {
		should_update_navi_mix_vol = TRUE;
	}
	
	if (Get_Reverse_Det_Flag) {
		g_audio_info.reverse_on = TRUE;
	} else {
		g_audio_info.reverse_on = FALSE;
	}

	if (should_update_navi_mix_vol) {
		audio_dev_update_navi_mix_vol(g_audio_info.navi_mix_vol);
	}
}

static void audio_source_handler(void)
{
	AUDIO_SOURCE src = AUDIO_SRC_NONE;

	if (g_audio_info.bt_phone_on) {
		if (g_audio_info.carplay_phone_on) {
			src = AUDIO_SRC_HOST;
		} else {
			if (1==g_bt_type) {
				src = AUDIO_SRC_HOST;
			} else if (2==g_bt_type) {
				src = AUDIO_SRC_HOST;
			} else {
				src = AUDIO_SRC_BT_MODULE;
			}
		}
	} else if (g_audio_info.bt_voice_on) {
		src = AUDIO_SRC_BT_MODULE;
	} else if (g_fake_pwr_off) {
		src = AUDIO_SRC_NONE;
	} else if (g_audio_info.rds_ta_break_on) {
		src = AUDIO_SRC_RADIO;
	} else if (g_audio_info.app_3rd_break_on) {
		src = AUDIO_SRC_HOST;
	} else {
		src = g_audio_info.sys_source;
	}

	if (g_audio_info.bt_phone_timer) {
		// we just switch to/from bt phone
		--g_audio_info.bt_phone_timer;
		src = AUDIO_SRC_NONE;
	}

	if ( (AUDIO_SRC_RADIO == src) && (AUDIO_MUTE_RADIO==g_audio_info.mute) ){
		src = AUDIO_SRC_NONE;
	}

	if ( (AUDIO_SRC_DVD == src) && (!dvd_is_audio_ready()) ) {
		g_audio_info.src_sw_state = AUDIO_SRC_SW_STATE_READY;
		src = AUDIO_SRC_NONE;
	}

	if ((AUDIO_SRC_HOST == src) && (!g_audio_info.android_snd_on)) {
		src =AUDIO_SRC_NONE;
	}

	if (AUDIO_SRC_NONE != src) {
		audio_set_mute(AUDIO_MUTE_DRIVER, FALSE);
	}
	
	if (src != g_audio_info.cur_source) {
		if (AUDIO_SRC_HOST == g_audio_info.cur_source) {
			// we are exiting HOST channel, do navi mix on if need
			if (g_audio_info.navi_break_on_cache) {
				_audio_do_set_navi_break(TRUE);
			}
		} else if (AUDIO_SRC_HOST == src) {
			// we are entering HOST channel, do naiv mix off if it's already on
			if (g_audio_info.navi_break_on_cache) {
				_audio_do_set_navi_break(FALSE);
			}
		}
		
		g_audio_info.cur_source = src;
		if (g_audio_info.app_3rd_break_on) {
			g_audio_info.src_sw_state = AUDIO_SRC_SW_STATE_READY;
		} else {
			g_audio_info.src_sw_state = AUDIO_SRC_SW_STATE_PREPARE;
		}
	}
	if (AUDIO_SRC_SW_STATE_READY == g_audio_info.src_sw_state) {
		audio_dev_update_source(g_audio_info.cur_source);
		audio_dev_update_eq();
		audio_dev_update_loudness(g_audio_info.loudness);
		g_audio_info.src_sw_state = AUDIO_SRC_SW_STATE_NONE;
	}
}

static void audio_volume_handler(void)
{
	u8 vol, target_vol;
	u16 tmp;

	target_vol = g_audio_info.system_vol;
	if (g_audio_info.bt_phone_on) {
		target_vol = g_audio_info.bt_phone_vol;
	} else if (g_audio_info.bt_voice_on) {
		target_vol = g_audio_info.bt_voice_vol;
	} else if (g_audio_info.reverse_on) {
		if (0==g_sys_info_store.vol_ctrl_when_reverse) {
			target_vol = 0;
		} else {
			tmp = (u16)(target_vol) * (u16)(g_sys_info_store.vol_ctrl_when_reverse) / (u16)100;
			target_vol = tmp&0xFF;
		}
	}

	if (AUDIO_SRC_SW_STATE_PREPARE == g_audio_info.src_sw_state) {
		target_vol = 0;
	}
	
	vol = _audio_change_volume(g_audio_info.cur_vol, target_vol);

	if (vol != g_audio_info.cur_vol) {
		g_audio_info.cur_vol = vol;
		audio_dev_update_volume(g_audio_info.cur_vol);
	} else {
		if (AUDIO_SRC_SW_STATE_PREPARE == g_audio_info.src_sw_state) {
			g_audio_info.src_sw_state = AUDIO_SRC_SW_STATE_READY;
		}
	}
}

static void audio_event_handler(void)
{
	EVENT *pEvt;
	pEvt=GetEvent(AUDIO_MODULE);
	switch(pEvt->ID) {
		case AUDIO_EVT_NAVI_EXTRA_GAIN:
			audio_set_navi_extra_gain(LSB(pEvt->prm));
			if (g_sys_info_store.audio_navi_mix_extra_gain != g_audio_info.navi_mix_extra_gain) {
				g_sys_info_store.audio_navi_mix_extra_gain = g_audio_info.navi_mix_extra_gain;
				ak_flash_save_info();
			}
			PostEvent(WINCE_MODULE, TX_TO_GUI_SYSTEM_NAVI_MIX_INFO, g_audio_info.navi_mix_extra_gain);
			break;
		case AUDIO_EVT_OUTPUT_TYPE:
			if (0x80 == LSB(pEvt->prm)) {
//				g_audio_info.overheat = FALSE;
			} else if (0x81 == LSB(pEvt->prm)) {
//				g_audio_info.overheat = TRUE;
//				if (g_audio_info.system_vol > DEFAULT_VOLUME) {
//					g_audio_info.system_vol = DEFAULT_VOLUME;
//				}
			} else {
				audio_set_output_type(LSB(pEvt->prm));
				if (g_sys_info_store.output_type != g_audio_info.output_type) {
					g_sys_info_store.output_type = g_audio_info.output_type;
					ak_flash_save_info();
				}
				PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_CUR_MAX_OUTPUT_VOL, NONE);
			}
			break;
		case AUDIO_EVT_ADJ_CH_VOL:
			audio_adjust_channel_volume();
			break;
		default:
			break;
	}
}

static void audio_temp_mute_handler(void)
{
	if (0!=g_audio_info.temp_mute_timer) {
		--g_audio_info.temp_mute_timer;
		if (0==g_audio_info.temp_mute_timer) {
			audio_set_mute(AUDIO_MUTE_TEMP, FALSE);
		}
	}
}

static void audio_ext_force_mute_handler(void)
{
	if ( (g_audio_info.mute & AUDIO_MUTE_EXT_FORCE) && (Bit_SET==GPIO_ReadInputDataBit(GPIO_TEL_MUTE_DET_GRP, GPIO_TEL_MUTE_DET_PIN)) ){
		++g_audio_info.ext_force_mute_timer;
		if (g_audio_info.ext_force_mute_timer >= T500MS_12) {
			g_audio_info.ext_force_mute_timer = 0;
			audio_set_mute(AUDIO_MUTE_EXT_FORCE, FALSE);
		}
	} else if ( (!(g_audio_info.mute & AUDIO_MUTE_EXT_FORCE)) && (Bit_RESET==GPIO_ReadInputDataBit(GPIO_TEL_MUTE_DET_GRP, GPIO_TEL_MUTE_DET_PIN)) ){
		++g_audio_info.ext_force_mute_timer;
		if (g_audio_info.ext_force_mute_timer >= T500MS_12) {
			g_audio_info.ext_force_mute_timer = 0;
			audio_set_mute(AUDIO_MUTE_EXT_FORCE, TRUE);
		}
	} else {
		g_audio_info.ext_force_mute_timer = 0;
	}
}

static void audio_sa_handler(void)
{
	if (FALSE == g_audio_info.sa_en) {
		return;
	}
	if (g_audio_info.sa_timer++ < T200MS_12) {
		return;
	}
	g_audio_info.sa_timer = 0;

	audio_dev_update_spectrum_data();

	PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_SA_INFO, NONE);
}

void audio_init(void)
{
	int cnt;

	g_audio_info.state = AUDIO_STATE_PWR_DOWN;
	g_audio_info.cur_source = AUDIO_SRC_NONE;
#ifdef AUDIO_INIT_SRC_FOR_TEST
	g_audio_info.sys_source = AUDIO_INIT_SRC_FOR_TEST;	// for test
#else
	g_audio_info.sys_source = AUDIO_SRC_NONE;
#endif
	g_audio_info.android_snd_on = FALSE;
	g_audio_info.cur_vol = 0;
	g_audio_info.system_vol = DEFAULT_VOLUME;
	g_audio_info.navi_mix_vol = DEFAULT_VOLUME;
	g_audio_info.extra_input_gain_factory[AUDIO_SRC_NONE] = 0;
	g_audio_info.extra_input_gain_factory[AUDIO_SRC_RADIO] = A_SRC_RADIO_EXTRA_GAIN_ATTEN;
	g_audio_info.extra_input_gain_factory[AUDIO_SRC_HOST] = A_SRC_HOST_EXTRA_GAIN_ATTEN;
	g_audio_info.extra_input_gain_factory[AUDIO_SRC_BT_MODULE] = A_SRC_BT_MODULE_EXTRA_GAIN_ATTEN;
	g_audio_info.extra_input_gain_factory[AUDIO_SRC_DVD] = A_SRC_DVD_EXTRA_GAIN_ATTEN;
	g_audio_info.extra_input_gain_factory[AUDIO_SRC_AUXIN] = A_SRC_AUXIN_EXTRA_GAIN_ATTEN;
	g_audio_info.extra_input_gain_factory[AUDIO_SRC_TV] = A_SRC_TV_EXTRA_GAIN_ATTEN;
	g_audio_info.extra_input_gain_user[AUDIO_SRC_NONE] = 0;
	g_audio_info.extra_input_gain_user[AUDIO_SRC_RADIO] = 0;
	g_audio_info.extra_input_gain_user[AUDIO_SRC_HOST] = 0;
	g_audio_info.extra_input_gain_user[AUDIO_SRC_BT_MODULE] = 0;
	g_audio_info.extra_input_gain_user[AUDIO_SRC_DVD] = 0;
	g_audio_info.extra_input_gain_user[AUDIO_SRC_AUXIN] = 0;
	g_audio_info.extra_input_gain_user[AUDIO_SRC_TV] = 0;
	for (cnt=0; cnt<AUDIO_SRC_NUMS; cnt++) {
		g_audio_info.extra_input_gain[cnt] = g_audio_info.extra_input_gain_factory[cnt] +
										g_audio_info.extra_input_gain_user[cnt];
	}
	g_audio_info.mute = 0;
	g_audio_info.bt_phone_on = FALSE;
	g_audio_info.bt_phone_vol = DEFAULT_VOLUME;
	g_audio_info.carplay_phone_on = FALSE;
	g_audio_info.navi_break_on = FALSE;
	g_audio_info.navi_break_on_cache = FALSE;
	g_audio_info.app_3rd_break_on = FALSE;
	g_audio_info.rds_ta_break_on = FALSE;
	g_audio_info.reverse_on = FALSE;
	g_audio_info.fader = DEFAULT_FIELD_LEVEL;
	g_audio_info.balance = DEFAULT_FIELD_LEVEL;
	g_audio_info.loudness = DEFAULT_LOUDNESS;
	g_audio_info.subwoofer = DEFAULT_SUBWOOFER_LVL;
	g_audio_info.pwr_timer = 0;
	g_audio_info.bt_phone_timer = 0;
	g_audio_info.temp_mute_timer = 0;
	g_audio_info.src_sw_state = AUDIO_SRC_SW_STATE_NONE;
	g_audio_info.bt_voice_on = FALSE;
	g_audio_info.bt_music_on = FALSE;
	g_audio_info.bt_voice_vol = DEFAULT_VOLUME;
	g_audio_info.bt_voice_timer = 0;
//	g_audio_info.overheat = FALSE;
	g_audio_info.ext_force_mute_timer = 0;

	g_audio_info.disabled_soundfield = FALSE;

#if ASP_MODEL==ASP_BU32107
	g_audio_info.dsp_set_user_id = AUDIO_DSP_USER_ID_0;
	audio_eq_init();	
	audio_init_dsp_v2_set(AUDIO_DSP_RESET_ID_ALL, TRUE);
#else
	audio_eq_init();	
#endif
}

void audio_main(void)
{
	if (AUDIO_STATE_PWR_OFF_ING == g_audio_info.state) {
		audio_set_mute(AUDIO_MUTE_DRIVER, TRUE);
		audio_dev_deinit();
		g_audio_info.pwr_timer = 0;
		g_audio_info.bt_phone_timer = 0;
		g_audio_info.state = AUDIO_STATE_PWR_DOWN;
		g_audio_info.bt_phone_on = FALSE;
		g_audio_info.carplay_phone_on = FALSE;
		g_audio_info.navi_break_on = FALSE;
		g_audio_info.navi_break_on_cache = FALSE;
		g_audio_info.reverse_on = FALSE;
		g_audio_info.bt_voice_on = FALSE;
		g_audio_info.bt_music_on = FALSE;
		g_audio_info.bt_voice_timer = 0;
		return;
	}
	if (AUDIO_STATE_PWR_ON_ING == g_audio_info.state) {
		audio_set_mute(AUDIO_MUTE_DRIVER, TRUE);
		g_audio_info.state = AUDIO_STATE_INIT_ING;
		g_audio_info.pwr_timer = 0;
		return;
	}
	if (AUDIO_STATE_INIT_ING == g_audio_info.state) {
		if (!IS_PANEL_PWR_ON) {
			return;
		}
		if (g_audio_info.pwr_timer < T200MS_12) {
			++g_audio_info.pwr_timer;
			return;
		}
		if (TRUE == audio_dev_init()) {
//			g_audio_info.overheat = FALSE;
			g_audio_info.cur_source = g_audio_info.sys_source;
			g_audio_info.bt_phone_on = FALSE;
			g_audio_info.carplay_phone_on = FALSE;
			g_audio_info.bt_voice_on = FALSE;
			g_audio_info.bt_music_on = FALSE;
			g_audio_info.navi_break_on = FALSE;
			g_audio_info.navi_break_on_cache = FALSE;
			g_audio_info.sa_en = FALSE;
			g_audio_info.sa_timer = 0;
			audio_dev_update_source(g_audio_info.cur_source);
			audio_dev_update_volume(g_audio_info.cur_vol);
			audio_dev_update_fader_balance(g_audio_info.fader, g_audio_info.balance);
			audio_dev_update_loudness(g_audio_info.loudness);
			audio_dev_update_subwoofer(g_audio_info.subwoofer);
			audio_dev_update_eq();
			audio_dev_update_dsp_settings_misc();
			audio_dev_update_dsp_settings_delay();
			audio_dev_update_dsp_settings_gain();
			g_audio_info.pwr_timer = 0;
			g_audio_info.bt_phone_timer = 0;
			g_audio_info.bt_voice_timer = 0;
			g_audio_info.state = AUDIO_STATE_IDLE;
			g_audio_info.temp_mute_timer = 0;
			g_audio_info.src_sw_state = AUDIO_SRC_SW_STATE_NONE;
			g_audio_info.ext_force_mute_timer = 0;
//			audio_set_mute(AUDIO_MUTE_DRIVER, FALSE);
			audio_set_mute(AUDIO_MUTE_TEMP, FALSE);
		}
		return;
	}

	if (AUDIO_STATE_IDLE == g_audio_info.state) {
		audio_event_handler();
		audio_reverse_handler();
		audio_source_handler();
		audio_volume_handler();
		audio_temp_mute_handler();
		audio_ext_force_mute_handler();
		audio_sa_handler();
	}
}

void audio_set_source(SOURCE av_src)
{
	AUDIO_SOURCE src;
	src = _audio_select_source(av_src);
	g_audio_info.sys_source = src;
}

void audio_set_volume(u8 vol)
{
	if (!IS_VALID_VOLUME(vol)) {
		return;
	}
	if (0!=vol) {
		audio_set_mute(AUDIO_MUTE_USER, FALSE);
	} else {
		audio_set_mute(AUDIO_MUTE_USER, TRUE);
	}
	if (g_audio_info.bt_phone_on) {
		g_audio_info.bt_phone_vol = vol;
	} else if (g_audio_info.bt_voice_on) {
		g_audio_info.bt_voice_vol = vol;
	} else {
		g_audio_info.system_vol = vol;
		g_audio_info.navi_mix_vol = vol;
		audio_dev_update_navi_mix_vol(g_audio_info.navi_mix_vol);
	}
}

u8 audio_get_volume(void)
{
	if ( g_audio_info.mute & AUDIO_MUTE_USER ) {
		return 0;
	} else if (g_audio_info.bt_phone_on) {
		return g_audio_info.bt_phone_vol;
	} else if (g_audio_info.bt_voice_on) {
		return g_audio_info.bt_voice_vol;
	} else {
		return g_audio_info.system_vol;
	}
}

void audio_volume_up(void)
{
	u8 vol;

	if (g_audio_info.bt_phone_on) {
		vol = g_audio_info.bt_phone_vol + 1;
		if (IS_VALID_VOLUME(vol)) {
			g_audio_info.bt_phone_vol = vol;
		}
	} else if (g_audio_info.bt_voice_on) {
		vol = g_audio_info.bt_voice_vol + 1;
		if (IS_VALID_VOLUME(vol)) {
			g_audio_info.bt_voice_vol = vol;
		}
	} else {
		vol = g_audio_info.system_vol + 1;
		if (IS_VALID_VOLUME(vol)) {
			g_audio_info.system_vol = vol;
			g_audio_info.navi_mix_vol = vol;
			audio_dev_update_navi_mix_vol(g_audio_info.navi_mix_vol);
		}
	}
	audio_set_mute(AUDIO_MUTE_USER, FALSE);
}

void audio_volume_down(void)
{
	u8 vol;

	if (g_audio_info.bt_phone_on) {
		vol = g_audio_info.bt_phone_vol - 1;
		if (IS_VALID_VOLUME(vol)) {
			g_audio_info.bt_phone_vol = vol;
		}
	} else if (g_audio_info.bt_voice_on) {
		vol = g_audio_info.bt_voice_vol - 1;
		if (IS_VALID_VOLUME(vol)) {
			g_audio_info.bt_voice_vol = vol;
		}
	} else {
		vol = g_audio_info.system_vol - 1;
		if (IS_VALID_VOLUME(vol)) {
			g_audio_info.system_vol = vol;
			g_audio_info.navi_mix_vol = vol;
			audio_dev_update_navi_mix_vol(g_audio_info.navi_mix_vol);
		}
	}

	if ( (!g_audio_info.bt_phone_on) && (0==g_audio_info.system_vol) ) {
		audio_set_mute(AUDIO_MUTE_USER, TRUE);
	} else if ( (!g_audio_info.bt_voice_on) && (0==g_audio_info.system_vol) ) {
		audio_set_mute(AUDIO_MUTE_USER, TRUE);
	} else {
		audio_set_mute(AUDIO_MUTE_USER, FALSE);
	}
}

void audio_set_mute(AUDIO_MUTE_FLAG flag, bool mute)
{
	if (FALSE == mute) {
		g_audio_info.mute = (g_audio_info.mute & (~flag));
	} else {
		g_audio_info.mute = (g_audio_info.mute | flag);
	}

	if (g_audio_info.mute & AUDIO_MUTE_EXT_FORCE) {
		AUDIO_HW_MUTE;
		return;
	}

	if (AUDIO_MUTE_RADIO==g_audio_info.mute) {
		// we will set audio channel to none, if just radio mute
		AUDIO_HW_UNMUTE;
		return;
	}

	if (0 == g_audio_info.mute) {
		AUDIO_HW_UNMUTE;
	} else {
		AUDIO_HW_MUTE;
	}
}

void audio_set_mute_temporary(u16 time_ms)
{
	// our audio_main is in the 12ms task
	g_audio_info.temp_mute_timer = time_ms/12;
	if (0!=g_audio_info.temp_mute_timer) {
		audio_set_mute(AUDIO_MUTE_TEMP, TRUE);
	} else {
		audio_set_mute(AUDIO_MUTE_TEMP, FALSE);
	}
}

void audio_set_bt_phone(bool on)
{
	if (g_audio_info.bt_phone_on != on) {
		g_audio_info.bt_phone_on = on;
		audio_dev_update_fader_balance(g_audio_info.fader, g_audio_info.balance);
		g_audio_info.bt_phone_timer = T300MS_12;

		// make sure volume will update according BT phone on/off
		audio_dev_update_volume(g_audio_info.cur_vol);

		// make sure adjust navi mix vol when in/out bt phone
		audio_dev_update_navi_mix_vol(g_audio_info.navi_mix_vol);

		// force unmute when enter/exit BT phone
		if (g_audio_info.mute & AUDIO_MUTE_USER) {
			audio_set_mute(AUDIO_MUTE_USER, FALSE);
			PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_VOLUME_INFO, NONE);
			PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_FLAG_INFO, NONE);
		}

		// let audio has chance to force unmute, when bt phone on
		audio_set_mute((AUDIO_MUTE_FLAG)(g_audio_info.mute), TRUE);

		audio_dev_update_source(g_audio_info.cur_source);
	}
}

void audio_set_bt_voice(bool on)
{
	if (g_audio_info.bt_voice_on != on) {
		g_audio_info.bt_voice_on = on;
		audio_dev_update_fader_balance(g_audio_info.fader, g_audio_info.balance);
		g_audio_info.bt_voice_timer = T300MS_12;

		// make sure adjust navi mix vol when in/out bt voice
		audio_dev_update_navi_mix_vol(g_audio_info.navi_mix_vol);

		// let audio has chance to force unmute, when bt voice on
		audio_set_mute((AUDIO_MUTE_FLAG)(g_audio_info.mute), TRUE);

		audio_dev_update_source(g_audio_info.cur_source);
	}
}

void audio_set_bt_music(bool on)
{
	if (g_audio_info.bt_music_on != on) {
		g_audio_info.bt_music_on = on;
		audio_dev_update_source(g_audio_info.cur_source);
	}
}

void audio_set_carplay_phone(bool on)
{
	g_audio_info.carplay_phone_on = on;
}

void audio_set_navi_break(bool on)
{
	g_audio_info.navi_break_on_cache = on;
	if (on && (AUDIO_SRC_HOST == g_audio_info.cur_source)) {
		// do not make mix if we are in HOST channel
		return;
	}
	_audio_do_set_navi_break(on);
}

void audio_set_rds_ta_break(bool on)
{
	g_audio_info.rds_ta_break_on = on;
}

void audio_set_app_3rd_break(bool on)
{
	g_audio_info.app_3rd_break_on = on;
}

void audio_set_android_sound_on(bool on)
{
	g_audio_info.android_snd_on = on;
}

void audio_set_eq_mode(EQ_MODE mode)
{
	bool do_eq_update=FALSE;
	if (!IS_VALID_EQ_MODE(mode)) {
		return;
	}
#if ASP_MODEL==ASP_BU32107
	if (mode != g_audio_info.eq_mode) {
		do_eq_update = TRUE;
	}
	if (do_eq_update) {
		audio_eq_update_mode(mode);
		audio_dev_update_eq();
		PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_EQUALIZER_INFO, NONE);
	}
#else
	if (mode != g_audio_info.eq_mode) {
		audio_eq_update_mode(mode);
		audio_dev_update_eq();
		PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_EQUALIZER_INFO, NONE);
	}
#endif
}

void audio_set_eq_custom_level(EQ_FREQ freq, u8 level)
{
	if (!IS_VALID_EQ_FREQ(freq)) {
		return;
	}

	audio_eq_update_level(freq, level);
	audio_dev_update_eq();
	PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_EQUALIZER_INFO, NONE);
}

void audio_set_fader(u8 level)
{
	if (level>MAX_FIELD_LEVEL) {
		return;
	}
	if (level != g_audio_info.fader) {
		g_audio_info.fader = level;
#if ASP_MODEL==ASP_BU32107
		g_audio_info.dsp_delay_c_pos = AUDIO_DSP_C_POS_USER;
		PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_V2_SETTINGS_2, 1);
#endif
		audio_dev_update_fader_balance(g_audio_info.fader, g_audio_info.balance);
		PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_EQUALIZER_INFO, NONE);
	}
}

void audio_set_balance(u8 level)
{
	if (level>MAX_FIELD_LEVEL) {
		return;
	}
	if (level != g_audio_info.balance) {
		g_audio_info.balance = level;
#if ASP_MODEL==ASP_BU32107
		g_audio_info.dsp_delay_c_pos = AUDIO_DSP_C_POS_USER;
		PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_DSP_V2_SETTINGS_2, 1);
#endif
		audio_dev_update_fader_balance(g_audio_info.fader, g_audio_info.balance);
		PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_EQUALIZER_INFO, NONE);
	}
}

void audio_set_loudness(u8 level)
{
	if (level>MAX_LOUDNESS) {
		return;
	}
	if (level != g_audio_info.loudness) {
//		g_audio_info.loudness = level;
		audio_dev_update_loudness(g_audio_info.loudness);
		PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_EQUALIZER_INFO, NONE);
	}
}

void audio_set_subwoofer(u8 level)
{
	if (level > MAX_SUBWOOFER_LVL) {
		return;
	}
	if (level != g_audio_info.subwoofer) {
		g_audio_info.subwoofer = level;
		audio_dev_update_subwoofer(g_audio_info.subwoofer);
		PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_EQUALIZER_INFO, NONE);
	}
}


void audio_set_navi_extra_gain(u8 gain)
{
	if (0==gain) {
		// for legacy MCU comm protocal support
		gain = NAVI_BREAK_DIRECT;
	}
	g_audio_info.navi_mix_extra_gain = gain;
	audio_dev_update_navi_mix_vol(g_audio_info.navi_mix_vol);
	audio_dev_update_fader_balance(g_audio_info.fader, g_audio_info.balance);
}

void audio_set_pwr_ctrl(bool on)
{
	if (FALSE == on) {
		g_audio_info.state = AUDIO_STATE_PWR_OFF_ING;
	} else {
		g_audio_info.state = AUDIO_STATE_PWR_ON_ING;
	}
}

void audio_set_vol_ctrl_when_reverse(u8 val)
{
	if (val>100) {
		return;
	}
	g_sys_info_store.vol_ctrl_when_reverse = val;
	audio_dev_update_navi_mix_vol(g_audio_info.navi_mix_vol);
}

void audio_set_output_type(u8 type)
{
	if (type >= AUDIO_OUTPUT_TYPE_NUMS) {
		return;
	}
	g_audio_info.output_type = type;
}

void audio_adjust_channel_volume(void)
{
	u8 cnt;
	for (cnt=0; cnt<AUDIO_SRC_NUMS; cnt++) {
		g_audio_info.extra_input_gain[cnt] = g_audio_info.extra_input_gain_factory[cnt] +
										g_audio_info.extra_input_gain_user[cnt];
	}

	// force the audio device to update our new extra gain
	audio_dev_update_source(g_audio_info.cur_source);
	audio_dev_update_volume(g_audio_info.cur_vol);
}

void audio_set_spectrum_analyzer(bool on)
{
	g_audio_info.sa_en = on;
}

void audio_soundfield_ctrl(u8 disabled)
{
	if (0==disabled) {
		g_audio_info.disabled_soundfield = FALSE;
	} else {
		g_audio_info.disabled_soundfield = TRUE;
	}
	audio_dev_update_fader_balance(g_audio_info.fader, g_audio_info.balance);
}

#if ASP_MODEL==ASP_BU32107
void audio_init_dsp_v2_set(AUDIO_DSP_RESET_ID reset, bool clear_user)
{
	u8 id;
	u8 cnt;

	if ((AUDIO_DSP_RESET_ID_DELAY==reset)||(AUDIO_DSP_RESET_ID_ALL==reset)) {
		g_audio_info.surround_mode = AUDIO_SR_MODE_FLAT;
		g_audio_info.dsp_delay_mode = AUDIO_DELAY_MANUAL;
		g_audio_info.dsp_delay_spk[AUDIO_SPK_FL] = 0;
		g_audio_info.dsp_delay_spk[AUDIO_SPK_FR] = 0;
		g_audio_info.dsp_delay_spk[AUDIO_SPK_RL] = 0;
		g_audio_info.dsp_delay_spk[AUDIO_SPK_RR] = 0;
		g_audio_info.dsp_delay_spk_width = 0;
		g_audio_info.dsp_delay_spk_height = 0;
		g_audio_info.fader = DEFAULT_FIELD_LEVEL;
		g_audio_info.balance = DEFAULT_FIELD_LEVEL;
		g_audio_info.dsp_delay_c_pos = AUDIO_DSP_C_POS_ALL;
	}
	if ((AUDIO_DSP_RESET_ID_GAIN==reset)||(AUDIO_DSP_RESET_ID_ALL==reset)) {
		g_audio_info.dsp_spk_sw.byte = 0;
		g_audio_info.dsp_spk_atten[AUDIO_SPK_FL] = DSP_SPK_GAIN_MAX;
		g_audio_info.dsp_spk_atten[AUDIO_SPK_FR] = DSP_SPK_GAIN_MAX;
		g_audio_info.dsp_spk_atten[AUDIO_SPK_RL] = DSP_SPK_GAIN_MAX;
		g_audio_info.dsp_spk_atten[AUDIO_SPK_RR] = DSP_SPK_GAIN_MAX;
		g_audio_info.dsp_all_atten = DSP_SPK_GAIN_MAX;
		g_audio_info.dsp_sub_gain = 0;
	}
	if ((AUDIO_DSP_RESET_ID_OTHER==reset)||(AUDIO_DSP_RESET_ID_ALL==reset)) {
		g_audio_info.dsp_loud_on = 1;
		g_audio_info.dsp_loud_lpf = 0;
		g_audio_info.dsp_loud_hpf = 0;
		g_audio_info.dsp_phat_on = 1;
		g_audio_info.dsp_phat_gain = 0;
		g_audio_info.dsp_bass_on = 1;
		g_audio_info.dsp_bass_fc = 0;
		g_audio_info.dsp_bass_gain = 0;
	}

	if ((AUDIO_DSP_RESET_ID_FILTER==reset)||(AUDIO_DSP_RESET_ID_ALL==reset)) {
		g_audio_info.dsp_hpf_fc = 0;
		g_audio_info.dsp_hpf_fc_rear = 0;
		g_audio_info.dsp_lpf_fc = 0;
		g_audio_info.dsp_lpf_fc_rear = 0;
		g_audio_info.dsp_eq_pos = AUDIO_EQ_FRONT;
	}

	if (clear_user) {
		g_audio_info.dsp_set_user_id = AUDIO_DSP_USER_ID_0;
		for (id=AUDIO_DSP_USER_ID_0; id<AUDIO_DSP_USER_ID_NUMS; id++) {
			g_dsp_set_v2[id].dsp_hpf_fc = g_audio_info.dsp_hpf_fc;
			g_dsp_set_v2[id].dsp_hpf_fc_rear = g_audio_info.dsp_hpf_fc_rear;
			g_dsp_set_v2[id].dsp_lpf_fc = g_audio_info.dsp_lpf_fc;
			g_dsp_set_v2[id].dsp_lpf_fc_rear = g_audio_info.dsp_lpf_fc_rear;
			g_dsp_set_v2[id].dsp_loud_on = g_audio_info.dsp_loud_on;
			g_dsp_set_v2[id].dsp_loud_lpf = g_audio_info.dsp_loud_lpf;
			g_dsp_set_v2[id].dsp_loud_hpf = g_audio_info.dsp_loud_hpf;
			g_dsp_set_v2[id].dsp_phat_on = g_audio_info.dsp_phat_on;
			g_dsp_set_v2[id].dsp_phat_gain = g_audio_info.dsp_phat_gain;
			g_dsp_set_v2[id].dsp_bass_on = g_audio_info.dsp_bass_on;
			g_dsp_set_v2[id].dsp_bass_fc = g_audio_info.dsp_bass_fc;
			g_dsp_set_v2[id].dsp_bass_gain = g_audio_info.dsp_bass_gain;
			g_dsp_set_v2[id].dsp_delay_mode = g_audio_info.dsp_delay_mode;
			g_dsp_set_v2[id].dsp_delay_spk[AUDIO_SPK_FL] = g_audio_info.dsp_delay_spk[AUDIO_SPK_FL];
			g_dsp_set_v2[id].dsp_delay_spk[AUDIO_SPK_FR] = g_audio_info.dsp_delay_spk[AUDIO_SPK_FR];
			g_dsp_set_v2[id].dsp_delay_spk[AUDIO_SPK_RL] = g_audio_info.dsp_delay_spk[AUDIO_SPK_RL];
			g_dsp_set_v2[id].dsp_delay_spk[AUDIO_SPK_RR] = g_audio_info.dsp_delay_spk[AUDIO_SPK_RR];
			g_dsp_set_v2[id].dsp_delay_spk_width = g_audio_info.dsp_delay_spk_width;
			g_dsp_set_v2[id].dsp_delay_spk_height = g_audio_info.dsp_delay_spk_height;
			g_dsp_set_v2[id].fader = g_audio_info.fader;
			g_dsp_set_v2[id].balance = g_audio_info.balance;
			g_dsp_set_v2[id].dsp_delay_c_pos = g_audio_info.dsp_delay_c_pos;
			g_dsp_set_v2[id].dsp_spk_sw.byte = g_audio_info.dsp_spk_sw.byte;
			g_dsp_set_v2[id].dsp_spk_atten[AUDIO_SPK_FL] = g_audio_info.dsp_spk_atten[AUDIO_SPK_FL];
			g_dsp_set_v2[id].dsp_spk_atten[AUDIO_SPK_FR] = g_audio_info.dsp_spk_atten[AUDIO_SPK_FR];
			g_dsp_set_v2[id].dsp_spk_atten[AUDIO_SPK_RL] = g_audio_info.dsp_spk_atten[AUDIO_SPK_RL];
			g_dsp_set_v2[id].dsp_spk_atten[AUDIO_SPK_RR] = g_audio_info.dsp_spk_atten[AUDIO_SPK_RR];
			g_dsp_set_v2[id].dsp_all_atten = g_audio_info.dsp_all_atten;
			g_dsp_set_v2[id].dsp_sub_gain = g_audio_info.dsp_sub_gain;
			for (cnt=0; cnt<EQ_FREQ_NUMS; cnt++) {
				g_dsp_set_v2[id].eq_custom_level[cnt] = g_audio_info.eq_custom_level[cnt];
			}
			g_dsp_set_v2[id].surround_mode = g_audio_info.surround_mode;
		}
	}

}

void audio_update_dsp_v2_set_eq(void)
{
	audio_eq_update_mode(g_audio_info.eq_mode);

	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_eq_pos = g_audio_info.dsp_eq_pos;

	audio_dev_update_eq();
	PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_EQUALIZER_INFO, NONE);
}
void audio_update_dsp_v2_set_1(void)
{
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_hpf_fc = g_audio_info.dsp_hpf_fc;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_hpf_fc_rear = g_audio_info.dsp_hpf_fc_rear;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_lpf_fc = g_audio_info.dsp_lpf_fc;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_lpf_fc_rear = g_audio_info.dsp_lpf_fc_rear;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_loud_on = g_audio_info.dsp_loud_on;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_loud_lpf = g_audio_info.dsp_loud_lpf;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_loud_hpf = g_audio_info.dsp_loud_hpf;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_phat_on = g_audio_info.dsp_phat_on;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_phat_gain = g_audio_info.dsp_phat_gain;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_bass_on = g_audio_info.dsp_bass_on;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_bass_fc = g_audio_info.dsp_bass_fc;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_bass_gain = g_audio_info.dsp_bass_gain;

	audio_dev_update_loudness(g_audio_info.loudness);
	audio_dev_update_dsp_settings_misc();
	audio_dev_update_eq();
}
void audio_update_dsp_v2_set_2(void)
{
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_mode = g_audio_info.dsp_delay_mode;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_spk[AUDIO_SPK_FL] = g_audio_info.dsp_delay_spk[AUDIO_SPK_FL];
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_spk[AUDIO_SPK_FR] = g_audio_info.dsp_delay_spk[AUDIO_SPK_FR];
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_spk[AUDIO_SPK_RL] = g_audio_info.dsp_delay_spk[AUDIO_SPK_RL];
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_spk[AUDIO_SPK_RR] = g_audio_info.dsp_delay_spk[AUDIO_SPK_RR];
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_spk_width = g_audio_info.dsp_delay_spk_width;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_spk_height = g_audio_info.dsp_delay_spk_height;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].fader = g_audio_info.fader;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].balance = g_audio_info.balance;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_c_pos = g_audio_info.dsp_delay_c_pos;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].surround_mode = g_audio_info.surround_mode;

	audio_dev_update_dsp_settings_delay();
}
void audio_update_dsp_v2_set_3(void)
{
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_spk_sw.byte = g_audio_info.dsp_spk_sw.byte;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_spk_atten[AUDIO_SPK_FL] = g_audio_info.dsp_spk_atten[AUDIO_SPK_FL];
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_spk_atten[AUDIO_SPK_FR] = g_audio_info.dsp_spk_atten[AUDIO_SPK_FR];
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_spk_atten[AUDIO_SPK_RL] = g_audio_info.dsp_spk_atten[AUDIO_SPK_RL];
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_spk_atten[AUDIO_SPK_RR] = g_audio_info.dsp_spk_atten[AUDIO_SPK_RR];
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_all_atten = g_audio_info.dsp_all_atten;
	g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_sub_gain = g_audio_info.dsp_sub_gain;

	audio_dev_update_dsp_settings_gain();
}
void audio_reset_dsp_v2_set(void)
{
	audio_init_dsp_v2_set(AUDIO_DSP_RESET_ID_ALL, FALSE);	
	audio_update_dsp_v2_set_1();
	audio_update_dsp_v2_set_2();
	audio_update_dsp_v2_set_3();

	audio_eq_init();
	audio_update_dsp_v2_set_eq();
}
void audio_set_dsp_v2_user(u8 id)
{
	u8 cnt;

	g_audio_info.dsp_set_user_id = id;
	g_audio_info.dsp_hpf_fc = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_hpf_fc;
	g_audio_info.dsp_hpf_fc_rear = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_hpf_fc_rear;
	g_audio_info.dsp_lpf_fc = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_lpf_fc;
	g_audio_info.dsp_lpf_fc_rear = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_lpf_fc_rear;
	g_audio_info.dsp_eq_pos = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_eq_pos;
	g_audio_info.dsp_loud_on = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_loud_on;
	g_audio_info.dsp_loud_lpf = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_loud_lpf;
	g_audio_info.dsp_loud_hpf = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_loud_hpf;
	g_audio_info.dsp_phat_on = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_phat_on;
	g_audio_info.dsp_phat_gain = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_phat_gain;
	g_audio_info.dsp_bass_on = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_bass_on;
	g_audio_info.dsp_bass_fc = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_bass_fc;
	g_audio_info.dsp_bass_gain = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_bass_gain;
	g_audio_info.dsp_delay_mode = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_mode;
	g_audio_info.dsp_delay_spk[AUDIO_SPK_FL] = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_spk[AUDIO_SPK_FL];
	g_audio_info.dsp_delay_spk[AUDIO_SPK_FR] = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_spk[AUDIO_SPK_FR];
	g_audio_info.dsp_delay_spk[AUDIO_SPK_RL] = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_spk[AUDIO_SPK_RL];
	g_audio_info.dsp_delay_spk[AUDIO_SPK_RR] = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_spk[AUDIO_SPK_RR];
	g_audio_info.dsp_delay_spk_width = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_spk_width;
	g_audio_info.dsp_delay_spk_height = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_spk_height;
	g_audio_info.fader = g_dsp_set_v2[g_audio_info.dsp_set_user_id].fader;
	g_audio_info.balance = g_dsp_set_v2[g_audio_info.dsp_set_user_id].balance;
	g_audio_info.dsp_delay_c_pos = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_delay_c_pos;
	g_audio_info.dsp_spk_sw.byte = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_spk_sw.byte;
	g_audio_info.dsp_spk_atten[AUDIO_SPK_FL] = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_spk_atten[AUDIO_SPK_FL];
	g_audio_info.dsp_spk_atten[AUDIO_SPK_FR] = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_spk_atten[AUDIO_SPK_FR];
	g_audio_info.dsp_spk_atten[AUDIO_SPK_RL] = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_spk_atten[AUDIO_SPK_RL];
	g_audio_info.dsp_spk_atten[AUDIO_SPK_RR] = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_spk_atten[AUDIO_SPK_RR];
	g_audio_info.dsp_all_atten = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_all_atten;
	g_audio_info.dsp_sub_gain = g_dsp_set_v2[g_audio_info.dsp_set_user_id].dsp_sub_gain;

	g_audio_info.eq_mode = g_dsp_set_v2[g_audio_info.dsp_set_user_id].eq_mode;
	for (cnt=0; cnt<EQ_FREQ_NUMS; cnt++) {
		g_audio_info.eq_custom_level[cnt]= g_dsp_set_v2[g_audio_info.dsp_set_user_id].eq_custom_level[cnt];
	}
	g_audio_info.surround_mode = g_dsp_set_v2[g_audio_info.dsp_set_user_id].surround_mode;

	audio_update_dsp_v2_set_eq();
	audio_dev_update_loudness(g_audio_info.loudness);
	audio_dev_update_dsp_settings_misc();
	audio_dev_update_dsp_settings_delay();
	audio_dev_update_dsp_settings_gain();
}

void audio_set_dsp_surround(u8 mode)
{
	g_audio_info.surround_mode = (AUDIO_SR_MODE)mode;

	switch (mode) {
		case AUDIO_SR_MODE_FLAT:
			g_audio_info.dsp_delay_spk[AUDIO_SPK_FL] = 0;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_FR] = 0;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_RL] = 0;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_RR] = 0;
			break;
		case AUDIO_SR_MODE_RECITAL:
			g_audio_info.dsp_delay_spk[AUDIO_SPK_FL] = 300;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_FR] = 300;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_RL] = 50;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_RR] = 50;
			break;
		case AUDIO_SR_MODE_CONCERT:
			g_audio_info.dsp_delay_spk[AUDIO_SPK_FL] = 200;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_FR] = 200;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_RL] = 400;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_RR] = 400;
			break;
		case AUDIO_SR_MODE_BGM:
			g_audio_info.dsp_delay_spk[AUDIO_SPK_FL] = 100;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_FR] = 100;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_RL] = 350;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_RR] = 350;
			break;
		case AUDIO_SR_MODE_MOVIE:
			g_audio_info.dsp_delay_spk[AUDIO_SPK_FL] = 250;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_FR] = 250;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_RL] = 50;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_RR] = 50;
			break;
		case AUDIO_SR_MODE_DRAMA:
			g_audio_info.dsp_delay_spk[AUDIO_SPK_FL] = 350;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_FR] = 350;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_RL] = 80;
			g_audio_info.dsp_delay_spk[AUDIO_SPK_RR] = 80;
			break;
	}
}

#endif

