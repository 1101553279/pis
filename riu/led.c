#include "led.h"
#include <stdio.h>
#include "debug.h"

// internal structure
struct riu_led{
    led_no_t no;    // led number
    void *group;    // operate which gpio group
    int pin;        // operate which gpio pin
    int mode;       // 0 : 0 -> on, 1 -> off
                    // 1 : 1 -> on, 0 -> off
};

#define MODE_ZERO    0
#define MODE_ONE     1

#define LED_SET( gpio, pin )    do { out( "%#x, %d, set\r\n", (u32_t)gpio, pin ); }while(0)
#define LED_RESET( gpio, pin )  do { out( "%#x, %d, reset\r\n", (u32_t)gpio, pin );}while( 0 )

#define LED_CHON( mode, gpio, pin )   do{                               \
                                    if( MODE_ZERO == (mode) )           \
                                        LED_RESET( (gpio), (pin) );     \
                                    else                                \
                                        LED_SET( (gpio), (pin) );       \
                                    }while( 0 )

#define LED_CHOFF( mode, gpio, pin )    do{                             \
                                        if( MODE_ZERO == (mode) )       \
                                            LED_SET( (gpio), (pin) );   \
                                        else                            \
                                            LED_RESET( (gpio), (pin) ); \
                                        }while( 0 )

static struct riu_led leds[ MAX_LED ];



/* init led module */
void led_init( void )
{
    leds[ LED_RUN ].no = LED_RUN;   
    leds[ LED_RUN ].group = 0;
    leds[ LED_RUN ].pin = 11;
    leds[ LED_RUN ].mode = 1;

    leds[ LED_USB ].no = LED_USB;
    leds[ LED_USB ].group = 0;
    leds[ LED_USB ].pin = 12;
    leds[ LED_USB ].mode = 0;

    leds[ LED_COM ].no = LED_COM;
    leds[ LED_COM ].group = 0;
    leds[ LED_COM ].pin = 13;
    leds[ LED_COM ].mode = 0;

    leds[ LED_ST ].no = LED_ST;
    leds[ LED_ST ].group = 0;
    leds[ LED_ST ].pin = 14;
    leds[ LED_ST ].mode = 0;

    leds[ LED_EMG ].no = LED_EMG;
    leds[ LED_EMG ].group = 0;
    leds[ LED_EMG ].pin = 15;
    leds[ LED_EMG ].mode = 0;

    return;
}

/* control led module */
void led_con( led_no_t no, u8_t key )
{
    struct riu_led *led;
    
    if( no >= MAX_LED )
        return;

    led = &leds[ no ];
//  printf( "led: group = %#x, led = %d,mode = %d\r\n", led->group, led->pin, led->mode );
    if( LED_ON == key )
        LED_CHON( led->mode, led->group, led->pin );
    else
        LED_CHOFF( led->mode, led->group, led->pin );
        
    return;
}


/*
 * control all leds 
 * */
void led_con_all( u8_t key )
{
    u8_t in = 0;

    if( LED_ON == key )
    {
        for( in = 0; in < MAX_LED; in++ )
            LED_CHON( leds[in].mode, leds[in].group, leds[in].pin );
    }
    else
    {
        for( in = 0; in < MAX_LED; in++ )
            LED_CHOFF( leds[in].mode, leds[in].group, leds[in].pin );
    }

    return;
}
