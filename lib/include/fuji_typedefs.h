/**
 * CONFIG custom types
 */

#ifndef FUJI_TYPEDEFS_H
#define FUJI_TYPEDEFS_H

#define FILE_MAXLEN 36
#define SSID_MAXLEN 33 /* 32 + NULL */

#define MODE_READ 1
#define MODE_WRITE 2

#define FUJINET_MAX_HOST_SLOTS    8
#define FUJINET_MAX_DEVICE_SLOTS  8

#define MAX_HOST_LEN 32
#define NUM_HOST_SLOTS 8

typedef enum _entry_types
{
  ENTRY_TYPE_TEXT,
  ENTRY_TYPE_FOLDER,
  ENTRY_TYPE_FILE,
  ENTRY_TYPE_LINK,
  ENTRY_TYPE_MENU
} EntryType;

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

typedef struct {
    unsigned char modified_year;
    unsigned char modified_month;
    unsigned char modified_day;
    unsigned char modified_hour;
    unsigned char modified_minute;
    unsigned char modified_second;

    // 32bit value for file size
    unsigned char file_size_0; //_lsb;
    unsigned char file_size_1;
    unsigned char file_size_2;
    unsigned char file_size_3; //_msb;

    // DIR_ENTRY_FF flags
    unsigned char file_flags;

    // DIR_ENTRY_FT type
    unsigned char file_type;
} DirEntryAttributes;

#define DIR_ENTRY_FF_DIR    0x01
#define DIR_ENTRY_FF_TRUNC  0x02

#define DIR_ENTRY_FT_UNKNOWN  0x00

#endif /* FUJI_TYPEDEFS_H */
