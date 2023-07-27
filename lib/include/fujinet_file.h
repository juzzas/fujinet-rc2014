//
// Created by jskists on 16/11/2022.
//

#include "fuji_typedefs.h"
#include "fujinet.h"

#ifndef FUJINET_RC2014_FUJINET_FILE_H
#define FUJINET_RC2014_FUJINET_FILE_H

#define OREAD   0x04
#define OWRITE  0x08
#define OUPDATE 0x0C


static struct file_status
{
    uint32_t file_size;
    uint32_t file_pointer;
    uint8_t file_error_code;
};

#define MAX_FILE_HANDLE 8


/**
 * Open device with handle
 *
 * @param file_handle - a file device handle to use
 * @param filespec - path of file to open
 * @param mode mode, 0=none, 1=cr, 2=lf, 3=cr/lf
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_file_open(uint8_t file_handle, char const* filespec, uint8_t mode);

/**
 * Close file device with handle
 *
 * @param file_handle - a file device handle to close
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_file_close(uint8_t file_handle);

/**
 * Get status of specific file device
 *
 * @param file_handle - a file device handle to close
 * @param status - the status buffer to use to fetch the status
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_file_status(uint8_t file_handle, struct file_status *status);

/**
 * Read # of bytes from specific file device.
 *
 * @param file_handle - a file device handle to read
 * @param buf - The buffer to read into, must be at least as big as len.
 * @param len - The # of bytes to read (up to 1024)
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_file_read(uint8_t file_handle, uint8_t* buf, uint16_t len);

/**
 * Write # of bytes to specific file device.
 *
 * @param file_handle - a file device handle to write
 * @param buf - The buffer to write to device, should be at least as big as len.
 * @param len - The # of bytes to write (up to 1024)
 *
 * @return FUJINET_RC_OK on success, else error
 */
FUJINET_RC fujinet_file_write(uint8_t file_handle, uint8_t* buf, uint16_t len);


#endif //FUJINET_RC2014_FUJINET_FILE_H
