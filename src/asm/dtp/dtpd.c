/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <asm/dtp/dtpd.h>
#include <ctx/types.h>
#include <stdio.h>

char *cp8_dsm_dtpd(unsigned short nnn) {
    static char mne[255];
    sprintf(mne, "DRW V%.1X, V%.1X, 0x%.1x", cp8_asm_var(x, nnn), cp8_asm_var(y, nnn), cp8_asm_var(n, nnn));
    return &mne[0];
}
