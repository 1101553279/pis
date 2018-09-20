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
    u8_t status;
    u8_t que;
};

type        op
-----------------
com         open / close
pa          open / close

ppt_con( type, op );

u8_t que;
      2   1
 *  | * | * 
      |   |-- com
      |------ pa


====== loud module =============================================================
enum loud_status{
    US_IDLE,
    US_BLSNING,
    US_PAING,
    US_OLSNING,     // occ listen
    US_TIPING,      // com tiping
    US_COMING,      // communication 
    US_FROZEN,      // other in communication
};

enum loud_op{
    UE_BLSN,        // broadcast listening
    UE_PA,          // pa_in / pa_out
    UE_OLSN,        // occ listening
    UE_TIP,         // communication tiping
    UE_COM,         // in commnunication 
    UE_FROZEN,      // other in communication
    UE_CLOSE,       // close
};

enum loud_type{
    UT_COM,
    UT_LSN,
    UT_PA,
    UT_BLSN,
};

struct loud{
    u8_t status;    // current status
    u32_t que;      // led queue, for that receive close event when having wait to blink
};


type        op
-----------------
pcom        tip / com / frozen / close
dcom        tip / com / frozen / close
olsn        lsn / close
pa          pa  / close
blsn        lsn / close

loud_con( type, no,  op );

u32_t que;             // pecu tip                  //dacu
 1     1     1     12 11 10  9 8 7 6 5 4 3 2 1 | 1 |  1  |  1  | 1
 *  |  *   |  *   | *  *  *  * * * * * * * * * | * |  *  |  *  | * 
       |      |     ---------------------------  |    |     |    |-- frozen
       |      |               |                  |    |     |------- com
       |      |               |                  |    |------------- occ listen
       |      |               |                  |------------------ dcom tip
       |      |               |------------------------------------- pcom tip
       |      |----------------------------------------------------- pa
       |------------------------------------------------------------ broadcast listen

====== led module =============================================================
enum led_status{
    LS_OFF;
    LS_BLINK;
    LS_ON;
};

enum led_op{
    LE_BLINK,   // blink led
    LE_ON,      // turn on led
    LE_CLOSE,   // close led
};

enum led_type{
    LT_PCOM,
    LT_DCOM,
    LT_PAIN
    LT_PAOUT,
};

struct led{
    u8_t type;          // pcom / dcom / pa_in / pa_out
    u8_t status;        // current status: off / blink / on
    u32_t que;          // led queue, for that receive close event when having wait to blink
};

struct led group[ 4 ];

type        no          op
--------------------------------
pcom    1 ..... 12      off / blink / on
dcom    1 ..... 4       off / blink / on
pa_in       1           off / on 
pa_out      1           off / on

led_con( type, no, op );



u32_t pled;             // passenger com led
   1    12 11 10 9 8 7 6 5 4 3 2 1    1
   *  | *  *  *  * * * * * * * * * |  * 
        --------------------------    |-- pecu com
                    |-------------------- pecu blink


u32_t dled;             // driver com led
   1     1    1
   *  |  *  | *   
         |    |-- dacu com
         |--------- dacu blink

u32_t piled;            // pa in led
   1    1
   *  | *
        |-- pa in on



u32_t poled;            // pa out led
   1    1
   *  | *
        |-- pa out on 
