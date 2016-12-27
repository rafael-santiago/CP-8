#ifndef CP8_CTX_CTX_H
#define CP8_CTX_CTX_H 1

#include <ctx/types.h>

void cp8_push(struct cp8_ctx *cp8, unsigned short nnn);

unsigned short cp8_pop(struct cp8_ctx *cp8);

#endif
