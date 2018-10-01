#include "dcore.h"
#include "event.h"

void dcore_hand( struct dcore *core, u8_t type, u32_t *value )
{
    u32_t vu;

    if( 0 == core || EVENT_NONE == type || 0 == value )
        return;

    vu = *value;        // for operation easyily

    switch( type )
    {
        case EVENT_DIAG:
        case EVENT_IP:
            //nothing for handling
            break;

        case EVENT_COM:
            break;

        case EVENT_OCC:
            break;

        case EVENT_PA:
            break;

        case EVENT_BLSN:
            break;

        case EVENT_PPT:
            break;

        case EVENT_LINK:
            break;

        default:
            break;
    }

    return;
}


