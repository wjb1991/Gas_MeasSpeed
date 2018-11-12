#include "app_cfg.h"

void DoorCloseFnct (void *p_tmr,void *p_arg);
OS_TMR CloseDoorTmr;
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
    OS_ERR os_err;
  
    Mod_MeasSpeedInit(&st_MeasSpeed);
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

