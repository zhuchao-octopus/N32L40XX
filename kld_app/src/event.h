
#ifndef _EVENT_H_
#define _EVENT_H_

typedef enum    //module name define
{
    NONE_MODULE =0, //��ģ��,����ʶ�������event�Ƿ��
    POWER_MODULE,
    MOTOR_MODULE,
    VIDEO_MODULE,
    AUDIO_MODULE,
    SCREEN_MODULE,
    MMI_MODULE,
    MAIN_MODULE,
    TUNER_MODULE,
    DVDP_MODULE,
    DVDC_MODULE,
    TV_MODULE,
    RDS_MODULE,
    GPS_MODULE, 
    WINCE_MODULE ,
    MPEG_MODULE,
    KEY_MODULE,
    NAVI_MODULE,
    CAN_APP_MODULE, //����CAN BOX����Ҫ�������ӵ�
    CAN_MODULE,
    JBL_MODULE,
    GM_CAN_MODULE,  //2����ͨ�ó�������¾�������³�ȣ���CAN Adapter
    SWC_MODULE,
    PANEL_MODULE,
    NUMOFEVT        //must be in the last position,  Total of the Module
}MODULE_NAME;

#define  EVENT_QUE_NUM  3


#define NUMOFEVTBODY    32


typedef   struct
{
    uchar       event_index;
    uchar       name;
    uchar       ID;
    uword       prm;
}POST_EVENT;

typedef   struct
{
    uchar       ID;
    uword       prm;
}EVENT;


typedef   struct
{
    uchar       headptr;
    uchar       tailptr;
    POST_EVENT  event_body[NUMOFEVTBODY];
}EVENT_QUE;

ext EVENT_QUE event_que[EVENT_QUE_NUM] ;

extern void PostEvent(MODULE_NAME name,EVENT_ID ID, uword parameter);
extern EVENT *GetEvent(MODULE_NAME Module);
extern void ClearEvent(MODULE_NAME Module);
extern EVENT *QueryEvent(MODULE_NAME Module);
extern void InitEvent(void);

extern void Evt_Lifetime_Manage(void);

#endif



