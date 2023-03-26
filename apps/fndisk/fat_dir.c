//
// Created by justin on 12/03/2023.
//

#include "fat_dir.h"
#include "ff/diskio_fn.h"
#include "ff/ff.h"
#include <stdio.h>
#include <string.h>

DIR dir;

FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
    )
{
    FRESULT res;
    UINT i;
    static FILINFO fno;

    printf("scan_files: %s\n", path);

    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printf("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }

    printf("scan_files: res %d\n", res);

    return res;
}


FATFS fs;
FRESULT res;
char buff[256];


FUJINET_RC do_fat_dir(void)
{
	memset(&fs, 0, sizeof(FATFS));
    res = f_mount(&fs, "0:", 0);
    if (res == FR_OK) {
        strcpy(buff, "/");
        res = scan_files(buff);
    }

    return res;
}
