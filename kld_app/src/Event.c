
#include "public.h"

#define EVT_TIME    200 //Time length =40*12ms=480ms

#define NULL_NEXT   NUMOFEVTBODY//0x0ff

static uchar Evt_Timeout[NUMOFEVTBODY];

static void Set_Evt_Timer(uchar AlignIndex,uchar Element)
{
    if (0==AlignIndex)
    {
        Evt_Timeout[Element]=EVT_TIME;
    }
}
static void Clrt_Evt_Timer(uchar AlignIndex,uchar Element)
{
    if (0==AlignIndex)
    {
        Evt_Timeout[Element]=0;
    }
}


extern void PostEvent(MODULE_NAME name,EVENT_ID ID, uword parameter)
{
    uchar temp;
    uchar i;
    uchar index;

    if (TUNER_MODULE==name)
    {
        index=2;
    }
    else if (WINCE_MODULE==name)
    {
        index=1;
    }
    else 
    {
        index = 0;
    }

    if (event_que[index].tailptr==event_que[index].headptr)
    {   //空
        event_que[index].tailptr = 1 ;
        event_que[index].headptr = 0;
        event_que[index].event_body[0].ID = ID;
        event_que[index].event_body[0].name = name;
        event_que[index].event_body[0].prm = parameter;
        event_que[index].event_body[0].event_index = 1 ;
        Set_Evt_Timer(index,0);
        return;
    }
    if (event_que[index].tailptr>=NUMOFEVTBODY)
    {
        return;     //满
    }
    temp = event_que[index].tailptr ;

    event_que[index].event_body[temp].ID = ID;
    event_que[index].event_body[temp].name = name;
    event_que[index].event_body[temp].prm = parameter;
    Set_Evt_Timer(index,temp);

    for (i=0;i<NUMOFEVTBODY;i++)
    {
        if (0 == event_que[index].event_body[i].name )
        {
            break;
        }
    }
    event_que[index].event_body[temp].event_index = i ;
    event_que[index].tailptr = i ;
    
}

extern EVENT *GetEvent(MODULE_NAME Module)
{
    static EVENT tmpEvt;
    uchar head,temp,temp2;
    uword evt_name;
    uchar index;
    if (TUNER_MODULE==Module)
    {
        index=2;
    }
    else if (WINCE_MODULE==Module)
    {
        index=1;
    }
    else 
    {
        index = 0;
    }
    tmpEvt.ID=NO_EVT;
    
    if (event_que[index].headptr != event_que[index].tailptr )
    {   //不空
        head = event_que[index].headptr ;
        for (;;)
        {
            evt_name = event_que[index].event_body[head].name ;
            if (Module == evt_name )
            {
                Clrt_Evt_Timer(index,head);
                
                tmpEvt.ID = event_que[index].event_body[head].ID ;  //取出ID
                tmpEvt.prm = event_que[index].event_body[head].prm ;    //取出prm
                event_que[index].event_body[head].name = NONE_MODULE;
                
                temp = event_que[index].event_body[head].event_index;
                if (event_que[index].headptr==head)
                {       //取出队列的第一个元素
                    event_que[index].headptr = temp ;

                }
                else
                {
                    event_que[index].event_body[temp2].event_index = temp ;
                }
                break;
            }
            else
            {   
                temp2 = head ;
                head = event_que[index].event_body[head].event_index ;
                if (event_que[index].tailptr == head)
                {
                    break;
                }
            }
        
        }
        
    }
    return &tmpEvt;
    
}       
        
extern void ClearEvent(MODULE_NAME Module)
{
    EVENT *tempEvt;
    do
    {
        tempEvt = GetEvent(Module);
        
    }
    while ( tempEvt->ID != NO_EVT);
}

extern void InitEvent(void)
{
    uchar index,i;
    for (index=0;index<EVENT_QUE_NUM;index++)
    {
        event_que[index].headptr =0;
        event_que[index].tailptr = 0;
        for (i=0;i<NUMOFEVTBODY;i++)
        {
            event_que[index].event_body[i].name = NONE_MODULE;
        }
    }
}       
        
extern EVENT *QueryEvent(MODULE_NAME Module)
{
    static EVENT Evt;
    uchar head;
    uchar index;
    if (TUNER_MODULE==Module)
    {
        index=2;
    }
    else if (WINCE_MODULE==Module)
    {
        index=1;
    }
    else
    {
        index = 0;
    }
    Evt.ID=NO_EVT;
    if (event_que[index].headptr != event_que[index].tailptr )
    {   //不空
        head = event_que[index].headptr ;
        for (;;)
        {
            if (Module == event_que[index].event_body[head].name)
            {
                Evt.ID = event_que[index].event_body[head].ID ;     //获得ID
                Evt.prm = event_que[index].event_body[head].prm ;   //获得prm
                break;
            }
            else
            {
                head = event_que[index].event_body[head].event_index ;
                if (event_que[index].tailptr == head)
                {
                    break;;
                }
            }
        }
        
    }
    
    return &Evt;
}        


extern void Evt_Lifetime_Manage(void)
{
    uchar i;
    for (i=0;i<NUMOFEVTBODY;i++)
    {
        if ( Evt_Timeout[i] )
        {
            Evt_Timeout[i]--;
            if (0==Evt_Timeout[i])
            {
                GetEvent((MODULE_NAME)(event_que[0].event_body[i].name));
            }
        }
    /*  if(Evt_Timeout[i]==0 && event_que[0].event_body[i].name!=NONE_MODULE)
        {
            (void)GetEvent(event_que[0].event_body[i].name);
        }*/
    }
}
    
  


        
        




        
                



    
                
    

