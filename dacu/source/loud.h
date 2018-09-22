#ifndef __LOUD_H__
#define __LOUD_H__

#include "btype.h"

enum loud_status{
    US_IDLE,
    US_BLSNING,
    US_PAING,
    US_TIPING,      // com tiping
    US_OLSNING,     // occ listen
    US_COMING,      // communication 
    US_FROZEN,      // other in communication
};

enum loud_op{
    UE_TIP,         // communication tiping
    UE_COM,         // in commnunication 
    UE_FROZEN,      // other in communication, frozen close
    UE_TIP_CLOSE,   // tip close
    UE_FROZEN_CLOSE,
    
    UE_LSN,         // broadcast or occ listening

    UE_PA,          // pa_in / pa_out

    UE_CLOSE,       // for all above
};

enum loud_type{
    UT_COM,
    UT_OLSN,
    UT_PA,
    UT_BLSN,
    MAX_UT,
};

typedef void ( *cmd_cb_t )( struct loud *loud, u8_t op );

struct loud{
//    struct loud_cnt{
        u32_t frozen:1;
        u32_t com:1;
        u32_t olisten:1;        // occ listen
        u32_t com_tip:13;
        u32_t pa:2;
        u32_t blisten:2;        // broadcast listen
        cmd_cb_t cmdcb[ MAX_UT ];        

//        void  op pca9539 for louder control
//    }status;
};

void loud_init( struct loud *loud );
void loud_con( struct loud *loud, u8_t type, u8_t op );

// for debug
void dump_loud( struct loud *loud );

#endif
