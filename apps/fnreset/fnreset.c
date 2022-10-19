#include <stdio.h>

#include "fujinet.h"
#include "fujinet_device.h"


/**
 * Reset the ESP
 */

int main(void) {
    printf("Resetting FujiNet module\n");

    fujinet_init();

    FUJINET_RC rc = fujinet_device_reset();

    switch (rc) {
    case FUJINET_RC_OK:
        printf("done\n");
        break;

    case FUJINET_RC_TIMEOUT:
        printf("Timed out!\n");
        break;

    default:
        printf("Unexpected error (%d)\n", rc);
    }

    return 0;
}
