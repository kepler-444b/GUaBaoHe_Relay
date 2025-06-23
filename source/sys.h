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
#define io_inout  0 // ��������-���
#define io_tuiwan 1 // �������
#define io_in     2 // ����
#define io_inlou  3 // ��©����
// �洢��ַ��16K��4000Hbyte   ռ�����256���ֽ�
#define flash_addrS 0x3F00

//////////////////////////////////////////////////
typedef struct Pro_flag {
    u8 ver;
    // 0=ǿ������  1=ǿ��ת������ 3=����485����ǿ��485����
    u8 sysMode;
    u8 ms_flag;
    u16 ms100_flag;
    u8 Debug8_reg;
    u16 Debug16_reg;
    u16 Debug32_reg;
    // u8 Debug_buf[30];
    u8 MyMAC;
    u8 Keys;
    ////����

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
    u8 tg_init_ci; // ��һ�ο���--���յ����ֵλ0
    u8 data_shua;  // ����ˢ��

    // �������
    u16 fa_zhi;    // ������ֵ-���16A�̵���
    u16 fa_zhi30a; // ������ֵ-����30A�̵���
    u16 zero_num;  // ����ʱ���������20msʱ�䣬�Լ�ˢ�¼̵���

    u8 led_shanFlag; // ����ʶ��Ϊ1���ʾ������˸
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
