#ifndef __PCOM_H__
#define __PCOM_H__

#include "btype.h"
#include "blist.h"

struct pcom;
typedef struct pcom pcom_t;

typedef void (* pcom_callback_t)( pcom_t *com, void *data, u8_t len );

typedef struct pecu{
    struct list_head head;  //for link list queue

    u8_t rcabno;        // receive cab no
    u8_t rdev;          // receive device
    u8_t rdevno;        // receive device no
    u8_t scabno;        // send cab no
    u8_t sdev;          // send device
    u8_t sdevno;        // send device no
    u8_t channel;       // use channel
}pecu_t;

typedef struct pcom{
    u8_t state;     //
                    //1. idle       // not busy
                    //2. pwait      // pecu request 
                    //3. setup      // intercom setup
                    //4. frozen     // intercom by other setup
                    
    pcom_callback_t pwait_call;     // callback when enter pecu wait state
    pcom_callback_t setup_call;     // callback when enter intercom setup
    pcom_callback_t frozen_call;    // callback when enter frozen state
    
    pecu_t info;    // pecu information store when intercom setup

    struct list_head pecuq;     
}pcom_t;


s8_t pcom_init( pcom_t *com );
//s8_t pcom_add( pcom_t *com, pecu_t 


#endif
