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
#include <emu/opt.h>
#include <rom/ld.h>
#include <asm/umount.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

// ROM

CUTE_TEST_CASE(cp8_romld_tests)
    char msg[255];
    // INFO(Rafael): Space invaders v0.9 by David Winter.
    unsigned char invaders_rom[] = {
        0x12, 0x25, 0x53, 0x50, 0x41, 0x43, 0x45, 0x20, 0x49, 0x4e, 0x56, 0x41,
        0x44, 0x45, 0x52, 0x53, 0x20, 0x76, 0x30, 0x2e, 0x39, 0x20, 0x42, 0x79,
        0x20, 0x44, 0x61, 0x76, 0x69, 0x64, 0x20, 0x57, 0x49, 0x4e, 0x54, 0x45,
        0x52, 0x60, 0x00, 0x61, 0x00, 0x62, 0x08, 0xa3, 0xd3, 0xd0, 0x18, 0x71,
        0x08, 0xf2, 0x1e, 0x31, 0x20, 0x12, 0x2d, 0x70, 0x08, 0x61, 0x00, 0x30,
        0x40, 0x12, 0x2d, 0x69, 0x05, 0x6c, 0x15, 0x6e, 0x00, 0x23, 0x87, 0x60,
        0x0a, 0xf0, 0x15, 0xf0, 0x07, 0x30, 0x00, 0x12, 0x4b, 0x23, 0x87, 0x7e,
        0x01, 0x12, 0x45, 0x66, 0x00, 0x68, 0x1c, 0x69, 0x00, 0x6a, 0x04, 0x6b,
        0x0a, 0x6c, 0x04, 0x6d, 0x3c, 0x6e, 0x0f, 0x00, 0xe0, 0x23, 0x6b, 0x23,
        0x47, 0xfd, 0x15, 0x60, 0x04, 0xe0, 0x9e, 0x12, 0x7d, 0x23, 0x6b, 0x38,
        0x00, 0x78, 0xff, 0x23, 0x6b, 0x60, 0x06, 0xe0, 0x9e, 0x12, 0x8b, 0x23,
        0x6b, 0x38, 0x39, 0x78, 0x01, 0x23, 0x6b, 0x36, 0x00, 0x12, 0x9f, 0x60,
        0x05, 0xe0, 0x9e, 0x12, 0xe9, 0x66, 0x01, 0x65, 0x1b, 0x84, 0x80, 0xa3,
        0xcf, 0xd4, 0x51, 0xa3, 0xcf, 0xd4, 0x51, 0x75, 0xff, 0x35, 0xff, 0x12,
        0xad, 0x66, 0x00, 0x12, 0xe9, 0xd4, 0x51, 0x3f, 0x01, 0x12, 0xe9, 0xd4,
        0x51, 0x66, 0x00, 0x83, 0x40, 0x73, 0x03, 0x83, 0xb5, 0x62, 0xf8, 0x83,
        0x22, 0x62, 0x08, 0x33, 0x00, 0x12, 0xc9, 0x23, 0x73, 0x82, 0x06, 0x43,
        0x08, 0x12, 0xd3, 0x33, 0x10, 0x12, 0xd5, 0x23, 0x73, 0x82, 0x06, 0x33,
        0x18, 0x12, 0xdd, 0x23, 0x73, 0x82, 0x06, 0x43, 0x20, 0x12, 0xe7, 0x33,
        0x28, 0x12, 0xe9, 0x23, 0x73, 0x3e, 0x00, 0x13, 0x07, 0x79, 0x06, 0x49,
        0x18, 0x69, 0x00, 0x6a, 0x04, 0x6b, 0x0a, 0x6c, 0x04, 0x7d, 0xf4, 0x6e,
        0x0f, 0x00, 0xe0, 0x23, 0x47, 0x23, 0x6b, 0xfd, 0x15, 0x12, 0x6f, 0xf7,
        0x07, 0x37, 0x00, 0x12, 0x6f, 0xfd, 0x15, 0x23, 0x47, 0x8b, 0xa4, 0x3b,
        0x12, 0x13, 0x1b, 0x7c, 0x02, 0x6a, 0xfc, 0x3b, 0x02, 0x13, 0x23, 0x7c,
        0x02, 0x6a, 0x04, 0x23, 0x47, 0x3c, 0x18, 0x12, 0x6f, 0x00, 0xe0, 0xa4,
        0xd3, 0x60, 0x14, 0x61, 0x08, 0x62, 0x0f, 0xd0, 0x1f, 0x70, 0x08, 0xf2,
        0x1e, 0x30, 0x2c, 0x13, 0x33, 0xf0, 0x0a, 0x00, 0xe0, 0xa6, 0xf4, 0xfe,
        0x65, 0x12, 0x25, 0xa3, 0xb7, 0xf9, 0x1e, 0x61, 0x08, 0x23, 0x5f, 0x81,
        0x06, 0x23, 0x5f, 0x81, 0x06, 0x23, 0x5f, 0x81, 0x06, 0x23, 0x5f, 0x7b,
        0xd0, 0x00, 0xee, 0x80, 0xe0, 0x80, 0x12, 0x30, 0x00, 0xdb, 0xc6, 0x7b,
        0x0c, 0x00, 0xee, 0xa3, 0xcf, 0x60, 0x1c, 0xd8, 0x04, 0x00, 0xee, 0x23,
        0x47, 0x8e, 0x23, 0x23, 0x47, 0x60, 0x05, 0xf0, 0x18, 0xf0, 0x15, 0xf0,
        0x07, 0x30, 0x00, 0x13, 0x7f, 0x00, 0xee, 0x6a, 0x00, 0x8d, 0xe0, 0x6b,
        0x04, 0xe9, 0xa1, 0x12, 0x57, 0xa6, 0x02, 0xfd, 0x1e, 0xf0, 0x65, 0x30,
        0xff, 0x13, 0xa5, 0x6a, 0x00, 0x6b, 0x04, 0x6d, 0x01, 0x6e, 0x01, 0x13,
        0x8d, 0xa5, 0x00, 0xf0, 0x1e, 0xdb, 0xc6, 0x7b, 0x08, 0x7d, 0x01, 0x7a,
        0x01, 0x3a, 0x07, 0x13, 0x8d, 0x00, 0xee, 0x3c, 0x7e, 0xff, 0xff, 0x99,
        0x99, 0x7e, 0xff, 0xff, 0x24, 0x24, 0xe7, 0x7e, 0xff, 0x3c, 0x3c, 0x7e,
        0xdb, 0x81, 0x42, 0x3c, 0x7e, 0xff, 0xdb, 0x10, 0x38, 0x7c, 0xfe, 0x00,
        0x00, 0x7f, 0x00, 0x3f, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
        0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x3f, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x3f, 0x08, 0x08, 0xff, 0x00, 0x00, 0xfe, 0x00, 0xfc,
        0x00, 0xfe, 0x00, 0x00, 0x00, 0x7e, 0x42, 0x42, 0x62, 0x62, 0x62, 0x62,
        0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0xff, 0x00, 0x7d, 0x00, 0x41, 0x7d, 0x05, 0x7d, 0x7d, 0x00,
        0x00, 0xc2, 0xc2, 0xc6, 0x44, 0x6c, 0x28, 0x38, 0x00, 0x00, 0xff, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00,
        0xf7, 0x10, 0x14, 0xf7, 0xf7, 0x04, 0x04, 0x00, 0x00, 0x7c, 0x44, 0xfe,
        0xc2, 0xc2, 0xc2, 0xc2, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0xef, 0x20, 0x28, 0xe8,
        0xe8, 0x2f, 0x2f, 0x00, 0x00, 0xf9, 0x85, 0xc5, 0xc5, 0xc5, 0xc5, 0xf9,
        0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
        0x00, 0x00, 0xff, 0x00, 0xbe, 0x00, 0x20, 0x30, 0x20, 0xbe, 0xbe, 0x00,
        0x00, 0xf7, 0x04, 0xe7, 0x85, 0x85, 0x84, 0xf4, 0x00, 0x00, 0xff, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00,
        0x00, 0x7f, 0x00, 0x3f, 0x00, 0x7f, 0x00, 0x00, 0x00, 0xef, 0x28, 0xef,
        0x00, 0xe0, 0x60, 0x6f, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xfe, 0x00, 0xfc,
        0x00, 0xfe, 0x00, 0x00, 0x00, 0xc0, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
        0x00, 0x00, 0xfc, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0xfc,
        0x10, 0x10, 0xff, 0xf9, 0x81, 0xb9, 0x8b, 0x9a, 0x9a, 0xfa, 0x00, 0xfa,
        0x8a, 0x9a, 0x9a, 0x9b, 0x99, 0xf8, 0xe6, 0x25, 0x25, 0xf4, 0x34, 0x34,
        0x34, 0x00, 0x17, 0x14, 0x34, 0x37, 0x36, 0x26, 0xc7, 0xdf, 0x50, 0x50,
        0x5c, 0xd8, 0xd8, 0xdf, 0x00, 0xdf, 0x11, 0x1f, 0x12, 0x1b, 0x19, 0xd9,
        0x7c, 0x44, 0xfe, 0x86, 0x86, 0x86, 0xfc, 0x84, 0xfe, 0x82, 0x82, 0xfe,
        0xfe, 0x80, 0xc0, 0xc0, 0xc0, 0xfe, 0xfc, 0x82, 0xc2, 0xc2, 0xc2, 0xfc,
        0xfe, 0x80, 0xf8, 0xc0, 0xc0, 0xfe, 0xfe, 0x80, 0xf0, 0xc0, 0xc0, 0xc0,
        0xfe, 0x80, 0xbe, 0x86, 0x86, 0xfe, 0x86, 0x86, 0xfe, 0x86, 0x86, 0x86,
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x18, 0x18, 0x18, 0x48, 0x48, 0x78,
        0x9c, 0x90, 0xb0, 0xc0, 0xb0, 0x9c, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xfe,
        0xee, 0x92, 0x92, 0x86, 0x86, 0x86, 0xfe, 0x82, 0x86, 0x86, 0x86, 0x86,
        0x7c, 0x82, 0x86, 0x86, 0x86, 0x7c, 0xfe, 0x82, 0xfe, 0xc0, 0xc0, 0xc0,
        0x7c, 0x82, 0xc2, 0xca, 0xc4, 0x7a, 0xfe, 0x86, 0xfe, 0x90, 0x9c, 0x84,
        0xfe, 0xc0, 0xfe, 0x02, 0x02, 0xfe, 0xfe, 0x10, 0x30, 0x30, 0x30, 0x30,
        0x82, 0x82, 0xc2, 0xc2, 0xc2, 0xfe, 0x82, 0x82, 0x82, 0xee, 0x38, 0x10,
        0x86, 0x86, 0x96, 0x92, 0x92, 0xee, 0x82, 0x44, 0x38, 0x38, 0x44, 0x82,
        0x82, 0x82, 0xfe, 0x30, 0x30, 0x30, 0xfe, 0x02, 0x1e, 0xf0, 0x80, 0xfe,
        0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00, 0x00, 0x60, 0x60, 0xc0,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18,
        0x7c, 0xc6, 0x0c, 0x18, 0x00, 0x18, 0x00, 0x00, 0xfe, 0xfe, 0x00, 0x00,
        0xfe, 0x82, 0x86, 0x86, 0x86, 0xfe, 0x08, 0x08, 0x08, 0x18, 0x18, 0x18,
        0xfe, 0x02, 0xfe, 0xc0, 0xc0, 0xfe, 0xfe, 0x02, 0x1e, 0x06, 0x06, 0xfe,
        0x84, 0xc4, 0xc4, 0xfe, 0x04, 0x04, 0xfe, 0x80, 0xfe, 0x06, 0x06, 0xfe,
        0xc0, 0xc0, 0xc0, 0xfe, 0x82, 0xfe, 0xfe, 0x02, 0x02, 0x06, 0x06, 0x06,
        0x7c, 0x44, 0xfe, 0x86, 0x86, 0xfe, 0xfe, 0x82, 0xfe, 0x06, 0x06, 0x06,
        0x44, 0xfe, 0x44, 0x44, 0xfe, 0x44, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8,
        0xa8, 0x6c, 0x5a, 0x00, 0x0c, 0x18, 0xa8, 0x30, 0x4e, 0x7e, 0x00, 0x12,
        0x18, 0x66, 0x6c, 0xa8, 0x5a, 0x66, 0x54, 0x24, 0x66, 0x00, 0x48, 0x48,
        0x18, 0x12, 0xa8, 0x06, 0x90, 0xa8, 0x12, 0x00, 0x7e, 0x30, 0x12, 0xa8,
        0x84, 0x30, 0x4e, 0x72, 0x18, 0x66, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8,
        0x90, 0x54, 0x78, 0xa8, 0x48, 0x78, 0x6c, 0x72, 0xa8, 0x12, 0x18, 0x6c,
        0x72, 0x66, 0x54, 0x90, 0xa8, 0x72, 0x2a, 0x18, 0xa8, 0x30, 0x4e, 0x7e,
        0x00, 0x12, 0x18, 0x66, 0x6c, 0xa8, 0x72, 0x54, 0xa8, 0x5a, 0x66, 0x18,
        0x7e, 0x18, 0x4e, 0x72, 0xa8, 0x72, 0x2a, 0x18, 0x30, 0x66, 0xa8, 0x30,
        0x4e, 0x7e, 0x00, 0x6c, 0x30, 0x54, 0x4e, 0x9c, 0xa8, 0xa8, 0xa8, 0xa8,
        0xa8, 0xa8, 0xa8, 0x48, 0x54, 0x7e, 0x18, 0xa8, 0x90, 0x54, 0x78, 0x66,
        0xa8, 0x6c, 0x2a, 0x30, 0x5a, 0xa8, 0x84, 0x30, 0x72, 0x2a, 0xa8, 0xd8,
        0xa8, 0x00, 0x4e, 0x12, 0xa8, 0xe4, 0xa2, 0xa8, 0x00, 0x4e, 0x12, 0xa8,
        0x6c, 0x2a, 0x54, 0x54, 0x72, 0xa8, 0x84, 0x30, 0x72, 0x2a, 0xa8, 0xde,
        0x9c, 0xa8, 0x72, 0x2a, 0x18, 0xa8, 0x0c, 0x54, 0x48, 0x5a, 0x78, 0x72,
        0x18, 0x66, 0xa8, 0x72, 0x18, 0x42, 0x42, 0x6c, 0xa8, 0x72, 0x2a, 0x00,
        0x72, 0xa8, 0x72, 0x2a, 0x18, 0xa8, 0x30, 0x4e, 0x7e, 0x00, 0x12, 0x18,
        0x66, 0x6c, 0xa8, 0x30, 0x4e, 0x0c, 0x66, 0x18, 0x00, 0x6c, 0x18, 0xa8,
        0x72, 0x2a, 0x18, 0x30, 0x66, 0xa8, 0x1e, 0x54, 0x66, 0x0c, 0x18, 0x9c,
        0xa8, 0x24, 0x54, 0x54, 0x12, 0xa8, 0x42, 0x78, 0x0c, 0x3c, 0xa8, 0xae,
        0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xff, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    unsigned char *iv, *iv_end;
    size_t invaders_rom_size = sizeof(invaders_rom) / sizeof(invaders_rom[0]);
    FILE *invaders;

    CUTE_ASSERT(cp8_romld(NULL, NULL) == 0);

    memset(msg, 0, sizeof(msg));
    CUTE_ASSERT(cp8_romld(NULL, msg) == 0);
    CUTE_ASSERT(msg[0] != 0);

    CUTE_ASSERT(cp8_romld("invaders.rom", NULL) == 0);

    memset(msg, 0, sizeof(msg));
    CUTE_ASSERT(cp8_romld("invaders.rom", msg) == 0);
    CUTE_ASSERT(msg[0] != 0);

    invaders = fopen("invaders.rom", "wb");
    CUTE_ASSERT(invaders != NULL);
    CUTE_ASSERT(fwrite(invaders_rom, 1, invaders_rom_size, invaders) == invaders_rom_size);
    fclose(invaders);

    CUTE_ASSERT(cp8_romld("invaders.rom", NULL) == 1);

    iv = &invaders_rom[0];
    iv_end = iv + invaders_rom_size;

    while (iv < iv_end) {
        CUTE_ASSERT(cp8_memget(CP8_TEXT_START + (invaders_rom_size - (iv_end - iv))) == *iv);
        iv++;
    }

    cp8_memzero();
    memset(msg, 0, sizeof(msg));
    CUTE_ASSERT(cp8_romld("invaders.rom", msg) == 1);
    CUTE_ASSERT(msg[0] != 0);

    iv = &invaders_rom[0];

    while (iv < iv_end) {
        CUTE_ASSERT(cp8_memget(CP8_TEXT_START + (invaders_rom_size - (iv_end - iv))) == *iv);
        iv++;
    }

    remove("invaders.rom");
CUTE_TEST_CASE_END

CUTE_TEST_CASE_SUITE(cp8_rom_suite)
    CUTE_RUN_TEST(cp8_romld_tests);
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
    CUTE_ASSERT(cp8_cpu_exec(0x00e0, &cp8) == 2);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_RET_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8_push(&cp8, 0xc0de);
    CUTE_ASSERT(cp8_cpu_exec(0x00ee, &cp8) == 0xc0e0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_JP_addr_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x121e, &cp8) == (0x021e - CP8_TEXT_START));
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_CALL_addr_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x2ca1, &cp8) == (0x0ca1 - CP8_TEXT_START));
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SE_Vx_byte_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8.v[0] = 6;
    CUTE_ASSERT(cp8_cpu_exec(0x3009, &cp8) == 2);
    cp8.pc = 0;
    cp8.v[0] = 9;
    CUTE_ASSERT(cp8_cpu_exec(0x3009, &cp8) == 4);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SNE_Vx_byte_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8.v[0] = 9;
    CUTE_ASSERT(cp8_cpu_exec(0x4009, &cp8) == 2);
    cp8.pc = 0;
    cp8.v[0] = 6;
    CUTE_ASSERT(cp8_cpu_exec(0x4009, &cp8) == 4);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SE_Vx_Vy_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8.v[1] = 0xff;
    cp8.v[4] = 0xf1;
    CUTE_ASSERT(cp8_cpu_exec(0x5140, &cp8) == 2);
    cp8.v[4] = 0xff;
    cp8.pc = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x5140, &cp8) == 4);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_LD_Vx_byte_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8.v[0] = 0;
    cp8.pc = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x60ba, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0] == 0xba);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_ADD_Vx_byte_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8.v[9] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x7902, &cp8) == 2);
    CUTE_ASSERT(cp8.v[9] == 3);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_LD_Vx_Vy_tests)
    struct cp8_ctx cp8;
    cp8.pc = 0;
    cp8.v[0xb] = 255;
    cp8.v[0xd] = 2;
    CUTE_ASSERT(cp8_cpu_exec(0x8bd0, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0xb] == 2);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_OR_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x0] = 0;
    cp8.v[0xf] = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x80f1, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 0);

    cp8.pc = 0;
    cp8.v[0x0] = 1;
    cp8.v[0xf] = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x80f1, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 1);

    cp8.pc = 0;
    cp8.v[0x0] = 0;
    cp8.v[0xf] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x80f1, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 1);

    cp8.pc = 0;
    cp8.v[0x0] = 1;
    cp8.v[0xf] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x80f1, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_AND_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x0] = 0;
    cp8.v[0xf] = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x80f2, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 0);

    cp8.pc = 0;
    cp8.v[0x0] = 1;
    cp8.v[0xf] = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x80f2, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 0);

    cp8.pc = 0;
    cp8.v[0x0] = 0;
    cp8.v[0xf] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x80f2, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 0);

    cp8.pc = 0;
    cp8.v[0x0] = 1;
    cp8.v[0xf] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x80f2, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_XOR_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x0] = 0;
    cp8.v[0xf] = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x80f3, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 0);

    cp8.pc = 0;
    cp8.v[0x0] = 1;
    cp8.v[0xf] = 0;
    CUTE_ASSERT(cp8_cpu_exec(0x80f3, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 1);

    cp8.pc = 0;
    cp8.v[0x0] = 0;
    cp8.v[0xf] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x80f3, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 1);

    cp8.pc = 0;
    cp8.v[0x0] = 1;
    cp8.v[0xf] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x80f3, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_ADD_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0xe] = 254;
    cp8.v[0xc] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x8ec4, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0xe] == 255);
    CUTE_ASSERT(cp8.v[0xf] == 0);

    cp8.pc = 0;
    cp8.v[0xe] = 255;
    cp8.v[0xc] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x8ec4, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0xe] == 0);
    CUTE_ASSERT(cp8.v[0xf] == 1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SUB_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0xe] = 254;
    cp8.v[0xc] = 1;
    CUTE_ASSERT(cp8_cpu_exec(0x8ec5, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0xe] == 253);
    CUTE_ASSERT(cp8.v[0xf] == 1);

    cp8.pc = 0;
    cp8.v[0xe] = 1;
    cp8.v[0xc] = 254;
    CUTE_ASSERT(cp8_cpu_exec(0x8ec5, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0xe] == 3);
    CUTE_ASSERT(cp8.v[0xf] == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SHR_Vx_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x0] = 0xe0;
    CUTE_ASSERT(cp8_cpu_exec(0x8006, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 0x70);
    CUTE_ASSERT(cp8.v[0xf] == 0);

    cp8.pc = 0;
    cp8.v[0x0] = 0xe1;
    CUTE_ASSERT(cp8_cpu_exec(0x8006, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 0x70);
    CUTE_ASSERT(cp8.v[0xf] == 1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SUBN_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x0] = 0x2;
    cp8.v[0x1] = 0x3;
    CUTE_ASSERT(cp8_cpu_exec(0x8017, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 1);

    cp8.pc = 0;
    cp8.v[0x0] = 0x2;
    cp8.v[0x1] = 0x1;
    CUTE_ASSERT(cp8_cpu_exec(0x8017, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 255);
    CUTE_ASSERT(cp8.v[0xf] == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SHL_Vx_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x0] = 0xe0;
    CUTE_ASSERT(cp8_cpu_exec(0x800e, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 0xc0);
    CUTE_ASSERT(cp8.v[0xf] == 1);

    cp8.pc = 0;
    cp8.v[0x0] = 0x20;
    CUTE_ASSERT(cp8_cpu_exec(0x800e, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 0x40);
    CUTE_ASSERT(cp8.v[0xf] == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_SNE_Vx_Vy_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0x3] = 0xa8;
    cp8.v[0x7] = 0xa8;
    CUTE_ASSERT(cp8_cpu_exec(0x9370, &cp8) == 2);
    cp8.v[0x7] = 0xa7;
    CUTE_ASSERT(cp8_cpu_exec(0x9370, &cp8) == 4);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_LD_I_addr_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.i = 0;
    CUTE_ASSERT(cp8_cpu_exec(0xab15, &cp8) == 2);
    CUTE_ASSERT(cp8.i == 0xb15);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_JP_V0_addr_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0] = 0xb1;
    CUTE_ASSERT(cp8_cpu_exec(0xbabc, &cp8) == (0xb6d - CP8_TEXT_START));
    CUTE_ASSERT(cp8.pc == (0xb6d - CP8_TEXT_START));
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_RND_Vx_byte_tests)
    // WARN(Rafael): Kind of stupid but here we go.
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0] = 255;
    CUTE_ASSERT(cp8_cpu_exec(0xc0ff, &cp8) == 2);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_LD_Vx_DT_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.dt = 0xf4;
    cp8.v[0x6] = 0x0;
    CUTE_ASSERT(cp8_cpu_exec(0xf607, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x6] == 0xf4);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_LD_DT_Vx_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.dt = 0;
    cp8.v[0x6] = 0xf4;
    CUTE_ASSERT(cp8_cpu_exec(0xf615, &cp8) == 2);
    CUTE_ASSERT(cp8.dt == 0xf4);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_LD_ST_Vx_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.st = 0;
    cp8.v[0x5] = 0xbb;
    CUTE_ASSERT(cp8_cpu_exec(0xf518, &cp8) == 2);
    CUTE_ASSERT(cp8.st == 0xbb);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_ADD_I_Vx_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.v[0xc] = 1;
    cp8.i = 6;
    CUTE_ASSERT(cp8_cpu_exec(0xfc1e, &cp8) == 2);
    CUTE_ASSERT(cp8.i == 7);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_LD_F_Vx_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.i = 0;
    cp8.v[0x1] = 0x99;
    CUTE_ASSERT(cp8_cpu_exec(0xf129, &cp8) == 2);
    CUTE_ASSERT(cp8.i == 0x2fd);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_LD_B_Vx_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.i = 0x200;
    cp8.v[0] = 0xfe;
    CUTE_ASSERT(cp8_cpu_exec(0xf033, &cp8) == 2);
    CUTE_ASSERT(cp8_memget(cp8.i) == 2);
    CUTE_ASSERT(cp8_memget(cp8.i + 1) == 5);
    CUTE_ASSERT(cp8_memget(cp8.i + 2) == 4);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_LD_I_Vx_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.i = 0x01a;
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
    CUTE_ASSERT(cp8_cpu_exec(0xff55, &cp8) == 2);
    CUTE_ASSERT(cp8_memget(cp8.i) == 0x0);
    CUTE_ASSERT(cp8_memget(cp8.i + 0x1) == 0x1);
    CUTE_ASSERT(cp8_memget(cp8.i + 0x2) == 0x2);
    CUTE_ASSERT(cp8_memget(cp8.i + 0x3) == 0x3);
    CUTE_ASSERT(cp8_memget(cp8.i + 0x4) == 0x4);
    CUTE_ASSERT(cp8_memget(cp8.i + 0x5) == 0x5);
    CUTE_ASSERT(cp8_memget(cp8.i + 0x6) == 0x6);
    CUTE_ASSERT(cp8_memget(cp8.i + 0x7) == 0x7);
    CUTE_ASSERT(cp8_memget(cp8.i + 0x8) == 0x8);
    CUTE_ASSERT(cp8_memget(cp8.i + 0x9) == 0x9);
    CUTE_ASSERT(cp8_memget(cp8.i + 0xa) == 0xa);
    CUTE_ASSERT(cp8_memget(cp8.i + 0xb) == 0xb);
    CUTE_ASSERT(cp8_memget(cp8.i + 0xc) == 0xc);
    CUTE_ASSERT(cp8_memget(cp8.i + 0xd) == 0xd);
    CUTE_ASSERT(cp8_memget(cp8.i + 0xe) == 0xe);
    CUTE_ASSERT(cp8_memget(cp8.i + 0xf) == 0xf);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_cpu_LD_Vx_I_tests)
    struct cp8_ctx cp8;

    cp8.pc = 0;
    cp8.i = 0x100;
    cp8_memset(cp8.i, 0xf);
    cp8_memset(cp8.i + 0x1, 0xe);
    cp8_memset(cp8.i + 0x2, 0xd);
    cp8_memset(cp8.i + 0x3, 0xc);
    cp8_memset(cp8.i + 0x4, 0xb);
    cp8_memset(cp8.i + 0x5, 0xa);
    cp8_memset(cp8.i + 0x6, 0x9);
    cp8_memset(cp8.i + 0x7, 0x8);
    cp8_memset(cp8.i + 0x8, 0x7);
    cp8_memset(cp8.i + 0x9, 0x6);
    cp8_memset(cp8.i + 0xa, 0x5);
    cp8_memset(cp8.i + 0xb, 0x4);
    cp8_memset(cp8.i + 0xc, 0x3);
    cp8_memset(cp8.i + 0xd, 0x2);
    cp8_memset(cp8.i + 0xe, 0x1);
    cp8_memset(cp8.i + 0xf, 0x0);
    CUTE_ASSERT(cp8_cpu_exec(0xff65, &cp8) == 2);
    CUTE_ASSERT(cp8.v[0x0] == 0xf);
    CUTE_ASSERT(cp8.v[0x1] == 0xe);
    CUTE_ASSERT(cp8.v[0x2] == 0xd);
    CUTE_ASSERT(cp8.v[0x3] == 0xc);
    CUTE_ASSERT(cp8.v[0x4] == 0xb);
    CUTE_ASSERT(cp8.v[0x5] == 0xa);
    CUTE_ASSERT(cp8.v[0x6] == 0x9);
    CUTE_ASSERT(cp8.v[0x7] == 0x8);
    CUTE_ASSERT(cp8.v[0x8] == 0x7);
    CUTE_ASSERT(cp8.v[0x9] == 0x6);
    CUTE_ASSERT(cp8.v[0xa] == 0x5);
    CUTE_ASSERT(cp8.v[0xb] == 0x4);
    CUTE_ASSERT(cp8.v[0xc] == 0x3);
    CUTE_ASSERT(cp8.v[0xd] == 0x2);
    CUTE_ASSERT(cp8.v[0xe] == 0x1);
    CUTE_ASSERT(cp8.v[0xf] == 0x0);
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
    CUTE_RUN_TEST(cp8_cpu_SNE_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_cpu_LD_I_addr_tests);
    CUTE_RUN_TEST(cp8_cpu_JP_V0_addr_tests);
    CUTE_RUN_TEST(cp8_cpu_RND_Vx_byte_tests);
    CUTE_RUN_TEST(cp8_cpu_LD_Vx_DT_tests);
    CUTE_RUN_TEST(cp8_cpu_LD_DT_Vx_tests);
    CUTE_RUN_TEST(cp8_cpu_LD_ST_Vx_tests);
    CUTE_RUN_TEST(cp8_cpu_ADD_I_Vx_tests);
    CUTE_RUN_TEST(cp8_cpu_LD_F_Vx_tests);
    CUTE_RUN_TEST(cp8_cpu_LD_B_Vx_tests);
    CUTE_RUN_TEST(cp8_cpu_LD_I_Vx_tests);
    CUTE_RUN_TEST(cp8_cpu_LD_Vx_I_tests);
CUTE_TEST_CASE_SUITE_END

// --

// DSL

CUTE_TEST_CASE(cp8_dsl_suite)
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

// EMU/OPT

CUTE_TEST_CASE(cp8_emu_opt_tests)
    char *argv[] = { "--processor=cp8",
                     "--dummy-flag",
                     "test" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    CUTE_ASSERT(cp8_emu_option("processor", NULL) == NULL);
    CUTE_ASSERT(cp8_emu_bool_option("dummy-flag", 0) == 0);
    CUTE_ASSERT(cp8_emu_task_option("test") == 0);

    cp8_emu_set_argc_argv(argc, argv);

    CUTE_ASSERT(strcmp(cp8_emu_option("processor", NULL), "cp8") == 0);
    CUTE_ASSERT(cp8_emu_bool_option("dummy-flag", 0) == 1);
    CUTE_ASSERT(cp8_emu_option("dummy-flag", NULL) == NULL);
    CUTE_ASSERT(cp8_emu_option("test", NULL) == NULL);
    CUTE_ASSERT(cp8_emu_bool_option("test", 0) == 0);
    CUTE_ASSERT(cp8_emu_task_option("test") == 1);

    CUTE_ASSERT(cp8_emu_option("foo", NULL) == NULL);
    CUTE_ASSERT(cp8_emu_bool_option("bar", 1) == 1);
    CUTE_ASSERT(cp8_emu_task_option("foobar") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE_SUITE(cp8_emu_opt_suite)
    CUTE_RUN_TEST(cp8_emu_opt_tests);
CUTE_TEST_CASE_SUITE_END

// --

// ASM

CUTE_TEST_CASE(cp8_asm_umount_CLS_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x00e0), "CLS") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_RET_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x00ee), "RET") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_JP_addr_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x1012), "JP 0x012") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_CALL_addr_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x2f00), "CALL 0xf00") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_SE_Vx_kk_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3000), "SE V0, 0x00") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3101), "SE V1, 0x01") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3202), "SE V2, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3303), "SE V3, 0x03") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3404), "SE V4, 0x04") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3505), "SE V5, 0x05") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3606), "SE V6, 0x06") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3707), "SE V7, 0x07") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3808), "SE V8, 0x08") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3909), "SE V9, 0x09") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3a0a), "SE VA, 0x0a") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3b0b), "SE VB, 0x0b") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3c0c), "SE VC, 0x0c") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3d0d), "SE VD, 0x0d") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3e0e), "SE VE, 0x0e") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x3f0f), "SE VF, 0x0f") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_SNE_Vx_kk_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4000), "SNE V0, 0x00") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4101), "SNE V1, 0x01") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4202), "SNE V2, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4303), "SNE V3, 0x03") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4404), "SNE V4, 0x04") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4505), "SNE V5, 0x05") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4606), "SNE V6, 0x06") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4707), "SNE V7, 0x07") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4808), "SNE V8, 0x08") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4909), "SNE V9, 0x09") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4a0a), "SNE VA, 0x0a") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4b0b), "SNE VB, 0x0b") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4c0c), "SNE VC, 0x0c") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4d0d), "SNE VD, 0x0d") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4e0e), "SNE VE, 0x0e") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x4f0f), "SNE VF, 0x0f") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_SE_Vx_Vy_tests)
    char *Vx[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vx_nr = sizeof(Vx) / sizeof(Vx[0]), x;
    char *Vy[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vy_nr = sizeof(Vy) / sizeof(Vy[0]), y;
    char expected_mne[255];

    for (x = 0; x < Vx_nr; x++) {
        for (y = 0; y < Vy_nr; y++) {
            sprintf(expected_mne, "SE %s, %s", Vx[x], Vy[y]);
            CUTE_ASSERT(strcmp(cp8_asm_umount((unsigned short) 0x5 << 12 | (unsigned short) x << 8 | (unsigned short) y << 4), expected_mne) == 0);
        }
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_LD_Vx_kk_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x60ff),"LD V0, 0xff") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x61ee),"LD V1, 0xee") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x62dd),"LD V2, 0xdd") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x63cc),"LD V3, 0xcc") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x64bb),"LD V4, 0xbb") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x65aa),"LD V5, 0xaa") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x6699),"LD V6, 0x99") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x6788),"LD V7, 0x88") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x6877),"LD V8, 0x77") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x6966),"LD V9, 0x66") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x6a55),"LD VA, 0x55") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x6b44),"LD VB, 0x44") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x6c33),"LD VC, 0x33") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x6d22),"LD VD, 0x22") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x6e11),"LD VE, 0x11") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x6f00),"LD VF, 0x00") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_ADD_Vx_kk_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x70ff),"ADD V0, 0xff") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x71ee),"ADD V1, 0xee") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x72dd),"ADD V2, 0xdd") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x73cc),"ADD V3, 0xcc") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x74bb),"ADD V4, 0xbb") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x75aa),"ADD V5, 0xaa") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x7699),"ADD V6, 0x99") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x7788),"ADD V7, 0x88") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x7877),"ADD V8, 0x77") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x7966),"ADD V9, 0x66") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x7a55),"ADD VA, 0x55") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x7b44),"ADD VB, 0x44") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x7c33),"ADD VC, 0x33") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x7d22),"ADD VD, 0x22") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x7e11),"ADD VE, 0x11") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x7f00),"ADD VF, 0x00") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_LD_Vx_Vy_tests)
    char *Vx[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vx_nr = sizeof(Vx) / sizeof(Vx[0]), x;
    char *Vy[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vy_nr = sizeof(Vy) / sizeof(Vy[0]), y;
    char expected_mne[255];

    for (x = 0; x < Vx_nr; x++) {
        for (y = 0; y < Vy_nr; y++) {
            sprintf(expected_mne, "LD %s, %s", Vx[x], Vy[y]);
            CUTE_ASSERT(strcmp(cp8_asm_umount((unsigned short) 0x8 << 12 | (unsigned short) x << 8 | (unsigned short) y << 4), expected_mne) == 0);
        }
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_OR_Vx_Vy_tests)
    char *Vx[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vx_nr = sizeof(Vx) / sizeof(Vx[0]), x;
    char *Vy[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vy_nr = sizeof(Vy) / sizeof(Vy[0]), y;
    char expected_mne[255];

    for (x = 0; x < Vx_nr; x++) {
        for (y = 0; y < Vy_nr; y++) {
            sprintf(expected_mne, "OR %s, %s", Vx[x], Vy[y]);
            CUTE_ASSERT(strcmp(cp8_asm_umount((unsigned short) 0x8 << 12 | (unsigned short) x << 8 | (unsigned short) y << 4 | 0x1), expected_mne) == 0);
        }
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_AND_Vx_Vy_tests)
    char *Vx[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vx_nr = sizeof(Vx) / sizeof(Vx[0]), x;
    char *Vy[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vy_nr = sizeof(Vy) / sizeof(Vy[0]), y;
    char expected_mne[255];

    for (x = 0; x < Vx_nr; x++) {
        for (y = 0; y < Vy_nr; y++) {
            sprintf(expected_mne, "AND %s, %s", Vx[x], Vy[y]);
            CUTE_ASSERT(strcmp(cp8_asm_umount((unsigned short) 0x8 << 12 | (unsigned short) x << 8 | (unsigned short) y << 4 | 0x2), expected_mne) == 0);
        }
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_XOR_Vx_Vy_tests)
    char *Vx[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vx_nr = sizeof(Vx) / sizeof(Vx[0]), x;
    char *Vy[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vy_nr = sizeof(Vy) / sizeof(Vy[0]), y;
    char expected_mne[255];

    for (x = 0; x < Vx_nr; x++) {
        for (y = 0; y < Vy_nr; y++) {
            sprintf(expected_mne, "XOR %s, %s", Vx[x], Vy[y]);
            CUTE_ASSERT(strcmp(cp8_asm_umount((unsigned short) 0x8 << 12 | (unsigned short) x << 8 | (unsigned short) y << 4 | 0x3), expected_mne) == 0);
        }
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_ADD_Vx_Vy_tests)
    char *Vx[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vx_nr = sizeof(Vx) / sizeof(Vx[0]), x;
    char *Vy[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vy_nr = sizeof(Vy) / sizeof(Vy[0]), y;
    char expected_mne[255];

    for (x = 0; x < Vx_nr; x++) {
        for (y = 0; y < Vy_nr; y++) {
            sprintf(expected_mne, "ADD %s, %s", Vx[x], Vy[y]);
            CUTE_ASSERT(strcmp(cp8_asm_umount((unsigned short) 0x8 << 12 | (unsigned short) x << 8 | (unsigned short) y << 4 | 0x4), expected_mne) == 0);
        }
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_SUB_Vx_Vy_tests)
    char *Vx[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vx_nr = sizeof(Vx) / sizeof(Vx[0]), x;
    char *Vy[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vy_nr = sizeof(Vy) / sizeof(Vy[0]), y;
    char expected_mne[255];

    for (x = 0; x < Vx_nr; x++) {
        for (y = 0; y < Vy_nr; y++) {
            sprintf(expected_mne, "SUB %s, %s", Vx[x], Vy[y]);
            CUTE_ASSERT(strcmp(cp8_asm_umount((unsigned short) 0x8 << 12 | (unsigned short) x << 8 | (unsigned short) y << 4 | 0x5), expected_mne) == 0);
        }
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_SHR_Vx_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8006),"SHR V0") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x81c6),"SHR V1") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8246),"SHR V2") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8326),"SHR V3") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8496),"SHR V4") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8546),"SHR V5") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8626),"SHR V6") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8716),"SHR V7") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8826),"SHR V8") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8976),"SHR V9") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8ad6),"SHR VA") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8bb6),"SHR VB") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8ce6),"SHR VC") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8da6),"SHR VD") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8e06),"SHR VE") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8f16),"SHR VF") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_SUBN_Vx_Vy_tests)
    char *Vx[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vx_nr = sizeof(Vx) / sizeof(Vx[0]), x;
    char *Vy[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vy_nr = sizeof(Vy) / sizeof(Vy[0]), y;
    char expected_mne[255];

    for (x = 0; x < Vx_nr; x++) {
        for (y = 0; y < Vy_nr; y++) {
            sprintf(expected_mne, "SUBN %s, %s", Vx[x], Vy[y]);
            CUTE_ASSERT(strcmp(cp8_asm_umount((unsigned short) 0x8 << 12 | (unsigned short) x << 8 | (unsigned short) y << 4 | 0x7), expected_mne) == 0);
        }
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_SHL_Vx_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x801e),"SHL V0") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x812e),"SHL V1") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x82ee),"SHL V2") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x83ae),"SHL V3") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x84ce),"SHL V4") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x85ce),"SHL V5") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x86fe),"SHL V6") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x873e),"SHL V7") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x889e),"SHL V8") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x894e),"SHL V9") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8a3e),"SHL VA") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8b1e),"SHL VB") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8c0e),"SHL VC") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8d6e),"SHL VD") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8ebe),"SHL VE") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0x8f8e),"SHL VF") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_SNE_Vx_Vy_tests)
    char *Vx[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vx_nr = sizeof(Vx) / sizeof(Vx[0]), x;
    char *Vy[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vy_nr = sizeof(Vy) / sizeof(Vy[0]), y;
    char expected_mne[255];

    for (x = 0; x < Vx_nr; x++) {
        for (y = 0; y < Vy_nr; y++) {
            sprintf(expected_mne, "SNE %s, %s", Vx[x], Vy[y]);
            CUTE_ASSERT(strcmp(cp8_asm_umount((unsigned short) 0x9 << 12 | (unsigned short) x << 8 | (unsigned short) y << 4), expected_mne) == 0);
        }
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_LD_I_nnn_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xab00), "LD I, 0xb00") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_JP_V0_nnn_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xb101), "JP V0, 0x101") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_RND_Vx_kk_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xc002), "RND V0, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xc102), "RND V1, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xc202), "RND V2, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xc302), "RND V3, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xc402), "RND V4, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xc502), "RND V5, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xc602), "RND V6, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xc702), "RND V7, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xc802), "RND V8, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xc902), "RND V9, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xca02), "RND VA, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xcb02), "RND VB, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xcc02), "RND VC, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xcd02), "RND VD, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xce02), "RND VE, 0x02") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xcf02), "RND VF, 0x02") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_DRW_Vx_Vy_n_tests)
    char *Vx[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vx_nr = sizeof(Vx) / sizeof(Vx[0]), x;
    char *Vy[] = { "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE", "VF"};
    size_t Vy_nr = sizeof(Vy) / sizeof(Vy[0]), y;
    char expected_mne[255];

    for (x = 0; x < Vx_nr; x++) {
        for (y = 0; y < Vy_nr; y++) {
            sprintf(expected_mne, "DRW %s, %s, 0x%.1x", Vx[x], Vy[y], y);
            CUTE_ASSERT(strcmp(cp8_asm_umount((unsigned short) 0xd << 12 | (unsigned short) x << 8 | (unsigned short) y << 4 | y), expected_mne) == 0);
        }
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_SKP_Vx_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe09e),"SKP V0") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe19e),"SKP V1") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe29e),"SKP V2") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe39e),"SKP V3") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe49e),"SKP V4") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe59e),"SKP V5") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe69e),"SKP V6") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe79e),"SKP V7") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe89e),"SKP V8") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe99e),"SKP V9") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xea9e),"SKP VA") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xeb9e),"SKP VB") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xec9e),"SKP VC") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xed9e),"SKP VD") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xee9e),"SKP VE") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xef9e),"SKP VF") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_SKPN_Vx_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe0a1),"SKPN V0") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe1a1),"SKPN V1") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe2a1),"SKPN V2") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe3a1),"SKPN V3") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe4a1),"SKPN V4") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe5a1),"SKPN V5") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe6a1),"SKPN V6") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe7a1),"SKPN V7") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe8a1),"SKPN V8") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xe9a1),"SKPN V9") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xeaa1),"SKPN VA") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xeba1),"SKPN VB") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xeca1),"SKPN VC") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xeda1),"SKPN VD") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xeea1),"SKPN VE") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xefa1),"SKPN VF") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_LD_Vx_DT_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf007),"LD V0, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf107),"LD V1, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf207),"LD V2, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf307),"LD V3, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf407),"LD V4, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf507),"LD V5, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf607),"LD V6, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf707),"LD V7, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf807),"LD V8, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf907),"LD V9, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfa07),"LD VA, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfb07),"LD VB, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfc07),"LD VC, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfd07),"LD VD, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfe07),"LD VE, DT") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xff07),"LD VF, DT") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_LD_Vx_K_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf00a),"LD V0, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf10a),"LD V1, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf20a),"LD V2, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf30a),"LD V3, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf40a),"LD V4, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf50a),"LD V5, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf60a),"LD V6, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf70a),"LD V7, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf80a),"LD V8, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf90a),"LD V9, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfa0a),"LD VA, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfb0a),"LD VB, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfc0a),"LD VC, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfd0a),"LD VD, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfe0a),"LD VE, K") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xff0a),"LD VF, K") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_LD_DT_Vx_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf015),"LD DT, V0") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf115),"LD DT, V1") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf215),"LD DT, V2") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf315),"LD DT, V3") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf415),"LD DT, V4") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf515),"LD DT, V5") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf615),"LD DT, V6") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf715),"LD DT, V7") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf815),"LD DT, V8") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf915),"LD DT, V9") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfa15),"LD DT, VA") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfb15),"LD DT, VB") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfc15),"LD DT, VC") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfd15),"LD DT, VD") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfe15),"LD DT, VE") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xff15),"LD DT, VF") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_LD_ST_Vx_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf018),"LD ST, V0") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf118),"LD ST, V1") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf218),"LD ST, V2") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf318),"LD ST, V3") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf418),"LD ST, V4") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf518),"LD ST, V5") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf618),"LD ST, V6") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf718),"LD ST, V7") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf818),"LD ST, V8") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf918),"LD ST, V9") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfa18),"LD ST, VA") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfb18),"LD ST, VB") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfc18),"LD ST, VC") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfd18),"LD ST, VD") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfe18),"LD ST, VE") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xff18),"LD ST, VF") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_ADD_I_Vx_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf01e),"ADD I, V0") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf11e),"ADD I, V1") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf21e),"ADD I, V2") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf31e),"ADD I, V3") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf41e),"ADD I, V4") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf51e),"ADD I, V5") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf61e),"ADD I, V6") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf71e),"ADD I, V7") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf81e),"ADD I, V8") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf91e),"ADD I, V9") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfa1e),"ADD I, VA") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfb1e),"ADD I, VB") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfc1e),"ADD I, VC") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfd1e),"ADD I, VD") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfe1e),"ADD I, VE") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xff1e),"ADD I, VF") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_LD_F_Vx_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf029),"LD F, V0") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf129),"LD F, V1") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf229),"LD F, V2") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf329),"LD F, V3") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf429),"LD F, V4") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf529),"LD F, V5") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf629),"LD F, V6") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf729),"LD F, V7") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf829),"LD F, V8") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf929),"LD F, V9") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfa29),"LD F, VA") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfb29),"LD F, VB") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfc29),"LD F, VC") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfd29),"LD F, VD") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfe29),"LD F, VE") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xff29),"LD F, VF") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_LD_B_Vx_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf033),"LD B, V0") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf133),"LD B, V1") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf233),"LD B, V2") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf333),"LD B, V3") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf433),"LD B, V4") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf533),"LD B, V5") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf633),"LD B, V6") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf733),"LD B, V7") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf833),"LD B, V8") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf933),"LD B, V9") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfa33),"LD B, VA") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfb33),"LD B, VB") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfc33),"LD B, VC") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfd33),"LD B, VD") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfe33),"LD B, VE") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xff33),"LD B, VF") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_LD_RI_Vx_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf055),"LD [I], V0") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf155),"LD [I], V1") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf255),"LD [I], V2") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf355),"LD [I], V3") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf455),"LD [I], V4") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf555),"LD [I], V5") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf655),"LD [I], V6") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf755),"LD [I], V7") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf855),"LD [I], V8") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf955),"LD [I], V9") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfa55),"LD [I], VA") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfb55),"LD [I], VB") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfc55),"LD [I], VC") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfd55),"LD [I], VD") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfe55),"LD [I], VE") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xff55),"LD [I], VF") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(cp8_asm_umount_LD_Vx_RI_tests)
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf065),"LD V0, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf165),"LD V1, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf265),"LD V2, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf365),"LD V3, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf465),"LD V4, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf565),"LD V5, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf665),"LD V6, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf765),"LD V7, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf865),"LD V8, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xf965),"LD V9, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfa65),"LD VA, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfb65),"LD VB, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfc65),"LD VC, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfd65),"LD VD, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xfe65),"LD VE, [I]") == 0);
    CUTE_ASSERT(strcmp(cp8_asm_umount(0xff65),"LD VF, [I]") == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE_SUITE(cp8_asm_suite)
    CUTE_RUN_TEST(cp8_asm_umount_CLS_tests);
    CUTE_RUN_TEST(cp8_asm_umount_RET_tests);
    CUTE_RUN_TEST(cp8_asm_umount_JP_addr_tests);
    CUTE_RUN_TEST(cp8_asm_umount_CALL_addr_tests);
    CUTE_RUN_TEST(cp8_asm_umount_SE_Vx_kk_tests);
    CUTE_RUN_TEST(cp8_asm_umount_SNE_Vx_kk_tests);
    CUTE_RUN_TEST(cp8_asm_umount_SE_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_asm_umount_LD_Vx_kk_tests);
    CUTE_RUN_TEST(cp8_asm_umount_ADD_Vx_kk_tests);
    CUTE_RUN_TEST(cp8_asm_umount_LD_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_asm_umount_OR_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_asm_umount_AND_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_asm_umount_XOR_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_asm_umount_ADD_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_asm_umount_SUB_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_asm_umount_SHR_Vx_tests);
    CUTE_RUN_TEST(cp8_asm_umount_SUBN_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_asm_umount_SHL_Vx_tests);
    CUTE_RUN_TEST(cp8_asm_umount_SNE_Vx_Vy_tests);
    CUTE_RUN_TEST(cp8_asm_umount_LD_I_nnn_tests);
    CUTE_RUN_TEST(cp8_asm_umount_JP_V0_nnn_tests);
    CUTE_RUN_TEST(cp8_asm_umount_RND_Vx_kk_tests);
    CUTE_RUN_TEST(cp8_asm_umount_DRW_Vx_Vy_n_tests);
    CUTE_RUN_TEST(cp8_asm_umount_SKP_Vx_tests);
    CUTE_RUN_TEST(cp8_asm_umount_SKPN_Vx_tests);
    CUTE_RUN_TEST(cp8_asm_umount_LD_Vx_DT_tests);
    CUTE_RUN_TEST(cp8_asm_umount_LD_Vx_K_tests);
    CUTE_RUN_TEST(cp8_asm_umount_LD_DT_Vx_tests);
    CUTE_RUN_TEST(cp8_asm_umount_LD_ST_Vx_tests);
    CUTE_RUN_TEST(cp8_asm_umount_ADD_I_Vx_tests);
    CUTE_RUN_TEST(cp8_asm_umount_LD_F_Vx_tests);
    CUTE_RUN_TEST(cp8_asm_umount_LD_B_Vx_tests);
    CUTE_RUN_TEST(cp8_asm_umount_LD_RI_Vx_tests);
    CUTE_RUN_TEST(cp8_asm_umount_LD_Vx_RI_tests);
CUTE_TEST_CASE_SUITE_END

// --

CUTE_TEST_CASE(cp8_tests)
    CUTE_RUN_TEST_SUITE(cp8_dsl_suite);
    CUTE_RUN_TEST_SUITE(cp8_ctx_suite);
    CUTE_RUN_TEST_SUITE(cp8_mem_suite);
    CUTE_RUN_TEST_SUITE(cp8_vid_suite);
    CUTE_RUN_TEST_SUITE(cp8_cpu_suite);
    CUTE_RUN_TEST_SUITE(cp8_rom_suite);
    CUTE_RUN_TEST_SUITE(cp8_asm_suite);
    CUTE_RUN_TEST_SUITE(cp8_emu_opt_suite);
CUTE_TEST_CASE_END

CUTE_MAIN(cp8_tests);
