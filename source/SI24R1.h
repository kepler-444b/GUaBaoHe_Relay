#ifndef __SI24R1__
#define __SI24R1__

#include "sys.h"

//IRQ=P25(12) CE=P03(7) CSN=P14 SCK=P15 MOSI=P24 MISO=P23
//SI24R1 PIN DEFINITION
// Master Out, Slave In pin (output)
// Master In, Slave Out pin (input)
// Serial Clock pin, (output)
// Slave Select pin, (output to CSN)
// Chip Enable pin signal (output)

#define CE_1        Gpio_SetIO(p0, pin3, TRUE)
#define CE_0        Gpio_SetIO(p0, pin3, FALSE)
#define CSN_1       Gpio_SetIO(p1, pin4, TRUE)
#define CSN_0       Gpio_SetIO(p1, pin4, FALSE)
#define SCK_1       Gpio_SetIO(p1, pin5, TRUE)
#define SCK_0       Gpio_SetIO(p1, pin5, FALSE)
#define MOSI_1      Gpio_SetIO(p2, pin4, TRUE)
#define MOSI_0      Gpio_SetIO(p2, pin4, FALSE)
#define	MISO        Gpio_GetIO(p2, pin3)  

//未启用IRQ
#define	IRQ       Gpio_GetIO(p2, pin5)             // Interrupt signal, from nRF24L01 (input)

#define TX_ADR_WIDTH   5  				// 5字节宽度的发送/接收地址
#define TX_PLOAD_WIDTH 32  				// 数据通道有效数据宽度

//********************************************************************************************************************//
// SPI(SI24R1) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

//********************************************************************************************************************//
// SPI(SI24R1) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define RSSI            0x09  // 'Received Signal Strength Indecator' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define FEATURE         0x1D    
#define DYNDP           0x1C

//********************************************************************************************************************//
// STATUS Register 
#define RX_DR						0x40  /**/
#define TX_DS						0x20
#define MAX_RT					0x10

//********************************************************************************************************************//
//                                        FUNCTION's PROTOTYPES                                                       //
//********************************************************************************************************************//
//SI24R1 API Functions
void SI24R1_Init(void); //SI24R1 Pin Init
u8 SI24R1_Write_Reg(u8 reg, u8 value); 
u8 SI24R1_Write_Buf(u8 reg,  u8 *pBuf, u8 bytes);
u8 SI24R1_Read_Reg(u8 reg);
u8 SI24R1_Read_Buf(u8 reg, u8 *pBuf, u8 bytes);

void SI24R1_RX_Mode(void);
void SI24R1_TX_Mode(void);
u8 SI24R1_RxPacket(u8 *rxbuf);
u8 SI24R1_TxPacket(u8 *txbuf);


void SI24R1_RX_ModeSet(u8 Channel);
void SI24R1_TX_ModeSet(u8 Channel,u8 setR);

//********************************************************************************************************************//
void SI24R1_Sendsub(void);
void SI24R1_revsub(void);


#endif

