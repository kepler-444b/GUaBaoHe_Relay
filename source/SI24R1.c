#include "SI24R1.h"
#include "usart.h"
#include "strs.h"


extern Pro_flag_s MCU_flag;


u8 SI24R1_en=0;
u8  TX_ADDRESS[TX_ADR_WIDTH] = {0x5A,0x4B,0x3C,0x2D,0x0F};  // ����һ����̬���͵�ַ

u8 SPI_RW(u8 byte)
{
	u8 bit_ctr;
	for(bit_ctr=0; bit_ctr<8; bit_ctr++)
	{
		if(byte & 0x80)
			MOSI_1;
		else
			MOSI_0;																 
		byte = (byte << 1);                      
		SCK_1;          
        delay_us(1);
		byte |= MISO;                             
		SCK_0;   
        delay_us(1);
	}
	return(byte);                              
}
 
uint8_t NRF24L01_Check(void)
{
 uint8_t buf[5]={0xA5,0xA5,0xA5,0xA5,0xA5};
 uint8_t buf1[5]={0};
 uint8_t i; 
 
 SI24R1_Write_Buf(WRITE_REG+TX_ADDR,buf,5);//??5??????. 
 SI24R1_Read_Buf(TX_ADDR,buf1,5); //???????  
 for(i=0;i<5;i++)if(buf1[i]!=0XA5)break;            
 if(i!=5)
    return 1;//??24L01?? 
 return 0;   //???24L01
}

/********************************************************
�������ܣ�SI24R1���ų�ʼ��                
��ڲ�������
����  ֵ����
*********************************************************/
void SI24R1_Init(void)
{
//    u8 state=0;
	io_init(p2, pin3, io_in);
    io_init(p2, pin5, io_inout);
    io_init(p0, pin3, io_tuiwan);
    io_init(p1, pin4, io_tuiwan);
    io_init(p1, pin5, io_tuiwan);
    io_init(p2, pin4, io_tuiwan);
    SCK_0;CSN_1;CE_0;
    //NRF24L01_Check();
    //SI24R1_TX_Mode();  
    //state=SI24R1_TxPacket(temp);
    SI24R1_RX_ModeSet(20);  
	
	
	//SCK_0;
    
}


/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��WRITE_REG��reg��
					value:�Ĵ�����ֵ
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
u8 SI24R1_Write_Reg(u8 reg, u8 value)
{
	u8 status;


    CSN_0;
	status = SPI_RW(reg);				
	SPI_RW(value);
    CSN_1;
 
	
	return(status);
}


/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��WRITE_REG��reg��
					pBuf:д�����׵�ַ
					bytes:д�����ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
u8 SI24R1_Write_Buf(u8 reg,  u8 *pBuf, u8 bytes)
{
	u8 status,byte_ctr;


  CSN_0;
  status = SPI_RW(reg);                          
  for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)     
    SPI_RW(*pBuf++);
                                    	
    CSN_1;
  return(status);       
}							  					   


/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��READ_REG��reg��
����  ֵ���Ĵ���ֵ
*********************************************************/
u8 SI24R1_Read_Reg(u8 reg)
{
 	u8 value;
    CSN_0;
 
	SPI_RW(reg);			
	value = SPI_RW(0);
            
    CSN_1;
	return(value);
}


/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ��READ_REG��reg��
					pBuf:���ջ��������׵�ַ
					bytes:��ȡ�ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
u8 SI24R1_Read_Buf(u8 reg, u8 *pBuf, u8 bytes)
{
	u8 status,byte_ctr;


  CSN_0;
  status = SPI_RW(reg);                           
  for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    pBuf[byte_ctr] = SPI_RW(0);                   //��ȡ���ݣ����ֽ���ǰ
                                      
    CSN_1;
  return(status);    
}
/********************************************************
�������ܣ�SI24R1���üĴ���                      
��ڲ�������
����  ֵ����
*********************************************************/
void SI24R1_Set(void)
{
    
}
#define sl_gL   0x0f

/********************************************************
�������ܣ�SI24R1����ģʽ��ʼ��                      
��ڲ�������
����  ֵ����
*********************************************************/
void SI24R1_RX_Mode(void)
{

    CE_0;
	SI24R1_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
	SI24R1_Write_Reg(WRITE_REG + EN_AA, 0x01);               						// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	SI24R1_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);           						// ʹ�ܽ���ͨ��0
	SI24R1_Write_Reg(WRITE_REG + RF_CH, 20);                 						// ѡ����Ƶͨ��0x40
	SI24R1_Write_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  						// ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
	SI24R1_Write_Reg(WRITE_REG + RF_SETUP, sl_gL);            						// ���ݴ�����2Mbps�����书��7dBm
	SI24R1_Write_Reg(WRITE_REG + CONFIG, 0x0f);              						// CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
	SI24R1_Write_Reg(WRITE_REG + STATUS, 0xff);  												//������е��жϱ�־λ
	CE_1;                                      									// ����CE���������豸
}						
/********************************************************
�������ܣ�SI24R1����ģʽ��ʼ��                      
��ڲ�����Channel �ŵ�1-125  ÿ��1M     Ĭ��20
        rx_set������λ����+bit3��������    Ĭ��0x0f
����  ֵ����
*********************************************************/

void SI24R1_RX_ModeSet(u8 Channel)
{

    CE_0;
	SI24R1_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
	SI24R1_Write_Reg(WRITE_REG + EN_AA, 0x01);               						// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	SI24R1_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);           						// ʹ�ܽ���ͨ��0
	SI24R1_Write_Reg(WRITE_REG + RF_CH, Channel);                 						// ѡ����Ƶͨ��0x40
	SI24R1_Write_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  						// ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
	SI24R1_Write_Reg(WRITE_REG + RF_SETUP, sl_gL);            						// ���ݴ�����2Mbps�����书��7dBm
	SI24R1_Write_Reg(WRITE_REG + CONFIG, 0x0f);              						// CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
	SI24R1_Write_Reg(WRITE_REG + STATUS, 0xff);   												//������е��жϱ�־λ
	CE_1;                                      									// ����CE���������豸
}						
/********************************************************
�������ܣ�SI24R1����ģʽ��ʼ��                      
��ڲ�����Channel �ŵ�1-125  ÿ��1M     Ĭ��20
        rx_set������λ����+bit3��������    Ĭ��0x0f
        Cfa:H�Զ��ط����ü��ʱ��250US-4000US��L�Զ��ط�����
����  ֵ����
*********************************************************/
void SI24R1_TX_ModeSet(u8 Channel,u8 setR)
{
	//CE = 0;
    CE_0;
	SI24R1_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // д�뷢�͵�ַ
	SI24R1_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ

	SI24R1_Write_Reg(WRITE_REG + EN_AA, 0x01);       //ʹ������ͨ���Զ�ȷ��											// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	SI24R1_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);   //ʹ�ܽ������ݹܵ���ַ											// ʹ�ܽ���ͨ��0
	SI24R1_Write_Reg(WRITE_REG + SETUP_RETR, setR);  //H�Զ��ط����ü��ʱ��250US-4000US��L�Զ��ط�10��											// �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
	SI24R1_Write_Reg(WRITE_REG + RF_CH, Channel);         //��Ƶ�ŵ� bit6-bit0											// ѡ����Ƶͨ��0x40
	SI24R1_Write_Reg(WRITE_REG + RF_SETUP, sl_gL);    //��Ƶ���ã�����bit ���书�ʣ�����bit5-3��1M=00 2M=01  250K=10											// ���ݴ�����2Mbps�����书��7dBm
	SI24R1_Write_Reg(WRITE_REG + CONFIG, 0x0e);      //���üĴ���	У��+����ģʽ										// CRCʹ�ܣ�16λCRCУ�飬�ϵ�
     CE_1;
}


/********************************************************
�������ܣ�SI24R1����ģʽ��ʼ��                      
��ڲ�������
����  ֵ����
*********************************************************/
void SI24R1_TX_Mode(void)
{
	//CE = 0;
    CE_0;
	SI24R1_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // д�뷢�͵�ַ
	SI24R1_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ

	SI24R1_Write_Reg(WRITE_REG + EN_AA, 0x01);       //ʹ������ͨ���Զ�ȷ��											// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	SI24R1_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);   //ʹ�ܽ������ݹܵ���ַ											// ʹ�ܽ���ͨ��0
	SI24R1_Write_Reg(WRITE_REG + SETUP_RETR, 0xf2);  //H�Զ��ط����ü��ʱ��250US-4000US��L�Զ��ط�10��											// �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
	SI24R1_Write_Reg(WRITE_REG + RF_CH, 20);         //��Ƶ�ŵ� bit6-bit0											// ѡ����Ƶͨ��0x40
	SI24R1_Write_Reg(WRITE_REG + RF_SETUP, sl_gL);    //��Ƶ���ã�����bit ���书�ʣ�����bit5-3��1M=00 2M=01  250K=10											// ���ݴ�����2Mbps�����书��7dBm
	SI24R1_Write_Reg(WRITE_REG + CONFIG, 0x0e);      //���üĴ���	У��+����ģʽ										// CRCʹ�ܣ�16λCRCУ�飬�ϵ�
     CE_1;
    //CE = 1;
}


/********************************************************
�������ܣ���ȡ��������                       
��ڲ�����rxbuf:�������ݴ���׵�ַ
����  ֵ��0:���յ�����
          1:û�н��յ�����
*********************************************************/

u8 SI24R1_RxPacket(u8 *rxbuf)
{
	u8 state=RX_DR;
	state = SI24R1_Read_Reg(STATUS);  			                 //��ȡ״̬�Ĵ�����ֵ    	  
	SI24R1_Write_Reg(WRITE_REG+STATUS,state);               //���RX_DS�жϱ�־
    
	if(state & RX_DR)								                           //���յ�����
	{
		SI24R1_Read_Buf(RD_RX_PLOAD,rxbuf,TX_PLOAD_WIDTH);     //��ȡ����
		SI24R1_Write_Reg(FLUSH_RX,0xff);					              //���RX FIFO�Ĵ���
		return 0; 
	}	   
	return 1;                                                   //û�յ��κ�����
}


/********************************************************
�������ܣ�����һ�����ݰ�                      
��ڲ�����txbuf:Ҫ���͵�����
����  ֵ��0x10:�ﵽ����ط�����������ʧ�� 
          0x20:���ͳɹ�            
          0xff:����ʧ��                  
*********************************************************/
u8 SI24R1_TxPacket(u8 *txbuf)
{
	u8 state;
    u8 pp[150];
    CE_0;
																											  //CE���ͣ�ʹ��SI24R1����
    SI24R1_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);	    //д���ݵ�TX FIFO,32���ֽ�
 	CE_1;																										  //CE�øߣ�ʹ�ܷ���	   
    led_shan_en(3,50,2);
    ///��ӡ����
    //Bufclr(pp);
    //StrToHexs(txbuf,pp, 32);
    //CH_H;
    //printf("SendChannel -> %d\r\n",SI24R1_Read_Reg(RF_CH));
    //printf("S24GSend -> %s\r\n",pp);
    //CH_L;
    //delay_ms(15);
	while(IRQ == 1);																				  //�ȴ��������
	state = SI24R1_Read_Reg(STATUS);  											  //��ȡ״̬�Ĵ�����ֵ	   
	SI24R1_Write_Reg(WRITE_REG+STATUS, state); 								//���TX_DS��MAX_RT�жϱ�־
	if(state&MAX_RT)																			    //�ﵽ����ط�����
	{
		SI24R1_Write_Reg(FLUSH_TX,0xff);										    //���TX FIFO�Ĵ��� 
		return MAX_RT; 
	}
	if(state&TX_DS)																			      //�������
	{
		return TX_DS;
	}
	return 0XFF;																						  //����ʧ��
}



u8 rev_v=0;
void SI24R1_Sendsub(void)
{
    volatile u8 state=0;
    u8 temp[35];
    rev_v++;
    if(rev_v>=100)
        rev_v=0;
    temp[0]=rev_v;
    SI24R1_TX_Mode();
	//state = SI24R1_Read_Reg(0x1d);  			                 //��ȡ״̬�Ĵ�����ֵ    	  
    state=SI24R1_TxPacket(temp);
    printf("Send-> %d\r\n",temp[0]);
    SI24R1_RX_Mode();
}

void SI24R1_revsub(void)
{
    //u8 state=0;
    u8 temp1[130];
    u8 temp[35];
    
    if(!SI24R1_RxPacket(temp))
    {
        Bufclr(temp1);
        StrToHexs(temp, temp1, 32);
        printf("Rev-> %s\r\n",temp1);
    }
    //Led7_ValueBCD(rev_v, send_v);
}


