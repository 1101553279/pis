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


/*
cmd functions:

    1>. play station start      ok  0x31
    2>. play stop               ok  0x32
    3>. play emgerency start
//    4>. play emgerency stop
    5>. tiu play start
    6>. tiu play stop
    7>. occ play start          ok  0x36
    8>. occ play stop           ok  0x37
    9>. query riu status
    10>. set riu volume size
*/
enum{
    CMD_NONE,
    CMD_PLAY_STA,   // play station start
    CMD_PLAY_EMGA,  // play emg start
    CMD_PLAY_STOP,  // play stop
    CMD_TIU_PLAY_A,
    CMD_TIU_PLAY_O,
    CMD_OCC_PLAY_A,
    CMD_OCC_PALY_O,
    CMD_STATE_CHECK,
    CMD_VOLUME_SET,
};

#if 1
typedef struct cmd_info{
    u8_t cmd;                       // record frame type
    st_start_t sta;                 // store station start info
    volume_t vol;                   // store volume info
    emg_start_t emga;               // store emg start info
}cmd_info_t;

typedef struct pcmd{
    cmd_info_t info;
    un_t *buff;                     // for frame_get
    u16_t len;                      // record frame length
    frame_t frame;
}pcmd_t;

u8_t pcmd_event( pcmd_t *cmd );  
/* static u8_t cmd_parse( cmd_info_t *info, un_t *buff, u16_t len );
 *
 * */
#else
v2
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

v1
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
