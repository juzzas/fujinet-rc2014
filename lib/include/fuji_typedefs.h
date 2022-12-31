/**
 * CONFIG custom types
 */

#ifndef FUJI_TYPEDEFS_H
#define FUJI_TYPEDEFS_H

#define FILE_MAXLEN 36
#define SSID_MAXLEN 32 /* 32 + NULL */

#define MODE_READ 1
#define MODE_WRITE 2

#define FUJINET_MAX_HOST_SLOTS    8
#define FUJINET_MAX_DEVICE_SLOTS  8

typedef unsigned short DirectoryPosition;


/**
 * Returned info for a single SSID entry
 * from a WiFi scan
 */
typedef struct {
  char ssid[SSID_MAXLEN];
  signed char rssi;
} SSIDInfo;

/**
 * The current wifi SSID and password
 */
typedef struct
{
  char ssid[SSID_MAXLEN];
  char password[64];
} NetConfig;

/**
 * The current network adapter configuration
 */
typedef struct
{
  char ssid[SSID_MAXLEN];
  char hostname[64];
  unsigned char localIP[4];
  unsigned char gateway[4];
  unsigned char netmask[4];
  unsigned char dnsIP[4];
  unsigned char macAddress[6];
  unsigned char bssid[6];
  char fn_version[15];
} AdapterConfig;

typedef struct {
  unsigned char hostname[32];
} HostSlot;

typedef struct {
  unsigned char hostSlot;
  unsigned char mode;
  unsigned char file[FILE_MAXLEN];
} DeviceSlot;

typedef struct
{
  unsigned short numSectors;
  unsigned short sectorSize;
  unsigned char hostSlot;
  unsigned char deviceSlot;
  char filename[256];
} NewDisk;

#endif /* FUJI_TYPEDEFS_H */
