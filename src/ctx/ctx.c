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
}

void cp8_pop(struct cp8_ctx *cp8) {
    if (cp8 == NULL || cp8->sp == NULL) {
        return;
    }

    if (cp8->sp == cp8_stack_floor(cp8)) {
        cp8->sp = NULL;
    } else {
        cp8->sp = cp8->sp - sizeof(cp8->stack[0]);
    }
}
