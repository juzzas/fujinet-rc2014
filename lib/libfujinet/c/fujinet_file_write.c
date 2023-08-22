//
// Created by jskists on 27/07/2023.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_file.h"

#define TIMEOUT 15

FUJINET_RC fujinet_file_write(uint8_t file_handle, uint8_t* buf, uint16_t len)
{
    struct fujinet_dcb dcb;
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = RC2014_DEVICEID_FILE + file_handle;      // Fuji Device Identifier
    dcb.command   = 'W';        // Write:w
    dcb.buffer  = buf;
    dcb.buffer_bytes = len;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;
    dcb.aux2 = (len >> 8) & 0xff;

    return fujinet_dcb_exec(&dcb);
}
