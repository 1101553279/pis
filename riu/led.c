#include "led.h"



struct stm32_led{
    led_no_t no;    // led number
    void *group;    // operate which gpio group
    int pin;        // operate which gpio pin
};

static struct stm32_led leds[ MAX_LED ];

/* init led module */
void led_init( void )
{
     

    return;
}

/* control led module */
void led_con( led_no_t no, u8_t key )
{


    return;
}
