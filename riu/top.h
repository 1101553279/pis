#ifndef __TOP_H__
#define __TOP_H__

#include "pcmd.h"
#include "out.h"


/*
    state:      
        idle        idle status
        play_st     station playing status
        play_emg    emergency playing status
        tiu_play    tiu playing
        occ_play    occ playing
        vs_setting  vs1003 setting
*/
typedef struct top{
    u8_t state;             // for self's status

    pcmd_t rec;             // for rec's command
    out_t leds[ MAX_LED ];  // for self's leds
}top_t;

s8_t top_hand( top_t *top );

#endif
