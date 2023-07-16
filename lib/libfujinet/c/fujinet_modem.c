//
// Created by jskists on 16/11/2022.
//

#include "fujinet.h"
#include "fujinet_modem.h"

#include <string.h>

extern struct fujinet_dcb dcb;

FUJINET_RC fujinet_modem_status(uint8_t modem_unit, struct modem_status *status)
{
    if (modem_unit > MAX_MODEM_UNIT)
        return FUJINET_RC_INVALID;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x50 + modem_unit;      // Fuji Device Identifier
    dcb.command   = 'S';        // Status
    dcb.response  = (uint8_t *)status;
    dcb.response_bytes     = sizeof(struct modem_status);
    dcb.timeout   = FUJINET_TIMEOUT;    // approximately 30 second timeout

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_modem_read(uint8_t modem_unit, uint8_t* buf, uint16_t len)
{
    if (modem_unit > MAX_MODEM_UNIT)
        return FUJINET_RC_INVALID;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x50 + modem_unit;      // Fuji Device Identifier
    dcb.command   = 'R';        // Read
    dcb.response  = buf;
    dcb.response_bytes = len;
    dcb.timeout   = FUJINET_TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;
    dcb.aux2 = (len >> 8) & 0xff;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_modem_write(uint8_t modem_unit, uint8_t* buf, uint16_t len)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    if (modem_unit > MAX_MODEM_UNIT)
        return FUJINET_RC_INVALID;

    dcb.device    = 0x50 + modem_unit;      // Fuji Device Identifier
    dcb.command   = 'W';        // Write
    dcb.buffer  = buf;
    dcb.buffer_bytes = len;
    dcb.timeout   = FUJINET_TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;
    dcb.aux2 = (len >> 8) & 0xff;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_modem_stream(uint8_t modem_unit)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    if (modem_unit > MAX_MODEM_UNIT)
        return FUJINET_RC_INVALID;

    dcb.device = 0x50 + modem_unit;
    dcb.command = 'X';
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}
