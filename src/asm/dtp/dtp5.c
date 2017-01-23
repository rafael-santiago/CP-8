/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <asm/dtp/dtp5.h>
#include <ctx/types.h>
#include <stdio.h>

char *cp8_dsm_dtp5(unsigned short nnn) {
    static char mne[255];
    sprintf(mne, "SE V%.1X, V%.1X", cp8_asm_var(x, nnn), cp8_asm_var(y, nnn));
    return &mne[0];

}
