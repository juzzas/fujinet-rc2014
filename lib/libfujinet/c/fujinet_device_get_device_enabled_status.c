//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_get_device_enabled_status(uint8_t d, uint8_t* response)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = RC2014_DEVICEID_FUJINET;
    dcb.command = 0xD1;
    dcb.timeout = 15;
    dcb.aux1 = d;
    dcb.response = (uint8_t *)response;
    dcb.response_bytes = 1;

    return fujinet_dcb_exec(&dcb);
}

