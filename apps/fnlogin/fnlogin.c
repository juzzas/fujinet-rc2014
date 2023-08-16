/**
 * FujiNet Login tool
 */

#pragma printf = "%s"
#pragma scanf  = "%["

#include <adt.h>
#include <stdio.h>
#include <stropts.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_logical.h"
#include "fujinet_network.h"



static char password_buf[256];
static char login_buf[256];
b_array_t edit_buffer;

int main(int argc, char **argv)
{
    printf("FujiNet module - Set login credentials\n");

    fujinet_init();

    if (argc == 1) {
        printf("Usage: fnlogin <DEVICE SPEC>\n");
        exit(1);
    }

    if (fujinet_logical_device_type(argv[1]) != FUJINET_LOGICAL_DEVICE_TYPE_NETWORK) {
        printf("Invalid device spec\n");
        exit(1);
    }

    ioctl(0, IOCTL_ITERM_GET_EDITBUF, &edit_buffer);

    printf("\n=====\n\nUsername: ");

    fflush(stdin);
    scanf("%19[^\n]", login_buf);

    printf("\nPassword: ");

    ioctl(0, IOCTL_ITERM_PASS, 1);

    fflush(stdin);
    scanf("%19[^\n]", password_buf);

    ioctl(0, IOCTL_ITERM_PASS, 0);

    printf("\n");

    FUJINET_RC rc = fujinet_network_login(fujinet_logical_device_unit(argv[1]),
                                          login_buf, password_buf);
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

    case FUJINET_RC_NO_COMPLETE:
        printf("Command did not complete!\n");
            break;

    default:
        printf("Unexpected error (%d)\n", rc);
    }

    return 0;
}
