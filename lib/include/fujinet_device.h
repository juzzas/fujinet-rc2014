#include <stdint.h>
#include "fujinet.h"

#ifndef FUJINET_DEVICE_H
#define FUJINET_DEVICE_H

#define SSID_MAXLEN 32

typedef struct {
    char ssid[SSID_MAXLEN];
    signed char rssi;
} SSIDInfo;

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


/** Scan for networks
 *
 * @param count [OUT] WiFi network count
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_scan_for_networks(uint8_t *count);

/**
 * Get scan result entry
 *
 * @param n [IN]
 * @param info [OUT]
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_get_scan_result(uint8_t n, SSIDInfo *info);

#endif /* FUJINET_DEVICE_H */
