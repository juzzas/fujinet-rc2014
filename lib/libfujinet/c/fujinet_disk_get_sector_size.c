//
// Created by justin on 22/01/2023.
//

#include <stdint.h>
#include <string.h>

#include "fujinet.h"
#include "fujinet_disk.h"


FUJINET_RC fujinet_disk_get_sector_size(uint8_t disk_id, uint8_t* sector_size) {
    sector_size = DISK_SECTOR_SIZE;
    return FUJINET_RC_OK;
}
