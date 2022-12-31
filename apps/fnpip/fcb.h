//
// Created by justin on 29/12/2022.
//
#include <arch/cpm.h>
#include <stdint.h>

#ifndef FCB_H
#define FCB_H
void fcb_parse(struct cpm_fcb* fc, char* name);

int fcb_create(struct cpm_fcb* fc);
int fcb_close(struct cpm_fcb* fc);

int fcb_write(struct cpm_fcb* fc, uint8_t* buffer);

#endif //FCB_H
