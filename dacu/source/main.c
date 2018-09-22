#include <stdio.h>
#include "led.h"
#include "loud.h"
#include "debug.h"


void led_callback( u8_t ledno, u8_t hl )
{
    
    dout( "%s: time: %d, ledno: %d, hl: %s\n", 
            __func__, ptime(), ledno, (LV_H == hl )?"H LEVEL": "L LEVEL" );

    return;
}

int main( void )
{
    u8_t i  = 0;
    u8_t flag = 0;
    struct led e;
    struct loud u;
    
    loud_init( &u );
    
    loud_con( &u, UT_COM, UE_TIP );
//    loud_con( &u, UT_COM, UE_COM );
//    loud_con( &u, UT_COM, UE_FROZEN );
//    loud_con( &u, UT_COM, UE_TIP_CLOSE );
//    loud_con( &u, UT_COM, UE_CLOSE );
//    loud_con( &u, UT_COM, UE_FROZEN_CLOSE );

    loud_con( &u, UT_OLSN, UE_LSN );
//    loud_con( &u, UT_OLSN, UE_CLOSE );

    loud_con( &u, UT_PA, UE_PA );
//    loud_con( &u, UT_PA, UE_CLOSE );

    loud_con( &u, UT_BLSN, UE_LSN );
#if 0
    loud_con( &u, UT_BLSN, UE_CLOSE );
#endif    
    dump_loud( &u );


#if 0    
    led_init( &e, LN_PCOM, led_callback, 10 );

    for( i = 0; i < 12; i++ )
        led_con( &e, LE_BLINK ); 

    led_con( &e, LE_ON );

    while( 1 )
    {
        led_check( &e );
        dump_led( &e );
        sleep( 5 );
        if( 0 == flag )
        {
            flag = 1;
            led_con( &e, LE_ON_OFF );
        }
    }
#endif
    return 0;
}
