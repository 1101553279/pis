#ifndef __CHIP_PCA_H__
#define __CHIP_PCA_H__

#include "btype.h"

//output part: pca9539:00
#define MIC_OPEN    0x0808
#define MIC_CLOSE   0x0000

#define LOUD_TIP    0x0001      // need modifying
#define LOUD_COM    0x0406
#define LOUD_LSN    0x0000
#define LOUD_PA     0x0414
#define LOUD_CLOSE  0x0010

#define OUT_UIC78_H     0x0100
#define OUT_UIC78_L     0x0200
#define OUT_UIC78_D     0x0000          //default value
/*
#define OUT_UIC78_H     0x0100
#define OUT_UIC78_L     0x0000
#define OUT_UIC78_SH    0x0200
#define OUT_UIC78_SL    0x0000
*/

#define OUT_UIC56_H     0x0080
#define OUT_UIC56_L     0x0000

#define VOLUME_0        0x0000
#define VOLUME_1        0x0020
#define VOLUME_2        0X0040
#define VOLUME_3        0x0060

//pca9539: 10
#define LED_PA_IN_ON    0x0101
#define LED_PA_IN_OFF   0x0100
#define LED_PCOM_ON     0x0202
#define LED_PCOM_OFF    0x0200
#define LED_DCOM_ON     0x0404
#define LED_DCOM_OFF    0x0400
#define LED_PA_OUT_ON   0x0808
#define LED_PA_OUT_OFF  0x0800
#define LED_MASK        0x0F00
#define LED_CON( led )  (LED_MASK &(led<<8))

#define BUT_READ_ID(id) (((id)&0x700)>>8)   
#define BUT_READ_VA(va) ((va)&0x1)          

#define BUT_ID_PA_OUT   0x4
#define BUT_ID_DCOM     0x3
#define BUT_ID_PCOM     0x2
#define BUT_ID_PA_IN    0x1

#define BUT_PUSHED      0x0
#define BUT_POPED       0x1


/*
enum pca_type{
    PCA_TYPE_IN,
    PCA_TYPE_OUT,
    PCA_TYPE_CHECK,
    MAX_PCA_TYPE,
};
*/

enum pca_id_out{
    PCA_ID_OUT_MIC,
    PCA_ID_OUT_LOUD,
    PCA_ID_OUT_UIC78,
    PCA_ID_OUT_UIC56,
    PCA_ID_OUT_VOLUME,
    PCA_ID_OUT_LED,
    MAX_PCA_ID_OUT,
};

enum pca_id_check{
    PCA_ID_CHECK_IP,
};

enum pca_id_event{
    PCA_ID_IN_NONE,     //0
    PCA_ID_IN_DIAG_78,  //1     diag infor
    PCA_ID_IN_DIAG_56,  //2
    PCA_ID_IN_UIC_78,   //3     dacu com
    PCA_ID_IN_UIC_56,   //4     listen 
    PCA_ID_IN_IP,       //5     ip dial info
    PCA_ID_IN_BUT,      //6     button pushed poped
    PCA_ID_IN_PPT,      //7     ppt pushed or poped
    MAX_PCA_ID_IN,
};

enum pca_type_event{
    PCA_TYPE_IN_NONE,
    PCA_TYPE_IN_CHIP,
    MAX_PCA_TYPE_IN,
};

typedef struct chip_event{
    u8_t type;      // out or check
                    // in or check
    u8_t id;        // which
    u16_t value;    // op
}chip_event_t;

#if 0       //for debug
#define MAX_PCA 3

typedef struct chip_pca{
//    u8_t no;
    u8_t rflag;     // new input flag
    u8_t addr;      // i2c address
    u16_t rmask;    // read mask
    u16_t wmask;    // write mask
    u16_t old;      // for read old store
    u16_t cur;      // current value
    u16_t new;      // for write new store
    char *spec;
}chip_pca_t;
#endif

void pca_init(void);
void pca_out( u8_t id, u16_t value );
s8_t pca_event(struct chip_event *e );
s8_t pca_check( u8_t id, u16_t *value );
void pca_rflag( u8_t no );


// for debug
void dump_pca( void );
//s8_t event_update();
//extern struct chip_pca all_pca[ 3];
#endif
