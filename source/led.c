#include "led.h"
#include "key.h"

extern Pro_flag_s MCU_flag;
// 继电器对应位
u8 relaySX_tab[24] =
    {
        k1_buf2, k2_buf2, k3_buf2, k4_buf2, k5_buf2, k6_buf2, k7_buf2, k8_buf1,
        k9_buf1, k10_buf1, k11_buf1, k12_buf1, k13_buf1, k14_buf1, k15_buf0, k16_buf0,
        k17_buf0, k18_buf0, k19_buf0, k20_buf0, k21_buf2, kNC1_buf0, kNC2_buf0, kNC3_buf1};
// BUF对应位
u8 relayBF_tab[24] =
    {
        2, 2, 2, 2, 2, 2, 2, 1,
        1, 1, 1, 1, 1, 1, 0, 0,
        0, 0, 0, 0, 2, 0, 0, 1};

void LED_Init(void)
{
    io_init(p0, pin3, io_tuiwan);
    // h595d_init();    // 2025.6.23 新增(两路可控硅调光扩展板)继电器,由于io口冲突,故而屏蔽 此行
    my_relay_init();
}

void led_set(uchar led_No, uchar open)
{
    if (open == 0) {
        if (led_No == 1)
            Led1_H;
    } else {
        if (led_No == 1)
            Led1_L;
    }
}

u16 led_shan_num = 0;
u8 S_timers; // 闪烁次数
u16 S_PIN;   // 闪烁频率
void led_shan_sub(void)
{
    if (S_timers > 0) {
        MCU_flag.led_shanFlag = 1;
        led_shan_num++;
        if (led_shan_num >= S_PIN) {
            if (Led1_re > 0) {
                Led1_L;
            } else {
                Led1_H;
            }
            S_timers--;
            led_shan_num = 0;
        }
    }
    if (S_timers == 0) {
        MCU_flag.led_shanFlag = 0;
    }
}

void led_shan(u8 timers, u16 pinLV)
{
    if (S_timers == 0) {
        S_timers = timers * 2;
        S_PIN    = pinLV;
    }
}

// 2025.6.23 新增(两路可控硅调光扩展板)继电器
void my_relay_init(void)
{
    io_init(p1, pin4, io_tuiwan);
    io_init(p1, pin5, io_tuiwan);
}

void h595d_init(void)
{
    io_init(p2, pin3, io_tuiwan);
    io_init(p1, pin4, io_tuiwan);
    io_init(p1, pin5, io_tuiwan);
}

void h595d_write(u8 *buf, u8 num)
{
    u8 temp, i, j;
    // r_clk_pc=0;
    r_clk_L;
    delay_us(1);
    for (i = 0; i < num; i++) {
        temp = buf[i];
        for (j = 0; j < 8; j++) {
            // st_clk_pc=0;
            st_clk_L;
            delay_us(1);
            if (temp & 0x80) {
                // seg_pc=1;
                seg_H;
            } else {
                // seg_pc=0;
                seg_L;
            }
            delay_us(1);
            // st_clk_pc=1;
            st_clk_H;
            delay_us(1);
            temp = temp << 1;
        }
    }

    delay_us(1);
    // r_clk_pc=1;
    r_clk_H;
    delay_us(1);
    r_clk_L;
    // r_clk_pc=0;
}

void relay_set(u8 relay_g, u8 open_g)
{
    if ((relay_g > 0) & (relay_g <= 24)) {
        if (open_g == 0) {
            MCU_flag.H595D_buf[relayBF_tab[relay_g - 1]] &= ~relaySX_tab[relay_g - 1];
        } else {
            MCU_flag.H595D_buf[relayBF_tab[relay_g - 1]] |= relaySX_tab[relay_g - 1];
        }
    }
}

u8 relay_read(u8 relay_g)
{
    if ((relay_g > 0) & (relay_g <= 24)) {
        if ((MCU_flag.H595D_buf[relayBF_tab[relay_g - 1]] & relaySX_tab[relay_g - 1]) > 0) {
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}

// 总开关
void relay_closeA(u32 relay1_24, u8 open_g)
{
    u8 i;
    for (i = 0; i < 24; i++) {
        if ((relay1_24 & (1 << i)) > 0) {
            relay_set(i + 1, open_g);
        }
    }
}

// 总开关
void relay_closeA_SBuf(u8 *buf, u8 open_g)
{
    u8 i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 8; j++) {
            if (buf[i] & (1 << j))
                relay_set(i * 8 + j + 1, open_g);
        }
    }
}
