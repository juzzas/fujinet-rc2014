//
// Created by justin on 31/12/2022.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"

#include "disk_list.h"
#include "devices.h"
#include "hosts.h"

FUJINET_RC do_disk_mount(char* cpm_drive, char* drive_device, char* mount_mode) {
    FUJINET_RC rc;
    HostSlot* hs = NULL;

    if (cpm_drive[1] = ':') {
        if ((cpm_drive[0] < 'A') || (cpm_drive[0] > 'P')) {
            printf("INVALID DRIVE\n");
            return FUJINET_RC_INVALID;
        }
    }

    uint8_t mode = 0;
    if (strcmp(mount_mode, "RO") == 0) {
        mode = 1;
    } else if (strcmp(mount_mode, "RW") == 0) {
        mode = 2;
    } else if (strcmp(mount_mode, "DEFAULT") == 0) {
        mode = 0;
    } else {
        printf("INVALID MODE\n");
        return FUJINET_RC_INVALID;
    }

    printf("mounting drive %s as device_id: = %s\n", cpm_drive, drive_device);
    uint8_t device_id = atoi(drive_device);
    printf("device_id: = %s\n", device_id);

    rc = fujinet_mount_disk_image(device_id, 0);


    return rc;
}

FUJINET_RC do_disk_mount_all(void) {
    FUJINET_RC rc;
    printf("MOUNT ALL\n");
    rc = fujinet_mount_all();
    return rc;
}
