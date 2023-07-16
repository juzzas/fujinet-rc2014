//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_get_adapter_config(AdapterConfig *ac)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xE8; // Get adaptor config
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.response = (uint8_t *)ac;
    dcb.response_bytes = sizeof(AdapterConfig);

    return fujinet_dcb_exec(&dcb);
}

