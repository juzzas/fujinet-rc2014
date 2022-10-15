//
// Created by jskists on 12/10/2022.
//
#include <z80.h>
#include "fujinet.h"
#include "fujinet_hal.h"

typedef union {
    struct frame
    {
        uint8_t device;
        uint8_t command;
        uint8_t aux1;
        uint8_t aux2;
        uint8_t checksum;
    } header;

    uint8_t raw[5];
} command_frame;

void fujinet_init(void) {
    fujinet_hal_init();
}

uint8_t fujinet_checksum(uint8_t *buf, uint16_t len)
{
    uint8_t checksum = 0x00;

    for (uint16_t i = 0; i < len; i++)
        checksum ^= buf[i];

    return checksum;
}

void fujinet_dcb_exec(struct fujinet_dcb *dcb) {
    command_frame frame;

    frame.header.device = dcb->device;
    frame.header.command = dcb->command;
    frame.header.aux1 = dcb->aux1;
    frame.header.aux2 = dcb->aux2;
    frame.header.checksum = fujinet_checksum((uint8_t *)&frame, 4);

    fujinet_hal_assert_cmd(true);

    for (uint8_t i = 0; i < sizeof(frame); i++)
        fujinet_hal_tx(frame.raw[i]);

    for (uint16_t i = 0; i < (dcb->timeout * 10); i++) {
        uint8_t rc = fujinet_hal_rx_avail();

        if (rc > 0)
            break;

        z80_delay_ms(100);
    }

    uint8_t ack = fujinet_hal_rx();
    if (ack != 'A') {
        dcb->dstats = ack;
        fujinet_hal_assert_cmd(false);
        return;
    }

    if (dcb->dstats & DSTATS_W) {
        for (uint8_t i = 0; i < dcb->bytes; i++)
            fujinet_hal_tx(dcb->buffer[i]);

        fujinet_hal_tx(fujinet_checksum(dcb->buffer, dcb->bytes));
    }

    ack = fujinet_hal_rx();
    if (ack != 'A') {
        dcb->dstats = ack;
        fujinet_hal_assert_cmd(false);
        return;
    }

    if (dcb->dstats & DSTATS_R) {
        for (uint8_t i = 0; i < dcb->bytes; i++)
            dcb->buffer[i] = fujinet_hal_rx();

        uint8_t chk = fujinet_hal_rx();
    }

    ack = fujinet_hal_rx();
    if (ack != 'C') {
        dcb->dstats = ack;
        fujinet_hal_assert_cmd(false);
        return;
    }

    dcb->dstats = 0;
    fujinet_hal_assert_cmd(false);
}


