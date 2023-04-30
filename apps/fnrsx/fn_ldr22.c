/*
 * Copyright (c) 2023 Justin Skists.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arch/cpm.h>
#include <z80.h>

#include "fn_ldr22_prl.h"

#pragma output REGISTER_SP           = 0x8000    // place stack at $d000 at startup
#define DEBUG


extern PRL_Header* fn_payload;
extern uint16_t fn_payload_size;

#include <stdio.h>
#include <stdint.h>

void memdump(uint8_t* ptr, int size) {
    for (int i = 0; i < size; i += 16) {
        printf("%p: ", ptr + i);

        for (int j = 0; j < 16; j++)
            printf("%02X ", ptr[i + j]);

        for (int j = 0; j < 16; j++) {
            uint8_t val = ptr[i + j];
            printf("%c", val >= 32 && val < 127 ? val : '.');
        }

        printf("\n");
    }
}

void main(void) {
    uint8_t bdos_page = z80_bpeek(0x0007);

    puts("FujiNet RSX (CP/M 2.2) installed");

#ifdef DEBUG
    printf("payload = 0x%p\n", fn_payload);
    printf("payload size = %u\n", (uint16_t)fn_payload_size);
    printf("old bdos = 0x%04x\n", z80_wpeek(0x0006));
#endif

    printf("current bdos_page = 0x%02x\n", bdos_page);

    uint8_t dest_page = bdos_page - prl_num_pages(fn_payload);

    prl_relocate(fn_payload, dest_page);

    RSX_Header* rsx = (RSX_Header*)(dest_page << 8);

    rsx->jp_next_addr = z80_wpeek(0x0006);
    z80_wpoke(0x0006, ((dest_page << 8) + 6));
#ifdef DEBUG
    printf("new bdos = 0x%04x\n", z80_wpeek(0x0006));

    memdump((uint8_t*)rsx, 512);
#endif

    int rc = cpm_bdos(0xf0, 0); // fujinet_init

    puts("FujiNet RSX installed");
}
