#ifndef __TOP_H__
#define __TOP_H__


/*
    state:      
        idle        idle status
        play_st     station playing status
        play_emg    emergency playing status
        tiu_play    tiu playing
        occ_play    occ playing
        vs_setting  vs1003 setting
*/
struct top{
    u8_t state;
}top_t;

#endif
