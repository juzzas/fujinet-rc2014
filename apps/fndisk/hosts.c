//
// Created by justin on 31/12/2022.
//
#include <stdlib.h>
#include "fujinet.h"
#include "fujinet_device.h"

#include "hosts.h"

static HostSlot hosts[FUJINET_MAX_HOST_SLOTS];

FUJINET_RC fetch_hosts(void) {
    return fujinet_get_host_slots(hosts);
}

FUJINET_RC write_hosts(void) {
    return fujinet_put_host_slots(hosts);
}

HostSlot* get_host_at(int i) {
    if (i > 0 && i <= FUJINET_MAX_HOST_SLOTS)
        return &hosts[i-1];

    return NULL;
}

void set_host_at(int i, HostSlot* host) {

}
