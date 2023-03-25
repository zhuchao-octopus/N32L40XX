
#ifndef _KEY_H_
#define _KEY_H_

#define IDLE_ADC_VALUE	0xFA

#define SWC_ADC_RETRY_VALUE	92

#define MAX_PANEL_KEY_NUM		48

#define KEY_SHORT_PRESS_TIME	T50MS_12
#define KEY_LONG_PRESS_TIME	T1S_12
#define KEY_REPEAT_PRESS_TIME	T200MS_12
#define KEY_FORCE_DVD_EJECT_TIME       T8S_12

typedef enum
{
	KEY_STUDY_SWC_PU_LARGE = 0,
	KEY_STUDY_SWC_PU_SMALL,
	KEY_STUDY_SWC_PU_TINY,
	KEY_STUDY_SWC_PU_NUMS
}KEY_STUDY_SWC_PU_TYPE;

typedef struct
{
	u8 adc_channel;
	u8 adc_value;
	u8 key_code_short;
	u8 key_code_long;
	KEY_STUDY_SWC_PU_TYPE swc_pu_type;
}KEY_INFO;

typedef enum
{
	KEY_STATE_NONE = 0,
	KEY_STATE_INIT,
	KEY_STATE_INIT_2,
	KEY_STATE_WORKING,
	KEY_STATE_ENTER_STUDY,
	KEY_STATE_STUDY_WAITING_KEY_PRESSED,
	KEY_STATE_STUDY_WAITING_KEY_CODE,
	KEY_STATE_EXIT_STUDY,
	KEY_STATE_NUMS
}KEY_STATE;

typedef enum
{
	KEY_STUDY_PANEL = 0,
	KEY_STUDY_SWC,
	KEY_STUDY_DEV_NUMS
}KEY_STUDY_DEVS;

typedef enum
{
	KEY_STUDY_ALREADY_EXIST = 0,
	KEY_STUDY_BEGIN,
	KEY_STUDY_FINISH,
	KEY_STUDY_STATUS_NUMS
}KEY_STUDY_STATUS;

typedef struct
{
	KEY_STATE status;
	u8 last_idx;
	u8 init_timer;
	u16 key_pressed_timer;
	u8 debounce_timer;
	KEY_STUDY_DEVS key_study_dev;
	KEY_STUDY_STATUS study_status;
	u8 study_index;
	u8 study_adc_ch;
	u8 study_adc_value;
	u8 study_pressed_timer;
	KEY_STUDY_SWC_PU_TYPE study_swc_pu_type;
	u8 swc_adc1_idle_large;
	u8 swc_adc1_idle_small;
	u8 swc_adc1_idle_tiny;
	u8 swc_adc2_idle_large;
	u8 swc_adc2_idle_small;
	u8 swc_adc2_idle_tiny;
	u8 recovery_key_timer;
	u16 reset_key_timer;
	u8 def_config;
	u8 swc_def_config;
	bool panel_adc_ch_inv;
}KEY_HANDLER;

typedef struct
{
	u8 magic_num0;
	u8 magic_num1;
	u8 magic_num2;
	u8 key_num;
	KEY_INFO key[MAX_PANEL_KEY_NUM];
} __attribute__((aligned(4))) KEY_INFO_STORE;

typedef struct
{
	u8 debounce_timer;
	u8 last_key_code;
	u8 last_adc_value;
}ENCODER_KEY_INFO;


typedef enum
{
	KEY_EVT_NONE = 0,
	KEY_EVT_ENTER_STUDY,
	KEY_EVT_EXIT_STUDY,
	KEY_EVT_QUERY_KEY_CODES,
	KEY_EVT_STUDY_KEY_CODE,
	KEY_EVT_CLEAR_KEYS,
	KEY_EVT_NUMS
}KEY_EVENTS;

#define KEY_DEFCFG_GENERIC		0x00
#define KEY_DEFCFG_ATENZA_UDLRE		0x01
#define KEY_DEFCFG_BENZ		0x02
#define KEY_DEFCFG_BENZ_W203		0x03
#define KEY_DEFCFG_BENZ_W211		0x04
#define KEY_DEFCFG_GENERIC_INV		0x05
#define KEY_DEFCFG_BENZ_INV		0x06
#define KEY_DEFCFG_ATENZA_UDLRE_INV		0x07

#define KEY_DEFCFG_SWC_GENERIC	0x80
#define KEY_DEFCFG_SWC_COROLLA	0x81

#define do_swc_pu(type) do {	\
	switch(type) {	\
		case KEY_STUDY_SWC_PU_LARGE:	\
			CAR_SWC_KEY_PULLUP_LARGE;	\
			break;						\
		case KEY_STUDY_SWC_PU_SMALL:	\
			CAR_SWC_KEY_PULLUP_SMALL;	\
			break;						\
		case KEY_STUDY_SWC_PU_TINY:	\
			CAR_SWC_KEY_PULLUP_TINY;	\
			break;						\
	}	\
} while(0);

#define toggle_swc_pu(type) ((type+1)%KEY_STUDY_SWC_PU_NUMS)

#define is_adc_value_equal(v1, v2) (MAX(v1,v2) -MIN(v1,v2) <= 5)
#define is_adc_value_equal_2(v1, v2) (MAX(v1,v2) -MIN(v1,v2) <= 30)


ext ENCODER_KEY_INFO g_vol_key_info;
ext ENCODER_KEY_INFO g_tune_key_info;
ext KEY_HANDLER g_key_handler;
ext KEY_INFO_STORE g_key_info_store;
ext bool g_disabed_encoder;

ext u8 g_nissan_xtrail_cam_sw_cntr;
ext bool g_nissan_xtrail_cam_sw_en;

extern void encoder_key_main(void);
extern void encoder2_key_main(void);
extern void panel_key_init(void);
extern void panel_key_main(void);
extern u8 panel_key_get_all_key_codes(KEY_STUDY_DEVS dev, u8 *pbuf);
extern u8 panel_key_get_cur_key_info(KEY_STUDY_DEVS dev, u8 index, u8 *pbuf);
extern void panel_key_set_defconfig(u8 cfg_id);
extern void nissan_xtrail_cam_sw_main(void);

#endif	//_KEY_H_

