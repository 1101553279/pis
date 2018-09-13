
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

