#include "debug.h"

void print_char( u8_t ch )
{
  dout( "%c", ch );
  
  return;
}

void print_dec( u8_t in )
{
  dout( "%d ", in );
  
  return;
}

void print_hex( u8_t hex )
{
  dout( "%#x ", hex );
  
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
    print( buff[in] );
  }
  
  return;
}
