//
// Created by jskists on 16/11/2022.
//
#include <stddef.h>
#include <string.h>

#include "fujinet_network.h"

#define TIMEOUT 30000 /* approx 30 seconds */

extern unsigned char nunit(char* devicespec);

FUJINET_RC fujinet_network_close(char* devicespec)
{
    struct fujinet_dcb dcb;
    unsigned char unit = nunit(devicespec);
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 'C';        // Close
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

    return fujinet_dcb_exec(&dcb);
}

