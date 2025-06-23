#include "key.h"
#include "led.h"

#include "usart.h"

extern Pro_flag_s MCU_flag;
void key_init(void)
{
    io_init(p3, pin3, io_inout);
    io_init(p3, pin2, io_inout);
}

void Get_MyMac(void)
{
    if (Bo1_re == 0)
        MCU_flag.MyMAC |= 0x01;
    if (Bo2_re == 0)
        MCU_flag.MyMAC |= 0x02;
    // MCU_flag.MyMAC=0x01;
}
