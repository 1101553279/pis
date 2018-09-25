#ifndef __CHIP_PCA_H__
#define __CHIP_PCA_H__

#include "btype.h"
/*
// pca9539, 00, mask
// output: uic78, uic56, loud, volume, mic
// input: uic78, uic56, diag_uic78, diag_uic56

mask: 0x0FFF
output:
    spe         pins        mask    level   list
    mic         12,4        0x0808   h/l    open mic / close mic
    loud        1,2,3,5,11  0x0417          for com / pa
    uic78       9,10        0x0300   h/l    for driver com
    uic56       8           0x0080   h/l    for send pa
    volume      6,7         0x0060   h/l    change volume size

mask: 0xF000
input:
    spe         pins        mask    level   list
    diag_uic78  16          0x8000          for driver com   uic78 error
    diag_uic56  15          0x4000          for listen       uic78 error
    uic78       14          0x2000          for driver com
    uic56       13          0x1000          for listen

// pca9539, 01, mask
// output: none
// input: ip
output:
    none

mask: 0x00FF
input:
    spe         pins        mask    level   list
    ip          1 ~ 8       0x00FF          read ip

// pca9539, 10, mask
// output: led
// input: button, ppt

mask: 0x0F00
output:
    spe         pins        mask    level   list
    led         9 ~ 12      0x0F00          control leds

mask: 0x001F
input:
    spe         pins        mask    level   list
    button      1 ~ 4       0x000F          read button pushed
    ppt         5           0x0010          read ppt    puhsed

*/
enum pca_type{
    PCA_TYPE_IN,
    PCA_TYPE_OUT,
    PCA_TYPE_CHECK,
    MAX_PCA_TYPE,
};

enum pca_id_in{
    PCA_ID_IN_DIAG_78,
    PCA_ID_IN_DIAG_56,
    PCA_ID_IN_UIC_78,
    PCA_ID_IN_UIC_56,
    PCA_ID_IN_IP,
    PCA_ID_IN_BUT,
    PCA_ID_IN_PPT,
    MAX_PCA_ID_IN,
};

enum pca_id_out{
    PCA_ID_OUT_MIC,
    PCA_ID_OUT_LOUD,
    PCA_ID_OUT_UIC78,
    PCA_ID_OUT_UIC56,
    PCA_ID_OUT_VOLUME,
    PCA_ID_OUT_LED,
    MAX_PCA_ID_OUT,
};

typedef struct chip_pca{
    u8_t rflag;     // new input flag
    u8_t addr;      // i2c address
    u16_t rmask;    // read mask
    u16_t wmask;    // write mask
    u16_t old;      // for read old store
    u16_t cur;      // current value
    u16_t new;      // for write new store
    char *spec;
}chip_pca_t;

typedef struct chip_com{
    u8_t type;      // out or check
                    // in or check
    u8_t id;        // which
    u16_t value;    // op
}chip_com_t;

void pca_init( );


// for debug
void dump_pca( );

#endif
