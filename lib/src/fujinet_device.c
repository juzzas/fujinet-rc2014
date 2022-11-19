//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"

FUJINET_RC fujinet_device_reset(void)
{
  struct fujinet_dcb dcb = {};
  dcb.device = 0x70;
  dcb.command = 0xFF; // reset FujiNet device
  dcb.aux1 = 0;
  dcb.aux2 = 0;
  dcb.timeout = FUJINET_TIMEOUT;

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_get_wifi_status(uint8_t *status)
{
  struct fujinet_dcb dcb = {};
  dcb.device = 0x70;
  dcb.command = 0xFA; // Get WiFi Status
  dcb.aux1 = 0;
  dcb.aux2 = 0;
  dcb.timeout = FUJINET_TIMEOUT;
  dcb.response = status;
  dcb.response_bytes = 1;
  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_get_ssid(NetConfig *nc)
{
  struct fujinet_dcb dcb = {};

  dcb.device = 0x70;
  dcb.command = 0xFE;
  dcb.response = (uint8_t *)nc;
  dcb.response_bytes = sizeof(NetConfig);
  dcb.timeout = 15;

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_scan_for_networks(uint8_t *count)
{
    struct fujinet_dcb dcb = {};
    dcb.device = 0x70;
    dcb.command = 0xFD; // Scan for networks
    dcb.aux1 = 0;
    dcb.aux2 = 0;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.response = count;
    dcb.response_bytes = 1;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_get_scan_result(uint8_t n, SSIDInfo *info)
{
    struct fujinet_dcb dcb = {};
    dcb.device = 0x70;
    dcb.command = 0xFC; // Scan for networks
    dcb.aux1 = n;
    dcb.aux2 = 0;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.response = (uint8_t *)info;
    dcb.response_bytes = 33; //sizeof(SSIDInfo);

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_get_adaptor_config(AdapterConfig *ac)
{
  struct fujinet_dcb dcb = {};
  dcb.device = 0x70;
  dcb.command = 0xE8; // Get adaptor config
  dcb.timeout = FUJINET_TIMEOUT;
  dcb.response = (uint8_t *)ac;
  dcb.response_bytes = sizeof(AdapterConfig);

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_set_ssid(NetConfig *nc)
{
  struct fujinet_dcb dcb = {};

  dcb.device = 0x70;
  dcb.command = 0xFB;
  dcb.buffer = (uint8_t *)nc;
  dcb.buffer_bytes = sizeof(NetConfig);
  dcb.timeout = 15;

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_get_device_slots(DeviceSlot *d)
{
  struct fujinet_dcb dcb = {};

  dcb.device = 0x70;
  dcb.command = 0xF2;
  dcb.response = (uint8_t *)d;
  dcb.response_bytes = sizeof(DeviceSlot) * 8;
  dcb.timeout = 15;

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_get_host_slots(HostSlot *h)
{
  struct fujinet_dcb dcb = {};

  dcb.device = 0x70;
  dcb.command = 0xF4;
  dcb.response = (uint8_t *)h;
  dcb.response_bytes = sizeof(HostSlot) * 8;
  dcb.timeout = 15;

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_put_host_slots(HostSlot *h)
{
  struct fujinet_dcb dcb = {};

  dcb.device = 0x70;
  dcb.command = 0xF3;
  dcb.buffer = (uint8_t *)h;
  dcb.buffer_bytes = sizeof(HostSlot) * 8;
  dcb.timeout = 15;

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_put_device_slots(DeviceSlot *d)
{
  struct fujinet_dcb dcb = {};

  dcb.device = 0x70;
  dcb.command = 0xF1;
  dcb.buffer = (uint8_t *)d;
  dcb.buffer_bytes = sizeof(DeviceSlot) * 8;
  dcb.timeout = 15;

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_mount_host_slot(unsigned char hs)
{
  struct fujinet_dcb dcb = {};

  dcb.device = 0x70;
  dcb.command = 0xF9;
  dcb.timeout = 15;
  dcb.aux1 = hs;

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_open_directory(unsigned char hs, char *p, char *f)
{
  struct fujinet_dcb dcb = {};
  char c[258];
  char *e;

  memset(&c,0,258);

  strcpy(c, p);
  e = c;

  if (f[0]!=0x00) {
    while (*e != 0x00)
      e++;

    e++;
    strcpy(e,f);
  }

  dcb.device = 0x70;
  dcb.command = 0xF1;
  dcb.buffer = (uint8_t *)c;
  dcb.buffer_bytes = 256;
  dcb.timeout = 15;
  dcb.aux1 = hs;

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_read_directory(char *dirent, unsigned char l, unsigned char a)
{
  struct fujinet_dcb dcb = {};

  dcb.device = 0x70;
  dcb.command = 0xF6;
  dcb.timeout = 15;
  dcb.aux1 = l;
  dcb.aux2 = a;
  dcb.response = dirent;
  dcb.response_bytes = 256;

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_close_directory(void)
{
  struct fujinet_dcb dcb = {};

  dcb.device = 0x70;
  dcb.command = 0xF5;
  dcb.timeout = 15;

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_set_directory_position(DirectoryPosition pos)
{
  struct fujinet_dcb dcb = {};

  dcb.device = 0x70;
  dcb.command = 0xE4;
  dcb.timeout = 15;
  dcb.aux1 = pos & 0xff;
  dcb.aux2 = (pos >> 8) & 0xff;

  return fujinet_dcb_exec(&dcb);
}


