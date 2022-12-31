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
    if (i < FUJINET_MAX_DEVICE_SLOTS)
        return &devices[i];

    return NULL;
}

void set_device_at(int i, DeviceSlot* device) {

}
