#include "pca9539.h"

 /* com to pca chip addr, op reg register, object to data */
void pca9539_read(u8_t addr, u8_t reg, u16_t *data )
{ 
    *data = 0;

    i2c_start();

    i2c_sendbyte( PCA_PREFIX| addr | PCA_WRITE );

    i2c_waitack();

    i2c_sendbyte(reg);

    i2c_waitack();

    i2c_start();

    i2c_sendbyte( PCA_PREFIX| addr |PCA_READ );

    i2c_waitack();    

    *data = i2c_recvbyte();

    i2c_ack();

    *data |= (i2c_recvbyte() << 8);

    i2c_noack();

    i2c_stop();
    
    return;
 }

 /* com to pca chip addr, op reg register, object to data */
void pca9539_write( u8_t addr, u8_t reg, u16_t data )
{
    u8_t t;

    i2c_start();

    i2c_sendbyte( PCA_PREFIX | addr | PCA_WRITE );

    i2c_waitack();

    i2c_sendbyte( reg );

    i2c_waitack();

    t = data & 0xff;
    i2c_sendbyte( t );

    i2c_waitack();
    
    t = (data >> 8) & 0xff;
    i2c_sendbyte( t );

    i2c_waitack();

    i2c_stop();

    return;
}


/* config pca9539 stm32 interface */
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
  
  PCA_RST_H();             // 低电平用于reset pca9539	

  /* 配置输入中断 */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);

  /* Configure Trigger falling edge generate the interrupt*/
  EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;// 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

//  PCA_RST_H();
//  PCA_RST_L(); 
//  PCA_RST_H();
  
// config pca9539 input ios
  pca9539_write( PCA_ADDR_00, REG_CONFIG, PCA_00_IN_MASK );
  pca9539_write( PCA_ADDR_01, REG_CONFIG, PCA_01_IN_MASK );
  pca9539_write( PCA_ADDR_02, REG_CONFIG, PCA_02_IN_MASK );

  return;
}  