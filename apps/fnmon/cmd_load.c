//
// Created by jskists on 04/07/2023.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"

#include "cmd.h"
#include "console.h"
#include "console_utils.h"


static struct file_status
{
    uint32_t file_size;
    uint32_t file_pointer;
    uint8_t file_error_code;
} status;

#define FILE_HANDLE 1
#define TIMEOUT 15

static struct fujinet_dcb dcb;

FUJINET_RC fujinet_file_open(unsigned char file_handle, char const* filespec, unsigned char trans)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x30 + file_handle;      // Fuji Device Identifier
    dcb.command   = 'O';        // Open
    dcb.buffer    = (uint8_t *)filespec; // eg: MYFILE.CMD
    dcb.buffer_bytes     = 256;        // max size of our device spec
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1     = 0;    // Read and write
    dcb.aux2     = trans;      // CR/LF translation

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_file_close(unsigned char file_handle)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + file_handle;      // Fuji Device Identifier
    dcb.command   = 'C';        // Close
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_file_status(unsigned char file_handle, struct file_status *status)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + file_handle;      // Fuji Device Identifier
    dcb.command   = 'S';        // Status
    dcb.response  = (uint8_t *)status;
    dcb.response_bytes     = sizeof(struct file_status);
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_file_read(unsigned char file_handle, uint8_t* buf, uint16_t len)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + file_handle;      // Fuji Device Identifier
    dcb.command   = 'R';        // Read
    dcb.response  = buf;
    dcb.response_bytes = len;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;
    dcb.aux2 = (len >> 8) & 0xff;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC do_file_load(uint8_t file_handle, char const* filespec, uint8_t *destination) {
    FUJINET_RC rc;
    bool done = false;

    rc = fujinet_file_open(file_handle, filespec, 0);
    while(!done && rc == FUJINET_RC_OK) {
        rc = fujinet_file_status(file_handle, &status);
        if (rc == FUJINET_RC_OK) {
            uint16_t bytes_waiting = (status.file_size - status.file_pointer) > 512 ? 512 : (uint16_t)(status.file_size - status.file_pointer);
            if (bytes_waiting > 0) {
                rc = fujinet_file_read(file_handle, destination, bytes_waiting);
                if (rc == FUJINET_RC_OK) {
                    destination += bytes_waiting;
                }
            } else {
                done = true;
            }
        }
    }

    rc = fujinet_file_close(file_handle);

    return rc;
}

enum CommandResult cmd_load(char* tokens[], int num_tokens)
{
    FUJINET_RC rc;

    uint8_t host_id = 1; //atoi(host);
    if (host_id >= FUJINET_MAX_HOST_SLOTS) {
        return COMMAND_ERROR_INVALID_ARGUMENTS;
    }

    rc = fujinet_mount_host_slot(host_id);
    if (rc == FUJINET_RC_OK) {
        if (num_tokens == 1) {
            rc = do_file_load(FILE_HANDLE, tokens[1], 0x9000);
        } else {
            rc = COMMAND_ERROR_INVALID_ARGUMENTS;
        }
    }

    return rc;
}
