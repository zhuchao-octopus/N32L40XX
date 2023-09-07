
#include "public.h"


#define RDS_CLEAR_AF_LIST() do {                \
        for (i=0; i<RDS_AF_LIST_MAX_NR; i++) {  \
            g_rds_info.af[i] = 0;               \
        }                                   \
        g_rds_info.af_nr = 0;                   \
    } while(0);

void rds_init(void)
{
    rds_reset_var();
    g_rds_info.af_switch_on = FALSE;
    g_rds_info.ta_switch_on = FALSE;
    g_rds_info.pty_select = 0;
}

void rds_main(void)
{
#if RDS_FUNCTION==1
        radio_dev_rds_update();
#if 0
    // force to notify HOST, even not received a complete message
    if (g_rds_info.rt_update_cntr++ > T2S_12) {
        g_rds_info.rt_update_cntr = 0;
        if (g_rds_info.rt_need_notify) {        
            PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_RT_INFO, 1);
        }
    }
#endif

#endif
}

void rds_reset_var(void)
{
    u8 i;
    g_rds_info.block_a = 0;
    g_rds_info.block_b = 0;
    g_rds_info.block_c = 0;
    g_rds_info.block_d = 0;
    g_rds_info.pi = 0;
    g_rds_info.pi_tmp = 0;
    g_rds_info.pi_cnt = 0;
    g_rds_info.pty = 0;
    g_rds_info.pty_tmp = 0;
    g_rds_info.pty_cnt = 0;
    for(i=0;i<RDS_PS_CHAR_NR;i++) {
        g_rds_info.ps[i] = 0;
        g_rds_info.ps_tmp0[i] = 0;
        g_rds_info.ps_tmp1[i] = 0;
        g_rds_info.ps_cnt[i] = 0;
    }
    RDS_CLEAR_AF_LIST();
    g_rds_info.tp_on = FALSE;
    g_rds_info.ta_on = FALSE;
    rds_rt_clear();
    audio_set_rds_ta_break(FALSE);
}

//-----------------------------------------------------------------------------
// This routine adds an additional level of error checking on the PI code.
// A PI code is only considered valid when it has been identical for several
// groups.
//-----------------------------------------------------------------------------
void rds_update_pi(u16 new_pi)
{
    // if the pi value is the same for a certain number of times, update
    // a validated pi variable
    if (new_pi != g_rds_info.pi_tmp) {
        g_rds_info.pi_tmp =  new_pi;
        g_rds_info.pi_cnt = 1;
    } else {
        g_rds_info.pi_cnt++;
    }

    if (g_rds_info.pi_cnt > RDS_PI_VALIDATE_LIMIT) {
        --g_rds_info.pi_cnt;    // make sure pi_cnt not overflow
        if (g_rds_info.pi != g_rds_info.pi_tmp) {
            // do some notify if needed
            // we can post event here before PI really set, because mcu never 
            // handle tx event when inside functions
            //PostEvent(WINCE_MODULE, xxxx, g_rds_info.pi_tmp);
        }
        g_rds_info.pi = g_rds_info.pi_tmp;
    }
}

//-----------------------------------------------------------------------------
// This routine adds an additional level of error checking on the PTY code.
// A PTY code is only considered valid when it has been identical for several
// groups.
//-----------------------------------------------------------------------------
void rds_update_pty(u8 new_pty)
{
    // if the pty value is the same for a certain number of times, update
    // a validated pty variable
    if (new_pty != g_rds_info.pty_tmp) {
        g_rds_info.pty_tmp = new_pty;
        g_rds_info.pty_cnt = 1;
    } else {
        g_rds_info.pty_cnt++;
    }

    if (g_rds_info.pty_cnt > RDS_PTY_VALIDATE_LIMIT) {
        --g_rds_info.pty_cnt;   // make sure pty_cnt not overflow
        if (g_rds_info.pty != g_rds_info.pty_tmp) {
            // we can post event here before PTY really set, because mcu never 
            // handle tx event when inside functions
            PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PTY_INFO, NONE);
        }
        g_rds_info.pty = g_rds_info.pty_tmp;
    }
}

//-----------------------------------------------------------------------------
// This implelentation of the Program Service update attempts to display
// only complete messages for stations who rotate text through the PS field
// in violation of the RBDS standard as well as providing enhanced error
// detection.
//-----------------------------------------------------------------------------
void rds_update_ps(u8 addr, u8 chr)
{
    u8 i;
    u8 textChange = 0; // indicates if the PS text is in transition
    u8 psComplete = 1; // indicates that the PS text is ready to be displayed

    if (addr >= RDS_PS_CHAR_NR) {
        // invalid ps char address
        return;
    }

    if (chr == g_rds_info.ps_tmp0[addr]) {
        // The new byte matches the high probability byte
        if (PS_VALIDATE_LIMIT > g_rds_info.ps_cnt[addr]) {
            g_rds_info.ps_cnt[addr]++;
        } else {
            // we have recieved this byte enough to max out our counter
            // and push it into the low probability array as well
            g_rds_info.ps_cnt[addr] = PS_VALIDATE_LIMIT;
            g_rds_info.ps_tmp1[addr] = chr;
        }
    } else if (chr == g_rds_info.ps_tmp1[addr]) {
        // The new byte is a match with the low probability byte. Swap
        // them, reset the counter and flag the text as in transition.
        // Note that the counter for this character goes higher than
        // the validation limit because it will get knocked down later
        if(g_rds_info.ps_cnt[addr] >= PS_VALIDATE_LIMIT) {
            textChange = 1;
            g_rds_info.ps_cnt[addr] = PS_VALIDATE_LIMIT + 1;
        } else {
            g_rds_info.ps_cnt[addr] = PS_VALIDATE_LIMIT;
        }
        g_rds_info.ps_tmp1[addr] = g_rds_info.ps_tmp0[addr];
        g_rds_info.ps_tmp0[addr] = chr;
    } else if(!g_rds_info.ps_cnt[addr]) {
        // The new byte is replacing an empty byte in the high
        // proability array
        g_rds_info.ps_tmp0[addr] = chr;
        g_rds_info.ps_cnt[addr] = 1;
    } else {
        // The new byte doesn't match anything, put it in the
        // low probablity array.
        g_rds_info.ps_tmp1[addr] = chr;
    }

    if(textChange) {
        // When the text is changing, decrement the count for all
        // characters to prevent displaying part of a message
        // that is in transition.
        for(i=0; i<RDS_PS_CHAR_NR; i++) {
            if(g_rds_info.ps_cnt[i] > 1) {
                g_rds_info.ps_cnt[i]--;
            }
        }
    }

    // The PS text is incomplete if any character in the high
    // probability array has been seen fewer times than the
    // validation limit.
    for (i=0;i<RDS_PS_CHAR_NR;i++) {
        if(g_rds_info.ps_cnt[i] < PS_VALIDATE_LIMIT) {
            psComplete = 0;
            break;
        }
    }

    // If the PS text in the high probability array is complete
    // copy it to the display array
    if (psComplete) {
        for (i=0;i<RDS_PS_CHAR_NR;i++) {
            if (g_rds_info.ps[i] != g_rds_info.ps_tmp0[i]) {
                // we can post event here before PS really set, because mcu never 
                // handle tx event when inside functions
                PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_PS_INFO, NONE);
                break;
            }
        }
        for (i=0;i<RDS_PS_CHAR_NR;i++) {
            g_rds_info.ps[i] = g_rds_info.ps_tmp0[i];
        }
    }
}

void rds_update_af_list(u16 af_blk)
{
    u8 data;
    u16 freq;
    u8 i;

    // the top 8 bits is either the AF Count or AF Data
    data = (u8)(af_blk >> 8);

    if ((data >= AF_COUNT_MIN) && (data <= AF_COUNT_MAX)) {
        // get AF count
        if ((data-AF_COUNT_MIN) != g_rds_info.af_nr) {
            // reset AF list
            RDS_CLEAR_AF_LIST();
            g_rds_info.af_nr = (u8)(data-AF_COUNT_MIN);
        }
    }

    // ignore AF data if AF list is full
    if ( (0!=g_rds_info.af_nr) && 
        (0!=g_rds_info.af[g_rds_info.af_nr-1]) ) {
        return;
    }

    while (af_blk) {
        data = (u8)(af_blk >> 8);   // get the top 8 bits
        if (/*(data >= (u8)AF_FREQ_MIN) &&*/ (data <= (u8)AF_FREQ_MAX)) {
            // get AF data
            freq = AF_FREQ_TO_U16F(data);
            for (i=0; i<g_rds_info.af_nr; i++) {
                if (freq==g_rds_info.af[i]) {
                    // already save this freq
                    break;
                }
                // save this freq in a empty slot
                if (0==g_rds_info.af[i]) {
                    g_rds_info.af[i] = freq;
                    break;
                }
            }
        }
        af_blk = af_blk<<8; // remove the MSB,  and turn the LSB into MSB
    }
}

void rds_update_tp(bool tp_on)
{
    if (tp_on!=g_rds_info.tp_on) {
        // we can post event here before TP really set, because mcu never 
        // handle tx event when inside functions
        PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_FLAG_INFO, NONE);
    }
    g_rds_info.tp_on = tp_on;
}

void rds_update_ta(bool ta_on)
{
    if (ta_on!=g_rds_info.ta_on) {
        // we can post event here before TP really set, because mcu never 
        // handle tx event when inside functions
        PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_FLAG_INFO, NONE);

        if (g_rds_info.ta_switch_on) {
            audio_set_rds_ta_break(ta_on);
        }

    }
    g_rds_info.ta_on = ta_on;
}

bool rds_is_af_list_ready(void)
{
    if (0 == g_rds_info.af_nr) {
        // no AF freq in the AF list
        return FALSE;
    }
    if (0 == g_rds_info.af[g_rds_info.af_nr-1]) {
        // AF list is not complete yet
        return FALSE;
    }
    return TRUE;
}

u16 rds_get_af_freq(u8 idx)
{
    if (FALSE == rds_is_af_list_ready()) {
        return 0;
    }
    if (idx >= g_rds_info.af_nr) {
        // invalid AF index
        return 0;
    }

    return g_rds_info.af[idx];
}

void rds_ta_switch(bool en)
{
    g_rds_info.ta_switch_on = en;
    if (g_rds_info.ta_switch_on) {
        audio_set_rds_ta_break(g_rds_info.ta_on);
    } else {
        audio_set_rds_ta_break(FALSE);
    }
}

void rds_af_switch(bool en)
{
    g_rds_info.af_switch_on = en;
}

void rds_pty_select(u8 pty)
{
    if (32<=pty) {
        return;
    }
    g_rds_info.pty_select= pty;
}

void rds_af_clear_info(void)
{
    u8 cnt;
    
    g_rds_af_info.pi = 0;
    g_rds_af_info.af_nr = 0;
    for(cnt=0; cnt<RDS_AF_LIST_MAX_NR; cnt++) {
        g_rds_af_info.af[cnt] = 0;
    }
}
void rds_af_save_info(void)
{
    u8 cnt;

    rds_af_clear_info();
    
    if (rds_is_af_list_ready()) {
        g_rds_af_info.pi = g_rds_info.pi;
        g_rds_af_info.af_nr = g_rds_info.af_nr;
        for (cnt=0; cnt<g_rds_info.af_nr; cnt++) {
            g_rds_af_info.af[cnt] = g_rds_info.af[cnt];
        }
    }
}

bool rds_af_is_hit_list(u16 freq)
{
    u8 cnt;

    for (cnt=0; cnt<g_rds_af_info.af_nr; cnt++) {
        if (freq == g_rds_af_info.af[cnt]) {
            return TRUE;
        }
    }

    return FALSE;
}

void rds_rt_clear(void)
{
    u8 cnt;
    for (cnt=0; cnt<RDS_RT_CHAR_NR; cnt++) {
        g_rds_info.rt[cnt] = 0;
        g_rds_info.rt_cnt[cnt] = 0;
    }
    g_rds_info.rt_need_notify = TRUE;
    g_rds_info.rt_ab_flag = 0;
    g_rds_info.rt_update_cntr = 0;
    PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_RT_INFO, 0);  // notify HOST to clear the RT
}

void rds_update_rt(u8 idx, u8 len, u8 ab_flag, u8 c1, u8 c2, u8 c3, u8 c4)
{
    bool text_change = FALSE;
    u8 cnt;
    u8 data = 0;
    u8 addr = idx*len;

    if (ab_flag != g_rds_info.rt_ab_flag) {
//      rds_rt_clear();
        g_rds_info.rt_ab_flag = ab_flag;
    }

    // check address & length
    switch (len) {
        case 2: // TYPE 2B
        case 4: // TYPE 2A
            if (idx>0x0F) {
                return;
            }
            break;
        default:
            return;
    }

    // fill data
    for (cnt=0; cnt<len; cnt++) {
        switch (cnt) {
            case 0:
                data = c1;
                break;
            case 1:
                data = c2;
                break;
            case 2:
                data = c3;
                break;
            case 3:
                data = c4;
                break;
        }

        if (g_rds_info.rt[addr+cnt] != data) {
            if (0!=g_rds_info.rt[addr+cnt]) {
                text_change = TRUE;
                g_rds_info.rt_need_notify = TRUE;
            }
            g_rds_info.rt[addr+cnt] = data;
            g_rds_info.rt_cnt[addr+cnt] = 1;
        } else {
            ++g_rds_info.rt_cnt[addr+cnt];
        }

        if (g_rds_info.rt_cnt[addr+cnt] > RDS_RT_VALIDATE_LIMIT) {
            g_rds_info.rt_cnt[addr+cnt] = RDS_RT_VALIDATE_LIMIT;
        }
    }

    // update check counter
    if (text_change) {
        for (cnt=0; cnt<RDS_RT_CHAR_NR; cnt++) {
            if (g_rds_info.rt_cnt[cnt] > 1) {
                --g_rds_info.rt_cnt[cnt];
            }
        }       
    }

    // notify HOST, if needed
    if (g_rds_info.rt_need_notify) {
        for (cnt=0; cnt<RDS_RT_CHAR_NR; cnt++) {
            if ( g_rds_info.rt_cnt[cnt]<RDS_RT_VALIDATE_LIMIT ) {
                break;
            }
            if ( 0 == g_rds_info.rt[cnt] ) {
                break;
            }
            if ( 0x0d == g_rds_info.rt[cnt] ) {
                break;
            }
        }

        if ( cnt > 0 ) {
            PostEvent(WINCE_MODULE, TX_TO_GUI_RDS_RT_INFO, 1);
        }
    }
}

u8 rds_fill_rt_info(u8 *pbuf, bool clear)
{
    u8 cnt;
    u8 len=0;

    if (clear) {
        pbuf[0] = 0;
        return 1;
    }

    for (cnt=0; cnt<RDS_RT_CHAR_NR; cnt++) {
        if (0x0d == g_rds_info.rt[cnt]) {
            break;
        }
        if (0x00 == g_rds_info.rt[cnt]) {
            break;
        }

        // only show number / alpha / normal symbol
        if ( (0x20<=g_rds_info.rt[cnt]) && 
                (0x7E>=g_rds_info.rt[cnt]) ) {
            pbuf[len] = g_rds_info.rt[cnt];
        } else {
            pbuf[len] = ' ';
        }

        ++len;
    }

    if (len<RDS_RT_CHAR_NR) {
        pbuf[len++] = 0;    // add null-terminater
    } else {
        // make sure we only show 64 char (including '\0')
        pbuf[RDS_RT_CHAR_NR-1] = 0;
        len = RDS_RT_CHAR_NR;
    }

    g_rds_info.rt_need_notify = FALSE;
    
    return len;
}

