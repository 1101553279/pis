#include "chip_pca.h"
#include "debug.h"


// pca9539 00
#define PCA00_OMASK 0x0FFF
#define O_00_MIC    0X0808
#define O_00_LOUD   0x0417
#define O_00_UIC78S 0x0300
//#define O_00_78SHORT   0x0200
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

#define PCA10_IMASK     0x001F
#define I_10_BUT_MASK   0x000F  // button mask
#define I_10_BUT_01     0x0001  // but: pa in
#define I_10_BUT_02     0x0002  // but: pcom
#define I_10_BUT_03     0x0004  // but: dcom
#define I_10_BUT_04     0x0008  // but: pa out

#define I_10_PPT_MASK   0x0010  // ppt mask
#define I_10_PPT        0x0010  // ppt

#define MAX_PCA 3

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

static s8_t pca_write( struct chip_pca *pca, u16_t mask, u16_t value )
{
    if( 0 == pca )
        return;
    
    pca->new &= ~(mask & pca->wmask);               // clear new
    pca->new |= ((mask & pca->wmask) & value);      // set  new
    pca->cur &= ~pca->wmask;                        // clear cur( write part )
    pca->cur |= pca->new;                           // set cur( write part )

/*
pca->addr, out_reg, pca->cur, 

if(!I2C_Start())return FALSE;               

I2C_SendByte(Dev_Pca9539 | addr | I2C_Write);
I2C_WaitAck();

I2C_SendByte(reg);                          

I2C_WaitAck();

I2C_SendByte((u8)(Dat&0x00ff));
I2C_WaitAck();
I2C_SendByte((u8)(Dat >> 8));
I2C_WaitAck();

I2C_Stop();
*/

    return;
}

static s8_t pca_read( struct chip_pca *pca, u16_t *value )
{
/*
pca->addr, in_reg, pca->cur

if(!I2C_Start())return FALSE;

I2C_SendByte(Dev_Pca9539 | addr | I2C_Write);
  I2C_WaitAck();

I2C_SendByte(reg);

I2C_WaitAck();
Systick_Delay_1ms(2);


if(!I2C_Start())return FALSE;  //start
I2C_SendByte(Dev_Pca9539 | addr | I2C_Read);
I2C_WaitAck();
Systick_Delay_1ms(10);

*Dat = I2C_ReceiveByte();
I2C_Ack();
*Dat |= I2C_ReceiveByte()<<8;
I2C_NoAck();

I2C_Stop();

*/    

    return;
}


/* 
void pca_out( u8_t id, u16_t value );       // set output

s8_t pca_event(struct chip_event *e );        // get one event

s8_t pca_check( u8_t id, u32_t *value );    // check ip

s8_t pca_update();        // update input
*/
void pca_out( u8_t id, u16_t value )
{
    u16_t mask;
    struct chip_pca *pca = 0;

    if( id >= MAX_PCA_ID_OUT )
        return;

    switch( id )
    {
        case PCA_ID_OUT_MIC:
            pca = &all_pca[0];
            mask = O_00_MIC;
            break;

        case PCA_ID_OUT_LOUD:
            pca = &all_pca[0];
            mask = O_00_LOUD;
            break;

        case PCA_ID_OUT_UIC78:
            pca = &all_pca[0];
            mask = O_00_UIC78S;
            break;   

        case PCA_ID_OUT_UIC56:
            pca = &all_pca[0];
            mask = O_00_UIC56;
            break;

        case PCA_ID_OUT_VOLUME:
            pca = &all_pca[0];
            mask = O_00_VOLUME;
            break;
/*                                |-------------------------------|
                                  |                               |
    |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
     16   15  14  13 12  11  10  9   8   7    6   5   4   3   2   1
    led1 1 -> 9
    led2 2 -> 10
    led3 3 -> 11
    led4 4 -> 12
*/
        case PCA_ID_OUT_LED:
            pca = &all_pca[2];
            mask = (value & 0xff00);            
            value = (value & 0xff)<<8;
            break;

        default:
            break;
    }

    if( 0 != pca )
        pca_write( pca, mask, value );

    return;
}

/*
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
*/
s8_t pca_event_fill( struct chip_pca *pca, struct chip_event *e )
{
    
    return 0;
}
s8_t pca_event(struct chip_event *e )
{
    s8_t ret = 0;
    u8_t i = 0;

    if( 0 == e )
        return -1;
   
    for( i = 0; i < MAX_PCA; i++ )
    {
        if( all_pca[i].cur != all_pca[i].old )
        {
            // fille e ;
            return 0;
        }
    }
    
//    e->type =;
    e->id = PCA_ID_IN_NONE;
 
/*
    if( all_pca[0].cur ! = all_pca[0].old )
    {
    }
    else if( all_pca[1].cur != all_pca[1].old )
    {
    }
    else if( all_pca[2].cur != all_pca[2].old )
    {
    }
    else
    {
        ;
    }
*/
    return 0;
}

s8_t pca_check( u8_t id, u32_t *value );    // check ip

s8_t pca_update();        // update input



// only for debug
void dump_pca( )
{
    u8_t i = 0;

    dout( "%-5s %-5s %-5s %-10s %-10s %-5s %-5s %-5s %-10s\n",
          "no", "rflag", "addr", "rmask", "wmask", "old", "cur", "new", "spec" );

    for( i = 0; i < MAX_PCA; i++ )
    {
        dout( "%-5d %-#5x %-#5x %-#10x %-#10x %-#5x %-#5x %-#5x %-10s\n",
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
