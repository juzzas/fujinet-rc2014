//
// Created by jskists on 27/07/2023.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_file.h"

#define TIMEOUT 15

FUJINET_RC fujinet_file_close(uint8_t file_handle)
{
    struct fujinet_dcb dcb;
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + file_handle;      // Fuji Device Identifier
    dcb.command   = 'C';        // Close
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

    return fujinet_dcb_exec(&dcb);
}
