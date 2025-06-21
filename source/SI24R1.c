#include "SI24R1.h"
#include "usart.h"
#include "strs.h"


extern Pro_flag_s MCU_flag;


u8 SI24R1_en=0;
u8  TX_ADDRESS[TX_ADR_WIDTH] = {0x5A,0x4B,0x3C,0x2D,0x0F};  // 定义一个静态发送地址

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
函数功能：SI24R1引脚初始化                
入口参数：无
返回  值：无
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
函数功能：写寄存器的值（单字节）                
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
					value:寄存器的值
返回  值：状态寄存器的值
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
函数功能：写寄存器的值（多字节）                  
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
					pBuf:写数据首地址
					bytes:写数据字节数
返回  值：状态寄存器的值
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
函数功能：读取寄存器的值（单字节）                  
入口参数：reg:寄存器映射地址（格式：READ_REG｜reg）
返回  值：寄存器值
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
函数功能：读取寄存器的值（多字节）                  
入口参数：reg:寄存器映射地址（READ_REG｜reg）
					pBuf:接收缓冲区的首地址
					bytes:读取字节数
返回  值：状态寄存器的值
*********************************************************/
u8 SI24R1_Read_Buf(u8 reg, u8 *pBuf, u8 bytes)
{
	u8 status,byte_ctr;


  CSN_0;
  status = SPI_RW(reg);                           
  for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    pBuf[byte_ctr] = SPI_RW(0);                   //读取数据，低字节在前
                                      
    CSN_1;
  return(status);    
}
/********************************************************
函数功能：SI24R1设置寄存器                      
入口参数：无
返回  值：无
*********************************************************/
void SI24R1_Set(void)
{
    
}
#define sl_gL   0x0f

/********************************************************
函数功能：SI24R1接收模式初始化                      
入口参数：无
返回  值：无
*********************************************************/
void SI24R1_RX_Mode(void)
{

    CE_0;
	SI24R1_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// 接收设备接收通道0使用和发送设备相同的发送地址
	SI24R1_Write_Reg(WRITE_REG + EN_AA, 0x01);               						// 使能接收通道0自动应答
	SI24R1_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);           						// 使能接收通道0
	SI24R1_Write_Reg(WRITE_REG + RF_CH, 20);                 						// 选择射频通道0x40
	SI24R1_Write_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  						// 接收通道0选择和发送通道相同有效数据宽度
	SI24R1_Write_Reg(WRITE_REG + RF_SETUP, sl_gL);            						// 数据传输率2Mbps，发射功率7dBm
	SI24R1_Write_Reg(WRITE_REG + CONFIG, 0x0f);              						// CRC使能，16位CRC校验，上电，接收模式
	SI24R1_Write_Reg(WRITE_REG + STATUS, 0xff);  												//清除所有的中断标志位
	CE_1;                                      									// 拉高CE启动接收设备
}						
/********************************************************
函数功能：SI24R1接收模式初始化                      
入口参数：Channel 信道1-125  每个1M     默认20
        rx_set：低三位功率+bit3传送速率    默认0x0f
返回  值：无
*********************************************************/

void SI24R1_RX_ModeSet(u8 Channel)
{

    CE_0;
	SI24R1_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// 接收设备接收通道0使用和发送设备相同的发送地址
	SI24R1_Write_Reg(WRITE_REG + EN_AA, 0x01);               						// 使能接收通道0自动应答
	SI24R1_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);           						// 使能接收通道0
	SI24R1_Write_Reg(WRITE_REG + RF_CH, Channel);                 						// 选择射频通道0x40
	SI24R1_Write_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  						// 接收通道0选择和发送通道相同有效数据宽度
	SI24R1_Write_Reg(WRITE_REG + RF_SETUP, sl_gL);            						// 数据传输率2Mbps，发射功率7dBm
	SI24R1_Write_Reg(WRITE_REG + CONFIG, 0x0f);              						// CRC使能，16位CRC校验，上电，接收模式
	SI24R1_Write_Reg(WRITE_REG + STATUS, 0xff);   												//清除所有的中断标志位
	CE_1;                                      									// 拉高CE启动接收设备
}						
/********************************************************
函数功能：SI24R1发送模式初始化                      
入口参数：Channel 信道1-125  每个1M     默认20
        rx_set：低三位功率+bit3传送速率    默认0x0f
        Cfa:H自动重发配置间隔时间250US-4000US，L自动重发次数
返回  值：无
*********************************************************/
void SI24R1_TX_ModeSet(u8 Channel,u8 setR)
{
	//CE = 0;
    CE_0;
	SI24R1_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
	SI24R1_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同

	SI24R1_Write_Reg(WRITE_REG + EN_AA, 0x01);       //使能数据通道自动确认											// 使能接收通道0自动应答
	SI24R1_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);   //使能接收数据管道地址											// 使能接收通道0
	SI24R1_Write_Reg(WRITE_REG + SETUP_RETR, setR);  //H自动重发配置间隔时间250US-4000US，L自动重发10次											// 自动重发延时等待250us+86us，自动重发10次
	SI24R1_Write_Reg(WRITE_REG + RF_CH, Channel);         //射频信道 bit6-bit0											// 选择射频通道0x40
	SI24R1_Write_Reg(WRITE_REG + RF_SETUP, sl_gL);    //射频配置，后三bit 发射功率，速率bit5-3：1M=00 2M=01  250K=10											// 数据传输率2Mbps，发射功率7dBm
	SI24R1_Write_Reg(WRITE_REG + CONFIG, 0x0e);      //配置寄存器	校验+开机模式										// CRC使能，16位CRC校验，上电
     CE_1;
}


/********************************************************
函数功能：SI24R1发送模式初始化                      
入口参数：无
返回  值：无
*********************************************************/
void SI24R1_TX_Mode(void)
{
	//CE = 0;
    CE_0;
	SI24R1_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
	SI24R1_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同

	SI24R1_Write_Reg(WRITE_REG + EN_AA, 0x01);       //使能数据通道自动确认											// 使能接收通道0自动应答
	SI24R1_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);   //使能接收数据管道地址											// 使能接收通道0
	SI24R1_Write_Reg(WRITE_REG + SETUP_RETR, 0xf2);  //H自动重发配置间隔时间250US-4000US，L自动重发10次											// 自动重发延时等待250us+86us，自动重发10次
	SI24R1_Write_Reg(WRITE_REG + RF_CH, 20);         //射频信道 bit6-bit0											// 选择射频通道0x40
	SI24R1_Write_Reg(WRITE_REG + RF_SETUP, sl_gL);    //射频配置，后三bit 发射功率，速率bit5-3：1M=00 2M=01  250K=10											// 数据传输率2Mbps，发射功率7dBm
	SI24R1_Write_Reg(WRITE_REG + CONFIG, 0x0e);      //配置寄存器	校验+开机模式										// CRC使能，16位CRC校验，上电
     CE_1;
    //CE = 1;
}


/********************************************************
函数功能：读取接收数据                       
入口参数：rxbuf:接收数据存放首地址
返回  值：0:接收到数据
          1:没有接收到数据
*********************************************************/

u8 SI24R1_RxPacket(u8 *rxbuf)
{
	u8 state=RX_DR;
	state = SI24R1_Read_Reg(STATUS);  			                 //读取状态寄存器的值    	  
	SI24R1_Write_Reg(WRITE_REG+STATUS,state);               //清除RX_DS中断标志
    
	if(state & RX_DR)								                           //接收到数据
	{
		SI24R1_Read_Buf(RD_RX_PLOAD,rxbuf,TX_PLOAD_WIDTH);     //读取数据
		SI24R1_Write_Reg(FLUSH_RX,0xff);					              //清除RX FIFO寄存器
		return 0; 
	}	   
	return 1;                                                   //没收到任何数据
}


/********************************************************
函数功能：发送一个数据包                      
入口参数：txbuf:要发送的数据
返回  值：0x10:达到最大重发次数，发送失败 
          0x20:发送成功            
          0xff:发送失败                  
*********************************************************/
u8 SI24R1_TxPacket(u8 *txbuf)
{
	u8 state;
    u8 pp[150];
    CE_0;
																											  //CE拉低，使能SI24R1配置
    SI24R1_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);	    //写数据到TX FIFO,32个字节
 	CE_1;																										  //CE置高，使能发送	   
    led_shan_en(3,50,2);
    ///打印发送
    //Bufclr(pp);
    //StrToHexs(txbuf,pp, 32);
    //CH_H;
    //printf("SendChannel -> %d\r\n",SI24R1_Read_Reg(RF_CH));
    //printf("S24GSend -> %s\r\n",pp);
    //CH_L;
    //delay_ms(15);
	while(IRQ == 1);																				  //等待发送完成
	state = SI24R1_Read_Reg(STATUS);  											  //读取状态寄存器的值	   
	SI24R1_Write_Reg(WRITE_REG+STATUS, state); 								//清除TX_DS或MAX_RT中断标志
	if(state&MAX_RT)																			    //达到最大重发次数
	{
		SI24R1_Write_Reg(FLUSH_TX,0xff);										    //清除TX FIFO寄存器 
		return MAX_RT; 
	}
	if(state&TX_DS)																			      //发送完成
	{
		return TX_DS;
	}
	return 0XFF;																						  //发送失败
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
	//state = SI24R1_Read_Reg(0x1d);  			                 //读取状态寄存器的值    	  
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


