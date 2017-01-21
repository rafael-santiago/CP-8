/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#ifndef NO_PTHREAD_SUPPORT

#include <tmr/tmr.h>
#include <ctx/types.h>
#include <kbd/kbd.h>
#include <emu/tsk.h>
#include <unistd.h>

void *cp8_tmrtck(void *args) {
    struct cp8_ctx *p = (struct cp8_ctx *)args;

    while (cp8_kbdlkey() != CP8_EMU_TSK_EMULATE_KQUIT) {
        pthread_mutex_lock(&p->mtx);

        if (p->st > 0) {
            p->st--;
        }

        if (p->dt > 0) {
            p->dt--;
        }

        pthread_mutex_unlock(&p->mtx);

        usleep(CP8_CLOCK_IN_MICROSECS);
    }

    return NULL;
}

#endif
