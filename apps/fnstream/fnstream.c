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

NetConfig nc;
AdapterConfig adapter;

struct device {
    char device;
    uint8_t device_id;
    uint8_t device_unit_max;
};

struct device devices[] = {
   // { 'C', 0x5A, 1 },
    { 'M', 0x50, 1 },
    { 'P', 0x40, 4 },
    { 0, 0, 0 }
};

uint8_t device_to_id(char* devicespec)
{
    struct device* i = devices;

    while (i->device != 0) {
        if (devicespec[0] == i->device) {
            return i->device_id;
        }
        i++;
    }

    return 0;
}

uint8_t get_unit(char* devicespec)
{
    unsigned char unit = 1;

    // Set unit to 1 unless explicitly specified.
    if (devicespec[1] == ':') {
        unit = 1;
    } else if (devicespec[2] == ':') {
        unit = devicespec[1] - 0x30; // convert from alpha to integer.
    } else {
        unit = 1;
    }

    return unit;
}

uint8_t get_device_id(char* devicespec)
{
    uint8_t device_id = 0;
    uint8_t unit = get_unit(devicespec);

    device_id = device_to_id(devicespec);

    return device_id;
}


int main(int argc, char **argv)
{
    printf("FujiNet module - Set stream\n");

    fujinet_init();

    if (argc == 1) {
        printf("Usage: fnstream <DEVICE SPEC>\n");
        exit(1);
    }

    FUJINET_RC rc = FUJINET_RC_INVALID;


    char device = argv[1][0];
    uint8_t unit = get_unit(argv[1]);

    printf("OPEN DEVICE FOR STREAM: %c%c:\n", device, unit);

    switch (device) {
    case 'P':
        rc = fujinet_printer_stream(unit);
        break;

    case 'M':
        rc = fujinet_modem_stream(unit);
        break;

    default:
        rc = FUJINET_RC_INVALID;
    }

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
