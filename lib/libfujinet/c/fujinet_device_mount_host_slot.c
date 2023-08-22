//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_mount_host_slot(unsigned char hs)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = RC2014_DEVICEID_FUJINET;
    dcb.command = 0xF9;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.aux1 = hs;

    return fujinet_dcb_exec(&dcb);
}
