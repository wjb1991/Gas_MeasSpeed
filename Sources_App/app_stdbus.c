#include "app_stdbus.h"

#define DEF_SUBCMD_READ     0x55
#define DEF_SUBCMD_WRITE    0x66

//==================================================================================
//| 函数名称 | DealAppCmd
//|----------|----------------------------------------------------------------------
//| 函数功能 | 处理上位机发送下来的命令 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 0:不应答 1:应答
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
uint8_t Deal_SlavePack(StdBus_t* pst_Fram)
{
    uint8_t res = 0;
    OS_ERR os_err;
    
    switch(pst_Fram->uch_Cmd)
    {
#if 0
    case 0x10:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
            if( pst_Fram->uin_PayLoadLenth == 3) 
            {
                auch_NumBuff[0] = pst_Fram->puc_PayLoad[0];
                auch_NumBuff[1] = pst_Fram->puc_PayLoad[1];
                auch_NumBuff[2] = pst_Fram->puc_PayLoad[2];   
                res = 1;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 3;
            pst_Fram->puc_PayLoad[0] = auch_NumBuff[0];
            pst_Fram->puc_PayLoad[1] = auch_NumBuff[1];
            pst_Fram->puc_PayLoad[2] = auch_NumBuff[2];
            res = 1;    //应答
        }
        break; 
#endif
    default:
        break;
      
    }


    return res;
}

uint8_t Deal_MasterPack(StdBus_t* pst_Fram)
{
    
    return 0;
}