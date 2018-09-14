
enum even_type{
    NET_WORK,       // from network
    PCA_CHIP,       // from pca chip
    TIME_OUT,       // from check timeout
};

enum part_mod{
    NCOM_MOD,       // network mod class
    NLISTEN_MOD,
    NPA_MOD,
    
    PPA_MOD,
    PCOM_MOD,
    PLISTEN_MOD,
    
    TPA_MOD,        // timeout check
    TCOM_MOD,
    TPPT_MOD,

};

struct ncom{
};
struct nlisten{
};
struct npa{
};

struct event{
    u8 type;        // from where
    struct part{
        u8 mod;     // for what
        union data{     // data store
            struct ncom com;
            struct nlisten ln;
            struct npa pa;
        };
    };
};

struct com{
    u8 state;       // indicate who -> who 
    //pecu -> dacu  // pidle 
                    // pend_wait        // pecu wait current dacu
                    // phost_using      // current dacu get the pecu call
                    // pother_using     // other dacu get the pecu call
    //dacu -> dacu
                    // didle
                    // dhost_wait
                    // dend_wait
                    // dusing
                        // dhost_using
                        // dend_using
                    // dother_using
        
    //using info
    //...
    //...
    u8_t pbut_flag;         // pcom button level record
    u8_t dbut_flag;         // dcom button level record

    u8_t duic78_flag;       // dcom uic78 level record
    u8_t duic78_count;      // dcom uic78 pluse count
    u32_t duic78_time;      // record time for check 3 times pluse during 300ms

    u32_t dtimeout;         // dcom :for  host call timeout
        
    struct list_head head;  // other call me wait communication queue
};

struct pa{
    u8_t state;
    // in  
                    //in_idle
                    //in_wait
                    //in_using
    //out
                    //out_idle
                    //out_wait
                    //out_using

    u8_t inbut_flag;        // pa in button last level record
    u8_t outbut_flag;       // pa out button last level record
    u32_t in_timeout;       // pa in timeout record
    u32_t out_timeout;      // pa out timeout record
};

struct listen{
    u8_t state;
    //broadcast
                        // bc_idle
                        // bc_wait
                        // bc_using
    //occ
                        // occ_idle
                        // occ_wait
                        // occ_using
    u8_t uic56_flag;        // record last time uic56 level
};

struct dacu{
//    u8 state;         // idle || wait || using
    struct com com;     // com function
    struct pa pa;       // public announce function
    struct listen ls;   // listen function
    u8_t ppt_flag;      // ppt level record
};

=======================  struct as a whole ======================================
struct event{
    u8_t func;          // which function's event
    union flag{
        struct{
            u32_t 
        };
    };
    data
};

struct event{
    struct flag flag;
};

struct timeout{
    u32_t times[ 10 ];      // record start time for check timeout
};

union status{
    u32_t status;           // com / pa / listen functions status
    struct{
            
    };
};

struct timeout{
};

struct dacu{
    struct event e;
    union status s;
//for action
    struct list_head head;  // for storing call wait queue
    struct timeout out;
};

----------------------each function : event an status -------------------------------------
pcom( pecu, pca:button, dacu )
    event:  
        network: 
            pecu:
                connect 
                close
            dacu:
                accept
                close
        
        pca9539:button
            accept
            close
            
        timeout  -> none

    status:
        pidle 
        pend_wait    
        phost_using  
        pother_using 


dcom( dacu, pca:uic78, pca:button )
    event:
        network: 
            dacu:
                connect   dacu7,dacu8
                accept    dacu7,dacu8
                close     dacu7,dacu8
        
        pca9539:button  
            connect       connect to end      
            accept        accept end
            close         close using

        pca9539:uic78
            connect       dacu1,dacu2
            accept        dacu1,dacu2
            close         dacu1,dacu2
            
        timeout
            connect_timeout

    status:
       didle
       dhost_wait 
       dend_wait
       dusing
           dhost_using;    // host be accepted
           dend_using      // end be accepted
       dother_using        // current dacu is not in using

broadcast
    event:
        network:
            uic_idle

        pca9539:button  
            pa_in_start
            pa_in_stop
            pa_out_start
            pa_out_stop

        timeout
            pa_in_timeout
            pa_out_timeout

    status:
        pa_in_idle
        pa_in_wait
        pa_in_using

        pa_out_idle
        pa_out_wait
        pa_out_using


listen( rec(occ) pc:uic56 )
    event:
        network:
            occ_start
            occ_stop

        pca9539:uic
            listen_start (uic56)
            listen_stop  (uic56)

        timeout
            none

    status:
        broadcast_idle
        broadcast_wait
        broadcast_using

        occ_idle
        occ_wait
        occ_using


