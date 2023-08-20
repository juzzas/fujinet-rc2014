//
// Created by jskists on 02/08/2023.
//

#ifndef FUJINET_LOGICAL_H
#define FUJINET_LOGICAL_H

#include <stdint.h>

typedef enum fujinet_logical_device_type {
    FUJINET_LOGICAL_DEVICE_TYPE_UNKNOWN = 0,
    FUJINET_LOGICAL_DEVICE_TYPE_NETWORK = 1,
    FUJINET_LOGICAL_DEVICE_TYPE_DISK = 2,
    FUJINET_LOGICAL_DEVICE_TYPE_PRINTER = 3,
} FUJINET_LOGICAL_DEVICE_TYPE;

FUJINET_LOGICAL_DEVICE_TYPE fujinet_logical_device_type(char const* logical);
uint8_t fujinet_logical_device_unit(char const* logical);
char* fujinet_logical_device_url(char const* logical);

#endif //FUJINET_LOGICAL_H
