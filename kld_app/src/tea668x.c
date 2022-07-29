
#include "public.h"

#if TUNER_MODEL==TUNER_NXP_TEA668X

#include "tea668x.h"

static TEA668X_STATE g_tea668x_state = TEA668X_STATE_UNKNOWN;
static u16 g_tea668x_patch_data_size = 0;
static u16 g_tea668x_patch_data_sent = 0;
static RADIO_BAND g_band = RADIO_BAND_FM;
static u16 g_wr_buf[8];
static u16 g_rd_buf[16];
static u8 g_counter;
static RADIO_DEV_SEEK_MODE g_tea668x_seek_mode = RDSM_DX;

static bool __tea668x_write(u8 module, u8 cmd, u8 index, const u16* param, u16 param_len)
{
   	u16 i;
	u8 value;

	i2c_start();
	i2c_send_byte(TEA668X_I2C_ADDR_W);
	i2c_wait_ack();
	i2c_send_byte(module);
	i2c_wait_ack();
	i2c_send_byte(cmd);
	i2c_wait_ack();
	i2c_send_byte(index);
	i2c_wait_ack();
   	for(i=0;i<param_len;i++)
    {   
		value = (param[i]>>8)&0xFF;
    	i2c_send_byte(value);
		i2c_wait_ack();
		value = (param[i])&0xFF;
    	i2c_send_byte(value);
		i2c_wait_ack();
    } 
	i2c_stop();

	return TRUE;
}

static bool __tea668x_read(u8 module, u8 cmd, u8 index, u16* param, u8 param_len)
{
	u8 i;
	u8 value;

	i2c_start();
	i2c_send_byte(TEA668X_I2C_ADDR_W);
	i2c_wait_ack();
	i2c_send_byte(module);
	i2c_wait_ack();
	i2c_send_byte(cmd);
	i2c_wait_ack();
	i2c_send_byte(index);
	i2c_wait_ack();
 	i2c_stop();

	delay_us(20); // do a delay to make sure data available
	
	i2c_start();
	i2c_send_byte(TEA668X_I2C_ADDR_R);
	i2c_wait_ack();
   	for(i=0;i<param_len;i++)
    {
		value = i2c_read_byte(1);
		param[i] = value<<8;
		
		if (i==(param_len-1)) {
			value = i2c_read_byte(0);
		} else {
			value = i2c_read_byte(1);
		}
		param[i] |= value;
    } 
	i2c_stop();

	return TRUE;
}

static void _do_boot_init(void)
{
	u16 to_send;

	if (0==g_tea668x_patch_data_sent) {
		// first send, send the 0x1C data first
		__tea668x_write(TEA668X_MODULE_INIT_CMD, 0x00, 0x00, 0, 0);
		__tea668x_write(TEA668X_MODULE_INIT_CMD, 0x00, 0x74, 0, 0);
	}

	if (g_tea668x_patch_data_size>g_tea668x_patch_data_sent) {
		to_send = g_tea668x_patch_data_size-g_tea668x_patch_data_sent;
		if (to_send>TEA668X_PATCH_DATA_SINGLE_SEND) {
			to_send = TEA668X_PATCH_DATA_SINGLE_SEND;
		}
		__tea668x_write(TEA668X_MODULE_INIT_DATA, 
			(g_tea668x_patch[g_tea668x_patch_data_sent]>>8)&0xFF,
			g_tea668x_patch[g_tea668x_patch_data_sent]&0xFF,
			&g_tea668x_patch[g_tea668x_patch_data_sent+1],
			to_send-1);
		g_tea668x_patch_data_sent += to_send;
	}

	if (g_tea668x_patch_data_sent==g_tea668x_patch_data_size) {
		// finish send patch data, send the LUT data .
		to_send = sizeof(g_tea668x_lut)/sizeof(u16);
		__tea668x_write(TEA668X_MODULE_INIT_CMD, 0x00, 0x00, 0, 0);
		__tea668x_write(TEA668X_MODULE_INIT_CMD, 0x00, 0x75, 0, 0);
		__tea668x_write(TEA668X_MODULE_INIT_DATA, 
			(g_tea668x_lut[0]>>8)&0xFF,
			g_tea668x_lut[0]&0xFF,
			&g_tea668x_lut[1],
			to_send-1);
		__tea668x_write(TEA668X_MODULE_INIT_CMD, 0x00, 0x00, 0, 0);

		// Start: [ w  14  0001 ]
		__tea668x_write(TEA668X_MODULE_START, 0x00, 0x01, 0, 0);

		// entering IDLE
		g_tea668x_state = TEA668X_STATE_ENTERING_IDLE;
	}
}

static void _do_idle_init(void)
{
	u16 value[4];

	/* Set reference frequency */
#if TEA668X_REF_FREQ == 9216000
	g_wr_buf[0]=0x008C;
	g_wr_buf[1]=0xA000;
	g_wr_buf[2]=0x0000;
	__tea668x_write(TEA668X_MODULE_APPL, TEA668X_CMD_SET_REF_CLK, 0x01, g_wr_buf, 3);
#elif TEA668X_REF_FREQ == 4000000
	/* APPL_Set_ReferenceClock freq. = 9216000/4000000, type = 0. 
	  [ w  40 04 01  003D 0900 0000 ] : 4 MHz crystal reference. */
	g_wr_buf[0]=0x003D;
	g_wr_buf[1]=0x0900;
	g_wr_buf[2]=0x0000;
	__tea668x_write(TEA668X_MODULE_APPL, TEA668X_CMD_SET_REF_CLK, 0x01, g_wr_buf, 3);
#else
#error "TEA668X reference freq error!"
#endif

	/* Get Identification */
	__tea668x_read(TEA668X_MODULE_APPL, TEA668X_CMD_GET_ID, 1, value, 3);
	g_tea668x_version = ( (value[1]>>8)<<8 ) | (value[2]>>8);

	/* Activate : APPL_Activate mode = 1. 
	    [ w  40 05 01  0001 ] */
	g_wr_buf[0]=0x0001;
	__tea668x_write(TEA668X_MODULE_APPL, TEA668X_CMD_ACTIVATE, 0x01, g_wr_buf, 1);
	
	// entering ACTIVATE
	g_tea668x_state = TEA668X_STATE_ENTERING_ACTIVATE;
}

static void _do_activate_init(void)
{
	// do some user setting
	
	// set to FM 
	g_wr_buf[0]=0x0001;
	g_wr_buf[1]=0x2652;
	__tea668x_write(TEA668X_MODULE_FM, TEA668X_CMD_SET_TUNE_TO, 0x01, g_wr_buf, 2);

	// set bandwidth
	g_wr_buf[0]=0x0001;
	g_wr_buf[1]=0x0938;
	g_wr_buf[2]=0x04B0;
	g_wr_buf[3]=0x03E8;
	g_wr_buf[4]=0x0230;
	g_wr_buf[5]=0x0938;
	g_wr_buf[6]=0x012C;
	__tea668x_write(TEA668X_MODULE_FM, 0x0A, 0x01, g_wr_buf, 7);

	// set channel eq
	g_wr_buf[0]=0x0001;
	__tea668x_write(TEA668X_MODULE_FM, 0x16, 0x01, g_wr_buf, 1);

	// set multipath suppression on
	g_wr_buf[0]=0x0001;
	__tea668x_write(TEA668X_MODULE_FM, 0x14, 0x01, g_wr_buf, 1);

	// set RF AGC
	g_wr_buf[0]=0x0360;
	g_wr_buf[1]=0x0000;
	__tea668x_write(TEA668X_MODULE_FM, 0x0B, 0x01, g_wr_buf, 2);

	// set level offset
	g_wr_buf[0]=0xFF9C;
	__tea668x_write(TEA668X_MODULE_FM, 0x27, 0x01, g_wr_buf, 1);

	// set softmute 
	g_wr_buf[0]=0x0002;
	g_wr_buf[1]=0x001E;
	g_wr_buf[2]=0x00F0;
	__tea668x_write(TEA668X_MODULE_FM, 0x2A, 0x01, g_wr_buf, 3);
	
	g_wr_buf[0]=0x0078;
	g_wr_buf[1]=0x01F4;
	g_wr_buf[2]=0x000A;
	g_wr_buf[3]=0x0014;
	__tea668x_write(TEA668X_MODULE_FM, 0x28, 0x01, g_wr_buf, 4);
	
	g_wr_buf[0]=0x0000;
	g_wr_buf[1]=0x0294;
	g_wr_buf[2]=0x03E8;
	__tea668x_write(TEA668X_MODULE_FM, 0x2C, 0x01, g_wr_buf, 3);
	
	g_wr_buf[0]=0x0000;
	g_wr_buf[1]=0x0294;
	g_wr_buf[2]=0x03E8;
	__tea668x_write(TEA668X_MODULE_FM, 0x2B, 0x01, g_wr_buf, 3);

	// set highcut
	g_wr_buf[0]=0x00C8;
	g_wr_buf[1]=0x07D0;
	g_wr_buf[2]=0x000A;
	g_wr_buf[3]=0x0050;
	__tea668x_write(TEA668X_MODULE_FM, 0x32, 0x01, g_wr_buf, 4);
	
	g_wr_buf[0]=0x0003;
	g_wr_buf[1]=0x0078;
	g_wr_buf[2]=0x00A0;
	__tea668x_write(TEA668X_MODULE_FM, 0x36, 0x01, g_wr_buf, 3);
	
	g_wr_buf[0]=0x0003;
	g_wr_buf[1]=0x0096;
	g_wr_buf[2]=0x00C8;
	__tea668x_write(TEA668X_MODULE_FM, 0x35, 0x01, g_wr_buf, 3);
	
	g_wr_buf[0]=0x0001;
	g_wr_buf[1]=0x0960;
	__tea668x_write(TEA668X_MODULE_FM, 0x37, 0x01, g_wr_buf, 2);

	// set lowcut
	g_wr_buf[0]=0x0001;
	g_wr_buf[1]=0x0064;
	__tea668x_write(TEA668X_MODULE_FM, 0x39, 0x01, g_wr_buf, 2);

	// set stereo 
	g_wr_buf[0]=0x0003;
	g_wr_buf[1]=0x0154;
	g_wr_buf[2]=0x00A0;
	__tea668x_write(TEA668X_MODULE_FM, 0x3E, 0x01, g_wr_buf, 3);
	
	g_wr_buf[0]=0x00C8;
	g_wr_buf[1]=0x0FA0;
	g_wr_buf[2]=0x0014;
	g_wr_buf[3]=0x0050;
	__tea668x_write(TEA668X_MODULE_FM, 0x3C, 0x01, g_wr_buf, 4);
	
	g_wr_buf[0]=0x0003;
	g_wr_buf[1]=0x0064;
	g_wr_buf[2]=0x00BE;
	__tea668x_write(TEA668X_MODULE_FM, 0x40, 0x01, g_wr_buf, 3);
	
	g_wr_buf[0]=0x0003;
	g_wr_buf[1]=0x0064;
	g_wr_buf[2]=0x00BE;
	__tea668x_write(TEA668X_MODULE_FM, 0x3F, 0x01, g_wr_buf, 3);
	
	g_wr_buf[0]=0x0001;
	g_wr_buf[1]=0x0190;
	__tea668x_write(TEA668X_MODULE_FM, 0x42, 0x01, g_wr_buf, 2);

	// set StHiBlend
	g_wr_buf[0]=0x0003;
	g_wr_buf[1]=0x01A4;
	g_wr_buf[2]=0x00A0;
	__tea668x_write(TEA668X_MODULE_FM, 0x48, 0x01, g_wr_buf, 3);
	
	g_wr_buf[0]=0x01F4;
	g_wr_buf[1]=0x07D0;
	g_wr_buf[2]=0x0014;
	g_wr_buf[3]=0x0050;
	__tea668x_write(TEA668X_MODULE_FM, 0x46, 0x01, g_wr_buf, 4);
	
	g_wr_buf[0]=0x0000;
	g_wr_buf[1]=0x00D2;
	g_wr_buf[2]=0x005A;
	g_wr_buf[3]=0x01F4;
	__tea668x_write(TEA668X_MODULE_FM, 0x47, 0x01, g_wr_buf, 4);
	
	g_wr_buf[0]=0x0003;
	g_wr_buf[1]=0x0064;
	g_wr_buf[2]=0x00BE;
	__tea668x_write(TEA668X_MODULE_FM, 0x4A, 0x01, g_wr_buf, 3);
	
	g_wr_buf[0]=0x0003;
	g_wr_buf[1]=0x0064;
	g_wr_buf[2]=0x00BE;
	__tea668x_write(TEA668X_MODULE_FM, 0x49, 0x01, g_wr_buf, 3);

	// set GPIO
	g_wr_buf[0]=0x0000;
	g_wr_buf[1]=0x0021;
	g_wr_buf[2]=0x0003;
	__tea668x_write(TEA668X_MODULE_APPL, 0x03, 0x01, g_wr_buf, 3);
	g_wr_buf[0]=0x0001;
	g_wr_buf[1]=0x0021;
	g_wr_buf[2]=0x0000;
	__tea668x_write(TEA668X_MODULE_APPL, 0x03, 0x01, g_wr_buf, 3);
	g_wr_buf[0]=0x0002;
	g_wr_buf[1]=0x0021;
	g_wr_buf[2]=0x0000;
	__tea668x_write(TEA668X_MODULE_APPL, 0x03, 0x01, g_wr_buf, 3);
	g_wr_buf[0]=0x0000;
	g_wr_buf[1]=0x0020;
	g_wr_buf[2]=0x0003;
	__tea668x_write(TEA668X_MODULE_APPL, 0x03, 0x01, g_wr_buf, 3);
	g_wr_buf[0]=0x0001;
	g_wr_buf[1]=0x0020;
	g_wr_buf[2]=0x0000;
	__tea668x_write(TEA668X_MODULE_APPL, 0x03, 0x01, g_wr_buf, 3);
	g_wr_buf[0]=0x0002;
	g_wr_buf[1]=0x0020;
	g_wr_buf[2]=0x0000;
	__tea668x_write(TEA668X_MODULE_APPL, 0x03, 0x01, g_wr_buf, 3);

	// set digital IO
	g_wr_buf[0]=0x0020;
	g_wr_buf[1]=0x0000;
	g_wr_buf[2]=0x0020;
	g_wr_buf[3]=0x0000;
	g_wr_buf[4]=0x113A;
	__tea668x_write(TEA668X_MODULE_AUDIO, 0x16, 0x01, g_wr_buf, 5);
	g_wr_buf[0]=0x0021;
	g_wr_buf[1]=0x0000;
	g_wr_buf[2]=0x0020;
	g_wr_buf[3]=0x0000;
	g_wr_buf[4]=0x113A;
	__tea668x_write(TEA668X_MODULE_AUDIO, 0x16, 0x01, g_wr_buf, 5);

	// set output source
	g_wr_buf[0]=0x0080;
	g_wr_buf[1]=0x00E0;
	__tea668x_write(TEA668X_MODULE_AUDIO, 0x0D, 0x01, g_wr_buf, 2);
		

	// if you wanna do some audio test, uncomment the following codes
#if 0
	{
		u16 tmp[8];
		tmp[0]=0x0003;	// L: wave1, R:wave2
		tmp[1]=0x0000;	// no offset
		tmp[2]=0x0000;	//0dB
		tmp[3]=1000;		// 1KHZ
		tmp[4]=0x0000;	//0dB
		tmp[5]=1000;		// 1KHZ
		__tea668x_write(TEA668X_MODULE_AUDIO, TEA668X_CMD_SET_WAVEGEN, 0x01, tmp, 6);

		tmp[0]=240;	//sine wave generator
		__tea668x_write(TEA668X_MODULE_AUDIO, TEA668X_CMD_SET_INPUT, 0x01, tmp, 1);
	}
#endif

	//  AUDIO_Set_Mute mode = 0 : disable mute. [ w  30 0B 01  0000 ] 
	g_wr_buf[0]=0x0000;
	__tea668x_write(TEA668X_MODULE_AUDIO, TEA668X_CMD_SET_MUTE, 0x01, g_wr_buf, 1);
	
	// enter NORMAL
	g_tea668x_state = TEA668X_STATE_NORMAL;
}

bool radio_dev_init(void) 
{
	u16 value=0xFFFF;

	g_band = RADIO_BAND_FM;

	if (TEA668X_STATE_NORMAL==g_tea668x_state) {
		return TRUE;
	}

	// get 668X state
	__tea668x_read(TEA668X_MODULE_APPL, TEA668X_CMD_GET_OP_STATUS, 1, &value, 1);
	
	// update state
	switch (g_tea668x_state) {
		case TEA668X_STATE_UNKNOWN:
			/* Repeat APPL_Get_Operation_Status read until status = 0 ('boot state') is found.  */
			if (0==value) {
				g_tea668x_patch_data_sent = 0;
				g_tea668x_patch_data_size = sizeof(g_tea668x_patch)/sizeof(u16);
				g_tea668x_state=TEA668X_STATE_BOOT;
			} else if ((2==value)||(3==value)||(4==value)) {
				g_tea668x_state=TEA668X_STATE_ACTIVATE;
			}
			break;
		case TEA668X_STATE_BOOT:
			_do_boot_init();
			break;
		case TEA668X_STATE_ENTERING_IDLE:
			/* Repeat APPL_Get_Operation_Status read until status = 1 ('idle state') is found. */
			if (1==value)
				g_tea668x_state=TEA668X_STATE_IDLE;
			break;
		case TEA668X_STATE_IDLE:
			_do_idle_init();
			break;
		case TEA668X_STATE_ENTERING_ACTIVATE:
			/* Repeat APPL_Get_Operation_Status read until status = 2 ('radio standby state') is found. */
			if (2==value)
				g_tea668x_state=TEA668X_STATE_ACTIVATE;
			break;
		case TEA668X_STATE_ACTIVATE:
			_do_activate_init();
			break;
		default:
			break;
	}
	return FALSE;
}

void radio_dev_rds_init(void)
{
}

bool radio_dev_deinit(void) 
{
	g_tea668x_state = TEA668X_STATE_UNKNOWN;
	return TRUE;
}
bool radio_dev_is_set_freq_done(void)
{
	return TRUE;
}
void radio_dev_set_freq(RADIO_BAND band, u16 freq)
{
	g_band = band;

	if (TEA668X_STATE_NORMAL != g_tea668x_state)
		return ;

	g_wr_buf[0] = 1;	// TUNING_MODE_PRESET
	g_wr_buf[1] = freq;
	if (RADIO_BAND_FM == band) {
		__tea668x_write(TEA668X_MODULE_FM, TEA668X_CMD_SET_TUNE_TO, 0x01, g_wr_buf, 2);
	} else {
		__tea668x_write(TEA668X_MODULE_AM, TEA668X_CMD_SET_TUNE_TO, 0x01, g_wr_buf, 2);
	}

	g_counter = Counter_4ms;
}
void radio_dev_set_freq_tune(RADIO_BAND band, u16 freq)
{
	radio_dev_set_freq(band, freq);
}
bool radio_dev_is_tune_ok(bool strict)
{
	u8 module;
	s16 value;
	u8 cnt=0;
	s16 level = 0;
	u16 usn = 0;
	u16 wam = 0;
	u16 offset = 0;
	u16 bw = 0;

	if (TEA668X_STATE_NORMAL != g_tea668x_state)
		return FALSE;

	if (RADIO_BAND_FM == g_band) {
		module = TEA668X_MODULE_FM;
	} else {
		module = TEA668X_MODULE_AM;
	}

	for (cnt = 0; cnt<3; cnt++) {
		// Read status and all available data
		__tea668x_read(module, TEA668X_CMD_GET_QUALITY_DATA, 1, g_rd_buf, 7);
		if (1000!=g_rd_buf[0]) {
			// quality data unalible now 
			return FALSE;
		}

		value = TEA668X_U16_TO_S16(g_rd_buf[1]);
		level += value;
		value = g_rd_buf[2];
		usn += value;
		value = g_rd_buf[3];
		wam += value;
		value = TEA668X_U16_TO_S16(g_rd_buf[4]);
		if (value<0)
			value = 0-value;
		offset += value;
		value = g_rd_buf[5];
		bw += value;

		if (strict) {
			delay_1ms(1);
		} else {
			break;
		}
	}

	if (strict) {
		level /=3;
		usn /= 3;
		wam /= 3;
		offset /= 3;
		bw /= 3;
	}

	if (RADIO_BAND_FM == g_band) {
		if (RDSM_DX == g_tea668x_seek_mode) {
			if (level < TEA668X_FM_LEVEL_OK) {
				return FALSE;
			}
		} else {
			if (level < TEA668X_FM_LOC_LEVEL_OK) {
				return FALSE;
			}
		}
	} else {
		if (level < TEA668X_AM_LEVEL_OK) {
			return FALSE;
		}
	}
	if (RADIO_BAND_FM == g_band) {
		// check USN
		if (usn > TEA668X_FM_USN_OK) {
			return FALSE;
		}
#if 0
		// check WAM
		if (wam > TEA668X_FM_WAM_OK) {
			return FALSE;
		}
#endif
	}

	// check freq offset
	if (RADIO_BAND_FM == g_band) {
		if (offset > TEA668X_FM_OFFSET_OK) {
			return FALSE;
		}
	} else {
		if (offset > TEA668X_AM_OFFSET_OK) {
			return FALSE;
		}
	}
#if 1
	// check bandwidth for FM
	if (RADIO_BAND_FM == g_band) {
		if (bw < TEA668X_FM_BANDWIDTH_OK) {
			return FALSE;
		}
	}
#endif
	return TRUE;
}
bool radio_dev_is_tune_status_ready(void)
{
#if 1
	u16 tmp;
	// consider status is ready more than 75mS after set freq
	if (Counter_4ms < g_counter) {
		tmp = (u16)(256-g_counter);
		tmp += Counter_4ms;
		if (tmp > T100MS_4) {
			return TRUE;
		} else {
			return FALSE;
		}
	} else {
		if ((Counter_4ms - g_counter) > T72MS_4) {
			return TRUE;
		} else {
			return FALSE;
		}
	}
#else
	u16 value;

	if (TEA668X_STATE_NORMAL != g_tea668x_state)
		return FALSE;

	if (RADIO_BAND_FM == g_band) {
		__tea668x_read(TEA668X_MODULE_FM, TEA668X_CMD_GET_QUALITY_STATUS, 1, &value, 1);
	} else {
		__tea668x_read(TEA668X_MODULE_AM, TEA668X_CMD_GET_QUALITY_STATUS, 1, &value, 1);
	}
	// we consider the data is reliable after 32ms after tuning.
	return ((value&0x3FF)>320);
#endif
}
u8 radio_dev_get_stereo_status(void)
{
	u16 value;

	if (TEA668X_STATE_NORMAL != g_tea668x_state)
		return 1;

	if (RADIO_BAND_FM != g_band) {
		return 1;
	}

	__tea668x_read(TEA668X_MODULE_FM, TEA668X_CMD_GET_SIGNAL_STATUS, 1, &value, 1);
	if (value&0x8000) {
		return 2;
	} else {
		return 1;
	}
}

bool radio_dev_set_seek_mode(RADIO_DEV_SEEK_MODE mode)
{
	if ((RDSM_DX==mode) || (RDSM_LOC==mode)) {
		g_tea668x_seek_mode = mode;
	}
	return TRUE;
}

bool radio_dev_set_st_mode(bool on)
{
	if (FALSE == on) {
		g_wr_buf[0]=0x0002;
		__tea668x_write(TEA668X_MODULE_FM, TEA668X_CMD_SET_STEREO_MIN, 0x01, g_wr_buf, 1);
	} else {
		g_wr_buf[0]=0x0001;
		g_wr_buf[1]=0x0190;
		__tea668x_write(TEA668X_MODULE_FM, TEA668X_CMD_SET_STEREO_MIN, 0x01, g_wr_buf, 2);
	}
	return TRUE;
}

bool radio_dev_set_mute(bool mute)
{
	if (mute)
		g_wr_buf[0]=0x0001;
	else
		g_wr_buf[0]=0x0000;
	__tea668x_write(TEA668X_MODULE_AUDIO, TEA668X_CMD_SET_MUTE, 0x01, g_wr_buf, 1);

	return TRUE;
}

#if RDS_FUNCTION==1
bool radio_dev_rds_update(void)
{
	u8 bleA, bleB, bleC, bleD;
	u8 group_type;
	u8 addr;

	// check RDS status
	__tea668x_read(TEA668X_MODULE_FM, TEA668X_CMD_GET_RDS_STATUS, 1, g_rd_buf, 1);
	if (0==(g_rd_buf[0]&(1<<15))) {
		return FALSE;
	}
	
	// Get the RDS data
	__tea668x_read(TEA668X_MODULE_FM, TEA668X_CMD_GET_RDS_DATA, 1, g_rd_buf, 6);
	if (0==(g_rd_buf[0]&(1<<15))) {
		return FALSE;
	}

	bleA = (g_rd_buf[5] & 0xC000) >> 14;
	bleB = (g_rd_buf[5] & 0x3000) >> 12;
	bleC = (g_rd_buf[5] & 0x0C00) >> 10;
	bleD = (g_rd_buf[5] & 0x0300) >> 8;

	// check RDS Block B Corrected Errors
	// It's critical that block B be correct since it determines what's
	// contained in the latter blocks. For this reason, a stricter tolerance
	// is placed on block B
	if (CORRECTED_SMALL < bleB) {
		// Drop the data if more than two errors were uncorrected in block B
		return FALSE;
	}

	g_rds_info.block_a = g_rd_buf[1];
	g_rds_info.block_b = g_rd_buf[2];
	g_rds_info.block_c = g_rd_buf[3];
	g_rds_info.block_d = g_rd_buf[4];

	if (CORRECTED_LARGE >= bleA) {
		rds_update_pi(g_rds_info.block_a);
	}

	group_type = g_rds_info.block_b >> 11;

	// Update pi code.  Version B formats always have the pi code in words A and C
	if (group_type & 0x01) {
		if (CORRECTED_LARGE >= bleC) {
			rds_update_pi(g_rds_info.block_c);
		}
	}

	rds_update_pty((g_rds_info.block_b >> 5) & 0x1f);
	rds_update_tp((bool)(0!=(g_rds_info.block_b & (1<<10))));

	switch (group_type) 
	{
		case RDS_TYPE_0A:
			if (CORRECTED_LARGE >= bleC) {
				rds_update_af_list(g_rds_info.block_c);
			}
			// fallthrough
		case RDS_TYPE_0B:
			rds_update_ta((bool)(0!=(g_rds_info.block_b & (1<<4))));
			addr = (g_rds_info.block_b & 0x3) * 2;
			if (CORRECTED_LARGE >= bleD) {
				rds_update_ps(addr+0, g_rds_info.block_d >> 8  );
				rds_update_ps(addr+1, g_rds_info.block_d & 0xff);
			}
			break;

		case RDS_TYPE_2A:
			if ((CORRECTED_LARGE>=bleD) && (CORRECTED_LARGE>=bleC)) {
				addr = g_rds_info.block_b&0x0F;
				rds_update_rt(addr, 4, 
					(g_rds_info.block_b&0x10)>>4, 
					g_rds_info.block_c>>8,
					g_rds_info.block_c&0xFF,
					g_rds_info.block_d>>8,
					g_rds_info.block_d&0xFF);
			}
			break;

		case RDS_TYPE_2B:
			if (CORRECTED_LARGE>=bleD) {
				addr = g_rds_info.block_b&0x0F;
				rds_update_rt(addr, 2, 
					(g_rds_info.block_b&0x10)>>4, 
					g_rds_info.block_d>>8,
					g_rds_info.block_d&0xFF,
					0,
					0);
			}
			break;

		case RDS_TYPE_4A:
			// TODO : CT 
			break;
		default:
			break;
	}

	return FALSE;
}
#endif

#endif

