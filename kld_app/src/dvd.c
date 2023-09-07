#include "public.h"

#ifdef DVD_FUNCTION_ENABLE

static void _dvd_do_inject(bool en)
{
    if (FALSE == en) {
        GPIO_ResetBits(GPIO_DVD_LOAD_P_GRP, GPIO_DVD_LOAD_P_PIN);
    } else {
        GPIO_SetBits(GPIO_DVD_LOAD_P_GRP, GPIO_DVD_LOAD_P_PIN);
    }
}

static void _dvd_do_eject(bool en)
{
    if (FALSE == en) {
        GPIO_ResetBits(GPIO_DVD_LOAD_N_GRP, GPIO_DVD_LOAD_N_PIN);
    } else {
        GPIO_SetBits(GPIO_DVD_LOAD_N_GRP, GPIO_DVD_LOAD_N_PIN);
    }
}

static void _dvd_clear_var(void)
{
    GPIO_ResetBits(GPIO_DVD_PWR_CTRL_GRP, GPIO_DVD_PWR_CTRL_PIN);
    GPIO_ResetBits(GPIO_DVD_RST_GRP, GPIO_DVD_RST_PIN);
    _dvd_do_inject(FALSE);
    _dvd_do_eject(FALSE);
    g_dvd_info.dvd_pwr_up = FALSE;
    g_dvd_info.disc_state = DISC_STATE_UNKNOWN;
    g_dvd_info.disc_cmd = DISC_CMD_NONE;
    g_dvd_info.auto_inject_timer = 0;
    g_dvd_info.pwr_timer = 0;
    g_dvd_info.timer = 0;
}

static DISC_STATE _dvd_check_disc(void)
{
    uint8_t trout, trin, disc_det;
    DISC_STATE state;

    trout = GPIO_ReadInputDataBit(GPIO_DVD_TRAY_OUT_GRP, GPIO_DVD_TRAY_OUT_PIN);
    trin = GPIO_ReadInputDataBit(GPIO_DVD_TRAY_IN_GRP, GPIO_DVD_TRAY_IN_PIN);
    disc_det = GPIO_ReadInputDataBit(GPIO_DVD_DISC_DET_GRP, GPIO_DVD_DISC_DET_PIN);
    
    state = DISC_STATE_UNKNOWN;
    if (Bit_RESET == trin) {
        state = DISC_STATE_NODISC;
    } else {
        if ((Bit_SET==trout) && (Bit_RESET==disc_det)) {
            state = DISC_STATE_READY;
        } else if ((Bit_RESET==trout) && (Bit_SET==disc_det)) {
            state = DISC_STATE_OUTSIDE;
        } else if ((Bit_SET==trout) && (Bit_SET==disc_det)) {
            if ((DISC_STATE_NODISC==g_dvd_info.disc_state) || 
                (DISC_STATE_OUTSIDE==g_dvd_info.disc_state)) {
                state = DISC_STATE_INJECTING;
            }
            if (DISC_STATE_READY==g_dvd_info.disc_state) {
                state = DISC_STATE_EJECTING;
            }
            if (DISC_STATE_EJECTING==g_dvd_info.disc_state) {
                if (DISC_CMD_INJECT==g_dvd_info.disc_cmd) {
                    state = DISC_STATE_INJECTING;
                } else {
                    state = DISC_STATE_EJECTING;
                }
            }
            if (DISC_STATE_INJECTING==g_dvd_info.disc_state) {
                if (DISC_CMD_EJECT==g_dvd_info.disc_cmd) {
                    state = DISC_STATE_EJECTING;
                } else {
                    state = DISC_STATE_INJECTING;
                }
            }
        }
    }

    return state;
}

static void dvd_event_handler(void)
{
    EVENT *pEvt;
    pEvt=GetEvent(DVDC_MODULE);
    switch(pEvt->ID) {
        case EVT_DVD_PWR_CTRL:
            if (0==pEvt->prm) {
                if (g_dvd_info.dvd_pwr_up) {
                    g_dvd_info.dvd_pwr_up = FALSE;
                    g_dvd_info.pwr_timer = 0;
                }
                PostEvent(WINCE_MODULE, TX_TO_GUI_DVD_PWR_INFO, NONE);
            } else if (1==pEvt->prm) {
                if (!g_dvd_info.dvd_pwr_up) {
                    g_dvd_info.dvd_pwr_up = TRUE;
                    g_dvd_info.pwr_timer = 0;
                }
                PostEvent(WINCE_MODULE, TX_TO_GUI_DVD_PWR_INFO, NONE);
            }
            break;
        case EVT_DVD_DISC_CTRL:
            if (0==pEvt->prm) {
                g_dvd_info.disc_cmd = DISC_CMD_INJECT;
            } else if (1==pEvt->prm) {
                if ( (DISC_STATE_UNKNOWN != g_dvd_info.disc_state) &&
                    (DISC_STATE_NODISC != g_dvd_info.disc_state) ) {
                    g_dvd_info.disc_cmd = DISC_CMD_EJECT;
                }
            }
            break;
        case EVT_DVD_FORCE_DISC_EJECT:
            g_dvd_info.force_eject_timer = T6S_12;
            break;
        default:
            break;
    }   
}

static void dvd_pwr_handler(void)
{
    ++g_dvd_info.pwr_timer;
    
    if (g_dvd_info.dvd_pwr_up) {
        GPIO_SetBits(GPIO_DVD_PWR_CTRL_GRP, GPIO_DVD_PWR_CTRL_PIN);
        if (g_dvd_info.pwr_timer > T600MS_12) {
            GPIO_SetBits(GPIO_DVD_RST_GRP, GPIO_DVD_RST_PIN);
        } else if (g_dvd_info.pwr_timer > T200MS_12) {
            GPIO_ResetBits(GPIO_DVD_RST_GRP, GPIO_DVD_RST_PIN);
        }
    } else {
        if (g_dvd_info.pwr_timer > T1S_12) {
            GPIO_ResetBits(GPIO_DVD_PWR_CTRL_GRP, GPIO_DVD_PWR_CTRL_PIN);
            GPIO_ResetBits(GPIO_DVD_RST_GRP, GPIO_DVD_RST_PIN);
        }
    }

    // make sure the pwr_timer not overflow
    if (g_dvd_info.pwr_timer > 250) {
        --g_dvd_info.pwr_timer;
    }
}

static void dvd_disc_handler(void)
{
    DISC_STATE cur_disc_state;
    cur_disc_state = _dvd_check_disc();

    if (DISC_STATE_UNKNOWN == g_dvd_info.disc_state) {
        if (++g_dvd_info.timer > T50MS_12) {
            g_dvd_info.disc_state = cur_disc_state;
            g_dvd_info.timer -= 1;
        }
        return;
    }

    switch (g_dvd_info.disc_state) {
        case DISC_STATE_READY:
            if (DISC_CMD_EJECT == g_dvd_info.disc_cmd) {
                _dvd_do_eject(TRUE);
            }
            break;
        case DISC_STATE_OUTSIDE:
            if ( (++g_dvd_info.auto_inject_timer > T10S_12) ||
                (DISC_CMD_INJECT == g_dvd_info.disc_cmd) ) {
                _dvd_do_inject(TRUE);
                g_dvd_info.auto_inject_timer = 0;
            } else if (DISC_STATE_NODISC == cur_disc_state) {
                _dvd_do_inject(FALSE);
                g_dvd_info.disc_cmd = DISC_CMD_NONE;
            }
            break;
        case DISC_STATE_INJECTING:
            if ( (DISC_STATE_READY == cur_disc_state) ||
                (DISC_STATE_NODISC == cur_disc_state) ) {
                _dvd_do_inject(FALSE);
                g_dvd_info.disc_cmd = DISC_CMD_NONE;
//              PostEvent(WINCE_MODULE, TX_TO_GUI_DVD_DISC_INFO, NONE);
            }
            if (DISC_CMD_EJECT == g_dvd_info.disc_cmd) {
                _dvd_do_inject(FALSE);
                _dvd_do_eject(TRUE);
            }
            break;
        case DISC_STATE_EJECTING:
            if ( (DISC_STATE_OUTSIDE == cur_disc_state) ||
                (DISC_STATE_NODISC == cur_disc_state) ){
                _dvd_do_eject(FALSE);
                g_dvd_info.disc_cmd = DISC_CMD_NONE;
                g_dvd_info.auto_inject_timer = 0;
//              PostEvent(WINCE_MODULE, TX_TO_GUI_DVD_DISC_INFO, NONE);
            }
            if (DISC_CMD_INJECT == g_dvd_info.disc_cmd) {
                _dvd_do_eject(FALSE);
                _dvd_do_inject(TRUE);
            }
            break;
        case DISC_STATE_NODISC:
            if (DISC_STATE_OUTSIDE == cur_disc_state) {
                _dvd_do_inject(TRUE);
            }
            break;
        default:
            break;
    }

    if (g_dvd_info.disc_state != cur_disc_state) {
        PostEvent(WINCE_MODULE, TX_TO_GUI_DVD_DISC_INFO, NONE);
    }
    g_dvd_info.disc_state = cur_disc_state;
}

void dvd_init(void)
{
    g_dvd_info.force_eject_timer = 0;
    _dvd_clear_var();
}

void dvd_poweroff(void)
{
    g_dvd_info.force_eject_timer = 0;
    _dvd_clear_var();
}

void dvd_main(void)
{
    if (Is_Machine_Power) {
        if (g_dvd_info.force_eject_timer>0) {
            --g_dvd_info.force_eject_timer;
            if (g_dvd_info.force_eject_timer > T200MS_12) {
                _dvd_do_inject(FALSE);
                _dvd_do_eject(TRUE);
            } else {
                _dvd_do_inject(FALSE);
                _dvd_do_eject(FALSE);
                _dvd_clear_var();
            }
        } else {
            dvd_event_handler();
            dvd_pwr_handler();
            dvd_disc_handler();
        }
    }
}

bool dvd_is_audio_ready(void)
{
    if (!g_dvd_info.dvd_pwr_up)
        return FALSE;

    if (g_dvd_info.pwr_timer < T1S5_12) {
        return FALSE;
    }

    return TRUE;
}

#endif

