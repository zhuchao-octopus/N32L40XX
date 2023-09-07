
#ifndef _LED_H_
#define _LED_H_

#define LED_LEVEL_MIN   1
#define LED_LEVEL_MAX   255

typedef struct
{
    bool color_changed;
    bool is_power_up;
    u8 temp_on_timer;
    u8 save_color_timer;
    bool notify_reset;
    u8 notify_timer;
    u8 auto_color_timer;
}LED_INFO;

ext LED_INFO g_led_info;

void led_init(void);
void led_main(void);
void led_set_color(u8 r_lvl, u8 g_lvl, u8 b_lvl);
void led_set_always_on(u8 on);
void led_notify_reset(void);

#endif

