#include <stdio.h>

#include "fujinet.h"
#include "fujinet_device.h"


/**
 * Reset the ESP
 */

int main(void) {
    printf("Initialising FujiNet module\n");

    fujinet_init();

    return 0;
}
