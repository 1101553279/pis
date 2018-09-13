
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

    u32_t dtime;            // dcom :for  host call timeout
        
    struct list_head head;  // other call me wait communication queue
};

struct dacu{
//    u8 state;         // idle || wait || using
    struct com com;     // com function
    struct pa pa;       // public announce function
    struct listen ls;   // listen function
    u8_t ppt_flag;      // ppt level record
};

