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
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>


typedef int (*cp8_emu_task)(void);

struct cp8_emu_tasks_ctx {
    const char *name;
    cp8_emu_task task;
};

static struct termios g_oldattr;

#define cp8_emu_register_task(t) { #t, cp8_emu_tsk_ ## t }

void cp8_emu_init(struct cp8_ctx *cp8) {
    struct termios attr;
    int res;
#ifndef NO_PTHREAD_SUPPORT
    pthread_mutexattr_t mtx_attr;
#endif

    tcgetattr(STDIN_FILENO, &attr);
    g_oldattr = attr;
    attr.c_lflag = ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &attr);
    ioctl(STDIN_FILENO, FIONREAD, &res);
    setbuf(stdout, NULL);

    if (cp8 != NULL) {
        memset(cp8, 0, sizeof(struct cp8_ctx));
#ifndef NO_PTHREAD_SUPPORT
        pthread_mutexattr_init(&mtx_attr);
        pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&cp8->mtx, &mtx_attr);
#endif
    }
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
        printf("ERROR: no valid tasks were supplied.\n");
        exit_code = 1;
    }

    return exit_code;
}

void cp8_emu_finis(void) {
    cp8_vidfinis();
    tcsetattr(STDIN_FILENO, TCSANOW, &g_oldattr);
}

#undef cp8_emu_register_task
