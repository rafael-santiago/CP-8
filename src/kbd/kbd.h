/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#ifndef CP8_KBD_KBD_H
#define CP8_KBD_KBD_H 1

unsigned char cp8_kbdhit(void);

void cp8_kbdinit(void);

unsigned char cp8_kbdlkey(void);

#ifdef NO_PTHREAD_SUPPORT

void cp8_kbdrelease(void);

void cp8_kbdread(void);

#endif

void cp8_kbdsetkey(unsigned char key);

#endif
