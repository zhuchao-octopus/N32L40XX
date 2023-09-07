
#ifndef _BU32107_H_
#define _BU32107_H_

#define BU32107_I2C_W_ADDR      0x80
#define BU32107_I2C_R_ADDR      0x81

typedef enum
{
    BU32107_DSP_SEL_1=0,            // 0x0203
    BU32107_INIT_SETUP_2,           // 0x0002
    BU32107_DIG_MIX_SEL,            // 0x0109
    BU32107_AVOL_DMIX,          // 0x010A
    BU32107_ANA_INPUT_SEL,      // 0x0103

    BU32107_FADER_VOL_FL,           // 0x0A00
    BU32107_FADER_VOL_FR,           // 0x0A01
    BU32107_FADER_VOL_RL,           // 0x0A02
    BU32107_FADER_VOL_RR,           // 0x0A03
    BU32107_FADER_VOL_SL,           // 0x0A04

    BU32107_DVOL_BOOST_FL,      // 0x0906
    BU32107_DVOL_BOOST_FR,      // 0x0907
    BU32107_DVOL_BOOST_RL,      // 0x0908
    BU32107_DVOL_BOOST_RR,      // 0x0909
    BU32107_DVOL_BOOST_SL,      // 0x090A
    BU32107_DVOL_BOOST_SR,      // 0x090B

    BU32107_DVOL_ATT_FL,            // 0x0900
    BU32107_DVOL_ATT_FR,            // 0x0901
    BU32107_DVOL_ATT_RL,            // 0x0902
    BU32107_DVOL_ATT_RR,            // 0x0903

    BU32107_EQ_50HZ,                // 0x0610
    BU32107_EQ_80HZ,                // 0x0611
    BU32107_EQ_125HZ,               // 0x0612
    BU32107_EQ_200HZ,               // 0x0613
    BU32107_EQ_315HZ,               // 0x0614
    BU32107_EQ_500HZ,               // 0x0615
    BU32107_EQ_800HZ,               // 0x0616
    BU32107_EQ_1K25,                // 0x0617
    BU32107_EQ_2K,                  // 0x0618
    BU32107_EQ_3K15,                // 0x0619
    BU32107_EQ_5K,                  // 0x061A
    BU32107_EQ_8K,                  // 0x061B
    BU32107_EQ_12K5,                // 0x061C
    BU32107_EQ_BASS_F,          // 0x061D
    BU32107_EQ_MID,             // 0x061E
    BU32107_EQ_TREBLE_F,            // 0x061F
    BU32107_EQ_BASS_R,          // 0x062D
    BU32107_EQ_TREBLE_R,            // 0x062F

    BU32107_FRONT_HPF,          // 0x0703
    BU32107_REAR_HPF,               // 0x0704

    BU32107_FRONT_P2BASS,           // 0x0705
    BU32107_REAR_P2BASS,            // 0x0706

    BU32107_LOUDNESS,               // 0x0702

    BU32107_INPUT_GAIN,         // 0x0101

    BU32107_TA_FL_H,                // 0x0400
    BU32107_TA_FL_L,                // 0x0401
    BU32107_TA_FR_H,                // 0x0402
    BU32107_TA_FR_L,                // 0x0403
    BU32107_TA_RL_H,                // 0x0404
    BU32107_TA_RL_L,                // 0x0405
    BU32107_TA_RR_H,                // 0x0406
    BU32107_TA_RR_L,                // 0x0407
    BU32107_TA_RL2_H,               // 0x040A
    BU32107_TA_RL2_L,               // 0x040B
    BU32107_TA_RR2_H,               // 0x040C
    BU32107_TA_RR2_L,               // 0x040D

    BU32107_DSP_SEL_3,          // 0x0205

    BU32107_IIR,                    // 0x0709

    BU32107_DIG_IO_SEL3,            // 0x0202

    BU32107_REG_NUMS    
}BU32107_REGS;

typedef struct
{
    u16 addr;
    u8 value;
    bool changed;
}BU32107_REG_INFO;

#define BU32107_LOUDNESS_STEP   1

#endif  /* _BU32107_H_ */

