//
// Created by justin on 22/01/2023.
//

#include <stdint.h>
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"
#include "fujinet_disk.h"


FUJINET_RC fujinet_disk_read(uint8_t disk_id,
                             uint16_t sector_number,
                             uint8_t* buffer) {
    struct fujinet_dcb dcb;

    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = disk_id + 0x31;
    dcb.command = 'R';
    dcb.aux1 = (uint8_t)(sector_number & 0xff);
    dcb.aux2 = (uint8_t)(sector_number >> 8);
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.response = buffer;
    dcb.response_bytes = DISK_SECTOR_SIZE;

    return fujinet_dcb_exec(&dcb);
}
