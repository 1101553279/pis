#include "net.h"



struct net_udppcb{
    // u8_t type;
    // local pcb
    // local port
    // remote pcb
    // remote port
    // callback
};

void net_init( )
{
    // test pcb init
    // multicast init

    return;
}


s8_t net_send( u8_t *buff, u16_t len, u8_t type )
{
    if(0 == buff || 0 == len  )
        return -1;

    if( type < NET_TYPE_MULTI || type >= MAX_NET_TYPE )
        return -1;

    
// get memory 
// cpy buff
// select destination according to type
// send to 
// put memory



    return 0;
}
