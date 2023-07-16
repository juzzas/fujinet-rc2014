//
// Created by jskists on 17/10/2022.
//
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"


FUJINET_RC fujinet_create_new(uint8_t selected_host_slot,
                              uint8_t selected_device_slot,
                              unsigned long selected_size,
                              char *path)
{
    struct fujinet_dcb dcb;

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