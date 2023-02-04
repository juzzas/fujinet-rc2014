//
// Created by justin on 22/01/2023.
//
#include <stdint.h>
#include "fujinet.h"
#include "fuji_typedefs.h"

#ifndef FUJINET_DISK_H
#define FUJINET_DISK_H

#define DISK_SECTOR_SIZE 512


FUJINET_RC fujinet_disk_read(uint8_t disk_id,
                             uint16_t sector_number,
                             uint8_t* buffer);

FUJINET_RC fujinet_disk_write(uint8_t disk_id,
                              uint16_t sector_number,
                              uint8_t* buffer);

FUJINET_RC fujinet_disk_get_sector_size(uint8_t disk_id,
                                        uint8_t* sector_size);

#endif //FUJINET_DISK_H
