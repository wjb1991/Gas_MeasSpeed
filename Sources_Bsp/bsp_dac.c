#include  "bsp.h"

DAC_HandleTypeDef hdac1 = {DAC};
DAC_HandleTypeDef hdac2 = {DAC1};

BSP_DAC_TYPE    Bsp_Dac1 = {
   .pv_DacHandle    = &hdac1,
   .ul_Channel      = DAC_CHANNEL_1,
   .ul_Alignment    = DAC_ALIGN_12B_R,
   .ul_Trigger      = DAC_TRIGGER_NONE,
   .ul_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE,
   .uin_DacValue    = 0,
   .f_DacVolt       = 0,
   .uin_Resolution  = 4095,
   .f_RefVolt       = 3.3,
};

void Bsp_DacInit(BSP_DAC_TYPE* pst_Dev)
{
    DAC_ChannelConfTypeDef sConfig;

    /**DAC Initialization 
    */
    if (HAL_DAC_Init(pst_Dev->pv_DacHandle) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    /**DAC channel OUT1 config 
    */
    sConfig.DAC_Trigger = pst_Dev->ul_Trigger;
    sConfig.DAC_OutputBuffer = pst_Dev->ul_OutputBuffer;
    if (HAL_DAC_ConfigChannel(pst_Dev->pv_DacHandle, &sConfig, pst_Dev->ul_Channel) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }
    
    //HAL_DAC_Start
    Bsp_DacSetVolt(pst_Dev,0);
    HAL_DAC_Start(pst_Dev->pv_DacHandle,pst_Dev->ul_Channel);
}

void Bsp_DacSetVolt(BSP_DAC_TYPE* pst_Dev,FP32 f_Volt)
{
    INT16U i =  (INT16U)(f_Volt * pst_Dev->uin_Resolution / pst_Dev->f_RefVolt);
    pst_Dev->f_DacVolt = f_Volt;
    HAL_DAC_SetValue(pst_Dev->pv_DacHandle,pst_Dev->ul_Channel,pst_Dev->ul_Alignment,i);
}

void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if(dacHandle->Instance==DAC)
    {
        __HAL_RCC_DAC_CLK_ENABLE();

        /**DAC GPIO Configuration    
        PA4     ------> DAC_OUT1 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle)
{

    if(dacHandle->Instance==DAC)
    {
        __HAL_RCC_DAC_CLK_DISABLE();

        /**DAC GPIO Configuration    
        PA4     ------> DAC_OUT1 
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
    }
} 

