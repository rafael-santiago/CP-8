/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <mem/mem.h>
#include <ctx/types.h>
#include <string.h>

unsigned char g_cp8_mmap[CP8_MEMORY_SZ];

void cp8_memset(unsigned short addr, unsigned char value) {
    g_cp8_mmap[addr] = value;
}

unsigned char cp8_memget(unsigned short addr) {
    return g_cp8_mmap[addr];
}

void cp8_memzero(void) {
    memset(g_cp8_mmap, 0, sizeof(g_cp8_mmap));
}
