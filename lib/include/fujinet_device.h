#include <stdint.h>
#include "fujinet.h"

#ifndef FUJINET_DEVICE_H
#define FUJINET_DEVICE_H

/**
 * Reset the FujiNet device
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_device_reset(void);

/**
 * Fetch the WiFi Status
 *
 * @param status [OUT] WiFi status code
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_get_wifi_status(uint8_t *status);


#endif /* FUJINET_DEVICE_H */
