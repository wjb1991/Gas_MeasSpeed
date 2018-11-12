#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

#include "bsp.h"

#include  <cpu.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <os.h>
#include  <os_app_hooks.h>


#include  "mod_measspeed.h"

#include  "task_cml.h"

#include  "app_save.h"

//==================================================================================
//                                   任务优先级    
//==================================================================================
#define  TASK_START_PRIO                OS_CFG_PRIO_MAX-4u   
#define  TASK_MEASSPEED_PRIO             4u

#define  TASK_STDBUS_PRIO               10u

#define  TASK_CML_RECV_PRIO             20u
#define  TASK_CML_SEND_PRIO             21u


//==================================================================================
//                                   任务堆栈大小 
//==================================================================================
#define  TASK_START_STK_SIZE            256u
#define  TASK_MEASSPEED_STK_SIZE        512u
#define  TASK_CML_SEND_STK_SIZE         256u
#define  TASK_CML_RECV_STK_SIZE         256u
#define  TASK_STDBUS_STK_SIZE           512u


//==================================================================================
//                                   任务控制块声明
//==================================================================================
extern  OS_TCB       TaskStartTCB;           /*  开始任务    */
extern  OS_TCB       TaskMeasSpeedTCB;       /*  测速任务            */
extern  OS_TCB       TaskCmlSendTCB;         /*  命令行调试任务    */
extern  OS_TCB       TaskCmlRecvTCB;         /*  命令行调试任务    */
extern  OS_TCB       TaskStdBusTCB;          /*  STDBUS任务    */

//==================================================================================
//                                   任务堆栈声明
//==================================================================================
extern  CPU_STK      TaskStartStk   [TASK_START_STK_SIZE];                  /*  开始任务    */
extern  CPU_STK      TaskCmlSendStk [TASK_CML_SEND_STK_SIZE];               /*  命令行调试任务    */
extern  CPU_STK      TaskCmlRecvStk [TASK_CML_RECV_STK_SIZE];               /*  命令行调试任务    */
extern  CPU_STK      TaskStdBusStk  [TASK_STDBUS_STK_SIZE];                 /*  STDBUS任务    */


//==================================================================================
//                                   任务函数声明
//==================================================================================
extern void Task_Start (void  *p_arg);               /*  开始任务    */
extern void Task_TransCml (void  *p_arg);            /*  命令行调试任务    */
extern void Task_RecvCml (void  *p_arg);             /*  命令行调试任务    */
extern void Task_StdBus (void  *p_arg);              /*  STDBUS总线通讯    */


//==================================================================================
//                                   队列声明
//==================================================================================
#if (OS_CFG_Q_EN > 0u)
extern  OS_Q         QCmlRecv;
extern  OS_Q         QCmlTrans;
extern  OS_Q         QSpeRecv;
extern  OS_Q         QSpeTrans;
#endif

//==================================================================================
//                                   信号量
//==================================================================================
#if (OS_CFG_SEM_EN > 0u)
extern  OS_SEM       AppTaskObjSem;
#endif

//==================================================================================
//                                   互斥信号量
//==================================================================================
#if (OS_CFG_MUTEX_EN > 0u)
static  OS_MUTEX     AppTaskObjMutex;
#endif

//==================================================================================
//                                   队列
//==================================================================================
#if (OS_CFG_Q_EN > 0u)
static  OS_Q         AppTaskObjQ;
#endif

//==================================================================================
//                                   事件标记
//==================================================================================
#if (OS_CFG_FLAG_EN > 0u)
static  OS_FLAG_GRP  AppTaskObjFlag;
#endif

//==================================================================================
//                                 跟踪调试配置
//==================================================================================
#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                        0u
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                       1u
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                        2u
#endif


#define  TRACE_LEVEL                            TRACE_LEVEL_DBG

#if 0
#define  TRACE(x)                               Bsp_Printf(x)//Task_CmlSendMsg(x,strlen(x))   //printf(x)                       

#define  TRACE_INFO(x)                          ((TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(TRACE(x)) : (void)0)
#define  TRACE_DBG(x)                           ((TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(TRACE(x)) : (void)0)
#endif

#define  TRACE_DBG(...)                         Bsp_Printf(__VA_ARGS__) 

#endif
