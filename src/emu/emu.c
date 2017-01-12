/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <emu/emu.h>
#include <emu/opt.h>
#include <vid/vid.h>
#include <kbd/kbd.h>

void cp8_emu_init(int argc, char **argv) {
    cp8_emu_set_argc_argv(argc, argv);
    cp8_vidinit();
    cp8_kbdinit();
}

int cp8_emu_exec(void) {
    return 0;
}

void cp8_emu_finis(void) {
    cp8_vidfini();
}
