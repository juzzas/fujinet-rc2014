//
// Created by justin on 31/12/2022.
//

#include <stdio.h>
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"

#include "disk_dump.h"
#include "disk_list.h"
#include "disk_mount.h"
#include "disk_set.h"
#include "disk_status.h"
#include "fat_dir.h"

HostSlot hosts[FUJINET_MAX_HOST_SLOTS];
DeviceSlot devices[FUJINET_MAX_DEVICE_SLOTS];


int main(int argc, char **argv)
{
    puts("FujiNet module - disk\n");

    fujinet_init();

    FUJINET_RC rc = FUJINET_RC_INVALID;

    if (argc > 1) {
        if (strcmp(argv[1], "LIST") == 0) {
            if (argc > 4) {
                rc = do_disk_list(argv[2], argv[3], argv[4]);
            } else if (argc > 3) {
                rc = do_disk_list(argv[2], argv[3], "*.*");
            } else if (argc > 2) {
                rc = do_disk_list(argv[2], "/", "*.*");
            } else {
                rc = FUJINET_RC_INVALID;
            }
        } else if (strcmp(argv[1], "SET") == 0) {
            if (argc > 3) {
                rc = do_disk_set(argv[2], argv[3]);
            } else if (argc > 2) {
                rc = do_disk_set(argv[2], "");
            } else {
                rc = FUJINET_RC_INVALID;
            }
        } else if (strcmp(argv[1], "MOUNTALL") == 0) {
            rc = do_disk_mount_all();
        } else if (strcmp(argv[1], "MOUNT") == 0) {
            if (argc > 4) {
                rc = do_disk_mount(argv[2], argv[3], argv[4]);
            } else if (argc > 3) {
                rc = do_disk_mount(argv[2], argv[3], "DEFAULT");
            } else {
                rc = FUJINET_RC_INVALID;
            }
        } else if (strcmp(argv[1], "DUMP") == 0) {
            if (argc > 3) {
                rc = do_disk_dump(argv[2], argv[3]);
            } else if (argc > 2) {
                rc = do_disk_dump(argv[2], 0);
            } else {
                rc = FUJINET_RC_INVALID;
            }
        } else if (strcmp(argv[1], "FATDIR") == 0) {
            rc = do_fat_dir();
        }
    } else {
        rc = do_disk_status();
    }
    switch (rc) {
    case FUJINET_RC_OK:
        puts("done\n");
        break;

    case FUJINET_RC_TIMEOUT:
        puts("Timed out!\n");
        break;

    case FUJINET_RC_INVALID:
        puts("Invalid parameter!\n");
        break;

    default:
        printf("Unexpected error (%d)\n", rc);
    }

    //#asm
    //    rst 0
    //#endasm
    return 0;
}