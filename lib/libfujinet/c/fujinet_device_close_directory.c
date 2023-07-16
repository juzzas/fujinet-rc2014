//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_close_directory(void)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF5;
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

