/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <vid/vid.h>
#include <mem/mem.h>
#include <ctx/types.h>
#include <accacia.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CP8_VID_MONITOR_W ( CP8_VIDMEM_MX + 2 )

#define CP8_VID_MONITOR_H ( CP8_VIDMEM_MY + 1 )

#define CP8_VID_MONITOR_X 1

#define CP8_VID_MONITOR_Y 8

#define cp8_monitor_x(x) ( ( (x) % CP8_VIDMEM_MX )  + CP8_VID_MONITOR_X + 1 )

#define cp8_monitor_y(y) ( ( (y) % CP8_VIDMEM_MY ) + CP8_VID_MONITOR_Y + 1 )

static int g_cp8_vid_tcolor = AC_BCOLOR_WHITE;

static int g_cp8_vid_bcolor = AC_TCOLOR_BLACK;

static unsigned char g_cp8_vid_builtin_fsprites[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xf0, 0x10, 0xf0, 0x80, 0xf0, // 2
    0xf0, 0x10, 0xf0, 0x10, 0xf0, // 3
    0x90, 0x90, 0xf0, 0x10, 0x10, // 4
    0xf0, 0x80, 0xf0, 0x10, 0xf0, // 5
    0xf0, 0x80, 0xf0, 0x90, 0xf0, // 6
    0xf0, 0x10, 0x20, 0x40, 0x40, // 7
    0xf0, 0x90, 0xf0, 0x90, 0xf0, // 8
    0xf0, 0x90, 0xf0, 0x10, 0xf0, // 9
    0xf0, 0x90, 0xf0, 0x90, 0x90, // A
    0xe0, 0x90, 0xe0, 0x90, 0xe0, // B
    0xf0, 0x80, 0x80, 0x80, 0xf0, // C
    0xe0, 0x90, 0x90, 0x90, 0xe0, // D
    0xf0, 0x80, 0xf0, 0x80, 0xf0, // E
    0xf0, 0x80, 0xf0, 0x80, 0x80  // F
};

// INFO(Rafael): This silly video memory map will help us on collision detection.

static unsigned char g_cp8_vidmem[CP8_VIDMEM_MX][CP8_VIDMEM_MY];

static int cp8_vidplotpixel(const unsigned char x, const unsigned char y, const unsigned char pixmap);

void cp8_vidcls(void) {
    int x, y;

    memset(g_cp8_vidmem, 0, sizeof(g_cp8_vidmem));

    accacia_textbackground(g_cp8_vid_bcolor);

    for (x = 0; x < CP8_VIDMEM_MX; x++) {
        for (y = 0; y < CP8_VIDMEM_MY; y++) {
            accacia_gotoxy(cp8_monitor_x(x), cp8_monitor_y(y)); printf(" ");
        }
    }

    accacia_screennormalize();
}

int cp8_viddrw(const unsigned char x, const unsigned char y, const unsigned char *sprite, const char unsigned sn) {
    unsigned char h;
    unsigned char cy = y;
    int collide = 0;
    for (h = 0; h < sn; h++) {
        if (cp8_vidplotpixel(x, cy, sprite[h])) {
            collide = 1;
        }
        cy++;
    }
    return collide;
}

static int cp8_vidplotpixel(const unsigned char x, const unsigned char y, const unsigned char pixmap) {
    int abs_x, abs_y;
    int bit;
    int cx = x, px = 0;
    int collide = 0;

    for (bit = 0; bit < 8; bit++) {
        abs_x = (cx % CP8_VIDMEM_MX);
        abs_y = (y % CP8_VIDMEM_MY);

        px = ((pixmap >> (7 - bit)) & 1);

        g_cp8_vidmem[abs_x][abs_y] ^= px;

        if (g_cp8_vidmem[abs_x][abs_y]) {
            accacia_textbackground(g_cp8_vid_tcolor);
        } else {
            accacia_textbackground(g_cp8_vid_bcolor);
            collide = 1;
        }

        accacia_gotoxy(cp8_monitor_x(cx), cp8_monitor_y(y)); printf(" ");
        accacia_gotoxy(1, 1);
        accacia_screennormalize();

        cx++;
    }

    return collide;
}

int cp8_vidinit(void) {
    size_t f;

    accacia_clrscr();
    cp8_vidcls();

    accacia_drawboxgine(CP8_VID_MONITOR_X, CP8_VID_MONITOR_Y, CP8_VID_MONITOR_W, CP8_VID_MONITOR_H,
                        g_cp8_vid_bcolor, g_cp8_vid_tcolor, "", g_cp8_vid_bcolor, "", g_cp8_vid_bcolor,
                        1, 1, 1, 1);
    accacia_screennormalize();
    accacia_gotoxy(CP8_VID_MONITOR_X, CP8_VID_MONITOR_Y);

    for (f = 0; f < sizeof(g_cp8_vid_builtin_fsprites); f++) {
        cp8_memset(CP8_FSPRITE_REGION + f, g_cp8_vid_builtin_fsprites[f]);
    }
}

int cp8_vidfinis(void) {
    cp8_vidcls();
    accacia_screennormalize();
    accacia_clrscr();
}

void cp8_vidset_tcolor(const int color) {
    g_cp8_vid_tcolor = color;
}

void cp8_vidset_bcolor(const int color) {
    g_cp8_vid_bcolor = color;
}

void cp8_vidblitchar(const int x, const int y, const cp8_blitchar_pxmap_t pxmap, const int tcolor, const int bcolor) {
    int cw, ch;

    if (pxmap == NULL) {
        return;
    }

    for (ch = 0; ch < CP8_BLITCHAR_MH; ch++) {
        for (cw = 0; cw < CP8_BLITCHAR_MW; cw++) {
            if ((pxmap[ch][cw] >> 4) == 0xf) {
                accacia_textbackground(tcolor);
            } else {
                accacia_textbackground(bcolor);
            }

            accacia_gotoxy(x + cw, y + ch); printf(" ");

            if ((pxmap[ch][cw] & 0xf) == 0xf) {
                accacia_textbackground(tcolor);
            } else {
                accacia_textbackground(bcolor);
            }

            accacia_gotoxy(x + cw + 1, y + ch); printf(" ");
            accacia_screennormalize();
        }
    }
}

#undef cp8_vid_monitor_x

#undef cp8_vid_monitor_y
