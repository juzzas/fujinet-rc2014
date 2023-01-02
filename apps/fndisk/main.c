//
// Created by justin on 31/12/2022.
//

#include <stdio.h>
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"

#include "disk_list.h"
#include "disk_status.h"

HostSlot hosts[FUJINET_MAX_HOST_SLOTS];
DeviceSlot devices[FUJINET_MAX_DEVICE_SLOTS];


int main(int argc, char **argv)
{
    puts("FujiNet module - disk\n");

    fujinet_init();

    FUJINET_RC rc = FUJINET_RC_INVALID;

    if (argc > 1) {
        if (strcmp(argv[1], "LS") == 0) {
            if (argc > 2) {
                rc = do_disk_list(argv[2], "/", "*.*");
            } if (argc > 3) {
                rc = do_disk_list(argv[2], argv[3], "*.*");
            } if (argc > 4) {
                rc = do_disk_list(argv[2], argv[3], argv[4]);
            }
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