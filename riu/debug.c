#include "debug.h"
#include "ty.h"

void print_char( s8_t ch )
{
  printf( "%c", ch );
  
  return;
}

void print_dec( u8_t in )
{
  printf( "%d ", in );
  
  return;
}

void print_hex( u8_t hex )
{
  printf( "%#x ", hex );
  
  return;
}


/* */
void debug_dump( u8_t *buff, u16_t len, void (*print)(u8_t ch ) )
{
  u16_t in = 0;
  
  if( NULL == buff || 0 == len )
    return;
  
  for( in = 0; in < len; in++ )
  {
    out( buff[in] );
  }
  
  out( "\r\n" );

  return;
}
