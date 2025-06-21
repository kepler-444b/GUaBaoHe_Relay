#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
/*******************************************************
����˿�ͳһ�趨
*KLED��7��=p03  
st_clk10=p23
r_clk9=p14
seg8=p15
********************************************************/



#define KLED                0x01
#define Led1_re             Gpio_GetIO(p0, pin3)        
#define Led1_H              Gpio_SetIO(p0, pin3, TRUE) 
#define Led1_L              Gpio_SetIO(p0, pin3, FALSE)



#define relay_init              h595d_init
 

#define st_clk_H				Gpio_SetIO(p2, pin3, TRUE)		//��λ�Ĵ���
#define	r_clk_H				    Gpio_SetIO(p1, pin4, TRUE)		//д������      
#define	seg_H					Gpio_SetIO(p1, pin5, TRUE)		//����        

#define st_clk_L				Gpio_SetIO(p2, pin3, FALSE)		//��λ�Ĵ���
#define	r_clk_L				    Gpio_SetIO(p1, pin4, FALSE)		//д������      
#define	seg_L					Gpio_SetIO(p1, pin5, FALSE)		//����     




#define kNC1_buf0					0x01
#define k20_buf0					0x02            
#define k19_buf0					0x04
#define k18_buf0					0x08
#define k17_buf0					0x10
#define k16_buf0					0x20
#define k15_buf0					0x40
#define kNC2_buf0					0x80


#define k8_buf1					    0x01
#define kNC3_buf1					0x02
#define k14_buf1					0x04
#define k13_buf1					0x08
#define k12_buf1					0x10
#define k11_buf1					0x20
#define k10_buf1					0x40
#define k9_buf1					    0x80


#define k1_buf2						0x01
#define k21_buf2				    0x02
#define k7_buf2					    0x04
#define k6_buf2					    0x08
#define k5_buf2					    0x10
#define k4_buf2					    0x20
#define k3_buf2						0x40
#define k2_buf2					    0x80

#define Zhu30A_1					0x40			//��5���ֽ�
#define Zhu30A_2					0x80			//��5���ֽ�




void LED_Init(void);
void led_set(uchar led_No,uchar open);
void led_shan_sub(void);
void led_shan(u8 timers,u16 pinLV);

void h595d_init(void);
void h595d_write(u8 *buf,u8 num); 				//sub	
void relay_set(u8 relay_g,u8 open_g);
void relay_closeA(u32 relay1_24, u8 open_g);
u8 relay_read(u8 relay_g);
void relay_closeA_SBuf(u8 *buf, u8 open_g);
void Relay_sub(void);


#endif

















