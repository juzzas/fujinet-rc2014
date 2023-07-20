#include <stdlib.h>
#include <string.h>

#include "console.h"
#include "console_utils.h"


// VT102 Escape Sequences
#define VT102_ESCAPE "\x1B"
#define VT102_ERASE_LINE (VT102_ESCAPE "[K")

void vt102_cursor_left(int n) {
    char buffer[5];

    if (n == 0)
        return;

    itoa(n, buffer, 10);
    console_puts(VT102_ESCAPE "[");
    console_puts(buffer);
    console_tx('D');
}

void vt102_cursor_right(int n) {
    char buffer[5];

    if (n == 0)
        return;

    itoa(n, buffer, 10);
    console_puts(VT102_ESCAPE "[");
    console_puts(buffer);
    console_puts("C");
}

void vt102_erase_line() {
    console_puts(VT102_ERASE_LINE);
}


void console_put_buffer(const char* buffer, int length) {
    for (int i = 0; i < length; i++) {
        console_tx(buffer[i]);
    }
}

void console_read_line(char* buffer, int max_length) {
    int buffer_index = 0;
    int cursor_index = 0;
    char c;

    while (true) {
        if (console_rx_avail()) {
            c = console_rx();

            if (c == 13) {
                break;
            } else if (c == 0x1B) { // ESC key pressed
                // Handle VT102 control sequences
                char seq[3];
                seq[0] = console_rx();
                seq[1] = console_rx();
                seq[2] = '\0';

                if (strcmp(seq, "[D") == 0) { // Left arrow key pressed
                    if (buffer_index > 0) {
                        // Move cursor left
                        cursor_index--;

                        vt102_cursor_left(1);
                    }
                } else if (strcmp(seq, "[C") == 0) { // Right arrow key pressed
                    if (cursor_index < buffer_index) {
                        // Move cursor right
                        cursor_index++;

                        vt102_cursor_right(1);
                    }
                } else {
                    // Ignore unrecognized VT102 control sequences
                    console_rx();
                }
            } else if (c == 0x7F || c == '\b') { // Backspace key pressed
                if (buffer_index > 0) {
                    // Move cursor left
                    vt102_cursor_left(1);

                    // Erase the character at the cursor position
                    vt102_erase_line();

                    // Move characters to the left in the buffer
                    memmove(&buffer[cursor_index - 1], &buffer[cursor_index], buffer_index - cursor_index);

                    buffer_index--;
                    cursor_index--;

                    // Print the updated buffer
                    console_put_buffer(buffer + cursor_index, buffer_index - cursor_index + 1);

                    // Move cursor to the correct position
                    vt102_cursor_left(buffer_index - cursor_index);
                }
            } else if (c >= 0x20 && c <= 0x7E && buffer_index < max_length - 1) { // Printable character
                // Move characters to the right in the buffer
                memmove(&buffer[cursor_index + 1], &buffer[cursor_index], buffer_index - cursor_index);

                // Store the new character in the buffer at the cursor position
                buffer[cursor_index] = c;

                buffer_index++;
                cursor_index++;

                // Print the updated buffer
                console_put_buffer(buffer + cursor_index - 1, buffer_index - cursor_index + 1);

                // Move cursor to the correct position
                vt102_cursor_left(buffer_index - cursor_index);
            }
        }
    }

    // Null-terminate the buffer
    buffer[buffer_index] = '\0';

    // Move cursor to the next line
    console_puts("\n");
}


void console_put_uint16(uint16_t value) {
    // output character for each nibble in value
    for (int nibble = 0; nibble < 4; nibble++) {
        uint8_t nibble_value = (value >> (12 - (nibble * 4))) & 0xF;
        console_tx(nibble_value < 9 ? '0' + nibble_value : 'A' + nibble_value - 10);
    }
}

void console_put_uint8(uint8_t value) {
    // output character for each nibble in value
    for (int nibble = 0; nibble < 2; nibble++) {
        uint8_t nibble_value = (value >> (4 - (nibble * 4))) & 0xF;
        console_tx(nibble_value < 9 ? '0' + nibble_value : 'A' + nibble_value - 10);
    }
}

void console_put_common_base10(uint16_t value, uint8_t leading_zero, uint16_t divisor) {
    while (divisor > 0) {
        uint8_t digit = value / divisor;
        if (digit > 0 || !leading_zero) {
            console_tx('0' + digit);
            leading_zero = 0;
        }
        value -= digit * divisor;
        divisor /= 10;
    }
}

void console_put_uint16_base10(uint16_t value, uint8_t leading_zero) {
    console_put_common_base10(value, leading_zero, 10000);
}

void console_put_uint8_base10(uint8_t value, uint8_t leading_zero) {
    console_put_common_base10(value, leading_zero, 100);
}
