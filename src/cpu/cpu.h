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
#include <cpu/itp/itp0.h>
#include <cpu/itp/itp1.h>
#include <cpu/itp/itp2.h>
#include <cpu/itp/itp3.h>
#include <cpu/itp/itp4.h>
#include <cpu/itp/itp5.h>
#include <cpu/itp/itp6.h>
#include <cpu/itp/itp7.h>
#include <cpu/itp/itp8.h>
#include <cpu/itp/itp9.h>
#include <cpu/itp/itpa.h>
#include <cpu/itp/itpb.h>
#include <cpu/itp/itpc.h>
#include <cpu/itp/itpd.h>
#include <cpu/itp/itpe.h>
#include <cpu/itp/itpf.h>

struct cp8_ins_gate_ctx {
    unsigned short (*itrap)(const unsigned short nnn, struct cp8_ctx *cp8);
};

#define set_cp8_itrap(i) ( itp ## i ## _gate )

struct cp8_ins_gate_ctx g_cp8_cpu_ins_gate[] = {
    set_cp8_itrap(0), set_cp8_itrap(1), set_cp8_itrap(2), set_cp8_itrap(3),
    set_cp8_itrap(4), set_cp8_itrap(5), set_cp8_itrap(6), set_cp8_itrap(7),
    set_cp8_itrap(8), set_cp8_itrap(9), set_cp8_itrap(a), set_cp8_itrap(b),
    set_cp8_itrap(c), set_cp8_itrap(d), set_cp8_itrap(e), set_cp8_itrap(f)
};

#undef set_cp8_itrap

// INFO(Rafael): Let's avoid a useless call overhead.

#define cp8_cpu_exec(i, c) ( g_cp8_cpu_ins_gate[((i) >> 12)].itrap( cp8_asm_var(nnn, i), c) )

#endif
