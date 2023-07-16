//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_set_directory_position(DirectoryPosition pos)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xE4;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.aux1 = pos & 0xff;
    dcb.aux2 = (pos >> 8) & 0xff;

    return fujinet_dcb_exec(&dcb);
}

