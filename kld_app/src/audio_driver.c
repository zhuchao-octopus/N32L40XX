
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
	g_audio_info.bt_voice_vol = DEFAULT_VOLUME;
	g_audio_info.bt_voice_timer = 0;
//	g_audio_info.overheat = FALSE;
	g_audio_info.ext_force_mute_timer = 0;
	g_audio_info.loud_on = FALSE;
	g_audio_info.hpf_on = FALSE;
	g_audio_info.hpf_freq = 0;
	g_audio_info.dsp_bass_on = FALSE;
	g_audio_info.dsp_bass_freq = 0;
	g_audio_info.dsp_bass_gain = 0;
	g_audio_info.subwoofer_on = FALSE;
	g_audio_info.phat_en = FALSE;
	g_audio_info.phat_gain = 0;
	g_audio_info.core_en = FALSE;
	g_audio_info.core_gain = 0;
	g_audio_info.space_en = FALSE;
	g_audio_info.space_gain = 0;
	g_audio_info.sf_mode = AUDIO_SF_MODE_USER;
	for (cnt=0; cnt<AUDIO_SPK_NUMS; cnt++) {
		g_audio_info.spk_on[cnt] = TRUE;
		g_audio_info.spk_gain[cnt] = DSP_SPK_GAIN_MAX;
		g_audio_info.spk_delay[cnt] = 0;
		g_audio_info.spk_user_gain[cnt] = DSP_SPK_GAIN_MAX;
		g_audio_info.spk_user_delay[cnt] = 0;
	}
	g_audio_info.soundfield_expert_mode = 0;
	g_audio_info.disabled_soundfield = FALSE;
	audio_eq_init();	
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
			audio_dev_update_dsp_settings_1();
			audio_dev_update_dsp_settings_2();
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
	if (!IS_VALID_EQ_MODE(mode)) {
		return;
	}
	if (mode != g_audio_info.eq_mode) {
		audio_eq_update_mode(mode);
		audio_dev_update_eq();
		PostEvent(WINCE_MODULE, TX_TO_GUI_AUDIO_EQUALIZER_INFO, NONE);
	}
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

void audio_save_eq_user_mode(u8 id)
{
#if ASP_MODEL==ASP_BU32107
	u8 cnt;
	for (cnt=0; cnt<EQ_FREQ_NUMS; cnt++) {
		switch (id) {
			case EQ_MODE_CUSTOM_1:
				g_audio_info.eq_custom_level_1[cnt] = g_audio_info.eq_custom_level[cnt];
				break;
			case EQ_MODE_CUSTOM_2:
				g_audio_info.eq_custom_level_2[cnt] = g_audio_info.eq_custom_level[cnt];
				break;
			case EQ_MODE_CUSTOM_3:
				g_audio_info.eq_custom_level_3[cnt] = g_audio_info.eq_custom_level[cnt];
				break;
			case EQ_MODE_CUSTOM_4:
				g_audio_info.eq_custom_level_4[cnt] = g_audio_info.eq_custom_level[cnt];
				break;
			case EQ_MODE_CUSTOM_5:
				g_audio_info.eq_custom_level_5[cnt] = g_audio_info.eq_custom_level[cnt];
				break;
			case EQ_MODE_CUSTOM_6:
				g_audio_info.eq_custom_level_6[cnt] = g_audio_info.eq_custom_level[cnt];
				break;
			default:
				return;
				break;
		}
	}		

	g_audio_info.eq_mode = id;
#endif
}

void audio_set_fader(u8 level)
{
	if (level>MAX_FIELD_LEVEL) {
		return;
	}
	if (level != g_audio_info.fader) {
		g_audio_info.fader = level;
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
		g_audio_info.loudness = level;
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

void audio_set_loud_on(bool on)
{
	g_audio_info.loud_on = on;
	audio_dev_update_loudness(g_audio_info.loudness);
}

void audio_set_hpf(bool on, u8 freq)
{
	g_audio_info.hpf_on = on;
	g_audio_info.hpf_freq = freq;
	audio_dev_update_dsp_settings_1();
}

void audio_set_dsp_bass(bool on, u8 freq, u8 gain)
{
	g_audio_info.dsp_bass_on = on;
	g_audio_info.dsp_bass_freq = freq;
	g_audio_info.dsp_bass_gain = gain;
	audio_dev_update_dsp_settings_1();
}

void audio_set_subwoofer_on(bool on)
{
	g_audio_info.subwoofer_on = on;
	audio_dev_update_subwoofer(g_audio_info.subwoofer);
}

void audio_set_spectrum_analyzer(bool on)
{
	g_audio_info.sa_en = on;
}

void audio_set_dsp_phat(bool on, u8 gain)
{
	g_audio_info.phat_en = on;
	g_audio_info.phat_gain = gain;
	audio_dev_update_dsp_settings_1();
}

void audio_set_dsp_core(bool on, u8 gain)
{
	g_audio_info.core_en = on;
	g_audio_info.core_gain = gain;
	audio_dev_update_dsp_settings_1();
}

void audio_set_dsp_space(bool on, u8 gain)
{
	g_audio_info.space_en = on;
	g_audio_info.space_gain = gain;
	audio_dev_update_dsp_settings_1();
}

void audio_set_dsp_soundfield(u8 mode)
{
	u8 cnt;

	g_audio_info.sf_mode = (AUDIO_SF_MODE)mode;
	switch (mode) {
		case AUDIO_SF_MODE_ALL:
		case AUDIO_SF_MODE_DRIVER:
		case AUDIO_SF_MODE_COPILOT:
		case AUDIO_SF_MODE_RL:
		case AUDIO_SF_MODE_RR:
			for (cnt=0; cnt<AUDIO_SPK_NUMS; cnt++) {
				g_audio_info.spk_on[cnt] = TRUE;
				g_audio_info.spk_gain[cnt] = DSP_SPK_GAIN_MAX;
				g_audio_info.spk_delay[cnt] = 0;
			}
			break;
		case AUDIO_SF_MODE_USER:
			for (cnt=0; cnt<AUDIO_SPK_NUMS; cnt++) {
				g_audio_info.spk_on[cnt] = g_audio_info.spk_user_on[cnt];
				g_audio_info.spk_gain[cnt] = g_audio_info.spk_user_gain[cnt];
				g_audio_info.spk_delay[cnt] = g_audio_info.spk_user_delay[cnt];
			}		
			break;
	}

	switch (mode) {
		case AUDIO_SF_MODE_DRIVER:
			g_audio_info.spk_gain[AUDIO_SPK_FL] = DSP_SPK_GAIN_MAX-3;
			g_audio_info.spk_delay[AUDIO_SPK_FL] = 8;
			break;
		case AUDIO_SF_MODE_COPILOT:
			g_audio_info.spk_gain[AUDIO_SPK_FR] = DSP_SPK_GAIN_MAX-3;
			g_audio_info.spk_delay[AUDIO_SPK_FR] = 8;
			break;
		case AUDIO_SF_MODE_RL:
			g_audio_info.spk_gain[AUDIO_SPK_RL] = DSP_SPK_GAIN_MAX-3;
			g_audio_info.spk_delay[AUDIO_SPK_RL] = 8;
			break;
		case AUDIO_SF_MODE_RR:
			g_audio_info.spk_gain[AUDIO_SPK_RR] = DSP_SPK_GAIN_MAX-3;
			g_audio_info.spk_delay[AUDIO_SPK_RR] = 8;
			break;
		default:
			break;
	}
	audio_dev_update_dsp_settings_2();
}

void audio_set_dsp_speaker(u8 spk, bool on, u8 gain, u8 delay)
{
	u8 cnt;

	// update the custom level from current visible level, because
	// maybe we are adjusting custom level from the preset levels.
	for (cnt=0; cnt<AUDIO_SPK_NUMS; cnt++) {
		g_audio_info.spk_user_gain[cnt] = g_audio_info.spk_gain[cnt];
		g_audio_info.spk_user_delay[cnt] = g_audio_info.spk_delay[cnt];
	}		

	g_audio_info.spk_user_on[spk] = on;
	g_audio_info.spk_on[spk] = on;
	if (gain > DSP_SPK_GAIN_MAX)
		gain = DSP_SPK_GAIN_MAX;
	g_audio_info.spk_user_gain[spk] = gain;
	g_audio_info.spk_user_delay[spk] = delay;
	g_audio_info.spk_gain[spk] = gain;
	g_audio_info.spk_delay[spk] = delay;

	g_audio_info.sf_mode = AUDIO_SF_MODE_USER;

	audio_dev_update_dsp_settings_2();
}

void audio_set_dsp_surround(u8 mode)
{
	g_audio_info.surround_mode = (AUDIO_SR_MODE)mode;
	audio_dev_update_dsp_settings_2();
}

void audio_set_dsp_sf_expert_mode(u8 mode)
{
	g_audio_info.soundfield_expert_mode = mode;
	audio_dev_update_dsp_settings_2();
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

