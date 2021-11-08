
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define SYSTEM_VARIABLE_NUMBER	14



extern void System_SetAuthChallenge(puchar pChallenge, uchar len);
extern uchar System_GetAuthSignature(puchar pSign, uchar len);

extern  void System_ClearSetting(void);
extern bool Check_CheckSum(void);
extern void Generate_CheckSum(void);
 extern void Clr_CheckSum(void);

extern uchar system_set_tiny_ram(puchar pbuf, uchar len);
extern uchar system_get_tiny_ram(puchar pbuf, uchar len);
extern uchar system_get_tiny_ram_size(void);

typedef enum
{
	DISP_INV_CMD_NONE = 0,
	DISP_INV_CMD_YES,
	DISP_INV_CMD_NO,
	DISP_INV_CMD_NUMS
}DISP_INV_CMD;

ext uchar CheckHead;
ext uchar CheckTail;

ext u8 g_bt_type;


ext bool g_app_in_charge;

ext u8 g_super_watchdog_timer;


#define DFT_SETTING_FLAG1 (ON<<5)|(DEF_BEEP_ONOFF<<4)|(ON<<6)


ext u8 g_startup_cntr;

ext u8 g_is_watchdog_rst;


ext  UCharField Sys_Flag1;
#define F_IAP_Request	 		Sys_Flag1.field.B0
#define F_SystemOff			Sys_Flag1.field.B2
#define F_SystemStandBy		Sys_Flag1.field.B3
#define F_SystemInitEnd		Sys_Flag1.field.B4


ext  UCharField Sys_Flag2;

#define Wince_Start_Flag		Sys_Flag2.field.B1
#define F_System_need_reset		Sys_Flag2.field.B2


extern void Init_Nochange_Section(void);


extern void main_goto_iap(void);


#endif

