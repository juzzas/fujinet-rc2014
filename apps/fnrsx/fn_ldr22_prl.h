//
// Created by jskists on 24/04/2023.
//

#ifndef FN_LDR22_RELOC_H
#define FN_LDR22_RELOC_H

#include <stdint.h>

typedef  struct  {
    uint8_t reserved_00;    // Always 0
    uint16_t bytes;         // Number of bytes in program image. This includes
                            // the code and initialised data groups.
    uint8_t reserved_03;    // Always 0
    uint16_t bss;           // Number of bytes reserved for uninitialised data.
                            // Always 0 in an OVL file
    uint8_t reserved_06;    // Always 0
    uint16_t loadadd;       // Load address. Always 0 except in OVL files,
                            // because  the other files are relocatable.
    uint8_t reserved_09;    // Always 0
    uint16_t csbase;        // BIOS link. Always 0 unless Link-80 was used
                            // with the [B] option, in which case it is the
                            // base address.
    uint8_t unused[0xF4];
    uint8_t* program_image; // pointer to the start of the relocatable
                            // binary.
} PRL_Header;

typedef struct {
    uint8_t serial[6];
    uint8_t jp_start_op;
    uint16_t jp_start_addr;
    uint8_t jp_next_op;
    uint16_t jp_next_addr;
    uint8_t prev_addr;
    uint8_t flag_remove;
    uint8_t flag_nonbank;
    uint8_t name[8];
    uint8_t flag_loader;
    uint8_t reserve00;
    uint8_t reserve01;
} RSX_Header;


void prl_relocate(PRL_Header* prl, uint8_t destination_page);

uint8_t prl_num_pages(PRL_Header* prl);

#endif //FN_LDR22_RELOC_H
