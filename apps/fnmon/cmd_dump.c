//
// Created by jskists on 03/07/2023.
//
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "console.h"

extern void console_put_uint8(uint8_t value);
extern void console_put_uint16(uint16_t value);

enum CommandResult cmd_dump(char* tokens[], int num_tokens) {
    // Using the start address (in hex) of the first argument token,
    // Produce a memory dump in lines of 16 bytes
    // Example: dump 0x2000
    uint16_t start_address = strtol(tokens[1], NULL, 16);
    uint8_t* memory = (uint8_t*)start_address;


    for (uint16_t dump_index = 0; dump_index < 256; dump_index += 16) {
        console_put_uint16(start_address);
        console_puts(": ");

        for (uint16_t index = start_address + dump_index; index < start_address + dump_index + 16; index++) {
            console_put_uint8(memory[index]);
        }

        console_puts(" ");

        for (uint16_t index = start_address + dump_index; index < start_address + dump_index + 16; index++) {
            console_tx(memory[index] >= ' ' && memory[index] <= '~' ? memory[index] : '.');
        }

        console_puts("\n");
    }

    return COMMAND_SUCCESS;
}

