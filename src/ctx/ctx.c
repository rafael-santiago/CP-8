/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <ctx/ctx.h>

void cp8_push(struct cp8_ctx *cp8, unsigned short nnn) {
    if (cp8 == NULL || cp8->sp == cp8_stack_ceil(cp8)) {
        return;
    }

    if (cp8->sp == NULL) {
        cp8->sp = &cp8->stack[0];
    } else {
        cp8->sp = cp8->sp + sizeof(cp8->stack[0]);
    }

    *cp8->sp = nnn;
}

unsigned short cp8_pop(struct cp8_ctx *cp8) {
    unsigned short value = 0;

    if (cp8 == NULL || cp8->sp == NULL) {
        return 0;
    }

    value = cp8_stack_top(cp8);

    if (cp8->sp == cp8_stack_floor(cp8)) {
        cp8->sp = NULL;
    } else {
        cp8->sp = cp8->sp - sizeof(cp8->stack[0]);
    }

    return value;
}
