#ifndef __SYS_H
#define __SYS_H
#include "gpio.h"
#include "bt.h"
#include "uart.h"
#include "flash.h"
#include "wdt.h"

#include <stdio.h>

#define uchar     unsigned char
#define uint      unsigned int
#define uint32    unsigned long
#define ulong     unsigned long

#define u8        unsigned char
#define u16       unsigned int
#define u32       unsigned long

#define p0        0
#define p1        1
#define p2        2
#define p3        3

#define pin0      0
#define pin1      1
#define pin2      2
#define pin3      3
#define pin4      4
#define pin5      5
#define pin6      6
#define pin7      7

#define RCH4M     0
#define RCH8M     1
#define RCH16M    2
#define RCH22M    3
#define RCH24M    4
#define XTH       5
#define XTL       6
#define RCL38     7
#define RCL32     8

#define bit0      0x01
#define bit1      0x02
#define bit2      0x04
#define bit3      0x08
#define bit4      0x10
#define bit5      0x20
#define bit6      0x40
#define bit7      0x80
#define io_inout  0 // 上拉输入-输出
#define io_tuiwan 1 // 推挽输出
#define io_in     2 // 输入
#define io_inlou  3 // 开漏输入
// 存储地址，16K的4000Hbyte   占用最后256个字节
#define flash_addrS 0x3F00

//////////////////////////////////////////////////
typedef struct Pro_flag {
    u8 ver;
    // 0=强电门牌  1=强电转换门牌 3=弱电485或者强电485门牌
    u8 sysMode;
    u8 ms_flag;
    u16 ms100_flag;
    u8 Debug8_reg;
    u16 Debug16_reg;
    u16 Debug32_reg;
    // u8 Debug_buf[30];
    u8 MyMAC;
    u8 Keys;
    ////门牌

    u8 Rbuf_num;
    u8 Rbuf[100];

    u8 H595D_buf[3];
    u8 Rev_buf[30];
    u8 Rev_num;
    u8 Rev_sta;
    u8 Rev_CRC;

    u8 ZMCU_relay[6];
    u8 ZMCU_TG[16];
    u8 ZMCU_TGSuDu;
    u8 TG1_sta;
    u8 TG1_Value;

    u8 TG2_sta;
    u8 TG2_Value;
    u8 tg_init_ci; // 第一次开机--等收到码后值位0
    u8 data_shua;  // 数据刷新

    // 过零测试
    u16 fa_zhi;    // 过零阈值-汇港16A继电器
    u16 fa_zhi30a; // 过零阈值-欣大30A继电器
    u16 zero_num;  // 过零时清除，超过20ms时间，自己刷新继电器

    u8 led_shanFlag; // 闪标识，为1则表示正在闪烁
} Pro_flag_s;

void App_ClkInit(u8 pinlv);

void io_init(u8 enPort, u8 enPin, u8 fangxiang);
u16 Get_UID(u8 *buf);

void delay_us(u16 nus);
void delay_ms(u16 ms);
unsigned int crc_chk_value(unsigned char *data_value, unsigned char length);
u8 GetCheckCodeX(u8 *cmd, u8 BoxLen);
u8 GetCheckHe(u8 *cmd, u8 BoxLen);

void flash_init(void);

void Data_save(u8 *data, u8 len);
void Data_load(u8 *data, u8 len);

void wdog_init(void);

#endif
