/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <asm/dtp/dtp8.h>
#include <ctx/types.h>
#include <stdio.h>

char *cp8_dsm_dtp8(unsigned short nnn) {
    static char mne[255];

    switch (nnn & 0xf) {
        case 0x0:
            sprintf(mne, "LD V%.1X, V%.1X", cp8_asm_var(x, nnn), cp8_asm_var(y, nnn));
            break;

        case 0x1:
            sprintf(mne, "OR V%.1X, V%.1X", cp8_asm_var(x, nnn), cp8_asm_var(y, nnn));
            break;

        case 0x2:
            sprintf(mne, "AND V%.1X, V%.1X", cp8_asm_var(x, nnn), cp8_asm_var(y, nnn));
            break;

        case 0x3:
            sprintf(mne, "XOR V%.1X, V%.1X", cp8_asm_var(x, nnn), cp8_asm_var(y, nnn));
            break;

        case 0x4:
            sprintf(mne, "ADD V%.1X, V%.1X", cp8_asm_var(x, nnn), cp8_asm_var(y, nnn));
            break;

        case 0x5:
            sprintf(mne, "SUB V%.1X, V%.1X", cp8_asm_var(x, nnn), cp8_asm_var(y, nnn));
            break;

        case 0x6:
            sprintf(mne, "SHR V%.1X", cp8_asm_var(x, nnn));
            break;

        case 0x7:
            sprintf(mne, "SUBN V%.1X, V%.1X", cp8_asm_var(x, nnn), cp8_asm_var(y, nnn));
            break;

        case 0xe:
            sprintf(mne, "SHL V%.1X", cp8_asm_var(x, nnn));
            break;
    }

    return &mne[0];

}
