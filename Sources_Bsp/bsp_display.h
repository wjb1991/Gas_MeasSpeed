#ifndef __BSP_DISPLAY_H__
#define __BSP_DISPLAY_H__

#include "bsp.h"

extern INT8U auch_NumBuff[3];

void Bsp_DispPoll(void);

void Bsp_DispSetNum(INT8U uch_Index, INT8U uch_Num);

#endif
