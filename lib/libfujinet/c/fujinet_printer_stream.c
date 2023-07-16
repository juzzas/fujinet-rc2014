//
// Created by jskists on 16/11/2022.
//

#include "fujinet.h"
#include "fujinet_printer.h"

#include <string.h>

#define TIMEOUT 15000 /* approx 15 seconds */


FUJINET_RC fujinet_printer_stream(uint8_t printer_unit)
{
    struct fujinet_dcb dcb;
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    if (printer_unit > MAX_PRINTER_UNIT)
        return FUJINET_RC_INVALID;

    dcb.device = 0x40 + printer_unit;
    dcb.command = 'X';
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

