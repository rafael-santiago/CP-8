/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <emu/opt.h>
#include <emu/emu.h>

int main(int argc, char **argv) {
    cp8_emu_set_argc_argv(argc, argv);
    return cp8_emu_exec();
}
