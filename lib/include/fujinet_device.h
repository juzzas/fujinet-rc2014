#include <stdint.h>
#include "fujinet.h"
#include "fuji_typedefs.h"

#ifndef FUJINET_DEVICE_H
#define FUJINET_DEVICE_H

#define RC2014_DEVICEID_DISK 0x31
#define RC2014_DEVICEID_DISK_LAST 0x3F

#define RC2014_DEVICEID_PRINTER 0x41
#define RC2014_DEVICEID_PRINTER_LAST 0x44


#define RC2014_DEVICEID_FUJINET 0x70
#define RC2014_DEVICEID_FN_NETWORK 0x71
#define RC2014_DEVICEID_FN_NETWORK_LAST 0x78

#define RC2014_DEVICEID_MODEM 0x50

#define RC2014_DEVICEID_CPM 0x5A

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
FUJINET_RC fujinet_get_adapter_config(AdapterConfig *ac);

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
 * @param dirent [OUT] buffer to put directory entry
 * @param l [IN] length of directory entry to read
 * @param a [IN] attributes (0x80 = extended info)
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

FUJINET_RC fujinet_set_device_filename(unsigned char ds, char* e);

FUJINET_RC fujinet_get_device_filename(unsigned char ds, char* response);

FUJINET_RC fujinet_create_new(uint8_t selected_host_slot,
                              uint8_t selected_device_slot,
                              unsigned long selected_size,
                              char *path);

FUJINET_RC fujinet_mount_disk_image(uint8_t ds, uint8_t mode);

FUJINET_RC fujinet_set_boot_config(uint8_t toggle);

FUJINET_RC fujinet_umount_disk_image(uint8_t ds);

FUJINET_RC fujinet_get_device_enabled_status(uint8_t d, uint8_t* response);

FUJINET_RC fujinet_enable_device(uint8_t d);

FUJINET_RC fujinet_disable_device(uint8_t d);

#endif /* FUJINET_DEVICE_H */
