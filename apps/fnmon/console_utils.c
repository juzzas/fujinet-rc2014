#include "console.h"
#include "console_utils.h"

void console_put_buffer(const char* buffer, int length) {
    for (int i = 0; i < length; i++) {
        console_tx(buffer[i]);
    }
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

void console_put_uint16_base10(uint16_t value, uint8_t leading_zero) {
    uint16_t divisor = 10000;

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

void console_put_uint8_base10(uint8_t value, uint8_t leading_zero) {
    uint8_t divisor = 100;

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
