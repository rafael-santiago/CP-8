#include <cpu/itp/itp2.h>
#include <ctx/ctx.h>

void itp2_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    // INFO(Rafael): CALL addr
    cp8_push(cp8, cp8->pc);
    cp8->pc = nnn;
}
