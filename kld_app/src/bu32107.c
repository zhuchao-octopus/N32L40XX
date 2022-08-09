
#include "public.h"

#if ASP_MODEL==ASP_BU32107

#define SPDIF_OUT_ENABLE

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
	{0x061D, 0x00, FALSE},		// BU32107_EQ_BASS_F
	{0x061E, 0x00, FALSE},		// BU32107_EQ_MID
	{0x061F, 0x00, FALSE},		// BU32107_EQ_TREBLE_F
	{0x062D, 0x00, FALSE},		// BU32107_EQ_BASS_R
	{0x062F, 0x00, FALSE},		// BU32107_EQ_TREBLE_R

	{0x0703, 0x00, FALSE},		// BU32107_FRONT_HPF
	{0x0704, 0x00, FALSE},		// BU32107_REAR_HPF

	{0x0705, 0x00, FALSE},		// BU32107_FRONT_P2BASS
	{0x0706, 0x00, FALSE},		// BU32107_REAR_P2BASS

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
	{0x040A, 0x00, FALSE},		// BU32107_TA_RL2_H
	{0x040B, 0x00, FALSE},		// BU32107_TA_RL2_L
	{0x040C, 0x00, FALSE},		// BU32107_TA_RR2_H
	{0x040D, 0x00, FALSE},		// BU32107_TA_RR2_L

	{0x0205, 0x00, FALSE},		// BU32107_DSP_SEL_3

	{0x0709, 0x00, FALSE},		// BU32107_IIR

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
static const u8 g_bu32107_atten_table[8] = {0, 3, 6, 9, 12, 16, 20, 63};

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

static void bu32107_update_time_alignment(void)
{
	u16 val;
	u8 cnt;

	for (cnt=0; cnt<AUDIO_SPK_NUMS; cnt++) {
		val = 0;
		if (g_audio_info.space_en) {
			if ( (AUDIO_SPK_FL==cnt) || (AUDIO_SPK_FR==cnt) ) {
				val += g_audio_info.space_gain;
			}
		}
		
		if (1 == g_audio_info.soundfield_expert_mode) {
			// professional sound field mode
			val += g_audio_info.spk_delay[cnt];
		}

		val *= 20;

		if (g_audio_info.surround_mode != AUDIO_SR_MODE_FLAT) {
			if ( (AUDIO_SPK_RL==cnt) || (AUDIO_SPK_RR==cnt) ) {
				val += 5;
			}
		}

		BU32107_UPDATE_REG(BU32107_TA_FL_H+2*cnt, MSB(val));
		BU32107_UPDATE_REG(BU32107_TA_FL_L+2*cnt, LSB(val));
	}

	val = 0;
	if (g_audio_info.surround_mode != AUDIO_SR_MODE_FLAT) {
		val += 9;
	}
	BU32107_UPDATE_REG(BU32107_TA_RL2_H, MSB(val));
	BU32107_UPDATE_REG(BU32107_TA_RL2_L, LSB(val));
	BU32107_UPDATE_REG(BU32107_TA_RR2_H, MSB(val));
	BU32107_UPDATE_REG(BU32107_TA_RR2_L, LSB(val));

	bu32107_update_regs();
}

bool audio_dev_init(void)
{
	u8 i;
	u8 j;
	
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
	bu32107_write(0x0203, 0x08);	// time-alignment 4-ch mode
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
	bu32107_write(0x0600, 0x00);		// 13-Band EQ + TONE
	bu32107_write(0x0610, 0x40);		// Front & Rear
	bu32107_write(0x0611, 0x40);		// Front & Rear
	bu32107_write(0x0612, 0x40);		// Front & Rear
	bu32107_write(0x0613, 0x40);		// Front & Rear
	bu32107_write(0x0614, 0x40);		// Front & Rear
	bu32107_write(0x0615, 0x40);		// Front & Rear
	bu32107_write(0x0616, 0x40);		// Front & Rear
	bu32107_write(0x0617, 0x40);		// Front & Rear
	bu32107_write(0x0618, 0x40);		// Front & Rear
	bu32107_write(0x0619, 0x40);		// Front & Rear
	bu32107_write(0x061A, 0x40);		// Front & Rear
	bu32107_write(0x061B, 0x40);		// Front & Rear
	bu32107_write(0x061C, 0x40);		// Front & Rear
	bu32107_write(0x061D, 0x40);		// Front & Rear
	bu32107_write(0x061E, 0x40);		// Front & Rear
	bu32107_write(0x061F, 0x40);		// Front & Rear
	bu32107_write(0x0620, 0x00);
	bu32107_write(0x0621, 0x00);
	bu32107_write(0x0622, 0x00);
	bu32107_write(0x0623, 0x00);
	bu32107_write(0x0624, 0x00);
	bu32107_write(0x0625, 0x00);
	bu32107_write(0x0626, 0x00);
	bu32107_write(0x0627, 0x00);
	bu32107_write(0x0628, 0x00);
	bu32107_write(0x0629, 0x00);
	bu32107_write(0x062A, 0x00);
	bu32107_write(0x062B, 0x00);
	bu32107_write(0x062C, 0x00);
	bu32107_write(0x062D, 0x00);
	bu32107_write(0x062E, 0x00);
	bu32107_write(0x062F, 0x00);
	bu32107_write(0x0700, 0x00);
	bu32107_write(0x0701, 0x55);		// Loudness: LPF 100HZ, HPF 10KHZ
	bu32107_write(0x0702, 0xA0);		// Loudness: HiBoost=0.55, 0dB
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
	} else if ( (AUDIO_SRC_BT_MODULE==src)&&(g_audio_info.bt_ring_on) ) {
		val = 0+A_BT_PHONE_EXTRA_DB;
	} else if ( (AUDIO_SRC_BT_MODULE==src)&&(g_audio_info.bt_voice_on) ) {
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
	
	bu32107_update_regs();

	audio_dev_update_volume(g_audio_info.cur_vol);
}

void audio_dev_update_volume(u8 vol)
{
	s8 gain;
	u8 val_fader;
	u8 val_dvol_boost;
	s8 gain_sub;
	u8 val_fader_sub;
	u8 val_dvol_boost_sub;

#if MAX_VOLUME == 30
	if (vol>MAX_VOLUME) {
		return;
	}

	gain = g_bu32107_gains[vol];
	gain_sub = g_bu32107_gains[vol];
	if (g_audio_info.subwoofer_on) {
		gain_sub += g_audio_info.subwoofer;
	}

	if ( (g_audio_info.loud_on) && (AUDIO_SRC_NONE != g_audio_info.cur_source) ) {
		// add loudness compansation
		gain += BU32107_LOUDNESS_STEP*g_audio_info.loudness;
		gain_sub += BU32107_LOUDNESS_STEP*g_audio_info.loudness;
	}

	if (2==g_bt_type) {
		if (SOURCE_BT == FrontSource) {
			gain -= 14;
		}
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
	if (g_audio_info.bt_phone_on || g_audio_info.bt_ring_on) {
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

	if (gain_sub>=0) {
		val_fader_sub = 0xA0;
		val_dvol_boost_sub = 0x80-gain_sub*2;
	} else {
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
	} else if (g_audio_info.bt_ring_on) {
		bu32107_update_mix_vol(0);
	} else if (g_audio_info.bt_voice_on) {
		bu32107_update_mix_vol(0);
	} else if (g_audio_info.navi_break_on) {
		bu32107_update_mix_vol(vol);
	}
}

// update EQ settings according to the AUDIO_INFO's eq_cur_freq & eq_cur_level member

static u8 bu32107_calc_eq_level_to_reg_gain(u8 val, u8 level)
{
	val &= ~0x1F;
	if (level > MAX_EQ_LEVEL) {
		level = MAX_EQ_LEVEL;
	}

	if (level >= DEFAULT_EQ_LEVEL) {
		val += (level-DEFAULT_EQ_LEVEL);
	} else {
		val += (DEFAULT_EQ_LEVEL-level);
		val |= (1<<4);
	}

	return val;
}

void audio_dev_update_eq(void)
{
	u8 val;
	u8 reg_id;
	u8 eq_id;
	u8 level;

	reg_id = BU32107_EQ_80HZ;

	for (eq_id=EQ_FREQ_80HZ; eq_id<EQ_FREQ_NUMS; eq_id++) {
		switch (eq_id) {
			case EQ_FREQ_200HZ_FAKE:
			case EQ_FREQ_1K25HZ_FAKE:
			case EQ_FREQ_8KHZ_FAKE:
				level = g_audio_info.eq_visible_level[eq_id];
				++eq_id;
				level = level+g_audio_info.eq_visible_level[eq_id];
				level = level / 2;
				break;
			default:
				level = g_audio_info.eq_visible_level[eq_id];
				break;
		}
		val = bu32107_calc_eq_level_to_reg_gain(
				g_regs[reg_id].value, 
				level);
		BU32107_UPDATE_REG(reg_id, val);
		++reg_id;
	}

	bu32107_update_regs();	
}

void audio_dev_update_fader_balance(u8 fad, u8 bal)
{
	u8 atten[AUDIO_SPK_NUMS];
	u8 val;
	u8 cnt;

	for (cnt=0; cnt<AUDIO_SPK_NUMS; cnt++) {
		atten[cnt] = 0;
	}

	if (0 == g_audio_info.soundfield_expert_mode) {
		// normal sound field mode
		if (!g_audio_info.disabled_soundfield) {
			if (fad<=7) {
				atten[AUDIO_SPK_RL] += g_bu32107_atten_table[7-fad];
				atten[AUDIO_SPK_RR] += g_bu32107_atten_table[7-fad];
			} else {
				atten[AUDIO_SPK_FL] += g_bu32107_atten_table[fad-7];
				atten[AUDIO_SPK_FR] += g_bu32107_atten_table[fad-7];
			}
			if (bal<=7) {
				atten[AUDIO_SPK_FR] += g_bu32107_atten_table[7-bal];
				atten[AUDIO_SPK_RR] += g_bu32107_atten_table[7-bal];
			} else {
				atten[AUDIO_SPK_FL] += g_bu32107_atten_table[bal-7];
				atten[AUDIO_SPK_RL] += g_bu32107_atten_table[bal-7];
			}
		}
	}

	if ((!g_audio_info.bt_phone_on)&&(!g_audio_info.bt_ring_on)&&(!g_audio_info.bt_voice_on)&&(g_audio_info.navi_break_on)) {
		if (NAVI_BREAK_DIRECT==g_audio_info.navi_mix_extra_gain) {	// navi break directly
			// mute the front speaker
			atten[AUDIO_SPK_FL] = 97;
			atten[AUDIO_SPK_FR] = 97;
		} else {	// navi mix
			// cut down the front speaker
			atten[AUDIO_SPK_FL] += VOLUME_CUT_WHEN_NAVI_MIX;
			atten[AUDIO_SPK_FR] += VOLUME_CUT_WHEN_NAVI_MIX;
		}
	}

	for (cnt=0; cnt<AUDIO_SPK_NUMS; cnt++) {
		// make sure no overflow
		if (atten[cnt]>97)	atten[cnt]=97;

		// turn step: 1dB -> 0.5dB
		atten[cnt] = atten[cnt]*2;

		if (1 == g_audio_info.soundfield_expert_mode) {
			// professional sound field mode

			// speak fader if needed, in DSP advanced settings 2
			atten[cnt] += (DSP_SPK_GAIN_MAX - g_audio_info.spk_gain[cnt]);

			// turn off speak if needed, in DSP advanced settings 2
			if (g_audio_info.spk_on[cnt] == FALSE) {
				atten[cnt] = 194;
			}
		}

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

	if (!g_audio_info.loud_on) {
		loud = 0;
	}

	// LOUD on->off, set volume early
	if (0==loud) {
		audio_dev_update_volume(g_audio_info.cur_vol);
	}

	val = g_regs[BU32107_LOUDNESS].value;
	val &= ~0x0F;
	val |= BU32107_LOUDNESS_STEP*g_audio_info.loudness;
	BU32107_UPDATE_REG(BU32107_LOUDNESS, val);

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

void audio_dev_update_dsp_settings_1(void)
{
	u8 val;

	// HPF
	if (g_audio_info.hpf_on) {
		val = g_audio_info.hpf_freq;
	} else {
		val = 0;
	}
	BU32107_UPDATE_REG(BU32107_FRONT_HPF, val);
	BU32107_UPDATE_REG(BU32107_REAR_HPF, val);

	// BASS
	val = 0x80;
	if (g_audio_info.dsp_bass_on) {
		val |= (g_audio_info.dsp_bass_freq << 4);

		if (g_audio_info.dsp_bass_gain > 12) {
			val |= 0x0C;
		} else {
			val |= g_audio_info.dsp_bass_gain;
		}
	} else {
		val = 0x80;
	}
	BU32107_UPDATE_REG(BU32107_FRONT_P2BASS, val);
	BU32107_UPDATE_REG(BU32107_REAR_P2BASS, val);

	// PHAT
	// fake with EQ 50HZ
	if (g_audio_info.phat_en) {
		val = 0x40 + g_audio_info.phat_gain;
	} else {
		val = 0x40;
	}
	BU32107_UPDATE_REG(BU32107_EQ_50HZ, val);

	// CORE
	// fake with EQ middle
	if (g_audio_info.core_en) {
		val = 0x70 + g_audio_info.core_gain;
	} else {
		val = 0x70;
	}
	BU32107_UPDATE_REG(BU32107_EQ_MID, val);

	// SPACE
	// fake with time-alignment
	bu32107_update_time_alignment();

	
	bu32107_update_regs();

}

void audio_dev_update_dsp_settings_2(void)
{
	u8 val;

	audio_dev_update_fader_balance(g_audio_info.fader, g_audio_info.balance);

	// some surround mode need modify delay
	bu32107_update_time_alignment();

	// surround mode
	switch (g_audio_info.surround_mode) {
		case AUDIO_SR_MODE_FLAT:
			val = 0x05;	// time-alignment -> p2bass rear
			BU32107_UPDATE_REG(BU32107_DSP_SEL_3, val);

			val = 0x40;	// front/rear -HZ 0dB
			BU32107_UPDATE_REG(BU32107_EQ_BASS_F, val);
			BU32107_UPDATE_REG(BU32107_EQ_TREBLE_F, val);

			val = 0x00;	// IIR through
			BU32107_UPDATE_REG(BU32107_IIR, val);
			break;
		case AUDIO_SR_MODE_RECITAL:
			val = 0x25;	// surround -> p2bass rear
			BU32107_UPDATE_REG(BU32107_DSP_SEL_3, val);

			val = 0x12;	// 63HZ 4dB
			BU32107_UPDATE_REG(BU32107_EQ_BASS_F, val);
			val = 0x02;	// 2.5K 4dB
			BU32107_UPDATE_REG(BU32107_EQ_TREBLE_F, val);
			val = 0x12;	// 63HZ 4dB
			BU32107_UPDATE_REG(BU32107_EQ_BASS_R, val);
			val = 0x00;	// -HZ 0dB
			BU32107_UPDATE_REG(BU32107_EQ_TREBLE_R, val);

			val = 0x80;	// IIR coef
			BU32107_UPDATE_REG(BU32107_IIR, val);
			break;
		case AUDIO_SR_MODE_CONCERT:
			val = 0x25;	// surround -> p2bass rear
			BU32107_UPDATE_REG(BU32107_DSP_SEL_3, val);

			val = 0x22;	// 100HZ 4dB
			BU32107_UPDATE_REG(BU32107_EQ_BASS_F, val);
			val = 0x22;	// 6.3K 4dB
			BU32107_UPDATE_REG(BU32107_EQ_TREBLE_F, val);
			val = 0x22;	// 100HZ 4dB
			BU32107_UPDATE_REG(BU32107_EQ_BASS_R, val);
			val = 0x22;	// 6.3K 4dB
			BU32107_UPDATE_REG(BU32107_EQ_TREBLE_R, val);

			val = 0x80;	// IIR coef
			BU32107_UPDATE_REG(BU32107_IIR, val);
			break;
		case AUDIO_SR_MODE_BGM:
			val = 0x25;	// surround -> p2bass rear
			BU32107_UPDATE_REG(BU32107_DSP_SEL_3, val);

			val = 0x2A;	// 100HZ -4dB
			BU32107_UPDATE_REG(BU32107_EQ_BASS_F, val);
			val = 0x00;	// -HZ 0dB
			BU32107_UPDATE_REG(BU32107_EQ_TREBLE_F, val);
			val = 0x00;	// -HZ 0dB
			BU32107_UPDATE_REG(BU32107_EQ_BASS_R, val);
			val = 0x3A;	// 10K -4dB
			BU32107_UPDATE_REG(BU32107_EQ_TREBLE_R, val);

			val = 0x80;	// IIR coef
			BU32107_UPDATE_REG(BU32107_IIR, val);
			break;
		case AUDIO_SR_MODE_MOVIE:
			val = 0x25;	// surround -> p2bass rear
			BU32107_UPDATE_REG(BU32107_DSP_SEL_3, val);

			val = 0x22;	// 100HZ 4dB
			BU32107_UPDATE_REG(BU32107_EQ_BASS_F, val);
			val = 0x03;	// 2.5K 6dB
			BU32107_UPDATE_REG(BU32107_EQ_TREBLE_F, val);
			val = 0x22;	// 100HZ 4dB
			BU32107_UPDATE_REG(BU32107_EQ_BASS_R, val);
			val = 0x00;	// -HZ 0dB
			BU32107_UPDATE_REG(BU32107_EQ_TREBLE_R, val);

			val = 0x80;	// IIR coef
			BU32107_UPDATE_REG(BU32107_IIR, val);
			break;
		case AUDIO_SR_MODE_DRAMA:
			val = 0x25;	// surround -> p2bass rear
			BU32107_UPDATE_REG(BU32107_DSP_SEL_3, val);

			val = 0x00;	// -HZ 0dB
			BU32107_UPDATE_REG(BU32107_EQ_BASS_F, val);
			val = 0x03;	// 2.5K 6dB
			BU32107_UPDATE_REG(BU32107_EQ_TREBLE_F, val);
			val = 0x02;	// 40HZ 4dB
			BU32107_UPDATE_REG(BU32107_EQ_BASS_R, val);
			val = 0x00;	// -HZ 0dB
			BU32107_UPDATE_REG(BU32107_EQ_TREBLE_R, val);

			val = 0x00;	// IIR through
			BU32107_UPDATE_REG(BU32107_IIR, val);
			break;
	}
	
	bu32107_update_regs();
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

