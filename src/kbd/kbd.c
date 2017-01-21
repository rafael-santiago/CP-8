/*
 *                          Copyright (C) 2016, 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */

#include <kbd/kbd.h>
#include <vid/vid.h>
#include <accacia.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

static int g_kbd_tcolor = AC_BCOLOR_WHITE;

static int g_kbd_bcolor = AC_BCOLOR_BLACK;

static int g_kbd_pcolor = AC_BCOLOR_CYAN;

static int g_kbd_lkey = 0;

static int g_cp8_kpad[0x10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#ifndef NO_PTHREAD_SUPPORT

#define CP8_KBD_TTR_CEIL 100

#define CP8_KBD_TTR_FLOOR -300

static int g_ttr = 0;

static pthread_mutex_t g_cp8_kpad_mtx = PTHREAD_MUTEX_INITIALIZER;

static void *cp8_kbdloop(void *args);

static void cp8_kbdread(void);

#else

void cp8_kbdrelease(void);

#endif

static void cp8_kbdplotpixel(const unsigned char x, const unsigned char y, const int color, const unsigned char pixmap);

static void cp8_kbddrwk(const unsigned char x, const unsigned char y, const int color, const unsigned char *sprite, const char unsigned sn);

#define CP8_KBD_X 70

#define CP8_KBD_Y 15

#define CP8_KBD_XPAD 2

#define CP8_KBD_YPAD 1

// INFO(Rafael): The following craziness is related with all pixel maps for the retro 8-bit keys stuff.

static unsigned char g_kbd_k0[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0
};

static unsigned char g_kbd_k1[] = {
    0x20, 0x60, 0x20, 0x20, 0x70
};

static unsigned char g_kbd_k2[] = {
    0xf0, 0x10, 0xf0, 0x80, 0xf0
};

static unsigned char g_kbd_k3[] = {
    0xf0, 0x10, 0xf0, 0x10, 0xf0
};

static unsigned char g_kbd_k4[] = {
    0x90, 0x90, 0xf0, 0x10, 0x10
};

static unsigned char g_kbd_k5[] = {
    0xf0, 0x80, 0xf0, 0x10, 0xf0
};

static unsigned char g_kbd_k6[] = {
    0xf0, 0x80, 0xf0, 0x90, 0xf0
};

static unsigned char g_kbd_k7[] = {
    0xf0, 0x10, 0x20, 0x40, 0x40
};

static unsigned char g_kbd_k8[] = {
    0xf0, 0x90, 0xf0, 0x90, 0xf0
};

static unsigned char g_kbd_k9[] = {
    0xf0, 0x90, 0xf0, 0x10, 0xf0
};

static unsigned char g_kbd_ka[] = {
    0xf0, 0x90, 0xf0, 0x90, 0x90
};

static unsigned char g_kbd_kb[] = {
    0xe0, 0x90, 0xe0, 0x90, 0xe0
};

static unsigned char g_kbd_kc[] = {
    0xf0, 0x80, 0x80, 0x80, 0xf0
};

static unsigned char g_kbd_kd[] = {
    0xe0, 0x90, 0x90, 0x90, 0xe0
};

static unsigned char g_kbd_ke[] = {
    0xf0, 0x80, 0xf0, 0x80, 0xf0
};

static unsigned char g_kbd_kf[] = {
    0xf0, 0x80, 0xf0, 0x80, 0x80
};

// WARN(Rafael): Boring code and kind of stupid do not waste your time reading it..

static void cp8_kbdkdraw(const unsigned char key, int kcolor);

struct cp8_kbd_key_ctx {
    cp8_blitchar_pxmap_t *key;
    const int x, y;
};

static unsigned char kval(const unsigned char k) {
    unsigned char v = toupper(k);
    switch (v) {

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            v = v - '0';
            break;

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            v = v - 55;
            break;

    }

    return v;
}

unsigned char cp8_kbdhit(void) {
    unsigned char retval = 0xff;
    size_t k;

#ifndef NO_PTHREAD_SUPPORT
    pthread_mutex_lock(&g_cp8_kpad_mtx);
#endif

    for (k = 0; k < 0xf && retval == 0xff; k++) {
        if (g_cp8_kpad[k] == 1) {
            retval = k;
        }
    }

#ifndef NO_PTHREAD_SUPPORT
    pthread_mutex_unlock(&g_cp8_kpad_mtx);
#endif

    return retval;
}

#ifdef NO_PTHREAD_SUPPORT

void cp8_kbdrelease(void) {
    memset(g_cp8_kpad, 0, sizeof(g_cp8_kpad));
}

#endif

#ifndef NO_PTHREAD_SUPPORT

static void cp8_kbdread(void) {

#else

void cp8_kbdread(void) {

#endif
    unsigned char key = 0xff;

    accacia_savecursorposition();

    if (accacia_kbhit()) {
        cp8_kbdkdraw(g_kbd_lkey, g_kbd_tcolor);
        g_kbd_lkey = accacia_getch();
        key = kval(g_kbd_lkey);
    }

    if (key >= 0x0 && key <= 0xf) {
#ifndef NO_PTHREAD_SUPPORT
        pthread_mutex_lock(&g_cp8_kpad_mtx);
#endif
        memset(g_cp8_kpad, 0, sizeof(g_cp8_kpad));
        g_cp8_kpad[key] = 1;
#ifndef NO_PTHREAD_SUPPORT
        g_ttr = CP8_KBD_TTR_CEIL;
        cp8_kbdkdraw(g_kbd_lkey, g_kbd_tcolor);
        pthread_mutex_unlock(&g_cp8_kpad_mtx);
#endif
    }

    accacia_restorecursorposition();
}

unsigned char cp8_kbdlkey(void) {
    return g_kbd_lkey;
}

static void cp8_kbdkdraw(const unsigned char key, const int kcolor) {
    int x, y, k;
    static unsigned char *kbd_k[] = {
        g_kbd_k0, g_kbd_k1, g_kbd_k2, g_kbd_k3, g_kbd_k4, g_kbd_k5, g_kbd_k6, g_kbd_k7,
        g_kbd_k8, g_kbd_k9, g_kbd_ka, g_kbd_kb, g_kbd_kc, g_kbd_kd, g_kbd_ke, g_kbd_kf
    };
    // WARN(Rafael): Good luck with this awful look...

    switch (tolower(key)) {
        // CLUE(Rafael): Line 1
        case '1':
            x = CP8_KBD_X;
            y = CP8_KBD_Y;
            k = 0x1;
            break;

        case '2':
            x = CP8_KBD_X + CP8_BLITCHAR_MW + CP8_KBD_XPAD;
            y = CP8_KBD_Y;
            k = 0x2;
            break;

        case '3':
            x = CP8_KBD_X + (2 * (CP8_BLITCHAR_MW + CP8_KBD_XPAD));
            y = CP8_KBD_Y;
            k = 0x3;
            break;

        case 'c':
            x = CP8_KBD_X + (3 * (CP8_BLITCHAR_MW + CP8_KBD_XPAD));
            y = CP8_KBD_Y;
            k = 0xc;
            break;

        // CLUE(Rafael): Line 2
        case '4':
            x = CP8_KBD_X;
            y = CP8_KBD_Y + CP8_BLITCHAR_MH + CP8_KBD_YPAD;
            k = 0x4;
            break;

        case '5':
            x = CP8_KBD_X + CP8_BLITCHAR_MW + CP8_KBD_XPAD;
            y = CP8_KBD_Y + CP8_BLITCHAR_MH + CP8_KBD_YPAD;
            k = 0x5;
            break;

        case '6':
            x = CP8_KBD_X + (2 * (CP8_BLITCHAR_MW + CP8_KBD_XPAD));
            y = CP8_KBD_Y + CP8_BLITCHAR_MH + CP8_KBD_YPAD;
            k = 0x6;
            break;

        case 'd':
            x = CP8_KBD_X + (3 * (CP8_BLITCHAR_MW + CP8_KBD_XPAD));
            y = CP8_KBD_Y + CP8_BLITCHAR_MH + CP8_KBD_YPAD;
            k = 0xd;
            break;

        // CLUE(Rafael): Line 3
        case '7':
            x = CP8_KBD_X;
            y = CP8_KBD_Y + (2 * (CP8_BLITCHAR_MH + CP8_KBD_YPAD));
            k = 0x7;
            break;

        case '8':
            x = CP8_KBD_X + CP8_BLITCHAR_MW + CP8_KBD_XPAD;
            y = CP8_KBD_Y + (2 * (CP8_BLITCHAR_MH + CP8_KBD_YPAD));
            k = 0x8;
            break;

        case '9':
            x = CP8_KBD_X + (2 * (CP8_BLITCHAR_MW + CP8_KBD_XPAD));
            y = CP8_KBD_Y + (2 * (CP8_BLITCHAR_MH + CP8_KBD_YPAD));
            k = 0x9;
            break;

        case 'e':
            x = CP8_KBD_X + (3 * (CP8_BLITCHAR_MW + CP8_KBD_XPAD));
            y = CP8_KBD_Y + (2 * (CP8_BLITCHAR_MH + CP8_KBD_YPAD));
            k = 0xe;
            break;

        // CLUE(Rafael): Line 4
        case 'a':
            x = CP8_KBD_X;
            y = CP8_KBD_Y + (3 * (CP8_BLITCHAR_MH + CP8_KBD_YPAD));
            k = 0xa;
            break;

        case '0':
            x = CP8_KBD_X + CP8_BLITCHAR_MW + CP8_KBD_XPAD;
            y = CP8_KBD_Y + (3 * (CP8_BLITCHAR_MH + CP8_KBD_YPAD));
            k = 0x0;
            break;

        case 'b':
            x = CP8_KBD_X + (2 * (CP8_BLITCHAR_MW + CP8_KBD_XPAD));
            y = CP8_KBD_Y + (3 * (CP8_BLITCHAR_MH + CP8_KBD_YPAD));
            k = 0xb;
            break;

        case 'f':
            x = CP8_KBD_X + (3 * (CP8_BLITCHAR_MW + CP8_KBD_XPAD));
            y = CP8_KBD_Y + (3 * (CP8_BLITCHAR_MH + CP8_KBD_YPAD));
            k = 0xf;
            break;

        default:
            return;

    }

    accacia_savecursorposition();

    cp8_kbddrwk(x, y + !g_cp8_kpad[k], g_kbd_bcolor, kbd_k[k], 5);

    cp8_kbddrwk(x, y + g_cp8_kpad[k], kcolor, kbd_k[k], 5);

    accacia_restorecursorposition();
}

void cp8_kbdinit(void) {
#ifndef NO_PTHREAD_SUPPORT
    pthread_t kbd;
    pthread_attr_t kbdattr;
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&g_cp8_kpad_mtx, &attr);
#endif

    accacia_drawboxgine(CP8_KBD_X - 2, CP8_KBD_Y - 2, CP8_BLITCHAR_MW * 5 + 1, CP8_BLITCHAR_MH * 5 + 1,
                        g_kbd_bcolor, g_kbd_tcolor, "", g_kbd_bcolor, "", g_kbd_bcolor,
                        1, 1, 1, 1);

    cp8_kbdkdraw('1', g_kbd_tcolor);
    cp8_kbdkdraw('2', g_kbd_tcolor);
    cp8_kbdkdraw('3', g_kbd_tcolor);
    cp8_kbdkdraw('c', g_kbd_tcolor);
    cp8_kbdkdraw('4', g_kbd_tcolor);
    cp8_kbdkdraw('5', g_kbd_tcolor);
    cp8_kbdkdraw('6', g_kbd_tcolor);
    cp8_kbdkdraw('d', g_kbd_tcolor);
    cp8_kbdkdraw('7', g_kbd_tcolor);
    cp8_kbdkdraw('8', g_kbd_tcolor);
    cp8_kbdkdraw('9', g_kbd_tcolor);
    cp8_kbdkdraw('e', g_kbd_tcolor);
    cp8_kbdkdraw('a', g_kbd_tcolor);
    cp8_kbdkdraw('0', g_kbd_tcolor);
    cp8_kbdkdraw('b', g_kbd_tcolor);
    cp8_kbdkdraw('f', g_kbd_tcolor);

#ifndef NO_PTHREAD_SUPPORT
    pthread_attr_init(&kbdattr);
    pthread_create(&kbd, &kbdattr, cp8_kbdloop, NULL);
#endif
}


#ifndef NO_PTHREAD_SUPPORT

static void *cp8_kbdloop(void *args) {
    while (g_kbd_lkey != 27) {
        cp8_kbdread();
        usleep(1);
        pthread_mutex_lock(&g_cp8_kpad_mtx);
        if (g_ttr == 0) {
            memset(g_cp8_kpad, 0, sizeof(g_cp8_kpad));
            g_ttr = -1;
        } else if (g_ttr > CP8_KBD_TTR_FLOOR) {
            g_ttr--;
            if (g_ttr == CP8_KBD_TTR_FLOOR + 1) {
               cp8_kbdkdraw(g_kbd_lkey, g_kbd_tcolor);
            }
        }
        pthread_mutex_unlock(&g_cp8_kpad_mtx);
    }

    return NULL;
}

#endif

void cp8_kbdsetkey(unsigned char key) {
#ifndef NO_PTHREAD_SUPPORT
    pthread_mutex_lock(&g_cp8_kpad_mtx);
#endif

    g_kbd_lkey = key;
    key = kval(key);

    if (key >= 0x0 && key <= 0xf) {
        memset(g_cp8_kpad, 0, sizeof(g_cp8_kpad));
        g_cp8_kpad[key] = 1;
    }

#ifndef NO_PTHREAD_SUPPORT
    pthread_mutex_unlock(&g_cp8_kpad_mtx);
#endif
}

static void cp8_kbddrwk(const unsigned char x, const unsigned char y, const int color, const unsigned char *sprite, const char unsigned sn) {
    unsigned char h;
    unsigned char cy = y;

    for (h = 0; h < sn; h++) {
        cp8_kbdplotpixel(x, cy, color, sprite[h]);
        cy++;
    }
}

static void cp8_kbdplotpixel(const unsigned char x, const unsigned char y, const int color, const unsigned char pixmap) {
    int bit;
    int cx = x, px = 0;

    for (bit = 0; bit < 8; bit++) {
        px = ((pixmap >> (7 - bit)) & 1);

        accacia_textbackground((px == 1) ? color : g_kbd_bcolor);

        accacia_gotoxy(cx, y); printf(" ");

        accacia_screennormalize();

        cx++;
    }

    accacia_gotoxy(1, 1);
}
