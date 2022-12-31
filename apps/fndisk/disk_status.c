//
// Created by justin on 31/12/2022.
//

#include <stdio.h>

#include "fujinet.h"
#include "fujinet_device.h"

#include "disk_status.h"
#include "hosts.h"
#include "devices.h"

FUJINET_RC do_disk_status(void) {
    FUJINET_RC rc;
    DeviceSlot* ds;
    HostSlot* hs;
    int i;

    rc = fetch_hosts();
    if (rc == FUJINET_RC_OK) {
        printf("Hosts:\n");
        for (i = 0; i < FUJINET_MAX_HOST_SLOTS; i++) {
            hs = get_host_at(i);
            if (hs)
                printf("%d> %s\n", i, hs->hostname);
        }
    }

    puts("----------------------------------------");

    rc = fetch_devices();
    if (rc == FUJINET_RC_OK) {
        printf("Devices:\n");
        for (i = 0; i < FUJINET_MAX_DEVICE_SLOTS; i++) {
            ds = get_device_at(i);
            if (ds) {
                printf("%d> host:%d mode:%x: %s\n", i,
                       ds->hostSlot,
                       ds->mode,
                       ds->file);
            }
        }
    }

    return rc;
}
