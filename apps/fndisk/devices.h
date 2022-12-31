//
// Created by justin on 31/12/2022.
//

#include "fujinet.h"
#include "fujinet_device.h"

#ifndef DEVICES_H
#define DEVICES_H

FUJINET_RC fetch_devices(void);
FUJINET_RC write_devices(void);
DeviceSlot *get_device_at(int i);
void set_device_at(int i, DeviceSlot* device);

#endif //DEVICES_H
