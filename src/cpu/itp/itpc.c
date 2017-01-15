/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <cpu/itp/itpc.h>
#include <stdlib.h>

unsigned short itpc_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    // INFO(Rafael): RND Vx, byte
    cp8_vreg(cp8_asm_var(x, nnn), cp8) = (rand() % 255) & cp8_asm_var(kk, nnn);

    return (cp8->pc + 2);
}
