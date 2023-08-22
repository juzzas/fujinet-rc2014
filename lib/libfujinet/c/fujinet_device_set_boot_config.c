//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_set_boot_config(uint8_t toggle)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = RC2014_DEVICEID_FUJINET;
    dcb.command = 0xD9;
    dcb.timeout = 15;
    dcb.aux1 = toggle;

    return fujinet_dcb_exec(&dcb);
}

