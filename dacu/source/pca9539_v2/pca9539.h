#ifndef __PCA9539_H__
#define __PCA9539_H__

#include "btype.h"
#include "stm32f10x.h"

#define PCA_RST_H()  do{ GPIOE->BSRR |= GPIO_Pin_15; }while( 0 )
#define PCA_RST_L()  do{ GPIOE->BRR |= GPIO_Pin_15; }while( 0 )

#define REG_RIN     0x00
#define REG_WOUT    0x02
#define REG_POLAR   0x04
#define REG_CONFIG  0x06


u8_t chip_write( u8_t addr, u16_t data );
void chip_config( u8_t addr, u16_t data );
void chip_polar( u8_t addr, u16_t data );
u8_t chip_read( u8_t addr, u16_t *data );

u8_t pca9539_read( u8_t addr, u8_t reg, u16_t *data );
u8_t pca9539_write( u8_t addr, u8_t reg, u16_t data );

void pca9539_setup( void );


#endif 
