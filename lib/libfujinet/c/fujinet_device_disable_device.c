//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_disable_device(uint8_t d)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xD4;
    dcb.timeout = 15;
    dcb.aux1 = d;

    return fujinet_dcb_exec(&dcb);
}


