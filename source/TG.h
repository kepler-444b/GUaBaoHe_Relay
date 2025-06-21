#ifndef __TG_H
#define __TG_H	 
#include "sys.h"


/********************************************
Zero=p24  T1=p25   T2=p26
*********************************************/



#define T1_H 				Gpio_SetIO(p2, pin5, TRUE) 
#define T1_L 				Gpio_SetIO(p2, pin5, FALSE) 

#define T2_H 				Gpio_SetIO(p2, pin6, TRUE) 	
#define T2_L 				Gpio_SetIO(p2, pin6, FALSE) 


   
#define Zero_re             Gpio_GetIO(p2, pin4)


/////////////////////////TG_Sta///////////////////
#define _TG_sta_open        0x01

#define _TG_sta_Up          0x10
#define _TG_sta_Down        0x20

//调光
//调光参数
#define _TG_num				100				//value_init
#define _TG_max				205				//max		37	
#define _TG_min				100				//min		120

///////////////////////////////////////////
void TG_Init(void);            //初始化	
void TG_Work(void);
void TG_TimOVR(void);

void TG_Rev(u8 *buf);
void jishu_sub(void);


#endif

















