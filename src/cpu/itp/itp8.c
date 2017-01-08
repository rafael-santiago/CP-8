/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <cpu/itp/itp8.h>

unsigned short itp8_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    switch (nnn & 0x1) {
        case 0x0:
            // INFO(Rafael): LD Vx, Vy
            cp8_vreg(cp8_asm_var(x, nnn), cp8) = cp8_vreg(cp8_asm_var(y, nnn), cp8);
            break;

        case 0x1:
            // INFO(Rafael): OR Vx, Vy
            cp8_vreg(cp8_asm_var(x, nnn), cp8) |= cp8_vreg(cp8_asm_var(y, nnn), cp8);
            break;

        case 0x2:
            // INFO(Rafael): AND Vx, Vy
            cp8_vreg(cp8_asm_var(x, nnn), cp8) &= cp8_vreg(cp8_asm_var(y, nnn), cp8);
            break;

        case 0x3:
            // INFO(Rafael): XOR Vx, Vy
            cp8_vreg(cp8_asm_var(x, nnn), cp8) ^= cp8_vreg(cp8_asm_var(y, nnn), cp8);
            break;

        case 0x4:
            // INFO(Rafael): ADD Vx, Vy
            cp8_vreg(cp8_asm_var(x, nnn), cp8) += cp8_vreg(cp8_asm_var(y, nnn), cp8);
            cp8->v[0xf] = (cp8_vreg(cp8_asm_var(x, nnn), cp8) > 0xff);
            break;

        case 0x5:
            // INFO(Rafael): SUB Vx, Vy
            cp8->v[0xf] = cp8_vreg(cp8_asm_var(x, nnn), cp8) > cp8_vreg(cp8_asm_var(y, nnn), cp8);
            cp8_vreg(cp8_asm_var(x, nnn), cp8) -= cp8_vreg(cp8_asm_var(y, nnn), cp8);
            break;

        case 0x6:
            // INFO(Rafael): SHR Vx {, Vy}
            cp8->v[0xf] = (cp8_vreg(cp8_asm_var(x, nnn), cp8) & 0x1);
            cp8_vreg(cp8_asm_var(x, nnn), cp8) <<= 1;
            break;

        case 0x7:
            // INFO(Rafael): SUBN Vx, Vy
            cp8->v[0xf] = (cp8_vreg(cp8_asm_var(y, nnn), cp8) > cp8_vreg(cp8_asm_var(x, nnn), cp8));
            cp8_vreg(cp8_asm_var(x, nnn), cp8) = cp8_vreg(cp8_asm_var(y, nnn), cp8) - cp8_vreg(cp8_asm_var(x, nnn), cp8);
            break;

        case 0xe:
            // INFO(Rafael): SHL Vx {, Vy}
            cp8->v[0xf] = (cp8_vreg(cp8_asm_var(x, nnn), cp8) >> 7);
            cp8_vreg(cp8_asm_var(x, nnn), cp8) >>= 1;
            break;

    }

    return (cp8->pc + 1);
}
