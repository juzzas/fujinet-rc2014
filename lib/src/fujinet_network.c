//
// Created by jskists on 16/11/2022.
//
#include <stddef.h>
#include <string.h>

#include "fujinet_network.h"

#define TIMEOUT 30000 /* approx 30 seconds */

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
    uint16_t login_len = 0;
    uint16_t password_len = 0;

    if (login)
        login_len = strlen(login);

    if (password)
        password_len = strlen(password);

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 0xFD;        // send login
    dcb.buffer  = login_len > 0 ? (uint8_t*)login : NULL;
    dcb.buffer_bytes = login_len < 255 ? login_len : 255 ;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = dcb.buffer_bytes;

    FUJINET_RC rc = fujinet_dcb_exec(&dcb);
    if (rc == FUJINET_RC_OK) {
        memset(&dcb, 0, sizeof(struct fujinet_dcb));
        dcb.device    = 0x70 + unit;      // Fuji Device Identifier
        dcb.command   = 0xFE;        // send password
        dcb.buffer  = password_len > 0 ? (uint8_t*)password : NULL;
        dcb.buffer_bytes = password_len < 255 ? password_len : 255;
        dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
        dcb.aux1 = dcb.buffer_bytes;

        rc = fujinet_dcb_exec(&dcb);
    }

    return rc;
}

FUJINET_RC fujinet_channel_mode(char* devicespec, uint8_t mode)
{
    unsigned char unit = nunit(devicespec);
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 0xFC;        // set channel mode
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1     = OUPDATE;    // Read and write
    dcb.aux2     = mode;      // Channel mode

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_json_query(char* devicespec, uint8_t* buf, uint16_t len)
{
    unsigned char unit = nunit(devicespec);
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 'Q';        // Query
    dcb.buffer    = buf; // eg: N:TCP//
    dcb.buffer_bytes     = 256;        // max size of our device spec
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1     = OUPDATE;    // Read and write
    dcb.aux2     = 0;      // CR/LF translation

    return fujinet_dcb_exec(&dcb);
}


FUJINET_RC fujinet_json_parse(char* devicespec)
{
    unsigned char unit = nunit(devicespec);
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 'P';        // Parse
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1     = OUPDATE;    // Read and write
    dcb.aux2     = 0;      // CR/LF translation

    return fujinet_dcb_exec(&dcb);
}
