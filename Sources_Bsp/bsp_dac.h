
#ifndef __BSP_DAC_H__
#define __BSP_DAC_H__

#include "Bsp.h"

typedef struct {
    void    *pv_DacHandle;
    
    INT32U  ul_Channel; 
    INT32U  ul_Alignment;
    INT32U  ul_Trigger;
    INT32U  ul_OutputBuffer;

    INT16U   uin_DacValue;
    FP32    f_DacVolt;
    INT16U   uin_Resolution;  
    FP32    f_RefVolt;


}BSP_DAC_TYPE;

extern BSP_DAC_TYPE    Bsp_Dac1;

void Bsp_DacInit(BSP_DAC_TYPE* pst_Dev);

void Bsp_DacSetVolt(BSP_DAC_TYPE* pst_Dev,FP32 f_Volt);

#endif
