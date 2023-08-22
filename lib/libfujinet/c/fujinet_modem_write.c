//
// Created by jskists on 16/11/2022.
//

#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_modem.h"

#include <string.h>


FUJINET_RC fujinet_modem_write(uint8_t modem_unit, uint8_t* buf, uint16_t len)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    if (modem_unit > MAX_MODEM_UNIT)
        return FUJINET_RC_INVALID;

    dcb.device    = RC2014_DEVICEID_MODEM + modem_unit;      // Fuji Device Identifier
    dcb.command   = 'W';        // Write
    dcb.buffer  = buf;
    dcb.buffer_bytes = len;
    dcb.timeout   = FUJINET_TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;
    dcb.aux2 = (len >> 8) & 0xff;

    return fujinet_dcb_exec(&dcb);
}

