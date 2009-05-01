/*
 * cacacurs.h - libcaca driver for the MyMan video game
 * Copyright 2007, Benjamin C. Wiley Sittler <bsittler@gmail.com>
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

#ifndef MYMAN_CACACURS_H_INCLUDED
#define MYMAN_CACACURS_H_INCLUDED 1

/* work-arounds for libcaca */

#ifdef CACACURSES0
#include <caca0.h>
#undef CACA_API_VERSION_1
#else
#include <caca.h>
#endif

#if defined(WIN32)

#include <windows.h>
#if HAVE_PSAPI_H
#include <psapi.h>
#endif
#if HAVE_SHLWAPI_H
#include <shlwapi.h>
#endif
#include <sys/stat.h>
#if HAVE_TLHELP32_H
#include <tlhelp32.h>
#endif

/* work-arounds for Win32 */

#ifndef HAVE_GETTIMEOFDAY
#define HAVE_GETTIMEOFDAY 0
#endif

#ifndef HAVE_USLEEP
#define HAVE_USLEEP 0
#endif

#else /* ! defined(WIN32) */

#if defined(__MSDOS__) || defined(CPM)

#ifdef __DJGPP__

#ifndef HAVE_GETTIMEOFDAY
#define HAVE_GETTIMEOFDAY 1
#endif

#ifndef HAVE_USLEEP
#define HAVE_USLEEP 1
#endif

#endif /* defined(__DJGPP__) */

#ifndef HAVE_GETTIMEOFDAY
#define HAVE_GETTIMEOFDAY 0
#endif

#ifndef HAVE_USLEEP
#define HAVE_USLEEP 0
#endif

#endif /* defined(__MSDOS__) || defined(CPM) */

#endif /* ! defined(WIN32) */

#ifndef HAVE_USLEEP
#ifdef macintosh
#define HAVE_USLEEP 0
#else
#define HAVE_USLEEP 1
#endif
#endif

#ifndef HAVE_GETTIMEOFDAY
#ifdef macintosh
#define HAVE_GETTIMEOFDAY 0
#else /* ! defined(macintosh) */
#define HAVE_GETTIMEOFDAY 1
#endif /* ! defined(macintosh) */
#endif /* ! defined(HAVE_GETTIMEOFDAY) */

/* for CHAR_BIT */
#include <limits.h>

/* for strcasecmp */
#include <strings.h>

/* for atof */
#include <stdlib.h>

#if ! HAVE_GETTIMEOFDAY

#undef gettimeofday
#define gettimeofday cacacurses_gettimeofday

static int cacacurses_gettimeofday(struct timeval *tv, void *tz)
{

    if (tv)
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

        static unsigned long
#ifndef __BCC__
            far
#endif
            *p;
        unsigned long ticks;

        if (!p) p = MK_FP(0, 0x46c);
        ticks = *p;
        tv->tv_usec = ((ticks & 0xff) * 55000L) % 1000000L;
        tv->tv_sec = (ticks & 0xff) * 55L / 1000L;

#else /* ! defined(__MSDOS__) */

#ifdef macintosh

        UnsignedWide microTickCount;
        double usecs;

        Microseconds(&microTickCount);
        usecs = (4294967296.0 * microTickCount.hi + microTickCount.lo);
        tv->tv_sec = (long) (usecs / 1e6);
        tv->tv_usec = (long) (usecs - (1e6 * tv->tv_sec));

#else /* ! defined(macintosh) */

#error no gettimeofday(2) implementation for your platform, sorry
        return 1;

#endif /* ! defined(macintosh) */

#endif /* ! defined(__MSDOS__) */

#endif /* ! defined(WIN32) */

    }
    return 0;
}

#endif /* ! HAVE_GETTIMEOFDAY */

#if ! HAVE_USLEEP

#if defined(WIN32)

#undef usleep

/* originally from http://wyw.dcweb.cn/sleep.h.txt by Wu Yongwei */

#define usleep(t) Sleep((t) / 1000)

#else /* ! defined(WIN32) */

#undef usleep
#define usleep cacacurses_usleep

static int
cacacurses_usleep(unsigned long usecs)
{
#ifdef macintosh
    EventRecord er;
    struct timeval tv, tv2;

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    myman_gettimeofday(&tv, NULL);
    tv2.tv_sec = tv.tv_sec + ((tv.tv_usec + usecs) / 1000000L);
    tv2.tv_usec = tv.tv_usec + ((tv.tv_usec + usecs) % 1000000L);
    while (1)
    {
        SpinCursor(32);
        if (WaitNextEvent(highLevelEventMask, &er, usecs / 60000, NULL))
        {
            if (er.what == kHighLevelEvent)
            {
                AEProcessAppleEvent(&er);
            }
            return -1;
        }
        myman_gettimeofday(&tv, NULL);
        if ((tv.tv_sec != tv2.tv_sec)
            ||
            (tv.tv_usec >= tv2.tv_usec))
        {
            break;
        }
        break;
    }
#else
    while (usecs)
    {
        struct timeval tv0, tv1;
        int ret;

        if (myman_gettimeofday(&tv0, NULL)) break;
        ret = 0;
#ifndef LSI_C
#if ! (defined(__DMC__) || defined(__TURBOC__))
        ret =
#endif
            sleep(0);
#endif
        if (ret) break;
        if (myman_gettimeofday(&tv1, NULL)) break;
        if (tv1.tv_sec < tv0.tv_sec) break;
        if ((tv1.tv_sec == tv0.tv_sec)
            &&
            (tv1.tv_usec < tv0.tv_usec))
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
#endif
    return 0;
}

#endif /* ! defined(WIN32) */

#endif /* ! HAVE_USLEEP */

#ifdef CACA_API_VERSION_1

/* libcaca 1.0+ */

#ifndef USE_PALETTE
#define USE_PALETTE 1
#endif

#ifndef SWAPDOTS
#define SWAPDOTS 1
#endif

#ifndef USE_SIGWINCH
#define USE_SIGWINCH 0
#endif

#ifndef USE_IOCTL
#define USE_IOCTL 0
#endif

static int cacacurses_ready = 0;

static char *cacacurses_bitmap;

static char *cacacurses_fixedpal;

static char *cacacurses_bitmap_double;

static char *cacacurses_bitmap_doubleheight;

typedef unsigned long int chtype;

typedef chtype attr_t;

static cucul_canvas_t *cv;

static caca_display_t *dp;

static int cacacurses_x, cacacurses_y, cacacurses_w, cacacurses_h;

static unsigned long int *cacacurses_framebuffer;

#define COLORS 16

#define COLOR_PAIRS (COLORS * COLORS)

static unsigned long int cacacurses_pal[COLORS], cacacurses_dimpal[COLORS];

static cucul_dither_t *cacacurses_dither;

static chtype pal[COLOR_PAIRS];

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
#define TRUE (1)
#endif

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

#define getyx(stdscr, y, x) ((y) = cacacurses_y), ((x) = cacacurses_x)

#define wrefresh(stdscr) refresh()

#define beep() addch('\7')

#define use_env(x)

#define echo()

#define curs_set(x)

#define keypad(stdscr, x)

#define can_change_color() (cacacurses_fixedpal == NULL)

#define has_colors() 1

#define start_color()

#define COLOR_PAIR(i) (pal[(i)])

#define LINES (cacacurses_bitmap ? (cacacurses_h / (cacacurses_bitmap_double ? 2 : 1) / (cacacurses_bitmap_doubleheight ? 2 : 1)) : cucul_get_canvas_height(cv))

#define COLS (cacacurses_bitmap ? (cacacurses_w / (cacacurses_bitmap_double ? 2 : 1)) : cucul_get_canvas_width(cv))

#define COLOR_BLACK CUCUL_BLACK

#define COLOR_BLUE CUCUL_BLUE

#define COLOR_GREEN CUCUL_GREEN

#define COLOR_CYAN CUCUL_CYAN

#define COLOR_RED CUCUL_RED

#define COLOR_MAGENTA CUCUL_MAGENTA

#define COLOR_YELLOW CUCUL_BROWN

#define COLOR_WHITE CUCUL_LIGHTGRAY

#define A_STANDOUT CUCUL_BLINK

#define A_BOLD CUCUL_BOLD

#define A_UNDERLINE CUCUL_UNDERLINE

#define A_REVERSE CUCUL_ITALICS

#define KEY_LEFT CACA_KEY_LEFT

#define KEY_RIGHT CACA_KEY_RIGHT

#define KEY_UP CACA_KEY_UP

#define KEY_DOWN CACA_KEY_DOWN

#define ACS_LRCORNER 0x2518

#define ACS_URCORNER 0x2510

#define ACS_ULCORNER 0x250c

#define ACS_LLCORNER 0x2514

#define ACS_PLUS 0x253c

#define ACS_HLINE 0x2500

#define ACS_LTEE 0x251c

#define ACS_RTEE 0x2524

#define ACS_BTEE 0x2534

#define ACS_TTEE 0x252c

#define ACS_VLINE 0x2502

#define ACS_BULLET 0x00b7

static int cacacurses_rotate;

#define INITSCR_WITH_HINTS

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    cacacurses_bitmap = myman_getenv("CACACURSES_BITMAP");
    if (cacacurses_bitmap && ! *cacacurses_bitmap) cacacurses_bitmap = NULL;
    cacacurses_fixedpal = myman_getenv("CACACURSES_FIXEDPAL");
    if (cacacurses_fixedpal && ! *cacacurses_fixedpal) cacacurses_fixedpal = NULL;
    cacacurses_bitmap_double = myman_getenv("CACACURSES_BITMAP_DOUBLE");
    if (cacacurses_bitmap_double && ! *cacacurses_bitmap_double) cacacurses_bitmap_double = NULL;
    cacacurses_bitmap_doubleheight = myman_getenv("CACACURSES_BITMAP_DOUBLEHEIGHT");
    if (cacacurses_bitmap_doubleheight && ! *cacacurses_bitmap_doubleheight) cacacurses_bitmap_doubleheight = NULL;
    if (cacacurses_bitmap)
    {
        cacacurses_w = w * (cacacurses_bitmap ? (cacacurses_bitmap_double ? 2 : 1) : 1);
        cacacurses_h = h * (cacacurses_bitmap ? ((cacacurses_bitmap_double ? 2 : 1) * (cacacurses_bitmap_doubleheight ? 2 : 1)) : 1);
    }
    cacacurses_rotate = 0;
    if ((! myman_getenv("CACA_DRIVER"))
        || strcasecmp(myman_getenv("CACA_DRIVER"), "x11")
        || (! myman_getenv("CACA_FONT"))
        || strcasecmp(myman_getenv("CACA_FONT"), "nil2"))
    {
        if (w > 128) w = 128;
        if (h > 50) h = 50;
    }
    cv = cucul_create_canvas(w * (cacacurses_bitmap ? (cacacurses_bitmap_double ? 2 : 1) : 1),
                             h * (cacacurses_bitmap ? ((cacacurses_bitmap_double ? 2 : 1) * (cacacurses_bitmap_doubleheight ? 2 : 1)) : 1));
    dp = caca_create_display(cv);
    if (! dp)
    {
        int goterror;

        goterror = errno;
        perror("caca_create_display");
        fflush(stderr);
        cucul_free_canvas(cv);
        if (goterror == ENODEV)
        {
            char const * const * drivers;

            fprintf(stderr, "Supported values for the CACA_DRIVER environment variable:\n");
            drivers = caca_get_display_driver_list();
            while (drivers && *drivers)
            {
                fprintf(stderr, "%s\t%s\n", drivers[0], drivers[1]);
                drivers += 2;
            }
            fflush(stderr);
        }
        exit(1);
    }
    cucul_put_char(cv, 0, 0, ' ');
    pal[0] = cucul_get_attr(cv, 0, 0);
    {
        int i;

        for (i = 0; i < COLORS; i ++)
        {
            cacacurses_pal[i] = (
                ((i & 4) ? ((i & 8) ? 0xff0000UL : 0x880000UL) : 0)
                |
                ((i & 2) ? ((i & 8) ? 0xff00UL : 0x8800UL) : 0)
                |
                ((i & 1) ? ((i & 8) ? 0xffUL : 0x88UL) : 0)
                |
                ((i == 8) ? 0x777777UL : 0)
                |
                ((i == 7) ? 0x222222UL : 0));
            cacacurses_dimpal[i] = (
                ((i & 4) ? ((i & 8) ? 0x770000UL : 0x440000UL) : 0)
                |
                ((i & 2) ? ((i & 8) ? 0x7700UL : 0x4400UL) : 0)
                |
                ((i & 1) ? ((i & 8) ? 0x77UL : 0x44UL) : 0)
                |
                ((i == 8) ? 0x333333UL : 0)
                |
                ((i == 7) ? 0x111111UL : 0));
        }
    }
    if (title) caca_set_display_title(dp, title);
    if (cacacurses_bitmap)
    {
        cacacurses_dither = cucul_create_dither(CHAR_BIT * sizeof(unsigned long int), cacacurses_w, cacacurses_h, cacacurses_w * sizeof(unsigned long int), 0xff0000UL, 0xff00UL, 0xffUL, 0UL);
        if (! cacacurses_dither)
        {
            perror("cucul_create_dither");
            fflush(stderr);
            caca_free_display(dp);
            cucul_free_canvas(cv);
            exit(1);
        }
        if (myman_getenv("CACACURSES_BITMAP_BRIGHTNESS"))
        {
            if (cucul_set_dither_brightness(cacacurses_dither, atof(myman_getenv("CACACURSES_BITMAP_BRIGHTNESS"))))
            {
                perror("cucul_set_dither_brightness: CACACURSES_BITMAP_BRIGHTNESS");
                fflush(stderr);
                cucul_free_dither(cacacurses_dither);
                caca_free_display(dp);
                cucul_free_canvas(cv);
                exit(1);
            }
        }
        if (myman_getenv("CACACURSES_BITMAP_GAMMA"))
        {
            if (cucul_set_dither_gamma(cacacurses_dither, atof(myman_getenv("CACACURSES_BITMAP_GAMMA"))))
            {
                perror("cucul_set_dither_gamma: CACACURSES_BITMAP_GAMMA");
                fflush(stderr);
                cucul_free_dither(cacacurses_dither);
                caca_free_display(dp);
                cucul_free_canvas(cv);
                exit(1);
            }
        }
        if (myman_getenv("CACACURSES_BITMAP_CONTRAST"))
        {
            if (cucul_set_dither_contrast(cacacurses_dither, atof(myman_getenv("CACACURSES_BITMAP_CONTRAST"))))
            {
                perror("cucul_set_dither_contrast: CACACURSES_BITMAP_CONTRAST");
                fflush(stderr);
                cucul_free_dither(cacacurses_dither);
                caca_free_display(dp);
                cucul_free_canvas(cv);
                exit(1);
            }
        }
        if (myman_getenv("CACACURSES_BITMAP_ANTIALIAS"))
        {
            if (cucul_set_dither_antialias(cacacurses_dither, myman_getenv("CACACURSES_BITMAP_ANTIALIAS")))
            {
                perror("cucul_set_dither_antialias: CACACURSES_BITMAP_ANTIALIAS");
                {
                    char const * const * antialias_list;

                    antialias_list = cucul_get_dither_antialias_list(cacacurses_dither);
                    fputs("Supported antialiasing modes:\n", stderr);
                    while((*antialias_list) && antialias_list[1])
                    {
                        fputs(*antialias_list, stderr);
                        fputc('\t', stderr);
                        fputs(antialias_list[1], stderr);
                        fputc('\n', stderr);
                        antialias_list += 2;
                    }
                }
                fflush(stderr);
                cucul_free_dither(cacacurses_dither);
                caca_free_display(dp);
                cucul_free_canvas(cv);
                exit(1);
            }
        }
        if (myman_getenv("CACACURSES_BITMAP_COLOR"))
        {
            if (cucul_set_dither_color(cacacurses_dither, myman_getenv("CACACURSES_BITMAP_COLOR")))
            {
                perror("cucul_set_dither_color: CACACURSES_BITMAP_COLOR");
                {
                    char const * const * color_list;

                    color_list = cucul_get_dither_color_list(cacacurses_dither);
                    fputs("Supported color modes:\n", stderr);
                    while((*color_list) && color_list[1])
                    {
                        fputs(*color_list, stderr);
                        fputc('\t', stderr);
                        fputs(color_list[1], stderr);
                        fputc('\n', stderr);
                        color_list += 2;
                    }
                }
                fflush(stderr);
                cucul_free_dither(cacacurses_dither);
                caca_free_display(dp);
                cucul_free_canvas(cv);
                exit(1);
            }
        }
        if (myman_getenv("CACACURSES_BITMAP_CHARSET"))
        {
            if (cucul_set_dither_charset(cacacurses_dither, myman_getenv("CACACURSES_BITMAP_CHARSET")))
            {
                perror("cucul_set_dither_charset: CACACURSES_BITMAP_CHARSET");
                {
                    char const * const * charset_list;

                    charset_list = cucul_get_dither_charset_list(cacacurses_dither);
                    fputs("Supported character sets:\n", stderr);
                    while((*charset_list) && charset_list[1])
                    {
                        fputs(*charset_list, stderr);
                        fputc('\t', stderr);
                        fputs(charset_list[1], stderr);
                        fputc('\n', stderr);
                        charset_list += 2;
                    }
                }
                fflush(stderr);
                cucul_free_dither(cacacurses_dither);
                caca_free_display(dp);
                cucul_free_canvas(cv);
                exit(1);
            }
        }
        if (myman_getenv("CACACURSES_BITMAP_ALGORITHM"))
        {
            if (cucul_set_dither_algorithm(cacacurses_dither, myman_getenv("CACACURSES_BITMAP_ALGORITHM")))
            {
                perror("cucul_set_dither_algorithm: CACACURSES_BITMAP_ALGORITHM");
                {
                    char const * const * mode_list;

                    mode_list = cucul_get_dither_algorithm_list(cacacurses_dither);
                    fputs("Supported dithering methods:\n", stderr);
                    while((*mode_list) && mode_list[1])
                    {
                        fputs(*mode_list, stderr);
                        fputc('\t', stderr);
                        fputs(mode_list[1], stderr);
                        fputc('\n', stderr);
                        mode_list += 2;
                    }
                }
                fflush(stderr);
                cucul_free_dither(cacacurses_dither);
                caca_free_display(dp);
                cucul_free_canvas(cv);
                exit(1);
            }
        }
        cacacurses_framebuffer = (unsigned long int *) malloc(cacacurses_w * cacacurses_h * sizeof(unsigned long int));
        if (! cacacurses_framebuffer)
        {
            perror("malloc");
            fflush(stderr);
            cucul_free_dither(cacacurses_dither);
            caca_free_display(dp);
            cucul_free_canvas(cv);
            exit(1);
        }
    }
    cacacurses_ready = 1;
}

static void endwin(void) {
    if (cacacurses_bitmap)
    {
        free((void *) cacacurses_framebuffer);
        cucul_free_dither(cacacurses_dither);
    }
    caca_free_display(dp);
    cucul_free_canvas(cv);
    cacacurses_ready = 0;
}

static int erase(void) {
    if (cacacurses_bitmap)
    {
        memset((void *) cacacurses_framebuffer, 0, cacacurses_w * cacacurses_h * sizeof(unsigned long int));
    }
    else
    {
        cucul_clear_canvas(cv);
    }
    cucul_gotoxy(cv, -1, -1);
    cacacurses_x = 0;
    cacacurses_y = 0;
    return OK;
}

static int refresh(void) {
    if (! cacacurses_ready) return OK;
    if (cacacurses_bitmap)
    {
        cucul_dither_bitmap(cv, 0, 0, cucul_get_canvas_width(cv), cucul_get_canvas_height(cv), cacacurses_dither, cacacurses_framebuffer);
    }
    switch (cacacurses_rotate)
    {
    case 1:
        cucul_flip(cv);
        break;
    case 2:
        cucul_flop(cv);
        break;
    case 3:
        cucul_rotate_180(cv);
        break;
    }
    caca_refresh_display(dp);
    return OK;
}

static chtype getch(void) {
    caca_event_t ev;

    if (caca_get_event(dp, CACA_EVENT_KEY_PRESS | CACA_EVENT_KEY_RELEASE | CACA_EVENT_QUIT, & ev, 0))
    {
        if (caca_get_event_type(& ev) == CACA_EVENT_QUIT) return 'Q';
        if (caca_get_event_type(& ev) == CACA_EVENT_KEY_PRESS) {
            if ((caca_get_event_key_ch(& ev) == '~') || (caca_get_event_key_ch(& ev) == '`')) cacacurses_rotate = (1 + cacacurses_rotate) % 4;
            switch (caca_get_event_key_ch(& ev))
            {
            case CACA_KEY_LEFT:
                if (cacacurses_rotate & 1) return CACA_KEY_RIGHT;
                break;
            case CACA_KEY_RIGHT:
                if (cacacurses_rotate & 1) return CACA_KEY_LEFT;
                break;
            case CACA_KEY_UP:
                if (cacacurses_rotate & 2) return CACA_KEY_DOWN;
                break;
            case CACA_KEY_DOWN:
                if (cacacurses_rotate & 2) return CACA_KEY_UP;
                break;
            }
            return caca_get_event_key_ch(& ev);
        }
    }
    return ERR;
}

static void standout() {
    cucul_set_color_ansi(cv, CUCUL_BLACK, CUCUL_WHITE);
    cucul_set_attr(cv, 0);
}

static void standend() {
    cucul_set_color_ansi(cv, CUCUL_LIGHTGRAY, CUCUL_BLACK);
    cucul_set_attr(cv, 0);
}

static int attrset(int a)
{
    unsigned char fg, bg;
    int a_cucul;

    a_cucul = (a & ~(A_STANDOUT | A_BOLD | A_UNDERLINE | A_REVERSE));
    if (a & (A_BOLD | A_STANDOUT)) a_cucul |= CUCUL_BOLD;
    if (a & A_UNDERLINE) a_cucul |= CUCUL_UNDERLINE;
    fg = ((a & A_BOLD) ? 8 : 0) | ((a & A_UNDERLINE) ? CUCUL_BLUE : CUCUL_LIGHTGRAY);
    bg = CUCUL_BLACK;
    if (a & (A_STANDOUT | A_REVERSE))
    {
        unsigned char tmp;

        tmp = fg;
        fg = bg;
        bg = tmp;
    }
    cucul_set_color_ansi(cv, fg, bg);
    cucul_set_attr(cv, 0);
    cucul_set_attr(cv, a_cucul);
    if (a & ~(A_BOLD | A_UNDERLINE | A_STANDOUT | A_REVERSE))
    {
        if (a & (A_STANDOUT | A_REVERSE))
        {
            fg = cucul_attr_to_ansi_fg(a_cucul);
            if (a_cucul & CUCUL_BOLD) fg |= 8;
            bg = cucul_attr_to_ansi_bg(a_cucul);
            cucul_set_color_ansi(cv, bg, fg);
            if (can_change_color())
            {
                cucul_set_color_argb(cv,
                                     0xf000u | cucul_attr_to_rgb12_bg(a_cucul),
                                     0xf000u | cucul_attr_to_rgb12_fg(a_cucul));
            }
            a_cucul = cucul_get_attr(cv, -1, -1);
        }
        else
        {
            fg = cucul_attr_to_ansi_fg(a_cucul);
            if (a_cucul & CUCUL_BOLD) fg |= 8;
            bg = cucul_attr_to_ansi_bg(a_cucul);
            cucul_set_color_ansi(cv, fg, bg);
            if (can_change_color())
            {
                cucul_set_color_argb(cv,
                                     0xf000u | cucul_attr_to_rgb12_fg(a_cucul),
                                     0xf000u | cucul_attr_to_rgb12_bg(a_cucul));
            }
            a_cucul = cucul_get_attr(cv, -1, -1);
        }
    }
    return cucul_set_attr(cv, a_cucul);
}

static int move(y, x) {
    cacacurses_y = y;
    cacacurses_x = x;
    return OK;
}

static int insch(const chtype ch)
{
    return ERR;
}

static int delch(void)
{
    return ERR;
}

static int cacacurses_wcwidth(const chtype ch) {
    return ((ch < 0x20) ? 0 : cucul_utf32_is_fullwidth(ch) ? 2 : 1);
}

#undef wcwidth
#define wcwidth cacacurses_wcwidth

static unsigned long cacacurses_rgb12_to_pixel(unsigned short rgb12)
{
    return (((unsigned long) (rgb12 & 0xf00u)) * 0x1100ul
            +
            ((unsigned long) (rgb12 & 0xf0u)) * 0x110ul
            +
            ((unsigned long) (rgb12 & 0xfu)) * 0x11ul);
}

static int addch(const chtype ch) {
    if (cacacurses_bitmap)
    {
        unsigned long attr;

        attr = cucul_get_attr(cv, -1, -1);
        {
            unsigned char fg, bg;
            unsigned long fgpixel, bgpixel, halfpixel;
            unsigned long outpixel, outpixel2, outpixel3, outpixel4;
            int charwidth, charheight;

            fg = 7;
            bg = 0;
            if (attr & A_UNDERLINE)
            {
                fg = 4;
            }
            if (attr & A_BOLD)
            {
                fg |= 8;
            }
            if (cucul_attr_to_ansi(attr) & 0xf)
            {
                fg = (cucul_attr_to_ansi(attr) & 0xf) | ((attr & A_BOLD) ? 8 : 0);
            }
            if ((cucul_attr_to_ansi(attr) >> 4) & 0xf)
            {
                bg = (cucul_attr_to_ansi(attr) >> 4) & 0xf;
            }
            fgpixel = cacacurses_pal[fg];
            halfpixel = cacacurses_dimpal[fg] ? cacacurses_dimpal[fg] : cacacurses_dimpal[bg];
            bgpixel = cacacurses_pal[bg];
            if (can_change_color())
            {
                bgpixel = cacacurses_rgb12_to_pixel(cucul_attr_to_rgb12_bg(attr));
                fgpixel = cacacurses_rgb12_to_pixel(cucul_attr_to_rgb12_fg(attr));
                /* FIXME: do gamma correction here, possibly... */
                halfpixel = 
                    ((((fgpixel & 0xff0000ul) + (bgpixel & 0xff0000ul)) >> 1) & 0xff0000ul)
                    |
                    ((((fgpixel & 0xff00ul) + (bgpixel & 0xff00ul)) >> 1) & 0xff00ul)
                    |
                    ((((fgpixel & 0xfful) + (bgpixel & 0xfful)) >> 1) & 0xfful);
            }
            outpixel = 
                ((ch == ' ') || (ch == '\0') || (ch == 0x00a0) || (ch == 0x3000) || (ch == 0x2008))
                ?
                bgpixel
                :
                ((ch == 0x2580) || (ch == 0x2584) || (ch == 0x258c) || (ch == 0x2590))
                ?
                halfpixel
                :
                fgpixel;
            outpixel2 = outpixel;
            if (cacacurses_bitmap_double || cacacurses_bitmap_doubleheight)
            {
                if (ch == 0x2580)
                {
                    outpixel = fgpixel;
                    outpixel2 = bgpixel;
                }
                else if (ch == 0x2584)
                {
                    outpixel = bgpixel;
                    outpixel2 = fgpixel;
                }
            }
            outpixel3 = outpixel;
            outpixel4 = outpixel2;
            if (cacacurses_bitmap_double || cucul_utf32_is_fullwidth(ch))
            {
                if (ch == 0x258c)
                {
                    outpixel = (outpixel2 = fgpixel);
                    outpixel3 = (outpixel4 = bgpixel);
                }
                else if (ch == 0x2590)
                {
                    outpixel = (outpixel2 = bgpixel);
                    outpixel3 = (outpixel4 = fgpixel);
                }
            }
            charwidth = cacacurses_w / COLS;
            charheight = cacacurses_h / LINES;
            cacacurses_framebuffer[cacacurses_x * charwidth + cacacurses_w * cacacurses_y * charheight] = outpixel;
            if (cacacurses_bitmap_double)
            {
                cacacurses_framebuffer[cacacurses_x * charwidth + 1 + cacacurses_w * cacacurses_y * charheight] = cucul_utf32_is_fullwidth(ch) ? outpixel : outpixel3;
                cacacurses_framebuffer[cacacurses_x * charwidth + cacacurses_w * (cacacurses_y * charheight + 1)] = cacacurses_bitmap_doubleheight ? outpixel : outpixel2;
                cacacurses_framebuffer[cacacurses_x * charwidth + 1 + cacacurses_w * (cacacurses_y * charheight + 1)] = cacacurses_bitmap_doubleheight ? (cucul_utf32_is_fullwidth(ch) ? outpixel : outpixel3) : (cucul_utf32_is_fullwidth(ch) ? outpixel2 : outpixel4);
            }
            if (cacacurses_bitmap_doubleheight)
            {
                cacacurses_framebuffer[cacacurses_x * charwidth + cacacurses_w * (cacacurses_y * charheight + charheight / 2)] = outpixel2;
                if (cacacurses_bitmap_double)
                {
                    cacacurses_framebuffer[cacacurses_x * charwidth + 1 + cacacurses_w * (cacacurses_y * charheight + charheight / 2)] = cucul_utf32_is_fullwidth(ch) ? outpixel2 : outpixel4;
                    cacacurses_framebuffer[cacacurses_x * charwidth + cacacurses_w * (cacacurses_y * charheight + charheight / 2 + 1)] = outpixel2;
                    cacacurses_framebuffer[cacacurses_x * charwidth + 1 + cacacurses_w * (cacacurses_y * charheight + charheight / 2 + 1)] = cucul_utf32_is_fullwidth(ch) ? outpixel2 : outpixel4;
                }
            }
            if (cucul_utf32_is_fullwidth(ch))
            {
                cacacurses_framebuffer[(cacacurses_x + 1) * charwidth + cacacurses_w * cacacurses_y * charheight] = outpixel3;
                if (cacacurses_bitmap_double)
                {
                    cacacurses_framebuffer[(cacacurses_x + 1) * charwidth + 1 + cacacurses_w * cacacurses_y * charheight] = outpixel3;
                    cacacurses_framebuffer[(cacacurses_x + 1) * charwidth + cacacurses_w * (cacacurses_y * charheight + 1)] = (
                        cacacurses_framebuffer[(cacacurses_x + 1) * charwidth + 1 + cacacurses_w * (cacacurses_y * charheight + 1)] = cacacurses_bitmap_doubleheight ? outpixel3 : outpixel4);
                }
                if (cacacurses_bitmap_doubleheight)
                {
                    cacacurses_framebuffer[(cacacurses_x + 1) * charwidth + cacacurses_w * (cacacurses_y * charheight + charheight / 2)] = outpixel4;
                    if (cacacurses_bitmap_double)
                    {
                        cacacurses_framebuffer[(cacacurses_x + 1) * charwidth + 1 + cacacurses_w * (cacacurses_y * charheight + charheight / 2)] = (
                            cacacurses_framebuffer[(cacacurses_x + 1) * charwidth + cacacurses_w * (cacacurses_y * charheight + charheight / 2 + 1)] = (
                                cacacurses_framebuffer[(cacacurses_x + 1) * charwidth + 1 + cacacurses_w * (cacacurses_y * charheight + charheight / 2 + 1)] = outpixel4));
                    }
                }
            }
        }
    }
    else
    {
        cucul_put_char(cv, cacacurses_x, cacacurses_y, ch);
    }
    cacacurses_x += cucul_utf32_is_fullwidth(ch) ? 2 : 1;
    return OK;
}

static int clrtobot(void) {
    unsigned int x, y;

    for (x = cacacurses_x; x < COLS; x ++)
    {
        move(cacacurses_y, x);
        addch(' ');
    }
    for (y = cacacurses_y + 1; y < LINES; y ++)
        for (x = 0; x < COLS; x ++)
        {
            move(y, x);
            addch(' ');
        }
    return OK;
}

static int clrtoeol(void) {
    unsigned int x;

    for (x = cacacurses_x; x < COLS; x ++)
    {
        move(cacacurses_y, x);
        addch(' ');
    }
    return OK;
}

static int addstr(const char *s) {
    while (*s)
    {
        addch(*s++);
    }
    return OK;
}

static int mvprintw(int y, int x, const char *s) {
    move(y, x);
    return addstr(s);
}

static int init_color(int i, short r, short g, short b) {
    if (! can_change_color()) return ERR;
    cacacurses_pal[i] = ((r * 0xffUL / 1000) << 16) | ((g * 0xffUL / 1000) << 8) | (b * 0xffUL / 1000);
    cacacurses_dimpal[i] = ((r * 0x77UL / 1000) << 16) | ((g * 0x77UL / 1000) << 8) | (b * 0x77UL / 1000);
    return OK;
}

static int color_content(int i, short *r, short *g, short *b) {
    if (! can_change_color()) return ERR;
    *r = ((cacacurses_pal[i] >> 16) & 0xffUL) * 1000 / 0xffUL;
    *g = ((cacacurses_pal[i] >> 8) & 0xffUL) * 1000 / 0xffUL;
    *b = (cacacurses_pal[i] & 0xffUL) * 1000 / 0xffUL;
    return OK;
}

static int init_pair(int i, short fg, short bg) {
    cucul_set_color_ansi(cv, fg, bg);
    if (can_change_color())
    {
        short fg_r, fg_g, fg_b, bg_r, bg_g, bg_b;

        if ((color_content(fg, &fg_r, &fg_g, &fg_b) == OK)
            &&
            (color_content(bg, &bg_r, &bg_g, &bg_b) == OK))
        {
            cucul_set_color_argb(cv,
                                 0xf000 | ((fg_r * 0xf / 1000) << 8) | ((fg_g * 0xf / 1000) << 4) | (fg_b * 0xf / 1000),
                                 0xf000 | ((bg_r * 0xf / 1000) << 8) | ((bg_g * 0xf / 1000) << 4) | (bg_b * 0xf / 1000));
        }
    }
    cucul_set_attr(cv, 0);
    pal[i] = cucul_get_attr(cv, -1, -1);
    return OK;
}

static int pair_content(int i, short *fg, short *bg) {
    *fg = i ? i : CUCUL_DEFAULT;
    *bg = i ? CUCUL_BLACK : CUCUL_TRANSPARENT;
    return OK;
}

#else

/* older (pre-1.0) libcaca */

#if ! defined(WIN32)
#include <wchar.h>
#else
#define wcwidth(ch) -1
#endif

#ifndef USE_COLOR
#define USE_COLOR 1
#endif

#ifndef USE_PALETTE
#define USE_PALETTE 1
#endif

#ifndef SWAPDOTS
#define SWAPDOTS 1
#endif

#ifndef USE_SIGWINCH
#define USE_SIGWINCH 0
#endif

#ifndef USE_IOCTL
#define USE_IOCTL 0
#endif

typedef unsigned long int chtype;

typedef chtype attr_t;

static int cacacurses_ready = 0;

static int cacacurses_x, cacacurses_y;

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
#define TRUE (1)
#endif

#define clear erase

#define cbreak()

#define use_env(x)

#define noecho()

#define nonl()

#define nodelay(stdscr, x)

#define intrflush(stdscr, x)

#define immedok(stdscr, x)

#define idlok(stdscr, x)

#define clearok(stdscr, x)

#define leaveok(stdscr, x)

#define getyx(stdscr, y, x) ((y) = cacacurses_y), ((x) = cacacurses_x)

#define wrefresh(stdscr) refresh()

#define beep() addch('\7')

#define echo()

#define A_STANDOUT 0x80000000UL

#define A_BOLD 0x40000000UL

#define A_UNDERLINE 0x20000000UL

#define A_REVERSE 0x10000000UL

static int attrset(int a)
{
    int reverse;

    reverse = a & (A_STANDOUT | A_REVERSE);
    a ^= reverse;
    if (! (a & 0xff)) a |= (((a & A_UNDERLINE) ? CACA_COLOR_BLUE : CACA_COLOR_LIGHTGRAY) << 4) | CACA_COLOR_BLACK;
    if (a & A_BOLD) a |= 0x80;
    if (reverse)
    {
        return caca_set_color(a & 0xf, a >> 4);
    }
    else
    {
        return caca_set_color(a >> 4, a & 0xf);
    }
}

#define curs_set(x)

#define keypad(stdscr, x)

#define can_change_color() 0

#define has_colors() 1

#define start_color()

#define COLOR_PAIR(i) (pal[(i)])

#define LINES (caca_get_height())

#define COLS (caca_get_width())

#define COLOR_BLACK CACA_COLOR_BLACK

#define COLOR_BLUE CACA_COLOR_BLUE

#define COLOR_GREEN CACA_COLOR_GREEN

#define COLOR_CYAN CACA_COLOR_CYAN

#define COLOR_RED CACA_COLOR_RED

#define COLOR_MAGENTA CACA_COLOR_MAGENTA

#define COLOR_YELLOW CACA_COLOR_BROWN

#define COLOR_WHITE CACA_COLOR_LIGHTGRAY

#define COLORS 16

#define COLOR_PAIRS 256

static chtype pal[COLOR_PAIRS];

#define ACS_LRCORNER 0x2518

#define ACS_URCORNER 0x2510

#define ACS_ULCORNER 0x250c

#define ACS_LLCORNER 0x2514

#define ACS_PLUS 0x253c

#define ACS_HLINE 0x2500

#define ACS_LTEE 0x251c

#define ACS_RTEE 0x2524

#define ACS_BTEE 0x2534

#define ACS_TTEE 0x252c

#define ACS_VLINE 0x2502

#define ACS_BULLET 0x00b7

#define KEY_LEFT CACA_KEY_LEFT

#define KEY_RIGHT CACA_KEY_RIGHT

#define KEY_UP CACA_KEY_UP

#define KEY_DOWN CACA_KEY_DOWN

static int cacacurses_use_acs = 1;

#define INITSCR_WITH_HINTS

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    char g[80];

    if ((! myman_getenv("CACA_DRIVER"))
        || strcasecmp(myman_getenv("CACA_DRIVER"), "x11")
        || (! myman_getenv("CACA_FONT"))
        || strcasecmp(myman_getenv("CACA_FONT"), "nil2"))
    {
        if (w > 128) w = 128;
        if (h > 50) h = 50;
    }
    sprintf(g, "%dx%d", w, h);
    if ((! myman_getenv("CACA_GEOMETRY")) || ! *(myman_getenv("CACA_GEOMETRY")))
    {
        myman_setenv("CACA_GEOMETRY", g);
    }
    if (caca_init())
    {
        fprintf(stderr, "caca_init() failed\n");
        fflush(stderr);
        exit(1);
    }
    if (myman_getenv("CACACURSES_ACS"))
    {
        cacacurses_use_acs = *(myman_getenv("CACACURSES_ACS")) && strcmp(myman_getenv("CACACURSES_ACS"), "0");
    }
    else
    {
        cacacurses_use_acs = 0;
    }
    caca_set_feature(CACA_BACKGROUND_BLACK);
    pal[0] = (CACA_COLOR_LIGHTGRAY << 4) | CACA_COLOR_BLACK;
    caca_set_window_title(title);
    cacacurses_ready = 1;
}

static void endwin(void) {
    caca_end();
    cacacurses_ready = 0;
}

static int erase(void) {
    caca_clear();
    cacacurses_x = 0;
    cacacurses_y = 0;
    return OK;
}

static int refresh(void) {
    if (! cacacurses_ready) return OK;
    caca_refresh();
    return OK;
}

static chtype getch(void) {
    unsigned int ev;

    ev = caca_get_event(CACA_EVENT_KEY_PRESS);
    if (ev & ~CACA_EVENT_ANY)
    {
        return ev & ~CACA_EVENT_ANY;
    }
    return ERR;
}

static void standout() {
    caca_set_color(CACA_COLOR_BLACK, CACA_COLOR_WHITE);
}

static void standend() {
    caca_set_color(CACA_COLOR_LIGHTGRAY, CACA_COLOR_BLACK);
}

static int move(y, x) {
    cacacurses_y = y;
    cacacurses_x = x;
    return OK;
}

static int insch(const chtype ch)
{
    return ERR;
}

static int delch(void)
{
    return ERR;
}

static int cacacurses_wcwidth(const chtype ch) {
    int wcw;

    wcw = wcwidth((wchar_t) ch);
    if ((wcw < 1)) return ((ch < 0x20) ? 0 : (ch < 0x3000) ? 1 : 2);
    return wcw;
}

#undef wcwidth
#define wcwidth cacacurses_wcwidth

static int addch(chtype ch) {
    if (cacacurses_use_acs)
    {
        switch(ch)
        {
        case ACS_LRCORNER: ch = ('j' - '_'); break;
        case ACS_URCORNER: ch = ('k' - '_'); break;
        case ACS_ULCORNER: ch = ('l' - '_'); break;
        case ACS_LLCORNER: ch = ('m' - '_'); break;
        case ACS_PLUS: ch = ('n' - '_'); break;
        case ACS_HLINE: ch = ('q' - '_'); break;
        case ACS_LTEE: ch = ('t' - '_'); break;
        case ACS_RTEE: ch = ('u' - '_'); break;
        case ACS_BTEE: ch = ('v' - '_'); break;
        case ACS_TTEE: ch = ('w' - '_'); break;
        case ACS_VLINE: ch = ('x' - '_'); break;
        case ACS_BULLET: ch = ('~' - '_'); break;
        }
    }

    caca_putchar(cacacurses_x, cacacurses_y, ch);
    cacacurses_x ++;
    return OK;
}

static int clrtobot(void) {
    unsigned int x, y;

    for (x = cacacurses_x; x < COLS; x ++)
    {
        move(cacacurses_y, x);
        addch(' ');
    }
    for (y = cacacurses_y + 1; y < LINES; y ++)
        for (x = 0; x < COLS; x ++)
        {
            move(y, x);
            addch(' ');
        }
    return OK;
}

static int clrtoeol(void) {
    unsigned int x;

    for (x = cacacurses_x; x < COLS; x ++)
    {
        move(cacacurses_y, x);
        addch(' ');
    }
    return OK;
}

static int addstr(const char *s) {
    while (*s)
    {
        addch(*s++);
    }
    return OK;
}

static int mvprintw(int y, int x, const char *s) {
    move(y, x);
    return addstr(s);
}

static int init_color(int i, short r, short g, short b) {
    return ERR;
}

static int color_content(int i, short *r, short *g, short *b) {
    return ERR;
}

static int init_pair(int i, short fg, short bg) {
    pal[i] = (fg << 4) | bg;
    return OK;
}

static int pair_content(int i, short *fg, short *bg) {
    *fg = pal[i] >> 4;
    *bg = pal[i] & 0xf;
    return OK;
}

#endif

#endif /* MYMAN_CACACURS_H_INCLUDED */
