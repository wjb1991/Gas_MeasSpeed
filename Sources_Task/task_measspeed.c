#include "app_cfg.h"

void DoorCloseFnct (void *p_tmr,  void *p_arg);
OS_TMR CloseDoorTmr;

void Task_MeasEvent(void *pst_Mod);

//==================================================================================================
//| 函数名称 | Task_MeasSpeed
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 测速任务
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Task_MeasSpeed (void *p_arg)
{
    OS_ERR err;
    
    OSTmrCreate(&CloseDoorTmr,          /* p_tmr */
                "MeasSpeedTimeOut",     /* p_name */
                1000,                    /* dly 1000* 0.01s = 3s*/
                0,                      /* period */
                OS_OPT_TMR_ONE_SHOT,    /* opt */
                DoorCloseFnct,          /* p_callback */
                0,                      /* p_callback_arg */
                &err);                  /* p_err */
    
    
    Mod_MeasSpeedInit(&st_MeasSpeed);
    st_MeasSpeed.cb_MeasSpeedEvent = Task_MeasEvent;
    
    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        OS_ERR  os_err;
        INT16U  uin_MsgSize = 0;
        void * pv_Msg;
        pv_Msg = OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &uin_MsgSize, 0, &os_err);
        
        if(os_err != OS_ERR_NONE)
            continue;
        
        Mod_MeasSpeedPoll(&st_MeasSpeed,pv_Msg);
                        
        TRACE_DBG(">>DBG:       测速任务\r\n");        
    }
}


void Task_MeasEvent(void *pst_Mod)
{
    OS_ERR err;
    
    MOD_MEASSPEED_TYPE *p = (MOD_MEASSPEED_TYPE*)pst_Mod;
    
    switch(p->e_State)
    {
    case e_Idle:
        //转入空闲状态 关闭超时   两个传感器都未被遮挡  
        //车子完全离开
        //一次测速完成
        Bsp_Sync1(FALSE);
        Bsp_Sync2(FALSE);
        OSTmrStop(&CloseDoorTmr,OS_OPT_TMR_NONE,NULL,&err);
        if(err != OS_ERR_NONE)
        {
            
        }
        
        break;
    case e_MeasSpeed:
        //转入测速1个传感器进入 被遮挡
        //车子刚刚进入
        //开启软件定时器
        Bsp_Sync1(TRUE);
        Bsp_Sync2(FALSE);
        OSTmrStart(&CloseDoorTmr,&err);
        if(err != OS_ERR_NONE)
        {

        }
        break;
    case e_MeasLenth:
        //转入测速2个传感器进入 被遮挡
        //车子完全进入
        Bsp_Sync1(TRUE);
        Bsp_Sync2(TRUE);
        break;
    case e_Leave:
        //转入测速1个传感器离开 被遮挡
        //车子刚要离开
        Bsp_Sync1(FALSE);
        Bsp_Sync2(TRUE);
        break;
    default:
        break;
    }
}

void DoorCloseFnct (void *p_tmr,  void *p_arg)
{
    /* Close the door! */
    Mod_MeasSpeedTimeOut(&st_MeasSpeed);
    Bsp_Sync1(FALSE);
    Bsp_Sync2(FALSE);
}
