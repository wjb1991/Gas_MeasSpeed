//==================================================================================================
//| 文件名称 | Bsp.c
//|----------|--------------------------------------------------------------------------------------
//| 文件描述 | Bsp.c 板级基本功能的实现
//|----------|--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|----------|-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | 初版
//==================================================================================================
#include  "bsp.h"

void Bsp_Init (void)
{
     
#ifdef  OS_SUPPORT 
    BSP_OSTickInit();                                           /* Initialize OS periodic time source                   */
#else
    //HAL_SetTickFreq(1000);                                    /* 1ms */
#endif    
    
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    
    Bsp_GpioInit();                                 //GPIO初始化
    
    //Bsp_SramInit();
    
    /* COM1->485串口 */
    COM1.ul_BaudRate   = 230400;
    COM1.ul_WordLength = UART_WORDLENGTH_8B;
    COM1.ul_StopBits   = UART_STOPBITS_1;
    COM1.ul_Parity     = UART_PARITY_NONE;
    COM1.ul_HwFlowCtl  = UART_HWCONTROL_NONE;
    COM1.ul_Mode       = UART_MODE_TX_RX;
    Bsp_UartOpen(&COM1); 
    
    Bsp_DacInit(&Bsp_Dac1);
    Bsp_DacSetVolt(&Bsp_Dac1,1.15f); 
    
    /* COM4->调试串口 */
    COM4.ul_BaudRate   = 230400;
    COM4.ul_WordLength = UART_WORDLENGTH_8B;
    COM4.ul_StopBits   = UART_STOPBITS_1;
    COM4.ul_Parity     = UART_PARITY_NONE;
    COM4.ul_HwFlowCtl  = UART_HWCONTROL_NONE;
    COM4.ul_Mode       = UART_MODE_TX_RX;
    Bsp_UartOpen(&COM4);
    
    Bsp_TimeSampleInit();                           //时间戳功能校准
    
    Bsp_ADC1Init();
    
    while(0)
    {
        Bsp_Sync1(TRUE);
        Bsp_Sync2(TRUE);  
        Bsp_DelayMs(1000);
        Bsp_Sync1(FALSE);
        Bsp_Sync2(FALSE); 
        Bsp_DelayMs(1000);
    }
}
