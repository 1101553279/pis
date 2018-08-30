#ifndef __FRAME_H__
#define __FRAME_H__

#include "com.h"
#include "ptime.h"

/* for match function, for deciding whether is useful */
typedef u8_t (* match_t)( un_t master, un_t slave );

typedef struct frame{
    un_t head;      // head frame byte
    un_t tail;      // tail frame byte
    un_t addr;      // match value
    u16_t frame;    // record frame number
    match_t match;  // match function
    com_t com;      // data cache
}frame_t;

#if 1
typedef struct st_start{
    ptime_t time; 
    u16_t line;
    u16_t id;
    u8_t state;
}st_start_t;

typedef struct volume{
    u32_t value;
}volume_t;

typedef struct emg_start{
    ptime_t time;
    u32_t id;
    u8_t state;
}emg_start_t;

typedef struct frame_info{
    u8_t cmd;               // frame type, command type
    union{
        st_start_t sta;     //a indicate start, store station information
        volume_t vol;       // set volume information
        emg_start_t emga;   //................, store emergencey information
    }info_u;
}frame_info_t;
#endif

/* init frame module */
void frame_init( frame_t *fm, un_t head, un_t tail, un_t addr, match_t match );

// get one frame info
s8_t frame_get( frame_t *fm, un_t *buff, u16_t *len );

// set match function
s8_t frame_match_set( frame_t *fm, match_t match ); 

// get frame number
u8_t frame_frame( frame_t *fm );        

// parse one frame info
u8_t frame_parse( un_t *buff, u16_t len, frame_info_t *info );  

// for debug
u8_t frame_query( frame_t *fm, frame_t *info );


#endif
