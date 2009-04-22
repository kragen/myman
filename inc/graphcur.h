/*
 * graphcur.h - Open Watcom-style graph.h driver for the MyMan video game
 * Copyright 2007-2009, Benjamin C. Wiley Sittler <bsittler@gmail.com>
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

/* work-arounds for Open Watcom-style graph.h */
#ifdef GRAPHCURSES_GRAPH_H
#include GRAPHCURSES_GRAPH_H
#else
#include <graph.h>
#endif

#ifdef GRAPHCURSES_CONIO_H
#include GRAPHCURSES_CONIO_H
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
#define timeval graphcurses_timeval
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
#define gettimeofday graphcurses_gettimeofday
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
#define usleep graphcurses_usleep
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
#define wchar_t graphcur_wchar_t
typedef int graphcur_wchar_t;
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
#define wcwidth(ch) graphcurses_wcwidth(ch)

static int graphcurses_ready = 0;
static int graphcurses_color = 0;

typedef unsigned long int graphcurses_chtype;

#undef chtype
#define chtype graphcurses_chtype

typedef graphcurses_chtype graphcurses_attr_t;

#undef attr_t
#define attr_t graphcurses_attr_t

static int graphcurses_w = 0, graphcurses_h = 0;

static int graphcurses_x = 0, graphcurses_y = 0;

static graphcurses_attr_t graphcurses_attr = 0;

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

#define getyx(stdscr, y, x) ((y) = graphcurses_y), ((x) = graphcurses_x)

#define wrefresh(stdscr) refresh()

#define beep() graphcurses_addch('\a')

#define use_env(x)

#define echo()

#define can_change_color() (0)

#define keypad(stdscr, x)

#define LINES (graphcurses_h)

#define COLS (graphcurses_w)

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

#define GRAPHCURSES___A_COLOR 0xff000000UL

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

#define COLOR_PAIR(p) (((graphcurses_attr_t) p) << _PAIR_SHIFT)

#define PAIR_NUMBER(a) (((a) & GRAPHCURSES___A_COLOR) >> _PAIR_SHIFT)

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

static struct { short fg, bg; } graphcurses_pairs[COLOR_PAIRS];

static int graphcurses_wcwidth(graphcurses_chtype ch)
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
    return ((((ch) >= ' ') && ((ch) == (graphcurses_chtype) (unsigned long) (unsigned char) (char) ch)) ? 1 : 0);
}

static int curs_set(x)
{
    if (! graphcurses_ready) return ERR;
    _displaycursor(x ? _GCURSORON : _GCURSOROFF);
    return OK;
}

static int attrset(graphcurses_attr_t a)
{
    if (! graphcurses_ready) return ERR;
    graphcurses_attr = a;
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
    if (! graphcurses_ready) return ERR;
    return graphcurses_color;
}

static int start_color(void)
{
    if (! graphcurses_ready) return ERR;
    if (! has_colors()) return ERR;
    return OK;
}

static int init_pair(short i, short fg, short bg);

#define INITSCR_WITH_HINTS

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    int i;
    struct videoconfig vc;

    graphcurses_color = 1;
    graphcurses_w = 80;
    graphcurses_h = 25;
    graphcurses_color = 1;
    memset((void *) &vc, 0, sizeof(vc));
    if (_getvideoconfig(&vc)
        &&
        vc.numtextcols
        &&
        vc.numtextrows)
    {
        graphcurses_w = vc.numtextcols;
        graphcurses_h = vc.numtextrows;
        graphcurses_color = vc.numcolors > 2;
    }
    graphcurses_ready = 1;
    graphcurses_attr = -1;
    for (i = 0; i < COLOR_PAIRS; i ++)
    {
        graphcurses_pairs[i].fg = i ? (i % COLORS) : COLOR_WHITE;
        graphcurses_pairs[i].bg = i / COLORS;
    }
    attrset(0);
}

static void endwin(void)
{
    graphcurses_ready = 0;
    _settextcolor((COLOR_WHITE) | ((COLOR_BLACK) << 4));
    _displaycursor(_GCURSORON);
    _clearscreen(_GCLEARSCREEN);
}

static int move(int y, int x);

static int graphcurses_addch(graphcurses_chtype ch);

static int erase(void)
{
    if (! graphcurses_ready) return ERR;
    _clearscreen(_GCLEARSCREEN);
    _settextposition(1, 1);
    attrset(0);
    move(0, 0);
    return OK;
}

static int resizeterm(int y, int x)
{
    struct videoconfig vc;

    if (! graphcurses_ready) return ERR;
    memset((void *) &vc, 0, sizeof(vc));
    if (_getvideoconfig(&vc)
        &&
        vc.numtextcols
        &&
        vc.numtextrows)
    {
        graphcurses_w = vc.numtextcols;
        graphcurses_h = vc.numtextrows;
    }
    return OK;
}

static int graphcurses_getch(void) {
    struct videoconfig vc;
    int ret = ERR;
    int i, j;

    if (! graphcurses_ready) return (graphcurses_chtype) ERR;
    memset((void *) &vc, 0, sizeof(vc));
    if (_getvideoconfig(&vc)
        &&
        vc.numtextcols
        &&
        vc.numtextrows)
    {
        i = vc.numtextcols;
        j = vc.numtextrows;
    }
    if ((i != graphcurses_w) || (j != graphcurses_h))
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
#define getch graphcurses_getch

#undef addch
#define addch graphcurses_addch

static int refresh(void)
{
    if (! graphcurses_ready) return OK;
    _settextposition(graphcurses_y + 1, graphcurses_x + 1);
    return OK;
}

static int move(int y, int x)
{
    if (! graphcurses_ready) return ERR;
    if (y < 0) return ERR;
    if (y > graphcurses_h) return ERR;
    if (x < 0) return ERR;
    if (x > graphcurses_w) return ERR;
    graphcurses_y = y;
    graphcurses_x = x;
    return OK;
}

static int insch(const graphcurses_chtype ch)
{
    return ERR;
}

static int delch(void)
{
    return ERR;
}

static int init_pair(short i, short fg, short bg)
{
    if (! graphcurses_ready) return ERR;
    if (! has_colors()) return ERR;
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    if ((fg < 0) || (fg > COLORS)) return ERR;
    if ((bg < 0) || (bg > COLORS)) return ERR;
    graphcurses_pairs[i].fg = fg;
    graphcurses_pairs[i].bg = bg;
    return OK;
}

static int pair_content(short i, short *fg, short *bg)
{
    if (! graphcurses_ready) return ERR;
    if (! has_colors()) return ERR;
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    *fg = graphcurses_pairs[i].fg;
    *bg = graphcurses_pairs[i].bg;
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

static int graphcurses_addch(graphcurses_chtype ch)
{
    short fg, bg;

    if (! graphcurses_ready) return ERR;
    if (ch & ~A_CHARTEXT)
    {
        int ret;
        graphcurses_attr_t a;

        a = graphcurses_attr;
        attrset((PAIR_NUMBER(ch & ~A_CHARTEXT) ? (a & ~COLOR_PAIR(PAIR_NUMBER(a))) : a) | (ch & ~A_CHARTEXT));
        ret = graphcurses_addch(ch & A_CHARTEXT);
        attrset(a);
        return ret;
    }
    if (ch == (graphcurses_chtype) (unsigned long) (unsigned char) '\a')
    {
        /*sound_beep(440);*/
        return OK;
    }
    else if (ch == (graphcurses_chtype) (unsigned long) (unsigned char) '\b')
    {
        if (graphcurses_y && ! graphcurses_x)
        {
            move(graphcurses_y - 1, graphcurses_w - 1);
        }
        else if (graphcurses_x)
        {
            move(graphcurses_y, graphcurses_x - 1);
        }
        return OK;
    }
    else if (ch == (graphcurses_chtype) (unsigned long) (unsigned char) '\n')
    {
        if ((graphcurses_y + 1) < graphcurses_h)
        {
            move(graphcurses_y + 1, 0);
        }
        else
        {
            move(graphcurses_y, 0);
        }
        return OK;
    }
    else if (ch == (graphcurses_chtype) (unsigned long) (unsigned char) '\t')
    {
        do
        {
            graphcurses_addch((graphcurses_chtype) (unsigned long) (unsigned char) ' ');
        }
        while (graphcurses_x % 8);
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
        pair_content(PAIR_NUMBER(graphcurses_attr), &fg, &bg);
    }
    if (graphcurses_attr & A_UNDERLINE)
    {
        fg = COLOR_BLUE;
        bg = COLOR_BLACK;
    }
    else if ((graphcurses_attr & A_STANDOUT) && has_colors())
    {
        fg = COLOR_MAGENTA;
        bg = COLOR_BLACK;
    }
    else if ((graphcurses_attr & A_BOLD) && has_colors())
    {
        fg = COLOR_YELLOW | PEN_BRIGHT;
        bg = COLOR_BLACK;
    }
    if (has_colors() && PAIR_NUMBER(graphcurses_attr))
    {
        pair_content(PAIR_NUMBER(graphcurses_attr), &fg, &bg);
    }
    if (graphcurses_attr & A_BOLD)
    {
        fg |= PEN_BRIGHT;
    }
    if (graphcurses_attr & (A_STANDOUT | A_REVERSE))
    {
        short tmp;

        tmp = bg;
        bg = fg;
        fg = tmp;
    }
    if (graphcurses_x < 0) graphcurses_x = 0;
    if (graphcurses_x >= graphcurses_w)
    {
        graphcurses_x = 0;
        graphcurses_y ++;
    }
    if (graphcurses_y < 0) graphcurses_y = 0;
    if (graphcurses_y >= graphcurses_h)
    {
        graphcurses_y = graphcurses_h - 1;
    }
    _settextposition(graphcurses_y + 1, graphcurses_x + 1);
    _settextcolor((fg) | ((bg) << 4));
    if (((graphcurses_y + 1) < graphcurses_h)
        ||
        ((graphcurses_x + 1) < graphcurses_w))
    {
        char buf[2];

        buf[0] = ch;
        buf[1] = '\0';
        _outtext(buf);
    }
    else
    {
        return ERR;
    }
    graphcurses_x ++;
    return OK;
}

static int clrtoeol(void)
{
    int x;
    int ox, oy;

    if (! graphcurses_ready) return ERR;
    getyx(stdscr, oy, ox);
    for (x = ox; x < COLS; x ++)
    {
        move(oy, x);
        addch(' ');
    }
    move(oy, ox);
    return OK;
}

static int clrtobot(void)
{
    int y;
    int ox, oy;

    if (! graphcurses_ready) return ERR;
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
    if (! graphcurses_ready) return ERR;
    while (*s)
    {
        graphcurses_addch(*s++);
    }
    return OK;
}

static int mvprintw(int y, int x, const char *s)
{
    if (! graphcurses_ready) return ERR;
    move(y, x);
    return addstr(s);
}
