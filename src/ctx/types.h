/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#ifndef CP8_CTX_TYPES_H
#define CP8_CTX_TYPES_H 1

#include <stdlib.h>

#ifndef NO_PTHREAD_SUPPORT

#include <pthread.h>

#endif

#define CP8_STACK_SZ 0x10

#define cp8_stack_ceil(cp8) ( ( &(cp8)->stack[CP8_STACK_SZ - 1] ) )

#define cp8_stack_floor(cp8) ( ( &(cp8)->stack[0] ) )

#define cp8_stack_top(cp8) ( *(cp8)->sp )

#define CP8_MEMORY_SZ 4096

#define CP8_TEXT_START 0x0200 // INFO(Rafael): No support for ETI 660 computers' ROMs.

#define CP8_FSPRITE_REGION 0x0

#define CP8_VIDMEM_MX 64

#define CP8_VIDMEM_MY 32

#define CP8_REGISTER_NR 0x10

#define CP8_MAX_INSTRUCTIONS_PER_CYCLE 220  // INFO(Rafael): Depending on your environment you should increment/decrement it.

#define CP8_CLOCK_IN_MICROSECS 16000

struct cp8_ctx {
#ifndef NO_PTHREAD_SUPPORT
    pthread_mutex_t mtx;
#endif
    unsigned char v[CP8_REGISTER_NR];
    unsigned char dt, st;
    unsigned short i;
    unsigned short pc;
    unsigned short *sp;
    unsigned short stack[CP8_STACK_SZ];
};

#define cp8_vreg(r, cp8) ( (cp8)->v[(r)] )

#define cp8_asm_var(var, innn) ( cp8_ ## var (innn) )

// INFO(Rafael): Variables mnemonics used on assembly instructions.

#define cp8_x(nnn) ( ((nnn) >> 8) )  //  WARN(Rafael): Taking in consideration that 'nnn' was acquired using the designed macro for.

#define cp8_y(nnn) ( ( ((nnn) >> 4) & 0xf ) )

#define cp8_nnn(i)  ( ( (i) & 0x0fff ) )

#define cp8_kk(nnn) ( ( (nnn) & 0xff ) )

#define cp8_n(nnn) ( ( (nnn) & 0xf ) )

#define CP8_BLITCHAR_MH 5

#define CP8_BLITCHAR_MW  8

#define CP8_EMU_TSK_EMULATE_KQUIT 27

#define CP8_VERSION "0.0.1"

#endif
