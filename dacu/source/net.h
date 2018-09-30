#ifndef __NET_H__
#define __NET_H__

#include "btype.h"


enum net_type{
    NET_TYPE_MULTI,     // udp
    MAX_NET_TYPE,
};



void net_init( );
s8_t net_send( u8_t *buff, u16_t len, u8_t type );

#endif

