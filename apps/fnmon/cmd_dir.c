//
// Created by justin on 31/12/2022.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "fujinet.h"
#include "fujinet_device.h"

#include "cmd.h"
#include "devices.h"
#include "hosts.h"

char direntry[256];
char filesize_str[32];

FUJINET_RC do_dirlist(uint8_t host_id, char* prefix, char* filter) {
    FUJINET_RC rc;
    bool done = false;

    rc = fujinet_open_directory(host_id, prefix, filter);
    printf("Opened (%d)...\n", rc);

    while(!done && rc == FUJINET_RC_OK) {
        rc = fujinet_read_directory(direntry, 44, 0x80);
        if (direntry[0] == 0x7f && direntry[1] == 0x7f) {
            done = true;
        }
        else
        {
            DirEntryAttributes* attribs = (DirEntryAttributes*)direntry;
            char* name = &direntry[12];
            unsigned long filesize = ((unsigned long)attribs->file_size_3 << 24) +
                ((unsigned long)attribs->file_size_2 << 16) +
                ((unsigned long)attribs->file_size_1 << 8) +
                ((unsigned long)attribs->file_size_0);

            if (filesize > 1048576) {
                sprintf(filesize_str, "%-3dMB", (filesize / 1048576));
            } else if (filesize > 1024) {
                sprintf(filesize_str, "%-3dKB", (filesize / 1024));
            } else {
                sprintf(filesize_str, "%-3d", filesize);
            }

            printf("%-32s %s %04d-%02d-%02d %02d:%02d\n",
                   name,
                   attribs->file_flags & DIR_ENTRY_FF_DIR ? "<DIR>" : filesize_str,
                   attribs->modified_year + 2000,
                   attribs->modified_month,
                   attribs->modified_day,
                   attribs->modified_hour,
                   attribs->modified_minute
                   );
        }
    }

    rc = fujinet_close_directory();

    return rc;
}

FUJINET_RC do_disk_list(char* host, char* prefix, char* filter) {
    FUJINET_RC rc;
    HostSlot* hs = NULL;

    uint8_t host_id = atoi(host);
    printf("host_id: = %d\n", host_id);
    if (host_id >= FUJINET_MAX_HOST_SLOTS) {
        return FUJINET_RC_INVALID;
    }

    rc = fetch_hosts();
    if (rc == FUJINET_RC_OK) {
        hs = get_host_at(host_id);
        if (hs) {
            printf("Host: %s\n", hs->hostname);

            rc = fujinet_mount_host_slot(host_id);
            if (rc == FUJINET_RC_OK) {
                printf("Mounted...\n");
                rc = do_dirlist(host_id, prefix, filter);
            }

        }
    }
    return FUJINET_RC_OK;
}

enum CommandResult cmd_dir(char* tokens[], int num_tokens)
{
    return do_disk_list(tokens[1], tokens[2], tokens[3]);
}

