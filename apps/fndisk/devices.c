//
// Created by justin on 31/12/2022.
//
#include <stdlib.h>
#include "fujinet.h"
#include "fujinet_device.h"

#include "devices.h"

static DeviceSlot devices[FUJINET_MAX_DEVICE_SLOTS];

FUJINET_RC fetch_devices(void) {
    return fujinet_get_device_slots(devices);
}

FUJINET_RC write_devices(void) {
    return fujinet_put_device_slots(devices);
}

DeviceSlot* get_device_at(int i) {
    if (i > 0 && i <= FUJINET_MAX_DEVICE_SLOTS)
        return &devices[i-1];

    return NULL;
}

void set_device_at(int i, DeviceSlot* device) {

}

FUJINET_RC set_device_filename_at(int i, char* filename) {
    FUJINET_RC rc = fujinet_set_device_filename((unsigned char)i, filename);
    if (rc == FUJINET_RC_OK)
        rc = fetch_devices();
}
