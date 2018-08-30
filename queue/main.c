#include <stdio.h>
#include "led.h"
#include "com.h"
#include "frame.h"
#include "debug.h"




u8_t frame_match( un_t master, un_t slave )
{
    return master == slave;
}

int main( void )
{
    int i = 0;  
    un_t da;
    s8_t ret= 0;
    s32_t index;
    com_t com;
    frame_t frame; 
    frame_t query; 
    un_t buff[ 100 ]; 
    u16_t len;
    un_t store[ ] = { 
                    0x02,0x42,0x35,0x03,0x38,0x3a,
                    0x02,0x42,0x37,0x03,0x38,0x38,
#if 0
                    0x02,0x2,0x42,0x37,0x38,0x38,
                    0x02,0x2,0x42,0x37,0x39,0x39,
                    0x02,0x41,0x36,0x03,0x38,0x39,
                    0x02,0x42,0x36,0x03,0x38,0x39,
                    0x02,0x41,0x31,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x33,0x32,0x03,0x30,0x30, 
                    0x02,0x41,0x32,0x03,0x38,0x3d,
                    0x02,0x41,0x38,0x30,0x30,0x30,0x30,0x03,0x30,0x30,
#endif
                    };
#if 1 
    out( ">> frame init & put two frames \n" );
    ret = frame_init( &frame, 0x02, 0x03, 0x41, 0 );
    if( 0 != ret )
        out( "frame_init fault!\n" );

    frame_match_set( &frame, frame_match );

    for( i= 0; i < sizeof(store); i++ )
    {
        ret = frame_put_one( &frame, store[i] );
        if( 0 != ret )
            out( "frame put one fail!: i = %d, data = %#x\n", i, store[i] );
    }
    
    frame_print( &frame ); 
    
//    ret = frame_frame( &frame ); 
//    out( "frame frame = %d\n", ret );

    for( i = 0; i < 2; i++ )
    {
        len = frame_get( &frame, buff, sizeof(buff) ); 
//        len = frame_match_get( &frame, buff, sizeof(buff) ); 
        if( 0 != len )
        {
            out( "frame get success!: i = %d, len = %d : ", i, len );
                debug_dump( buff, len, print_hex ); 
        }
        else
            out( "frame get fail!: i = %d, len = %d \n", i, len );
    }

//    ret = frame_frame( &frame ); 
//    out( "frame frame = %d\n", ret );

    out( "\n" );
    frame_print( &frame );

    out( ">> frame query !\n" );
    frame_query( &frame, &query );
    frame_print( &query );
    out( "\n" );

#endif

#if 0
    com_init( &com );
#if 0
    com_push( &com, 0 );

    if( com_in( &com, 0 ) )
        out( "0 in com \n" );
    else
        out( "0 not in com \n" );
#endif 
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
        else
            out( "%d pop %d\n", i, da );
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

    out( ">>. com pop 5 units!\n" );
    for( i = 0; i < 5; i++ )
    {
        ret = com_pop( &com, &da );
        if( 0 != ret )
            out( "pop error: ret = %d, i = %d\n", ret, i );
        else
            out( "%d pop %d\n", i, da );
    }
   
    com_print( &com ); 
    out( ">>. com push rear 6 unit\n" );
    for( i = 0; i < 6; i++ )
    {
        ret = com_push_rear( &com, i );
        if( 0 != ret )
            out( "push error: ret = %d, i = %d\n", ret , i );
    }
    com_print( &com );
   
    out( ">> com reset !\n" ); 
    com_reset( &com );
    com_print( &com);

    out( ">> com push rear 6 unit !\n" );
    for( i = 0; i < 6; i++ )
    {
        ret = com_push_rear( &com, i );
        if( 0 != ret )
            out( "push error: ret = %d, i = %d\n", ret , i );
    }
    com_print( &com );
    
    out( ">> com find !\n" );
    for( i = 0; i < 7; i++ )
    {
        index = com_find( &com, i ); 
        if( index >= 0 )
            out( "find %d, find %d\n", i, index);
        else 
            out( "find %d, no find!\n", i );
    }


#endif

#if 0
//   printf( " LED_RUN = %d\n", LED_RUN );        // output 0

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
