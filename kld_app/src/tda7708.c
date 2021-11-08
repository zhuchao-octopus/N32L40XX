
#include "public.h"

#if TUNER_MODEL==TUNER_ST_TDA7708

#include "TDA7708LX_OM_v2.6.0_boot.h"
#include "tda7708.h"

static u8 g_tda7708_addr[3];
static u8 g_tda7708_data[TDA7708_I2C_DATA_BUFF_MAX_NUM];
static TDA7708_STATUS g_tda7708_status = TDA7708_STATUS_NONE;

static RADIO_BAND g_tda7708_band;
static RADIO_DEV_SEEK_MODE g_tda7708_seek_mode = RDSM_DX;

static u16 s_addr_h = 0;
static u16 s_addr_l = 0;
static u16 s_byteNum = 0;
static u16 s_startNum = 0;
static u16 s_cnt = 0;

static u8 g_tda7708_timer = 0;

static void tda7708_i2c_read(u8 len)
{
	u8 cnt;
	
	i2c_start();
	i2c_send_byte(TDA7708_I2C_ADDR_W);
	i2c_wait_ack();
	i2c_send_byte(g_tda7708_addr[0]);
	i2c_wait_ack();
	i2c_send_byte(g_tda7708_addr[1]);
	i2c_wait_ack();
	i2c_send_byte(g_tda7708_addr[2]);
	i2c_wait_ack();
 	i2c_stop();

	delay_us(50);

	i2c_start();
 	i2c_send_byte(TDA7708_I2C_ADDR_R);
	i2c_wait_ack();
	delay_us(20);
	for(cnt=0; cnt<len; cnt++) {
		if (cnt==(len-1)) {
			g_tda7708_data[cnt] = i2c_read_byte(0);
		} else {
			g_tda7708_data[cnt] = i2c_read_byte(1);
		}
	}
 	i2c_stop();

}

static void tda7708_i2c_read_param(u8 len)
{
	u8 cnt;

	if(0==len)
		return;
	
	i2c_start();
 	i2c_send_byte(TDA7708_I2C_ADDR_R);
	i2c_wait_ack();
	delay_us(20);
	for(cnt=0; cnt<len; cnt++) {
		if (cnt==(len-1)) {
			g_tda7708_data[3+cnt] = i2c_read_byte(0);
		} else {
			g_tda7708_data[3+cnt] = i2c_read_byte(1);
		}
	}
 	i2c_stop();

}

static void tda7708_i2c_write(u8 len)
{
	u8 cnt;
	
	i2c_start();
	i2c_send_byte(TDA7708_I2C_ADDR_W);
	i2c_wait_ack();
	i2c_send_byte(g_tda7708_addr[0]);
	i2c_wait_ack();
	i2c_send_byte(g_tda7708_addr[1]);
	i2c_wait_ack();
	i2c_send_byte(g_tda7708_addr[2]);
	i2c_wait_ack();

	for(cnt=0; cnt<len; cnt++) {
		i2c_send_byte(g_tda7708_data[cnt]);
		i2c_wait_ack();
	}

 	i2c_stop();
}

static void tda7708_i2c_cmd(u8 cmd_id, u8 cmd_param_len, u8 answer_param_len)
{
	u16 checksum_h;
	u16 checksum_l;
	u16 tmp;
	u8 cnt;

	g_tda7708_addr[0] = 0xF2;
	g_tda7708_addr[1] = 0x01;
	g_tda7708_addr[2] = 0x80;

	g_tda7708_data[TDA7708_CMD_HEAD_POS_H] = 0x00;
	g_tda7708_data[TDA7708_CMD_HEAD_POS_M] = cmd_id;
	g_tda7708_data[TDA7708_CMD_HEAD_POS_L] = cmd_param_len;

	checksum_h = 0;
	checksum_l = 0;

	for (cnt=0; cnt<(3+cmd_param_len*3); cnt+=3) {
		checksum_h += g_tda7708_data[cnt];
		
		tmp = ( (g_tda7708_data[cnt+1]<<8) | g_tda7708_data[cnt+2] );
		U32_ADD(checksum_h, checksum_l, tmp);
	}

	g_tda7708_data[cnt] = checksum_h&0xFF;
	g_tda7708_data[cnt+1] = (checksum_l>>8)&0xFF;
	g_tda7708_data[cnt+2] = (checksum_l>>0)&0xFF;

	tda7708_i2c_write(cnt+3);

	delay_us(90);

	// read the answer
	g_tda7708_addr[0] = 0x72;
	if (0xFF==answer_param_len) {
		tda7708_i2c_read(3);
		cnt = g_tda7708_data[2];
		tda7708_i2c_read_param(cnt*3);
	} else {
		tda7708_i2c_read(3+answer_param_len*3);
	}
}

void tda7708_switch_band(RADIO_BAND band)
{
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_H] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_L] = 0x01;
	g_tda7708_data[TDA7708_CMD_PARAM2_POS_H] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM2_POS_M] = 0x00;
	if (RADIO_BAND_FM==band) {
		g_tda7708_data[TDA7708_CMD_PARAM2_POS_L] = 0x01;
	} else {
		g_tda7708_data[TDA7708_CMD_PARAM2_POS_L] = 0x05;
	}
	g_tda7708_data[TDA7708_CMD_PARAM3_POS_H] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM3_POS_M] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM3_POS_L] = 0x00;
	tda7708_i2c_cmd(CMD_CODE_TUNER_CHANGE_BAND, 3, 0);
}

bool radio_dev_init(void) 
{
#if 1
	u8 sendByteNum, j;
	u8 tmp;

	switch (g_tda7708_status) {
		case TDA7708_STATUS_NONE:
			g_tda7708_status = TDA7708_STATUS_DOWNLOADING;
			// fall through

		case TDA7708_STATUS_DOWNLOADING:

			if(s_byteNum == 0)
			{
				if ((CMOST_Firmware[s_cnt] != 0xFF) || (CMOST_Firmware[s_cnt + 1] != 0xFF) || (CMOST_Firmware[s_cnt + 2] != 0xFF))
				{
					s_byteNum = CMOST_Firmware[s_cnt + 3] * 256 + CMOST_Firmware[s_cnt + 4];   //bytes number (data)      
					s_addr_h = CMOST_Firmware[s_cnt++];
					s_addr_l = CMOST_Firmware[s_cnt++];
					s_addr_l <<= 8;
					s_addr_l |= CMOST_Firmware[s_cnt++];
					s_startNum = s_cnt;
				}
				else
				{
					g_tda7708_status = TDA7708_STATUS_WAIT_INIT;
				}
			}

			if(s_byteNum != 0)
			{
				if(s_byteNum > TDA7708_I2C_DATA_BUFF_MAX_NUM)
					sendByteNum = TDA7708_I2C_DATA_BUFF_MAX_NUM;
				else
					sendByteNum = s_byteNum;

				g_tda7708_addr[0] = (unsigned char)(s_addr_h & 0xff)| MODE_DIRECT_WRITE;
				g_tda7708_addr[1] = (unsigned char)((s_addr_l >> 8) & 0xff);
				g_tda7708_addr[2] = (unsigned char)(s_addr_l& 0xff);
				
				for(j = 0; j < sendByteNum; j++)
				{
					g_tda7708_data[j] = CMOST_Firmware[s_startNum+2+j] ;
				}
				tda7708_i2c_write(sendByteNum);

				s_startNum += sendByteNum;
				s_byteNum -= sendByteNum;
				tmp = sendByteNum/4;
				U32_ADD(s_addr_h, s_addr_l, tmp);

				s_cnt = s_startNum+2;
			}
			break;
		case TDA7708_STATUS_WAIT_INIT:
			g_tda7708_status = TDA7708_STATUS_CHECK_INIT;
			g_tda7708_timer = 0;
			break;
		case TDA7708_STATUS_CHECK_INIT:
			g_tda7708_addr[0] = 0x62;
			g_tda7708_addr[1] = 0x01;
			g_tda7708_addr[2] = 0x80;
			tda7708_i2c_read(4);
			if ( ( g_tda7708_data[0] == 0xAF) && ( g_tda7708_data[1] == 0xFE) && ( g_tda7708_data[2] == 0x42) && ( g_tda7708_data[3] == 0x00) )
			{
				g_tda7708_status = TDA7708_STATUS_READY;
			} else {
				++g_tda7708_timer;
				if ( (g_startup_cntr<10) && (g_tda7708_timer>30) ) {
					if (0==g_is_watchdog_rst) {
						delay_1ms(3000);
						REAL_SYS_PWR_OFF;
						delay_1ms(3000);
						for(;;);	//wait for reset
					} else {
						u8 x,y;
						for (x=PRESET_BAND_FM1; x<=PRESET_BAND_FM3; x++) {
							for(y=0;y<FM_PRESET_NUM;y++) {
								g_radio_info.preset_fm[x-PRESET_BAND_FM1][y]=2;
							}
						}

					}
				}
			}
			break;
		case TDA7708_STATUS_READY:
			g_tda7708_data[TDA7708_CMD_PARAM1_POS_H] = 0x00;
			g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] = 0x00;
			g_tda7708_data[TDA7708_CMD_PARAM1_POS_L] = 0x00;
			tda7708_i2c_cmd(CMD_CODE_TUNER_SET_BB_IF, 1, 0);
			g_tda7708_band = RADIO_BAND_NUMS;
			return TRUE;
		default:
			break;
	}

	return FALSE;
#else	// for debug
	g_tda7708_addr[0] = 0x61;
	g_tda7708_addr[1] = 0x40;
	g_tda7708_addr[2] = 0x1E;
	tda7708_i2c_read(4);
	
	if ((( g_tda7708_data[0] == 0xFF) && ( g_tda7708_data[1] == 0xFF) && ( g_tda7708_data[2] == 0xFF) && ( g_tda7708_data[3] == 0xFF) )   ||
	   (( g_tda7708_data[0] == 0x00) && ( g_tda7708_data[1] == 0x00) && ( g_tda7708_data[2] == 0x00) && ( g_tda7708_data[3] == 0x00) )  )
	{
		return FALSE;
	}

	g_tda7708_addr[0] = 0x61;
	g_tda7708_addr[1] = 0x40;
	g_tda7708_addr[2] = 0x22;
	tda7708_i2c_read(4);
	
	if ((( g_tda7708_data[0] == 0xFF) && ( g_tda7708_data[1] == 0xFF) && ( g_tda7708_data[2] == 0xFF) && ( g_tda7708_data[3] == 0xFF) )   ||
	   (( g_tda7708_data[0] == 0x00) && ( g_tda7708_data[1] == 0x00) && ( g_tda7708_data[2] == 0x00) && ( g_tda7708_data[3] == 0x00) )  )
	{
		return FALSE;
	}

	return TRUE;
#endif
}

void radio_dev_rds_init(void)
{
}

bool radio_dev_deinit(void) 
{
	g_tda7708_status = TDA7708_STATUS_NONE;
	s_addr_h = 0;
	s_addr_l = 0;
	s_byteNum = 0;
	s_startNum = 0;
	s_cnt = 0;

	return TRUE;
}
bool radio_dev_is_set_freq_done(void)
{
	if (TDA7708_STATUS_READY!=g_tda7708_status) {
		return FALSE;
	}

	g_tda7708_addr[0] = 0x62;
	g_tda7708_addr[1] = 0x01;
	g_tda7708_addr[2] = 0x00;
	tda7708_i2c_read(4);
	if (g_tda7708_data[2]&(1<<0)) {
		return FALSE;
	}
	if (g_tda7708_data[1]&(1<<4)) {
		return FALSE;
	}

	if (RADIO_BAND_FM == g_tda7708_band) {
		// reset & activate RDS buffer
		g_tda7708_data[TDA7708_CMD_PARAM1_POS_H] = 0x00;
		g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] = 0x00;
		g_tda7708_data[TDA7708_CMD_PARAM1_POS_L] = 0x01;
		g_tda7708_data[TDA7708_CMD_PARAM2_POS_H] = 0x00;
		g_tda7708_data[TDA7708_CMD_PARAM2_POS_M] = 0x02;
		g_tda7708_data[TDA7708_CMD_PARAM2_POS_L] = 0x21;
		g_tda7708_data[TDA7708_CMD_PARAM3_POS_H] = 0x00;
		g_tda7708_data[TDA7708_CMD_PARAM3_POS_M] = 0x00;
		g_tda7708_data[TDA7708_CMD_PARAM3_POS_L] = 0x08;
		tda7708_i2c_cmd(CMD_CODE_TUNER_RDSBUFFER_SET, 3, 0);
	}
	return TRUE;
}
void radio_dev_set_freq(RADIO_BAND band, u16 freq)
{
	u32 value;
	u32 tmp;

	if (TDA7708_STATUS_READY!=g_tda7708_status) {
		return;
	}

	if (band != g_tda7708_band) {
		g_tda7708_band = band;
		tda7708_switch_band(band);
		delay_1ms(10);
	}

	tmp = (u32)freq;
	if (RADIO_BAND_FM==band) {
		value = (u32)tmp*10;
	} else {
		value = (u32)tmp;
	}
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_H] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_L] = 0x01;
	g_tda7708_data[TDA7708_CMD_PARAM2_POS_H] = (value>>16)&0xFF;
	g_tda7708_data[TDA7708_CMD_PARAM2_POS_M] = (value>>8)&0xFF;
	g_tda7708_data[TDA7708_CMD_PARAM2_POS_L] = (value>>0)&0xFF;
	g_tda7708_data[TDA7708_CMD_PARAM3_POS_H] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM3_POS_M] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM3_POS_L] = 0x00;
	tda7708_i2c_cmd(CMD_CODE_TUNER_TUNE, 3, 0);
	
}
bool radio_dev_is_tune_ok(bool strict)
{
	u8 tmp;

	if (TDA7708_STATUS_READY!=g_tda7708_status) {
		return FALSE;
	}

	g_tda7708_data[TDA7708_CMD_PARAM1_POS_H] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_L] = 0x01;

	if ( RADIO_BAND_FM == g_tda7708_band ) {

		tda7708_i2c_cmd(CMD_CODE_TUNER_GET_CHANNEL_QUALITY, 1, 3);
		
		// check MultiPath
		if (g_tda7708_data[TDA7708_CMD_PARAM2_POS_H] > 0x60) {
			return FALSE;
		}
		// check SNR
		if (g_tda7708_data[TDA7708_CMD_PARAM2_POS_L] < 0x40) {
			return FALSE;
		}
		// check field strength
		if (g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] > 0x7F) {
			// -dBuV
			return FALSE;
		}
		// check detune
		if (g_tda7708_data[TDA7708_CMD_PARAM1_POS_L] > 0x1B) {
			return FALSE;
		}
		// check MPX
		if (g_tda7708_data[TDA7708_CMD_PARAM2_POS_M] > 0xC0) {
			return FALSE;
		}
		// check adjchannel
		tmp = g_tda7708_data[TDA7708_CMD_PARAM3_POS_H];
		if ( (tmp>0x64) && (tmp<=0x7F) ) {
			return FALSE;
		}
		
		// check deviation
//		if ((g_tda7708_data[TDA7708_CMD_PARAM3_POS_L]>>1) > 10) {
//			return FALSE;
//		}
		
		if (RDSM_LOC==g_tda7708_seek_mode) {
			if (g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] < 0x20) {
				// +dBuV
				return FALSE;
			}
		} else {
			if (g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] < 0x0E) {
				// +dBuV
				return FALSE;
			}
		}
	} else {
		tda7708_i2c_cmd(CMD_CODE_TUNER_GET_RECEPTION_QUALITY, 1, 3);

		// check SNR
//		if (g_tda7708_data[TDA7708_CMD_PARAM2_POS_L] < 0x66) {
//			return FALSE;
//		}

		// check field strength
		if (g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] > 0x7F) {
			// -dBuV
			return FALSE;
		}
		if (g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] < 0x14) {
			// +dBuV
			return FALSE;
		}
	}
	
	return TRUE;
}
bool radio_dev_is_tune_status_ready(void)
{

	return TRUE;
}
u8 radio_dev_get_stereo_status(void)
{
	u8 stereo = 1;

	if (TDA7708_STATUS_READY!=g_tda7708_status) {
		return 1;
	}

	if (RADIO_BAND_FM != g_tda7708_band) {
		return 1;
	}


	g_tda7708_data[TDA7708_CMD_PARAM1_POS_H] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_L] = 0x01;
	tda7708_i2c_cmd(CMD_CODE_TUNER_GET_RECEPTION_QUALITY, 1, 3);
	// get stereo
	stereo = (g_tda7708_data[TDA7708_CMD_PARAM3_POS_L]&0x01)?2:1;

	return stereo;
}

bool radio_dev_set_seek_mode(RADIO_DEV_SEEK_MODE mode)
{
	if ((RDSM_DX==mode) || (RDSM_LOC==mode)) {
		g_tda7708_seek_mode = mode;
	}
	return TRUE;
}

bool radio_dev_set_st_mode(bool on)
{
	if (TDA7708_STATUS_READY!=g_tda7708_status) {
		return FALSE;
	}

	g_tda7708_data[TDA7708_CMD_PARAM1_POS_H] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] = 0x00;
	if (FALSE == on) {
		// force mono
		g_tda7708_data[TDA7708_CMD_PARAM1_POS_L] = 0x01;
	} else {
		g_tda7708_data[TDA7708_CMD_PARAM1_POS_L] = 0x00;
	}
	tda7708_i2c_cmd(CMD_CODE_TUNER_FM_STEREO_MODE, 1, 0);
	return TRUE;
}

bool radio_dev_set_mute(bool mute)
{
	if (TDA7708_STATUS_READY!=g_tda7708_status) {
		return FALSE;
	}

	g_tda7708_data[TDA7708_CMD_PARAM1_POS_H] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] = 0x00;
	if (FALSE == mute) {
		g_tda7708_data[TDA7708_CMD_PARAM1_POS_L] = 0x03;
	} else {
		g_tda7708_data[TDA7708_CMD_PARAM1_POS_L] = 0x00;
	}
	tda7708_i2c_cmd(CMD_CODE_TUNER_MUTE, 1, 0);
	return TRUE;
}

#if RDS_FUNCTION==1
bool radio_dev_rds_update(void)
{
	static u8 s_got_a=0;
	static u8 s_got_b=0;
	static u8 s_got_c=0;
	static u8 s_got_d=0;
	u8 err_cnt;
	u8 block_id;
	u8 block_num;
	u8 cnt;
	u8 group_type;
	u8 addr;

	if (TDA7708_STATUS_READY!=g_tda7708_status) {
		return FALSE;
	}

	g_tda7708_data[TDA7708_CMD_PARAM1_POS_H] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_M] = 0x00;
	g_tda7708_data[TDA7708_CMD_PARAM1_POS_L] = 0x01;
	tda7708_i2c_cmd(CMD_CODE_TUNER_RDSBUFFER_READ, 1, 0xFF);

	block_num = g_tda7708_data[TDA7708_CMD_HEAD_POS_L]-1;
	if (block_num==0) {
		// no RDS block
		return FALSE;
	}

	if ((g_tda7708_data[TDA7708_CMD_PARAM1_POS_H] & 0x40) == 0x00) {
		// not sync
		return FALSE;
	}

	for (cnt=0; cnt<block_num; cnt++) {
		block_id = g_tda7708_data[TDA7708_CMD_PARAM2_POS_H+3*cnt] & 0x03;
		err_cnt = (g_tda7708_data[TDA7708_CMD_PARAM2_POS_H+3*cnt]>>4) & 0x07;
		switch (block_id) {
			case 0:	// block A
				if (err_cnt>CORRECTED_LARGE) {
					s_got_a=0;
				} else {
					s_got_a=1;
					g_rds_info.block_a = (g_tda7708_data[TDA7708_CMD_PARAM2_POS_M+3*cnt]<<8)|g_tda7708_data[TDA7708_CMD_PARAM2_POS_L+3*cnt];
				}
				s_got_b=0;
				s_got_c=0;
				s_got_d=0;
				break;
			case 1:	// block B
				if (err_cnt>CORRECTED_SMALL) {
					s_got_a=0;
					s_got_b=0;
				} else {
					s_got_b=1;
					g_rds_info.block_b = (g_tda7708_data[TDA7708_CMD_PARAM2_POS_M+3*cnt]<<8)|g_tda7708_data[TDA7708_CMD_PARAM2_POS_L+3*cnt];
				}
				s_got_c=0;
				s_got_d=0;
				break;
			case 2:	// block C
				if (err_cnt>CORRECTED_LARGE) {
					s_got_a=0;
					s_got_b=0;
					s_got_c=0;
				} else {
					s_got_c=1;
					g_rds_info.block_c = (g_tda7708_data[TDA7708_CMD_PARAM2_POS_M+3*cnt]<<8)|g_tda7708_data[TDA7708_CMD_PARAM2_POS_L+3*cnt];
				}
				s_got_d=0;
				break;
			case 3:	// block D
				if (err_cnt>CORRECTED_LARGE) {
					s_got_a=0;
					s_got_b=0;
					s_got_c=0;
					s_got_d=0;
				} else {
					s_got_d=1;
					g_rds_info.block_d = (g_tda7708_data[TDA7708_CMD_PARAM2_POS_M+3*cnt]<<8)|g_tda7708_data[TDA7708_CMD_PARAM2_POS_L+3*cnt];
				}
				break;
		}

		if ( (0!=s_got_a) && (0!=s_got_b) && (0!=s_got_c) && (0!=s_got_d) ) {
			s_got_a = 0;
			s_got_b = 0;
			s_got_c = 0;
			s_got_d = 0;

			// do RDS block process
			rds_update_pi(g_rds_info.block_a);

			group_type = g_rds_info.block_b >> 11;

			// Update pi code.  Version B formats always have the pi code in words A and C
			if (group_type & 0x01) {
				rds_update_pi(g_rds_info.block_c);
			}

			rds_update_pty((g_rds_info.block_b >> 5) & 0x1f);
			rds_update_tp(0!=(g_rds_info.block_b & (1<<10)));

			switch (group_type) 
			{
				case RDS_TYPE_0A:
					rds_update_af_list(g_rds_info.block_c);
					// fallthrough
				case RDS_TYPE_0B:
					rds_update_ta(0!=(g_rds_info.block_b & (1<<4)));
					addr = (g_rds_info.block_b & 0x3) * 2;
					rds_update_ps(addr+0, g_rds_info.block_d >> 8  );
					rds_update_ps(addr+1, g_rds_info.block_d & 0xff);
					break;

				case RDS_TYPE_2A:
					addr = g_rds_info.block_b&0x0F;
					rds_update_rt(addr, 4, 
						(g_rds_info.block_b&0x10)>>4, 
						g_rds_info.block_c>>8,
						g_rds_info.block_c&0xFF,
						g_rds_info.block_d>>8,
						g_rds_info.block_d&0xFF);
					break;

				case RDS_TYPE_2B:
					addr = g_rds_info.block_b&0x0F;
					rds_update_rt(addr, 2, 
						(g_rds_info.block_b&0x10)>>4, 
						g_rds_info.block_d>>8,
						g_rds_info.block_d&0xFF,
						0,
						0);
					break;

				case RDS_TYPE_4A:
					// TODO : CT 
					break;
				default:
					break;
			}
		}
	}

	return FALSE;
}
#endif

#endif

