/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#ifndef CP8_ASM_UMOUNT_H
#define CP8_ASM_UMOUNT_H 1

typedef char *(*cp8_dsm_mne)(unsigned short nnn);

struct cp8_dsm_ctx {
    cp8_dsm_mne mne;
};

extern struct cp8_dsm_ctx g_cp8_dsm_dtp[0x10];

#define cp8_asm_umount(i) ( g_cp8_dsm_dtp[(i) >> 12].mne( cp8_asm_var(nnn, i) ) )

#endif
