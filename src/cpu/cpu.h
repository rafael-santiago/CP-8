/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#ifndef CP8_CPU_CPU_H
#define CP8_CPU_CPU_H 1

#include <ctx/types.h>

struct cp8_ins_gate_ctx {
    unsigned short (*itrap)(const unsigned short nnn, struct cp8_ctx *cp8);
};

extern struct cp8_ins_gate_ctx g_cp8_cpu_ins_gate[];

// INFO(Rafael): Let's avoid a useless call overhead.

#define cp8_cpu_exec(i, c) ( g_cp8_cpu_ins_gate[((i) >> 12)].itrap( cp8_asm_var(nnn, i), c) )

#endif
