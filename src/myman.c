/*BINFMTC:-lcurses -I../inc utils.c
 * myman.c - game logic for the MyMan video game
 * Copyright 1997-2008, Benjamin C. Wiley Sittler <bsittler@gmail.com>
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

/* configuration information */
#ifdef HAVE_CONFIG_H
#ifndef MYMAN_CONFIG_H_INCLUDED
#include "config.h"
#endif
#endif

/* feature guessing */
#ifndef MYMAN_GUESS_H_INCLUDED
#include "guess.h"
#endif

/* used in macbuild.txt to avoid 8bit chars */
#ifndef CHAR_SOLIDUS
#define CHAR_SOLIDUS "/"
#endif

/* used in various WIN32 workaround functions */
#ifdef __BORLANDC__
#ifndef LIT64
#define LIT64(lit) lit##L
#endif
#endif

#ifndef LIT64
#define LIT64(lit) lit##LL
#endif

#ifdef MACCURSES
/* needed for the argv[0] trick */
#ifdef macintosh
#if TARGET_API_MAC_CARBON
#include <Carbon.h>
#else
#include <Files.h>
#include <Processes.h>
#endif
#else
#include <Carbon/Carbon.h>
#endif
#endif

#include <errno.h>

#include <signal.h>

#ifndef SIG_ERR
#define SIG_ERR ((void (*) (int)) -1)
#endif

#if defined(__PACIFIC__) || defined(HI_TECH_C) || defined(SMALL_C)
#ifndef HAVE_RAISE
#define HAVE_RAISE 0
#endif
#endif

#ifndef HAVE_RAISE
#define HAVE_RAISE 1
#endif

#if ! HAVE_RAISE
#undef raise
#define raise(sig) kill(getpid(), (sig))
#endif

#include <limits.h>
#if HAVE_LOCALE_H
#include <locale.h>
#endif
#include <math.h>
#include <ctype.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#if HAVE_UNIXIO_H
#include <unixio.h>
#endif
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <time.h>

/* MyMan utilities; also defines cruft like __MSDOS__ under some circumstances */
#ifndef MYMAN_UTILS_H_INCLUDED
#include "utils.h"
#endif

#if HAVE_LANGINFO_H
#include <langinfo.h>
#endif

#ifndef F_OK
#define F_OK 0
#endif

#ifndef USE_SDL_MIXER
#define USE_SDL_MIXER 0
#endif

#ifndef USE_SDL
#undef USE_SDL_MIXER
#define USE_SDL_MIXER 0
#else
#if ! USE_SDL
#undef USE_SDL_MIXER
#define USE_SDL_MIXER 0
#endif
#endif

/* SDL sometimes redefines main */
#ifdef USE_SDL
#if USE_SDL
#ifdef main
#define _SDL_main_h
#else
#define MAIN_NO_ENVP
#endif
#include <SDL.h>
#if USE_SDL_MIXER
#include <SDL_mixer.h>
#endif
#endif
#endif

/* terminal-screen handling library */

#ifdef NCURSES_XOPEN_HACK
#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif
#endif

#ifndef MY_CURSES_H
#ifdef SLCURSES
#define MY_CURSES_H <slcurses.h>
#endif
#endif

#ifndef MY_CURSES_H
#ifdef XCURSES
#define MY_CURSES_H <xcurses.h>
#endif
#endif

#ifndef MY_CURSES_H
#ifdef CACACURSES
#include "cacacurs.h"
#define MY_CURSES_H "optcurs.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef GTKCURSES
#include "gtkcurs.h"
#define MY_CURSES_H "optcurs.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef FLTKCURSES
#include "fltkcurs.h"
#define MY_CURSES_H "optcurs.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef AACURSES
#define MY_CURSES_H "aacurses.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef NEWTCURSES
#include "newtcurs.h"
#define MY_CURSES_H "optcurs.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef DISPCURSES
#include "dispcurs.h"
#define MY_CURSES_H "optcurs.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef CONIOCURSES
#include "coniocur.h"
#define MY_CURSES_H "optcurs.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef GRAPHCURSES
#include "graphcur.h"
#define MY_CURSES_H "optcurs.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef RAWCURSES
#include "rawcurs.h"
#define MY_CURSES_H "optcurs.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef MACCURSES
#include "maccurs.h"
#define MY_CURSES_H "optcurs.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef ALLEGROCURSES
#include "allegcur.h"
#define MY_CURSES_H "optcurs.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef TWCURSES
#define MY_CURSES_H "twcurses.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef GGICURSES
#include "ggicurs.h"
#define MY_CURSES_H "optcurs.h"
#endif
#endif

#ifndef MY_CURSES_H
#ifdef SDLCURSES
#include "sdlcurs.h"
#define MY_CURSES_H "optcurs.h"
#endif
#endif

#ifdef MY_CURSES_H
#include MY_CURSES_H
#else
#include <curses.h>
#endif

#ifdef OPTCURSES
#include "optcurs.h"
#endif

/* work-arounds for old VMS curses */
#if defined(_VMS_CURSES) || defined(__VMS_CURSES)

#include <smg$routines.h>
#include <ssdef.h>

#pragma __nostandard

#ifndef OLDCURSES
#define OLDCURSES 1
#endif

#ifdef _BOLD
#ifndef HAVE_SETATTR
#define HAVE_SETATTR 1
#endif
#endif

#ifndef cbreak
#define cbreak() crmode()
#endif

#ifndef idlok
#define idlok(s,f)
#endif

#ifndef mvprintw
#define mvprintw mvaddstr
#endif

static int
vmscurses_getch(void)
{
    int res;
    unsigned long kbdid;
    unsigned short keycode = ERR;
    int timeout;

    kbdid = stdkb->_id;
    timeout = 0;
    res = smg$read_keystroke(&kbdid, &keycode, 0, &timeout);
    if (res == SS$_TIMEOUT) return ERR;
    if (res & 1) return keycode;
    return getch();
}

#undef getch
#define getch vmscurses_getch

#ifndef KEY_UP
#define KEY_UP SMG$K_TRM_UP
#endif

#ifndef KEY_DOWN
#define KEY_DOWN SMG$K_TRM_DOWN
#endif

#ifndef KEY_LEFT
#define KEY_LEFT SMG$K_TRM_LEFT
#endif

#ifndef KEY_RIGHT
#define KEY_RIGHT SMG$K_TRM_RIGHT
#endif

#ifndef keypad
#define keypad(s,f) OK
#endif

#ifndef curs_set
#define curs_set(f) ((smg$set_cursor_mode(&(stdkb->_id),(f)?SMG$M_CURSOR_ON:SMG$M_CURSOR_OFF)&1)?1:-1)
#endif

#pragma __standard

#endif

/* work-arounds for BSD 4.4 curses as seen on OpenVMS */
#if defined(_BSD44_CURSES) || defined(__BSD44_CURSES)

#ifndef OLDCURSES
#define OLDCURSES 1
#endif

#endif

/* work-arounds for old BSD curses */
#ifdef OLDCURSES

#ifndef DANGEROUS_ATTRS
#define DANGEROUS_ATTRS 1
#endif

#ifndef intrflush
#define intrflush(win, x)
#endif

#ifndef use_env
#define use_env(x)
#endif

#ifndef HAVE_NODELAY
#ifndef nodelay
#define HAVE_NODELAY 0
#endif
#endif

#ifndef HAVE_CURS_SET
#ifndef curs_set
#define HAVE_CURS_SET 0
#endif
#endif

#ifndef A_STANDOUT
#ifndef HAVE_ATTRSET
#ifndef attrset
#define HAVE_ATTRSET 0
#endif
#endif
#endif

#ifndef USE_ATTR
#define USE_ATTR 1
#endif

#if USE_ATTR
#ifndef A_STANDOUT
#ifdef _STANDOUT
#define A_STANDOUT _STANDOUT
#else
#ifdef __STANDOUT
#define A_STANDOUT ((__STANDOUT) << 8)
#else
#ifdef _REVERSE
#ifdef _BOLD
#define A_STANDOUT ((_BOLD | _REVERSE) << 8)
#else
#define A_STANDOUT ((_REVERSE) << 8)
#endif
#else
#define A_STANDOUT 0x100
#endif
#endif
#endif
#endif
#ifndef A_BLINK
#ifdef _BLINK
#define A_BLINK ((_BLINK) << 8)
#endif
#endif
#ifndef A_BOLD
#ifdef _BOLD
#define A_BOLD ((_BOLD) << 8)
#endif
#endif
#ifndef A_REVERSE
#ifdef _REVERSE
#define A_REVERSE ((_REVERSE) << 8)
#endif
#ifndef A_UNDERLINE
#ifdef _UNDERLINE
#define A_UNDERLINE ((_UNDERLINE) << 8)
#endif
#endif
#endif

#endif

#ifndef beep
#define beep() do{putchar('\a');fflush(stdout);}while(0)
#endif

#ifndef HAVE_CHTYPE
#ifndef chtype
#define HAVE_CHTYPE 0
#endif
#endif

#ifdef A_STANDOUT
#if A_STANDOUT == 0x80
#ifndef A_CHARTEXT
#define A_CHARTEXT 0x7f
#endif
#endif
#endif

#ifndef A_CHARTEXT
#define A_CHARTEXT 0xff
#endif

#ifndef USE_A_CHARTEXT
#define USE_A_CHARTEXT 1
#endif

#endif

/* work-arounds for slcurses */
#ifdef SLCURSES

#ifndef use_env
#define use_env(x)
#endif

#ifndef USE_A_CHARTEXT
#define USE_A_CHARTEXT 1
#endif

#ifndef curscr
#define curscr stdscr
#endif

#ifndef MY_INIT_PAIR_RET
#define MY_INIT_PAIR_RET , ~ERR
#endif

#ifndef USE_PALETTE
#define USE_PALETTE 0
#endif

#ifndef intrflush
#define intrflush(win, x)
#endif

#ifndef DISABLE_IDLOK
#ifndef idlok
#define DISABLE_IDLOK
#endif
#endif

#ifndef pair_content
#define pair_content(p,f,b) \
((*(f) = (p) ? ((SLtt_get_color_object ((p))) >> 16) : 7), \
 (*(b) = (p) ? (((SLtt_get_color_object ((p))) >> 8) & 0xff) : 0))
#endif

#endif

/* HAVE_SETATTR: does our curses implementation include setattr()/clrattr()? */

#ifndef HAVE_SETATTR
#define HAVE_SETATTR 0
#endif

/* HAVE_NODELAY: does our curses implementation include nodelay()? */

#ifndef HAVE_NODELAY
#define HAVE_NODELAY 1
#endif

/* for resizing */

#ifndef USE_IOCTL
#ifdef __PDCURSES__
#define USE_IOCTL 0
#else
#define USE_IOCTL 1
#endif
#endif

#if USE_IOCTL
#if HAVE_IOCTL_H
#include <ioctl.h>
#else
#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#endif
#ifdef TIOCGWINSZ
#ifndef VMS
#include <termios.h>
#endif
#endif
#endif

#ifndef USE_SIGWINCH
#ifdef __PDCURSES__
#define USE_SIGWINCH 0
#else
#ifdef SIGWINCH
#define USE_SIGWINCH 1
#else
#define USE_SIGWINCH 0
#endif
#endif
#endif

/* command-line argument parser */
#ifdef MYGETOPT_H
#include MYGETOPT_H
#else
#include <getopt.h>
#endif

/* character set conversion library */
#ifndef USE_ICONV
#define USE_ICONV 0
#endif

#if USE_ICONV
#include <iconv.h>
#ifndef wcwidth
#if HAVE_WCHAR_H
#include <wchar.h>
#endif
#endif
#ifdef LC_CTYPE
#ifndef uint32_t
/* for uint32_t */
#if HAVE_STDINT_H
#include <stdint.h>
#endif
#endif
#endif

static iconv_t cd_to_wchar = (iconv_t) -1;

static iconv_t cd_to_uni = (iconv_t) -1;

static wchar_t ucs_to_wchar(unsigned long ucs)
{
    wchar_t wcbuf[2];
#ifdef LC_CTYPE
    uint32_t ucsbuf[2];
    const char *ibuf;
    char *obuf;
    size_t ibl;
    size_t obl;
    const char *my_locale;

    do
    {
        if ((! (my_locale = setlocale(LC_CTYPE, "")))
            ||
            (! *my_locale)
            ||
            (! strcmp(my_locale, "C"))
            ||
            (! strcmp(my_locale, "POSIX")))
        {
            wcbuf[0] = 0;
            break;
        }
        if ((cd_to_wchar == (iconv_t) -1)
            &&
            ((cd_to_wchar = iconv_open("wchar_t//IGNORE", "UCS-4-INTERNAL")) == (iconv_t) -1))
        {
            wcbuf[0] = 0;
            break;
        }
        ucsbuf[0] = ucs;
        ucsbuf[1] = 0;
        wcbuf[0] = 0;
        wcbuf[1] = 0;
        ibuf = (char *) (void *) ucsbuf;
        obuf = (char *) (void *) wcbuf;
        ibl = sizeof(ucsbuf);
        obl = sizeof(wcbuf);
        if ((! iconv(cd_to_wchar, &ibuf, &ibl, &obuf, &obl))
            ||
            wcbuf[1]
            ||
            (! wcbuf[0]))
        {
            wcbuf[0] = 0;
            break;
        }
        if (cd_to_uni == (iconv_t) -1)
        {
            cd_to_uni = iconv_open("UCS-4-INTERNAL//IGNORE", "wchar_t");
        }
        ucsbuf[0] = 0;
        ibuf = (char *) (void *) wcbuf;
        obuf = (char *) (void *) ucsbuf;
        ibl = sizeof(wcbuf);
        obl = sizeof(ucsbuf);
        if ((cd_to_uni != (iconv_t) -1)
            &&
            (iconv(cd_to_uni, &ibuf, &ibl, &obuf, &obl))
            &&
            (ucsbuf[0] != ucs))
        {
            /* does not round-trip, probably a broken character */
            wcbuf[0] = 0;
            break;
        }
    }
    while (0);
    if (my_locale)
    {
        setlocale(LC_CTYPE, my_locale);
    }
#else /* ! defined(LC_CTYPE) */
    wcbuf[0] = 0;
#endif /* ! defined(LC_CTYPE) */
    return wcbuf[0] ? wcbuf[0] : (((ucs >= 0x20) && (ucs <= 0x7e)) ? ((wchar_t) ucs) : 0);
}

#else

#define ucs_to_wchar(ucs) ((((unsigned long) (wchar_t) (unsigned long) (ucs)) == ((unsigned long) (ucs))) ? ((wchar_t) (unsigned long) (ucs)) : ((wchar_t) 0))

#endif

/* resize handler */
static volatile int got_sigwinch = 0;

#if USE_SIGWINCH

static void (*old_sigwinch_handler)(int);

static void sigwinch_handler(int signum)
{
    if (signum == SIGWINCH)
    {
        got_sigwinch = 1;
    }
}

#endif

#ifndef CRLF
#define CRLF "\r\n"
#endif

#ifdef __PDCURSES__
#ifdef USE_SDL
#if USE_SDL
#ifndef USE_RAW
#define USE_RAW 1
#endif
#ifndef USE_RAW_UCS
#define USE_RAW_UCS 0
#endif /* ! defined(USE_RAW_UCS) */
#endif
#endif
#ifdef PDC_BUILD
#if PDC_BUILD >= 3300
/* PDCurses does implement init_color as of release 3.3 */
#ifndef USE_PALETTE
#define USE_PALETTE 1
#endif
#endif
#endif
/* PDCurses doesn't implement init_color as of release 2.2 */
#ifndef USE_PALETTE
#define USE_PALETTE 0
#endif
#endif

#ifndef USE_WIDEC_SUPPORT
#ifdef WACS_ULCORNER
#define USE_WIDEC_SUPPORT 1
#else
#define USE_WIDEC_SUPPORT 0
#endif
#endif

#ifndef CCHARW_MAX
#define CCHARW_MAX 6
#endif

#ifndef USE_KEYPAD
#ifdef KEY_LEFT
#define USE_KEYPAD 1
#else
#define USE_KEYPAD 0
#endif
#endif

#ifndef USE_A_CHARTEXT
#define USE_A_CHARTEXT 0
#endif

#ifndef MYMANCTRL
#define MYMANCTRL(x) (((x) == '\?') ? 0x7f : ((x) & ~0x60))
#endif

#ifndef KEY_LEFT
#define KEY_LEFT MYMANCTRL('B')
#endif

#ifndef KEY_RIGHT
#define KEY_RIGHT MYMANCTRL('F')
#endif

#ifndef KEY_UP
#define KEY_UP MYMANCTRL('P')
#endif

#ifndef KEY_DOWN
#define KEY_DOWN MYMANCTRL('N')
#endif

#define IS_LEFT_ARROW(k) ((k == 'h') || (k == 'H') || (k == '4') || (k == KEY_LEFT) || (k == MYMANCTRL('B')))
#define IS_RIGHT_ARROW(k) ((k == 'l') || (k == 'L') || (k == '6') || (k == KEY_RIGHT) || (k == MYMANCTRL('F')))
#define IS_UP_ARROW(k) ((k == 'k') || (k == 'K') || (k == '8') || (k == KEY_UP) || (k == MYMANCTRL('P')))
#define IS_DOWN_ARROW(k) ((k == 'j') || (k == 'J') || (k == '2') || (k == KEY_DOWN) || (k == MYMANCTRL('N')))

#ifndef USE_DIM_AND_BRIGHT
#define USE_DIM_AND_BRIGHT 1
#endif

#ifndef SWAPDOTS
#ifdef __PDCURSES__
#ifndef XCURSES
#define SWAPDOTS 1
#endif
#endif
#endif

#ifndef SWAPDOTS
#define SWAPDOTS ((USE_WIDEC_SUPPORT) ? locale_is_utf8() : 0)
#ifndef NEED_LOCALE_IS_UTF8
#define NEED_LOCALE_IS_UTF8 1
#endif
#endif

#ifdef NEED_LOCALE_IS_UTF8
static int locale_is_utf8(void)
{
    const char *my_locale;
    char *my_locale_lower;
    int is_utf8 = 0;
    int i;

#ifdef LC_CTYPE
    my_locale = setlocale(LC_CTYPE, "");
    if (my_locale)
    {
        my_locale_lower = strdup(my_locale);
        if (my_locale_lower)
        {
            for (i = 0; my_locale_lower[i]; i ++)
            {
                my_locale_lower[i] = tolower(my_locale_lower[i]);
            }
            if (strstr(my_locale_lower, "utf8")
                ||
                strstr(my_locale_lower, "utf-8"))
            {
                is_utf8 = 1;
            }
            free((void *) my_locale_lower);
            my_locale_lower = NULL;
        }
#ifdef CODESET
        {
            char *codeset, *codeset_lower;

            codeset = nl_langinfo(CODESET);
            if (codeset)
            {
                codeset_lower = strdup(codeset);
                if (codeset_lower)
                {
                    for (i = 0; codeset_lower[i]; i ++)
                    {
                        codeset_lower[i] = tolower(codeset_lower[i]);
                    }
                    if ((! strcmp(codeset_lower, "utf8"))
                        ||
                        (! strcmp(codeset_lower, "utf-8")))
                    {
                        is_utf8 = 1;
                    }
                    free((void *) codeset_lower);
                    codeset_lower = NULL;
                }
            }
        }
#endif /* defined(CODESET) */
        setlocale(LC_CTYPE, my_locale);
    }
#endif /* defined(LC_CTYPE) */
    /* for broken systems that do not yet support UTF-8 locales
     * (Cygwin comes to mind) */
    my_locale = myman_getenv("LC_CTYPE");
    if (! my_locale) my_locale = myman_getenv("LC_ALL");
    if (! my_locale) my_locale = myman_getenv("LANG");
    if (my_locale)
    {
        my_locale_lower = strdup(my_locale);
        if (my_locale_lower)
        {
            for (i = 0; my_locale_lower[i]; i ++)
            {
                my_locale_lower[i] = tolower(my_locale_lower[i]);
            }
            if (strstr(my_locale_lower, "utf8")
                ||
                strstr(my_locale_lower, "utf-8"))
            {
                is_utf8 = 1;
            }
            free((void *) my_locale_lower);
            my_locale_lower = NULL;
        }
    }
    return is_utf8;
}
#endif

#ifndef MY_A_BOLD
#ifdef A_BOLD
#define MY_A_BOLD A_BOLD
#endif
#endif

#ifndef MY_A_UNDERLINE
#ifdef A_UNDERLINE
#define MY_A_UNDERLINE A_UNDERLINE
#endif
#endif

#ifndef MY_A_STANDOUT
#ifdef A_STANDOUT
#define MY_A_STANDOUT A_STANDOUT
#endif
#endif

#ifndef MY_A_REVERSE
#ifdef A_REVERSE
#define MY_A_REVERSE A_REVERSE
#endif
#endif

#ifndef MY_A_DIM
#ifdef A_DIM
#define MY_A_DIM A_DIM
#endif
#endif

#ifndef MY_A_ATTRIBUTES
#ifdef A_ATTRIBUTES
#define MY_A_ATTRIBUTES A_ATTRIBUTES
#endif
#endif

#ifndef MY_A_CHARTEXT
#ifdef A_CHARTEXT
#define MY_A_CHARTEXT A_CHARTEXT
#endif
#endif

#ifndef MY_A_REVERSE
#ifdef MY_A_STANDOUT
#define MY_A_REVERSE MY_A_STANDOUT
#endif
#endif

#ifndef MY_A_CHARTEXT
#ifdef MY_A_ATTRIBUTES
#define MY_A_CHARTEXT ~MY_A_ATTRIBUTES
#else
#define MY_A_CHARTEXT 0xFF
#endif
#endif

#ifndef USE_UNDERLINE
#define USE_UNDERLINE 0
#endif

#ifndef USE_ATTR
#ifdef MY_A_BOLD
#define USE_ATTR 1
#else
#define USE_ATTR 0
#endif
#endif

#ifndef DANGEROUS_ATTRS
#define DANGEROUS_ATTRS 0
#endif

#ifndef HAVE_CURS_SET
#define HAVE_CURS_SET USE_ATTR
#endif

#if ! USE_ATTR
#ifndef USE_COLOR
#define USE_COLOR 0
#endif
#endif

#ifndef USE_BEEP
#define USE_BEEP 1
#endif

#ifndef SOUND
#define SOUND 0
#endif

#ifndef USE_COLOR
#ifdef COLOR_BLACK
#define USE_COLOR 1
#else
#define USE_COLOR 0
#endif
#endif

#ifndef COLORIZE
#define COLORIZE 1
#endif

#ifndef USE_PALETTE
#define USE_PALETTE USE_COLOR
#endif

#ifndef USE_RAW_UCS
#ifdef SLANG_VERSION
#if SLANG_VERSION >= 20000
#define USE_RAW_UCS 1
#else
#ifdef SLCURSES
#ifdef UTF8
#define USE_RAW_UCS 1
#endif
#endif
#endif
#endif
#endif

#ifndef USE_RAW_UCS
#if USE_WIDEC_SUPPORT
#define USE_RAW_UCS 1
#else
#define USE_RAW_UCS 0
#endif
#endif

#ifndef USE_RAW
#define USE_RAW 0
#endif

#ifndef USE_ACS
#define USE_ACS 1
#endif

#if USE_WIDEC_SUPPORT

#ifndef MY_ACS_BDDB
#ifdef WACS_BDDB
#define MY_ACS_BDDB WACS_BDDB
#endif
#endif
#ifndef MY_ACS_BSSB
#ifdef WACS_BSSB
#define MY_ACS_BSSB WACS_BSSB
#endif
#endif
#ifndef MY_ACS_ULCORNER
#ifdef WACS_ULCORNER
#define MY_ACS_ULCORNER WACS_ULCORNER
#endif
#endif
#ifndef MY_ACS_DDBB
#ifdef WACS_DDBB
#define MY_ACS_DDBB WACS_DDBB
#endif
#endif
#ifndef MY_ACS_SSBB
#ifdef WACS_SSBB
#define MY_ACS_SSBB WACS_SSBB
#endif
#endif
#ifndef MY_ACS_LLCORNER
#ifdef WACS_LLCORNER
#define MY_ACS_LLCORNER WACS_LLCORNER
#endif
#endif
#ifndef MY_ACS_BBDD
#ifdef WACS_BBDD
#define MY_ACS_BBDD WACS_BBDD
#endif
#endif
#ifndef MY_ACS_BBSS
#ifdef WACS_BBSS
#define MY_ACS_BBSS WACS_BBSS
#endif
#endif
#ifndef MY_ACS_URCORNER
#ifdef WACS_URCORNER
#define MY_ACS_URCORNER WACS_URCORNER
#endif
#endif
#ifndef MY_ACS_DBBD
#ifdef WACS_DBBD
#define MY_ACS_DBBD WACS_DBBD
#endif
#endif
#ifndef MY_ACS_SBBS
#ifdef WACS_SBBS
#define MY_ACS_SBBS WACS_SBBS
#endif
#endif
#ifndef MY_ACS_LRCORNER
#ifdef WACS_LRCORNER
#define MY_ACS_LRCORNER WACS_LRCORNER
#endif
#endif
#ifndef MY_ACS_DBDD
#ifdef WACS_DBDD
#define MY_ACS_DBDD WACS_DBDD
#endif
#endif
#ifndef MY_ACS_SBSD
#ifdef WACS_SBSD
#define MY_ACS_SBSD WACS_SBSD
#endif
#endif
#ifndef MY_ACS_DBDS
#ifdef WACS_DBDS
#define MY_ACS_DBDS WACS_DBDS
#endif
#endif
#ifndef MY_ACS_SBSS
#ifdef WACS_SBSS
#define MY_ACS_SBSS WACS_SBSS
#endif
#endif
#ifndef MY_ACS_RTEE
#ifdef WACS_RTEE
#define MY_ACS_RTEE WACS_RTEE
#endif
#endif
#ifndef MY_ACS_DDDB
#ifdef WACS_DDDB
#define MY_ACS_DDDB WACS_DDDB
#endif
#endif
#ifndef MY_ACS_SDSB
#ifdef WACS_SDSB
#define MY_ACS_SDSB WACS_SDSB
#endif
#endif
#ifndef MY_ACS_DSDB
#ifdef WACS_DSDB
#define MY_ACS_DSDB WACS_DSDB
#endif
#endif
#ifndef MY_ACS_SSSB
#ifdef WACS_SSSB
#define MY_ACS_SSSB WACS_SSSB
#endif
#endif
#ifndef MY_ACS_LTEE
#ifdef WACS_LTEE
#define MY_ACS_LTEE WACS_LTEE
#endif
#endif
#ifndef MY_ACS_DDBD
#ifdef WACS_DDBD
#define MY_ACS_DDBD WACS_DDBD
#endif
#endif
#ifndef MY_ACS_SDBD
#ifdef WACS_SDBD
#define MY_ACS_SDBD WACS_SDBD
#endif
#endif
#ifndef MY_ACS_DSBS
#ifdef WACS_DSBS
#define MY_ACS_DSBS WACS_DSBS
#endif
#endif
#ifndef MY_ACS_SSBS
#ifdef WACS_SSBS
#define MY_ACS_SSBS WACS_SSBS
#endif
#endif
#ifndef MY_ACS_BTEE
#ifdef WACS_BTEE
#define MY_ACS_BTEE WACS_BTEE
#endif
#endif
#ifndef MY_ACS_BDDD
#ifdef WACS_BDDD
#define MY_ACS_BDDD WACS_BDDD
#endif
#endif
#ifndef MY_ACS_BDSD
#ifdef WACS_BDSD
#define MY_ACS_BDSD WACS_BDSD
#endif
#endif
#ifndef MY_ACS_BSDS
#ifdef WACS_BSDS
#define MY_ACS_BSDS WACS_BSDS
#endif
#endif
#ifndef MY_ACS_BSSS
#ifdef WACS_BSSS
#define MY_ACS_BSSS WACS_BSSS
#endif
#endif
#ifndef MY_ACS_TTEE
#ifdef WACS_TTEE
#define MY_ACS_TTEE WACS_TTEE
#endif
#endif
#ifndef MY_ACS_BDSS
#ifdef WACS_BDSS
#define MY_ACS_BDSS WACS_BDSS
#endif
#endif
#ifndef MY_ACS_BDSB
#ifdef WACS_BDSB
#define MY_ACS_BDSB WACS_BDSB
#endif
#endif
#ifndef MY_ACS_SSDB
#ifdef WACS_SSDB
#define MY_ACS_SSDB WACS_SSDB
#endif
#endif
#ifndef MY_ACS_BSDB
#ifdef WACS_BSDB
#define MY_ACS_BSDB WACS_BSDB
#endif
#endif
#ifndef MY_ACS_SDBS
#ifdef WACS_SDBS
#define MY_ACS_SDBS WACS_SDBS
#endif
#endif
#ifndef MY_ACS_SDBB
#ifdef WACS_SDBB
#define MY_ACS_SDBB WACS_SDBB
#endif
#endif
#ifndef MY_ACS_DSSB
#ifdef WACS_DSSB
#define MY_ACS_DSSB WACS_DSSB
#endif
#endif
#ifndef MY_ACS_DSBB
#ifdef WACS_DSBB
#define MY_ACS_DSBB WACS_DSBB
#endif
#endif
#ifndef MY_ACS_BSSD
#ifdef WACS_BSSD
#define MY_ACS_BSSD WACS_BSSD
#endif
#endif
#ifndef MY_ACS_BBSD
#ifdef WACS_BBSD
#define MY_ACS_BBSD WACS_BBSD
#endif
#endif
#ifndef MY_ACS_SBDS
#ifdef WACS_SBDS
#define MY_ACS_SBDS WACS_SBDS
#endif
#endif
#ifndef MY_ACS_BBDS
#ifdef WACS_BBDS
#define MY_ACS_BBDS WACS_BBDS
#endif
#endif
#ifndef MY_ACS_SSBD
#ifdef WACS_SSBD
#define MY_ACS_SSBD WACS_SSBD
#endif
#endif
#ifndef MY_ACS_SBBD
#ifdef WACS_SBBD
#define MY_ACS_SBBD WACS_SBBD
#endif
#endif
#ifndef MY_ACS_DBSS
#ifdef WACS_DBSS
#define MY_ACS_DBSS WACS_DBSS
#endif
#endif
#ifndef MY_ACS_DBBS
#ifdef WACS_DBBS
#define MY_ACS_DBBS WACS_DBBS
#endif
#endif
#ifndef MY_ACS_BDBD
#ifdef WACS_BDBD
#define MY_ACS_BDBD WACS_BDBD
#endif
#endif
#ifndef MY_ACS_BSBS
#ifdef WACS_BSBS
#define MY_ACS_BSBS WACS_BSBS
#endif
#endif
#ifndef MY_ACS_HLINE
#ifdef WACS_HLINE
#define MY_ACS_HLINE WACS_HLINE
#endif
#endif
#ifndef MY_ACS_DBDB
#ifdef WACS_DBDB
#define MY_ACS_DBDB WACS_DBDB
#endif
#endif
#ifndef MY_ACS_SBSB
#ifdef WACS_SBSB
#define MY_ACS_SBSB WACS_SBSB
#endif
#endif
#ifndef MY_ACS_VLINE
#ifdef WACS_VLINE
#define MY_ACS_VLINE WACS_VLINE
#endif
#endif
#ifndef MY_ACS_DDDD
#ifdef WACS_DDDD
#define MY_ACS_DDDD WACS_DDDD
#endif
#endif
#ifndef MY_ACS_SDSD
#ifdef WACS_SDSD
#define MY_ACS_SDSD WACS_SDSD
#endif
#endif
#ifndef MY_ACS_DSDS
#ifdef WACS_DSDS
#define MY_ACS_DSDS WACS_DSDS
#endif
#endif
#ifndef MY_ACS_SSSS
#ifdef WACS_SSSS
#define MY_ACS_SSSS WACS_SSSS
#endif
#endif
#ifndef MY_ACS_PLUS
#ifdef WACS_PLUS
#define MY_ACS_PLUS WACS_PLUS
#endif
#endif
#ifndef MY_ACS_DIAMOND
#ifdef WACS_DIAMOND
#define MY_ACS_DIAMOND WACS_DIAMOND
#endif
#endif
#ifndef MY_ACS_DEGREE
#ifdef WACS_DEGREE
#define MY_ACS_DEGREE WACS_DEGREE
#endif
#endif
#ifndef MY_ACS_BBBB
#ifdef WACS_BBBB
#define MY_ACS_BBBB WACS_BBBB
#endif
#endif
#ifndef MY_ACS_BULLET
#ifdef WACS_BULLET
#define MY_ACS_BULLET WACS_BULLET
#endif
#endif
#ifndef MY_ACS_DARROW
#ifdef WACS_DARROW
#define MY_ACS_DARROW WACS_DARROW
#endif
#endif
#ifndef MY_ACS_UARROW
#ifdef WACS_UARROW
#define MY_ACS_UARROW WACS_UARROW
#endif
#endif
#ifndef MY_ACS_LANTERN
#ifdef WACS_LANTERN
#define MY_ACS_LANTERN WACS_LANTERN
#endif
#endif
#ifndef MY_ACS_BOARD
#ifdef WACS_BOARD
#define MY_ACS_BOARD WACS_BOARD
#endif
#endif
#ifndef MY_ACS_CKBOARD
#ifdef WACS_CKBOARD
#define MY_ACS_CKBOARD WACS_CKBOARD
#endif
#endif
#ifndef MY_ACS_BLOCK
#ifdef WACS_BLOCK
#define MY_ACS_BLOCK WACS_BLOCK
#endif
#endif
#ifndef MY_ACS_LARROW
#ifdef WACS_LARROW
#define MY_ACS_LARROW WACS_LARROW
#endif
#endif
#ifndef MY_ACS_LEQUAL
#ifdef WACS_LEQUAL
#define MY_ACS_LEQUAL WACS_LEQUAL
#endif
#endif
#ifndef MY_ACS_RARROW
#ifdef WACS_RARROW
#define MY_ACS_RARROW WACS_RARROW
#endif
#endif
#ifndef MY_ACS_GEQUAL
#ifdef WACS_GEQUAL
#define MY_ACS_GEQUAL WACS_GEQUAL
#endif
#endif
#ifndef MY_ACS_PI
#ifdef WACS_PI
#define MY_ACS_PI WACS_PI
#endif
#endif
#ifndef MY_ACS_STERLING
#ifdef WACS_STERLING
#define MY_ACS_STERLING WACS_STERLING
#endif
#endif

#else /* ! USE_WIDEC_SUPPORT */

#ifndef MY_ACS_BDDB
#ifdef ACS_BDDB
#define MY_ACS_BDDB ACS_BDDB
#endif
#endif
#ifndef MY_ACS_BSSB
#ifdef ACS_BSSB
#define MY_ACS_BSSB ACS_BSSB
#endif
#endif
#ifndef MY_ACS_ULCORNER
#ifdef ACS_ULCORNER
#define MY_ACS_ULCORNER ACS_ULCORNER
#endif
#endif
#ifndef MY_ACS_DDBB
#ifdef ACS_DDBB
#define MY_ACS_DDBB ACS_DDBB
#endif
#endif
#ifndef MY_ACS_SSBB
#ifdef ACS_SSBB
#define MY_ACS_SSBB ACS_SSBB
#endif
#endif
#ifndef MY_ACS_LLCORNER
#ifdef ACS_LLCORNER
#define MY_ACS_LLCORNER ACS_LLCORNER
#endif
#endif
#ifndef MY_ACS_BBDD
#ifdef ACS_BBDD
#define MY_ACS_BBDD ACS_BBDD
#endif
#endif
#ifndef MY_ACS_BBSS
#ifdef ACS_BBSS
#define MY_ACS_BBSS ACS_BBSS
#endif
#endif
#ifndef MY_ACS_URCORNER
#ifdef ACS_URCORNER
#define MY_ACS_URCORNER ACS_URCORNER
#endif
#endif
#ifndef MY_ACS_DBBD
#ifdef ACS_DBBD
#define MY_ACS_DBBD ACS_DBBD
#endif
#endif
#ifndef MY_ACS_SBBS
#ifdef ACS_SBBS
#define MY_ACS_SBBS ACS_SBBS
#endif
#endif
#ifndef MY_ACS_LRCORNER
#ifdef ACS_LRCORNER
#define MY_ACS_LRCORNER ACS_LRCORNER
#endif
#endif
#ifndef MY_ACS_DBDD
#ifdef ACS_DBDD
#define MY_ACS_DBDD ACS_DBDD
#endif
#endif
#ifndef MY_ACS_SBSD
#ifdef ACS_SBSD
#define MY_ACS_SBSD ACS_SBSD
#endif
#endif
#ifndef MY_ACS_DBDS
#ifdef ACS_DBDS
#define MY_ACS_DBDS ACS_DBDS
#endif
#endif
#ifndef MY_ACS_SBSS
#ifdef ACS_SBSS
#define MY_ACS_SBSS ACS_SBSS
#endif
#endif
#ifndef MY_ACS_RTEE
#ifdef ACS_RTEE
#define MY_ACS_RTEE ACS_RTEE
#endif
#endif
#ifndef MY_ACS_DDDB
#ifdef ACS_DDDB
#define MY_ACS_DDDB ACS_DDDB
#endif
#endif
#ifndef MY_ACS_SDSB
#ifdef ACS_SDSB
#define MY_ACS_SDSB ACS_SDSB
#endif
#endif
#ifndef MY_ACS_DSDB
#ifdef ACS_DSDB
#define MY_ACS_DSDB ACS_DSDB
#endif
#endif
#ifndef MY_ACS_SSSB
#ifdef ACS_SSSB
#define MY_ACS_SSSB ACS_SSSB
#endif
#endif
#ifndef MY_ACS_LTEE
#ifdef ACS_LTEE
#define MY_ACS_LTEE ACS_LTEE
#endif
#endif
#ifndef MY_ACS_DDBD
#ifdef ACS_DDBD
#define MY_ACS_DDBD ACS_DDBD
#endif
#endif
#ifndef MY_ACS_SDBD
#ifdef ACS_SDBD
#define MY_ACS_SDBD ACS_SDBD
#endif
#endif
#ifndef MY_ACS_DSBS
#ifdef ACS_DSBS
#define MY_ACS_DSBS ACS_DSBS
#endif
#endif
#ifndef MY_ACS_SSBS
#ifdef ACS_SSBS
#define MY_ACS_SSBS ACS_SSBS
#endif
#endif
#ifndef MY_ACS_BTEE
#ifdef ACS_BTEE
#define MY_ACS_BTEE ACS_BTEE
#endif
#endif
#ifndef MY_ACS_BDDD
#ifdef ACS_BDDD
#define MY_ACS_BDDD ACS_BDDD
#endif
#endif
#ifndef MY_ACS_BDSD
#ifdef ACS_BDSD
#define MY_ACS_BDSD ACS_BDSD
#endif
#endif
#ifndef MY_ACS_BSDS
#ifdef ACS_BSDS
#define MY_ACS_BSDS ACS_BSDS
#endif
#endif
#ifndef MY_ACS_BSSS
#ifdef ACS_BSSS
#define MY_ACS_BSSS ACS_BSSS
#endif
#endif
#ifndef MY_ACS_TTEE
#ifdef ACS_TTEE
#define MY_ACS_TTEE ACS_TTEE
#endif
#endif
#ifndef MY_ACS_BDSS
#ifdef ACS_BDSS
#define MY_ACS_BDSS ACS_BDSS
#endif
#endif
#ifndef MY_ACS_BDSB
#ifdef ACS_BDSB
#define MY_ACS_BDSB ACS_BDSB
#endif
#endif
#ifndef MY_ACS_SSDB
#ifdef ACS_SSDB
#define MY_ACS_SSDB ACS_SSDB
#endif
#endif
#ifndef MY_ACS_BSDB
#ifdef ACS_BSDB
#define MY_ACS_BSDB ACS_BSDB
#endif
#endif
#ifndef MY_ACS_SDBS
#ifdef ACS_SDBS
#define MY_ACS_SDBS ACS_SDBS
#endif
#endif
#ifndef MY_ACS_SDBB
#ifdef ACS_SDBB
#define MY_ACS_SDBB ACS_SDBB
#endif
#endif
#ifndef MY_ACS_DSSB
#ifdef ACS_DSSB
#define MY_ACS_DSSB ACS_DSSB
#endif
#endif
#ifndef MY_ACS_DSBB
#ifdef ACS_DSBB
#define MY_ACS_DSBB ACS_DSBB
#endif
#endif
#ifndef MY_ACS_BSSD
#ifdef ACS_BSSD
#define MY_ACS_BSSD ACS_BSSD
#endif
#endif
#ifndef MY_ACS_BBSD
#ifdef ACS_BBSD
#define MY_ACS_BBSD ACS_BBSD
#endif
#endif
#ifndef MY_ACS_SBDS
#ifdef ACS_SBDS
#define MY_ACS_SBDS ACS_SBDS
#endif
#endif
#ifndef MY_ACS_BBDS
#ifdef ACS_BBDS
#define MY_ACS_BBDS ACS_BBDS
#endif
#endif
#ifndef MY_ACS_SSBD
#ifdef ACS_SSBD
#define MY_ACS_SSBD ACS_SSBD
#endif
#endif
#ifndef MY_ACS_SBBD
#ifdef ACS_SBBD
#define MY_ACS_SBBD ACS_SBBD
#endif
#endif
#ifndef MY_ACS_DBSS
#ifdef ACS_DBSS
#define MY_ACS_DBSS ACS_DBSS
#endif
#endif
#ifndef MY_ACS_DBBS
#ifdef ACS_DBBS
#define MY_ACS_DBBS ACS_DBBS
#endif
#endif
#ifndef MY_ACS_BDBD
#ifdef ACS_BDBD
#define MY_ACS_BDBD ACS_BDBD
#endif
#endif
#ifndef MY_ACS_BSBS
#ifdef ACS_BSBS
#define MY_ACS_BSBS ACS_BSBS
#endif
#endif
#ifndef MY_ACS_HLINE
#ifdef ACS_HLINE
#define MY_ACS_HLINE ACS_HLINE
#endif
#endif
#ifndef MY_ACS_DBDB
#ifdef ACS_DBDB
#define MY_ACS_DBDB ACS_DBDB
#endif
#endif
#ifndef MY_ACS_SBSB
#ifdef ACS_SBSB
#define MY_ACS_SBSB ACS_SBSB
#endif
#endif
#ifndef MY_ACS_VLINE
#ifdef ACS_VLINE
#define MY_ACS_VLINE ACS_VLINE
#endif
#endif
#ifndef MY_ACS_DDDD
#ifdef ACS_DDDD
#define MY_ACS_DDDD ACS_DDDD
#endif
#endif
#ifndef MY_ACS_SDSD
#ifdef ACS_SDSD
#define MY_ACS_SDSD ACS_SDSD
#endif
#endif
#ifndef MY_ACS_DSDS
#ifdef ACS_DSDS
#define MY_ACS_DSDS ACS_DSDS
#endif
#endif
#ifndef MY_ACS_SSSS
#ifdef ACS_SSSS
#define MY_ACS_SSSS ACS_SSSS
#endif
#endif
#ifndef MY_ACS_PLUS
#ifdef ACS_PLUS
#define MY_ACS_PLUS ACS_PLUS
#endif
#endif
#ifndef MY_ACS_DIAMOND
#ifdef ACS_DIAMOND
#define MY_ACS_DIAMOND ACS_DIAMOND
#endif
#endif
#ifndef MY_ACS_DEGREE
#ifdef ACS_DEGREE
#define MY_ACS_DEGREE ACS_DEGREE
#endif
#endif
#ifndef MY_ACS_BBBB
#ifdef ACS_BBBB
#define MY_ACS_BBBB ACS_BBBB
#endif
#endif
#ifndef MY_ACS_BULLET
#ifdef ACS_BULLET
#define MY_ACS_BULLET ACS_BULLET
#endif
#endif
#ifndef MY_ACS_DARROW
#ifdef ACS_DARROW
#define MY_ACS_DARROW ACS_DARROW
#endif
#endif
#ifndef MY_ACS_UARROW
#ifdef ACS_UARROW
#define MY_ACS_UARROW ACS_UARROW
#endif
#endif
#ifndef MY_ACS_LANTERN
#ifdef ACS_LANTERN
#define MY_ACS_LANTERN ACS_LANTERN
#endif
#endif
#ifndef MY_ACS_BOARD
#ifdef ACS_BOARD
#define MY_ACS_BOARD ACS_BOARD
#endif
#endif
#ifndef MY_ACS_CKBOARD
#ifdef ACS_CKBOARD
#define MY_ACS_CKBOARD ACS_CKBOARD
#endif
#endif
#ifndef MY_ACS_BLOCK
#ifdef ACS_BLOCK
#define MY_ACS_BLOCK ACS_BLOCK
#endif
#endif
#ifndef MY_ACS_LARROW
#ifdef ACS_LARROW
#define MY_ACS_LARROW ACS_LARROW
#endif
#endif
#ifndef MY_ACS_LEQUAL
#ifdef ACS_LEQUAL
#define MY_ACS_LEQUAL ACS_LEQUAL
#endif
#endif
#ifndef MY_ACS_RARROW
#ifdef ACS_RARROW
#define MY_ACS_RARROW ACS_RARROW
#endif
#endif
#ifndef MY_ACS_GEQUAL
#ifdef ACS_GEQUAL
#define MY_ACS_GEQUAL ACS_GEQUAL
#endif
#endif
#ifndef MY_ACS_PI
#ifdef ACS_PI
#define MY_ACS_PI ACS_PI
#endif
#endif
#ifndef MY_ACS_STERLING
#ifdef ACS_STERLING
#define MY_ACS_STERLING ACS_STERLING
#endif
#endif

#endif /* ! USE_WIDEC_SUPPORT */

#ifdef BUILTIN_SIZE
extern const char *builtin_size;
#undef MYMANSIZE
#define MYMANSIZE builtin_size
#else
#ifndef MYMANSIZE
#define MYMANSIZE "big"
#endif
static const char *MYMANSIZE_str = MYMANSIZE;
#undef MYMANSIZE
#define MYMANSIZE MYMANSIZE_str
#endif

#ifndef TILEFILE
#define TILEFILE "chr/chr5x2.txt"
#endif

#ifdef BUILTIN_TILE
#undef TILEFILE
#define TILEFILE 0
extern const char *builtin_tilefile;
#else
static const char TILEFILE_str[] = TILEFILE;
#undef TILEFILE
#define TILEFILE TILEFILE_str
#define builtin_tilefile TILEFILE
#endif

#ifndef SPRITEFILE
#define SPRITEFILE "spr/spr7x3.txt"
#endif

#ifdef BUILTIN_SPRITE
#undef SPRITEFILE
#define SPRITEFILE 0
extern const char *builtin_spritefile;
#else
static const char SPRITEFILE_str[] = SPRITEFILE;
#undef SPRITEFILE
#define SPRITEFILE SPRITEFILE_str
#define builtin_spritefile SPRITEFILE
#endif

#ifndef HAVE_CHTYPE
#define HAVE_CHTYPE 1
#endif

#ifndef HAVE_ATTRSET
#define HAVE_ATTRSET 1
#endif

/* NOTE: while this is actually "char" inside the old BSD libcurses,
 * the old C calling conventions mean we can safely use int instead,
 * and it will eventually get coerced */
#if ! HAVE_CHTYPE
#ifndef chtype
#define chtype int
#endif
#endif

/* mapping from CP437 to VT-100 altcharset */
#if USE_WIDEC_SUPPORT
/* ncurses defines WACS_* as cchar_t * whereas unix95 (at least
 * according to TenDRA) defines them as cchar_t */
#ifdef NCURSES_VERSION
#ifndef MY_WACS_PTR
#define MY_WACS_PTR
#endif
#ifndef MY_WACS_REF
#define MY_WACS_REF *
#endif
#else
#ifndef MY_WACS_PTR
#define MY_WACS_PTR &
#endif
#ifndef MY_WACS_REF
#define MY_WACS_REF
#endif
#endif
static cchar_t MY_WACS_REF
#else
static chtype
#endif
altcharset_cp437[256];

/* mapping from CP437 to ASCII */
static chtype
ascii_cp437[256];

#ifdef NEED_CP437_TO_ASCII
static chtype cp437_to_ascii(unsigned char ch)
{
    return ascii_cp437[(ch & 0xff)];
}
#endif


#ifndef USE_WCWIDTH
#if USE_WIDEC_SUPPORT
#define USE_WCWIDTH 1
#else
#if USE_RAW_UCS
#define USE_WCWIDTH 1
#else
#define USE_WCWIDTH 0
#endif
#endif
#endif

#if USE_WCWIDTH

#if ! USE_ICONV
/* for wcwidth(3) */
#ifndef wcwidth
#include <wchar.h>
#endif
#endif

static int wcwidth_is_suspect = -1;

static int my_wcwidth(wchar_t wc)
{
    int len;

    len = wcwidth(wc);
    if ((len == 1) && (wc & ~0xff))
    {
        if (wcwidth_is_suspect == -1)
        {
            wchar_t twc;

            wcwidth_is_suspect = 0;
            twc = ucs_to_wchar(0xff21); /* U+FF21 FULLWIDTH LATIN CAPITAL LETTER A */
            if (twc
                &&
                (twc != 0xff21)
                &&
                (twc & ~0xff)
                &&
                wcwidth(twc) == 1)
            {
                wcwidth_is_suspect = 1;
            }
        }
        if (wcwidth_is_suspect)
        {
            len = 2;
        }
    }
    return len;
}

#if USE_WIDEC_SUPPORT

static int
my_wcswidth(const wchar_t *s, size_t n)
{
    int ret = 0;

    while ((n --) && (*s != L'\0'))
    {
        int cret = my_wcwidth(*s);
        if (cret != -1)
        {
            ret += cret;
        }
        s ++;
    }
    return ret;
}

#endif

#endif

static void
init_trans(int use_bullet_for_dots)
{
    int i;

    for (i = 0; i < 256; i ++)
        if (isprint(i))
        {
            altcharset_cp437[i] =
#if USE_WIDEC_SUPPORT
                NULL;
#endif
            ascii_cp437[i] = i;
        }
        else
        {
            altcharset_cp437[i] =
#if USE_WIDEC_SUPPORT
                NULL;
#endif
            ascii_cp437[i] =
                '\?';
        }
    altcharset_cp437[19] =
#if USE_WIDEC_SUPPORT
                NULL;
#endif
        ascii_cp437[19] = '!';
    altcharset_cp437[220] =
#if USE_WIDEC_SUPPORT
                NULL;
#endif
        ascii_cp437[220] = ',';
    altcharset_cp437[221] =
#if USE_WIDEC_SUPPORT
                NULL;
#endif
        ascii_cp437[221] = '#';
    altcharset_cp437[222] =
#if USE_WIDEC_SUPPORT
                NULL;
#endif
        ascii_cp437[222] = '#';
    altcharset_cp437[223] =
#if USE_WIDEC_SUPPORT
                NULL;
#endif
        ascii_cp437[223] = '\"';
    
#if USE_ATTR
#ifdef MY_A_REVERSE
    if (! isprint(8))
    {
#if ! USE_WIDEC_SUPPORT
        if (! (MY_A_REVERSE & 0xff))
        {
            altcharset_cp437[8] |= MY_A_REVERSE;
        }
#endif
        if (! (MY_A_REVERSE & 0x7f))
        {
            ascii_cp437[8] |= MY_A_REVERSE;
        }
    }
#endif
#endif
    altcharset_cp437[201] =
#ifdef MY_ACS_BDDB
        MY_ACS_BDDB;
#endif
    altcharset_cp437[218] =
#ifdef MY_ACS_BSSB
        MY_ACS_BSSB;
#else
#ifdef MY_ACS_ULCORNER
    MY_ACS_ULCORNER;
#endif
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[201] =
        ascii_cp437[218] =
        '+';
    altcharset_cp437[200] =
#ifdef MY_ACS_DDBB
        MY_ACS_DDBB;
#endif
    altcharset_cp437[192] =
#ifdef MY_ACS_SSBB
        MY_ACS_SSBB;
#else
#ifdef MY_ACS_LLCORNER
    MY_ACS_LLCORNER;
#endif
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[200] =
        ascii_cp437[192] =
        '+';
    altcharset_cp437[187] =
#ifdef MY_ACS_BBDD
        MY_ACS_BBDD;
#endif
    altcharset_cp437[191] =
#ifdef MY_ACS_BBSS
        MY_ACS_BBSS;
#else
#ifdef MY_ACS_URCORNER
    MY_ACS_URCORNER;
#endif
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[187] =
        ascii_cp437[191] =
        '+';
    altcharset_cp437[188] =
#ifdef MY_ACS_DBBD
        MY_ACS_DBBD;
#endif
    altcharset_cp437[217] =
#ifdef MY_ACS_SBBS
        MY_ACS_SBBS;
#else
#ifdef MY_ACS_LRCORNER
    MY_ACS_LRCORNER;
#endif
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[188] =
        ascii_cp437[217] =
        '+';
    altcharset_cp437[185] =
#ifdef MY_ACS_DBDD
        MY_ACS_DBDD;
#endif
    altcharset_cp437[181] =
#ifdef MY_ACS_SBSD
        MY_ACS_SBSD;
#endif
    altcharset_cp437[182] =
#ifdef MY_ACS_DBDS
        MY_ACS_DBDS;
#endif
    altcharset_cp437[180] =
#ifdef MY_ACS_SBSS
        MY_ACS_SBSS;
#else
#ifdef MY_ACS_RTEE
    MY_ACS_RTEE;
#endif
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[189] =
        ascii_cp437[183] =
        ascii_cp437[185] =
        ascii_cp437[181] =
        ascii_cp437[182] =
        ascii_cp437[180] =
        '+';
    altcharset_cp437[204] =
#ifdef MY_ACS_DDDB
        MY_ACS_DDDB;
#endif
    altcharset_cp437[198] =
#ifdef MY_ACS_SDSB
        MY_ACS_SDSB;
#endif
    altcharset_cp437[199] =
#ifdef MY_ACS_DSDB
        MY_ACS_DSDB;
#endif
    altcharset_cp437[195] =
#ifdef MY_ACS_SSSB
        MY_ACS_SSSB;
#else
#ifdef MY_ACS_LTEE
    MY_ACS_LTEE;
#endif
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[211] =
        ascii_cp437[214] =
        ascii_cp437[204] =
        ascii_cp437[198] =
        ascii_cp437[199] =
        ascii_cp437[195] =
        '+';
    altcharset_cp437[202] =
#ifdef MY_ACS_DDBD
        MY_ACS_DDBD;
#endif
    altcharset_cp437[207] =
#ifdef MY_ACS_SDBD
        MY_ACS_SDBD;
#endif
    altcharset_cp437[208] =
#ifdef MY_ACS_DSBS
        MY_ACS_DSBS;
#endif
    altcharset_cp437[193] =
#ifdef MY_ACS_SSBS
        MY_ACS_SSBS;
#else
#ifdef MY_ACS_BTEE
    MY_ACS_BTEE;
#endif
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[190] =
        ascii_cp437[212] =
        ascii_cp437[202] =
        ascii_cp437[207] =
        ascii_cp437[208] =
        ascii_cp437[193] =
        '+';
    altcharset_cp437[203] =
#ifdef MY_ACS_BDDD
        MY_ACS_BDDD;
#endif
    altcharset_cp437[209] =
#ifdef MY_ACS_BDSD
        MY_ACS_BDSD;
#endif
    altcharset_cp437[210] =
#ifdef MY_ACS_BSDS
        MY_ACS_BSDS;
#endif
    altcharset_cp437[194] =
#ifdef MY_ACS_BSSS
        MY_ACS_BSSS;
#else
#ifdef MY_ACS_TTEE
    MY_ACS_TTEE;
#endif
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[184] =
        ascii_cp437[213] =
        ascii_cp437[203] =
        ascii_cp437[209] =
        ascii_cp437[210] =
        ascii_cp437[194] =
        '+';
    altcharset_cp437[213] =
#ifdef MY_ACS_BDSS
        MY_ACS_BDSS;
#else
#ifdef MY_ACS_BDSB
    MY_ACS_BDSB;
#else
    altcharset_cp437[194];
#endif
#endif
    altcharset_cp437[214] =
#ifdef MY_ACS_SSDB
        MY_ACS_SSDB;
#else
#ifdef MY_ACS_BSDB
    MY_ACS_BSDB;
#else
    altcharset_cp437[195];
#endif
#endif
    altcharset_cp437[212] =
#ifdef MY_ACS_SDBS
        MY_ACS_SDBS;
#else
#ifdef MY_ACS_SDBB
    MY_ACS_SDBB;
#else
    altcharset_cp437[193];
#endif
#endif
    altcharset_cp437[211] =
#ifdef MY_ACS_DSSB
        MY_ACS_DSSB;
#else
#ifdef MY_ACS_DSBB
    MY_ACS_DSBB;
#else
    altcharset_cp437[195];
#endif
#endif
    altcharset_cp437[184] =
#ifdef MY_ACS_BSSD
        MY_ACS_BSSD;
#else
#ifdef MY_ACS_BBSD
    MY_ACS_BBSD;
#else
    altcharset_cp437[194];
#endif
#endif
    altcharset_cp437[183] =
#ifdef MY_ACS_SBDS
        MY_ACS_SBDS;
#else
#ifdef MY_ACS_BBDS
    MY_ACS_BBDS;
#else
    altcharset_cp437[180];
#endif
#endif
    altcharset_cp437[190] =
#ifdef MY_ACS_SSBD
        MY_ACS_SSBD;
#else
#ifdef MY_ACS_SBBD
    MY_ACS_SBBD;
#else
    altcharset_cp437[193];
#endif
#endif
    altcharset_cp437[189] =
#ifdef MY_ACS_DBSS
        MY_ACS_DBSS;
#else
#ifdef MY_ACS_DBBS
    MY_ACS_DBBS;
#else
    altcharset_cp437[180];
#endif
#endif
    altcharset_cp437[205] =
#ifdef MY_ACS_BDBD
        MY_ACS_BDBD;
#endif
    altcharset_cp437[196] =
#ifdef MY_ACS_BSBS
        MY_ACS_BSBS;
#else
#ifdef MY_ACS_HLINE
    MY_ACS_HLINE;
#endif
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[205] =
        ascii_cp437[196] =
        '-';
    altcharset_cp437[186] =
#ifdef MY_ACS_DBDB
        MY_ACS_DBDB;
#endif
    altcharset_cp437[179] =
#ifdef MY_ACS_SBSB
        MY_ACS_SBSB;
#else
#ifdef MY_ACS_VLINE
    MY_ACS_VLINE;
#endif
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[186] =
        ascii_cp437[179] =
        '|';
    altcharset_cp437[206] =
#ifdef MY_ACS_DDDD
        MY_ACS_DDDD;
#endif
    altcharset_cp437[215] =
#ifdef MY_ACS_SDSD
        MY_ACS_SDSD;
#endif
    altcharset_cp437[216] =
#ifdef MY_ACS_DSDS
        MY_ACS_DSDS;
#endif
    altcharset_cp437[197] =
#ifdef MY_ACS_SSSS
        MY_ACS_SSSS;
#else
#ifdef MY_ACS_PLUS
    MY_ACS_PLUS;
#endif
#endif
    altcharset_cp437[4] =
#ifdef MY_ACS_DIAMOND
        MY_ACS_DIAMOND;
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[206] =
        ascii_cp437[215] =
        ascii_cp437[216] =
        ascii_cp437[197] =
        ascii_cp437[4] =
        '+';
    altcharset_cp437[248] =
#ifdef MY_ACS_DEGREE
        MY_ACS_DEGREE;
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[248] =
        '\'';
    altcharset_cp437[241] =
#ifdef  MY_ACS_PLMINUS
        MY_ACS_PLMINUS;
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[241] =
        '#';
    altcharset_cp437[7] =
#ifdef MY_ACS_BBBB
        MY_ACS_BBBB;
#endif
#if ! USE_WIDEC_SUPPORT
    altcharset_cp437[8] =
#endif
        altcharset_cp437[9] =
        altcharset_cp437[254] =
#ifdef MY_ACS_BULLET
        MY_ACS_BULLET;
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[7] =
        ascii_cp437[8] =
        ascii_cp437[9] =
        ascii_cp437[254] =
        'o';
#if USE_ATTR
#ifdef MY_A_REVERSE
#if ! USE_WIDEC_SUPPORT
    if (! (MY_A_REVERSE & 0xff))
    {
        altcharset_cp437[8] |= MY_A_REVERSE;
    }
#endif
    if (! (MY_A_REVERSE & 0x7f))
    {
        ascii_cp437[8] |= MY_A_REVERSE;
    }
#endif
#endif
    altcharset_cp437[25] =
        altcharset_cp437[31] =
#ifdef MY_ACS_DARROW
        MY_ACS_DARROW;
#else
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[25] =
        ascii_cp437[31] =
        'v';
#endif
    altcharset_cp437[24] =
        altcharset_cp437[30] =
#ifdef MY_ACS_UARROW
        MY_ACS_UARROW;
#else
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[24] =
        ascii_cp437[30] =
        '^';
#endif
    altcharset_cp437[15] =
#ifdef MY_ACS_LANTERN
        MY_ACS_LANTERN;
#endif
    altcharset_cp437[176] =
#ifdef MY_ACS_BOARD
        MY_ACS_BOARD;
#endif
    altcharset_cp437[177] =
        altcharset_cp437[178] =
#ifdef MY_ACS_CKBOARD
        MY_ACS_CKBOARD;
#endif
    altcharset_cp437[10] =
        altcharset_cp437[219] =
#ifdef MY_ACS_BLOCK
        MY_ACS_BLOCK;
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[15] =
        ascii_cp437[176] =
        ascii_cp437[177] =
        ascii_cp437[178] =
        ascii_cp437[10] =
        ascii_cp437[219] =
        '#';
    altcharset_cp437[27] =
        altcharset_cp437[17] =
#ifdef MY_ACS_LARROW
        MY_ACS_LARROW;
#endif
    altcharset_cp437[174] =
        altcharset_cp437[243] =
#ifdef MY_ACS_LEQUAL
        MY_ACS_LEQUAL;
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[174] =
        ascii_cp437[243] =
        ascii_cp437[27] =
        ascii_cp437[17] =
        '<';
    altcharset_cp437[26] =
        altcharset_cp437[16] =
#ifdef MY_ACS_RARROW
        MY_ACS_RARROW;
#endif
    altcharset_cp437[175] =
        altcharset_cp437[242] =
#ifdef MY_ACS_GEQUAL
        MY_ACS_GEQUAL;
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[175] =
        ascii_cp437[242] =
        ascii_cp437[26] =
        ascii_cp437[16] =
        '>';
    altcharset_cp437[227] =
#ifdef MY_ACS_PI
        MY_ACS_PI;
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[227] =
        '*';
    altcharset_cp437[156] =
#ifdef MY_ACS_STERLING
        MY_ACS_STERLING;
#endif
#if USE_WIDEC_SUPPORT
    altcharset_cp437[0] = NULL;
#endif
    ascii_cp437[156] =
        'f';
#if ! USE_WIDEC_SUPPORT
    altcharset_cp437[0] =
#endif
        ascii_cp437[0] =
        ' ';
#if ! USE_WIDEC_SUPPORT
    altcharset_cp437[240] =
#endif
        ascii_cp437[240] =
        '=';
#if ! USE_WIDEC_SUPPORT
    altcharset_cp437[247] =
#endif
        ascii_cp437[247] =
        '=';
    ascii_cp437[249] =
        ascii_cp437[250] =
        '.';
    ascii_cp437[254] =
        'o';
    if (use_bullet_for_dots)
    {
        altcharset_cp437[249] =
            altcharset_cp437[250] = altcharset_cp437[254];
        altcharset_cp437[254] =
#if USE_WIDEC_SUPPORT
            NULL;
#else
        'o';
#endif
    }
    else
    {
        altcharset_cp437[249] =
            altcharset_cp437[250] =
#if USE_WIDEC_SUPPORT
            NULL;
#else
        '.';
#endif
    }
    altcharset_cp437[255] =
#if USE_WIDEC_SUPPORT
        NULL;
#endif
    ascii_cp437[255] =
        ' ';
    altcharset_cp437[158] =
#if USE_WIDEC_SUPPORT
        NULL;
#endif
    ascii_cp437[158] =
        'P';
}

static int use_raw = USE_RAW;

static int use_raw_ucs = USE_RAW_UCS;

int use_underline = USE_UNDERLINE;

static int use_idlok = 1;

static int use_acs = 1;
static int use_acs_p = 0;

static int use_dim_and_bright = 0;
static int use_dim_and_bright_p = 0;

int use_color = 0;
static int use_color_p = 0;

static int use_bullet_for_dots = 0;
static int use_bullet_for_dots_p = 0;

static int quit_requested = 0;

int reinit_requested = 0;

int xoff_received = 0;

static int use_fullwidth = 0;

static int use_sound = SOUND;

#define MY_COLS (COLS / (use_fullwidth ? 2 : 1))

#ifdef BUILTIN_VARIANT
extern const char *builtin_variant;
#undef MYMANVARIANT
#define MYMANVARIANT builtin_variant
#else
#ifndef MYMANVARIANT
#define MYMANVARIANT "myman"
#endif
static const char *MYMANVARIANT_str = MYMANVARIANT;
#undef MYMANVARIANT
#define MYMANVARIANT MYMANVARIANT_str
#endif


#ifndef MAZEFILE
#define MAZEFILE "lvl/maze.txt"
#endif

#ifdef BUILTIN_MAZE
extern const char *maze_data;
extern const char *maze_color_data;
extern const char *builtin_mazefile;
#undef MAZEFILE
#define MAZEFILE 0
#else
static char MAZEFILE_str[] = MAZEFILE;
#undef MAZEFILE
#define MAZEFILE MAZEFILE_str
#define builtin_mazefile MAZEFILE
#endif

unsigned short *inside_wall = NULL;

/*

algo idea:

FIXME: this does not paint nested structures correctly. example:

+-----------+
|           |
| +-------+ |
| |       | |
| | +---+ | |
| | |   | | |
| | +---+ | |
| |       | |
| +-------+ |
|           |
+-----------+

phases 0 and 1: find dots and zaps and paint their contiguous regions
"non-invertable", stopping at single or double walls

phase 2: find single walls adjacent to non-invertable cells and for
each one try to provisionally paint the contiguous region, stopping at
single walls; any conflict with an already-painted cell causes the
provisional painting to be undone; success removes the provisional
flag

phase 3: like phase 2, but try it for all unpainted cells

phase 4: for each cell, calculate whether it meets inversion criteria:
majority of quadrants inside wall, or exactly 50% and upper-left
quadrant inside wall

painting: iterate until no new cells are affected, painting in each
direction not blocked by an appropriate wall

*/

static chtype
pen[NPENS];

/* color palette for USE_PALETTE and HTML snapshots */
static const short
pen_pal[16][3] =
{
/*  {  red,green, blue } */
    {    0,    0,    0 }, /* 0: black */
    {    0,    0,  867 }, /* 1: blue */
    {    0,  718,    0 }, /* 2: green (peach stem) */
    {    0,  867,  867 }, /* 3: cyan */
    {  867,  589,  277 }, /* 4: brown (apple/cherry stem) */
    { 1000,  128,  589 }, /* 5: magenta (mypal trim?) */
    { 1000,  718,  277 }, /* 6: salmon (clyde, peach, PUSH START BUTTON) */
    { 1000,  718,  589 }, /* 7: light yellow (dot, blue face, BONUS MYMAN FOR x Pts) */
    {  400,  400,  400 }, /* 8: grey */
    {  128,  128,  867 }, /* 9: light blue (wall, blue ghost) */
    {  589, 1000,    0 }, /* A: light green (mypal?, super dot?) */
    {    0, 1000,  867 }, /* B: light cyan (inky, key top, 1 PLAYER ONLY, ghost scores) */
    { 1000,    0,    0 }, /* C: light red (blinky, apple/cherry, GAME  OVER) */
    {  980,  701,  847 }, /* D: pink (pinky, door, NEW MAN - X, fruit scores) */
    { 1000, 1000,    0 }, /* E: yellow (myman, ship, READY!) */
    {  867,  867,  867 }  /* F: light grey (text, eye, apple/cherry shine, key, bell) */
};

#if USE_COLOR

#ifndef COLORS
#define COLORS 8
#endif

#ifndef COLOR_PAIRS
#define COLOR_PAIRS ((COLORS) * (COLORS))
#endif

#define trans_color(i) \
(((i) == 0) ? COLOR_BLACK \
: ((i) == 1) ? COLOR_BLUE \
: ((i) == 2) ? COLOR_GREEN \
: ((i) == 3) ? COLOR_CYAN \
: ((i) == 4) ? COLOR_RED \
: ((i) == 5) ? COLOR_MAGENTA \
: ((i) == 6) ? COLOR_YELLOW \
: COLOR_WHITE)

#if USE_ATTR
#ifndef PEN_BRIGHT
#ifdef MY_A_BOLD
#define PEN_BRIGHT MY_A_BOLD
#endif
#endif
#ifndef PEN_DIM
#ifdef MY_A_DIM
#define PEN_DIM MY_A_DIM
#endif
#endif
#endif

#ifndef PEN_BRIGHT
#define PEN_BRIGHT 0
#endif

#ifndef PEN_DIM
#define PEN_DIM 0
#endif

#if USE_PALETTE
static short old_pal[16][3];
#endif

static short old_pair[256][2];

#ifndef MY_INIT_PAIR_RET
#define MY_INIT_PAIR_RET
#endif
#define my_init_pair(x,y,z) init_pair(x,y,z) MY_INIT_PAIR_RET

static void
destroy_pen(void)
{
    int i;

#if USE_PALETTE
    if (can_change_color() && (COLORS >= 16) && (COLOR_PAIRS >= 16)) {
        for (i = 0; i < 16; i ++)
            init_color(i, old_pal[i][0], old_pal[i][1], old_pal[i][2]);
    }
#endif
    for (i = 0; i < 256; i ++)
        if (i < COLOR_PAIRS)
            my_init_pair(i, old_pair[i][0], old_pair[i][1]);
}

static void
init_pen(void)
{
    int i;
    int nextpair;
    unsigned char pair_allocated[32];

    memset((void *) pair_allocated, 0, sizeof(pair_allocated));
    for (i = 0; i < 256; i ++)
        if (i < COLOR_PAIRS)
            pair_content(i, old_pair[i], old_pair[i] + 1);
#if USE_PALETTE
    if (can_change_color() && (COLORS >= 16) && (COLOR_PAIRS >= 16))
    {
        short trans_dynamic_color[16];
        short trans_dynamic_color_reverse[16];

        /* attempt to use similar colors for the dynamic palette in case
         * setting the dynamic palette does not actually work */
#ifdef MY_A_BOLD
#define trans_dynamic_pen_bright ((PEN_BRIGHT == MY_A_BOLD) ? 8 : (((PEN_BRIGHT > 0) && (PEN_BRIGHT < 16)) ? PEN_BRIGHT : 8))
#else
#define trans_dynamic_pen_bright (((PEN_BRIGHT > 0) && (PEN_BRIGHT < 16)) ? PEN_BRIGHT : 8)
#endif
        for (i = 0; i < 16; i ++)
        {
            trans_dynamic_color[i] = (short) ERR;
            trans_dynamic_color_reverse[i] = (short) ERR;
        }
        for (i = 0; i < 8; i ++)
        {
            if ((trans_color(i) >= 0)
                &&
                (trans_color(i) < 16)
                &&
                (trans_color(i) != (short) ERR)
                &&
                (i != (short) ERR)
                &&
                (trans_dynamic_color_reverse[trans_color(i)] == (short) ERR))
            {
                trans_dynamic_color[i] = trans_color(i);
                trans_dynamic_color_reverse[trans_color(i)] = i;
                if (((trans_color(i) | trans_dynamic_pen_bright) > 0)
                    &&
                    ((trans_color(i) | trans_dynamic_pen_bright) < 16)
                    &&
                    ((trans_color(i) | trans_dynamic_pen_bright) != (short) ERR)
                    &&
                    ((i | 8) != (short) ERR)
                    &&
                    (trans_dynamic_color_reverse[trans_color(i) | trans_dynamic_pen_bright] == (short) ERR))
                {
                    trans_dynamic_color[i | 8] = trans_color(i) | trans_dynamic_pen_bright;
                    trans_dynamic_color_reverse[trans_color(i) | trans_dynamic_pen_bright] = i | 8;
                }
            }
        }
        for (i = 0; i < 16; i ++)
        {
            if ((trans_dynamic_color[i] == (short) ERR)
                ||
                (trans_dynamic_color_reverse[i] == (short) ERR))
            {
                break;
            }
        }
        if (i != 16)
        {
            for (i = 0; i < 16; i ++)
            {
                trans_dynamic_color[i] = i;
                trans_dynamic_color_reverse[i] = i;
            }
        }
        for (i = 0; i < 16; i ++)
            color_content(i, old_pal[i], old_pal[i] + 1, old_pal[i] + 2);
        for (i = 0; i < 16; i ++) {
            init_color(trans_dynamic_color[i],
                       pen_pal[i][0],
                       pen_pal[i][1],
                       pen_pal[i][2]);
            if ((! i) || (my_init_pair(i, trans_dynamic_color[i], trans_dynamic_color[0]) == ERR))
            {
                pen[i] = COLOR_PAIR(0);
            }
            else
            {
                pen[i] = COLOR_PAIR(i);
                pair_allocated[i / 8] =
                    ((unsigned) (unsigned char) pair_allocated[i / 8])
                    |
                    (1U << (i % 8));
            }
        }
        nextpair = 16;
        for (i = 16; i < 256; i ++)
        {
            pen[i] = pen[i % 16];
#ifdef A_REVERSE
            pen[i] = pen[i / 16] ^ A_REVERSE;
            if (((i / 16) > (i % 16)) && ! (pen[(i % 16) * 16 + (i / 16)] & A_REVERSE))
            {
                pen[i] = pen[(i % 16) * 16 + (i / 16)] | A_REVERSE;
            }
            else if (i % 16)
#endif
            {
                if ((nextpair < COLOR_PAIRS) && ((i % 16) != (i / 16)))
                {
                    if (my_init_pair(nextpair, trans_dynamic_color[i % 16], trans_dynamic_color[i / 16]) != ERR)
                    {
                        pen[i] = COLOR_PAIR(nextpair);
                        pair_allocated[i / 8] =
                            ((unsigned) (unsigned char) pair_allocated[i / 8])
                            |
                            (1U << (i % 8));
                        nextpair++;
                    }
                }
            }
        }
    }
    else
#endif
    {
        for (i = 0; i < 8; i ++)
        {
            if (i &&
                (i < COLOR_PAIRS) &&
                (my_init_pair(i, trans_color(i), COLOR_BLACK) != ERR))
            {
                pen[i] = COLOR_PAIR(i);
                pair_allocated[i / 8] =
                    ((unsigned) (unsigned char) pair_allocated[i / 8])
                    |
                    (1U << (i % 8));
                pen[8 + i] = COLOR_PAIR(i) |
                    (use_dim_and_bright ? PEN_BRIGHT : 0);
            }
            else
            {
                pen[i] = COLOR_PAIR(0);
                pen[8 + i] = pen[i] |
                    (use_dim_and_bright
                     ? ((i == 8) ? PEN_DIM : PEN_BRIGHT)
                     : 0);
            }
        }
        pen[0] = pen[7];
        pen[8] = pen[0] | (use_dim_and_bright ? PEN_DIM : 0);
        nextpair = 8;
        for (i = 16; i < 256; i ++)
        {
            int fgansi, bgansi;

            fgansi = i % 16;
            bgansi = i / 16;
            if (! use_dim_and_bright)
            {
                if ((fgansi > 7) || (bgansi > 7))
                {
                    pen[i] = pen[16 * (bgansi & 7) + (fgansi & 7)];
                    continue;
                }
            }
            else
            {
#ifndef A_REVERSE
                if (bgansi > 7)
                {
                    pen[i] = pen[16 * (bgansi & 7) + fgansi];
                    continue;
                }
#endif
                if ((bgansi <= 7) && (fgansi > 7) && ((fgansi & 7) != bgansi))
                {
#ifdef A_REVERSE
                    if (! (pen[16 * bgansi + (fgansi & 7)] & A_REVERSE))
#endif
                    {
                        pen[i] = pen[16 * bgansi + (fgansi & 7)] | ((fgansi == 8) ? PEN_DIM : PEN_BRIGHT);
                        continue;
                    }
                }
#ifdef A_REVERSE
                if ((fgansi <= 7) && (bgansi > 7) && (fgansi != (bgansi & 7)))
                {
                    if (! (pen[16 * fgansi + (bgansi & 7)] & A_REVERSE))
                    {
                        pen[i] = pen[16 * fgansi + (bgansi & 7)] | ((bgansi == 8) ? PEN_DIM : PEN_BRIGHT) | A_REVERSE;
                        continue;
                    }
                }
                if ((fgansi > 7) && (bgansi > 7))
                {
                    pen[i] = pen[16 * bgansi + (fgansi & 7)];
                    continue;
                }
                if (((fgansi & 7) == (bgansi & 7)) && (bgansi > 7) && (fgansi != bgansi))
                {
                    if (! (pen[16 * fgansi + bgansi] & A_REVERSE))
                    {
                        pen[i] = pen[16 * fgansi + bgansi] | A_REVERSE;
                        continue;
                    }
                }
#endif
                if ((fgansi > 7) && (bgansi > 7))
                {
                    pen[i] = pen[16 * (bgansi & 7) + fgansi];
                    continue;
                }
            }
            pen[i] = pen[fgansi];
#ifdef A_REVERSE
            pen[i] = pen[bgansi] | A_REVERSE;
#endif
            if (fgansi && (fgansi != bgansi))
            {
                if (nextpair < COLOR_PAIRS)
                {
#ifdef A_REVERSE
                    if ((bgansi > 7) && (my_init_pair(nextpair, trans_color(bgansi & 7), trans_color(fgansi & 7)) != ERR))
                    {
                        pen[i] = COLOR_PAIR(nextpair) | A_REVERSE;
                        pair_allocated[i / 8] =
                            ((unsigned) (unsigned char) pair_allocated[i / 8])
                            |
                            (1U << (i % 8));
                        if (bgansi > 7)
                        {
                            pen[i] |= (use_dim_and_bright ? ((bgansi == 8) ? PEN_DIM : PEN_BRIGHT) : 0);
                        }
                        nextpair ++;
                    }
                    else
#endif
                    {
                        if (my_init_pair(nextpair, trans_color(fgansi & 7), trans_color(bgansi & 7)) != ERR)
                        {
                            pen[i] = COLOR_PAIR(nextpair);
                            pair_allocated[i / 8] =
                                ((unsigned) (unsigned char) pair_allocated[i / 8])
                                |
                                (1U << (i % 8));
                            if (fgansi > 7)
                            {
                                pen[i] |= (use_dim_and_bright ? ((fgansi == 8) ? PEN_DIM : PEN_BRIGHT) : 0);
                            }
                            nextpair ++;
                        }
                    }
                }
            }
        }
        if ((COLORS == 16)
            &&
            (nextpair < COLOR_PAIRS))
        {
            int workable;

            workable = 1;
            for (i = 0; i < 8; i ++)
            {
                if ((trans_color(i) < 0) || (trans_color(i) > 8))
                {
                    workable = 0;
                    break;
                }
            }
            if (workable)
            {
                /* NOTE: we assume colors 8-15 are the bright versions
                 * of 0-7 in this case */
                for (i = 0; i < 256; i ++)
                {
                    int fgansi, bgansi;

                    fgansi = i % 16;
                    bgansi = i / 16;
                    if (((fgansi > 7) || (bgansi > 7)) && (nextpair < COLOR_PAIRS))
                    {
                        if (bgansi == fgansi)
                        {
                            pen[i] = pen[16 * bgansi];
                            continue;
                        }
#ifdef A_REVERSE
                        if ((bgansi > fgansi) && (! (pen[16 * fgansi + bgansi] & A_REVERSE)))
                        {
                            pen[i] = pen[16 * fgansi + bgansi] | A_REVERSE;
                            continue;
                        }
#endif
                        if (my_init_pair(nextpair, trans_color(fgansi & 7) | (fgansi & 8), trans_color(bgansi & 7) | (bgansi & 8)) != ERR)
                        {
                            pen[i] = COLOR_PAIR(nextpair);
                            pair_allocated[i / 8] =
                                ((unsigned) (unsigned char) pair_allocated[i / 8])
                                |
                                (1U << (i % 8));
                            if (fgansi > 7)
                            {
                                pen[i] |= (use_dim_and_bright ? ((fgansi == 8) ? PEN_DIM : PEN_BRIGHT) : 0);
                            }
                            nextpair ++;
                        }
                    }
                }
            }
        }
        /* NOTE: we assume the default xterm-256color/xterm-88color
         * palette in these cases */
        if (((COLORS == 88)
             ||
             (COLORS == 256))
            &&
            (COLOR_PAIRS >= 16))
        {
            nextpair = 1;
            for (i = 0; i < 256; i ++)
            {
                int fg, bg;
                int rgbscale;
                int grayscale;
                short fg_rgb[3], bg_rgb[3];
                int fgansi, bgansi;

                fgansi = i % 16;
                bgansi = i / 16;
                if (fgansi == bgansi) fgansi = 0;
#ifdef A_REVERSE
                if (COLOR_PAIRS < 256)
                {
                    if ((fgansi < bgansi) && (! pen[(16 * fgansi) + bgansi] & A_REVERSE))
                    {
                        pen[i] = pen[(16 * fgansi) + bgansi] | A_REVERSE;
                        continue;
                    }
                }
#endif
                if (! i)
                {
                    pen[i] = COLOR_PAIR(0);
                    continue;
                }
                else if (nextpair > COLOR_PAIRS)
                {
                    pen[i] = pen[fgansi];
#ifdef A_REVERSE
                    pen[i] = pen[bgansi] | A_REVERSE;
#endif
                    continue;
                }
                rgbscale = (COLORS == 256) ? 6 : 4;
                grayscale = (COLORS == 256) ? 26 : 10;
                fg_rgb[0] = mille_to_scale(pen_pal[fgansi][0], rgbscale);
                fg_rgb[1] = mille_to_scale(pen_pal[fgansi][1], rgbscale);
                fg_rgb[2] = mille_to_scale(pen_pal[fgansi][2], rgbscale);
                if (fg_rgb[0]
                    &&
                    (fg_rgb[0] < (rgbscale - 1))
                    &&
                    (fg_rgb[0]
                     ==
                     fg_rgb[1])
                    &&
                    (fg_rgb[0]
                     ==
                     fg_rgb[2])
                    &&
                    ((((long) fg_rgb[0]) * (grayscale - 1) / (rgbscale - 1)) != (long) mille_to_scale(pen_pal[fgansi][0], grayscale)))
                {
                    fg = 16 + rgbscale * rgbscale * rgbscale + mille_to_scale(299L * pen_pal[fgansi][0] / 1000 + 587L * pen_pal[fgansi][1] / 1000 + 114L * pen_pal[fgansi][2] / 1000, grayscale) - 1;
                }
                else
                {
                    fg = 16 + rgbscale * (rgbscale * fg_rgb[0] + fg_rgb[1]) + fg_rgb[2];
                }
                bg_rgb[0] = mille_to_scale(pen_pal[bgansi][0], rgbscale);
                bg_rgb[1] = mille_to_scale(pen_pal[bgansi][1], rgbscale);
                bg_rgb[2] = mille_to_scale(pen_pal[bgansi][2], rgbscale);
                if (bg_rgb[0]
                    &&
                    (bg_rgb[0] < (rgbscale - 1))
                    &&
                    (bg_rgb[0]
                     ==
                     bg_rgb[1])
                    &&
                    (bg_rgb[0]
                     ==
                     bg_rgb[2])
                    &&
                    ((((long) bg_rgb[0]) * (grayscale - 1) / (rgbscale - 1)) != (long) mille_to_scale(pen_pal[i/ 16][0], grayscale)))
                {
                    bg = 16 + rgbscale * rgbscale * rgbscale + mille_to_scale(299L * pen_pal[bgansi][0] / 1000 + 587L * pen_pal[bgansi][1] / 1000 + 114L * pen_pal[bgansi][2] / 1000, grayscale) - 1;
                }
                else
                {
                    bg = 16 + rgbscale * (rgbscale * bg_rgb[0] + bg_rgb[1]) + bg_rgb[2];
                }
                if (my_init_pair(nextpair, fg, bg) != ERR)
                {
                    pen[i] = COLOR_PAIR(nextpair);
                    pair_allocated[i / 8] =
                        ((unsigned) (unsigned char) pair_allocated[i / 8])
                        |
                        (1U << (i % 8));
                    nextpair ++;
                }
            }
        }
    }
    /* HACK: throw away the black pen, use white instead */
    pen[0] = pen[7];
}

#endif

/* wrappers around some curses functions to allow raw CP437-mode and
 * snapshots; note that these wrappers support only a small subset of
 * the corresponding curses behavior */

FILE *snapshot = NULL;
FILE *snapshot_txt = NULL;
static int snapshot_x = 0;
static int snapshot_y = 0;
static chtype snapshot_attrs = 0;
static chtype snapshot_attrs_active = 0;
static int snapshot_use_color = 0;

/* simulate a subset of curses attributes in HTML; note that this
 * generates presentational markup (<font color="...">, <u>, <b>,
 * etc.) which is considered deprecated in modern HTML; however there
 * is really no acceptable alternative since this markup needs to look
 * colorful even in older browsers */
static void
snapshot_attrset_active(chtype attrs)
{
    if (! snapshot)
    {
        return;
    }
    if (snapshot_attrs_active != attrs)
    {
        int i = 16;

        if (snapshot_use_color)
        {
            for (i = 0; i < (int) (sizeof(pen)/sizeof(*pen)); i ++)
            {
                if (pen[i] &&
                    snapshot_attrs_active == pen[i])
                {
                    fprintf(snapshot,
                            "</font>");
                    break;
                }
            }
        }
#ifdef MY_A_BOLD
        if (i == 16)
        {
            if (snapshot_attrs_active & MY_A_BOLD)
            {
                fprintf(snapshot,
                        "</b>");
            }
        }
#endif
#ifdef MY_A_UNDERLINE
        if (i == 16)
        {
            if (snapshot_attrs_active & MY_A_UNDERLINE)
            {
                fprintf(snapshot,
                        "</u>");
            }
        }
#endif
        snapshot_attrs_active = attrs;
        if (snapshot_use_color)
        {
            int iodd;

            for (iodd = 0; iodd < (int) (sizeof(pen)/sizeof(*pen)); iodd ++)
            {
                i = (((iodd & 1) ? 8 : 0) | ((iodd & 14) >> 1) | (iodd & ~15)) ^ 7;
                if (pen[i] &&
                    snapshot_attrs_active == pen[i])
                {
                    unsigned long r, g, b;

                    r = (255 * pen_pal[i % 16][0]) / 1000;
                    g = (255 * pen_pal[i % 16][1]) / 1000;
                    b = (255 * pen_pal[i % 16][2]) / 1000;
                    fprintf(snapshot,
                            "<font color=\"#%2.2lX%2.2lX%2.2lX\"",
                            r & 0xffUL, g & 0xffUL, b & 0xffUL);
                    if (i / 16)
                    {
                        r = (255 * pen_pal[i / 16][0]) / 1000;
                        g = (255 * pen_pal[i / 16][1]) / 1000;
                        b = (255 * pen_pal[i / 16][2]) / 1000;
                        fprintf(snapshot,
                                " style=\"%sbackground:#%2.2lX%2.2lX%2.2lX\"",
                                ((i / 16) == (i % 16)) ? "color: #000000; " : "",
                                r & 0xffUL, g & 0xffUL, b & 0xffUL);
                    }
                    fprintf(snapshot,
                            ">");
                    break;
                }
            }
        }
#ifdef MY_A_UNDERLINE
        if (i == 16)
        {
            if (snapshot_attrs_active & MY_A_UNDERLINE)
            {
                fprintf(snapshot,
                        "<u>");
            }
        }
#endif
#ifdef MY_A_BOLD
        if (i == 16)
        {
            if (snapshot_attrs_active & MY_A_BOLD)
            {
                fprintf(snapshot,
                        "<b>");
            }
        }
#endif
    }
    fflush(snapshot);
}

/* non-outputting version of snapshot_attrset */
static void
snapshot_attrset(chtype attrs)
{
    snapshot_attrs = attrs;
}

static int location_is_suspect = 0;
static int last_valid_line = 0;
static int last_valid_col = -1;

static int
my_erase(void)
{
    if (snapshot || snapshot_txt)
    {
        const char *my_locale = "en";
        char *my_locale_dynamic = NULL;

#ifdef LC_CTYPE
        my_locale = setlocale(LC_CTYPE, "");
#endif /* defined(LC_CTYPE) */
        if ((! my_locale) || (! *my_locale))
        {
            my_locale = "en";
        }
        my_locale_dynamic = strdup(my_locale);
        if (! my_locale_dynamic)
        {
            my_locale = "en";
        }
        else
        {
            int i;

            my_locale = my_locale_dynamic;
            for (i = 0; my_locale_dynamic[i]; i ++)
            {
                if (my_locale_dynamic[i] == '_')
                {
                    my_locale_dynamic[i] = '-';
                    continue;
                }
                if ((my_locale_dynamic[i] == '.')
                    ||
                    (my_locale_dynamic[i] == '\"')
                    ||
                    (my_locale_dynamic[i] == '@')
                    ||
                    (my_locale_dynamic[i] < 0x20)
                    ||
                    (my_locale_dynamic[i] > 0x7E))
                {
                    my_locale_dynamic[i] = '\0';
                    break;
                }
            }
            if ((! my_locale_dynamic[0])
                ||
                (! strcmp(my_locale_dynamic, "C"))
                ||
                (! strcmp(my_locale_dynamic, "POSIX")))
            {
                free((void *) my_locale_dynamic);
                my_locale_dynamic = NULL;
                my_locale = "en";
            }
        }
        snapshot_x = 0;
        snapshot_y = 0;
        snapshot_attrs = 0;
        snapshot_attrs_active = 0;
        if (snapshot)
        {
            fprintf(
                snapshot,
                "%s"
                CRLF
                "%s"
                CRLF
                "<html xmlns=\"%s\" xml:lang=\"%s\" lang=\"%s\">"
                CRLF
                "<head>"
                CRLF
                "<meta name=\"generator\" content=\"%s %s\" />"
                CRLF
                "<meta http-equiv=\"Content-type\" content=\"%s\" />"
                CRLF
                "<title>%s</title>"
                CRLF
                "</head>"
                CRLF
                "<body%s%s><pre><font face=\"%s\">"
                CRLF,

                ((use_acs && use_raw && ! use_raw_ucs)
                 ? "<\?xml version=\"1.0\" encoding=\"CP437\"\?>"
                 : "<\?xml version=\"1.0\" encoding=\"UTF-8\"\?>"),

                "<!DOCTYPE html"
                CRLF
                "     PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\""
                CRLF
                "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">",

                "http://www.w3.org/1999/xhtml",

                my_locale, my_locale,

                MYMAN, MYMANVERSION,

                ((use_acs && use_raw && ! use_raw_ucs)
                 ? "text/html; charset=CP437"
                 : "text/html; charset=UTF-8"),

                "MyMan Screenshot [" MYMAN " " MYMANVERSION "]",

                snapshot_use_color ? " text=\"white\"" : "",
                snapshot_use_color ? " bgcolor=\"black\"" : "",

                (CJK_MODE
                 ?
                 "sazanami gothic, kochi gothic, ar pl sew sung, osaka, kai, biaukai, stkaiti, ms gothic, nsimsun, mingliu, fixedsys, courier, monospace"
                 :
                 "courier new, courier, monaco, fixedsys, lucida sans unicode, freemono, fixed, monospace"));
            fflush(snapshot);
        }
        if (snapshot_txt)
        {
            fputc_utf8(0xFEFF, snapshot_txt);
            /* Title */
            fprintf(snapshot_txt,
                    "%s"
                    CRLF,
                    "MyMan Screenshot [" MYMAN " " MYMANVERSION "]");
            fflush(snapshot_txt);
        }
        if (my_locale)
        {
#ifdef LC_CTYPE
            setlocale(LC_CTYPE, my_locale);
#endif /* defined(LC_CTYPE) */
            if (my_locale_dynamic)
            {
                free((void *) my_locale_dynamic);
                my_locale_dynamic = NULL;
                my_locale = NULL;
            }
        }
    }

    if (location_is_suspect)
    {
        last_valid_line = 0;
        last_valid_col = -1;
#ifdef OK
        return OK;
#else
        return ! ERR;
#endif
    }
    last_valid_line = LINES - 1;
    last_valid_col = COLS - 1;
    {
        int ret;

        ret = erase();
        return ret;
    }
}

int
my_clear(void)
{
    location_is_suspect = 0;
    return clear();
}

void
my_clearok(int ok)
{
    clearok(curscr, (ok ? TRUE : FALSE));
}

static int
my_refresh(void)
{
    if (snapshot)
    {
        snapshot_attrset_active(0);
        fprintf(snapshot,
                CRLF
                "</font></pre></body></html>"
                CRLF);
        fflush(snapshot);
        fclose(snapshot);
        snapshot = (FILE *) 0;
    }
    if (snapshot_txt)
    {
        fprintf(snapshot_txt,
                CRLF);
        fflush(snapshot_txt);
        fclose(snapshot_txt);
        snapshot_txt = (FILE *) 0;
    }
    if (location_is_suspect)
    {
        if (((last_valid_col + 1) < COLS)
            ||
            ((last_valid_line + 1) < LINES))
        {
            move((last_valid_line + (last_valid_col + 1) / COLS), (last_valid_col + 1) % COLS);
            clrtobot();
        }
        last_valid_col = COLS - 1;
        last_valid_line = LINES - 1;
    }
    return refresh();
}

static void
my_move(int y, int x)
{
    if ((y < 0) || (x < 0) || (y > LINES) || (x > COLS))
    {
        return;
    }
    if ((snapshot || snapshot_txt)
        &&
        ((x != snapshot_x) || (y != snapshot_y)))
    {
        snapshot_attrset_active(0);
        if ((snapshot || snapshot_txt)
            &&
            (y < snapshot_y))
        {
            if (snapshot)
            {
                fprintf(snapshot, "<!-- cuu%d -->", snapshot_y - y);
                fflush(snapshot);
            }
            snapshot_y = y;
        }
        if (snapshot && (x < snapshot_x) && (y == snapshot_y))
        {
            fprintf(snapshot, "<!-- cub%d -->", snapshot_x - x);
            fflush(snapshot);
        }
        while ((y > snapshot_y) || (x < snapshot_x))
        {
            snapshot_y ++;
            snapshot_x = 0;
            if (snapshot)
            {
                fprintf(snapshot,
                        CRLF);
                fflush(snapshot);
            }
            if (snapshot_txt)
            {
                fprintf(snapshot_txt,
                        CRLF);
                fflush(snapshot_txt);
            }
        }
        while (x > snapshot_x)
        {
            if (snapshot)
            {
                fprintf(snapshot,
                        " ");
                fflush(snapshot);
            }
            if (snapshot_txt)
            {
                fprintf(snapshot_txt,
                        " ");
                fflush(snapshot_txt);
            }
            snapshot_x ++;
        }
    }

    do
    {
        int cur_y, cur_x;

        getyx(stdscr, cur_y, cur_x);
        if (location_is_suspect)
        {
            if (last_valid_col == (COLS - 1))
            {
                last_valid_col = -1;
                last_valid_line ++;
            }
            while (y > last_valid_line)
            {
                move(last_valid_line, last_valid_col + 1);
                clrtoeol();
                last_valid_line ++;
                last_valid_col = -1;
            }
            while ((y == last_valid_line)
                   &&
                   (x > (last_valid_col + 1)))
            {
                move(last_valid_line, ++ last_valid_col);
                addch(' ');
            }
        }
        getyx(stdscr, cur_y, cur_x);
        if ((y != cur_y) || (x != cur_x))
        {
            move(y, x);
        }
    }
    while(0);
}

#if USE_ATTR || USE_COLOR

static int
my_real_attrset(chtype attrs)
{
#if DANGEROUS_ATTRS
    if (attrs)
    {
        int cur_x, cur_y;

        getyx(stdscr, cur_y, cur_x);
        /* classic BSD curses has an annoying bug which causes it to
         * hang if attributes are used in the last writable screen
         * cell */
        if ((cur_x >= (COLS - (CJK_MODE ? 1 : 0) - 2 * (cur_y == (LINES - 1)))))
        {
            return 1;
        }
    }
#endif
#if HAVE_ATTRSET
    attrset(attrs);
#else
    {
#ifdef MY_A_STANDOUT
        if (attrs & MY_A_STANDOUT) standout();
        else standend();
#endif
#if HAVE_SETATTR
        {
#ifdef MY_A_BLINK
#ifdef _BLINK
            if (attrs & MY_A_BLINK) setattr(_BLINK);
            else clrattr(_BLINK);
#endif
#endif
#ifdef MY_A_BOLD
#ifdef _BOLD
            if (attrs & MY_A_BOLD) setattr(_BOLD);
            else clrattr(_BOLD);
#endif
#endif
#ifdef MY_A_REVERSE
#ifdef _REVERSE
            if (attrs & MY_A_REVERSE) setattr(_REVERSE);
            else clrattr(_REVERSE);
#endif
#endif
#ifdef MY_A_UNDERLINE
#ifdef _UNDERLINE
            if (attrs & MY_A_UNDERLINE) setattr(_UNDERLINE);
            else clrattr(_UNDERLINE);
#endif
#endif
        }
#endif
    }
#endif
    return 1;
}

#if DANGEROUS_ATTRS
static chtype my_attrs = 0;
#endif

#endif /* USE_ATTR || USE_COLOR */

static int
my_attrset(chtype attrs)
{
    snapshot_attrset(attrs);
#if USE_ATTR || USE_COLOR
    attrs ^= (snapshot || snapshot_txt) ?
#ifdef MY_A_REVERSE
      MY_A_REVERSE
#else
      0
#endif
      : 0;
#if DANGEROUS_ATTRS
    my_attrs = attrs;
#else
    my_real_attrset(attrs);
#endif
#endif /* USE_ATTR || USE_COLOR */
    return 1;
}

/* add a cp437 string to the HTML snapshot */
static void
snapshot_addch(short inbyte)
{

#undef SNAPSHOT_ADDCH__NARROWC
#if USE_WIDEC_SUPPORT
#define SNAPSHOT_ADDCH__NARROWC(c) NULL
#else
#define SNAPSHOT_ADDCH__NARROWC(c) (c)
#endif

    if (snapshot || snapshot_txt)
    {
        unsigned long codepoint;

        if (use_acs)
        {
            codepoint = ((use_raw && use_raw_ucs) ? uni_cp437 : uni_cp437_halfwidth)[inbyte];
            if (CJK_MODE && use_raw && use_raw_ucs)
            {
                snapshot_x ++;
            }
            else if (! use_raw)
            {
                switch (inbyte)
                {
                case 201:
                    if (altcharset_cp437[201] != altcharset_cp437[218])
                    {
                        codepoint = (altcharset_cp437[201] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[201])
                            ) ? uni_cp437_halfwidth[201] : (ascii_cp437[201] & MY_A_CHARTEXT);
                        break;
                    }
                case 218:
                    codepoint = (altcharset_cp437[218] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[218])
                        ) ? uni_cp437_halfwidth[218] : (ascii_cp437[218] & MY_A_CHARTEXT);
                    break;
                case 200:
                    if (altcharset_cp437[200] != altcharset_cp437[192])
                    {
                        codepoint = (altcharset_cp437[200] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[200])
                            ) ? uni_cp437_halfwidth[200] : (ascii_cp437[200] & MY_A_CHARTEXT);
                        break;
                    }
                case 192:
                    codepoint = (altcharset_cp437[192] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[192])
                        ) ? uni_cp437_halfwidth[192] : (ascii_cp437[192] & MY_A_CHARTEXT);
                    break;
                case 187:
                    if (altcharset_cp437[187] != altcharset_cp437[191])
                    {
                        codepoint = (altcharset_cp437[187] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[187])
                            ) ? uni_cp437_halfwidth[187] : (ascii_cp437[187] & MY_A_CHARTEXT);
                        break;
                    }
                case 191:
                    codepoint = (altcharset_cp437[191] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[191])
                        ) ? uni_cp437_halfwidth[191] : (ascii_cp437[191] & MY_A_CHARTEXT);
                    break;
                case 188:
                    if (altcharset_cp437[188] != altcharset_cp437[217])
                    {
                        codepoint = (altcharset_cp437[188] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[188])
                            ) ? uni_cp437_halfwidth[188] : (ascii_cp437[188] & MY_A_CHARTEXT);
                        break;
                    }
                case 217:
                    codepoint = (altcharset_cp437[217] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[217])
                        ) ? uni_cp437_halfwidth[217] : (ascii_cp437[217] & MY_A_CHARTEXT);
                    break;
                case 185:
                    if (altcharset_cp437[185] != altcharset_cp437[181])
                    {
                        codepoint = (altcharset_cp437[185] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[185])
                            ) ? uni_cp437_halfwidth[185] : (ascii_cp437[185] & MY_A_CHARTEXT);
                        break;
                    }
                case 181:
                    if (altcharset_cp437[181] != altcharset_cp437[182])
                    {
                        codepoint = (altcharset_cp437[181] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[181])
                            ) ? uni_cp437_halfwidth[181] : (ascii_cp437[181] & MY_A_CHARTEXT);
                        break;
                    }
                case 182:
                    if (altcharset_cp437[182] != altcharset_cp437[180])
                    {
                        codepoint = (altcharset_cp437[182] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[182])
                            ) ? uni_cp437_halfwidth[182] : (ascii_cp437[182] & MY_A_CHARTEXT);
                        break;
                    }
                case 180:
                    codepoint = (altcharset_cp437[180] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[180])
                        ) ? uni_cp437_halfwidth[180] : (ascii_cp437[180] & MY_A_CHARTEXT);
                    break;
                case 204:
                    if (altcharset_cp437[204] != altcharset_cp437[198])
                    {
                        codepoint = (altcharset_cp437[204] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[204])
                            ) ? uni_cp437_halfwidth[204] : (ascii_cp437[204] & MY_A_CHARTEXT);
                        break;
                    }
                case 198:
                    if (altcharset_cp437[198] != altcharset_cp437[199])
                    {
                        codepoint = (altcharset_cp437[198] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[198])
                            ) ? uni_cp437_halfwidth[198] : (ascii_cp437[198] & MY_A_CHARTEXT);
                        break;
                    }
                case 199:
                    if (altcharset_cp437[199] != altcharset_cp437[195])
                    {
                        codepoint = (altcharset_cp437[199] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[199])
                            ) ? uni_cp437_halfwidth[199] : (ascii_cp437[199] & MY_A_CHARTEXT);
                        break;
                    }
                case 195:
                    codepoint = (altcharset_cp437[195] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[195])
                        ) ? uni_cp437_halfwidth[195] : (ascii_cp437[195] & MY_A_CHARTEXT);
                    break;
                case 202:
                    if (altcharset_cp437[202] != altcharset_cp437[207])
                    {
                        codepoint = (altcharset_cp437[202] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[202])
                            ) ? uni_cp437_halfwidth[202] : (ascii_cp437[202] & MY_A_CHARTEXT);
                        break;
                    }
                case 207:
                    if (altcharset_cp437[207] != altcharset_cp437[208])
                    {
                        codepoint = (altcharset_cp437[207] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[207])
                            ) ? uni_cp437_halfwidth[207] : (ascii_cp437[207] & MY_A_CHARTEXT);
                        break;
                    }
                case 208:
                    if (altcharset_cp437[208] != altcharset_cp437[193])
                    {
                        codepoint = (altcharset_cp437[208] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[208])
                            ) ? uni_cp437_halfwidth[208] : (ascii_cp437[208] & MY_A_CHARTEXT);
                        break;
                    }
                case 193:
                    codepoint = (altcharset_cp437[193] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[193])
                        ) ? uni_cp437_halfwidth[193] : (ascii_cp437[193] & MY_A_CHARTEXT);
                    break;
                case 203:
                    if (altcharset_cp437[203] != altcharset_cp437[209])
                    {
                        codepoint = (altcharset_cp437[203] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[203])
                            ) ? uni_cp437_halfwidth[203] : (ascii_cp437[203] & MY_A_CHARTEXT);
                        break;
                    }
                case 209:
                    if (altcharset_cp437[209] != altcharset_cp437[210])
                    {
                        codepoint = (altcharset_cp437[209] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[209])
                            ) ? uni_cp437_halfwidth[209] : (ascii_cp437[209] & MY_A_CHARTEXT);
                        break;
                    }
                case 210:
                    if (altcharset_cp437[210] != altcharset_cp437[194])
                    {
                        codepoint = (altcharset_cp437[210] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[210])
                            ) ? uni_cp437_halfwidth[210] : (ascii_cp437[210] & MY_A_CHARTEXT);
                        break;
                    }
                case 194:
                    codepoint = (altcharset_cp437[194] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[194])
                        ) ? uni_cp437_halfwidth[194] : (ascii_cp437[194] & MY_A_CHARTEXT);
                    break;
                case 213:
                    codepoint = (altcharset_cp437[194] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[194])
                        ) ? uni_cp437_halfwidth[194] : (ascii_cp437[194] & MY_A_CHARTEXT);
                    break;
                case 214:
                    codepoint = (altcharset_cp437[195] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[195])
                        ) ? uni_cp437_halfwidth[195] : (ascii_cp437[195] & MY_A_CHARTEXT);
                    break;
                case 212:
                    codepoint = (altcharset_cp437[193] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[193])
                        ) ? uni_cp437_halfwidth[193] : (ascii_cp437[193] & MY_A_CHARTEXT);
                    break;
                case 211:
                    codepoint = (altcharset_cp437[195] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[195])
                        ) ? uni_cp437_halfwidth[195] : (ascii_cp437[195] & MY_A_CHARTEXT);
                    break;
                case 184:
                    codepoint = (altcharset_cp437[194] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[194])
                        ) ? uni_cp437_halfwidth[194] : (ascii_cp437[194] & MY_A_CHARTEXT);
                    break;
                case 183:
                    codepoint = (altcharset_cp437[180] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[180])
                        ) ? uni_cp437_halfwidth[180] : (ascii_cp437[180] & MY_A_CHARTEXT);
                    break;
                case 190:
                    codepoint = (altcharset_cp437[193] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[193])
                        ) ? uni_cp437_halfwidth[193] : (ascii_cp437[193] & MY_A_CHARTEXT);
                    break;
                case 189:
                    codepoint = (altcharset_cp437[180] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[180])
                        ) ? uni_cp437_halfwidth[180] : (ascii_cp437[180] & MY_A_CHARTEXT);
                    break;
                case 205:
                    if (altcharset_cp437[205] != altcharset_cp437[196])
                    {
                        codepoint = (altcharset_cp437[205] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[205])
                            ) ? uni_cp437_halfwidth[205] : (ascii_cp437[205] & MY_A_CHARTEXT);
                        break;
                    }
                case 196:
                    codepoint = (altcharset_cp437[196] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[196])
                        ) ? uni_cp437_halfwidth[196] : (ascii_cp437[196] & MY_A_CHARTEXT);
                    break;
                case 186:
                    if (altcharset_cp437[186] != altcharset_cp437[179])
                    {
                        codepoint = (altcharset_cp437[186] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[186])
                            ) ? uni_cp437_halfwidth[186] : (ascii_cp437[186] & MY_A_CHARTEXT);
                        break;
                    }
                case 179:
                    codepoint = (altcharset_cp437[179] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[179])
                        ) ? uni_cp437_halfwidth[179] : (ascii_cp437[179] & MY_A_CHARTEXT);
                    break;
                case 206:
                    if (altcharset_cp437[206] != altcharset_cp437[215])
                    {
                        codepoint = (altcharset_cp437[206] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[206])
                            ) ? uni_cp437_halfwidth[206] : (ascii_cp437[206] & MY_A_CHARTEXT);
                        break;
                    }
                case 215:
                    if (altcharset_cp437[215] != altcharset_cp437[216])
                    {
                        codepoint = (altcharset_cp437[215] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[215])
                            ) ? uni_cp437_halfwidth[215] : (ascii_cp437[215] & MY_A_CHARTEXT);
                        break;
                    }
                case 216:
                    if (altcharset_cp437[216] != altcharset_cp437[197])
                    {
                        codepoint = (altcharset_cp437[216] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[216])
                            ) ? uni_cp437_halfwidth[216] : (ascii_cp437[216] & MY_A_CHARTEXT);
                        break;
                    }
                case 197:
                    codepoint = (altcharset_cp437[197] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[197])
                        ) ? uni_cp437_halfwidth[197] : (ascii_cp437[197] & MY_A_CHARTEXT);
                    break;
                case 15:
                    if (altcharset_cp437[15] != altcharset_cp437[176])
                    {
                        codepoint = (altcharset_cp437[15] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[15])
                            ) ? uni_cp437_halfwidth[15] : (ascii_cp437[15] & MY_A_CHARTEXT);
                        break;
                    }
                case 176:
                    if (altcharset_cp437[176] != altcharset_cp437[177])
                    {
                        codepoint = (altcharset_cp437[176] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[176])
                            ) ? uni_cp437_halfwidth[176] : (ascii_cp437[176] & MY_A_CHARTEXT);
                        break;
                    }
                case 177:
                    if (altcharset_cp437[177] != altcharset_cp437[178])
                    {
                        codepoint = (altcharset_cp437[177] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[177])
                            ) ? uni_cp437_halfwidth[177] : (ascii_cp437[177] & MY_A_CHARTEXT);
                        break;
                    }
                case 178:
                    if (altcharset_cp437[178] != altcharset_cp437[10])
                    {
                        codepoint = (altcharset_cp437[178] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[178])
                            ) ? uni_cp437_halfwidth[178] : (ascii_cp437[178] & MY_A_CHARTEXT);
                        break;
                    }
                case 10:
                    if (altcharset_cp437[10] != altcharset_cp437[219])
                    {
                        codepoint = (altcharset_cp437[10] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[10])
                            ) ? uni_cp437_halfwidth[10] : (ascii_cp437[10] & MY_A_CHARTEXT);
                        break;
                    }
                case 219:
                    codepoint = (altcharset_cp437[219] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[219])
                        ) ? uni_cp437_halfwidth[219] : (ascii_cp437[219] & MY_A_CHARTEXT);
                    break;
                case 27:
                    if (altcharset_cp437[27] != altcharset_cp437[17])
                    {
                        codepoint = (altcharset_cp437[27] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[27])
                            ) ? uni_cp437_halfwidth[27] : (ascii_cp437[27] & MY_A_CHARTEXT);
                        break;
                    }
                case 17:
                    if (altcharset_cp437[17] != altcharset_cp437[174])
                    {
                        codepoint = (altcharset_cp437[17] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[17])
                            ) ? uni_cp437_halfwidth[17] : (ascii_cp437[17] & MY_A_CHARTEXT);
                        break;
                    }
                case 174:
                    if (altcharset_cp437[174] != altcharset_cp437[243])
                    {
                        codepoint = (altcharset_cp437[174] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[174])
                            ) ? uni_cp437_halfwidth[174] : (ascii_cp437[174] & MY_A_CHARTEXT);
                        break;
                    }
                case 243:
                    codepoint = (altcharset_cp437[243] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[243])
                        ) ? uni_cp437_halfwidth[243] : (ascii_cp437[243] & MY_A_CHARTEXT);
                    break;
                case 26:
                    if (altcharset_cp437[26] != altcharset_cp437[16])
                    {
                        codepoint = (altcharset_cp437[26] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[26])
                            ) ? uni_cp437_halfwidth[26] : (ascii_cp437[26] & MY_A_CHARTEXT);
                        break;
                    }
                case 16:
                    if (altcharset_cp437[16] != altcharset_cp437[175])
                    {
                        codepoint = (altcharset_cp437[16] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[16])
                            ) ? uni_cp437_halfwidth[16] : (ascii_cp437[16] & MY_A_CHARTEXT);
                        break;
                    }
                case 175:
                    if (altcharset_cp437[175] != altcharset_cp437[242])
                    {
                        codepoint = (altcharset_cp437[175] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[175])
                            ) ? uni_cp437_halfwidth[175] : (ascii_cp437[175] & MY_A_CHARTEXT);
                        break;
                    }
                case 242:
                    codepoint = (altcharset_cp437[242] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[242])
                        ) ? uni_cp437_halfwidth[242] : (ascii_cp437[242] & MY_A_CHARTEXT);
                    break;
                case 7:
                    if (altcharset_cp437[7] != altcharset_cp437[9])
                    {
                        codepoint = (altcharset_cp437[7] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[7])
                            ) ? uni_cp437_halfwidth[7] : (ascii_cp437[7] & MY_A_CHARTEXT);
                        break;
                    }
                case 9:
                    if (altcharset_cp437[9] != altcharset_cp437[254])
                    {
                        codepoint = (altcharset_cp437[9] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[9])
                            ) ? uni_cp437_halfwidth[9] : (ascii_cp437[9] & MY_A_CHARTEXT);
                        break;
                    }
                case 8:
                    codepoint = (altcharset_cp437[8] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[8])
                        ) ? uni_cp437_halfwidth[8] : (ascii_cp437[8] & MY_A_CHARTEXT);
                    break;
                case 4:
                case 25:
                case 31:
                case 24:
                case 30:
                case 248:
                case 241:
                case 227:
                case 156:
                case 249:
                case 250:
                case 254:
                    codepoint = (altcharset_cp437[inbyte] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[inbyte])
                        ) ? uni_cp437_halfwidth[inbyte] : (ascii_cp437[inbyte] & MY_A_CHARTEXT);
                    break;
                default:
                    inbyte = (int) (unsigned char)
                        (ascii_cp437[inbyte] & MY_A_CHARTEXT);
                    codepoint = inbyte;
                }
            }
        }
        else
        {
            inbyte = (int) (unsigned char)
                (ascii_cp437[inbyte] & MY_A_CHARTEXT);
            codepoint = inbyte;
        }
        if (snapshot)
        {
            snapshot_attrset_active(snapshot_attrs);
            if (codepoint == '&')
            {
                fprintf(snapshot,
                        "&amp;");
            }
            else if (codepoint == '<')
            {
                fprintf(snapshot,
                        "&lt;");
            }
            else if (codepoint == '>')
            {
                fprintf(snapshot,
                        "&gt;");
            }
            else if (codepoint == '\"')
            {
                fprintf(snapshot,
                        "&quot;");
            }
            else if (use_acs && use_raw && ! use_raw_ucs)
            {
                fprintf(snapshot,
                        "%c",
                        (char) inbyte);
            }
            else if ((codepoint >= 0x20) && (codepoint <= 0x7E))
            {
                fprintf(snapshot,
                        "%c",
                        (char) codepoint);
            }
            else
            {
                fprintf(snapshot,
                        "&#%lu;",
                        codepoint);
            }
            fflush(snapshot);
        }
        if (snapshot_txt)
        {
#if USE_ATTR
#ifdef MY_A_BOLD
            if (snapshot_attrs_active & MY_A_BOLD)
            {
#ifdef MY_A_UNDERLINE
                if (snapshot_attrs_active & MY_A_UNDERLINE)
                {
                    fputs("_\b", snapshot_txt);
                }
#endif
                fputc_utf8(codepoint, snapshot_txt);
                fputc('\b', snapshot_txt);
            }
#endif
#ifdef MY_A_UNDERLINE
            if (snapshot_attrs_active & MY_A_UNDERLINE)
            {
                fputs("_\b", snapshot_txt);
            }
#endif
#endif
            fputc_utf8(codepoint, snapshot_txt);
            fflush(snapshot_txt);
        }
        snapshot_x ++;
    }
}

/* non-blocking version of getch(); return a single character if it is
 * available, ERR otherwise */
static int
my_getch(void)
{
    int k = ERR;
#if ! HAVE_NODELAY
    {
        int avail = 1;

#ifdef FIONREAD
        ioctl(fileno(stdin), FIONREAD, &avail);
#endif /* defined(FIONREAD) */
        if (! avail)
        {
            return k;
        }
    }
#endif
    k = getch();
    return k;
}

/* add CP437 byte b with attributes attrs */
static int
my_addch(unsigned long b, chtype attrs)
{
    int ret = 0;
    chtype c = '\?';
    int old_y, old_x;
    int new_y, new_x;

    if (! b) b = ' ';
    getyx(stdscr, old_y, old_x);
    if ((old_y == last_valid_line)
        &&
        (old_x == (last_valid_col + 1)))
    {
        last_valid_col += CJK_MODE ? 2 : 1;
    }
    my_attrset(attrs);
    snapshot_addch(b);
    if (CJK_MODE && ! (use_acs && use_raw && use_raw_ucs))
    {
        unsigned char rhs;

        rhs = cp437_fullwidth_rhs[b];
        if ((int) (unsigned char) rhs)
        {
            snapshot_addch(rhs);
        }
    }
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
    my_real_attrset(my_attrs);
#endif
#endif /* USE_ATTR || USE_COLOR */
    do
    {
        if (use_acs && use_raw && ! use_raw_ucs)
        {
            char buf[2];
            buf[0] = (char) (unsigned char) (b & 0xFF);
            buf[1] = '\0';
            ret = addstr(buf);
            getyx(stdscr, new_y, new_x);
            if ((old_x != new_x) || (old_y != new_y))
            {
                if (CJK_MODE && ((new_x % COLS) != ((old_x + 2) % COLS)))
                {
                    unsigned char rhs;

                    rhs = cp437_fullwidth_rhs[b];
                    if ((int) (unsigned char) rhs)
                    {
                        buf[0] = (char) (unsigned char) (0xFFU & (unsigned) rhs);
                        addstr(buf);
                    }
                }
                break;
            }
        }
        if (b <= 0xFF)
        {
            if (use_acs)
            {
#if USE_WIDEC_SUPPORT
                if (use_raw && use_raw_ucs)
                {
                    unsigned long my_ucs;
                    wchar_t my_wch;
                    int my_wcw;

                    my_ucs = uni_cp437[b];
                    my_wch = ucs_to_wchar(my_ucs);
                    my_wcw = my_wch ? my_wcwidth(my_wch) : 0;
                    if ((my_wcw > 0) && (my_wcw <= ((CJK_MODE) ? 2 : 1)))
                    {
                        ret = addnwstr(&my_wch, 1);
                        getyx(stdscr, new_y, new_x);
                        if ((old_x != new_x) || (old_y != new_y))
                        {
                            if (CJK_MODE && (my_wcw == 1))
                            {
                                unsigned char rhs;

                                rhs = cp437_fullwidth_rhs[b];
                                if ((int) (unsigned char) rhs)
                                {
                                    wchar_t wrhs;

                                    wrhs = ucs_to_wchar(uni_cp437_fullwidth[(int) (unsigned char) rhs]);
                                    my_wcw += wrhs ? my_wcwidth(wrhs) : 0;
                                    if (my_wcw == 2)
                                    {
                                        addnwstr(&wrhs, 1);
                                    }
                                }
                            }
                            else if (CJK_MODE && (my_wcw == 2)
                                     &&
                                     (((old_x + 1) % COLS) == (new_x % COLS)))
                            {
                                location_is_suspect = 1;
                                leaveok(stdscr, FALSE);
                                move(0, 0);
                                refresh();
                                leaveok(stdscr, TRUE);
                                move((old_y + ((old_x + 2) / COLS)) % LINES, (old_x + 2) % COLS);
                            }
                            break;
                        }
                        /* U+30FB KATAKANA MIDDLE DOT -> 0xFF0E FULLWIDTH FULL STOP */
                        if (my_ucs == 0x30fb)
                        {
                            my_wch = ucs_to_wchar(0xff0e);
                            my_wcw = my_wch ? my_wcwidth(my_wch) : 0;
                            ret = addnwstr(&my_wch, 1);
                            getyx(stdscr, new_y, new_x);
                            if ((old_x != new_x) || (old_y != new_y))
                            {
                                if (CJK_MODE && (my_wcw == 2)
                                    &&
                                    (((old_x + 1) % COLS) == (new_x % COLS)))
                                {
                                }
                                break;
                            }
                        }
                        /* U+301C WAVE DASH -> 0xFF5E FULLWIDTH TILDE */
                        if (my_ucs == 0x301c)
                        {
                            my_wch = ucs_to_wchar(0xff5e);
                            my_wcw = my_wch ? my_wcwidth(my_wch) : 0;
                            ret = addnwstr(&my_wch, 1);
                            getyx(stdscr, new_y, new_x);
                            if ((old_x != new_x) || (old_y != new_y))
                            {
                                if (CJK_MODE && (my_wcw == 2)
                                    &&
                                    (((old_x + 1) % COLS) == (new_x % COLS)))
                                {
                                }
                                break;
                            }
                        }
                    }
                }
                if (altcharset_cp437[b])
                {
                    wchar_t my_wchbuf[CCHARW_MAX];
                    attr_t my_acs_attrs;
                    short my_color_pair;
                    attr_t my_current_attrs;
                    int my_len;

                    my_len =
                        getcchar(
                            MY_WACS_PTR altcharset_cp437[b],
                            NULL,
                            &my_acs_attrs,
                            &my_color_pair,
                            NULL);
                    if (my_len &&
                        (getcchar(
                            MY_WACS_PTR altcharset_cp437[b],
                            my_wchbuf,
                            &my_acs_attrs,
                            &my_color_pair,
                            NULL) != ERR) &&
                        (my_wcswidth(my_wchbuf, my_len) == 1))
                    {
#ifdef _XOPEN_SOURCE_EXTENDED
                        attr_get(
                            & my_current_attrs,
                            & my_color_pair,
                            NULL);
                        attr_set(
                            my_current_attrs | my_acs_attrs,
                            my_color_pair,
                            NULL);
#else
                        my_current_attrs = attr_get();
                        attr_set(my_current_attrs | my_acs_attrs);
#endif
                        ret = addnwstr(
                            my_wchbuf,
                            my_len);
                        getyx(stdscr, new_y, new_x);
                        if (CJK_MODE && ((old_x != new_x) || (old_y != new_y)))
                        {
                            unsigned char rhs;
                        
                            rhs = cp437_fullwidth_rhs[b];
                            if ((int) (unsigned char) rhs)
                            {
                                if (altcharset_cp437[(int) (unsigned char) rhs])
                                {
                                    my_len =
                                        getcchar(
                                            MY_WACS_PTR altcharset_cp437[(int) (unsigned char) rhs],
                                            NULL,
                                            &my_acs_attrs,
                                            &my_color_pair,
                                            NULL);
                                    if (my_len &&
                                        (getcchar(
                                            MY_WACS_PTR altcharset_cp437[(int) (unsigned char) rhs],
                                            my_wchbuf,
                                            &my_acs_attrs,
                                            &my_color_pair,
                                            NULL) != ERR) &&
                                        (my_wcswidth(my_wchbuf, my_len) == 1))
                                    {
                                        addnwstr(
                                            my_wchbuf,
                                            my_len);
                                    }
                                }
                                else
                                {
                                    addch(ascii_cp437[(int) (unsigned char) rhs]);
                                }
                            }
                        }
#ifdef _XOPEN_SOURCE_EXTENDED
                        attr_set(
                            my_current_attrs,
                            my_color_pair,
                            NULL);
#else
                        attr_set(my_current_attrs);
#endif
                        if ((old_x != new_x) || (old_y != new_y))
                        {
                            break;
                        }
                    }
                }
#else /* ! USE_WIDEC_SUPPORT */
                if (use_raw && use_raw_ucs)
                {
                    c = uni_cp437[b];
                    ret = addch(c);
                    getyx(stdscr, new_y, new_x);
                    if ((old_x != new_x) || (old_y != new_y))
                    {
#if USE_WCWIDTH
                        if (CJK_MODE)
                        {
                            wchar_t my_wch;

                            my_wch = ucs_to_wchar(c);
                            if (my_wch && (my_wcwidth(my_wch) < 2))
                            {
                                unsigned char rhs;
                    
                                rhs = cp437_fullwidth_rhs[b];
                                if ((int) (unsigned char) rhs)
                                {
                                    c = uni_cp437[(int) (unsigned char) rhs];
                                    my_wch = ucs_to_wchar(c);
                                    getyx(stdscr, old_y, old_x);
                                    if (my_wch && (my_wcwidth(my_wch) < 2))
                                    {
                                        addch(c);
                                        getyx(stdscr, new_y, new_x);
                                        if ((old_x != new_x) || (old_y != new_y))
                                        {
                                            break;
                                        }
                                    }
                                    c = altcharset_cp437[(int) (unsigned char) rhs];
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                                    if (c & ~A_CHARTEXT)
                                    {
                                        my_attrset(attrs);
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
                                        my_real_attrset(my_attrs);
#endif
#endif /* USE_ATTR || USE_COLOR */
                                    }
#endif
#endif
                                    addch(c);
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                                    if (c & ~A_CHARTEXT)
                                    {
                                        my_attrset(attrs);
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
                                        my_real_attrset(my_attrs);
#endif
#endif /* USE_ATTR || USE_COLOR */
                                    }
#endif
#endif
                                    getyx(stdscr, new_y, new_x);
                                    if ((old_x != new_x) || (old_y != new_y))
                                    {
                                        break;
                                    }
                                    addch(ascii_cp437[(int) (unsigned char) rhs]);
                                }
                            }
                        }
#endif
                        break;
                    }
                }
                c = altcharset_cp437[b];
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                if (c & ~A_CHARTEXT)
                {
                    my_attrset(attrs | (c & ~A_CHARTEXT));
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
                    my_real_attrset(my_attrs);
#endif
#endif /* USE_ATTR || USE_COLOR */
                }
#endif
#endif
                ret = addch(c);
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                if (c & ~A_CHARTEXT)
                {
                    my_attrset(attrs);
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
                    my_real_attrset(my_attrs);
#endif
#endif /* USE_ATTR || USE_COLOR */
                }
#endif
#endif
                getyx(stdscr, new_y, new_x);
                if ((old_x != new_x) || (old_y != new_y))
                {
                    if (CJK_MODE && ((new_x % COLS) != ((old_x + 2) % COLS)))
                    {
                        unsigned char rhs;
                    
                        rhs = (unsigned char) (unsigned) (chtype) cp437_fullwidth_rhs[b];
                        if ((int) (unsigned char) rhs)
                        {
                            c = altcharset_cp437[(int) (unsigned char) rhs];
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                            if (c & ~A_CHARTEXT)
                            {
                                my_attrset(attrs | (c & ~A_CHARTEXT));
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
                                my_real_attrset(my_attrs);
#endif
#endif /* USE_ATTR || USE_COLOR */
                            }
#endif
#endif
                            addch(c);
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                            if (c & ~A_CHARTEXT)
                            {
                                my_attrset(attrs);
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
                                my_real_attrset(my_attrs);
#endif
#endif /* USE_ATTR || USE_COLOR */
                            }
#endif
#endif
                        }
                    }
                    break;
                }
#endif /* ! USE_WIDEC_SUPPORT */
            }
        }
        c = ascii_cp437[b];
        getyx(stdscr, old_y, old_x);
        ret = addch(c);
        getyx(stdscr, new_y, new_x);
        if (CJK_MODE && ((new_x % COLS) != ((old_x + 2) % COLS)))
        {
            unsigned char rhs;

            rhs = cp437_fullwidth_rhs[b];
            if ((int) (unsigned char) rhs)
            {
                addch(ascii_cp437[(int) (unsigned char) rhs]);
            }
        }
    }
    while (0);
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
    if (my_attrs) my_real_attrset(0);
#endif
#endif /* USE_ATTR || USE_COLOR */
    return ret;
}

/* add CP437 string s with attributes attrs */
static int
my_addstr(const char *s, chtype attrs)
{
    size_t i;
    int ret = 0;
    int y, x;

    getyx(stdscr, y, x);
    for (i = 0; s[i]; i ++)
    {
        unsigned long b;

        b = (unsigned long) (unsigned char) s[i];
        move(y, x + i * (CJK_MODE ? 2 : 1));
        ret = my_addch(b, attrs);
        if (ret == ERR)
        {
            break;
        }
    }
    return ret;
}

#ifdef XCURSES
#define XCURSES_USAGE \
" [ -- toolkit-options]"
#else
#define XCURSES_USAGE
#endif

#define SUMMARY(progname) \
"Usage: %s [-h] [options]" \
XCURSES_USAGE \
"\n", progname

const char * pager_notice = 0;
const char * pager_remaining = 0;
/* left then right in the pager is equivalent to space */
int pager_arrow_magic = 0;

#define pager_tile_h (tile_h + 1)
#define pager_big ((((COLS / tile_w) * (LINES / pager_tile_h)) >= 80) && (tile_w >= 4) && ((tile_h >= 3) || ((tile_h) * 2 == tile_w)))
#define PAGER_COLS (pager_big ? (MY_COLS / tile_w) : MY_COLS)
#define PAGER_LINES (pager_big ? (LINES / pager_tile_h) : LINES)

#if USE_ATTR
#ifdef MY_A_REVERSE
#ifndef PAGER_A_REVERSE
#define PAGER_A_REVERSE MY_A_REVERSE
#endif
#endif
#endif

#ifndef PAGER_A_REVERSE
#define PAGER_A_REVERSE 0
#endif

#ifndef PAGER_A_STANDOUT
#define PAGER_A_STANDOUT (((USE_COLOR) && use_color) ? pen[PAUSE_COLOR] : PAGER_A_REVERSE)
#endif

static void
pager_move(int y, int x)
{
    my_move((pager_big ? ((y) * pager_tile_h) : y),
            ((pager_big ? ((x) * tile_w) : x) * (use_fullwidth ? 2 : 1)));
}

#define pager_getyx(stdscr,y,x) do \
{ \
    getyx(stdscr, y, x); \
    if (pager_big) \
    { \
        (y) /= pager_tile_h; \
        (x) /= tile_w; \
    } \
    (x) /= use_fullwidth ? 2 : 1; \
} while(0)

static void
pager_addch(unsigned long c, chtype a)
{
    int pager_addch__x, pager_addch__y, pager_addch__i, pager_addch__j;
    
    pager_getyx(stdscr, pager_addch__y, pager_addch__x);
    if (pager_addch__x >= PAGER_COLS)
    {
        pager_addch__x = 0;
        pager_addch__y ++;
    }
    if (pager_addch__y >= PAGER_LINES)
    {
        pager_move(PAGER_LINES - 1, PAGER_COLS - 1);
    }
    else
    {
        if (pager_big)
        {
            unsigned long pager_addch__c, pager_addch__cc;
            chtype pager_addch__a;
    
            pager_addch__c = (unsigned long) (unsigned char) (c);
            pager_addch__cc = pager_addch__c;
            while ((! tile_used[pager_addch__cc])
                   &&
                   (((unsigned long) (unsigned char) fallback_cp437[pager_addch__cc]) != pager_addch__c)
                   &&
                   (((unsigned long) (unsigned char) fallback_cp437[pager_addch__cc]) != pager_addch__cc))
            {
                pager_addch__cc = (unsigned long) (unsigned char) fallback_cp437[pager_addch__cc];
            }
            pager_addch__c = pager_addch__cc;
            pager_addch__a = (a);
            for (pager_addch__j = 0;
                 pager_addch__j < (((pager_addch__y + 1) == PAGER_LINES)
                                   ?
                                   (LINES - pager_addch__y * pager_tile_h)
                                   :
                                   pager_tile_h);
                 pager_addch__j ++)
            {
                for (pager_addch__i = 0; pager_addch__i < tile_w; pager_addch__i ++)
                {
                    pager_addch__cc = (unsigned long) (unsigned char) ((pager_addch__j < tile_h) ? tile[(pager_addch__c * tile_h + pager_addch__j) * tile_w + pager_addch__i] : ' ');
                    if (! pager_addch__cc) pager_addch__cc = (unsigned long) (unsigned char) ' ';
                    my_move(pager_addch__y * pager_tile_h + pager_addch__j,
                            (pager_addch__x * tile_w + pager_addch__i) * (use_fullwidth ? 2 : 1));
                    my_addch(pager_addch__cc,
                             (pager_addch__j < pager_tile_h)
                             ?
                             pager_addch__a
                             :
                             ((USE_COLOR && use_color)
                              ?
                              pen[TEXT_COLOR]
                              :
                              0));
                }
                if ((pager_addch__x + 1) == PAGER_COLS)
                {
                    for (pager_addch__i = (pager_addch__x + 1) * tile_w; pager_addch__i < MY_COLS; pager_addch__i ++)
                    {
                        my_move(pager_addch__y * pager_tile_h + pager_addch__j,
                                pager_addch__i * (use_fullwidth ? 2 : 1));
                        my_addch((unsigned long) (unsigned char) ' ',
                                 (USE_COLOR && use_color)
                                 ?
                                 pen[TEXT_COLOR]
                                 :
                                 0);
                    }
                }
            }
        }
        else
        {
            my_addch(c, a);
        }
        if ((pager_addch__x + 1) < PAGER_COLS)
        {
            pager_move(pager_addch__y, pager_addch__x + 1);
        }
        else if ((pager_addch__y + 1) < PAGER_LINES)
        {
            pager_move(pager_addch__y + 1, 0);
        }
        else
        {
            pager_move(PAGER_LINES - 1, PAGER_COLS - 1);
        }
    }
}

static void
pager_addstr(const char *s, chtype a)
{
    const char *pager_addstr__s = (s);

    while (*pager_addstr__s)
    {
        pager_addch((unsigned long) (unsigned char) *pager_addstr__s, (a));
        pager_addstr__s ++;
    }
}

void
my_usleep(long usecs)
{
    usleep(usecs);
}

double
doubletime(void)
{
    struct timeval tval;

    tval.tv_sec = 0;
    tval.tv_usec = 0;
    if (myman_gettimeofday(&tval, 0))
    {
        return -1.0L;
    }
    return 1.0L * tval.tv_sec + 1e-6L * tval.tv_usec;
}

static void
pager(void)
{
    int c = ERR;
    int k = ERR;

#if USE_ATTR || USE_COLOR
    my_attrset(0);
#endif
    my_erase();
#if USE_COLOR
    if (use_color)
    {
        my_attrset(pen[TEXT_COLOR]);
    }
#endif
    if (! pager_remaining)
    {
        if (pager_notice)
        {
            pager_remaining = pager_notice;
        }
    }
    while (pager_remaining && (! quit_requested) && (! reinit_requested))
    {
        const char *pager;
        int y, x;

        pager_move(0, 0);
#if USE_COLOR
        if (use_color)
        {
            my_attrset(pen[TEXT_COLOR]);
        }
#endif
        pager = pager_remaining;
        while (*pager)
        {
            c = *pager;
            pager ++;
            pager_getyx(stdscr, y, x);
            if (y && ((y + 1) >= PAGER_LINES) && (c != '\n') && (c != ' '))
            {
                pager --;
                c = '\n';
            }
            if (c == ' ')
            {
                int wlen;

                for (wlen = 0; (pager[wlen] != ' ') && (pager[wlen] != '\n') && (pager[wlen] != '\0'); wlen ++)
                {
                    if ((x + wlen + 1) >= PAGER_COLS)
                    {
                        c = '\n';
                        break;
                    }
                }
                if ((x + 1) >= PAGER_COLS)
                {
                    c = '\n';
                }
                if (c == '\n')
                {
                    while (*pager == ' ')
                    {
                        pager ++;
                    }
                    if (*pager == '\n')
                    {
                        pager ++;
                    }
                }
            }
            else if ((c != '\n')
                     &&
                     (! (((c >= 'A') && (c <= 'Z'))
                         ||
                         ((c >= 'a') && (c <= 'z'))
                         ||
                         ((c >= '0') && (c <= '9'))
                         ||
                         (c == '(')
                         ||
                         (c == '`')
                         ||
                         (c == '\'')
                         ||
                         (c == '"')
                         ||
                         (c == '{')
                         ||
                         (c == '[')
                         ||
                         (c == '<'))))
            {
                int wlen;

                for (wlen = 0;
                     ((pager[wlen] >= 'A') && (pager[wlen] <= 'Z'))
                         ||
                         ((pager[wlen] >= 'a') && (pager[wlen] <= 'z'))
                         ||
                         ((pager[wlen] >= '0') && (pager[wlen] <= '9'))
                         ||
                         (pager[wlen] == '>')
                         ||
                         (pager[wlen] == ']')
                         ||
                         (pager[wlen] == '}')
                         ||
                         (pager[wlen] == '%')
                         ||
                         (pager[wlen] == '\'')
                         ||
                         (pager[wlen] == '"')
                         ||
                         (pager[wlen] == ')')
                         ||
                         (pager[wlen] == '.')
                         ||
                         (pager[wlen] == ',')
                         ||
                         (pager[wlen] == '!')
                         ||
                         (pager[wlen] == '\?')
                         ||
                         (pager[wlen] == ':')
                         ||
                         (pager[wlen] == ';');
                     wlen ++)
                {
                    if ((x + wlen + 1) >= PAGER_COLS)
                    {
                        c = '\n';
                        break;
                    }
                }
                if ((x + 1) >= PAGER_COLS)
                {
                    c = '\n';
                }
                if (c == '\n')
                {
                    pager_addch((unsigned long) (unsigned char) *(pager - 1),
                                (USE_COLOR && use_color)
                                ?
                                pen[TEXT_COLOR]
                                :
                                0
                        );
                    x ++;
                    while (*pager == ' ')
                    {
                        pager ++;
                    }
                    if (*pager == '\n')
                    {
                        pager ++;
                    }
                }
            }
            else if (((x + 2) == PAGER_COLS)
                     &&
                     (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')))
                     &&
                     (((pager[0] >= 'A') && (pager[0] <= 'Z')) || ((pager[0] >= 'a') && (pager[0] <= 'z')))
                     &&
                     (((pager[1] >= 'A') && (pager[1] <= 'Z')) || ((pager[1] >= 'a') && (pager[1] <= 'z'))))
            {
                /* FIXME: this hyphenation rule is really not even
                 * adequate for English. Fortunately, we almost
                 * never actually use it. */
                pager_addch((unsigned long) (unsigned char) c,
                            (USE_COLOR && use_color)
                            ?
                            pen[TEXT_COLOR]
                            :
                            0
                    );
                x ++;
                pager_addch((unsigned long) (unsigned char) '-',
                            (USE_COLOR && use_color)
                            ?
                            pen[TEXT_COLOR]
                            :
                            0
                    );
                x ++;
                c = '\n';
            }
            if (c != '\n')
            {
                pager_getyx(stdscr, y, x);
                pager_addch((unsigned long) (unsigned char) c,
                            (USE_COLOR && use_color)
                            ?
                            pen[TEXT_COLOR]
                            :
                            0
                    );
                pager_getyx(stdscr, y, x);
                while ((x == 0) && (PAGER_COLS > 1) && (*pager == ' '))
                {
                    pager ++;
                }
                if (x == 0)
                {
                    y --;
                    x = PAGER_COLS;
                    c = '\n';
                }
                if (c == '\n')
                {
                    while (*pager == ' ')
                    {
                        pager ++;
                    }
                    if (*pager == '\n')
                    {
                        pager ++;
                    }
                }
            }
            if (c == '\n')
            {
                while ((x ++) < PAGER_COLS)
                {
                    pager_addch((unsigned long) (unsigned char) ' ',
                                (USE_COLOR && use_color)
                                ?
                                pen[TEXT_COLOR]
                                :
                                0
                        );
                }
                y ++;
                x = 0;
                pager_move(y, x);
                if ((y + 1) >= PAGER_LINES)
                {
                    while (*pager == '\n')
                    {
                        pager ++;
                    }
                    if (! *pager)
                    {
                        break;
                    }
                    if (PAGER_LINES > 1)
                    {
                        pager_move(PAGER_LINES - 1, 0);
                        pager_addstr(MOREMESSAGE, PAGER_A_STANDOUT);
                        pager_getyx(stdscr, y, x);
                        while (((x ++) < PAGER_COLS) && (y < PAGER_LINES))
                        {
                            pager_addch((unsigned long) (unsigned char) ' ', PAGER_A_STANDOUT);
                        }
                    }
                    my_refresh();
                    while ((k = my_getch()) == ERR)
                    {
                        my_refresh();
                        if (got_sigwinch) break;
                        my_usleep(100000UL);
                    }
                    if (IS_LEFT_ARROW(k) || (k == '<') || (k == ','))
                    {
                        pager_arrow_magic = 1;
                    }
                    else
                    {
                        if ((pager_arrow_magic == 1) && (IS_RIGHT_ARROW(k) || (k == '>') || (k == '.')))
                        {
                            k = 27;
                        }
                        pager_arrow_magic = 0;
                    }
                    my_attrset(0);
                    my_erase();
#if USE_COLOR
                    if (use_color)
                    {
                        my_attrset(pen[TEXT_COLOR]);
                    }
#endif
                    y = 0;
                    x = 0;
                    pager_move(y, x);
#ifdef KEY_RESIZE
                    if (k == KEY_RESIZE)
                    {
                        reinit_requested = 1;
                        pager = pager_remaining;
                        break;
                    }
                    else
#endif
                        if ((k == '@') || (got_sigwinch && (k == ERR)))
                        {
                            if (got_sigwinch)
                            {
                                use_env(FALSE);
                            }
                            got_sigwinch = 0;
                            reinit_requested = 1;
                            pager = pager_remaining;
                            break;
                        }
                        else if ((k == 'r') || (k == 'R') || (k == MYMANCTRL('L')) || (k == MYMANCTRL('R')))
                        {
                            my_clear();
                            clearok(curscr, TRUE);
                            pager = pager_remaining;
                            break;
                        }
                        else if (k == 27)
                        {
                            pager_arrow_magic = 0;
                            pager_notice = 0;
                            pager_remaining = 0;
                            pager = pager_remaining;
                            break;
                        }
                        else if ((k == 'q') || (k == 'Q') || (k == MYMANCTRL('C')))
                        {
                            quit_requested = 1;
                            pager = pager_remaining;
                            break;
                        }
                        else if ((k == 'a') || (k == 'A'))
                        {
                            use_acs = ! use_acs;
                            use_acs_p = 1;
                            my_clear();
                            clearok(curscr, TRUE);
                            pager = pager_remaining;
                            break;
                        }
#if USE_COLOR
                        else if ((k == 'c') || (k == 'C'))
                        {
                            use_color = ! use_color;
                            use_color_p = 1;
                            if (use_color)
                                init_pen();
                            else
                                destroy_pen();
                            my_attrset(0);
                            my_clear();
                            clearok(curscr, TRUE);
                            pager = pager_remaining;
                            continue;
                        }
#endif
                        else if ((k == 'b') || (k == 'B'))
                        {
                            use_dim_and_bright =
                                ! use_dim_and_bright;
                            use_dim_and_bright_p = 1;
#if USE_COLOR
                            if (use_color)
                            {
                                destroy_pen();
                                init_pen();
                            }
#endif
                            my_attrset(0);
                            my_clear();
                            clearok(curscr, TRUE);
                            pager = pager_remaining;
                            continue;
                        }
                        else if ((k == 'x') || (k == 'X'))
                        {
                            use_raw = ! use_raw;
                            my_clear();
                            clearok(curscr, TRUE);
                            pager = pager_remaining;
                            break;
                        }
                        else if ((k == 'e') || (k == 'E'))
                        {
                            use_raw_ucs = ! use_raw_ucs;
                            my_clear();
                            clearok(curscr, TRUE);
                            pager = pager_remaining;
                            break;
                        }
                        else if (IS_UP_ARROW(k) && (pager_remaining != pager_notice))
                        {
                            if (pager_remaining != pager_notice)
                            {
                                pager_remaining --;
                                while ((pager_remaining != pager_notice)
                                       &&
                                       ((*(pager_remaining - 1)) != '\n'))
                                {
                                    pager_remaining --;
                                }
                            }
                            pager = pager_remaining;
                            break;
                        }
                        else if (IS_DOWN_ARROW(k) || (k == '\r') || (k == '\n'))
                        {
                            while ((pager_remaining != pager)
                                   &&
                                   (*pager_remaining)
                                   &&
                                   ((*pager_remaining) != '\n'))
                            {
                                pager_remaining ++;
                            }
                            if (*pager_remaining == '\n')
                            {
                                pager_remaining ++;
                            }
                            pager = pager_remaining;
                            break;
                        }
                        else if (IS_LEFT_ARROW(k) || IS_RIGHT_ARROW(k))
                        {
                            pager = pager_remaining;
                            break;
                        }
                        else if ((k == MYMANCTRL('@')) && (k != ERR))
                        {
                            /* NUL - idle keepalive (iTerm, maybe others?) */
                            pager = pager_remaining;
                            break;
                        }
                        else if (k == MYMANCTRL('S'))
                        {
                            xoff_received = 1;
                            pager = pager_remaining;
                            break;
                        }
                        else if (k == MYMANCTRL('Q'))
                        {
                            xoff_received = 0;
                            pager = pager_remaining;
                            break;
                        }
                        else if (k == ' ')
                        {
                            pager_remaining = pager;
                            continue;
                        }
                        else if ((k == 's') || (k == 'S'))
                        {
                            use_sound = ! use_sound;
                            pager = pager_remaining;
                            break;
                        }
                        else if (k != ERR)
                        {
#if USE_BEEP
                            if (use_sound) beep();
#endif
                            pager = pager_remaining;
                            break;
                        }
                }
            }
            pager_move(y, x);
        }
        if (pager && (! (*pager)))
        {
            if (PAGER_LINES > 1)
            {
                pager_getyx(stdscr, y, x);
                while ((y + 1) < PAGER_LINES)
                {
                    if (! x)
                    {
                        pager_move(y, x);
                        pager_addch((unsigned long) (unsigned char) '~',
                                    (USE_COLOR && use_color)
                                    ?
                                    pen[TEXT_COLOR]
                                    :
                                    0
                            );
                        x ++;
                        while ((x ++) < PAGER_COLS)
                        {
                            pager_addch((unsigned long) (unsigned char) ' ',
                                        (USE_COLOR && use_color)
                                        ?
                                        pen[TEXT_COLOR]
                                        :
                                        0
                                );
                        }
                    }
                    x = 0;
                    y ++;
                }
                pager_move(y, 0);
                pager_addstr(DONEMESSAGE, PAGER_A_STANDOUT);
                pager_getyx(stdscr, y, x);
                while (((x ++) < PAGER_COLS) && (y < PAGER_LINES))
                {
                    pager_addch((unsigned long) (unsigned char) ' ', PAGER_A_STANDOUT);
                }
            }
            my_refresh();
            while ((k = my_getch()) == ERR)
            {
                my_refresh();
                if (got_sigwinch) break;
                my_usleep(100000UL);
            }
            if (IS_LEFT_ARROW(k) || (k == '<') || (k == ','))
            {
                pager_arrow_magic = 1;
            }
            else
            {
                if ((pager_arrow_magic == 1) && (IS_RIGHT_ARROW(k) || (k == '>') || (k == '.')))
                {
                    k = 27;
                }
                pager_arrow_magic = 0;
            }
#ifdef KEY_RESIZE
            if (k == KEY_RESIZE)
            {
                reinit_requested = 1;
            }
            else
#endif
                if (k == 27)
                {
                    pager_arrow_magic = 0;
                    pager_notice = 0;
                    pager_remaining = 0;
                }
                else if ((k == 'q') || (k == 'Q') || (k == MYMANCTRL('C')))
                {
                    quit_requested = 1;
                }
                else if ((k == 'a') || (k == 'A'))
                {
                    use_acs = ! use_acs;
                    use_acs_p = 1;
                    my_clear();
                    clearok(curscr, TRUE);
                }
#if USE_COLOR
                else if ((k == 'c') || (k == 'C'))
                {
                    use_color = ! use_color;
                    use_color_p = 1;
                    if (use_color)
                        init_pen();
                    else
                        destroy_pen();
                    my_attrset(0);
                    my_clear();
                    clearok(curscr, TRUE);
                }
#endif
                else if ((k == 'b') || (k == 'B'))
                {
                    use_dim_and_bright =
                        ! use_dim_and_bright;
                    use_dim_and_bright_p = 1;
#if USE_COLOR
                    if (use_color)
                    {
                        destroy_pen();
                        init_pen();
                    }
#endif
                    my_attrset(0);
                    my_clear();
                    clearok(curscr, TRUE);
                }
                else if ((k == 'x') || (k == 'X'))
                {
                    use_raw = ! use_raw;
                    my_clear();
                    clearok(curscr, TRUE);
                }
                else if ((k == 'e') || (k == 'E'))
                {
                    use_raw_ucs = ! use_raw_ucs;
                    my_clear();
                    clearok(curscr, TRUE);
                }
                else if (IS_UP_ARROW(k) && (pager_remaining != pager_notice))
                {
                    if (pager_remaining != pager_notice)
                    {
                        pager_remaining --;
                        while ((pager_remaining != pager_notice)
                               &&
                               ((*(pager_remaining - 1)) != '\n'))
                        {
                            pager_remaining --;
                        }
                    }
                }
                else if (IS_DOWN_ARROW(k) || (k == '\r') || (k == '\n'))
                {
                    while ((*pager_remaining)
                           &&
                           ((*pager_remaining) != '\n'))
                    {
                        pager_remaining ++;
                    }
                    if (*pager_remaining == '\n')
                    {
                        pager_remaining ++;
                    }
                }
                else if (IS_LEFT_ARROW(k) || IS_RIGHT_ARROW(k))
                {
                }
                else if ((k == 'r') || (k == 'R') || (k == MYMANCTRL('L')) || (k == MYMANCTRL('R')))
                {
                    my_clear();
                    clearok(curscr, TRUE);
                }
                else if ((k == '@') || (got_sigwinch && (k == ERR)))
                {
                    if (got_sigwinch)
                    {
                        use_env(FALSE);
                    }
                    got_sigwinch = 0;
                    reinit_requested = 1;
                }
                else if ((k == MYMANCTRL('@')) && (k != ERR))
                {
                    /* NUL - idle keepalive (iTerm, maybe others?) */
                }
                else if (k == MYMANCTRL('S'))
                {
                    xoff_received = 1;
                }
                else if (k == MYMANCTRL('Q'))
                {
                    xoff_received = 0;
                }
                else if (k == ' ')
                {
                    pager_arrow_magic = 0;
                    pager_notice = 0;
                    pager_remaining = 0;
                }
                else if ((k == 's') || (k == 'S'))
                {
                    use_sound = ! use_sound;
                }
                else if (k != ERR)
                {
#if USE_BEEP
                    if (use_sound) beep();
#endif
                }
        }
        my_attrset(0);
        my_erase();
#if USE_COLOR
        if (use_color)
        {
            my_attrset(pen[TEXT_COLOR]);
        }
#endif
        my_move(0, 0);
    }
}

int key_buffer = ERR;
int key_buffer_ERR = ERR;

double td = 0.0L;
#if USE_SDL_MIXER
static int sdl_audio_open = 0;
#endif

void
gamesfx(void)
{
#ifdef ALLEGROCURSES
#define handle_sfx(n) \
            do { if (myman_sfx & myman_sfx_ ## n) \
            { \
                static MIDI *n##_music = 0; \
                myman_sfx &= ~myman_sfx_ ## n; \
                if ((use_sound && ! myman_demo) && ! n##_music) \
                { \
                    n##_music = load_midi("sfx/"#n".mid"); \
                } \
                if ((use_sound && ! myman_demo) && n##_music) \
                { \
                    stop_midi(); \
                    play_midi(n##_music, 0); \
                } \
            } } while (0)
#else
#if USE_SDL_MIXER
#define handle_sfx(n) \
            do { if ((myman_sfx & myman_sfx_ ## n) && sdl_audio_open) \
            { \
                static Mix_Music *n##_music = 0; \
                myman_sfx &= ~myman_sfx_ ## n; \
                if ((use_sound && ! myman_demo) && ! n##_music) \
                { \
                    n##_music = Mix_LoadMUS("sfx/"#n".xm"); \
                } \
                if ((use_sound && ! myman_demo) && ! n##_music) \
                { \
                    n##_music = Mix_LoadMUS("sfx/"#n".mid"); \
                } \
                if ((use_sound && ! myman_demo) && n##_music) \
                { \
                    if (! Mix_PlayingMusic()) Mix_PlayMusic(n##_music, 1); \
                } \
            } } while (0)
#else
#if USE_BEEP
#define handle_sfx(n) do { if (myman_sfx & myman_sfx_ ## n) { myman_sfx &= ~myman_sfx_##n; if ((myman_sfx_##n & ~myman_sfx_nobeep_mask) && use_sound && ! myman_demo) beep(); } } while (0)
#else
#define handle_sfx(n) do { if (myman_sfx & myman_sfx_ ## n) { myman_sfx &= ~myman_sfx_##n; } } while (0)
#endif
#endif
#endif
    handle_sfx(credit);
    handle_sfx(dot);
    handle_sfx(dying);
    handle_sfx(ghost);
    handle_sfx(intermission);
    handle_sfx(pellet);
    handle_sfx(siren0_down);
    handle_sfx(siren0_up);
    handle_sfx(siren1_down);
    handle_sfx(siren1_up);
    handle_sfx(siren2_down);
    handle_sfx(siren2_up);
    handle_sfx(start);
    handle_sfx(fruit);
    handle_sfx(life);
    handle_sfx(level);
    handle_sfx(bonus);
    if (myman_sfx)
    {
        myman_sfx = 0UL;
#if USE_BEEP
        if (use_sound && ! myman_demo) beep();
#endif
    }
}

void
gamerender(void)
{
    int i, j;
    long c = 0;
    int x1, y1;
    int r_off, c_off;
    int line, col;
    int vline, vcol;
    int pause_shown;

    pause_shown = 0;
    {
        int s;
        for (s = 0; s < SPRITE_REGISTERS; s ++)
        {
            if (sprite_register_used[s])
            {
                mark_sprite_register(s);
            }
        }
    }
    if (snapshot || snapshot_txt || all_dirty)
    {
        my_erase();
        DIRTY_ALL();
        ignore_delay = 1;
        frameskip = 0;
    }
#define VLINES (reflect ? MY_COLS : LINES)
#define VCOLS (reflect ? LINES : MY_COLS)
#define vmove(y, x) (reflect ? my_move((x), (y) * (use_fullwidth ? 2 : 1)) :  my_move((y), (x) * (use_fullwidth ? 2 : 1)))
    x1 = sprite_register_x[HERO] - VCOLS / 2;
    y1 = sprite_register_y[HERO] - VLINES / 2 - deadpan;
    if (x1 + VCOLS - (reflect ? 1 : 0) > maze_w * gfx_w)
        x1 = maze_w * gfx_w - (VCOLS - (reflect ? 1 : 0));
    if (y1 + VLINES - (reflect ? 0 : 1) > maze_h * gfx_h)
        y1 = maze_h * gfx_h - (VLINES - (reflect ? 0 : 1));
    if (x1 < 0)
        x1 = 0;
    if (y1 < 0)
        y1 = 0;
    r_off = 0;
    c_off = 0;
    if (((gfx_h * maze_h + (reflect ? 0 : (3 * tile_h + sprite_h)))) <= VLINES)
        r_off = (VLINES - (reflect ? 0 : (3 * tile_h + sprite_h)) - gfx_h * maze_h + 1) / 2 + (reflect ? 0 : (3 * tile_h));
    else if (((gfx_h * maze_h + (reflect ? 0 : (2 * tile_h + sprite_h)))) <= VLINES)
        r_off = (VLINES - (reflect ? 0 : (2 * tile_h + sprite_h)) - gfx_h * maze_h + 1) / 2 + (reflect ? 0 : (2 * tile_h));
    else if (gfx_h * maze_h <= VLINES)
        r_off = (VLINES - gfx_h * maze_h + 1) / 2;
    if (r_off < 0) r_off = 0;
    if ((gfx_w * maze_w + (reflect ? (3 * tile_h + sprite_h) : 0)) <= VCOLS)
        c_off = (VCOLS - (reflect ? (3 * tile_h + sprite_h) : 0) - gfx_w * maze_w + 1) / 2 + (reflect ? (3 * tile_h) : 0);
    else if ((gfx_w * maze_w + (reflect ? (2 * tile_h + sprite_h) : 0)) <= VCOLS)
        c_off = (VCOLS - (reflect ? (2 * tile_h + sprite_h) : 0) - gfx_w * maze_w + 1) / 2 + (reflect ? (2 * tile_h) : 0);
    else if (gfx_w * maze_w <= VCOLS)
        c_off = (VCOLS - gfx_w * maze_w + 1) / 2;
    if (c_off < 0) c_off = 0;
#if USE_ATTR || USE_COLOR
    standend();
#if HAVE_ATTRSET
    attrset(0);
#endif
#endif
    for (vline = -(3 * tile_h); (vline < LINES) && (vline < (sprite_h + ((reflect ? (gfx_w * maze_w) : (gfx_h * maze_h))))); vline++)
    {
        if ((vline + (reflect ? c_off : r_off)) < 0) continue;
        if ((vline + (reflect ? c_off : r_off)) >= LINES) continue;
        if ((vline < 0) || (vline >= (reflect ? (gfx_w * maze_w) : (gfx_h * maze_h))))
        {
            for (vcol = 0; (vcol < MY_COLS) && (vcol < (reflect ? (gfx_h * maze_h) : (gfx_w * maze_w))); vcol++)
            {
                int filler_tile = 0;
                chtype a = 0;

                if (snapshot || snapshot_txt || all_dirty)
                {
                    filler_tile = ' ';
                }

                if ((reflect ? c_off : r_off) >= (2 * tile_h))
                {
                    int player_anchor;

                    player_anchor = (reflect ? r_off : c_off) + 7 * tile_w - 1;
                    if (player_anchor >= MY_COLS) player_anchor = MY_COLS - 1;
                    line = vline + (((reflect ? c_off : r_off) >= (3 * tile_h)) ? (3 * tile_h) : (2 * tile_h));
                    if ((line >= 0) && (line < tile_h))
                    {
                        col = (vcol + (reflect ? r_off : c_off));
                        if ((col >= 0) && (col < MY_COLS))
                        {
                            if (col <= player_anchor)
                            {
                                int player_col;
                                int player_tile_x;
                                unsigned char player_tile;
                                int tmp, tmp2;

                                player_tile = 0;
                                player_col = (player_anchor - col) / tile_w;
                                player_tile_x = tile_w - 1 - (player_anchor - col) % tile_w;
                                {
                                    player_tile = (player_col > 3) ? 0 : '0';
                                    if (player_col < 3)
                                    {
                                        const char *msg_up_ = "UP ";

                                        player_tile = msg_up_[2 - player_col];
                                    }
                                    else
                                    {
                                        tmp = player * 1000;
                                        tmp2 = player * 1000;
                                        while (player_col)
                                        {
                                            player_col --;
                                            tmp /= 10;
                                            tmp2 /= 10;
                                        }
                                        if (tmp)
                                        {
                                            player_tile = '0' + (tmp % 10);
                                        }
                                        else if (tmp2)
                                        {
                                            player_tile = ' ';
                                        }
                                    }
                                }
                                if (((unsigned) player_tile)
                                    &&
                                    ! ((! intermission_running)
                                       &&
                                       ((((cycles * 2) % TWOSECS) <= ONESEC)
                                        ||
                                        myman_demo
                                        ||
                                        myman_start
                                        ||
                                        myman_intro)
                                       &&
                                       (0 < (NET_LIVES - ((munched == HERO) && dying && sprite_register_used[HERO])))))
                                {
                                    player_tile = ' ';
                                }
                                if (((unsigned) player_tile)
                                    &&
                                    tile_used[(unsigned) player_tile])
                                {
                                    filler_tile = player_tile;
                                }
                            }
                        }
                    }
                }
                if ((reflect ? c_off : r_off) >= tile_h)
                {
                    int score_anchor;

                    score_anchor = (reflect ? r_off : c_off) + 7 * tile_w - 1;
                    if (score_anchor >= MY_COLS) score_anchor = MY_COLS - 1;
                    line = vline + (((reflect ? c_off : r_off) >= (3 * tile_h)) ? (2 * tile_h) : tile_h);
                    if ((line >= 0) && (line < tile_h))
                    {
                        col = (vcol + (reflect ? r_off : c_off));
                        if ((col >= 0) && (col < MY_COLS))
                        {
                            if ((col <= score_anchor)
                                &&
                                ! intermission_running)
                            {
                                int score_col;
                                int score_tile_x;
                                unsigned char score_tile;
                                int tmp;

                                score_col = (score_anchor - col) / tile_w;
                                score_tile_x = tile_w - 1 - (score_anchor - col) % tile_w;
                                score_tile = (score_col > 1) ? ' ' : '0';
                                tmp = score;
                                while (score_col)
                                {
                                    score_col --;
                                    tmp /= 10;
                                }
                                if (tmp)
                                {
                                    score_tile = '0' + (tmp % 10);
                                }
                                if (tile_used[(unsigned) score_tile] && (' ' != (unsigned) score_tile))
                                {
                                    filler_tile = (unsigned) score_tile;
                                }
                            }
                        }
                    }
                }
                if (filler_tile && tile_used[filler_tile])
                {
#if USE_COLOR
                    if (use_color) {
                        a = tile_color[filler_tile];
                        if (! a) a = TEXT_COLOR;
                        a = pen[a];
                    }
#endif
                    my_move(vline + (reflect ? c_off : r_off), (vcol + (reflect ? r_off : c_off)) * (use_fullwidth ? 2 : 1));
                    my_addch((unsigned long) (unsigned char) tile[(filler_tile * tile_h + ((vline + (3 * tile_h)) % tile_h)) * tile_w + (vcol % tile_w)], a);
                }
            }
            continue;
        }
        if (((reflect ? c_off : r_off) < tile_h)
            &&
            ((reflect ? r_off : c_off) >= (5 * tile_w))
            &&
            (vline < tile_h)
            &&
            (! intermission_running))
        {
            int hud_score_anchor;

            hud_score_anchor = (reflect ? r_off : c_off) - 1;
            for (col = 0; col <= hud_score_anchor; col ++)
            {
                int score_x;
                int score_col;
                int tmp;

                score_x = hud_score_anchor - col;
                score_col = 0;
                tmp = score;
                while (score_col < score_x / tile_w)
                {
                    score_col ++;
                    tmp /= 10;
                }
                if (tmp
                    ||
                    (score_col < 2))
                {
                    unsigned char score_tile;

                    score_tile = (tmp % 10) + '0';
                    if (tile_used[(unsigned) score_tile])
                    {
                        chtype a;

                        a = 0;
#if USE_COLOR
                        if (use_color) {
                            a = tile_color[(unsigned) score_tile];
                            if (! a) a = TEXT_COLOR;
                            a = pen[a];
                        }
#endif
                        my_move(vline + (reflect ? c_off : r_off), col * (use_fullwidth ? 2 : 1));
                        my_addch((unsigned long) (unsigned char) tile[(((unsigned) score_tile) * tile_h + vline) * tile_w + tile_w - 1 - (score_x % tile_w)], a);

                    }
                }
            }
        }
        else if (((reflect ? r_off : c_off) >= sprite_w)
                 &&
                 (! intermission_running)
                 &&
                 (LINES < ((reflect ? c_off : r_off) + (reflect ? (maze_w * gfx_w) : (maze_h * gfx_h)) + sprite_h))
                 &&
                 (LINES >= (tile_h + sprite_h))
                 &&
                 (((vline + sprite_h) >= LINES)
                  ||
                  ((vline + sprite_h) >= (reflect ? (gfx_w * maze_w) : (gfx_h * maze_h)))))
        {
            int hud_line;
            int hud_life_anchor;

            hud_line = vline + sprite_h - ((LINES > (reflect ? (gfx_w * maze_w) : (gfx_h * maze_h))) ? (reflect ? (gfx_w * maze_w) : (gfx_h * maze_h)) : LINES);
            hud_life_anchor = showlives * sprite_w;
            for (col = 0; col < hud_life_anchor; col ++)
            {
                int life_sprite;

                life_sprite = SPRITE_LIFE;
                if (! sprite_used[life_sprite])
                {
                    life_sprite = SPRITE_HERO + 4 + 2;
                }
                if (sprite_used[life_sprite])
                {
                    chtype a;

                    c = (unsigned long) (unsigned char) sprite[(life_sprite * sprite_h + hud_line) * sprite_w + (col % sprite_w)];
                    if (c)
                    {
                        a = 0;
#if USE_COLOR
                        if (use_color) {
                            a = sprite_color[life_sprite];
                            if (! a)
                            {
                                a = sprite_register_color[HERO];
                            }
                            a = pen[a];
                        }
                        else
#endif
                        {
#if USE_ATTR
#ifdef MY_A_BOLD
                            a |= use_dim_and_bright ? MY_A_BOLD : 0;
#endif
#endif
                        }
                        if ((col + (reflect ? r_off : c_off) - hud_life_anchor) >= 0)
                        {
                            my_move(vline + (reflect ? c_off : r_off),
                                    (col + (reflect ? r_off : c_off) - hud_life_anchor) * (use_fullwidth ? 2 : 1));
                            my_addch((unsigned long) (unsigned char) c, a);
                        }
                        continue;
                    }
                }
            }
        }
        for (vcol = 0; (vcol < MY_COLS) && (vcol < (reflect ? (gfx_h * maze_h) : (gfx_w * maze_w))); vcol++) {
            int xtile, ytile;
            int x_off, y_off;
            int s;
            chtype a = 0;
            int is_wall = 0;

            if (reflect)
            {
                line = vcol;
                col = vline;
            }
            else
            {
                line = vline;
                col = vcol;
            }
            a = 0;
            c = 0;
            xtile = XTILE((i = col + x1));
            ytile = YTILE((j = line + y1));
            if (! (line || col))
            {
                int nscrolling;

                nscrolling = (i != scroll_offset_x0) ? 2 : (j != scroll_offset_y0) ? 1 : 0;
                scroll_offset_x0 = i;
                scroll_offset_y0 = j;
                if ((scrolling != nscrolling))
                {
                    if (! nscrolling)
                    {
                        frameskip1 = frameskip;
                        frameskip = (frameskip > frameskip0) ? frameskip0 : frameskip;
                        ignore_delay = 1;
                    }
                    else
                    {
                        frameskip0 = frameskip;
                        frameskip = (frameskip1 > frameskip) ? frameskip1 : frameskip;
                        ignore_delay = 1;
                    }
                    scrolling = nscrolling;
                }
                if (scrolling)
                {
                    DIRTY_ALL();
                }
            }
            if ((paused && ! (snapshot || snapshot_txt))
                &&
                ((vcol + (reflect ? r_off : c_off)) >= (MY_COLS - tile_w * (int) strlen(PAUSE) + 1) / 2)
                &&
                ((vcol + (reflect ? r_off : c_off)) < ((MY_COLS - tile_w * (int) strlen(PAUSE) + 1) / 2 + tile_w * (int) strlen(PAUSE)))
                &&
                (MY_COLS >= tile_w * (int) strlen(PAUSE))
                &&
                ((reflect ? (maze_h * gfx_h) : (maze_w * gfx_w)) >= tile_w * (int) strlen(PAUSE))
                &&
                ((vline + (reflect ? c_off : r_off)) >= (LINES - tile_h + 1) / 2)
                &&
                ((vline + (reflect ? c_off : r_off)) < ((LINES - tile_h + 1) / 2 + tile_h))
                &&
                ((reflect ? (maze_w * gfx_w) : (maze_h * gfx_h)) >= tile_h))
            {
                int pause_x;
                int pause_y;
                unsigned char pause_tile;
                const char *msg_pause = PAUSE;

                pause_shown = 1;
                pause_x = vcol + (reflect ? r_off : c_off) - ((MY_COLS - tile_w * (int) strlen(PAUSE) + 1) / 2);
                pause_y = vline + (reflect ? c_off : r_off) - ((LINES - tile_h + 1) / 2);
                pause_tile = (unsigned long) (unsigned char) msg_pause[pause_x / tile_w];
                if (tile_used[(unsigned) pause_tile])
                {
                    c = (unsigned long) (unsigned char) tile[(msg_pause[pause_x / tile_w] * tile_h + pause_y) * tile_w + (pause_x % tile_w)];
                    if (! c)
                    {
                        c = ' ';
                    }
#if USE_COLOR
                    if (use_color)
                    {
                        a = pen[PAUSE_COLOR];
                    }
                    else
#endif
                    {
                        a = 0;
#if USE_ATTR
#ifdef MY_A_REVERSE
                        a |= MY_A_REVERSE;
#endif
#endif
                    }
                }
            }
            if (IS_CELL_DIRTY(xtile, ytile)
                ||
                ISPELLET((unsigned) (unsigned char) (char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile])
                ||
                winning)
            {
                if (! c)
                {
                    for (s = 0; s < SPRITE_REGISTERS; s ++) {
                        int t, x, y, iseyes;

                        t = ((unsigned) sprite_register[s]) + ((sprite_register_frame[s] < 0) ? (-sprite_register_frame[s]) : sprite_register_frame[s]);
                        iseyes = ((s == GHOSTEYES(UNGHOSTEYES(s)))
                                  &&
                                  (UNGHOSTEYES(s) >= 0)
                                  &&
                                  (UNGHOSTEYES(s) < ghosts));
                        if (debug
                            && sprite_register_used[s]
                            && ((x = sprite_register_x[s]) == i)
                            && ((y = sprite_register_y[s]) == j))
                        {
                            if (iseyes)
                            {
                                const char *msg__uldr = ".^<v>";

                                c = msg__uldr[ghost_mem[UNGHOSTEYES(s)]];
                            }
                            else
                            {
                                c = '.';
                            }
#if USE_COLOR
                            if (use_color) {
                                a = sprite_color[t];
                                if (! a)
                                    a = sprite_register_color[s];
                                a = pen[a];
                            }
                            else
#endif
                            {
#if USE_ATTR
#ifdef MY_A_BOLD
                                if ((s == HERO) || (((unsigned) sprite_register[s]) == SPRITE_WHITE) || iseyes)
                                {
                                    a |= use_dim_and_bright ? MY_A_BOLD : 0;
                                    break;
                                }
#endif
#ifdef MY_A_UNDERLINE
                                if (((unsigned) sprite_register[s]) == SPRITE_BLUE)
                                {
                                    a |= use_underline ? MY_A_UNDERLINE : 0;
                                    break;
                                }
#endif
#endif
                            }
                            break;
                        } else if (sprite_register_used[s]
                                   && sprite_used[t]
                                   && ((x = sprite_register_x[s] - sgfx_w / 2) <= i)
                                   && ((x_off = i - x) < sgfx_w)
                                   && ((y = sprite_register_y[s] - sgfx_h / 2) <= j)
                                   && ((y_off = j - y) < sgfx_h)
                                   && ((c = sgfx(t, y_off, x_off)) != 0)) {
#if USE_COLOR
                            if (use_color) {
                                a = sprite_color[t];
                                if (! a)
                                    a = sprite_register_color[s];
                                a = pen[a];
                            }
                            else
#endif
                            {
#if USE_ATTR
#ifdef MY_A_BOLD
                                if ((s == HERO) || (((unsigned) sprite_register[s]) == SPRITE_WHITE) || iseyes)
                                {
                                    a |= use_dim_and_bright ? MY_A_BOLD : 0;
                                    break;
                                }
#endif
#ifdef MY_A_UNDERLINE
                                if (((unsigned) sprite_register[s]) == SPRITE_BLUE)
                                {
                                    a |= use_underline ? MY_A_UNDERLINE : 0;
                                    break;
                                }
#endif
#endif
                            }
                            break;
                        } else if (sprite_register_used[s]
                                   && (! sprite_used[t])
                                   && ((unsigned) cp437_sprite[t])
                                   && tile_used[(unsigned long) (unsigned char) cp437_sprite[t]]
                                   && ((x = sprite_register_x[s] - gfx_w / 2) <= i)
                                   && ((x_off = i - x) < gfx_w)
                                   && ((y = sprite_register_y[s] - gfx_h / 2) <= j)
                                   && ((y_off = j - y) < gfx_h)
                                   && ((c = gfx((unsigned long) (unsigned char) cp437_sprite[t], y_off, x_off)) != 0))
                        {
#if USE_COLOR
                            if (use_color)
                            {
                                a = tile_color[t];
                                if (! a)
                                    a = sprite_register_color[s];
                                a = pen[a];
                            }
                            else
#endif
                            {
#if USE_ATTR
#ifdef MY_A_BOLD
                                if ((s == HERO) || (((unsigned) sprite_register[s]) == SPRITE_WHITE) || iseyes)
                                {
                                    a |= use_dim_and_bright ? MY_A_BOLD : 0;
                                    break;
                                }
#endif
#ifdef MY_A_UNDERLINE
                                if (((unsigned) sprite_register[s]) == SPRITE_BLUE)
                                {
                                    a |= use_underline ? MY_A_UNDERLINE : 0;
                                    break;
                                }
#endif
#endif
                            }
                            break;
                        }
                    }
                }
                if ((! c) && (ytile < maze_h) && (xtile < maze_w)) {
                    c = maze_visual(maze_level, ytile, xtile);
                    {
                        int c_mapped;

                        c_mapped = c;
                        if (c_mapped == ':')
                        {
                            c_mapped = ' ';
                        }
                        else if (c_mapped == 'l')
                        {
                            c_mapped = 179;
                        }
                        else if (c_mapped == '~')
                        {
                            c_mapped = 196;
                        }
                        else if ((c_mapped == 'o') && (! tile_used[c_mapped]))
                        {
                            c_mapped = 254;
                        }
                        while ((! tile_used[c_mapped])
                               &&
                               (((int) (unsigned) fallback_cp437[c_mapped]) != c)
                               &&
                               (((int) (unsigned) fallback_cp437[c_mapped]) != c_mapped))
                        {
                            c_mapped = (unsigned long) (unsigned char) fallback_cp437[c_mapped];
                        }
                        if (tile_used[c_mapped]) {
                            if ((ISWALL(c) && ! ISDOOR(c)) || (c == ' '))
                            {
                                is_wall = 1;
                            }
#if USE_COLOR
                            if (use_color)
                            {
                                a = (int) (unsigned char) maze_color[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile];
                                if (! a)
                                {
                                    a = tile_color[c_mapped];
                                }
                                if (! a)
                                {
                                    if (ISPELLET(c))
                                    {
                                        a = PELLET_COLOR ? PELLET_COLOR : DOT_COLOR;
                                    }
                                    else if (ISDOT(c))
                                    {
                                        a = DOT_COLOR;
                                    }
                                    else if (is_wall)
                                    {
                                        a = EFFECTIVE_MORTAR_COLOR;
                                    }
                                    else if (ISTEXT(c))
                                    {
                                        a = TEXT_COLOR;
                                    }
                                }
                                a = pen[a];
                            }
                            else
#endif
                            {
#if USE_ATTR
#ifdef MY_A_BOLD
                                if (ISPELLET(c))
                                    a |= use_dim_and_bright ? MY_A_BOLD : 0;
#endif
#ifdef MY_A_UNDERLINE
                                if (use_underline)
                                    if (ISWALL(c) && (! ISDOOR(c)))
                                        a |= MY_A_UNDERLINE;
#endif
#endif
                            }
                            if (debug) {
                                int s = WHOSE_HOME_DIR(ytile, xtile);
                                unsigned char d;
                                            
                                d = home_dir[(s % ghosts*maze_h+ytile)*(maze_w+1)+xtile];
                                c = (((unsigned) d) == MYMAN_UP) ? '^'
                                    : (((unsigned) d) == MYMAN_DOWN) ? 'v'
                                    : (((unsigned) d) == MYMAN_LEFT) ? '<'
                                    : (((unsigned) d) == MYMAN_RIGHT) ? '>'
                                    : ISDOT(c) ? ','
                                    : ISPELLET(c) ? ';'
                                    : ISOPEN(c) ? ' '
                                    : ISDOOR(c) ? 'X'
                                    : '@';
#if USE_COLOR
                                if (use_color && ((unsigned) d)) {
                                    a = sprite_color[((unsigned) sprite_register[MEANGHOST(s)]) + sprite_register_frame[MEANGHOST(s)]];
                                    if (! a)
                                        a = sprite_register_color[MEANGHOST(s)];
                                    a = pen[a];
                                }
#endif
                            }
                            else
                            {
                                if ((ISPELLET(c) && ((cycles / MYMANFIFTH) & 4) && (! dead) &&
                                     ! (sprite_register_used[HERO] && ghost_eaten_timer)) ||
                                    ((winning < ONESEC) && winning && (! myman_intro) && (! intermission_running) && (! myman_start) &&
                                     (ISDOT(c) || ISPELLET(c) || (((winning / MYMANFIFTH) & 4) && ISDOOR(c)))))
                                {
                                    is_wall = 0;
                                    c_mapped = ' ';
                                }
                                else if ((winning < (2 * TWOSECS)) && ((winning / MYMANFIFTH) & 4) && ! ghost_eaten_timer)
                                {
                                    is_wall = 0;
#if USE_COLOR
                                    if (use_color)
                                        a = pen[0xF];
                                    else
#endif
                                        c_mapped = ' ';
                                }
                                c = gfx(c_mapped, j, i);
                                if ((SOLID_WALLS || SOLID_WALLS_BGCOLOR)
                                    &&
                                    TRANSLATED_WALL_COLOR
                                    &&
                                    is_wall
                                    &&
                                    (! (myman_intro || myman_start || intermission_running))
                                    &&
                                    (! IS_FULLY_NON_INVERTED(xtile, ytile)))
                                {
                                    if (SOLID_WALLS
                                        &&
                                        (((c == ' ') && (IS_FULLY_INVERTED(xtile, ytile) || (! IS_INVERTED(xtile, ytile))))
                                         ||
                                         ((c == '\0') && (IS_FULLY_INVERTED(xtile, ytile) || IS_INVERTED(xtile, ytile)))))
                                    {
                                        if (! SOLID_WALLS_BGCOLOR)
                                        {
                                            c = '\xdb';
                                        }
                                        else
                                        {
                                            c = ' ';
                                        }
#if USE_COLOR
                                        if (use_color)
                                        {
                                            if (TRANSLATED_WALL_COLOR)
                                            {
                                                a = pen[(((unsigned long) (unsigned char) TRANSLATED_WALL_COLOR) * (SOLID_WALLS_BGCOLOR ? 16 : 1)) % 256];
                                            }
                                        }
                                        else
#endif
                                        {
#if USE_ATTR
#ifdef MY_A_REVERSE
                                            if (SOLID_WALLS_BGCOLOR)
                                            {
                                                a |= MY_A_REVERSE;
                                            }
#endif
#ifdef MY_A_UNDERLINE
                                            if (use_underline)
                                                a |= MY_A_UNDERLINE;
#endif
#endif
                                        }
                                    }
                                    else if ((c_mapped != ' ')
                                             &&
                                             (! ISNONINVERTABLE(c_mapped))
                                             &&
                                             SOLID_WALLS_BGCOLOR
                                             &&
                                             (((c != '\0') && (IS_FULLY_INVERTED(xtile, ytile) || (! IS_INVERTED(xtile, ytile))))
                                              ||
                                              ((c != ' ') && (IS_FULLY_INVERTED(xtile, ytile) || IS_INVERTED(xtile, ytile)))))
                                    {
#if USE_COLOR
                                        if (use_color)
                                        {
                                            if (TRANSLATED_WALL_COLOR)
                                            {
                                                a = pen[(((unsigned long) (unsigned char) EFFECTIVE_MORTAR_COLOR)
                                                         |
                                                         (((unsigned long) (unsigned char) TRANSLATED_WALL_COLOR) * 16)) % 256];
                                            }
                                        }
                                        else
#endif
                                        {
#if USE_ATTR
#ifdef MY_A_REVERSE
                                            if (SOLID_WALLS_BGCOLOR)
                                            {
                                                a |= MY_A_REVERSE;
                                            }
#endif
#ifdef MY_A_UNDERLINE
                                            if (use_underline)
                                                a |= MY_A_UNDERLINE;
#endif
#endif
                                        }
                                    }
                                }
                            }
                        }
                        else
                            c = ' ';
                    }
                }
                if (! c)
                    c = ' ';
            }
            if (c)
            {
                vmove(line + r_off, c_off + col);
#if USE_ATTR
#ifdef MY_A_UNDERLINE
#if USE_COLOR
                if (! use_color)
#endif
                    if (use_underline
                        &&
                        (a & MY_A_UNDERLINE)
                        &&
#ifdef MY_A_REVERSE
                        (! (a & MY_A_REVERSE))
                        &&
#endif
                        (c == ' '))
                    {
                        a &= ~MY_A_UNDERLINE;
                    }
#endif
#endif
                my_addch((unsigned long) (unsigned char) c, a);
            }
        }
        if (((MY_COLS - (reflect ? (r_off + maze_h * gfx_h) : (c_off + maze_w * gfx_w))) >= sprite_w)
            &&
            (LINES >= (tile_h + sprite_h))
            &&
            (LINES < ((reflect ? c_off : r_off) + (reflect ? (maze_w * gfx_w) : (maze_h * gfx_h)) + sprite_h))
            &&
            (((vline + sprite_h) >= LINES)
             ||
             ((vline + sprite_h) >= (reflect ? (gfx_w * maze_w) : (gfx_h * maze_h)))))
        {
            int hud_line;
            int hud_level_anchor;
            int hud_level_anchor2;

            hud_line = vline + sprite_h - ((LINES > (reflect ? (gfx_w * maze_w) : (gfx_h * maze_h))) ? (reflect ? (gfx_w * maze_w) : (gfx_h * maze_h)) : LINES);
            hud_level_anchor = reflect ? (r_off + maze_h * gfx_h) : (c_off + maze_w * gfx_w);
            hud_level_anchor2 = hud_level_anchor + sprite_w * ((level > 7) ? 7 : level) - 1;
            if (hud_level_anchor2 >= MY_COLS)
            {
                hud_level_anchor2 = MY_COLS - 1;
            }
            for (col = hud_level_anchor; col <= hud_level_anchor2; col ++)
            {
                int level_sprite;
                int level_x;

                level_x = col - hud_level_anchor;
                level_sprite = SPRITE_FRUIT + BONUS(level - (level_x / sprite_w));
                if (sprite_used[level_sprite] && ! myman_demo)
                {
                    chtype a;

                    c = (unsigned long) (unsigned char) sprite[(level_sprite * sprite_h + hud_line) * sprite_w + (level_x % sprite_w)];
                    if (c)
                    {
                        a = 0;
#if USE_COLOR
                        if (use_color) {
                            a = sprite_color[level_sprite];
                            if (! a)
                            {
                                a = sprite_register_color[FRUIT];
                            }
                            a = pen[a];
                        }
#endif
                        my_move(vline + (reflect ? c_off : r_off), col * (use_fullwidth ? 2 : 1));
                        my_addch((unsigned long) (unsigned char) c, a);
                        continue;
                    }
                }
            }
        }
    }
    if (LINES >= ((reflect ? c_off : r_off) + (reflect ? (maze_w * gfx_w) : (maze_h * gfx_h)) + sprite_h))
    {
        int life_anchor;
        int level_anchor;
        int level_anchor2;

        life_anchor = showlives * sprite_w + (reflect ? r_off : c_off) + 2 * tile_w - 1;
        level_anchor2 = (reflect ? r_off : c_off) + ((reflect ? r_off : c_off) ? (reflect ? (maze_h * gfx_h) : (maze_w * gfx_w)) : MY_COLS) - 2 * tile_w - 1;
        level_anchor = level_anchor2 + 1 - ((level > 7) ? 7 : level) * sprite_w;
        while ((level_anchor <= life_anchor)
               &&
               ((level_anchor + 2 * sprite_w - 1) <= level_anchor2))
        {
            level_anchor += sprite_w;
        }
        while ((life_anchor >= level_anchor)
               &&
               ((life_anchor + 1 - (reflect ? r_off : c_off) - 2 * sprite_w) >= 2 * tile_w))
        {
            life_anchor -= sprite_w;
        }
        for (line = 0; line < sprite_h; line ++)
        {
            for (col = 0; col < MY_COLS; col ++)
            {
                if ((col - (reflect ? r_off : c_off) >= (2 * tile_w))
                    &&
                    (col <= life_anchor)
                    &&
                    (! intermission_running))
                {
                    int life_sprite;

                    my_move(line + (reflect ? c_off : r_off) + (reflect ? (maze_w * gfx_w) : (maze_h * gfx_h)), col * (use_fullwidth ? 2 : 1));
                    life_sprite = SPRITE_LIFE;
                    if (! sprite_used[life_sprite])
                    {
                        life_sprite = SPRITE_HERO + 4 + 2;
                    }
                    if (sprite_used[life_sprite])
                    {
                        chtype a;

                        c = (unsigned long) (unsigned char) sprite[(life_sprite * sprite_h + line) * sprite_w + ((col - (reflect ? r_off : c_off) - 2 * tile_w) % sprite_w)];
                        if (c)
                        {
                            a = 0;
#if USE_COLOR
                            if (use_color) {
                                a = sprite_color[life_sprite];
                                if (! a)
                                {
                                    a = sprite_register_color[HERO];
                                }
                                a = pen[a];
                            }
                            else
#endif
                            {
#if USE_ATTR
#ifdef MY_A_BOLD
                                a |= use_dim_and_bright ? MY_A_BOLD : 0;
#endif
#endif
                            }
                            my_addch((unsigned long) (unsigned char) c, a);
                            continue;
                        }
                    }
                } else if ((col <= level_anchor2)
                           &&
                           (col >= level_anchor))
                {
                    int level_sprite;
                    int level_x;

                    my_move(line + (reflect ? c_off : r_off) + (reflect ? (maze_w * gfx_w) : (maze_h * gfx_h)), col * (use_fullwidth ? 2 : 1));
                    level_x = col - level_anchor;
                    level_sprite = SPRITE_FRUIT + BONUS(level - (level_x / sprite_w));
                    if (sprite_used[level_sprite] && ! myman_demo)
                    {
                        chtype a;

                        c = (unsigned long) (unsigned char) sprite[(level_sprite * sprite_h + line) * sprite_w + (level_x % sprite_w)];
                        if (c)
                        {
                            a = 0;
#if USE_COLOR
                            if (use_color) {
                                a = sprite_color[level_sprite];
                                if (! a)
                                {
                                    a = sprite_register_color[FRUIT];
                                }
                                a = pen[a];
                            }
#endif
                            my_addch((unsigned long) (unsigned char) c, a);
                            continue;
                        }
                    }
                }
            }
        }
    }
    my_attrset(0);
    if (debug)
    {
        my_move(0, 0);
        for (i = 0; i < MAXFRAMESKIP; i ++)
        {
            if (i <= frameskip)
            {
                my_addstr("\xdb", 0);
            }
            else
            {
                my_addstr(" ", 0);
            }
        }
    }
    if (sprite_register_used[FRUIT] && (LINES > 6) && ! use_sound) {
        static char msg[8][12] = {" <  <N>  > ",
                                  "<  <ONU>  >",
                                  "  <BONUS>  ",
                                  " < BONUS > ",
                                  "<  BONUS  >",
                                  " > BONUS < ",
                                  "  >BONUS<  ",
                                  ">  >ONU<  <"};

        my_move(LINES - 1, 1 * (use_fullwidth ? 2 : 1));
        my_addstr(msg[(cycles / MYMANFIFTH) & 7], 0);
    }
    if ((! myman_demo)
        &&
        (! myman_intro)
        &&
        (! myman_start)
        &&
        ((LINES > 6) && (MY_COLS > 46))
        &&
        (((reflect ? c_off : r_off) < tile_h)
         ||
         ((LINES - ((reflect ? c_off : r_off) + (reflect ? (maze_w * gfx_w) : (maze_h * gfx_h)))) < sprite_h))
        &&
        ((LINES < (tile_h + sprite_h))
         ||
         ((reflect ? r_off : c_off) < (5 * tile_w))
         ||
         ((reflect ? r_off : c_off) < sprite_w)
         ||
         ((MY_COLS - (reflect ? (r_off + maze_h * gfx_h) : (c_off + maze_w * gfx_w))) < sprite_w)))
    {
        static char buf[128];

        sprintf(buf,
                " Level: %-10u Lives: %d Score: %-10u ",
                level, NET_LIVES, score);
        my_move(LINES - 1, (MY_COLS - 46) * (use_fullwidth ? 2 : 1));
        my_addstr(buf, 0);
    }
    if (paused && ! (snapshot || snapshot_txt || pause_shown))
    {
        standout();
        mvprintw(LINES / 2, ((COLS - (int) strlen(PAUSE)) & ~(use_fullwidth ? 1 : 0)) / 2, PAUSE);
        standend();
    }
    {
        int was_inverted;

        was_inverted = snapshot || snapshot_txt;
        my_refresh();
        if (was_inverted)
        {
            DIRTY_ALL();
            ignore_delay = 1;
            frameskip = 0;
        }
        else
        {
            CLEAN_ALL();
        }
    }
    {
        int s;
        for (s = 0; s < SPRITE_REGISTERS; s ++)
        {
            if (sprite_register_used[s])
            {
                mark_sprite_register(s);
            }
        }
    }
}

int
gameinput(void)
{
    int k;
    int hero_can_move_left = 0;
    int hero_can_move_right = 0;
    int hero_can_move_up = 0;
    int hero_can_move_down = 0;
    unsigned char m1, m2;
    int xtile, ytile, x_off, y_off;

    x_off = sprite_register_x[HERO] % gfx_w;
    y_off = sprite_register_y[HERO] % gfx_h;
    xtile = XTILE(sprite_register_x[HERO]);
    ytile = YTILE(sprite_register_y[HERO]);
    while (1)
    {
        double td_pre, td_post;

        td_pre = doubletime();
        k = my_getch();
        td_post = doubletime();
        /* a very slow keypress is likely a sign of unmapping, suspending, or some similar mess */

        /* TODO: treat job control signals similarly */
        if ((td_post - td_pre) >= 1.0)
        {
            ignore_delay = 1;
            frameskip = 0;
        }
        m1 = (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile - NOTRIGHT(x_off))];
        m2 = (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile];
        hero_can_move_left =
            ISOPEN((unsigned) m1)
            ||
            ISZAPLEFT((unsigned) m2);
        m1 = (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile + NOTLEFT(x_off))];
        m2 = (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile];
        hero_can_move_right =
            ISOPEN((unsigned) m1)
            ||
            ISZAPRIGHT((unsigned) m2);
        m1 = (unsigned char) maze[(maze_level*maze_h+YWRAP(ytile - NOTBOTTOM(y_off))) * (maze_w + 1)+xtile];
        m2 = (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile];
        hero_can_move_up =
            ISOPEN((unsigned) m1)
            ||
            ISZAPUP((unsigned) m2);
        m1 = (unsigned char) maze[(maze_level*maze_h+YWRAP(ytile + NOTTOP(y_off))) * (maze_w + 1)+xtile];
        m2 = (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile];
        hero_can_move_down =
            ISOPEN((unsigned) m1)
            ||
            ISZAPDOWN((unsigned) m2);
#ifdef KEY_RESIZE
        if (k == KEY_RESIZE)
        {
            k = '@';
        }
#endif
        if ((k == 'q') || (k == 'Q') || (k == MYMANCTRL('C')) || quit_requested)
        {
            quit_requested = 0;
            return 0;
        }
        else if ((k == MYMANCTRL('@')) && (k != ERR))
        {
            /* NUL - idle keepalive (iTerm, maybe others?) */
            return 1;
        }
        else if (k == MYMANCTRL('S'))
        {
            xoff_received = 1;
            return 1;
        }
        else if (k == MYMANCTRL('Q'))
        {
            xoff_received = 0;
            return 1;
        }
        else if (k == '!')
        {
            gameinfo();
            return 0;
        }
        else if ((k == '\?') || (k == MYMANCTRL('H')))
        {
            gamehelp();
            return 0;
        } else if ((k == '@') || (got_sigwinch && (k == ERR)))
        {
            if (got_sigwinch)
            {
                use_env(FALSE);
            }
            got_sigwinch = 0;
            reinit_requested = 1;
            return 0;
        } else if ((k == 'r') || (k == 'R') || (k == MYMANCTRL('L')) || (k == MYMANCTRL('R'))) {
            my_clear();
            clearok(curscr, TRUE);
            DIRTY_ALL();
            wrefresh(stdscr);
            ignore_delay = 1;
            frameskip = 0;
            return 1;
        } else if ((k == 'i') || (k == 'I'))
        {
            use_idlok = ! use_idlok;
#ifndef DISABLE_IDLOK
            if (use_idlok)
            {
                idlok(stdscr, TRUE);
            }
            else
            {
                idlok(stdscr, FALSE);
            }
#endif
#if USE_COLOR
        } else if ((k == 'c') || (k == 'C')) {
            use_color = ! use_color;
            use_color_p = 1;
            if (use_color)
                init_pen();
            else
                destroy_pen();
            my_attrset(0);
            my_clear();
            clearok(curscr, TRUE);
            DIRTY_ALL();
            ignore_delay = 1;
            frameskip = 0;
            return 1;
#endif
        } else if ((k == 'b') || (k == 'B')) {
            use_dim_and_bright =
                ! use_dim_and_bright;
            use_dim_and_bright_p = 1;
#if USE_COLOR
            if (use_color)
            {
                destroy_pen();
                init_pen();
            }
#endif
            my_attrset(0);
            my_clear();
            clearok(curscr, TRUE);
            DIRTY_ALL();
            ignore_delay = 1;
            frameskip = 0;
            return 1;
#if USE_ATTR
        } else if ((k == 'u') || (k == 'U')) {
            use_underline = ! use_underline;
            my_clear();
            clearok(curscr, TRUE);
            DIRTY_ALL();
            ignore_delay = 1;
            frameskip = 0;
            return 1;
#endif
        } else if ((k == 's') || (k == 'S')) {
            use_sound = ! use_sound;
            return 1;
        } else if ((k == 'o') || (k == 'O') || (k == '0'))
        {
            use_bullet_for_dots = ! use_bullet_for_dots;
            use_bullet_for_dots_p = 1;
            init_trans(use_bullet_for_dots);
            my_clear();
            clearok(curscr, TRUE);
            DIRTY_ALL();
            ignore_delay = 1;
            frameskip = 0;
            return 1;
        } else if ((k == 'a') || (k == 'A'))
        {
            use_acs = ! use_acs;
            use_acs_p = 1;
            my_clear();
            clearok(curscr, TRUE);
            DIRTY_ALL();
            ignore_delay = 1;
            frameskip = 0;
            return 1;
        } else if ((k == 'x') || (k == 'X'))
        {
            use_raw = ! use_raw;
            my_clear();
            clearok(curscr, TRUE);
            DIRTY_ALL();
            ignore_delay = 1;
            frameskip = 0;
            return 1;
        } else if ((k == '/') || (k == '\\'))
        {
            reflect = ! reflect;
            my_clear();
            clearok(curscr, TRUE);
            DIRTY_ALL();
            ignore_delay = 1;
            frameskip = 0;
            if (IS_LEFT_ARROW(key_buffer)) key_buffer = KEY_UP;
            else if (IS_UP_ARROW(key_buffer)) key_buffer = KEY_LEFT;
            else if (IS_RIGHT_ARROW(key_buffer)) key_buffer = KEY_DOWN;
            else if (IS_DOWN_ARROW(key_buffer)) key_buffer = KEY_RIGHT;
            return 1;
        } else if ((k == 'e') || (k == 'E'))
        {
            use_raw_ucs = ! use_raw_ucs;
            my_clear();
            clearok(curscr, TRUE);
            DIRTY_ALL();
            ignore_delay = 1;
            frameskip = 0;
            return 1;
        } else if ((k == 't') || (k == 'T'))
        {
            char buf[128];
            char buf_txt[128];
            unsigned int idx;

            if ((! snapshot)
                &&
                (! snapshot_txt))
            {
                /* try to find a free slot */
                for (idx = 0; idx <= 9999; idx ++)
                {
                    sprintf(buf,
                            "snap%4.4u%s",
                            idx, HTM_SUFFIX);
                    sprintf(buf_txt,
                            "snap%4.4u%s",
                            idx, TXT_SUFFIX);
                    if (access(buf, F_OK) && access(buf_txt, F_OK))
                    {
                        break;
                    }
                }
                snapshot = fopen(buf, "wb");
                snapshot_txt = fopen(buf_txt, "wb");
                snapshot_use_color = use_color;
            }
            return 1;
        } else if ((k == 'p') || (k == 'P') || (k == 27)) {
            if (paused)
            {
                DIRTY_ALL();
            }
            paused = ! paused;
            ignore_delay = 1;
            frameskip = 0;
            continue;
        } else if ((k == ',') || (k == '<')) {
            if (reflect ? (IS_LEFT_ARROW(key_buffer) || IS_RIGHT_ARROW(key_buffer)) : (IS_UP_ARROW(key_buffer) || IS_DOWN_ARROW(key_buffer)))
            {
                if (reflect ? ((hero_dir != MYMAN_LEFT) && (hero_dir != MYMAN_RIGHT)) : ((hero_dir != MYMAN_UP) && (hero_dir != MYMAN_DOWN)))
                {
                    key_buffer = ERR;
                }
            }
        } else if ((k == '.') || (k == '>')) {
            if (reflect ? (IS_UP_ARROW(key_buffer) || IS_DOWN_ARROW(key_buffer)) : (IS_LEFT_ARROW(key_buffer) || IS_RIGHT_ARROW(key_buffer)))
            {
                if (reflect ? ((hero_dir != MYMAN_UP) && (hero_dir != MYMAN_DOWN)) : ((hero_dir != MYMAN_LEFT) && (hero_dir != MYMAN_RIGHT)))
                {
                    key_buffer = ERR;
                }
            }
        } else if ((k == 'w') || (k == 'W')) {
            dots = total_dots[maze_level] - 1;
            continue;
        } else if ((k == 'd') || (k == 'D')) {
            debug = ! debug;
            DIRTY_ALL();
            ignore_delay = 1;
            frameskip = 0;
            continue;
        } else if ((reflect ? IS_UP_ARROW(((k == ERR) ? key_buffer : k)) : IS_LEFT_ARROW(((k == ERR) ? key_buffer : k)))
                   && hero_can_move_left)
        {
            if (! (winning || dying || (dead && ! ghost_eaten_timer)))
            {
                hero_dir = MYMAN_LEFT;
                sprite_register[HERO] = SPRITE_HERO + 4;
            }
        } else if ((reflect ? IS_DOWN_ARROW(((k == ERR) ? key_buffer : k)) : IS_RIGHT_ARROW(((k == ERR) ? key_buffer : k)))
                   && hero_can_move_right)
        {
            if (! (winning || dying || (dead && ! ghost_eaten_timer)))
            {
                hero_dir = MYMAN_RIGHT;
                sprite_register[HERO] = SPRITE_HERO + 12;
            }
        } else if ((reflect ? IS_LEFT_ARROW(((k == ERR) ? key_buffer : k)) : IS_UP_ARROW(((k == ERR) ? key_buffer : k)))
                   && hero_can_move_up)
        {
            if (! (winning || dying || (dead && ! ghost_eaten_timer)))
            {
                hero_dir = MYMAN_UP;
                sprite_register[HERO] = SPRITE_HERO;
            }
        } else if ((reflect ? IS_RIGHT_ARROW(((k == ERR) ? key_buffer : k)) : IS_DOWN_ARROW(((k == ERR) ? key_buffer : k)))
                   && hero_can_move_down)
        {
            if (! (winning || dying || (dead && ! ghost_eaten_timer)))
            {
                hero_dir = MYMAN_DOWN;
                sprite_register[HERO] = SPRITE_HERO + 16;
            }
        }
        if (k == ERR)
        {
            k = key_buffer;
        }
        else if (! ignore_delay)
        {
            if (paused)
            {
                DIRTY_ALL();
                ignore_delay = 1;
                frameskip = 0;
            }
            paused = 0;
            key_buffer = k;
            continue;
        }
        break;
    }
    return (k == ERR) ? -1 : -2;
}

static void
myman(void)
{
#ifdef SLANG_VERSION
#if SLANG_VERSION >= 20000
    SLutf8_enable(-1);
    SLtt_utf8_enable(1);
    SLsmg_utf8_enable(1);
    SLinterp_utf8_enable(1);
#endif
#endif
    do
    {
#if USE_SDL_MIXER
        SDL_Init(SDL_INIT_EVERYTHING);
        if ((! sdl_audio_open) &&
            (! Mix_OpenAudio(44100, AUDIO_S16, 1, 4096)))
        {
            sdl_audio_open = 1;
        }
#endif
        if (! myman_lines) myman_lines = (reflect ? (maze_w * gfx_w) : (maze_h * gfx_h)) + (3 * tile_h + sprite_h);
        if (! myman_columns) myman_columns = (reflect ? (maze_h * gfx_h) : (maze_w * gfx_w)) * (use_fullwidth ? 2 : 1);
#ifdef GTKCURSES
        if (((! myman_getenv("GTKCURSES_ICON")) || ! *(myman_getenv("GTKCURSES_ICON"))) && MYMANICONPNG && *MYMANICONPNG)
        {
            myman_setenv("GTKCURSES_ICON", MYMANICONPNG);
        }
#endif
#ifdef INITSCR_WITH_HINTS
        initscrWithHints(myman_lines,
                         myman_columns,
                         "MyMan [" MYMAN " " MYMANVERSION "]",
                         MYMAN);
#else
#if defined(SLCURSES) || defined(__PDCURSES__)
        if (! reinit_requested)
#endif
        {
#ifdef XCURSES
            if (! Xinitscr(argc, argv))
            {
                perror("Xinitscr");
                fflush(stderr);
                exit(1);
            }
#else
            if (! initscr())
            {
                perror("initscr");
                fflush(stderr);
                exit(1);
            }
#endif
#ifdef __PDCURSES__
#ifdef PDC_BUILD
#if PDC_BUILD >= 2400
            PDC_set_title("MyMan [" MYMAN " " MYMANVERSION "]");
#endif
#endif
            use_default_colors();
#else
#ifdef NCURSES_VERSION
            use_default_colors();
#endif
#endif
        }
#endif
        my_clear();
        cbreak();
        noecho();
        nonl();
#if HAVE_NODELAY
        nodelay(stdscr, TRUE);
#endif
        intrflush(stdscr, FALSE);
        my_attrset(0);
#if HAVE_CURS_SET
        curs_set(0);
#endif
#if USE_KEYPAD
        keypad(stdscr, TRUE);
#endif
#ifndef DISABLE_IDLOK
        if (use_idlok)
        {
            idlok(stdscr, TRUE);
        }
        else
        {
            idlok(stdscr, FALSE);
        }
#endif
        leaveok(stdscr, TRUE);
        if (! use_bullet_for_dots_p)
        {
            use_bullet_for_dots = SWAPDOTS;
        }
        if (! use_dim_and_bright_p)
        {
            use_dim_and_bright = USE_DIM_AND_BRIGHT;
        }
        if (! use_acs_p)
        {
            use_acs = USE_ACS;
        }
        init_trans(use_bullet_for_dots);
#if USE_COLOR
#if COLORIZE
        if (! use_color_p) {
            use_color = has_colors();
            use_color_p = 1;
        }
#endif
#ifdef SLCURSES
        if (! reinit_requested)
#endif
        {
            start_color();
        }
        if (use_color)
            init_pen();
#endif
#if USE_SIGWINCH
        old_sigwinch_handler = signal(SIGWINCH, sigwinch_handler);
#endif
        reinit_requested = 0;
        pager();
        old_lines = 0;
        old_cols = 0;
        old_score = 0;
        old_showlives = 0;
        old_level = 0;
        while (! reinit_requested)
        {
            if (! gamecycle(LINES, COLS))
            {
                break;
            }
        }
#if USE_SIGWINCH
        if (old_sigwinch_handler) signal(SIGWINCH, old_sigwinch_handler);
        else signal(SIGWINCH, SIG_DFL);
#endif
        my_attrset(0);
#if HAVE_CURS_SET
        curs_set(1); /* slcurses doesn't do this in endwin() */
#endif
        my_clear();
#if USE_COLOR
        if (use_color)
        {
            standout();
            mvprintw(LINES ? 1 : 0, 0, " ");
            standend();
            refresh();
            destroy_pen();
            mvprintw(LINES ? 1 : 0, 0, " ");
            addch('\n');
        }
#endif
        refresh();
        echo();
        endwin();
        if (reinit_requested)
        {
            refresh();
            {
#if USE_IOCTL
#ifdef TIOCGWINSZ
                struct winsize wsz;
#endif
#ifdef TIOCGSIZE
                struct ttysize tsz;
#endif


#ifdef TIOCGWINSZ
                if (! ioctl(fileno(stdout), TIOCGWINSZ, &wsz))
                {
                    myman_lines = wsz.ws_row;
                    myman_columns = wsz.ws_col;
                }
                else
#endif
#ifdef TIOCGSIZE
                if (! ioctl(fileno(stdout), TIOCGSIZE, &tsz))
                {
                    myman_lines = tsz.ts_lines;
                    myman_columns = tsz.ts_cols;
                }
                else
#endif
                {
                    myman_lines = LINES;
                    myman_columns = COLS;
                }
#endif
                if (! myman_lines) myman_lines = LINES;
                if (! myman_columns) myman_columns = COLS;
#ifdef KEY_RESIZE
#ifdef __PDCURSES__
                resize_term(0, 0);
#else
                resizeterm(myman_lines ? myman_lines : LINES, myman_columns ? myman_columns : COLS);
#endif
#else
                {
                    static char buf[32];

                    sprintf(buf, "%d", myman_lines);
                    myman_setenv("LINES", buf);
                    sprintf(buf, "%d", myman_columns);
                    myman_setenv("COLUMNS", buf);
                }
#endif
            }
        }
    } while (reinit_requested);
    fprintf(stderr, "%s: scored %d points\n",
            progname, score);
#ifdef XCURSES
    XCursesExit();
#endif
#if USE_ICONV
    if (cd_to_wchar != (iconv_t) -1)
    {
        iconv_close(cd_to_wchar);
        cd_to_wchar = (iconv_t) -1;
    }
    if (cd_to_uni != (iconv_t) -1)
    {
        iconv_close(cd_to_uni);
        cd_to_uni = (iconv_t) -1;
    }
#endif
}

void
usage(const char *mazefile, const char *spritefile, const char *tilefile)
{
    printf("Usage: %s [options]" XCURSES_USAGE "\n", progname);
    puts("-h \tdisplay this help and exit");
    puts("-b \tenable sounds");
    puts("-q \tdisable sounds");
#if USE_COLOR
    puts("-c \tenable color support");
#else
    puts("-c \tenable color support (must recompile first)");
#endif
    puts("-n \tdisable color support");
    puts("-B \tuse dim and bright attributes for missing colors");
    puts("-N \tdon't use dim and bold attributes for extra colors");
    puts("-o \tuse 'o' for power pellets and bullet for dots");
    puts("-p \tuse bullet for power pellets and '.' for dots");
#if MYMANDELAY
    puts("-d NUM \tdelay NUM microseconds/refresh");
#else
    puts("-d NUM \tdelay NUM microseconds/refresh (must recompile first)");
#endif
    puts("-D NAME \tdefine environment variable NAME with value 1");
    puts("-D NAME=VALUE \tdefine environment variable NAME with value VALUE");
    puts("-g NUM \tplay against NUM monsters");
    puts("-l NUM \tstart with NUM lives");
#if USE_ATTR
    puts("-u \tuse the underline attribute for maze walls");
#else
    puts("-u \tuse the underline attribute for maze walls (must recompile first)");
#endif
    puts("-U \tdon't use the underline attribute for maze walls");
    puts("-r \tuse raw tile characters (CP437 or UCS/Unicode character graphics)");
    puts("-R \tuse altcharset translations (VT100-style graphics)");
    puts("-e \tuse UCS/Unicode for internal representation of raw tile characters");
    puts("-E \tuse CP437 for internal representation of raw tile characters");
    puts("-a \tuse ASCII for altcharset translation");
    puts("-A \tuse your terminal's altcharset translations");
    puts("-i \tscroll vertically by redrawing the screen");
    puts("-I \tuse your terminal's vertical scrolling capabilities");
    puts("-1 \tuse default UCS/Unicode mapping");
    puts("-2 \tuse CJK (fullwidth) UCS/Unicode mapping");
    puts("-m FILE \tuse the maze in FILE");
    puts("-s FILE \tuse sprites from FILE");
    puts("-t FILE \tuse tiles from FILE");
    puts("-M \twrite the maze to stdout in C format and exit");
    puts("-S \twrite sprites to stdout in C format and exit");
    puts("-T \twrite tiles to stdout in C format and exit");
    puts("-f FILE \tredirect stdout to FILE (append)");
    puts("-F FILE \tredirect stdout to FILE (truncate)");
    puts("-x \treflect maze diagonally, exchanging the upper right and lower left corners");
    puts("-X \tdo not reflect maze");
    printf("Defaults:");
    printf(use_raw ? " -r" : " -R");
    printf(use_raw_ucs ? " -e" : " -E");
    printf(use_acs_p ? (use_acs ? " -A" : " -a") : "");
    printf(use_idlok ? " -I" : " -i");
    printf(use_sound ? " -b" : " -q");
    printf(use_color ? " -c" : " -n");
    printf(use_dim_and_bright_p ? (use_dim_and_bright ? " -B" : " -N") : "");
    printf(use_bullet_for_dots_p ? (use_bullet_for_dots ? " -o" : " -p") : "");
    printf(use_underline ? " -u" : " -U");
    printf(use_fullwidth ? " -2" : " -1");
    printf(reflect ? " -x" : " -X");
    printf(" -d %lu -l %d -m \"",
           mymandelay ? mymandelay : 0,
           lives);
    if (mazefile)
        mymanescape(mazefile, strlen(mazefile));
    else {
        printf("(");
        mymanescape(builtin_mazefile, strlen(builtin_mazefile));
        printf(")");
    }
    printf("\" -s \"");
    if (spritefile)
        mymanescape(spritefile, strlen(spritefile));
    else {
        printf("(");
        mymanescape(builtin_spritefile, strlen(builtin_spritefile));
        printf(")");
    }
    printf("\" -t \"");
    if (tilefile)
        mymanescape(tilefile, strlen(tilefile));
    else {
        printf("(");
        mymanescape(builtin_tilefile, strlen(builtin_tilefile));
        printf(")");
    }
    printf("\"\n");
}

static void
parse_myman_args(int argc, char **argv)
{
    int i;
    int
        dump_maze = 0,
        dump_sprite = 0,
        dump_tile = 0;
    const char *tilefile = TILEFILE;
    const char *spritefile = SPRITEFILE;
    const char *mazefile = MAZEFILE;
    int option_index;
    const char *defvariant = MYMANVARIANT;
    const char *defsize = MYMANSIZE;
    unsigned long uli;

    while ((i = getopt_long(argc, argv, short_options,
                            long_options, &option_index))
           != -1)
        switch (i) {
        case 'V':
            printf("%s-%s (%s) %s\n%s\n",
                   MYMANVARIANT,
                   MYMANSIZE,
                   MYMAN,
                   MYMANVERSION,
                   MYMANCOPYRIGHT);
            fflush(stdout), fflush(stderr), exit(0);
        case 'v':
            defvariant = optarg;
            break;
        case 'z':
            defsize = optarg;
            break;
        case 'b':
            use_sound = 1;
            break;
        case 'q':
            use_sound = 0;
            break;
	case 'i':
            use_idlok = 0;
            break;
	case 'I':
            use_idlok = 1;
            break;
        case 'r':
            use_raw = 1;
            break;
        case 'R':
            use_raw = 0;
            break;
        case 'e':
            use_raw_ucs = 1;
            break;
        case 'E':
            use_raw_ucs = 0;
            break;
        case 'a':
            use_acs = 0;
            use_acs_p = 1;
            break;
        case 'A':
            use_acs = 1;
            use_acs_p = 1;
            break;
        case 'x':
            reflect = 1;
            break;
        case 'X':
            reflect = 0;
            break;
        case 'd':
        {
            char garbage;

            if (sscanf(optarg, "%lu%c", &uli, &garbage) != 1)
            {
                fprintf(stderr,
                        "%s: argument to -d must be an unsigned long integer.\n",
                        progname);
                fflush(stderr), exit(1);
            }
            mymandelay = uli;
            mindelay = mymandelay / 2;
            break;
        }
        case 'D':
        {
            char *name;
            const char *value;

            value = "1";
            name = strdup(optarg);
            if (! name)
            {
                perror("strdup");
                fflush(stderr), exit(1);
            }
            if (strchr(name, '='))
            {
                *(strchr(name, '=')) = '\0';
                value = name + strlen(name) + 1;
            }
            if (myman_setenv(name, value))
            {
                perror("setenv");
                fflush(stderr), exit(1);
            }
            {
                const char *check_value;

                check_value = myman_getenv(name);
                if ((! check_value) || strcmp(check_value, value))
                {
                    fprintf(stderr, "setenv: did not preserve value, %s=%s vs %s=%s\n",
                            name, value,
                            name, check_value ? check_value : "(null)");
                    fflush(stderr), exit(1);
                }
            }
            free((void *) name);
            break;
        }
        case 'g':
        {
            const char *tmp_ghosts_endp = NULL;

            maze_GHOSTS = strtollist(optarg, &tmp_ghosts_endp, &maze_GHOSTS_len);
            if (! maze_GHOSTS)
            {
                perror("-g");
                fflush(stderr), exit(1);
            }
            else if (tmp_ghosts_endp && *tmp_ghosts_endp)
            {
                fprintf(stderr,
                        "%s: -g: garbage after argument: %s\n",
                        progname,
                        tmp_ghosts_endp);
                fflush(stderr), exit(1);
            }
            ghosts_p = 1;
            break;
        }
        case 'l':
        {
            char garbage;

            if (sscanf(optarg, "%lu%c", &uli, &garbage) != 1)
            {
                fprintf(stderr,
                        "%s: argument to -l must be an unsigned integer.\n",
                        progname);
                fflush(stderr), exit(1);
            }
            lives = (int) uli;
            break;
        }
        case 'h':
            usage(mazefile, spritefile, tilefile);
            fflush(stdout), fflush(stderr), exit(0);
            break;
        case 'k':
            printf("%s", MYMANKEYS);
            fflush(stdout), fflush(stderr), exit(0);
            break;
        case 'L':
            printf("%s", MYMANLEGALNOTICE);
            fflush(stdout), fflush(stderr), exit(0);
            break;
        case 'u':
            use_underline = 1;
            break;
        case 'U':
            use_underline = 0;
            break;
        case 'M':
            dump_maze = 1;
            nogame = 1;
            break;
        case 'S':
            dump_sprite = 1;
            nogame = 1;
            break;
        case 'T':
            dump_tile = 1;
            nogame = 1;
            break;
        case 'm':
#ifdef BUILTIN_MAZE
            if ((*optarg == '(') &&
                (strlen(optarg) == (strlen(builtin_mazefile) + 2)) &&
                (optarg[strlen(optarg) - 1] == ')') &&
                ! strncmp(optarg + 1, builtin_mazefile, strlen(builtin_mazefile))) {
                mazefile = 0;
                break;
            }
#endif
            mazefile = optarg;
            break;
        case 'n':
            use_color_p = 1;
            use_color = 0;
            break;
        case 'o':
            use_bullet_for_dots = 1;
            use_bullet_for_dots_p = 1;
            break;
        case 'p':
            use_bullet_for_dots = 0;
            use_bullet_for_dots_p = 1;
            break;
        case '2':
            use_fullwidth = 1;
            break;
        case '1':
            use_fullwidth = 0;
            break;
        case 'c':
            use_color_p = 1;
            use_color = 1;
            break;
        case 'B':
            use_dim_and_bright = 1;
            use_dim_and_bright_p = 1;
            break;
        case 'N':
            use_dim_and_bright = 0;
            use_dim_and_bright_p = 1;
            break;
        case 't':
#ifdef BUILTIN_TILE
            if ((*optarg == '(') &&
                (strlen(optarg) == (strlen(builtin_tilefile) + 2)) &&
                (optarg[strlen(optarg) - 1] == ')') &&
                ! strncmp(optarg + 1, builtin_tilefile, strlen(builtin_tilefile))) {
                tilefile = 0;
                break;
            }
#endif
            tilefile = optarg;
            break;
        case 's':
#ifdef BUILTIN_SPRITE
            if ((*optarg == '(') &&
                (strlen(optarg) == (strlen(builtin_spritefile) + 2)) &&
                (optarg[strlen(optarg) - 1] == ')') &&
                ! strncmp(optarg + 1, builtin_spritefile, strlen(builtin_spritefile))) {
                spritefile = 0;
                break;
            }
#endif
            spritefile = optarg;
            break;
        case 'f':
            if (freopen(optarg, "a", stdout) == NULL)
            {
                perror(optarg);
                fflush(stderr), exit(1);
            }
            break;
        case 'F':
            if (freopen(optarg, "w", stdout) == NULL)
            {
                perror(optarg);
                fflush(stderr), exit(1);
            }
            break;
        default:
            fprintf(stderr, SUMMARY(progname));
            fflush(stderr), exit(2);
        }
    if (myman_getenv("MYMAN_DEBUG") && *(myman_getenv("MYMAN_DEBUG")) && strcmp(myman_getenv("MYMAN_DEBUG"), "0"))
    {
        debug = atoi(myman_getenv("MYMAN_DEBUG"));
        debug = debug ? debug : 1;
    }
#ifdef XCURSES
    argv[optind - 1] = progname;
    argc -= optind;
    argc ++;
    argv += optind;
    argv --;
#else
    if (optind < argc)
    {
        fprintf(stderr, SUMMARY(progname));
        fflush(stderr), exit(2);
    }
#endif
    if (strcmp(defvariant, MYMANVARIANT))
    {
        fprintf(stderr,
                "%s: game variant `%s' not included in this binary\n",
                progname,
                defvariant);
        exit(2);
    }
    if (strcmp(defsize, MYMANSIZE))
    {
        fprintf(stderr,
                "%s: game size `%s' not included in this binary\n",
                progname,
                defsize);
        exit(2);
    }

#if ! USE_ATTR
    if (use_underline)
    {
        fprintf(stderr,
                "%s: compile with -DUSE_ATTR=1 to enable the -u option.\n",
                progname);
        fflush(stderr), exit(1);
    }
#endif
#if ! USE_COLOR
    if (use_color)
    {
        fprintf(stderr,
                "%s: compile with -DUSE_COLOR=1 to enable color support.\n",
                progname);
        fflush(stderr), exit(1);
    }
#endif
    if ((tilefile && readfont(tilefile, &tile_w, &tile_h, &tile, tile_used, &tile_flags, tile_color, &tile_args)) ||
        (spritefile && readfont(spritefile, &sprite_w, &sprite_h, &sprite, sprite_used, &sprite_flags, sprite_color, &sprite_args)))
        exit(1);

    if (tile_args)
        if (parse_tile_args(tilefile ? tilefile : builtin_tilefile,
                            tile_args))
        {
            exit(1);
        }

    if (sprite_args)
        if (parse_sprite_args(spritefile ? spritefile : builtin_spritefile,
                              sprite_args))
        {
            exit(1);
        }

    gfx_reflect = reflect && ! REFLECT_LARGE;

#if ! MYMANDELAY
    if (mymandelay)
    {
        fprintf(stderr,
                "%s: compile with -DMYMANDELAY=1 to enable the -d option.\n",
                progname);
        fflush(stderr), exit(1);
    }
#else
    mymandelay = mymandelay / MYMANFIFTH;
    mindelay = mymandelay / 2;
#endif

#ifdef BUILTIN_MAZE
    if (! mazefile)
    {
        maze = (char *) malloc(maze_n * maze_h * (maze_w + 1) * sizeof(*maze));
        if (! maze)
        {
            perror("malloc");
            exit(1);
        }
        memcpy((void *) maze,
               (void *) maze_data,
               maze_n * maze_h * (maze_w + 1));
        maze_color = (char *) malloc(maze_n * maze_h * (maze_w + 1) * sizeof(*maze_color));
        if (! maze_color)
        {
            perror("malloc");
            exit(1);
        }
        memcpy((void *) maze_color,
               (void *) maze_color_data,
               maze_n * maze_h * (maze_w + 1));
    }
#endif /* defined(BUILTIN_MAZE) */
    if (mazefile && readmaze(mazefile, &maze_n, &maze_w, &maze_h, &maze, &maze_flags, &maze_color, &maze_args))
    {
        exit(1);
    }
    if (maze_args)
        if (parse_maze_args(mazefile ? mazefile : builtin_mazefile,
                            maze_args))
        {
            exit(1);
        }

    msglen = MAX(MAX(strlen(msg_PLAYER1), strlen(msg_PLAYER2)), MAX(strlen(msg_READY), strlen(msg_GAMEOVER)));
    total_dots = (int *) malloc(maze_n * sizeof(*total_dots));
    if (! total_dots)
    {
        perror("malloc");
        exit(1);
    }
    memset((void *) total_dots, 0, maze_n * sizeof(*total_dots));
    pellets = (int *) malloc(maze_n * sizeof(*pellets));
    if (! pellets)
    {
        perror("malloc");
        exit(1);
    }
    memset((void *) pellets, 0, maze_n * sizeof(*pellets));
    blank_maze = (char *) malloc(maze_n * maze_h * (maze_w + 1) * sizeof(*blank_maze));
    if (! blank_maze)
    {
        perror("malloc");
        exit(1);
    }
    memset((void *) blank_maze, 0, maze_n * maze_h * (maze_w + 1) * sizeof(*blank_maze));
    blank_maze_color = (char *) malloc(maze_n * maze_h * (maze_w + 1) * sizeof(*blank_maze_color));
    if (! blank_maze_color)
    {
        perror("malloc");
        exit(1);
    }
    memset((void *) blank_maze_color, 0, maze_n * maze_h * (maze_w + 1) * sizeof(*blank_maze_color));
    inside_wall = (unsigned short *) malloc(maze_n * maze_h * (maze_w + 1) * sizeof(*inside_wall));
    if (! inside_wall)
    {
        perror("malloc");
        exit(1);
    }
    memset((void *) inside_wall, 0, maze_n * maze_h * (maze_w + 1) * sizeof(*inside_wall));
    dirty_cell = (unsigned char *) malloc(maze_h * ((maze_w + 1 + 7) >> 3) * sizeof(*dirty_cell));
    if (! dirty_cell)
    {
        perror("malloc");
        exit(1);
    }
    memset((void *) dirty_cell, 0, maze_h * ((maze_w + 1 + 7) >> 3) * sizeof(*dirty_cell));
    home_dir = (unsigned char *) malloc(MAXGHOSTS * maze_h * (maze_w + 1) * sizeof(*home_dir));
    if (! home_dir)
    {
        perror("malloc");
        exit(1);
    }
    memset((void *) home_dir, 0, MAXGHOSTS * maze_h * (maze_w + 1) * sizeof(*home_dir));
    memcpy((void *)blank_maze,
           (void *)maze,
           (maze_w + 1) * maze_h * maze_n * sizeof(unsigned char));
    memcpy((void *)blank_maze_color,
           (void *)maze_color,
           (maze_w + 1) * maze_h * maze_n * sizeof(unsigned char));

    CLEAN_ALL();
    paint_walls(isatty(fileno(stderr)));
    gamereset();

    if (dump_maze)
        writemaze(mazefile ? mazefile : builtin_mazefile);

    if (dump_sprite)
        writefont(spritefile ? spritefile : builtin_spritefile,
                  "sprite", sprite_w, sprite_h, sprite, sprite_used, sprite_flags, sprite_color, sprite_args);
    if (dump_tile)
        writefont(tilefile ? tilefile : builtin_tilefile,
                  "tile", tile_w, tile_h, tile, tile_used, tile_flags, tile_color, tile_args);
}

int
main(int argc, char *argv[]
#ifndef MAIN_NO_ENVP
     , char *envp[]
#endif
    )
{
    int i;
    long c = 0;

#ifndef MAIN_NO_ENVP
    if (envp)
    {
        /* we should care */
    }
#endif
    progname = (argc > 0) ? argv[0] : "";
    pager_notice = MYMANLEGALNOTICE;
#ifdef MACCURSES
#ifdef __CARBON__
    /* when launched as a CFM application under Mac OS X, there is no
     * argv[0], so we jump through a few hoops to figure out what it
     * should have been. */
    if ((argc == 0)
        ||
        ((argc == 2) && (! strncmp(argv[1], "-psn_", strlen("-psn_")))))
    {
        ProcessSerialNumber psn;

        if (noErr == MacGetCurrentProcess(&psn))
        {
            FSSpec processAppSpec;
            ProcessInfoRec pir;

            memset((void *) &pir, 0, sizeof(pir));
            pir.processInfoLength = sizeof(pir);
            pir.processAppSpec = &processAppSpec;
            if (noErr == GetProcessInformation(&psn, &pir))
            {
                FSRef location;

                if (noErr == FSpMakeFSRef(&processAppSpec, &location))
                {
                    static UInt8 path[256];

                    if (noErr == FSRefMakePath(&location, path, sizeof(path) - 1))
                    {
                        progname = (char *) path;
                    }
                }
            }
        }
    }
    /* when launched as a native application under Mac OS X, there may
     * be a bogus process serial number parameter. */
    if ((argc == 2) && (! strncmp(argv[1], "-psn_", strlen("-psn_"))))
    {
        argc = 1;
    }
#endif /* defined(__CARBON__) */
#endif /* defined(MACCURSES) */
    progname = (progname && *progname) ? progname : MYMAN;
    td = 0.0L;
    for (i = 0; i < SPRITE_REGISTERS; i ++) {
        sprite_register_used[i] = 0;
        sprite_register_frame[i] = 0;
        sprite_register_color[i] = 0x7;
    }
    for (i = 0; i < 256; i ++) {
#ifndef BUILTIN_TILE
        tile_color[i] = 0x7;
#endif
#ifndef BUILTIN_SPRITE
        sprite_color[i] = 0x7;
#endif
    }
    parse_myman_args(argc, argv);

    for (i = 0; i < 256; i ++)
    {
        int c_mapped;

        c = (unsigned long) (unsigned char) cp437_sprite[i];
        c_mapped = c;
        while (c_mapped
               &&
               (! tile_used[c_mapped])
               &&
               (((int) (unsigned) fallback_cp437[c_mapped]) != c)
               &&
               (((int) (unsigned) fallback_cp437[c_mapped]) != c_mapped))
        {
            c_mapped = (unsigned long) (unsigned char) fallback_cp437[c_mapped];
            cp437_sprite[i] = (unsigned char) c_mapped;
        }
    }

    if (nogame)
        fflush(stdout), fflush(stderr), exit(0);

#ifdef LC_CTYPE
    if (! setlocale(LC_CTYPE, ""))
    {
        fprintf(
            stderr,
            "warning: setlocale(LC_CTYPE, \"\") failed\n");
        fflush(stderr);
    }
#endif /* defined(LC_CTYPE) */

    if (use_fullwidth)
    {
        uni_cp437 = uni_cp437_fullwidth;
    }
    myman();
    return 0;
}
#ifdef END_OF_MAIN
END_OF_MAIN()
#endif
