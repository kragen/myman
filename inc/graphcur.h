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

/* To build a 16-bit DOS executable I used these compiler flags:
 *
 * CC='owcc -bDOS -zm -mtune=i086 -O3 -mcmodel=h -DGRAPHCURSES'
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

#endif /* defined(WIN32) */

#if HAVE_WINSOCK_H
#include <winsock.h>
#endif

#if defined(__MSDOS__)
#include <dos.h>
#include <i86.h>

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

#if defined(WIN32) || defined(WIN16)

#undef usleep

/* originally from http://wyw.dcweb.cn/sleep.h.txt by Wu Yongwei */

#define usleep(t) Sleep((t) / 1000)

#else /* ! (defined(WIN32) || defined(WIN16)) */

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

#endif /* ! (defined(WIN32) || defined(WIN16)) */

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

typedef unsigned long int graphcurses_chtype;

#undef chtype
#define chtype graphcurses_chtype

typedef graphcurses_chtype graphcurses_attr_t;

#undef attr_t
#define attr_t graphcurses_attr_t

static int graphcurses_w = 0, graphcurses_h = 0;

static int graphcurses_x = 0, graphcurses_y = 0;

static int graphcurses_bitmap = 0;

static int graphcurses_bitmap_count = 0;
static int graphcurses_bitmap_x = 0;
static int graphcurses_bitmap_y = 0;
static int graphcurses_bitmap_fg = 0;

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

#define can_change_color() (graphcurses_ccc)

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

#define GRAPHCURSES_MAXCOLORS 16

#define COLORS 16

#define COLOR_BLACK 0

#define COLOR_BLUE 1

#define COLOR_GREEN 2

#define COLOR_CYAN (COLOR_BLUE | COLOR_GREEN)

#define COLOR_RED 4

#define COLOR_MAGENTA (COLOR_RED | COLOR_BLUE)

#define COLOR_YELLOW (COLOR_RED | COLOR_GREEN)

#define COLOR_WHITE (COLOR_RED | COLOR_GREEN | COLOR_BLUE)

#define GRAPHCURSES_COLOR_BRIGHT 8

#define GRAPHCURSES_COLOR_BLINK 16

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

#define GRAPHCURSES_CGA_MODE_CONTROL_PORT 0x03d8
#define GRAPHCURSES_CGA_MODE_BLINK (1 << 5)
#define GRAPHCURSES_CGA_MODE_HIGHRESGFX (1 << 4)
#define GRAPHCURSES_CGA_MODE_ENABLED (1 << 3)
#define GRAPHCURSES_CGA_MODE_RGB (1 << 2)
#define GRAPHCURSES_CGA_MODE_BITMAPGFX (1 << 1)
#define GRAPHCURSES_CGA_MODE_HIGHRES 1

static int graphcurses_ready = 0;
static int graphcurses_colors = GRAPHCURSES_MAXCOLORS;
static int graphcurses_has_color = 1;
static int graphcurses_ccc = 1;
static int graphcurses_brightbg = 1;
static int graphcurses_orig_rows = 0;
static int graphcurses_mode = _DEFAULTMODE;
static int graphcurses_orig_mode = _DEFAULTMODE;
static int graphcurses_orig_cga_mode = -1;
static short graphcurses_whichpalette = -1;
static short graphcurses_orig_whichpalette = -1;
static int graphcurses_textmode = 1;
static short graphcurses_orig_text_fg = COLOR_WHITE;
static short graphcurses_orig_fg = _WHITE;
static long graphcurses_orig_bk = _BLACK;
static short graphcurses_fg = -1;
static short graphcurses_bg = -1;

static struct { short fg, bg; } graphcurses_pairs[COLOR_PAIRS];

static long graphcurses_rgb[GRAPHCURSES_MAXCOLORS];
static long graphcurses_orig_rgb[GRAPHCURSES_MAXCOLORS];

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
    return (graphcurses_has_color && (graphcurses_colors > 2));
}

static int start_color(void)
{
    if (! graphcurses_ready) return ERR;
    if (! has_colors()) return ERR;
    return OK;
}

static int init_pair(short i, short fg, short bg);

#define GRAPHCURSES_COLOR_TO_RGB(c) \
           (((c) == COLOR_BLACK) ? _BLACK \
            : ((c) == COLOR_BLUE) ? _BLUE \
            : ((c) == COLOR_GREEN) ? _GREEN \
            : ((c) == COLOR_CYAN) ? _CYAN \
            : ((c) == COLOR_RED) ? _RED \
            : ((c) == COLOR_MAGENTA) ? _MAGENTA \
            : ((c) == COLOR_YELLOW) ? _BROWN \
            : ((c) == COLOR_WHITE) ? _WHITE \
            : ((c) == (GRAPHCURSES_COLOR_BRIGHT | COLOR_BLACK)) ? _GRAY \
            : ((c) == (GRAPHCURSES_COLOR_BRIGHT | COLOR_BLUE)) ? _LIGHTBLUE \
            : ((c) == (GRAPHCURSES_COLOR_BRIGHT | COLOR_GREEN)) ? _LIGHTGREEN \
            : ((c) == (GRAPHCURSES_COLOR_BRIGHT | COLOR_CYAN)) ? _LIGHTCYAN \
            : ((c) == (GRAPHCURSES_COLOR_BRIGHT | COLOR_RED)) ? _LIGHTRED \
            : ((c) == (GRAPHCURSES_COLOR_BRIGHT | COLOR_MAGENTA)) ? _LIGHTMAGENTA \
            : ((c) == (GRAPHCURSES_COLOR_BRIGHT | COLOR_YELLOW)) ? _YELLOW \
            : ((c) == (GRAPHCURSES_COLOR_BRIGHT | COLOR_WHITE)) ? _BRIGHTWHITE \
            : _WHITE)

#define INITSCR_WITH_HINTS

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    int i;
    struct videoconfig vc;
    short rows;
    char *ignored;

    graphcurses_bitmap_count = 0;
    graphcurses_bitmap_x = 0;
    graphcurses_bitmap_y = 0;
    graphcurses_bitmap_fg = 0;
    graphcurses_mode = _DEFAULTMODE;
    rows = 0;
    _getvideoconfig(&vc);
    graphcurses_orig_rows = 0;
    graphcurses_orig_mode = _DEFAULTMODE;
    if (_grstatus() == _GROK)
    {
        graphcurses_orig_mode = vc.mode;
        if (vc.numtextrows > 0)
        {
            graphcurses_orig_rows = vc.numtextrows;
            rows = vc.numtextrows;
        }
    }
    graphcurses_whichpalette = -1;
    graphcurses_orig_whichpalette = -1;
    graphcurses_bitmap = 0;
    if (myman_getenv("GRAPHCURSES_BITMAP")
        &&
        *myman_getenv("GRAPHCURSES_BITMAP"))
    {
        graphcurses_bitmap = strcmp(myman_getenv("GRAPHCURSES_BITMAP"), "0");
        graphcurses_mode = _MAXCOLORMODE;
    }
    if (myman_getenv("GRAPHCURSES_MODE")
        &&
        *myman_getenv("GRAPHCURSES_MODE"))
    {
        graphcurses_mode = (short) strtol(myman_getenv("GRAPHCURSES_MODE"), &ignored, 0);
    }
    if (myman_getenv("GRAPHCURSES_ROWS")
        &&
        *myman_getenv("GRAPHCURSES_ROWS"))
    {
        rows = (short) strtol(myman_getenv("GRAPHCURSES_ROWS"), &ignored, 0);
    }
    if (myman_getenv("GRAPHCURSES_PALETTE")
        &&
        *myman_getenv("GRAPHCURSES_PALETTE"))
    {
        graphcurses_whichpalette = (short) strtol(myman_getenv("GRAPHCURSES_PALETTE"), &ignored, 0);
    }
    graphcurses_ccc = 1;
    if (myman_getenv("GRAPHCURSES_CCC")
        &&
        *myman_getenv("GRAPHCURSES_CCC"))
    {
        graphcurses_ccc = (short) strtol(myman_getenv("GRAPHCURSES_CCC"), &ignored, 0);
    }
    graphcurses_brightbg = 1;
    if (myman_getenv("GRAPHCURSES_BRIGHTBG")
        &&
        *myman_getenv("GRAPHCURSES_BRIGHTBG"))
    {
        graphcurses_brightbg = strcmp(myman_getenv("GRAPHCURSES_BRIGHTBG"), "0");
    }
    graphcurses_has_color = 1;
    if (myman_getenv("GRAPHCURSES_COLOR")
        &&
        *myman_getenv("GRAPHCURSES_COLOR"))
    {
        graphcurses_has_color = strcmp(myman_getenv("GRAPHCURSES_CCC"), "0");
    }
    graphcurses_orig_text_fg = _gettextcolor();
    graphcurses_orig_fg = _getcolor();
    graphcurses_orig_bk = _getbkcolor();
    graphcurses_colors = 16;
    graphcurses_textmode = 1;
    graphcurses_w = 80;
    graphcurses_h = 25;
    if (rows)
    {
        graphcurses_h = _setvideomoderows(graphcurses_mode, rows);
        if (! graphcurses_h)
        {
            _setvideomode(graphcurses_mode);
            graphcurses_h = _settextrows(rows);
            if (! graphcurses_h)
            {
                graphcurses_h = 25;
                _setvideomode(graphcurses_mode);
            }
        }
    }
    else
    {
        _setvideomode(graphcurses_mode);
    }
    if (_grstatus() != _GROK)
    {
        graphcurses_mode = _DEFAULTMODE;
        graphcurses_h = 25;
        if (rows)
        {
            _settextrows(rows);
            if (_grstatus() == _GROK)
            {
                graphcurses_h = rows;
            }
        }
    }
    else if (rows)
    {
        graphcurses_h = rows;
    }
    memset((void *) &vc, 0, sizeof(vc));
    vc.numtextcols = graphcurses_w;
    vc.numtextrows = graphcurses_h;
    vc.numcolors = graphcurses_colors;
    vc.mode = graphcurses_mode;
    vc.adapter = _UNKNOWN;
    vc.monitor = _UNKNOWN;
    vc.numxpixels = 0;
    vc.numypixels = 0;
    _getvideoconfig(&vc);
    graphcurses_mode = vc.mode;
    if (vc.numtextcols > 0) graphcurses_w = vc.numtextcols;
    if (vc.numtextrows > 0) graphcurses_h = vc.numtextrows;
    if (vc.numcolors > 0) graphcurses_colors = vc.numcolors;
    if (graphcurses_colors > GRAPHCURSES_MAXCOLORS) graphcurses_colors = GRAPHCURSES_MAXCOLORS;
    if ((vc.adapter == _MDPA)
        || (vc.adapter == _HERCULES)
        || (vc.monitor == _MONO)
        || (vc.monitor == _ANALOGMONO)
        || (graphcurses_mode == _MRESNOCOLOR)
        || (graphcurses_mode == _HRESBW)
        || (graphcurses_mode == _TEXTMONO)
        || (graphcurses_mode == _HERCMONO)
        || (graphcurses_mode == _ERESNOCOLOR)
        || (graphcurses_mode == _VRES2COLOR))
    {
        graphcurses_has_color = 0;
        graphcurses_brightbg = 0;
    }
    graphcurses_orig_cga_mode = -1;
    if (vc.numxpixels && vc.numypixels)
    {
        graphcurses_textmode = 0;
        graphcurses_brightbg = 0;
        if (graphcurses_bitmap)
        {
            graphcurses_w = vc.numxpixels;
            graphcurses_h = vc.numypixels;
        }
    }
    else
    {
#ifdef __MSDOS__
        if (graphcurses_brightbg
            &&
            (vc.adapter == _CGA)
            &&
            ((vc.numcolors == 16)
             ||
             (vc.numcolors == 32))
            &&
            graphcurses_has_color)
        {
            graphcurses_orig_cga_mode = (((vc.numtextcols != 40) ? GRAPHCURSES_CGA_MODE_HIGHRES : 0)
                                         | (((vc.mode == _TEXTBW40) || (vc.mode == _TEXTBW80)) ? GRAPHCURSES_CGA_MODE_RGB : 0)
                                         | GRAPHCURSES_CGA_MODE_BLINK);
            outp(GRAPHCURSES_CGA_MODE_CONTROL_PORT,
                 graphcurses_orig_cga_mode
                 & ~GRAPHCURSES_CGA_MODE_BLINK);
        }
        else if (graphcurses_brightbg
                 &&
                 ((vc.adapter == _EGA)
                  || (vc.adapter == _VGA)
                  || (vc.adapter == _MCGA)
                  || (vc.adapter == _SVGA))
                 &&
                 ((vc.numcolors == 16)
                  ||
                  (vc.numcolors == 32))
                 &&
                 graphcurses_has_color)
        {
            union REGPACK regs;

            memset((void *) &regs, 0, sizeof(regs));
            regs.h.ah = 0x10; /* Function 10h: Select Colors in EGA/VGA */
            regs.h.al = 3; /* Subfunction 3h: BL = enable/disable blinking */
            regs.h.bl = 0; /* BL: 0 = bright background, 1 = blinking */
            intr(0x10, &regs);
        }
        else
#endif /* defined(__MSDOS__) */
        {
            graphcurses_brightbg = 0;
        }
        graphcurses_bitmap = 0;
    }
    if (graphcurses_whichpalette != -1)
    {
        graphcurses_orig_whichpalette = _selectpalette(graphcurses_whichpalette);
    }
    graphcurses_ready = 1;
    graphcurses_attr = -1;
    graphcurses_fg = -1;
    graphcurses_bg = -1;
    for (i = 0; i < COLOR_PAIRS; i ++)
    {
        graphcurses_pairs[i].fg = i ? (i % COLORS) : COLOR_WHITE;
        graphcurses_pairs[i].bg = i / COLORS;
    }
    if (graphcurses_ccc)
    {
        graphcurses_ccc = has_colors() && (graphcurses_colors >= 8);
    }
    for (i = 0; i < GRAPHCURSES_MAXCOLORS; i ++)
    {
        graphcurses_orig_rgb[i] = -1;
        graphcurses_rgb[i] = GRAPHCURSES_COLOR_TO_RGB(i);
        if (can_change_color() && (i < graphcurses_colors))
        {
            graphcurses_orig_rgb[i] = _remappalette(i, graphcurses_rgb[i]);
            if (graphcurses_orig_rgb[i] == -1)
            {
                graphcurses_ccc = 0;
            }
        }
    }
    attrset(0);
    _setcolor(COLOR_WHITE);
    _setbkcolor(_BLACK);
    _settextcolor(COLOR_WHITE);
}

static void endwin(void)
{
    int i;

#ifdef __MSDOS__
    if (graphcurses_brightbg && (graphcurses_orig_cga_mode != -1))
    {
        outp(GRAPHCURSES_CGA_MODE_CONTROL_PORT,
             graphcurses_orig_cga_mode);
    }
    else if (graphcurses_brightbg)
    {
        union REGPACK regs;

        memset((void *) &regs, 0, sizeof(regs));
        regs.h.ah = 0x10; /* Select Colors in EGA/VGA */
        regs.h.al = 0x03; /* BL = enable/disable blinking */
        regs.h.bl = 1; /* 0 = bright background, 1 = blinking */
        intr(0x10, &regs);
    }
#endif /* defined(__MSDOS__) */
    for (i = 0; i < GRAPHCURSES_MAXCOLORS; i ++)
    {
        graphcurses_rgb[i] = GRAPHCURSES_COLOR_TO_RGB(i);
        if (can_change_color() && (i < graphcurses_colors)) _remappalette(i, graphcurses_orig_rgb[i]);
    }
    if (graphcurses_whichpalette != -1)
    {
        _selectpalette(graphcurses_orig_whichpalette);
    }
    graphcurses_whichpalette = 0;
    graphcurses_bitmap = 0;
    graphcurses_ready = 0;
    if (graphcurses_orig_rows)
    {
        if (! _setvideomoderows(graphcurses_orig_mode, graphcurses_orig_rows))
        {
            _setvideomode(graphcurses_orig_mode);
            _settextrows(graphcurses_orig_rows);
        }
    }
    else
    {
        _setvideomode(_DEFAULTMODE);
    }
    _setcolor(graphcurses_orig_fg);
    _setbkcolor(graphcurses_orig_bk);
    _settextcolor(graphcurses_orig_text_fg);
    _displaycursor(_GCURSORON);
    _clearscreen(_GCLEARSCREEN);
}

static int move(int y, int x);

static int graphcurses_addch(graphcurses_chtype ch);

static int erase(void)
{
    short text_fg;
    long fg, bk;

    if (! graphcurses_ready) return ERR;
    graphcurses_bitmap_count = 0;
    graphcurses_bitmap_x = 0;
    graphcurses_bitmap_y = 0;
    graphcurses_bitmap_fg = 0;
    text_fg = _gettextcolor();
    fg = _getcolor();
    bk = _getbkcolor();
    _setcolor(COLOR_WHITE);
    _setbkcolor(_BLACK);
    _settextcolor(COLOR_WHITE);
    _clearscreen(_GCLEARSCREEN);
    _setcolor(fg);
    _setbkcolor(bk);
    _settextcolor(text_fg);
    _settextposition(1, 1);
    attrset(0);
    move(0, 0);
    return OK;
}

static int resizeterm(int y, int x)
{
    if (! graphcurses_ready) return ERR;
    return OK;
}

static int graphcurses_getch(void) {
    int ret = ERR;

    if (! graphcurses_ready) return (graphcurses_chtype) ERR;
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
    if (graphcurses_bitmap && graphcurses_bitmap_count)
    {
        if (graphcurses_fg != graphcurses_bitmap_fg)
        {
            _setcolor(graphcurses_bitmap_fg);
            graphcurses_fg = graphcurses_bitmap_fg;
        }
        if (graphcurses_bitmap_count == 1)
        {
            _setpixel(graphcurses_bitmap_x, graphcurses_bitmap_y);
        }
        else
        {
            _rectangle(_GFILLINTERIOR,
                       graphcurses_bitmap_x, graphcurses_bitmap_y,
                       graphcurses_bitmap_x + graphcurses_bitmap_count - 1, graphcurses_bitmap_y);
        }
        graphcurses_bitmap_count = 0;
    }
    if (! graphcurses_bitmap)
    {
        _settextposition(graphcurses_y + 1, graphcurses_x + 1);
    }
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
    long pal;

    if (! graphcurses_ready) return ERR;
    if (! has_colors()) return ERR;
    if (! can_change_color()) return ERR;
    if ((i < 0) || (i > graphcurses_colors)) return ERR;
    graphcurses_rgb[i] = (0x3fL * r / 1000L) + ((0x3fL * g / 1000L) << 8) + ((0x3fL * b / 1000L) << 16);
    _remappalette(i, graphcurses_rgb[i]);
    return ERR;
}

static int color_content(short i, short *r, short *g, short *b)
{
    if (! graphcurses_ready) return ERR;
    if (! has_colors()) return ERR;
    if (! can_change_color()) return ERR;
    if ((i < 0) || (i > graphcurses_colors)) return ERR;
    *r = (short) (1000L * (graphcurses_rgb[i] & 0x3fL) / 0x3fL);
    *g = (short) (1000L * ((graphcurses_rgb[i] >> 8) & 0x3fL) / 0x3fL);
    *b = (short) (1000L * ((graphcurses_rgb[i] >> 16) & 0x3fL) / 0x3fL);
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
        fg = COLOR_YELLOW | GRAPHCURSES_COLOR_BRIGHT;
        bg = COLOR_BLACK;
    }
    if (has_colors() && PAIR_NUMBER(graphcurses_attr))
    {
        pair_content(PAIR_NUMBER(graphcurses_attr), &fg, &bg);
    }
    if (graphcurses_attr & A_BOLD)
    {
        fg |= GRAPHCURSES_COLOR_BRIGHT;
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
    if (ch == ACS_BLOCK)
    {
        bg = COLOR_BLACK;
    }
    if ((bg != COLOR_BLACK) && (! graphcurses_textmode) && (! graphcurses_bitmap))
    {
        switch (ch)
        {
        case 0:
        case ' ':
        case 0xff:
            if (bg)
            {
                fg = bg;
                ch = ACS_BLOCK;
            }
        }
        if (fg == COLOR_BLACK) fg = bg;
        bg = COLOR_BLACK;
    }
    if (graphcurses_bitmap)
    {
        switch (ch)
        {
        case 0:
        case ' ':
        case 0xff:
            fg = bg;
            break;
        case '#':
        case ACS_BLOCK:
            break;
        default:
            if (bg != COLOR_BLACK) fg = bg;
        }
        ch = ACS_BLOCK;
    }
    if (ch == ACS_BLOCK)
    {
        bg = COLOR_BLACK;
    }
    if (graphcurses_mode == _TEXTMONO)
    {
        if (fg == (GRAPHCURSES_COLOR_BRIGHT | COLOR_BLACK)) fg = COLOR_WHITE;
        if (bg == (GRAPHCURSES_COLOR_BRIGHT | COLOR_BLACK)) bg = COLOR_WHITE;
        if (fg) fg = (fg & GRAPHCURSES_COLOR_BRIGHT) ? (COLOR_WHITE | GRAPHCURSES_COLOR_BRIGHT) : COLOR_YELLOW;
        if (bg) bg = (bg & GRAPHCURSES_COLOR_BRIGHT) ? (COLOR_WHITE | GRAPHCURSES_COLOR_BRIGHT) : COLOR_YELLOW;
    }
    else if ((graphcurses_mode == _MRES4COLOR)
             || (graphcurses_mode == _MRESNOCOLOR))
    {
        if ((fg != COLOR_BLACK)
            && (ch == ACS_BLOCK)
            && ((fg == (GRAPHCURSES_COLOR_BRIGHT | COLOR_BLACK))
                || (! (fg & GRAPHCURSES_COLOR_BRIGHT)))) ch = 0xb1;
        if (((fg & ~GRAPHCURSES_COLOR_BRIGHT) == COLOR_RED)
            ||
            ((fg & ~GRAPHCURSES_COLOR_BRIGHT) == COLOR_GREEN)
            ||
            ((fg & ~GRAPHCURSES_COLOR_BRIGHT) == COLOR_BLUE)
            ||
            (fg == (GRAPHCURSES_COLOR_BRIGHT | COLOR_YELLOW))
            ||
            (fg == (GRAPHCURSES_COLOR_BRIGHT | COLOR_BLACK)))
        {
            if (ch == ACS_BLOCK) ch = 0xb2;
            else if (ch == 0xb1) ch = 0xb0;
        }
        if (fg == COLOR_WHITE) fg = COLOR_MAGENTA;
        if (fg == (GRAPHCURSES_COLOR_BRIGHT | COLOR_BLACK)) fg = COLOR_WHITE;
        if (bg == (GRAPHCURSES_COLOR_BRIGHT | COLOR_BLACK)) bg = COLOR_WHITE;
        fg &= ~GRAPHCURSES_COLOR_BRIGHT;
        bg &= ~GRAPHCURSES_COLOR_BRIGHT;
        fg = (fg == COLOR_MAGENTA) ? 2 : (((fg >> 1) | (fg & 1)) % 4);
        bg = (bg == COLOR_MAGENTA) ? 2 : (((bg >> 1) | (bg & 1)) % 4);
    }
    else if (graphcurses_textmode && (! graphcurses_brightbg) && (graphcurses_colors == 16))
    {
        if (bg == (GRAPHCURSES_COLOR_BRIGHT | COLOR_BLACK))
        {
            bg = COLOR_WHITE;
        }
    }
    if (fg >= graphcurses_colors)
    {
        if (fg == (GRAPHCURSES_COLOR_BRIGHT | COLOR_BLACK)) fg = COLOR_WHITE;
        if (fg & GRAPHCURSES_COLOR_BRIGHT)
        {
            fg &= ~GRAPHCURSES_COLOR_BRIGHT;
        }
    }
    if (fg >= graphcurses_colors)
    {
        fg = (fg == COLOR_WHITE) ? (graphcurses_colors - 1) : fg ? ((fg - 1) % (graphcurses_colors - 1) + 1) : COLOR_BLACK;
    }
    if (fg == bg)
    {
        switch (ch)
        {
        case 0:
        case ' ':
        case 0xff:
        case ACS_BLOCK:
            break;
        default:
            fg = bg ? COLOR_BLACK : (COLOR_WHITE | GRAPHCURSES_COLOR_BRIGHT);
        }
    }
    if (graphcurses_bitmap)
    {
        if (graphcurses_bitmap_count
            &&
            (graphcurses_x == (graphcurses_bitmap_x + graphcurses_bitmap_count))
            &&
            (graphcurses_y == graphcurses_bitmap_y)
            &&
            (fg == graphcurses_bitmap_fg)
            &&
            ((graphcurses_y + 1) < graphcurses_h)
            &&
            ((graphcurses_x + 1) < graphcurses_w))
        {
            graphcurses_bitmap_count ++;
        }
        else
        {
            if (graphcurses_bitmap_count)
            {
                if (graphcurses_fg != graphcurses_bitmap_fg)
                {
                    _setcolor(graphcurses_bitmap_fg);
                    graphcurses_fg = graphcurses_bitmap_fg;
                }
                if (graphcurses_bitmap_count == 1)
                {
                    _setpixel(graphcurses_bitmap_x, graphcurses_bitmap_y);
                }
                else
                {
                    _rectangle(_GFILLINTERIOR,
                               graphcurses_bitmap_x, graphcurses_bitmap_y,
                               graphcurses_bitmap_x + graphcurses_bitmap_count - 1, graphcurses_bitmap_y);
                }
                graphcurses_bitmap_count = 0;
            }
            if (((graphcurses_y + 1) < graphcurses_h)
                &&
                ((graphcurses_x + 1) < graphcurses_w))
            {
                graphcurses_bitmap_count = 1;
                graphcurses_bitmap_x = graphcurses_x;
                graphcurses_bitmap_y = graphcurses_y;
                graphcurses_bitmap_fg = fg;
            }
        }
        graphcurses_x ++;
        return OK;
    }
    if (! graphcurses_bitmap)
    {
        _settextposition(graphcurses_y + 1, graphcurses_x + 1);
    }
    if (graphcurses_bitmap)
    {
        if (graphcurses_fg != fg)
        {
            _setcolor(fg);
            graphcurses_fg = fg;
        }
    }
    else
    {
        if (graphcurses_textmode && graphcurses_brightbg && (graphcurses_colors == 16) && (bg & GRAPHCURSES_COLOR_BRIGHT))
        {
            fg |= GRAPHCURSES_COLOR_BLINK;
        }
        if (graphcurses_fg != fg)
        {
            _settextcolor(fg);
            graphcurses_fg = fg;
        }
    }
    if (graphcurses_textmode)
    {
        if (graphcurses_bg != bg)
        {
            _setbkcolor(GRAPHCURSES_COLOR_TO_RGB(bg));
            graphcurses_bg = bg;
        }
    }
    if (((graphcurses_y + 1) < graphcurses_h)
        &&
        ((graphcurses_x + 1) < graphcurses_w))
    {
        if (graphcurses_bitmap)
        {
            _setpixel(graphcurses_x, graphcurses_y);
        }
        else
        {
            char buf[2];

            buf[0] = ch;
            buf[1] = '\0';
            _outmem(buf, 1);
        }
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
