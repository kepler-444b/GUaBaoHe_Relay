#include "SetUp.h"
extern Pro_flag_s MCU_flag;

//////检测优先级寄存器,优先亮第一个数组数据
//////V1.0 led转换常规程序--插卡入住灯亮，不插卡不能响门铃
//////VF.0 邦奇定制程序--入住不需要led亮，不要入住线

int main(void)
{	
    MCU_flag.ver=0x10;
	init();
	while(1)
	{
		Thread_Ms_sub();
		Thread_Imm_sub();
		Thread_Ms100_sub();
	}	 
} 


	




