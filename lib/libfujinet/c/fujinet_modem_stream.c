//
// Created by jskists on 16/11/2022.
//

#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_modem.h"

#include <string.h>


FUJINET_RC fujinet_modem_stream(uint8_t modem_unit)
{
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    if (modem_unit > MAX_MODEM_UNIT)
        return FUJINET_RC_INVALID;

    dcb.device = RC2014_DEVICEID_MODEM + modem_unit;
    dcb.command = 'X';
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}
