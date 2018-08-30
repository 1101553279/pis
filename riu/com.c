#include "com.h"
#include <string.h>
#include "debug.h"

#define CACHE_SIZE  11

static un_t com_cache[ CACHE_SIZE ];

void com_init( com_t *com )
{
    if( 0 == com )
        return;
   
    memset( com_cache, 0, sizeof(com_cache) ); 

    com->front = 0;
    com->rear = 0;
    com->frame = 0;
    com->size = CACHE_SIZE;
    com->cache = com_cache;

    return;
}

u16_t com_isempty( com_t *com )
{
    return ( com->front == com->rear );
}

u16_t com_isfull( com_t *com )
{
    return ( (com->front+1) % com->size ) == com->rear; 
}

s8_t com_push( com_t *com, un_t da )
{
    if( 0 == com )
       return -1; 
    
    if( com_isfull( com ) )
        return -2;

    com->cache[ com->front ] = da; 
    com->front = (com->front+1) % com->size;

    /* add frame detection */

    return 0;
}

s8_t com_pop( com_t *com, un_t *da )
{
    if( 0 == com )
        return -1;

    if( com_isempty(com ) )
        return -2;

    *da = com->cache[ com->rear ];
    com->rear = (com->rear+1) % com->size;

    return 0;
}

s8_t com_pop_frame( com_t *com, un_t *buff, u16_t *len )
{
    return 0;
}

u8_t com_type( un_t da )
{
    
    return 0;
}

/* get frame's number */
u16_t com_frame( com_t *com )
{   
    if( 0 == com )
        return 0;

   return com->frame;
}

/* get cache size */
u16_t com_size( com_t *com )
{
    if( 0 == com )
        return 0;

    return com->size-1;
}

/* get used space */
u16_t com_used( com_t *com )
{
    if( 0 == com )
        return 0;

    return ( com->front - com->rear + com->size ) % com->size;
}

/* get idle space */
u16_t com_space( com_t *com )
{
    if( 0 == com )
        return 0;
    
    if( com_isempty(com) )
        return com->size - 1 ; 
    else
        return ( com->rear + com->size - com->front ) % com->size - 1;
}

/* decide index place whether is used */
u8_t com_in( com_t *com, u16_t index )
{
    if( 0 == com )
        return 0;

    if( com->front >= com->rear )
        return ( index >= com->rear && index < com->front ) ? 1: 0;
    else
        return ( index >= com->front && index < com->rear ) ? 0: 1; 
}

/* for debug */
void com_print( com_t *com )
{
    u16_t size;
    u16_t i;

    if( 0 == com )
        return ;
    
    out( "summary: size = %u, used = %u, space = %u, full = %s, empty = %s, ",
                    com_size(com), com_used(com), com_space(com), 
                    ( 0 == com_isfull(com)) ? "no": "yes",
                    ( 0 == com_isempty(com) ? "no": "yes") );
    out( "rear = %u, front = %u\n", com->rear, com->front ); 
   
    
    for( i=0; i < com->size; i++ )
        out( "%02d ", i );
    out( "\n" );

    for( i=0; i < com->size; i++ )
    {
        out( "%-2c ", ( 1 ==  com_in( com, i ) ) ? '*': '-');
#if 0
        if( com_in( com, i ) )
            out( "* " );
        else
            out( "- " );
#endif
    }

    out( "\n\n" );

    return;
}


