//
// Created by jskists on 16/11/2022.
//
#include <stddef.h>
#include <string.h>

#include "fujinet_network.h"

#define TIMEOUT 30000 /* approx 30 seconds */


FUJINET_RC fujinet_network_status(uint8_t network_unit, struct network_status *status)
{
    struct fujinet_dcb dcb;
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x71 + network_unit;      // Fuji Device Identifier
    dcb.command   = 'S';        // Status
    dcb.response  = (uint8_t *)status;
    dcb.response_bytes     = sizeof(struct network_status);
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

    return fujinet_dcb_exec(&dcb);
}

