//
// Created by jskists on 16/11/2022.
//
#include <stddef.h>
#include <string.h>

#include "fujinet_network.h"

#define TIMEOUT 30000 /* approx 30 seconds */

FUJINET_RC fujinet_json_parse(uint8_t network_unit)
{
    struct fujinet_dcb dcb;
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x71 + network_unit;      // Fuji Device Identifier
    dcb.command   = 'P';        // Parse
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1     = OUPDATE;    // Read and write
    dcb.aux2     = 0;      // CR/LF translation

    return fujinet_dcb_exec(&dcb);
}
