//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


char open_dir_buffer[258];
FUJINET_RC fujinet_open_directory(unsigned char hs, char *p, char *f)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));
    char *e;

    memset(&open_dir_buffer,0,258);

    strcpy(open_dir_buffer, p);
    e = open_dir_buffer;

    if (f[0]!=0x00) {
        while (*e != 0x00)
            e++;

        e++;
        strcpy(e,f);
    }

    dcb.device = 0x70;
    dcb.command = 0xF7;
    dcb.buffer = (uint8_t *)open_dir_buffer;
    dcb.buffer_bytes = 256;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.aux1 = hs;

    return fujinet_dcb_exec(&dcb);
}

