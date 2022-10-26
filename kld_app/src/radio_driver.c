
#include "public.h"

// macro with the radio state
#define IS_IN_IDLE() (RADIO_STATE_IDLE == g_radio_info.state)

#define IS_IN_FREQ_SEEK() (RADIO_STATE_FREQ_SEEKING==g_radio_info.state)
#define G_RFS_STATE() (g_radio_info.evt_info.freq_seek.state)
#define S_RFS_STATE(s) (g_radio_info.evt_info.freq_seek.state=s)
#define G_RFS_CMD() (g_radio_info.evt_info.freq_seek.cmd)
#define S_RFS_CMD(c) (g_radio_info.evt_info.freq_seek.cmd=c)
#define VAR_RFS_BAND (g_radio_info.evt_info.freq_seek.band)
#define VAR_RFS_S_FREQ (g_radio_info.evt_info.freq_seek.start_freq)
#define VAR_RFS_C_FREQ (g_radio_info.evt_info.freq_seek.cur_freq)
#define VAR_RFS_TIMER (g_radio_info.evt_info.freq_seek.timer)
#define VAR_RFS_STOPPED (g_radio_info.evt_info.freq_seek.stopped)
#define IS_DONE_FREQ_SEEK() (RFS_STATE_FINISHED==G_RFS_STATE())

#define IS_IN_PRESET_SEEK() (RADIO_STATE_PRESET_SEEKING==g_radio_info.state)
#define G_RPS_STATE() (g_radio_info.evt_info.preset_seek.state)
#define S_RPS_STATE(s) (g_radio_info.evt_info.preset_seek.state=s)
#define G_RPS_CMD() (g_radio_info.evt_info.preset_seek.cmd)
#define S_RPS_CMD(c) (g_radio_info.evt_info.preset_seek.cmd=c)
#define VAR_RPS_BAND (g_radio_info.evt_info.preset_seek.band)
#define VAR_RPS_S_ID (g_radio_info.evt_info.preset_seek.start_id)
#define VAR_RPS_C_ID (g_radio_info.evt_info.preset_seek.cur_id)
#define VAR_RPS_TIMER (g_radio_info.evt_info.preset_seek.timer)
#define IS_DONE_PRESET_SEEK() (RPS_STATE_FINISHED==G_RPS_STATE())

#define IS_IN_FREQ_SET() (RADIO_STATE_FREQ_SETTING==g_radio_info.state)
#define G_RFSET_STATE() (g_radio_info.evt_info.freq_set.state)
#define S_RFSET_STATE(s) (g_radio_info.evt_info.freq_set.state=s)
#define G_RFSET_CMD() (g_radio_info.evt_info.freq_set.cmd)
#define S_RFSET_CMD(c) (g_radio_info.evt_info.freq_set.cmd=c)
#define VAR_RFSET_BAND	(g_radio_info.evt_info.freq_set.band)
#define VAR_RFSET_FREQ	(g_radio_info.evt_info.freq_set.freq)
#define VAR_RFSET_TIMER	(g_radio_info.evt_info.freq_set.timer)
#define IS_DONE_FREQ_SET() (RFSET_STATE_FINISHED==G_RFSET_STATE())

#define IS_IN_PARAM_SET() (RADIO_STATE_PARAM_SETTING==g_radio_info.state)
#define G_RPMSET_STATE() (g_radio_info.evt_info.param_set.state)
#define S_RPMSET_STATE(s) (g_radio_info.evt_info.param_set.state=s)
#define G_RPMSET_CMD() (g_radio_info.evt_info.param_set.cmd)
#define S_RPMSET_CMD(c) (g_radio_info.evt_info.param_set.cmd=c)
#define G_RPMSET_VAL() (g_radio_info.evt_info.param_set.value)
#define S_RPMSET_VAL(v) (g_radio_info.evt_info.param_set.value=v)
#define IS_DONE_PARAM_SET() (RPMSET_STATE_FINISHED==G_RPMSET_STATE())


// macro with the radio band & freq
#define IS_IN_FM(b) (b<=PRESET_BAND_FM3)
#define IS_IN_AM(b) (b>=PRESET_BAND_AM1)
#define IS_RUSSIA_RADIO() (RADIO_AREA_RUSSIA == g_radio_area)
#define FM_MAX_FREQ		(g_radio_info.max_freq[RADIO_BAND_FM])
#define FM_MIN_FREQ		(g_radio_info.min_freq[RADIO_BAND_FM])
#define FM_STEP				(g_radio_info.step[RADIO_BAND_FM])
#define AM_MAX_FREQ		(g_radio_info.max_freq[RADIO_BAND_AM])
#define AM_MIN_FREQ		(g_radio_info.min_freq[RADIO_BAND_AM])
#define AM_STEP				(g_radio_info.step[RADIO_BAND_AM])
static u16 _correct_fm_freq_for_russia(u16 freq) 
{
	if (freq > 8000) {
		// 87.5~108.0 MHZ @ 100KHZ step
		if (freq > 10800) {
			freq = 6500;
		} else if (freq<8750) {
			freq = 7400;
		} else {
			if ((freq-8750)%10)
				freq=freq-((freq-8750)%10);	
		}
	} else {
		// 65.0~74.0 MHZ @ 30KHZ step
		if (freq > 7400) {
			freq = 8750;
		} else if (freq<6500) {
			freq = 10800;
		} else {
			if ((freq-6500)%3)
				freq=freq-((freq-6500)%3);	
		}
	}
	return freq;
}
static u8 _get_fm_step_for_russia(u16 freq)
{
	if (freq > 8000) {
		return 10;
	} else {
		return 3;
	}
}
#define CORRECT_FM_FREQ(n) do { \
			if (IS_RUSSIA_RADIO()) {	\
				n = _correct_fm_freq_for_russia(n);	\
			} else {	\
				if ((n) == 0)  (n) = FM_MIN_FREQ; \
				if ((n-FM_MIN_FREQ)%FM_STEP) n=n-((n-FM_MIN_FREQ)%FM_STEP);			\
				if ((n) > FM_MAX_FREQ)  (n) = FM_MIN_FREQ; \
				if ((n) < FM_MIN_FREQ)  (n) = FM_MAX_FREQ; \
			}	\
		}while(0);
#define CORRECT_AM_FREQ(n) do { \
			if ((n) == 0)  (n) = AM_MIN_FREQ; \
			if ((n-AM_MIN_FREQ)%AM_STEP) n=n-((n-AM_MIN_FREQ)%AM_STEP);			\
			if ((n) > AM_MAX_FREQ)  (n) = AM_MIN_FREQ; \
			if ((n) < AM_MIN_FREQ)  (n) = AM_MAX_FREQ; \
		}while(0);
#define CORRECT_FM_PRESET_NUM(n) do { \
			if ((n) >= FM_PRESET_NUM)  (n) = 0; \
			if ((n) < 0) (n) = (FM_PRESET_NUM-1); \
		}while(0);
#define CORRECT_AM_PRESET_NUM(n) do { \
			if ((n) >= AM_PRESET_NUM)  (n) = 0; \
			if ((n) < 0) (n) = (AM_PRESET_NUM-1); \
		}while(0);
#define G_CUR_BAND() (g_radio_info.cur_band)
#define G_ANOTHER_BAND() (g_radio_info.another_band)
#define G_CUR_FREQ() (g_radio_info.cur_freq[g_radio_info.cur_band])
#define G_CUR_FREQ_BY_BAND(b) (g_radio_info.cur_freq[b])
#define S_CUR_BAND(b) (g_radio_info.cur_band=b)
#define S_ANOTHER_BAND(b) (g_radio_info.another_band=b)
#define S_CUR_FREQ(f) (g_radio_info.cur_freq[g_radio_info.cur_band]=f)
#define S_CUR_FREQ_BY_BAND(b, f) (g_radio_info.cur_freq[b]=f)
#define G_PRESET_FM(b, i) (g_radio_info.preset_fm[b-PRESET_BAND_FM1][i])
#define G_PRESET_AM(b, i) (g_radio_info.preset_am[b-PRESET_BAND_AM1][i])
#define S_PRESET_FM(b, i, f) (g_radio_info.preset_fm[b-PRESET_BAND_FM1][i]=f)
#define S_PRESET_AM(b, i, f) (g_radio_info.preset_am[b-PRESET_BAND_AM1][i]=f)
#define G_CUR_IDX(b) (g_radio_info.cur_idx[b])
#define S_CUR_IDX(b, i) (g_radio_info.cur_idx[b]=i)

// other operation util
#define GET_SIGNAL_LEVEL() (g_radio_info.signal_level)
#define SET_SIGNAL_LEVEL(l) (g_radio_info.signal_level=l)
#define RESET_SEEK_FLAGS() do {		\
		g_radio_info.flag.field.F_RADIO_SEEK = 0;	\
		g_radio_info.flag.field.F_RADIO_AMS = 0;	\
		g_radio_info.flag.field.F_RADIO_SCAN= 0;	\
		g_radio_info.flag.field.F_RADIO_PS = 0;	\
	} while(0);
static void _set_cur_band_freq(PRESET_BAND band, u16 freq)
{
	S_CUR_BAND(band);
	S_CUR_FREQ(freq);
}
static void _set_cur_band_freq_by_preset_id(PRESET_BAND band, s8 id)
{
	S_CUR_BAND(band);
	if (IS_IN_FM(band)) {
		S_CUR_FREQ(G_PRESET_FM(band, id));
	} else if (IS_IN_AM(band)) {
		S_CUR_FREQ(G_PRESET_AM(band, id));
	}
}

#ifdef FIX_RADIO_BAND_SWITCH_POP
static void radio_config_fix_pop(u8 timer)
{
	audio_set_mute(AUDIO_MUTE_RADIO, 1);	// we will recovery at RADIO_STATE_IDLE
	S_RFSET_STATE(RFSET_STATE_PREPARING);//RFSET_STATE_BEGIN);
	VAR_RFSET_TIMER = timer;
}
#endif

static void radio_handle_freq_set(void)
{
	u16 step;

	switch(G_RFSET_STATE()) {
#ifdef FIX_RADIO_BAND_SWITCH_POP
		case RFSET_STATE_PREPARING:
			--VAR_RFSET_TIMER;
			if (0==VAR_RFSET_TIMER) {
				S_RFSET_STATE(RFSET_STATE_BEGIN);
			}
			break;
#endif
		case RFSET_STATE_BEGIN:
			rds_reset_var();
			PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PS_INFO, NONE);	// notify HOST clear the PS info
			if (IS_IN_AM(VAR_RFSET_BAND)) {
				g_radio_info.flag.field.F_RADIO_ST_MONO = 0;
			}
			if (IS_IN_FM(VAR_RFSET_BAND)) {
				if (IS_RUSSIA_RADIO()) {
					step = _get_fm_step_for_russia(VAR_RFSET_FREQ);
				} else {
					step = FM_STEP;
				}
			} else {
				step = AM_STEP;
			}
			if (RFSET_CMD_STEP_UP==G_RFSET_CMD()) {
				VAR_RFSET_FREQ += step;
			} else if (RFSET_CMD_STEP_DN==G_RFSET_CMD()) {
				VAR_RFSET_FREQ -= step;
			}
			if (IS_IN_FM(VAR_RFSET_BAND)) {
				CORRECT_FM_FREQ(VAR_RFSET_FREQ);
				radio_dev_set_freq_tune(RADIO_BAND_FM, VAR_RFSET_FREQ);
			} else {
				CORRECT_AM_FREQ(VAR_RFSET_FREQ);
				radio_dev_set_freq_tune(RADIO_BAND_AM, VAR_RFSET_FREQ);
			}
			VAR_RFSET_TIMER = T300MS_12;
			S_RFSET_STATE(RFSET_STATE_SET_ING);
			break;
		case RFSET_STATE_SET_ING:
			--VAR_RFSET_TIMER;
			if (0==VAR_RFSET_TIMER) {
				// force to done set freq op
				S_RFSET_STATE(RFSET_STATE_SET_DONE);
			}
			if (radio_dev_is_set_freq_done()) {
				S_RFSET_STATE(RFSET_STATE_SET_DONE);
			}
			break;
		case RFSET_STATE_SET_DONE:
			_set_cur_band_freq(VAR_RFSET_BAND, VAR_RFSET_FREQ);
			PostEvent(WINCE_MODULE,TX_TO_GUI_RADIO_CURRENT_INFO,NONE);
			if (RFSET_CMD_SWITCH_BAND == G_RFSET_CMD()) {
				PostEvent(WINCE_MODULE, TX_TO_GUI_RADIO_FREQENCY_LIST_INFO,NONE );
			}
			PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_FLAG_INFO,NONE );
			PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PTY_INFO, NONE);
			PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PS_INFO, NONE);
			S_RFSET_STATE(RFSET_STATE_FINISHED);
			break;
		case RFSET_STATE_FINISHED:
			// Note: acturally, we will never get here!
			break;
		default:
			break;
	}
}
static void radio_handle_param_set(void)
{
	switch(G_RPMSET_STATE()) {
		case RPMSET_STATE_BEGIN:
			switch (G_RPMSET_CMD()) {
				case RPMSET_CMD_LOC_DX:
					if (RDSM_LOC == (RADIO_DEV_SEEK_MODE)G_RPMSET_VAL()) {
						g_radio_info.flag.field.F_RADIO_LOC_DX = 1;
					} else {
						g_radio_info.flag.field.F_RADIO_LOC_DX = 0;
					}
					radio_dev_set_seek_mode((RADIO_DEV_SEEK_MODE)G_RPMSET_VAL());
					break;
				case RPMSET_CMD_ST:
					g_radio_info.flag.field.F_RADIO_ST_MONO = 0;
					if (0 == G_RPMSET_VAL()) {
						g_radio_info.flag.field.F_RADIO_ST_SW = 1;
						radio_dev_set_st_mode(FALSE);
					} else {
						g_radio_info.flag.field.F_RADIO_ST_SW = 0;
						radio_dev_set_st_mode(TRUE);
					}
					break;
				case RPMSET_CMD_RDS_TA_SWITCH:
					rds_ta_switch((bool)G_RPMSET_VAL());
					PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_FLAG_INFO,NONE );
					break;
				case RPMSET_CMD_RDS_AF_SWITCH:
					rds_af_switch((bool)G_RPMSET_VAL());
					PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_FLAG_INFO,NONE );
					break;
				case RPMSET_CMD_RDS_PTY_SEL:
					rds_pty_select((u8)G_RPMSET_VAL());
					PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PTY_INFO, NONE);
					break;
			}
			S_RPMSET_STATE(RPMSET_STATE_FINISHED);
			break;
		default:
			break;
	}
}
static void radio_handle_freq_seek(void)
{
	u8 step;
	u8 cnt;

	switch (G_RFS_STATE()) {
		case RFS_STATE_BEGIN:
			VAR_RFS_STOPPED = FALSE;
			rds_reset_var();
			RESET_SEEK_FLAGS();
			PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PS_INFO, NONE);	// notify HOST clear the PS info
			switch(G_RFS_CMD()) {
				case RFS_CMD_AUTO_SAVE:
					S_CUR_IDX(VAR_RFS_BAND, 0);
					g_radio_info.flag.field.F_RADIO_SEEK = TRUE;
					g_radio_info.flag.field.F_RADIO_AMS = TRUE;
					if (IS_IN_FM(VAR_RFS_BAND)) {
						for(cnt=0;cnt<FM_PRESET_NUM;cnt++) {
							S_PRESET_FM(VAR_RFS_BAND, cnt, 7600);
						}
					} else {
						for(cnt=0;cnt<AM_PRESET_NUM;cnt++) {
							S_PRESET_AM(VAR_RFS_BAND, cnt, 522);
						}
					}
					S_RFS_STATE(RFS_STATE_SET_FREQ);
					break;
				case RFS_CMD_AUTO_UP:
				case RFS_CMD_AUTO_DN:
				case RFS_CMD_AF_SEEK_LIST:
				case RFS_CMD_AF_SEEK_PI:
					g_radio_info.flag.field.F_RADIO_SEEK = TRUE;
					S_RFS_STATE(RFS_STATE_GOTO_NEXT_FREQ);
					break;
				case RFS_CMD_SCAN:
					g_radio_info.flag.field.F_RADIO_SEEK = TRUE;
					g_radio_info.flag.field.F_RADIO_SCAN = TRUE;
					S_RFS_STATE(RFS_STATE_SET_FREQ);
					break;
			}
			break;
		case RFS_STATE_SET_FREQ:
			rds_reset_var();
			audio_set_mute(AUDIO_MUTE_RADIO, TRUE);
			radio_dev_set_mute(TRUE);
			if (TRUE==VAR_RFS_STOPPED) {
				if (IS_IN_FM(VAR_RFS_BAND)) {
					radio_dev_set_freq_tune(RADIO_BAND_FM, VAR_RFS_C_FREQ);
				} else {
					radio_dev_set_freq_tune(RADIO_BAND_AM, VAR_RFS_C_FREQ);
				}
			} else {
				if (IS_IN_FM(VAR_RFS_BAND)) {
					radio_dev_set_freq(RADIO_BAND_FM, VAR_RFS_C_FREQ);
				} else {
					radio_dev_set_freq_tune(RADIO_BAND_AM, VAR_RFS_C_FREQ);
				}
			}
			VAR_RFS_TIMER = T100MS_12;
			S_RFS_STATE(RFS_STATE_SET_FREQ_ING);
			break;
		case RFS_STATE_SET_FREQ_ING:
			--VAR_RFS_TIMER;
			if (0==VAR_RFS_TIMER) {
				// force to done set freq op
				S_RFS_STATE(RFS_STATE_SET_FREQ_DONE);
			}
			if (radio_dev_is_set_freq_done()) {
				S_RFS_STATE(RFS_STATE_SET_FREQ_DONE);
			}
			break;
		case RFS_STATE_SET_FREQ_DONE:
			VAR_RFS_TIMER = T300MS_12;
			_set_cur_band_freq(VAR_RFS_BAND, VAR_RFS_C_FREQ);
			if (FrontSource == SOURCE_TUNER) {
				PostEvent(WINCE_MODULE,TX_TO_GUI_RADIO_CURRENT_INFO,NONE);
			}
			if (TRUE==VAR_RFS_STOPPED) {
				S_RFS_STATE(RFS_STATE_FINISHED);
			} else {
				S_RFS_STATE(RFS_STATE_WAIT_TUNE_STATUS);
			}
			break;
		case RFS_STATE_WAIT_TUNE_STATUS:
			--VAR_RFS_TIMER;
			if (0==VAR_RFS_TIMER) {
				// time out waiting for tune status
				S_RFS_STATE(RFS_STATE_GOTO_NEXT_FREQ);
			}
			if (radio_dev_is_tune_status_ready()) {
				S_RFS_STATE(RFS_STATE_GET_SIGNAL_LEVEL);
			}
			break;
		case RFS_STATE_GET_SIGNAL_LEVEL:
			if (radio_dev_is_tune_ok(TRUE)) {
				if (RFS_CMD_AF_SEEK_PI == G_RFS_CMD()) {
					VAR_RFS_TIMER = T1S_12;
					S_RFS_STATE(RFS_STATE_WAIT_RDS_PI_MATCH);
				} else {
					VAR_RFS_TIMER = T300MS_12;
					S_RFS_STATE(RFS_STATE_WAIT_RDS_TP_MATCH);
				}
			} else {
				S_RFS_STATE(RFS_STATE_GOTO_NEXT_FREQ);
			}
			break;
		case RFS_STATE_WAIT_RDS_PI_MATCH:
			--VAR_RFS_TIMER;
			if (0==VAR_RFS_TIMER) {
				// time out, no match PI found, goto next freq
				S_RFS_STATE(RFS_STATE_GOTO_NEXT_FREQ);
			} else if ( g_rds_info.pi != 0 ) {
				if (g_rds_info.pi == g_rds_af_info.pi) {
					VAR_RFS_TIMER = T300MS_12;
					S_RFS_STATE(RFS_STATE_WAIT_RDS_TP_MATCH);
				} else {
					S_RFS_STATE(RFS_STATE_GOTO_NEXT_FREQ);
				}
			}
			break;
		case RFS_STATE_WAIT_RDS_TP_MATCH:
			--VAR_RFS_TIMER;
			if (0==VAR_RFS_TIMER) {
				// time out, no TP indicator found, goto next freq
				S_RFS_STATE(RFS_STATE_GOTO_NEXT_FREQ);
			} else if ( (FALSE==g_rds_info.ta_switch_on) ||
						(g_rds_info.tp_on) ) {
				VAR_RFS_TIMER = T600MS_12;
				S_RFS_STATE(RFS_STATE_WAIT_RDS_PTY_MATCH);
			}
			break;
		case RFS_STATE_WAIT_RDS_PTY_MATCH:
			if ((0 == g_rds_info.pty_select) || 
				(g_rds_info.pty_select == g_rds_info.pty)) {
				//pty switch off, or matched PTY found
				switch(G_RFS_CMD()) {
					case RFS_CMD_AUTO_SAVE:
						if (IS_IN_FM(VAR_RFS_BAND)) {
							CORRECT_FM_PRESET_NUM(G_CUR_IDX(VAR_RFS_BAND));
							S_PRESET_FM(VAR_RFS_BAND, G_CUR_IDX(VAR_RFS_BAND), VAR_RFS_C_FREQ);
						} else if (IS_IN_AM(VAR_RFS_BAND)) {
							CORRECT_AM_PRESET_NUM(G_CUR_IDX(VAR_RFS_BAND));
							S_PRESET_AM(VAR_RFS_BAND, G_CUR_IDX(VAR_RFS_BAND), VAR_RFS_C_FREQ);
						}
						S_CUR_IDX(VAR_RFS_BAND, (s8)(G_CUR_IDX(VAR_RFS_BAND)+1));
						S_RFS_STATE(RFS_STATE_GOTO_NEXT_FREQ);
						break;
					case RFS_CMD_SCAN:
						// play this freq for 5S
						VAR_RFS_TIMER = T3S_12+T2S_12;
						radio_dev_set_mute(FALSE);
						audio_set_mute(AUDIO_MUTE_RADIO, FALSE);
						S_RFS_STATE(RFS_STATE_PLAY_ING);
						SET_SIGNAL_LEVEL((RADIO_SIGNAL_LEVEL)(GET_SIGNAL_LEVEL()|0x80));
						PostEvent(WINCE_MODULE,TX_TO_GUI_RADIO_CURRENT_INFO,NONE);
						break;
					case RFS_CMD_AUTO_UP:
					case RFS_CMD_AUTO_DN:
					case RFS_CMD_AF_SEEK_LIST:
					case RFS_CMD_AF_SEEK_PI:
						// found the proper freq, goto FINISHED directly
#if TUNER_MODEL==TUNER_ST_TDA7708
						if (IS_IN_FM(VAR_RFS_BAND)) {
							radio_dev_set_freq_tune(RADIO_BAND_FM, VAR_RFS_C_FREQ);
						}
#endif
						S_RFS_STATE(RFS_STATE_FINISHED);
						break;
				}				
			} else if (0==VAR_RFS_TIMER) {
				// time out, no matched PTY found, goto next freq
				S_RFS_STATE(RFS_STATE_GOTO_NEXT_FREQ);
			} else {
				--VAR_RFS_TIMER;
				if (g_rds_info.pty!=0) {	// we already got an unmatch PTY
					S_RFS_STATE(RFS_STATE_GOTO_NEXT_FREQ);
				}
			}
			break;
		case RFS_STATE_PLAY_ING:
			--VAR_RFS_TIMER;
			if (0==VAR_RFS_TIMER) {
				// finish playing
				S_RFS_STATE(RFS_STATE_GOTO_NEXT_FREQ);
				SET_SIGNAL_LEVEL((RADIO_SIGNAL_LEVEL)(GET_SIGNAL_LEVEL()&~0x80));
				PostEvent(WINCE_MODULE,TX_TO_GUI_RADIO_CURRENT_INFO,NONE);
			}
			break;
		case RFS_STATE_GOTO_NEXT_FREQ:
			if (IS_IN_FM(VAR_RFS_BAND)) {
				if (IS_RUSSIA_RADIO()) {
					step = _get_fm_step_for_russia(VAR_RFS_C_FREQ);
				} else {
					step = FM_STEP;
				}
			} else if (IS_IN_AM(VAR_RFS_BAND)) {
				step = AM_STEP;
			}
			switch(G_RFS_CMD()) {
				case RFS_CMD_AUTO_DN:
					VAR_RFS_C_FREQ -= step;
					break;
				default:
					VAR_RFS_C_FREQ += step;
					break;
			}
			if (IS_IN_FM(VAR_RFS_BAND)) {
				CORRECT_FM_FREQ(VAR_RFS_C_FREQ);
			} else if (IS_IN_AM(VAR_RFS_BAND)) {
				CORRECT_AM_FREQ(VAR_RFS_C_FREQ);
			}
			
			S_RFS_STATE(RFS_STATE_SET_FREQ);
			
			if (VAR_RFS_S_FREQ==VAR_RFS_C_FREQ) {
				// already travell all freqs
				S_RFS_STATE(RFS_STATE_STOP);
			} else {
				if (RFS_CMD_AUTO_SAVE==G_RFS_CMD()) {
					if (IS_IN_FM(VAR_RFS_BAND)) {
						if (FM_PRESET_NUM<=G_CUR_IDX(VAR_RFS_BAND)) {
							// we've already fill up the preset list
							S_RFS_STATE(RFS_STATE_STOP);
						}
					} else if (IS_IN_AM(VAR_RFS_BAND)) {
						if (AM_PRESET_NUM<=G_CUR_IDX(VAR_RFS_BAND)) {
							// we've already fill up the preset list
							S_RFS_STATE(RFS_STATE_STOP);
						}
					}
				}
			}

			if (RFS_CMD_AF_SEEK_LIST == G_RFS_CMD()) {
				if (!rds_af_is_hit_list(VAR_RFS_C_FREQ)) {
					// the freq not in AF list, try next
					S_RFS_STATE(RFS_STATE_GOTO_NEXT_FREQ);
				}
			}
			break;
		case RFS_STATE_STOP:
			// do some handler and set the proper freq
			VAR_RFS_STOPPED = TRUE;
			switch (G_RFS_CMD()) {
				case RFS_CMD_AUTO_SAVE:
					if (IS_IN_FM(VAR_RFS_BAND)) {
						S_CUR_IDX(VAR_RFS_BAND, 0);
						VAR_RFS_C_FREQ = G_PRESET_FM(VAR_RFS_BAND, 0);
					} else if (IS_IN_AM(VAR_RFS_BAND)) {
						S_CUR_IDX(VAR_RFS_BAND, 0);
						VAR_RFS_C_FREQ = G_PRESET_AM(VAR_RFS_BAND, 0);
					}
					break;
				case RFS_CMD_AUTO_UP:
				case RFS_CMD_AUTO_DN:
				case RFS_CMD_AF_SEEK_LIST:
				case RFS_CMD_AF_SEEK_PI:
				case RFS_CMD_SCAN:
					// use the current freq
					break;
				default:
					if (IS_IN_FM(VAR_RFS_BAND)) {
						VAR_RFS_C_FREQ = FM_MIN_FREQ;
					} else if (IS_IN_AM(VAR_RFS_BAND)) {
						VAR_RFS_C_FREQ = AM_MIN_FREQ;
					}
					break;
			}
			_set_cur_band_freq(VAR_RFS_BAND, VAR_RFS_C_FREQ);
			S_RFS_STATE(RFS_STATE_SET_FREQ);
			break;
		default:
			break;
	}
	
	if (RFS_STATE_FINISHED == G_RFS_STATE()) {
		if (RFS_CMD_AUTO_SAVE==G_RFS_CMD()) {
			PostEvent(WINCE_MODULE,TX_TO_GUI_RADIO_FREQENCY_LIST_INFO,NONE);
		}
		PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_FLAG_INFO,NONE );
		PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PTY_INFO, NONE);
		PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PS_INFO, NONE);
		radio_dev_set_mute(FALSE);
		audio_set_mute(AUDIO_MUTE_RADIO, FALSE);
		SET_SIGNAL_LEVEL((RADIO_SIGNAL_LEVEL)(GET_SIGNAL_LEVEL()&~0x80));
		RESET_SEEK_FLAGS();
	}
}
static void radio_handle_preset_seek(void)
{
	if (RPS_CMD_STOP == G_RPS_CMD()) {
		_set_cur_band_freq_by_preset_id(VAR_RPS_BAND, VAR_RPS_C_ID);
		SET_SIGNAL_LEVEL((RADIO_SIGNAL_LEVEL)(GET_SIGNAL_LEVEL()&~0x80));
		PostEvent(WINCE_MODULE,TX_TO_GUI_RADIO_CURRENT_INFO,NONE);
		S_RPS_STATE(RPS_STATE_FINISHED);
	}
	switch(G_RPS_STATE()) {
		case RPS_STATE_BEGIN:
			RESET_SEEK_FLAGS();
			rds_reset_var();
			SET_SIGNAL_LEVEL((RADIO_SIGNAL_LEVEL)((GET_SIGNAL_LEVEL()|0x80)));
			PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PS_INFO, NONE);	// notify HOST clear the PS info
			if (RPS_CMD_SCAN == G_RPS_CMD()) {
				g_radio_info.flag.field.F_RADIO_SEEK = TRUE;
				g_radio_info.flag.field.F_RADIO_PS = TRUE;
			}
			S_RPS_STATE(RPS_STATE_SET_FREQ);
			break;
		case RPS_STATE_SET_FREQ:
			if (IS_IN_FM(VAR_RPS_BAND)) {
				CORRECT_FM_PRESET_NUM(VAR_RPS_C_ID);
				radio_dev_set_freq_tune(RADIO_BAND_FM, G_PRESET_FM(VAR_RPS_BAND, VAR_RPS_C_ID));
			} else if (IS_IN_AM(VAR_RPS_BAND)) {
				CORRECT_AM_PRESET_NUM(VAR_RPS_C_ID);
				radio_dev_set_freq_tune(RADIO_BAND_AM, G_PRESET_AM(VAR_RPS_BAND, VAR_RPS_C_ID));
			}
			VAR_RPS_TIMER = T300MS_12;
			S_RPS_STATE(RPS_STATE_SET_FREQ_ING);
			break;
		case RPS_STATE_SET_FREQ_ING:
			--VAR_RPS_TIMER;
			if (0==VAR_RPS_TIMER) {
				// force to done set freq op
				S_RPS_STATE(RPS_STATE_SET_FREQ_DONE);
			}
			if (radio_dev_is_set_freq_done()) {
				S_RPS_STATE(RPS_STATE_SET_FREQ_DONE);
			}
			break;
		case RPS_STATE_SET_FREQ_DONE:
			_set_cur_band_freq_by_preset_id(VAR_RPS_BAND, VAR_RPS_C_ID);
			S_CUR_IDX(VAR_RPS_BAND, VAR_RPS_C_ID);
			PostEvent(WINCE_MODULE,TX_TO_GUI_RADIO_CURRENT_INFO,NONE);
			// play this preset freq for 5S
			VAR_RPS_TIMER = T3S_12+T2S_12;
			S_RPS_STATE(RPS_STATE_PLAY_ING);
			break;
		case RPS_STATE_PLAY_ING:
			--VAR_RPS_TIMER;
			if (0==VAR_RPS_TIMER) {
				++VAR_RPS_C_ID;
				if (IS_IN_FM(VAR_RPS_BAND)) {
					CORRECT_FM_PRESET_NUM(VAR_RPS_C_ID);
				} else {
					CORRECT_AM_PRESET_NUM(VAR_RPS_C_ID);
				}
				if (VAR_RPS_C_ID == VAR_RPS_S_ID) {
					// travell all preset freqs
					S_RPS_STATE(RPS_STATE_FINISHED);
				} else {
					// tune to next preset freq
					S_RPS_STATE(RPS_STATE_SET_FREQ);
				}
			}
			break;
		default:
			break;
	}
	
	if (RPS_STATE_FINISHED == G_RPS_STATE()) {
		RESET_SEEK_FLAGS();
	}
}

void radio_event_handler(void)
{
	EVENT *pEvt;
	bool stop_seek = FALSE;
	bool to_discard_evt = FALSE;
	u16 tmp_freq;
	s8 tmp_id;

	pEvt=QueryEvent(TUNER_MODULE);
	if ( (RADIO_EVT_BAND==pEvt->ID) ||
		(RADIO_EVT_SEEK==pEvt->ID) ||
		(RADIO_EVT_PRESET_SELECT==pEvt->ID) ||
		(RADIO_EVT_FREQ_SET==pEvt->ID) ||
		(RADIO_EVT_AREA_SET==pEvt->ID) ||
		(RADIO_EVT_RDS_SWITCH==pEvt->ID) ||
		(RADIO_EVT_RDS_PTY_SET==pEvt->ID) ||
		(RADIO_EVT_LOC_DX==pEvt->ID) ||
		(RADIO_EVT_ST==pEvt->ID) ||
		(RADIO_EVT_PRESET_SAVE==pEvt->ID) ||
		(RADIO_EVT_STOP_SEEK==pEvt->ID) ) {
		stop_seek = TRUE;
	}

	// Are we doing something?
	switch (g_radio_info.state) {
		case RADIO_STATE_FREQ_SEEKING:
			if (IS_DONE_FREQ_SEEK()) {
				g_radio_info.state = RADIO_STATE_IDLE;
			} else {
				if (stop_seek) {
					if (VAR_RFS_STOPPED) {
						// the freq seek handler is processing STOP event, no need
						// to set the STOP state again, or it will disturb the handler.
					} else {
						S_RFS_STATE(RFS_STATE_STOP);
					}
					to_discard_evt = TRUE;

					// clear RDS AF SEEK stuff
					g_radio_info.af_check_timer = 0;
					rds_af_clear_info();
				}
				radio_handle_freq_seek();
			}
			break;
		case RADIO_STATE_PRESET_SEEKING:
			if (IS_DONE_PRESET_SEEK()) {
				g_radio_info.state = RADIO_STATE_IDLE;
			} else {
				if (stop_seek) {
					S_RPS_CMD(RPS_CMD_STOP);
					to_discard_evt = TRUE;
				}
				radio_handle_preset_seek();
			}
			break;
		case RADIO_STATE_FREQ_SETTING:
			if (IS_DONE_FREQ_SET()) {
				g_radio_info.state = RADIO_STATE_IDLE;
			} else {
				radio_handle_freq_set();
			}
			break;
		case RADIO_STATE_PARAM_SETTING:
			if (IS_DONE_PARAM_SET()) {
				g_radio_info.state = RADIO_STATE_IDLE;
			} else {
				radio_handle_param_set();
			}
			break;
	}

	if (!IS_IN_IDLE()) {
		if (to_discard_evt) {
			switch (pEvt->ID) {
				case RADIO_EVT_FREQ_SET:
					// donot discard this event, we need it to select
					// frequency next loop.
					break;
				case RADIO_EVT_AREA_SET:
					// donot discard this event, we need it to
					// select radio area
					break;
				case RADIO_EVT_RDS_SWITCH:
					// donot discard this event, we need it to
					// set the TA/AF
					break;
				case RADIO_EVT_RDS_PTY_SET:
					// donot discard this event, we need it to
					// set the PTY
					break;
				case RADIO_EVT_LOC_DX:
				case RADIO_EVT_ST:
				case RADIO_EVT_PRESET_SAVE:
				case RADIO_EVT_PRESET_SELECT:
					break;
				default:
					//dummy get the event to discard it 
					GetEvent(TUNER_MODULE);
					break;
			}
		}
		return;
	}

	pEvt=GetEvent(TUNER_MODULE);
#if 0
	// discard the events when we are not in radio mode
	if (FrontSource != SOURCE_TUNER) {
		switch (pEvt->ID) {
			case RADIO_EVT_AREA_SET:
			case RADIO_EVT_RDS_SWITCH:
				break;
			default:
				return;
		}
	}
#endif
	switch(pEvt->ID) {
		case RADIO_EVT_BAND:
			g_radio_info.state = RADIO_STATE_FREQ_SETTING;
#ifdef FIX_RADIO_BAND_SWITCH_POP
			radio_config_fix_pop(T500MS_12);
#else
			S_RFSET_STATE(RFSET_STATE_BEGIN);
#endif
			S_RFSET_CMD(RFSET_CMD_SWITCH_BAND);

			if (pEvt->prm<PRESET_BAND_NUMS) {
				if ( IS_IN_FM(pEvt->prm) && IS_IN_AM(G_CUR_BAND()) ) {
					S_ANOTHER_BAND(G_CUR_BAND());
				} else if ( IS_IN_AM(pEvt->prm) && IS_IN_FM(G_CUR_BAND()) ) {
					S_ANOTHER_BAND(G_CUR_BAND());
				}
				S_CUR_BAND((PRESET_BAND)(pEvt->prm));
				VAR_RFSET_BAND = (PRESET_BAND)(pEvt->prm);
				VAR_RFSET_FREQ = G_CUR_FREQ_BY_BAND(pEvt->prm);
			} else if (0xFF == pEvt->prm) {
				PRESET_BAND tmp = G_CUR_BAND();
				S_CUR_BAND(G_ANOTHER_BAND());
				S_ANOTHER_BAND(tmp);
				VAR_RFSET_BAND = G_CUR_BAND();
				VAR_RFSET_FREQ = G_CUR_FREQ_BY_BAND(G_CUR_BAND());
			}
			
			radio_handle_freq_set();
			break;
		case RADIO_EVT_LOC_DX:
			g_radio_info.state = RADIO_STATE_PARAM_SETTING;
			S_RPMSET_STATE(RPMSET_STATE_BEGIN);
			S_RPMSET_CMD(RPMSET_CMD_LOC_DX);
			if (0==pEvt->prm) {
				S_RPMSET_VAL((u16)RDSM_DX);
			} else {
				S_RPMSET_VAL((u16)RDSM_LOC);
			}
			radio_handle_param_set();
			PostEvent(WINCE_MODULE, TX_TO_GUI_RADIO_CURRENT_INFO,NONE );
			break;
		case RADIO_EVT_ST:
			g_radio_info.state = RADIO_STATE_PARAM_SETTING;
			S_RPMSET_STATE(RPMSET_STATE_BEGIN);
			S_RPMSET_CMD(RPMSET_CMD_ST);
			if (0==pEvt->prm) {
				S_RPMSET_VAL(0);
			} else {
				S_RPMSET_VAL(1);
			}
			radio_handle_param_set();
			PostEvent(WINCE_MODULE, TX_TO_GUI_RADIO_CURRENT_INFO,NONE );
			break;
		case RADIO_EVT_PRESET_SELECT:
			tmp_id = (s8)(pEvt->prm);
			if (IS_IN_FM(G_CUR_BAND())) {
				CORRECT_FM_PRESET_NUM(tmp_id);
				tmp_freq = G_PRESET_FM(G_CUR_BAND(), tmp_id);
			} else if (IS_IN_AM(G_CUR_BAND())) {
				CORRECT_AM_PRESET_NUM(tmp_id);
				tmp_freq = G_PRESET_AM(G_CUR_BAND(), tmp_id);
			}
			S_CUR_IDX(G_CUR_BAND(), tmp_id);
			
			if (G_CUR_FREQ() != tmp_freq) {
				g_radio_info.state = RADIO_STATE_FREQ_SETTING;
#ifdef FIX_RADIO_BAND_SWITCH_POP
				radio_config_fix_pop(T300MS_12);
#else
				S_RFSET_STATE(RFSET_STATE_BEGIN);
#endif
				S_RFSET_CMD(RFSET_CMD_SET_TO);
				VAR_RFSET_BAND = G_CUR_BAND();
				VAR_RFSET_FREQ = tmp_freq;
				radio_handle_freq_set();
			} else {
				// notify HOST, even we didn't set freq 
				PostEvent(WINCE_MODULE,TX_TO_GUI_RADIO_CURRENT_INFO,NONE);
			}
			break;
		case RADIO_EVT_PRESET_SAVE:
			tmp_id = (s8)(pEvt->prm);
			if (IS_IN_FM(G_CUR_BAND())) {
				CORRECT_FM_PRESET_NUM(tmp_id);
				S_PRESET_FM(G_CUR_BAND(), tmp_id, G_CUR_FREQ());
			} else if (IS_IN_AM(G_CUR_BAND())) {
				CORRECT_AM_PRESET_NUM(tmp_id);
				S_PRESET_AM(G_CUR_BAND(), tmp_id, G_CUR_FREQ());
			}
			S_CUR_IDX(G_CUR_BAND(), tmp_id);
			PostEvent(WINCE_MODULE, TX_TO_GUI_RADIO_FREQENCY_LIST_INFO,NONE );
			PostEvent(WINCE_MODULE,TX_TO_GUI_RADIO_CURRENT_INFO,NONE);
			break;
		case RADIO_EVT_FREQ_SET:
			if (IS_IN_FM(G_CUR_BAND())) {
				CORRECT_FM_FREQ(pEvt->prm);
				tmp_freq = pEvt->prm;
			} else if (IS_IN_AM(G_CUR_BAND())) {
				CORRECT_AM_FREQ(pEvt->prm);
				tmp_freq = pEvt->prm;
			}			
			if (G_CUR_FREQ() != tmp_freq) {
				g_radio_info.state = RADIO_STATE_FREQ_SETTING;
#ifdef FIX_RADIO_BAND_SWITCH_POP
				radio_config_fix_pop(T300MS_12);
#else
				S_RFSET_STATE(RFSET_STATE_BEGIN);
#endif
				S_RFSET_CMD(RFSET_CMD_SET_TO);
				VAR_RFSET_BAND = G_CUR_BAND();
				VAR_RFSET_FREQ = tmp_freq;
				radio_handle_freq_set();
			}
			break;
		case RADIO_EVT_SEEK:
			switch(pEvt->prm) {
				case 0:	// AS
					g_radio_info.state = RADIO_STATE_FREQ_SEEKING;
					g_radio_info.evt_info.freq_seek.state = RFS_STATE_BEGIN;
					g_radio_info.evt_info.freq_seek.cmd = RFS_CMD_AUTO_SAVE;
					g_radio_info.evt_info.freq_seek.band = G_CUR_BAND();
					if (IS_IN_FM(G_CUR_BAND())) {
						g_radio_info.evt_info.freq_seek.cur_freq = FM_MIN_FREQ;
					} else {
						g_radio_info.evt_info.freq_seek.cur_freq = AM_MIN_FREQ;
					}
					g_radio_info.evt_info.freq_seek.start_freq = g_radio_info.evt_info.freq_seek.cur_freq;
					radio_handle_freq_seek();
					break;
				case 2:	// SCAN
					g_radio_info.state = RADIO_STATE_FREQ_SEEKING;
					g_radio_info.evt_info.freq_seek.state = RFS_STATE_BEGIN;
					g_radio_info.evt_info.freq_seek.cmd = RFS_CMD_SCAN;
					g_radio_info.evt_info.freq_seek.band = G_CUR_BAND();
					g_radio_info.evt_info.freq_seek.cur_freq = G_CUR_FREQ();
					g_radio_info.evt_info.freq_seek.start_freq = G_CUR_FREQ();
					radio_handle_freq_seek();
					break;
				case 1:	// PS
					g_radio_info.state = RADIO_STATE_PRESET_SEEKING;
					g_radio_info.evt_info.preset_seek.state = RPS_STATE_BEGIN;
					g_radio_info.evt_info.preset_seek.cmd = RPS_CMD_SCAN;
					g_radio_info.evt_info.preset_seek.band = G_CUR_BAND();
					g_radio_info.evt_info.preset_seek.start_id = 0;
					g_radio_info.evt_info.preset_seek.cur_id = 0;
					radio_handle_preset_seek();
					break;
				case 3:	// Auto seek up
				case 4:	// Auto seek down
					g_radio_info.state = RADIO_STATE_FREQ_SEEKING;
					g_radio_info.evt_info.freq_seek.state = RFS_STATE_BEGIN;
					if (3==pEvt->prm) {
						g_radio_info.evt_info.freq_seek.cmd = RFS_CMD_AUTO_DN;
					} else if (4==pEvt->prm) {
						g_radio_info.evt_info.freq_seek.cmd = RFS_CMD_AUTO_UP;
					}
					g_radio_info.evt_info.freq_seek.band = G_CUR_BAND();
					g_radio_info.evt_info.freq_seek.start_freq = G_CUR_FREQ();
					g_radio_info.evt_info.freq_seek.cur_freq = G_CUR_FREQ();
					radio_handle_freq_seek();
					break;
				case 5:	// step up
				case 6:	// step down
					g_radio_info.state = RADIO_STATE_FREQ_SETTING;
#ifdef FIX_RADIO_BAND_SWITCH_POP
					radio_config_fix_pop(T300MS_12);
#else
					S_RFSET_STATE(RFSET_STATE_BEGIN);
#endif
					if (5==pEvt->prm) {
						S_RFSET_CMD(RFSET_CMD_STEP_DN);
					} else if (6==pEvt->prm) {
						S_RFSET_CMD(RFSET_CMD_STEP_UP);
					}
					VAR_RFSET_BAND = G_CUR_BAND();
					VAR_RFSET_FREQ = G_CUR_FREQ();
					radio_handle_freq_set();
					break;
				case 8:	// prev preset
				case 9:	// next preset
					if (IS_IN_FM(G_CUR_BAND())) {
						tmp_id = G_CUR_IDX(G_CUR_BAND());
						CORRECT_FM_PRESET_NUM(tmp_id);
						if (8==pEvt->prm) {
							--tmp_id;
						} else if (9==pEvt->prm) {
							++tmp_id;
						}
						CORRECT_FM_PRESET_NUM(tmp_id);
						S_CUR_IDX(G_CUR_BAND(), tmp_id);
						tmp_freq = G_PRESET_FM(G_CUR_BAND(), tmp_id);
					}else if (IS_IN_AM(G_CUR_BAND())) {
						tmp_id = G_CUR_IDX(G_CUR_BAND());
						CORRECT_AM_PRESET_NUM(tmp_id);
						if (8==pEvt->prm) {
							--tmp_id;
						} else if (9==pEvt->prm) {
							++tmp_id;
						}
						CORRECT_AM_PRESET_NUM(tmp_id);
						S_CUR_IDX(G_CUR_BAND(), tmp_id);
						tmp_freq = G_PRESET_AM(G_CUR_BAND(), tmp_id);
					}
					g_radio_info.state = RADIO_STATE_FREQ_SETTING;
#ifdef FIX_RADIO_BAND_SWITCH_POP
					radio_config_fix_pop(T300MS_12);
#else
					S_RFSET_STATE(RFSET_STATE_BEGIN);
#endif
					S_RFSET_CMD(RFSET_CMD_SET_TO);
					VAR_RFSET_BAND = G_CUR_BAND();
					VAR_RFSET_FREQ = tmp_freq;
					radio_handle_freq_set();
					break;
				default:
					break;

			}
			break;
		case RADIO_EVT_RDS_SWITCH:
			switch (pEvt->prm) {
				case 0:
				case 1:
					g_radio_info.state = RADIO_STATE_PARAM_SETTING;
					S_RPMSET_STATE(RPMSET_STATE_BEGIN);
					S_RPMSET_CMD(RPMSET_CMD_RDS_AF_SWITCH);
					if (0==pEvt->prm) {
						S_RPMSET_VAL((u16)FALSE);
					} else {
						S_RPMSET_VAL((u16)TRUE);
					}
					radio_handle_param_set();
					break;
				case 2:
				case 3:
					g_radio_info.state = RADIO_STATE_PARAM_SETTING;
					S_RPMSET_STATE(RPMSET_STATE_BEGIN);
					S_RPMSET_CMD(RPMSET_CMD_RDS_TA_SWITCH);
					if (2==pEvt->prm) {
						S_RPMSET_VAL((u16)FALSE);
					} else {
						S_RPMSET_VAL((u16)TRUE);
						g_radio_info.tp_loss_timer = (T8S_12-T500MS_12);// make auto seek TP station later, if need.
					}
					radio_handle_param_set();
					break;
				default:
					break;
			}
			break;
		case RADIO_EVT_RDS_PTY_SET:
			g_radio_info.state = RADIO_STATE_PARAM_SETTING;
			S_RPMSET_STATE(RPMSET_STATE_BEGIN);
			S_RPMSET_CMD(RPMSET_CMD_RDS_PTY_SEL);
			S_RPMSET_VAL(pEvt->prm);
			radio_handle_param_set();
			if (0 != pEvt->prm) {
				PostEvent(TUNER_MODULE, RADIO_EVT_SEEK, 4);	// auto seek the next valid PTY station
			}
			break;
		case RADIO_EVT_AREA_SET:
			radio_init_area(g_radio_area);
			PostEvent(WINCE_MODULE,TX_TO_GUI_RADIO_CURRENT_INFO,NONE);
			break;
		default:
			break;
	}

}

void radio_do_af_seek(bool seek_list)
{
	// check AF
	if (FALSE == g_rds_info.af_switch_on) {
		return;
	}

	if (0==g_rds_af_info.pi) {
		// invalid PI
		return;
	}
	
	g_radio_info.state = RADIO_STATE_FREQ_SEEKING;
	g_radio_info.evt_info.freq_seek.state = RFS_STATE_BEGIN;
	g_radio_info.evt_info.freq_seek.band = G_CUR_BAND();
	g_radio_info.evt_info.freq_seek.start_freq = G_CUR_FREQ();
	g_radio_info.evt_info.freq_seek.cur_freq = G_CUR_FREQ();
	if (seek_list) {
		g_radio_info.evt_info.freq_seek.cmd = RFS_CMD_AF_SEEK_LIST;
	} else {
		g_radio_info.evt_info.freq_seek.cmd = RFS_CMD_AF_SEEK_PI;
	}
	radio_handle_freq_seek();

}

void radio_status_monitor(void)
{
	u8 stereo;
	bool notify_radio_info = FALSE;
	bool notify_rds_info = FALSE;

	if (IS_IN_AM(G_CUR_BAND())) {
		return;
	}
	
	// detect stereo change
	if (FrontSource == SOURCE_TUNER) {
		if (++g_radio_info.st_det_timer >= T1S5_12) {
			g_radio_info.st_det_timer = 0;

			// only detect ST, if the ST switch is on
			if (0==g_radio_info.flag.field.F_RADIO_ST_SW) {
				stereo = radio_dev_get_stereo_status();
				if ( (0==g_radio_info.flag.field.F_RADIO_ST_MONO) &&
					 (2==stereo) ) {
//					 g_radio_info.flag.field.F_RADIO_ST_MONO = 1;
//					 notify_radio_info = TRUE;
				} else if ( (1==g_radio_info.flag.field.F_RADIO_ST_MONO) &&
					(1==stereo) ) {
					 g_radio_info.flag.field.F_RADIO_ST_MONO = 0;
//					 notify_radio_info = TRUE;
				}
			}
		}
	}

	// count signal timer
	if (!Is_Machine_Power) {
		g_radio_info.signal_loss_timer = 0;
		g_radio_info.af_check_timer = 0;
	} else if (!radio_dev_is_tune_ok(FALSE)) {
		++g_radio_info.signal_loss_timer;
	} else {
		g_radio_info.signal_loss_timer = 0;
		g_radio_info.af_check_timer = 0;
	}

	// count TP timer
	if (g_rds_info.ta_switch_on) {
		if (FALSE == g_rds_info.tp_on) {
			++g_radio_info.tp_loss_timer;
		} else {
			g_radio_info.tp_loss_timer = 0;
		}
	} else {
		g_radio_info.tp_loss_timer = 0;
	}

	// process TP lost	
	if (g_radio_info.tp_loss_timer >= T8S_12) {
		g_radio_info.tp_loss_timer = 0;
		// issue an auto seek
		g_radio_info.state = RADIO_STATE_FREQ_SEEKING;
		g_radio_info.evt_info.freq_seek.state = RFS_STATE_BEGIN;
		g_radio_info.evt_info.freq_seek.cmd = RFS_CMD_AUTO_UP;
		g_radio_info.evt_info.freq_seek.band = G_CUR_BAND();
		g_radio_info.evt_info.freq_seek.start_freq = G_CUR_FREQ();
		g_radio_info.evt_info.freq_seek.cur_freq = G_CUR_FREQ();
		radio_handle_freq_seek();
		return;
	}

	// process signal lost
	if (g_radio_info.signal_loss_timer >= T2S_12) {
		g_radio_info.signal_loss_timer = 0;

		if (g_rds_info.af_switch_on) {
			if (0==g_radio_info.af_check_timer) {
				// first AF seek, save AF&PI info, and seek AF list
				rds_af_save_info();
				radio_do_af_seek(TRUE);
			}
			if (1==(g_radio_info.af_check_timer%4)) {
				// seek program which PI matched
				radio_do_af_seek(FALSE);

				// roll counter over, make sure not to overflow
				if (g_radio_info.af_check_timer>240) {
					g_radio_info.af_check_timer = 1;
				}
			}
			++g_radio_info.af_check_timer;
		} else {
			g_radio_info.af_check_timer = 0;
		}
		
		// clear RDS info
		if ( (g_rds_info.tp_on) || (g_rds_info.ta_on) ||
			(g_rds_info.pty != 0) ||
			(g_rds_info.ps[0] != 0) ) {
			notify_rds_info = TRUE;
		}
		rds_reset_var();
	}

	if (notify_radio_info) {
		PostEvent(WINCE_MODULE,TX_TO_GUI_RADIO_CURRENT_INFO,NONE);
	}
	if (notify_rds_info) {
		PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_FLAG_INFO,NONE );
		PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PTY_INFO, NONE);
		PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PS_INFO, NONE);
	}
}

void radio_init_area(u8 area)
{
	u8 i;
	u8 j;
	switch (area) {
		case RADIO_AREA_USA:
			FM_MIN_FREQ = 8750;
			FM_MAX_FREQ = 10790;
			FM_STEP = 20;
			AM_MIN_FREQ = 530;
			AM_MAX_FREQ = 1710;
			AM_STEP = 10;
			break;
		case RADIO_AREA_LATIN:
			FM_MIN_FREQ = 8750;
			FM_MAX_FREQ = 10800;
			FM_STEP = 10;
			AM_MIN_FREQ = 530;
			AM_MAX_FREQ = 1710;
			AM_STEP = 10;
			break;
		case RADIO_AREA_EUROPE:
			FM_MIN_FREQ = 8750;
			FM_MAX_FREQ = 10800;
			FM_STEP = 5;
			AM_MIN_FREQ = 522;
			AM_MAX_FREQ = 1620;
			AM_STEP = 9;
			break;
		case RADIO_AREA_OIRT:
			FM_MIN_FREQ = 6500;
			FM_MAX_FREQ = 7400;
			FM_STEP = 3;
			AM_MIN_FREQ = 522;
			AM_MAX_FREQ = 1620;
			AM_STEP = 9;
			break;
		case RADIO_AREA_JAPAN:
			FM_MIN_FREQ = 7600;
			FM_MAX_FREQ = 9900;
			FM_STEP = 10;
			AM_MIN_FREQ = 522;
			AM_MAX_FREQ = 1629;
			AM_STEP = 9;
			break;
		case RADIO_AREA_CHN:
			FM_MIN_FREQ = 8750;
			FM_MAX_FREQ = 10800;
			FM_STEP = 10;
			AM_MIN_FREQ = 522;
			AM_MAX_FREQ = 1620;
			AM_STEP = 9;
			break;
		case RADIO_AREA_KOREA:
			FM_MIN_FREQ = 8810;
			FM_MAX_FREQ = 10790;
			FM_STEP = 20;
			AM_MIN_FREQ = 531;
			AM_MAX_FREQ = 1611;
			AM_STEP = 9;
			break;
		case RADIO_AREA_SOUTH_AMERICA:
			FM_MIN_FREQ = 8790;
			FM_MAX_FREQ = 10790;
			FM_STEP = 10;
			AM_MIN_FREQ = 531;
			AM_MAX_FREQ = 1629;
			AM_STEP = 9;
			break;
		case RADIO_AREA_AUSTRALIA:
			FM_MIN_FREQ = 8750;
			FM_MAX_FREQ = 10800;
			FM_STEP = 10;
			AM_MIN_FREQ = 522;
			AM_MAX_FREQ = 1710;
			AM_STEP = 9;
			break;
		case RADIO_AREA_ASIA_PACIFIC:
			FM_MIN_FREQ = 8750;
			FM_MAX_FREQ = 10800;
			FM_STEP = 5;
			AM_MIN_FREQ = 531;
			AM_MAX_FREQ = 1629;
			AM_STEP = 9;
			break;
		case RADIO_AREA_RUSSIA:
			FM_MIN_FREQ = 6500;
			FM_MAX_FREQ = 10800;
			FM_STEP = 10;
			AM_MIN_FREQ = 522;
			AM_MAX_FREQ = 1620;
			AM_STEP = 9;
			break;
		case RADIO_AREA_JAPAN_2:
			FM_MIN_FREQ = 7600;
			FM_MAX_FREQ = 10800;
			FM_STEP = 10;
			AM_MIN_FREQ = 522;
			AM_MAX_FREQ = 1629;
			AM_STEP = 9;
			break;
	}
	CORRECT_FM_FREQ(G_CUR_FREQ_BY_BAND(PRESET_BAND_FM1));
	CORRECT_FM_FREQ(G_CUR_FREQ_BY_BAND(PRESET_BAND_FM2));
	CORRECT_FM_FREQ(G_CUR_FREQ_BY_BAND(PRESET_BAND_FM3));
	CORRECT_AM_FREQ(G_CUR_FREQ_BY_BAND(PRESET_BAND_AM1));
	CORRECT_AM_FREQ(G_CUR_FREQ_BY_BAND(PRESET_BAND_AM2));

	//add some default FM freq
	switch (area) {
		case RADIO_AREA_USA:
			for (i=PRESET_BAND_FM1; i<=PRESET_BAND_FM3; i++) {
				if (0==G_PRESET_FM(i, 0)) S_PRESET_FM(i, 0, 8750);
				if (0==G_PRESET_FM(i, 1)) S_PRESET_FM(i, 1, 9010);
				if (0==G_PRESET_FM(i, 2)) S_PRESET_FM(i, 2, 9810);
				if (0==G_PRESET_FM(i, 3)) S_PRESET_FM(i, 3, 10610);
				if (0==G_PRESET_FM(i, 4)) S_PRESET_FM(i, 4, 10790);
			}
			break;
		case RADIO_AREA_LATIN:
		case RADIO_AREA_EUROPE:
		case RADIO_AREA_CHN:
		case RADIO_AREA_AUSTRALIA:
		case RADIO_AREA_ASIA_PACIFIC:
			for (i=PRESET_BAND_FM1; i<=PRESET_BAND_FM3; i++) {
				if (0==G_PRESET_FM(i, 0)) S_PRESET_FM(i, 0, 8750);
				if (0==G_PRESET_FM(i, 1)) S_PRESET_FM(i, 1, 9010);
				if (0==G_PRESET_FM(i, 2)) S_PRESET_FM(i, 2, 9810);
				if (0==G_PRESET_FM(i, 3)) S_PRESET_FM(i, 3, 10610);
				if (0==G_PRESET_FM(i, 4)) S_PRESET_FM(i, 4, 10800);
			}
			break;
		case RADIO_AREA_OIRT:
			for (i=PRESET_BAND_FM1; i<=PRESET_BAND_FM3; i++) {
				if (0==G_PRESET_FM(i, 0)) S_PRESET_FM(i, 0, 6500);
				if (0==G_PRESET_FM(i, 1)) S_PRESET_FM(i, 1, 6710);
				if (0==G_PRESET_FM(i, 2)) S_PRESET_FM(i, 2, 7040);
				if (0==G_PRESET_FM(i, 3)) S_PRESET_FM(i, 3, 7250);
				if (0==G_PRESET_FM(i, 4)) S_PRESET_FM(i, 4, 7400);
			}
			break;
		case RADIO_AREA_JAPAN:
		case RADIO_AREA_JAPAN_2:
			for (i=PRESET_BAND_FM1; i<=PRESET_BAND_FM3; i++) {
				if (0==G_PRESET_FM(i, 0)) S_PRESET_FM(i, 0, 7600);
				if (0==G_PRESET_FM(i, 1)) S_PRESET_FM(i, 1, 7800);
				if (0==G_PRESET_FM(i, 2)) S_PRESET_FM(i, 2, 8300);
				if (0==G_PRESET_FM(i, 3)) S_PRESET_FM(i, 3, 8810);
				if (0==G_PRESET_FM(i, 4)) S_PRESET_FM(i, 4, 9500);
				if (0==G_PRESET_FM(i, 5)) S_PRESET_FM(i, 5, 9900);
			}
			break;
		case RADIO_AREA_KOREA:
			for (i=PRESET_BAND_FM1; i<=PRESET_BAND_FM3; i++) {
				if (0==G_PRESET_FM(i, 0)) S_PRESET_FM(i, 0, 8810);
				if (0==G_PRESET_FM(i, 1)) S_PRESET_FM(i, 1, 9010);
				if (0==G_PRESET_FM(i, 2)) S_PRESET_FM(i, 2, 9810);
				if (0==G_PRESET_FM(i, 3)) S_PRESET_FM(i, 3, 10610);
				if (0==G_PRESET_FM(i, 4)) S_PRESET_FM(i, 4, 10790);
			}
			break;
		case RADIO_AREA_SOUTH_AMERICA:
			for (i=PRESET_BAND_FM1; i<=PRESET_BAND_FM3; i++) {
				if (0==G_PRESET_FM(i, 0)) S_PRESET_FM(i, 0, 8790);
				if (0==G_PRESET_FM(i, 1)) S_PRESET_FM(i, 1, 9010);
				if (0==G_PRESET_FM(i, 2)) S_PRESET_FM(i, 2, 9810);
				if (0==G_PRESET_FM(i, 3)) S_PRESET_FM(i, 3, 10610);
				if (0==G_PRESET_FM(i, 4)) S_PRESET_FM(i, 4, 10790);
			}
			break;
		case RADIO_AREA_RUSSIA:
			for (i=PRESET_BAND_FM1; i<=PRESET_BAND_FM3; i++) {
				if (0==G_PRESET_FM(i, 0)) S_PRESET_FM(i, 0, 6500);
				if (0==G_PRESET_FM(i, 1)) S_PRESET_FM(i, 1, 6710);
				if (0==G_PRESET_FM(i, 2)) S_PRESET_FM(i, 2, 7040);
				if (0==G_PRESET_FM(i, 3)) S_PRESET_FM(i, 3, 7250);
				if (0==G_PRESET_FM(i, 4)) S_PRESET_FM(i, 4, 7400);
				if (0==G_PRESET_FM(i, 5)) S_PRESET_FM(i, 5, 6500);
				if (0==G_PRESET_FM(i, 6)) S_PRESET_FM(i, 6, 8750);
				if (0==G_PRESET_FM(i, 7)) S_PRESET_FM(i, 7, 9010);
				if (0==G_PRESET_FM(i, 8)) S_PRESET_FM(i, 8, 9810);
				if (0==G_PRESET_FM(i, 9)) S_PRESET_FM(i, 9, 10610);
				if (0==G_PRESET_FM(i, 10)) S_PRESET_FM(i, 10, 10800);
			}
			break;
	}

	//add some default AM freq
	switch (area) {
		case RADIO_AREA_USA:
		case RADIO_AREA_LATIN:
			for (i=PRESET_BAND_AM1; i<=PRESET_BAND_AM2; i++) {
				if (0==G_PRESET_AM(i, 0)) S_PRESET_AM(i, 0, 530);
				if (0==G_PRESET_AM(i, 1)) S_PRESET_AM(i, 1, 600);
				if (0==G_PRESET_AM(i, 2)) S_PRESET_AM(i, 2, 1000);
				if (0==G_PRESET_AM(i, 3)) S_PRESET_AM(i, 3, 1400);
				if (0==G_PRESET_AM(i, 4)) S_PRESET_AM(i, 4, 1710);
			}
			break;
			
		case RADIO_AREA_KOREA:
			for (i=PRESET_BAND_AM1; i<=PRESET_BAND_AM2; i++) {
				if (0==G_PRESET_AM(i, 0)) S_PRESET_AM(i, 0, 531);
				if (0==G_PRESET_AM(i, 1)) S_PRESET_AM(i, 1, 603);
				if (0==G_PRESET_AM(i, 2)) S_PRESET_AM(i, 2, 999);
				if (0==G_PRESET_AM(i, 3)) S_PRESET_AM(i, 3, 1404);
				if (0==G_PRESET_AM(i, 4)) S_PRESET_AM(i, 4, 1611);
			}
			break;
		case RADIO_AREA_SOUTH_AMERICA:
		case RADIO_AREA_ASIA_PACIFIC:
			for (i=PRESET_BAND_AM1; i<=PRESET_BAND_AM2; i++) {
				if (0==G_PRESET_AM(i, 0)) S_PRESET_AM(i, 0, 531);
				if (0==G_PRESET_AM(i, 1)) S_PRESET_AM(i, 1, 603);
				if (0==G_PRESET_AM(i, 2)) S_PRESET_AM(i, 2, 999);
				if (0==G_PRESET_AM(i, 3)) S_PRESET_AM(i, 3, 1404);
				if (0==G_PRESET_AM(i, 4)) S_PRESET_AM(i, 4, 1629);
			}
			break;

		case RADIO_AREA_JAPAN:
		case RADIO_AREA_JAPAN_2:
			for (i=PRESET_BAND_AM1; i<=PRESET_BAND_AM2; i++) {
				if (0==G_PRESET_AM(i, 0)) S_PRESET_AM(i, 0, 522);
				if (0==G_PRESET_AM(i, 1)) S_PRESET_AM(i, 1, 603);
				if (0==G_PRESET_AM(i, 2)) S_PRESET_AM(i, 2, 999);
				if (0==G_PRESET_AM(i, 3)) S_PRESET_AM(i, 3, 1295);
				if (0==G_PRESET_AM(i, 4)) S_PRESET_AM(i, 4, 1440);
				if (0==G_PRESET_AM(i, 5)) S_PRESET_AM(i, 5, 1629);
			}
			break;

		case RADIO_AREA_AUSTRALIA:
			for (i=PRESET_BAND_AM1; i<=PRESET_BAND_AM2; i++) {
				if (0==G_PRESET_AM(i, 0)) S_PRESET_AM(i, 0, 522);
				if (0==G_PRESET_AM(i, 1)) S_PRESET_AM(i, 1, 603);
				if (0==G_PRESET_AM(i, 2)) S_PRESET_AM(i, 2, 999);
				if (0==G_PRESET_AM(i, 3)) S_PRESET_AM(i, 3, 1404);
				if (0==G_PRESET_AM(i, 4)) S_PRESET_AM(i, 4, 1710);
			}
			break;
			
		case RADIO_AREA_RUSSIA:
		case RADIO_AREA_EUROPE:
		case RADIO_AREA_OIRT:
		case RADIO_AREA_CHN:
			for (i=PRESET_BAND_AM1; i<=PRESET_BAND_AM2; i++) {
				if (0==G_PRESET_AM(i, 0)) S_PRESET_AM(i, 0, 522);
				if (0==G_PRESET_AM(i, 1)) S_PRESET_AM(i, 1, 603);
				if (0==G_PRESET_AM(i, 2)) S_PRESET_AM(i, 2, 999);
				if (0==G_PRESET_AM(i, 3)) S_PRESET_AM(i, 3, 1404);
				if (0==G_PRESET_AM(i, 4)) S_PRESET_AM(i, 4, 1620);
			}
			break;
	}

	for (i=PRESET_BAND_FM1; i<=PRESET_BAND_FM3; i++) {
		for(j=0;j<FM_PRESET_NUM;j++) {
			CORRECT_FM_FREQ(G_PRESET_FM(i, j));
		}
	}
	for (i=PRESET_BAND_AM1; i<=PRESET_BAND_AM2; i++) {
		for(j=0;j<AM_PRESET_NUM;j++) {
			CORRECT_AM_FREQ(G_PRESET_AM(i, j));
		}
	}
}

void radio_init(void)
{
	u8 i;
	u8 j;
	g_radio_info.state = RADIO_STATE_UNKNOWN;
	g_radio_info.flag.field.F_RADIO_ST_SW = 0;

	S_CUR_BAND(PRESET_BAND_FM1);
	S_ANOTHER_BAND(PRESET_BAND_AM1);
	// we will correct the freq by radio_init_area function later.
	S_CUR_FREQ_BY_BAND(PRESET_BAND_FM1, 0);
	S_CUR_FREQ_BY_BAND(PRESET_BAND_FM2, 0);
	S_CUR_FREQ_BY_BAND(PRESET_BAND_FM3, 0);
	S_CUR_FREQ_BY_BAND(PRESET_BAND_AM1, 0);
	S_CUR_FREQ_BY_BAND(PRESET_BAND_AM2, 0);

	for (i=PRESET_BAND_FM1; i<=PRESET_BAND_FM3; i++) {
		for(j=0;j<FM_PRESET_NUM;j++) {
			S_PRESET_FM(i, j, 0);
		}
	}
	for (i=PRESET_BAND_AM1; i<=PRESET_BAND_AM2; i++) {
		for(j=0;j<AM_PRESET_NUM;j++) {
			S_PRESET_AM(i, j, 0);
		}
	}
	rds_init();
}

void radio_main(void)
{
	if (RADIO_STATE_PWR_OFF_ING == g_radio_info.state) {
		// deinit the radio device
		radio_dev_deinit();
		g_radio_info.state = RADIO_STATE_UNKNOWN;
		return;
	}
	if (RADIO_STATE_PWR_ON_ING == g_radio_info.state) {
		g_radio_info.state = RADIO_STATE_INIT_ING;
		g_radio_info.pwr_timer = 0;
		GPIO_SetBits(GPIO_ANT_CTRL_GRP, GPIO_ANT_CTRL_PIN);
		return;
	}
	if (RADIO_STATE_INIT_ING == g_radio_info.state) {
		if (!IS_PANEL_PWR_ON) {
			return;
		}
		if (g_radio_info.pwr_timer < T3S_12) {
			++g_radio_info.pwr_timer;
			return;
		}
		if (TRUE==radio_dev_init()) {
			// We should init radio area here, because the g_radio_area is valid
			// after System_LoadSetting, and it may change at run-time
			radio_init_area(g_radio_area);

			if (IS_IN_FM(G_CUR_BAND())) {
				radio_dev_set_freq_tune(RADIO_BAND_FM, G_CUR_FREQ());
			} else {
				radio_dev_set_freq_tune(RADIO_BAND_AM, G_CUR_FREQ());
			}
			rds_reset_var();
			radio_dev_rds_init();
			RESET_SEEK_FLAGS();
			g_radio_info.flag.field.F_RADIO_ST_MONO = 0;
			g_radio_info.signal_loss_timer = 0;
			g_radio_info.tp_loss_timer = 0;
			g_radio_info.st_det_timer = 0;
			g_radio_info.af_check_timer = 0;
			g_radio_info.state = RADIO_STATE_IDLE;
		}
		return;
	}
	if (RADIO_STATE_UNKNOWN == g_radio_info.state) {
		return;
	}

	radio_event_handler();
	if (IS_IN_FM(G_CUR_BAND())) {
		rds_main();
	}
	// monitor radio status, like stereo/mono etc
	if (RADIO_STATE_IDLE==g_radio_info.state) {
		radio_status_monitor();

		// workaround for some unexpect radio mute bug
		if (g_audio_info.mute & AUDIO_MUTE_RADIO) {
			radio_dev_set_mute(FALSE);
			audio_set_mute(AUDIO_MUTE_RADIO, FALSE);
		}
	}
}

void radio_set_pwr_ctrl(bool on)
{
	if (FALSE == on) {
		g_radio_info.state = RADIO_STATE_PWR_OFF_ING;
	} else {
		g_radio_info.state = RADIO_STATE_PWR_ON_ING;
	}
}

u8 radio_comm_fill_total_info(u8 *pbuf)
{
	u8 cnt;
	u8 len;

	if (IS_IN_FM(G_CUR_BAND())) {
		// FM info
		pbuf[0]=G_CUR_BAND();
		pbuf[1]=G_CUR_IDX(G_CUR_BAND());
		pbuf[2]=LSB(G_CUR_FREQ());
		pbuf[3]=MSB(G_CUR_FREQ());
		pbuf[4]=(u8)GET_SIGNAL_LEVEL();
		pbuf[5]=g_radio_info.flag.byte;	
		pbuf[6]=0;
		for(cnt=0;cnt<FM_PRESET_NUM;cnt++)
		{
			pbuf[7+cnt*2]=MSB(G_PRESET_FM(G_CUR_BAND(), cnt));
			pbuf[7+cnt*2+1]=LSB(G_PRESET_FM(G_CUR_BAND(), cnt));
		}
		len = 7+2*FM_PRESET_NUM;
	} else {
		// use AM info
		pbuf[0]=G_CUR_BAND();
		pbuf[1]=G_CUR_IDX(G_CUR_BAND());
		pbuf[2]=LSB(G_CUR_FREQ());
		pbuf[3]=MSB(G_CUR_FREQ());
		pbuf[4]=(u8)GET_SIGNAL_LEVEL();
		pbuf[5]=g_radio_info.flag.byte;	
		pbuf[6]=0;
		for(cnt=0;cnt<AM_PRESET_NUM;cnt++)
		{
			pbuf[7+cnt*2]=MSB(G_PRESET_AM(G_CUR_BAND(), cnt));
			pbuf[7+cnt*2+1]=LSB(G_PRESET_AM(G_CUR_BAND(), cnt));
		}
		len = 7+2*AM_PRESET_NUM;
	}
	return len;
}
u8 radio_comm_fill_cur_info(u8 *pbuf)
{
	u8 len;

	if (IS_IN_FM(G_CUR_BAND())) {
		// FM info
		pbuf[0]=G_CUR_BAND();
		pbuf[1]=G_CUR_IDX(G_CUR_BAND());
		pbuf[2]=MSB(G_CUR_FREQ());
		pbuf[3]=LSB(G_CUR_FREQ());
		pbuf[4]=(u8)GET_SIGNAL_LEVEL();
		pbuf[5]=g_radio_info.flag.byte;	
		pbuf[6]=MSB(FM_MIN_FREQ);
		pbuf[7]=LSB(FM_MIN_FREQ);
		pbuf[8]=MSB(FM_MAX_FREQ);
		pbuf[9]=LSB(FM_MAX_FREQ);
		len = 10;
	} else {
		// use AM info
		pbuf[0]=G_CUR_BAND();
		pbuf[1]=G_CUR_IDX(G_CUR_BAND());
		pbuf[2]=MSB(G_CUR_FREQ());
		pbuf[3]=LSB(G_CUR_FREQ());
		pbuf[4]=(u8)GET_SIGNAL_LEVEL();
		pbuf[5]=g_radio_info.flag.byte;	
		pbuf[6]=MSB(AM_MIN_FREQ);
		pbuf[7]=LSB(AM_MIN_FREQ);
		pbuf[8]=MSB(AM_MAX_FREQ);
		pbuf[9]=LSB(AM_MAX_FREQ);
		len = 10;
	}
	return len;
}
u8 radio_comm_fill_preset_info(u8 *pbuf)
{
	u8 cnt;
	u8 len;

	if (IS_IN_FM(G_CUR_BAND())) {
		// FM info
		for(cnt=0;cnt<FM_PRESET_NUM;cnt++)
		{
			pbuf[cnt*2]=MSB(G_PRESET_FM(G_CUR_BAND(), cnt));
			pbuf[cnt*2+1]=LSB(G_PRESET_FM(G_CUR_BAND(), cnt));
		}
		len = 2*FM_PRESET_NUM;
	} else {
		// use AM info
		for(cnt=0;cnt<AM_PRESET_NUM;cnt++)
		{
			pbuf[cnt*2]=MSB(G_PRESET_AM(G_CUR_BAND(), cnt));
			pbuf[cnt*2+1]=LSB(G_PRESET_AM(G_CUR_BAND(), cnt));
		}
		len = 2*AM_PRESET_NUM;
	}
	return len;
}

#if TUNER_MODEL==TUNER_NONE
#endif

