enum pca_id_event{
    PCA_ID_IN_NONE,
    PCA_ID_IN_DIAG_78,
    PCA_ID_IN_DIAG_56,
    PCA_ID_IN_UIC_78,
    PCA_ID_IN_UIC_56,
    PCA_ID_IN_IP,
    PCA_ID_IN_BUT,
    PCA_ID_IN_PPT,
    MAX_PCA_ID_IN,
};

enum net_in_id{
    NET_IN_ID_OCC = 0,      // listen
    NET_IN_ID_UIC_IDLE,     // pa
    NET_IN_ID_CAB_LINK,     // cab link info
    NET_IN_ID_PCOM,          // dacu pecu com
    NET_IN_ID_DCOM,          // dacu pecu com
    MAX_NET_IN_ID,
};


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

struct event{
    u8_t type;

};

struct event_info{

};




====== com module ==============================================================
struct com{
    struct com_info{
        u8_t com_type;  // communication type
                    // pcom or dcom
        u8_t status;    // current status
                        idle
                        setup
                        frozen

        u8_t rd_cab     // receive device cab
        u8_t rd_type;   // receive device type
        u8_t rd_no;     // receive device number
        u8_t sd_cab;    // send device cab
        u8_t sd_dev;    // send device type
        u8_t sd_no;     // send device number
        u8_t dc_no;     // device channel number
    }setup;             // com setup information
    struct list_head chead;         // cache wait or call status communication
                        // status   -> wait
                        //          -> call
};

====== ppt module =============================================================

enum ppt_status{
    PS_IDLE,
    PS_USING,
};

enum ppt_op{
    PE_CLOSE,
    PE_OPEN,
};

enum ppt_type{
    PT_COM,
    PT_PA,
};

struct ppt{
//    struct ppt_cnt{
        u8_t com:2;
        u8_t pa:2;
//    }status;
};

u8_t ppt_con( struct ppt *ppt, u8_t type, u8_t op );

//--------------------- old method --------------------------
/*
struct ppt{
    u8_t status;
    u8_t que;
};

type        op
-----------------
com         open / close
pa          open / close

ppt_con( struct ppt *ppt, type, op );

u8_t que;
      2   1
 *  | * | * 
      |   |-- com
      |------ pa

*/
====== loud module =============================================================
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
    UE_PA,          // pa_in / pa_out
    UE_TIP,         // communication tiping
    UE_LSN,         // broadcast or occ listening
    UE_COM,         // in commnunication 
    UE_FROZEN,      // other in communication, frozen close
    UE_CLOSE,       // com or olsn or pa or bsln close
    UE_TIP_CLOSE,   // tip close
};
  
enum loud_type{
    UT_COM,
    UT_OLSN,
    UT_PA,
    UT_BLSN,
};
  
struct loud{
//    struct loud_cnt{
        u32_t frozen:1;
        u32_t com:1;
        u32_t olisten:1;        // occ listen
        u32_t com_tip:13;
        u32_t pa:2;
        u32_t blisten:2;        // broadcast listen
//    }status;
};


u8_t loud_con( struct loud *loud, u8_t type, u8_t op );

//----------------------- old method ------------------------
/*
type    no        op
-----------------------
com     0...12    tip 
        0...12    com 
        0...12    frozen 
        0...12    close

olsn    0         lsn 
        0         close

pa      0         pa    
        0         close

blsn    0         lsn   
        0         close

loud_con( struct loud *loud, type, no,  op );

u32_t que;             // pecu tip             //dacu
 1     1     1     12 11 10  9 8 7 6 5 4 3 2 1 | 1 |  1  |  1  | 1
 *  |  *   |  *   | *  *  *  * * * * * * * * * | * |  *  |  *  | * 
       |      |     -------------------------------   |     |    |-- frozen
       |      |               |                       |     |------- com
       |      |               |                       |------------- occ listen
       |      |               |------------------------------------- com tip
       |      |----------------------------------------------------- pa
       |------------------------------------------------------------ broadcast listen
*/

====== led module =============================================================
enum led_status{
    LS_OFF;
    LS_BLINK;
    LS_ON;
};

// led group index
enum led_type{
    LT_PCOM = 0,
    LT_DCOM,
    LT_PAIN,
    LT_PAOUT,
};

enum led_op{
    LE_BLINK,   // add blink led
    LE_ON,      // add on led
    LE_BK_OFF,  // remove blink led
    LE_ON_OFF,  // remove on led
};

typedef void ( *led_cb_t )( u8_t ledno, u8_t hl );

struct led{
    u8_t type;      // pcom / dcom / pa_in / pa_out
    struct led_cnt{
        u8_t on:2;
        u8_t blink:5;
        u8_t off:1;
    }status;

    u8_t hl;        // current level for blink
    u32_t stime;    // start time   for blink
    u32_t timeout;  // period for blink

    led_cb_t cb;    // led timeout callback for blink status
};

struct led group[ 4 ];

pcom:
event               cur_status      next_status     op          should_led
d <- p: request     idle            wait            blink       blink   
                    wait            wait            blink       blink
                    setup           setup           blink       on
                    frozen          frozen          blink       on
                    ------          ------
d <- p: hangup      wait            idle            bk_off      off
                                    wait            bk_off      blink
                                    ------
                    setup           setup           bk_off      on
                    frozen          frozen          bk_off      on
                    ------          ------
d <- d: response    wait            frozen          on          on
                    ------          ------
d <- d: hangup      frozen          idle            on_off      off
                                    wait            on_off      blink
                    ------          ------
pushed: get         wait            setup           on          blink
                                    ------
pushed: put         setup           idle            on_off      off
                                    wait            on_off      blink

/*      op: on     ->     blink--; on++
 *          blink  ->     blink++;
 *          bk_off ->     blink--
 *          on_off ->     on--;
 */
void led_con( struct led *led, u8_t op );       // op one led

// for led blink 
void led_check( struct led *group, u8_t nu );   // literate all leds



//----------------------------- old method ----------------------------------------
/*              old method
u32_t pled;             // passenger com led
   1    12 11 10 9 8 7 6 5 4 3 2 1      1
   *  | *  *  *  * * * * * * * * *   |  * 
        ----------------------------    |-- pecu com
                    |----------------------  pecu blink

u32_t dled;             // driver com led
   1     1    1
   *  |  *  | *   
         |    |---- dacu com
         |--------- dacu blink

u32_t piled;            // pa in led
   1    1
   *  | *
        |-- pa in on



u32_t poled;            // pa out led
   1    1
   *  | *
        |-- pa out on 
*/
