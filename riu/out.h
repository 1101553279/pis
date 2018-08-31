#ifndef __OUT_H__
#define __OUT_H__

#include "ty.h"

//for out active( enable or disable )
enum{ 
    OUT_NOACT = 0,
    OUT_ACTIVE,
};

// for out mode
enum{
    MODE_NONE = 0,
    MODE_SINGLE,
    MODE_PERIOD,
};

// for out flag
enum{
    FLAG_OFF = 0,
    FLAG_ON,
};

// for out_config
enum{
    CON_ACTIVE = 1,
    CON_MODE,
    CON_FLAG,
    CON_STIME,
    CON_TIMEOUT,
    CON_HANDLE,
    MAX_CON,
};

typedef void ( *out_timeout_t )( u8_t no, u8_t flag );

typedef struct out{
    u32_t no:8;         // id number
    u32_t active:2;     // active or inacitve( enable or disable)
    u32_t mode:2;       // run mode: one-shoot or period
    u32_t flag:4;       // device status store
    u32_t resv:16;      // reserve bits
    u32_t stime;        // start time
    u32_t timeout;      // status keep time
    out_timeout_t hand;   // timeout handler
}out_t;

// init out status
void out_init( out_t *out, u8_t no, u8_t mode, u32_t timeout, out_timeout_t handle );

// config out module parameter
s8_t out_config( out_t *out, u8_t type, u32_t data );       // for config out parameter

// out module start work
void out_start( out_t *out ); 

// out module stop work
void out_stop( out_t *out );

// check whether is timeout
void out_check( out_t *out );

// only for test
void out_print( out_t *outs, u8_t no );


#endif
