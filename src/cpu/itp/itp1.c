#include <cpu/itp/itp1.h>

void itp1_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    // INFO(Rafael): JP addr
    cp8->pc = nnn;
}
