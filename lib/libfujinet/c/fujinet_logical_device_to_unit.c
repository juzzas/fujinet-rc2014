//
// Created by jskists on 02/08/2023.
//
#include "fujinet_logical.h"

uint8_t fujinet_logical_device_to_unit(char const* logical)
{
    if (fujinet_logical_device_to_type(logical) != FUJINET_LOGICAL_DEVICE_TYPE_UNKNOWN) {
        if (logical[3] != ':') {
            // it's a valid logical device with a unit number
            return logical[3] - '0';
        }
    }

    return 0;
}