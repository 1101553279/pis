#include "i2c.h"

#define SDA_H()   do{ GPIOE->BSRR |= GPIO_Pin_12; }while( 0 )
#define SDA_L()   do{ GPIOE->BRR |= GPIO_Pin_12; }while( 0 )
#define SCL_H()   do{ GPIOE->BSRR |= GPIO_Pin_13; }while( 0 )
#define SCL_L()   do{ GPIOE->BRR |= GPIO_Pin_13; }while( 0 )

#define SDA     (GPIOE->IDR & GPIO_Pin_12)

/* i2c com delay, you can optimize speed use this */
void i2c_delay(void)
{   
   u16 i = 240; 

   while( i ) 
   { 
     i--; 
   } 

    return;
}

/* start com */
void i2c_start(void)
{
    SDA_H();
    SCL_H();
    i2c_delay();
  
    SDA_L();
    i2c_delay();
  
    i2c_delay();
  
    return;
}

/* stop com */
void i2c_stop(void)
{
    SCL_L();
    i2c_delay();

    SDA_L();
    i2c_delay();

    SCL_H();
    i2c_delay();

    SDA_H();
    i2c_delay();

    return;
}

/* put a ack */
void i2c_ack(void)
{   
    SCL_L();
    i2c_delay();

    SDA_L();
    i2c_delay();

    SCL_H();
    i2c_delay();

    SCL_L();
    i2c_delay();

    return;
}

/* put a no ack */
void i2c_noack(void)
{   
  SCL_L();
  i2c_delay();

  SDA_H();
  i2c_delay();

  SCL_H();
  i2c_delay();

  SCL_L();
  i2c_delay();

  return;
}

/* wait a ack */
void i2c_waitack(void)
{
  SCL_L();
  i2c_delay();

  SDA_H();
  i2c_delay();

  SCL_H();
  i2c_delay();

  SCL_L();

  return; 
}

/* send one byte */
void i2c_sendbyte( u8_t byte )
{
    u8_t i = 8;

    while( i-- )
    {
        SCL_L();
        i2c_delay();

        if( byte & 0x80 )
            SDA_H();  
        else 
            SDA_L();   

        byte <<= 1;

        i2c_delay();

        SCL_H();
        i2c_delay();
    }
    
    SCL_L();
}

/* receive a byte from i2c bus */
u8_t i2c_recvbyte( void )
{ 
    u8_t i=8;
    u8_t byte=0;

    SDA_H();              
    while( i-- )
    {
      byte <<= 1;      

      SCL_L();
      i2c_delay();

      SCL_H();
      i2c_delay();
      
      if( SDA )
      {
        byte |= 0x01;
      }
    }

    SCL_L();

    return byte;
}

