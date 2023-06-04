/**
 * FujiNet Stream tool
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_modem.h"
#include "fujinet_printer.h"



int main(int argc, char **argv)
{
    printf("FujiNet module - printer test\n");

    (void)argc;
    (void)argv;

    fujinet_init();

    FUJINET_RC rc = FUJINET_RC_INVALID;

    rc = fujinet_printer_write(1, "Hello!\n\r", 8);
    rc = fujinet_printer_write(1, "Justin!\n\r", 8);
    rc = fujinet_printer_write(1, "123\n456\n\r", 9);
    rc = fujinet_printer_write(1, "end\n\r", 5);

    switch (rc) {
        case FUJINET_RC_OK:
            printf("done\n");
            break;

        case FUJINET_RC_TIMEOUT:
            printf("Timed out!\n");
            break;

        case FUJINET_RC_INVALID:
            printf("Invalid parameter!\n");
            break;

        default:
            printf("Unexpected error (%d)\n", rc);
    }

    return 0;
}
