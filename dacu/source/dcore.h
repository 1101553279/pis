#ifndef __DCORE_H__
#define __DCORE_H__

#include "btype.h"
#include "blist.h"
#include "loud.h"
#include "led.h"
#include "chip_pca.h"

/*
    idle
    frozen
    setup
*/
struct dcom{
    u8_t status;      // idle / frozen / setup
    u8_t type;

    u8_t rd_cab;    // receive device cab
    u8_t rd_type;   // receive device type
    u8_t rd_no;     // receive device number
    u8_t sd_cab;    // send device cab
    u8_t sd_dev;    // send device type
    u8_t sd_no;     // send device number
    u8_t dc_no;     // device channel number

    struct list_head *comq;     // com wait queue
    struct led pled;            // pcom led
    struct led dled;            // dcom led
};

struct docc{
    u8_t status;
};

struct dpa{
    u8_t status;
};

struct dlsn{
    u8_t status;
};

struct dcore{
    struct dcom com;
    struct docc occ;
    struct dpa pa;
    struct dlsn lsn;
    struct loud loud;
};



#endif
