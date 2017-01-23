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

#ifndef NO_PTHREAD_SUPPORT

static pthread_mutex_t g_cp8_emu_mtx = PTHREAD_MUTEX_INITIALIZER;

#endif

typedef int (*cp8_emu_task)(void);

struct cp8_emu_tasks_ctx {
    const char *name;
    cp8_emu_task task;
};

static struct termios g_oldattr;

#define cp8_emu_register_task(t) { #t, cp8_emu_tsk_ ## t }

static int cp8_help(void);

static int cp8_version(void);

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

#ifndef NO_PTHREAD_SUPPORT
    pthread_mutexattr_init(&mtx_attr);
    pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&g_cp8_emu_mtx, &mtx_attr);
#endif

    if (cp8 != NULL) {
        memset(cp8, 0, sizeof(struct cp8_ctx));
#ifndef NO_PTHREAD_SUPPORT
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

    if (cp8_emu_bool_option("version", 0)) {
        return cp8_version();
    } else if (cp8_emu_bool_option("help", 0)) {
        return cp8_help();
    }

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

#ifndef NO_PTHREAD_SUPPORT

void cp8_emu_lock(void) {
    pthread_mutex_lock(&g_cp8_emu_mtx);
}

void cp8_emu_unlock(void) {
    pthread_mutex_unlock(&g_cp8_emu_mtx);
}

#endif

static int cp8_help(void) {
    printf("usage: cp8 emulate <options>\n\n"
           "cp8 is Copyright (C) 2017 by Rafael Santiago.\n\n"
           "Bug reports, feedback, etc: <voidbrainvoid@gmail.com> "
           "or <https://github.com/rafael-santiago/CP-8/issues>\n");
    return 0;
}

static int cp8_version(void) {
    printf("cp8-%s\n", CP8_VERSION);
    return 0;
}
