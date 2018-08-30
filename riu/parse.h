#ifndef __PARSE_H__
#define __PARSE_H__

#include "ty.h"
#include "frame.h"
#include "ptime.h"

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

typedef struct parse_info{
    u8_t cmd;               // parse type, command type
    union{
        st_start_t sta;     //a indicate start, store station information
        volume_t vol;       // set volume information
        emg_start_t emga;   //................, store emergencey information
    }info_u;
}parse_t;

// parse one frame info
s8_t parse_frame( un_t *buff, u16_t len, parse_t *info );  


#endif
