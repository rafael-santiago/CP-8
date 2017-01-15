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
#include <rom/ld.h>
#include <ctx/types.h>
#include <accacia.h>
#include <stdio.h>

#define CP8_EMU_TSK_EMULATE_KQUIT 27

int cp8_emu_tsk_emulate(void) {
    char *rom = cp8_emu_option("rom", NULL);
    char msg[255];
    unsigned short instr;
    struct cp8_ctx processor;
    unsigned char k = CP8_EMU_TSK_EMULATE_KQUIT >> 1;

    if (rom == NULL) {
        printf("ERROR: the option --rom is missing.\n");
        return 1;
    }

    if (cp8_romld(rom, msg) == 0) {
        printf("%s", msg);
        return 1;
    }

    cp8_emu_init(&processor);

#define cp8_emu_next_instr(pc) ( ((unsigned short)(cp8_memget(CP8_TEXT_START + (pc) )) << 8 ) |\
                                  cp8_memget(CP8_TEXT_START + ((pc) + 1)) )

    instr = cp8_emu_next_instr(processor.pc);

    while (k != CP8_EMU_TSK_EMULATE_KQUIT) {
        accacia_gotoxy(1, 1); printf("INSTRUCTION = 0x%.4X", instr);
        accacia_gotoxy(1, 2); printf("PC = 0x%.4X", processor.pc);
        accacia_getch();
        processor.pc = cp8_cpu_exec(instr, &processor);
        instr = cp8_emu_next_instr(processor.pc);
        if (accacia_kbhit()) {
            k = accacia_getch();
        }
    }

#undef cp8_emu_next_instr

    cp8_emu_finis();

    return 0;
}

