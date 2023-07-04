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

char direntry[256];
char filesize_str[32];
static struct fujinet_dcb dcb;

FUJINET_RC fujinet_mount_host_slot(unsigned char hs)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF9;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.aux1 = hs;

    return fujinet_dcb_exec(&dcb);
}

char open_dir_buffer[258];
FUJINET_RC fujinet_open_directory(unsigned char hs, char *p, char *f)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));
    char *e;

    memset(&open_dir_buffer,0,258);

    strcpy(open_dir_buffer, p);
    e = open_dir_buffer;

    if (f[0]!=0x00) {
        while (*e != 0x00)
            e++;

        e++;
        strcpy(e,f);
    }

    dcb.device = 0x70;
    dcb.command = 0xF7;
    dcb.buffer = (uint8_t *)open_dir_buffer;
    dcb.buffer_bytes = 256;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.aux1 = hs;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_read_directory(char *dirent, unsigned char l, unsigned char a)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF6;
    dcb.timeout = FUJINET_TIMEOUT;
    dcb.aux1 = l;
    dcb.aux2 = a;
    dcb.response = (uint8_t *)dirent;
    dcb.response_bytes = l;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_close_directory(void)
{
    memset(&dcb, 0, sizeof(struct fujinet_dcb));

    dcb.device = 0x70;
    dcb.command = 0xF5;
    dcb.timeout = FUJINET_TIMEOUT;

    return fujinet_dcb_exec(&dcb);
}

FUJINET_RC do_dirlist(uint8_t host_id, char* prefix, char* filter) {
    FUJINET_RC rc;
    bool done = false;

    rc = fujinet_open_directory(host_id, prefix, filter);
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

enum CommandResult cmd_dir(char* tokens[], int num_tokens)
{
    FUJINET_RC rc;

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

