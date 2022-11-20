//
// Created by jskists on 12/10/2022.
//
#include "fujinet.h"
#include "fujinet_hal.h"
#include <stdint.h>
#include <stdlib.h>


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

bool is_rx_avail(uint16_t timeout) {
    int timeout_x10 = timeout * 10;
    uint8_t avail = 0;

    if (timeout) {
        for (uint16_t i = 0; i < timeout_x10; i++) {
            avail = fujinet_hal_rx_avail();

            if (avail > 0)
                break;

            msleep(100);
        }
    } else {
        avail = fujinet_hal_rx_avail();
    }

    return (avail > 0);
}

FUJINET_RC fujinet_dcb_exec(struct fujinet_dcb *dcb) {
    FUJINET_RC rc = FUJINET_RC_OK;
    command_frame frame;

    frame.header.device = dcb->device;
    frame.header.command = dcb->command;
    frame.header.aux1 = dcb->aux1;
    frame.header.aux2 = dcb->aux2;
    frame.header.checksum = fujinet_checksum((uint8_t *)&frame, 4);

    fujinet_hal_assert_cmd(true);

    for (uint8_t i = 0; i < sizeof(frame); i++)
        fujinet_hal_tx(frame.raw[i]);

    fujinet_hal_assert_cmd(false);

    if (!is_rx_avail(dcb->timeout)) {
        rc = FUJINET_RC_TIMEOUT;
        goto err_exit;
    }

    uint8_t ack = fujinet_hal_rx();
    if (ack != 'A') {
        rc = FUJINET_RC_NO_ACK;
        goto err_exit;
    }

    if (dcb->buffer_bytes != 0) {
        for (uint16_t i = 0; i < dcb->buffer_bytes; i++)
            fujinet_hal_tx(dcb->buffer[i]);

        fujinet_hal_tx(fujinet_checksum(dcb->buffer, dcb->buffer_bytes));

        if (!is_rx_avail(dcb->timeout)) {
            rc = FUJINET_RC_TIMEOUT;
            goto err_exit;
        }

        ack = fujinet_hal_rx();
        if (ack != 'A') {
            rc = FUJINET_RC_NO_ACK;
            goto err_exit;
        }
    }

    if (dcb->response_bytes != 0) {
        if (!is_rx_avail(dcb->timeout)) {
            rc = FUJINET_RC_TIMEOUT;
            goto err_exit;
        }

        for (uint16_t i = 0; i < dcb->response_bytes; i++)
            dcb->response[i] = fujinet_hal_rx();

        uint8_t chk = fujinet_hal_rx();
    }

    if (!is_rx_avail(dcb->timeout)) {
        rc = FUJINET_RC_TIMEOUT;
        goto err_exit;
    }

    ack = fujinet_hal_rx();
    if (ack != 'C') {
        rc = FUJINET_RC_NO_COMPLETE;
        goto err_exit;
    }

err_exit:
    fujinet_hal_assert_cmd(false);

    return rc;
}


