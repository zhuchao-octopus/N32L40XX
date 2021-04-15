
#ifndef _RDS_H_
#define _RDS_H_

#define RDS_PI_VALIDATE_LIMIT  4
#define RDS_PTY_VALIDATE_LIMIT 2
#define PS_VALIDATE_LIMIT 2
#define RDS_RT_VALIDATE_LIMIT	2

#define RDS_TYPE_0A     0x00  //( 0 * 2 + 0)
#define RDS_TYPE_0B     0x01  //( 0 * 2 + 1)
#define RDS_TYPE_1A     0x02  //( 1 * 2 + 0)
#define RDS_TYPE_1B     0x03  //( 1 * 2 + 1)
#define RDS_TYPE_2A     0x04  //( 2 * 2 + 0)
#define RDS_TYPE_2B     0x05  //( 2 * 2 + 1)
#define RDS_TYPE_3A     0x06  //( 3 * 2 + 0)
#define RDS_TYPE_3B     0x07  //( 3 * 2 + 1)
#define RDS_TYPE_4A     0x08  //( 4 * 2 + 0)
#define RDS_TYPE_4B     0x09  //( 4 * 2 + 1)
#define RDS_TYPE_5A     0x0A  //( 5 * 2 + 0)
#define RDS_TYPE_5B     0x0B  //( 5 * 2 + 1)
#define RDS_TYPE_6A     0x0C  //( 6 * 2 + 0)
#define RDS_TYPE_6B     0x0D  //( 6 * 2 + 1)
#define RDS_TYPE_7A     0x0E  //( 7 * 2 + 0)
#define RDS_TYPE_7B     0x0F  //( 7 * 2 + 1)
#define RDS_TYPE_8A     0x10  //( 8 * 2 + 0)
#define RDS_TYPE_8B     0x11  //( 8 * 2 + 1)
#define RDS_TYPE_9A     0x12  //( 9 * 2 + 0)
#define RDS_TYPE_9B     0x13  //( 9 * 2 + 1)
#define RDS_TYPE_10A    0x14  //(10 * 2 + 0)
#define RDS_TYPE_10B    0x15  //(10 * 2 + 1)
#define RDS_TYPE_11A    0x16  //(11 * 2 + 0)
#define RDS_TYPE_11B    0x17  //(11 * 2 + 1)
#define RDS_TYPE_12A    0x18  //(12 * 2 + 0)
#define RDS_TYPE_12B    0x19  //(12 * 2 + 1)
#define RDS_TYPE_13A    0x1A  //(13 * 2 + 0)
#define RDS_TYPE_13B    0x1B  //(13 * 2 + 1)
#define RDS_TYPE_14A    0x1C  //(14 * 2 + 0)
#define RDS_TYPE_14B    0x1D  //(14 * 2 + 1)
#define RDS_TYPE_15A    0x1E  //(15 * 2 + 0)
#define RDS_TYPE_15B    0x1F  //(15 * 2 + 1)

#define RDS_PS_CHAR_NR	8
#define RDS_AF_LIST_MAX_NR	25
#define AF_COUNT_MIN 224
#define AF_COUNT_MAX (AF_COUNT_MIN + 25)
#define AF_FREQ_MIN 0
#define AF_FREQ_MAX 204
#define AF_FREQ_TO_U16F(af) (8750+((af-AF_FREQ_MIN)*10))
#define RDS_RT_CHAR_NR	64

typedef struct
{
	u16 block_a;
	u16 block_b;
	u16 block_c;
	u16 block_d;
	u16 pi;
	u16 pi_tmp;
	u8 pi_cnt;
	u8 pty;
	u8 pty_tmp;
	u8 pty_cnt;
	u8 ps[RDS_PS_CHAR_NR];
	u8 ps_tmp0[RDS_PS_CHAR_NR];       // Temporary PS text (high probability)
	u8 ps_tmp1[RDS_PS_CHAR_NR];       // Temporary PS text (low probability)
	u8 ps_cnt[RDS_PS_CHAR_NR];        // Hit count of high probability PS text
	u16 af[RDS_AF_LIST_MAX_NR];
	u8 rt[RDS_RT_CHAR_NR];
	u8 rt_cnt[RDS_RT_CHAR_NR];
	
	u8 af_nr;
	bool tp_on;
	bool ta_on;
	bool ta_switch_on;
	bool af_switch_on;
	u8 pty_select;
	bool rt_need_notify;
	u8 rt_ab_flag;
	u8 rt_update_cntr;
}RDS_INFO;

typedef struct
{
	u16 pi;
	u16 af[RDS_AF_LIST_MAX_NR];
	u8 af_nr;
}RDS_AF_INFO;

/***************************************
* our varible
**************************************/
ext RDS_INFO g_rds_info;	// do not write any member except raw block data outside rds.c, just read only.
ext RDS_AF_INFO g_rds_af_info;


/*******************************************************
*   interface for system
*******************************************************/
void rds_init(void);
void rds_main(void);
void rds_reset_var(void);
void rds_update_pi(u16 new_pi);
void rds_update_pty(u8 new_pty);
void rds_update_ps(u8 addr, u8 chr);
void rds_update_af_list(u16 af_blk);
void rds_update_tp(bool tp_on);
void rds_update_ta(bool ta_on);
bool rds_is_af_list_ready(void);
u16 rds_get_af_freq(u8 idx);
void rds_ta_switch(bool en);
void rds_af_switch(bool en);
void rds_pty_select(u8 pty);
void rds_af_clear_info(void);
void rds_af_save_info(void);
bool rds_af_is_hit_list(u16 freq);
void rds_rt_clear(void);
void rds_update_rt(u8 idx, u8 len, u8 ab_flag, u8 c1, u8 c2, u8 c3, u8 c4);
u8 rds_fill_rt_info(u8 *pbuf, bool clear);


/*******************************************************
*   interface for physical radio device
*******************************************************/

// return value: true, if radio device finish initialization.
//                   false, if radio device still in initialization.
void radio_dev_rds_init(void);

bool radio_dev_rds_update(void);

#endif /* _RDS_H_ */

