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
    led->hl = LV_OFF;
    led->stime = ptime( );
    led->timeout = timeout;
    led->cb = cb;

    return;
}

static u16_t led_pca( u8_t ledno, u8_t lv )
{
    u16_t out = 0;
    
    switch( ledno )
    {
        case LN_PAIN:
            out = (LV_ON == lv) ? LED_PA_IN_ON: LED_PA_IN_OFF; 
            break;
        
        case LN_PCOM:
            out = (LV_ON == lv) ? LED_PCOM_ON: LED_PCOM_OFF;
            break;
        
        case LN_DCOM:
            out = (LV_ON == lv) ? LED_DCOM_ON: LED_DCOM_OFF;
            break;
        
        case LN_PAOUT:
            out = (LV_ON == lv) ? LED_PA_OUT_ON: LED_PA_OUT_OFF;
            break;
        
        default:
            break;
    }
    
    return out;
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
        if( LV_OFF == led->hl )
        {
            led->hl = LV_ON;
            if( led->cb )
                led->cb( PCA_ID_OUT_LED, led_pca( led->no, led->hl) );
        }

        //for led blink
        led->stime = ptime( );
    }
    
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
        led->hl = ( LV_ON == led->hl ) ? LV_OFF: LV_ON; 

        if( led->cb )
            led->cb( PCA_ID_OUT_LED, led_pca( led->no, led->hl) );
        led->stime = ptime();
    }

    return;
}


/* for debug */
void dump_led( struct led *led )
{
    if( 0 == led )
        return;
    
    dout( "%-5s %-5s %-10s %-5s %-10s %-10s %-10s\n",
            "no", "on", "blink", "hl", "stime",  "timeout", "curtime" );
    dout( "%-5d %-5d %-10d %-5d %-10d %-10d %-10d\n", 
            led->no, led->status.on, led->status.blink, led->hl,
            led->stime, led->timeout, ptime() );
    dout( "\n" );

    return;
}
