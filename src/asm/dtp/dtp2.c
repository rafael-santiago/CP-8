/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <asm/dtp/dtp2.h>
#include <stdio.h>

char *cp8_dsm_dtp2(unsigned short nnn) {
    static char mne[255];
    sprintf(mne, "CALL 0x%.3x", nnn);
    return &mne[0];
}
