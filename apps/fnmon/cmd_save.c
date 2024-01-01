//
// Created by jskists on 04/07/2023.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_file.h"

#include "cmd.h"
#include "console.h"
#include "console_utils.h"


#define FILE_HANDLE 0

static struct file_status status;


FUJINET_RC do_file_save(uint8_t file_handle, uint8_t host_id, char const* filespec, uint8_t* source, uint16_t length) {
    FUJINET_RC rc;
    bool done = false;

    console_puts("Saving &");
    console_put_uint16((uint16_t)source);
    console_puts(",&");
    console_put_uint16((uint16_t)length);
    console_puts(" to \"");
    console_puts(filespec);
    console_puts("\"\r\n");

    rc = fujinet_file_open(file_handle, host_id, filespec, 0);
    while((length > 0) && rc == FUJINET_RC_OK) {
        rc = fujinet_file_status(file_handle, &status);
        if (rc == FUJINET_RC_OK) {
            console_puts("Writing &");
            console_put_uint16((uint16_t)(length));
            console_puts("\r");

            uint16_t bytes_writing = length > 512 ? 512 : length;
            rc = fujinet_file_write(file_handle, source, bytes_writing);
            if (rc == FUJINET_RC_OK) {
                length -= bytes_writing;
            }
        }
    }

    rc = fujinet_file_close(file_handle);
    console_puts("\r\n");

    return rc;
}

enum CommandResult cmd_save(char* tokens[], int num_tokens)
{
    enum CommandResult result = COMMAND_SUCCESS;
    FUJINET_RC rc;

    uint8_t host_id = 0; //atoi(host);
    if (host_id >= NUM_HOST_SLOTS) {
        return COMMAND_ERROR_INVALID_ARGUMENTS;
    }

    rc = fujinet_mount_host_slot(host_id);
    if (rc == FUJINET_RC_OK) {
        if (num_tokens == 2) {
            rc = do_file_save(FILE_HANDLE, host_id, tokens[1], (uint8_t*)0x9000, (uint16_t)1024);
            if (rc != FUJINET_RC_OK) {
                return COMMAND_ERROR_INVALID_ARGUMENTS;
            }
        } else {
            return COMMAND_ERROR_INVALID_ARGUMENTS;
        }
    } else {
        return COMMAND_ERROR_FUJINET;
    }

    return COMMAND_SUCCESS;
}
