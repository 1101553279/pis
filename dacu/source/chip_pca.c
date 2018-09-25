#include "chip_pca.h"
#include "debug.h"

/*
no  which       event           op
1   all         interrupt
                                step1: read all pca9539
                                step2: lieterate all pca9539
                                       find one input event  && fill info
                diag_uic78
                                h
                                l
                diag_uic56
                                h
                                l
                uic78
                                h
                                l
                uic56
                                h
                                l
                ip
                                value(0 ~ 255)
                button
                                bid( 1 ~ 4 )
                                            pushed
                                            poped
                ppt
                                pushed
                                poped


    pca_00 output
2               mic
                                open / close
3               loud
                                
                                tip,         // communication tiping
                                com,         // in commnunication 
                                frozen,      // other in communication, frozen close
                                tip_close,   // tip close
                                frozen_close,
                                lsn,         // broadcast or occ listening
                                pa,          // pa_in / pa_out
                                close,       // for all above

4               uic78
                                high level
                                low level
5               uic56
                                high level
                                low level 

6               volume          
                                volume size(0, 1, 2, 3)

    pca_01 output
                none

    pca_10 output
7               led             light / extinguish
*/


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
    all_pca[ 0 ].spec = "uic / loud / volume /mic..";
    all_pca[ 1 ].rmask = PCA01_IMASK;
    all_pca[ 1 ].wmask = 0x0000;
    all_pca[ 1 ].spec = "ip";
    all_pca[ 2 ].rmask = PCA10_IMASK;
    all_pca[ 2 ].wmask = PCA10_OMASK;
    all_pca[ 2 ].spec = "led / button / ppt";
/*
no    rflag addr  rmask      wmask      old   cur   new   spec      
0     0     0     0xf000     0xfff      0     0     0     uic / loud / volume /mic..
1     0     0x1   0xff       0          0     0     0     ip        
2     0     0x2   0x1f       0xf00      0     0     0     led / button / ppt
*/
    return;
}




// only for debug
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
        (0 == all_pca[ i ].spec) ? "null": all_pca[i].spec );

    }
          
    
    return;
}

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
