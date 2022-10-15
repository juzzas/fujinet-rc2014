//
// Created by jskists on 12/10/2022.
//

#ifndef FUJINET_RC2014_FUJINET_HAL_H
#define FUJINET_RC2014_FUJINET_HAL_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Initialised the FujiNet hardware
 */
void fujinet_hal_init(void);

/**
 * Output byte to FujiNet device
 *
 * @param ch byte to output
 */
void fujinet_hal_tx(uint8_t ch) __z88dk_fastcall;

/**
 * Input byte from FujiNet device
 *
 * Blocks waiting for character
 *
 * @return character inputted
 */
uint8_t fujinet_hal_rx(void);


/**
 * Examine number of characters available in queue
 *
 * @return characters waiting
 */
uint8_t fujinet_hal_rx_avail(void);


/**
 * Assert the COMMAND line
 *
 * @param assert true to assert, false to de-assert
 */
void fujinet_hal_assert_cmd(bool assert) __z88dk_fastcall;


#endif //FUJINET_RC2014_FUJINET_HAL_H
