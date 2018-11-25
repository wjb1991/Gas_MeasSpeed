#include "Bsp.h"


#define ADCCONVERTEDVALUES_BUFFER_SIZE   320    /* Size of array aADCxConvertedValues[] */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;

/* Variable containing ADC conversions results */
__IO uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE]; /* ADC conversion results table of regular group, channel on rank1 */

#define ADCx    ADC1

__IO FP32     af_ADCVoltFilte[16] = {0};

void Bsp_ADC1Init(void)
{
  ADC_ChannelConfTypeDef   sConfig;
  ADC_InjectionConfTypeDef sConfigInjected;
  
  /* Configuration of ADCx init structure: ADC parameters and regular group */
  AdcHandle.Instance = ADCx;

  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_ENABLE;              /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.ContinuousConvMode    = ENABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  AdcHandle.Init.NbrOfConversion       = 16;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Trig of conversion start done by external event */

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    //Error_Handler();
  }
 
  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
  /* Note: Considering IT occurring after each number of                      */
  /*       "ADCCONVERTEDVALUES_BUFFER_SIZE"  ADC conversions (IT by DMA end   */
  /*       of transfer), select sampling time and ADC clock with sufficient   */
  /*       duration to not create an overhead situation in IRQHandler.        */

    sConfig.Channel = ADC_CHANNEL_15;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    sConfig.Channel = ADC_CHANNEL_2;
    sConfig.Rank = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = ADC_REGULAR_RANK_4;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    sConfig.Channel = ADC_CHANNEL_14;
    sConfig.Rank = ADC_REGULAR_RANK_5;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    sConfig.Channel = ADC_CHANNEL_7;
    sConfig.Rank = ADC_REGULAR_RANK_6;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    sConfig.Channel = ADC_CHANNEL_6;
    sConfig.Rank = ADC_REGULAR_RANK_7;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    sConfig.Channel = ADC_CHANNEL_13;
    sConfig.Rank = ADC_REGULAR_RANK_8;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_REGULAR_RANK_9;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_10;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    sConfig.Channel = ADC_CHANNEL_10;
    sConfig.Rank = ADC_REGULAR_RANK_11;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    sConfig.Channel = ADC_CHANNEL_11;
    sConfig.Rank = ADC_REGULAR_RANK_12;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    /**Configure Regular Channel 
    */
    sConfig.Channel = ADC_CHANNEL_12;
    sConfig.Rank = ADC_REGULAR_RANK_13;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    /**Configure Regular Channel 
    */
    sConfig.Channel = ADC_CHANNEL_5;
    sConfig.Rank = ADC_REGULAR_RANK_14;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    /**Configure Regular Channel 
    */
    sConfig.Channel = ADC_CHANNEL_VREFINT;
    sConfig.Rank = ADC_REGULAR_RANK_15;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    /**Configure Regular Channel 
    */
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = ADC_REGULAR_RANK_16;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }
   
  
  
    if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
    {
        /* Calibration Error */
        //Error_Handler();
    }

    /* Start ADC conversion on regular group with transfer by DMA */
    if (HAL_ADC_Start_DMA(&AdcHandle,
                        (uint32_t *)aADCxConvertedValues,
                        ADCCONVERTEDVALUES_BUFFER_SIZE
                       ) != HAL_OK)
    {
        /* Start Error */
        //Error_Handler();
    }
  
}

void DMA1_Channel1_IRQHandler(void)
{
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif 
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    static DMA_HandleTypeDef  DmaHandle;
    
    
    if(adcHandle->Instance==ADC1)
    {
        /* ADC1 clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_DMA1_CLK_ENABLE();
        /**ADC1 GPIO Configuration    
        PC0     ------> ADC1_IN10
        PC1     ------> ADC1_IN11
        PC2     ------> ADC1_IN12
        PC3     ------> ADC1_IN13
        PA0-WKUP     ------> ADC1_IN0
        PA1     ------> ADC1_IN1
        PA2     ------> ADC1_IN2
        PA3     ------> ADC1_IN3
        PA5     ------> ADC1_IN5
        PA6     ------> ADC1_IN6
        PA7     ------> ADC1_IN7
        PC4     ------> ADC1_IN14
        PC5     ------> ADC1_IN15
        PB0     ------> ADC1_IN8
        PB1     ------> ADC1_IN9 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                      |GPIO_PIN_4|GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                      |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        DmaHandle.Instance = DMA1_Channel1;

        DmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        DmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
        DmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
        DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;   /* Transfer from ADC by half-word to match with ADC resolution 10 or 12 bits */
        DmaHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;   /* Transfer to memory by half-word to match with buffer variable type: half-word */
        DmaHandle.Init.Mode                = DMA_CIRCULAR;
        DmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;

        /* Deinitialize  & Initialize the DMA for new transfer */
        HAL_DMA_DeInit(&DmaHandle);  
        HAL_DMA_Init(&DmaHandle);

        /* Associate the initialized DMA handle to the ADC handle */
        __HAL_LINKDMA(adcHandle, DMA_Handle, DmaHandle);
        
      HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
      HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
        
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

    if(adcHandle->Instance==ADC1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE();
        __HAL_RCC_DMA1_CLK_DISABLE();
        
        /**ADC1 GPIO Configuration    
        PC0     ------> ADC1_IN10
        PC1     ------> ADC1_IN11
        PC2     ------> ADC1_IN12
        PC3     ------> ADC1_IN13
        PA0-WKUP     ------> ADC1_IN0
        PA1     ------> ADC1_IN1
        PA2     ------> ADC1_IN2
        PA3     ------> ADC1_IN3
        PA5     ------> ADC1_IN5
        PA6     ------> ADC1_IN6
        PA7     ------> ADC1_IN7
        PC4     ------> ADC1_IN14
        PC5     ------> ADC1_IN15
        PB0     ------> ADC1_IN8
        PB1     ------> ADC1_IN9 
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5);

        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1);

        /* ADC1 DMA DeInit */
        HAL_DMA_DeInit(adcHandle->DMA_Handle);

    }
} 


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    INT32U  aul_ADCFilte[16] = {0};
    INT16U  i = 0;
    INT16U  j = 0;
    for(i = 0; i < 20; i++)
    {
        for(j = 0; j<16; j++)
        {
            aul_ADCFilte[j] += aADCxConvertedValues[i*16+j];
        }
    }
    
    for(j = 0; j<16; j++)
    {
        af_ADCVoltFilte[j] = aul_ADCFilte[j] * 3.3 / 4096 /20;
    }
    
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{

}


