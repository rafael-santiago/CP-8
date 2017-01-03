/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <cpu/itp/itpf.h>
#include <kbd/kbd.h>
#include <mem/mem.h>

void itpf_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    unsigned char x;

    switch (nnn & 0xff) {
        case 0x07:
            // INFO(Rafael): LD Vx, DT
            cp8_vreg(cp8_asm_var(x, nnn), cp8) = cp8->dt;
            break;

        case 0x0a:
            // INFO(Rafael): LD Vx, K
            cp8_vreg(cp8_asm_var(x, nnn), cp8) = cp8_kbdwait();
            break;

        case 0x15:
            // INFO(Rafael): LD DT, Vx
            cp8->dt = cp8_vreg(cp8_asm_var(x, nnn), cp8);
            break;

        case 0x18:
            // INFO(Rafael): LD ST, Vx
            cp8->st = cp8_vreg(cp8_asm_var(x, nnn), cp8);
            break;

        case 0x1e:
            // INFO(Rafael): ADD I, Vx
            cp8->i += cp8_vreg(cp8_asm_var(x, nnn), cp8);
            break;

        case 0x29:
            // INFO(Rafael): LD I, Vx (?)
            cp8->i = cp8_vreg(cp8_asm_var(x, nnn), cp8);
            break;

        case 0x33:
            // TODO(Rafael): LD B, Vx
            break;

        case 0x55:
            // INFO(Rafael): LD [I], Vx
            for (x = 0; x < cp8_vreg(cp8_asm_var(x, nnn), cp8); x++) {
                cp8_memset(cp8->i + x, cp8_vreg(x, cp8));
            }
            break;

        case 0x65:
            // INFO(Rafael): LD Vx, [I]
            for (x = 0; x < cp8_vreg(cp8_asm_var(x, nnn), cp8); x++) {
                cp8_vreg(x, cp8) = cp8_memget(cp8->i + x);
            }
            break;
    }
}
