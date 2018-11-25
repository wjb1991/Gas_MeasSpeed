#include  "bsp.h"

#define  BSP_RS485DE_GPIO_PIN                   GPIO_PIN_8
#define  BSP_RS485DE_GPIO_PORT                  GPIOA
#define  BSP_RS485DE_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE() 

#define  BSP_SYNC1_GPIO_PIN                     GPIO_PIN_15
#define  BSP_SYNC1_GPIO_PORT                    GPIOA
#define  BSP_SYNC1_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE() 

#define  BSP_SYNC2_GPIO_PIN                     GPIO_PIN_6
#define  BSP_SYNC2_GPIO_PORT                    GPIOC
#define  BSP_SYNC2_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE() 

#define  BSP_LED1_GPIO_PIN                      GPIO_PIN_8
#define  BSP_LED1_GPIO_PORT                     GPIOC
#define  BSP_LED1_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE() 

#define  BSP_LED2_GPIO_PIN                      GPIO_PIN_11
#define  BSP_LED2_GPIO_PORT                     GPIOA
#define  BSP_LED2_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE() 

#define  BSP_LASER1_GPIO_PIN                    GPIO_PIN_7
#define  BSP_LASER1_GPIO_PORT                   GPIOC
#define  BSP_LASER1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE() 

#define  BSP_LASER2_GPIO_PIN                    GPIO_PIN_12
#define  BSP_LASER2_GPIO_PORT                   GPIOA
#define  BSP_LASER2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE() 


#define  BSP_RUNLED_GPIO_PIN                    GPIO_PIN_12
#define  BSP_RUNLED_GPIO_PORT                   GPIOB
#define  BSP_RUNLED_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE() 

void  (*Bsp_GpioEventHandle)(BSP_GPIOEVENT_TYPE* pst_Event);

void Bsp_GpioInit (void)
{
    GPIO_InitTypeDef  gpio_init;

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    gpio_init.Pin   = BSP_SYNC1_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BSP_SYNC1_GPIO_PORT, &gpio_init);
    
    gpio_init.Pin   = BSP_SYNC2_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BSP_SYNC2_GPIO_PORT, &gpio_init);
    
    gpio_init.Pin   = BSP_LED1_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BSP_LED1_GPIO_PORT, &gpio_init);

    gpio_init.Pin   = BSP_LED2_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BSP_LED2_GPIO_PORT, &gpio_init);
    
    gpio_init.Pin   = BSP_LASER1_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BSP_LASER1_GPIO_PORT, &gpio_init);
    
    gpio_init.Pin   = BSP_LASER2_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BSP_LASER2_GPIO_PORT, &gpio_init);
    
    gpio_init.Pin   = BSP_RS485DE_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BSP_RS485DE_GPIO_PORT, &gpio_init);
    
    gpio_init.Pin   = BSP_RUNLED_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BSP_RUNLED_GPIO_PORT, &gpio_init);
    
    /*Configure GPIO pins : PD8 PD9 PD10 PD11 
                           PD12 PD13 PD14 PD15 
                           PD0 PD1 PD2 PD3 
                           PD4 PD5 PD6 PD7 */
    gpio_init.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    gpio_init.Mode = GPIO_MODE_IT_RISING;//GPIO_MODE_IT_RISING_FALLING;
    gpio_init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOD, &gpio_init);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    
    Bsp_Rs485de(eRs485Recv);
    Bsp_Laser1(e_LaserOff);
    Bsp_Laser2(e_LaserOff);
    Bsp_Led1(e_LedOff);
    Bsp_Led2(e_LedOff);
}

void Bsp_RunLed(Led_t status)
{
    HAL_GPIO_WritePin(BSP_RUNLED_GPIO_PORT,BSP_RUNLED_GPIO_PIN,(GPIO_PinState)status);
}

void Bsp_Rs485de(Rs485de_t status)
{
    HAL_GPIO_WritePin(BSP_RS485DE_GPIO_PORT,BSP_RS485DE_GPIO_PIN,(GPIO_PinState)status);
}

static Laser_t b_Laser1Status = e_LaserOff;
static Laser_t b_Laser2Status = e_LaserOff;


void Bsp_Laser1(Laser_t status)
{
    b_Laser1Status = status;
    HAL_GPIO_WritePin(BSP_LASER1_GPIO_PORT,BSP_LASER1_GPIO_PIN,(GPIO_PinState)status);
}

void Bsp_Laser2(Laser_t status)
{
    b_Laser2Status = status;
    HAL_GPIO_WritePin(BSP_LASER2_GPIO_PORT,BSP_LASER2_GPIO_PIN,(GPIO_PinState)status);
}

Laser_t Bsp_GetLaser1(void)
{
    return b_Laser1Status;
}

Laser_t Bsp_GetLaser2(void)
{
    return b_Laser2Status;
}

void Bsp_Led1(Led_t status)
{
    HAL_GPIO_WritePin(BSP_LED1_GPIO_PORT,BSP_LED1_GPIO_PIN,(GPIO_PinState)status);
}

void Bsp_Led2(Led_t status)
{
    HAL_GPIO_WritePin(BSP_LED2_GPIO_PORT,BSP_LED2_GPIO_PIN,(GPIO_PinState)status);
}

void Bsp_Sync1(BOOL status)
{
    HAL_GPIO_WritePin(BSP_SYNC1_GPIO_PORT,BSP_SYNC1_GPIO_PIN,(GPIO_PinState)status);
}

void Bsp_Sync2(BOOL status)
{
    HAL_GPIO_WritePin(BSP_SYNC2_GPIO_PORT,BSP_SYNC2_GPIO_PIN,(GPIO_PinState)status);
}

void EXTI0_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}

void EXTI1_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}

void EXTI2_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}

void EXTI3_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}

void EXTI4_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}

void EXTI9_5_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif

}

void EXTI15_10_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    //100个时间缓冲区 双边沿触发1边16个事件 两边32个事件
    static BSP_GPIOEVENT_TYPE st_Event[100];
    static BSP_TIMESAMPLE_TYPE  st_Ts[100];
    static INT8U uch_Index = 0;
    BOOL b_IsRising = HAL_GPIO_ReadPin(GPIOD,GPIO_Pin);     //读取一次电平
    
    Bsp_GetTimeSample(&st_Ts[uch_Index]);  //获取时间戳
    st_Event[uch_Index].uin_GpioPin = GPIO_Pin;
    st_Event[uch_Index].vp_GpioPort = GPIOD;
    st_Event[uch_Index].b_IsRising = b_IsRising;
    st_Event[uch_Index].st_Ts = &st_Ts[uch_Index];
    
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) != b_IsRising)      //读取第二次电平作为消抖
        return;
    
    //if(b_IsRising != FALSE)
    //    return;
    
    /* 两次消抖都通过才发送消息 单边沿适用 双边沿不适用 */
#ifdef  OS_SUPPORT
    OS_ERR  os_err;
    OSTaskQPost(&TaskMeasSpeedTCB,&st_Event[uch_Index],sizeof(BSP_GPIOEVENT_TYPE),OS_OPT_POST_FIFO ,&os_err);
    if(os_err != OS_ERR_NONE)
    {
    
    }
#else
    if(Bsp_GpioEventHandle != NULL)
        Bsp_GpioEventHandle(&st_Event);
#endif 
    
    if(++uch_Index >= 100)
        uch_Index = 0;
}

