
#ifndef _SI475X_H_
#define _SI475X_H_

#define SI475X_I2C_ADDR_W       0xC0
#define SI475X_I2C_ADDR_R       0xC1

#define CMD_BUFFER_SIZE 16
#define RSP_BUFFER_SIZE 16


//==================================================================
// General Commands
//==================================================================

// STATUS bits - Used by all methods
#define STCINT  0x01
#define ACFINT  0x02
#define RDSINT  0x04
#define SAMEINT 0x04
#define RSQINT  0x08
#define ASQINT  0x10
#define ERR     0x40
#define CTS     0x80

// Boot Commands
#define POWER_UP          0x01
#define PART_INFO         0x02

// Common Commands
#define POWER_DOWN        0x11
#define FUNC_INFO         0x12
#define SET_PROPERTY      0x13
#define GET_PROPERTY      0x14
#define GET_INT_STATUS    0x15
#define AGC_STATUS        0x17
#define DIG_AUDIO_PIN_CFG 0x18
#define ZIF_PIN_CFG       0x19
#define IC_LINK_PIN_CFG   0x1A
#define ANA_AUDIO_PIN_CFG 0x1B

// FM Receive Commands
#define FM_TUNE_FREQ      0x30
#define FM_SEEK_START     0x31
#define FM_RSQ_STATUS     0x32
#define FM_ACF_STATUS     0x35

// FM Receive RDS Commands
#define FM_RDS_STATUS     0x36
#define FM_RDS_BLOCKCOUNT 0x37

// AM Receive Commands
#define AM_TUNE_FREQ      0x40
#define AM_SEEK_START     0x41
#define AM_RSQ_STATUS     0x42
#define AM_ACF_STATUS     0x45
#define AM_AGC_STATUS     0x47

// WB Receive Commands
#define WB_TUNE_FREQ      0x50
#define WB_SEEK_START     0x51
#define WB_RSQ_STATUS     0x52
#define WB_ASQ_STATUS     0x54
#define WB_ACF_STATUS     0x55

// WB Receive SAME Commands
#define WB_SAME_STATUS    0x56
#define WB_SAME_SOFTBYTE  0x57



// General Properties
#define INT_CTL_ENABLE                         0x0000
#define AUDIO_ANALOG_VOLUME                    0x0300
#define AUDIO_MUTE                             0x0301

// AM Receive Properties
#define AM_SEEK_BAND_BOTTOM                    0x1100
#define AM_SEEK_BAND_TOP                       0x1101
#define AM_SEEK_FREQUENCY_SPACING              0x1102
#define AM_VALID_SNR_THRESHOLD                 0x2003
#define AM_VALID_RSSI_THRESHOLD                0x2004

// FM Receive Properties
#define AUDIO_DE_EMPHASIS                      0x0302
#define FM_AGC_FE_CONFIG                       0x0700
#define FM_AGC_PD_CONFIG                       0x0701
#define FM_SEEK_BAND_BOTTOM                    0x1100
#define FM_SEEK_BAND_TOP                       0x1101
#define FM_SEEK_FREQUENCY_SPACING              0x1102
#define FM_VALID_SNR_THRESHOLD                 0x2003
#define FM_VALID_RSSI_THRESHOLD                0x2004
#define FM_CAL_CAPACITOR                       0x8000
#define FM_CAL_FMAX                            0x8001
#define FM_BLEND_RSSI_STEREO_SEP        0x3501

// FM RDS Receive Properties
#define FM_RDS_INTERRUPT_SOURCE               0x4000
#define FM_RDS_INTERRUPT_FIFO_COUNT           0x4001
#define FM_RDS_CONFIG                         0x4002

#endif  // _SI475X_H_

