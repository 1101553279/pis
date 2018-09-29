#include "net_event.h"
#include "ptime.h"
#include "debug.h"

#define NET_EVENT_TIMEOUT   10000
#define MAX_NET_CACHE   50

enum dev_no{
    DEV_NO_DACU = 0x04,
    DEV_NO_PECU = 0x05,
};

enum cmd_no{
    CMD_OCC = 0x33,
    CMD_UIC_IDLE = 0x35,
    CMD_REQ = 0x36,
    CMD_REP = 0x37,
};

enum net_no{
    NET_NO_OCC = 0,
    NET_NO_UIC_IDLE,
    NET_NO_CAB_LINK,
    NET_NO_PECU_REQ,
    NET_NO_DACU_REQ,
    NET_NO_TO_PECU,
    NET_NO_TO_DACU,
    MAX_NET_NO,
};


char *cache_info_spec[ MAX_NET_NO ] = { "occ",
                                        "uic idle",
                                        "cab link", 
                                        "pecu req",
                                        "dacu req", 
                                        "to pecu",
                                        "to dacu" };
struct cache_info{
    char *spec;
    u8_t no;            // no
    u32_t time;         // last time since updating
    u8_t len;           // data length
    u8_t data[ MAX_NET_CACHE ];
};

struct cache_info cache_info[ MAX_NET_NO ];
struct net_info net_info[ MAX_NET_IN_ID ];

void net_event_init( )
{
    u8_t i;

    for( i = NET_NO_OCC; i < MAX_NET_NO; i++ )
    {
        cache_info[i].spec = cache_info_spec[i];
        cache_info[i].no = i;
        cache_info[i].time = 0;
        cache_info[i].len = 0;
        memset( cache_info[i].data, 0, MAX_NET_CACHE );
    }

    for( i = NET_IN_ID_OCC; i < MAX_NET_IN_ID; i++ )
    {
        memset( &net_info[i], 0, sizeof(struct net_info) );
        net_info[ i ].type = TYPE_NET_IN;
        net_info[ i ].id = i;
    } 

    return;
}

static s8_t info_index( u8_t cmd, u8_t dev )
{
    s8_t ret = 0;

    switch( cmd )
    {
        case CMD_OCC:
            ret = NET_NO_OCC;
            break;

        case CMD_UIC_IDLE:
            ret = NET_NO_UIC_IDLE;
            break;

        case CMD_REQ:
            ret = ( DEV_NO_PECU == dev ) ? NET_NO_PECU_REQ: NET_NO_DACU_REQ;
            break;

        case CMD_REP:
            ret = ( DEV_NO_PECU == dev )? NET_NO_TO_PECU: NET_NO_TO_DACU;
            break;

        default:
            ret = -1;
            break;
    }

    return ret;
}

static void info_parse( struct cache_info *info, u8_t *buff, u8_t len )
{
    u8_t flag = 0;

    if( NET_NO_UIC_IDLE != info->no )
    {
        if( 0 == memcmp(info->data, buff, len ) )       // all same data
            return;
        
        switch( info->no )
        {
            case NET_NO_OCC:
                if( info->data[15] != buff[15] )
                {
                    net_info[NET_IN_ID_OCC].data.occ = (0x30 == buff[15]) ? OCC_OFF: OCC_ON;

                    net_info[NET_IN_ID_OCC].update = 1;

                    flag = 1;
                }
                break;

            case NET_NO_CAB_LINK:
                break;

            case NET_NO_PECU_REQ:       //same operation
            case NET_NO_DACU_REQ:
                if( info->data[6] != buff[6] || info->data[9] != buff[9] )
                {
                    net_info[NET_IN_ID_COM].data.com.cmd = COM_REQ;
                    net_info[NET_IN_ID_COM].data.com.op = (0x31 == buff[9]) ? COM_REQ_START: COM_REQ_STOP;
                    net_info[NET_IN_ID_COM].data.com.rd_cab = buff[1];
                    net_info[NET_IN_ID_COM].data.com.rd_type = buff[2];
                    net_info[NET_IN_ID_COM].data.com.rd_no = buff[3];
                    net_info[NET_IN_ID_COM].data.com.sd_cab = buff[4];
                    net_info[NET_IN_ID_COM].data.com.sd_dev = buff[5];
                    net_info[NET_IN_ID_COM].data.com.sd_no = buff[6];
                    net_info[NET_IN_ID_COM].data.com.dc_no = buff[10];
                    
                    net_info[NET_IN_ID_COM].update = 1;

                    flag = 1;
                }
                break;

            case NET_NO_TO_PECU:
            case NET_NO_TO_DACU:
                if( info->data[3] != buff[3] || info->data[9] != buff[9] )
                {
                    net_info[NET_IN_ID_COM].data.com.cmd = COM_REP;
                    net_info[NET_IN_ID_COM].data.com.op = (0x31 == buff[9]) ? COM_REP_START: COM_REP_STOP;
                    net_info[NET_IN_ID_COM].data.com.rd_cab = buff[1];
                    net_info[NET_IN_ID_COM].data.com.rd_type = buff[2];
                    net_info[NET_IN_ID_COM].data.com.rd_no = buff[3];
                    net_info[NET_IN_ID_COM].data.com.sd_cab = buff[4];
                    net_info[NET_IN_ID_COM].data.com.sd_dev = buff[5];
                    net_info[NET_IN_ID_COM].data.com.sd_no = buff[6];
                    net_info[NET_IN_ID_COM].data.com.dc_no = buff[10];


                    net_info[NET_IN_ID_COM].update = 1;

                    flag = 1;
                }
                break;

            default:
                break;
        }

        if( 1 == flag )
        {
            info->len = len;
            memcpy( info->data, buff, len );
        }
    }
    else
    {
        if( ptime() - info->time > NET_EVENT_TIMEOUT )  // uic idle timeout
        {
            info->time = ptime();
            info->len = len;
            memcpy( info->data, buff, len );

            net_info[NET_IN_ID_UIC_IDLE].update = 1;
        }
    }

    return;
}

/* parse one datagram */
void net_event_parse( u8_t *buff, u8_t len )
{
    s8_t inx = -1;
    u8_t cmd;
    u8_t dev;
    struct cache_info *info;

    if( 16 != len && 21 != len )
        return;

    if( 0 == buff )
        return;

    if( 0xff != buff[0] || 0xfe != buff[len-1] )
        return;

    cmd = buff[7];
    if( CMD_OCC != cmd && CMD_UIC_IDLE != cmd &&
        CMD_REQ != cmd && CMD_REP != cmd )
        return;
//   dout( "kk\n" ); 
    if( CMD_REQ == cmd )
        dev = buff[5];
    else if( CMD_REP == cmd )
        dev = buff[2];
    else
        dev = 0;

    inx = info_index( cmd, dev );
    if( inx < 0 )
        return;
    
    info = &cache_info[ inx ]; 
    
    info_parse( info, buff, len ); 

    return;
}

/* get one event */
s8_t net_event( struct net_info *e )
{
    u8_t i;
    
    if( 0 == e )
        return -1;

    for( i = NET_IN_ID_OCC; i < MAX_NET_IN_ID; i++ )
    {
        if( 1 == net_info[i].update )
        {
            memcpy( e, &net_info[i], sizeof(struct net_info) );

            net_info[i].update = 0;

            return 0;
        }
    }

    e->id = MAX_NET_IN_ID;

    return 1;
}

/* only for debug */
void dump_net_event( )
{
    u8_t  i;
    
    dout("net event: cache\n" );
    dout( "%-10s %-5s %-10s %-5s %-10s\n",
            "spec", "no", "time", "len", "content" );

    for( i = NET_NO_OCC; i < MAX_NET_NO; i++ )
    {
        dout( "%-10s %-5d %-10d %-5d  ",
                cache_info[i].spec,
                cache_info[i].no,
                cache_info[i].time,
                cache_info[i].len );
        debug_dump( cache_info[i].data, cache_info[i].len, print_hex );

        dout("\n" );
    }

    dout( "\n" );
    
    dout("net event: event\n" );
    dout( "%-10s %-10s %-5s\n",
            "update", "type", "id" );

    for( i = NET_IN_ID_OCC; i < MAX_NET_IN_ID; i++ )
    {
        dout( "%-10d %-10d %-5d  ",
                net_info[i].update,
                net_info[i].type,
                net_info[i].id );
        if( NET_IN_ID_OCC == i )
            dout( "occ: %s", (OCC_ON == net_info[i].data.occ)?"on":"off" );
        else if( NET_IN_ID_UIC_IDLE == i )
            dout( "%s: %s", "uic", "idle" );
        else if( NET_IN_ID_CAB_LINK == i )
            dout( "%s: %s", "link", "none" );
        else if( NET_IN_ID_COM == i )
        {
            dout( "%s: cmd = %#x, op = %#x, rd_cab = %#x, rd_type = %#x, rd_no = %#x, sd_cab = %#x, sd_dev = %#x, sd_no = %#x, dc_no = %#x",
                  "com",
                  net_info[i].data.com.cmd,
                  net_info[i].data.com.op,
                  net_info[i].data.com.rd_cab,
                  net_info[i].data.com.rd_type,
                  net_info[i].data.com.rd_no,
                  net_info[i].data.com.sd_cab,
                  net_info[i].data.com.sd_dev,
                  net_info[i].data.com.sd_no,
                  net_info[i].data.com.dc_no
               );
        }

        dout("\n" );
    }

    dout( "\n" );

    return;
}
