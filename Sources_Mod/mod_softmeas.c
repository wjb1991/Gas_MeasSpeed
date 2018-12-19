#include "app_cfg.h"

void Task_SoftMeas(void *p_arg)
{
    while(1)
    {
        OS_ERR os_err;
        
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,NULL,&os_err);
      
        //因为会延时较长时间 就不应答了
        Bsp_Sync1(FALSE);
        Bsp_Sync2(FALSE);
        OSTimeDlyHMSM(0u, 0u, 0u, 100u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &os_err);
        
        Bsp_Sync1(TRUE);            
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &os_err);
        Bsp_Sync2(TRUE);  
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &os_err);
        Bsp_Sync1(FALSE);  
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &os_err);
        Bsp_Sync2(FALSE);  
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &os_err);
        
        /* 生成一次测速数据 */
        st_MeasSpeed.ul_Count++;
        st_MeasSpeed.e_Dirction = e_Sense1ToSense2;
        st_MeasSpeed.ul_Speed_mph = st_MeasSpeed.ul_Count % 10 + 100;
        st_MeasSpeed.ul_Acc_mps2 = st_MeasSpeed.ul_Count % 10;            
        
          
        OSTimeDlyHMSM(0u, 0u, 1u, 10u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &os_err);
        Bsp_Sync2(FALSE); 
    }
}