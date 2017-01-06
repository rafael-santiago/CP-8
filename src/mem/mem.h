/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#ifndef CP8_MEM_MEM_H
#define CP8_MEM_MEM_H 1

void cp8_memset(unsigned short addr, unsigned char value);

unsigned char cp8_memget(unsigned short addr);

void cp8_memzero(void);

#endif
