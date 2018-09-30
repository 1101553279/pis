#include "sine_wave.h"
#include "stm32f10x.h"
#include "btype.h"

#define DAC_DHR12RD_Address       0x40007420

u32_t DualSine12bit[32];

const uint16_t Sine12bit[32] = {
                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 
                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};

void DAC_DMA_Config(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
    
    // config dma for sine wave
    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12RD_Address;         
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&DualSine12bit ;       
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                      
    DMA_InitStructure.DMA_BufferSize = 32;                                  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;         
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                     
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            
    DMA_Init( DMA2_Channel4, &DMA_InitStructure );

    DMA_Cmd( DMA2_Channel4, ENABLE );

    return;
}

void DAC_TIM_Config(void)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_Period = 0x753;                    
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;                   
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;               
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_SelectOutputTrigger( TIM2, TIM_TRGOSource_Update );

//    TIM_Cmd( TIM2, DISABLE );
    TIM_Cmd( TIM2, ENABLE );
    
    return;
}


void DAC_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef  DAC_InitStructure;

    /* setup dac gpio parameter */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* config dac parameter */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;           
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; 

    //  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /* channel 1 PA4, channel 2 PA5 */
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_Cmd(DAC_Channel_2, DISABLE);

    /* enable DAC's DMA request */
    DAC_DMACmd( DAC_Channel_2, ENABLE );

    return;
}


void sinewave_setup( )
{
    u8_t idx = 0; 

    DAC_DMA_Config();
    DAC_TIM_Config();
    DAC_Config();

    /* 填充正弦波形数据，双通道右对齐 */
    for( idx = 0; idx < 32; idx++ )
    {
        DualSine12bit[idx] = (Sine12bit[idx] << 16) + (Sine12bit[idx]);
    }
    return;
}


