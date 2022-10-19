#include <stdint.h>
#include "fujinet.h"

#ifndef FUJINET_DEVICE_H
#define FUJINET_DEVICE_H

/**
 * Reset the FujiNet device
 *
 * @return FUJINET_TRC_OK on success, else error
 */
FUJINET_RC fujinet_device_reset(void);


#endif /* FUJINET_DEVICE_H */
