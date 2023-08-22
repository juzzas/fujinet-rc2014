//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_put_device_slots(DeviceSlot *d)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = RC2014_DEVICEID_FUJINET;
    dcb.command = 0xF1;
    dcb.buffer = (uint8_t *)d;
    dcb.buffer_bytes = sizeof(DeviceSlot) * 8;
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

