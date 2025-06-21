#include "RelayAPP.h"
#include "usart.h"
#include "led.h"
#include "key.h"

extern Pro_flag_s MCU_flag;

u8 Rcu_type = 2; // 0=豪华主机魔盒类 1=新款主机 2=按照拨码
u16 GetRelaybuf10(u8 mac)
{
    u8 x, y, i;
    u16 res = 0;
    u8 cc = mac * 10;
    for (i = 0; i < 10; i++)
    {
        x = (cc + i) / 8;
        y = (cc + i) % 8;
        if ((MCU_flag.ZMCU_relay[x] & (1 << y)) > 0)
            res |= 1 << i;
    }
    return res;
}

u16 GetRelaybuf11(u8 mac)
{
    u8 x, y, i;
    u16 res = 0;
    u8 cc = mac * 11;
    for (i = 0; i < 11; i++)
    {
        x = (cc + i) / 8;
        y = (cc + i) % 8;
        if ((MCU_flag.ZMCU_relay[x] & (1 << y)) > 0)
            res |= 1 << i;
    }
    return res;
}

void Relay_sub(void)
{
    u8 temp[6] = {0, 0, 0, 0, 0, 0};
    u8 temp1[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    u16 res = 0;
    if (MCU_flag.data_shua == 0)
        return;
    if (Rcu_type == 0)
    { // 豪华主机---自己按位
        if (MCU_flag.MyMAC == 0)
        {
            temp[0] = MCU_flag.ZMCU_relay[0];
            temp[1] = MCU_flag.ZMCU_relay[1];
            temp[2] = MCU_flag.ZMCU_relay[2] & 0x0f;
            if (MCU_flag.ZMCU_relay[4] & Zhu30A_1)
                temp[2] |= 0x10;
            else
                temp[2] &= ~0x10;
            relay_closeA_SBuf(temp, 1);
            temp[0] = ~temp[0];
            temp[1] = ~temp[1];
            temp[2] = (~temp[2]) & 0x1f;
            relay_closeA_SBuf(temp, 0);
        }
        else if (MCU_flag.MyMAC == 1)
        {
            temp[0] = ((MCU_flag.ZMCU_relay[2] & 0xf0) >> 4) | ((MCU_flag.ZMCU_relay[3] << 4) & 0xf0);
            temp[1] = ((MCU_flag.ZMCU_relay[3] & 0xf0) >> 4) | ((MCU_flag.ZMCU_relay[4] << 4) & 0xf0);
            temp[2] = ((MCU_flag.ZMCU_relay[4] & 0xf0) >> 4) & 0x0f;
            if (MCU_flag.ZMCU_relay[4] & Zhu30A_2)
                temp[2] |= 0x10;
            else
                temp[2] &= ~0x10;
            relay_closeA_SBuf(temp, 1);
            temp[0] = ~temp[0];
            temp[1] = ~temp[1];
            temp[2] = (~temp[2]) & 0x1f;
            relay_closeA_SBuf(temp, 0);
        }
        else if (MCU_flag.MyMAC == 2)
        {
            temp[0] = ((MCU_flag.ZMCU_relay[3] & 0xc0) >> 6) | ((MCU_flag.ZMCU_relay[4] << 2) & 0xfc);
            temp[1] = ((MCU_flag.ZMCU_relay[4] & 0xc0) >> 6) | ((MCU_flag.ZMCU_relay[5] << 2) & 0xfc);
            // temp[2]=((MCU_flag.ZMCU_relay[4]&0xf0)>>4)&0x0f;

            relay_closeA_SBuf(temp, 1);
            temp[0] = ~temp[0];
            temp[1] = ~temp[1];
            // temp[2]=(~temp[2])&0x1f;
            relay_closeA_SBuf(temp, 0);
        }
        else if (MCU_flag.MyMAC == 3)
        {
            temp[0] = ((MCU_flag.ZMCU_relay[3] & 0xc0) >> 6) | ((MCU_flag.ZMCU_relay[4] << 2) & 0xfc);
            temp[1] = ((MCU_flag.ZMCU_relay[4] & 0xc0) >> 6) | ((MCU_flag.ZMCU_relay[5] << 2) & 0xfc);
            // temp[2]=((MCU_flag.ZMCU_relay[4]&0xf0)>>4)&0x0f;

            relay_closeA_SBuf(temp, 1);
            temp[0] = ~temp[0];
            temp[1] = ~temp[1];
            // temp[2]=(~temp[2])&0x1f;
            relay_closeA_SBuf(temp, 0);
        }
    }
    else if (Rcu_type == 1)
    { // 新款主机
        if (MCU_flag.MyMAC == 0)
        {
            relay_closeA_SBuf(temp1, 0);
            temp[0] = MCU_flag.ZMCU_relay[0];
            temp[1] = MCU_flag.ZMCU_relay[1];
            relay_closeA_SBuf(temp, 1);
        }
        else if (MCU_flag.MyMAC == 1)
        {
            relay_closeA_SBuf(temp1, 0);
            temp[0] = (MCU_flag.ZMCU_relay[1] >> 2) | (MCU_flag.ZMCU_relay[2] << 6);
            temp[1] = (MCU_flag.ZMCU_relay[2] >> 2);
            relay_closeA_SBuf(temp, 1);
        }
        else if (MCU_flag.MyMAC == 2)
        {
            relay_closeA_SBuf(temp1, 0);
            temp[0] = (MCU_flag.ZMCU_relay[2] >> 4) | (MCU_flag.ZMCU_relay[3] << 4);
            temp[1] = (MCU_flag.ZMCU_relay[3] >> 4);
            relay_closeA_SBuf(temp, 1);
        }
    }
    else if (Rcu_type == 2)
    { // 豪华主机按拨码计算
        relay_closeA_SBuf(temp1, 0);
        // res=GetRelaybuf10(MCU_flag.MyMAC);
        res = GetRelaybuf11(MCU_flag.MyMAC);
        temp[0] = res & 0xff;
        temp[1] = (res & 0xff00) >> 8;
        relay_closeA_SBuf(temp, 1);
    }
    h595d_write(MCU_flag.H595D_buf, 3);
    MCU_flag.data_shua = 0;
}

/**************************************
RCU数据处理
**************************************/
void RcuData_rev(u8 *dat)
{
    u16 crc16;
    u8 crc = GetCheckHe(dat, 12);
    u8 sends[10];
    if ((crc == dat[12]) & (dat[0] == 0xfb))
    {
        led_shan(3, 50);
        // Rcu_type=0;
        if (dat[1] == 1)
        {

            MCU_flag.ZMCU_relay[0] = dat[2];
            MCU_flag.ZMCU_relay[1] = dat[3];
            MCU_flag.ZMCU_relay[2] = dat[4];
            MCU_flag.ZMCU_relay[3] = dat[5];
            MCU_flag.ZMCU_relay[4] = dat[6];
            MCU_flag.ZMCU_relay[5] = dat[7];
            if (dat[8] == 0)
                MCU_flag.ZMCU_TG[0] = 208;
            else
                MCU_flag.ZMCU_TG[0] = dat[8];
            if (dat[9] == 0)
                MCU_flag.ZMCU_TG[1] = 208;
            else
                MCU_flag.ZMCU_TG[1] = dat[9];
            if (dat[10] == 0)
                MCU_flag.ZMCU_TG[2] = 208;
            else
                MCU_flag.ZMCU_TG[2] = dat[10];
            if (dat[11] == 0)
                MCU_flag.ZMCU_TG[3] = 208;
            else
                MCU_flag.ZMCU_TG[3] = dat[11];
            MCU_flag.data_shua = 1;
            MCU_flag.tg_init_ci = 0;
        }
    }
    if ((dat[0] == 0xfb) & (dat[1] == 0x1) & (dat[12] == 0xce) & (dat[13] == 0xec))
    {
        led_shan(3, 50);
        MCU_flag.ZMCU_relay[0] = dat[2];
        MCU_flag.ZMCU_relay[1] = dat[3];
        MCU_flag.ZMCU_relay[2] = dat[4];
        MCU_flag.ZMCU_relay[3] = dat[5];
        MCU_flag.ZMCU_relay[4] = dat[6];
        MCU_flag.ZMCU_relay[5] = dat[7];
        MCU_flag.ZMCU_TG[0] = dat[8];
        MCU_flag.ZMCU_TG[1] = dat[9];
        MCU_flag.ZMCU_TG[2] = dat[10];
        MCU_flag.ZMCU_TG[3] = dat[11];
        MCU_flag.ZMCU_TGSuDu = dat[12];
        MCU_flag.data_shua = 1;
        MCU_flag.tg_init_ci = 0;
    }
    else if ((dat[0] == 0x5a) & (dat[1] == 0x29))
    {
        crc16 = crc_chk_value(dat, 27);
        if (crc16 != (dat[27] * 256 + dat[28]))
            return;
        // Rcu_type=1;
        led_shan(3, 50);
        MCU_flag.ZMCU_relay[0] = dat[2];
        MCU_flag.ZMCU_relay[1] = dat[3];
        MCU_flag.ZMCU_relay[2] = dat[4];
        MCU_flag.ZMCU_relay[3] = dat[5];
        MCU_flag.ZMCU_relay[4] = dat[6];
        MCU_flag.ZMCU_relay[5] = dat[7];
        // MCU_flag.ZMCU_relay[6]=dat[8];
        // MCU_flag.ZMCU_relay[7]=dat[9];
        MCU_flag.ZMCU_TG[0] = dat[10];
        MCU_flag.ZMCU_TG[1] = dat[11];
        MCU_flag.ZMCU_TG[2] = dat[12];
        MCU_flag.ZMCU_TG[3] = dat[13];
        MCU_flag.ZMCU_TG[4] = dat[14];
        // MCU_flag.ZMCU_TG[5]=dat[15];
        // MCU_flag.ZMCU_TG[6]=dat[16];
        // MCU_flag.ZMCU_TG[7]=dat[17];
        // MCU_flag.ZMCU_TG[8]=dat[18];
        // MCU_flag.ZMCU_TG[9]=dat[19];
        // MCU_flag.ZMCU_TG[10]=dat[20];
        // MCU_flag.ZMCU_TG[11]=dat[21];
        // MCU_flag.ZMCU_TG[12]=dat[22];
        // MCU_flag.ZMCU_TG[13]=dat[23];
        // MCU_flag.ZMCU_TG[14]=dat[24];
        // MCU_flag.ZMCU_TG[15]=dat[25];
        // MCU_flag.ZMCU_TGSuDu=dat[26];
        // jishu_clear();
        MCU_flag.tg_init_ci = 0;
        MCU_flag.data_shua = 1;
    }
    else if ((dat[0] == 0xa1) & (dat[1] == 0xa2) & (dat[2] == 0xa3))
    { // 调试过零
        led_shan(3, 50);
        return;
        MCU_flag.fa_zhi = dat[3] * 256 + dat[4];
        MCU_flag.fa_zhi30a = dat[5] * 256 + dat[6];
        sends[0] = 0x7f;
        sends[1] = MCU_flag.fa_zhi / 256;
        sends[2] = MCU_flag.fa_zhi % 256;
        sends[3] = MCU_flag.fa_zhi30a / 256;
        sends[4] = MCU_flag.fa_zhi30a % 256;
        UART1_SendBox(sends, 5);
    }
    else if ((dat[0] == 0xa1) & (dat[1] == 0xa3))
    { // 调试过零
        led_shan(3, 50);
        return;
        MCU_flag.ZMCU_relay[0] = dat[2];
        MCU_flag.ZMCU_relay[1] = dat[3];
        MCU_flag.ZMCU_relay[2] = dat[4];
        MCU_flag.ZMCU_relay[3] = dat[5];
        MCU_flag.ZMCU_relay[4] = dat[6];
        MCU_flag.ZMCU_relay[5] = dat[7];
        MCU_flag.ZMCU_TG[0] = dat[8];
        MCU_flag.ZMCU_TG[1] = dat[9];
        MCU_flag.ZMCU_TG[2] = dat[10];
        MCU_flag.ZMCU_TG[3] = dat[11];
        // jishu_clear();
        MCU_flag.data_shua = 1;
        sends[0] = 0x7f;
        sends[1] = MCU_flag.fa_zhi / 256;
        sends[2] = MCU_flag.fa_zhi % 256;
        sends[3] = MCU_flag.fa_zhi30a / 256;
        sends[4] = MCU_flag.fa_zhi30a % 256;
        UART1_SendBox(sends, 5);
    }
}

u8 opens = 0;
void relay_try(void)
{
    if (opens == 0)
    {
        opens = 1;
        MCU_flag.ZMCU_relay[0] = 0xff;
        MCU_flag.ZMCU_relay[1] = 0xff;
        MCU_flag.ZMCU_relay[2] = 0xff;
        MCU_flag.ZMCU_relay[3] = 0xff;
        MCU_flag.ZMCU_relay[4] = 0xff;
        MCU_flag.ZMCU_relay[5] = 0xff;
        MCU_flag.data_shua = 1;
    }
    else
    {
        opens = 0;
        MCU_flag.ZMCU_relay[0] = 0;
        MCU_flag.ZMCU_relay[1] = 0;
        MCU_flag.ZMCU_relay[2] = 0;
        MCU_flag.ZMCU_relay[3] = 0;
        MCU_flag.ZMCU_relay[4] = 0;
        MCU_flag.ZMCU_relay[5] = 0;
        MCU_flag.data_shua = 1;
    }
}
