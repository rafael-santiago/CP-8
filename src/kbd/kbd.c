/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <kbd/kbd.h>
#include <accacia.h>

int cp8_kbdhit(void) {
    return accacia_kbhit();
}

unsigned char cp8_kbdwait(void) {
    return accacia_getch();
}
