#ifndef __CHIP_PCA_H__
#define __CHIP_PCA_H__

#include "btype.h"

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
