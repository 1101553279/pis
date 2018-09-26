#include "led.h"
#include "ptime.h"
#include "debug.h"
#include "chip_pca.h"

void led_init( struct led *led, u8_t no, led_cb_t cb, u32_t timeout )
{
    if( 0 == led )
        return;
    
    led->no = no;
    led->status.on = 0;
    led->status.blink = 0;
    led->hl = LV_L;
    led->stime = ptime( );
    led->timeout = timeout;
    led->cb = cb;

//    if( led->cb )
//        led->cb( PCA_TYPE_OUT, PCA_ID_OUT_LED, (led->no << 8)|led->hl );

    return;
}

/* off-> blink, blink->off, blink to on, on -> off */
void led_con( struct led *led, u8_t op )
{
    if( 0 == led )
        return;
    
    switch( op )
    {
        case LE_BLINK:
            led->status.blink++;
            break;

        case LE_ON:
            if( led->status.blink > 0 )
                led->status.blink--;
            led->status.on++;
            break;

        case LE_BK_OFF:
            if( led->status.blink > 0 )
                led->status.blink--;
            break;

        case LE_ON_OFF:
            if( led->status.on > 0 )
                led->status.on--;
            break;

        default:
            break;
    }
   
                                    // on or blink  // defult on
    if( led->status.on > 0 || led->status.blink > 0 )
    {
        led->hl = LV_H;

        //for led blink
        led->stime = ptime( );
    }
    
//   if( led->cb )
//        led->cb( PCA_TYPE_OUT, PCA_ID_OUT_LED, (led->no << 8)|led->hl );
/*
        if( led->cb )
            led->cb( led->no, led->hl );
    }
    else                            // led off
    {
        if( led->cb )
            led->cb( led->no, LV_L );
    }
*/
    return;
}

/* check timeout */
void led_check( struct led *led )
{
    if( 0 == led )
        return;

    // for blink
    if( led->status.on <= 0 &&          // no led on
        led->status.blink > 0 &&        // is led blink
        (ptime() - led->stime) >= led->timeout ) // timeout
    {
       led->hl = ( LV_H == led->hl ) ? LV_L: LV_H; 
       led->stime = ptime();

//       if( led->cb )
//           led->cb( PCA_TYPE_OUT, PCA_ID_OUT_LED, (led->no << 8)|led->hl );
    }

    return;
}


/* for debug */
void dump_led( struct led *led )
{
    if( 0 == led )
        return;
    
    dout( "%-5s %-5s %-10s %-5s %-10s %-10s\n",
            "no", "on", "blink", "hl", "stime",  "timeout" );
    dout( "%-5d %-5d %-10d %-5d %-10d %-10d\n", 
            led->no, led->status.on, led->status.blink, led->hl,
            led->stime, led->timeout );
    dout( "\n" );

    return;
}
