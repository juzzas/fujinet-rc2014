//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_read_directory(char *dirent, unsigned char l, unsigned char a)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF6;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.aux1 = l;
    dcb.aux2 = a;
    dcb.response = (uint8_t *)dirent;
    dcb.response_bytes = l;

    return fujinet_dcb_exec(&dcb);
}

