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

unsigned short itpf_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    unsigned char x, y;
    unsigned short next = 2;

    switch (nnn & 0xff) {
        case 0x07:
#ifndef NO_PTHREAD_SUPPORT
            pthread_mutex_lock(&cp8->mtx);
#endif
            // INFO(Rafael): LD Vx, DT
            cp8_vreg(cp8_asm_var(x, nnn), cp8) = cp8->dt;

#ifndef NO_PTHREAD_SUPPORT
            pthread_mutex_unlock(&cp8->mtx);
#endif
            break;

        case 0x0a:
            // INFO(Rafael): LD Vx, K
            x = cp8_kbdhit();
            if (x != 0xff) {
                cp8_vreg(cp8_asm_var(x, nnn), cp8) = x;
            } else {
                next = 0;
            }
            break;

        case 0x15:
#ifndef NO_PTHREAD_SUPPORT
            pthread_mutex_lock(&cp8->mtx);
#endif

            // INFO(Rafael): LD DT, Vx
            cp8->dt = cp8_vreg(cp8_asm_var(x, nnn), cp8);

#ifndef NO_PTHREAD_SUPPORT
            pthread_mutex_unlock(&cp8->mtx);
#endif
            break;

        case 0x18:
#ifndef NO_PTHREAD_SUPPORT
            pthread_mutex_lock(&cp8->mtx);
#endif

            // INFO(Rafael): LD ST, Vx
            cp8->st = cp8_vreg(cp8_asm_var(x, nnn), cp8);

#ifndef NO_PTHREAD_SUPPORT
            pthread_mutex_unlock(&cp8->mtx);
#endif
            break;

        case 0x1e:
            // INFO(Rafael): ADD I, Vx
            cp8->i += cp8_vreg(cp8_asm_var(x, nnn), cp8);
            break;

        case 0x29:
            // INFO(Rafael): LD I, Vx (?)
            cp8->i = cp8_vreg(cp8_asm_var(x, nnn), cp8) * 0x5;
            break;

        case 0x33:
            // INFO(Rafael): LD B, Vx
            // WARN(Rafael): Pretty slow, maybe it could be improved. Anyway, by now, this is what we get.
            x = cp8_vreg(cp8_asm_var(x, nnn), cp8);
            y = x / 100;
            cp8_memset(cp8->i, y);
            x = x - (y * 100);
            y = x / 10;
            cp8_memset(cp8->i + 1, y);
            y = x - (y * 10);
            cp8_memset(cp8->i + 2, y);
            break;

        case 0x55:
            // INFO(Rafael): LD [I], Vx
            for (x = 0; x <= cp8_asm_var(x, nnn); x++) {
                cp8_memset(cp8->i + x, cp8_vreg(x, cp8));
            }
            break;

        case 0x65:
            // INFO(Rafael): LD Vx, [I]
            for (x = 0; x <= cp8_asm_var(x, nnn); x++) {
                cp8_vreg(x, cp8) = cp8_memget(cp8->i + x);
            }
            break;
    }

    return (cp8->pc + next);
}
