/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#ifndef CP8_EMU_EMU_H
#define CP8_EMU_EMU_H 1

#include <ctx/types.h>

void cp8_emu_init(struct cp8_ctx *cp8);

int cp8_emu_exec(void);

void cp8_emu_finis(void);

#ifndef NO_PTHREAD_SUPPORT

void cp8_emu_lock(void);

void cp8_emu_unlock(void);

#endif

#endif