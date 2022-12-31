//
// Created by justin on 29/12/2022.
//
#include <arch/cpm.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*
 *  Syntax of CP/M file names is:
 *
 *  [[0-9]+[:]][[a-pA-P]:]name[.ext] | [[a-pA-p][[0-9]+]:]name[.ext]
 *
 *  E.g.
 *      file.txt
 *      a:file
 *      10:b:READ-ME.doc
 */


static char upcase(char c)
{
    if (islower(c))
        return toupper(c);
    return c;
}

void fcb_parse(struct cpm_fcb* fc, char* name)
{
    char* cp;
    char* np;
    char c;
    char num[3];

    fc->drive = 0;
    fc->uid = getuid();

    cp = name;
    if (cp[1]==':' || cp[2]==':' || cp[3]==':')  /* Drive/user */
    {
        np = num;
        while (*cp != ':')
        {
            if (isdigit(*cp))
                *np++ = *cp;
            else if (fc->drive == 0)
                fc->drive = upcase(*cp) - 'A' + 1;
            ++cp;
        }
        *np = 0;
        if (num[0])
            fc->uid = atoi(num);
        ++cp;
    }
    if (cp[1]==':' && fc->drive == 0 && !isdigit(cp[0]))  /* u:d: */
    {
        fc->drive=upcase(*cp) - 'A' + 1;
        cp += 2;
    }
    name = cp;
    cp = fc->name;
    while (*name != '.' && *name != '*' && *name > ' ' && cp < &fc->name[8])
        *cp++ = upcase(*name++);
    if (*name == '*')
        c = '?';
    else
        c = ' ';
    while (cp < &fc->name[8])
        *cp++ = c;
    while (*name && *name++ != '.')
        continue;
    while (*name > ' ' && *name != '*' && cp < &fc->ext[3])
        *cp++ = upcase(*name++);
    if (*name == '*')
        c = '?';
    else
        c = ' ';
    while (cp < &fc->ext[3])
        *cp++ = c;
    fc->extent = fc->next_record = 0;
}

int fcb_create(struct cpm_fcb* fc)
{
    return cpm_bdos_hl(22, (int)fc);
}


int fcb_close(struct cpm_fcb* fc)
{
    return cpm_bdos_hl(16, (int)fc);
}

int fcb_write(struct cpm_fcb* fc, uint8_t* buffer)
{
    cpm_bdos(26, (int)buffer);
    return cpm_bdos_hl(21, (int)fc);
}
