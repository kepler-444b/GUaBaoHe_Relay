#include "SetUp.h"
extern Pro_flag_s MCU_flag;

//////������ȼ��Ĵ���,��������һ����������
//////V1.0 ledת���������--�忨��ס���������忨����������
//////VF.0 ���涨�Ƴ���--��ס����Ҫled������Ҫ��ס��

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


	




