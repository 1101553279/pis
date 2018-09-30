#include "systick.h"
#include "stm32f10x.h"



void systick_setup( u32_t count )
{
    RCC_ClocksTypeDef clock;
    
    RCC_GetClocksFreq( &clock );

    
    SysTick_Config( clock.SYSCLK_Frequency / count );
    
    return;
}