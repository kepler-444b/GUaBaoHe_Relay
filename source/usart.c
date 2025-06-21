#include "usart.h"
#include "TG.h"
#include "RelayAPP.h"




#define  StackLen_Value  100
u8 BufRead_JiShi=0;						/////�հ�ʱ�������	

extern Pro_flag_s MCU_flag;
int fputc(int ch, FILE *f)
{

    uart1_send((u8)ch);
    return ch;
}


////�����ж�
void RxIntCallback(void)
{
    u8 temp;
    temp = M0P_UART1->SBUF;
    UART1_R_Interr(temp,StackLen_Value);

}
void ErrIntCallback(void)
{
  
}

static void App_UartInit(void)
{
   uint16_t timer=0;
    uint32_t pclk=0;
	
    stc_uart_config_t  stcConfig;
    stc_uart_irq_cb_t stcUartIrqCb;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_config_t stcBaud;
    stc_bt_config_t stcBtConfig;
    

    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcBtConfig);

    
    //Gpio_InitIO(T1_PORT,T1_PIN,GpioDirIn); 
    //Gpio_InitIO(0,3,GpioDirOut);
    //Gpio_SetIO(0,3,1);
    
    Gpio_InitIOExt(3,5,GpioDirOut,TRUE,FALSE,FALSE,FALSE);   
    Gpio_InitIOExt(3,6,GpioDirOut,TRUE,FALSE,FALSE,FALSE); 
    
    //ͨ���˿�����
    Gpio_SetFunc_UART1TX_P35();
    Gpio_SetFunc_UART1RX_P36();

    
    //����ʱ��ʹ��
    Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//ģʽ0/2���Բ�ʹ��
    Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);



    stcUartIrqCb.pfnRxIrqCb = RxIntCallback;
    stcUartIrqCb.pfnTxIrqCb = NULL;
    stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
    stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;
  

    stcConfig.enRunMode = UartMode1;//��������Ĵ˴���ת��4��ģʽ����
   

    stcMulti.enMulti_mode = UartNormal;//��������Ĵ˴���ת��������ģʽ��mode2/3���ж�����ģʽ

    stcConfig.pstcMultiMode = &stcMulti;

    stcBaud.bDbaud = 0u;//˫�������ʹ���
    stcBaud.u32Baud = 9600u;//���²�����λ��
    stcBaud.u8Mode = UartMode1; //���㲨������Ҫģʽ����
    pclk = Clk_GetPClkFreq();
    timer=Uart_SetBaudRate(UARTCH1,pclk,&stcBaud);

    stcBtConfig.enMD = BtMode2;
    stcBtConfig.enCT = BtTimer;
    Bt_Init(TIM1, &stcBtConfig);//����basetimer1���ú�������������
    Bt_ARRSet(TIM1,timer);
    Bt_Cnt16Set(TIM1,timer);
    Bt_Run(TIM1);

    Uart_Init(UARTCH1, &stcConfig);
    Uart_EnableIrq(UARTCH1,UartRxIrq);
    Uart_ClrStatus(UARTCH1,UartRxFull);
    Uart_EnableFunc(UARTCH1,UartRx);

}

void uart1_init(void)
{
    //CH
    io_init(p3, pin4, io_tuiwan);
	App_UartInit();
    CH_L;
}	


void  uart1_send(uchar dat)
{
   
   Uart_SendData(UARTCH1, dat); //����UART1���͵�һ���ֽ�
}






//////�������ݴ���
void UART1_ReadBoxSUB(void)

{
	BufRead_JiShi++;
	if(BufRead_JiShi>=5)
	{
		if(MCU_flag.Rbuf_num>0)
		{//�յ����ݴ���
		    //led_shan(3, 50);
		    //TG_Rev(MCU_flag.Rbuf);
		    RcuData_rev(MCU_flag.Rbuf);
		}
		MCU_flag.Rbuf_num=0;
	}
}



////////////////////////////////////////////////////buf-delay//////////////////////////////
////���ݷ���
void UART1_SendBox(u8 *buf,u8 len)
{   
	u8 i;   
    CH_H;
    for(i=0;i<len;i++)
    	uart1_send(buf[i]);
    delay_us(200);
    CH_L;
}





//////�����жϴ���
/////���ݳ��ȣ���ջ����
void UART1_R_Interr(u8 datas,u8 StackLen)
{
	if(StackLen==0)
		return;
	MCU_flag.Rbuf[MCU_flag.Rbuf_num]=datas;
    if(MCU_flag.Rbuf_num<(StackLen-1))
    	MCU_flag.Rbuf_num++;
    else
    	MCU_flag.Rbuf_num=0;
    BufRead_JiShi=0;
}



////////////
//u8 mm[2];
void UART1_try(void)
{
     //Uart_SendDataIt(M0P_UART1, 55);//��������
    //Uart_SetTb8(M0P_UART1,UartEven,0);
    //Uart_SendDataIt(M0P_UART1,MCU_flag.Debug8_reg++);
    //Uart_SetTb8(M0P_UART1,UartEven,2);
    //Uart_SendDataPoll(M0P_UART1,3);
    //mm[0]++;
    //UART1_SendBox(mm, 1);
}





