#include "xunSub.h"

extern Pro_flag_s MCU_flag;

// HC32L110C6UA引脚
/**************************************
**01=p00/rest                   11=p24
**02=p01                        12=p25
**03=p02                        13=p26
**04=avss                       14=p27/swdio
**05=vcap                       15=p31/swclk
**06=dvcc                       16=p32
**07=p03                        17=p33
**08=p15                        18=p34
**09=p14                        19=p35
**10=p23                        20=p36
//03=bopan/p02  07=ML/p03 08=ss-in/p15 09=rz-in/p14  10=key/p23
//11=led1/p24 12=led2/p25 13=led3/p26 17=QL-in/p33 18=ch/p34 19 =tx/p35 20=rx/p36
//16=wr/p32
***************************************/

void init(void)
{
    App_ClkInit(RCH16M);
    // App_ClkInit(XTH);
    wdog_init();
    LED_Init();
    uart1_init();
    key_init();
    timer0_Init();
    // timer2_Init();
    Get_MyMac();
    TG_Init();
    // 由于有2路调光，此值与其它不一样
    MCU_flag.fa_zhi = 0x90;
    led_shan(3, 50);
    MCU_flag.ZMCU_TGSuDu = 0;
    // MCU_flag.Debug32_reg= Clk_GetHClkFreq();
    // MCU_flag.data_shua=1;
    // relay_closeA(0xffffff, 1);
}

void Thread_Ms_sub(void)
{
    // u8 temp;
    if (MCU_flag.ms_flag >= 33)
    {
        xunSub_diao();
        Wdt_Feed();
        MCU_flag.ms_flag = 0;
    }
}
void Thread_Ms100_sub(void)
{
    if (MCU_flag.ms100_flag >= 1000)
    {

        MCU_flag.ms100_flag = 0;
    }
}

void Set_default(void)
{
}

int32_t main(void)
{

    init();

    while (1)
    {
        Thread_Ms_sub();
    }
}
