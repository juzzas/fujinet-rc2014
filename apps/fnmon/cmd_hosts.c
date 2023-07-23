//
// Created by justin on 31/12/2022.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"

#include "console.h"
#include "console_utils.h"
#include "cmd.h"

static HostSlot hosts[FUJINET_MAX_HOST_SLOTS];

enum CommandResult cmd_hosts(char* tokens[], int num_tokens) {
    FUJINET_RC rc = fujinet_get_host_slots(hosts);
    enum CommandResult result = COMMAND_SUCCESS;

    if (rc == FUJINET_RC_OK) {
        for (int i = 0; i < FUJINET_MAX_HOST_SLOTS; i++) {
            HostSlot *host = &hosts[i];
            console_put_uint8(i);
            console_puts(": ");
            console_puts(host->hostname);
            console_puts("\r\n");
        }
    } else {
        result = COMMAND_ERROR_FUJINET;
    }

    return result;
}
