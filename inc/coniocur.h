/*
 * coniocur.h - Borland-style CONIO driver for the MyMan video game
 * Copyright 2007-2008, Benjamin C. Wiley Sittler <bsittler@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy,
 *  modify, merge, publish, distribute, sublicense, and/or sell copies
 *  of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */

/* work-arounds for Borland-style CONIO */
#ifdef CONIOCURSES_CONIO_H
#include CONIOCURSES_CONIO_H
#else
#include <conio.h>
#endif

#if defined(WIN32)
#include <windows.h>

#ifndef HAVE_GETTIMEOFDAY
#define HAVE_GETTIMEOFDAY 0
#endif

#ifndef HAVE_USLEEP
#define HAVE_USLEEP 0
#endif

#endif

#if defined(__MSDOS__)
#include <dos.h>

#ifdef __DJGPP__

/* DJGPP supports termios and full Borland-style CONIO */

#ifndef HAVE_GETTIMEOFDAY
#define HAVE_GETTIMEOFDAY 1
#endif

#ifndef HAVE_USLEEP
#define HAVE_USLEEP 1
#endif

#ifndef HAVE_STRUCT_TIMEVAL
#define HAVE_STRUCT_TIMEVAL 1
#endif

#endif /* defined(__DJGPP__) */

#ifdef __BCC__

#ifndef HAVE_STRUCT_TIMEVAL
#define HAVE_STRUCT_TIMEVAL 1
#endif

#endif

#ifndef HAVE_STRUCT_TIMEVAL
#define HAVE_STRUCT_TIMEVAL 0
#endif

#ifndef HAVE_GETTIMEOFDAY
#define HAVE_GETTIMEOFDAY 0
#endif

#ifndef HAVE_USLEEP
#define HAVE_USLEEP 0
#endif

#endif

#ifndef HAVE_STRUCT_TIMEVAL
#define HAVE_STRUCT_TIMEVAL 1
#endif

#if ! HAVE_STRUCT_TIMEVAL

#undef timeval
#define timeval coniocurses_timeval
struct timeval
{
    long tv_sec;
    long tv_usec;
};

#endif

#ifndef HAVE_GETTIMEOFDAY
#define HAVE_GETTIMEOFDAY 1
#endif

#if ! HAVE_GETTIMEOFDAY

#undef gettimeofday
#define gettimeofday coniocurses_gettimeofday
static int gettimeofday(struct timeval *tv, void *tz)
{

#if defined(WIN32)

/* originally from http://curl.haxx.se/mail/lib-2005-01/0089.html by Gisle Vanem */
    union {
        __int64 ns100;
        FILETIME ft;
    } now;
    GetSystemTimeAsFileTime(&now.ft);
    tv->tv_usec = (long) ((now.ns100 / LIT64(10)) % LIT64(1000000));
    tv->tv_sec = (long) ((now.ns100 - LIT64(116444736000000000)) / LIT64(10000000));

#else /* ! defined(WIN32) */

#if defined(__MSDOS__)

/* HACK: this is wildly, wildly wrong for anything but tick counting! */

/* originally from http://www.lightner.net/lightner/bruce/photopc/msdos/patch/usleep.c */

    static unsigned long far *p;
    unsigned long ticks;

    if (!p) p = MK_FP(0, 0x46c);
    ticks = *p;
    tv->tv_usec = ((ticks & 0xff) * 55000L) % 1000000L;
    tv->tv_sec = (ticks & 0xff) * 55L / 1000L;

#else /* ! defined(__MSDOS__) */

#error no gettimeofday(2) implementation for your platform, sorry
    return 1;

#endif /* ! defined(__MSDOS__) */

#endif /* ! defined(WIN32) */

    return 0;
}

#endif

#ifndef HAVE_USLEEP
#define HAVE_USLEEP 1
#endif

#if ! HAVE_USLEEP

#if defined(WIN32)

#undef usleep

/* originally from http://wyw.dcweb.cn/sleep.h.txt by Wu Yongwei */

#define usleep(t) Sleep((t) / 1000)

#else /* ! defined(WIN32) */

#undef usleep
#define usleep coniocurses_usleep
static int usleep(unsigned long usecs)
{
    while (usecs)
    {
        struct timeval tv0, tv1;

        if (myman_gettimeofday(&tv0, NULL)) break;
        sleep(0);
        if (myman_gettimeofday(&tv1, NULL)) break;
        if (tv1.tv_sec < tv0.tv_sec) break;
        if ((tv1.tv_sec == tv0.tv_sec)
            &&
            (tv1.tv_usec <= tv0.tv_usec))
        {
            break;
        }
        if ((tv1.tv_sec - tv0.tv_sec) > (usecs / 1000000L))
        {
            break;
        }
        if (((tv1.tv_sec - tv0.tv_sec) * 1000000L
             +
             (tv1.tv_usec - tv0.tv_usec))
            >=
            usecs)
        {
            break;
        }
        usecs -= (tv1.tv_sec - tv0.tv_sec) * 1000000L;
        usecs -= tv1.tv_usec - tv0.tv_usec;
    }
    return 0;
}

#endif /* ! defined(WIN32) */

#endif

#if ! (defined(LSI_C) || defined(__PACIFIC__) || defined(HI_TECH_C) || defined(SMALL_C) || defined(__TURBOC__) || (defined(__BCC__) && defined(__MSDOS__)))
#ifdef __atarist__
#if ((! defined(__GNUC__)) || (__GNUC__ > 2))
#include <stdint.h>
#endif /* ((! defined(__GNUC__)) || (__GNUC__ > 2)) */
#else /* ! defined(__atarist__) */
#include <wchar.h>
#endif /* ! defined(__atarist__) */
#endif /* ! (defined(LSI_C) || defined(__PACIFIC__) || defined(HI_TECH_C) || defined(SMALL_C) || defined(__TURBOC__) || (defined(__BCC__) && defined(__MSDOS__))) */

#if defined(LSI_C) || defined(__TURBOC__)
#ifndef wchar_t
#define wchar_t coniocur_wchar_t
typedef int coniocur_wchar_t;
#endif
#endif

#ifndef USE_ATTR
#define USE_ATTR 1
#endif

#ifndef USE_UNDERLINE
#define USE_UNDERLINE 1
#endif

#ifndef SWAPDOTS
#define SWAPDOTS 0
#endif

#ifndef USE_ACS
#define USE_ACS 1
#endif

#ifndef USE_RAW_UCS
#define USE_RAW_UCS 0
#endif

#ifndef USE_SIGWINCH
#define USE_SIGWINCH 0
#endif

#ifndef USE_IOCTL
#define USE_IOCTL 0
#endif

#ifndef USE_WCWIDTH
#define USE_WCWIDTH 1
#endif

#ifndef USE_ICONV
#define USE_ICONV 0
#endif

#ifndef OK
#define OK (0)
#endif

#ifndef ERR
#define ERR (-1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (! FALSE)
#endif

#undef wcwidth
#define wcwidth(ch) coniocurses_wcwidth(ch)

static int coniocurses_ready = 0;
static int coniocurses_color = 0;

typedef unsigned long int coniocurses_chtype;

#undef chtype
#define chtype coniocurses_chtype

typedef coniocurses_chtype coniocurses_attr_t;

#undef attr_t
#define attr_t coniocurses_attr_t

static int coniocurses_w = 0, coniocurses_h = 0;

static int coniocurses_x = 0, coniocurses_y = 0;

static coniocurses_attr_t coniocurses_attr = 0;

#define clear erase

#define cbreak()

#define noecho()

#define nonl()

#define nodelay(stdscr, x)

#define intrflush(stdscr, x)

#define immedok(stdscr, x)

#define idlok(stdscr, x)

#define clearok(stdscr, x)

#define leaveok(stdscr, x)

#define getyx(stdscr, y, x) ((y) = coniocurses_y), ((x) = coniocurses_x)

#define wrefresh(stdscr) refresh()

#define beep() coniocurses_addch('\a')

#define use_env(x)

#define echo()

#define can_change_color() (0)

#define keypad(stdscr, x)

#define LINES (coniocurses_h)

#define COLS (coniocurses_w)

#undef KEY_RESIZE
#define KEY_RESIZE 0xef00

#undef KEY_UP
#define KEY_UP 0x0f01

#undef KEY_DOWN
#define KEY_DOWN 0xef02

#undef KEY_LEFT
#define KEY_LEFT 0xef03

#undef KEY_RIGHT
#define KEY_RIGHT 0xef04

#define A_DIM 0x800000UL

#define A_UNDERLINE A_DIM

#define A_BOLD 0x400000UL

#define A_STANDOUT 0x200000UL

#define A_CHARTEXT 0x1fffffUL

#define A_REVERSE A_STANDOUT

#define CONIOCURSES___A_COLOR 0xff000000UL

#define COLOR_PAIRS 256

#define COLORS 16

#define COLOR_BLACK 0

#define COLOR_BLUE 1

#define COLOR_GREEN 2

#define COLOR_CYAN (COLOR_BLUE | COLOR_GREEN)

#define COLOR_RED 4

#define COLOR_MAGENTA (COLOR_RED | COLOR_BLUE)

#define COLOR_YELLOW (COLOR_RED | COLOR_GREEN)

#define COLOR_WHITE (COLOR_RED | COLOR_GREEN | COLOR_BLUE)

#define PEN_BRIGHT 8

#define _PAIR_SHIFT 24

#define COLOR_PAIR(p) (((coniocurses_attr_t) p) << _PAIR_SHIFT)

#define PAIR_NUMBER(a) (((a) & CONIOCURSES___A_COLOR) >> _PAIR_SHIFT)

#define ACS_LRCORNER 0xd9UL

#define ACS_URCORNER 0xbfUL

#define ACS_ULCORNER 0xdaUL

#define ACS_LLCORNER 0xc0UL

#define ACS_PLUS 0xc5UL

#define ACS_HLINE 0xc4UL

#define ACS_LTEE 0xc3UL

#define ACS_RTEE 0xb4UL

#define ACS_BTEE 0xc1UL

#define ACS_TTEE 0xc2UL

#define ACS_VLINE 0xb3UL

/* #define ACS_BULLET 0xf9UL */

#define ACS_BULLET 0xfeUL

#define ACS_BLOCK 0xdbUL

static struct { short fg, bg; } coniocurses_pairs[COLOR_PAIRS];

static int coniocurses_wcwidth(coniocurses_chtype ch)
{
    ch &= A_CHARTEXT;
    switch (ch)
    {
    case ACS_LRCORNER:
    case ACS_URCORNER:
    case ACS_ULCORNER:
    case ACS_LLCORNER:
    case ACS_PLUS:
    case ACS_HLINE:
    case ACS_LTEE:
    case ACS_RTEE:
    case ACS_BTEE:
    case ACS_TTEE:
    case ACS_VLINE:
    case ACS_BULLET:
    case ACS_BLOCK:
        return 1;
    }
    return ((((ch) >= ' ') && ((ch) == (coniocurses_chtype) (unsigned long) (unsigned char) (char) ch)) ? 1 : 0);
}

static int curs_set(x)
{
    int ret;

    if (! coniocurses_ready) return ERR;
    if (x)
    {
#ifdef _NORMALCURSOR
        _setcursortype(_NORMALCURSOR);
        ret = OK;
#else
        ret = ERR;
#endif
    }
    else
    {
#ifdef _NOCURSOR
        _setcursortype(_NOCURSOR);
        ret = OK;
#else
        ret = ERR;
#endif
    }
    return ret;
}

static int attrset(coniocurses_attr_t a)
{
    if (! coniocurses_ready) return ERR;
    coniocurses_attr = a;
    return OK;
}

static int standout(void)
{
    return attrset(A_STANDOUT);
}

static int standend(void)
{
    return attrset(0);
}

static int has_colors(void)
{
    if (! coniocurses_ready) return ERR;
    return coniocurses_color;
}

static int start_color(void)
{
    if (! coniocurses_ready) return ERR;
    if (! has_colors()) return ERR;
    return OK;
}

static int init_pair(short i, short fg, short bg);

#define INITSCR_WITH_HINTS

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    int i;

    coniocurses_color = 1;
    coniocurses_w = 80;
    coniocurses_h = 25;
    do
    {
        struct text_info screensize_info;

#if ! defined(_CONIO2_H_)
        screensize_info.currmode = -1;
#endif
        screensize_info.screenwidth = 0;
        screensize_info.screenheight = 0;
        gettextinfo(&screensize_info);
#if ! defined(_CONIO2_H_)
        switch (screensize_info.currmode)
        {
        case BW40:
        case C40:
            coniocurses_w = 40;
            coniocurses_h = 25;
            break;
        case BW80:
        case C80:
            coniocurses_w = 80;
            coniocurses_h = 25;
            break;
        case MONO:
            break;
#ifndef __TURBOC__
        case C4350:
            coniocurses_w = 80;
            coniocurses_h = 43;
            break;
#endif
#if defined(__BORLANDC__) && defined(WIN32)
        case C40X14:
        case BW40X14:
            coniocurses_w = 40;
            coniocurses_h = 14;
            break;
        case C40X21:
        case BW40X21:
            coniocurses_w = 40;
            coniocurses_h = 21;
            break;
        case C40X28:
        case BW40X28:
            coniocurses_w = 40;
            coniocurses_h = 28;
            break;
        case C40X43:
        case BW40X43:
            coniocurses_w = 40;
            coniocurses_h = 43;
            break;
        case C40X50:
        case BW40X50:
            coniocurses_w = 40;
            coniocurses_h = 50;
            break;
        case C40X60:
        case BW40X60:
            coniocurses_w = 40;
            coniocurses_h = 60;
            break;
        case C80X14:
        case BW80X14:
        case MONO14:
            coniocurses_w = 80;
            coniocurses_h = 14;
            break;
        case C80X21:
        case BW80X21:
        case MONO21:
            coniocurses_w = 80;
            coniocurses_h = 21;
            break;
        case C80X28:
        case BW80X28:
        case MONO28:
            coniocurses_w = 80;
            coniocurses_h = 28;
            break;
        case C80X43:
        case BW80X43:
        case MONO43:
            coniocurses_w = 80;
            coniocurses_h = 43;
            break;
        case C80X50:
        case BW80X50:
        case MONO50:
            coniocurses_w = 80;
            coniocurses_h = 50;
            break;
        case C80X60:
        case BW80X60:
        case MONO60:
            coniocurses_w = 80;
            coniocurses_h = 60;
            break;
#endif /* defined(__BORLANDC__) && defined(WIN32) */
        }
#endif /* ! defined(_CONIO2_H_) */
        if (screensize_info.screenwidth)
        {
            coniocurses_w = screensize_info.screenwidth;
        }
        if (screensize_info.screenheight)
        {
            coniocurses_h = screensize_info.screenheight;
        }
    }
    while (0);
#if ! defined(_CONIO2_H_)
    do
    {
        struct text_info color_info;

        color_info.currmode = -1;
        gettextinfo(&color_info);
        switch (color_info.currmode)
        {
        case BW40:
        case BW80:
        case MONO:
#if defined(__BORLANDC__) && defined(WIN32)
        case BW40X14:
        case BW40X21:
        case BW40X28:
        case BW40X43:
        case BW40X50:
        case BW40X60:
        case BW80X14:
        case BW80X21:
        case BW80X28:
        case BW80X43:
        case BW80X50:
        case BW80X60:
        case MONO14:
        case MONO21:
        case MONO28:
        case MONO43:
        case MONO50:
        case MONO60:
#endif /* defined(__BORLANDC__) && defined(WIN32) */
            coniocurses_color = 0;
            break;
        default:
            coniocurses_color = 1;
        }
    }
    while (0);
#endif /* ! defined(_CONIO2_H_) */
    coniocurses_ready = 1;
    coniocurses_attr = -1;
    for (i = 0; i < COLOR_PAIRS; i ++)
    {
        coniocurses_pairs[i].fg = i ? (i % COLORS) : COLOR_WHITE;
        coniocurses_pairs[i].bg = i / COLORS;
    }
    attrset(0);
}

static void endwin(void)
{
    int ox, oy;

    coniocurses_ready = 0;
    textcolor(COLOR_WHITE);
    textbackground(COLOR_BLACK);
#ifdef _NORMALCURSOR
    _setcursortype(_NORMALCURSOR);
#endif
    ox = wherex();
    oy = wherey();
    putch(' ');
    gotoxy(ox, oy);
    clrscr();
}

static int move(int y, int x);

static int coniocurses_addch(coniocurses_chtype ch);

static int erase(void)
{
    if (! coniocurses_ready) return ERR;
    clrscr();
    gotoxy(1, 1);
    attrset(0);
    move(0, 0);
    return OK;
}

static int resizeterm(int y, int x)
{
    if (! coniocurses_ready) return ERR;
    do
    {
        struct text_info screensize_info;

#if ! defined(_CONIO2_H_)
        screensize_info.currmode = -1;
#endif
        screensize_info.screenwidth = 0;
        screensize_info.screenheight = 0;
        gettextinfo(&screensize_info);
#if ! defined(_CONIO2_H_)
        switch (screensize_info.currmode)
        {
        case BW40:
        case C40:
            coniocurses_w = 40;
            coniocurses_h = 25;
            break;
        case BW80:
        case C80:
            coniocurses_w = 80;
            coniocurses_h = 25;
            break;
        case MONO:
            break;
#ifndef __TURBOC__
        case C4350:
            coniocurses_w = 80;
            coniocurses_h = 43;
            break;
#endif
#if defined(__BORLANDC__) && defined(WIN32)
        case C40X14:
        case BW40X14:
            coniocurses_w = 40;
            coniocurses_h = 14;
            break;
        case C40X21:
        case BW40X21:
            coniocurses_w = 40;
            coniocurses_h = 21;
            break;
        case C40X28:
        case BW40X28:
            coniocurses_w = 40;
            coniocurses_h = 28;
            break;
        case C40X43:
        case BW40X43:
            coniocurses_w = 40;
            coniocurses_h = 43;
            break;
        case C40X50:
        case BW40X50:
            coniocurses_w = 40;
            coniocurses_h = 50;
            break;
        case C40X60:
        case BW40X60:
            coniocurses_w = 40;
            coniocurses_h = 60;
            break;
        case C80X14:
        case BW80X14:
        case MONO14:
            coniocurses_w = 80;
            coniocurses_h = 14;
            break;
        case C80X21:
        case BW80X21:
        case MONO21:
            coniocurses_w = 80;
            coniocurses_h = 21;
            break;
        case C80X28:
        case BW80X28:
        case MONO28:
            coniocurses_w = 80;
            coniocurses_h = 28;
            break;
        case C80X43:
        case BW80X43:
        case MONO43:
            coniocurses_w = 80;
            coniocurses_h = 43;
            break;
        case C80X50:
        case BW80X50:
        case MONO50:
            coniocurses_w = 80;
            coniocurses_h = 50;
            break;
        case C80X60:
        case BW80X60:
        case MONO60:
            coniocurses_w = 80;
            coniocurses_h = 60;
            break;
#endif /* defined(__BORLANDC__) && defined(WIN32) */
        }
#endif /* ! defined(_CONIO2_H_) */
        if (screensize_info.screenwidth)
        {
            coniocurses_w = screensize_info.screenwidth;
        }
        if (screensize_info.screenheight)
        {
            coniocurses_h = screensize_info.screenheight;
        }
    }
    while (0);
    return OK;
}

static int coniocurses_getch(void) {
    int ret = ERR;
    int i, j;

    if (! coniocurses_ready) return (coniocurses_chtype) ERR;
    do
    {
        struct text_info screensize_info;

#if ! defined(_CONIO2_H_)
        screensize_info.currmode = -1;
#endif
        screensize_info.screenwidth = 0;
        screensize_info.screenheight = 0;
        gettextinfo(&screensize_info);
#if ! defined(_CONIO2_H_)
        switch (screensize_info.currmode)
        {
        case BW40:
        case C40:
            i = 40;
            j = 25;
            break;
        case BW80:
        case C80:
            i = 80;
            j = 25;
            break;
        case MONO:
            break;
#ifndef __TURBOC__
        case C4350:
            i = 80;
            j = 43;
            break;
#endif
#if defined(__BORLANDC__) && defined(WIN32)
        case C40X14:
        case BW40X14:
            i = 40;
            j = 14;
            break;
        case C40X21:
        case BW40X21:
            i = 40;
            j = 21;
            break;
        case C40X28:
        case BW40X28:
            i = 40;
            j = 28;
            break;
        case C40X43:
        case BW40X43:
            i = 40;
            j = 43;
            break;
        case C40X50:
        case BW40X50:
            i = 40;
            j = 50;
            break;
        case C40X60:
        case BW40X60:
            i = 40;
            j = 60;
            break;
        case C80X14:
        case BW80X14:
        case MONO14:
            i = 80;
            j = 14;
            break;
        case C80X21:
        case BW80X21:
        case MONO21:
            i = 80;
            j = 21;
            break;
        case C80X28:
        case BW80X28:
        case MONO28:
            i = 80;
            j = 28;
            break;
        case C80X43:
        case BW80X43:
        case MONO43:
            i = 80;
            j = 43;
            break;
        case C80X50:
        case BW80X50:
        case MONO50:
            i = 80;
            j = 50;
            break;
        case C80X60:
        case BW80X60:
        case MONO60:
            i = 80;
            j = 60;
            break;
#endif /* defined(__BORLANDC__) && defined(WIN32) */
        }
#endif /* ! defined(_CONIO2_H_) */
        if (screensize_info.screenwidth)
        {
            i = screensize_info.screenwidth;
        }
        if (screensize_info.screenheight)
        {
            j = screensize_info.screenheight;
        }
    }
    while (0);
    if ((i != coniocurses_w) || (j != coniocurses_h))
    {
        return KEY_RESIZE;
    }
    if (kbhit())
    {
        ret = getch();
        if (ret == 0)
        {
            ret = getch();
            if ((ret == 0)
                ||
                (ret == 0x03))
            {
                ret = 0;
            }
            else if (ret == 0x48)
            {
                return KEY_UP;
            }
            else if (ret == 0x4b)
            {
                return KEY_LEFT;
            }
            else if (ret == 0x4d)
            {
                return KEY_RIGHT;
            }
            else if (ret == 0x50)
            {
                return KEY_DOWN;
            }
            else
            {
                ret = ERR;
            }
        }
    }
    return ret;
}

#undef getch
#define getch coniocurses_getch

#undef addch
#define addch coniocurses_addch

static int refresh(void)
{
    if (! coniocurses_ready) return OK;
    gotoxy(coniocurses_x + 1, coniocurses_y + 1);
    return OK;
}

static int move(int y, int x)
{
    if (! coniocurses_ready) return ERR;
    if (y < 0) return ERR;
    if (y > coniocurses_h) return ERR;
    if (x < 0) return ERR;
    if (x > coniocurses_w) return ERR;
    coniocurses_y = y;
    coniocurses_x = x;
    return OK;
}

static int insch(const coniocurses_chtype ch)
{
    int ret;
    int oy, ox;

    if (! coniocurses_ready) return ERR;
    movetext(coniocurses_x + 1 + 1, coniocurses_y + 1,
             coniocurses_w, coniocurses_y + 1,
             coniocurses_x + 1, coniocurses_y + 1);
    getyx(stdscr, oy, ox);
    ret = coniocurses_addch(ch);
    move(oy, ox);
    return ret;
}

static int delch(void)
{
    int ret;
    int oy, ox;

    if (! coniocurses_ready) return ERR;
    movetext(coniocurses_x + 1, coniocurses_y + 1,
             coniocurses_w, coniocurses_y + 1,
             coniocurses_x + 1 + 1, coniocurses_y + 1);
    return OK;
}

static int init_pair(short i, short fg, short bg)
{
    if (! coniocurses_ready) return ERR;
    if (! has_colors()) return ERR;
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    if ((fg < 0) || (fg > COLORS)) return ERR;
    if ((bg < 0) || (bg > COLORS)) return ERR;
    coniocurses_pairs[i].fg = fg;
    coniocurses_pairs[i].bg = bg;
    return OK;
}

static int pair_content(short i, short *fg, short *bg)
{
    if (! coniocurses_ready) return ERR;
    if (! has_colors()) return ERR;
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    *fg = coniocurses_pairs[i].fg;
    *bg = coniocurses_pairs[i].bg;
    return OK;
}

static int init_color(short i, short r, short g, short b)
{
    return ERR;
}

static int color_content(short i, short *r, short *g, short *b)
{
    return ERR;
}

static int coniocurses_addch(coniocurses_chtype ch)
{
    short fg, bg;

    if (! coniocurses_ready) return ERR;
    if (ch & ~A_CHARTEXT)
    {
        int ret;
        coniocurses_attr_t a;

        a = coniocurses_attr;
        attrset((PAIR_NUMBER(ch & ~A_CHARTEXT) ? (a & ~COLOR_PAIR(PAIR_NUMBER(a))) : a) | (ch & ~A_CHARTEXT));
        ret = coniocurses_addch(ch & A_CHARTEXT);
        attrset(a);
        return ret;
    }
    if (ch == (coniocurses_chtype) (unsigned long) (unsigned char) '\a')
    {
        /*sound_beep(440);*/
        return OK;
    }
    else if (ch == (coniocurses_chtype) (unsigned long) (unsigned char) '\b')
    {
        if (coniocurses_y && ! coniocurses_x)
        {
            move(coniocurses_y - 1, coniocurses_w - 1);
        }
        else if (coniocurses_x)
        {
            move(coniocurses_y, coniocurses_x - 1);
        }
        return OK;
    }
    else if (ch == (coniocurses_chtype) (unsigned long) (unsigned char) '\n')
    {
        if ((coniocurses_y + 1) < coniocurses_h)
        {
            move(coniocurses_y + 1, 0);
        }
        else
        {
            move(coniocurses_y, 0);
        }
        return OK;
    }
    else if (ch == (coniocurses_chtype) (unsigned long) (unsigned char) '\t')
    {
        do
        {
            coniocurses_addch((coniocurses_chtype) (unsigned long) (unsigned char) ' ');
        }
        while (coniocurses_x % 8);
        return OK;
    }
    else if (wcwidth(ch) < 1)
    {
        return ERR;
    }
    fg = COLOR_WHITE;
    bg = COLOR_BLACK;
    if (has_colors())
    {
        pair_content(PAIR_NUMBER(coniocurses_attr), &fg, &bg);
    }
    if (coniocurses_attr & A_UNDERLINE)
    {
        fg = COLOR_BLUE;
        bg = COLOR_BLACK;
    }
    else if ((coniocurses_attr & A_STANDOUT) && has_colors())
    {
        fg = COLOR_MAGENTA;
        bg = COLOR_BLACK;
    }
    else if ((coniocurses_attr & A_BOLD) && has_colors())
    {
        fg = COLOR_YELLOW | PEN_BRIGHT;
        bg = COLOR_BLACK;
    }
    if (has_colors() && PAIR_NUMBER(coniocurses_attr))
    {
        pair_content(PAIR_NUMBER(coniocurses_attr), &fg, &bg);
    }
    if (coniocurses_attr & A_BOLD)
    {
        fg |= PEN_BRIGHT;
    }
    if (coniocurses_attr & (A_STANDOUT | A_REVERSE))
    {
        short tmp;

        tmp = bg;
        bg = fg;
        fg = tmp;
    }
    if (coniocurses_x < 0) coniocurses_x = 0;
    if (coniocurses_x >= coniocurses_w)
    {
        coniocurses_x = 0;
        coniocurses_y ++;
    }
    if (coniocurses_y < 0) coniocurses_y = 0;
    if (coniocurses_y >= coniocurses_h)
    {
        coniocurses_y = coniocurses_h - 1;
    }
    gotoxy(coniocurses_x + 1, coniocurses_y + 1);
    textcolor(fg);
    textbackground(bg);
    if (((coniocurses_y + 1) < coniocurses_h)
        ||
        ((coniocurses_x + 1) < coniocurses_w))
    {
        putch((int) (unsigned char) ch);
    }
    else
    {
        return ERR;
    }
    coniocurses_x ++;
    return OK;
}

static int clrtoeol(void)
{
    if (! coniocurses_ready) return ERR;
    gotoxy(coniocurses_y + 1, coniocurses_x + 1);
    clreol();
    return OK;
}

static int clrtobot(void)
{
    int y;
    int ox, oy;

    if (! coniocurses_ready) return ERR;
    getyx(stdscr, oy, ox);
    clrtoeol();
    for (y = oy + 1; y < LINES; y ++)
    {
        move(y, 0);
        clrtoeol();
    }
    move(oy, ox);
    return OK;
}

static int addstr(const char *s)
{
    if (! coniocurses_ready) return ERR;
    while (*s)
    {
        coniocurses_addch(*s++);
    }
    return OK;
}

static int mvprintw(int y, int x, const char *s)
{
    if (! coniocurses_ready) return ERR;
    move(y, x);
    return addstr(s);
}
