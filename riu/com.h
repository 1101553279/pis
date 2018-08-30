#ifndef __COM_H__
#define __COM_H__
#include "ty.h"

/* define cache data type */
typedef u8_t un_t;

/* for rec's communication */
typedef struct com{
    u16_t front;        // loop queue front
    u16_t rear;         // loop queue rear
//    u16_t frame;        // frame number
    u16_t size;         // cache max size
    un_t *cache;        // data cache
}com_t;


void com_init( com_t *com );            /* init com */
u16_t com_isfull( com_t *com );         /* wheter queue is full */
u16_t com_isempty( com_t *com );        /* whether queue is empty */
s8_t com_push( com_t *com, un_t da );   /* push one unit */
s8_t com_pop( com_t *com, un_t *da );   /* pop one unit */
s8_t com_pop_frame( com_t *com, un_t *buff, u16_t *len );   /* pop one frame start: 0x2...tail:0x03| | | shift two units*/
u8_t com_type( un_t da );               /* decide frame type */
u16_t com_frame( com_t *com );          /* frame number */
u16_t com_size( com_t *com );           /* queue max size */
u16_t com_used( com_t *com );           /* queue used size */
u16_t com_space( com_t *com );          /* queue unused size */
u8_t com_in( com_t *com, u16_t index );

void com_print( com_t *com );

#endif
