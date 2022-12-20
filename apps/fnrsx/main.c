//
// Created by jskists on 19/12/2022.
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <z80.h>

#define DEBUG 1

#pragma output REGISTER_SP = 32768

/*
 * Exported values from loader.asm
 */
extern const uint8_t* driver_code;
extern const uint16_t driver_code_size;
extern const uint16_t* reloc_info;
extern const uint16_t reloc_info_size;

extern void driver_code_init(uint16_t init_call_addr) __z88dk_fastcall;

uint16_t load_addr;

void main(void) {
  puts("FujNet RSX loader");

  uint16_t bdos_addr = z80_wpeek(0x0006);
  uint16_t driver_pages = ((driver_code_size / 256) + 1);

  load_addr = (bdos_addr - (driver_pages << 8)) & 0xff00;

#if DEBUG
  printf("driver code = 0x%04x (%x bytes)\n", driver_code, driver_code_size);
  printf("reloc info = 0x%04x (%u words)\n", reloc_info, reloc_info_size);
  printf("bdos = 0x%04x\n", bdos_addr);
  printf("pages needed = %u\n", driver_pages);
  printf("load_addr = 0x%04x\n", load_addr);
#endif

  memcpy((uint8_t*)load_addr, driver_code, driver_code_size);

  for (int i = 0; i < reloc_info_size; i++) {
    uint16_t addr = z80_wpeek((load_addr + reloc_info[i]));
    printf("patching 0x%04x: 0x%04x > 0x%04x\n", reloc_info[i], addr, addr + load_addr);
    z80_wpoke(load_addr + reloc_info[i], addr + load_addr);
  }

  driver_code_init(load_addr);

  puts("FujNet RSX installed");
  printf("new bdos = 0x%04x\n", z80_wpeek(0x0006));
}