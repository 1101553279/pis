#include <stdio.h>
#include "led.h"
#include "loud.h"
#include "debug.h"
#include "chip_pca.h"
#include "net_event.h"


void led_callback( u8_t type, u8_t id, u8_t no_hl )
{
    
    dout( "%s: time: %d, ledno: %d, hl: %s\n", 
            __func__, ptime(), id, (LV_H == (no_hl & 0xff) )?"H LEVEL": "L LEVEL" );

    return;
}

u8_t net_data[ 10 ][ 21 ] = { 
                            {
                                0xff, 0x01, 0xf9, 0x1, 0x1, 0x01, 0x01, 0x33,
                                0x08, 0x32, 0x31, 0x31, 0x31, 0x31, 0x31, 0x30,
                                0x31, 0x31, 0x31, 0x31, 0xfe
                            }
                            ,
                            {
                                0xff, 0x30, 0x04, 0x31, 0x30, 0x01, 0x30, 0x35, 
                                0x31, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x31, 0xfe
//                              0xff 0x30   0x4   0x31  0x30  0x1   0x30  0x35
//                              0x31 0xaa   0     0     0     0     0x31  0xfe 
                            }
                            ,   // 2
                            {   // dacu request start
                                0xff, 0x30, 0xf7, 0x31, 0x30, 0x4, 0x31, 0x36, 
                                0x31, 0x31, 0x00, 0x00, 0x00, 0x00, 0x31, 0xfe
                            },  // 3
                            {   // dacu request stop
                                0xff, 0x30, 0xf7, 0x31, 0x30, 0x4, 0x31, 0x36, 
                                0x31, 0x32, 0x00, 0x00, 0x00, 0x00, 0x31, 0xfe
                            },  // 4
                            {   // pecu request start
                                0xff, 0x30, 0xf7, 0x31, 0x30, 0x5, 0x31, 0x36, 
                                0x31, 0x31, 0x00, 0x00, 0x00, 0x00, 0x31, 0xfe
                            },  // 5
                            {   // pecu request stop
                                0xff, 0x30, 0xf7, 0x31, 0x30, 0x5, 0x31, 0x36, 
                                0x31, 0x32, 0x00, 0x00, 0x00, 0x00, 0x31, 0xfe
                            },
                                // 6
                            {   // dacu respone start
                                0xff, 0x30, 0x04, 0x31, 0x30, 0x4, 0x31, 0x37,
                                0x31, 0x31, 0x00, 0x00, 0x00, 0x00, 0x31, 0xfe
                            },  // 7
                            {   // dacu response stop
                                0xff, 0x30, 0x04, 0x31, 0x30, 0x4, 0x31, 0x37,
                                0x31, 0x32, 0x00, 0x00, 0x00, 0x00, 0x31, 0xfe
                            },  // 8
                            {   // pecu response start
                                0xff, 0x30, 0x05, 0x31, 0x30, 0x5, 0x31, 0x37,
                                0x31, 0x31, 0x00, 0x00, 0x00, 0x00, 0x31, 0xfe
                            },  // 9 
                            {   // pecu response stop
                                0xff, 0x30, 0x05, 0x31, 0x30, 0x5, 0x31, 0x37,
                                0x31, 0x32, 0x00, 0x00, 0x00, 0x00, 0x31, 0xfe
                            },
                            
                            };
                                    
int main( void )
{
    u8_t i  = 0;
    u8_t flag = 0;
    struct led e;
    struct loud u;
    struct chip_event en;
    s8_t ret = -1;
    u16_t value = 0;
    struct net_info ei;

    pca_init( );

#if 1
    net_event_init( );
   
    net_event_parse( net_data[0], 21 );
    net_event_parse( net_data[1], 16 );
    net_event_parse( net_data[2], 16 ); // dacu request start
//    net_event_parse( net_data[3], 16 ); // dacu request stop
//    net_event_parse( net_data[4], 16 ); // pecu request start
//    net_event_parse( net_data[5], 16 ); // pecu request stop
//    net_event_parse( net_data[6], 16 ); // dacu response start
//    net_event_parse( net_data[7], 16 ); // dacu response stop
//    net_event_parse( net_data[8], 16 ); // pecu response start
//    net_event_parse( net_data[9], 16 ); // pecu response stop
    
    ret = net_event( &ei );
    ret = net_event( &ei );
    ret = net_event( &ei );
    if( 0 == ret )
    {
        dout( "type = %d, id = %d, ", ei.type, ei.id );
        if( NET_IN_ID_OCC == ei.id )
            dout( "occ: %s", (OCC_ON == ei.data.occ)?"on":"off" );
        else if( NET_IN_ID_UIC_IDLE == ei.id )
            dout( "%s: %s", "uic", "idle" );
        else if( NET_IN_ID_CAB_LINK == ei.id )
            dout( "%s: %s", "link", "none" );
        else if( NET_IN_ID_COM == ei.id )
        {
            dout( "%s: cmd = %#x, op = %#x, rd_cab = %#x, rd_type = %#x, rd_no = %#x, sd_cab = %#x, sd_dev = %#x, sd_no = %#x, dc_no = %#x",
                  "com",
                  ei.data.com.cmd,
                  ei.data.com.op,
                  ei.data.com.rd_cab,
                  ei.data.com.rd_type,
                  ei.data.com.rd_no,
                  ei.data.com.sd_cab,
                  ei.data.com.sd_dev,
                  ei.data.com.sd_no,
                  ei.data.com.dc_no
               );
        }

        dout( "\n" );
    }
    else
        dout( "no net event\n" );
/*
enum net_in_id{
    NET_IN_ID_OCC = 0,      // listen
    NET_IN_ID_UIC_IDLE,     // pa
    NET_IN_ID_CAB_LINK,     // cab link info
    NET_IN_ID_COM,          // dacu pecu com
    MAX_NET_IN_ID,
};
*/
    dump_net_event();
#endif

#if 0
    pca_rflag( 0 );
    pca_rflag( 1 );
    pca_rflag( 2 );

    ret = pca_event( &en );
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
#endif



#if 0
    ret = pca_check( PCA_ID_CHECK_IP, &value );
    dout( "ret = %d, value = %#x\n", ret, value );
#endif

#if 0
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
    
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
    
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );

    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
//     ip
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );

// but && ppt 
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
    
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
    
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
    
    
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );

    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
    
    
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );

    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
    
    all_pca[ 0 ].cur = 0;
    all_pca[ 1 ].cur = 0;
    all_pca[ 2 ].cur = 0;
    
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );

    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
    
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );

    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );

//     ip
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );

// but && ppt 
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
    
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
    
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
    
    
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );

    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );
    
    
    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );

    ret = pca_event( &en ); 
    dout( "ret = %d, en.type= %d, en.id = %d, en.value = %#x\n",
            ret, en.type, en.id, en.value );

    dump_pca();
#endif    

#if 0

//    pca_out( PCA_ID_OUT_MIC, MIC_OPEN );
//    pca_out( PCA_ID_OUT_LOUD, LOUD_TIP );
//    pca_out( PCA_ID_OUT_LOUD, LOUD_COM );
//    pca_out( PCA_ID_OUT_LOUD, LOUD_LSN );
//    pca_out( PCA_ID_OUT_LOUD, LOUD_PA );
//    pca_out( PCA_ID_OUT_UIC78, OUT_UIC78_H );
//    pca_out( PCA_ID_OUT_UIC56, OUT_UIC56_H );
//    pca_out( PCA_ID_OUT_VOLUME, VOLUME_3);
//    pca_out( PCA_ID_OUT_VOLUME, VOLUME_2);
//    pca_out( PCA_ID_OUT_VOLUME, VOLUME_1);
//    pca_out( PCA_ID_OUT_LED, LED_PA_IN_H );
//    pca_out( PCA_ID_OUT_LED, LED_PCOM_H );
//    pca_out( PCA_ID_OUT_LED, LED_DCOM_H );
//    pca_out( PCA_ID_OUT_LED, LED_PA_OUT_H );



//    pca_out( PCA_ID_OUT_LED, LED_PA_OUT_L );
//    pca_out( PCA_ID_OUT_LED, LED_DCOM_L );
//    pca_out( PCA_ID_OUT_LED, LED_PCOM_L );
//    pca_out( PCA_ID_OUT_LED, LED_PA_IN_L );
//    pca_out( PCA_ID_OUT_VOLUME, VOLUME_0);
//    pca_out( PCA_ID_OUT_UIC56, OUT_UIC56_L );
//    pca_out( PCA_ID_OUT_UIC78, OUT_UIC78_L );
//    pca_out( PCA_ID_OUT_MIC, MIC_CLOSE );
//    pca_out( PCA_ID_OUT_LOUD, LOUD_CLOSE );


    dump_pca();
#endif 

#if 0
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
    loud_con( &u, UT_BLSN, UE_CLOSE );
    dump_loud( &u );
#endif    

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
