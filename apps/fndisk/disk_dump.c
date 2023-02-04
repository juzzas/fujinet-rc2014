//
// Created by justin on 31/12/2022.
//
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_disk.h"

#include "disk_list.h"
#include "devices.h"
#include "hosts.h"

uint8_t sector_buffer[DISK_SECTOR_SIZE];

FUJINET_RC do_disk_dump(char* drive_device, char* sector_num)
{
    FUJINET_RC rc;
    uint8_t device_id = atoi(drive_device);
    printf("device_id: = %d\n", device_id);

    uint16_t sector = atoi(sector_num);
    printf("sector: = %d\n", sector);

    rc = fujinet_disk_read(device_id, sector, sector_buffer);
    if (rc == FUJINET_RC_OK) {
        for (int i = 0; i < DISK_SECTOR_SIZE; i += 16) {
            printf("%04x: ", i);
            for (int n = 0; n < 16; n++) {
                printf("%02x", sector_buffer[i + n]);
            }

            printf("    ");
            for (int n = 0; n < 16; n++) {
                printf("%c", isprint(sector_buffer[i + n]) ? sector_buffer[i + n] : '.');
            }

            printf("\n");
        }
    }

    return rc;
}
