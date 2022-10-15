#include <stdint.h>

// FujiNet Device Control Block
struct fujinet_dcb {
	uint8_t device;
	uint8_t unit;
	uint8_t command;
	uint8_t dstats; // transfer direction on Input, status on output
	uint8_t *buffer;
	uint8_t timeout; // timeout in seconds
	uint8_t reserved;
	uint16_t bytes; // data length in bytes
	uint8_t aux1;
	uint8_t aux2;
};

#define DSTATS_R  0x40
#define DSTATS_W  0x80
#define DSTATS_RW 0xC0
#define DSTATS_NO 0x00


/**
 * Initialse the Fujinet driver
 */
void fujinet_init(void);


/**
 * Checksum uses a simple 8-bit XOR of each successive byte
 *
 * @param buf pointer to buffer
 * @param len length of buffer
 * @return calculated checksum
 */
uint8_t fujinet_checksum(uint8_t *buf, uint16_t len);


/**
 * execute the command in the DCB
 *
 * @param dcb pointer to the device control block
 */
void fujinet_dcb_exec(struct fujinet_dcb *dcb);

