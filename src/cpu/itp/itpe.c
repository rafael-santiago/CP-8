/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <cpu/itp/itpe.h>
#include <kbd/kbd.h>

void itpe_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    switch (nnn & 0xff) {
        case 0x9e:
            // INFO(Rafael): SKP Vx
            if (cp8_kbdhit() == cp8_vreg(cp8_asm_var(x, nnn), cp8)) {
                cp8->pc += 2;
            }
            break;

        case 0xa1:
            // INFO(Rafael): SKNP Vx
            if (cp8_kbdhit() != cp8_vreg(cp8_asm_var(x, nnn), cp8)) {
                cp8->pc += 2;
            }
            break;
    }
}
