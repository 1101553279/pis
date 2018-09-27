#include <stdio.h>
#include "led.h"
#include "loud.h"
#include "debug.h"
#include "chip_pca.h"


void led_callback( u8_t type, u8_t id, u8_t no_hl )
{
    
    dout( "%s: time: %d, ledno: %d, hl: %s\n", 
            __func__, ptime(), id, (LV_H == (no_hl & 0xff) )?"H LEVEL": "L LEVEL" );

    return;
}

int main( void )
{
    u8_t i  = 0;
    u8_t flag = 0;
    struct led e;
    struct loud u;
    struct chip_event en;
    s8_t ret = 0;
    u16_t value = 0;

    pca_init( );

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
#endif    

    dump_pca();

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
