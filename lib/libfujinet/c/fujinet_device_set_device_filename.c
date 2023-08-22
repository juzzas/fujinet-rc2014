//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_set_device_filename(uint8_t ds, char* e)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = RC2014_DEVICEID_FUJINET;
    dcb.command = 0xE2;
    dcb.timeout = 15;
    dcb.buffer = (uint8_t *)e;
    dcb.buffer_bytes = 256;
    dcb.aux1 = ds;

    return fujinet_dcb_exec(&dcb);
}


