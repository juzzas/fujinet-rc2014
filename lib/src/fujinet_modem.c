//
// Created by jskists on 16/11/2022.
//

#include "fujinet.h"
#include "fujinet_modem.h"

#include <string.h>

extern struct fujinet_dcb dcb;

FUJINET_RC fujinet_modem_stream(uint8_t modem_unit)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    if (modem_unit > MAX_MODEM_UNIT)
        return FUJINET_RC_INVALID;

    dcb.device = 0x50 + modem_unit;
    dcb.command = 'X';
    dcb.timeout = 15;

    return fujinet_dcb_exec(&dcb);
}
