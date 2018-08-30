#ifndef __FRAME_H__
#define __FRAME_H__

#include "com.h"

/* for match function, for deciding whether is useful */
typedef u8_t (* match_t)( un_t master, un_t slave );

typedef struct frame{
    un_t head;      // head frame byte
    un_t tail;      // tail frame byte
    un_t addr;      // match value
    u8_t state;     // for frame state
    u16_t frame;    // record frame number
    match_t match;  // match function
    com_t com;      // data cache
}frame_t;


/* init frame module */
s8_t frame_init( frame_t *fm, un_t head, un_t tail, un_t addr, match_t match );

// put one unit data
s8_t frame_put_one( frame_t *fm, un_t da );

// get one frame info
u16_t frame_get( frame_t *fm, un_t *buff, u16_t blen );

// set match function
s8_t frame_match_set( frame_t *fm, match_t match ); 

// get frame number
u8_t frame_frame( frame_t *fm );        


// for debug
s8_t frame_query( frame_t *fm, frame_t *info );


#endif
