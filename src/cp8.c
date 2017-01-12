/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <emu/emu.h>

int main(int argc, char **argv) {
    int exit_code = 1;
    cp8_emu_init(argc, argv);
    exit_code = cp8_emu_exec();
    cp8_emu_finis();
    return exit_code;
}
