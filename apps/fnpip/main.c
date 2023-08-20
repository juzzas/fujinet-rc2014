/**
 * Network Function Testbed
 */
#include <arch/cpm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "console.h"
#include "fcb.h"
#include "fujinet_logical.h"
#include "fujinet_network.h"

struct cpm_fcb fcb_input;
struct cpm_fcb fcb_output;

struct network_status status;

typedef enum {
    DEVICE_UNKNOWN,
    DEVICE_FILE,
    DEVICE_FN,
    DEVICE_CON,
    DEVICE_LST
} DEVICE_TYPE;

char command[256];
char buffer[256];
char src_drivespec[256];
const char* src;
DEVICE_TYPE src_type = DEVICE_UNKNOWN;
FUJINET_LOGICAL_DEVICE_TYPE src_logical_type = FUJINET_LOGICAL_DEVICE_TYPE_UNKNOWN;
uint8_t src_logical_unit = 0;
const char* src_logical_url = NULL;
const char* dest;
DEVICE_TYPE dest_type = DEVICE_UNKNOWN;
uint16_t src_size = 0;


int in_fujinet(void)
{
    FUJINET_RC rc = fujinet_network_status(src_logical_unit, &status);

    if (status.extended_error != 1) {
        printf("extended error: %d\n", status.extended_error);
        return status.extended_error;
    }

    memset(buffer, 0, 256);

    if (rc == FUJINET_RC_OK) {
        uint16_t l = (status.bytes_waiting_high << 8) + status.bytes_waiting_low;

        //printf("reading %u bytes\n", l);
        if (l > 0) {
            uint16_t read_l = l > 128 ? 128 : l;

            rc = fujinet_network_read(src_logical_unit, buffer, read_l);
            if (rc == FUJINET_RC_OK) {
                src_size += read_l;
                //printf("\rread %u bytes", src_size);
                return 0;
            }
            else
            {
                printf("read error: %d", rc);
            }
        }
    }

    printf("last status: 0x%02x 0x%02x 0x%02x 0x%02x\n",
             status.bytes_waiting_low,
             status.bytes_waiting_high,
             status.protocol_status,
             status.extended_error);

    return -1;
}

int out_file(void)
{
    //printf("write sector\n");
    int rc = fcb_write(&fcb_output, buffer);
    //printf("write sector (rc = %x)\n", rc);

    return 0;
}

int main(int argc, char **argv)
{
//    clrscr();

    printf("FNPIP - The PIP for FujiNet\n");
    fujinet_init();

    if (argc == 1) {
        printf("Usage: FNPIP <DEST>=<SRC#1>,<SRC#2>,...,<SRC#N>\n");
        exit(1);
    }

    strcpy(command, argv[1]);
    printf("* %s\n", command);
    dest = strtok(command, "=");

    if (dest == NULL) {
        dest = "CON:";
        dest_type = DEVICE_CON;
    }
    else
    {
        fcb_parse(&fcb_output, dest);
        fcb_create(&fcb_output);
        dest_type = DEVICE_FILE;
    }

    printf("DEST URI: %s\n", dest);

    src = strtok(NULL, ",");
    if (src != NULL) {
        while (src != NULL && fujinet_logical_device_type(src) == FUJINET_LOGICAL_DEVICE_TYPE_NETWORK) {
            src_logical_type = fujinet_logical_device_type(src);
            src_logical_unit = fujinet_logical_device_unit(src);
            src_logical_url = fujinet_logical_device_url(src);
            printf("SRC URI: %s\n", src);

            if ((strncmp(src_logical_url, "HTTP:", 5) == 0) ||
                    (strncmp(src_logical_url, "HTTPS:", 6) == 0) ||
                    (strncmp(src_logical_url, "FTP:", 4) == 0)) {
                printf("OPEN DRIVESPEC: %s\n", src);
                printf("OPEN RETURN: %02X\n", fujinet_network_open(src_logical_unit, src_logical_url, 0));
                src_type = DEVICE_FN;
            }
            else
            {
                src_type = DEVICE_FILE;
            }

            bool done = false;

            while(!done) {
                switch (src_type) {
                    case DEVICE_FN: {
                        int rc = in_fujinet();
                        if (rc != 0) {
                            if (rc != NETWORK_ERROR_END_OF_FILE) {
                                printf("Network error: %d", rc);
                            }

                            printf("read %u bytes\n", src_size);
                            done = true;
                        }
                        break;
                    }
                    default:
                        break;
                }

                switch (dest_type) {
                    case DEVICE_FILE:
                        if (out_file() != 0) {
                            done = true;
                        }
                        break;
                    default:
                        break;
                }
            }

            if (src_type == DEVICE_FN) {
                fujinet_network_close(src_logical_unit);
            }

            src = strtok(NULL, ",");
        }
    }
    else
    {
        src = "CON:";
    }

    if (dest_type == DEVICE_FILE)
        fcb_close(&fcb_output);


    return 0;
}
