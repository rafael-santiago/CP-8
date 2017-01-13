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
#include <emu/tsk.h>
#include <stdlib.h>
#include <stdio.h>

typedef int (*cp8_emu_task)(void);

struct cp8_emu_tasks_ctx {
    const char *name;
    cp8_emu_task task;
};

#define cp8_emu_register_task(t) { #t, cp8_emu_tsk_ ## t }

void cp8_emu_init(int argc, char **argv) {
    cp8_emu_set_argc_argv(argc, argv);
    cp8_vidinit();
    cp8_kbdinit();
}

int cp8_emu_exec(void) {
    struct cp8_emu_tasks_ctx tasks [] = {
        cp8_emu_register_task(emulate)
    };
    const size_t tasks_nr = sizeof(tasks) / sizeof(tasks[0]);
    size_t t;
    int exit_code = 0;
    cp8_emu_task curr_task = NULL;

    for (t = 0; t < tasks_nr; t++) {
        if (cp8_emu_task_option(tasks[t].name)) {
            exit_code += (curr_task = tasks[t].task)();
        }
    }

    if (curr_task == NULL) {
        printf("ERROR: any valid task was supplied.\n");
        exit_code = 1;
    }

    return exit_code;
}

void cp8_emu_finis(void) {
    cp8_vidfinis();
}

#undef cp8_emu_register_task
