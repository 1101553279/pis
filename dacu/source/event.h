#ifndef __EVENT_H__
#define __EVENT_H__

#include "chip_pca.h"
#include "net_event.h"

// EVENT_DIAG
#define EVENT_DIAG_78       (0X1<<0)
#define EVENT_DIAG_56       (0x2<<0)

// EVENT_COM
#define COM_EVENT_PD_MASK   (0x3<<14)
#define COM_EVENT_TYPE_MASK (0X3<<12)
#define COM_VALUE_PQ_MASK   (0xf<<8)
#define COM_VALUE_RD_MASK   (0xf<<4)
#define COM_VALUE_SD_MASK   (0xf<<0)

#define EVENT_PCOM      (0x1<<14)
/* | type | con / accept | rd_no | sd_no | 
 *   2         4            4       4
 *   type: req / rep
 *   con
 *
 */
#define EVENT_PCOM_REP      (0x1<<12)
#define EVENT_PCOM_REP_BUT  (0x1<<8)

#define EVENT_PCOM_REQ          (0x2<<12)
#define EVENT_PCOM_REQ_CON      (0x1<<8)    // connect
#define EVENT_PCOM_REQ_UNCON    (0x2<<8)    // unconnect
#define EVENT_PCOM_REQ_APT      (0x3<<8)    // accept
#define EVENT_PCOM_REQ_UNAPT    (0x4<<8)    // unaccept
#define EVENT_PCOM_REQ_RD_MASK  (0xf<<4)
#define EVENT_PCOM_REQ_SD_MASK  (0xf<<0)

#define EVENT_DCOM      (0x2<<14)
/* | type | uic/but |   rising/ falling |
 *
 * | type | con / accept | rd_no | sd_no | 
 *   2         4            4       4
 *   type: req / rep
 *   con
 *
 */
#define EVENT_DCOM_REP      (0x1<<12)
#define EVENT_DCOM_REP_BUT  (0x1<<8)

#define EVENT_DCOM_REQ          (0x2<<12)
#define EVENT_DCOM_REQ_RING     (0x1<<8)        // uic rising
#define EVENT_DCOM_REQ_FING     (0x2<<8)        // uic falling
#define EVENT_DCOM_REQ_CON      (0x3<<8)        // connect
#define EVENT_DCOM_REQ_UNCON    (0x4<<8)        // unconnect
#define EVENT_DCOM_REQ_APT      (0x5<<8)        // accept
#define EVENT_DCOM_REQ_UNAPT    (0x6<<8)        // unaccept
#define EVENT_DCOM_REQ_RD_MASK  (0xf<<4)
#define EVENT_DCOM_REQ_SD_MASK  (0xf<<0)

// EVENT_OCC
#define EVENT_OCC_REQ_OFF   (0x1<<0)
#define EVENT_OCC_REQ_ON    (0x2<<0)

/*  | type  | which
 *      2       2
 *      |-- rep_but
 *      |-- req_idle
 */
// EVENT_PA
#define EVENT_PA_REP            (0x1<<2)
#define EVENT_PA_REP_BUT_IN     (0x1<<0)        // pa in button
#define EVENT_PA_REP_BUT_OUT    (0x2<<0)        // pa out button

#define EVENT_PA_REQ_IDLE       (0x2<<2)

// EVENT_PPT
#define EVENT_PPT_REQ_PUSHED    (0x1<<0)
#define EVENT_PPT_REQ_POPED     (0x2<<0)

// EVENT_BLSN
#define EVENT_BLSN_REQ_START    (0X1<<0)
#define EVENT_BLSN_REQ_STOP     (0X2<<0)

// EVENT_LINK
#define EVENT_CAB_UNLINK    0x01
#define EVENT_CAB_LINK      0x02

// EVENT_IP
#define EVENT_IP_REQ_MASK   0xff

//all event type
enum event_type{
    EVENT_NONE,
    EVENT_DIAG, // uic78 diag or uic56 diag
    EVENT_COM,  // EVENT_PCOM
                // EVENT_DCOM
                    // pcom pushed
                        // pecu req  start / stop 
                        // other dacu rep  start / stop
                    // uic78 rising / falling or dcom pushed
                        // other dacu req start / stop
                        // other dacu rep start / stop
    EVENT_OCC,  // occ start / stop
    EVENT_PA,   // but in/out pushed or NET_IN_ID_UIC_IDLE,
    EVENT_BLSN, // start / stop
    EVENT_PPT,  // start / stop
    EVENT_LINK, // link / unlink
    EVENT_IP,   // ip changed
    MAX_EVENT,
};

u8_t event_check( u32_t *value );

#endif
