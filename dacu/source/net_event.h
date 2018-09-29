#ifndef __NET_EVENT_H__
#define __NET_EVENT_H__

#include "btype.h"


#define TYPE_NET_IN     0x2

enum net_in_id{
    NET_IN_ID_OCC = 0,      // listen
    NET_IN_ID_UIC_IDLE,     // pa
    NET_IN_ID_CAB_LINK,     // cab link info
    NET_IN_ID_COM,          // dacu pecu com
    MAX_NET_IN_ID,
};

enum{
    OCC_OFF = 0,
    OCC_ON,
};

enum{
    COM_REQ = 1,    //1
    COM_REQ_START,  //2
    COM_REQ_STOP,   //3
    COM_REP,        //4
    COM_REP_START,  //5
    COM_REP_STOP,   //6
};

union net_union{
    u8_t occ;
    u8_t cablink;
    struct com{
        u8_t cmd;       // request or response
        u8_t op;        // operation
                        // request start    -> pecu / dacu send
                        // request stop     -> pecu / dacu send
                        // response start   -> dacu send
                        // response stop    -> dacu send
        u8_t rd_cab;    // receive device cab
        u8_t rd_type;   // receive device type
        u8_t rd_no;     // receive device number
        u8_t sd_cab;    // send device cab
        u8_t sd_dev;    // send device type
        u8_t sd_no;     // send device number
        u8_t dc_no;     // device channel number
    }com;
};

typedef struct event_info{
    u8_t update;
    u8_t type;      // TYPE_NET_IN
    u8_t id;        // 
    union net_union data;
}net_event_t;

void net_event_init( void );
void net_event_parse( u8_t *buff, u8_t len );
s8_t net_event( struct event_info *e );

//only for debug
void dump_net_event( );

#endif
