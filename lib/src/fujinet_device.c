//
// Created by jskists on 17/10/2022.
//

#include "fujinet.h"
#include "fujinet_device.h"

FUJINET_RC fujinet_device_reset(void)
{
    struct fujinet_dcb dcb;
    dcb.device = 0x70;
    dcb.unit = 1;
    dcb.command = 0xFF; // reset FujiNet device
    dcb.dstats = 0x40; // Peripheral->Computer
    dcb.buffer = 0x00;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.bytes = 0;
    dcb.aux1 = 0;
    dcb.aux2 = 0;

    return fujinet_dcb_exec(&dcb);
}
