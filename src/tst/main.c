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

CUTE_TEST_CASE_SUITE(cp8_cpu_suite)
CUTE_TEST_CASE_SUITE_END

// --

CUTE_TEST_CASE(cp8_tests)
    CUTE_RUN_TEST_SUITE(cp8_mem_suite);
    CUTE_RUN_TEST_SUITE(cp8_ctx_suite);
    CUTE_RUN_TEST_SUITE(cp8_vid_suite);
    CUTE_RUN_TEST_SUITE(cp8_cpu_suite);
    CUTE_RUN_TEST_SUITE(cp8_opt_suite);
    CUTE_RUN_TEST_SUITE(cp8_rom_suite);
CUTE_TEST_CASE_END

CUTE_MAIN(cp8_tests);
