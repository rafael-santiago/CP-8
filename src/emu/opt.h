/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#ifndef CP8_EMU_OPT_H
#define CP8_EMU_OPT_H 1

char *cp8_emu_option(const char *option, char *default_value);

int cp8_emu_bool_option(const char *option, int default_value);

void cp8_emu_set_argc_argv(int argc, char **argv);

#endif