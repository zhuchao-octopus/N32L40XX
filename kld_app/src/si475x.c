
#include "public.h"

#if TUNER_MODEL==TUNER_SILICONLAB_SI475X
#include "si475x.h"

#define XCLOAD 32

static RADIO_BAND g_si475x_band;
static RADIO_DEV_SEEK_MODE g_si475x_seek_mode = RDSM_DX;
static uint8_t  g_si475x_cmd[CMD_BUFFER_SIZE];
static uint8_t  g_si475x_rsp[RSP_BUFFER_SIZE];


void si475x_i2c_write(uint8_t* src_addr, uint8_t len)
{
	uint8_t cnt;
	
	i2c_start();
	
	i2c_send_byte(SI475X_I2C_ADDR_W);
	i2c_wait_ack();
	
	for (cnt=0;cnt<len;cnt++) {
		i2c_send_byte(src_addr[cnt]);
		i2c_wait_ack();
	}
 	i2c_stop();
}

void si475x_i2c_read (uint8_t* dest_addr, uint8_t len)
{
	uint8_t cnt;

	i2c_start();
	
	i2c_send_byte(SI475X_I2C_ADDR_R);
	i2c_wait_ack();

	for (cnt=0;cnt<len;cnt++) {
		if (cnt==(len-1)) {
	   	dest_addr[cnt] = i2c_read_byte(0);
		} else {
	   	dest_addr[cnt] = i2c_read_byte(1);
		}
	}
 	i2c_stop();

}

//-----------------------------------------------------------------------------
// Command that will wait for CTS before returning
//-----------------------------------------------------------------------------
void si475x_waitForCTS()
{
	uint16_t i = 20000;
	uint8_t status;

	do
	{
		CLEAR_WATCHDOG;
		si475x_i2c_read(&status, 1);
		delay_us(5);
	}while (--i && !(status & CTS));
}

//-----------------------------------------------------------------------------
// Sends a command to the part and returns the reply bytes
//-----------------------------------------------------------------------------
void si475x_command(uint8_t cmd_size, uint8_t *cmd, uint8_t reply_size, uint8_t *reply)
{
	// It is always a good idea to check for cts prior to sending a command to
	// the part.
	if ( (POWER_UP!=cmd[0]) && (0xFB!=cmd[0]) ) {
		si475x_waitForCTS();
	}

	// Write the command to the part
	si475x_i2c_write(cmd, cmd_size);

	// Wait for CTS after sending the command
	if ( (POWER_UP!=cmd[0]) && (0xFB!=cmd[0]) ) {
		si475x_waitForCTS();
	}

	// If the calling function would like to have results then read them.
	if(reply_size)
	{
		si475x_i2c_read(reply, reply_size);
	}
}

void si475x_set_property(uint16_t propNumber, uint16_t propValue)
{
	// Put the ID for the command in the first byte.
	g_si475x_cmd[0] = SET_PROPERTY;

	// Initialize the reserved section to 0
	g_si475x_cmd[1] = 0;

	// Put the property number in the third and fourth bytes.
	g_si475x_cmd[2] = (uint8_t)(propNumber >> 8);
	g_si475x_cmd[3] = (uint8_t)(propNumber & 0x00FF);

	// Put the property value in the fifth and sixth bytes.
	g_si475x_cmd[4] = (uint8_t)(propValue >> 8);
	g_si475x_cmd[5] = (uint8_t)(propValue & 0x00FF);

	// Invoke the command
	si475x_command(6, g_si475x_cmd, 0, NULL);
}


void si475x_powerup(RADIO_BAND band)
{
	/* do reset */
	GPIO_ResetBits(GPIO_RADIO_RST_GRP, GPIO_RADIO_RST_PIN);
	delay_1ms(2);
	GPIO_SetBits(GPIO_RADIO_RST_GRP, GPIO_RADIO_RST_PIN);
	delay_1ms(2);

	/* power up */
	// Put the ID for the command in the first byte.
	g_si475x_cmd[0] = POWER_UP;

	// 4MHZ crystal
	g_si475x_cmd[1] = 0x77;

	// xcload
	g_si475x_cmd[2] = XCLOAD;

	// This value is for starting up the 4MHz crystal.
	g_si475x_cmd[3] = 0x05;  

	// 0=4MHz (Si475x only), 1=37.209375MHz, 2=36.4MHz, 3=37.8MHz
	g_si475x_cmd[4] = 0;

	if (RADIO_BAND_FM == band) {
		g_si475x_cmd[4] |= (1 /*FMRX*/ << 4);
	} else {
		g_si475x_cmd[4] |= (2 /*AMRX*/ << 4);
	}

	// This is zero initially to start up the crystal.
	g_si475x_cmd[5] = 0x00;

	// Uncomment out this code if a resistor is not connected to the crystal.
	g_si475x_cmd[5] |= 0x01;

	// Power up the device.  DO NOT WAIT FOR CTS as it will not be set until
	// after the boot sequence is complete.
	si475x_command(6, g_si475x_cmd, 0, NULL);

	// Wait for a specified amount of time for the crystal to boot.
	delay_1ms(40);

	// Write this command to force CTS for the next power up command.  
	// DO NOT WAIT FOR CTS.
	g_si475x_cmd[0] = 0xFB;
	g_si475x_cmd[1] = 0x06;
	g_si475x_cmd[2] = 0x80;
	si475x_command(3, g_si475x_cmd, 0, NULL);


	// Set the values back in the command for the final powerup.
	g_si475x_cmd[0] = POWER_UP;
	g_si475x_cmd[1] = 0x77;
	g_si475x_cmd[2] = XCLOAD;

	// This value is the final crystal value for 4MHz.
	g_si475x_cmd[3] = 0x03;  
	
	// Comment out the following line if a resistor is not connected to the crystal.
	// cmd[3] += 0x20;

	// Set the 4th parameter to the clock frequency.
	g_si475x_cmd[4] = 0;  // 0=4MHz (Si475x only), 1=37.209375MHz, 2=36.4MHz, 3=37.8MHz

	if (RADIO_BAND_FM == band) {
		g_si475x_cmd[4] |= (1 /*FMRX*/ << 4);
	} else {
		g_si475x_cmd[4] |= (2 /*AMRX*/ << 4);
	}

	// This is the value required for final boot with a 4MHz crystal.
	g_si475x_cmd[5] = 0x11;

	// Powerup the device
	si475x_command(6, g_si475x_cmd, 1, g_si475x_rsp);
	delay_1ms(110);               // wait for si475x/6x to powerup

	/* configure */

	// ***************************************
	// Analog Output
	// ***************************************
	// Configure the command array to output analog audio through the analog
	// audio pins
	g_si475x_cmd[0] = ANA_AUDIO_PIN_CFG;
	g_si475x_cmd[1] = 2;  // Audio Out
	si475x_command(2, g_si475x_cmd, 0, NULL);

	if (RADIO_BAND_FM == band) {

		// Enable the RDS and STC interrupt here
		si475x_set_property(INT_CTL_ENABLE, 0x0005);

		// Typically the settings used for seek are determined by the designer
		// and not exposed to the end user. They should be adjusted here.
		si475x_set_property(FM_VALID_SNR_THRESHOLD, 8);
		si475x_set_property(FM_VALID_RSSI_THRESHOLD, 12);

		// This interrupt will be used to determine when RDS is available.
		si475x_set_property(FM_RDS_INTERRUPT_SOURCE, 0x0002); // RDS Sync Interrupt

		// Enable the RDS and allow all blocks so we can compute the error
		// rate later.
		si475x_set_property(FM_RDS_CONFIG, 0x00F1);

		si475x_set_property(AUDIO_DE_EMPHASIS, 1); // 0 = 75us, 1 = 50us
		// Band is already set to 87.5-107.9MHz (Europe)
		si475x_set_property(FM_SEEK_FREQUENCY_SPACING, 10); // 100 kHz Spacing

		// Setup the RDS Interrupt to interrupt when RDS data is available (RDSRECV).
		si475x_set_property(FM_RDS_INTERRUPT_SOURCE, 0x0001);
		si475x_set_property(FM_RDS_INTERRUPT_FIFO_COUNT, 1);

		// Setup the RDS configuration to enable RDS and allow any error condition.
		si475x_set_property(FM_RDS_CONFIG, 0x00F1);

	} else {
		// Enable the STC interrupt here
		si475x_set_property(INT_CTL_ENABLE, 0x0001);

		// Typically the settings used for seek are determined by the designer
		// and not exposed to the end user. They should be adjusted here.
		si475x_set_property(AM_VALID_SNR_THRESHOLD, 5);
		si475x_set_property(AM_VALID_RSSI_THRESHOLD, 10);

		si475x_set_property(AM_SEEK_FREQUENCY_SPACING, 9); // Set spacing to 9kHz
		si475x_set_property(AM_SEEK_BAND_BOTTOM, 520); // Set the band bottom to 520kHz
		si475x_set_property(AM_SEEK_BAND_TOP, 1710);   // Set the band top to 1710kHz
	}

	// Turn off the mute
	si475x_set_property(AUDIO_MUTE, 0);

	// Set the volume
	si475x_set_property(AUDIO_ANALOG_VOLUME, 63);

}

//#define TEST_GET_REV
bool radio_dev_init(void) 
{
#ifdef TEST_GET_REV
	char revision;
	uint8_t partNumber;
	char partMajor;
	char partMinor;
	char partBuild;
	uint8_t   romID;
#endif

	g_si475x_band = RADIO_BAND_FM;
	si475x_powerup(g_si475x_band);

#ifdef TEST_GET_REV
	/* Get the reversion info */
	g_si475x_cmd[0] = PART_INFO;
	si475x_command(1, g_si475x_cmd, 9, g_si475x_rsp);
	revision   = (char)g_si475x_rsp[1];
	partNumber = g_si475x_rsp[2];
	partMajor  = (char)g_si475x_rsp[3];
	partMinor  = (char)g_si475x_rsp[4];
	partBuild  = (char)g_si475x_rsp[5];
	romID      = g_si475x_rsp[8];
#endif


	return TRUE;
}

void radio_dev_rds_init(void)
{
}

bool radio_dev_deinit(void) 
{
	return TRUE;
}
bool radio_dev_is_set_freq_done(void)
{
	g_si475x_cmd[0] = GET_INT_STATUS;
	si475x_command(1, g_si475x_cmd, 1, g_si475x_rsp);
	return (g_si475x_rsp[0] & STCINT);
}
void radio_dev_set_freq(RADIO_BAND band, u16 freq)
{
	if (band != g_si475x_band) {
		g_si475x_band = band;
		si475x_powerup(g_si475x_band);
	}
	
	if (RADIO_BAND_FM == g_si475x_band) {
		// Put the ID for the command in the first byte.
		g_si475x_cmd[0] = FM_TUNE_FREQ;

		// Depending on what tune functionality is needed or in the appropriate bits
		g_si475x_cmd[1] = 0;

		// Determine if the metrics should be smoothly transitioned after a tune
//		g_si475x_cmd[1] |= 0; // 0 = Metrics match new channel, 4 = trans. from prev. channel

		// Set the tune mode
//		g_si475x_cmd[1] |= 0; // Normal tune

		// Determine if HD ready mode is needed
//		g_si475x_cmd[1] |= 0; // 0 = Normal bandwidth, 0x40 = wide bandwidth/HD mode

		// Put the frequency in the second and third bytes.
		g_si475x_cmd[2] = (uint8_t)(freq >> 8);
		g_si475x_cmd[3] = (uint8_t)(freq & 0x00FF);

		// Set the antenna calibration value.
		g_si475x_cmd[4] = (uint8_t)0;  // Auto
		g_si475x_cmd[5] = (uint8_t)0;

		// Invoke the command
		si475x_command(6, g_si475x_cmd, 1, g_si475x_rsp);
	} else {
		// Put the ID for the command in the first byte.
		g_si475x_cmd[0] = AM_TUNE_FREQ;

		// Depending on what tune functionality is needed or in the appropriate bits
		g_si475x_cmd[1] = 0;

		// Determine if HD ready mode is needed (Si4761/63/65/67 only)
//		g_si475x_cmd[1] |= 0; // 0 = Normal bandwidth, 0x40 = wide bandwidth/HD mode

		// Put the frequency in the second and third bytes.
		g_si475x_cmd[2] = (uint8_t)(freq >> 8);
		g_si475x_cmd[3] = (uint8_t)(freq & 0x00FF);

		// Set the antenna calibration value.
		g_si475x_cmd[4] = (uint8_t)0;  // Auto
		g_si475x_cmd[5] = (uint8_t)0;

		// Invoke the command
		si475x_command(6, g_si475x_cmd, 1, g_si475x_rsp);
	}
}

void radio_dev_set_freq_tune(RADIO_BAND band, u16 freq)
{
	radio_dev_set_freq(band, freq);
}

bool radio_dev_is_tune_ok(bool strict)
{
	if (RADIO_BAND_FM == g_si475x_band) {
		g_si475x_cmd[0] = FM_RSQ_STATUS;
		g_si475x_cmd[1] = 0x05;	// intack & attune
		si475x_command(2, g_si475x_cmd, 16, g_si475x_rsp);

		if (RDSM_DX == g_si475x_seek_mode) {
			return (g_si475x_rsp[2] & 0x01);
		} else {
			if (g_si475x_rsp[2] & 0x01) {
				return ( (g_si475x_rsp[6]>18) && (g_si475x_rsp[6]<0x7F) );
			} else {
				return 0;
			}
		}
	} else {
		g_si475x_cmd[0] = AM_RSQ_STATUS;
		g_si475x_cmd[1] = 0x05;	// intack & attune
		si475x_command(2, g_si475x_cmd, 13, g_si475x_rsp);
		return (g_si475x_rsp[2] & 0x01);
	}
}
bool radio_dev_is_tune_status_ready(void)
{

	return TRUE;
}
u8 radio_dev_get_stereo_status(void)
{
	uint8_t blend;
	if (RADIO_BAND_FM != g_si475x_band) {
		return 1;
	}

	g_si475x_cmd[0] = FM_ACF_STATUS;
	g_si475x_cmd[1] = 1;
	si475x_command(2, g_si475x_cmd, 8, g_si475x_rsp);

	blend = g_si475x_rsp[7];
	if ( (blend&0x80) && ((blend&0x7F)>10) ) {
		return 2;
	} else {
		return 1;
	}
}

bool radio_dev_set_seek_mode(RADIO_DEV_SEEK_MODE mode)
{
	g_si475x_seek_mode = mode;
	return TRUE;
}

bool radio_dev_set_st_mode(bool on)
{
	if (0 == on) {
		si475x_set_property(FM_BLEND_RSSI_STEREO_SEP, 0x4040);
	} else {
		si475x_set_property(FM_BLEND_RSSI_STEREO_SEP, 0x2D00);
	}
	return TRUE;
}

bool radio_dev_set_mute(bool mute)
{
	if (mute) {
		si475x_set_property(AUDIO_MUTE, 0x03);
	} else {
		si475x_set_property(AUDIO_MUTE, 0);
	}
	return TRUE;
}

#if RDS_FUNCTION==1
// RDS Block x Corrected Errors
#define CORRECTED_NONE		0
#define CORRECTED_SMALL	1
#define CORRECTED_LARGE	2
#define UNCORRECTABLE		3

bool radio_dev_rds_update(void)
{
	u8 bleA, bleB, bleC, bleD;
	u8 group_type;
	u8 addr;

	g_si475x_cmd[0] = FM_RDS_STATUS;
	g_si475x_cmd[1] = 0x01;       // INTACK
	si475x_command(2, g_si475x_cmd, 16, g_si475x_rsp);
	if (0==g_si475x_rsp[6]) {
		return FALSE;
	}

	bleA = (g_si475x_rsp[7] & 0x00C0) >> 6;
	bleB = (g_si475x_rsp[7] & 0x0030) >> 4;
	bleC = (g_si475x_rsp[7] & 0x000C) >> 2;
	bleD = (g_si475x_rsp[7] & 0x0003) >> 0;

	// check RDS Block B Corrected Errors
	// It's critical that block B be correct since it determines what's
	// contained in the latter blocks. For this reason, a stricter tolerance
	// is placed on block B
	if (CORRECTED_SMALL < bleB) {
		// Drop the data if more than two errors were uncorrected in block B
		return FALSE;
	}

	g_rds_info.block_a = ((u16)g_si475x_rsp[8]  << 8) | (u16)g_si475x_rsp[9];
	g_rds_info.block_b = ((u16)g_si475x_rsp[10]  << 8) | (u16)g_si475x_rsp[11];
	g_rds_info.block_c = ((u16)g_si475x_rsp[12]  << 8) | (u16)g_si475x_rsp[13];
	g_rds_info.block_d = ((u16)g_si475x_rsp[14]  << 8) | (u16)g_si475x_rsp[15];

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
	rds_update_tp(0!=(g_rds_info.block_b & (1<<10)));

	switch (group_type) 
	{
		case RDS_TYPE_0A:
			if (CORRECTED_LARGE >= bleC) {
				rds_update_af_list(g_rds_info.block_c);
			}
			// fallthrough
		case RDS_TYPE_0B:
			rds_update_ta(0!=(g_rds_info.block_b & (1<<4)));
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

