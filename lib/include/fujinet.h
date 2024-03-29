#include <stdint.h>
#include "fuji_typedefs.h"

#ifndef FUJINET_H
#define FUJINET_H

// FujiNet Device Control Block
struct fujinet_dcb {
    uint8_t device;
    uint8_t command;
    uint8_t aux1;
    uint8_t aux2;
    uint8_t *buffer;
    uint16_t buffer_bytes; // data length in bytes
    uint8_t *response;
    uint16_t response_bytes; // data length in bytes
    uint16_t timeout;   // milliseconds
};

#define FUJINET_TIMEOUT 15000

typedef enum {
    FUJINET_RC_OK,
    FUJINET_RC_NOT_IMPLEMENTED,
    FUJINET_RC_NOT_SUPPORTED,
    FUJINET_RC_INVALID,
    FUJINET_RC_TIMEOUT,
    FUJINET_RC_NO_ACK,
    FUJINET_RC_NO_COMPLETE,
} FUJINET_RC;

/**
 * Initialse the Fujinet driver
 */
void fujinet_init(void);


/**
 * Checksum uses a simple 8-bit XOR of each successive byte
 *
 * @param buf pointer to buffer
 * @param len length of buffer
 * @return calculated checksum
 */
uint8_t fujinet_checksum(uint8_t *buf, uint16_t len);


/**
 * execute the command in the DCB
 *
 * @param dcb pointer to the device control block
 */
FUJINET_RC fujinet_dcb_exec(struct fujinet_dcb *dcb) __z88dk_fastcall;

/**
 * poll the PROCEED line
 *
 * true = data waiting
 */
int fujinet_poll_proceed(void) __z88dk_fastcall;
#endif /* FUJINET_H */
