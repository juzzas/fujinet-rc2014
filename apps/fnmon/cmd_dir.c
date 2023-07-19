//
// Created by justin on 31/12/2022.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"

#include "cmd.h"
#include "console.h"
#include "console_utils.h"

uint8_t direntry[44];

FUJINET_RC do_dirlist(uint8_t host_id, char* prefix, char* filter) {
    FUJINET_RC rc;
    bool done = false;

    rc = fujinet_open_directory(host_id, prefix, filter);
    while(!done && rc == FUJINET_RC_OK) {
        rc = fujinet_read_directory(direntry, 44, 0x80);
        if (direntry[0] == 0x7f && direntry[1] == 0x7f) {
            done = true;
        } else {
            DirEntryAttributes* attribs = (DirEntryAttributes*)direntry;
            uint8_t* name = &direntry[12];
            unsigned long filesize = ((unsigned long)attribs->file_size_3 << 24) +
                ((unsigned long)attribs->file_size_2 << 16) +
                ((unsigned long)attribs->file_size_1 << 8) +
                ((unsigned long)attribs->file_size_0);

            /* TODO: emulate the following code:
             * printf("%-32s %s %04d-%02d-%02d %02d:%02d\n",
                    name,
                    attribs->file_flags & DIR_ENTRY_FF_DIR ? "<DIR>" : filesize_str,
                    attribs->modified_year + 2000,
                    attribs->modified_month,
                    attribs->modified_day,
                    attribs->modified_hour,
                    attribs->modified_minute
                    );
             */

            console_puts(name);

            if (attribs->file_flags & DIR_ENTRY_FF_DIR) {
                console_puts("<DIR>");
            } else {
                if (filesize > 1048576) {
                    console_put_uint16_base10(filesize / 1048576, 0);
                    console_puts("MB");
                } else if (filesize > 1024) {
                    console_put_uint16_base10(filesize / 1024, 0);
                    console_puts("KB");
                } else {
                    console_put_uint16_base10(filesize, 0);
                }
            }

        }
    }

    rc = fujinet_close_directory();

    return rc;
}

enum CommandResult cmd_dir(char* tokens[], int num_tokens)
{
    FUJINET_RC rc = FUJINET_RC_OK;

    uint8_t host_id = 1; //atoi(host);
    if (host_id >= FUJINET_MAX_HOST_SLOTS) {
        return COMMAND_ERROR_INVALID_ARGUMENTS;
    }

    rc = fujinet_mount_host_slot(host_id);
    if (rc == FUJINET_RC_OK) {
        if (num_tokens == 1) {
            rc = do_dirlist(host_id, "*.*", "");
        } else if (num_tokens == 2) {
            rc = do_dirlist(host_id, tokens[1], "");
        } else {
            rc = COMMAND_ERROR_INVALID_ARGUMENTS;
        }
    }

    return rc;
}

