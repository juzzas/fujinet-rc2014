//
// Created by jskists on 16/11/2022.
//

#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_printer.h"

#include <string.h>

#define TIMEOUT 15000 /* approx 15 seconds */

FUJINET_RC fujinet_printer_write(uint8_t unit, uint8_t* buf, uint16_t len)
{
    struct fujinet_dcb dcb;
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = RC2014_DEVICEID_PRINTER + unit;
    dcb.command   = 'W';        // Write
    dcb.buffer  = buf;
    dcb.buffer_bytes = len;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;

    return fujinet_dcb_exec(&dcb);
}
