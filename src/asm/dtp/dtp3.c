/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <asm/dtp/dtp3.h>
#include <ctx/types.h>
#include <stdio.h>

char *cp8_dsm_dtp3(unsigned short nnn) {
    static char mne[255];
    sprintf(mne, "SE V%.1X, 0x%.2x", cp8_asm_var(x, nnn), cp8_asm_var(kk, nnn));
    return &mne[0];
}
