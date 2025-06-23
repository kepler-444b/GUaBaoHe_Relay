#ifndef __key_H
#define __key_H
#include "sys.h"
/*******************************************************
输入端口统一设定
*key1=p26
********************************************************/


#define Bo1_re              Gpio_GetIO(p3, pin3)    //17
#define Bo2_re              Gpio_GetIO(p3, pin2)    //16


void key_init(void);

void Get_MyMac(void);


#endif

