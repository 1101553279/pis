#include <stdio.h>
#include "led.h"
#include "com.h"
#include "debug.h"

int main( void )
{
    int i = 0;  
    un_t da;
    s8_t ret= 0;
    com_t com;
    
    com_init( &com );
#if 0
    com_push( &com, 0 );

    if( com_in( &com, 0 ) )
        out( "0 in com \n" );
    else
        out( "0 not in com \n" );
#endif
    
#if 1
    out( ">>. com start\n" );
    com_print( &com );

    out( ">>. com push 11 units\n" );
    for( i = 0; i < 11; i++ )
    {
        ret = com_push( &com, i );
        if( 0 != ret )
           out( "push error: ret = %d, i = %d\n", ret, i ); 
    }

    com_print( &com );
    
    out( ">>. com pop 4 units\n" );
    for( i = 0; i < 4; i++ )
    {
        ret = com_pop( &com, &da );
        if( 0 != ret )
            out( "pop error: ret = %d, i = %d\n", ret, i );
    }

    com_print( &com );

    out( ">>. com push 3 units\n" );
    for( i = 0; i < 3; i++ )
    {
        ret = com_push( &com, i );
        if( 0 != ret )
           out( "push error: ret = %d, i = %d\n", ret, i ); 
    }

    com_print( &com);
    
    out( ">>. com push 1 unit\n" );
    ret = com_push( &com, i );
    if( 0 != ret )
        out( "push error: ret = %d, i = %d\n", ret , i );
    
    com_print( &com );
   
    out( ">>. com push 1 unit\n" );
    i++; 
    ret = com_push( &com, i );
    if( 0 != ret )
        out( "push error: ret = %d, i = %d\n", ret , i );
    com_print( &com );

#endif

#if 0
//   printf( " LED_RUN = %d\n", LED_RUN );        // output 0
    int i = 0;

//init
    led_init( );


//for test
    for( i = 0; i < 2; i++ )
    {
        led_con( i, LED_ON );
        led_con( i, LED_OFF );
        dbg_out( "-------------\r\n" );
        
    }

    led_con_all( LED_ON );
    led_con_all( LED_OFF );
#endif 
    return 0;
}
