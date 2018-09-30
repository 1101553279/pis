#ifndef __EVENT_H__
#define __EVENT_H__

#include "chip_pca.h"
#include "net_event.h"

#define EVENT_DIAG_78       (0X1<<0)
#define EVENT_DIAG_56       (0x2<<0)

/* | type | con / accept | rd_no | sd_no | 
 *   2         4            4       4
 *   type: req / rep
 *   con
 *
 */
#define EVENT_PCOM_REP      (0x1<<12)
#define EVENT_PCOM_REP_BUT  (0x1<<8)
#define EVENT_PCOM_REQ      (0x2<<12)
#define EVENT_PCOM_REQ_CON      (0x1<<8)
#define EVENT_PCOM_REQ_UNCON    (0x2<<8)
#define EVENT_PCOM_REQ_APT      (0x3<<8)
#define EVENT_PCOM_REQ_UNAPT    (0x4<<8)
#define EVENT_PCOM_REQ_RD_MASK  (0xf<<4)
#define EVENT_PCOM_REQ_SD_MASK  (0xf<<0)

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
#define EVENT_DCOM_REQ      (0x2<<12)

#define EVENT_DCOM_REQ_RING     (0x1<<8)        // uic rising
#define EVENT_DCOM_REQ_FING     (0x2<<8)        // uic falling
#define EVENT_DCOM_REQ_CON      (0x3<<8)        // con
#define EVENT_DCOM_REQ_UNCON    (0x4<<8)        // uncon
#define EVENT_DCOM_REQ_APT      (0x5<<8)        // accept
#define EVENT_DCOM_REQ_UNQPT    (0x6<<8)        // unaccept
#define EVENT_DCOM_REQ_RD_MASK  (0xf<<4)
#define EVENT_DCOM_REQ_SD_MASK  (0xf<<0)


#define EVENT_OCC_REQ_OFF   (0x1<<0)
#define EVENT_OCC_REQ_ON    (0x2<<0)

/*  | type  | which
 *      2       2
 *      |-- rep_but
 *      |-- req_idle
 */
#define EVENT_PA_REP            (0x1<<2)
#define EVENT_PA_REP_BUT_IN     (0x1<<0)        // pa in
#define EVENT_PA_REP_BUT_OUT    (0x2<<0)        // pa out

#define EVENT_PA_REQ_IDLE       (0x2<<2)

#define EVENT_PPT_REQ_PUSHED    (0x1<<0)
#define EVENT_PPT_REQ_POPED     (0x2<<0)

#define EVENT_BLSN_REQ_START    (0X1<<0)
#define EVENT_BLSN_REQ_STOP     (0X2<<0)

#define EVENT_CAB_UNLINK    0x01
#define EVENT_CAB_LINK      0x02

#define EVENT_IP_REQ_MASK   0xff


//addition


enum event_type{
    EVENT_NONE,
    EVENT_DIAG,
//            PCA_ID_IN_DIAG_78,
//            PCA_ID_IN_DIAG_56,

    EVENT_PCOM, 
//            PCA_ID_IN_BUT ( pcom but )
                        // pushed
//            NET_IN_ID_PCOM,
                        // req  start / stop 
                        // rep  start / stop

    EVENT_DCOM,
//            PCA_ID_IN_UIC_78,
                        // rising / falling
//            PCA_ID_IN_BUT ( dcom but )
                        // pushed
//            NET_IN_ID_DCOM,
                        // req start / stop
                        // rep start / stop

    EVENT_OCC,
//            NET_IN_ID_OCC
                        // start
                        // stop

    EVENT_PA,
//            PCA_ID_IN_BUT ( pa in but )
                        // pushed
//            PCA_ID_IN_BUT ( pa out but )
                        // pushed
//            NET_IN_ID_UIC_IDLE,


    EVENT_BLSN,
//            PCA_ID_IN_UIC_56,
                        // start // stop

    EVENT_PPT,
//            PCA_ID_IN_PPT,
                        // start
                        // stop

    EVENT_LINK,
//            NET_IN_ID_CAB_LINK,
                        // link
                        // unlink

    EVENT_IP,
//            PCA_ID_IN_IP, 
                        // ip changed
    MAX_EVENT,
};


void event_init( void );
u8_t event_check( u32_t *value );
















#if 0
enum event_type{
    EVENT_DIAG,
//            PCA_ID_IN_DIAG_78,
//            PCA_ID_IN_DIAG_56,

    EVENT_PCOM, 
//            PCA_ID_IN_BUT ( pcom but )
                        // pushed
//            NET_IN_ID_PCOM,
                        // req  start / stop 
                        // rep  start / stop

    EVENT_DCOM,
//            PCA_ID_IN_UIC_78,
                        // rising / falling
//            PCA_ID_IN_BUT ( dcom but )
                        // pushed
//            NET_IN_ID_DCOM,
                        // req start / stop
                        // rep start / stop

    EVENT_OCC,
//            NET_IN_ID_OCC
                        // start
                        // stop

    EVENT_PA,
//            PCA_ID_IN_BUT ( pa in but )
                        // pushed
//            PCA_ID_IN_BUT ( pa out but )
                        // pushed
//            NET_IN_ID_UIC_IDLE,


    EVENT_BLSN,
//            PCA_ID_IN_UIC_56,
                        // start
                        // stop

    EVENT_PPT,
//            PCA_ID_IN_PPT,
                        // start
                        // stop

    EVENT_LINK,
//            NET_IN_ID_CAB_LINK,
                        // link
                        // unlink

    EVENT_IP,
//            PCA_ID_IN_IP, 
                        // ip changed
};
#endif
#endif
