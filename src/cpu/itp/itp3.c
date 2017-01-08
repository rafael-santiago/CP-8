/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <cpu/itp/itp3.h>

unsigned short itp3_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    // INFO(Rafael): SE Vx, byte
    if (cp8_vreg(cp8_asm_var(x, nnn), cp8) == cp8_asm_var(kk, nnn)) {
        cp8->pc += 2;
    }

    return cp8->pc;
}
