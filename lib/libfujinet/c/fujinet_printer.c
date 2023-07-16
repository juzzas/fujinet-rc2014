//
// Created by jskists on 16/11/2022.
//

#include "fujinet.h"
#include "fujinet_printer.h"

#include <string.h>

#define TIMEOUT 15000 /* approx 15 seconds */

extern struct fujinet_dcb dcb;

FUJINET_RC fujinet_printer_stream(uint8_t printer_unit)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    if (printer_unit > MAX_PRINTER_UNIT)
        return FUJINET_RC_INVALID;

    dcb.device = 0x40 + printer_unit;
    dcb.command = 'X';
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_printer_write(uint8_t unit, uint8_t* buf, uint16_t len)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x40 + unit;      // Fuji Device Identifier
    dcb.command   = 'W';        // Write
    dcb.buffer  = buf;
    dcb.buffer_bytes = len;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;

    return fujinet_dcb_exec(&dcb);
}
