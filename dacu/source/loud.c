#include "loud.h"
#include "debug.h"

static void com_con( struct loud *loud, u8_t op )
{
    if( 0 == loud )
        return;
    
    if( UE_TIP != op &&     // parameter check
        UE_COM != op &&
        UE_FROZEN != op &&
        UE_TIP_CLOSE != op &&
        UE_FROZEN_CLOSE != op &&
        UE_CLOSE != op )
        return;
    
    switch( op )
    {
        case UE_TIP:
            loud->com_tip++;
            break;

        case UE_COM:
            if( loud->com_tip > 0 )
                loud->com_tip--;
            loud->com++;
            break;

        case UE_FROZEN:
            if( loud->com_tip > 0 )
                loud->com_tip--;
            loud->frozen++;
            break;

        case UE_TIP_CLOSE:
            if( loud->com_tip > 0 )
                loud->com_tip--;
            break;
        
        case UE_FROZEN_CLOSE:
            if( loud->frozen > 0 )
                loud->frozen--;
            break;

        case UE_CLOSE:
            if( loud->com > 0 )
                loud->com--;
            break;

        default:
            break;
    }


//    dout( "%s: callback!, op = %d\n", __func__, op );

    return;
}

static void olsn_con( struct loud *loud, u8_t op )
{
    if( 0 == loud )
        return;

    if( UE_LSN != op &&     // parameter check
        UE_CLOSE != op )
        return;
    
    switch( op )
    {
        case UE_LSN:
            loud->olisten++;
            break;

        case UE_CLOSE:
            if( loud->olisten > 0 )
                loud->olisten--;
            break;

        default:
            break;
    }


//    dout( "%s: callback!, op = %d\n", __func__, op );

    return;
}

static void pa_con( struct loud *loud, u8_t op )
{
    if( 0 == loud )
        return;

    if( UE_PA != op &&     // parameter check
        UE_CLOSE != op )
        return;
    
    switch( op )
    {
        case UE_PA:
            loud->pa++;
            break;

        case UE_CLOSE:
            if( loud->pa > 0 )
                loud->pa--;
            break;

        default:
            break;
    }
    
//    dout( "%s: callback!, op = %d\n", __func__, op );

    return;
}

static void blsn_con( struct loud *loud, u8_t op )
{
    if( 0 == loud )
        return; 
    
    if( UE_LSN != op &&     // parameter check
        UE_CLOSE != op )
        return;
    
    switch( op )
    {
        case UE_LSN:
            loud->blisten++;
            break;

        case UE_CLOSE:
            if( loud->blisten > 0 )
                loud->blisten--;
            break;

        default:
            break;
    }
    
//    dout( "%s: callback!, op = %d\n", __func__, op );

    return;
}


/* first call this function before using loud_con */
void loud_init( struct loud *loud )
{
    if( 0 == loud )
        return;
    
    loud->frozen = 0;
    loud->com = 0;
    loud->olisten = 0;
    loud->com_tip = 0;
    loud->pa = 0;
    loud->blisten = 0;

    loud->cmdcb[ UT_COM ] = com_con;
    loud->cmdcb[ UT_OLSN ] = olsn_con;
    loud->cmdcb[ UT_PA ] = pa_con;
    loud->cmdcb[ UT_BLSN ] = blsn_con;

    return;
}

/* operate loud */
void loud_con( struct loud *loud, u8_t type, u8_t op )
{
    if( 0 == loud )
        return;

    if( type < MAX_UT && 0 != loud->cmdcb[type] ) 
        loud->cmdcb[type]( loud, op );

    //call pca loud set

    return;
}

// only for debug
void dump_loud( struct loud *loud )
{
    if( 0 == loud )
        return;
    
    dout( "%-10s %-5s %-10s %-10s %-5s %-10s\n",
            "frozen", "com", "olisten", "com_tip", "pa",  "blisten" );

    dout( "%-10d %-5d %-10d %-10d %-5d %-10d\n",
            loud->frozen, loud->com, loud->olisten, loud->com_tip,
            loud->pa, loud->blisten );

    dout( "\n" );

    return;
}
