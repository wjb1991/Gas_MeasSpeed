#include "bsp_display.h"

const INT8U auch_DMCodeTab[16] = {
  0x3f,0x06,0x5b,0x4f,
  0x66,0x6d,0x7d,0x07,
  0x7f,0x6f,0x77,0x7c,
  0x39,0x5e,0x79,0x71
};

INT8U auch_DispBuff[6] = {0};

INT8U auch_NumBuff[3] = {0x12,0x23,0x34};


void Bsp_DispPoll(void)
{
    static INT8U sel = 0;
    INT16U i = 0;
    
    auch_DispBuff[0] = auch_NumBuff[1] % 0x10;
    auch_DispBuff[1] = auch_NumBuff[1] / 0x10;
    auch_DispBuff[2] = auch_NumBuff[2] / 0x10;
    auch_DispBuff[3] = auch_NumBuff[2] % 0x10;  
    auch_DispBuff[4] = auch_NumBuff[0] % 0x10;
    auch_DispBuff[5] = auch_NumBuff[0] / 0x10; 
    
    if(++sel >= 6)
    {
        sel = 0;
    }
    
    GPIOB->ODR = 0x0000;
    i |= (1ul<<(sel+8));
    i |= auch_DMCodeTab[auch_DispBuff[sel]];
    GPIOB->ODR = i;
    

}

void Bsp_DispSetNum(INT8U uch_Index, INT8U uch_Num)
{

    auch_NumBuff[uch_Index] = uch_Num;
}
