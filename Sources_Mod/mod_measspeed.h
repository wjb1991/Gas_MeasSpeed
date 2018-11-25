#ifndef __MOD_MEASSPEED_H__
#define __MOD_MEASSPEED_H__

#include "app_cfg.h"

#define     MOD_MEASSPEED_DEBUG     (TRUE)

typedef enum {
    e_Idle, e_MeasSpeed, e_MeasLenth, e_Leave,
}MOD_MEASSTATE_TYPE;

typedef enum {
    e_Null, e_Sense1ToSense2 = 0,   e_Sense2ToSense1 = 1,
}MOD_MEASSDIRCTION_TYPE;

typedef struct{
    INT16U  uin_Id;
    INT16U  uin_UsePinMask;      //使用的引脚
    INT16U  uin_PinStatus;       //引脚的状态
    INT32U  ul_RisingThreshold;  //切换到有车状态的阈值 检测到被遮挡的传感器个数
    INT32U  ul_FallThreshold;    //切换到没车状态的阈值 检测到未遮挡的传感器个数
    
    BOOL    b_Status;           //FALSE:没车 TRUE:有车
    INT32U  ul_Count;
    BSP_TIMESAMPLE_TYPE st_RisingTs;
    BSP_TIMESAMPLE_TYPE st_FallTs;
}MOD_MEASSENSN_TYPE;

typedef struct{
    MOD_MEASSENSN_TYPE*     pst_Sense1;
    MOD_MEASSENSN_TYPE*     pst_Sense2;
    MOD_MEASSTATE_TYPE      e_State;
    MOD_MEASSDIRCTION_TYPE  e_Dirction;

    INT32U  ul_LeaveDelayMs;
    
    BSP_TIMESAMPLE_TYPE     st_Ts1;
    BSP_TIMESAMPLE_TYPE     st_Ts2;
    BSP_TIMESAMPLE_TYPE     st_Ts3;
    BSP_TIMESAMPLE_TYPE     st_Ts4;
    
    INT32U  ul_Lenth;
    INT32U  ul_Count;
    FP32    ul_Speed_mph;       //米/小时 相当于 千米/小时 放大1000倍
    FP32    ul_Acc_mps2;        //米/秒^2    
    
    void(*cb_MeasSpeedEvent)(void *pst_Mod);
}MOD_MEASSPEED_TYPE;

extern MOD_MEASSENSN_TYPE st_LaserSense1;
extern MOD_MEASSENSN_TYPE st_LaserSense2;
extern MOD_MEASSPEED_TYPE st_MeasSpeed;

void Mod_MeasSpeedInit(MOD_MEASSPEED_TYPE *psd_Mod);
void Mod_MeasSpeedPoll(MOD_MEASSPEED_TYPE *psd_Mod,BSP_GPIOEVENT_TYPE* pst_GpioEvent);
void Mod_MeasSpeedTimeOut(MOD_MEASSPEED_TYPE *pst_Mod);

#endif
