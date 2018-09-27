#ifndef __PCA9539_H__
#define __PCA9539_H__

#include "btype.h"
#include "stm32f10x.h"

#define PCA_00_IN_MASK  0xf000
#define PCA_01_IN_MASK  0x00ff
#define PCA_02_IN_MASK  0x001f

#define PCA_RST_H()  do{ GPIOE->BSRR |= GPIO_Pin_15; }while( 0 )
#define PCA_RST_L()  do{ GPIOE->BRR |= GPIO_Pin_15; }while( 0 )

#define PCA_READ    0x01
#define PCA_WRITE   0x00

#define PCA_PREFIX  0xe8
#define PCA_ADDR_00 0x00
#define PCA_ADDR_01 0x02
#define PCA_ADDR_02 0x04

#define REG_RIN     0x00
#define REG_WOUT    0x02
#define REG_POLAR   0x04
#define REG_CONFIG  0x06

u8_t pca9539_read( u8_t addr, u8_t reg, u16_t *data );
u8_t pca9539_write( u8_t addr, u8_t reg, u16_t data );

void stm32_pca9539_setup( void );


#endif 
