/*BINFMTC:-lcurses -I../inc
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

#undef DATADIR /* FIXME: conflicts with Win32 header files */

/* base name for package (without version number or suffix) */
#ifndef MYMAN
#define MYMAN "myman"
#endif

/* copyright information for the program as a whole */
#ifndef MYMANCOPYRIGHT
#define MYMANCOPYRIGHT "Copyright 1997-2008, Benjamin C. Wiley Sittler <bsittler@gmail.com>"
#endif

/* package version number */
#ifndef MYMANVERSION
#define MYMANVERSION "devel"
#endif

/* notice to display on the intro screen */
#ifndef MYMANNOTICE
#define MYMANNOTICE "DEDICATED TO TORU"
#endif

/* legal notice to display before starting */
#ifndef MYMANLEGALNOTICE
#define MYMANLEGALNOTICE \
"LEGAL NOTICE" "\n" \
"\n" \
"MyMan is an unofficial and unlicensed clone of the original Pac-Man" " " \
"and Puckman games and of their sequels and imitators, and is not" " " \
"endorsed by Namco Bandai Games Inc., owners of the Pac-Man and Puckman" " " \
"registered trademarks and copyrights. Any trademarks used herein are" " " \
"the property of their respective owners. No mention of a trademark," " " \
"trademark owner, or other party shall be construed as an endorsement" " " \
"of MyMan or any other product by any party. If you believe that by" " " \
"using or posessing MyMan the rights of others would be infringed, you" " " \
"are strongly encouraged to cease using MyMan and its derivatives and" " " \
"delete all copies of the MyMan software and its derivatives, and to" " " \
"inform the author or authors of MyMan and the distributor or" " " \
"distributors from whom you obtained MyMan of the reasons for such" " " \
"belief so that the infringing part may be removed or replaced. Any" " " \
"imitation by MyMan of other software is intended to be purely for" " " \
"purposes of humor and amusement." "\n" \
"\n" \
MYMAN " - The MyMan video game" "\n" \
MYMANCOPYRIGHT "\n" \
"\n" \
"Permission is hereby granted, free of charge, to any person" " " \
"obtaining a copy of this software and associated documentation" " " \
"files (the \"Software\"), to deal in the Software without" " " \
"restriction, including without limitation the rights to use, copy," " " \
"modify, merge, publish, distribute, sublicense, and/or sell copies" " " \
"of the Software, and to permit persons to whom the Software is" " " \
"furnished to do so, subject to the following conditions:" "\n" \
"\n" \
"The above copyright notice and this permission notice shall be" " " \
"included in all copies or substantial portions of the Software." "\n" \
"\n" \
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND," " " \
"EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF" " " \
"MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND" " " \
"NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT" " " \
"HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY," " " \
"WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM," " " \
"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER" " " \
"DEALINGS IN THE SOFTWARE." "\n" \
"\n" \
"Author contact information:" "\n" \
"\n" \
"Benjamin C. W. Sittler <bsittler@gmail.com>" "\n" \
"2806 Foothill Boulevard" "\n" \
"Oakland, California 94601" "\n" \
"U.S.A." "\n" \
"\n" \
"Press \? or Ctrl-H during the game for help and additional notices." "\n" \
""
#endif

/* used for HTML screenshots */
#ifndef HTM_SUFFIX
#define HTM_SUFFIX ".html"
#endif

/* used for text screenshots */
#ifndef TXT_SUFFIX
#define TXT_SUFFIX ".txt"
#endif 

#ifndef maze_ABOUT_prefix
#define maze_ABOUT_prefix "\n" \
"Maze: "
#endif

#ifndef maze_FIXME_prefix
#define maze_FIXME_prefix "\n" \
"FIXME (Maze): "
#endif

#ifndef maze_NOTE_prefix
#define maze_NOTE_prefix "\n" \
"NOTE (Maze): "
#endif

#ifndef tile_ABOUT_prefix
#define tile_ABOUT_prefix "\n" \
"Tiles: "
#endif

#ifndef tile_FIXME_prefix
#define tile_FIXME_prefix "\n" \
"FIXME (Tiles): "
#endif

#ifndef tile_NOTE_prefix
#define tile_NOTE_prefix "\n" \
"NOTE (Tiles): "
#endif

#ifndef sprite_ABOUT_prefix
#define sprite_ABOUT_prefix "\n" \
"Sprites: "
#endif

#ifndef sprite_FIXME_prefix
#define sprite_FIXME_prefix "\n" \
"FIXME (Sprites): "
#endif

#ifndef sprite_NOTE_prefix
#define sprite_NOTE_prefix "\n" \
"NOTE (Sprites): "
#endif

#ifndef MYMANKEYS_prefix
#define MYMANKEYS_prefix "Keyboard Map" "\n" \
"\n"
#endif

#ifndef MYMANKEYS
#define MYMANKEYS \
"The following case-insensitive keystroke commands are recognized during" " " \
"the game:" "\n" \
"Q or Ctrl-C: quit the game" "\n" \
"P or ESC: pause the game" "\n" \
"Ctrl-Q: re-enable output (after Ctrl-S)" "\n" \
"R, Ctrl-L or Ctrl-R: refresh (redraw) the screen" "\n" \
"@: reset the display subsystem and refresh (redraw) the screen" "\n" \
"Ctrl-S: inhibit output until Ctrl-Q is typed" "\n" \
"S: toggle sound on/off" "\n" \
"W: warp to the next level (after consuming the next dot)" "\n" \
"C: toggle color on/off (if enabled at compile-time)" "\n" \
"B: toggle use of dim and bold attributes for missing colors" "\n" \
"U: toggle underlining of walls on/off (if enabled at compile-time)" "\n" \
"D: toggle maze debugging on/off" "\n" \
"T: save an HTML screenshot to the file snap####" HTM_SUFFIX ", where #### is" " " \
"a four-digit sequence number, and flash the screen briefly; a" " " \
"plain text version is saved as snap####" TXT_SUFFIX " using backspacing" " " \
"to represent underlined and bold text" "\n" \
"O or 0: toggle appearance of power pellets and dots" "\n" \
"A: toggle between ASCII altcharset translations and your terminal\'s" " " \
"altcharset translations" "\n" \
"I: toggle between using your terminal\'s vertical scrolling capabilities" " " \
"and redrawing the screen" "\n" \
"X: toggle between raw tile characters (CP437 or UCS/Unicode character" " " \
"graphics) and altcharset translations (VT100-style graphics)" "\n" \
"E: toggle between UCS/Unicode and CP437 for internal representation" " " \
"of raw tile characters" "\n" \
"H, 4, Ctrl-B or LEFT: move left" "\n" \
"J, 2, Ctrl-N or DOWN: move down" "\n" \
"K, 8, Ctrl-P or UP: move up" "\n" \
"L, 6, Ctrl-F or RIGHT: move right" "\n" \
", (comma) or <: cancel pending vertical move" "\n" \
". (full stop) or >: cancel pending horizontal move" "\n" \
"/ or \\: toggle diagonal maze reflection" "\n" \
"\? or Ctrl-H: display help screen" "\n" \
"\n" \
"The pager recognizes the following special commands:" "\n" \
"SPACE: display next page or finish" "\n" \
"ESC: finish immediately" "\n" \
"Move left then right with no intervening keystrokes:" " " \
"equivalent to ESC" "\n" \
""
#endif

#ifndef MOREMESSAGE
#define MOREMESSAGE "[Space: more, Q: quit]"
#endif

#ifndef DONEMESSAGE
#define DONEMESSAGE "[Space: OK, Q: quit]"
#endif

#ifndef BONUSHERO
#define BONUSHERO 10000
#endif

#ifndef BONUSHERO2
#define BONUSHERO2 50000
#endif

#ifndef BONUSHEROTEXT
#if BONUSHERO
#define BONUSHEROTEXT "BONUS MYMAN FOR 10000 \x9es"
#else
#define BONUSHEROTEXT "BONUS MYMAN FOR 50000 \x9es"
#endif
#endif

/* used in macbuild.txt to avoid 8bit chars */
#ifndef CHAR_SOLIDUS
#define CHAR_SOLIDUS "/"
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
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <ctype.h>
#ifndef macintosh
#include <unistd.h>
#endif
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef macintosh
#include <sys/time.h>
#endif
#include <time.h>
#ifndef WIN32
#ifndef macintosh
#include <langinfo.h>
#endif
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

#ifdef MY_CURSES_H
#include MY_CURSES_H
#else
#ifdef SLCURSES
#include <slcurses.h>
#else
#ifdef XCURSES
#include <xcurses.h>
#else
#ifdef CACACURSES
#include "cacacurs.h"
#include "optcurs.h"
#else
#ifdef AACURSES
#include "aacurses.h"
#else
#ifdef RAWCURSES
#include "rawcurs.h"
#include "optcurs.h"
#else
#ifdef MACCURSES
#include "maccurs.h"
#include "optcurs.h"
#else
#ifdef ALLEGROCURSES
#include "allegcur.h"
#include "optcurs.h"
#else
#ifdef TWCURSES
#include "twcurses.h"
#else
#ifdef GGICURSES
#include "ggicurs.h"
#include "optcurs.h"
#else
#ifdef SDLCURSES
#include "sdlcurs.h"
#include "optcurs.h"
#else
#include <curses.h>
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

#include "utils.h"

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

/* for resizing */

#ifndef USE_IOCTL
#ifdef __PDCURSES__
#define USE_IOCTL 0
#else
#define USE_IOCTL 1
#endif
#endif

#if USE_IOCTL
#include <sys/ioctl.h>
#ifdef TIOCGWINSZ
#include <termios.h>
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
#ifdef MY_GETOPT_H
#include MY_GETOPT_H
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
#include <wchar.h>
#endif
/* for uint32_t */
#include <stdint.h>

static iconv_t cd_to_wchar = (iconv_t) -1;

static iconv_t cd_to_uni = (iconv_t) -1;

static wchar_t ucs_to_wchar(unsigned long ucs)
{
    uint32_t ucsbuf[2];
    wchar_t wcbuf[2];
    const char *ibuf;
    char *obuf;
    size_t ibl;
    size_t obl;
    const char *my_locale;

    if ((! (my_locale = setlocale(LC_CTYPE, "")))
        ||
        (! *my_locale)
        ||
        (! strcmp(my_locale, "C"))
        ||
        (! strcmp(my_locale, "POSIX")))
    {
        wcbuf[0] = 0;
        goto done;
    }
    if ((cd_to_wchar == (iconv_t) -1)
        &&
        ((cd_to_wchar = iconv_open("wchar_t//IGNORE", "UCS-4-INTERNAL")) == (iconv_t) -1))
    {
        wcbuf[0] = 0;
        goto done;
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
        goto done;
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
        goto done;
    }
  done:
    if (my_locale)
    {
        setlocale(LC_CTYPE, my_locale);
    }
    return wcbuf[0] ? wcbuf[0] : (((ucs >= 0x20) && (ucs <= 0x7e)) ? ((wchar_t) ucs) : 0);
}

#else

#define ucs_to_wchar(ucs) ((((unsigned long) (wchar_t) (unsigned long) (ucs)) == (unsigned long) (ucs)) ? ((wchar_t) (unsigned long) (ucs)) : ((wchar_t) 0))

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
#endif USE_RAW_UCS
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

#ifndef CTRL
#define CTRL(x) (((x) == '\?') ? 0x7f : ((x) & ~0x60))
#endif

#ifndef KEY_LEFT
#define KEY_LEFT CTRL('B')
#endif

#ifndef KEY_RIGHT
#define KEY_RIGHT CTRL('F')
#endif

#ifndef KEY_UP
#define KEY_UP CTRL('P')
#endif

#ifndef KEY_DOWN
#define KEY_DOWN CTRL('N')
#endif

#define IS_LEFT_ARROW(k) ((k == 'h') || (k == 'H') || (k == '4') || (k == KEY_LEFT) || (k == CTRL('B')))
#define IS_RIGHT_ARROW(k) ((k == 'l') || (k == 'L') || (k == '6') || (k == KEY_RIGHT) || (k == CTRL('F')))
#define IS_UP_ARROW(k) ((k == 'k') || (k == 'K') || (k == '8') || (k == KEY_UP) || (k == CTRL('P')))
#define IS_DOWN_ARROW(k) ((k == 'j') || (k == 'J') || (k == '2') || (k == KEY_DOWN) || (k == CTRL('N')))

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
#endif
        setlocale(LC_CTYPE, my_locale);
    }
    /* for broken systems that do not yet support UTF-8 locales
     * (Cygwin comes to mind) */
    my_locale = getenv("LC_CTYPE");
    if (! my_locale) my_locale = getenv("LC_ALL");
    if (! my_locale) my_locale = getenv("LANG");
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

#ifdef A_BOLD
#define MY_A_BOLD A_BOLD
#endif

#ifdef A_UNDERLINE
#define MY_A_UNDERLINE A_UNDERLINE
#endif

#ifdef A_STANDOUT
#define MY_A_STANDOUT A_STANDOUT
#endif

#ifdef A_REVERSE
#define MY_A_REVERSE A_REVERSE
#endif

#ifdef A_DIM
#define MY_A_DIM A_DIM
#endif

#ifdef A_ATTRIBUTES
#define MY_A_ATTRIBUTES A_ATTRIBUTES
#endif

#ifdef A_CHARTEXT
#define MY_A_CHARTEXT A_CHARTEXT
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

#if ! USE_ATTR
#ifndef USE_COLOR
#define USE_COLOR 0
#endif
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
#define tile_storage extern
tile_storage const char *builtin_tilefile;
#else
static const char TILEFILE_str[] = TILEFILE;
#undef TILEFILE
#define TILEFILE TILEFILE_str
#define tile_storage static
#define builtin_tilefile TILEFILE
#endif

tile_storage int tile_w;
tile_storage int tile_h;
tile_storage int tile_flags;
tile_storage const char *tile_args;
tile_storage const char *tile;
tile_storage int tile_used[256];
tile_storage int tile_color[256];

#ifndef SOLID_WALLS
#define SOLID_WALLS ((tile_flags) & 2)
#endif

#ifndef SOLID_WALLS_BGCOLOR
#define SOLID_WALLS_BGCOLOR ((tile_flags) & 4)
#endif

#ifndef SPRITEFILE
#define SPRITEFILE "spr/spr7x3.txt"
#endif

#ifdef BUILTIN_SPRITE
#undef SPRITEFILE
#define SPRITEFILE 0
#define sprite_storage extern
sprite_storage const char *builtin_spritefile;
#else
static const char SPRITEFILE_str[] = SPRITEFILE;
#undef SPRITEFILE
#define SPRITEFILE SPRITEFILE_str
#define sprite_storage static
#define builtin_spritefile SPRITEFILE
#endif

sprite_storage int sprite_w;
sprite_storage int sprite_h;
sprite_storage int sprite_flags;
sprite_storage const char *sprite_args;
sprite_storage const char *sprite;
sprite_storage int sprite_used[256];
sprite_storage int sprite_color[256];

#ifndef VISIBLE_EYES
#define VISIBLE_EYES (! ((sprite_flags) & 1))
#endif

/* mapping from CP437 to VT-100 altcharset */
#if USE_WIDEC_SUPPORT
static cchar_t *
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
            twc = ucs_to_wchar(0xff21); // U+FF21 FULLWIDTH LATIN CAPITAL LETTER A
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
        altcharset_cp437[8] |= MY_A_REVERSE;
#endif
        ascii_cp437[8] |= MY_A_REVERSE;
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
    altcharset_cp437[8] |= MY_A_REVERSE;
#endif
    ascii_cp437[8] |= MY_A_REVERSE;
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

static int use_idlok = 1;

static int use_acs = 1;
static int use_acs_p = 0;

static int use_dim_and_bright = 0;
static int use_dim_and_bright_p = 0;

static int use_color = 0;
static int use_color_p = 0;

static int use_underline = USE_UNDERLINE;

static int use_bullet_for_dots = 0;
static int use_bullet_for_dots_p = 0;

static int quit_requested = 0;

static int reinit_requested = 0;

static int xoff_received = 0;

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
#define maze_storage extern
maze_storage const char *maze_data;
maze_storage const char *maze_color_data;
#else
#define maze_storage static
#endif
maze_storage int maze_n;
maze_storage int maze_w;
maze_storage int maze_h;
maze_storage int maze_flags;
maze_storage const char *maze_args;
#ifdef BUILTIN_MAZE
#undef MAZEFILE
#define MAZEFILE 0
maze_storage const char *builtin_mazefile;
#else
static char MAZEFILE_str[] = MAZEFILE;
#undef MAZEFILE
#define MAZEFILE MAZEFILE_str
#define builtin_mazefile MAZEFILE
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) (((y) <= (x)) ? (x) : (y))
#endif

/* which level to show intermissions after */
#ifndef INTERMISSION
#define INTERMISSION(level) \
    (((level) == 2) || \
     (((level) >= 5) && (! (((level) - 1) % 4))))
#endif

/* intermission length */
#ifndef INTERMISSION_TIME
#define INTERMISSION_TIME (PIX_W * 3 + TWOSECS)
#endif

/* how many times to show a given intermission */
#ifndef INTERMISSION_REPEAT
#define INTERMISSION_REPEAT(intermission) \
    (((intermission) == 2) ? 3 : 1)
#endif

/* how many intermissions are there? */
#ifndef INTERMISSION_N
#define INTERMISSION_N 3
#endif

/* whether to always switch maze levels, rather than only during intermissions */
#ifndef FLIP_ALWAYS
#define FLIP_ALWAYS (! ((maze_flags) & 1))
#endif

/* whether to repeat the flip_to maze after showing the last one */
#ifndef FLIP_LOCK
#define FLIP_LOCK ((maze_flags) & 2)
#endif

/* if set, base the WALL_COLOR vs. MORTAR_COLOR choice on wall thickness rather than wall edge vs. wall fill*/
#ifndef BICOLOR_WALLS
#define BICOLOR_WALLS ((maze_flags) & 4)
#endif

static char *maze = NULL;
static char *maze_color = NULL;
static char *blank_maze = NULL;
static char *blank_maze_color = NULL;
static unsigned char *dirty_cell = NULL;
static int all_dirty;
#define CLEAN_ALL() do { memset((void *)dirty_cell, 0, sizeof(dirty_cell)); all_dirty = 0; } while (0)
#define DIRTY_ALL() do { all_dirty = 1; } while (0)
#define DIRTY_CELL(x,y) do { if ((! all_dirty) && ((((long) (x)) >= 0) && (((long) (y)) >= 0) && ((x) <= maze_w) && ((y) < maze_h))) dirty_cell[((y)) * ((maze_w+1+7) >> 3) + ((x)>>3)]|=(1<<((x)&7)); } while (0)
#define IS_CELL_DIRTY(x,y) (all_dirty || ((((long) (x)) >= 0) && (((long) (y)) >= 0) && ((x) <= maze_w) && ((y) < maze_h) && (dirty_cell[((y)) * ((maze_w+1+7) >> 3) + ((x)>>3)]&(1<<((x)&7)))))
static unsigned short *inside_wall = NULL;
#define INSIDE_WALL_INVERTED 0x0001
#define INSIDE_WALL_NON_INVERTABLE 0x0002
#define INSIDE_WALL_FULLY_INVERTED 0x0004
#define INSIDE_WALL_FULLY_NON_INVERTED 0x0008
#define INSIDE_WALL_PROVISIONAL 0x0010
#define INSIDE_WALL_YES 0x0020
#define INSIDE_WALL_NO 0x0040
#define INSIDE_WALL_PHASE2 0x0080
#define INSIDE_WALL_PHASE3 0x0100
#define IS_INVERTED(x,y) (inside_wall[(maze_level*maze_h+(y)) * (maze_w + 1)+(x)] & INSIDE_WALL_INVERTED)
#define IS_FULLY_INVERTED(x,y) (inside_wall[(maze_level*maze_h+(y)) * (maze_w + 1)+(x)] & INSIDE_WALL_FULLY_INVERTED)
#define IS_FULLY_NON_INVERTED(x,y) (inside_wall[(maze_level*maze_h+(y)) * (maze_w + 1)+(x)] & INSIDE_WALL_FULLY_NON_INVERTED)

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

#ifndef LIVES
#define LIVES 3
#endif

#ifndef GHOSTS
#define GHOSTS 4
#endif

#ifndef MAXGHOSTS
#if GHOSTS > 16
#define MAXGHOSTS GHOSTS
#else
#define MAXGHOSTS 16
#endif
#endif

/* sprite register numbers */
#define GHOSTEYES(ghost) ((ghost) * 2)
#define UNGHOSTEYES(sprite_register) ((sprite_register) / 2)
#define MEANGHOST(ghost) (GHOSTEYES(ghost) + 1)
#define HERO MEANGHOST(MAXGHOSTS)
#define BLUEGHOST(ghost) ((ghost) + HERO + 1)
#define FRUIT BLUEGHOST(MAXGHOSTS)
#define GHOST_SCORE (FRUIT + 1)
#define FRUIT_SCORE (GHOST_SCORE + 1)
/* large hero sprite register, split into four quadrants */
#define BIGHERO_UL (FRUIT_SCORE + 1)
#define BIGHERO_UR (BIGHERO_UL + 1)
#define BIGHERO_LL (BIGHERO_UR + 1)
#define BIGHERO_LR (BIGHERO_LL + 1)
/* total sprite register count */
#define SPRITE_REGISTERS (BIGHERO_LR + 1)

static unsigned char sprite_register[SPRITE_REGISTERS];
static int sprite_register_frame[SPRITE_REGISTERS];
static int sprite_register_x[SPRITE_REGISTERS], sprite_register_y[SPRITE_REGISTERS];
static int sprite_register_used[SPRITE_REGISTERS];
static int sprite_register_timer[SPRITE_REGISTERS];
#if USE_COLOR
static int sprite_register_color[SPRITE_REGISTERS];
#endif

static int ghost_dir[MAXGHOSTS], ghost_mem[MAXGHOSTS], ghost_man[MAXGHOSTS], ghost_timer[MAXGHOSTS];
static unsigned char *home_dir = NULL;

#define SPRITE_FRUIT 0x00
#define SPRITE_MEAN 0x08
#define SPRITE_EYES 0x0a
#define SPRITE_BLUE 0x0e
#define SPRITE_HERO 0x10
#define SPRITE_FRUIT_SCORE 0x29
#define SPRITE_200 0x31
#define SPRITE_400 0x32
#define SPRITE_800 0x33
#define SPRITE_1600 0x34
#define SPRITE_WHITE 0x35
#define SPRITE_LIFE 0x37
/* rightward-facing large hero sprite, split into four quadrants with four frames each */
#define SPRITE_BIGHERO_UL 0x38
#define SPRITE_BIGHERO_UR 0x3C
#define SPRITE_BIGHERO_LL 0x40
#define SPRITE_BIGHERO_LR 0x44

static unsigned char reflect_sprite[256] = {
    /* 0x00 */
    /*%8*/0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, /*"^*/0x0b, /*"<*/0x0a, /*"v*/0x0d, /*">*/0x0c, /*m2*/0x0e, 0x0f,
    /* 0x10 */
    /*C<4*/0x14, 0x15, 0x16, 0x17, /*C^4*/0x10, 0x11, 0x12, 0x13,
    /*CX4*/0x18, 0x19, 0x1a, 0x1b, /*Cv4*/0x20, 0x21, 0x22, 0x23,
    /* 0x20 */
    /*C>4*/0x1c, 0x1d, 0x1e, 0x1f,
    /**/0x24, 0x25, 0x26, 0x27,
    /**/0x28, /*1*/0x29, /*3*/0x2a, /*5*/0x2b, /*7*/0x2c, /*10*/0x2d, /*20*/0x2e, /*30*/0x2f,
    /* 0x30 */
    /*50*/0x30, /*2*/0x31, /*4*/0x32, /*8*/0x33, /*16*/0x34, /*M2*/0x35, 0x36, /*C*/0x37,
    /*C(UL)*/0x38, 0x39, 0x3a, 0x3b,
    /*C(UR)*/0x40, 0x41, 0x42, 0x43,
    /* 0x40 */
    /*C(LL)*/0x3c, 0x3d, 0x3e, 0x3f,
    /*C(LR)*/0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    /* 0x50 */
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
    /* 0x60 */
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
    /* 0x70 */
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
    0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
    /* 0x80 */
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
    /* 0x90 */
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
    /* 0xa0 */
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
    /* 0xb0 */
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
    /* 0xc0 */
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    /* 0xd0 */
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
    /* 0xe0 */
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
    /* 0xf0 */
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};

static unsigned char cp437_sprite[256] = {
    /* 0x00 */
    /*%8*/'%', 'v', '@', '@', '@', 'Y', 'A', 'F',
    'A', 'A', /*"^*/'\"', /*"<*/'\"', /*"v*/'\"', /*">*/'\"', /*m2*/'m', 'm',
    /* 0x10 */
    /*C<4*/'C', 'c', 'C', 'c', /*C^4*/'C', 'c', 'C', 'c',
    /*CX4*/'C', '(', '*', '\'', /*Cv4*/'C', 'c', 'C', 'c',
    /* 0x20 */
    /*C>4*/'C', 'c', 'C', 'c',
    /**/0x00, 0x00, 0x00, 0x00,
    /**/0x00,
    /*1*/'1', /*3*/'3', /*5*/'5', /*7*/'7', /*10*/'1', /*20*/'2', /*30*/'3',
    /* 0x30 */
    /*50*/'5', /*2*/'2', /*4*/'4', /*8*/'8', /*16*/'!', /*M2*/'M', 'M', /*C*/'C',
    /*C(UL)*/'/', '/', '/', '/',
    /*C(UR)*/'\\', '-', '/', '-',
    /* 0x40 */
    /*C(LL)*/'\\', '\\', '\\', '\\',
    /*C(LR)*/'/', '-', '\\', '-',
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0x50 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0x60 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0x70 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0x80 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0x90 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xa0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xb0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xc0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xd0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xe0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xf0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define XWRAP(x) (((x) + maze_w + 1) % (maze_w + 1))
#define YWRAP(y) (((y) + maze_h) % maze_h)

#define XWRAP2(x) (XWRAP(x) % maze_w)

#define ISTEXT(c) (((c) == '!') || (((c) >= '0') && ((c) <= '9')) || ((c) == '@') || (((c) >= 'A') && ((c) <= 'Z')))
#define ISDOT(c) (((c) == 249) || ((c) == '.'))
#define ISPELLET(c) (((c) == 254) || ((c) == 'o'))
#define ISZAPLEFT(c) (((c) == '<') || (c == 174))
#define ISZAPRIGHT(c) (((c) == '>') || (c == 175))
#define ISZAPUP(c) ((c) == '^')
#define ISZAPDOWN(c) ((c) == 'v')
#define ISOPEN(c) (((c) == ' ') || ISDOT(c) || ISPELLET(c) || \
((c) == '!') || (((c) >= 'A') && ((c) <= 'Z')) || ((c) == 'l') || ((c) == '~') || ISZAPRIGHT(c) || ISZAPLEFT(c) || ISZAPUP(c) || ISZAPDOWN(c))
#define ISDOOR(c) (((c) == '=') || (c == ':') || \
((c) == 240) || (c == 255))

static const unsigned char
udlr[256] = {
/*00*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*10*/
    0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x40, 0x01, 0x04, 0x00, 0x00, 0x20, 0x80,
/*20*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x05, 0x00, 0x00,
/*30*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
/*40*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*50*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*60*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00,
/*70*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x05, 0x00,
/*80*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*90*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*A0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*B0*/
    0x00, 0x00, 0x00, 0x50, 0x54, 0x58, 0xa4, 0x64, 0x19, 0xa8, 0xa0, 0x28, 0x88, 0x94, 0x49, 0x14,
/*C0*/
    0x41, 0x45, 0x15, 0x51, 0x05, 0x55, 0x52, 0xa1, 0x82, 0x22, 0x8a, 0x2a, 0x82, 0x0a, 0xaa, 0x4a,
/*D0*/
    0x85, 0x1a, 0x25, 0x91, 0x46, 0x16, 0x61, 0xa5, 0x5a, 0x44, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00,
/*E0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*F0*/
    0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define ISWALLCENTER(c) ((((unsigned char) (char) (c)) == 0x07) || (((unsigned char) (char) (c)) == 0x12) || (((unsigned char) (char) (c)) == 0x1d) || (((unsigned char) (char) (c)) == 0x3c) || (((unsigned char) (char) (c)) == 0x3e) || (((unsigned char) (char) (c)) == 0x5e) || (((unsigned char) (char) (c)) == 0x76) || (((unsigned char) (char) (c)) == 0xae) || (((unsigned char) (char) (c)) == 0xaf))
#define ISWALLUP(c) (udlr[(unsigned int) (unsigned char) (c)] & 0xc0)
#define ISWALLDOWN(c) (udlr[(unsigned int) (unsigned char) (c)] & 0x30)
#define ISWALLLEFT(c) (udlr[(unsigned int) (unsigned char) (c)] & 0x0c)
#define ISWALLRIGHT(c) (udlr[(unsigned int) (unsigned char) (c)] & 0x03)
#define ISWALL(c) (ISWALLUP(c) || ISWALLDOWN(c) || ISWALLLEFT(c) || ISWALLRIGHT(c) || ISWALLCENTER(c))
#define ISNONINVERTABLE(c) (ISPELLET(c) || ISDOT(c) || ISZAPLEFT(c) || ISZAPRIGHT(c) || ISZAPUP(c) || ISZAPDOWN(c) || ISDOOR(c))

#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4

#ifndef DIRHERO
#define DIRHERO LEFT
#endif

#define DIRWRAP(dir) (((dir) + 3) % 4 + 1)

#define XDIR(dir) (((dir) == RIGHT) - ((dir) == LEFT))
#define YDIR(dir) (((dir) == DOWN) - ((dir) == UP))

#define YLEAVING(dir, y) ( -(((dir) == UP) && ! NOTTOP(y)) + \
(((dir) == DOWN) && ! NOTBOTTOM(y)))
#define XLEAVING(dir, x) ( -(((dir) == LEFT) && ! NOTLEFT(x)) + \
(((dir) == RIGHT) && ! NOTRIGHT(x)))

#ifndef CHERO
#define CHERO (maze_CHERO_len ? maze_CHERO[maze_level % maze_CHERO_len] : (maze_w * 0.5))
#endif

#ifndef RHERO
#define RHERO (maze_RHERO_len ? maze_RHERO[maze_level % maze_RHERO_len] : 23.5)
#endif

#ifndef CFRUIT
#define CFRUIT (maze_CFRUIT_len ? maze_CFRUIT[maze_level % maze_CFRUIT_len] : CHERO)
#endif

#ifndef RFRUIT
#define RFRUIT (maze_RFRUIT_len ? maze_RFRUIT[maze_level % maze_RFRUIT_len] : 17.5)
#endif

#ifndef RGHOST
#define RGHOST (maze_RGHOST_len ? maze_RGHOST[maze_level % maze_RGHOST_len] : (RFRUIT - 3.0))
#endif

#ifndef CGHOST
#define CGHOST (maze_CGHOST_len ? maze_CGHOST[maze_level % maze_CGHOST_len] : CHERO)
#endif

#ifndef COGHOST
#define COGHOST (maze_COGHOST_len ? maze_COGHOST[maze_level % maze_COGHOST_len] : 2)
#endif

#ifndef ROGHOST
#define ROGHOST (maze_ROGHOST_len ? maze_ROGHOST[maze_level % maze_ROGHOST_len] : 3)
#endif

#ifndef RTOP
#define RTOP (maze_RTOP_len ? maze_RTOP[maze_level % maze_RTOP_len] : (RFRUIT - 5.0))
#endif

#ifndef CMSG
#define CMSG (maze_CMSG_len ? maze_CMSG[maze_level % maze_CMSG_len] : 9)
#endif

#ifndef RMSG
#define RMSG (maze_RMSG_len ? maze_RMSG[maze_level % maze_RMSG_len] : 17)
#endif

#ifndef CMSG2
#define CMSG2 (maze_CMSG2_len ? maze_CMSG2[maze_level % maze_CMSG2_len] : 9)
#endif

#ifndef RMSG2
#define RMSG2 (maze_RMSG2_len ? maze_RMSG2[maze_level % maze_RMSG2_len] : 11)
#endif

#ifndef PLAYER1
#define PLAYER1  "PLAYER ONE"
#endif

#ifndef PLAYER2
#define PLAYER2  "PLAYER TWO"
#endif

#ifndef MAXPLAYERS
#define MAXPLAYERS 2
#endif

#define PLAYER(n) \
(((n) == 1) \
? msg_PLAYER1 \
: msg_PLAYER2)

#ifndef START
#define START "PUSH START BUTTON"
#endif

#ifndef CREDIT1
#define CREDIT1 "1 PLAYER ONLY"
#endif

#ifndef CREDIT2
#define CREDIT2 "1 OR 2 PLAYERS"
#endif

#define CREDIT(n) (((n) > 1) ? CREDIT1 : CREDIT1)

#ifndef READY
#define READY    "  READY!  "
#endif

#ifndef GAMEOVER
#define GAMEOVER "GAME  OVER"
#endif

#ifndef PAUSE
#define PAUSE " - PAUSED - "
#endif

#define BONUS(n) \
(((n) < 4) \
? ((n) - 1) \
: (((n) < 15) \
? ((n) + 1) / 2 \
: 7))

static int bonus_score[8] = {100, 300, 500, 700, 1000, 2000, 3000, 5000};

#ifndef MYMANDELAY
#define MYMANDELAY 166667
#endif

#ifndef SPEEDUP
#define SPEEDUP 1000
#endif

static unsigned char fallback_cp437[256] = {
/* 0x00 */
    0x00, '@', '@', '*', '+', '%', '&', 'o',
    '#', 'o', '#', 'd', 'q', 'l', 'H', '*',
/* 0x10 */
    0xcd, 0xcd, 0xb3, '!', 'P', '$', '#', 'L',
    0xb3, 0xb3, 0xc4, 0xc4, '!', 0xc4, 0xba, 0xba,
/* 0x20 */
    ' ', '!', '\"', '0', '$', '%', '&', '\'',
    '(', ')', '*', '+', ',', '-', '.', '/',
/* 0x30 */
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', ':', ';', '<', '-', '>', '\?',
/* 0x40 */
    'C', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
/* 0x50 */
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
/* 0x60 */
    '`', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
/* 0x70 */
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', '{', '|', '}', '-', 'D',
/* 0x80 */
    'C', 'u', 'e', 'a', 'a', 'a', 'a', 'c',
    'e', 'e', 'a', 'i', 'i', 'i', 'A', 'A',
/* 0x90 */
    'E', 'e', 'E', 'o', 'o', 'o', 'u', 'u',
    'y', 'O', 'U', 'c', 'L', 'Y', 'P', 'f',
/* 0xA0 */
    'a', 'i', 'o', 'u', 'n', 'N', 'a', 'o',
    '\?', '-', '-', '%', '%', '!', '<', '>',
/* 0xB0 */
    0xb1, '#', 0xb1, '|', 0xc5, 0xb4, 0xb4, 0xb6,
    0xd1, 0xb4, 0xb3, 0xbf, 0xd9, 0xb6, 0xcf, 0xc5,
/* 0xC0 */
    0xc5, 0xc5, 0xc5, 0xc5, '-', '+', 0xc3, 0xc3,
    0xc0, 0xda, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc1,
/* 0xD0 */
    0xc1, 0xc2, 0xc2, 0xc7, 0xcf, 0xd1, 0xc7, 0xc5,
    0xc5, 0xc5, 0xc5, '#', '_', 0xb1, 0xb1, '\"',
/* 0xE0 */
    'a', 'B', 'G', 'p', 'S', 's', 'm', 't',
    'p', 't', 'W', 'd', '8', 'f', 'e', '^',
/* 0xF0 */
    '=', '+', '>', '<', 's', 's', '%', '=',
    0x09, '.', 0x07, 'V', 'n', '2', 0xfa, ' '
};

static unsigned char reflect_cp437[256] = {
/* 0x00 */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
/* 0x10 */
    0x1f, 0x1e, 0x1d, '=', 0x14, 0x15, 0x16, 0x17,
    0x1a, 0x1b, 0x18, 0x19, 0x1c, 0x12, 0x11, 0x10,
/* 0x20 */
    ' ', '!', '\"', '#', '$', '%', '&', ',',
    '^', '_', '*', '+', '\'', '|', '.', '/',
/* 0x30 */
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', ':', ';', '^', '!', 'v', '\?',
/* 0x40 */
    '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
/* 0x50 */
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', '-', '\\', '-', '<', '|',
/* 0x60 */
    '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', '~', 'm', 'n', 'o',
/* 0x70 */
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
    'x', 'y', 'z', '-', '-', '-', 'l', 0x7f,
/* 0x80 */
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
/* 0x90 */
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
/* 0xA0 */
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, '^', 'v',
/* 0xB0 */
    0xb0, 0xb1, 0xb2, 0xc4, 0xc1, 0xd0, 0xcf, 0xd4,
    0xd3, 0xca, 0xcd, 0xc8, 0xbc, 0xbe, 0xbd, 0xc0,
/* 0xC0 */
    0xbf, 0xb4, 0xc3, 0xc2, 0xb3, 0xc5, 0xd2, 0xd1,
    0xbb, 0xc9, 0xb9, 0xcc, 0xcb, 0xba, 0xce, 0xb6,
/* 0xD0 */
    0xb5, 0xc7, 0xc6, 0xb8, 0xb7, 0xd6, 0xd5, 0xd8,
    0xd7, 0xd9, 0xda, 0xdb, 0xde, 0xdf, 0xdc, 0xdd,
/* 0xE0 */
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xeb, '8', 0xed, 'm', 'c',
/* 0xF0 */
    0xba, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, '$',
    0xf8, 0xf9, 0xfa, '7', 0xfc, 0xfd, 0xfe, 0xff
};

#define CJK_MODE (uni_cp437[0x20] == uni_cp437_fullwidth[0x20])

static chtype
pen[256];

static const char *progname = NULL;

static FILE *fopen_datafile(const char *path, const char *mode)
{
    char *buf = NULL;
    FILE *ret = NULL;

    ret = fopen(path, mode);
#ifdef macintosh
    if (strrchr(path, '/'))
    {
        buf = strdup(path);
        if (buf)
        {
            *(strrchr(buf, '/')) = ':';
            ret = fopen_datafile(buf, mode);
            free((void *) buf);
            buf = NULL;
        }        
    }
#endif
    if (progname && *progname && (! ret))
    {
        buf = (char *) malloc(strlen(progname) + 1 + strlen(path) + 1);
        if (buf)
        {
            char *sep;

            strcpy(buf, progname);
            sep = strrchr(buf, '/');
            if (! sep)
            {
                sep = buf;
            }
#ifdef WIN32
            if (strrchr(sep, '\\'))
            {
                sep = strrchr(sep, '\\');
            }
#endif
#ifdef macintosh
            if (strrchr(sep, ':'))
            {
                sep = strrchr(sep, ':');
            }
#endif
            if (sep && (sep != buf))
            {
                sep += 1;
                sep[0] = '\0';
                strcpy(sep, path);
                ret = fopen(buf, mode);
                if (! ret)
                {
                    strcpy(buf, progname);
                    sep = strstr(buf, ".app/");
                    if (sep)
                    {
                        sep += strlen(".app");
                        sep[0] = '\0';
                    }
#ifdef WIN32
                    if (strstr(buf, ".app\\"))
                    {
                        sep = strstr(buf, ".app\\");
                        sep += strlen(".app");
                        sep[0] = '\0';
                    }
#endif
#ifdef macintosh
                    if (strstr(buf, ".app:"))
                    {
                        sep = strstr(buf, ".app:");
                        sep += strlen(".app");
                        sep[0] = '\0';
                    }
#endif
                    if (sep)
                    {
                        sep = strrchr(buf, '/');
                        if (! sep)
                        {
                            sep = buf;
                        }
#ifdef WIN32
                        if (strrchr(sep, '\\'))
                        {
                            sep = strrchr(sep, '\\');
                        }
#endif
#ifdef macintosh
                        if (strrchr(sep, ':'))
                        {
                            sep = strrchr(sep, ':');
                        }
#endif
                        if (sep && (sep != buf))
                        {
                            sep += 1;
                            sep[0] = '\0';
                            strcpy(sep, path);
                            ret = fopen(buf, mode);
                        }
                    }
                }
            }
            free((void *) buf);
            buf = NULL;
        }
    }
#ifdef PRIVATEDATADIR
    if ((! ret) && (PRIVATEDATADIR)[0])
    {
        buf = (char *) malloc(strlen(PRIVATEDATADIR) + 1 + strlen(path) + 1);
        if (buf)
        {
            sprintf(buf,
                    "%s%c%s",
                    strlen(PRIVATEDATADIR) ? PRIVATEDATADIR :
#ifdef macintosh
                    ""
#else
                    "."
#endif
                    ,
#ifdef macintosh
                    ':'
#else
                    '/'
#endif
                    ,
                    path);
            ret = fopen(buf, mode);
            free((void *) buf);
            buf = NULL;
        }
    }
#endif
    if ((! ret)
        &&
        ((strlen(path) < strlen(".txt"))
         ||
         (strcmp(path + strlen(path) - strlen(".txt"), ".txt"))))
    {
        buf = (char *) malloc(strlen(path) + strlen(".txt") + 1);
        if (buf)
        {
            sprintf(buf, "%s%s", path, ".txt");
            return fopen_datafile(buf, mode);
            free((void *) buf);
            buf = NULL;
        }
    }
    return ret;
}

static int
readmaze(const char *mazefile,
         int *levels,
         int *w,
         int *h,
         char **maze,
         int *flags,
         char **color,
         const char **args)
{
    char X;
    int c = ERR, i, j;
    int n;
    FILE *infile = NULL;

    if (! (infile = fopen_datafile(mazefile, "rb"))) {
        perror(mazefile);
        return 1;
    }
    ignore_bom_utf8(infile);
    {
        int rn, rw, rh;

        if ((fscanf(infile, "%d%d%c%d", &rn, &rw, &X, &rh) != 4) ||
            (tolower(X) != 'x'))
        {
            fprintf(stderr, "%s: can't find a dimension specification N WxH\n",
                    mazefile);
            return 1;
        }
        if ((rw < 1) || (rh < 1) || (rn < 1)) {
            fprintf(stderr, "%s: dimension specification %d %dx%d is too small\n",
                    mazefile, rn, rw, rh);
            return 1;
        }
        *levels = rn;
        *h = rh;
        *w = rw;
    }
    *flags = 0;
    *args = NULL;
    if (! feof(infile))
    {
        c = fgetc_cp437_utf8(infile);
        if (c == '~')
        {
            if (fscanf(infile, "%d", flags) != 1)
            {
                fprintf(stderr, "%s: can't find flags ~F after dimension specification %d %dx%d\n",
                        mazefile,
                        *levels, *w, *h);
                return 1;
            }
        }
        else
        {
            ungetc_cp437_utf8(c, infile);
        }
        c = fgetc_cp437_utf8(infile);
        if ((c == ' ') || (c == '\t'))
        {
            char *args_tmp = NULL;
            size_t args_tmp_len = 0;
            int escaped = 0;

            args_tmp = strdup("");
            if (! args_tmp)
            {
                perror("strdup");
                return 1;
            }
            while (((c = fgetc_cp437_utf8(infile)) != EOF)
                   &&
                   (c != '\v') && (c != '\f') && (c != '\n') && (c != '\r'))
            {
                if (c == '\\')
                {
                    int c2;

                    escaped = ! escaped;
                    if (escaped)
                    {
                        c2 = fgetc_cp437_utf8(infile);
                        if (c2 == '\r')
                        {
                            int c3;

                            c3 = fgetc_cp437_utf8(infile);
                            if (c3 == '\n')
                            {
                                c2 = c3;
                            }
                            else
                            {
                                ungetc_cp437_utf8(c3, infile);
                            }
                        }
                        if ((c2 == '\v') || (c2 == '\f') || (c2 == '\n') || (c2 == '\r'))
                        {
                            escaped = 0;
                            continue;
                        }
                        ungetc_cp437_utf8(c2, infile);
                    }
                }
                args_tmp = (char *) realloc((void *) args_tmp, args_tmp_len + 1 + ((c == 0) ? 4 : 1));
                if (! args_tmp)
                {
                    perror("realloc");
                    return 1;
                }
                if (c == 0)
                {
                    if (! escaped)
                    {
                        args_tmp[args_tmp_len] = '\\';
                        args_tmp_len ++;
                    }
                    args_tmp[args_tmp_len] = 'x';
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '0';
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '0';
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '\0';
                }
                else
                {
                    args_tmp[args_tmp_len] = c;
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '\0';
                }
            }
            if (args_tmp_len)
            {
                *args = args_tmp;
            }
            else
            {
                free((void *) args_tmp);
                args_tmp = NULL;
            }
        }
        ungetc_cp437_utf8(c, infile);
    }
    if ((! feof(infile))
        &&
        (((X = fgetc_cp437_utf8(infile)) != '\n') &&
         (X != '\r') && (X != '\v') && (X != '\f')))
    {
        fprintf(stderr, "%s: garbage after dimension specification %d %dx%d~%d%s%s %X\n",
                mazefile,
                *levels, *w, *h, *flags, *args ? " " : "", *args ? *args : "", X);
        return 1;
    }
    *maze = (char *) malloc(*levels * *h * (*w + 1) * sizeof(**maze));
    if (! *maze)
    {
        perror("malloc");
        return 1;
    }
    memset((void *) *maze, 0, *levels * *h * (*w + 1) * sizeof(**maze));
    *color = (char *) malloc(*levels * *h * (*w + 1) * sizeof(**color));
    if (! *color)
    {
        perror("malloc");
        return 1;
    }
    memset((void *) *color, 0, *levels * *h * (*w + 1) * sizeof(**color));
    for (n = 0; n < *levels; n ++)
    {
        for (i = 0; i < *h; i ++)
        {
            for (j = 0; j < *w; j ++)
            {
                if ((c = fgetc_cp437_utf8(infile)) == EOF)
                {
                    if (feof(infile))
                        fprintf(stderr, "%s: premature EOF\n", mazefile);
                    else
                        perror(mazefile);
                    return 1;
                } else if ((c == '\r') || (c == '\n') || (c == '\f') || (c == '\v'))
                    j --;
                else
                    (*maze)[(n * *h+i) * (*w + 1)+j] = (char) (unsigned char) c;
            }
            if (ISPELLET(c) || ISDOT(c))
            {
                c = ' ';
            }
            (*maze)[(n * *h+i) * (*w + 1)+*w] = (char) (unsigned char) c;
        }
    }
    fclose(infile);
    return 0;
}

static int
readfont(const char *fontfile,
         int *w,
         int *h,
         const char **font,
         int *used,
         int *flags,
         int *color,
         const char **args)
{
    FILE *infile;
    int c, i, j, k;
    int rw, rh;
    char X;
    char *font_dynamic;

    *args = NULL;
    *flags = 0;
    for (i = 0; i < 256; i ++) {
        used[i] = 0;
        color[i] = 0;
    }
    if (! (infile = fopen_datafile(fontfile, "rb"))) {
        perror(fontfile);
        return 1;
    }
    ignore_bom_utf8(infile);
    if ((fscanf(infile, "%d%c%d", &rw, &X, &rh) != 3) ||
        (tolower(X) != 'x')) {
        fprintf(stderr, "%s: can't find a dimension specification WxH\n",
                fontfile);
        return 1;
    }
    *w = rw;
    *h = rh;
    *font = font_dynamic = (char *) malloc(256 * rh * rw);
    if (! font_dynamic)
    {
        perror("malloc");
        return 1;
    }
    if (! feof(infile))
    {
        c = fgetc(infile);
        if (c == '~')
        {
            if (fscanf(infile, "%d", flags) != 1)
            {
                fprintf(stderr, "%s: can't find flags ~F after dimension specification %dx%d\n",
                        fontfile,
                        rw, rh);
                return 1;
            }
        }
        else
        {
            ungetc(c, infile);
        }
    }
    if (! feof(infile))
    {
        c = fgetc_cp437_utf8(infile);
        if ((c == ' ') || (c == '\t'))
        {
            char *args_tmp = NULL;
            size_t args_tmp_len = 0;
            int escaped = 0;

            args_tmp = strdup("");
            if (! args_tmp)
            {
                perror("strdup");
                return 1;
            }
            while (((c = fgetc_cp437_utf8(infile)) != EOF)
                   &&
                   (c != '\v') && (c != '\f') && (c != '\n') && (c != '\r'))
            {
                if (c == '\\')
                {
                    int c2;

                    escaped = ! escaped;
                    if (escaped)
                    {
                        c2 = fgetc_cp437_utf8(infile);
                        if (c2 == '\r')
                        {
                            int c3;

                            c3 = fgetc_cp437_utf8(infile);
                            if (c3 == '\n')
                            {
                                c2 = c3;
                            }
                            else
                            {
                                ungetc_cp437_utf8(c3, infile);
                            }
                        }
                        if ((c2 == '\v') || (c2 == '\f') || (c2 == '\n') || (c2 == '\r'))
                        {
                            escaped = 0;
                            continue;
                        }
                        ungetc_cp437_utf8(c2, infile);
                    }
                }
                args_tmp = (char *) realloc((void *) args_tmp, args_tmp_len + 1 + ((c == 0) ? 4 : 1));
                if (! args_tmp)
                {
                    perror("realloc");
                    return 1;
                }
                if (c == 0)
                {
                    if (! escaped)
                    {
                        args_tmp[args_tmp_len] = '\\';
                        args_tmp_len ++;
                    }
                    args_tmp[args_tmp_len] = 'x';
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '0';
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '0';
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '\0';
                }
                else
                {
                    args_tmp[args_tmp_len] = c;
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '\0';
                }
            }
            if (args_tmp_len)
            {
                *args = args_tmp;
            }
            else
            {
                free((void *) args_tmp);
                args_tmp = NULL;
            }
        }
        else
        {
            ungetc_cp437_utf8(c, infile);
        }
    }
    while (! feof(infile))
    {
        if (fscanf(infile, "%x", &i) != 1)
        {
            if (! feof(infile))
            {
                long offset;

                offset = ftell(infile);
                fprintf(stderr, "%s: can't find an index at byte %ld near \'\\u%4.4lx\' or equivalent\n",
                        fontfile, offset, uni_cp437[(unsigned int) (unsigned char) fgetc_cp437_utf8(infile)]);
                return 1;
            }
            continue;
        }
        if ((i < 0) || (i > 255))
        {
            fprintf(stderr, "%s: invalid index %2.2X ignored\n", fontfile, i);
            continue;
        }
        if (used[i])
            fprintf(stderr, "%s: duplicate definition for %2.2X\n", fontfile, i);
        used[i] = 1;
        if ((c = fgetc_cp437_utf8(infile)) == '~')
        {
            if (fscanf(infile, "%x", &c) != 1)
            {
                if (! feof(infile))
                {
                    fprintf(stderr, "%s: can't find a color for index %2.2X\n",
                            fontfile, i);
                    return 1;
                }
                continue;
            }
            if ((c < 0) || (c >= (int) (sizeof(pen)/sizeof(*pen))))
                fprintf(stderr, "%s: invalid color %2.2X ignored\n", fontfile, c);
            else
                color[i] = c;
        }
        else
            ungetc_cp437_utf8(c, infile);
        for (j = 0; j < rh; j ++)
            for (k = 0; k < rw; k ++)
                font_dynamic[(i * rh + j) * rw + k] = ' ';
        for (j = 0; j < rh; j ++)
        {
            while ((c = fgetc_cp437_utf8(infile)) != ':')
                if (c == EOF)
                {
                    if (! feof(infile))
                        perror(fontfile);
                    else
                        fprintf(stderr, "%s: premature EOF in index %2.2X\n",
                                fontfile, i);
                    return 1;
                }
            if ((c = fgetc_cp437_utf8(infile)) == EOF)
                if (! feof(infile))
                {
                    perror(fontfile);
                    return 1;
                }
            for (k = 0;
                 (k < rw) &&
                     (c != '\v') && (c != '\f') &&
                     (c != '\n') && (c != '\r') && ! feof(infile);
                 k ++)
            {
                font_dynamic[(i * rh + j) * rw + k] = c;
                if ((c = fgetc_cp437_utf8(infile)) == EOF) {
                    if (feof(infile))
                        continue;
                    perror(fontfile);
                    return 1;
                }
            }
            while ((c != '\v') && (c != '\f') &&
                   (c != '\n') && (c != '\r') && ! feof(infile)) {
                if (c == EOF) {
                    if (feof(infile))
                        continue;
                    perror(fontfile);
                    return 1;
                }
                c = fgetc_cp437_utf8(infile);
            }
        }
    }
    fclose(infile);
    return 0;
}

/* color palette for USE_PALETTE and HTML snapshots */
static const short
pen_pal[16][3] =
{
/*{ red,green, blue} */
    {   0,    0,    0}, /* 0: black */
    {   0,    0,  867}, /* 1: blue */
    {   0,  718,    0}, /* 2: green (peach stem) */
    {   0,  867,  867}, /* 3: cyan */
    { 867,  589,  277}, /* 4: brown (apple/cherry stem) */
    {1000,  128,  589}, /* 5: magenta (mypal trim?) */
    {1000,  718,  277}, /* 6: salmon (clyde, peach, PUSH START BUTTON) */
    {1000,  718,  589}, /* 7: light yellow (dot, blue face, BONUS MYMAN FOR x Pts) */
    { 400,  400,  400}, /* 8: grey */
    { 128,  128,  867}, /* 9: light blue (wall, blue ghost) */
    { 589, 1000,    0}, /* A: light green (mypal?, super dot?) */
    {   0, 1000,  867}, /* B: light cyan (inky, key top, 1 PLAYER ONLY, ghost scores) */
    {1000,    0,    0}, /* C: light red (blinky, apple/cherry, GAME  OVER) */
    { 980,  701,  847}, /* D: pink (pinky, door, NEW MAN - X, fruit scores) */
    {1000, 1000,    0}, /* E: yellow (myman, ship, READY!) */
    { 867,  867,  867}, /* F: light grey (text, eye, apple/cherry shine, key, bell) */
};

#ifndef WALL_COLORS
#define WALL_COLORS "\x00"
#endif

#ifndef DOT_COLORS
#define DOT_COLORS "\x07"
#endif

#ifndef PELLET_COLORS
#define PELLET_COLORS "\x00"
#endif

#ifndef MORTAR_COLORS
#define MORTAR_COLORS "\x09"
#endif

#ifndef WALL_COLOR
#define WALL_COLOR maze_WALL_COLORS[maze_level % maze_WALL_COLORS_len]
#endif

#ifndef DOT_COLOR
#define DOT_COLOR maze_DOT_COLORS[maze_level % maze_DOT_COLORS_len]
#endif

#ifndef PELLET_COLOR
#define PELLET_COLOR maze_PELLET_COLORS[maze_level % maze_PELLET_COLORS_len]
#endif

#ifndef MORTAR_COLOR
#define MORTAR_COLOR maze_MORTAR_COLORS[maze_level % maze_MORTAR_COLORS_len]
#endif

#define TRANSLATED_MORTAR_COLOR (((BICOLOR_WALLS) && (udlr[(unsigned char) (char) maze[(maze_level*maze_h+ytile)*(maze_w+1)+xtile]] & 0xAA)) ? (WALL_COLOR) : (MORTAR_COLOR))

#define TRANSLATED_WALL_COLOR ((BICOLOR_WALLS) ? TRANSLATED_MORTAR_COLOR : (WALL_COLOR))

#define EFFECTIVE_MORTAR_COLOR (((TRANSLATED_WALL_COLOR) && ! (SOLID_WALLS || SOLID_WALLS_BGCOLOR)) ? (TRANSLATED_WALL_COLOR) : (TRANSLATED_MORTAR_COLOR))

#define NET_LIVES ((int) lives + (int) earned - (int) lives_used + (myman_demo ? 1 : 0))

#ifndef MSG_COLOR
#define MSG_COLOR ((NET_LIVES && ! myman_demo) ? 0xE : 0xC)
#endif

#ifndef MSG2_COLOR
#define MSG2_COLOR 0xB
#endif

#ifndef TEXT_COLOR
#define TEXT_COLOR 0xF
#endif

#ifndef PAUSE_COLOR
#define PAUSE_COLOR 0xF0
#endif

#ifndef EXTRA_GHOST_COLORS
#define EXTRA_GHOST_COLORS "\x0A\x05\x04\x03"
#endif

#if USE_COLOR

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

#define _MYMAN_LINEARSCALE \
    "\x0\x0\x1\x1\x2\x2\x3\x3\x4\x4\x5\x5\x6\x6\x7\x7" \
    "\x8\x8\x9\x9\xa\xa\xb\xb\xc\xc\xd\xd\xe\xe\xf\xf" \
    "\x10\x10\x11\x11\x12\x12\x13\x13\x14\x14\x15\x15\x16\x16\x17\x17" \
    "\x18\x18\x19\x19\x1a\x1a\x1b\x1b\x1c\x1c\x1d\x1d\x1e\x1e\x1f\x1f" \
    "\x20\x20\x21\x21\x22\x22\x23\x23\x24\x24\x25\x25\x26\x26\x27\x27" \
    "\x28\x28\x29\x29\x2a\x2a\x2b\x2b\x2c\x2c\x2d\x2d\x2e\x2e\x2f\x2f" \
    "\x30\x30\x31\x31\x32\x32\x33\x33\x34\x34\x35\x35\x36\x36\x37\x37" \
    "\x38\x38\x39\x39\x3a\x3a\x3b\x3b\x3c\x3c\x3d\x3d\x3e\x3e\x3f\x3f" \
    "\x40\x40\x41\x41\x42\x42\x43\x43\x44\x44\x45\x45\x46\x46\x47\x47" \
    "\x48\x48\x49\x49\x4a\x4a\x4b\x4b\x4c\x4c\x4d\x4d\x4e\x4e\x4f\x4f" \
    "\x50\x50\x51\x51\x52\x52\x53\x53\x54\x54\x55\x55\x56\x56\x57\x57" \
    "\x58\x58\x59\x59\x5a\x5a\x5b\x5b\x5c\x5c\x5d\x5d\x5e\x5e\x5f\x5f" \
    "\x60\x60\x61\x61\x62\x62\x63\x63\x64\x64\x65\x65\x66\x66\x67\x67" \
    "\x68\x68\x69\x69\x6a\x6a\x6b\x6b\x6c\x6c\x6d\x6d\x6e\x6e\x6f\x6f" \
    "\x70\x70\x71\x71\x72\x72\x73\x73\x74\x74\x75\x75\x76\x76\x77\x77" \
    "\x78\x78\x79\x79\x7a\x7a\x7b\x7b\x7c\x7c\x7d\x7d\x7e\x7e\x7f\x7f" \
    "\x80\x80\x81\x81\x82\x82\x83\x83\x84\x84\x85\x85\x86\x86\x87\x87" \
    "\x88\x88\x89\x89\x8a\x8a\x8b\x8b\x8c\x8c\x8d\x8d\x8e\x8e\x8f\x8f" \
    "\x90\x90\x91\x91\x92\x92\x93\x93\x94\x94\x95\x95\x96\x96\x97\x97" \
    "\x98\x98\x99\x99\x9a\x9a\x9b\x9b\x9c\x9c\x9d\x9d\x9e\x9e\x9f\x9f" \
    "\xa0\xa0\xa1\xa1\xa2\xa2\xa3\xa3\xa4\xa4\xa5\xa5\xa6\xa6\xa7\xa7" \
    "\xa8\xa8\xa9\xa9\xaa\xaa\xab\xab\xac\xac\xad\xad\xae\xae\xaf\xaf" \
    "\xb0\xb0\xb1\xb1\xb2\xb2\xb3\xb3\xb4\xb4\xb5\xb5\xb6\xb6\xb7\xb7" \
    "\xb8\xb8\xb9\xb9\xba\xba\xbb\xbb\xbc\xbc\xbd\xbd\xbe\xbe\xbf\xbf" \
    "\xc0\xc0\xc1\xc1\xc2\xc2\xc3\xc3\xc4\xc4\xc5\xc5\xc6\xc6\xc7\xc7" \
    "\xc8\xc8\xc9\xc9\xca\xca\xcb\xcb\xcc\xcc\xcd\xcd\xce\xce\xcf\xcf" \
    "\xd0\xd0\xd1\xd1\xd2\xd2\xd3\xd3\xd4\xd4\xd5\xd5\xd6\xd6\xd7\xd7" \
    "\xd8\xd8\xd9\xd9\xda\xda\xdb\xdb\xdc\xdc\xdd\xdd\xde\xde\xdf\xdf" \
    "\xe0\xe0\xe1\xe1\xe2\xe2\xe3\xe3\xe4\xe4\xe5\xe5\xe6\xe6\xe7\xe7" \
    "\xe8\xe8\xe9\xe9\xea\xea\xeb\xeb\xec\xec\xed\xed\xee\xee\xef\xef" \
    "\xf0\xf0\xf1\xf1\xf2\xf2\xf3\xf3\xf4\xf4\xf5\xf5\xf6\xf6\xf7\xf7" \
    "\xf8\xf8\xf9\xf9\xfa\xfa\xfb\xfb\xfc\xfc\xfd\xfd\xfe\xfe\xff\xff" \
    ""

static short mille_to_scale(short n, short scale)
{
    return (short) (((long) (scale - 1)) * ((long) (unsigned char) _MYMAN_LINEARSCALE[n * 511L / 1000]) / 255);
}

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
    unsigned char pair_allocated[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

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
                pair_allocated[i / 8] |= 1 << (i % 8);
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
                        pair_allocated[i / 8] |= 1 << (i % 8);
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
                pair_allocated[i / 8] |= 1 << (i % 8);
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
                        pair_allocated[i / 8] |= 1 << (i % 8);
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
                            pair_allocated[i / 8] |= 1 << (i % 8);
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
                            pair_allocated[i / 8] |= 1 << (i % 8);
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
                    pair_allocated[i / 8] |= 1 << (i % 8);
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

static FILE *snapshot = 0;
static FILE *snapshot_txt = 0;
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
                            r, g, b);
                    if (i / 16)
                    {
                        r = (255 * pen_pal[i / 16][0]) / 1000;
                        g = (255 * pen_pal[i / 16][1]) / 1000;
                        b = (255 * pen_pal[i / 16][2]) / 1000;
                        fprintf(snapshot,
                                " style=\"%sbackground:#%2.2lX%2.2lX%2.2lX\"",
                                ((i / 16) == (i % 16)) ? "color: #000000; " : "",
                                r, g, b);
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

        my_locale = setlocale(LC_CTYPE, "");
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
            setlocale(LC_CTYPE, my_locale);
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

static int
my_clear(void)
{
    location_is_suspect = 0;
    return clear();
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
    attrset(attrs);
#endif
    return 1;
}

/* add a cp437 string to the HTML snapshot */
static void
snapshot_addch(unsigned long inbyte)
{
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
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[201]
#endif
                            ) ? uni_cp437_halfwidth[201] : (ascii_cp437[201] & MY_A_CHARTEXT);
                        break;
                    }
                case 218:
                    codepoint = (altcharset_cp437[218] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[218]
#endif
                        ) ? uni_cp437_halfwidth[218] : (ascii_cp437[218] & MY_A_CHARTEXT);
                    break;
                case 200:
                    if (altcharset_cp437[200] != altcharset_cp437[192])
                    {
                        codepoint = (altcharset_cp437[200] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[200]
#endif
                            ) ? uni_cp437_halfwidth[200] : (ascii_cp437[200] & MY_A_CHARTEXT);
                        break;
                    }
                case 192:
                    codepoint = (altcharset_cp437[192] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[192]
#endif
                        ) ? uni_cp437_halfwidth[192] : (ascii_cp437[192] & MY_A_CHARTEXT);
                    break;
                case 187:
                    if (altcharset_cp437[187] != altcharset_cp437[191])
                    {
                        codepoint = (altcharset_cp437[187] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[187]
#endif
                            ) ? uni_cp437_halfwidth[187] : (ascii_cp437[187] & MY_A_CHARTEXT);
                        break;
                    }
                case 191:
                    codepoint = (altcharset_cp437[191] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[191]
#endif
                        ) ? uni_cp437_halfwidth[191] : (ascii_cp437[191] & MY_A_CHARTEXT);
                    break;
                case 188:
                    if (altcharset_cp437[188] != altcharset_cp437[217])
                    {
                        codepoint = (altcharset_cp437[188] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[188]
#endif
                            ) ? uni_cp437_halfwidth[188] : (ascii_cp437[188] & MY_A_CHARTEXT);
                        break;
                    }
                case 217:
                    codepoint = (altcharset_cp437[217] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[217]
#endif
                        ) ? uni_cp437_halfwidth[217] : (ascii_cp437[217] & MY_A_CHARTEXT);
                    break;
                case 185:
                    if (altcharset_cp437[185] != altcharset_cp437[181])
                    {
                        codepoint = (altcharset_cp437[185] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[185]
#endif
                            ) ? uni_cp437_halfwidth[185] : (ascii_cp437[185] & MY_A_CHARTEXT);
                        break;
                    }
                case 181:
                    if (altcharset_cp437[181] != altcharset_cp437[182])
                    {
                        codepoint = (altcharset_cp437[181] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[181]
#endif
                            ) ? uni_cp437_halfwidth[181] : (ascii_cp437[181] & MY_A_CHARTEXT);
                        break;
                    }
                case 182:
                    if (altcharset_cp437[182] != altcharset_cp437[180])
                    {
                        codepoint = (altcharset_cp437[182] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[182]
#endif
                            ) ? uni_cp437_halfwidth[182] : (ascii_cp437[182] & MY_A_CHARTEXT);
                        break;
                    }
                case 180:
                    codepoint = (altcharset_cp437[180] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[180]
#endif
                        ) ? uni_cp437_halfwidth[180] : (ascii_cp437[180] & MY_A_CHARTEXT);
                    break;
                case 204:
                    if (altcharset_cp437[204] != altcharset_cp437[198])
                    {
                        codepoint = (altcharset_cp437[204] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[204]
#endif
                            ) ? uni_cp437_halfwidth[204] : (ascii_cp437[204] & MY_A_CHARTEXT);
                        break;
                    }
                case 198:
                    if (altcharset_cp437[198] != altcharset_cp437[199])
                    {
                        codepoint = (altcharset_cp437[198] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[198]
#endif
                            ) ? uni_cp437_halfwidth[198] : (ascii_cp437[198] & MY_A_CHARTEXT);
                        break;
                    }
                case 199:
                    if (altcharset_cp437[199] != altcharset_cp437[195])
                    {
                        codepoint = (altcharset_cp437[199] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[199]
#endif
                            ) ? uni_cp437_halfwidth[199] : (ascii_cp437[199] & MY_A_CHARTEXT);
                        break;
                    }
                case 195:
                    codepoint = (altcharset_cp437[195] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[195]
#endif
                        ) ? uni_cp437_halfwidth[195] : (ascii_cp437[195] & MY_A_CHARTEXT);
                    break;
                case 202:
                    if (altcharset_cp437[202] != altcharset_cp437[207])
                    {
                        codepoint = (altcharset_cp437[202] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[202]
#endif
                            ) ? uni_cp437_halfwidth[202] : (ascii_cp437[202] & MY_A_CHARTEXT);
                        break;
                    }
                case 207:
                    if (altcharset_cp437[207] != altcharset_cp437[208])
                    {
                        codepoint = (altcharset_cp437[207] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[207]
#endif
                            ) ? uni_cp437_halfwidth[207] : (ascii_cp437[207] & MY_A_CHARTEXT);
                        break;
                    }
                case 208:
                    if (altcharset_cp437[208] != altcharset_cp437[193])
                    {
                        codepoint = (altcharset_cp437[208] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[208]
#endif
                            ) ? uni_cp437_halfwidth[208] : (ascii_cp437[208] & MY_A_CHARTEXT);
                        break;
                    }
                case 193:
                    codepoint = (altcharset_cp437[193] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[193]
#endif
                        ) ? uni_cp437_halfwidth[193] : (ascii_cp437[193] & MY_A_CHARTEXT);
                    break;
                case 203:
                    if (altcharset_cp437[203] != altcharset_cp437[209])
                    {
                        codepoint = (altcharset_cp437[203] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[203]
#endif
                            ) ? uni_cp437_halfwidth[203] : (ascii_cp437[203] & MY_A_CHARTEXT);
                        break;
                    }
                case 209:
                    if (altcharset_cp437[209] != altcharset_cp437[210])
                    {
                        codepoint = (altcharset_cp437[209] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[209]
#endif
                            ) ? uni_cp437_halfwidth[209] : (ascii_cp437[209] & MY_A_CHARTEXT);
                        break;
                    }
                case 210:
                    if (altcharset_cp437[210] != altcharset_cp437[194])
                    {
                        codepoint = (altcharset_cp437[210] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[210]
#endif
                            ) ? uni_cp437_halfwidth[210] : (ascii_cp437[210] & MY_A_CHARTEXT);
                        break;
                    }
                case 194:
                    codepoint = (altcharset_cp437[194] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[194]
#endif
                        ) ? uni_cp437_halfwidth[194] : (ascii_cp437[194] & MY_A_CHARTEXT);
                    break;
                case 213:
                    codepoint = (altcharset_cp437[194] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[194]
#endif
                        ) ? uni_cp437_halfwidth[194] : (ascii_cp437[194] & MY_A_CHARTEXT);
                    break;
                case 214:
                    codepoint = (altcharset_cp437[195] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[195]
#endif
                        ) ? uni_cp437_halfwidth[195] : (ascii_cp437[195] & MY_A_CHARTEXT);
                    break;
                case 212:
                    codepoint = (altcharset_cp437[193] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[193]
#endif
                        ) ? uni_cp437_halfwidth[193] : (ascii_cp437[193] & MY_A_CHARTEXT);
                    break;
                case 211:
                    codepoint = (altcharset_cp437[195] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[195]
#endif
                        ) ? uni_cp437_halfwidth[195] : (ascii_cp437[195] & MY_A_CHARTEXT);
                    break;
                case 184:
                    codepoint = (altcharset_cp437[194] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[194]
#endif
                        ) ? uni_cp437_halfwidth[194] : (ascii_cp437[194] & MY_A_CHARTEXT);
                    break;
                case 183:
                    codepoint = (altcharset_cp437[180] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[180]
#endif
                        ) ? uni_cp437_halfwidth[180] : (ascii_cp437[180] & MY_A_CHARTEXT);
                    break;
                case 190:
                    codepoint = (altcharset_cp437[193] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[193]
#endif
                        ) ? uni_cp437_halfwidth[193] : (ascii_cp437[193] & MY_A_CHARTEXT);
                    break;
                case 189:
                    codepoint = (altcharset_cp437[180] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[180]
#endif
                        ) ? uni_cp437_halfwidth[180] : (ascii_cp437[180] & MY_A_CHARTEXT);
                    break;
                case 205:
                    if (altcharset_cp437[205] != altcharset_cp437[196])
                    {
                        codepoint = (altcharset_cp437[205] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[205]
#endif
                            ) ? uni_cp437_halfwidth[205] : (ascii_cp437[205] & MY_A_CHARTEXT);
                        break;
                    }
                case 196:
                    codepoint = (altcharset_cp437[196] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[196]
#endif
                        ) ? uni_cp437_halfwidth[196] : (ascii_cp437[196] & MY_A_CHARTEXT);
                    break;
                case 186:
                    if (altcharset_cp437[186] != altcharset_cp437[179])
                    {
                        codepoint = (altcharset_cp437[186] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[186]
#endif
                            ) ? uni_cp437_halfwidth[186] : (ascii_cp437[186] & MY_A_CHARTEXT);
                        break;
                    }
                case 179:
                    codepoint = (altcharset_cp437[179] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[179]
#endif
                        ) ? uni_cp437_halfwidth[179] : (ascii_cp437[179] & MY_A_CHARTEXT);
                    break;
                case 206:
                    if (altcharset_cp437[206] != altcharset_cp437[215])
                    {
                        codepoint = (altcharset_cp437[206] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[206]
#endif
                            ) ? uni_cp437_halfwidth[206] : (ascii_cp437[206] & MY_A_CHARTEXT);
                        break;
                    }
                case 215:
                    if (altcharset_cp437[215] != altcharset_cp437[216])
                    {
                        codepoint = (altcharset_cp437[215] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[215]
#endif
                            ) ? uni_cp437_halfwidth[215] : (ascii_cp437[215] & MY_A_CHARTEXT);
                        break;
                    }
                case 216:
                    if (altcharset_cp437[216] != altcharset_cp437[197])
                    {
                        codepoint = (altcharset_cp437[216] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[216]
#endif
                            ) ? uni_cp437_halfwidth[216] : (ascii_cp437[216] & MY_A_CHARTEXT);
                        break;
                    }
                case 197:
                    codepoint = (altcharset_cp437[197] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[197]
#endif
                        ) ? uni_cp437_halfwidth[197] : (ascii_cp437[197] & MY_A_CHARTEXT);
                    break;
                case 15:
                    if (altcharset_cp437[15] != altcharset_cp437[176])
                    {
                        codepoint = (altcharset_cp437[15] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[15]
#endif
                            ) ? uni_cp437_halfwidth[15] : (ascii_cp437[15] & MY_A_CHARTEXT);
                        break;
                    }
                case 176:
                    if (altcharset_cp437[176] != altcharset_cp437[177])
                    {
                        codepoint = (altcharset_cp437[176] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[176]
#endif
                            ) ? uni_cp437_halfwidth[176] : (ascii_cp437[176] & MY_A_CHARTEXT);
                        break;
                    }
                case 177:
                    if (altcharset_cp437[177] != altcharset_cp437[178])
                    {
                        codepoint = (altcharset_cp437[177] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[177]
#endif
                            ) ? uni_cp437_halfwidth[177] : (ascii_cp437[177] & MY_A_CHARTEXT);
                        break;
                    }
                case 178:
                    if (altcharset_cp437[178] != altcharset_cp437[10])
                    {
                        codepoint = (altcharset_cp437[178] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[178]
#endif
                            ) ? uni_cp437_halfwidth[178] : (ascii_cp437[178] & MY_A_CHARTEXT);
                        break;
                    }
                case 10:
                    if (altcharset_cp437[10] != altcharset_cp437[219])
                    {
                        codepoint = (altcharset_cp437[10] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[10]
#endif
                            ) ? uni_cp437_halfwidth[10] : (ascii_cp437[10] & MY_A_CHARTEXT);
                        break;
                    }
                case 219:
                    codepoint = (altcharset_cp437[219] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[219]
#endif
                        ) ? uni_cp437_halfwidth[219] : (ascii_cp437[219] & MY_A_CHARTEXT);
                    break;
                case 27:
                    if (altcharset_cp437[27] != altcharset_cp437[17])
                    {
                        codepoint = (altcharset_cp437[27] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[27]
#endif
                            ) ? uni_cp437_halfwidth[27] : (ascii_cp437[27] & MY_A_CHARTEXT);
                        break;
                    }
                case 17:
                    if (altcharset_cp437[17] != altcharset_cp437[174])
                    {
                        codepoint = (altcharset_cp437[17] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[17]
#endif
                            ) ? uni_cp437_halfwidth[17] : (ascii_cp437[17] & MY_A_CHARTEXT);
                        break;
                    }
                case 174:
                    if (altcharset_cp437[174] != altcharset_cp437[243])
                    {
                        codepoint = (altcharset_cp437[174] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[174]
#endif
                            ) ? uni_cp437_halfwidth[174] : (ascii_cp437[174] & MY_A_CHARTEXT);
                        break;
                    }
                case 243:
                    codepoint = (altcharset_cp437[243] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[243]
#endif
                        ) ? uni_cp437_halfwidth[243] : (ascii_cp437[243] & MY_A_CHARTEXT);
                    break;
                case 26:
                    if (altcharset_cp437[26] != altcharset_cp437[16])
                    {
                        codepoint = (altcharset_cp437[26] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[26]
#endif
                            ) ? uni_cp437_halfwidth[26] : (ascii_cp437[26] & MY_A_CHARTEXT);
                        break;
                    }
                case 16:
                    if (altcharset_cp437[16] != altcharset_cp437[175])
                    {
                        codepoint = (altcharset_cp437[16] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[16]
#endif
                            ) ? uni_cp437_halfwidth[16] : (ascii_cp437[16] & MY_A_CHARTEXT);
                        break;
                    }
                case 175:
                    if (altcharset_cp437[175] != altcharset_cp437[242])
                    {
                        codepoint = (altcharset_cp437[175] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[175]
#endif
                            ) ? uni_cp437_halfwidth[175] : (ascii_cp437[175] & MY_A_CHARTEXT);
                        break;
                    }
                case 242:
                    codepoint = (altcharset_cp437[242] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[242]
#endif
                        ) ? uni_cp437_halfwidth[242] : (ascii_cp437[242] & MY_A_CHARTEXT);
                    break;
                case 7:
                    if (altcharset_cp437[7] != altcharset_cp437[9])
                    {
                        codepoint = (altcharset_cp437[7] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[7]
#endif
                            ) ? uni_cp437_halfwidth[7] : (ascii_cp437[7] & MY_A_CHARTEXT);
                        break;
                    }
                case 9:
                    if (altcharset_cp437[9] != altcharset_cp437[254])
                    {
                        codepoint = (altcharset_cp437[9] !=
#if USE_WIDEC_SUPPORT
                                     NULL
#else
                                     ascii_cp437[9]
#endif
                            ) ? uni_cp437_halfwidth[9] : (ascii_cp437[9] & MY_A_CHARTEXT);
                        break;
                    }
                case 8:
                    codepoint = (altcharset_cp437[8] !=
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[8]
#endif
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
#if USE_WIDEC_SUPPORT
                                 NULL
#else
                                 ascii_cp437[inbyte]
#endif
                        ) ? uni_cp437_halfwidth[inbyte] : (ascii_cp437[inbyte] & MY_A_CHARTEXT);
                    break;
                default:
                    inbyte = (unsigned char)
                        (ascii_cp437[inbyte] & MY_A_CHARTEXT);
                    codepoint = inbyte;
                }
            }
        }
        else
        {
            inbyte = (unsigned char)
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
            if (snapshot_attrs_active & MY_A_BOLD)
            {
                if (snapshot_attrs_active & MY_A_UNDERLINE)
                {
                    fputs("_\b", snapshot_txt);
                }
                fputc_utf8(codepoint, snapshot_txt);
                fputc('\b', snapshot_txt);
            }
            if (snapshot_attrs_active & MY_A_UNDERLINE)
            {
                fputs("_\b", snapshot_txt);
            }
            fputc_utf8(codepoint, snapshot_txt);
            fflush(snapshot_txt);
        }
        snapshot_x ++;
    }
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
        if (rhs)
        {
            snapshot_addch(rhs);
        }
    }
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
                if (rhs)
                {
                    buf[0] = (char) (unsigned char) (rhs & 0xFF);
                    addstr(buf);
                }
            }
            return ret;
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
                            if (rhs)
                            {
                                wchar_t wrhs;

                                wrhs = ucs_to_wchar(uni_cp437_fullwidth[rhs]);
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
                        return ret;
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
                            return ret;
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
                            return ret;
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
                        altcharset_cp437[b],
                        NULL,
                        &my_acs_attrs,
                        &my_color_pair,
                        NULL);
                if (my_len &&
                    (getcchar(
                        altcharset_cp437[b],
                        my_wchbuf,
                        &my_acs_attrs,
                        &my_color_pair,
                        NULL) != ERR) &&
                    (my_wcswidth(my_wchbuf, my_len) == 1))
                {
                    attr_get(
                        & my_current_attrs,
                        & my_color_pair,
                        NULL);
                    attr_set(
                        my_current_attrs | my_acs_attrs,
                        my_color_pair,
                        NULL);
                    ret = addnwstr(
                        my_wchbuf,
                        my_len);
                    getyx(stdscr, new_y, new_x);
                    if (CJK_MODE && ((old_x != new_x) || (old_y != new_y)))
                    {
                        unsigned char rhs;
                        
                        rhs = cp437_fullwidth_rhs[b];
                        if (rhs)
                        {
                            if (altcharset_cp437[rhs])
                            {
                                my_len =
                                    getcchar(
                                        altcharset_cp437[rhs],
                                        NULL,
                                        &my_acs_attrs,
                                        &my_color_pair,
                                        NULL);
                                if (my_len &&
                                    (getcchar(
                                        altcharset_cp437[rhs],
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
                                addch(ascii_cp437[rhs]);
                            }
                        }
                    }
                    attr_set(
                        my_current_attrs,
                        my_color_pair,
                        NULL);
                    if ((old_x != new_x) || (old_y != new_y))
                    {
                        return ret;
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
                            if (rhs)
                            {
                                c = uni_cp437[rhs];
                                my_wch = ucs_to_wchar(c);
                                getyx(stdscr, old_y, old_x);
                                if (my_wch && (my_wcwidth(my_wch) < 2))
                                {
                                    addch(c);
                                    getyx(stdscr, new_y, new_x);
                                    if ((old_x != new_x) || (old_y != new_y))
                                    {
                                        return ret;
                                    }
                                }
                                c = altcharset_cp437[rhs];
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                                my_attrset(attrs | (c & ~A_CHARTEXT));
#endif
#endif
                                addch(c);
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                                my_attrset(attrs);
#endif
#endif
                                getyx(stdscr, new_y, new_x);
                                if ((old_x != new_x) || (old_y != new_y))
                                {
                                    return ret;
                                }
                                addch(ascii_cp437[rhs]);
                            }
                        }
                    }
#endif
                    return ret;
                }
            }
            c = altcharset_cp437[b];
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
            my_attrset(attrs | (c & ~A_CHARTEXT));
#endif
#endif
            ret = addch(c);
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
            my_attrset(attrs);
#endif
#endif
            getyx(stdscr, new_y, new_x);
            if ((old_x != new_x) || (old_y != new_y))
            {
                if (CJK_MODE && ((new_x % COLS) != ((old_x + 2) % COLS)))
                {
                    unsigned char rhs;
                    
                    rhs = cp437_fullwidth_rhs[b];
                    if (rhs)
                    {
                        c = altcharset_cp437[rhs];
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                        my_attrset(attrs | (c & ~A_CHARTEXT));
#endif
#endif
                        addch(c);
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                        my_attrset(attrs);
#endif
#endif
                    }
                }
                return ret;
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
        if (rhs)
        {
            addch(ascii_cp437[rhs]);
        }
    }
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

#define MYMAN_ISPRINT(c) ((' ' == 0x20) ? (((c) >= 0x20) && ((c) <= 0x7e)) : isprint(c))

static void
escape(const char *s, int len)
{
    int c;
    int i;

    for (i = 0; i < len; i ++)
        if (MYMAN_ISPRINT((c = (unsigned char) s[i]))) {
            if ((c == '\"') || (c == '\\') || (c == '\'') || (c == '\?'))
                putchar('\\');
            putchar(c);
        }
        else
        {
            if (((i + 1 == len) || ! isdigit((unsigned char) s[i + 1])) && (c == '\0'))
                printf("\\0");
            else
                printf("\\%3.3o", c);
        }
}

static void
writefont(const char *file,
          const char *prefix,
          int w, int h,
          const char *font,
          int *used,
          int flags,
          int *color,
          const char *args
    )
{
    int c, i, j;

    printf("static const char builtin_%sfile_str[] = \"", prefix);
    escape(file, strlen(file));
    printf("\";\n");
    printf("const char *builtin_%sfile = builtin_%sfile_str;\n", prefix, prefix);
    printf("int %s_flags = %d;\n", prefix, flags);
    printf("int %s_w = %d;\n", prefix, w);
    printf("int %s_h = %d;\n", prefix, h);
    printf("const char *%s_args = ", prefix);
    if (args)
    {
        printf("\"");
        escape(args, strlen(args));
        printf("\"");
    }
    else
    {
        printf("0");
    }
    printf(";\n");
    printf("static const char builtin_%s_data[256 * %d * %d] = {", prefix, h, w);
    for (c = 0; c < 256; c ++) {
        printf("\n/* 0x%2.2X */", c);
        for (i = 0; i < h; i ++) {
            printf("\n ");
            for (j = 0; j < w; j ++) {
                char k;

                printf(" \'");
                k = font[(c * h + i) * w + j];
                escape(&k, 1);
                printf("\'");
                if (((c + 1) < 256) || ((i + 1) < h) || ((j + 1) < w))
                    printf(",");
            }
        }
    }
    printf("};\n");
    printf("const char *%s = builtin_%s_data;", prefix, prefix);
    printf("int %s_used[256] = {\n", prefix);
    for (c = 0; c < 256; c ++) {
        if (c && ! (c & 3))
            printf(",\n");
        else if (c)
            printf(", ");
        printf("/* 0x%2.2X */ %d", c, used[c]);
    }
    printf("};\n");
    printf("int %s_color[256] = {\n", prefix);
    for (c = 0; c < 256; c ++) {
        if (c && ! (c & 3))
            printf(",\n");
        else if (c)
            printf(", ");
        printf("/* 0x%2.2X */ 0x%X", c, color[c]);
    }
    printf("};\n");
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

/* heuristic for rewriting maze tiles */
static long
maze_visual(int n, int i, int j)
{
    long c;

    c = (unsigned char) maze[(n*maze_h+i) * (maze_w + 1)+j];
    switch (c)
    {
    case 0xb5:
        if ((! ISWALLUP(blank_maze[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]))
            ||
            (! ISWALLDOWN(blank_maze[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j])))
            c = 0x10;
        break;
    case 0xc6:
        if ((! ISWALLUP(blank_maze[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]))
            ||
            (! ISWALLDOWN(blank_maze[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j])))
            c = 0x11;
        break;
    case 'l':
    case 0xb3:
        if ((! ISWALLUP(blank_maze[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]))
            &&
            (! ISWALLDOWN(blank_maze[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j])))
            c = 0x12;
        else if (! ISWALLUP(blank_maze[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]))
            c = 0x19;
        else if (! ISWALLDOWN(blank_maze[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j]))
            c = 0x18;
        break;
    case 0xd0:
        if ((! ISWALLLEFT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j + 1)]))
            ||
            (! ISWALLRIGHT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j - 1)])))
            c = 0x1f;
        break;
    case 0xd2:
        if ((! ISWALLLEFT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j + 1)]))
            ||
            (! ISWALLRIGHT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j - 1)])))
            c = 0x1e;
        break;
    case '~':
    case 0xc4:
        if ((! ISWALLLEFT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j + 1)]))
            &&
            (! ISWALLRIGHT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j - 1)])))
            c = 0x1d;
        else if (! ISWALLLEFT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j + 1)]))
            c = 0x1b;
        else if (! ISWALLRIGHT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j - 1)]))
            c = 0x1a;
        break;
    }
    return c;
}

static const char * pager_notice = MYMANLEGALNOTICE;
static const char * pager_remaining = 0;
/* left then right in the pager is equivalent to space */
static int pager_arrow_magic = 0;

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

#define pager_move(y,x) do \
{ \
    my_move((pager_big ? ((y) * pager_tile_h) : y), \
            ((pager_big ? ((x) * tile_w) : x) * (use_fullwidth ? 2 : 1))); \
} while(0)
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
#define pager_addch(c,a) do \
{ \
    int pager_addch__x, pager_addch__y, pager_addch__i, pager_addch__j; \
     \
    pager_getyx(stdscr, pager_addch__y, pager_addch__x); \
    if (pager_addch__x >= PAGER_COLS) \
    { \
        pager_addch__x = 0; \
        pager_addch__y ++; \
    } \
    if (pager_addch__y >= PAGER_LINES) \
    { \
        pager_move(PAGER_LINES - 1, PAGER_COLS - 1); \
    } \
    else \
    { \
        if (pager_big) \
        { \
            unsigned long pager_addch__c, pager_addch__cc; \
            chtype pager_addch__a; \
     \
            pager_addch__c = (unsigned long) (unsigned char) (c); \
            pager_addch__cc = pager_addch__c; \
            while ((! tile_used[pager_addch__cc]) \
                   && \
                   (fallback_cp437[pager_addch__cc] != pager_addch__c) \
                   && \
                   (fallback_cp437[pager_addch__cc] != pager_addch__cc)) \
            { \
                pager_addch__cc = (unsigned long) (unsigned char) fallback_cp437[pager_addch__cc]; \
            } \
            pager_addch__c = pager_addch__cc; \
            pager_addch__a = (a); \
            for (pager_addch__j = 0; \
                 pager_addch__j < (((pager_addch__y + 1) == PAGER_LINES) \
                                   ? \
                                   (LINES - pager_addch__y * pager_tile_h) \
                                   : \
                                   pager_tile_h); \
                 pager_addch__j ++) \
            { \
                for (pager_addch__i = 0; pager_addch__i < tile_w; pager_addch__i ++) \
                { \
                    pager_addch__cc = (unsigned long) (unsigned char) ((pager_addch__j < tile_h) ? tile[(pager_addch__c * tile_h + pager_addch__j) * tile_w + pager_addch__i] : ' '); \
                    if (! pager_addch__cc) pager_addch__cc = (unsigned long) (unsigned char) ' '; \
                    my_move(pager_addch__y * pager_tile_h + pager_addch__j, \
                            (pager_addch__x * tile_w + pager_addch__i) * (use_fullwidth ? 2 : 1)); \
                    my_addch(pager_addch__cc, \
                             (pager_addch__j < pager_tile_h) \
                             ? \
                             pager_addch__a \
                             : \
                             ((USE_COLOR && use_color) \
                              ? \
                              pen[TEXT_COLOR] \
                              : \
                              0)); \
                } \
                if ((pager_addch__x + 1) == PAGER_COLS) \
                { \
                    for (pager_addch__i = (pager_addch__x + 1) * tile_w; pager_addch__i < MY_COLS; pager_addch__i ++) \
                    { \
                        my_move(pager_addch__y * pager_tile_h + pager_addch__j, \
                                pager_addch__i * (use_fullwidth ? 2 : 1)); \
                        my_addch((unsigned long) (unsigned char) ' ', \
                                 (USE_COLOR && use_color) \
                                 ? \
                                 pen[TEXT_COLOR] \
                                 : \
                                 0); \
                    } \
                } \
            } \
        } \
        else \
        { \
            my_addch(c, a); \
        } \
        if ((pager_addch__x + 1) < PAGER_COLS) \
        { \
            pager_move(pager_addch__y, pager_addch__x + 1); \
        } \
        else if ((pager_addch__y + 1) < PAGER_LINES) \
        { \
            pager_move(pager_addch__y + 1, 0); \
        } \
        else \
        { \
            pager_move(PAGER_LINES - 1, PAGER_COLS - 1); \
        } \
    } \
} while(0)
#define pager_addstr(s,a) do \
{ \
    const char *pager_addstr__s = (s); \
 \
    while (*pager_addstr__s) \
    { \
        pager_addch((unsigned long) (unsigned char) *pager_addstr__s, (a)); \
        pager_addstr__s ++; \
    } \
} while(0)

void
pager(void)
{
    int c = ERR;
    int k = ERR;

    my_attrset(0);
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
                    while ((k = getch()) == ERR)
                    {
                        my_refresh();
                        if (got_sigwinch) break;
                        usleep(100000UL);
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
                        else if ((k == 'r') || (k == 'R') || (k == CTRL('L')) || (k == CTRL('R')))
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
                        else if ((k == 'q') || (k == 'Q') || (k == CTRL('C')))
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
                            if ((use_color = ! use_color))
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
                        else if (k == CTRL('@'))
                        {
                            /* NUL - idle keepalive (iTerm, maybe others?) */
                            pager = pager_remaining;
                            break;
                        }
                        else if (k == CTRL('S'))
                        {
                            xoff_received = 1;
                            pager = pager_remaining;
                            break;
                        }
                        else if (k == CTRL('Q'))
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
                            if (use_sound) beep();
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
            while ((k = getch()) == ERR)
            {
                my_refresh();
                if (got_sigwinch) break;
                usleep(100000UL);
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
                else if ((k == 'q') || (k == 'Q') || (k == CTRL('C')))
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
                    if ((use_color = ! use_color))
                        init_pen();
                    else
                        destroy_pen();
                    my_attrset(0);
                    my_clear();
                    clearok(curscr, TRUE);
                }
#endif
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
                else if ((k == 'r') || (k == 'R') || (k == CTRL('L')) || (k == CTRL('R')))
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
                else if (k == CTRL('@'))
                {
                    /* NUL - idle keepalive (iTerm, maybe others?) */
                }
                else if (k == CTRL('S'))
                {
                    xoff_received = 1;
                }
                else if (k == CTRL('Q'))
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
                    if (use_sound) beep();
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

#define rmsg (RMSG % maze_h)
#define cmsg (CMSG % maze_w)
#define rmsg2 (RMSG2 % maze_h)
#define cmsg2 (CMSG2 % maze_w)

#define COMPLEXITY_ADJUST(x) ((x) * (maze_h + maze_w) / (28 + 31))

#define TWOSECS    (10 * FIFTH)
#define ONESEC     (TWOSECS / 2)
#define FRUITLIFE  COMPLEXITY_ADJUST(TWOSECS * 15 / 4)
#define DEATHSHIFT 3
#define DEATHDELAY (1 << (DEATHSHIFT + 2))
#define MEMDELAY(ghost) (COMPLEXITY_ADJUST((3 + ((ghost) > 3) * (1.0 * ((ghost) - 3) / (ghosts - 3))) * TWOSECS))

#define maze_erase() \
        do { \
            memset((void *) (maze + maze_level * maze_h * (maze_w + 1)), 0, (maze_w + 1) * maze_h); \
            memset((void *) (maze_color + maze_level * maze_h * (maze_w + 1)), 0, (maze_w + 1) * maze_h); \
            DIRTY_ALL(); \
        } while(0)
#define maze_puts(y, x, c, s) \
        do { \
            int _mpi; \
            unsigned char _mpc; \
            int _mpy, _mpx; \
             \
            _mpy = (y); \
            _mpx = (x); \
            if (_mpy < 0) continue; \
            if (_mpy >= maze_h) continue; \
            for (_mpi = 0; (_mpc = (unsigned char) (s)[_mpi]); _mpi ++) \
            { \
                if (((_mpx + _mpi) >= 0) && ((_mpx + _mpi) < maze_w)) \
                { \
                    maze[(maze_level*maze_h+YWRAP(_mpy)) * (maze_w + 1)+XWRAP(_mpx + _mpi)] = _mpc; \
                    maze_color[(maze_level*maze_h+YWRAP(_mpy)) * (maze_w + 1)+XWRAP(_mpx + _mpi)] = (unsigned char) (c); \
                    DIRTY_CELL(XWRAP(_mpx + _mpi), YWRAP(_mpy)); \
                } \
            } \
        } while(0)
#define maze_putsn_nonblank(y, x, c, s, n) \
        do { \
            int _mpi; \
            unsigned char _mpc; \
            int _mpy, _mpx; \
             \
            _mpy = (y); \
            _mpx = (x); \
            if (_mpy < 0) continue; \
            if (_mpy >= maze_h) continue; \
            for (_mpi = 0; (_mpi < (n)) && ((_mpc = (unsigned char) (s)[_mpi])); _mpi ++) \
            { \
                int _mpcc; \
                 \
                _mpcc = (c); \
                if (((_mpx + _mpi) >= 0) && ((_mpx + _mpi) < maze_w)) \
                { \
                    if (_mpc == ' ') \
                    { \
                        _mpc = blank_maze[(maze_level*maze_h+YWRAP(_mpy)) * (maze_w + 1)+XWRAP(_mpx + _mpi)]; \
                        _mpcc = blank_maze_color[(maze_level*maze_h+YWRAP(_mpy)) * (maze_w + 1)+XWRAP(_mpx + _mpi)]; \
                    } \
                    maze[(maze_level*maze_h+YWRAP(_mpy)) * (maze_w + 1)+XWRAP(_mpx + _mpi)] = _mpc; \
                    maze_color[(maze_level*maze_h+YWRAP(_mpy)) * (maze_w + 1)+XWRAP(_mpx + _mpi)] = (unsigned char) _mpcc; \
                    DIRTY_CELL(XWRAP(_mpx + _mpi), YWRAP(_mpy)); \
                } \
            } \
        } while(0)

#define REFLECT_LARGE (reflect && (tile_w == tile_h) && (sprite_w == sprite_h))

#define gfx2(c) (((reflect ^ gfx_reflect) && ! REFLECT_LARGE) ? reflect_cp437[(unsigned long) (unsigned char) (c)] : (c))
#define gfx1(c,y,x,w,h) ((reflect ^ ((reflect ^ gfx_reflect) && ! REFLECT_LARGE)) ? (((c) * (h) + (x)) * (w) + (y)) : (((c) * (h) + (y)) * (w) + (x)))
#define gfx0(c,m) ((REFLECT_LARGE | gfx_reflect) ? ((unsigned long) (unsigned char) ((m)[(unsigned long) (unsigned char) (c)])) : (c))

#define gfx_w (gfx_reflect ? tile_h : tile_w)
#define gfx_h (gfx_reflect ? tile_w : tile_h)
#define gfx(c,y,x) ((unsigned long) (unsigned char) gfx2(tile[gfx1(gfx0((c), (reflect_cp437)), (y) % gfx_h, (x) % gfx_w, tile_w, tile_h)]))

#define sgfx_w (gfx_reflect ? sprite_h : sprite_w)
#define sgfx_h (gfx_reflect ? sprite_w : sprite_h)
#define sgfx(c,y,x) ((unsigned long) (unsigned char) gfx2(sprite[gfx1(gfx0((c), (reflect_sprite)), (y) % sgfx_h, (x) % sgfx_w, sprite_w, sprite_h)]))

#define XTILE(x) ((x) / gfx_w)
#define YTILE(y) ((y) / gfx_h)

#define DIRTY_SPRITE_REGISTER(s) \
    do { \
        int _dirty_sprite_register_x, _dirty_sprite_register_y; \
        for (_dirty_sprite_register_y = 0; _dirty_sprite_register_y < ((gfx_h > sgfx_h) ? gfx_h : sgfx_h); _dirty_sprite_register_y ++) \
            for (_dirty_sprite_register_x = 0; _dirty_sprite_register_x < ((gfx_w > sgfx_w) ? gfx_w : sgfx_w); _dirty_sprite_register_x ++) \
            { \
                DIRTY_CELL(XTILE(sprite_register_x[s] + _dirty_sprite_register_x - ((gfx_w > sgfx_w) ? gfx_w : sgfx_w) / 2), \
                           YTILE(sprite_register_y[s] + _dirty_sprite_register_y - ((gfx_h > sgfx_h) ? gfx_h : sgfx_h) / 2)); \
            } \
    } while (0)

#define collide(s1, s2) ((XTILE(sprite_register_x[s1] - sgfx_w / 2) == XTILE(sprite_register_x[s2] - sgfx_w / 2)) \
&& (YTILE(sprite_register_y[s1] - sgfx_h / 2) == YTILE(sprite_register_y[s2] - sgfx_h / 2)))

#define PIX_W ((maze_w + 1) * gfx_w)
#define PIX_H (maze_h * gfx_h)

#define XPIXWRAP(x) (((x) + PIX_W) % PIX_W)
#define YPIXWRAP(y) (((y) + PIX_H) % PIX_H)

#define NOTTOP(y) (y >= (gfx_h / 2))
#define NOTBOTTOM(y) (y <= (gfx_h / 2))
#define NOTLEFT(x) (x >= (gfx_w / 2))
#define NOTRIGHT(x) (x <= (gfx_w / 2))

#define XHERO (CHERO * gfx_w)
#define YHERO (RHERO * gfx_h)
#define XFRUIT (CFRUIT * gfx_w)
#define YFRUIT (RFRUIT * gfx_h)
#define YGHOST (RGHOST * gfx_h)
#define XGHOST (CGHOST * gfx_w)
#define YTOP (RTOP * gfx_h)

#ifndef MAXFRAMESKIP
#define MAXFRAMESKIP (1 + tile_w)
#endif

#define SQUARE ((tile_h + tile_h) > tile_w)

#define FIFTH     (tile_w * (SQUARE ? 2 : 1))

#define PELLET_ADJUST(x) (COMPLEXITY_ADJUST(x * 4) / (pellets[maze_level] ? pellets[maze_level] : 4))

#define GHOST0 ((ghosts > 2) ? 0 : 2)
#define GHOST1 1
#define GHOST2 ((ghosts > 2) ? 2 : 0)
#define GHOST3 3

#ifndef NAME_HEADER
#define NAME_HEADER "CHARACTER /"
#endif

#ifndef NICK_HEADER
#define NICK_HEADER " NICKNAME"
#endif

#ifndef GHOST_NAMES
#define GHOST_NAMES GHOST_NAMES_XLT
#endif

#define GHOST_NAMES_JA 0

#define GHOST_NAMES_ALT 1

#define GHOST_NAMES_EN 2

#define GHOST_NAMES_XLT 3

#if GHOST_NAMES == GHOST_NAMES_JA

#ifndef GHOST0_NAME
#define GHOST0_NAME " KIMAGURE--"
#endif

#ifndef GHOST0_NICK
#define GHOST0_NICK "\"AOSUKE\""
#endif

#ifndef GHOST2_NAME
#define GHOST2_NAME " MACHIBUSE-"
#endif

#ifndef GHOST2_NICK
#define GHOST2_NICK "-\"PINKY\""
#endif

#ifndef GHOST1_NAME
#define GHOST1_NAME " OIKAKE----"
#endif

#ifndef GHOST1_NICK
#define GHOST1_NICK "\"AKABEI\""
#endif

#ifndef GHOST3_NAME
#define GHOST3_NAME " OTOBOKE---"
#endif

#ifndef GHOST3_NICK
#define GHOST3_NICK "\"GUZUTA\""
#endif

#endif

#if GHOST_NAMES == GHOST_NAMES_ALT

#ifndef GHOST0_NAME
#define GHOST0_NAME " STYLIST"
#endif

#ifndef GHOST0_NICK
#define GHOST0_NICK "\"MUCKY\""
#endif

#ifndef GHOST2_NAME
#define GHOST2_NAME " ROMP"
#endif

#ifndef GHOST2_NICK
#define GHOST2_NICK "\"MICKY\""
#endif

#ifndef GHOST1_NAME
#define GHOST1_NAME " URCHIN"
#endif

#ifndef GHOST1_NICK
#define GHOST1_NICK "\"MACKY\""
#endif

#ifndef GHOST3_NAME
#define GHOST3_NAME " CRYBABY"
#endif

#ifndef GHOST3_NICK
#define GHOST3_NICK "\"MOCKY\""
#endif

#endif

#if GHOST_NAMES == GHOST_NAMES_EN

#ifndef GHOST0_NAME
#define GHOST0_NAME "-BASHFUL"
#endif

#ifndef GHOST0_NICK
#define GHOST0_NICK "\"INKY\""
#endif

#ifndef GHOST2_NAME
#define GHOST2_NAME "-SPEEDY"
#endif

#ifndef GHOST2_NICK
#define GHOST2_NICK "\"PINKY\""
#endif

#ifndef GHOST1_NAME
#define GHOST1_NAME "-SHADOW"
#endif

#ifndef GHOST1_NICK
#define GHOST1_NICK "\"BLINKY\""
#endif

#ifndef GHOST3_NAME
#define GHOST3_NAME "-POKEY"
#endif

#ifndef GHOST3_NICK
#define GHOST3_NICK "\"CLYDE\""
#endif

#endif

#if GHOST_NAMES == GHOST_NAMES_XLT

#ifndef GHOST0_NAME
#define GHOST0_NAME " WHIMSICAL-"
#endif

#ifndef GHOST0_NICK
#define GHOST0_NICK "---\"CYAN\""
#endif

#ifndef GHOST2_NAME
#define GHOST2_NAME " AMBUSHER--"
#endif

#ifndef GHOST2_NICK
#define GHOST2_NICK "--\"MAUVE\""
#endif

#ifndef GHOST1_NAME
#define GHOST1_NAME " CHASER----"
#endif

#ifndef GHOST1_NICK
#define GHOST1_NICK "\"SCARLET\""
#endif

#ifndef GHOST3_NAME
#define GHOST3_NAME " CLOWN-----"
#endif

#ifndef GHOST3_NICK
#define GHOST3_NICK "--\"SLOTH\""
#endif

#endif

#define WHOSE_HOME_DIR(r,c) (home_dir[(GHOST2 % ghosts*maze_h+(r))*(maze_w+1)+(c)] ? GHOST2 \
: home_dir[(GHOST0 % ghosts*maze_h+(r))*(maze_w+1)+(c)] ? GHOST0 \
: home_dir[(GHOST3 % ghosts*maze_h+(r))*(maze_w+1)+(c)] ? GHOST3 \
: GHOST1)

#define HOME_DIR(s,r,c) (home_dir[(WHOSE_HOME_DIR((r),(c)) % ghosts*maze_h+(r))*(maze_w+1)+(c)] \
                         ? \
                         home_dir[(WHOSE_HOME_DIR((r),(c)) % ghosts*maze_h+(r))*(maze_w+1)+(c)] \
                         : \
                         home_dir[((s)*maze_h+(r))*(maze_w+1)+(c)])

static long myman_intro = 1;
static unsigned long myman_start = 0;
static unsigned long myman_demo = 0;
static int munched = HERO;
static int old_lines, old_cols, old_score, old_showlives, old_level;
static int ignore_delay = 0;
static long frameskip = 0, frameskip0 = 0, frameskip1 = 0;
static long scrolling = 0;
static long frames = 0;
static long winning = 1;
static unsigned long delay = MYMANDELAY;
static unsigned long mindelay = MYMANDELAY / 2;
static int ghost_eaten_timer = 0;
static int paused = 0;
static int reflect = 0;
static int gfx_reflect = 0;
static long intermission_running = 0;
static unsigned long myman_demo_setup = 0;
static int need_reset = 0;
static int level = 0,
    maze_level = 0,
    intermission = 0,
    intermission_shown = 0,
    cycles = 0,
    score = 0,
    dots = 0,
    points = 0,
    lives = LIVES,
    lives_used = 0,
    earned = 0,
    dying = 0,
    dead = 0,
    deadpan = 0,
    lines = 0,
    columns = 0,
    oldplayer = 0,
    player = 1;
static int key_buffer = ERR;
static long pellet_timer = 0,
    pellet_time = 0;
static struct timeval tv, tvt, tvt2;
static int tvt_used = 0;
static char *tmp_notice = 0;
static const char * maze_ABOUT = 0;
static const char * maze_FIXME = 0;
static const char * maze_NOTE = 0;
static const char * tile_ABOUT = 0;
static const char * tile_FIXME = 0;
static const char * tile_NOTE = 0;
static const char * sprite_ABOUT = 0;
static const char * sprite_FIXME = 0;
static const char * sprite_NOTE = 0;
static const char *msg_READY = READY;
static const char *msg_GAMEOVER = GAMEOVER;
static const char *msg_PLAYER1 = PLAYER1;
static const char *msg_PLAYER2 = PLAYER2;
static const char *maze_WALL_COLORS = WALL_COLORS;
static size_t maze_WALL_COLORS_len = sizeof(WALL_COLORS) - 1;
static const char *maze_DOT_COLORS = DOT_COLORS;
static size_t maze_DOT_COLORS_len = sizeof(DOT_COLORS) - 1;
static const char *maze_PELLET_COLORS = PELLET_COLORS;
static size_t maze_PELLET_COLORS_len = sizeof(PELLET_COLORS) - 1;
static const char *maze_MORTAR_COLORS = MORTAR_COLORS;
static size_t maze_MORTAR_COLORS_len = sizeof(MORTAR_COLORS) - 1;
static double *maze_RGHOST = NULL;
static size_t maze_RGHOST_len = 0;
static double *maze_CGHOST = NULL;
static size_t maze_CGHOST_len = 0;
static double *maze_ROGHOST = NULL;
static size_t maze_ROGHOST_len = 0;
static double *maze_COGHOST = NULL;
static size_t maze_COGHOST_len = 0;
static double *maze_RFRUIT = NULL;
static size_t maze_RFRUIT_len = 0;
static double *maze_CFRUIT = NULL;
static size_t maze_CFRUIT_len = 0;
static double *maze_RTOP = NULL;
static size_t maze_RTOP_len = 0;
static double *maze_RHERO = NULL;
static size_t maze_RHERO_len = 0;
static double *maze_CHERO = NULL;
static size_t maze_CHERO_len = 0;
static long *maze_RMSG = NULL;
static size_t maze_RMSG_len = 0;
static long *maze_CMSG = NULL;
static size_t maze_CMSG_len = 0;
static long *maze_RMSG2 = NULL;
static size_t maze_RMSG2_len = 0;
static long *maze_CMSG2 = NULL;
static size_t maze_CMSG2_len = 0;
static int dirhero = DIRHERO;
static long scroll_offset_x0 = 0;
static long scroll_offset_y0 = 0;
static int msglen = 0;
static int hero_dir;
static int *total_dots = NULL;
static int *pellets = NULL;
static long flip_to = 0;
static int debug = 0;
static int ghosts = GHOSTS;
static int ghosts_p = 0;
#if USE_SDL_MIXER
static int sdl_audio_open = 0;
#endif

static int
gamecycle(void)
{
    long c = 0;
    int i, j, k;
    int s, x1, y1, line, col, xtile, ytile, x_off, y_off, vline, vcol, pause_shown;
    int r_off, c_off;
    int visible_frame;
    static unsigned long sfx = 0UL;
    int showlives;

    showlives = ((myman_intro || myman_start || myman_demo) ? 0 : NET_LIVES) - 1 + (((munched == HERO) && (! sprite_register_used[HERO])) ? 1 : 0);
    if ((old_lines != LINES)
        ||
        (old_cols != COLS)
        ||
        (old_score > score)
        ||
        (old_showlives != showlives)
        ||
        (old_level != level))
    {
        DIRTY_ALL();
        ignore_delay = 1;
        frameskip = 0;
        old_lines = LINES;
        old_cols = COLS;
        /* TODO: make some video memory for the status areas
         * in order to avoid unnecessary full refreshes */
        old_score = score;
        old_showlives = showlives;
        old_level = level;
    }
#define sfx_credit 0x1UL
#define sfx_dot 0x2UL
#define sfx_dying 0x4UL
#define sfx_ghost 0x8UL
#define sfx_intermission 0x10UL
#define sfx_pellet 0x20UL
#define sfx_siren0_down 0x40UL
#define sfx_siren0_up 0x80UL
#define sfx_siren1_down 0x100UL
#define sfx_siren1_up 0x200UL
#define sfx_siren2_down 0x400UL
#define sfx_siren2_up 0x800UL
#define sfx_start 0x1000UL
#define sfx_fruit 0x2000UL
#define sfx_life 0x4000UL
#define sfx_level 0x8000UL
#define sfx_bonus 0x10000UL
#define sfx_nobeep_mask (sfx_siren0_up|sfx_siren0_down|sfx_siren1_up|sfx_siren1_down|sfx_siren2_up|sfx_siren2_down|sfx_dot)
#ifdef ALLEGROCURSES
#define handle_sfx(n) \
            do { if (sfx & sfx_ ## n) \
            { \
                static MIDI *n##_music = 0; \
                sfx &= ~sfx_ ## n; \
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
            do { if ((sfx & sfx_ ## n) && sdl_audio_open) \
            { \
                static Mix_Music *n##_music = 0; \
                sfx &= ~sfx_ ## n; \
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
#define handle_sfx(n) do { if (sfx & sfx_ ## n) { sfx &= ~sfx_##n; if ((sfx_##n & ~sfx_nobeep_mask) && use_sound && ! myman_demo) beep(); } } while (0)
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
    if (sfx) { sfx = 0UL; if (use_sound && ! myman_demo) beep(); }
    if (! (winning || NET_LIVES || dead || dying || ghost_eaten_timer || myman_intro || myman_demo || myman_start || intermission_running || need_reset))
    {
        key_buffer = ERR;
        myman_intro = 1;
        myman_start = 0;
        myman_demo = 0;
        myman_demo_setup = 0;
        lives_used = 0;
        earned = 0;
        winning = 1;
        ghost_eaten_timer = 0;
        level = 0;
        maze_level = 0;
        intermission = 0;
        intermission_shown = 0;
        for (s = 0; s < SPRITE_REGISTERS; s ++)
        {
            sprite_register_used[s] = 0;
            sprite_register_timer[s] = 0;
            sprite_register_frame[s] = 0;
        }
        maze_erase();
        oldplayer = 0;
        player = 1;
        pellet_timer = 0;
        pellet_time = PELLET_ADJUST(7 * ONESEC);
        cycles = 0;
        dots = 0;
        dead = 0;
        deadpan = 0;
        dying = 0;
    }
    pause_shown = 0;
#if MYMANDELAY
    if (delay
        &&
        (! myman_demo_setup)
        &&
        ! ((frames + frameskip) % (frameskip ? frameskip : 1)))
    {
        struct timeval tv2;
        unsigned long actual_delay;

        do
        {
            actual_delay = (myman_demo ? ((delay + mindelay) / 2) : delay) * (frameskip ? frameskip : 1);
            if (gettimeofday(&tv2, 0))
            {
                tv.tv_sec = 0;
                tv.tv_usec = 0;
                tv2.tv_sec = 0;
                tv2.tv_usec = 0;
                frameskip = 0;
            }
            if ((! (tv.tv_sec))
                &&
                (! (tv.tv_usec)))
            {
                tv.tv_sec = tv2.tv_sec - (actual_delay / 1000000L);
                tv.tv_usec = tv2.tv_usec - (actual_delay % 1000000L);
            }
            if ((tv2.tv_sec || tv2.tv_usec)
                &&
                (tv.tv_usec || tv.tv_sec))
            {
                if ((tv2.tv_sec > tv.tv_sec)
                    ||
                    ((tv2.tv_sec == tv.tv_sec)
                     &&
                     (tv2.tv_usec > tv.tv_usec)))
                {
                    unsigned long delta;
                    double nframeskip;
                    size_t kfr;
                    int use_buffer;
                    static double onframeskip[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

                    nframeskip = onframeskip[0];
                    for (kfr = 1; kfr < sizeof(onframeskip) / sizeof(*onframeskip); kfr ++)
                    {
                        nframeskip += onframeskip[kfr];
                        onframeskip[kfr - 1] = onframeskip[kfr];
                    }
                    nframeskip /= kfr;
                    use_buffer = (frameskip == (nframeskip + 0.5));
                    delta = (unsigned long int) ((tv2.tv_sec * 1e6 + tv2.tv_usec)
                                                 -
                                                 (tv.tv_sec * 1e6 + tv.tv_usec));
                    nframeskip = ((frameskip ? frameskip : 1) * delta) / (actual_delay ? actual_delay : 1);
                    if (nframeskip > MAXFRAMESKIP)
                    {
                        nframeskip = MAXFRAMESKIP;
                    }
                    onframeskip[kfr - 1] = nframeskip;
                    if (use_buffer) nframeskip = 0.0;
                    for (kfr = 0; kfr < sizeof(onframeskip) / sizeof(*onframeskip); kfr ++)
                    {
                        if (use_buffer)
                            nframeskip += onframeskip[kfr];
                        else
                            onframeskip[kfr] = nframeskip;
                    }
                    if (use_buffer) nframeskip /= kfr;
                    if (! ignore_delay)
                    {
                        frameskip = (unsigned long) (nframeskip + 0.5);
                    }
                    actual_delay = (myman_demo ? ((delay + mindelay) / 2) : delay) * (frameskip ? frameskip : 1);
                    if (delta <= actual_delay)
                    {
                        actual_delay -= delta;
                    }
                    else
                    {
                        actual_delay = 0;
                    }
                }
            }
            if (actual_delay)
            {
                unsigned long secs;

                secs = actual_delay / 999999L;
                while (secs --)
                {
                    usleep(999999UL);
                    actual_delay -= 999999UL;
                }
                if (actual_delay % 999999UL)
                {
                    usleep(actual_delay % 999999UL);
                }
                if (actual_delay == ((myman_demo ? ((delay + mindelay) / 2) : delay) * (frameskip ? frameskip : 1)))
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        while (1);
        ignore_delay = 0;
        if (gettimeofday(&tv, 0))
        {
            ignore_delay = 1;
            frameskip = 0;
        }
    }
#endif
    x_off = sprite_register_x[HERO] % gfx_w;
    y_off = sprite_register_y[HERO] % gfx_h;
    xtile = XTILE(sprite_register_x[HERO]);
    ytile = YTILE(sprite_register_y[HERO]);
  nextkey:
    {
        struct timeval tv_pre, tv_post;

        tv_pre.tv_sec = 0;
        tv_pre.tv_usec = 0;
        gettimeofday(&tv_pre, 0);
        k = getch();
        tv_post.tv_sec = 0;
        tv_post.tv_usec = 0;
        gettimeofday(&tv_post, 0);
        /* a very slow keypress is likely a sign of unmapping, suspending, or some similar mess */

        /* TODO: treat job control signals similarly */
        if (((1.0L * tv_post.tv_sec + 1e-6L * tv_post.tv_usec)
             -
             (1.0L * tv_pre.tv_sec + 1e-6L * tv_pre.tv_usec)) >= 1.0)
        {
            ignore_delay = 1;
            frameskip = 0;
        }
    }
#ifdef KEY_RESIZE
    if (k == KEY_RESIZE)
    {
        k = '@';
    }
#endif
    if ((k == 'q') || (k == 'Q') || (k == CTRL('C')) || quit_requested)
    {
        quit_requested = 0;
        return 0;
    }
    else if (k == CTRL('@'))
    {
        /* NUL - idle keepalive (iTerm, maybe others?) */
        return 1;
    }
    else if (k == CTRL('S'))
    {
        xoff_received = 1;
        return 1;
    }
    else if (k == CTRL('Q'))
    {
        xoff_received = 0;
        return 1;
    }
    else if ((k == '\?') || (k == CTRL('H')))
    {
        if (MYMANKEYS
            || maze_ABOUT || maze_FIXME || maze_NOTE
            || tile_ABOUT || tile_FIXME || tile_NOTE
            || sprite_ABOUT || sprite_FIXME || sprite_NOTE)
        {
            if (tmp_notice)
            {
                free((void *) tmp_notice);
                tmp_notice = 0;
            }
            tmp_notice = (char *) malloc(
                ((MYMANKEYS ? ((MYMANKEYS_prefix ? strlen(MYMANKEYS_prefix) : 0) + strlen(MYMANKEYS)) : 0)
                 + (maze_ABOUT ? ((maze_ABOUT_prefix ? strlen(maze_ABOUT_prefix) : 0) + strlen(maze_ABOUT)) : 0) + (maze_FIXME ? ((maze_FIXME_prefix ? strlen(maze_FIXME_prefix) : 0) + strlen(maze_FIXME)) : 0) + (maze_NOTE ? ((maze_NOTE_prefix ? strlen(maze_NOTE_prefix) : 0) + strlen(maze_NOTE)) : 0)
                 + (tile_ABOUT ? ((tile_ABOUT_prefix ? strlen(tile_ABOUT_prefix) : 0) + strlen(tile_ABOUT)) : 0) + (tile_FIXME ? ((tile_FIXME_prefix ? strlen(tile_FIXME_prefix) : 0) + strlen(tile_FIXME)) : 0) + (tile_NOTE ? ((tile_NOTE_prefix ? strlen(tile_NOTE_prefix) : 0) + strlen(tile_NOTE)) : 0)
                 + (sprite_ABOUT ? ((sprite_ABOUT_prefix ? strlen(sprite_ABOUT_prefix) : 0) + strlen(sprite_ABOUT)) : 0) + (sprite_FIXME ? ((sprite_FIXME_prefix ? strlen(sprite_FIXME_prefix) : 0) + strlen(sprite_FIXME)) : 0) + (sprite_NOTE ? ((sprite_NOTE_prefix ? strlen(sprite_NOTE_prefix) : 0) + strlen(sprite_NOTE)) : 0))
                + 1
                );
            if (tmp_notice)
            {
                *tmp_notice = '\0';
                if (MYMANKEYS) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (MYMANKEYS_prefix ? MYMANKEYS_prefix : ""), MYMANKEYS);
                if (maze_ABOUT) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (maze_ABOUT_prefix ? maze_ABOUT_prefix : ""), maze_ABOUT);
                if (maze_FIXME) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (maze_FIXME_prefix ? maze_FIXME_prefix : ""), maze_FIXME);
                if (maze_NOTE) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (maze_NOTE_prefix ? maze_NOTE_prefix : ""), maze_NOTE);
                if (tile_ABOUT) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (tile_ABOUT_prefix ? tile_ABOUT_prefix : ""), tile_ABOUT);
                if (tile_FIXME) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (tile_FIXME_prefix ? tile_FIXME_prefix : ""), tile_FIXME);
                if (tile_NOTE) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (tile_NOTE_prefix ? tile_NOTE_prefix : ""), tile_NOTE);
                if (sprite_ABOUT) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (sprite_ABOUT_prefix ? sprite_ABOUT_prefix : ""), sprite_ABOUT);
                if (sprite_FIXME) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (sprite_FIXME_prefix ? sprite_FIXME_prefix : ""), sprite_FIXME);
                if (sprite_NOTE) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (sprite_NOTE_prefix ? sprite_NOTE_prefix : ""), sprite_NOTE);
                pager_notice = tmp_notice;
                reinit_requested = 1;
            }
        }
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
    } else if ((k == 'r') || (k == 'R') || (k == CTRL('L')) || (k == CTRL('R'))) {
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
        if ((use_color = ! use_color))
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
        goto nextkey;
    } else if ((k == ',') || (k == '<')) {
        if (reflect ? (IS_LEFT_ARROW(key_buffer) || IS_RIGHT_ARROW(key_buffer)) : (IS_UP_ARROW(key_buffer) || IS_DOWN_ARROW(key_buffer)))
        {
            if (reflect ? ((hero_dir != LEFT) && (hero_dir != RIGHT)) : ((hero_dir != UP) && (hero_dir != DOWN)))
            {
                key_buffer = ERR;
            }
        }
    } else if ((k == '.') || (k == '>')) {
        if (reflect ? (IS_UP_ARROW(key_buffer) || IS_DOWN_ARROW(key_buffer)) : (IS_LEFT_ARROW(key_buffer) || IS_RIGHT_ARROW(key_buffer)))
        {
            if (reflect ? ((hero_dir != UP) && (hero_dir != DOWN)) : ((hero_dir != LEFT) && (hero_dir != RIGHT)))
            {
                key_buffer = ERR;
            }
        }
    } else if ((k == 'w') || (k == 'W')) {
        dots = total_dots[maze_level] - 1;
        goto nextkey;
    } else if ((k == 'd') || (k == 'D')) {
        debug = ! debug;
        DIRTY_ALL();
        ignore_delay = 1;
        frameskip = 0;
        goto nextkey;
    } else if ((reflect ? IS_UP_ARROW(((k == ERR) ? key_buffer : k)) : IS_LEFT_ARROW(((k == ERR) ? key_buffer : k)))
               && (ISOPEN((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile - NOTRIGHT(x_off))])
                   ||
                   ISZAPLEFT((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile]))) {
        if (! (winning || dying || (dead && ! ghost_eaten_timer)))
        {
            hero_dir = LEFT;
            sprite_register[HERO] = SPRITE_HERO + 4;
        }
    } else if ((reflect ? IS_DOWN_ARROW(((k == ERR) ? key_buffer : k)) : IS_RIGHT_ARROW(((k == ERR) ? key_buffer : k)))
               && (ISOPEN((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile + NOTLEFT(x_off))])
                   ||
                   ISZAPRIGHT((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile]))) {
        if (! (winning || dying || (dead && ! ghost_eaten_timer)))
        {
            hero_dir = RIGHT;
            sprite_register[HERO] = SPRITE_HERO + 12;
        }
    } else if ((reflect ? IS_LEFT_ARROW(((k == ERR) ? key_buffer : k)) : IS_UP_ARROW(((k == ERR) ? key_buffer : k)))
               && (ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile - NOTBOTTOM(y_off))) * (maze_w + 1)+xtile])
                   ||
                   ISZAPUP((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile]))) {
        if (! (winning || dying || (dead && ! ghost_eaten_timer)))
        {
            hero_dir = UP;
            sprite_register[HERO] = SPRITE_HERO;
        }
    } else if ((reflect ? IS_RIGHT_ARROW(((k == ERR) ? key_buffer : k)) : IS_DOWN_ARROW(((k == ERR) ? key_buffer : k)))
               && (ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile + NOTTOP(y_off))) * (maze_w + 1)+xtile])
                   ||
                   ISZAPDOWN((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile]))) {
        if (! (winning || dying || (dead && ! ghost_eaten_timer)))
        {
            hero_dir = DOWN;
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
        goto nextkey;
    }
    visible_frame = ! ((frames ++) % (frameskip ? frameskip : 1));
    if (myman_intro && ! (paused || snapshot || snapshot_txt))
    {
        int s;

        if (myman_intro == 1)
        {
            cycles = -1;
            for (s = 0; s < SPRITE_REGISTERS; s ++)
            {
                sprite_register_used[s] = 0;
                sprite_register_timer[s] = 0;
            }
            maze_erase();
            ghost_eaten_timer = 0;
        }
        for (s = 0; s < 4; s ++)
        {
            int eyes;
            int mean;
            int blue;
            int ghost;
            int nick_col, nick_row;
            int name_col, name_row;
            const char *ghost_name;
            const char *ghost_nick;

            ghost = (s == 0) ? GHOST1 : (s == 1) ? GHOST2 : (s == 2) ? GHOST0 : (s == 3) ? GHOST3 : s;
            ghost_nick = (ghost == GHOST0) ? GHOST0_NICK : (ghost == GHOST2) ? GHOST2_NICK : (ghost == GHOST1) ? GHOST1_NICK : (ghost == GHOST3) ? GHOST3_NICK : "";
            ghost_name = (ghost == GHOST0) ? GHOST0_NAME : (ghost == GHOST2) ? GHOST2_NAME : (ghost == GHOST1) ? GHOST1_NAME : (ghost == GHOST3) ? GHOST3_NAME : "-?????";
            name_row = 3 * (s + 1) + 1;
            if (name_row < 2) name_row = 2;
            name_col = 7 + (maze_w - 28) / 2;
            if (name_col < (sgfx_w + gfx_w - 1) / gfx_w) name_col = (sgfx_w + gfx_w - 1) / gfx_w;
            nick_row = name_row;
            nick_col = name_col + (int) strlen(NAME_HEADER);
            if (nick_col + (int) strlen(NICK_HEADER) > maze_w)
            {
                nick_col = name_col + 1;
                nick_row = name_row + 1;
            }
            if ((! s) && (myman_intro == 1))
            {
                maze_puts(name_row - 2, name_col, 0xF, NAME_HEADER);
                maze_puts(nick_row - 2, nick_col, 0xF, NICK_HEADER);
            }
            if (ghost >= ghosts) continue;
            eyes = GHOSTEYES(ghost);
            mean = MEANGHOST(ghost);
            blue = BLUEGHOST(ghost);
            if (myman_intro == (1 + s * 3 * TWOSECS / 4))
            {
                int hero_x;

                hero_x = (9 + maze_w - 28) * gfx_w / 2;
                if (hero_x < sgfx_w / 2)
                {
                    hero_x = XPIXWRAP(sgfx_w / 2);
                }
                sprite_register_used[eyes] = VISIBLE_EYES;
                sprite_register_used[mean] = 1;
                sprite_register_used[blue] = 0;
                sprite_register_frame[mean] = 0;
                sprite_register_x[HERO] =
                    sprite_register_x[eyes] =
                    sprite_register_x[mean] = hero_x;
                sprite_register_y[HERO] =
                    sprite_register_y[eyes] =
                    sprite_register_y[mean] = (6 * s + 9) * gfx_h / 2;
                if (sprite_register_y[HERO] < sgfx_h / 2)
                {
                    sprite_register_y[HERO] =
                        sprite_register_y[eyes] =
                        sprite_register_y[mean] = YPIXWRAP(sgfx_h / 2);
                }
                deadpan = 0;
                sprite_register_frame[eyes] = RIGHT - 1;
            }
            else if ((myman_intro > (1 + s * 3 * TWOSECS / 4))
                     &&
                     (myman_intro < (1 + (s * 3 + 1) * TWOSECS / 4)))
            {
                sprite_register_x[HERO] -= ((myman_intro - 1) - (1 + s * 3 * TWOSECS / 4)) * (name_col * gfx_w - sprite_register_x[mean]) / (1 + TWOSECS / 4);
                sprite_register_x[HERO] += (myman_intro - (1 + s * 3 * TWOSECS / 4)) * (name_col * gfx_w - sprite_register_x[mean]) / (1 + TWOSECS / 4);
            }
            if (myman_intro == (1 + (s * 3 + 1) * TWOSECS / 4))
            {
#if USE_COLOR
#define MEANCOLOR sprite_register_color[mean]
#else
#define MEANCOLOR 0xF
#endif                            
                sprite_register_x[HERO] = name_col * gfx_w + gfx_w / 2;
                maze_puts(name_row, name_col,
                          sprite_color[sprite_register[mean] + sprite_register_frame[mean]]
                          ?
                          sprite_color[sprite_register[mean] + sprite_register_frame[mean]]
                          :
                          MEANCOLOR
                          ,
                          ghost_name);
            }
            else if ((myman_intro > (1 + (s * 3 + 1) * TWOSECS / 4))
                     &&
                     (myman_intro < (1 + (s * 3 + 2) * TWOSECS / 4)))
            {
                sprite_register_x[HERO] -= ((myman_intro - 1) - (1 + (s * 3 + 1) * TWOSECS / 4)) * ((int) strlen(NAME_HEADER) * gfx_w) / (1 + TWOSECS / 4);
                sprite_register_x[HERO] += (myman_intro - (1 + (s * 3 + 1) * TWOSECS / 4)) * ((int) strlen(NAME_HEADER) * gfx_w) / (1 + TWOSECS / 4);
            }
            if (myman_intro == (1 + (s * 3 + 2) * TWOSECS / 4))
            {
                sprite_register_x[HERO] = nick_col * gfx_w + gfx_w / 2;
                maze_puts(nick_row, nick_col,
                          sprite_color[sprite_register[mean] + sprite_register_frame[mean]]
                          ?
                          sprite_color[sprite_register[mean] + sprite_register_frame[mean]]
                          :
                          MEANCOLOR
                          ,
                          ghost_nick);
            }
            else if ((myman_intro > (1 + (s * 3 + 2) * TWOSECS / 4))
                     &&
                     (myman_intro < (1 + ((s + 1) * 3) * TWOSECS / 4)))
            {
                sprite_register_x[HERO] -= ((myman_intro - 1) - (1 + (s * 3 + 2) * TWOSECS / 4)) * ((int) strlen(NICK_HEADER) * gfx_w) / (1 + TWOSECS / 4);
                sprite_register_x[HERO] += (myman_intro - (1 + (s * 3 + 2) * TWOSECS / 4)) * ((int) strlen(NICK_HEADER) * gfx_w) / (1 + TWOSECS / 4);
            }
            if (myman_intro > (4 * 3 * TWOSECS / 4))
            {
                sprite_register_frame[mean] = ((myman_intro / FIFTH) & 1) ? 1 : 0;
            }
        }
        if (myman_intro == (1 + 4 * 3 * TWOSECS / 4))
        {
            sprite_register_x[HERO] = gfx_w * (maze_w + 1) - sgfx_w;
            sprite_register_y[HERO] = ((17 * 2 + 1) * gfx_h) / 2;
            if ((YTILE(sprite_register_y[HERO]) + 6) >= maze_h)
            {
                maze_erase();
                for (s = 0; s < SPRITE_REGISTERS; s ++)
                {
                    sprite_register_used[s] = 0;
                    sprite_register_timer[s] = 0;
                    sprite_register_frame[s] = 0;
                }
                sprite_register_y[HERO] = 3 * gfx_h / 2;
            }
            sprite_register_frame[HERO] = 0;
            sprite_register[HERO] = SPRITE_HERO + 4;
            sprite_register_used[HERO] = 1;
            if ((YTILE(sprite_register_y[HERO]) + 11) < maze_h)
            {
                maze_puts(YWRAP(YTILE(sprite_register_y[HERO]) + 11), ((int) maze_w - (int) strlen(MYMANNOTICE)) / 2, 0xD, MYMANNOTICE);
            }
            else
            {
                maze_puts(YWRAP(maze_h - 1), ((int) maze_w - (int) strlen(MYMANNOTICE)) / 2, 0xD, MYMANNOTICE);
            }
            maze_puts(YTILE(sprite_register_y[HERO]) + 6, 12 + (maze_w - 28) / 2, 0xF, "50 \x9es");
            maze_puts(YTILE(sprite_register_y[HERO]) + 4, 12 + (maze_w - 28) / 2, 0xF, "10 \x9es");
            maze_puts(YTILE(sprite_register_y[HERO]) + 6, 10 + (maze_w - 28) / 2, 0x7, "\xfe");
            maze_puts(YTILE(sprite_register_y[HERO]) + 4, 10 + (maze_w - 28) / 2, 0x7, "\xf9");
            maze_puts(YTILE(sprite_register_y[HERO]), 4 + (maze_w - 28) / 2, 0x7, "\xfe");
        }
        else if (sprite_register_used[HERO] || ghost_eaten_timer)
        {
            if (ghost_eaten_timer)
            {
                ghost_eaten_timer --;
                if (! ghost_eaten_timer)
                {
                    sprite_register_used[HERO] = 1;
                    sprite_register_used[GHOST_SCORE] = 0;
                }
            }
            else
            {
                sprite_register_frame[HERO] = (myman_intro / (1 + (FIFTH / 2))) % 4;
                if ((sprite_register[HERO] == (SPRITE_HERO + 4))
                    &&
                    (sprite_register_x[HERO] == gfx_w * (4 + (maze_w - 28) / 2)))
                {
                    maze_puts(YTILE(sprite_register_y[HERO]), 4 + (maze_w - 28) / 2, 0x7, " ");
                    sprite_register[HERO] = SPRITE_HERO + 12;
                    sprite_register_frame[HERO] = 0;
                }
                else if (sprite_register[HERO] == (SPRITE_HERO + 4))
                {
                    sprite_register_x[HERO] --;
                }
                else
                {
                    sprite_register_x[HERO] ++;
                }
            }
            for (s = 0; s < 4; s ++)
            {
                int eyes;
                int mean;
                int blue;
                int ghost;
                            
                ghost = MAXGHOSTS - 4 + ((s == 0) ? GHOST1 : (s == 1) ? GHOST2 : (s == 2) ? GHOST0 : (s == 3) ? GHOST3 : s);
                if ((ghost < ghosts) || (ghost >= MAXGHOSTS)) continue;
                eyes = GHOSTEYES(ghost);
                mean = MEANGHOST(ghost);
                blue = BLUEGHOST(ghost);
                sprite_register[eyes] = SPRITE_EYES;
                sprite_register[mean] = SPRITE_MEAN;
                sprite_register[blue] = SPRITE_BLUE;
                sprite_register_y[eyes] =
                    sprite_register_y[mean] =
                    sprite_register_y[blue] = sprite_register_y[HERO];
                sprite_register_frame[eyes] = RIGHT - 1;
                sprite_register_frame[blue] =
                    (sprite_register_frame[mean] = ((myman_intro / FIFTH) & 1) ? 1 : 0);
                if ((sprite_register[HERO] == (SPRITE_HERO + 4)) && ! ghost_eaten_timer)
                {
                    sprite_register_frame[eyes] = LEFT - 1;
                    sprite_register_used[eyes] = VISIBLE_EYES;
                    sprite_register_used[mean] = 1;
                    sprite_register_used[blue] = 0;
                    sprite_register_x[eyes] =
                        sprite_register_x[mean] =
                        sprite_register_x[blue] = 3 * (sprite_register_x[HERO] - gfx_w * (3 + (maze_w - 28) / 2)) / 2 + sgfx_w * s + gfx_w * (4 + (maze_w - 28) / 2);
                }
                else if (sprite_register_used[mean] && ! ghost_eaten_timer)
                {
                    sprite_register_used[eyes] =
                        sprite_register_used[mean] = 0;
                    sprite_register_used[blue] = 1;
                }
                else if (sprite_register_used[eyes])
                {
                    sprite_register_x[eyes] =
                        sprite_register_x[mean] =
                        sprite_register_x[blue] += 2;
                }
                else if (sprite_register_used[blue] && collide(blue, HERO) && ! ghost_eaten_timer)
                {
                    sprite_register_used[blue] = 0;
                    sprite_register_used[eyes] = 1;
                    ghost_eaten_timer = ONESEC;
                    sprite_register_frame[HERO] = 0;
                    sprite_register_used[HERO] = 0;
                    sprite_register_used[GHOST_SCORE] = 1;
                    sprite_register_x[GHOST_SCORE] = sprite_register_x[blue];
                    sprite_register_y[GHOST_SCORE] = sprite_register_y[blue];
                    sprite_register_frame[GHOST_SCORE] = s;
                }
                else if (sprite_register_used[blue] && (myman_intro & 1) && ! ghost_eaten_timer)
                {
                    sprite_register_x[eyes] =
                        sprite_register_x[mean] =
                        sprite_register_x[blue] ++;
                }
#if USE_COLOR
                sprite_register_color[eyes] = 0xF;
                sprite_register_color[blue] = 0x9;
                sprite_register_color[mean] = (EXTRA_GHOST_COLORS)[(s % strlen(EXTRA_GHOST_COLORS))];
                if (s == 2) sprite_register_color[mean] = 0xB;
                if (s == 0) sprite_register_color[mean] = 0xC;
                if (s == 1) sprite_register_color[mean] = 0xD;
                if (s == 3) sprite_register_color[mean] = 0x6;
#endif
            }
        }
        if (! ghost_eaten_timer)
            for (s = 0; s < SPRITE_REGISTERS; s ++) {
                if (sprite_register_used[s] && sprite_register_timer[s])
                    if (! -- sprite_register_timer[s])
                        sprite_register_used[s] = 0;
            }
        myman_intro ++;
        cycles ++;
        if (((! ghost_eaten_timer)
             &&
             ((sprite_register_frame[GHOST_SCORE] == 3)
              ||
              ((sprite_register_used[HERO])
               &&
               (sprite_register[HERO] == (SPRITE_HERO + 12))
               &&
               (XTILE(sprite_register_x[HERO]) >= maze_w))))
            ||
            (k != ERR))
        {
            myman_intro = 0;
            myman_demo = 1;
        }
    }
    if (myman_demo && ! (paused || snapshot || snapshot_txt))
    {
        if ((myman_demo == 1) && (! myman_demo_setup))
        {
            level = 0;
            maze_level = 0;
            intermission = 0;
            intermission_shown = 0;
            for (s = 0; s < SPRITE_REGISTERS; s ++)
            {
                sprite_register_used[s] = 0;
                sprite_register_timer[s] = 0;
                sprite_register_frame[s] = 0;
            }
            maze_erase();
            ghost_eaten_timer = 0;
            winning = 1;
            oldplayer = 0;
            player = 1;
            pellet_timer = 0;
            pellet_time = PELLET_ADJUST(7 * ONESEC);
            for (s = 0; s < frames % 8; s ++)
            {
                pellet_time -= PELLET_ADJUST(7 * ONESEC);
                if (level && (FLIP_ALWAYS ||
                              INTERMISSION(level)))
                {
                    ++ maze_level;
                    maze_level %= maze_n;
                    if (! maze_level)
                    {
                        maze_level = flip_to % maze_n;
                    }
                    if (FLIP_LOCK && ! maze_level)
                    {
                        maze_level = maze_n - 1;
                    }
                }
                ++ level;
                sprite_register_frame[FRUIT] =
                    sprite_register_frame[FRUIT_SCORE] = BONUS(level);
                pellet_time += PELLET_ADJUST(7 * ONESEC);
                if (pellet_time > PELLET_ADJUST(ONESEC))
                    pellet_time -= PELLET_ADJUST(ONESEC);
                else
                    pellet_time = 0;
            }
            cycles = 0;
            dots = 0;
            dead = 0;
            deadpan = 0;
            dying = 0;
            myman_demo_setup = 1 + (15UL * (maze_h * maze_w) * TWOSECS / (28 * 31)) / 2;
        }
        if (! (winning || dying || (dead && ! ghost_eaten_timer)))
        {
            if (! (frames % ((TWOSECS / 20) + 1)))
            {
                if (ISOPEN((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile - NOTRIGHT(x_off))])
                    &&
                    ISPELLET((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile - NOTRIGHT(x_off))]))
                {
                    hero_dir = LEFT;
                    sprite_register[HERO] = SPRITE_HERO + 4;
                }
                else if (ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile + NOTTOP(y_off))) * (maze_w + 1)+xtile])
                         &&
                         ISPELLET((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile + NOTTOP(y_off))) * (maze_w + 1)+xtile]))
                {
                    hero_dir = DOWN;
                    sprite_register[HERO] = SPRITE_HERO + 16;
                }
                else if (ISOPEN((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile + NOTLEFT(x_off))])
                         &&
                         ISPELLET((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile + NOTLEFT(x_off))]))
                {
                    hero_dir = RIGHT;
                    sprite_register[HERO] = SPRITE_HERO + 12;
                }
                else if (ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile - NOTBOTTOM(y_off))) * (maze_w + 1)+xtile])
                         &&
                         ISPELLET((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile - NOTBOTTOM(y_off))) * (maze_w + 1)+xtile]))
                {
                    hero_dir = UP;
                    sprite_register[HERO] = SPRITE_HERO;
                }
                else if (ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile - NOTBOTTOM(y_off))) * (maze_w + 1)+xtile])
                         &&
                         ISDOT((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile - NOTBOTTOM(y_off))) * (maze_w + 1)+xtile]))
                {
                    hero_dir = UP;
                    sprite_register[HERO] = SPRITE_HERO;
                }
                else if (ISOPEN((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile - NOTRIGHT(x_off))])
                         &&
                         ISDOT((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile - NOTRIGHT(x_off))]))
                {
                    hero_dir = LEFT;
                    sprite_register[HERO] = SPRITE_HERO + 4;
                }
                else if (ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile + NOTTOP(y_off))) * (maze_w + 1)+xtile])
                         &&
                         ISDOT((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile + NOTTOP(y_off))) * (maze_w + 1)+xtile]))
                {
                    hero_dir = DOWN;
                    sprite_register[HERO] = SPRITE_HERO + 16;
                }
                else if (ISOPEN((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile + NOTLEFT(x_off))])
                         &&
                         ISDOT((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile + NOTLEFT(x_off))]))
                {
                    hero_dir = RIGHT;
                    sprite_register[HERO] = SPRITE_HERO + 12;
                }
                else if (ISOPEN((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile - NOTRIGHT(x_off))])
                         &&
                         (hero_dir != RIGHT))
                {
                    hero_dir = LEFT;
                    sprite_register[HERO] = SPRITE_HERO + 4;
                }
                else if (ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile - NOTBOTTOM(y_off))) * (maze_w + 1)+xtile])
                         &&
                         (hero_dir != DOWN))
                {
                    hero_dir = UP;
                    sprite_register[HERO] = SPRITE_HERO;
                }
                else if (ISOPEN((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile + NOTLEFT(x_off))])
                         &&
                         (hero_dir != LEFT))
                {
                    hero_dir = RIGHT;
                    sprite_register[HERO] = SPRITE_HERO + 12;
                }
                else if (ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile + NOTTOP(y_off))) * (maze_w + 1)+xtile])
                         &&
                         (hero_dir != UP))
                {
                    hero_dir = DOWN;
                    sprite_register[HERO] = SPRITE_HERO + 16;
                }
            }
        }
        if (myman_demo_setup)
        {
            myman_demo_setup --;
        }
        if (! myman_demo_setup)
        {
            myman_demo ++;
        }
        maze_puts(rmsg, cmsg, MSG_COLOR, msg_GAMEOVER);
        if ((myman_demo > (1 + (20UL * (maze_h * maze_w) * TWOSECS / (28 * 31)) / 2))
            ||
            (k != ERR))
        {
            ghost_eaten_timer = 0;
            myman_demo = 0;
            myman_demo_setup = 0;
            myman_intro = (k == ERR);
            myman_start = (k != ERR) ? (30 * ONESEC) : 0;
            if (myman_start)
            {
                sfx |= sfx_credit;
            }
            winning = 1;
            oldplayer = 0;
            player = 1;
            pellet_timer = 0;
            pellet_time = PELLET_ADJUST(7 * ONESEC);
            cycles = 0;
            dots = 0;
            dead = 0;
            deadpan = 0;
            dying = 0;
            level = 0;
            maze_level = 0;
            intermission = 0;
            intermission_shown = 0;
            for (s = 0; s < SPRITE_REGISTERS; s ++)
            {
                sprite_register_used[s] = 0;
                sprite_register_timer[s] = 0;
                sprite_register_frame[s] = 0;
            }
            maze_erase();
            if (myman_start)
            {
                int r_off;

                r_off = maze_h - 16;
                if (r_off < 0)
                {
                    r_off = 0;
                }
                maze_puts(r_off + 12, ((int) maze_w - (int) strlen(MYMANNOTICE)) / 2, 0xD, MYMANNOTICE);
                maze_puts(r_off + 8, ((int) maze_w - (int) strlen(BONUSHEROTEXT)) / 2, 0x7, BONUSHEROTEXT);
                maze_puts(r_off + 4, ((int) maze_w - (int) strlen(CREDIT(1))) / 2, MSG2_COLOR, CREDIT(1));
                maze_puts(r_off, ((int) maze_w - (int) strlen(START)) / 2, 0x6, START);
                if (debug)
                {
                    for (s = 0; s < 256; s ++)
                    {
                        if ((s / 16) < maze_h)
                            if ((s % 16) < maze_w)
                                maze_puts(maze_h - 1 - (s / 16), s % 16, s, "*");
                    }
                }
                sprite_register_x[HERO] = maze_w * gfx_w / 2;
                sprite_register_y[HERO] = r_off * gfx_h;
                k = ERR;
                key_buffer = ERR;
            }
        }
    }
    if (myman_start && ! (paused || snapshot || snapshot_txt))
    {
        myman_start --;
        if ((! myman_start)
            ||
            (k != ERR))
        {
            level = 0;
            maze_level = 0;
            intermission = 0;
            intermission_shown = 0;
            maze_erase();
            ghost_eaten_timer = 0;
            winning = 1;
            oldplayer = 0;
            player = 1;
            pellet_timer = 0;
            pellet_time = PELLET_ADJUST(7 * ONESEC);
            cycles = 0;
            score = 0;
            dots = 0;
            dead = 0;
            deadpan = 0;
            dying = 0;
            myman_start = 0;
        }
    }
    if (intermission_running && ! (paused || snapshot || snapshot_txt))
    {
        if (intermission_running == (1 + INTERMISSION_TIME))
        {
            for (s = 0; s < SPRITE_REGISTERS; s ++)
            {
                sprite_register_used[s] = 0;
                sprite_register_timer[s] = 0;
                sprite_register_frame[s] = 0;
            }
            maze_erase();
            sfx |= sfx_intermission;
        }
        intermission_running --;
        if (intermission == 0)
        {
            if ((INTERMISSION_TIME - intermission_running) <= PIX_W)
            {
                sprite_register_used[HERO] = 1;
                sprite_register_y[HERO] = (maze_h / 2 - 2) * gfx_h;
                sprite_register_x[HERO] = PIX_W - gfx_w / 2 - (INTERMISSION_TIME - intermission_running);
                sprite_register[HERO] = SPRITE_HERO + 4;
                sprite_register_frame[HERO] = ((INTERMISSION_TIME - intermission_running) / (1 + (FIFTH / 2))) % 4;
                if (GHOST1 < ghosts)
                {
                    int mean;
                    int eyes;
                    int blue;

                    mean = MEANGHOST(GHOST1);
                    eyes = GHOSTEYES(GHOST1);
                    blue = BLUEGHOST(GHOST1);
                    sprite_register_x[mean] =
                        sprite_register_x[eyes] =
                        sprite_register_x[blue] = 2 * sprite_register_x[HERO] + sgfx_w;
                    sprite_register_y[mean] =
                        sprite_register_y[eyes] =
                        sprite_register_y[blue] = sprite_register_y[HERO];
                    sprite_register[mean] = SPRITE_MEAN;
                    sprite_register[eyes] = SPRITE_EYES;
                    sprite_register[blue] = SPRITE_BLUE;
                    sprite_register_frame[eyes] = LEFT - 1;
                    sprite_register_frame[blue] =
                        (sprite_register_frame[mean] = (((INTERMISSION_TIME - intermission_running) / FIFTH) & 1) ? 1 : 0);
                    sprite_register_used[mean] = 1;
                    sprite_register_used[eyes] = VISIBLE_EYES;
                    sprite_register_used[blue] = 0;
                }
            }
            else if ((intermission_running <= (2 * PIX_W)) && intermission_running)
            {
                /* TODO: need a giant hero here */
                sprite_register_used[HERO] = 1;
                sprite_register_used[BIGHERO_UL] = 0;
                sprite_register_used[BIGHERO_UR] = 0;
                sprite_register_used[BIGHERO_LL] = 0;
                sprite_register_used[BIGHERO_LR] = 0;
                if (sprite_used[SPRITE_BIGHERO_UL]
                    &&
                    sprite_used[SPRITE_BIGHERO_UR]
                    &&
                    sprite_used[SPRITE_BIGHERO_LL]
                    &&
                    sprite_used[SPRITE_BIGHERO_LR])
                {
                    sprite_register_used[HERO] = 0;
                    sprite_register_used[BIGHERO_UL] = 1;
                    sprite_register_used[BIGHERO_UR] = 1;
                    sprite_register_used[BIGHERO_LL] = 1;
                    sprite_register_used[BIGHERO_LR] = 1;
                }
                sprite_register_y[HERO] = (maze_h / 2 + 2) * gfx_h;
                sprite_register_x[HERO] = PIX_W - gfx_w / 2 - intermission_running;
                sprite_register_y[BIGHERO_UL] = sprite_register_y[HERO] - sgfx_h;
                sprite_register_y[BIGHERO_UR] = sprite_register_y[HERO] - sgfx_h;
                sprite_register_y[BIGHERO_LL] = sprite_register_y[HERO];
                sprite_register_y[BIGHERO_LR] = sprite_register_y[HERO];
                sprite_register_x[BIGHERO_UL] = sprite_register_x[HERO] - sgfx_w;
                sprite_register_x[BIGHERO_UR] = sprite_register_x[HERO];
                sprite_register_x[BIGHERO_LL] = sprite_register_x[HERO] - sgfx_w;
                sprite_register_x[BIGHERO_LR] = sprite_register_x[HERO];
                sprite_register[HERO] = SPRITE_HERO + 12;
                sprite_register[BIGHERO_UL] = SPRITE_BIGHERO_UL;
                sprite_register[BIGHERO_UR] = SPRITE_BIGHERO_UR;
                sprite_register[BIGHERO_LL] = SPRITE_BIGHERO_LL;
                sprite_register[BIGHERO_LR] = SPRITE_BIGHERO_LR;
                sprite_register_frame[HERO] = ((INTERMISSION_TIME - intermission_running) / (1 + (FIFTH / 2))) % 4;
                sprite_register_frame[BIGHERO_UL] = sprite_register_frame[HERO];
                sprite_register_frame[BIGHERO_UR] = sprite_register_frame[HERO];
                sprite_register_frame[BIGHERO_LL] = sprite_register_frame[HERO];
                sprite_register_frame[BIGHERO_LR] = sprite_register_frame[HERO];
                if (GHOST1 < ghosts)
                {
                    int mean;
                    int eyes;
                    int blue;

                    mean = MEANGHOST(GHOST1);
                    eyes = GHOSTEYES(GHOST1);
                    blue = BLUEGHOST(GHOST1);
                    sprite_register_x[mean] =
                        sprite_register_x[eyes] =
                        sprite_register_x[blue] = PIX_W - gfx_w / 2 - (PIX_W - gfx_w / 2 - sprite_register_x[HERO]) / 2 + sgfx_w;
                    sprite_register_y[mean] =
                        sprite_register_y[eyes] =
                        sprite_register_y[blue] = sprite_register_y[HERO];
                    sprite_register[mean] = SPRITE_MEAN;
                    sprite_register[eyes] = SPRITE_EYES;
                    sprite_register[blue] = SPRITE_BLUE;
                    sprite_register_frame[eyes] = LEFT - 1;
                    sprite_register_frame[blue] =
                        (sprite_register_frame[mean] = (((INTERMISSION_TIME - intermission_running) / FIFTH) & 1) ? 1 : 0);
                    sprite_register_used[mean] = 0;
                    sprite_register_used[eyes] = 0;
                    sprite_register_used[blue] = 1;
                }
            }
            else
            {
                for (s = 0; s < SPRITE_REGISTERS; s ++)
                {
                    sprite_register_used[s] = 0;
                    sprite_register_timer[s] = 0;
                    sprite_register_frame[s] = 0;
                }
            }
        }
        else
        {
            maze_erase();
            sprite_register_y[HERO] = gfx_h * (maze_h * intermission_running / (1 + INTERMISSION_TIME));
            sprite_register_x[HERO] = gfx_w * maze_w / 2;
            maze_puts(YTILE(sprite_register_y[HERO]), (maze_w - (int) strlen("COFFEE BREAK")) / 2, 1 + 0xE * intermission_running / (1 + INTERMISSION_TIME), "COFFEE BREAK");
        }
        if ((! intermission_running) || myman_demo || (k != ERR))
        {
            if (! myman_demo) k = ERR;
            intermission_running = 0;
            need_reset = 1;
            for (s = 0; s < SPRITE_REGISTERS; s ++)
            {
                sprite_register_used[s] = 0;
            }
        }
    }
    if (! (paused || snapshot || snapshot_txt || myman_intro || myman_start || intermission_running))
    {
        if (ghost_eaten_timer && ! -- ghost_eaten_timer) {
            sprite_register_used[GHOST_SCORE] = 0;
            sprite_register_frame[GHOST_SCORE] ++;
            if ((sprite_register[GHOST_SCORE] + sprite_register_frame[GHOST_SCORE]) > SPRITE_1600) sprite_register_frame[GHOST_SCORE] = SPRITE_1600 - sprite_register[GHOST_SCORE];
            if (sprite_register_used[HERO])
            {
                memcpy((void *) (maze + (maze_level * maze_h + rmsg) * (maze_w + 1) + cmsg),
                       (void *) (blank_maze + (maze_level * maze_h + rmsg) * (maze_w + 1) + cmsg),
                       MIN(msglen, maze_w - cmsg));
                memcpy((void *) (maze_color + (maze_level * maze_h + rmsg) * (maze_w + 1) + cmsg),
                       (void *) (blank_maze_color + (maze_level * maze_h + rmsg) * (maze_w + 1) + cmsg),
                       MIN(msglen, maze_w - cmsg));
                {
                    int dirty_i;
                    for (dirty_i = 0; dirty_i < msglen; dirty_i ++)
                    {
                        DIRTY_CELL(cmsg + dirty_i, rmsg);
                    }
                }
                sfx |= sfx_siren0_up;
            }
            else
            {
                sprite_register_used[HERO] = NET_LIVES ? 1 : 0;
                sprite_register_used[munched] = (NET_LIVES || (munched != HERO)) ? 1 : 0;
            }
        }
        if (winning || dying || need_reset) {
            int reset = 0;
            static int init = 0;

            if (! need_reset)
            {
                if (dying && ! -- dying) {
                    reset = 1;
                    if (! myman_demo) ++ lives_used;
                } else if ((! ghost_eaten_timer) && winning && ! -- winning) {
                    need_reset = 1;
                    if (INTERMISSION(level)
                        &&
                        (intermission < INTERMISSION_N))
                    {
                        intermission_running = (1 + INTERMISSION_TIME);
                        key_buffer = ERR;
                        return 1;
                    }
                }
            }
            if (need_reset)
            {
                sfx |= sfx_start;
                need_reset = 0;
                reset = 1;
                memcpy((void *)maze,
                       (void *)blank_maze,
                       (maze_w + 1) * maze_h * maze_n * sizeof(unsigned char));
                memcpy((void *)maze_color,
                       (void *)blank_maze_color,
                       (maze_w + 1) * maze_h * maze_n * sizeof(unsigned char));
                DIRTY_ALL();
                ignore_delay = 1;
                frameskip = 0;
                dots = 0;
                pellet_timer = 0;
                pellet_time -= PELLET_ADJUST(7 * ONESEC);
                if (level && (FLIP_ALWAYS ||
                              INTERMISSION(level)))
                {
                    ++ maze_level;
                    maze_level %= maze_n;
                    if (! maze_level)
                    {
                        maze_level = flip_to % maze_n;
                    }
                    if (FLIP_LOCK && ! maze_level)
                    {
                        maze_level = maze_n - 1;
                    }
                }
                if (INTERMISSION(level) &&
                    (intermission < INTERMISSION_N))
                {
                    ++ intermission_shown;
                    if (intermission_shown >= INTERMISSION_REPEAT(intermission))
                    {
                        ++ intermission;
                        intermission_shown = 0;
                    }
                }
                ++ level;
                sprite_register_frame[FRUIT] =
                    sprite_register_frame[FRUIT_SCORE] = BONUS(level);
                pellet_time += PELLET_ADJUST(7 * ONESEC);
                if (pellet_time > PELLET_ADJUST(ONESEC))
                    pellet_time -= PELLET_ADJUST(ONESEC);
                else
                    pellet_time = 0;
                if (! myman_demo)
                {
                    if ((delay > SPEEDUP)
                        &&
                        (delay >= (mindelay + SPEEDUP)))
                    {
                        delay -= SPEEDUP;
                    }
                }
            }
            if ((! init) && (winning || reset)) {
                init = 1;
                if (sprite_register_used[FRUIT])
                {
                    DIRTY_ALL();
                    ignore_delay = 1;
                    frameskip = 0;
                }
                sprite_register_used[FRUIT] =
                    sprite_register_used[FRUIT_SCORE] =
                    sprite_register_used[GHOST_SCORE] = 0;
                for (i = 0; i < ghosts; i++) {
                    int eyes, mean, blue;

                    eyes = GHOSTEYES(i);
                    mean = MEANGHOST(i);
                    blue = BLUEGHOST(i);
                    sprite_register_used[eyes] =
                        sprite_register_used[mean] =
                        sprite_register_used[blue] = 0;
                }
                sprite_register_frame[HERO] = 0;
            }
            if (reset) {
                memset((void *)home_dir, 0, sizeof(home_dir));
                for (i = 0; i < ghosts; i++) {
                    int eyes, mean, blue;

                    eyes = GHOSTEYES(i);
                    mean = MEANGHOST(i);
                    blue = BLUEGHOST(i);
                    sprite_register_used[eyes] =
                        sprite_register_used[mean] =
                        sprite_register_used[blue] = 0;
                    sprite_register_frame[eyes] = (ghost_dir[i] = DIRWRAP(i + 1)) - 1;
                    ghost_mem[i] = 0;
                    ghost_timer[i] = TWOSECS;
                    ghost_man[i] = 0;
                    sprite_register_x[mean] =
                        (sprite_register_x[eyes] = (int) (XGHOST + COGHOST * gfx_w *
                                                          ((i == GHOST3) - (i == GHOST0))));
                    sprite_register_y[mean] =
                        (sprite_register_y[eyes] = (int) (YGHOST - ROGHOST * gfx_h *
                                                          (i == GHOST1)));
                }
                key_buffer = ERR;
                init = 0;
                cycles = 0;
                sprite_register_used[HERO] = 0;
                sprite_register_frame[HERO] = 0;
                sprite_register_x[HERO] = (int) XHERO;
                sprite_register_y[HERO] = (int) YHERO;
                dead =
                    deadpan = (int) (YHERO - YGHOST);
                if (dead < 0)
                    dead = -dead;
                if (dead < 2)
                    dead = 2;
                maze_putsn_nonblank(rmsg, cmsg, MSG_COLOR, (NET_LIVES && ! myman_demo) ? msg_READY : msg_GAMEOVER, msglen);
                if (player != oldplayer)
                {
                    oldplayer = player;
                    ghost_eaten_timer = myman_demo ? 0 : ONESEC;
                    munched = HERO;
                    if (! myman_demo)
                    {
                        maze_putsn_nonblank(rmsg2, cmsg2, MSG2_COLOR, PLAYER(player), msglen);
                    }
                }
            }
            if (dying)
                sprite_register_frame[HERO] = ((DEATHDELAY - dying) >> DEATHSHIFT) % 4;
        } else if (dead && ! ghost_eaten_timer) {
            if (deadpan)
                deadpan += (YHERO < YGHOST) - (YHERO > YGHOST);
            if (! -- dead) {
                for (i = 0; i < ghosts; i ++) {
                    int eyes, mean, blue;

                    eyes = GHOSTEYES(i);
                    mean = MEANGHOST(i);
                    blue = BLUEGHOST(i);
                    sprite_register_used[eyes] = NET_LIVES ? VISIBLE_EYES : 0;
                    sprite_register_used[mean] = NET_LIVES ? 1 : 0;
                }
                sprite_register_used[HERO] = NET_LIVES ? 1 : 0;
                ghost_eaten_timer = ONESEC;
                munched = HERO;
                if (! myman_demo)
                {
                    memcpy((void *) (maze + (maze_level * maze_h + rmsg2) * (maze_w + 1) + cmsg2),
                           (void *) (blank_maze + (maze_level * maze_h + rmsg2) * (maze_w + 1) + cmsg2),
                           MIN(msglen, maze_w - cmsg2));
                    memcpy((void *) (maze_color + (maze_level * maze_h + rmsg2) * (maze_w + 1) + cmsg2),
                           (void *) (blank_maze_color + (maze_level * maze_h + rmsg2) * (maze_w + 1) + cmsg2),
                           MIN(msglen, maze_w - cmsg2));
                    {
                        int dirty_i;
                        for (dirty_i = 0; dirty_i < msglen; dirty_i ++)
                        {
                            DIRTY_CELL(cmsg2 + dirty_i, rmsg2);
                        }
                    }
                }
                hero_dir = dirhero;
                sprite_register[HERO] = SPRITE_HERO + ((hero_dir == LEFT) ? 4 : (hero_dir == RIGHT) ? 12 : (hero_dir == DOWN) ? 16 : 0);
#if USE_COLOR
                if (! use_color)
#endif
                    if (use_underline)
                    {
                        my_clear();
                        clearok(curscr, TRUE);
                        DIRTY_ALL();
                        ignore_delay = 1;
                        frameskip = 0;
                        visible_frame = 1;
                    }
            }
        }
        else
        {
            if ((BONUSHERO && (score >= BONUSHERO) && ! earned) ||
                ((score >= BONUSHERO2) && (earned == (BONUSHERO ? 1 : 0)))) {
                if (! myman_demo)
                {
                    earned ++;
                }
            }
            if (ISPELLET((c = (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile]))
                || ISDOT(c)) {
                maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile] = ' ';
                sprite_register_frame[HERO] = 0;
                if (! myman_demo) score += 10 + 40 * ISPELLET(c);
                if (ISPELLET(c)) {
                    sfx |= sfx_pellet;
                    points = 200;
                    sprite_register_frame[GHOST_SCORE] = 0;
                    pellet_timer = pellet_time + 1;
                    for (s = 0; s < ghosts; s++) {
                        int eyes, mean, blue;

                        eyes = GHOSTEYES(s);
                        blue = BLUEGHOST(s);
                        mean = MEANGHOST(s);
                        sprite_register_frame[blue] = 0;
                        if (sprite_register_used[mean] || sprite_register_used[blue]) {
                            if (sprite_register_used[mean]) {
                                sprite_register_x[blue] = sprite_register_x[eyes];
                                sprite_register_y[blue] = sprite_register_y[eyes];
                            }
                            sprite_register_used[eyes] = 0;
                            sprite_register_used[mean] = 0;
                            sprite_register_used[blue] = 1;
                            sprite_register[blue] = SPRITE_BLUE;
                            ghost_dir[s] = DIRWRAP(ghost_dir[s] + 2);
                        }
                    }
                }
                else
                {
                    sfx |= sfx_dot;
                }
#ifndef DOTS_FRUIT1
#define DOTS_FRUIT1 (70 * total_dots[maze_level] / 244)
#endif
#ifndef DOTS_FRUIT2
#define DOTS_FRUIT2 (170 * total_dots[maze_level] / 244)
#endif
                if ((++ dots == DOTS_FRUIT1) || (dots == DOTS_FRUIT2))
                {
                    sprite_register_x[FRUIT] =
                        sprite_register_x[FRUIT_SCORE] = (int) XFRUIT;
                    sprite_register_y[FRUIT] =
                        sprite_register_y[FRUIT_SCORE] = (int) YFRUIT;
                    sprite_register_used[FRUIT] = 1;
                    sprite_register_timer[FRUIT] = FRUITLIFE;
                    sfx |= sfx_bonus;
                } else if (dots == total_dots[maze_level]) {
                    munched = HERO;
                    winning = 2 * TWOSECS;
                    sfx |= sfx_level;
                }
            } else if (sprite_register_used[FRUIT] &&
                       collide(HERO, FRUIT))
            {
                DIRTY_ALL();
                ignore_delay = 1;
                frameskip = 0;
                sprite_register_used[FRUIT] = 0;
                sprite_register_used[FRUIT_SCORE] = 1;
                sprite_register_timer[FRUIT_SCORE] = TWOSECS;
                if (! myman_demo) score += bonus_score[sprite_register_frame[FRUIT]];
                sprite_register_frame[HERO] = 0;
                sfx |= sfx_fruit;
            }
            if (! ghost_eaten_timer) {
                if (XDIR(hero_dir))
                    sprite_register_y[HERO] = (1 + 2 * ytile) * gfx_h / 2;
                else if (hero_dir)
                    sprite_register_x[HERO] = (1 + 2 * xtile) * gfx_w / 2;
                if (ISZAPLEFT((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile])
                    &&
                    (! YDIR(hero_dir))
                    &&
                    (hero_dir == LEFT)) {
                    int ii;

                    for (ii = 1; ii < maze_w; ii ++)
                    {
                        if (ISZAPRIGHT((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile - ii)]))
                        {
                            break;
                        }
                    }
                    sprite_register_x[HERO] = (1 + 2 * XWRAP(xtile - ii)) * gfx_w / 2;
                }
                else if (ISZAPRIGHT((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile])
                         &&
                         (! YDIR(hero_dir))
                         &&
                         (hero_dir == RIGHT)) {
                    int ii;

                    for (ii = 1; ii < maze_w; ii ++)
                    {
                        if (ISZAPLEFT((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile + ii)]))
                        {
                            break;
                        }
                    }
                    sprite_register_x[HERO] = (1 + 2 * XWRAP(xtile + ii)) * gfx_w / 2;
                }
                else if (ISZAPUP((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile])
                         &&
                         (! XDIR(hero_dir))
                         &&
                         (hero_dir == UP)) {
                    int ii;

                    for (ii = 1; ii < maze_h; ii ++)
                    {
                        if (ISZAPDOWN((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile - ii)) * (maze_w + 1)+xtile]))
                        {
                            break;
                        }
                    }
                    sprite_register_y[HERO] = (1 + 2 * YWRAP(ytile - ii)) * gfx_h / 2;
                }
                else if (ISZAPDOWN((unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile])
                         &&
                         (! XDIR(hero_dir))
                         &&
                         (hero_dir == DOWN)) {
                    int ii;

                    for (ii = 1; ii < maze_h; ii ++)
                    {
                        if (ISZAPUP((unsigned char) maze[(maze_level*maze_h+YWRAP(ytile + ii)) * (maze_w + 1)+xtile]))
                        {
                            break;
                        }
                    }
                    sprite_register_y[HERO] = (1 + 2 * YWRAP(ytile + ii)) * gfx_h / 2;
                }
                else if (ISOPEN((unsigned char)
                                maze[(maze_level*maze_h+YWRAP(ytile + YLEAVING(hero_dir, y_off + YDIR(hero_dir)))) * (maze_w + 1)+XWRAP(xtile + XLEAVING(hero_dir, x_off + XDIR(hero_dir)))])) {
                    sprite_register_x[HERO] = XPIXWRAP(sprite_register_x[HERO] + XDIR(hero_dir) * ((SQUARE || gfx_reflect) ? (cycles & 1) : 1));
                    sprite_register_y[HERO] = YPIXWRAP(sprite_register_y[HERO] + YDIR(hero_dir) * ((gfx_reflect && ! SQUARE) ? 1 : (cycles & 1)));
                    sprite_register_frame[HERO] = (cycles / (1 + (FIFTH / 2))) % 4;
                }
            }
            for (s = 0; s < ghosts; s ++) {
                int eyes, blue, mean;

                eyes = GHOSTEYES(s);
                blue = BLUEGHOST(s);
                mean = MEANGHOST(s);
                if ((! ghost_eaten_timer) || ((munched != eyes) && (munched != HERO)))
                    sprite_register_frame[blue] =
                        (sprite_register_frame[mean] = ((cycles / FIFTH) & 1) ? 1 : 0);
#define IF_COLLISION_HANDLE_COLLISION \
                        if (sprite_register_used[mean] && collide(mean, HERO) && ! ghost_eaten_timer) { \
                            sfx |= sfx_dying; \
                            dying = DEATHDELAY; \
                            sprite_register[HERO] = SPRITE_HERO + 8; \
                            sprite_register_frame[HERO] = 0; \
                            ghost_eaten_timer = ONESEC; \
                            munched = HERO; \
                            s = -1; \
                            continue; \
                        } else if (sprite_register_used[blue] && collide(blue, HERO) && ! ghost_eaten_timer) { \
                            sfx |= sfx_ghost; \
                            if (! myman_demo) score += points; \
                            points *= 2; \
                            if (points > 1600) points = 1600; \
                            ghost_eaten_timer = ONESEC; \
                            sprite_register_used[HERO] = 0; \
                            munched = eyes; \
                            sprite_register_used[GHOST_SCORE] = 1; \
                            sprite_register_x[GHOST_SCORE] = sprite_register_x[blue]; \
                            sprite_register_y[GHOST_SCORE] = sprite_register_y[blue]; \
                            sprite_register_x[eyes] = sprite_register_x[blue]; \
                            sprite_register_y[eyes] = sprite_register_y[blue]; \
                            sprite_register_used[blue] = 0; \
                        }
                if (dying || winning) {
                    sprite_register_used[eyes] =
                        (sprite_register_used[blue] =
                         (sprite_register_used[mean] = 0));
                } else IF_COLLISION_HANDLE_COLLISION
                           else
                           {
                               int dir0, dir1, dir2, o0, o1, o2, x, y, i1, j1, i2, j2;

                               dir1 = DIRWRAP(ghost_dir[s]);
                               dir0 = DIRWRAP(dir1 + (s != GHOST2) - (s == GHOST2));
                               dir2 = DIRWRAP(dir1 - (s != GHOST2) + (s == GHOST2));
                               if (sprite_register_used[blue] && ! ghost_eaten_timer)
                               {
                                   /* runnin' scared */
                                   if (cycles & 1) {
                                       x = sprite_register_x[blue];
                                       y = sprite_register_y[blue];
                                       i1 = XTILE(x);
                                       j1 = YTILE(y);
                                       i2 = XTILE(sprite_register_x[HERO]);
                                       j2 = YTILE(sprite_register_y[HERO]);
                                       if ((i1 == i2) && (j1 <= j2)) {
                                           for (j = j1; j <= j2; j ++)
                                               if (! ISOPEN((unsigned char) maze[(maze_level*maze_h+j) * (maze_w + 1)+i1]))
                                                   break;
                                           if (j > j2) {
                                               ghost_mem[s] = hero_dir;
                                               ghost_timer[s] = (int) MEMDELAY(s);
                                           }
                                       } else if ((i1 == i2) && (j1 >= j2)) {
                                           for (j = j2; j <= j1; j ++)
                                               if (! ISOPEN((unsigned char) maze[(maze_level*maze_h+j) * (maze_w + 1)+i1]))
                                                   break;
                                           if (j > j1) {
                                               ghost_mem[s] = hero_dir;
                                               ghost_timer[s] = (int) MEMDELAY(s);
                                           }
                                       } else if ((j1 == j2) && (i1 <= i2)) {
                                           for (i = i1; i <= i2; i ++)
                                               if (! ISOPEN((unsigned char) maze[(maze_level*maze_h+j1) * (maze_w + 1)+i]))
                                                   break;
                                           if (i > i2) {
                                               ghost_mem[s] = hero_dir;
                                               ghost_timer[s] = (int) MEMDELAY(s);
                                           }
                                       } else if ((j1 == j2) && (i1 >= i2)) {
                                           for (i = i2; i <= i1; i ++)
                                               if (! ISOPEN((unsigned char) maze[(maze_level*maze_h+j1) * (maze_w + 1)+i]))
                                                   break;
                                           if (i > i1) {
                                               ghost_mem[s] = hero_dir;
                                               ghost_timer[s] = (int) MEMDELAY(s);
                                           }
                                       }
                                       o0 = ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir0))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir0))]);
                                       o1 = ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir1))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir1))]);
                                       o2 = ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir2))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir2))]);
                                       if (((gfx_w / 2 == x % gfx_w) && XDIR(dir1))
                                           || ((gfx_h / 2 == y % gfx_h) && YDIR(dir1))) {
                                           if (o0 || o2) {
                                               if (! ghost_mem[s]) {
                                                   if (o2)
                                                       dir1 = dir2;
                                                   else if (! o1)
                                                       dir1 = dir0;
                                               }
                                               else
                                               {
                                                   if (o0 && (dir0 != ghost_mem[s]))
                                                       dir1 = dir0;
                                                   else if (o2 && (dir2 != ghost_mem[s]))
                                                       dir1 = dir2;
                                                   else if (o1 && (dir1 != ghost_mem[s]))
                                                       ;
                                                   else if (o0)
                                                       dir1 = dir0;
                                                   else
                                                       dir1 = dir2;
                                               }
                                           } else if (! o1)
                                               dir1 = DIRWRAP(dir1 + 2);
                                       }
                                       if (XDIR(dir1))
                                           y = (1 + 2 * j1) * gfx_h / 2;
                                       else if (dir1)
                                           x = (1 + 2 * i1) * gfx_w / 2;
                                       if ((SQUARE || gfx_reflect) ? (cycles & 2) : 1)
                                           sprite_register_x[blue] = XPIXWRAP(x + XDIR(dir1));
                                       if ((gfx_reflect && ! SQUARE) ? 1 : (cycles & 2))
                                           sprite_register_y[blue] = YPIXWRAP(y + YDIR(dir1));
                                       if (! home_dir[(s % ghosts*maze_h+YTILE(sprite_register_y[blue]))*(maze_w+1)+XTILE(sprite_register_x[blue])])
                                           home_dir[(s % ghosts*maze_h+YTILE(sprite_register_y[blue]))*(maze_w+1)+XTILE(sprite_register_x[blue])] =
                                               DIRWRAP(dir1 + 2);
                                   }
                               }
                               else if (sprite_register_used[mean] && ! ghost_eaten_timer)
                               {
                                   /* out huntin' */
                                   int d0, d1, d2;

                                   x = sprite_register_x[mean];
                                   y = sprite_register_y[mean];
                                   i1 = XTILE(x);
                                   j1 = YTILE(y);
                                   i2 = XTILE(sprite_register_x[HERO]);
                                   j2 = YTILE(sprite_register_y[HERO]);
                                   if ((i1 == i2) && (j1 <= j2)) {
                                       for (j = j1; j <= j2; j ++)
                                           if (! ISOPEN((unsigned char) maze[(maze_level*maze_h+j) * (maze_w + 1)+i1]))
                                               break;
                                       if (j > j2) {
                                           ghost_mem[s] = hero_dir;
                                           ghost_timer[s] = (int) MEMDELAY(s);
                                       }
                                   } else if ((i1 == i2) && (j1 >= j2)) {
                                       for (j = j2; j <= j1; j ++)
                                           if (! ISOPEN((unsigned char) maze[(maze_level*maze_h+j) * (maze_w + 1)+i1]))
                                               break;
                                       if (j > j1) {
                                           ghost_mem[s] = hero_dir;
                                           ghost_timer[s] = (int) MEMDELAY(s);
                                       }
                                   } else if ((j1 == j2) && (i1 <= i2)) {
                                       for (i = i1; i <= i2; i ++)
                                           if (! ISOPEN((unsigned char) maze[(maze_level*maze_h+j1) * (maze_w + 1)+i]))
                                               break;
                                       if (i > i2) {
                                           ghost_mem[s] = hero_dir;
                                           ghost_timer[s] = (int) MEMDELAY(s);
                                       }
                                   } else if ((j1 == j2) && (i1 >= i2)) {
                                       for (i = i2; i <= i1; i ++)
                                           if (! ISOPEN((unsigned char) maze[(maze_level*maze_h+j1) * (maze_w + 1)+i]))
                                               break;
                                       if (i > i1) {
                                           ghost_mem[s] = hero_dir;
                                           ghost_timer[s] = (int) MEMDELAY(s);
                                       }
                                   }
                                   o0 = ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir0))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir0))]);
                                   o1 = ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir1))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir1))]);
                                   o2 = ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir2))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir2))]);
                                   d0 = d2 = 0;
                                   d1 = ISDOOR((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir1))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir1))]);
                                   if (! ISDOOR((unsigned char) maze[(maze_level*maze_h+j1) * (maze_w + 1)+i1]))
                                   {
                                       d0 = ISDOOR((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir0))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir0))]);
                                       d2 = ISDOOR((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir2))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir2))]);
                                   }
                                   d0 = d0 && (dir0 !=
                                               HOME_DIR(s,
                                                        YWRAP(j1 + YDIR(dir0)),
                                                        XWRAP(i1 + XDIR(dir0))))
                                       && dir0 != DOWN
                                       && dir0 != LEFT;
                                   d2 = d2 && (dir2 !=
                                               HOME_DIR(s,
                                                        YWRAP(j1 + YDIR(dir2)),
                                                        XWRAP(i1 + XDIR(dir2))))
                                       && dir2 != DOWN
                                       && dir2 != LEFT;
                                   d1 = d1 && (dir1 !=
                                               HOME_DIR(s,
                                                        YWRAP(j1 + YDIR(dir1)),
                                                        XWRAP(i1 + XDIR(dir1))))
                                       && dir1 != DOWN
                                       && dir1 != LEFT;
                                   if (((gfx_w / 2 == x % gfx_w) && XDIR(dir1))
                                       || ((gfx_h / 2 == y % gfx_h) && YDIR(dir1))) {
                                       if (d2 || (o2 && (dir2 == ghost_mem[s])))
                                           dir1 = dir2;
                                       else if (d1 || (o1 && (dir1 == ghost_mem[s])))
                                           ;
                                       else if (d0 || (o0 && (dir0 == ghost_mem[s])))
                                           dir1 = dir0;
                                       else if (o1)
                                           ;
                                       else if (o2)
                                           dir1 = dir2;
                                       else if (o0)
                                           dir1 = dir0;
                                       else
                                           dir1 = DIRWRAP(dir1 + 2);
                                   }
                                   if (XDIR(dir1))
                                       y = (1 + 2 * j1) * gfx_h / 2;
                                   else if (dir1)
                                       x = (1 + 2 * i1) * gfx_w / 2;
                                   if ((SQUARE || gfx_reflect) ? (cycles & 1) : 1)
                                       sprite_register_x[mean] = (sprite_register_x[eyes] = XPIXWRAP(x + XDIR(dir1)));
                                   if ((gfx_reflect && ! SQUARE) ? 1 : (cycles & 1))
                                       sprite_register_y[mean] = (sprite_register_y[eyes] = YPIXWRAP(y + YDIR(dir1)));
                                   if (! home_dir[(s % ghosts*maze_h+YTILE(sprite_register_y[mean]))*(maze_w+1)+XTILE(sprite_register_x[mean])])
                                       home_dir[(s % ghosts*maze_h+YTILE(sprite_register_y[mean]))*(maze_w+1)+XTILE(sprite_register_x[mean])] =
                                           DIRWRAP(dir1 + 2);
                               } else if (sprite_register_used[eyes]
                                          &&
                                          ((munched != eyes) || (! ghost_eaten_timer))
                                          &&
                                          ((! ghost_eaten_timer) || ! sprite_register_used[MEANGHOST(s)])) {
                                   int dx, dy;
                                   unsigned char d, d1;

                                   /* goin' home */
                                   ghost_timer[s] = (int) MEMDELAY(s);
                                   x = sprite_register_x[eyes];
                                   y = sprite_register_y[eyes];
                                   i1 = XTILE(x);
                                   j1 = YTILE(y);
                                   dx = (int) ((XGHOST - x) / gfx_w);
                                   dy = (int) (((dx ? YTOP : YGHOST) - y) / gfx_h);
                                   if ((d = HOME_DIR(s, j1, i1)) &&
                                       (! ((d1 = DIRWRAP(HOME_DIR(s, YWRAP(j1 + YDIR(d)), XWRAP(i1 + XDIR(d))) + 2)) &&
                                           (d1 == d))))
                                       ghost_mem[s] = d;
                                   else {
                                       if (dx * dx > dy * dy) {
                                           if (dx > 0)
                                               ghost_mem[s] = RIGHT;
                                           else
                                               ghost_mem[s] = LEFT;
                                       }
                                       else
                                       {
                                           if (dy < 0)
                                               ghost_mem[s] = UP;
                                           else
                                               ghost_mem[s] = DOWN;
                                       }
                                   }
                                   if (! (dx || dy)) {
                                       sprite_register_used[eyes] = NET_LIVES ? VISIBLE_EYES : 0;
                                       sprite_register_used[mean] = 1;
                                       sprite_register_x[mean] = x;
                                       sprite_register_y[mean] = y;
                                       sprite_register_frame[eyes] = (ghost_dir[s] = DIRWRAP(s + 1)) - 1;
                                       ghost_mem[s] = 0;
                                       continue;
                                   }
                                   o0 = ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir0))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir0))])
                                       || ISDOOR((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir0))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir0))]);
                                   o1 = ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir1))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir1))])
                                       || ISDOOR((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir1))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir1))]);
                                   o2 = ISOPEN((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir2))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir2))])
                                       || ISDOOR((unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir2))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir2))]);
                                   if (o2 && (dir2 == ghost_mem[s]))
                                       dir1 = dir2;
                                   else if (o1 && (dir1 == ghost_mem[s]))
                                       ;
                                   else if (o0 && (dir0 == ghost_mem[s]))
                                       dir1 = dir0;
                                   else if (o1)
                                       ;
                                   else if (o2)
                                       dir1 = dir2;
                                   else if (o0)
                                       dir1 = dir0;
                                   else
                                       dir1 = DIRWRAP(dir1 + 2);
                                   sprite_register_x[mean] = (sprite_register_x[eyes] = XPIXWRAP(x + XDIR(dir1)));
                                   sprite_register_x[eyes] = XPIXWRAP(x + XDIR(dir1));
                                   sprite_register_y[eyes] = YPIXWRAP(y + YDIR(dir1));
                                   if (! home_dir[(s % ghosts*maze_h+YTILE(sprite_register_y[eyes]))*(maze_w+1)+XTILE(sprite_register_x[eyes])])
                                       home_dir[(s % ghosts*maze_h+YTILE(sprite_register_y[eyes]))*(maze_w+1)+XTILE(sprite_register_x[eyes])] =
                                           DIRWRAP(dir1 + 2);
                               }
                               if ((! ghost_eaten_timer) && ghost_timer[s] && ! -- ghost_timer[s]) {
                                   ghost_timer[s] = (int) MEMDELAY(s);
                                   if (ghost_mem[s]) {
                                       ghost_mem[s] = 0;
                                   }
                                   else
                                       dir1 = DIRWRAP(ghost_dir[s] + 2);
                               }
                               ghost_dir[s] = (sprite_register_frame[eyes] = dir1 - 1) + 1;
                               IF_COLLISION_HANDLE_COLLISION
                                   }
            }
            if (! ghost_eaten_timer)
                for (s = 0; s < SPRITE_REGISTERS; s ++) {
                    if (sprite_register_used[s] && sprite_register_timer[s])
                        if (! -- sprite_register_timer[s])
                        {
                            sprite_register_used[s] = 0;
                            if (s == FRUIT)
                            {
                                DIRTY_ALL();
                                ignore_delay = 1;
                                frameskip = 0;
                            }
                        }
                }
        }
    }
    if (visible_frame && ! (xoff_received || myman_demo_setup))
    {
        {
            int s;
            for (s = 0; s < SPRITE_REGISTERS; s ++)
            {
                if (sprite_register_used[s])
                {
                    DIRTY_SPRITE_REGISTER(s);
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
        attrset(0);
        for (vline = -(3 * tile_h); (vline < LINES) && (vline < (sprite_h + ((reflect ? (gfx_w * maze_w) : (gfx_h * maze_h))))); vline++)
        {
            if ((vline + (reflect ? c_off : r_off)) < 0) continue;
            if ((vline + (reflect ? c_off : r_off)) >= LINES) continue;
            if ((vline < 0) || (vline >= (reflect ? (gfx_w * maze_w) : (gfx_h * maze_h))))
            {
                for (vcol = 0; (vcol < MY_COLS) && (vcol < (reflect ? (gfx_h * maze_h) : (gfx_w * maze_w))); vcol++)
                {
                    int filler_tile = 0;
                    int a = 0;

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
                                            player_tile = "UP "[2 - player_col];
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
                                    if (player_tile
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
                                    if (player_tile && tile_used[player_tile])
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
                                    if (tile_used[score_tile] && (score_tile != ' '))
                                    {
                                        filler_tile = score_tile;
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
                        if (tile_used[score_tile])
                        {
                            int a;

                            a = 0;
#if USE_COLOR
                            if (use_color) {
                                a = tile_color[score_tile];
                                if (! a) a = TEXT_COLOR;
                                a = pen[a];
                            }
#endif
                            my_move(vline + (reflect ? c_off : r_off), col * (use_fullwidth ? 2 : 1));
                            my_addch((unsigned long) (unsigned char) tile[(score_tile * tile_h + vline) * tile_w + tile_w - 1 - (score_x % tile_w)], a);

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
                        int a;

                        c = (unsigned long) (unsigned char) sprite[(life_sprite * sprite_h + hud_line) * sprite_w + (col % sprite_w)];
                        if (c)
                        {
                            a = 0;
#if USE_COLOR
                            if (use_color) {
                                if (! (a = sprite_color[life_sprite]))
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
                int s;
                int a = 0;
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

                    pause_shown = 1;
                    pause_x = vcol + (reflect ? r_off : c_off) - ((MY_COLS - tile_w * (int) strlen(PAUSE) + 1) / 2);
                    pause_y = vline + (reflect ? c_off : r_off) - ((LINES - tile_h + 1) / 2);
                    pause_tile = (unsigned long) (unsigned char) PAUSE[pause_x / tile_w];
                    if (tile_used[pause_tile])
                    {
                        c = (unsigned long) (unsigned char) tile[(PAUSE[pause_x / tile_w] * tile_h + pause_y) * tile_w + (pause_x % tile_w)];
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
                    ISPELLET((unsigned char) (char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile])
                    ||
                    winning)
                {
                    if (! c)
                    {
                        for (s = 0; s < SPRITE_REGISTERS; s ++) {
                            int t, x, y, iseyes;

                            t = sprite_register[s] + ((sprite_register_frame[s] < 0) ? (-sprite_register_frame[s]) : sprite_register_frame[s]);
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
                                    c = (".^<v>")[ghost_mem[UNGHOSTEYES(s)]];
                                }
                                else
                                {
                                    c = '.';
                                }
#if USE_COLOR
                                if (use_color) {
                                    if (! (a = sprite_color[t]))
                                        a = sprite_register_color[s];
                                    a = pen[a];
                                }
                                else
#endif
                                {
#if USE_ATTR
#ifdef MY_A_BOLD
                                    if ((s == HERO) || (sprite_register[s] == SPRITE_WHITE) || iseyes)
                                    {
                                        a |= use_dim_and_bright ? MY_A_BOLD : 0;
                                        break;
                                    }
#endif
#ifdef MY_A_UNDERLINE
                                    if (sprite_register[s] == SPRITE_BLUE)
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
                                       && (c = sgfx(t, y_off, x_off))) {
#if USE_COLOR
                                if (use_color) {
                                    if (! (a = sprite_color[t]))
                                        a = sprite_register_color[s];
                                    a = pen[a];
                                }
                                else
#endif
                                {
#if USE_ATTR
#ifdef MY_A_BOLD
                                    if ((s == HERO) || (sprite_register[s] == SPRITE_WHITE) || iseyes)
                                    {
                                        a |= use_dim_and_bright ? MY_A_BOLD : 0;
                                        break;
                                    }
#endif
#ifdef MY_A_UNDERLINE
                                    if (sprite_register[s] == SPRITE_BLUE)
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
                                       && cp437_sprite[t]
                                       && tile_used[cp437_sprite[t]]
                                       && ((x = sprite_register_x[s] - gfx_w / 2) <= i)
                                       && ((x_off = i - x) < gfx_w)
                                       && ((y = sprite_register_y[s] - gfx_h / 2) <= j)
                                       && ((y_off = j - y) < gfx_h)
                                       && (c = gfx((unsigned long) (unsigned char) cp437_sprite[t], y_off, x_off)))
                            {
#if USE_COLOR
                                if (use_color)
                                {
                                    if (! (a = tile_color[t]))
                                        a = sprite_register_color[s];
                                    a = pen[a];
                                }
                                else
#endif
                                {
#if USE_ATTR
#ifdef MY_A_BOLD
                                    if ((s == HERO) || (sprite_register[s] == SPRITE_WHITE) || iseyes)
                                    {
                                        a |= use_dim_and_bright ? MY_A_BOLD : 0;
                                        break;
                                    }
#endif
#ifdef MY_A_UNDERLINE
                                    if (sprite_register[s] == SPRITE_BLUE)
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
                                   (fallback_cp437[c_mapped] != c)
                                   &&
                                   (fallback_cp437[c_mapped] != c_mapped))
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
                                    c = (d == UP) ? '^'
                                        : (d == DOWN) ? 'v'
                                        : (d == LEFT) ? '<'
                                        : (d == RIGHT) ? '>'
                                        : ISDOT(c) ? ','
                                        : ISPELLET(c) ? ';'
                                        : ISOPEN(c) ? ' '
                                        : ISDOOR(c) ? 'X'
                                        : '@';
#if USE_COLOR
                                    if (use_color && d) {
                                        a = sprite_color[sprite_register[MEANGHOST(s)] + sprite_register_frame[MEANGHOST(s)]];
                                        if (! a)
                                            a = sprite_register_color[MEANGHOST(s)];
                                        a = pen[a];
                                    }
#endif
                                }
                                else
                                {
                                    if ((ISPELLET(c) && ((cycles / FIFTH) & 4) && (! dead) &&
                                         ! (sprite_register_used[HERO] && ghost_eaten_timer)) ||
                                        ((winning < ONESEC) && winning && (! myman_intro) && (! intermission_running) && (! myman_start) &&
                                         (ISDOT(c) || ISPELLET(c) || (((winning / FIFTH) & 4) && ISDOOR(c)))))
                                    {
                                        is_wall = 0;
                                        c_mapped = ' ';
                                    }
                                    else if ((winning < (2 * TWOSECS)) && ((winning / FIFTH) & 4) && ! ghost_eaten_timer)
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
                                                    a = pen[(((unsigned long) (unsigned char) EFFECTIVE_MORTAR_COLOR) | (((unsigned long) (unsigned char) TRANSLATED_WALL_COLOR) * 16)) % 256];
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
                        int a;

                        c = (unsigned long) (unsigned char) sprite[(level_sprite * sprite_h + hud_line) * sprite_w + (level_x % sprite_w)];
                        if (c)
                        {
                            a = 0;
#if USE_COLOR
                            if (use_color) {
                                if (! (a = sprite_color[level_sprite]))
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
                            int a;

                            c = (unsigned long) (unsigned char) sprite[(life_sprite * sprite_h + line) * sprite_w + ((col - (reflect ? r_off : c_off) - 2 * tile_w) % sprite_w)];
                            if (c)
                            {
                                a = 0;
#if USE_COLOR
                                if (use_color) {
                                    if (! (a = sprite_color[life_sprite]))
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
                            int a;

                            c = (unsigned long) (unsigned char) sprite[(level_sprite * sprite_h + line) * sprite_w + (level_x % sprite_w)];
                            if (c)
                            {
                                a = 0;
#if USE_COLOR
                                if (use_color) {
                                    if (! (a = sprite_color[level_sprite]))
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
            my_addstr(msg[(cycles / FIFTH) & 7], 0);
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
                    DIRTY_SPRITE_REGISTER(s);
                }
            }
        }
    }
    if (! (paused || snapshot || snapshot_txt))
    {
        if (pellet_timer && (! ghost_eaten_timer)) {
            int s, eyes, blue, mean;
                
            if (! -- pellet_timer) {
                for (s = 0; s < ghosts; s ++)
                    if (sprite_register_used[(blue = BLUEGHOST(s))]) {
                        sprite_register_used[(mean = MEANGHOST(s))] = 1;
                        sprite_register_used[blue] = 0;
                        sprite_register_used[(eyes = GHOSTEYES(s))] = VISIBLE_EYES;
                        sprite_register_x[eyes] =
                            (sprite_register_x[mean] = sprite_register_x[blue]);
                        sprite_register_y[eyes] =
                            (sprite_register_y[mean] = sprite_register_y[blue]);
                        sprite_register_frame[eyes] = (ghost_dir[s] = DIRWRAP(s + 1)) - 1;
                    }
            } else if (pellet_timer <= TWOSECS)
                for (s = 0; s < ghosts; s ++)
                    sprite_register[BLUEGHOST(s)] = ((2 * pellet_timer / ONESEC) & 1)
                        ? SPRITE_BLUE
                        : SPRITE_WHITE;
        }
        cycles ++;
    }
    return 1;
}

int
main(int argc, char *argv[]
#ifndef MAIN_NO_ENVP
     , char *envp[]
#endif
    )
{
    int n, i, j;
    long c = 0;
    int nogame = 0;
    int
        dump_maze = 0,
        dump_sprite = 0,
        dump_tile = 0;
    const char *tilefile = TILEFILE;
    const char *spritefile = SPRITEFILE;
    const char *mazefile = MAZEFILE;
    unsigned long uli;
    int option_index;
    const char *defvariant = MYMANVARIANT;
    const char *defsize = MYMANSIZE;
    static struct option long_options[] =
        {
            {"version", 0, 0, 'V'},
            {"help", 0, 0, 'h'},
            {"keys", 0, 0, 'k'},
            {"legal", 0, 0, 'L'},
            {"variant", 1, 0, 'v'},
            {"size", 1, 0, 'z'},
            {0, 0, 0, 0}
        };

    progname = (argc > 0) ? argv[0] : "";
#if defined(MACCURSES)
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
#endif
    progname = (progname && *progname) ? progname : MYMAN;
    if (getenv("MYMAN_DEBUG") && *(getenv("MYMAN_DEBUG")) && strcmp(getenv("MYMAN_DEBUG"), "0"))
    {
        debug = atoi(getenv("MYMAN_DEBUG"));
        debug = debug ? debug : 1;
    }
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    for (i = 0; i < SPRITE_REGISTERS; i ++) {
        sprite_register_used[i] = 0;
        sprite_register_frame[i] = 0;
#if USE_COLOR
        sprite_register_color[i] = 0x7;
#endif
    }
    for (i = 0; i < 256; i ++) {
#ifndef BUILTIN_TILE
        tile_color[i] = 0x7;
#endif
#ifndef BUILTIN_SPRITE
        sprite_color[i] = 0x7;
#endif
    }
    while ((i = getopt_long(argc, argv, "Vv:z:bcd:g:l:Lhkm:noprqs:t:uUMRSTf:F:aAeEBNiI12xX",
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
            delay = uli;
            mindelay = delay / 2;
            break;
        }
        case 'g':
        {
            char garbage;

            if ((sscanf(optarg, "%lu%c", &uli, &garbage) != 1)
                ||
                (uli > (unsigned long) MAXGHOSTS))
            {
                fprintf(stderr,
                        "%s: argument to -g must be an unsigned integer no greater than %lu.\n",
                        progname,
                        (unsigned long) MAXGHOSTS);
                fflush(stderr), exit(1);
            }
            ghosts = (int) uli;
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
            printf("Usage: %s [options]"
                   XCURSES_USAGE
                   "\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s"
                   "\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
                   progname,
                   "-h \tdisplay this help and exit",
                   "-b \tenable sounds",
                   "-q \tdisable sounds",
#if USE_COLOR
                   "-c \tenable color support",
#else
                   "-c \tenable color support (must recompile first)",
#endif
                   "-n \tdisable color support",
                   "-B \tuse dim and bright attributes for missing colors",
                   "-N \tdon't use dim and bold attributes for extra colors",
                   "-o \tuse 'o' for power pellets and bullet for dots",
                   "-p \tuse bullet for power pellets and '.' for dots",
#if MYMANDELAY
                   "-d NUM \tdelay NUM microseconds/refresh",
#else
                   "-d NUM \tdelay NUM microseconds/refresh (must recompile first)",
#endif
                   "-g NUM \tplay against NUM monsters",
                   "-l NUM \tstart with NUM lives",
#if USE_ATTR
                   "-u \tuse the underline attribute for maze walls",
#else
                   "-u \tuse the underline attribute for maze walls (must recompile first)",
#endif
                   "-U \tdon't use the underline attribute for maze walls",
                   "-r \tuse raw tile characters (CP437 or UCS/Unicode character graphics)",
                   "-R \tuse altcharset translations (VT100-style graphics)",
                   "-e \tuse UCS/Unicode for internal representation of raw tile characters",
                   "-E \tuse CP437 for internal representation of raw tile characters",
                   "-a \tuse ASCII for altcharset translation",
                   "-A \tuse your terminal's altcharset translations",
                   "-i \tscroll vertically by redrawing the screen",
                   "-I \tuse your terminal's vertical scrolling capabilities",
                   "-1 \tuse default UCS/Unicode mapping",
                   "-2 \tuse CJK (fullwidth) UCS/Unicode mapping",
                   "-m FILE \tuse the maze in FILE",
                   "-s FILE \tuse sprites from FILE",
                   "-t FILE \tuse tiles from FILE",
                   "-M \twrite the maze to stdout in C format and exit",
                   "-S \twrite sprites to stdout in C format and exit",
                   "-T \twrite tiles to stdout in C format and exit",
                   "-f FILE \tredirect stdout to FILE (append)",
                   "-F FILE \tredirect stdout to FILE (truncate)",
                   "-x \treflect maze diagonally, exchanging the upper right and lower left corners",
                   "-X \tdo not reflect maze");
            printf("Defaults:%s%s%s%s%s%s%s%s%s%s%s -d %lu -l %d -g %d -m \"",
                   use_raw ? " -r" : " -R",
                   use_raw_ucs ? " -e" : " -E",
                   (use_acs_p ? (use_acs ? " -A" : " -a") : ""),
                   use_idlok ? " -I" : " -i",
                   use_sound ? " -b" : " -q",
                   use_color ? " -c" : " -n",
                   (use_dim_and_bright_p ? (use_dim_and_bright ? " -B" : " -N") : ""),
                   (use_bullet_for_dots_p ? (use_bullet_for_dots ? " -o" : " -p") : ""),
                   use_underline ? " -u" : " -U",
                   use_fullwidth ? " -2" : " -1",
                   reflect ? " -x" : " -X",
                   delay ? delay : 0,
                   lives,
                   ghosts);
            if (mazefile)
                escape(mazefile, strlen(mazefile));
            else {
                printf("(");
                escape(builtin_mazefile, strlen(builtin_mazefile));
                printf(")");
            }
            printf("\" -s \"");
            if (spritefile)
                escape(spritefile, strlen(spritefile));
            else {
                printf("(");
                escape(builtin_spritefile, strlen(builtin_spritefile));
                printf(")");
            }
            printf("\" -t \"");
            if (tilefile)
                escape(tilefile, strlen(tilefile));
            else {
                printf("(");
                escape(builtin_tilefile, strlen(builtin_tilefile));
                printf(")");
            }
            printf("\"\n");
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
        return 1;

    if (tile_args)
    {
        const char *argp = tile_args;

        while ((c = *argp))
        {
            const char *endp;

            if (isspace(c))
            {
                argp ++;
                continue;
            }
            else if ((endp = strchr(argp, '=')))
            {
                if (! strncmp(argp, "ABOUT", endp - argp))
                {
                    argp = endp + 1;
                    tile_ABOUT = strword(argp, &endp, 0);
                    if (! tile_ABOUT)
                    {
                        perror("ABOUT");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "NOTE", endp - argp))
                {
                    argp = endp + 1;
                    tile_NOTE = strword(argp, &endp, 0);
                    if (! tile_NOTE)
                    {
                        perror("NOTE");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "FIXME", endp - argp))
                {
                    argp = endp + 1;
                    tile_FIXME = strword(argp, &endp, 0);
                    if (! tile_FIXME)
                    {
                        perror("FIXME");
                        return 1;
                    }
                    argp = endp;
                }
                else
                {
                    fprintf(stderr, "%s: unrecognized tile argument: ",
                            tilefile ? tilefile : builtin_tilefile);
                    fflush(stderr);
                    fwrite((void *) argp, 1, endp - argp, stderr);
                    fflush(stderr);
                    fprintf(stderr, "\n");
                    fflush(stderr);
                    return 1;
                }
            }
            else
            {
                fprintf(stderr, "%s: unrecognized tile arguments: %s\n",
                        tilefile ? tilefile : builtin_tilefile,
                        argp);
                fflush(stderr);
                return 1;
            }
        }
    }

    if (sprite_args)
    {
        const char *argp = sprite_args;

        while ((c = *argp))
        {
            const char *endp;

            if (isspace(c))
            {
                argp ++;
                continue;
            }
            else if ((endp = strchr(argp, '=')))
            {
                if (! strncmp(argp, "ABOUT", endp - argp))
                {
                    argp = endp + 1;
                    sprite_ABOUT = strword(argp, &endp, 0);
                    if (! sprite_ABOUT)
                    {
                        perror("ABOUT");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "NOTE", endp - argp))
                {
                    argp = endp + 1;
                    sprite_NOTE = strword(argp, &endp, 0);
                    if (! sprite_NOTE)
                    {
                        perror("NOTE");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "FIXME", endp - argp))
                {
                    argp = endp + 1;
                    sprite_FIXME = strword(argp, &endp, 0);
                    if (! sprite_FIXME)
                    {
                        perror("FIXME");
                        return 1;
                    }
                    argp = endp;
                }
                else
                {
                    fprintf(stderr, "%s: unrecognized sprite argument: ",
                            spritefile ? spritefile : builtin_spritefile);
                    fflush(stderr);
                    fwrite((void *) argp, 1, endp - argp, stderr);
                    fflush(stderr);
                    fprintf(stderr, "\n");
                    fflush(stderr);
                    return 1;
                }
            }
            else
            {
                fprintf(stderr, "%s: unrecognized sprite arguments: %s\n",
                        spritefile ? spritefile : builtin_spritefile,
                        argp);
                fflush(stderr);
                return 1;
            }
        }
    }

    gfx_reflect = reflect && ! REFLECT_LARGE;

#if ! MYMANDELAY
    if (delay)
    {
        fprintf(stderr,
                "%s: compile with -DMYMANDELAY=1 to enable the -d option.\n",
                progname);
        fflush(stderr), exit(1);
    }
#else
    delay = delay / FIFTH;
    mindelay = delay / 2;
#endif

#ifdef BUILTIN_MAZE
    if (! mazefile)
    {
        maze = (char *) malloc(maze_n * maze_h * (maze_w + 1) * sizeof(*maze));
        if (! maze)
        {
            perror("malloc");
            return 1;
        }
        memcpy((void *) maze,
               (void *) maze_data,
               maze_n * maze_h * (maze_w + 1));
        maze_color = (char *) malloc(maze_n * maze_h * (maze_w + 1) * sizeof(*maze_color));
        if (! maze_color)
        {
            perror("malloc");
            return 1;
        }
        memcpy((void *) maze_color,
               (void *) maze_color_data,
               maze_n * maze_h * (maze_w + 1));
    }
#endif /* defined(BUILTIN_MAZE) */
    if (mazefile && readmaze(mazefile, &maze_n, &maze_w, &maze_h, &maze, &maze_flags, &maze_color, &maze_args))
    {
        return 1;
    }
    if (maze_args)
    {
        const char *argp = maze_args;

        while ((c = *argp))
        {
            const char *endp;

            if (isspace(c))
            {
                argp ++;
                continue;
            }
            else if ((endp = strchr(argp, '=')))
            {
                if (! strncmp(argp, "FLIP_TO", endp - argp))
                {
                    argp = endp + 1;
                    flip_to = strtol(argp, (char **) &endp, 0);
                    if (endp == argp)
                    {
                        perror("strtol: FLIP_TO");
                        return 1;
                    }
                    if ((*endp) && ! isspace(*endp))
                    {
                        fprintf(stderr, "%s: FLIP_TO: garbage after argument: %s\n",
                                mazefile ? mazefile : builtin_mazefile,
                                endp);
                        fflush(stderr);
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "GHOSTS", endp - argp))
                {
                    int tmp_ghosts;

                    argp = endp + 1;
                    tmp_ghosts = strtol(argp, (char **) &endp, 0);
                    if (endp == argp)
                    {
                        perror("strtol: GHOSTS");
                        return 1;
                    }
                    if ((*endp) && ! isspace(*endp))
                    {
                        fprintf(stderr, "%s: GHOSTS: garbage after argument: %s\n",
                                mazefile ? mazefile : builtin_mazefile,
                                endp);
                        fflush(stderr);
                        return 1;
                    }
                    argp = endp;
                    if (! ghosts_p) ghosts = tmp_ghosts;
                }
                else if (! strncmp(argp, "RGHOST", endp - argp))
                {
                    argp = endp + 1;
                    maze_RGHOST = strtodlist(argp, &endp, &maze_RGHOST_len);
                    if (! maze_RGHOST)
                    {
                        perror("RGHOST");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "CGHOST", endp - argp))
                {
                    argp = endp + 1;
                    maze_CGHOST = strtodlist(argp, &endp, &maze_CGHOST_len);
                    if (! maze_CGHOST)
                    {
                        perror("CGHOST");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "ROGHOST", endp - argp))
                {
                    argp = endp + 1;
                    maze_ROGHOST = strtodlist(argp, &endp, &maze_ROGHOST_len);
                    if (! maze_ROGHOST)
                    {
                        perror("ROGHOST");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "COGHOST", endp - argp))
                {
                    argp = endp + 1;
                    maze_COGHOST = strtodlist(argp, &endp, &maze_COGHOST_len);
                    if (! maze_COGHOST)
                    {
                        perror("COGHOST");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "RFRUIT", endp - argp))
                {
                    argp = endp + 1;
                    maze_RFRUIT = strtodlist(argp, &endp, &maze_RFRUIT_len);
                    if (! maze_RFRUIT)
                    {
                        perror("RFRUIT");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "CFRUIT", endp - argp))
                {
                    argp = endp + 1;
                    maze_CFRUIT = strtodlist(argp, &endp, &maze_CFRUIT_len);
                    if (! maze_CFRUIT)
                    {
                        perror("CFRUIT");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "RTOP", endp - argp))
                {
                    argp = endp + 1;
                    maze_RTOP = strtodlist(argp, &endp, &maze_RTOP_len);
                    if (! maze_RTOP)
                    {
                        perror("RTOP");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "RHERO", endp - argp))
                {
                    argp = endp + 1;
                    maze_RHERO = strtodlist(argp, &endp, &maze_RHERO_len);
                    if (! maze_RHERO)
                    {
                        perror("RHERO");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "CHERO", endp - argp))
                {
                    argp = endp + 1;
                    maze_CHERO = strtodlist(argp, &endp, &maze_CHERO_len);
                    if (! maze_CHERO)
                    {
                        perror("CHERO");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "RMSG", endp - argp))
                {
                    argp = endp + 1;
                    maze_RMSG = strtollist(argp, &endp, &maze_RMSG_len);
                    if (! maze_RMSG)
                    {
                        perror("RMSG");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "CMSG", endp - argp))
                {
                    argp = endp + 1;
                    maze_CMSG = strtollist(argp, &endp, &maze_CMSG_len);
                    if (! maze_CMSG)
                    {
                        perror("CMSG");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "RMSG2", endp - argp))
                {
                    argp = endp + 1;
                    maze_RMSG2 = strtollist(argp, &endp, &maze_RMSG2_len);
                    if (! maze_RMSG2)
                    {
                        perror("RMSG2");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "CMSG2", endp - argp))
                {
                    argp = endp + 1;
                    maze_CMSG2 = strtollist(argp, &endp, &maze_CMSG2_len);
                    if (! maze_CMSG2)
                    {
                        perror("CMSG2");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "ABOUT", endp - argp))
                {
                    argp = endp + 1;
                    maze_ABOUT = strword(argp, &endp, 0);
                    if (! maze_ABOUT)
                    {
                        perror("ABOUT");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "NOTE", endp - argp))
                {
                    argp = endp + 1;
                    maze_NOTE = strword(argp, &endp, 0);
                    if (! maze_NOTE)
                    {
                        perror("NOTE");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "FIXME", endp - argp))
                {
                    argp = endp + 1;
                    maze_FIXME = strword(argp, &endp, 0);
                    if (! maze_FIXME)
                    {
                        perror("FIXME");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "DIRHERO", endp - argp))
                {
                    char *dirhero_tmp = NULL;

                    argp = endp + 1;
                    dirhero_tmp = strword(argp, &endp, 0);
                    if (! dirhero_tmp)
                    {
                        perror("DIRHERO");
                        return 1;
                    }
                    argp = endp;
                    if (! strcmp(dirhero_tmp, "UP"))
                    {
                        dirhero = UP;
                    }
                    else if (! strcmp(dirhero_tmp, "DOWN"))
                    {
                        dirhero = DOWN;
                    }
                    else if (! strcmp(dirhero_tmp, "LEFT"))
                    {
                        dirhero = LEFT;
                    }
                    else if (! strcmp(dirhero_tmp, "RIGHT"))
                    {
                        dirhero = RIGHT;
                    }
                    else
                    {
                        fprintf(stderr, "%s: DIRHERO: must be one of UP, DOWN, LEFT or RIGHT; got \"%s\" instead\n",
                                mazefile ? mazefile : builtin_mazefile,
                                dirhero_tmp);
                        fflush(stderr);
                        return 1;
                    }
                    free((void *) dirhero_tmp);
                    dirhero_tmp = NULL;
                }
                else if (! strncmp(argp, "GAMEOVER", endp - argp))
                {
                    argp = endp + 1;
                    msg_GAMEOVER = strword(argp, &endp, 0);
                    if (! msg_GAMEOVER)
                    {
                        perror("GAMEOVER");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "PLAYER1", endp - argp))
                {
                    argp = endp + 1;
                    msg_PLAYER1 = strword(argp, &endp, 0);
                    if (! msg_PLAYER1)
                    {
                        perror("PLAYER1");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "PLAYER2", endp - argp))
                {
                    argp = endp + 1;
                    msg_PLAYER2 = strword(argp, &endp, 0);
                    if (! msg_PLAYER2)
                    {
                        perror("PLAYER2");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "READY", endp - argp))
                {
                    argp = endp + 1;
                    msg_READY = strword(argp, &endp, 0);
                    if (! msg_READY)
                    {
                        perror("READY");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "WALL_COLORS", endp - argp))
                {
                    argp = endp + 1;
                    maze_WALL_COLORS = strword(argp, &endp, &maze_WALL_COLORS_len);
                    if (! maze_WALL_COLORS)
                    {
                        perror("WALL_COLORS");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "DOT_COLORS", endp - argp))
                {
                    argp = endp + 1;
                    maze_DOT_COLORS = strword(argp, &endp, &maze_DOT_COLORS_len);
                    if (! maze_MORTAR_COLORS)
                    {
                        perror("DOT_COLORS");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "PELLET_COLORS", endp - argp))
                {
                    argp = endp + 1;
                    maze_PELLET_COLORS = strword(argp, &endp, &maze_PELLET_COLORS_len);
                    if (! maze_MORTAR_COLORS)
                    {
                        perror("PELLET_COLORS");
                        return 1;
                    }
                    argp = endp;
                }
                else if (! strncmp(argp, "MORTAR_COLORS", endp - argp))
                {
                    argp = endp + 1;
                    maze_MORTAR_COLORS = strword(argp, &endp, &maze_MORTAR_COLORS_len);
                    if (! maze_MORTAR_COLORS)
                    {
                        perror("MORTAR_COLORS");
                        return 1;
                    }
                    argp = endp;
                }
                else
                {
                    fprintf(stderr, "%s: unrecognized maze argument: ",
                            mazefile ? mazefile : builtin_mazefile);
                    fflush(stderr);
                    fwrite((void *) argp, 1, endp - argp, stderr);
                    fflush(stderr);
                    fprintf(stderr, "\n");
                    fflush(stderr);
                    return 1;
                }
            }
            else
            {
                fprintf(stderr, "%s: unrecognized maze arguments: %s\n",
                        mazefile ? mazefile : builtin_mazefile,
                        argp);
                fflush(stderr);
                return 1;
            }
        }
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
    memset((void *) inside_wall, '\0', sizeof(inside_wall));
    tvt.tv_sec = 0;
    tvt.tv_usec = 0;
    gettimeofday(&tvt, 0);
    tvt_used = 0;
    for (n = 0; n < maze_n; n ++)
    {
        int phase;

        total_dots[n] = 0;
        pellets[n] = 0;
        for (phase = 0; phase <= 4; phase ++)
        {
            int phase_done;

            do
            {
                phase_done = 1;
                for (i = 0; i < maze_h; i ++)
                {
                    if (isatty(fileno(stderr)))
                    {
                        tvt2.tv_sec = 0;
                        tvt2.tv_usec = 0;
                        gettimeofday(&tvt2, 0);
                        if (tvt2.tv_sec != tvt.tv_sec)
                        {
                            tvt.tv_sec = tvt2.tv_sec;
                            tvt.tv_usec = tvt2.tv_usec;
                            tvt_used = 1;
                            fprintf(stderr, "%3d%%\r",
                                    (int) (((((double) n) * 5.0 + (double) phase) * maze_h + (double) i) * 100.0
                                           /
                                           (((double) maze_n) * 5.0 * ((double) maze_h))
                                           +
                                           0.5));
                        }
                    }
                    for (j = 0; j <= maze_w; j ++)
                    {
                        c = maze_visual(n, i, j);
                        switch (phase)
                        {
                        case 0:
                            if (ISPELLET(c) || ISDOT(c))
                            {
                                total_dots[n] ++;
                                if (ISPELLET(c))
                                {
                                    pellets[n] ++;
                                }
                            }
                            if (ISNONINVERTABLE(c))
                            {
                                inside_wall[(n*maze_h+i) * (maze_w + 1)+j] |= INSIDE_WALL_NON_INVERTABLE;
                            }
                            break;
                        case 1:
                            if ((! (inside_wall[(n*maze_h+i) * (maze_w + 1)+j] & INSIDE_WALL_NON_INVERTABLE))
                                &&
                                (! udlr[c])
                                &&
                                ((inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j] & INSIDE_WALL_NON_INVERTABLE)
                                 ||
                                 (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j] & INSIDE_WALL_NON_INVERTABLE)
                                 ||
                                 (inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                 ||
                                 (inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE)))
                            {
                                inside_wall[(n*maze_h+i) * (maze_w + 1)+j] |= INSIDE_WALL_NON_INVERTABLE;
                                phase_done = 0;
                            }
                            break;
                        case 2:
                        case 3:
                            if ((! (inside_wall[(n*maze_h+i) * (maze_w + 1)+j] & (INSIDE_WALL_NON_INVERTABLE | INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES | INSIDE_WALL_NO | ((phase == 2) ? INSIDE_WALL_PHASE2 : INSIDE_WALL_PHASE3))))
                                &&
                                ((phase == 3)
                                 ||
                                 ((((inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j] & INSIDE_WALL_NON_INVERTABLE)
                                    ^
                                    (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j] & INSIDE_WALL_NON_INVERTABLE))
                                   &&
                                   ((udlr[c] & 0x05) == 0x05))
                                  ||
                                  (((inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                    ^
                                    (inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE))
                                   &&
                                   ((udlr[c] & 0x50) == 0x50)))))
                            {
                                int painting_done;

                                inside_wall[(n*maze_h+i) * (maze_w + 1)+j] |= INSIDE_WALL_PROVISIONAL;
                                if ((inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j] & INSIDE_WALL_NON_INVERTABLE)
                                    &&
                                    ((udlr[c] & 0x05) == 0x05))
                                {
                                    inside_wall[(n*maze_h+i) * (maze_w + 1)+j] |= INSIDE_WALL_NO;
                                    if (! (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j] & (INSIDE_WALL_NON_INVERTABLE | INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES | INSIDE_WALL_NO)))
                                    {
                                        inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j] |= (INSIDE_WALL_YES | INSIDE_WALL_PROVISIONAL);
                                    }
                                }
                                else if ((inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                         &&
                                         ((udlr[c] & 0x50) == 0x50))
                                {
                                    inside_wall[(n*maze_h+i) * (maze_w + 1)+j] |= INSIDE_WALL_NO;
                                    if (! (inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)] & (INSIDE_WALL_NON_INVERTABLE | INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES | INSIDE_WALL_NO)))
                                    {
                                        inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)] |= (INSIDE_WALL_YES | INSIDE_WALL_PROVISIONAL);
                                    }
                                }
                                else
                                {
                                    inside_wall[(n*maze_h+i) * (maze_w + 1)+j] |= INSIDE_WALL_YES;
                                }
                                do
                                {
                                    int i2, j2;
                                    int undo = 0;

                                    painting_done = 1;
                                    for (i2 = 0; i2 < maze_h; i2 ++)
                                        for (j2 = 0; j2 <= maze_w; j2 ++)
                                        {
                                            long c2;

                                            c2 = maze_visual(n, i2, j2);
                                            if (! undo)
                                            {
                                                if (((! (udlr[c2] & 0x04))
                                                     &&
                                                     (inside_wall[(n*maze_h+YWRAP(i2 + 1)) * (maze_w + 1)+j2] & INSIDE_WALL_YES))
                                                    ||
                                                    ((! (udlr[c2] & 0x40))
                                                     &&
                                                     (inside_wall[(n*maze_h+i2) * (maze_w + 1)+XWRAP2(j2 + 1)] & INSIDE_WALL_YES)))
                                                {
                                                    if (inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] & (INSIDE_WALL_NON_INVERTABLE | INSIDE_WALL_NO))
                                                    {
                                                        undo = 1;
                                                        i2 = 0;
                                                        j2 = 0;
                                                        painting_done = 1;
                                                    }
                                                    else
                                                    {
                                                        if (! (inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] & INSIDE_WALL_YES))
                                                        {
                                                            inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] |= INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES;
                                                            painting_done = 0;
                                                        }
                                                    }
                                                }
                                                else if (((udlr[c2] & 0x04)
                                                          &&
                                                          ((inside_wall[(n*maze_h+YWRAP(i2 + 1)) * (maze_w + 1)+j2] & (INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES)) == ((INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES) ^ (inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] & INSIDE_WALL_PROVISIONAL))))
                                                         ||
                                                         ((udlr[c2] & 0x40)
                                                          &&
                                                          ((inside_wall[(n*maze_h+i2) * (maze_w + 1)+XWRAP2(j2 + 1)] & (INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES)) == ((INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES) ^ (inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] & INSIDE_WALL_PROVISIONAL)))))
                                                {
                                                    if ((inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] & INSIDE_WALL_YES) == INSIDE_WALL_YES)
                                                    {
                                                        undo = 1;
                                                        i2 = 0;
                                                        j2 = 0;
                                                        painting_done = 1;
                                                    }
                                                }
                                            }
                                            if (! undo)
                                            {
                                                if ((inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] & INSIDE_WALL_YES)
                                                    &&
                                                    (! (udlr[c2] & 0x04)))
                                                {
                                                    if (inside_wall[(n*maze_h+YWRAP(i2 + 1)) * (maze_w + 1)+j2] & (INSIDE_WALL_NON_INVERTABLE | INSIDE_WALL_NO))
                                                    {
                                                        undo = 1;
                                                        i2 = 0;
                                                        j2 = 0;
                                                        painting_done = 1;
                                                    }
                                                    else
                                                    {
                                                        if (! (inside_wall[(n*maze_h+YWRAP(i2 + 1)) * (maze_w + 1)+j2] & INSIDE_WALL_YES))
                                                        {
                                                            inside_wall[(n*maze_h+YWRAP(i2 + 1)) * (maze_w + 1)+j2] |= INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES;
                                                            painting_done = 0;
                                                        }
                                                    }
                                                }
                                            }
                                            if (! undo)
                                            {
                                                if ((inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] & INSIDE_WALL_YES)
                                                    &&
                                                    (! (udlr[c2] & 0x40)))
                                                {
                                                    if (inside_wall[(n*maze_h+i2) * (maze_w + 1)+XWRAP2(j2 + 1)] & (INSIDE_WALL_NON_INVERTABLE | INSIDE_WALL_NO))
                                                    {
                                                        undo = 1;
                                                        i2 = 0;
                                                        j2 = 0;
                                                        painting_done = 1;
                                                    }
                                                    else
                                                    {
                                                        if (! (inside_wall[(n*maze_h+i2) * (maze_w + 1)+XWRAP2(j2 + 1)] & INSIDE_WALL_YES))
                                                        {
                                                            inside_wall[(n*maze_h+i2) * (maze_w + 1)+XWRAP2(j2 + 1)] |= INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES;
                                                            painting_done = 0;
                                                        }
                                                    }
                                                }
                                            }
                                            if (undo)
                                            {
                                                if (inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] & INSIDE_WALL_PROVISIONAL)
                                                {
                                                    inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] &= ~(INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES | INSIDE_WALL_NO);
                                                }
                                            }
                                        }
                                }
                                while (! painting_done);
                                {
                                    int i2, j2;

                                    for (i2 = 0; i2 < maze_h; i2 ++)
                                        for (j2 = 0; j2 <= maze_w; j2 ++)
                                        {
                                            if (inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] & INSIDE_WALL_PROVISIONAL)
                                            {
                                                inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] &= ~INSIDE_WALL_PROVISIONAL;
                                            }
                                        }
                                }
                                phase_done = 0;
                                inside_wall[(n*maze_h+i) * (maze_w + 1)+j] |= ((phase == 2) ? INSIDE_WALL_PHASE2 : INSIDE_WALL_PHASE3);
                            }
                            break;
                        case 4:
                            if (! (inside_wall[(n*maze_h+i) * (maze_w + 1)+j] & INSIDE_WALL_NON_INVERTABLE))
                            {
                                int ul, ll, ur, lr;

                                ul = !! (inside_wall[(n*maze_h+i) * (maze_w + 1)+j] & INSIDE_WALL_YES);
                                ll = !! (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j] & INSIDE_WALL_YES);
                                ur = !! (inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_YES);
                                lr = !! (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_YES);
                                if ((ul + ll + ur + lr) == 0)
                                {
                                    if ((udlr[c] & 0x05) == 0x05)
                                    {
                                        if (inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ll = 1;
                                            lr = 1;
                                        }
                                        if (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ur = 1;
                                        }
                                    }
                                    if ((udlr[c] & 0x50) == 0x50)
                                    {
                                        if (inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ll = 1;
                                        }
                                        if (inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ur = 1;
                                            lr = 1;
                                        }
                                    }
                                    if ((udlr[c] & 0x55) == 0x44)
                                    {
                                        if (inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ur = 1;
                                            ll = 1;
                                            lr = 1;
                                        }
                                        if (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                        }
                                    }
                                    if ((udlr[c] & 0x55) == 0x41)
                                    {
                                        if (inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ll = 1;
                                            lr = 1;
                                        }
                                        if (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ur = 1;
                                        }
                                    }
                                    if ((udlr[c] & 0x55) == 0x14)
                                    {
                                        if (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ur = 1;
                                            lr = 1;
                                        }
                                        if (inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ll = 1;
                                        }
                                    }
                                    if ((udlr[c] & 0x55) == 0x11)
                                    {
                                        if (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ur = 1;
                                            ll = 1;
                                        }
                                        if (inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            lr = 1;
                                        }
                                    }
                                    if ((udlr[c] & 0x55) == 0x15)
                                    {
                                        if ((inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                            ||
                                            (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE))
                                        {
                                            ul = 1;
                                            ur = 1;
                                        }
                                        if (inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ll = 1;
                                            lr = 1;
                                        }
                                    }
                                    if ((udlr[c] & 0x55) == 0x45)
                                    {
                                        if ((inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                            ||
                                            (inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE))
                                        {
                                            ll = 1;
                                            lr = 1;
                                        }
                                        if (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ur = 1;
                                        }
                                    }
                                    if ((udlr[c] & 0x55) == 0x51)
                                    {
                                        if ((inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE)
                                            ||
                                            (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE))
                                        {
                                            ul = 1;
                                            ll = 1;
                                        }
                                        if (inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ur = 1;
                                            lr = 1;
                                        }
                                    }
                                    if ((udlr[c] & 0x55) == 0x54)
                                    {
                                        if ((inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                            ||
                                            (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE))
                                        {
                                            ur = 1;
                                            lr = 1;
                                        }
                                        if (inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ll = 1;
                                        }
                                    }
                                    if ((udlr[c] & 0x55) == 0x55)
                                    {
                                        if ((inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE)
                                            ||
                                            (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE))
                                        {
                                            ur = 1;
                                            ll = 1;
                                        }
                                        if ((inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j + 1)] & INSIDE_WALL_NON_INVERTABLE)
                                            ||
                                            (inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j - 1)] & INSIDE_WALL_NON_INVERTABLE))
                                        {
                                            ul = 1;
                                            lr = 1;
                                        }
                                    }
                                    if ((ul + ur + ll + lr) == 4)
                                    {
                                        ul = 0;
                                        ur = 0;
                                        ll = 0;
                                        lr = 0;
                                        inside_wall[(n*maze_h+i) * (maze_w + 1)+j] |= INSIDE_WALL_FULLY_NON_INVERTED;
                                    }
                                }
                                if (((ul + ll + ur + lr) > 2)
                                    ||
                                    (((ul + ll + ur + lr) == 2)
                                     &&
                                     ul))
                                {
                                    inside_wall[(n*maze_h+i) * (maze_w + 1)+j] |= INSIDE_WALL_INVERTED;
                                }
                                if (((ul + ll + ur + lr) == 4)
                                    &&
                                    (((!! (udlr[c] & 0x40))
                                      +
                                      (!! (udlr[c] & 0x10))
                                      +
                                      (!! (udlr[c] & 0x04))
                                      +
                                      (!! (udlr[c] & 0x01))) > 1))
                                {
                                    inside_wall[(n*maze_h+i) * (maze_w + 1)+j] |= INSIDE_WALL_FULLY_INVERTED;
                                }
                            }
                            break;
                        }
                    }
                }
            }
            while (! phase_done);
        }
    }
    if (tvt_used)
    {
        fprintf(stderr, "    \r"); fflush(stderr);
        tvt_used = 0;
    }

    pellet_time = PELLET_ADJUST(7 * ONESEC);
    sprite_register[FRUIT] = SPRITE_FRUIT;
    sprite_register[FRUIT_SCORE] = SPRITE_FRUIT_SCORE;
    sprite_register_frame[FRUIT] =
        sprite_register_frame[FRUIT_SCORE] = BONUS(level);
    sprite_register_x[FRUIT] =
        sprite_register_x[FRUIT_SCORE] = (int) XFRUIT;
    sprite_register_y[FRUIT] =
        sprite_register_y[FRUIT_SCORE] = (int) YFRUIT;

    sprite_register[GHOST_SCORE] = SPRITE_200;
    sprite_register_frame[GHOST_SCORE] = 0;

    hero_dir = dirhero;
    sprite_register[HERO] = SPRITE_HERO + ((hero_dir == LEFT) ? 4 : (hero_dir == RIGHT) ? 12 : (hero_dir == DOWN) ? 16 : 0);
    sprite_register_frame[HERO] = 0;
    sprite_register_x[HERO] = (int) XHERO;
    sprite_register_y[HERO] = (int) YHERO;
    sprite_register_used[HERO] = 0;

#if USE_COLOR
    sprite_register_color[HERO] = 0xE;
    sprite_register_color[BIGHERO_UL] = 0xE;
    sprite_register_color[BIGHERO_UR] = 0xE;
    sprite_register_color[BIGHERO_LL] = 0xE;
    sprite_register_color[BIGHERO_LR] = 0xE;
#endif

    for (i = 0; i < ghosts; i++) {
        int eyes, mean, blue;

        eyes = GHOSTEYES(i);
        mean = MEANGHOST(i);
        blue = BLUEGHOST(i);
        sprite_register[eyes] = SPRITE_EYES;
        sprite_register[mean] = SPRITE_MEAN;
        sprite_register[blue] = SPRITE_BLUE;
        sprite_register_used[eyes] =
            sprite_register_used[mean] =
            sprite_register_used[blue] = 0;
        sprite_register_frame[eyes] =
            sprite_register_frame[mean] =
            sprite_register_frame[blue] = 0;
        ghost_mem[i] = 0;
        ghost_timer[i] = TWOSECS;
        ghost_man[i] = 0;
#if USE_COLOR
        sprite_register_color[eyes] = 0xF;
        sprite_register_color[blue] = 0x9;
        sprite_register_color[mean] = (EXTRA_GHOST_COLORS)[(i % strlen(EXTRA_GHOST_COLORS))];
#endif
    }

#if USE_COLOR
    if (GHOST0 < ghosts) sprite_register_color[MEANGHOST(GHOST0)] = 0xB;
    if (GHOST1 < ghosts) sprite_register_color[MEANGHOST(GHOST1)] = 0xC;
    if (GHOST2 < ghosts) sprite_register_color[MEANGHOST(GHOST2)] = 0xD;
    if (GHOST3 < ghosts) sprite_register_color[MEANGHOST(GHOST3)] = 0x6;
#endif

    if (dump_maze) {
        printf("int maze_n = %d;\n", maze_n);
        printf("int maze_w = %d;\n", maze_w);
        printf("int maze_h = %d;\n", maze_h);
        printf("int maze_flags = %d;\n", maze_flags);
        printf("const char *maze_args = ");
        if (maze_args)
        {
            printf("\"");
            escape(maze_args, strlen(maze_args));
            printf("\"");
        }
        else
        {
            printf("0");
        }
        printf(";\n");
        printf("static const char builtin_mazefile_str[] = \"");
        escape(mazefile ? mazefile : builtin_mazefile,
               strlen(mazefile ? mazefile : builtin_mazefile));
        printf("\";\n");
        printf("const char *builtin_mazefile = builtin_mazefile_str;\n");
        printf("const char *maze_data = \n");
        for (n = 0; n < maze_n; n ++)
        {
            printf("/* 0x%X */\n", n);
            for (i = 0; i < maze_h; i ++) {
                printf("  \"");
                escape(maze + ((n * maze_h) + i) * (maze_w + 1), maze_w + 1);
                printf("\"\n");
            }
        }
        printf(";\n");
        printf("const char *maze_color_data = \n");
        for (n = 0; n < maze_n; n ++)
        {
            printf("/* 0x%X */\n", n);
            for (i = 0; i < maze_h; i ++) {
                printf("  \"");
                escape(maze_color + ((n * maze_h) + i) * (maze_w + 1), maze_w + 1);
                printf("\"\n");
            }
        }
        printf(";\n");
    }
    if (dump_sprite)
        writefont(spritefile ? spritefile : builtin_spritefile,
                  "sprite", sprite_w, sprite_h, sprite, sprite_used, sprite_flags, sprite_color, sprite_args);
    if (dump_tile)
        writefont(tilefile ? tilefile : builtin_tilefile,
                  "tile", tile_w, tile_h, tile, tile_used, tile_flags, tile_color, tile_args);

    for (i = 0; i < 256; i ++)
    {
        int c_mapped;

        c = (unsigned long) (unsigned char) cp437_sprite[i];
        c_mapped = c;
        while (c_mapped
               &&
               (! tile_used[c_mapped])
               &&
               (fallback_cp437[c_mapped] != c)
               &&
               (fallback_cp437[c_mapped] != c_mapped))
        {
            c_mapped = (unsigned long) (unsigned char) fallback_cp437[c_mapped];
            cp437_sprite[i] = (unsigned char) c_mapped;
        }
    }

    if (nogame)
        fflush(stdout), fflush(stderr), exit(0);

    if (! setlocale(LC_CTYPE, ""))
    {
        fprintf(
            stderr,
            "warning: setlocale(LC_CTYPE, \"\") failed\n");
        fflush(stderr);
    }

    if (use_fullwidth)
    {
        uni_cp437 = uni_cp437_fullwidth;
    }

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
        if (! lines) lines = (reflect ? (maze_w * gfx_w) : (maze_h * gfx_h)) + (3 * tile_h + sprite_h);
        if (! columns) columns = (reflect ? (maze_h * gfx_h) : (maze_w * gfx_w)) * (use_fullwidth ? 2 : 1);
#ifdef INITSCR_WITH_HINTS
        initscrWithHints(lines,
                         columns,
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
        nodelay(stdscr, TRUE);
        intrflush(stdscr, FALSE);
        my_attrset(0);
#if USE_ATTR
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
            if (! gamecycle())
            {
                break;
            }
        }
#if USE_SIGWINCH
        signal(SIGWINCH, old_sigwinch_handler ? old_sigwinch_handler : SIG_DFL);
#endif
        my_attrset(0);
#if USE_ATTR
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

                if (! ioctl(fileno(stdout), TIOCGWINSZ, &wsz))
                {
                    lines = wsz.ws_row;
                    columns = wsz.ws_col;
                }
#else
#ifdef TIOCGSIZE
                struct ttysize tsz;

                if (! ioctl(fileno(stdout), TIOCGSIZE, &tsz))
                {
                    lines = tsz.ts_lines;
                    columns = tsz.ts_cols;
                }
#endif
#endif
#endif
                if (! lines) lines = LINES;
                if (! columns) columns = COLS;
#ifdef KEY_RESIZE
#ifdef __PDCURSES__
                resize_term(0, 0);
#else
                resizeterm(lines ? lines : LINES, columns ? columns : COLS);
#endif
#else
                {
                    static char buf[15];

                    sprintf(buf, "%d", lines);
                    setenv("LINES", buf, 1);
                    sprintf(buf, "%d", columns);
                    setenv("COLUMNS", buf, 1);
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
    }
    if (cd_to_uni != (iconv_t) -1)
    {
        iconv_close(cd_to_uni);
    }
#endif
    return 0;
}
#ifdef END_OF_MAIN
END_OF_MAIN()
#endif
