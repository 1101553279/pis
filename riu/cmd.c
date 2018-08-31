#include "cmd.h"

/*  station start
 *  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16 
 *  head|   |   |   |   |   |   |   |   |   |    |  |   |    tail|   |
 *      |   cmd |   mon |   hour|   sec |   lineL|  idL |      crc1  |
 *      addr    year    date    min     lineH    idH    state        crc2
 *
 *  emg start
 *  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17
 *  head|   |                                                   tail|   |
 *      |   cmd                                                     crc1|
 *      addr                                                            crc2
 *
 * volume
 * 0    1   2   3   4   5   6   7   8   9   10    
 * head |   |   |   v3  v2  v1      tail.....
 *      |   cmd |
 *      addr    resv
 *
 *
 * */

#if 0
/* parse buff , put result into info */
s8_t parse_frame( un_t *buff, u16_t len, parse_t *info )
{
    if( 0 == buff || 0 == info )
        return -1;

    return 0;
}
#endif
