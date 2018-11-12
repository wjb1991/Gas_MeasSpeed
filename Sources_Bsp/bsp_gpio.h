
#ifndef  BSP_LED_H_
#define  BSP_LED_H_

#include "bsp.h"

#ifdef __cplusplus
extern  "C" {           
#endif

typedef enum {
    eRs485Trans = GPIO_PIN_SET, eRs485Recv =  GPIO_PIN_RESET,
} Rs485de_t;

typedef enum {
    e_LedOn = GPIO_PIN_RESET, e_LedOff =  GPIO_PIN_SET,
} Led_t;

typedef enum {
    e_LaserOn = GPIO_PIN_SET, e_LaserOff =  GPIO_PIN_RESET,
} Laser_t;

typedef struct {
    void * vp_GpioPort;
    INT16U  uin_GpioPin;
    BOOL  b_IsRising;
    BSP_TIMESAMPLE_TYPE* st_Ts;   //BSP_TIMESAMPLE_TYPE:报错  struct _Bsp_TimeSample_Type_:则不报错 
}BSP_GPIOEVENT_TYPE;

extern void  (*Bsp_GpioEventHandle)(BSP_GPIOEVENT_TYPE* pst_Event);

void  Bsp_GpioInit(void);
void  Bsp_Rs485de(Rs485de_t status);
void  Bsp_Laser1(Laser_t status);
void  Bsp_Laser2(Laser_t status);
void  Bsp_Led1(Led_t status);
void  Bsp_Led2(Led_t status);
void  Bsp_RunLed(Led_t status);

#ifdef __cplusplus
}                                             
#endif


#endif
