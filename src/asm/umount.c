/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <asm/umount.h>
#include <asm/dtp/dtp0.h>
#include <asm/dtp/dtp1.h>
#include <asm/dtp/dtp2.h>
#include <asm/dtp/dtp3.h>
#include <asm/dtp/dtp4.h>
#include <asm/dtp/dtp5.h>
#include <asm/dtp/dtp6.h>
#include <asm/dtp/dtp7.h>
#include <asm/dtp/dtp8.h>
#include <asm/dtp/dtp9.h>
#include <asm/dtp/dtpa.h>
#include <asm/dtp/dtpb.h>
#include <asm/dtp/dtpc.h>
#include <asm/dtp/dtpd.h>
#include <asm/dtp/dtpe.h>
#include <asm/dtp/dtpf.h>

#define set_cp8_dtp(i) ( cp8_dsm_dtp ## i )

struct cp8_dsm_ctx g_cp8_dsm_dtp[0x10] = {
    set_cp8_dtp(0), set_cp8_dtp(1), set_cp8_dtp(2), set_cp8_dtp(3),
    set_cp8_dtp(4), set_cp8_dtp(5), set_cp8_dtp(6), set_cp8_dtp(7),
    set_cp8_dtp(8), set_cp8_dtp(9), set_cp8_dtp(a), set_cp8_dtp(b),
    set_cp8_dtp(c), set_cp8_dtp(d), set_cp8_dtp(e), set_cp8_dtp(f)
};

#undef set_cp8_dtp

