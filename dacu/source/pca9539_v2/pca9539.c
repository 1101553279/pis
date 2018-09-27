#include "pca9539.h"

#define SDA_H()   do{ GPIOE->BSRR |= GPIO_Pin_12; }while( 0 )
#define SDA_L()   do{ GPIOE->BRR |= GPIO_Pin_12; }while( 0 )
#define SCL_H()   do{ GPIOE->BSRR |= GPIO_Pin_13; }while( 0 )
#define SCL_L()   do{ GPIOE->BRR |= GPIO_Pin_13; }while( 0 )

#define SDA     (GPIOE->IDR & GPIO_Pin_12)

//#define	SDA_State	  GPIO_Pin_12

static void i2c_delay( void );
static u8_t i2c_start( void );
static void i2c_stop( void );
static void i2c_ack( void );
static void i2c_noack( void );
static u8_t i2c_waitack( void );
static void i2c_sendbyte( u8 byte );
static u8_t i2c_recvbyte( void );
static void delay_ms( u32_t cnt );

/* i2c com delay, you can optimize speed use this */
void i2c_delay(void)
{	
//   u16 i = 240; //这里可以优化速度
//   while( i) 
//   { 
//     i--; 
//   }
        
    return;
}

/* start com */
u8_t i2c_start(void)
{
	SDA_H();
	SCL_H();
	i2c_delay();
  
//	if(!( SDA == SDA_State))
//        return 0;
  
	SDA_L();
	i2c_delay();
  
	if( SDA ) 
        return 0;
  
	SDA_L();
	i2c_delay();
  
	return 1;
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
//	SDAH;
    
    return;
}

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

/* return   1 -> ack
            0 -> noack
*/
u8_t i2c_waitack( void )
{
  SCL_L();
  i2c_delay();
    
  SDA_H();
  i2c_delay();
    
  SCL_H();
  i2c_delay();
    
  if( SDA )
  {
      SCL_L();
        return 0;
  }
  
  SCL_L();
  
  return 1;
}

/* send one byte */
void i2c_sendbyte( u8_t byte )
{
    u8_t i = 8;
    
    while(i--)
    {
        SCL_L();
        i2c_delay();
        
        if( byte &0x80)
            SDA_H();  
        else 
            SDA_L();  
        
        byte <<= 1;
        i2c_delay();
        
        SCL_H();
        i2c_delay();
    }
    
    SCL_L();
    
    return;
}

/* receive a byte from i2c bus */
u8_t i2c_recvbyte( void )
{ 
    u8_t i = 8;
    u8_t byte =0;

    SDA_H();				
    while(i--)
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

void delay_ms( u32_t cnt )
{
//  u16 delay;
//  
//  while(nCount--)
//  {
//    delay = 24000;
//    while(delay--);
//  }
    return;
}

/* com to pca chip addr, op reg register, object to data */
u8_t pca9539_read( u8_t addr, u8_t reg, u16_t *data )
{
    *data = 0;
    
    if(!i2c_start())
        return 0;

    i2c_sendbyte( PCA_PREFIX | addr | PCA_WRITE );
    i2c_waitack();

    i2c_sendbyte(reg);

    i2c_waitack();
    delay_ms( 2 );

    if(!i2c_start())
        return 0;
    
    i2c_sendbyte( PCA_PREFIX | addr | PCA_READ );
    i2c_waitack();
	
    delay_ms( 10 );

    *data = i2c_recvbyte();
    i2c_ack();
    
    *data |= i2c_recvbyte()<<8;
    i2c_noack();

    i2c_stop();

  return 1;
 }

/* com to pca chip addr, op reg register, object to data */
u8_t pca9539_write( u8_t addr, u8_t reg, u16_t data )
{
    u8_t t;
    
    if( !i2c_start() )
        return 0; 

    i2c_sendbyte( PCA_PREFIX | addr | PCA_WRITE );
    i2c_waitack();

    i2c_sendbyte(reg);

    i2c_waitack();

    t = data & 0xff;
    i2c_sendbyte( t );
    i2c_waitack();
    
    t = (data >> 8) & 0xff;
    i2c_sendbyte( t );
    i2c_waitack();

    i2c_stop();

  return 1;
}


void stm32_pca9539_setup(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    /* Configure the pca9539 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Configure the pca9539 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  
  
  /* hardware initializes */
  /* Enable GPIOs clocks */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE );
                         
  /* Configure the PCA9539 I2C driver */			  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
  
  // pca9539 interrupt
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  // pca9539 reset
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  PCA_RST_L();


  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);

  /* Configure Trigger falling edge generate the interrupt*/
  EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;// 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  PCA_RST_H();
  delay_ms( 10 );
  PCA_RST_L(); 
  delay_ms( 10 );
  PCA_RST_H();
  
  pca9539_write( PCA_ADDR_00, REG_CONFIG, PCA_00_IN_MASK );
  pca9539_write( PCA_ADDR_01, REG_CONFIG, PCA_01_IN_MASK );
  pca9539_write( PCA_ADDR_02, REG_CONFIG, PCA_02_IN_MASK );

  pca9539_write( PCA_ADDR_00, REG_POLAR, PCA_00_IN_MASK );	
  pca9539_write( PCA_ADDR_02, REG_POLAR, PCA_02_IN_MASK );	

  return;
}  
