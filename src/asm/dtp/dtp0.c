/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <asm/dtp/dtp0.h>
#include <stdio.h>

char *cp8_dsm_dtp0(unsigned short nnn) {
    static char mne[255];

    switch (nnn) {
        case 0x00e0:
            sprintf(mne, "CLS");
            break;

        case 0x00ee:
            sprintf(mne, "RET");
            break;

        default:
            mne[0] = 0;
            break;
    }

    return &mne[0];
}
