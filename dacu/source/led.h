#ifndef __LED_H__
#define __LED_H__

#include "btype.h"

enum led_status{
    LS_OFF,
    LS_BLINK,
    LS_ON,
};

// led group index
enum led_no{
    LN_PCOM = 0,
    LN_DCOM,
    LN_PAIN,
    LN_PAOUT,
};

enum led_op{
    LE_BLINK,   // add blink led
    LE_ON,      // add on led
    LE_BK_OFF,  // remove blink led
    LE_ON_OFF,  // remove on led
};

enum led_hl{
    LV_L = 0,       // low level
    LV_H,       // high level
};

typedef void ( *led_cb_t )( u8_t ledno, u8_t hl );

struct led{
    u8_t no;      // pcom / dcom / pa_in / pa_out
    struct led_cnt{
        u8_t on:2;
        u8_t blink:6;
    }status;

    u8_t hl;        // current level for blink
    u32_t stime;    // start time   for blink
    u32_t timeout;  // period for blink

    led_cb_t cb;    // led timeout callback for blink status
};

/* call this first before using */
void led_init( struct led *led, u8_t type, led_cb_t cb, u32_t timeout );

// op one led
void led_con( struct led *led, u8_t op );

// check one led for led blink
void led_check( struct led *led );            

// for debug
void dump_led( struct led *led );

#endif
