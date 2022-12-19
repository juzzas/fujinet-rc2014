//
// Created by jskists on 19/12/2022.
//

#include <stdio.h>
#include <stdint.h>

#define DEBUG 1


/*
 * Exported values from loader.asm
 */
extern const uint8_t* driver_code;
extern const uint16_t driver_code_size;
extern const uint16_t* reloc_info;
extern const uint16_t reloc_info_size;


void main(void) {
  puts("FujNet RSX loader");

#if DEBUG
  printf("driver code = 0x%04x (%ubytes)\n", driver_code, driver_code_size);
  printf("reloc info = 0x%04x (%u words)\n", reloc_info, reloc_info_size);
#endif
}