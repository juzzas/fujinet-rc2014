#include <stdio.h>

#include "fujinet.h"
/**
 * Reset the ESP
 */
FUJINET_RC do_reset(void)
{
    struct fujinet_dcb dcb;
    dcb.device = 0x70;
    dcb.unit = 1;
    dcb.command = 0xFF; // reset FujiNet device
    dcb.dstats = 0x40; // Peripheral->Computer
    dcb.buffer = 0x00;
    dcb.timeout = 0x0F; // 15 second timeout
    dcb.bytes = 0;
    dcb.aux1 = 0;
    dcb.aux2 = 0;

    return fujinet_dcb_exec(&dcb);
}

int main(void) {
    printf("Resetting FujiNet module\n");

    fujinet_init();

    FUJINET_RC rc = do_reset();

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
