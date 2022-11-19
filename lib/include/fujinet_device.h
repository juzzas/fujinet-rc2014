#include <stdint.h>
#include "fujinet.h"
#include "fuji_typedefs.h"

#ifndef FUJINET_DEVICE_H
#define FUJINET_DEVICE_H

/**
 * Reset the FujiNet device
 *
 * @return FUJINET_TRC_OK on success, else error
 */
FUJINET_RC fujinet_device_reset(void);

/**
 * Fetch the WiFi Status
 *
 * @param status [OUT] WiFi status code
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_get_wifi_status(uint8_t *status);

/**
 *
 * @param nc
 * @return
 */
FUJINET_RC fujinet_get_ssid(NetConfig *nc);

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

/**
 * Get adaptor configuration
 *
 * @param ac [OUT]
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_get_adaptor_config(AdapterConfig *ac);

/**
 * Set WiFi SSID and password
 *
 * @param nc [IN]
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_set_ssid(NetConfig *nc);

/**
 * Fetch (load) device slots
 *
 * @param d [OUT]
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_get_device_slots(DeviceSlot *d);

/**
 * Fetch (load) host slots
 *
 * @param h [OUT]
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_get_host_slots(HostSlot *h);

/**
 * Put (save) host slots
 *
 * @param h [IN]
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_put_host_slots(HostSlot *h);

/**
 * Put (save) device slots
 *
 * @param d [IN]
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_put_device_slots(DeviceSlot *d);

/**
 * Mount host slot
 *
 * @param hs [IN]
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_mount_host_slot(unsigned char hs);

/**
 * Open directory
 *
 * @param hs [IN]
 * @param p [IN]
 * @param f [IN]
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_open_directory(unsigned char hs, char *p, char *f);

/**
 * Read directory entry at current index
 *
 * @param dirent [OUT]
 * @param l [IN]
 * @param a [IN]
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_read_directory(char *dirent, unsigned char l, unsigned char a);

/**
 * Close directory
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_close_directory(void);

/**
 * Set directory index position
 *
 * @param pos [IN]
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_set_directory_position(DirectoryPosition pos);

#endif /* FUJINET_DEVICE_H */
