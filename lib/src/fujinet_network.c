//
// Created by jskists on 16/11/2022.
//
#include <stddef.h>
#include <string.h>

#include "fujinet_network.h"

#define TIMEOUT 0x1f /* approx 30 seconds */

extern struct fujinet_dcb dcb;

unsigned char nunit(char* devicespec)
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

FUJINET_RC fujinet_network_open(char* devicespec, unsigned char trans)
{
    unsigned char unit = nunit(devicespec);
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 'O';        // Open
    dcb.buffer    = (uint8_t *)devicespec; // eg: N:TCP//
    dcb.buffer_bytes     = 256;        // max size of our device spec
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1     = OUPDATE;    // Read and write
    dcb.aux2     = trans;      // CR/LF translation

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_network_close(char* devicespec)
{
    unsigned char unit = nunit(devicespec);
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 'C';        // Close
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_network_status(char* devicespec, struct network_status *status)
{
    unsigned char unit = nunit(devicespec);
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 'S';        // Status
    dcb.response  = (uint8_t *)status;
    dcb.response_bytes     = sizeof(struct network_status);
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_network_read(char* devicespec, uint8_t* buf, uint16_t len)
{
    unsigned char unit = nunit(devicespec);
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 'R';        // Read
    dcb.response  = buf;
    dcb.response_bytes = len;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;
    dcb.aux2 = (len >> 8) & 0xff;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_network_write(char* devicespec, uint8_t* buf, uint16_t len)
{
    unsigned char unit = nunit(devicespec);
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 'W';        // Write
    dcb.buffer  = buf;
    dcb.buffer_bytes = len;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;
    dcb.aux2 = (len >> 8) & 0xff;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_network_login(char* devicespec, char* login, char* password)
{
    unsigned char unit = nunit(devicespec);
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 0xFD;        // send login
    dcb.buffer  = (uint8_t*)login;
    dcb.buffer_bytes = 256;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

    FUJINET_RC rc = fujinet_dcb_exec(&dcb);
    if (rc == FUJINET_RC_OK) {
        dcb.device    = 0x70 + unit;      // Fuji Device Identifier
        dcb.command   = 0xFE;        // send password
        dcb.buffer  = (uint8_t*)password;
        dcb.buffer_bytes = 256;
        dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

        rc = fujinet_dcb_exec(&dcb);
    }

    return rc;
}