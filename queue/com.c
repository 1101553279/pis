#include "com.h"
#include <string.h>
#include "debug.h"

#define CACHE_SIZE  31

/* for cache alloc */
static un_t com_cache[ CACHE_SIZE ];

static void *com_cache_alloc( u16_t size )
{
    return com_cache;
}

/*  you must call this before use other functions */
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
        com->cache = cache;
    }
    else
    {
        com->size = 0;
        com->cache = 0;
        return -2;
    }

    return 0;
}

/* com module reset: clear cache / reset rear & front */
void com_reset( com_t *com )
{
    if( 0 == com )
        return;
   
    if( 0 != com->cache )
        memset( com->cache, 0, com->size );
    
   com->front = 0; 
   com->rear = 0;

    return;
}

/* com module: cache clear , set front & rear = 0 */
void com_clear( com_t *com )
{
    if( 0 == com )
        return;
   
    if( 0 != com->cache )
        memset( com->cache, 0, com->size );
    
   com->front = 0; 
   com->rear = 0;

    return;
}

/* decide com is empty */
u16_t com_isempty( com_t *com )
{
    return ( com->front == com->rear );
}

/* decide com is full */
u16_t com_isfull( com_t *com )
{
    return ( (com->front+1) % com->size ) == com->rear; 
}

/* find first da, return index */
s32_t com_find( com_t *com, un_t da )
{
    u16_t i = 0;
    u16_t len;
    u16_t pos;

    if( 0 == com || com_isempty(com) )
        return -1;

    len = com_used( com );          // get used size
    for( i = 0; i < len; i++ )
    {
        pos = (com->rear+i) % com->size;
        if( da == com->cache[ pos ] )
            break;
    }
    
    if( i == len )
       return -1; 
    
    return pos;     // return da unit index
}

/* push one unit into com cache from rear */
s8_t com_push_rear( com_t *com, un_t da )
{
    u16_t index;

    if( 0 == com )
        return -1;

    if( com_isfull( com ) )
    {
//        com_clear( com );      // clear data
        return -2;
    }
    
    com->rear = (0 == com->rear) ? com->size-1: com->rear-1;
    com->cache[ com->rear ] = da;
//    com->rear = index;      // need for inde == com->size-1

    return 0; 
}

/* push one unit into com cache from front */
s8_t com_push( com_t *com, un_t da )
{
    if( 0 == com )
       return -1; 
    
    if( com_isfull( com ) )
    {
//        com_clear( com );
        return -2;
    }

    com->cache[ com->front ] = da; 
    com->front = (com->front+1) % com->size;

    /* add frame detection */

    return 0;
}

/* pop one unit inito da from com cache  */
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


/* get cache max size */
u16_t com_size( com_t *com )
{
    if( 0 == com )
        return 0;

    return com->size-1;
}

/* decide the length from rear to index ( include index )*/
u16_t com_len( com_t *com, u16_t index )
{
    if( 0 == com || !com_in( com, index ) ) 
        return 0;
    
    return (( index - com->rear + com->size ) % com->size) + 1;
}

/* get already used space by unit*/
u16_t com_used( com_t *com )
{
    if( 0 == com )
        return 0;

    return ( com->front - com->rear + com->size ) % com->size;
}

/* get idle space by unit */
u16_t com_space( com_t *com )
{
    if( 0 == com )
        return 0;
    
    if( com_isempty(com) )
        return com->size - 1 ; 
    else
        return ( com->rear + com->size - com->front ) % com->size - 1;
}

/* decide index place whether is in com cache */
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
    
//    out("debug! com->size = %d\n", com->size );
#if 1    
    out( "\n" );
    for( i=0; i < com->size; i++ )
        out( "%02x ", com->cache[i] );
    out( "\n" );
#endif

    out( "\n\n" );

    return;
}


