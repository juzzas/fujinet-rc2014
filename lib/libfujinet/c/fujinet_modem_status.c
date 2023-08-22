//
// Created by jskists on 16/11/2022.
//

#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_modem.h"

#include <string.h>

FUJINET_RC fujinet_modem_status(uint8_t modem_unit, struct modem_status *status)
{
    struct fujinet_dcb dcb;

    if (modem_unit > MAX_MODEM_UNIT)
        return FUJINET_RC_INVALID;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device    = RC2014_DEVICEID_MODEM + modem_unit;      // Fuji Device Identifier
    dcb.command   = 'S';        // Status
    dcb.response  = (uint8_t *)status;
    dcb.response_bytes     = sizeof(struct modem_status);
    dcb.timeout   = FUJINET_TIMEOUT;    // approximately 30 second timeout

    return fujinet_dcb_exec(&dcb);
}
