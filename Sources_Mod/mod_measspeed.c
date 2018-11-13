#include "mod_measspeed.h"

/*
    被遮挡->高电平->上升沿       不遮挡->低电平->下降沿
    
    s1 -> 传感器1被遮挡   传感器2未遮挡     ->检测到来车
    s2 -> 传感器1被遮挡   传感器2被遮挡     ->计算出车速     两传感器安装距离 / 传感器1与传感器2间隔时间
    s3 -> 传感器1未遮挡   传感器2被遮挡     ->计算出车长     传感器1与传感器1间隔时间 * 车速
    s4 -> 传感器1未遮挡   传感器2未遮挡     ->检测车离去
    
    240km/h = 4km/min = 66.66m/s = 0.066m/ms;

    两个传感器间距270*2 = 540mm?
    540mm = 0.540m
    0.540m/10ms = 54m/s = 194km/h
    0.540m/9ms  = 54m/s = 194km/h
*/

#if (MOD_MEASSPEED_DEBUG != FALSE)
    #define S_TRACE_DBG(...)      TRACE_DBG(__VA_ARGS__)
#else
    #define S_TRACE_DBG(...)      (void)(0)
#endif

void Mod_SaveTsAtoTsB(BSP_TIMESAMPLE_TYPE* pst_A,BSP_TIMESAMPLE_TYPE* pst_B)
{
    memcpy(pst_B,pst_A,sizeof(BSP_TIMESAMPLE_TYPE));
}

MOD_MEASSENSN_TYPE st_Sense1 = {
    .uin_Id = 1,
    .uin_UsePinMask = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
                      GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,   //使用的引脚
//    .uin_UsePinMask = GPIO_PIN_0,   //使用的引脚
    .uin_PinStatus = 0,//引脚的状态
    .ul_RisingThreshold = 1,    //切换到有车状态的阈值 检测到被遮挡的传感器个数
    .ul_FallThreshold =1,       //切换到没车状态的阈值 检测到未遮挡的传感器个数
    
    .b_Status = FALSE,           //FALSE:没车 TRUE:有车
    .ul_Count = 0,
    .st_RisingTs = {0,0},
    .st_FallTs = {0,0},
};

MOD_MEASSENSN_TYPE st_Sense2 = {
    .uin_Id = 2,
    .uin_UsePinMask = GPIO_PIN_8|GPIO_PIN_9 |GPIO_PIN_10|GPIO_PIN_11|
                      GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,   //使用的引脚
//    .uin_UsePinMask = GPIO_PIN_8,   //使用的引脚
    .uin_PinStatus = 0,         //引脚的状态
    .ul_RisingThreshold = 1,    //切换到有车状态的阈值 检测到被遮挡的传感器个数
    .ul_FallThreshold =1,       //切换到没车状态的阈值 检测到未遮挡的传感器个数
    
    .b_Status = FALSE,          //FALSE:无信号 TRUE:有信号
    .ul_Count = 0,
    .st_RisingTs = {0,0},
    .st_FallTs = {0,0},
};

MOD_MEASSPEED_TYPE st_MeasSpeed = {
    .pst_Sense1 = &st_Sense1,
    .pst_Sense2 = &st_Sense2,
    .e_State    = e_Idle,
    .e_Dirction = e_Null,
    .st_Ts1     = 0,
    .st_Ts2     = 0,
    .st_Ts3     = 0,
    .st_Ts4     = 0,
    
    .ul_Lenth   = 540,
    .ul_IntervalTime_us = 0,
    .ul_Speed1_mph  = 0,
    .ul_Speed2_mph  = 0,
    .ul_SpeedAvg_mph  = 0,
    .ul_Acc_mps2    = 0,
    .cb_MeasSpeedEvent = NULL,
};

void Mod_SenseEnableRising(MOD_MEASSENSN_TYPE* pst_Sense)
{
    GPIO_InitTypeDef  gpio_init;
    gpio_init.Pin = pst_Sense->uin_UsePinMask;
    gpio_init.Mode = GPIO_MODE_IT_RISING;//GPIO_MODE_IT_RISING_FALLING;
    gpio_init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOD, &gpio_init);
}

void Mod_SenseEnableFalling(MOD_MEASSENSN_TYPE* pst_Sense)
{
    GPIO_InitTypeDef  gpio_init;
    gpio_init.Pin = pst_Sense->uin_UsePinMask;
    gpio_init.Mode = GPIO_MODE_IT_FALLING;//GPIO_MODE_IT_RISING_FALLING;
    gpio_init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOD, &gpio_init);
}


void Mod_SenseEventHandle(MOD_MEASSENSN_TYPE* pst_Sense,BSP_GPIOEVENT_TYPE* pst_GpioEvent)
{   
#if 1
    /* 判断是否是使用的引脚 */
    if((pst_Sense->uin_UsePinMask & pst_GpioEvent->uin_GpioPin) != 0)
    {
        if(pst_Sense->b_Status == TRUE)
        {
            if(HAL_GPIO_ReadPin(pst_GpioEvent->vp_GpioPort,pst_GpioEvent->uin_GpioPin) == 0)    //读取电平消抖
            {
                if(pst_GpioEvent->b_IsRising == FALSE)      //检测下降沿
                {
                    if(++pst_Sense->ul_Count >= pst_Sense->ul_FallThreshold)
                    {
                        pst_Sense->b_Status = FALSE;
                        pst_Sense->ul_Count = 0;
                        Mod_SaveTsAtoTsB(pst_GpioEvent->st_Ts,
                                         &pst_Sense->st_FallTs);
                        Mod_SenseEnableRising(pst_Sense);  //切换到检测上升沿

                    }   
                }
            }
        }
        else
        {
            if(HAL_GPIO_ReadPin(pst_GpioEvent->vp_GpioPort,pst_GpioEvent->uin_GpioPin) == 1)    //读取电平消抖
            {
                if(pst_GpioEvent->b_IsRising == TRUE)       //检测上升沿
                {
                    if(++pst_Sense->ul_Count >= pst_Sense->ul_RisingThreshold)
                    {
                        pst_Sense->b_Status = TRUE;
                        pst_Sense->ul_Count = 0;
                        Mod_SaveTsAtoTsB(pst_GpioEvent->st_Ts,
                                         &pst_Sense->st_RisingTs);
                        Mod_SenseEnableFalling(pst_Sense);  //切换到检测下降沿
                    }
                }
            }
        }
    } 
#else
    /* 判断是否是使用的引脚 */
    if((pst_Sense->uin_UsePinMask & pst_GpioEvent->uin_GpioPin) != 0)
    {
        if(pst_GpioEvent->b_IsRising == TRUE)
        {
            /* 是上升沿被遮挡有车进入 */
            if(HAL_GPIO_ReadPin(pst_GpioEvent->vp_GpioPort,pst_GpioEvent->uin_GpioPin) == 1)    //读取电平消抖
            {
                if((pst_Sense->uin_PinStatus & pst_GpioEvent->uin_GpioPin) == 0)                //判断当前脚是否已经被置1
                {
                    //未被置1
                    pst_Sense->uin_PinStatus |= pst_GpioEvent->uin_GpioPin;
                    
                    if(++pst_Sense->ul_Count > 8)
                        pst_Sense->ul_Count = 8;
                    
                    if(pst_Sense->ul_Count >= pst_Sense->ul_RisingThreshold)       //大于设定个数
                    {
                        pst_Sense->b_Status = TRUE;
                        pst_Sense->st_RisingTs.ul_Cnt = pst_GpioEvent->st_Ts->ul_Cnt;
                        pst_Sense->st_RisingTs.ul_Tick = pst_GpioEvent->st_Ts->ul_Tick;
                    }
                }
            }
        }
        else
        {
            /* 是下降沿被遮挡有车 */
            if(HAL_GPIO_ReadPin(pst_GpioEvent->vp_GpioPort,pst_GpioEvent->uin_GpioPin) == 0)    //读取电平消抖
            {

                if((pst_Sense->uin_PinStatus & pst_GpioEvent->uin_GpioPin) != 0)//判断当前脚是否已经清零
                {
                    //未清零
                    pst_Sense->uin_PinStatus &= (~pst_GpioEvent->uin_GpioPin);
                    
                    if(++pst_Sense->ul_Count > 8)
                        pst_Sense->ul_Count = 8;
                    
                    if(pst_Sense->ul_Count >= pst_Sense->ul_RisingThreshold)       //大于设定个数
                    {
                        pst_Sense->b_Status = TRUE;
                        pst_Sense->st_FallTs.ul_Cnt = pst_GpioEvent->st_Ts->ul_Cnt;
                        pst_Sense->st_FallTs.ul_Tick = pst_GpioEvent->st_Ts->ul_Tick;
                    }
                }
            }
        }
    }
#endif
}

void Mod_SenseSetLow(MOD_MEASSENSN_TYPE* pst_Sense)
{
    pst_Sense->b_Status = FALSE;
    pst_Sense->ul_Count = 0;
    Mod_SenseEnableRising(pst_Sense);  //切换到检测上升沿
}

void Mod_SenseSetHigh(MOD_MEASSENSN_TYPE* pst_Sense)
{
    pst_Sense->b_Status = TRUE;
    pst_Sense->ul_Count = 0;
    Mod_SenseEnableFalling(pst_Sense);  //切换到检测下降沿
}

void Mod_MeasSpeedInit(MOD_MEASSPEED_TYPE *pst_Mod)
{
    Mod_SenseSetLow(pst_Mod->pst_Sense1);       //重置为低电平状态等待上升沿
    Mod_SenseSetLow(pst_Mod->pst_Sense2);       //重置为低电平状态等待上升沿
}

void Mod_MeasSpeedCal(MOD_MEASSPEED_TYPE *pst_Mod)
{
    /* lenth/ts2-ts1 = speed1 lenth/ts4-ts3 = speed2 */
    /* 1. um/us = m/s  2.m/s * 3600 = m/h */
  
    INT32U ul_IntervalUs = 0;
    FP64 f_mps1,f_mps2;
    ul_IntervalUs = Bsp_GetInterval(&pst_Mod->st_Ts1,&pst_Mod->st_Ts2);
    f_mps1 = pst_Mod->ul_Lenth * 1000.0 / ul_IntervalUs; 
  
    ul_IntervalUs = Bsp_GetInterval(&pst_Mod->st_Ts3,&pst_Mod->st_Ts4);
    f_mps2 = pst_Mod->ul_Lenth * 1000.0 / ul_IntervalUs;
    
    pst_Mod->ul_Acc_mps2 = (f_mps2 - f_mps1) * 1000000 / ul_IntervalUs;
    
    
    pst_Mod->ul_SpeedAvg_mph = (f_mps2 + f_mps1) * 3600/2;
    

    
    if(pst_Mod->pst_Sense1->uin_Id == 1)
    {
        pst_Mod->e_Dirction = e_Sense1ToSense2;
    }
    else if(pst_Mod->pst_Sense1->uin_Id == 2)
    {
        pst_Mod->e_Dirction = e_Sense2ToSense1;
    }
    
    if(pst_Mod->cb_MeasSpeedEvent != NULL)
    {
        pst_Mod->cb_MeasSpeedEvent(pst_Mod);
    } 
    
    S_TRACE_DBG(">>MeasSpeed:    测速完成\r\n");
    S_TRACE_DBG(">>MeasSpeed:    车辆方向%u\r\n",pst_Mod->e_Dirction);
    S_TRACE_DBG(">>MeasSpeed:    车辆速度%u(m/h)\r\n",pst_Mod->ul_SpeedAvg_mph);
    S_TRACE_DBG(">>MeasSpeed:    车辆加速度%f(m/s^2)\r\n",pst_Mod->ul_Acc_mps2);
}

void Mod_MeasSpeedPoll(MOD_MEASSPEED_TYPE *pst_Mod,BSP_GPIOEVENT_TYPE* pst_GpioEvent)
{        
    Mod_SenseEventHandle(pst_Mod->pst_Sense1,pst_GpioEvent);
    Mod_SenseEventHandle(pst_Mod->pst_Sense2,pst_GpioEvent);
    switch (pst_Mod->e_State)
    {
    case e_Idle:        //等待第一个上升沿  
        if(pst_Mod->pst_Sense1->b_Status == TRUE && pst_Mod->pst_Sense2->b_Status == TRUE)
        {
            //两边都有信号
            Mod_SenseSetLow(pst_Mod->pst_Sense1);       //重置为低电平状态等待上升沿
            Mod_SenseSetLow(pst_Mod->pst_Sense2);       //重置为低电平状态等待上升沿
            S_TRACE_DBG(">>MeasSpeed:   两边都有信号\r\n");
        }
        else if(pst_Mod->pst_Sense1->b_Status == TRUE && pst_Mod->pst_Sense2->b_Status == FALSE)
        {
            Mod_SaveTsAtoTsB(&pst_Mod->pst_Sense1->st_RisingTs,
                             &pst_Mod->st_Ts1);
            pst_Mod->e_State = e_MeasSpeed;
            S_TRACE_DBG(">>MeasSpeed:   第一次上升沿 传感器%u触发\r\n",pst_Mod->pst_Sense1->uin_Id);
        }
        else if(pst_Mod->pst_Sense1->b_Status == FALSE && pst_Mod->pst_Sense2->b_Status == TRUE)
        {
            /* 交换指针 Sense1总是是先触发的那个 */
            void* p = pst_Mod->pst_Sense2;
            pst_Mod->pst_Sense2 = pst_Mod->pst_Sense1;
            pst_Mod->pst_Sense1 = p;
          
            Mod_SaveTsAtoTsB(&pst_Mod->pst_Sense1->st_RisingTs,
                             &pst_Mod->st_Ts1);
            pst_Mod->e_State = e_MeasSpeed;
            
            S_TRACE_DBG(">>MeasSpeed:    第一次上升沿 传感器%u触发\r\n",pst_Mod->pst_Sense1->uin_Id);
        }
        break;
    case e_MeasSpeed:       //等待第二个上升沿
        if(pst_Mod->pst_Sense2->b_Status == TRUE)
        {
            Mod_SaveTsAtoTsB(&pst_Mod->pst_Sense2->st_RisingTs,
                             &pst_Mod->st_Ts2); 
            pst_Mod->e_State = e_MeasLenth;
            S_TRACE_DBG(">>MeasSpeed:   第二次上升沿 传感器%u触发\r\n",pst_Mod->pst_Sense2->uin_Id);
        }
        break;
    case e_MeasLenth:       //等待第一个下升沿
        if(pst_Mod->pst_Sense1->b_Status == FALSE)
        {
            Mod_SaveTsAtoTsB(&pst_Mod->pst_Sense1->st_FallTs,
                             &pst_Mod->st_Ts3);
            pst_Mod->e_State = e_Leave;
            S_TRACE_DBG(">>MeasSpeed:   第一次下升沿 传感器%u触发\r\n",pst_Mod->pst_Sense1->uin_Id);
        }
        break;
    case e_Leave:           //
        if(pst_Mod->pst_Sense2->b_Status == FALSE)
        {
            Mod_SaveTsAtoTsB(&pst_Mod->pst_Sense2->st_FallTs,
                             &pst_Mod->st_Ts4);

            pst_Mod->e_State = e_Idle;
            S_TRACE_DBG(">>MeasSpeed:   第二次下升沿 传感器%u触发\r\n",pst_Mod->pst_Sense2->uin_Id);

            Mod_MeasSpeedCal(pst_Mod);

        }
        break;
    default:
        break;
    }
}

void Mod_MeasSpeedTimeOut(MOD_MEASSPEED_TYPE *pst_Mod)
{
    
}
