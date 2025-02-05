
#define TDA7708_I2C_ADDR_W  0xC2
#define TDA7708_I2C_ADDR_R      0xC3

#define TDA7708_I2C_DATA_BUFF_MAX_NUM   64

#define MODE_DIRECT_WRITE       0xE0
#define MODE_DIRECT_READ        0x60
#define MODE_CMD_WRITE      0xF0
#define MODE_CMD_READ           0x70

typedef enum
{
    TDA7708_STATUS_NONE = 0,
    TDA7708_STATUS_DOWNLOADING,
    TDA7708_STATUS_WAIT_INIT,
    TDA7708_STATUS_CHECK_INIT,
    TDA7708_STATUS_READY,
    TDA7708_STATUS_NUMS
}TDA7708_STATUS;

#define TDA7708_CMD_HEAD_POS_H  0
#define TDA7708_CMD_HEAD_POS_M  1
#define TDA7708_CMD_HEAD_POS_L  2
#define TDA7708_CMD_PARAM1_POS_H        3
#define TDA7708_CMD_PARAM1_POS_M        4
#define TDA7708_CMD_PARAM1_POS_L        5
#define TDA7708_CMD_PARAM2_POS_H        6
#define TDA7708_CMD_PARAM2_POS_M        7
#define TDA7708_CMD_PARAM2_POS_L        8
#define TDA7708_CMD_PARAM3_POS_H        9
#define TDA7708_CMD_PARAM3_POS_M        10
#define TDA7708_CMD_PARAM3_POS_L        11
#define TDA7708_CMD_PARAM4_POS_H        12
#define TDA7708_CMD_PARAM4_POS_M        13
#define TDA7708_CMD_PARAM4_POS_L        14


// RDS block error count
#define CORRECTED_NONE      0
#define CORRECTED_SMALL 1
#define CORRECTED_LARGE 2
#define UNCORRECTABLE       6
#define UNAVAILABLE         7




//Command Code
//Basic commands
#define CMD_CODE_TUNER_PING                     0x00
#define CMD_CODE_TUNER_READ                     0x1E
#define CMD_CODE_TUNER_WRITE                    0x1F
#define CMD_CODE_TUNER_CHANGE_BAND              0x0A
#define CMD_CODE_TUNER_TUNE                     0x08
#define CMD_CODE_TUNER_SET_FM_PROC              0x11
#define CMD_CODE_TUNER_GET_RECEPTION_QUALITY    0x12
#define CMD_CODE_TUNER_GET_CHANNEL_QUALITY      0x13
#define CMD_CODE_TUNER_MUTE                     0x16
#define CMD_CODE_TUNER_FM_STEREO_MODE           0x17
#define CMD_CODE_TUNER_SET_VICS_OUTPUT          0x1C
#define CMD_CODE_DEVICE_BB_IF_ON_OFF            0x33
#define CMD_CODE_DEVICE_Set_BB_PROC             0x30

//RDS commands
#define CMD_CODE_TUNER_RDSBUFFER_SET            0x1A
#define CMD_CODE_TUNER_RDSBUFFER_READ           0x1B

//AF check commands
#define CMD_CODE_TUNER_AF_START                 0x20
#define CMD_CODE_TUNER_AF_END                   0x21
#define CMD_CODE_TUNER_AF_CHECK                     0x22
#define CMD_CODE_TUNER_AF_SWITCH                    0x23
#define CMD_CODE_TUNER_GET_AF_QUALITY           0x24

//Seek commands
#define CMD_CODE_TUNER_SEEK_START                   0x26
#define CMD_CODE_TUNER_SEEK_END                     0x27
#define CMD_CODE_TUNER_GET_SEEK_STATUS          0x28
#define CMD_CODE_TUNER_SET_SEEK_TRESHOLD        0x29

//Configuration commands
#define CMD_CODE_TUNER_SET_HD_BLEND                 0x14
#define CMD_CODE_TUNER_CONF_HD_BLEND            0x15
#define CMD_CODE_TUNER_SET_AUDIO_IF                 0x0D
#define CMD_CODE_TUNER_SET_BB_IF                        0x04
#define CMD_CODE_TUNER_CONF_BB_SAI                  0x05
#define CMD_CODE_TUNER_CONF_JESD204                 0x06

//WX SAME commands
#define CMD_CODE_TUNER_READ_SAMEBUFF            0x1D



//Debugging commands
#define CMD_CODE_DEVICE_SET_DISS                    0x31
#define CMD_CODE_DEVICE_GET_WSP_STATUS          0x32


#define U32_ADD(u32h, u32l, delta) do { \
    if (u32l>(u32l+delta)) {    \
        u32h += 1;  \
        u32l = u32l+delta;  \
    } else {    \
        u32l = u32l+delta;  \
    }   \
} while(0);

