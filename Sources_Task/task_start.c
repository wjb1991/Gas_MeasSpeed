#include "app_cfg.h"

//==================================================================================
//                                   任务控制块定义
//==================================================================================
OS_TCB       TaskStartTCB;                                      /*  开始任务            */
OS_TCB       TaskMeasSpeedTCB;                                  /*  测速任务            */
OS_TCB       TaskCmlSendTCB;                                    /*  命令行调试任务      */
OS_TCB       TaskCmlRecvTCB;                                    /*  命令行调试任务      */
OS_TCB       TaskStdBusTCB;                                     /*  STDBUS任务          */

//==================================================================================
//                                   任务堆栈定义
//==================================================================================
CPU_STK      TaskStartStk       [TASK_START_STK_SIZE];          /*  开始任务            */
CPU_STK      TaskMeasSpeedStk   [TASK_MEASSPEED_STK_SIZE];      /*  测速任务            */
CPU_STK      TaskCmlSendStk     [TASK_CML_SEND_STK_SIZE];       /*  命令行调试任务    */
CPU_STK      TaskCmlRecvStk     [TASK_CML_RECV_STK_SIZE];       /*  命令行调试任务    */
CPU_STK      TaskStdBusStk      [TASK_STDBUS_STK_SIZE];         /*  STDBUS任务        */
//==================================================================================
//                                   任务函数定义
//==================================================================================
void Task_Start (void *p_arg);                                  /*  开始任务    */
void Task_MeasSpeed (void  *p_arg);                             /*  测速任务            */
void Task_CmlSend (void  *p_arg);                               /*  命令行调试任务    */
void Task_CmlRecv (void  *p_arg);                               /*  命令行调试任务    */
void Task_StdBus (void *p_arg);                                 /*  STDBUS任务    */

/*  信号量     */
#if (OS_CFG_SEM_EN > 0u)
OS_SEM       Sem_CmlSend;                                       /*  命令行发送计数信号量    */
#endif


/*  队列声明  */
#if (OS_CFG_Q_EN > 0u)
OS_Q         QCmlRecv;
OS_Q         QCmlTrans;
OS_Q         QSpeRecv;
OS_Q         QSpeTrans;
#endif

void AppTaskCreate (void);
void AppObjCreate (void);

//==================================================================================================
//| 函数名称 | Task_Start
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 开始任务
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Task_Start (void *p_arg)
{
    OS_ERR  os_err;


   (void)p_arg;

    Bsp_Init();                                                 /* Initialize BSP functions                             */
    //Bsp_TimeSampleInit();
    
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&os_err);                            /* 开启统计任务                                         */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* 测量中断关闭时间                                     */
#endif
    
    AppObjCreate();                                             /* 创建内核对象                                         */
    
    AppTaskCreate();                                            /* 创建任务                                             */

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
           
        static BOOL b_Led = FALSE;
        
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &os_err);
        
        //TRACE_DBG(">>DBG:       开始任务\r\n"); 
        
        b_Led = ~b_Led;
        Bsp_RunLed(b_Led);
        Bsp_Laser1(TRUE);
        Bsp_Laser2(TRUE);
        Bsp_Led1(b_Led);
        Bsp_Led2(b_Led);
        
        
#if 0
        {
            static BSP_TIMESAMPLE_TYPE   st_Timing[2] = {0};
            FP32   f_IntervalMs = 0;        
            Bsp_GetTimeSample(&st_Timing[0]);
            Bsp_DelayUs(20);
            Bsp_GetTimeSample(&st_Timing[1]);
            
            f_IntervalMs = Bsp_GetInterval(&st_Timing[0],&st_Timing[1]);
            TRACE_DBG(">>DBG:       间隔时间 = %f\r\n",f_IntervalMs);        
        }
#endif

    }
}

//==================================================================================================
//| 函数名称 | AppTaskCreate
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 创建任务
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void AppTaskCreate (void)
{
    OS_ERR  os_err;
    
    /* 按优先级从低倒高开始创建 */

    /* 命令行调试 */
    OSTaskCreate((OS_TCB       *)&TaskCmlSendTCB,                               /* 创建任务控制块 */
                 (CPU_CHAR     *)"Debug Task",                                  /* 任务名称 */
                 (OS_TASK_PTR   )Task_CmlSend,                                  /* 任务函数 */    
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_CML_SEND_PRIO,                            /* 任务优先级 */
                 (CPU_STK      *)&TaskCmlSendStk[0u],                           /* 任务堆载地址 */    
                 (CPU_STK_SIZE  )TASK_CML_SEND_STK_SIZE / 10u,                  /* 任务栈深限制 */        
                 (CPU_STK_SIZE  )TASK_CML_SEND_STK_SIZE,                        /* 任务堆栈大小 */ 
                 (OS_MSG_QTY    )25u,                                          /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */ 

    /* STDBUS */
    OSTaskCreate((OS_TCB       *)&TaskStdBusTCB,                                /* 创建任务控制块 */
                 (CPU_CHAR     *)"StdBus Task",                                 /* 任务名称 */
                 (OS_TASK_PTR   )Task_StdBus,                                   /* 任务函数 */    
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_STDBUS_PRIO,                              /* 任务优先级 */
                 (CPU_STK      *)&TaskStdBusStk[0u],                            /* 任务堆载地址 */    
                 (CPU_STK_SIZE  )TASK_STDBUS_STK_SIZE / 10u,                    /* 任务栈深限制 */        
                 (CPU_STK_SIZE  )TASK_STDBUS_STK_SIZE,                          /* 任务堆栈大小 */ 
                 (OS_MSG_QTY    )100u,                                           /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */
    

    /* 测速处理任务 */
    OSTaskCreate((OS_TCB       *)&TaskMeasSpeedTCB,                               /* 创建任务控制块 */
                 (CPU_CHAR     *)"Meas Speed",                                 /* 任务名称 */
                 (OS_TASK_PTR   )Task_MeasSpeed,                                  /* 任务函数 */    
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_MEASSPEED_PRIO,                             /* 任务优先级 */
                 (CPU_STK      *)&TaskMeasSpeedStk[0u],                           /* 任务堆载地址 */    
                 (CPU_STK_SIZE  )TASK_MEASSPEED_STK_SIZE / 10u,                   /* 任务栈深限制 */        
                 (CPU_STK_SIZE  )TASK_MEASSPEED_STK_SIZE,                         /* 任务堆栈大小 */ 
                 (OS_MSG_QTY    )10u,                                           /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */
#if 0 
    /* 灰度处理 */
    OSTaskCreate((OS_TCB       *)&TaskGreyProcTCB,                              /* 创建任务控制块 */
                 (CPU_CHAR     *)"Grey Process",                                /* 任务名称 */
                 (OS_TASK_PTR   )Task_GreyProc,                                 /* 任务函数 */    
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_GREYPROC_PRIO,                            /* 任务优先级 */
                 (CPU_STK      *)&TaskGreyProcStk[0u],                          /* 任务堆载地址 */    
                 (CPU_STK_SIZE  )TASK_GREYPROC_STK_SIZE / 10u,                  /* 任务栈深限制 */        
                 (CPU_STK_SIZE  )TASK_GREYPROC_STK_SIZE,                        /* 任务堆栈大小 */ 
                 (OS_MSG_QTY    )10u,                                           /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */
    
    
    /* 光谱仪通信 */
    OSTaskCreate((OS_TCB       *)&TaskUsbHostTCB,                               /* 创建任务控制块 */
                 (CPU_CHAR     *)"USB HOST",                                    /* 任务名称 */
                 (OS_TASK_PTR   )Task_UsbHost,                                  /* 任务函数 */    
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )TASK_USB_HOST_PRIO,                            /* 任务优先级 */
                 (CPU_STK      *)&TaskUsbHostStk[0u],                           /* 任务堆载地址 */    
                 (CPU_STK_SIZE  )TASK_USB_HOST_STK_SIZE / 10u,                  /* 任务栈深限制 */        
                 (CPU_STK_SIZE  )TASK_USB_HOST_STK_SIZE,                        /* 任务堆栈大小 */ 
                 (OS_MSG_QTY    )10u,                                           /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */ 


    /* 命令行调试 */
    OSTaskCreate((OS_TCB       *)&AppTaskCmlRecvTCB,                           /* 创建任务控制块 */
                 (CPU_CHAR     *)"App Task CML Recv",                          /* 任务名称 */
                 (OS_TASK_PTR   )AppTaskCmlRecv,                               /* 任务函数 */    
                 (void         *)0u,                                            /* 任务入参 */
                 (OS_PRIO       )APP_CFG_TASK_CML_RECV_PRIO,                   /* 任务优先级 */
                 (CPU_STK      *)&AppTaskCmlRecvStk[0u],                       /* 任务堆载地址 */    
                 (CPU_STK_SIZE  )APP_CFG_TASK_CML_RECV_STK_SIZE / 10u,         /* 任务栈深限制 */        
                 (CPU_STK_SIZE  )APP_CFG_TASK_CML_RECV_STK_SIZE,               /* 任务堆栈大小 */ 
                 (OS_MSG_QTY    )0u,                                            /* 内部消息队列的最大消息数目 */
                 (OS_TICK       )0u,                                            /* 时间片轮询的时间片数 */
                 (void         *)0u,                                            /* 用户补充存储区 */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&os_err);                                      /* 存放错误值 */ 
#endif

}

//==================================================================================================
//| 函数名称 | AppObjCreate
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 创建内核对象
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void AppObjCreate (void)
{
    OS_ERR  os_err;
#if (OS_CFG_SEM_EN > 0u)
    OSSemCreate(&Sem_CmlSend,
                "Sem Test",
                 1u,
                 &os_err);
#endif
    
#if (OS_CFG_Q_EN > 0u)
    OSQCreate(&QCmlRecv,
              "Queue Command Line Recv",
               10,
              &os_err);
    
    OSQCreate(&QCmlTrans,
              "Queue Command Line Trans",
               10,
              &os_err);
    
    OSQCreate(&QSpeRecv,
              "Queue Spectrometer Recv",
               5,
              &os_err);
    
    OSQCreate(&QSpeTrans,
              "Queue Spectrometer Trans",
               5,
              &os_err);
#endif
}

