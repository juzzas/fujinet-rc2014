//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"

struct fujinet_dcb dcb;

FUJINET_RC fujinet_device_reset(void)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));
    dcb.device = 0x70;
    dcb.command = 0xFF; // reset FujiNet device
    dcb.aux1 = 0;
    dcb.aux2 = 0;
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_get_wifi_status(uint8_t *status)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));
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
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xFE;
    dcb.response = (uint8_t *)nc;
    dcb.response_bytes = sizeof(NetConfig);
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_scan_for_networks(uint8_t *count)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));
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
    memset(&dcb, 0, sizeof(struct fujinet_dcb));
    dcb.device = 0x70;
    dcb.command = 0xFC; // Scan for networks
    dcb.aux1 = n;
    dcb.aux2 = 0;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.response = (uint8_t *)info;
    dcb.response_bytes = 33; //sizeof(SSIDInfo);

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_get_adapter_config(AdapterConfig *ac)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xE8; // Get adaptor config
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.response = (uint8_t *)ac;
    dcb.response_bytes = sizeof(AdapterConfig);

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_set_ssid(NetConfig *nc)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xFB;
    dcb.buffer = (uint8_t *)nc;
    dcb.buffer_bytes = sizeof(NetConfig);
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_get_device_slots(DeviceSlot *d)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF2;
    dcb.response = (uint8_t *)d;
    dcb.response_bytes = sizeof(DeviceSlot) * 8;
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_get_host_slots(HostSlot *h)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF4;
    dcb.response = (uint8_t *)h;
    dcb.response_bytes = sizeof(HostSlot) * 8;
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_put_host_slots(HostSlot *h)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF3;
    dcb.buffer = (uint8_t *)h;
    dcb.buffer_bytes = sizeof(HostSlot) * 8;
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_put_device_slots(DeviceSlot *d)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF1;
    dcb.buffer = (uint8_t *)d;
    dcb.buffer_bytes = sizeof(DeviceSlot) * 8;
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_mount_host_slot(unsigned char hs)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF9;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.aux1 = hs;

    return fujinet_dcb_exec(&dcb);
}

char open_dir_buffer[258];
FUJINET_RC fujinet_open_directory(unsigned char hs, char *p, char *f)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));
    char *e;

    memset(&open_dir_buffer,0,258);

    strcpy(open_dir_buffer, p);
    e = open_dir_buffer;

    if (f[0]!=0x00) {
        while (*e != 0x00)
            e++;

        e++;
        strcpy(e,f);
    }

    dcb.device = 0x70;
    dcb.command = 0xF7;
    dcb.buffer = (uint8_t *)open_dir_buffer;
    dcb.buffer_bytes = 256;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.aux1 = hs;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_read_directory(char *dirent, unsigned char l, unsigned char a)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF6;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.aux1 = l;
    dcb.aux2 = a;
    dcb.response = (uint8_t *)dirent;
    dcb.response_bytes = l;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_close_directory(void)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF5;
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_set_directory_position(DirectoryPosition pos)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xE4;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.aux1 = pos & 0xff;
    dcb.aux2 = (pos >> 8) & 0xff;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_set_device_filename(uint8_t ds, char* e)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xE2;
    dcb.timeout = 15;
    dcb.buffer = (uint8_t *)e;
    dcb.buffer_bytes = 256;
    dcb.aux1 = ds;

    return fujinet_dcb_exec(&dcb);
}


FUJINET_RC fujinet_get_device_filename(uint8_t ds, char* buffer)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xDA;
    dcb.timeout = 15;
    dcb.response = (uint8_t*)buffer;
    dcb.response_bytes = 256;
    dcb.aux1 = ds;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_create_new(uint8_t selected_host_slot,
                              uint8_t selected_device_slot,
                              unsigned long selected_size,
                              char *path)
{
    char nd[263]={0xE7,0x00,0x00,0x00,0x00,0x00,0x00};
    char *c = &nd[3];
    unsigned long *l = (unsigned long *)c;

    nd[1]=selected_host_slot;
    nd[2]=selected_device_slot;
    *l=selected_size;
    strcpy(&nd[7],path);

    //  eos_write_character_device(FUJI_DEV,&nd,sizeof(nd));
    return FUJINET_RC_NOT_IMPLEMENTED;
}


FUJINET_RC fujinet_mount_disk_image(uint8_t ds, uint8_t mode)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF8;
    dcb.timeout = 15;
    dcb.aux1 = ds;
    dcb.aux2 = mode;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_set_boot_config(uint8_t toggle)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xD9;
    dcb.timeout = 15;
    dcb.aux1 = toggle;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_umount_disk_image(uint8_t ds)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xE9;
    dcb.timeout = 15;
    dcb.aux1 = ds;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_get_device_enabled_status(uint8_t d, uint8_t* response)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xD1;
    dcb.timeout = 15;
    dcb.aux1 = d;
    dcb.response = (uint8_t *)response;
    dcb.response_bytes = 1;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_enable_device(uint8_t d)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xD5;
    dcb.timeout = 15;
    dcb.aux1 = d;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_disable_device(uint8_t d)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xD4;
    dcb.timeout = 15;
    dcb.aux1 = d;

    return fujinet_dcb_exec(&dcb);
}


FUJINET_RC fujinet_mount_all(void)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xD7;
    dcb.timeout = 15;

    return fujinet_dcb_exec(&dcb);
}
