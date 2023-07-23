//
// Created by jskists on 18/07/2023.
//
#include <stdint.h>

#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

void console_put_buffer(const char* buffer, int length);
void console_puts_pad(const char* str, uint8_t padding);
void console_read_line(char* buffer, int max_length);
void console_put_uint8(uint8_t value);
void console_put_uint16(uint16_t value);
void console_put_uint8_base10(uint8_t value, bool pad);
void console_put_uint16_base10(uint16_t value, bool pad);

#endif //CONSOLE_UTILS_H
