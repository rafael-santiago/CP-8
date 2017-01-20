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
#include <rom/ld.h>
#include <ctx/types.h>
#include <accacia.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define CP8_EMU_TSK_EMULATE_KQUIT 27

int cp8_emu_tsk_emulate(void) {
    char *rom = cp8_emu_option("rom", NULL);
    char msg[255];
    unsigned short instr;
    struct cp8_ctx processor;
    unsigned char k = CP8_EMU_TSK_EMULATE_KQUIT >> 1;
    int cycles;

    if (rom == NULL) {
        printf("ERROR: the option --rom is missing.\n");
        return 1;
    }

    if (cp8_romld(rom, msg) == 0) {
        printf("%s", msg);
        return 1;
    }

___let_the_good_times_roll:   // INFO(Rafael): When CPU was singular.... (...)

    cp8_emu_init(&processor);

#define cp8_emu_next_instr(pc) ( ((unsigned short)(cp8_memget(CP8_TEXT_START + (pc) )) << 8 ) |\
                                  cp8_memget(CP8_TEXT_START + ((pc) + 1)) )

    instr = cp8_emu_next_instr(processor.pc);

    while ((k = toupper(cp8_kbdlkey())) != CP8_EMU_TSK_EMULATE_KQUIT) {

        // INFO(Rafael): To execute the instructions per cycles was the way that I found to reduce the flickering a little.
        //               However, a CHIP-8 game normally can flicker and still all here is done over an ANSI/TERM.

#ifndef NO_PTHREAD_SUPPORT
        for (cycles = 0; cycles < CP8_MAX_INSTRUCTIONS_PER_CYCLE; cycles++) {
            processor.pc = cp8_cpu_exec(instr, &processor);
            instr = cp8_emu_next_instr(processor.pc);
            usleep(5);
        }
#else
        // INFO(Rafael): Nasty trick to make some games more playable on a non multi-threaded environment.
        //               If you are under MINIX 3.3.1 or under, this code shi.. I mean trick is valuable
        //               for you.

        for (cycles = 0; cycles < CP8_MAX_INSTRUCTIONS_PER_CYCLE; cycles++) {
            cp8_kbdread();
            processor.pc = cp8_cpu_exec(instr, &processor);
            cp8_kbdread();
            instr = cp8_emu_next_instr(processor.pc);
            cp8_kbdread();
            usleep(1);
        }

        cp8_kbdrelease();

#endif
        if (processor.dt > 0) {
            processor.dt--;
        }

        if (processor.st > 0) {
            processor.st--;
        }

        if (k == 'r' || k == 'R') {
            cp8_kbdsetkey(CP8_EMU_TSK_EMULATE_KQUIT);
            cp8_emu_finis();
            usleep(100);
            memset(&processor, 0, sizeof(processor));
            cp8_kbdsetkey(CP8_EMU_TSK_EMULATE_KQUIT >> 1);
            goto ___let_the_good_times_roll;
        }
    }

#undef cp8_emu_next_instr

    cp8_emu_finis();

    return 0;
}
