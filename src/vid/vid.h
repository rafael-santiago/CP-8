/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#ifndef CP8_VID_VID_H
#define CP8_VID_VID_H 1

void cp8_vidcls(void);

int cp8_viddrw(const unsigned char x, const unsigned char y, const char unsigned n);

int cp8_vidinit(void);

void cp8_vidset_tcolor(const int color);

void cp8_vidset_bcolor(const int color);

int cp8_vidfini(void);

#endif
