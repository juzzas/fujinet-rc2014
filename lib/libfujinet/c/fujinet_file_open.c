//
// Created by jskists on 27/07/2023.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_file.h"

#define TIMEOUT 15

FUJINET_RC fujinet_file_open(uint8_t file_handle, uint8_t host_id, char const* filespec, uint8_t mode)
{
    struct fujinet_dcb dcb;
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = RC2014_DEVICEID_FILE + file_handle;      // Fuji Device Identifier
    dcb.command   = 'O';        // Open
    dcb.buffer    = (uint8_t *)filespec; // eg: MYFILE.CMD
    dcb.buffer_bytes     = 256;        // max size of our device spec
    dcb.timeout   = TIMEOUT;    // approximately 15 second timeout
    dcb.aux1     = mode;    // Read and write
    dcb.aux2     = host_id;

    return fujinet_dcb_exec(&dcb);
}
