//
// Created by justin on 31/12/2022.
//

#include "fujinet.h"
#include "fujinet_device.h"

#ifndef HOSTS_H
#define HOSTS_H

FUJINET_RC fetch_hosts(void);
FUJINET_RC write_hosts(void);
HostSlot* get_host_at(int i);
void set_host_at(int i, HostSlot* host);

#endif //HOSTS_H
