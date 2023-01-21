//
// Created by justin on 31/12/2022.
//

#include "fujinet.h"

#ifndef DISK_MOUNT_H
#define DISK_MOUNT_H

FUJINET_RC do_disk_mount(char* cpm_drive, char* drive_device, char* mount_mode);
FUJINET_RC do_disk_mount_all(void);

#endif //DISK_LIST_H
