#include "out.h"
#include "ptime.h"
#include "debug.h"

/* init a out_t : no / mode / timeout */
void out_init( out_t *out, u8_t no, u8_t mode, u32_t timeout, out_timeout_t hand )
{
    if( 0 == out )
        return;

    if( MODE_PERIOD != mode && MODE_SINGLE != mode  )   //prarmeter invalid
        return;

    out->no = no;
    out->active = ( MODE_PERIOD == mode )? OUT_ACTIVE: OUT_NOACT;
    out->mode = mode; 
    out->stime = ptime_get(); 
    out->timeout = timeout;
    out->hand = hand;
    
    return;
}

/* config out parameter */
s8_t out_config( out_t *out, u8_t type, u32_t data )
{
    s8_t ret = 0;

    if( 0 == out )
        return -1;
    
   if( type < CON_ACTIVE || type >= MAX_CON )
       return -2;

    switch( type )
    {
        case CON_ACTIVE:
            if( OUT_ACTIVE == data || OUT_NOACT == data )
                out->active = data;
            else
            {
                ret = -3;
                goto err;
            }

            break;

        case CON_MODE:
            if( MODE_SINGLE == data || MODE_PERIOD == data )
                out->mode = data;
            else
            {
                ret = -3;
                goto err;
            }
            break;

        case CON_FLAG:
            if( FLAG_ON == data || FLAG_OFF == data )
                out->flag = data;
            else
            {
                ret = -3;
                goto err;
            }
            break;

        case CON_STIME:
            out->stime = data;
            break;

        case CON_TIMEOUT:
            out->timeout = data;
            break;

        case CON_HANDLE:
            out->hand = ( out_timeout_t *)data;
            break;

        default:
            break;
    }
    
    return 0;

err: 
    return ret;
}

/* active a out */
void out_start( out_t *out )
{
    if( 0 == out )
        return;
    
    if( out->mode == MODE_SINGLE )
        out->flag = FLAG_ON;

    out->stime = ptime_get();
    out->active = OUT_ACTIVE;
    
    return;
}

/* disbale a out */
void out_stop( out_t *out )
{
    if( 0 == out )
        return;

    out->active = OUT_NOACT;

    return;
}

/* check out checkout */
void out_check( out_t *out )
{
    if( 0 == out || OUT_NOACT == out->active )
        return ;
    
    if( ptime_get() < out->stime + out->timeout )// no timeout
        return;

    if( MODE_SINGLE == out->mode )
    {
        out->hand( out->no, !out->flag );        // execute handler
        out_stop( out );                         // one shot
    }
    else if( MODE_PERIOD == out->mode )
    {
        out->flag = ( FLAG_ON == out->flag ) ? FLAG_OFF: FLAG_ON ;
        out->stime = ptime_get();
        out->hand( out->no, out->flag );
    }

    return;
}
     

/* output out info */
void out_print( out_t *outs, u8_t no )
{
    u8_t i = 0;
    if( 0 == outs )
        return;

#if 0 
    out( "no : %d, active: %s, mode: %s, flag: %s, stime: %u, timeout: %u, aimtime: %u\n",
         out->no, 
         (out->active == OUT_ACTIVE) ?"active": "noact", 
         (out->mode == MODE_SINGLE) ? "single": (out->mode==MODE_PERIOD)? "period":"none",
         (out->flag == FLAG_ON ) ? "flag_on": "flag_off",
         out->stime,
         out->timeout,
         out->stime + out->timeout );
    for( i = 0; i < no; i++ )
    {
        out( "no        : %d\n", outs[i].no);
        out( "active    : %s\n", (outs[i].active == OUT_ACTIVE) ?"active": "noact" );
        out( "mode      : %s\n", (outs[i].mode == MODE_SINGLE) ? "single": (outs[i].mode==MODE_PERIOD)? "period":"none" );
        out( "flag      : %s\n", (outs[i].flag == FLAG_ON ) ? "flag_on": "flag_off" );
        out( "stime     : %u\n", outs[i].stime );
        out( "timeout   : %u\n", outs[i].timeout );
        out( "atime     : %u\n", outs[i].stime+ outs[i].timeout );
        out( "hand      : %x\n", outs[i].hand );
        out( "\n" );
    }
#endif
    out( "%-5s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n",
            "no", "active", "mode", "flag", "stime", "timeout", "atime", "curtime", "hand" );

    for( i = 0; i < no; i++ )
    {
        out( "%-5d %-10s %-10s %-10s %-10u %-10u %-10u %-10u %-10x\n",
            outs[i].no,
            (outs[i].active == OUT_ACTIVE) ?"active": "noact",
            (outs[i].mode == MODE_SINGLE) ? "single": (outs[i].mode==MODE_PERIOD)? "period":"none",
            (outs[i].flag == FLAG_ON ) ? "on": "off",
            outs[i].stime,
            outs[i].timeout,
            outs[i].stime+ outs[i].timeout,
            ptime_get( ),
            outs[i].hand );
    }
    out("\n" );

    return;
}
