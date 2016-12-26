#ifndef CP8_CTX_TYPES_H
#define CP8_CTX_TYPES_H 1

#include <stdlib.h>

#define CP8_STACK_SZ 0x10

#define cp8_stack_ceil(cp8) ( ( &(cp8)->stack[CP8_STACK_SZ] ) )

#define cp8_stack_floor(cp8) ( ( &(cp8)->stack[0] ) )

#define CP8_MEMORY_SZ 4096

struct cp8_ctx {
    unsigned char v[8];
    unsigned char dt, st;
    size_t *pc;
    size_t *sp;
    size_t stack[CP8_STACK_SZ];
};

#endif
