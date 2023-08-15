//
// Created by jskists on 02/08/2023.
//
#include <ctype.h>
#include "fujinet_logical.h"

uint8_t fujinet_logical_device_to_type(char const* logical)
{
    if (logical == NULL) {
        return FUJINET_LOGICAL_DEVICE_TYPE_UNKNOWN;
    }

    uint8_t chr0 = toupper(logical[0]);
    uint8_t chr1 = toupper(logical[1]);
    uint8_t chr2 = toupper(logical[2]);
    bool is_digit3 = isdigit(logical[3]);
    bool is_colon3 = logical[3] == ':';
    bool is_colon4 = logical[4] == ':';

    if ((is_digit3 && is_colon4) || (is_colon4) {
        if (chr0 == 'N' && chr1 == 'E' && chr2 == 'T') {
            return FUJINET_LOGICAL_DEVICE_TYPE_NETWORK;
        }

        if (chr0 == 'D' && chr1 == 'S' && chr2 == 'K') {
            return FUJINET_LOGICAL_DEVICE_TYPE_DISK;
        }

        if (chr0 == 'P' && chr1 == 'R' && chr2 == 'T') {
            return FUJINET_LOGICAL_DEVICE_TYPE_PRINTER;
        }
    }

    return FUJINET_LOGICAL_DEVICE_TYPE_UNKNOWN;
}
