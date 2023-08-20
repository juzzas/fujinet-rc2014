//
// Created by jskists on 02/08/2023.
//
#include <stdlib.h>
#include <string.h>

#include "fujinet_logical.h"

char* fujinet_logical_device_url(char const* logical)
{
    char* url = NULL;

    if (fujinet_logical_device_type(logical) != FUJINET_LOGICAL_DEVICE_TYPE_UNKNOWN) {
        url = strchr(logical, ':') + 1;
    }

    return url;
}