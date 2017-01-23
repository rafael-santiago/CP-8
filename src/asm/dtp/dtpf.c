/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <asm/dtp/dtpf.h>
#include <ctx/types.h>
#include <stdio.h>

char *cp8_dsm_dtpf(unsigned short nnn) {
    static char mne[255];

    switch (nnn & 0xff) {
        case 0x07:
            sprintf(mne, "LD V%.1X, DT", cp8_asm_var(x, nnn));
            break;

        case 0x0a:
            sprintf(mne, "LD V%.1X, K", cp8_asm_var(x, nnn));
            break;

        case 0x15:
            sprintf(mne, "LD DT, V%.1X", cp8_asm_var(x, nnn));
            break;

        case 0x18:
            sprintf(mne, "LD ST, V%.1X", cp8_asm_var(x, nnn));
            break;

        case 0x1e:
            sprintf(mne, "ADD I, V%.1X", cp8_asm_var(x, nnn));
            break;

        case 0x29:
            sprintf(mne, "LD F, V%.1X", cp8_asm_var(x, nnn));
            break;

        case 0x33:
            sprintf(mne, "LD B, V%.1X", cp8_asm_var(x, nnn));
            break;

        case 0x55:
            sprintf(mne, "LD [I], V%.1X", cp8_asm_var(x, nnn));
            break;

        case 0x65:
            sprintf(mne, "LD V%.1X, [I]", cp8_asm_var(x, nnn));
            break;

        default:
            return NULL;
            break;
    }

    return &mne[0];
}
