#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "Bsp.h"

typedef struct Bsp_TimeSample_Type  {
    INT32U ul_Cnt; 
    INT32U ul_Tick;
    INT32U ul_CntReload;
    INT32U ul_TickReload;
}BSP_TIMESAMPLE_TYPE;

void Bsp_DelayUs(INT32U us);
void Bsp_DelayMs(INT32U ms);

void Bsp_TimeSampleInit(void);
void Bsp_GetTimeSample(BSP_TIMESAMPLE_TYPE* pst_Ts);
INT32U Bsp_GetInterval(BSP_TIMESAMPLE_TYPE* pst_TsOld,BSP_TIMESAMPLE_TYPE * pst_TsNew);


#endif
