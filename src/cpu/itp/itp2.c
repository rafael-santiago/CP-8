/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <cpu/itp/itp2.h>
#include <ctx/ctx.h>

unsigned short itp2_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    // INFO(Rafael): CALL addr
    cp8_push(cp8, cp8->pc);
    cp8->pc = (nnn - CP8_TEXT_START);
    return cp8->pc;
}
