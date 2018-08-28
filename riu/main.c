#include <stdio.h>
#include "led.h"

int main( void )
{
//   printf( " LED_RUN = %d\n", LED_RUN );        // output 0
    int i = 0;

//init
    led_init( );


//for test
    for( i = 0; i < 2; i++ )
    {
        led_con( i, LED_ON );
        led_con( i, LED_OFF );
        printf( "-------------\r\n" );
        
    }
    
    return 0;
}
