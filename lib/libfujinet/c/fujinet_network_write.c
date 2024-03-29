//
// Created by jskists on 16/11/2022.
//
#include <stddef.h>
#include <string.h>

#include "fujinet_device.h"
#include "fujinet_network.h"

#define TIMEOUT 30000 /* approx 30 seconds */

FUJINET_RC fujinet_network_write(uint8_t network_unit, uint8_t* buf, uint16_t len)
{
    struct fujinet_dcb dcb;
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = RC2014_DEVICEID_NETWORK + network_unit;      // Fuji Device Identifier
    dcb.command   = 'W';        // Write
    dcb.buffer  = buf;
    dcb.buffer_bytes = len;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;
    dcb.aux2 = (len >> 8) & 0xff;

    return fujinet_dcb_exec(&dcb);
}

