//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"

FUJINET_RC fujinet_get_wifi_status(uint8_t *status)
{
    struct fujinet_dcb dcb;
    memset(&dcb, 0, sizeof(struct fujinet_dcb));
    dcb.device = 0x70;
    dcb.command = 0xFA; // Get WiFi Status
    dcb.aux1 = 0;
    dcb.aux2 = 0;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.response = status;
    dcb.response_bytes = 1;
    return fujinet_dcb_exec(&dcb);
}
