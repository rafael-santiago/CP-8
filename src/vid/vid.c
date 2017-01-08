/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <vid/vid.h>
#include <ctx/types.h>
#include <accacia.h>
#include <stdio.h>
#include <string.h>

#define CP8_VID_MONITOR_W ( CP8_VIDMEM_MX + 1 )

#define CP8_VID_MONITOR_H ( CP8_VIDMEM_MY + 1 )

#define CP8_VID_MONITOR_X 1

#define CP8_VID_MONITOR_Y 8

#define cp8_monitor_x(x) ( ( (x) % CP8_VIDMEM_MX )  + CP8_VID_MONITOR_X + 1 )

#define cp8_monitor_y(y) ( ( (y) % CP8_VIDMEM_MY ) + CP8_VID_MONITOR_Y + 1 )

static int g_cp8_vid_tcolor = AC_BCOLOR_WHITE;

static int g_cp8_vid_bcolor = AC_TCOLOR_BLACK;

// INFO(Rafael): This silly video memory map will help us on collision detection.

static unsigned char g_cp8_vidmem[CP8_VIDMEM_MX][CP8_VIDMEM_MY];

static int cp8_vidplotpixel(const unsigned char x, const unsigned char y);

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
    unsigned char h, w;
    unsigned char cx, cy;
    int collide = 0;
    cy = y;
    for (h = 0; h < sn; h++) {
        cx = x;
        for (w = 0; w < 8; w++) {
            if (cp8_vidplotpixel(cx, cy)) {
                collide = 1;
            }
            cx++;
        }
        cy++;
    }
    return collide;
}

static int cp8_vidplotpixel(const unsigned char x, const unsigned char y) {
    int abs_x = (x % CP8_VIDMEM_MX), abs_y = (y % CP8_VIDMEM_MY);
    int collide = ((g_cp8_vidmem[abs_x][abs_y] ^ 1) == 0);
    g_cp8_vidmem[abs_x][abs_y] = 1;
    accacia_textbackground(g_cp8_vid_tcolor);
    accacia_gotoxy(cp8_monitor_x(x), cp8_monitor_y(y)); printf(" ");
    accacia_gotoxy(CP8_VID_MONITOR_X, CP8_VID_MONITOR_Y);
    accacia_screennormalize();
    return collide;
}

int cp8_vidinit(void) {
    accacia_clrscr();
    cp8_vidcls();
    accacia_drawboxgine(CP8_VID_MONITOR_X, CP8_VID_MONITOR_Y, CP8_VID_MONITOR_W, CP8_VID_MONITOR_H,
                        g_cp8_vid_bcolor, g_cp8_vid_tcolor, "", g_cp8_vid_bcolor, "", g_cp8_vid_bcolor,
                        1, 1, 1, 1);
    accacia_screennormalize();
    accacia_gotoxy(CP8_VID_MONITOR_X, CP8_VID_MONITOR_Y);
}

int cp8_vidfini(void) {
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
