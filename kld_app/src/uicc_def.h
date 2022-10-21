
#ifndef _UICC_DEF_H_
#define _UICC_DEF_H_
	
#define NO_KEY				0x00

	#define UICC_MUTE		0x01				//静音
	#define UICC_PLAY_PAUSE	0x02			//播放暂停
	#define UICC_NEXT		0x03				//下一曲
	#define UICC_PREV		0x04				//上一曲
	#define UICC_EJECT		0x05				//出碟		
	#define UICC_SOURCE		0x06			//源切换
	#define UICC_PLAY		0x07
	#define UICC_PAUSE		0x08
	#define UICC_EQ			0x09			//打开EQ
	#define UICC_RADIO		0x0A			//进入收音机
	#define UICC_AS			0x0B			//自动存台
	#define UICC_VOLUME_DOWN	0x0C		//音量减	
	#define UICC_VOLUME_UP		0x0D			//音量加
	#define UICC_UP					0x0E		//上
	#define UICC_DOWN				0x0F		//下
	#define UICC_LEFT				0x10		//左
	#define UICC_RIGHT				0x11		//右
	#define UICC_0					0x12		// 0
	#define UICC_1					0x13		// 1
	#define UICC_2					0x14		// 2
	#define UICC_3					0x15		// 3
	#define UICC_4					0x16		// 4
	#define UICC_5					0x17		//5
	#define UICC_6					0x18		//6	
	#define UICC_7					0x19		//7
	#define UICC_8					0x1A		//8
	#define UICC_9					0x1B		//9
	#define UICC_SETUP				0x1C		//设置
	#define UICC_STOP				0x1D		//停止
	#define UICC_ALL_APP			0x1E
	#define UICC_FASTF				0x1F		//快进
	#define UICC_FASTR				0x20			//快退
	#define UICC_ENTER				0x21    	 //OK   
	#define UICC_SMART_CW			0x22	//右旋钮正旋
	#define UICC_SMART_CCW			0x23	//右旋钮反旋
	#define UICC_HOME				0x24			//菜单
	#define UICC_BACK				0x25	//返回按键
	#define UICC_MENU				0x26	//功能按键(android专用，Wince不要使用)  
	#define UICC_TIME				0x27
	#define UICC_XING				0x28	// *
	#define UICC_JING				0x29	// #
	#define UICC_RDS_TA_SW			0x2A
	#define UICC_ESC				0x2B
	#define UICC_BT					0x2C	
	#define UICC_IPOD				0x2D			//进入IPOD模式
	#define UICC_DIAL				0x2e				//拨号	
	#define UICC_HANG				0x2f 			//挂电?
	#define UICC_DVD				0x30	//进入DVD模式
	#define UICC_NAVI				0x31	//进入导航界面

	#define UICC_AUX				0x33
	#define UICC_FAKE_POWER_OFF	0x34
	#define UICC_CH_UP				0x35	//上一个频道
	#define UICC_CH_DOWN			0x36	//下一个频道
	#define UICC_SPEECH				0x37
	#define UICC_ZOOM_IN			0x38
	#define UICC_ZOOM_OUT			0x39
	#define UICC_POWER				0x3A	// not support now	//电源按键
	#define UICC_DVR				0x3B
	#define UICC_MUSIC				0x3C
	#define UICC_VIDEO				0x3D
	#define UICC_PICTURE			0x3E
	#define UICC_BT_MUSIC			0x3F
	#define UICC_RADIO_PWR			0x40
	#define UICC_TFT_STANDBY		0x41		//进入或者退出Standby模式
	#define UICC_TFT_DIM			0x42


	#define UICC_SUB_TITLE			0x45
	#define UICC_TITLE				0x46
	#define UICC_PBC				0x47
	#define UICC_TV					0x48
	#define UICC_AMS_RPT			0x49
	#define UICC_LOC_RDM			0x4A
	#define UICC_NUM_10_PLUS		0x4B
	#define UICC_ZOOM				0x4C
	#define UICC_GOTO				0x4D
	#define UICC_ST_PROG			0x4E
	#define UICC_ANGLE				0x4F
	#define UICC_OSD				0x50
	#define UICC_LOUDNESS			0x51

	#define UICC_SEEK_NEXT			0x52
	#define UICC_SEEK_PREV			0x53

//	#define UICC_XXX					0x54~0x5A	// reserved by APP
	#define UICC_STEP_DOWN			0x54
	#define UICC_STEP_UP			0x55
	#define UICC_SPEECH_2				0x56

	#define UICC_DVD_SETUP			0x5B
	#define UICC_DVD_UP				0x5C
	#define UICC_DVD_DOWN			0x5D
	#define UICC_DVD_LEFT			0x5E
	#define UICC_DVD_RIGHT			0x5F
	#define UICC_DVD_ENTER			0x60
	
	#define UICC_EQ_SEL			0x78

	#define UICC_RADIO_SCAN	0x7A
	#define UICC_RADIO_PS		0x7B
	
	#define UICC_FF2 	0xA7
	#define UICC_FR2	0xA8

	#define UICC_NISSAN_XTRAIL_CAM_SW		0xF0
	#define UICC_MOTOR_TOGGLE	0xF1
	#define UICC_MOTOR_FORCE_ROLL	0xF2
#endif



