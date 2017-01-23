/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <emu/tsk.h>
#include <emu/emu.h>
#include <emu/opt.h>
#include <mem/mem.h>
#include <cpu/cpu.h>
#include <kbd/kbd.h>
#include <tmr/tmr.h>
#include <rom/ld.h>
#include <ctx/types.h>
#include <accacia.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int cp8_emu_get_cycles(void);

int cp8_emu_tsk_emulate(void) {
    char *rom = cp8_emu_option("rom", NULL);
    char msg[255];
    unsigned short instr;
    struct cp8_ctx processor;
    unsigned char k = CP8_EMU_TSK_EMULATE_KQUIT >> 1;
    int cycles, cycles_nr;
#ifndef NO_PTHREAD_SUPPORT
    pthread_t processor_tmu;
    pthread_attr_t processor_tmu_attr;

    pthread_attr_init(&processor_tmu_attr);
#endif

    if (rom == NULL) {
        printf("ERROR: the option --rom is missing.\n");
        return 1;
    }

    if (cp8_romld(rom, msg) == 0) {
        printf("%s", msg);
        return 1;
    }

    cycles_nr = cp8_emu_get_cycles();

___let_the_good_times_roll:   // INFO(Rafael): When CPU was singular.... (...)

#ifndef NO_PTHREAD_SUPPORT
    pthread_create(&processor_tmu, &processor_tmu_attr, cp8_tmrtck, &processor);
#endif

    cp8_emu_init(&processor);

#define cp8_emu_next_instr(pc) ( ((unsigned short)(cp8_memget(CP8_TEXT_START + (pc) )) << 8 ) |\
                                  cp8_memget(CP8_TEXT_START + ((pc) + 1)) )

    instr = cp8_emu_next_instr(processor.pc);

    while ((k = toupper(cp8_kbdlkey())) != CP8_EMU_TSK_EMULATE_KQUIT) {

        // INFO(Rafael): To execute the instructions per cycles was the way that I found to reduce the flickering a little.
        //               However, a CHIP-8 game normally can flicker and still all here is done over an ANSI/TERM.

#ifndef NO_PTHREAD_SUPPORT
        //  TODO(Rafael): It should be improved on. It would be better if executed $cycles_delta$ instead of a silly constant,
        //                btw really true in just one moment. This crazy cycles stuff is done because put something like
        //                "sleep(60Hz)" in the middle of the emulation loop would be pretty stupid. At this point, we just
        //                have to slow down a little. However, the ST and DT should be decremented at ~60Hz.
        //
        //  DUH(Rafael):  Yes, $cycles_delta$ should be evaluated based on the real machine. It is about you execute now
        //                what should be executed in the past, thus we can compensate the "buzzing" introduced by the
        //                real machine in your crazy equation. Who knows... d:-S
        for (cycles = 0; cycles < cycles_nr; cycles++) {
            processor.pc = cp8_cpu_exec(instr, &processor);
            instr = cp8_emu_next_instr(processor.pc);
            usleep(5);
        }
#else
        // INFO(Rafael): Nasty trick to make some games more playable on a non multi-threaded environment.
        //               If you are under MINIX 3.3.1 or under, this code shi.. I mean trick is valuable
        //               for you. Few games should be emulated using this crap, the more static stuff should
        //               works.

        for (cycles = 0; cycles < cycles_nr; cycles++) {
            cp8_kbdread();
            processor.pc = cp8_cpu_exec(instr, &processor);
            cp8_kbdread();
            instr = cp8_emu_next_instr(processor.pc);
            cp8_kbdread();
            usleep(CP8_CLOCK_IN_MICORSECS);
        }

        cp8_kbdrelease();

        if (processor.dt > 0) {
            processor.dt--;
        }

        if (processor.st > 0) {
            processor.st--;
        }

#endif
        if (k == 'r' || k == 'R') {
            cp8_kbdsetkey(CP8_EMU_TSK_EMULATE_KQUIT);
            cp8_emu_finis();
            usleep(CP8_EMU_TSK_EMULATE_KQUIT);
            memset(&processor, 0, sizeof(processor));
            cp8_kbdsetkey('?');
            goto ___let_the_good_times_roll;
        }
    }

#undef cp8_emu_next_instr

    cp8_emu_finis();

    return 0;
}

static int cp8_emu_get_cycles(void) {
#define cp8_cycles_as_str(c) #c
#define cp8_default_cycles(c) cp8_cycles_as_str(c)
    char *cycles = cp8_emu_option("cycles", cp8_default_cycles(CP8_MAX_INSTRUCTIONS_PER_CYCLE));

    char *cp = cycles, *cp_end = cp + strlen(cycles);

    while (cp != cp_end) {
        if (!isdigit(*cp)) {
            cycles = cp8_default_cycles(CP8_MAX_INSTRUCTIONS_PER_CYCLE);
            break;
        }
        cp++;
    }

    return atoi(cycles);
#undef cp8_cycles_as_str
#undef cp8_default_cycles
}
