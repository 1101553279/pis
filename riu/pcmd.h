#ifndef __CMD_H__
#define __CMD_H__

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

enum{
    CMD_NONE,
};
#if 1
typedef struct cmd_info{
    u8_t cmd;                       // record frame type
    st_start_t sta;                 // store station start info
    volume_t vol;                   // store volume info
    emg_start_t emga;               // store emg start info
    un_t *buff;                     // for frame_get
    u16_t len;                      // record frame length
}cmd_info_t;

typedef struct pcmd{
    cmd_info_t info;
    frame_t frame;
}pcmd_t;

u8_t pcmd_event( pcmd_t *cmd );  

#else

typedef struct pcmd{
    u8_t cmd;                       // record frame type
    st_start_t sta;                 // store station start info
    volume_t vol;                   // store volume info
    emg_start_t emga;               // store emg start info
    un_t *buff;                     // for frame_get, need alloc space
    u16_t len;                      // record frame length

    frame_t frame;
}pcmd_t;

#endif


#if 0       // need compare previous && current id
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


#endif
