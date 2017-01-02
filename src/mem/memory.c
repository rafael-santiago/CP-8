/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <mem/memory.h>
#include <ctx/types.h>

unsigned char g_cp8_mmap[CP8_MEMORY_SZ];

void set_maddr(unsigned short addr, unsigned char value) {
    g_cp8_mmap[addr] = value;
}

unsigned short get_maddr(unsigned short addr) {
    return g_cp8_mmap[addr];
}
