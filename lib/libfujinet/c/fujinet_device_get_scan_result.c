//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_get_scan_result(uint8_t n, SSIDInfo *info)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));
    dcb.device = RC2014_DEVICEID_FUJINET;
    dcb.command = 0xFC; // Scan for networks
    dcb.aux1 = n;
    dcb.aux2 = 0;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.response = (uint8_t *)info;
    dcb.response_bytes = 33; //sizeof(SSIDInfo);

    return fujinet_dcb_exec(&dcb);
}
