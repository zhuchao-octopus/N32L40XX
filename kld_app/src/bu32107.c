
#include "public.h"

#if ASP_MODEL==ASP_BU32107

//#define SPDIF_OUT_ENABLE

#include "bu32107.h"

static BU32107_REG_INFO g_regs[BU32107_REG_NUMS] = 
{
	{0x0203, 0x00, FALSE},		// BU32107_DSP_SEL_1
	{0x0002, 0x00, FALSE},		// BU32107_INIT_SETUP_2
	{0x0109, 0x00, FALSE},		// BU32107_DIG_MIX_SEL
	{0x010A, 0x00, FALSE},		// BU32107_AVOL_DMIX
	{0x0103, 0x00, FALSE},		// BU32107_ANA_INPUT_SEL

	{0x0A00, 0x00, FALSE},		// BU32107_FADER_VOL_FL
	{0x0A01, 0x00, FALSE},		// BU32107_FADER_VOL_FR
	{0x0A02, 0x00, FALSE},		// BU32107_FADER_VOL_RL
	{0x0A03, 0x00, FALSE},		// BU32107_FADER_VOL_RR
	{0x0A04, 0x00, FALSE},		// BU32107_FADER_VOL_SL

	{0x0906, 0x00, FALSE},		// BU32107_DVOL_BOOST_FL
	{0x0907, 0x00, FALSE},		// BU32107_DVOL_BOOST_FR
	{0x0908, 0x00, FALSE},		// BU32107_DVOL_BOOST_RL
	{0x0909, 0x00, FALSE},		// BU32107_DVOL_BOOST_RR
	{0x090A, 0x00, FALSE},		// BU32107_DVOL_BOOST_SL
	{0x090B, 0x00, FALSE},		// BU32107_DVOL_BOOST_SR

	{0x0900, 0x00, FALSE},		// BU32107_DVOL_ATT_FL
	{0x0901, 0x00, FALSE},		// BU32107_DVOL_ATT_FR
	{0x0902, 0x00, FALSE},		// BU32107_DVOL_ATT_RL
	{0x0903, 0x00, FALSE},		// BU32107_DVOL_ATT_RR

	{0x0600, 0x00, FALSE},		// BU32107_EQ_MODE_SCALER

	{0x061D, 0x00, FALSE},		// BU32107_EQ_20HZ
	{0x0610, 0x00, FALSE},		// BU32107_EQ_50HZ
	{0x0611, 0x00, FALSE},		// BU32107_EQ_80HZ
	{0x0612, 0x00, FALSE},		// BU32107_EQ_125HZ
	{0x0613, 0x00, FALSE},		// BU32107_EQ_200HZ
	{0x0614, 0x00, FALSE},		// BU32107_EQ_315HZ
	{0x0615, 0x00, FALSE},		// BU32107_EQ_500HZ
	{0x0616, 0x00, FALSE},		// BU32107_EQ_800HZ
	{0x0617, 0x00, FALSE},		// BU32107_EQ_1K25
	{0x0618, 0x00, FALSE},		// BU32107_EQ_2K
	{0x0619, 0x00, FALSE},		// BU32107_EQ_3K15
	{0x061A, 0x00, FALSE},		// BU32107_EQ_5K
	{0x061B, 0x00, FALSE},		// BU32107_EQ_8K
	{0x061C, 0x00, FALSE},		// BU32107_EQ_12K5
	{0x061F, 0x00, FALSE},		// BU32107_EQ_20K
	{0x061E, 0x00, FALSE},		// BU32107_EQ_31HZ5
	{0x062D, 0x00, FALSE},		// BU32107_EQ_20HZ_R
	{0x0620, 0x00, FALSE},		// BU32107_EQ_50HZ_R
	{0x0621, 0x00, FALSE},		// BU32107_EQ_80HZ_R
	{0x0622, 0x00, FALSE},		// BU32107_EQ_125HZ_R
	{0x0623, 0x00, FALSE},		// BU32107_EQ_200HZ_R
	{0x0624, 0x00, FALSE},		// BU32107_EQ_315HZ_R
	{0x0625, 0x00, FALSE},		// BU32107_EQ_500HZ_R
	{0x0626, 0x00, FALSE},		// BU32107_EQ_800HZ_R
	{0x0627, 0x00, FALSE},		// BU32107_EQ_1K25_R
	{0x0628, 0x00, FALSE},		// BU32107_EQ_2K_R
	{0x0629, 0x00, FALSE},		// BU32107_EQ_3K15_R
	{0x062A, 0x00, FALSE},		// BU32107_EQ_5K_R
	{0x062B, 0x00, FALSE},		// BU32107_EQ_8K_R
	{0x062C, 0x00, FALSE},		// BU32107_EQ_12K5_R
	{0x062F, 0x00, FALSE},		// BU32107_EQ_20K_R

	{0x0703, 0x00, FALSE},		// BU32107_FRONT_HPF
	{0x0704, 0x00, FALSE},		// BU32107_REAR_HPF

	{0x0705, 0x00, FALSE},		// BU32107_FRONT_P2BASS
	{0x0706, 0x00, FALSE},		// BU32107_REAR_P2BASS

	{0x0701, 0x00, FALSE},		// BU32107_LOUDNESS_FILTER
	{0x0702, 0x00, FALSE},		// BU32107_LOUDNESS

	{0x0101, 0x00, FALSE},		// BU32107_INPUT_GAIN

	{0x0400, 0x00, FALSE},		// BU32107_TA_FL_H
	{0x0401, 0x00, FALSE},		// BU32107_TA_FL_L
	{0x0402, 0x00, FALSE},		// BU32107_TA_FR_H
	{0x0403, 0x00, FALSE},		// BU32107_TA_FR_L
	{0x0404, 0x00, FALSE},		// BU32107_TA_RL_H
	{0x0405, 0x00, FALSE},		// BU32107_TA_RL_L
	{0x0406, 0x00, FALSE},		// BU32107_TA_RR_H
	{0x0407, 0x00, FALSE},		// BU32107_TA_RR_L
//	{0x040A, 0x00, FALSE},		// BU32107_TA_RL2_H
//	{0x040B, 0x00, FALSE},		// BU32107_TA_RL2_L
//	{0x040C, 0x00, FALSE},		// BU32107_TA_RR2_H
//	{0x040D, 0x00, FALSE},		// BU32107_TA_RR2_L

	{0x0205, 0x00, FALSE},		// BU32107_DSP_SEL_3

//	{0x0709, 0x00, FALSE},		// BU32107_IIR

	{0x0202, 0x00, FALSE},		// BU32107_DIG_IO_SEL3
};

static void BU32107_UPDATE_REG(uint8_t id, uint8_t val) {
	if (val != g_regs[id].value) {
		g_regs[id].value = val;
		g_regs[id].changed = TRUE;
	}
}

static const s8 g_bu32107_gains[] = {
	-79,	/* volume 0 */
	-65, -58, -53, -48, -44,	/* volume 1~5 */
	-41, -38, -35, -32, -30,	/* volume 6~10 */
	-28, -26, -25, -24, -23,	/* volume 11~15 */
	-22, -21, -20, -18, -16,		/* volume 16~20 */
	-14, -12, -10, -8, -6,		/* volume 21~25 */
	-4, -2, 0, 2, 4		/* volume 26~30 */
};

static void bu32107_write(u16 addr, u8 value)
{
	i2c_start();
	i2c_send_byte(BU32107_I2C_W_ADDR);
	i2c_wait_ack();
	i2c_send_byte(MSB(addr));
	i2c_wait_ack();
	i2c_send_byte(LSB(addr));
	i2c_wait_ack();
	i2c_send_byte(value);
	i2c_wait_ack();
	i2c_stop();
}
static u8 bu32107_read(u16 addr)
{
	u8 value;
	
	i2c_start();
	i2c_send_byte(BU32107_I2C_W_ADDR);
	i2c_wait_ack();
	i2c_send_byte(0xD0);
	i2c_wait_ack();
	i2c_send_byte(0x00);
	i2c_wait_ack();
	i2c_send_byte(MSB(addr));
	i2c_wait_ack();
	i2c_send_byte(LSB(addr));
	i2c_wait_ack();
	i2c_stop();

	delay_us(20); // do a delay to make sure data available
	
	i2c_start();
	i2c_send_byte(BU32107_I2C_R_ADDR);
	i2c_wait_ack();
	value = i2c_read_byte(0);
	i2c_stop();
	
	return value;
}

static void bu32107_update_regs(void)
{
	u8 i;
	for (i=0; i<BU32107_REG_NUMS; i++) {
		if (!g_regs[i].changed) {
			continue;
		}
		bu32107_write(g_regs[i].addr, g_regs[i].value);
		g_regs[i].changed = FALSE;
	}
}

bool audio_dev_init(void)
{
	u8 i;
	
	// reset
	bu32107_write(0xFEFE, 0x81);

	delay_1ms(20);

	// through
	bu32107_write(0x0001, 0x1C);
	bu32107_write(0x0002, 0x08);	// Loudness bypass
	bu32107_write(0x0003, 0x00);
	bu32107_write(0x0004, 0x00);
	bu32107_write(0x0005, 0x00);
	bu32107_write(0x0006, 0x33);
	bu32107_write(0x0007, 0xFF);
	bu32107_write(0x0008, 0xF3);
	bu32107_write(0x0010, 0x0F);
	bu32107_write(0x0011, 0x00);	// digital io input1: I2S, 24bit
	bu32107_write(0x0012, 0x00);	// digital io input3: I2S, 24bit
	bu32107_write(0x0013, 0x0C);
	bu32107_write(0x0014, 0x00);
	bu32107_write(0x0015, 0x00);
	bu32107_write(0x0016, 0x0C);
	bu32107_write(0x0017, 0x00);
	bu32107_write(0x0018, 0x00);
	bu32107_write(0x0019, 0x00);
	bu32107_write(0x001A, 0x00);
	bu32107_write(0x001B, 0x00);
	bu32107_write(0x001C, 0x00);
	bu32107_write(0x001D, 0x00);
	bu32107_write(0x001E, 0xC0);
	bu32107_write(0x001F, 0x00);
	bu32107_write(0x0020, 0xC0);
	bu32107_write(0x0021, 0x34);
	bu32107_write(0x0022, 0x05);
	bu32107_write(0x0101, 0x00);
	bu32107_write(0x0102, 0x20);	// SL/SL -> SL/SR
	bu32107_write(0x0103, 0x00);
	bu32107_write(0x0104, 0xFF);	// Prohibite FL/FR/RL/RR analog mixing
	bu32107_write(0x0105, 0x0F);	// Prohibite SL/SR analog mixing
	bu32107_write(0x0106, 0x80);
	bu32107_write(0x0107, 0x80);
	bu32107_write(0x0108, 0x80);
	bu32107_write(0x0109, 0xC0);	// DAC digital mix mode:ON, FL/FR mix OFF
	bu32107_write(0x010A, 0x80);
	bu32107_write(0x0200, 0x00);	// disable digital ext output&input
	bu32107_write(0x0201, 0x00);	// disable digital output 1&3
	bu32107_write(0x0202, 0x66);	// digital input 3: C-1; digital input 1: C-1
	bu32107_write(0x0203, 0x00);	// time-alignment 2-ch mode
	bu32107_write(0x0204, 0x00);
	bu32107_write(0x0205, 0x05);	// time-alignment -> p2bass
	bu32107_write(0x0206, 0x32);	// Digital Mixing -> SR; Digital Stereo mix (L+R)
	bu32107_write(0x0207, 0x00);
	bu32107_write(0x0208, 0x00);
	bu32107_write(0x0400, 0x00);
	bu32107_write(0x0401, 0x00);
	bu32107_write(0x0402, 0x00);
	bu32107_write(0x0403, 0x00);
	bu32107_write(0x0404, 0x00);
	bu32107_write(0x0405, 0x00);
	bu32107_write(0x0406, 0x00);
	bu32107_write(0x0407, 0x00);
	bu32107_write(0x0408, 0x00);
	bu32107_write(0x0409, 0x00);
	bu32107_write(0x040A, 0x00);
	bu32107_write(0x040B, 0x00);
	bu32107_write(0x040C, 0x00);
	bu32107_write(0x040D, 0x00);
	bu32107_write(0x0500, 0x04);
	bu32107_write(0x0501, 0x10);		// SA2: 36dB
	bu32107_write(0x0600, 0x80);		// 16-Band EQ
	bu32107_write(0x0610, 0x00);		// Front
	bu32107_write(0x0611, 0x00);		// Front
	bu32107_write(0x0612, 0x00);		// Front
	bu32107_write(0x0613, 0x00);		// Front
	bu32107_write(0x0614, 0x00);		// Front
	bu32107_write(0x0615, 0x00);		// Front
	bu32107_write(0x0616, 0x00);		// Front
	bu32107_write(0x0617, 0x00);		// Front
	bu32107_write(0x0618, 0x00);		// Front
	bu32107_write(0x0619, 0x00);		// Front
	bu32107_write(0x061A, 0x00);		// Front
	bu32107_write(0x061B, 0x00);		// Front
	bu32107_write(0x061C, 0x00);		// Front
	bu32107_write(0x061D, 0x00);		// Front
	bu32107_write(0x061E, 0x40);		// Front & Rear
	bu32107_write(0x061F, 0x00);		// Front
	bu32107_write(0x0620, 0x00);		// Rear
	bu32107_write(0x0621, 0x00);		// Rear
	bu32107_write(0x0622, 0x00);		// Rear
	bu32107_write(0x0623, 0x00);		// Rear
	bu32107_write(0x0624, 0x00);		// Rear
	bu32107_write(0x0625, 0x00);		// Rear
	bu32107_write(0x0626, 0x00);		// Rear
	bu32107_write(0x0627, 0x00);		// Rear
	bu32107_write(0x0628, 0x00);		// Rear
	bu32107_write(0x0629, 0x00);		// Rear
	bu32107_write(0x062A, 0x00);		// Rear
	bu32107_write(0x062B, 0x00);		// Rear
	bu32107_write(0x062C, 0x00);		// Rear
	bu32107_write(0x062D, 0x00);		// Rear
	bu32107_write(0x062E, 0x00);		// Rear (Invalid)
	bu32107_write(0x062F, 0x00);		// Rear
	bu32107_write(0x0700, 0x00);
	bu32107_write(0x0701, 0x55);		// Loudness: LPF 100HZ, HPF 10KHZ
	bu32107_write(0x0702, 0xB0);		// Loudness: HiBoost=1.0, 0dB
	bu32107_write(0x0703, 0x00);		// Front HPF through
	bu32107_write(0x0704, 0x00);		// Rear HPF through
	bu32107_write(0x0705, 0x80);		// Front P2BASS through
	bu32107_write(0x0706, 0x80);		// Rear P2BASS through
	bu32107_write(0x0707, 0x00);
	bu32107_write(0x0708, 0x00);
	bu32107_write(0x0709, 0x00);		// IIR A/IIR B through
	bu32107_write(0x0800, 0x00);
	bu32107_write(0x0801, 0x00);
	bu32107_write(0x0802, 0x00);
	bu32107_write(0x0803, 0x00);
	bu32107_write(0x0804, 0x00);
	bu32107_write(0x0805, 0x40);
	bu32107_write(0x0900, 0x40);
	bu32107_write(0x0901, 0x40);
	bu32107_write(0x0902, 0x40);
	bu32107_write(0x0903, 0x40);
	bu32107_write(0x0904, 0x40);
	bu32107_write(0x0905, 0x40);
	bu32107_write(0x0906, 0x80);
	bu32107_write(0x0907, 0x80);
	bu32107_write(0x0908, 0x80);
	bu32107_write(0x0909, 0x80);
	bu32107_write(0x090A, 0x80);
	bu32107_write(0x090B, 0x80);
	bu32107_write(0x0A00, 0x80);		// mute FL
	bu32107_write(0x0A01, 0x80);		// mute FR
	bu32107_write(0x0A02, 0x80);		// mute RL
	bu32107_write(0x0A03, 0x80);		// mute RR
	bu32107_write(0x0A04, 0x80);		// mute SL
	bu32107_write(0x0A05, 0xA0);		// 0dB

#ifdef SPDIF_OUT_ENABLE
	bu32107_write(0x0016, 0x0C);
	bu32107_write(0x0017, 0x00);
	bu32107_write(0x0018, 0x00);
	bu32107_write(0x0201, 0x40);
#endif

#if 0
	// setup IIR A/IIR B coef
	bu32107_write(0x0709, 0x80);		// IIR A/IIR B coef
	for (i=0; i<2; i++) {
		bu32107_write(0x1100, 0x06+i);		// select IIR A or IIR B
		bu32107_write(0x1101, 0x0F);		// b0[31:24]
		bu32107_write(0x1102, 0xF6);		// b0[23:16]
		bu32107_write(0x1103, 0x4C);		// b0[15:8]
		bu32107_write(0x1104, 0x17);		// b0[7:0]

		// b1/b2/a1/a2 all 0h
		for (j=0; j<16; j++) {
			bu32107_write(0x1105+j, 0x00);
		}
	}
	bu32107_write(0x0709, 0x00);		// IIR A/IIR B through
#endif

	for (i=0; i<BU32107_REG_NUMS; i++) {
		g_regs[i].value = bu32107_read(g_regs[i].addr);
		g_regs[i].changed = FALSE;
	}

	return TRUE;
}

void audio_dev_deinit(void)
{
}

void audio_dev_update_source(AUDIO_SOURCE src)
{
	u8 val;

	//select analog input
	val = g_regs[BU32107_ANA_INPUT_SEL].value;
	val &= ~(7<<0);
	switch (src) {
		case AUDIO_SRC_RADIO:
			val |= (2<<0);
			break;
		case AUDIO_SRC_BT_MODULE:
			val |= (1<<0);
			break;
		case AUDIO_SRC_DVD:
			val |= (3<<0);
			break;
		case AUDIO_SRC_AUXIN:
			val |= (0<<0);
			break;
		case AUDIO_SRC_TV:
			val |= (4<<0);
			break;
		default:
			break;
	}
	BU32107_UPDATE_REG(BU32107_ANA_INPUT_SEL, val);

	// select DSP input
	val = g_regs[BU32107_DSP_SEL_1].value;
	switch (src) {
		case AUDIO_SRC_RADIO:
		case AUDIO_SRC_BT_MODULE:
		case AUDIO_SRC_DVD:
		case AUDIO_SRC_AUXIN:
		case AUDIO_SRC_TV:
			val &= ~(1<<7);
			break;
		case AUDIO_SRC_HOST:
		default:
			val |= (1<<7);
			break;
	}
	BU32107_UPDATE_REG(BU32107_DSP_SEL_1, val);

	// modify input gain
	if ( (AUDIO_SRC_BT_MODULE==src)&&(g_audio_info.bt_phone_on) ) {
		val = 0+A_BT_PHONE_EXTRA_DB;
	} else if ( (AUDIO_SRC_BT_MODULE==src)&&(g_audio_info.bt_voice_vol) ) {
		val = 0+A_BT_PHONE_EXTRA_DB;
	} else {
		val = 0;
	}
	BU32107_UPDATE_REG(BU32107_INPUT_GAIN, val);

	// modify digital IO selector if needed
	if (g_fake_pwr_off&&(!g_audio_info.bt_phone_on)) {
		BU32107_UPDATE_REG(BU32107_DIG_IO_SEL3, 0x00);
	} else {
		BU32107_UPDATE_REG(BU32107_DIG_IO_SEL3, 0x66);
	}

	if ( (AUDIO_SRC_BT_MODULE==src) && (!g_audio_info.bt_phone_on) && (!g_audio_info.bt_voice_on)
		&& (!g_audio_info.bt_music_on) ) {

		val = g_regs[BU32107_DSP_SEL_1].value;
		val |= (1<<7);
		BU32107_UPDATE_REG(BU32107_DSP_SEL_1, val);

		BU32107_UPDATE_REG(BU32107_DIG_IO_SEL3, 0x00);
	}

	if (AUDIO_SRC_BT_MODULE==src) {
		BU32107_UPDATE_REG(BU32107_EQ_MODE_SCALER, 0x90);
	} else {
		BU32107_UPDATE_REG(BU32107_EQ_MODE_SCALER, 0x80);
	}

	
	bu32107_update_regs();

	audio_dev_update_volume(g_audio_info.cur_vol);
}

void audio_dev_update_volume(u8 vol)
{
	s8 gain;
	u8 val_fader;
	u8 val_dvol_boost;
	s16 gain_sub;
	u8 val_fader_sub;
	u8 val_dvol_boost_sub;

#if MAX_VOLUME == 30
	if (vol>MAX_VOLUME) {
		return;
	}

	gain = g_bu32107_gains[vol];
	gain_sub = g_bu32107_gains[vol];
	gain_sub -= g_audio_info.dsp_sub_gain;

	if ( (1==g_audio_info.dsp_loud_on) && (AUDIO_SRC_NONE != g_audio_info.cur_source) ) {
		// add loudness compansation
		gain += (g_audio_info.loudness+g_audio_info.dsp_loud_lpf+g_audio_info.dsp_loud_hpf);
		gain_sub += (g_audio_info.loudness+g_audio_info.dsp_loud_lpf+g_audio_info.dsp_loud_hpf);
	}

	//  fake for PHAT
	if (g_audio_info.dsp_phat_on) {
		gain += 1;
		gain += g_audio_info.dsp_phat_gain;
	}

	switch (g_audio_info.output_type) {
		case AUDIO_OUTPUT_N_15DB:
			gain -= 15;
			break;
		case AUDIO_OUTPUT_N_12DB:
			gain -= 12;
			break;
		case AUDIO_OUTPUT_N_9DB:
			gain -= 9;
			break;
		case AUDIO_OUTPUT_N_6DB:
			gain -= 6;
			break;
		case AUDIO_OUTPUT_N_3DB:
			gain -= 3;
			break;
		case AUDIO_OUTPUT_NORMAL:
		default:
			break;
	}
	gain += g_audio_info.extra_input_gain[g_audio_info.cur_source];

	// decrease bt phone vol, for GOC BT BC6
	if (g_audio_info.bt_phone_on) {
		if (1==g_bt_type) {
			gain -= 15;
		}
	}

	if (gain>=0) {
		val_fader = 0xA0;
		val_dvol_boost = 0x80-gain*2;
	} else {
		val_fader = 0xA0+(u8)((s8)0-gain);
		val_dvol_boost = 0x80;
	}

	if (g_audio_info.dsp_spk_sw.field.sub_mute) {
		val_fader_sub = 0x80;
		val_dvol_boost_sub = 0x00;
	} else if (gain_sub>=0) {
		val_fader_sub = 0xA0;
		val_dvol_boost_sub = 0x80-gain_sub*2;
	} else {
		if (gain_sub<-79) {
			gain_sub = -79;
		}
		val_fader_sub = 0xA0+(u8)((s8)0-gain_sub);
		val_dvol_boost_sub = 0x80;
	}

	BU32107_UPDATE_REG(BU32107_FADER_VOL_FL, val_fader);
	BU32107_UPDATE_REG(BU32107_FADER_VOL_FR, val_fader);
	BU32107_UPDATE_REG(BU32107_FADER_VOL_RL, val_fader);
	BU32107_UPDATE_REG(BU32107_FADER_VOL_RR, val_fader);
	BU32107_UPDATE_REG(BU32107_FADER_VOL_SL, val_fader_sub);
	BU32107_UPDATE_REG(BU32107_DVOL_BOOST_FL, val_dvol_boost);
	BU32107_UPDATE_REG(BU32107_DVOL_BOOST_FR, val_dvol_boost);
	BU32107_UPDATE_REG(BU32107_DVOL_BOOST_RL, val_dvol_boost);
	BU32107_UPDATE_REG(BU32107_DVOL_BOOST_RR, val_dvol_boost);
	BU32107_UPDATE_REG(BU32107_DVOL_BOOST_SL, val_dvol_boost_sub);

	bu32107_update_regs();	
#else
#error "we didn't support this max volume" MAX_VOLUME
#endif	
}

static void bu32107_update_mix_vol(u8 vol)
{
	s8 gain;
	u16 tmp;
	u8 value;

#if MAX_VOLUME == 30
	if (vol>MAX_VOLUME) {
		return;
	}

	if (g_audio_info.reverse_on) {
		if (0==g_sys_info_store.vol_ctrl_when_reverse) {
			vol = 0;
		} else { 
			tmp = (u16)(vol) * (u16)(g_sys_info_store.vol_ctrl_when_reverse) / (u16)100;
			vol = tmp&0xFF;
		}
	}

	if (0==vol) {
		value = g_regs[BU32107_DIG_MIX_SEL].value;
		value &= ~(3<<4);
		BU32107_UPDATE_REG(BU32107_DIG_MIX_SEL, value);
	} else {
		gain = g_bu32107_gains[vol];
		if (NAVI_BREAK_DIRECT!=g_audio_info.navi_mix_extra_gain) {
			// add extra gain from user
			if (g_audio_info.navi_mix_extra_gain > 0x80) {
				gain += (s8)(g_audio_info.navi_mix_extra_gain-0x80);
			} else if (g_audio_info.navi_mix_extra_gain < 0x80) {
				gain -= (s8)(0x80-g_audio_info.navi_mix_extra_gain);
			}
		}

		// clamp the gain
		if (gain>36) {
			gain = 36;
		} else if (gain<-69) {
			gain = -69;
		}

		if (gain>=0) {
			value = 0xA0;
			BU32107_UPDATE_REG(BU32107_AVOL_DMIX, value);

			value = 0x80-gain*2;
			BU32107_UPDATE_REG(BU32107_DVOL_BOOST_SR, value);
		} else {
			value = 0xA0+(u8)((s8)0-gain);
			BU32107_UPDATE_REG(BU32107_AVOL_DMIX, value);
		
			value = 0x80;
			BU32107_UPDATE_REG(BU32107_DVOL_BOOST_SR, value);
		}

		value = g_regs[BU32107_DIG_MIX_SEL].value;
		value |= (3<<4);
		BU32107_UPDATE_REG(BU32107_DIG_MIX_SEL, value);

	}

	bu32107_update_regs();
#else
#error "we didn't support this max volume" MAX_VOLUME
#endif
}

void audio_dev_update_navi_mix(bool on)
{
	if (FALSE == on) {
		bu32107_update_mix_vol(0);
	} else {
		bu32107_update_mix_vol(g_audio_info.navi_mix_vol);
	}
}

void audio_dev_update_navi_mix_vol(u8 vol)
{
	if (g_audio_info.bt_phone_on) {
		bu32107_update_mix_vol(0);
	} else if (g_audio_info.bt_voice_on) {
		bu32107_update_mix_vol(0);
	} else if (g_audio_info.navi_break_on) {
		bu32107_update_mix_vol(vol);
	}
}

// update EQ settings according to the AUDIO_INFO's eq_cur_freq & eq_cur_level member

static u8 bu32107_calc_eq_level_to_reg_gain(u8 val, s8 level)
{
	val &= ~0x1F;
	if (level > MAX_EQ_LEVEL) {
		level = MAX_EQ_LEVEL;
	}

	if (level >= DEFAULT_EQ_LEVEL) {
		val += (level-DEFAULT_EQ_LEVEL)/2;
	} else if (level<0) {
		if (-1==level) {
			val += 11;
		} else {
			val += 12;
		}
		val |= (1<<4);
	} else {
		val += (DEFAULT_EQ_LEVEL-level);
		val |= (1<<4);
	}

	return val;
}

static s8 _audio_do_fake_hpf_lpf(s8 level, u8 eq_id, u8 hpf, u8 lpf)
{
	s8 result = level;

	if (hpf>0) {
		switch (eq_id) {
			case EQ_FREQ_20HZ:
				if (hpf>1) {
					result = result-(hpf);
				}
				if (hpf>6) {
					result = result -4;
				}
				break;
			case EQ_FREQ_50HZ:
				if (hpf>3) {
					result = result-((hpf-2));
				}
				if (hpf>6) {
					result = result -3;
				}
				break;
			case EQ_FREQ_80HZ:
				if (hpf>5) {
					result = result-((hpf-4));
				}
				break;
			case EQ_FREQ_125HZ:
				if (hpf>7) {
					result = result-((hpf-6));
				}
				break;
			case EQ_FREQ_200HZ:
				if (hpf>9) {
					result = result-((hpf-8));
				}
				break;
			case EQ_FREQ_315HZ:
				if (hpf>11) {
					result = result-((hpf-10));
				}
				break;
			case EQ_FREQ_500HZ:
				if (hpf>13) {
					result = result-((hpf-12));
				}
				break;
			case EQ_FREQ_800HZ:
				if (hpf>15) {
					result = result-((hpf-14));
				}
				break;
			case EQ_FREQ_1K25HZ:
				if (hpf>17) {
					result = result-((hpf-16));
				}
				break;
			case EQ_FREQ_2KHZ:
				if (hpf>19) {
					result = result-((hpf-18));
				}
				break;
			case EQ_FREQ_3K15HZ:
				if (hpf>21) {
					result = result-((hpf-20));
				}
				break;
			case EQ_FREQ_5KHZ:
				if (hpf>23) {
					result = result-((hpf-22));
				}
				break;
			case EQ_FREQ_8KHZ:
				if (hpf>25) {
					result = result-((hpf-24));
				}
				break;
			case EQ_FREQ_12K5HZ:
				if (hpf>27) {
					result = result-((hpf-26));
				}
				break;
			case EQ_FREQ_20KHZ:
				if (hpf>29) {
					result = result-((hpf-28));
				}
				break;
		}
	}

	if (lpf>0) {
		switch (eq_id) {
			case EQ_FREQ_20HZ:
				if (lpf<3) {
					result = result - ((2-lpf));
				}
				break;
			case EQ_FREQ_50HZ:
				if (lpf<5) {
					result = result - ((4-lpf));
				}
				break;
			case EQ_FREQ_80HZ:
				if (lpf<7) {
					result = result - ((6-lpf));
				}
				break;
			case EQ_FREQ_125HZ:
				if (lpf<9) {
					result = result - ((8-lpf));
				}
				break;
			case EQ_FREQ_200HZ:
				if (lpf<11) {
					result = result - ((10-lpf));
				}
				break;
			case EQ_FREQ_315HZ:
				if (lpf<13) {
					result = result - ((12-lpf));
				}
				break;
			case EQ_FREQ_500HZ:
				if (lpf<15) {
					result = result - ((14-lpf));
				}
				break;
			case EQ_FREQ_800HZ:
				if (lpf<17) {
					result = result - ((16-lpf));
				}
				break;
			case EQ_FREQ_1K25HZ:
				if (lpf<19) {
					result = result - ((18-lpf));
				}
				break;
			case EQ_FREQ_2KHZ:
				if (lpf<21) {
					result = result - ((20-lpf));
				}
				break;
			case EQ_FREQ_3K15HZ:
				if (lpf<23) {
					result = result - ((22-lpf));
				}
				break;
			case EQ_FREQ_5KHZ:
				if (lpf<25) {
					result = result - ((24-lpf));
				}
				break;
			case EQ_FREQ_8KHZ:
				if (lpf<27) {
					result = result - ((26-lpf));
				}
				break;
			case EQ_FREQ_12K5HZ:
				if (lpf<29) {
					result = result - ((28-lpf));
				}
				break;
			case EQ_FREQ_20KHZ:
				if (lpf<31) {
					result = result - ((30-lpf));
				}
				break;

			default:
				break;
		}
	}

	return result;
}

void audio_dev_update_eq()
{
	u8 val;
	u8 reg_id;
	u8 reg_id_rear;
	u8 eq_id;
	s8 level;
	u8 hpf;
	u8 lpf;

	reg_id = BU32107_EQ_20HZ;
	reg_id_rear = BU32107_EQ_20HZ_R;

	for (eq_id=EQ_FREQ_20HZ; eq_id<EQ_FREQ_NUMS; eq_id++) {
		// Front
		level = g_audio_info.eq_visible_level[eq_id];
		hpf = g_audio_info.dsp_hpf_fc;
		lpf = g_audio_info.dsp_lpf_fc;
		level = _audio_do_fake_hpf_lpf(level, eq_id, hpf, lpf);
		val = bu32107_calc_eq_level_to_reg_gain(
				g_regs[reg_id].value, 
				level);
		BU32107_UPDATE_REG(reg_id, val);
		++reg_id;

		// Rear
		level = g_audio_info.eq_visible_level[eq_id];
		if (AUDIO_EQ_MIDDLE==g_audio_info.dsp_eq_pos) {
			hpf = g_audio_info.dsp_hpf_fc;
			lpf = g_audio_info.dsp_lpf_fc;
		} else {
			hpf = g_audio_info.dsp_hpf_fc_rear;
			lpf = g_audio_info.dsp_lpf_fc_rear;
		}

		level = _audio_do_fake_hpf_lpf(level, eq_id, hpf, lpf);
		val = bu32107_calc_eq_level_to_reg_gain(
				g_regs[reg_id_rear].value, 
				level);
		BU32107_UPDATE_REG(reg_id_rear, val);
		++reg_id_rear;
	}

	bu32107_update_regs();	
}

void audio_dev_update_fader_balance(u8 fad, u8 bal)
{
	u8 atten[AUDIO_SPK_NUMS];
	u8 val;
	u8 cnt;

	for (cnt=AUDIO_SPK_FL; cnt<AUDIO_SPK_NUMS; cnt++) {
		// if all speaker mute
		if (g_audio_info.dsp_spk_sw.field.all_mute) {
			atten[cnt] = 194;
			continue;
		}

		// if individual speaker mute
		if (g_audio_info.dsp_spk_sw.byte & (1<<cnt)) {
			atten[cnt] = 194;
			continue;
		}

		// individual speaker atten
		val = g_audio_info.dsp_spk_atten[cnt];
		if (val>DSP_SPK_GAIN_MIN) {
			val = DSP_SPK_GAIN_MIN;
		}
		atten[cnt] = val*2;

		// all speaker atten
		val = g_audio_info.dsp_all_atten;
		if (val>DSP_SPK_GAIN_MIN) {
			val = DSP_SPK_GAIN_MIN;
		}
		atten[cnt] = atten[cnt] + val*2;
	}

	if ((!g_audio_info.bt_phone_on)&&(!g_audio_info.bt_voice_on)&&(g_audio_info.navi_break_on)) {
		if (NAVI_BREAK_DIRECT==g_audio_info.navi_mix_extra_gain) {	// navi break directly
			// mute the front speaker
			atten[AUDIO_SPK_FL] = 194;
			atten[AUDIO_SPK_FR] = 194;
		} else {	// navi mix
			// cut down the front speaker
			atten[AUDIO_SPK_FL] += VOLUME_CUT_WHEN_NAVI_MIX*2;
			atten[AUDIO_SPK_FR] += VOLUME_CUT_WHEN_NAVI_MIX*2;
		}
	}

	for (cnt=AUDIO_SPK_FL; cnt<AUDIO_SPK_NUMS; cnt++) {
		if (atten[cnt]>190) {
			val = 0x00;	// -inf
		} else {
			val = 0x40+atten[cnt];
		}
		BU32107_UPDATE_REG(BU32107_DVOL_ATT_FL+cnt, val);
		
	}

	bu32107_update_regs();	
}

void audio_dev_update_loudness(u8 loud)
{
	u8 val;
	
	if (AUDIO_SRC_NONE == g_audio_info.cur_source) {
		loud = 0;
	}

	if (0==g_audio_info.dsp_loud_on) {
		loud = 0;
	}

	// LOUD on->off, set volume early
	if (0==loud) {
		audio_dev_update_volume(g_audio_info.cur_vol);
	}

	val = g_regs[BU32107_LOUDNESS].value;
	val &= ~0x0F;
	val |= (g_audio_info.loudness+g_audio_info.dsp_loud_lpf+g_audio_info.dsp_loud_hpf);
	BU32107_UPDATE_REG(BU32107_LOUDNESS, val);
#if 0
	val = g_regs[BU32107_LOUDNESS_FILTER].value;
	val &= ~0x77;
	switch (g_audio_info.dsp_loud_lpf) {
		case 0:
			val |= 0x00;	// 30HZ
			break;
		case 1:
			val |= 0x01;	// 40HZ
			break;
		case 2:
			val |= 0x02;	// 50HZ
			break;
		case 3:
			val |= 0x04;	// 80HZ
			break;
		case 4:
		default:
			val |= 0x05;	// 100HZ
			break;
	}
	switch (g_audio_info.dsp_loud_hpf) {
		case 0:
			val |= 0x00;	// 3KHZ
			break;
		case 1:
			val |= 0x10;	// 4KHZ
			break;
		case 2:
			val |= 0x20;	// 5KHZ
			break;
		case 3:
			val |= 0x40;	// 8KHZ
			break;
		case 4:
		default:
			val |= 0x50;	// 10KHZ
			break;
	}
	BU32107_UPDATE_REG(BU32107_LOUDNESS_FILTER, val);
#endif
	val = g_regs[BU32107_INIT_SETUP_2].value;
	if (0==loud) {
		val |= (1<<3);
	} else {
		val &= ~(1<<3);
	}
	BU32107_UPDATE_REG(BU32107_INIT_SETUP_2, val);
	bu32107_update_regs();
	
	// LOUD off->on, set volume late
	if (0!=loud) {
		audio_dev_update_volume(g_audio_info.cur_vol);
	}

}

void audio_dev_update_subwoofer(u8 subwoofer)
{
#if MAX_SUBWOOFER_LVL==15
	audio_dev_update_volume(g_audio_info.cur_vol);
#else
#error "we didn't support this subwoofer max level" MAX_SUBWOOFER_LVL
#endif
}

void audio_dev_update_dsp_settings_misc(void)
{
	u8 val;
	u8 gain;
	u8 fc;

	// HPF
	val = 0;//g_audio_info.dsp_hpf_fc & 0x0F;
	BU32107_UPDATE_REG(BU32107_FRONT_HPF, val);
	BU32107_UPDATE_REG(BU32107_REAR_HPF, val);
#if 0
	// PHAT
	// fake with EQ 31.5HZ
	if (g_audio_info.dsp_phat_on) {
		val = 0x41 + g_audio_info.dsp_phat_gain;
	} else {
		val = 0x40;
	}
	BU32107_UPDATE_REG(BU32107_EQ_31HZ5, val);
#endif
	// BASS
	val = 0x80;
	if (g_audio_info.dsp_bass_on) {
		fc = g_audio_info.dsp_bass_fc+3;
		if (fc>7)
			fc = 7;
		val |= (fc << 4);

		gain = (g_audio_info.dsp_bass_gain+1)*2;
		if (gain>12)
			gain = 12;
		val |= gain;

	} else {
		val = 0x80;
	}
	BU32107_UPDATE_REG(BU32107_FRONT_P2BASS, val);
	BU32107_UPDATE_REG(BU32107_REAR_P2BASS, val);

	bu32107_update_regs();
}

void audio_dev_update_dsp_settings_delay(void)
{
	u16 tmp;
	u8 id;
	u8 val[AUDIO_SPK_NUMS] = {0, 0, 0, 0};
	u8 fader;
	u8 balance;

	if (AUDIO_DELAY_AUTO == g_audio_info.dsp_delay_mode) {
		switch (g_audio_info.dsp_delay_c_pos) {
			case AUDIO_DSP_C_POS_FL:
				fader = MIN_FIELD_LEVEL;
				balance = MIN_FIELD_LEVEL;
				break;
			case AUDIO_DSP_C_POS_FR:
				fader = MIN_FIELD_LEVEL;
				balance = MAX_FIELD_LEVEL;
				break;
			case AUDIO_DSP_C_POS_RL:
				fader = MAX_FIELD_LEVEL;
				balance = MIN_FIELD_LEVEL;
				break;
			case AUDIO_DSP_C_POS_RR:
				fader = MAX_FIELD_LEVEL;
				balance = MAX_FIELD_LEVEL;
				break;
			case AUDIO_DSP_C_POS_ALL:
				fader = DEFAULT_FIELD_LEVEL;
				balance = DEFAULT_FIELD_LEVEL;
				break;
			case AUDIO_DSP_C_POS_USER:
			default:
				fader = g_audio_info.fader;
				balance = g_audio_info.balance;
				break;
		}

		if (fader < DEFAULT_FIELD_LEVEL) {
			val[AUDIO_SPK_FL] = val[AUDIO_SPK_FL] + (DEFAULT_FIELD_LEVEL-fader) * (g_audio_info.dsp_delay_spk_height+1);
			val[AUDIO_SPK_FR] = val[AUDIO_SPK_FR] + (DEFAULT_FIELD_LEVEL-fader) * (g_audio_info.dsp_delay_spk_height+1);
		} else {
			val[AUDIO_SPK_RL] = val[AUDIO_SPK_RL] + (fader-DEFAULT_FIELD_LEVEL) * (g_audio_info.dsp_delay_spk_height+1);
			val[AUDIO_SPK_RR] = val[AUDIO_SPK_RR] + (fader-DEFAULT_FIELD_LEVEL) * (g_audio_info.dsp_delay_spk_height+1);
		}
		if (balance < DEFAULT_FIELD_LEVEL) {
			val[AUDIO_SPK_FL] = val[AUDIO_SPK_FL] + (DEFAULT_FIELD_LEVEL-balance) * (g_audio_info.dsp_delay_spk_width+1);
			val[AUDIO_SPK_RL] = val[AUDIO_SPK_RL] + (DEFAULT_FIELD_LEVEL-balance) * (g_audio_info.dsp_delay_spk_width+1);
		} else {
			val[AUDIO_SPK_FR] = val[AUDIO_SPK_FR] + (balance-DEFAULT_FIELD_LEVEL) * (g_audio_info.dsp_delay_spk_width+1);
			val[AUDIO_SPK_RR] = val[AUDIO_SPK_RR] + (balance-DEFAULT_FIELD_LEVEL) * (g_audio_info.dsp_delay_spk_width+1);
		}
	}
	for (id=AUDIO_SPK_FL; id<AUDIO_SPK_NUMS; id++) {
		if (AUDIO_DELAY_MANUAL == g_audio_info.dsp_delay_mode) {
			tmp = (g_audio_info.dsp_delay_spk[id] * 48 / 34);
		} else {
			tmp = (u16)(val[id]) * 10 / 3;
		}
		if (tmp>0x01FF) {
			tmp = 0x01FF;
		}
		BU32107_UPDATE_REG(BU32107_TA_FL_H+id*2, MSB(tmp));
		BU32107_UPDATE_REG(BU32107_TA_FL_L+id*2, LSB(tmp));
	}

	bu32107_update_regs();
}

void audio_dev_update_dsp_settings_gain(void)
{
	audio_dev_update_fader_balance(0,0);
	audio_dev_update_volume(g_audio_info.cur_vol);
}

void audio_dev_update_spectrum_data(void)
{
	u8 cnt=0;
	u8 value;

	for(cnt=0; cnt<SPECTRUM_NUMS; cnt++) {
		value = bu32107_read(0xA000 + 2*cnt);
		if (value >= 0x80) {
			value = 0;
		}
		g_audio_info.sa_data[cnt] = value;
	}
}

#endif

