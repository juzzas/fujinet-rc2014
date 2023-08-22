//
// Created by jskists on 16/11/2022.
//
#include <stddef.h>
#include <string.h>

#include "fujinet_device.h"
#include "fujinet_network.h"

#define TIMEOUT 30000 /* approx 30 seconds */


FUJINET_RC fujinet_network_close(uint8_t network_unit)
{
    struct fujinet_dcb dcb;
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = RC2014_DEVICEID_NETWORK + network_unit;      // Fuji Device Identifier
    dcb.command   = 'C';        // Close
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

    return fujinet_dcb_exec(&dcb);
}

