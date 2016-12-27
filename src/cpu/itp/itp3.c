#include <cpu/itp/itp3.h>

void itp3_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    // INFO(Rafael): SE Vx, byte
    if (cp8_vreg(cp8_asm_var(x, nnn), cp8) == cp8_asm_var(kk, nnn)) {
        cp8->pc += 2;
    }
}
