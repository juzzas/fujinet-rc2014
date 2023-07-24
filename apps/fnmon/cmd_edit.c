//
// Created by jskists on 03/07/2023.
//
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "console.h"
#include "console_utils.h"

enum CommandResult cmd_edit(char* tokens[], int num_tokens) {
    // Using the start address (in hex) of the first argument token,
    // Produce a memory dump in lines of 16 bytes
    // Example: dump 0x2000
    uint16_t start_address = strtol(tokens[1], NULL, 16);
    uint8_t* memory = (uint8_t*)start_address;
    bool done = false;
    uint8_t buffer[3];

    while (!done) {
        console_put_uint16((uint16_t)memory);
        console_puts(": ");

        console_put_uint8(*memory);
        console_puts("? ");

        buffer[0] = 0x00;
        console_read_line(buffer, 3);
        buffer[2] = '\0';

        if (buffer[0] == 0x00) {
            memory++;
            continue;
        }

        if (strcmp(buffer, "q") == 0) {
            done = true;
        } else if (isxdigit(buffer[0]) && isxdigit(buffer[1])) {
            *memory = (uint8_t)strtol(buffer, NULL, 16) & 0xff;
            memory++;
        } else {
            console_puts("ERROR\r\n");
        }

    }

    return COMMAND_SUCCESS;
}

