//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_scan_for_networks(uint8_t *count)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));
    dcb.device = RC2014_DEVICEID_FUJINET;
    dcb.command = 0xFD; // Scan for networks
    dcb.aux1 = 0;
    dcb.aux2 = 0;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.response = count;
    dcb.response_bytes = 1;

    return fujinet_dcb_exec(&dcb);
}

