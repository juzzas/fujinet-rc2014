//
// Created by jskists on 16/11/2022.
//
#include <stddef.h>

#define TIMEOUT 0x1f /* approx 30 seconds */

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
  struct fujinet_dcb dcb = {};

  dcb.device    = 0x70 + unit;      // Fuji Device Identifier
  dcb.command   = 'O';        // Open
  dcb.buffer    = devicespec; // eg: N:TCP//
  dcb.response_bytes     = 256;        // max size of our device spec
  dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
  dcb.daux1     = OUPDATE;    // Read and write
  dcb.daux2     = trans;      // CR/LF translation

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_network_close(char* devicespec)
{
  unsigned char unit = nunit(devicespec);
  struct fujinet_dcb dcb = {};

  dcb.device    = 0x70 + unit;      // Fuji Device Identifier
  dcb.command   = 'C';        // Close
  dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

  return fujinet_dcb_exec(&dcb);
}

unsigned char fujinet_network_status(char* devicespec, struct network_status *status)
{
  unsigned char unit = nunit(devicespec);
  struct fujinet_dcb dcb = {};

  dcb.device    = 0x70 + unit;      // Fuji Device Identifier
  dcb.command   = 'S';        // Status
  dcb.response  = status;
  dcb.response_bytes     = sizeof(struct network_status);
  dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

  return fujinet_dcb_exec(&dcb);
}

unsigned char fujinet_network_read(char* devicespec, uint8_t* buf, uint16_t len)
{
    unsigned char unit = nunit(devicespec);
    struct fujinet_dcb dcb = {};

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 'R';        // Read
    dcb.response  = status;
    dcb.response_bytes = len;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;
    dcb.aux2 = (len >> 8) & 0xff;

    return fujinet_dcb_exec(&dcb);
}

unsigned char fujinet_network_write(char* devicespec, uint8_t* buf, uint16_t len)
{
    unsigned char unit = nunit(devicespec);
    struct fujinet_dcb dcb = {};

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 'W';        // Write
    dcb.buffer  = status;
    dcb.buff_bytes = len;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;
    dcb.aux2 = (len >> 8) & 0xff;

    return fujinet_dcb_exec(&dcb);
}

unsigned char fujinet_network_login(char* devicespec, char* login, char* password)
{
    unsigned char unit = nunit(devicespec);
    struct fujinet_dcb dcb = {};

    dcb.device    = 0x70 + unit;      // Fuji Device Identifier
    dcb.command   = 0xFD;        // send login
    dcb.buffer  = login;
    dcb.buff_bytes = 256;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = len & 0xff;
    dcb.aux2 = (len >> 8) & 0xff;

    rc = fujinet_dcb_exec(&dcb);

    if (rc == FUJINET_RC_OK) {
        dcb.device    = 0x70 + unit;      // Fuji Device Identifier
        dcb.command   = 0xFE;        // send password
        dcb.buffer  = password;
        dcb.buff_bytes = 256;
        dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
        dcb.aux1 = len & 0xff;
        dcb.aux2 = (len >> 8) & 0xff;

        rc = fujinet_dcb_exec(&dcb);
    }

    return rc;
}