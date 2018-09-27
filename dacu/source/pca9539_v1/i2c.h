#ifndef __I2C_H__
#define __I2C_H__

#include "btype.h"
#include "stm32f10x.h"

void i2c_delay( void );
void i2c_start( void );
void i2c_stop( void );
void i2c_ack( void );
void i2c_noack( void );
void i2c_waitack( void );
void i2c_sendbyte( u8_t byte );
u8_t i2c_recvbyte( void );

#endif
