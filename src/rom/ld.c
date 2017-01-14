/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <rom/ld.h>
#include <ctx/types.h>
#include <mem/mem.h>
#include <stdio.h>
#include <stdlib.h>

#define cp8_romld_naddr(sz, curr, end) ( CP8_TEXT_START + ( (sz) - ( (end) - (curr) ) ) )

int cp8_romld(const char *filepath, char *msg) {
    FILE *rom = NULL;
    unsigned char *romdata = NULL, *rp, *rp_end;
    size_t romdata_sz;
    int ok = 1;

    if (filepath == NULL) {
        if (msg != NULL) {
            sprintf(msg, "ERROR: no file path was supplied.\n");
        }
        ok = 0;
        goto ___cp8_romld_epilogue;
    }

    if ((rom = fopen(filepath, "rb")) == NULL) {
        if (msg != NULL) {
            sprintf(msg, "ERROR: unable to open the file \"%s\".\n", filepath);
        }
        ok = 0;
        goto ___cp8_romld_epilogue;
    }

    fseek(rom, 0L, SEEK_END);
    romdata_sz = ftell(rom);
    fseek(rom, 0L, SEEK_SET);

    if ((CP8_TEXT_START + romdata_sz) > CP8_MEMORY_SZ) {
        if (msg != NULL) {
            sprintf(msg, "ERROR: the ROM \"%s\" is too long and cannot be loaded.\n", filepath);
        }
        ok = 0;
        goto ___cp8_romld_epilogue;
    }

    romdata = (unsigned char *) malloc(romdata_sz + 1);
    if (romdata == NULL) {
        if (msg != NULL) {
            sprintf(msg, "ERROR: no memory.\n");
        }
        ok = 0;
        goto ___cp8_romld_epilogue;
    }

    if (fread(romdata, 1, romdata_sz, rom) != romdata_sz) {
        if (msg != NULL) {
            sprintf(msg, "ERROR: unable to load \"%s\" due to an unexpected read error.\n", filepath);
        }
        ok = 0;
        goto ___cp8_romld_epilogue;
    }

    rp = romdata;
    rp_end = romdata + romdata_sz;

    while (rp <= rp_end) {
        cp8_memset(cp8_romld_naddr(romdata_sz, rp, rp_end), *rp);
        rp++;
    }

    if (msg != NULL) {
        sprintf(msg, "INFO: the ROM \"%s\" was loaded.\n", filepath);
    }

___cp8_romld_epilogue:

    if (rom != NULL) {
        fclose(rom);
    }

    if (romdata != NULL) { // INFO(Rafael): With love to true UNIX Dinosaurs out there! :-)
        free(romdata);
    }

    return ok;
}

#undef cp8_romld_naddr
