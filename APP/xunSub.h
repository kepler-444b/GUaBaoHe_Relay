#ifndef __XUNSUB_H
#define __XUNSUB_H
#include "led.h"
#include "sys.h"
#include "timers.h"
#include "usart.h"
#include "key.h"
#include "strs.h"
#include "TG.h"

// #include "relay.h"
// #include "lafei.h"

void init(void);
void Thread_Ms_sub(void);
void Thread_Imm_sub(void);
void Thread_Ms100_sub(void);
void Set_default(void);

/// 函数和间隔自行调整
#define JG_time1  1
#define JG_sub1() UART1_ReadBoxSUB()

#define JG_time2  1000
#define JG_sub2() UART1_try()

#define JG_time3  1
#define JG_sub3()   \
    led_shan_sub(); \
    jishu_sub()

#define JG_time4   20
#define JG_sub4()  TG_Work() // Relay_sub();

#define JG_time5   2000
#define JG_sub5()  xunSub_nc() //;relay_try()

#define JG_time6   50000
#define JG_sub6()  xunSub_nc()

#define JG_time7   50000
#define JG_sub7()  xunSub_nc()

#define JG_time8   50000
#define JG_sub8()  xunSub_nc()

#define JG_time9   50000
#define JG_sub9()  xunSub_nc()

#define JG_time10  50000
#define JG_sub10() xunSub_nc()

////appsub head///////
void xunSub_timer(u16 mme);
void xunSub_diao(void);
void xunSub_nc(void);

#endif
