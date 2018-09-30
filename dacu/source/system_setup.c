#include "system_setup.h"
#include "stm32f10x.h"


void system_setup()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

    //for sine_wave 
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA2, ENABLE );
    
    /*PCA9539 GPIO && PCA9539 EXIT INTERRUPT */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE );
    
    // for sine wave 
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
    
    // for sine wave
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    
    return;
}