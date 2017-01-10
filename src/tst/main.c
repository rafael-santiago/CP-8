/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <cutest.h>
#include <mem/mem.h>
#include <ctx/ctx.h>
#include <ctx/types.h>
#include <cpu/cpu.h>
#include <stdlib.h>
#include <time.h>

// MEM

CUTE_TEST_CASE(cp8_memzero_tests)
    size_t m;
    cp8_memzero();
    for (m = 0; m < CP8_MEMORY_SZ; m++) {
        CUTE_ASSERT(cp8_memget(m) == 0);
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_memget_memset_tests)
    unsigned short addr;
    srand(time(0));
    addr = rand() % CP8_MEMORY_SZ;
    cp8_memset(addr, 0xf);
    CUTE_ASSERT_EQ(cp8_memget(addr), 0xf);
CUTE_TEST_CASE_END

CUTE_TEST_CASE_SUITE(cp8_mem_suite)
    CUTE_RUN_TEST(cp8_memzero_tests);
    CUTE_RUN_TEST(cp8_memget_memset_tests);
CUTE_TEST_CASE_SUITE_END

// --

// CTX

CUTE_TEST_CASE(cp8_stack_tests)
    struct cp8_ctx cp8;
    cp8.sp = NULL;
    cp8_push(&cp8, 0x1);
    cp8_push(&cp8, 0x1);
    cp8_push(&cp8, 0x2);
    cp8_push(&cp8, 0x3);
    cp8_push(&cp8, 0x4);
    cp8_push(&cp8, 0x5);
    cp8_push(&cp8, 0x6);
    cp8_push(&cp8, 0x7);
    cp8_push(&cp8, 0x8);
    cp8_push(&cp8, 0x9);
    cp8_push(&cp8, 0xa);
    cp8_push(&cp8, 0xb);
    cp8_push(&cp8, 0xc);
    cp8_push(&cp8, 0xd);
    cp8_push(&cp8, 0xe);
    cp8_push(&cp8, 0xf);
    cp8_push(&cp8, 0xe);
    cp8_push(&cp8, 0xd);
    cp8_push(&cp8, 0xc);
    cp8_push(&cp8, 0xb);
    cp8_push(&cp8, 0xa);
    cp8_push(&cp8, 0x9);
    CUTE_ASSERT(cp8_pop(&cp8) == 0xf);
    CUTE_ASSERT(cp8_pop(&cp8) == 0xe);
    CUTE_ASSERT(cp8_pop(&cp8) == 0xd);
    CUTE_ASSERT(cp8_pop(&cp8) == 0xc);
    CUTE_ASSERT(cp8_pop(&cp8) == 0xb);
    CUTE_ASSERT(cp8_pop(&cp8) == 0xa);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x9);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x8);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x7);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x6);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x5);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x4);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x3);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x2);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x1);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x1);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x0);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x0);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x0);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x0);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x0);
    CUTE_ASSERT(cp8_pop(&cp8) == 0x0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE_SUITE(cp8_ctx_suite)
    CUTE_RUN_TEST(cp8_stack_tests);
CUTE_TEST_CASE_SUITE_END

// --

// OPT

CUTE_TEST_CASE_SUITE(cp8_opt_suite)
CUTE_TEST_CASE_SUITE_END

// --

// ROM

CUTE_TEST_CASE_SUITE(cp8_rom_suite)
CUTE_TEST_CASE_SUITE_END

// --

// VID

CUTE_TEST_CASE_SUITE(cp8_vid_suite)
CUTE_TEST_CASE_SUITE_END

// --

// CPU

CUTE_TEST_CASE(cp8_cpu_CLS_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x00e0, &cp8) == 1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_RET_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8_push(&cp8, 0xc0de);
    CUTE_ASSERT(cp8_cpu_exec(0x00ee, &cp8) == 0xc0de);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_JP_addr_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x121e, &cp8) == 0x021e);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_CALL_addr_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x2ca1, &cp8) == 0x0ca1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SE_Vx_byte_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8.v[0] = 6;
    CUTE_ASSERT(cp8_cpu_exec(0x3009, &cp8) == 1);
    cp8.pc = 0;
    cp8.v[0] = 9;
    CUTE_ASSERT(cp8_cpu_exec(0x3009, &cp8) == 2);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SNE_Vx_byte_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8.v[0] = 9;
    CUTE_ASSERT(cp8_cpu_exec(0x4009, &cp8) == 1);
    cp8.pc = 0;
    cp8.v[0] = 6;
    CUTE_ASSERT(cp8_cpu_exec(0x4009, &cp8) == 2);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SE_Vx_Vy_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8.v[1] = 0xff;
    cp8.v[4] = 0xf1;
    CUTE_ASSERT(cp8_cpu_exec(0x5140, &cp8) == 1);
    cp8.v[4] = 0xff;
    cp8.pc = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x5140, &cp8) == 2);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_LD_Vx_byte_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8.v[0] = 0;
    cp8.pc = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x60ba, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0] == 0xba);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_ADD_Vx_byte_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8.v[9] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x7902, &cp8) == 1);
    CUTE_ASSERT(cp8.v[9] == 3);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_LD_Vx_Vy_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8.v[0xb] = 255;
    cp8.v[0xd] = 2;
    CUTE_ASSERT(cp8_cpu_exec(0x8bd0, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0xb] == 2);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_OR_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x0] = 0;
    cp8.v[0xf] = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x80f1, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 0);

    cp8.pc = 0;
    cp8.v[0x0] = 1;
    cp8.v[0xf] = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x80f1, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 1);

    cp8.pc = 0;
    cp8.v[0x0] = 0;
    cp8.v[0xf] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x80f1, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 1);

    cp8.pc = 0;
    cp8.v[0x0] = 1;
    cp8.v[0xf] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x80f1, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_AND_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x0] = 0;
    cp8.v[0xf] = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x80f2, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 0);

    cp8.pc = 0;
    cp8.v[0x0] = 1;
    cp8.v[0xf] = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x80f2, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 0);

    cp8.pc = 0;
    cp8.v[0x0] = 0;
    cp8.v[0xf] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x80f2, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 0);

    cp8.pc = 0;
    cp8.v[0x0] = 1;
    cp8.v[0xf] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x80f2, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_XOR_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x0] = 0;
    cp8.v[0xf] = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x80f3, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 0);

    cp8.pc = 0;
    cp8.v[0x0] = 1;
    cp8.v[0xf] = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x80f3, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 1);

    cp8.pc = 0;
    cp8.v[0x0] = 0;
    cp8.v[0xf] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x80f3, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 1);

    cp8.pc = 0;
    cp8.v[0x0] = 1;
    cp8.v[0xf] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x80f3, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_ADD_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0xe] = 254;
    cp8.v[0xc] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x8ec4, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0xe] == 255);
    CUTE_ASSERT(cp8.v[0xf] == 0);

    cp8.pc = 0;
    cp8.v[0xe] = 255;
    cp8.v[0xc] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x8ec4, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0xe] == 0);
    CUTE_ASSERT(cp8.v[0xf] == 1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SUB_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0xe] = 254;
    cp8.v[0xc] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x8ec5, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0xe] == 253);
    CUTE_ASSERT(cp8.v[0xf] == 1);

    cp8.pc = 0;
    cp8.v[0xe] = 1;
    cp8.v[0xc] = 254;
    CUTE_ASSERT(cp8_cpu_exec(0x8ec5, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0xe] == 3);
    CUTE_ASSERT(cp8.v[0xf] == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SHR_Vx_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x0] = 0xe0;
    CUTE_ASSERT(cp8_cpu_exec(0x8006, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 0x70);
    CUTE_ASSERT(cp8.v[0xf] == 0);

    cp8.pc = 0;
    cp8.v[0x0] = 0xe1;
    CUTE_ASSERT(cp8_cpu_exec(0x8006, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 0x70);
    CUTE_ASSERT(cp8.v[0xf] == 1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SUBN_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x0] = 0x2;
    cp8.v[0x1] = 0x3;
    CUTE_ASSERT(cp8_cpu_exec(0x8017, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 1);

    cp8.pc = 0;
    cp8.v[0x0] = 0x2;
    cp8.v[0x1] = 0x1;
    CUTE_ASSERT(cp8_cpu_exec(0x8017, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 255);
    CUTE_ASSERT(cp8.v[0xf] == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SHL_Vx_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x0] = 0xe0;
    CUTE_ASSERT(cp8_cpu_exec(0x800e, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 0xc0);
    CUTE_ASSERT(cp8.v[0xf] == 1);

    cp8.pc = 0;
    cp8.v[0x0] = 0x20;
    CUTE_ASSERT(cp8_cpu_exec(0x800e, &cp8) == 1);
    CUTE_ASSERT(cp8.v[0x0] == 0x40);
    CUTE_ASSERT(cp8.v[0xf] == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE_SUITE(cp8_cpu_suite)
    CUTE_RUN_TEST(cp8_cpu_CLS_tests);
    CUTE_RUN_TEST(cp8_cpu_RET_tests);
    CUTE_RUN_TEST(cp8_cpu_JP_addr_tests);
    CUTE_RUN_TEST(cp8_cpu_CALL_addr_tests);
    CUTE_RUN_TEST(cp8_cpu_SE_Vx_byte_tests);
    CUTE_RUN_TEST(cp8_cpu_SNE_Vx_byte_tests);
    CUTE_RUN_TEST(cp8_cpu_SE_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_cpu_LD_Vx_byte_tests);
    CUTE_RUN_TEST(cp8_cpu_ADD_Vx_byte_tests);
    CUTE_RUN_TEST(cp8_cpu_LD_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_cpu_OR_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_cpu_AND_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_cpu_XOR_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_cpu_ADD_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_cpu_SUB_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_cpu_SHR_Vx_tests);
    CUTE_RUN_TEST(cp8_cpu_SUBN_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_cpu_SHL_Vx_tests);
CUTE_TEST_CASE_SUITE_END

// --

// MACROS

CUTE_TEST_CASE(cp8_macros_suite)
    struct cp8_ctx cp8;
    unsigned short instr = 0xd128;

    cp8.v[0x0] = 0x0;
    cp8.v[0x1] = 0x1;
    cp8.v[0x2] = 0x2;
    cp8.v[0x3] = 0x3;
    cp8.v[0x4] = 0x4;
    cp8.v[0x5] = 0x5;
    cp8.v[0x6] = 0x6;
    cp8.v[0x7] = 0x7;
    cp8.v[0x8] = 0x8;
    cp8.v[0x9] = 0x9;
    cp8.v[0xa] = 0xa;
    cp8.v[0xb] = 0xb;
    cp8.v[0xc] = 0xc;
    cp8.v[0xd] = 0xd;
    cp8.v[0xe] = 0xe;
    cp8.v[0xf] = 0xf;

    CUTE_ASSERT(cp8_vreg(0x0, &cp8) == 0x0);
    CUTE_ASSERT(cp8_vreg(0x1, &cp8) == 0x1);
    CUTE_ASSERT(cp8_vreg(0x2, &cp8) == 0x2);
    CUTE_ASSERT(cp8_vreg(0x3, &cp8) == 0x3);
    CUTE_ASSERT(cp8_vreg(0x4, &cp8) == 0x4);
    CUTE_ASSERT(cp8_vreg(0x5, &cp8) == 0x5);
    CUTE_ASSERT(cp8_vreg(0x6, &cp8) == 0x6);
    CUTE_ASSERT(cp8_vreg(0x7, &cp8) == 0x7);
    CUTE_ASSERT(cp8_vreg(0x8, &cp8) == 0x8);
    CUTE_ASSERT(cp8_vreg(0x9, &cp8) == 0x9);
    CUTE_ASSERT(cp8_vreg(0xa, &cp8) == 0xa);
    CUTE_ASSERT(cp8_vreg(0xb, &cp8) == 0xb);
    CUTE_ASSERT(cp8_vreg(0xc, &cp8) == 0xc);
    CUTE_ASSERT(cp8_vreg(0xd, &cp8) == 0xd);
    CUTE_ASSERT(cp8_vreg(0xe, &cp8) == 0xe);
    CUTE_ASSERT(cp8_vreg(0xf, &cp8) == 0xf);

    CUTE_ASSERT(cp8_asm_var(nnn, instr) == 0x0128);

    CUTE_ASSERT(cp8_asm_var(x, cp8_asm_var(nnn, instr)) == 0x1);
    CUTE_ASSERT(cp8_asm_var(y, cp8_asm_var(nnn, instr)) == 0x2);
    CUTE_ASSERT(cp8_asm_var(n, cp8_asm_var(nnn, instr)) == 0x8);
    CUTE_ASSERT(cp8_asm_var(kk, cp8_asm_var(nnn, instr)) == 0x28);
CUTE_TEST_CASE_END

// --

CUTE_TEST_CASE(cp8_tests)
    CUTE_RUN_TEST_SUITE(cp8_macros_suite);
    CUTE_RUN_TEST_SUITE(cp8_mem_suite);
    CUTE_RUN_TEST_SUITE(cp8_ctx_suite);
    CUTE_RUN_TEST_SUITE(cp8_vid_suite);
    CUTE_RUN_TEST_SUITE(cp8_cpu_suite);
    CUTE_RUN_TEST_SUITE(cp8_opt_suite);
    CUTE_RUN_TEST_SUITE(cp8_rom_suite);
CUTE_TEST_CASE_END

CUTE_MAIN(cp8_tests);
