//
// Created by justin on 31/12/2022.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"

#include "cmd.h"

static HostSlot hosts[FUJINET_MAX_HOST_SLOTS];

static struct fujinet_dcb dcb;
FUJINET_RC fujinet_get_host_slots(HostSlot *h)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF4;
    dcb.response = (uint8_t *)h;
    dcb.response_bytes = sizeof(HostSlot) * 8;
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

enum CommandResult cmd_hosts(char* tokens[], int num_tokens) {
    FUJINET_RC rc = fujinet_get_host_slots(hosts);
    enum CommandResult result = COMMAND_SUCCESS;

    if (rc == FUJINET_RC_OK) {
        for (int i = 0; i < FUJINET_MAX_HOST_SLOTS; i++) {
            HostSlot *host = &hosts[i];
            printf("%d: %s\n", i, host->hostname);
        }
    } else {
        result = COMMAND_ERROR_FUJINET;
    }

    return result;
}
