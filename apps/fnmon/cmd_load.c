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


#define FILE_HANDLE 1

static struct file_status status;


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
            rc = do_file_load(FILE_HANDLE, tokens[1], (uint8_t*)0x9000);
        } else {
            rc = COMMAND_ERROR_INVALID_ARGUMENTS;
        }
    }

    return rc;
}
