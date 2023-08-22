//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_get_device_filename(uint8_t ds, char* buffer)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = RC2014_DEVICEID_FUJINET;
    dcb.command = 0xDA;
    dcb.timeout = 15;
    dcb.response = (uint8_t*)buffer;
    dcb.response_bytes = 256;
    dcb.aux1 = ds;

    return fujinet_dcb_exec(&dcb);
}

