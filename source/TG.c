#include "TG.h"
#include "timers.h"
#include "led.h"
#include "timers.h"
#include "RelayAPP.h"

extern Pro_flag_s MCU_flag;
/****************************************************
计时器内的计数器
*****************************************************/
u16 TG1_num=0;
u16 TG2_num=0;
/****************************************************
调光起步值
*****************************************************/
u8 TG1_V=2;
u8 TG2_V=2;
/****************************************************
调光实际值
*****************************************************/
u8 TG1_SV;
u8 TG2_SV;

extern u8 Rcu_type;   //0=豪华主机魔盒类 1=新款主机


void TG_Init(void)
{
    io_init(p2, pin5, io_tuiwan);
    io_init(p2, pin6, io_tuiwan);
	//初始化外部IO 过零
    Gpio_InitIOExt(2, 4, GpioDirIn, TRUE, FALSE, FALSE, 0);
    
    //开启GPIO中断
    Gpio_ClearIrq(2, 4);
    Gpio_EnableIrq(2, 4, GpioIrqRising);
    EnableNvic(PORT2_IRQn, 3, TRUE);
    MCU_flag.ZMCU_TG[0]=0;
    MCU_flag.ZMCU_TG[1]=0;
	MCU_flag.ZMCU_TG[2]=0;
    MCU_flag.ZMCU_TG[3]=0;
	MCU_flag.ZMCU_TG[4]=0;
    MCU_flag.ZMCU_TG[5]=0;
	MCU_flag.tg_init_ci=1;
}

/*当没有过零时刷新继电器*/
void jishu_sub(void)
{
    MCU_flag.zero_num++;
    if(MCU_flag.zero_num>200)
    {
    	Relay_sub();
		//Relay30A_sub();
		if(MCU_flag.led_shanFlag==0)
			led_set(1, 0);
		MCU_flag.zero_num=0;
    }
}
/****************************************************
外部过零中断执行程序
*****************************************************/
u16 zero_delay=0;
u8  zero_en=0;

void ZeroEX(void)
{
    //过零使能
    if(zero_en==0)
    {
    	zero_delay=0;
		zero_en=1;
    }
    MCU_flag.zero_num=0;
}

void ZeroDelay(void)
{
    //刷新继电器
	if(zero_en==1)
	{
		zero_delay++;
		if(zero_delay>=MCU_flag.fa_zhi)
		{
			zero_en=0;
			Relay_sub();
			if(MCU_flag.led_shanFlag==0)
				led_set(1, 1);
		}
		if(zero_delay==MCU_flag.fa_zhi30a)
		{
			
		}
	}
    
}


//调光慢慢变亮
u8 tg_jianliang1=0;
u8 tg_jianliang2=0;

void tg_JL_sub()
{
    if(MCU_flag.MyMAC==0)
    {
        if((MCU_flag.ZMCU_TG[0]>=100)&(MCU_flag.ZMCU_TG[0]>=205))
        {
            if((208-MCU_flag.ZMCU_TG[0])*2>tg_jianliang1)
            {
                tg_jianliang1++;
            }
            else if((208-MCU_flag.ZMCU_TG[0])*2<tg_jianliang1)
            {
                tg_jianliang1--;
            }
        }
        else if(MCU_flag.ZMCU_TG[0]==0)
        {
            if(tg_jianliang1>0)
                tg_jianliang1--;
        }
        
        if((MCU_flag.ZMCU_TG[1]>=100)&(MCU_flag.ZMCU_TG[1]>=205))
        {
            if((208-MCU_flag.ZMCU_TG[1])*2>tg_jianliang2)
            {
                tg_jianliang2++;
            }
            else if((208-MCU_flag.ZMCU_TG[1])*2<tg_jianliang2)
            {
                tg_jianliang2--;
            }
        }
        else if(MCU_flag.ZMCU_TG[1]==0)
        {
            if(tg_jianliang2>0)
                tg_jianliang2--;
        }
    }
    if(MCU_flag.MyMAC==1)
    {
        if((MCU_flag.ZMCU_TG[2]>=100)&(MCU_flag.ZMCU_TG[2]>=205))
        {
            if((208-MCU_flag.ZMCU_TG[2])*2>tg_jianliang1)
            {
                tg_jianliang1++;
            }
            else if((208-MCU_flag.ZMCU_TG[2])*2<tg_jianliang1)
            {
                tg_jianliang1--;
            }
        }
        else if(MCU_flag.ZMCU_TG[2]==0)
        {
            if(tg_jianliang1>0)
                tg_jianliang1--;
        }
        
        if((MCU_flag.ZMCU_TG[3]>=100)&(MCU_flag.ZMCU_TG[3]>=205))
        {
            if((208-MCU_flag.ZMCU_TG[2])*2>tg_jianliang2)
            {
                tg_jianliang2++;
            }
            else if((208-MCU_flag.ZMCU_TG[3])*2<tg_jianliang2)
            {
                tg_jianliang2--;
            }
        }
        else if(MCU_flag.ZMCU_TG[3]==0)
        {
            if(tg_jianliang2>0)
                tg_jianliang2--;
        }
    }
}
u8 tg_buf[6]={0,0,0,0,0,0};
u8 suLv_num=0;
void TG_Work(void)
{
    u8 tg[8];
    tg[0]=MCU_flag.ZMCU_TG[0];
    tg[1]=MCU_flag.ZMCU_TG[1];
    tg[2]=MCU_flag.ZMCU_TG[2];
    tg[3]=MCU_flag.ZMCU_TG[3];
    tg[4]=MCU_flag.ZMCU_TG[4];
    tg[5]=MCU_flag.ZMCU_TG[5];
	
		if(tg[0]==0)
			{tg[0]=208;tg_buf[0]=208;}
		if(tg[1]==0)
			{tg[1]=208;tg_buf[1]=208;}
		if(tg[2]==0)
			{tg[2]=208;tg_buf[2]=208;}
		if(tg[3]==0)
			{tg[3]=208;tg_buf[3]=208;}
	
    if((Rcu_type==0)|(Rcu_type==2))
    {
    	suLv_num++;
        if(suLv_num<MCU_flag.ZMCU_TGSuDu)
            return;
        suLv_num=0;
		
        if(MCU_flag.MyMAC==0)
        {
        	
        	if(tg_buf[0]>tg[0])
            {
                tg_buf[0]--;
            }
            if(tg_buf[0]<tg[0])
            {
                tg_buf[0]++;
            }
			
            if(tg_buf[0]>0)
            {
                TG1_SV=(208-tg_buf[0])*2;
            }
            else
            {
                TG1_SV=0;
            }

			if(tg_buf[1]>tg[1])
            {
                tg_buf[1]--;
            }
            if(tg_buf[1]<tg[1])
            {
                tg_buf[1]++;
            }
            if(tg_buf[1]>0)
            {
                TG2_SV=(208-tg_buf[1])*2;
            }
            else
            {
                TG2_SV=0;
            }
        }

        else if(MCU_flag.MyMAC==1)
        {
        	if(tg_buf[2]>tg[2])
            {
                tg_buf[2]--;
            }
            if(tg_buf[2]<tg[2])
            {
                tg_buf[2]++;
            }
            if(tg_buf[2]>0)
            {
                TG1_SV=(208-tg_buf[2])*2;
            }
            else
            {
                TG1_SV=0;
            }

			if(tg_buf[3]>tg[3])
            {
                tg_buf[3]--;
            }
            if(tg_buf[3]<tg[3])
            {
                tg_buf[3]++;
            }
            if(tg_buf[3]>0)
            {
                TG2_SV=(208-tg_buf[3])*2;
            }
            else
            {
                TG2_SV=0;
            }
        }
    }
    
}


/****************************************************
外部过零中断
*****************************************************/

void TG_ZeroEX(void)
{
    
    //if(Zero_re==TRUE)
    //{
        Bt_Cnt16Set(TIM0,0xffff-30);
    T1_H;
    T2_H;
        TG1_num=0;
        TG2_num=0;
        delay_us(30);
    //}
}


/****************************************************
Tim中断计时
*****************************************************/
void TG_TimOVR(void)
{//时间溢出中断
	ZeroDelay();
	

    TG1_num++;
	TG2_num++;
    if(TG1_SV>0)
    {
    	if((TG1_num>=TG1_V)&(TG1_num<TG1_V+3))
    	{
    	    T1_L;
            
        }
        else if((TG1_num>=TG1_V+5+TG1_SV)&(TG1_num<=240))
        {
            T1_H;
            
        }
    }
    if(TG2_SV>0)
    {
    	if((TG2_num>=TG2_V)&(TG2_num<TG2_V+3))
    	{
    	    T2_L;
            
        }
        else if((TG2_num>=TG2_V+5+TG2_SV)&(TG2_num<=240))
        {
            T2_H;
            
        }
    }
}
//加强输出


void Gpio_IRQHandler(uint8_t u8Param)
{
    u32 tmp=*((uint32_t *)((uint32_t)&M0P_GPIO->P2ICLR));
    *((uint32_t *)((uint32_t)&M0P_GPIO->P2ICLR)) =tmp&0xef;
    TG_ZeroEX();
    ZeroEX();
}


void TG_Rev(u8 *buf)
{


    if((buf[0]==0xaa)&(buf[1]==0xbb))
    {
        TG1_V=buf[2];
        TG1_SV=buf[3];
    }
    else if((buf[0]==0xaa)&(buf[1]==0xcc))
    {
        //JianGe1=buf[2];
        MCU_flag.ZMCU_relay[0]=buf[2];
            MCU_flag.ZMCU_relay[1]=buf[3];
            MCU_flag.ZMCU_relay[2]=buf[4];
            MCU_flag.ZMCU_relay[3]=buf[5];
            MCU_flag.ZMCU_relay[4]=buf[6];
            MCU_flag.ZMCU_relay[5]=buf[7];
            MCU_flag.ZMCU_TG[0]=buf[8];
            MCU_flag.ZMCU_TG[1]=buf[9];
            MCU_flag.ZMCU_TG[2]=buf[10];
            MCU_flag.ZMCU_TG[3]=buf[11];
            MCU_flag.data_shua=1;
    }
}


