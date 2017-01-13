/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <emu/opt.h>
#include <stdio.h>
#include <string.h>

static int g_cp8_emu_argc = 0;

static char **g_cp8_emu_argv = NULL;

static void *cp8_emu_get_option(const char *option, void *default_value, const int is_bool, const int is_task);

static void *cp8_emu_get_option(const char *option, void *default_value, const int is_bool, const int is_task) {
    char temp[255];
    int a, matches;
    static int has = 1;

    if (option == NULL || g_cp8_emu_argv == NULL) {
        return default_value;
    }

    sprintf(temp, "%s%s%s", (!is_task) ? "--" : "", option, (!is_bool) ? "=" : "");

    for (a = 0; a < g_cp8_emu_argc; a++) {
        if (is_bool) {
            matches = (strcmp(temp, g_cp8_emu_argv[a]) == 0);
        } else {
            matches = (strstr(g_cp8_emu_argv[a], temp) == &g_cp8_emu_argv[a][0]);
        }

        if (matches) {
            return (!is_bool) ? ( (void *) (&g_cp8_emu_argv[a][0] + strlen(temp)) ) : (void *)&has;
        }
    }

    return default_value;
}

char *cp8_emu_option(const char *option, char *default_value) {
    return cp8_emu_get_option(option, default_value, 0, 0);
}

int cp8_emu_bool_option(const char *option, int default_value) {
    return *(int *)cp8_emu_get_option(option, &default_value, 1, 0);
}

int cp8_emu_task_option(const char *option) {
    int notask = 0;
    return *(int *)cp8_emu_get_option(option, &notask, 1, 1);
}

void cp8_emu_set_argc_argv(int argc, char **argv) {
    g_cp8_emu_argc = argc;
    g_cp8_emu_argv = argv;
}
