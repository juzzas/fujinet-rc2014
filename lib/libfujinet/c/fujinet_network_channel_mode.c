//
// Created by jskists on 16/11/2022.
//
#include <stddef.h>
#include <string.h>

#include "fujinet_network.h"

#define TIMEOUT 30000 /* approx 30 seconds */

extern unsigned char nunit(char* devicespec);

FUJINET_RC fujinet_channel_mode(char* devicespec, uint8_t mode)
{
    struct fujinet_dcb dcb;
    unsigned char unit = nunit(devicespec);
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 0xFC;        // set channel mode
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1     = OUPDATE;    // Read and write
    dcb.aux2     = mode;      // Channel mode

    return fujinet_dcb_exec(&dcb);
}

