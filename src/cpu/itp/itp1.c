/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <cpu/itp/itp1.h>

void itp1_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    // INFO(Rafael): JP addr
    cp8->pc = nnn;
}
