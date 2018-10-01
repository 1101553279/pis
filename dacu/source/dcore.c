#include "dcore.h"
#include "event.h"


#define EVENT_PCOM_REQ_MASK
#define EVENT_PCOM_VA(va, mask, sh) (((va)&(mask))>>(sh))

static u8_t event_is( u32_t value, u32_t mask, u32_t cp )
{
    return cp == (value & mask);
}

static u32_t value_is( u32_t value, u32_t mask)
{
    return (value & mask);
}


static void hand_com( struct dcore *core, u32_t *value )
{
    u32_t vu;

    if( 0 == core || 0 == value )
        return;

    vu = *value;        // for operation easyily
    if( event_is(vu, COM_EVENT_PD_MASK, EVENT_PCOM) )
    {
        if( event_is(vu, COM_EVENT_TYPE_MASK, EVENT_PCOM_REP) )
        {
            if( EVENT_PCOM_REP_BUT == value_is(vu, COM_VALUE_PQ_MASK) )
            {
                // pcom button pushed
            }
        }
        else if( event_is(vu, COM_EVENT_TYPE_MASK, EVENT_PCOM_REQ) )
        {
            switch( value_is(vu, COM_VALUE_PQ_MASK) )
            {
                case EVENT_PCOM_REQ_CON:
                    // do pecu connect
                    break;
                case EVENT_PCOM_REQ_UNCON:
                    // do pecu unconnect
                    break;
                case EVENT_PCOM_REQ_APT:
                    // do dacu accept
                    break;
                case EVENT_PCOM_REQ_UNAPT:
                    // do dacu unaccept
                    break;
                default:
                    // do nothing
                    break;
            }
        }
        else
        {
            // do nothing
        }
    }
    else if( event_is(vu, COM_EVENT_PD_MASK, EVENT_DCOM) )
    {
        if( event_is(vu, COM_EVENT_TYPE_MASK, EVENT_DCOM_REP) )
        {
            if( EVENT_DCOM_REP_BUT == value_is(vu, COM_VALUE_PQ_MASK) )
            {
                // dcom button pushed
            }
        }
        else if( event_is(vu, COM_EVENT_TYPE_MASK, EVENT_DCOM_REQ) )
        {
            switch( value_is(vu, COM_VALUE_PQ_MASK) )
            {
                case EVENT_DCOM_REQ_RING:
                    // do uic78 rising
                    break;
                case EVENT_DCOM_REQ_FING:
                    // do uic78 falling
                    break;
                case EVENT_DCOM_REQ_CON:
                    // do dacu connect
                    break;
                case EVENT_DCOM_REQ_UNCON:
                    // do dacu unconnect
                    break;
                case EVENT_DCOM_REQ_APT:
                    // do dacu accept
                    break;
                case EVENT_DCOM_REQ_UNAPT:
                    // do dacu unaccept
                    break;
                default:
                    // do nothing
                    break;
            }
        }
        else
        {
            // do nothing
        }
    }

    return;
}

void dcore_hand( struct dcore *core, u8_t type, u32_t *value )
{
    if( 0 == core || EVENT_NONE == type || 0 == value )
        return;


    switch( type )
    {
        case EVENT_DIAG:
        case EVENT_IP:
            //nothing for handling
            break;

        case EVENT_COM:
            hand_com( core, value );
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


