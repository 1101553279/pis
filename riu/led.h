#ifndef __LED_H__
#define __LED_H__
#include "ty.h"

#define LED_ON  1
#define LED_OFF 0


typedef enum _led_no{
    LED_RUN,    // 0 run led
    LED_USB,    // 1 usb led
    LED_COM,    // 2 communicate led
    LED_ST,     // 3 station play
    LED_EMG,    // 4 emg play
    MAX_LED,    
}led_no_t;


void led_init( void );
void led_con( led_no_t no, u8_t key );


/*
// one way
                                GPIO
#define RUN_LED( flag ) ...
#define USB_LED( flag ) ...
#define COM_LED( flag )  ...
#define ST_LED( flag ) ....
#define EMG_LED( flags ) ...

// second way
void led_on_con( led_no_t no );
void led_off_con( led_no_t no );

// third way
void led_con( led_on_t no, int flag );      // flag = on/off
*/


#endif
