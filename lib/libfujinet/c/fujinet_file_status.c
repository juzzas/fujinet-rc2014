//
// Created by jskists on 27/07/2023.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_file.h"

#define TIMEOUT 15

FUJINET_RC fujinet_file_status(uint8_t file_handle, struct file_status *status)
{
    struct fujinet_dcb dcb;
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = RC2014_DEVICEID_FILE + file_handle;      // Fuji Device Identifier
    dcb.command   = 'S';        // Status
    dcb.response  = (uint8_t *)status;
    dcb.response_bytes     = sizeof(struct file_status);
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

    return fujinet_dcb_exec(&dcb);
}

