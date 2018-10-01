#include "event.h"


/* but handle */
static u8_t event_but( u8_t id, u8_t va, u32_t *value )
{
    u8_t type = EVENT_NONE;
    
    switch( id )
    {
        case BUT_ID_PA_OUT:
            if( BUT_PUSHED ==  va )              // pushed
            {
                type = EVENT_PA;
                *value |= EVENT_PA_REP;
                *value |= EVENT_PA_REP_BUT_OUT;
            }
            break;

        case BUT_ID_DCOM:
            if( BUT_POPED == va )               // poped
            {
                type = EVENT_DCOM;
                *value |= EVENT_DCOM_REP;
                *value |= EVENT_DCOM_REP_BUT;
            }
            break;

        case BUT_ID_PCOM:
            if( BUT_POPED == va )               // poped
            {
                type = EVENT_PCOM;
                *value |= EVENT_PCOM_REP;
                *value |= EVENT_PCOM_REP_BUT;
            }
            break;

        case BUT_ID_PA_IN:
            if( BUT_PUSHED == va )               // pushed
            {
                type = EVENT_PA;
                *value |= EVENT_PA_REP;
                *value |= EVENT_PA_REP_BUT_IN;
            }
            break;

        default:
            type = EVENT_NONE;
            break;
    } 
    
    return type;
}

/* chip pca event to event */
static u8_t event_pca( struct chip_event *e, u32_t *value )
{
    u8_t type = EVENT_NONE;

    if( 0 == e || 0 == value )
        return EVENT_NONE;

   *value = 0;    

    switch( e->id )
    {
        case PCA_ID_IN_DIAG_78:
            type = EVENT_DIAG;
            *value |= EVENT_DIAG_78;
            break;

        case PCA_ID_IN_DIAG_56:
            type = EVENT_DIAG;
            *value |= EVENT_DIAG_56;
            break;

        case PCA_ID_IN_UIC_78:
            type = EVENT_DCOM;
            *value |= EVENT_DCOM_REQ;
            if( 1 == e->value )
                *value |= EVENT_DCOM_REQ_RING;
            else 
                *value |= EVENT_DCOM_REQ_FING;
            break;

        case PCA_ID_IN_UIC_56:
            type = EVENT_BLSN;
            if( 1 == e->value )
                *value |= EVENT_BLSN_REQ_START;
            else
                *value |= EVENT_BLSN_REQ_STOP;
            break;

        case PCA_ID_IN_IP:
            type = EVENT_IP;
            *value |= EVENT_IP_REQ_MASK & e->value; 
            break;

        case PCA_ID_IN_BUT:
            type = event_but( BUT_READ_ID(e->value), BUT_READ_VA(e->value), value );
            break;

        case PCA_ID_IN_PPT:
            type = EVENT_PPT;
            if( 1 == e->value )
                *value |= EVENT_PPT_REQ_POPED;
            else
                *value |= EVENT_PPT_REQ_PUSHED;
            break;

        default:
            type = EVENT_NONE;
            break;
    }

    return type;
}

/* net event to event */
static u8_t event_net( struct net_info *n, u32_t *value )
{
    u8_t type = EVENT_NONE;

    if( 0 == n || 0 == value )
        return EVENT_NONE;
    
    *value = 0;

    switch( n->id )
    {
        case NET_IN_ID_OCC:
            type = EVENT_OCC; 
            if( OCC_OFF == n->data.occ )
                *value = EVENT_OCC_REQ_OFF; 
            else 
                *value = EVENT_OCC_REQ_ON;
            break;

        case NET_IN_ID_UIC_IDLE:
            type = EVENT_PA;
            *value = EVENT_PA_REQ_IDLE;
            break;

        case NET_IN_ID_CAB_LINK:
            break;

        case NET_IN_ID_PCOM:
            type = EVENT_PCOM;
            *value |= EVENT_PCOM_REQ;

            if(COM_REQ ==  n->data.com.cmd )
            {
                if( COM_REQ_START == n->data.com.op ) 
                    *value |= EVENT_PCOM_REQ_CON;
                else 
                    *value |= EVENT_PCOM_REQ_UNCON;
            }
            else
            {
                if(  COM_REP_START == n->data.com.op ) 
                    *value |= EVENT_PCOM_REQ_APT;
                else 
                    *value |= EVENT_PCOM_REQ_UNAPT;
            }

            *value |= EVENT_PCOM_REQ_RD_MASK & ((n->data.com.rd_no - 0x30)<<4);
            *value |= EVENT_PCOM_REQ_SD_MASK & ((n->data.com.sd_no - 0x30)<<0);

            break;

        case NET_IN_ID_DCOM:
            type = EVENT_DCOM;
            *value |= EVENT_DCOM_REQ;

            if(COM_REQ ==  n->data.com.cmd )
            {
                if( COM_REQ_START == n->data.com.op ) 
                    *value |= EVENT_DCOM_REQ_CON;
                else 
                    *value |= EVENT_DCOM_REQ_UNCON;
            }
            else
            {
                if(  COM_REP_START == n->data.com.op ) 
                    *value |= EVENT_DCOM_REQ_APT;
                else 
                    *value |= EVENT_DCOM_REQ_UNQPT;
            }

            *value |= EVENT_DCOM_REQ_RD_MASK & ((n->data.com.rd_no - 0x30)<<4);
            *value |= EVENT_DCOM_REQ_SD_MASK & ((n->data.com.sd_no - 0x30)<<0);

            break;

        default:
            type = EVENT_NONE;
            break;
    }

    return type;

}

/* check one event && get one value */
u8_t event_check( u32_t *value )
{
    struct chip_event e;
    struct net_info n;
    u8_t type = EVENT_NONE;

    if( 0 == value )
        return type;

    if( 0 == pca_event( &e ) )
        type = event_pca( &e, value );
    else if ( 0 == net_event( &n )  )
        type = event_net( &n, value );
    else
        type = EVENT_NONE;

    return type;
}
