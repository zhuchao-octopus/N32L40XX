
#ifndef _AUDIO_DRIVER_H_
#define _AUDIO_DRIVER_H_

#define MIN_VOLUME            0
#define MAX_VOLUME            30
#define DEFAULT_VOLUME        10

#define MIN_LOUDNESS           0
#define MAX_LOUDNESS           4
#define DEFAULT_LOUDNESS       0
#define MIN_SUBWOOFER_LVL           0
#if ASP_MODEL==ASP_BU32107
#define MAX_SUBWOOFER_LVL           15
#else
#define MAX_SUBWOOFER_LVL           14
#endif
#define DEFAULT_SUBWOOFER_LVL       9
#define MIN_FIELD_LEVEL           0
#define MAX_FIELD_LEVEL           14
#define DEFAULT_FIELD_LEVEL       7

#define MIN_EQ_LEVEL           0
#define MAX_EQ_LEVEL           20
#define DEFAULT_EQ_LEVEL       10

#define NAVI_BREAK_DIRECT		0xE0

#define SPECTRUM_NUMS	16

#define DSP_SPK_GAIN_MAX	12
#define DSP_SPK_GAIN_MIN	0

typedef enum
{
	AUDIO_SRC_NONE = 0,
	AUDIO_SRC_RADIO,
	AUDIO_SRC_HOST,
	AUDIO_SRC_BT_MODULE,
	AUDIO_SRC_DVD,
	AUDIO_SRC_AUXIN,
	AUDIO_SRC_TV,
	AUDIO_SRC_NUMS
}AUDIO_SOURCE;


#if ASP_MODEL==ASP_BU32107

typedef enum
{
	EQ_MODE_CUSTOM = 0,
	EQ_MODE_FLAT,
	EQ_MODE_JAZZ,
	EQ_MODE_POP,
	EQ_MODE_CLASSIC,
	EQ_MODE_ROCK,
	EQ_MODE_DBB,
	EQ_MODE_NUMS,
	EQ_MODE_CUSTOM_1 = 0x10,
	EQ_MODE_CUSTOM_2 = 0x11,
	EQ_MODE_CUSTOM_3 = 0x12,
	EQ_MODE_CUSTOM_4 = 0x13,
	EQ_MODE_CUSTOM_5 = 0x14,
	EQ_MODE_CUSTOM_6 = 0x15
}EQ_MODE;

typedef enum
{
	EQ_FREQ_80HZ=0,
	EQ_FREQ_125HZ,
	EQ_FREQ_200HZ_FAKE,
	EQ_FREQ_200HZ,
	EQ_FREQ_315HZ,
	EQ_FREQ_500HZ,
	EQ_FREQ_800HZ,
	EQ_FREQ_1K25HZ_FAKE,
	EQ_FREQ_1K25HZ,
	EQ_FREQ_2KHZ,
	EQ_FREQ_3K15HZ,
	EQ_FREQ_5KHZ,
	EQ_FREQ_8KHZ_FAKE,
	EQ_FREQ_8KHZ,
	EQ_FREQ_12K5HZ,

	EQ_FREQ_NUMS	
}EQ_FREQ;

#else

typedef enum
{
	EQ_MODE_CUSTOM = 0,
	EQ_MODE_FLAT,
	EQ_MODE_JAZZ,
	EQ_MODE_POP,
	EQ_MODE_CLASSIC,
	EQ_MODE_ROCK,
	EQ_MODE_NEWS,
	EQ_MODE_URBAN,
	EQ_MODE_TECHNO,
	EQ_MODE_FILM,
	EQ_MODE_MIDI,
	EQ_MODE_NUMS
}EQ_MODE;

typedef enum
{
	/* bass */
	EQ_FREQ_60HZ=0,
	EQ_FREQ_100HZ,
	EQ_FREQ_120HZ,
	/* middle */
	EQ_FREQ_0K5HZ,
	EQ_FREQ_1KHZ,
	EQ_FREQ_1K5HZ,
	/* treble */
	EQ_FREQ_10KHZ,
	EQ_FREQ_12K5HZ,
	EQ_FREQ_15KHZ,

	EQ_FREQ_NUMS	
}EQ_FREQ;

#endif


typedef enum
{
	AUDIO_MUTE_NONE = 0x00,
	AUDIO_MUTE_SYSTEM = 0x01,
	AUDIO_MUTE_BATT = 0x02,
	AUDIO_MUTE_DRIVER = 0x04,
	AUDIO_MUTE_USER = 0x08,
	AUDIO_MUTE_RADIO = 0x10,
	AUDIO_MUTE_TEMP = 0x20,
	AUDIO_MUTE_EXT_FORCE = 0x40
}AUDIO_MUTE_FLAG;

typedef enum
{
	AUDIO_STATE_PWR_DOWN = 0,
	AUDIO_STATE_PWR_OFF_ING,
	AUDIO_STATE_PWR_ON_ING,
	AUDIO_STATE_INIT_ING,
	AUDIO_STATE_IDLE,
	AUDIO_STATE_NUMS
}AUDIO_STATE;

typedef enum
{
	AUDIO_EVT_NONE,
	AUDIO_EVT_NAVI_EXTRA_GAIN,
	AUDIO_EVT_OUTPUT_TYPE,
	AUDIO_EVT_ADJ_CH_VOL,
	AUDIO_EVT_NUMS
}AUDIO_EVENT;

typedef enum
{
	AUDIO_SRC_SW_STATE_NONE = 0,
	AUDIO_SRC_SW_STATE_PREPARE,
	AUDIO_SRC_SW_STATE_READY,
	AUDIO_SRC_SW_STATE_NUMS
}AUDIO_SOURCE_SWITCH_STATE;

typedef enum
{
	AUDIO_OUTPUT_NORMAL = 0,
	AUDIO_OUTPUT_N_3DB,
	AUDIO_OUTPUT_N_6DB,
	AUDIO_OUTPUT_N_9DB,
	AUDIO_OUTPUT_N_12DB,
	AUDIO_OUTPUT_N_15DB,
	AUDIO_OUTPUT_TYPE_NUMS
}AUDIO_OUTPUT_TYPE;

typedef enum
{
	AUDIO_SF_MODE_ALL = 0,
	AUDIO_SF_MODE_DRIVER,
	AUDIO_SF_MODE_COPILOT,
	AUDIO_SF_MODE_RL,
	AUDIO_SF_MODE_RR,
	AUDIO_SF_MODE_USER,
	AUDIO_SF_MODE_NUMS
}AUDIO_SF_MODE;

typedef enum
{
	AUDIO_SR_MODE_FLAT = 0,
	AUDIO_SR_MODE_RECITAL,
	AUDIO_SR_MODE_CONCERT,
	AUDIO_SR_MODE_BGM,
	AUDIO_SR_MODE_MOVIE,
	AUDIO_SR_MODE_DRAMA,
	AUDIO_SR_MODE_NUMS
}AUDIO_SR_MODE;

typedef enum
{
	AUDIO_SPK_FL = 0,
	AUDIO_SPK_FR,
	AUDIO_SPK_RL,
	AUDIO_SPK_RR,
	AUDIO_SPK_NUMS
}AUDIO_SPK;

typedef struct
{
	AUDIO_STATE state;
	AUDIO_SOURCE sys_source;
	AUDIO_SOURCE cur_source;
	u8 cur_vol;
	u8 system_vol;
	u8 navi_mix_vol;
	s8 extra_input_gain_factory[AUDIO_SRC_NUMS];
	s8 extra_input_gain_user[AUDIO_SRC_NUMS];
	s8 extra_input_gain[AUDIO_SRC_NUMS];
	u8 mute;
	bool bt_phone_on;
	u8 bt_phone_vol;
	bool navi_break_on;
	bool navi_break_on_cache;
	/* navi_mix_extra_gain: */
	/* 0x00: means no navi mix, but break directly. */
	/* 0x01~0xFF: means navi mix, and the mix extra gain is  */
	/*   -127dB ~ 127dB, which 0x80 is 0dB, 0x7F is -1dB, 0x81 is +1dB and etc. */
	u8 navi_mix_extra_gain;	
	bool rds_ta_break_on;
	bool app_3rd_break_on;
	bool reverse_on;
	u8 fader;	/* 0->14: front->rear */
	u8 balance;	/* 0->14: left->right */
	u8 loudness;
	u8 subwoofer;
	u8 pwr_timer;
	u8 bt_phone_timer;
	u16 temp_mute_timer;	/* in ms unit */
	AUDIO_SOURCE_SWITCH_STATE src_sw_state;
	bool android_snd_on;

	bool bt_voice_on;
	u8 bt_voice_vol;
	u8 bt_voice_timer;
	bool carplay_phone_on;

	u8 output_type;
//	bool overheat;
	u8 ext_force_mute_timer;

	bool loud_on;
	bool hpf_on;
	u8 hpf_freq;
	bool dsp_bass_on;
	u8 dsp_bass_freq;
	u8 dsp_bass_gain;
	bool subwoofer_on;

	/* EQ member */
	EQ_MODE eq_mode;
	u8 eq_visible_level[EQ_FREQ_NUMS];
	u8 eq_custom_level[EQ_FREQ_NUMS];
	u8 eq_custom_level_1[EQ_FREQ_NUMS];
	u8 eq_custom_level_2[EQ_FREQ_NUMS];
	u8 eq_custom_level_3[EQ_FREQ_NUMS];
	u8 eq_custom_level_4[EQ_FREQ_NUMS];
	u8 eq_custom_level_5[EQ_FREQ_NUMS];
	u8 eq_custom_level_6[EQ_FREQ_NUMS];

	/* Spectrum analyzer */
	bool sa_en;
	u8 sa_timer;
	u8 sa_data[SPECTRUM_NUMS];

	bool phat_en;
	u8 phat_gain;
	bool core_en;
	u8 core_gain;
	bool space_en;
	u8 space_gain;
	AUDIO_SF_MODE sf_mode;	/* sound field mode */
	bool spk_on[AUDIO_SPK_NUMS];
	u8 spk_gain[AUDIO_SPK_NUMS];
	u8 spk_delay[AUDIO_SPK_NUMS];
	bool spk_user_on[AUDIO_SPK_NUMS];
	u8 spk_user_gain[AUDIO_SPK_NUMS];
	u8 spk_user_delay[AUDIO_SPK_NUMS];
	AUDIO_SR_MODE surround_mode;	/* surround mode */
	u8 soundfield_expert_mode;

	bool disabled_soundfield;
}AUDIO_INFO;

#define IS_VALID_VOLUME(vol)	(/*(vol>=MIN_VOLUME)&&*/(vol<=MAX_VOLUME))
#if ASP_MODEL==ASP_BU32107
#define IS_VALID_EQ_MODE(mode) ((mode<EQ_MODE_NUMS) || ((mode>=EQ_MODE_CUSTOM_1)&&(mode<=EQ_MODE_CUSTOM_6)))
#else
#define IS_VALID_EQ_MODE(mode) (mode<EQ_MODE_NUMS)
#endif
#define IS_VALID_EQ_FREQ(freq) (freq<EQ_FREQ_NUMS)
#define IS_VALID_FAD_BAL_LEVEL(lvl) ((lvl>=MIN_FIELD_LEVEL)&&(lvl<=MAX_FIELD_LEVEL))
#define IS_VALID_LOUDNESS_LEVEL(lvl) ((lvl>=MIN_LOUDNESS)&&(lvl<=MAX_LOUDNESS))
#define IS_VALID_SUBWOOFER_LEVEL(lvl) ((lvl>=MIN_SUBWOOFER_LVL)&&(lvl<=MAX_SUBWOOFER_LVL))

#define EQ3_U8_TO_S8(u, s)	do {		\
		s = (s8)((s8)u-(s8)DEFAULT_EQ_LEVEL);	\
	} while(0);

/***************************************
* our varible
**************************************/
ext AUDIO_INFO g_audio_info;	// do not write any member outside audio_driver.c, just read only.

/*******************************************************
*   interface for system
*******************************************************/
void audio_init(void);
void audio_main(void);
void audio_set_source(SOURCE av_src);
void audio_set_volume(u8 vol);
u8 audio_get_volume(void);
void audio_volume_up(void);
void audio_volume_down(void);
void audio_set_vol_ctrl_when_reverse(u8 val);
void audio_set_mute(AUDIO_MUTE_FLAG flag, bool mute);
void audio_set_mute_temporary(u16 time_ms);
void audio_set_bt_phone(bool on);
void audio_set_carplay_phone(bool on);
void audio_set_bt_voice(bool on);
void audio_set_navi_break(bool on);
void audio_set_rds_ta_break(bool on);
void audio_set_app_3rd_break(bool on);
void audio_set_android_sound_on(bool on);
void audio_set_eq_mode(EQ_MODE mode);
void audio_set_eq_custom_level(EQ_FREQ freq, u8 level);
void audio_save_eq_user_mode(u8 id);
void audio_set_fader(u8 level);
void audio_set_balance(u8 level);
void audio_set_loudness(u8 level);
void audio_set_subwoofer(u8 level);
void audio_set_navi_extra_gain(u8 gain);
void audio_set_pwr_ctrl(bool on);
void audio_set_output_type(u8 type);
void audio_adjust_channel_volume(void);
void audio_set_loud_on(bool on);
void audio_set_hpf(bool on, u8 freq);
void audio_set_dsp_bass(bool on, u8 freq, u8 gain);
void audio_set_subwoofer_on(bool on);
void audio_set_spectrum_analyzer(bool on);
void audio_set_dsp_phat(bool on, u8 gain);
void audio_set_dsp_core(bool on, u8 gain);
void audio_set_dsp_space(bool on, u8 gain);
void audio_set_dsp_soundfield(u8 mode);
void audio_set_dsp_speaker(u8 spk, bool on, u8 gain, u8 delay);
void audio_set_dsp_surround(u8 mode);
void audio_set_dsp_sf_expert_mode(u8 mode);
void audio_soundfield_ctrl(u8 disabled);



/*******************************************************
*   interface for eq
*******************************************************/
void audio_eq_init(void);
void audio_eq_update_mode(EQ_MODE mode);
void audio_eq_update_level(EQ_FREQ freq, u8 level);

/*******************************************************
*   interface for physical audio device
*******************************************************/
bool audio_dev_init(void);
void audio_dev_deinit(void);
void audio_dev_update_source(AUDIO_SOURCE src);
void audio_dev_update_volume(u8 vol);
void audio_dev_update_navi_mix(bool on);
void audio_dev_update_navi_mix_vol(u8 vol);
void audio_dev_update_eq(void);	// update EQ settings according to the AUDIO_INFO's eq_cur_freq & eq_cur_level member
void audio_dev_update_fader_balance(u8 fad, u8 bal);
void audio_dev_update_loudness(u8 loud);
void audio_dev_update_subwoofer(u8 subwoofer);
void audio_dev_update_dsp_settings_1(void);
void audio_dev_update_dsp_settings_2(void);
void audio_dev_update_spectrum_data(void);

#endif /* _AUDIO_DRIVER_H_ */

