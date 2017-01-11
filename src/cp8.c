/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <vid/vid.h>
#include <kbd/kbd.h>
#include <accacia.h>

int main(int argc, char **argv) {
    cp8_vidinit();
    cp8_kbdinit();
    accacia_getch();
    cp8_vidfini();
    return 0;
}
