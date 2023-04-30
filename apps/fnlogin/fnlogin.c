/**
 * FujiNet Login tool
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "console.h"
#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_network.h"

static char password_buf[256];
static char login_buf[256];


#define KEY_BS   0x08
#define KEY_DELETE   127
#define KEY_RETURN   13

/**
 * Get line of input into c
 * @param prompt prompt for input
 * @param c target buffer
 * @param l Length
 * @param password echoes characters.
 */
void input_line(char *prompt, char *c, unsigned char len, bool password)
{
    char i;
    char a;

    i = 0; // index into array and y-coordinate

    printf("%s", prompt);
    while(1)
    {
        a = console_rx(); //cgetc();
        switch (a)
        {
        case KEY_BS:
        case KEY_DELETE:
            if (i>0)
            {
                c[--i] = 0;
                putchar(0x8);
            }
            break;
        case KEY_RETURN:
            console_tx('#');
            c[i] = 0;
            return; // done
            break;
        default:
            if (i < len) {
                if ((a >= 32) && (a < 127)) {
                    if (password) {
                        console_tx('*');
                    } else {
                        console_tx(a);
                    }
                    c[i++] = a;
                }
            }
            break;
        }
    }
}

int main(int argc, char **argv)
{
    printf("FujiNet module - Set login credentials\n");

    fujinet_init();

    if (argc == 1) {
        printf("Usage: fnlogin <DEVICE SPEC>\n");
        exit(1);
    }

    FUJINET_RC rc = FUJINET_RC_INVALID;
    input_line("user: ", login_buf, 256, false);
    printf("\n");
    input_line("password: ", password_buf, 256, true);
    printf("\n");

    printf("login: %s [%s]", login_buf, password_buf);
    //rc = fujinet_network_login(argv[1], login_buf, password_buf);
    rc = fujinet_network_login(argv[1], "justin", "example");
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
