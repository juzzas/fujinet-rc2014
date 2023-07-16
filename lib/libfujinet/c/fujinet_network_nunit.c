//
// Created by jskists on 16/11/2022.
//
#include <stddef.h>
#include <string.h>

#include "fujinet_network.h"

#define TIMEOUT 30000 /* approx 30 seconds */

unsigned char nunit(char* devicespec)
{
    unsigned char unit = 1;

    // Set unit to 1 unless explicitly specified.
    if (devicespec[1] == ':') {
        unit = 1;
    } else if (devicespec[2] == ':') {
        unit = devicespec[1] - 0x30; // convert from alpha to integer.
    } else {
        unit = 1;
    }

    return unit;
}
