#include "ptime.h"
#include <time.h>

u32_t ptime_get( void )
{
    return time( 0 );
}
