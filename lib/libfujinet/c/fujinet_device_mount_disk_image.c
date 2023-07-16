//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_mount_disk_image(uint8_t ds, uint8_t mode)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF8;
    dcb.timeout = 15;
    dcb.aux1 = ds;
    dcb.aux2 = mode;

    return fujinet_dcb_exec(&dcb);
}
