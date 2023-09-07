 
/******************************************************************************/

#include "public.h"  


/*--------------------------------------------------------------------------
Routine : MainEvtProc
Input   : 
Output  :
Description 
        -  channel functions
--------------------------------------------------------------------------*/ 
void channel_main(void)
{
    EVENT * nEvt;
    nEvt=GetEvent(MAIN_MODULE);
    if (nEvt->ID==NO_EVT)  return;

    switch(nEvt->ID)
    {

        case EVT_SRC_POWER_OFF:
            audio_set_pwr_ctrl(FALSE);
            radio_set_pwr_ctrl(FALSE);
            video_set_pwr_ctrl(FALSE);
            if (POWEROFF_FROM_VOLTAGE != (nEvt->prm)) {
                PostEvent(WINCE_MODULE,TX_TO_GUI_POWER_CMD,WCE_POWER_OFF);
            }
            break;

        case EVT_SRC_POWER_ON:
            g_fake_pwr_off = FALSE;
            video_set_pwr_ctrl(TRUE);
            radio_set_pwr_ctrl(TRUE);
            audio_set_pwr_ctrl(TRUE);
            break;

        case EVT_SRC_FRONT_REAR_CHG:
            FrontSource = (SOURCE)(nEvt->prm);              
            audio_set_source((SOURCE)(nEvt->prm));
            break;  

        default:
            break;
    }


}


