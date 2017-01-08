/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <cpu/itp/itpd.h>
#include <mem/mem.h>
#include <vid/vid.h>

void itpd_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    unsigned char sprite[255];
    int s, sn;
    sn = cp8_asm_var(n, nnn);
    for (s = 0; s < sn; s++) {
        sprite[s] = cp8_memget(cp8->i + s);
    }
    // INFO(Rafael): DRW Vx, Vy, nibble
    cp8->v[0xf] = cp8_viddrw(cp8_asm_var(x, nnn), cp8_asm_var(y, nnn), sprite, sn);
}
