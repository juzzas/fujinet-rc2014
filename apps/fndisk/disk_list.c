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

char direntry[256];

FUJINET_RC do_dirlist(uint8_t host_id, char* prefix, char* filter) {
    FUJINET_RC rc;
    bool done = false;

    rc = fujinet_open_directory(host_id, prefix, filter);
    printf("Opened (%d)...\n", rc);

    while(!done && rc == FUJINET_RC_OK) {
        rc = fujinet_read_directory(direntry, 80, 0);
        if (direntry[0] == 0x7f && direntry[1] == 0x7f) {
            done = true;
        }
        else
        {
            printf("%s", direntry);
        }
    }

    rc = fujinet_close_directory();

    return rc;
}

FUJINET_RC do_disk_list(char* host, char* prefix, char* filter) {
    FUJINET_RC rc;
    HostSlot* hs = NULL;

    uint8_t host_id = atoi(host);
    printf("host_id: = %d\n", host_id);
    if (host_id >= FUJINET_MAX_HOST_SLOTS) {
        return FUJINET_RC_INVALID;
    }

    rc = fetch_hosts();
    if (rc == FUJINET_RC_OK) {
        hs = get_host_at(host_id);
        if (hs) {
            printf("Host: %s\n", hs->hostname);

            rc = fujinet_mount_host_slot(host_id);
            if (rc == FUJINET_RC_OK) {
                printf("Mounted...\n");
                rc = do_dirlist(host_id, prefix, filter);
            }

        }
    }
    return FUJINET_RC_OK;
}
