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
    //OS_ERR os_err;
    
    switch(pst_Fram->uch_Cmd)
    {
//=======================================================
//                  设置/读取DAC电压
//=======================================================   
    case 0x10:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
            if( pst_Fram->uin_PayLoadLenth == 4) 
            {
                FP32    f_Tmp = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[0],FALSE);
                
                Bsp_DacSetVolt(&Bsp_Dac1,f_Tmp);
                
                res = 1;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 4;
            
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],Bsp_Dac1.f_DacVolt,FALSE);
            res = 1;    //应答
        }
        break; 
//=======================================================
//                  读取13路传感器的电压 7路/6路 13路
//=======================================================   
    case 0x20:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令
            INT8U   i = 0;

            pst_Fram->uin_PayLoadLenth = 4*13; 
            
            for( i = 0; i<13; i++)
            {
                Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4],af_ADCVoltFilte[i],FALSE);            
            }

            res = 1;    //应答
        }
        break; 
//=======================================================
//                  设置/读取激光器状态
//=======================================================   
    case 0x30:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
            if( pst_Fram->uin_PayLoadLenth == 2) 
            {
                if(pst_Fram->puc_PayLoad[0] == 0)
                    Bsp_Laser1(e_LaserOff);
                else
                    Bsp_Laser1(e_LaserOn);
                  
                if(pst_Fram->puc_PayLoad[1] == 0)
                    Bsp_Laser2(e_LaserOff);
                else
                    Bsp_Laser2(e_LaserOn);
                
                res = 1;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令
            if( pst_Fram->uin_PayLoadLenth == 1) 
            {
                if(pst_Fram->puc_PayLoad[0] == 0)
                {
                    pst_Fram->puc_PayLoad[1] = Bsp_GetLaser1();
                }
                else if(pst_Fram->puc_PayLoad[0] == 1)
                {
                    pst_Fram->puc_PayLoad[1] = Bsp_GetLaser2();
                }
                pst_Fram->uin_PayLoadLenth = 2;          
                res = 1;    //应答       
            }
        }
        break;
        
//=======================================================
//                  读取一次测速结果
//=======================================================   
    case 0x40:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   

        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {
            //读命令
            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[0],st_MeasSpeed.ul_Count,FALSE);
            
            pst_Fram->puc_PayLoad[4] = st_MeasSpeed.e_Dirction;
            
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[5],st_MeasSpeed.ul_Speed_mph,FALSE);    
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[9],st_MeasSpeed.ul_Acc_mps2,FALSE);
            
            pst_Fram->uin_PayLoadLenth = 13;          
            res = 1;    //应答   
        }
        break; 
        
//=======================================================
//              调试用  软件产生一次 同步信号
//=======================================================   
    case 0x80:
        if(pst_Fram->uch_SubCmd == DEF_SUBCMD_WRITE) 
        {   
            OS_ERR os_err;
            OSTaskSemPost(&TaskSoftMeasTCB,OS_OPT_POST_NONE,&os_err);
            res = 1;    //应答   
        }
        else if(pst_Fram->uch_SubCmd == DEF_SUBCMD_READ)
        {

            
        }
        break; 
    default:
        break;
      
    }

    return res;
}

uint8_t Deal_MasterPack(StdBus_t* pst_Fram)
{
    
    return 0;
}