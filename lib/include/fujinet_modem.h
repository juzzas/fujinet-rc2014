//
// Created by jskists on 16/11/2022.
//
#include <stdint.h>
#include "fujinet.h"
#include "fuji_typedefs.h"

#ifndef FUJINET_RC2014_FUJINET_MODEM_H
#define FUJINET_RC2014_FUJINET_MODEM_H

#define MAX_MODEM_UNIT  3

struct modem_status
{
    uint8_t bytes_waiting_low;
    uint8_t bytes_waiting_high;
    uint8_t protocol_status;
    uint8_t extended_error;
};

/**
 * Get status of specific Modem device
 *
 * @param modem_unit - a modem device spec to status (the unit number is extracted)
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_modem_status(uint8_t modem_unit, struct modem_status *status);

/**
 * Read # of bytes from specific N: device.
 *
 * @param modem_unit - a modem device spec to read bytes from.
 * @param buf - The buffer to read into, must be at least as big as len.
 * @param len - The # of bytes to read (up to 65535)
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_modem_read(uint8_t modem_unit, uint8_t* buf, uint16_t len);

/**
 * Write # of bytes to specific N: device.
 *
 * @param modem_unit - a modem device spec to write to.
 * @param buf - The buffer to write to device, should be at least as big as len.
 * @param len - The # of bytes to write (up to 65535)
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_modem_write(uint8_t modem_unit, uint8_t* buf, uint16_t len);


FUJINET_RC fujinet_modem_stream(uint8_t modem_unit);

#endif //FUJINET_RC2014_FUJINET_MODEM_H
