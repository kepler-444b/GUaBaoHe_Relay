#include "sys.h"

Pro_flag_s MCU_flag;

// 获取硬件的UID，共80位占用12个字节
u16 Get_UID(u8 *buf)
{
    uint32_t temp;
    uint32_t addr = 0x00100E74;
    u16 i;
    for (i = 0; i < 3; i++) {
        temp           = *((volatile uint32_t *)(addr + i * 4));
        buf[i * 4]     = (u8)temp;
        buf[i * 4 + 1] = (u8)(temp >> 8);
        buf[i * 4 + 2] = (u8)(temp >> 16);
        buf[i * 4 + 3] = (u8)(temp >> 24);
    }
    return crc_chk_value(buf, 12);
}
/*******************************************************************************
 * FLASH 中断服务函数
 ******************************************************************************/
void FlashInt(void)
{
    if (TRUE == Flash_GetIntFlag(flash_int0)) {
        Flash_ClearIntFlag(flash_int0);
        Flash_DisableIrq(flash_int0);
    }
    if (TRUE == Flash_GetIntFlag(flash_int1)) {
        Flash_ClearIntFlag(flash_int1);

        Flash_DisableIrq(flash_int1);
    }
}
void flash_init(void)
{
    Flash_Init(FlashInt, 2);
}

/// 存储数据 占用16个字节

void Data_save(u8 *data, u8 len)
{

    u16 i;
    Flash_SectorErase(flash_addrS);
    for (i = 0; i < len; i++) {
        Flash_WriteByte(flash_addrS + i, data[i]);
    }
}
void Data_load(u8 *data, u8 len)
{
    u16 i;
    for (i = 0; i < len; i++) {
        data[i] = *((volatile uint8_t *)(flash_addrS + i));
    }
}

/****************************************
校验和为0x64
*****************************************/
u8 GetCheckCodeX(u8 *cmd, u8 BoxLen)
{
    u8 i, res;
    res = 0x64;
    for (i = 0; i < BoxLen; i++)
        res -= cmd[i];
    return res;
}
/****************************************
校验和
*****************************************/
u8 GetCheckHe(u8 *cmd, u8 BoxLen)
{
    u8 i, res = 0;

    for (i = 0; i < BoxLen; i++)
        res += cmd[i];
    return res;
}

/****************************************
modbus 校验双字节
*****************************************/
unsigned int crc_chk_value(unsigned char *data_value, unsigned char length)
{
    unsigned int crc_value = 0xFFFF;
    int i;
    while (length--) {
        crc_value ^= *data_value++;
        for (i = 0; i < 8; i++) {
            if (crc_value & 0x0001) {
                crc_value = (crc_value >> 1) ^ 0xa001;
            } else {
                crc_value = crc_value >> 1;
            }
        }
    }
    return (crc_value);
}

// 时钟初始化配置
void App_ClkInit(u8 pinlv)
{
    volatile uint32_t u32Val = 0;
    // Clk_SwitchTo(ClkRCL);
    // Clk_SetRCHFreq(ClkFreq4Mhz);
    // Clk_SwitchTo(ClkRCH);
    // 系统时钟频率设置与切换
    if (pinlv == RCH4M) {
        // RCH 4MHz
        Clk_SwitchTo(ClkRCL);
        Clk_SetRCHFreq(ClkFreq4Mhz);
        Clk_SwitchTo(ClkRCH);
        u32Val = Clk_GetHClkFreq();
    } else if (pinlv == RCH8M) {
        // RCH 8MHz
        Clk_SwitchTo(ClkRCL);
        Clk_SetRCHFreq(ClkFreq8Mhz);
        Clk_SwitchTo(ClkRCH);
        u32Val = Clk_GetHClkFreq();
    } else if (pinlv == RCH16M) {
        // RCH 16MHz
        Clk_SwitchTo(ClkRCL);
        Clk_SetRCHFreq(ClkFreq16Mhz);
        Clk_SwitchTo(ClkRCH);
        u32Val = Clk_GetHClkFreq();
    } else if (pinlv == RCH22M) {
        // RCH 22.12MHz
        Clk_SwitchTo(ClkRCL);
        Clk_SetRCHFreq(ClkFreq22_12Mhz);
        Clk_SwitchTo(ClkRCH);
        u32Val = Clk_GetHClkFreq();
    } else if (pinlv == RCH24M) {
        // RCH 24MHz
        Clk_SwitchTo(ClkRCL);
        Clk_SetRCHFreq(ClkFreq24Mhz);
        Clk_SwitchTo(ClkRCH);
        u32Val = Clk_GetHClkFreq();
    } else if (pinlv == XTH) {
        // XTH
        // Clk_SwitchTo(ClkRCL);
        Clk_SwitchTo(ClkXTH);
        u32Val = Clk_GetHClkFreq();
    } else if (pinlv == XTL) {
        // XTL
        Clk_SwitchTo(ClkXTL);
        u32Val = Clk_GetHClkFreq();
    } else if (pinlv == RCL38) {
        // RCL   38.4K
        Clk_SetRCLFreq(ClkFreq38_4K);
        Clk_SwitchTo(ClkRCL);
        u32Val = Clk_GetHClkFreq();
    } else if (pinlv == RCL32) {
        // RCL 32768
        Clk_SetRCLFreq(ClkFreq32768);
        u32Val = Clk_GetHClkFreq();
    }

    u32Val = Clk_GetHClkFreq();
    u32Val = 0;
}

void io_init(u8 enPort, u8 enPin, u8 fangxiang)
{

    if (fangxiang == io_in) {

        // Gpio_InitIO(enPort, enPin, GpioDirIn);
        // 初始化外部GPIO P0-pin-输入-上拉-下拉-开漏-驱动能力
        Gpio_InitIOExt(enPort, enPin, GpioDirIn, FALSE, FALSE, FALSE, FALSE);
    } else if (fangxiang == io_inlou) {
        // Gpio_InitIO(enPort, enPin, GpioDirIn);
        // 初始化外部GPIO P0-pin-输入-上拉-下拉-开漏-驱动能力
        Gpio_InitIOExt(enPort, enPin, GpioDirIn, FALSE, FALSE, TRUE, FALSE);
    } else if (fangxiang == io_inout) {
        // Gpio_InitIO(enPort, enPin, GpioDirIn);
        // 初始化外部GPIO P0-pin-输入-上拉-下拉-开漏-驱动能力
        Gpio_InitIOExt(enPort, enPin, GpioDirIn, TRUE, FALSE, TRUE, FALSE);
    } else if (fangxiang == io_tuiwan) {
        // Gpio_InitIO(enPort, enPin, GpioDirOut);
        // 初始化外部GPIO P0-pin-输chu-上拉-下拉-开漏-驱动能力
        Gpio_InitIOExt(enPort, enPin, GpioDirOut, FALSE, FALSE, FALSE, FALSE);
    }
}

void delay_us(u16 nus)
{
    volatile u8 mm;
    while (nus--) {
        mm = 0;
        mm = 1;
        mm = 2;
        mm = 3;
        mm = 4;
        mm = 5;
        mm = 6;
        mm = 7;
        mm = 0;
        mm = 1;
        mm = 2;
        mm = 3;
        mm = 4;
        mm = 5;
        mm = 6;
        mm = 7;
    }
}

void delay_ms(u16 ms)
{
    u16 i, j;
    while (ms--) {
        for (i = 40; i != 0; i--)
            for (j = 30; j != 0; j--);
    }
}

/***************************************************************
看门狗初始化程序
0x00=1.6ms
0x01=3.2ms
0x0a=1.64s
0x0b=3.28s
****************************************************************/
void wdog_init(void)
{
    stc_wdt_config_t stcWdt_Config;
    DDL_ZERO_STRUCT(stcWdt_Config);
    stcWdt_Config.u8LoadValue   = 0x0b;      // 3.2s
    stcWdt_Config.enResetEnable = WRESET_EN; // WINT_EN;//WRESET_EN;////
    // stcWdt_Config.pfnWdtIrqCb = WdtCallback;   //中断回调函数
    Clk_SetPeripheralGate(ClkPeripheralWdt, TRUE); //
    Wdt_Init(&stcWdt_Config);

    Wdt_Start();
    // Wdt_Feed();//测试喂狗功能时，放开屏蔽
}
