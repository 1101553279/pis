#include "led.h"
#include <stdio.h>


// internal structure
struct riu_led{
    led_no_t no;    // led number
    void *group;    // operate which gpio group
    int pin;        // operate which gpio pin
    int flag;       // 0 : 0 -> on, 1 -> off
                    // 1 : 1 -> on, 0 -> off
};

#define LED_FLAG_ZERO    0
#define LED_FLAG_ONE     1

#define LED_SET( gpio, pin )    printf( "## %#x, %d, set\r\n", (u32_t)gpio, pin )
#define LED_RESET( gpio, pin )  printf( "## %#x, %d, reset\r\n", (u32_t)gpio, pin )

static struct riu_led leds[ MAX_LED ];



/* init led module */
void led_init( void )
{
    leds[ LED_RUN ].no = LED_RUN;   
    leds[ LED_RUN ].group = 0;
    leds[ LED_RUN ].pin = 11;
    leds[ LED_RUN ].flag = 1;

    leds[ LED_USB ].no = LED_USB;
    leds[ LED_USB ].group = 0;
    leds[ LED_USB ].pin = 12;
    leds[ LED_USB ].flag = 0;

    leds[ LED_COM ].no = LED_COM;
    leds[ LED_COM ].group = 0;
    leds[ LED_COM ].pin = 13;
    leds[ LED_COM ].flag = 0;

    leds[ LED_ST ].no = LED_ST;
    leds[ LED_ST ].group = 0;
    leds[ LED_ST ].pin = 14;
    leds[ LED_ST ].flag = 0;

    leds[ LED_EMG ].no = LED_EMG;
    leds[ LED_EMG ].group = 0;
    leds[ LED_EMG ].pin = 15;
    leds[ LED_EMG ].flag = 0;

    return;
}

/* control led module */
void led_con( led_no_t no, u8_t key )
{
    struct riu_led *led;
    
    if( no >= MAX_LED )
        return;

    led = &leds[ no ];
//  printf( "led: group = %#x, led = %d,flag = %d\r\n", led->group, led->pin, led->flag );
    if( LED_ON == key )
    {
        if( LED_FLAG_ZERO == led->flag )
            LED_RESET( led->group, led->pin );
        else
            LED_SET( led->group, led->pin );
    }
    else
    {
        if( LED_FLAG_ZERO == led->flag )
            LED_SET( led->group, led->pin );
        else
            LED_RESET( led->group, led->pin );
    }
        
    return;
}
