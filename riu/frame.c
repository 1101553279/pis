#include "frame.h"
#include <string.h>

/* init head tail addr match information for frame module */
s8_t frame_init( frame_t *fm, un_t head, un_t tail, un_t addr, match_t match )
{
    if( 0 == fm );
        return -1;
    
    fm->head = head;
    fm->tail = tail;
    fm->addr = addr;
    fm->state = 0;
    fm->frame = 0;
    fm->match = match;

    return com_init( &fm->com ); 
}

s8_t frame_put_one( frame_t *fm, un_t da )
{
    s8_t ret = 0;

    if( 0 == fm )
        return -1;
    
     if( 0 != com_push( &fm->com, da ) )
         return -2;

// for frame handle , spical handle, because data foramt: ...| tail | crc1 | crc2 |
     if( fm->tail == da || 0 != fm->state )
         fm->state++;

     if( fm->state >= 3 )
     {
         fm->state = 0;
         fm->frame++;
     }

     return 0;
}

/* get one frame info,, according to match funtion & head ,tail, addr 
 * buff: buffer start address
 * blen: buffer length
 * return: frame length
 * */
u16_t frame_get( frame_t *fm, un_t *buff, u16_t blen )
{
    u16_t len = 0;  // frame length
    u16_t i;        // for loop
    un_t am;        // store pop data
    s8_t ret;       // for pop return
    com_t *com;

    if( 0 == fm || 0 == buff || 0 == blen )
        return 0;

    com = &fm->com; 

    /* ready writing */
//    for( ret=com_pop( com, &am ); (0 == ret && fm->head != am); ret = com_pop( com, &am ) )
    ret = com_pop( com, &am );          //remove head's unuseful data
    while( 0 == ret && fm->head != am )
        ret = com_pop( com, &am );
    
    *(buff + len++) = am;  
    ret = com_pop( com, &am );
    while( 0 == ret && fm->tail != am )
    {
        if( len < blen )
            *(buff + len++) = am;

        ret = com_pop( com, &am );
    }
    
    if( 0 != ret )          // exception exit
        return 0;
    
    if( len < blen )        // store tail unit
        *(buff + len++) = am;
    
    fm->frame--;            // decremenet one frame

    //for 2 crc checksum
    for( i = 0; i < 2; i++ )
    {
        ret = com_pop( com, &am );
        if( 0 == ret && len < blen )
            *(buff + len++ ) = am ;
    }

    return len;
}

u16_t frame_match_get( frame_t *fm, un_t *buff, u16_t blen )
{
    u16_t len = 0;  // frame length

    if( 0 == fm || 0 == buff || 0 == blen )
        return 0;
    
    len = frame_get( fm, buff, blen );
    while( len > 3 && 0 == fm->match(buff[2], fm->addr ) )
        len = frame_get( fm, buff, blen );

    return len;
}


/* set match function */
s8_t frame_match_set( frame_t *fm, match_t match )
{
    if( 0 == fm )
        return -1;

    fm->match = match;

    return 0;
}

/* get frame number */
u8_t frame_frame( frame_t *fm )
{
    if( 0 == fm )
        return 0;

    return fm->frame;
}


/* query fram_t information */
s8_t frame_query( frame_t *fm, frame_t *info )
{
    if( 0 == fm  || 0 == info )
        return -1;

    info->head = fm->head;
    info->tail = fm->tail;
    info->addr = fm->addr;
    info->state = fm->state;
    info->frame = fm->frame;
    info->match = fm->match;

    memcpy( &info->com, &fm->com, sizeof(fm->com) );
    if( 0 != info->com.cache && 0 != fm->com.cache )
        memcpy( info->com.cache, fm->com.cache, fm->com.size );

    return 0;
}
