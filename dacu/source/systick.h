#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stm32f10x.h"
#include "btype.h"

#define SYSTICK_PERIOD_US    1000000
#define SYSTICK_PERIOD_MS    1000



void systick_setup( u32_t count );

#endif