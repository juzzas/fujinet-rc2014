//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"

FUJINET_RC fujinet_get_ssid(NetConfig *nc)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xFE;
    dcb.response = (uint8_t *)nc;
    dcb.response_bytes = sizeof(NetConfig);
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

