#include "timers.h"
#include "xunSub.h"
#include "led.h"
#include "TG.h"

extern Pro_flag_s MCU_flag;
/*******************************************************************************
 * BT0中断服务函数
 ******************************************************************************/
void Bt0Int(void)
{
    if (TRUE == Bt_GetIntFlag(TIM0)) {
        Bt_ClearIntFlag(TIM0);
        TG_TimOVR();
        // TG_TimOVRKKG();
        xunSub_timer(33);
        MCU_flag.ms_flag++;
        MCU_flag.ms100_flag++;
    }
}

void App_Timer0Cfg(uint16_t u16Period)
{
    stc_bt_config_t stcConfig;
    volatile en_result_t enResult = Error;
    uint16_t u16ArrData           = 0xffff - u16Period;
    uint16_t u16InitCntData       = 0xffff - u16Period;
    // CLK INIT
    // stc_clk_config_t stcClkCfg;
    // stcClkCfg.enClkSrc  = ClkRCH;
    // stcClkCfg.enHClkDiv = ClkDiv1;
    // stcClkCfg.enPClkDiv = ClkDiv1;

    // Clk_Init(&stcClkCfg);

    // 打开BT外设时钟
    // Clk_SetPeripheralGate(ClkPeripheralBt, TRUE);

    stcConfig.pfnTim0Cb = Bt0Int;
    // P25设置为门控使能IO
    // Gpio_SetFunc_TIM0_EXT_P34();
    stcConfig.enGateP = BtPositive;
    stcConfig.enGate  = BtGateDisable;
    stcConfig.enPRS   = BtPCLKDiv16;
    stcConfig.enTog   = BtTogDisable;
    stcConfig.enCT    = BtTimer;
    stcConfig.enMD    = BtMode2;
    // Bt初始化
    Bt_Init(TIM0, &stcConfig);

    // TIM1中断使能
    Bt_ClearIntFlag(TIM0);
    Bt_EnableIrq(TIM0);
    EnableNvic(TIM0_IRQn, 3, TRUE);

    // 设置重载值和计数值，启动计数
    Bt_ARRSet(TIM0, u16ArrData);
    Bt_Cnt16Set(TIM0, u16InitCntData);
    Bt_Run(TIM0);
}

void timer0_Init(void)
{
    // CLK INIT
    stc_clk_config_t stcClkCfg;
    stcClkCfg.enClkSrc  = ClkRCH;
    stcClkCfg.enHClkDiv = ClkDiv1;
    stcClkCfg.enPClkDiv = ClkDiv1;

    Clk_Init(&stcClkCfg);

    // 打开GPIO、BT外设时钟
    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);
    Clk_SetPeripheralGate(ClkPeripheralBt, TRUE);
    App_Timer0Cfg(30); // 100us
}

/*******************************************************************************
 * BT2中断服务函数
 ******************************************************************************/
void Bt2Int(void)
{
    if (TRUE == Bt_GetIntFlag(TIM2)) {
        Bt_ClearIntFlag(TIM2);
        TG_TimOVR();
        // TG_TimOVRKKG();
        xunSub_timer(33);
        MCU_flag.ms_flag++;
        MCU_flag.ms100_flag++;
    }
}

void App_Timer2Cfg(uint16_t u16Period)
{
    stc_bt_config_t stcConfig;
    volatile en_result_t enResult = Error;
    uint16_t u16ArrData           = 0xffff - u16Period;
    uint16_t u16InitCntData       = 0xffff - u16Period;

    stcConfig.pfnTim2Cb = Bt2Int;
    stcConfig.enGateP   = BtPositive;
    stcConfig.enGate    = BtGateDisable;
    stcConfig.enPRS     = BtPCLKDiv16;
    stcConfig.enTog     = BtTogDisable;
    stcConfig.enCT      = BtTimer;
    stcConfig.enMD      = BtMode2;
    // Bt初始化
    Bt_Init(TIM2, &stcConfig);

    // TIM1中断使能
    Bt_ClearIntFlag(TIM2);
    Bt_EnableIrq(TIM2);
    EnableNvic(TIM2_IRQn, 0, TRUE);

    // 设置重载值和计数值，启动计数
    Bt_ARRSet(TIM2, u16ArrData);
    Bt_Cnt16Set(TIM2, u16InitCntData);
    Bt_Run(TIM2);
}

void timer2_Init(void)
{
    // CLK INIT
    stc_clk_config_t stcClkCfg;
    stcClkCfg.enClkSrc  = ClkRCH;
    stcClkCfg.enHClkDiv = ClkDiv1;
    stcClkCfg.enPClkDiv = ClkDiv1;

    Clk_Init(&stcClkCfg);

    // 打开GPIO、BT外设时钟
    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);
    Clk_SetPeripheralGate(ClkPeripheralBt, TRUE);
    App_Timer2Cfg(30); // 30us
}
