#include "com.h"
#include <string.h>
#include "debug.h"

#define CACHE_SIZE  31 

static un_t com_cache[ CACHE_SIZE ];

void *com_cache_alloc( u16_t size )
{
    return com_cache;
}

s8_t com_init( com_t *com )
{
    void *cache;

    if( 0 == com )
        return -1;
   
    memset( com_cache, 0, sizeof(com_cache) ); 

    com->front = 0;
    com->rear = 0;
//    com->frame = 0;
    cache = com_cache_alloc( CACHE_SIZE );
    if( 0 != cache )
    {
        com->size = CACHE_SIZE;
        com->cache = com_cache;
    }
    else
    {
        com->size = 0;
        com->cache = 0;
        return -2;
    }

    return 0;
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
    if( 0 == com || 0 == da )
        return -1;

    if( com_isempty(com ) )
        return -2;

    *da = com->cache[ com->rear ];
    com->cache[ com->rear ] = 0;        // clear unit
    com->rear = (com->rear+1) % com->size;

    return 0;
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
   
#if 1    
    for( i=0; i < com->size; i++ )
        out( "%02d ", i );
    out( "\n" );
#endif
    for( i=0; i < com->size; i++ )
    {
        out( "%-2c ", ( 1 ==  com_in( com, i ) ) ? '*': '-');
        if((i+1) % 30 == 0 )       // 30 unit put newline
            out("\n" );
#if 0
        if( com_in( com, i ) )
            out( "* " );
        else
            out( "- " );
#endif
    }

#if 1    
    out( "\n" );
    for( i=0; i < com->size; i++ )
        out( "%02x ", com->cache[i] );
    out( "\n" );
#endif

    out( "\n\n" );

    return;
}


