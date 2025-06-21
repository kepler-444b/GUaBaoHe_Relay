#include "xunSub.h"


//计时，1ms一次
u16 xunSub_jishi[10]={0,0,0,0,0,0,0,0,0,0};
u16 xunSub_num;
void xunSub_timer(u16 mme)
{
    xunSub_num++;
    if(xunSub_num>=mme)
    {
        xunSub_jishi[0]++;
        xunSub_jishi[1]++;
        xunSub_jishi[2]++;
        xunSub_jishi[3]++;
        xunSub_jishi[4]++;
        xunSub_jishi[5]++;
        xunSub_jishi[6]++;
        xunSub_jishi[7]++;
        xunSub_jishi[8]++;
        xunSub_jishi[9]++;
        xunSub_num=0;
    }
}



void xunSub_diao(void)
{
    if(JG_time1>0)
    {
        if(xunSub_jishi[0]>=JG_time1)
        {
            JG_sub1();
            xunSub_jishi[0]=0;
        }
    }

    if(JG_time2>0)
    {
        if(xunSub_jishi[1]>=JG_time2)
        {
            JG_sub2();
            xunSub_jishi[1]=0;
        }
    }

    if(JG_time3>0)
    {
        if(xunSub_jishi[2]>=JG_time3)
        {
            JG_sub3();
            xunSub_jishi[2]=0;
        }
    }

    if(JG_time4>0)
    {
        if(xunSub_jishi[3]>=JG_time4)
        {
            JG_sub4();
            xunSub_jishi[3]=0;
        }
    }

    if(JG_time5>0)
    {
        if(xunSub_jishi[4]>=JG_time5)
        {
            JG_sub5();
            xunSub_jishi[4]=0;
        }
    }

    if(JG_time6>0)
    {
        if(xunSub_jishi[5]>=JG_time6)
        {
            JG_sub6();
            xunSub_jishi[5]=0;
        }
    }

    if(JG_time7>0)
    {
        if(xunSub_jishi[6]>=JG_time7)
        {
            JG_sub7();
            xunSub_jishi[6]=0;
        }
    }

    if(JG_time8>0)
    {
        if(xunSub_jishi[7]>=JG_time8)
        {
            JG_sub8();
            xunSub_jishi[7]=0;
        }
    }

    if(JG_time9>0)
    {
        if(xunSub_jishi[8]>=JG_time9)
        {
            JG_sub9();
            xunSub_jishi[8]=0;
        }
    }

    if(JG_time10>0)
    {
        if(xunSub_jishi[9]>=JG_time10)
        {
            JG_sub10();
            xunSub_jishi[9]=0;
        }
    }

    
    
}

void xunSub_nc(void)
{

}









