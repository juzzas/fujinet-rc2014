//
// Created by jskists on 16/11/2022.
//
#include <stddef.h>
#include <string.h>

#include "fujinet_device.h"
#include "fujinet_network.h"

#define TIMEOUT 30000 /* approx 30 seconds */


FUJINET_RC fujinet_network_login(uint8_t network_unit, char* login, char* password)
{
    struct fujinet_dcb dcb;
    memset(&dcb, 0, sizeof(struct fujinet_dcb));
    uint16_t login_len = 0;
    uint16_t password_len = 0;

    if (login)
        login_len = strlen(login);

    if (password)
        password_len = strlen(password);

    dcb.device    = RC2014_DEVICEID_NETWORK + network_unit;      // Fuji Device Identifier
    dcb.command   = 0xFD;        // send login
    dcb.buffer  = login_len > 0 ? (uint8_t*)login : NULL;
    dcb.buffer_bytes = login_len < 255 ? login_len : 255 ;
    dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
    dcb.aux1 = dcb.buffer_bytes;

    FUJINET_RC rc = fujinet_dcb_exec(&dcb);
    if (rc == FUJINET_RC_OK) {
        memset(&dcb, 0, sizeof(struct fujinet_dcb));
        dcb.device    = 0x71 + network_unit;      // Fuji Device Identifier
        dcb.command   = 0xFE;        // send password
        dcb.buffer  = password_len > 0 ? (uint8_t*)password : NULL;
        dcb.buffer_bytes = password_len < 255 ? password_len : 255;
        dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
        dcb.aux1 = dcb.buffer_bytes;

        rc = fujinet_dcb_exec(&dcb);
    }

    return rc;
}

