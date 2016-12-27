#include <cpu/itp/itp0.h>
#include <ctx/ctx.h>
#include <vid/vid.h>

void itp0_gate(const unsigned short nnn, struct cp8_ctx *cp8) {
    switch (nnn) {
        case 0x00e0:
            //  INFO(Rafael): CLS
            cp8_vidcls();
            break;

        case 0x00ee:
            //  INFO(Rafael): RET
            cp8->pc = cp8_pop(cp8);
            break;

        default:
            // INFO(Rafael): SYS addr is a pretty old stuff and should be ignored.
            break;
    }
}
