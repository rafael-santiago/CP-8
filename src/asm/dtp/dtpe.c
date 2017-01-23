/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <asm/dtp/dtpe.h>
#include <ctx/types.h>
#include <stdio.h>

char *cp8_dsm_dtpe(unsigned short nnn) {
    static char mne[255];

    switch (nnn & 0xff) {
        case 0x9e:
            sprintf(mne, "SKP V%.1X", cp8_asm_var(x, nnn));
            break;

        case 0xa1:
            sprintf(mne, "SKPN V%.1X", cp8_asm_var(x, nnn));
            break;

        default:
            return NULL;
            break;
    }

    return &mne[0];
}
