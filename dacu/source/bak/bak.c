#include "chip_pca.h"
#include "debug.h"

// pca9539 00
#define PCA00_OMASK 0x0FFF
#define O_00_MIC    0X0808
#define O_00_LOUD   0x0417
#define O_00_UIC78  0x0100
#define O_00_78SHORT   0x0200
#define O_00_UIC56  0x0080
#define O_00_VOLUME 0x0060

#define PCA00_IMASK 0xF000
#define I_00_diag_78    0x8000
#define I_00_diag_56    0x4000
#define I_00_78         0x2000
#define I_00_56         0x1000

//pca9539 01
#define PCA01_IMASK 0x00FF
#define I_01_IP     0x00FF

//pca9539 10
#define PCA10_OMASK 0x0F00
#define O_10_LED_01 0x0100      // led: pa in
#define O_10_LED_02 0x0200      // led: pcom
#define O_10_LED_03 0x0400      // led: dcom
#define O_10_LED_04 0x0800      // led: pa out

#define PCA10_IMASK 0x001F
#define I_10_BUT_MASK   0x000F  // button mask
#define I_10_BUT_01 0x0001      // but: pa in
#define I_10_BUT_02 0x0002      // but: pcom
#define I_10_BUT_03 0x0004      // but: dcom
#define I_10_BUT_04 0x0008      // but: pa out

#define I_10_PPT_MASK   0x0010  // ppt mask
#define I_10_PPT    0x0010      // ppt

#define MAX_PCA 3
static struct chip_pca all_pca[ MAX_PCA ] = { 0 };

void pca_init( )
{
    u8_t i = 0;
    
    for( i = 0; i < MAX_PCA; i++ )
    {
        all_pca[ i ].rflag = 0;
        all_pca[ i ].addr = i;
        all_pca[ i ].old = 0;
        all_pca[ i ].cur = 0;
        all_pca[ i ].new = 0;
    }
    
    all_pca[ 0 ].rmask = PCA00_IMASK;
    all_pca[ 0 ].wmask = PCA00_OMASK;
    all_pca[ 0 ].spec = "loud
    all_pca[ 1 ].rmask = PCA01_IMASK;
    all_pca[ 1 ].wmask = 0x0000;
    all_pca[ 2 ].rmask = PCA10_IMASK;
    all_pca[ 2 ].wmask = PCA10_OMASK;

    return;
}

void dump_pca( )
{
    u8_t i = 0;

    dout( "%-5s %-5s %-5s %-10s %-10s %-5s %-5s %-5s %-10s\n",
          "no", "rflag", "addr", "rmask", "wmask", "old", "cur", "new", "spec" );

    for( i = 0; i < MAX_PCA; i++ )
    {
        dout( "%-5d %-#5x %-#5x %-#10x %-#10x %-5d %-5d %-5d %-10s\n",
        i ,
        all_pca[ i ].rflag,
        all_pca[ i ].addr,
        all_pca[ i ].rmask,
        all_pca[ i ].wmask,
        all_pca[ i ].old,
        all_pca[ i ].cur,
        all_pca[ i ].new,
        all_pca[ i ].spec );

    }
          
    
    return;
}
