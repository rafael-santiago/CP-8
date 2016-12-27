#include <cpu/itp/itp5.h>

void itp5_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    // INFO(Rafael): SE Vx, Vy
    if (cp8_vreg(cp8_asm_var(x, nnn), cp8) == cp8_vreg(cp8_asm_var(y, nnn), cp8)) {
        cp8->pc += 2;
    }
}
