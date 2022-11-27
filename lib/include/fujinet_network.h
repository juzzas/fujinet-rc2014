//
// Created by jskists on 16/11/2022.
//

#include "fuji_typedefs.h"
#include "fujinet.h"

#ifndef FUJINET_RC2014_FUJINET_NETWORK_H
#define FUJINET_RC2014_FUJINET_NETWORK_H

#define DFUJI   0x71
#define DREAD   0x40
#define DWRITE  0x80
#define DUPDATE 0xC0

#define OREAD   0x04
#define OWRITE  0x08
#define OUPDATE 0x0C

#define SUCCESS 1
#define DERROR  144


#define DVSTAT_BYTES_WATING_LO  0
#define DVSTAT_BYTES_WATING_HI  1
#define DVSTAT_PROTOCOL		    2
#define DVSTAT_EXTENDED_ERROR	3

struct network_status
{
    uint8_t bytes_waiting_low;
    uint8_t bytes_waiting_high;
    uint8_t protocol_status;
    uint8_t extended_error;
};


/**
 * Open N: device with devicespec
 *
 * @param devicespec - an N: device spec, e.g. N:TCP://FOO.COM:1234/
 * @param translation mode, 0=none, 1=cr, 2=lf, 3=cr/lf
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_network_open(char* devicespec, unsigned char trans);

/**
 * Close N: device with devicespec
 *
 * @param devicespec - an N: device spec to close (the unit number is extracted)
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_network_close(char* devicespec);

/**
 * Get status of specific N: device
 *
 * @param devicespec - an N: device spec to status (the unit number is extracted)
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_network_status(char* devicespec, struct network_status *status);

/**
 * Read # of bytes from specific N: device.
 *
 * @param devicespec - an N: device spec to read bytes from.
 * @param buf - The buffer to read into, must be at least as big as len.
 * @param len - The # of bytes to read (up to 65535)
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_network_read(char* devicespec, uint8_t* buf, uint16_t len);

/**
 * Write # of bytes to specific N: device.
 *
 * @param devicespec - an N: device spec to write to.
 * @param buf - The buffer to write to device, should be at least as big as len.
 * @param len - The # of bytes to write (up to 65535)
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_network_write(char* devicespec, uint8_t* buf, uint16_t len);

/**
 * Send username and password credentials
 *
 * @param devicespec - The devicespec.
 * @param login - The username to send
 * @param password - The password to send
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_network_login(char* devicespec, char* login, char* password);

#endif //FUJINET_RC2014_FUJINET_NETWORK_H
