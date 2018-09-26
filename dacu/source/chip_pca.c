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

#if 1
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

static struct chip_pca all_pca[ MAX_PCA ] = { 0 };
#else       // for debug
struct chip_pca all_pca[ MAX_PCA ] = { 0 };
#endif

static u8_t event_check( struct chip_pca *pca );
static s8_t pca_update();
static s8_t chip_update( struct chip_pca *pca );
static s8_t event_fill( struct chip_pca *pca, struct chip_event *e );
static u16_t event_value( struct chip_pca *pca, u16_t mask );
static void event_clear( struct chip_pca *pca, u16_t mask, u16_t event );
static u8_t event_is( struct chip_pca *pca, u16_t mask, u16_t event  );
static s8_t pca_read( struct chip_pca *pca, u16_t *value );
static s8_t chip_read( u8_t addr, u8_t reg, u16_t *data );
static s8_t pca_write( struct chip_pca *pca, u16_t mask, u16_t value );
static s8_t chip_write( u8_t addr, u8_t reg, u16_t data );

void pca_init( )
{
    u8_t i = 0;
    
    for( i = 0; i < MAX_PCA; i++ )
    {
//        all_pca[ i ].no = i;
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

/*  for test
    all_pca[ 0 ].cur = I_00_diag_78 | I_00_diag_56 | I_00_78 |
                        I_00_56;
    all_pca[ 1 ].cur = I_01_IP ;
    all_pca[ 2 ].cur = I_10_BUT_01 |I_10_BUT_02 | I_10_BUT_03 | I_10_BUT_04 |I_10_PPT;

    all_pca[ 1 ].cur = 0x1f;
*/

/*
no    rflag addr  rmask      wmask      old   cur   new   spec      
0     0     0     0xf000     0xfff      0     0     0     uic / loud / volume /mic..
1     0     0x1   0xff       0          0     0     0     ip        
2     0     0x2   0x1f       0xf00      0     0     0     led / button / ppt
*/
    return;
}

/* 
set output
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


/* get one event & store event data to e */
s8_t pca_event(struct chip_event *e )
{
    s8_t ret = 0;
    u8_t i = 0;

    if( 0 == e )
        return -1;
    
    pca_update( );

    for( i = 0; i < MAX_PCA; i++ )
    {
//dout( "%d: cur = %#x, old = %#x\n", i, all_pca[i].cur, all_pca[i].old );
        if( event_check( &all_pca[i] ) )
        {
            event_fill( &all_pca[i], e );
            return 0;
        }
    }
    
    e->type = 0;
    e->id = PCA_ID_IN_NONE;
    e->value = 0; 

    return 1;
}

// check ip
s8_t pca_check( u8_t id, u16_t *value )
{   
    s8_t ret = 1;

    if( 0 == value )
        return -1;
    
    switch( id )
    {
        case PCA_ID_CHECK_IP:
            *value = event_value( &all_pca[1], I_01_IP );
            ret = 0;
            break;

        default:
            *value = 0;
            ret = 1;
            break;
    }

    return ret;
}

/* mark for needing to update*/
void pca_rflag( u8_t no )
{
    if( no >= MAX_PCA )
        return;
    
    all_pca[ no ].rflag = 1;

    return;
}




// only for debug
void dump_pca( )
{
    u8_t i = 0;

    dout( "%-5s %-5s %-5s %-10s %-10s %-10s %-10s %-10s %-10s\n",
          "no", "rflag", "addr", "rmask", "wmask", "old", "cur", "new", "spec" );

    for( i = 0; i < MAX_PCA; i++ )
    {
        dout( "%-5d %-#5x %-#5x %-#10x %-#10x %-#10x %-#10x %-#10x %-10s\n",
//        all_pca[ i ].no ,
        i,
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

/*======================= static functions ================*/
static u8_t event_check( struct chip_pca *pca )
{
    return ( 0 !=pca ) &&
        ((pca->cur & pca->rmask) != (pca->old & pca->rmask) );
}

static s8_t chip_write( u8_t addr, u8_t reg, u16_t data )
{

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

    return 0;
}

static s8_t pca_write( struct chip_pca *pca, u16_t mask, u16_t value )
{
    if( 0 == pca )
        return;
    
    pca->new &= ~(mask & pca->wmask);               // clear new
    pca->new |= ((mask & pca->wmask) & value);      // set  new
    pca->cur &= ~pca->wmask;                        // clear cur( write part )
    pca->cur |= pca->new;                           // set cur( write part )

// call chip write
//
    return;
}

static s8_t chip_read( u8_t addr, u8_t reg, u16_t *data )
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

    return 0;
}

static s8_t pca_read( struct chip_pca *pca, u16_t *value )
{
// chip read
// set pca->cur
//
    return;
}

/* decide event */
static u8_t event_is( struct chip_pca *pca, u16_t mask, u16_t event  )
{
    return (pca->cur & pca->rmask & mask & event) != (pca->old & pca->rmask & mask & event );
}

/* set cur -> old */
static void event_clear( struct chip_pca *pca, u16_t mask, u16_t event )
{
/*
    dout( "pca->cur = %#x, pca->rmask = %#x, mask = %#x, event = %#x\n",
           pca->cur, pca->rmask, mask, event );
*/
    pca->old &= ~(pca->rmask & mask );
    
    pca->old |= (pca->cur & pca->rmask & mask & event); 

    return;
}

/* get cur value */
static u16_t event_value( struct chip_pca *pca, u16_t mask )
{
    return pca->cur & pca->rmask & mask;
}

/*fill information: according to event*/
static s8_t event_fill( struct chip_pca *pca, struct chip_event *e )
{
    u8_t id = PCA_ID_IN_NONE;
    u16_t value;
    
    switch( pca->addr )
    {
        case 0x00:
            if( event_is(pca, I_00_diag_78, I_00_diag_78 ) )
            {
                id = PCA_ID_IN_DIAG_78;
                value = event_value( pca, I_00_diag_78 );
                event_clear(pca, I_00_diag_78, I_00_diag_78 );
            }
            else if( event_is(pca, I_00_diag_56, I_00_diag_56 ) )
            {
                id = PCA_ID_IN_DIAG_56;
                value = event_value( pca, I_00_diag_56 );
                event_clear( pca, I_00_diag_56, I_00_diag_56 );
            }
            else if( event_is( pca, I_00_78, I_00_78 ) )
            {
                id = PCA_ID_IN_UIC_78;
                value = event_value( pca, I_00_78 );
                event_clear( pca, I_00_78, I_00_78 );
            }
            else if( event_is( pca, I_00_56, I_00_56 ) )
            {
                id = PCA_ID_IN_UIC_56;
                value = event_value( pca, I_00_56 );
                event_clear( pca, I_00_56, I_00_56 );
            }
            break;

        case 0x01:
            if( event_is( pca, I_01_IP, I_01_IP ) )
            {
                id = PCA_ID_IN_IP;
                value = event_value( pca, PCA01_IMASK );
                event_clear( pca, I_01_IP, I_01_IP );
            }
            break;
        
        case 0x02:
            if( event_is( pca, I_10_PPT_MASK, I_10_PPT) )
            {
                id = PCA_ID_IN_PPT;
                value = event_value( pca, I_10_PPT);
                event_clear( pca, I_10_PPT_MASK, I_10_PPT);
            }
            else if( event_is( pca, I_10_BUT_MASK, I_10_BUT_04 ) )
            {
                id = PCA_ID_IN_BUT;
                value = event_value( pca, I_10_BUT_04 );
                value |= (1 << 11);
                event_clear( pca, I_10_BUT_04, I_10_BUT_04);
            }
            else if( event_is( pca, I_10_BUT_MASK, I_10_BUT_03 ) )
            {
                id = PCA_ID_IN_BUT;
                value = event_value( pca, I_10_BUT_03 );
                value |= (1 << 10);
                event_clear( pca, I_10_BUT_03, I_10_BUT_03);
            }
            else if( event_is( pca, I_10_BUT_MASK, I_10_BUT_02 ) )
            {
                id = PCA_ID_IN_BUT;
                value = event_value( pca, I_10_BUT_02 );
                value |= (1 << 9);
//    dout( "value = %#x, but--02 = %#x\n", value, I_10_BUT_02 );
                event_clear( pca, I_10_BUT_02, I_10_BUT_02);
            }
            else if( event_is( pca, I_10_BUT_MASK, I_10_BUT_01 ) )
            {
                id = PCA_ID_IN_BUT;
                value = event_value( pca, I_10_BUT_01 );
                value |= (1 << 8);
                event_clear( pca, I_10_BUT_01, I_10_BUT_01);
            }
            
            break;
        
        default:
            e->type = 0;
            e->id = PCA_ID_IN_NONE;
            e->value = 0; 
            break;
    }

    if( PCA_ID_IN_NONE == id ) 
        return 1;
    
    e->type = 0; 
    e->id = id;
    e->value = value; 

    return 0;
}

static s8_t chip_update( struct chip_pca *pca )
{
    if(  0 == pca )
        return -1;

//chip read
//set pca cur value

    return 0;
}

// update input
static s8_t pca_update()
{
    u8_t i = 0;
    s8_t ret = 0;

    for( i = 0; i < MAX_PCA; i++ )
    {
        if( 1 == all_pca[i].rflag )         // chip need update
        {
            ret += chip_update( &all_pca[i] );
            0 == all_pca[i].rflag;
        }
    }

    return ret;
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
