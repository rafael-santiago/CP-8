/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <cpu/itp/itpa.h>

unsigned short itpa_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    // INFO(Rafael): LD I, addr
    cp8->i = nnn;

    return (cp8->pc + 2);
}
