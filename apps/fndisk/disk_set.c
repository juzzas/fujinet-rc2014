//
// Created by justin on 31/12/2022.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "fujinet.h"
#include "fujinet_device.h"

#include "disk_list.h"
#include "devices.h"
#include "hosts.h"


FUJINET_RC do_disk_set(char* slot, char* path) {
    FUJINET_RC rc;

    uint8_t slot_id = atoi(slot);
    printf("slot_id: = %d\n", slot_id);
    if (slot_id >= FUJINET_MAX_DEVICE_SLOTS) {
        return FUJINET_RC_INVALID;
    }

    rc = set_device_filename_at(slot_id, path);
    if (rc == FUJINET_RC_OK) {
        printf("Set.\n");
    }

    return rc;
}
