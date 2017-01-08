/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <cpu/itp/itpb.h>

unsigned short itpb_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    // INFO(Rafael): JP V0, addr
    cp8->pc = nnn + cp8->v[0];

    return cp8->pc;
}
