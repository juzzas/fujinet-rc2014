//
// Created by justin on 09/07/2023.
//

/**
 * Modem Function Testbed
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"
#include "fujinet.h"
#include "fujinet_modem.h"

unsigned char buf[1024];
unsigned char txbuf[64];
unsigned char t=0;

struct modem_status status;
static bool done = false;

static uint8_t modem_unit = 0;

char url[256];


void in(void)
{
    if (fujinet_poll_proceed()) {
        FUJINET_RC rc = fujinet_modem_status(modem_unit, &status);

        if (rc == FUJINET_RC_OK) {
            uint16_t l = (status.bytes_waiting_high << 8) + status.bytes_waiting_low;
            if (l == 0 && status.protocol_status == 0)
                done = true;

            //printf("reading %u bytes\n", l);
            if (l > 0) {
                rc = fujinet_modem_read(modem_unit, buf, l);
                //printf("read %u bytes (%u)\n", l, rc);
                if (rc == FUJINET_RC_OK) {
                    for (uint16_t i = 0; i < l; i++) {
                        putchar(buf[i]);
                    }
                }
            }
        }
    }
}

void out(void)
{
    char k;

    k = console_rx_avail();
    if (k > 0) {
        txbuf[0] = console_rx();
        if (txbuf[0] == 26) {
            done = true;
        } else {
            fujinet_modem_write(modem_unit, txbuf, 1); // send one char.
        }
    }
}

int main(int argc, char **argv)
{
    //    clrscr();
    fujinet_init();

    printf("This is a very simple 80 column terminal program.\n");

    printf("OPEN MODEM: %d\n", modem_unit);

    while(!done) {
        in();
        out();
    }

    return 0;
}
