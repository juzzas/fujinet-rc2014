//
// Created by jskists on 24/04/2023.
//
#include <stdio.h>
#include <string.h>

#include "fn_ldr22_prl.h"


void prl_relocate(PRL_Header* prl, uint8_t destination_page) {
    printf("relocate: %p\n", prl);

    uint8_t* binary = (uint8_t*)prl + 256;
    uint16_t binsize = prl->bytes;
    uint8_t* bitmap = binary + binsize;
    uint8_t* destination = (uint8_t*)(destination_page << 8);

    printf("move: %p --> %p, %ubytes\n", binary, destination, binsize);

    uint16_t bitmap_size = (binsize + 7) / 8;
    for (uint16_t i = 0; i < binsize; i++) {
        uint16_t byte_index = i / 8;
        if (byte_index < bitmap_size) {
            uint8_t bit_index = 7 - (i % 8);
            uint8_t mask = 1 << bit_index;

            if (bitmap[byte_index] & mask) {
                binary[i] += (destination_page - 1);
            }
        }
    }

    memmove(destination, binary, binsize);
}

uint8_t prl_num_pages(PRL_Header* prl) {
   return (prl->bytes >> 8) + 1;
}
