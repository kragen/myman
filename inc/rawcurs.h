/*
 * rawcurs.h - Win32 Console / CONIO / TOS / AROS / *nix TTY driver for the MyMan video game
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

/* NOTE: Right now the stdio (i.e. non-Win32 Console, non-CONIO) driver
 * outputs raw ANSI/VT100/XTerm-style, VT52-style, or ADM3A-style
 * escape sequences [probably about 99% of all terminals and
 * emulations still in use that are not block mode and not Tektronix
 * can be convinced to fall into one of these categories] and expects
 * special keys (only LEFT, RIGHT, UP and DOWN so far) and reports
 * (cursor position, title and icon title) in the corresponding
 * format. The VT52-style and ADM3A-style modes don't support color or
 * attributes. */

/* FIXME: Maybe we should use termcap or terminfo for the stdio
 * (i.e. non-Win32 Console, non-CONIO, non-TOS, non-AROS) case... then
 * again, maybe not. If you have those, you probably have curses too
 * and don't care about this light-weight reimplementation of a curses
 * subset. */

/* NOTE: You can test the VT52 mode fairly easily using a real VT100,
 * an XTerm, or something compatible.
 * to put a VT100 or XTerm in VT52 mode:
 * $ printf '\033[?2l'; oTERM="$TERM"; TERM=xterm-vt52
 * to return to VT100/XTerm native mode:
 * $ printf '\033<'; TERM="$oTERM"; unset oTERM
 * Adjust as appropriate if your shell is not Bourne-style. Enjoy! :)
 */

/*
 * Win32 console custom color palettes (HACK!)
 *
 * This driver includes a huge and gruesome hack to get and set the
 * console palette on Win32 systems. It has not been tested on Win9x
 * and is unlikely to work there; although a VGA register trick might
 * work with severe convolution, I have not implemented that and do
 * not have Win9x for testing. This code relies on various
 * undocumented features of the Win32 console, and disables itself
 * under Wine (detected by presence of HKCU\Software\Wine) in order to
 * prevent a hard crash due to calling unimplemented
 * GetCurrentConsoleFont.
 *
 * To read the palette, we use one of (in order of preference):
 *
 * 1. GetConsoleScreenBufferInfoEx (not tested, but should work)
 *
 * 2. Console registry values named ColorTable<nn> (type DWORD,
 * formatted as COLORREF), where <nn> is a zero-padded decimal palette
 * index in the range [00, 15]
 *
 *    NOTE: This will not reflect unsaved palette changes made using
 *    the console property dialog
 *
 * 3. Built-in default palette (seems to match the O.S. defaults, at
 * least on WinXP)
 *
 * To write the palette, we use one of (in order of preference):
 *
 * 1. GetConsoleScreenBufferInfoEx + SetConsoleScreenBufferInfoEx (not
 * tested, but should work)
 *
 * 2. Sending the undocumented message WM_SETCONSOLEINFO (WM_USER +
 * 201) to the console window (twice, if fullscreen)
 *
 *    This requires determining all the console settings beforehand;
 *    for many of them there does not seem to be a portable or
 *    documented technique, so we often fall back to the console
 *    registry entries; see the code for details.
 *
 *    This also involves temporarily injecting a copy of the settings
 *    data structure into the console window owner address space (and
 *    removing it afterward.)
 *
 *    We also have to revert the console font index afterward because
 *    the message does not seem to preserve the TrueType/raster
 *    distinction.
 *
 * Console registry entries are read from (in order of preference):
 *
 * 1. HKCU\Console\<Title> where <Title> is one of (in order of
      preference):
 *
 *    a. Value fetched by GetConsoleTitle
 *
 *    b. Value fetched by GetConsoleTitle with appended command line
 *    matching the one returned by GetCommandLine (delimited by any
 *    single character between spaces) stripped off
 *
 *    c. Console process executable name, determined by one of (in
 *    order of preference):
 *
 *       i. QueryFullProcessImageName
 *       ii. GetModuleFileNameEx
 *       iii. CreateToolhelp32Snapshot
 *       iv. _pgmptr (if the current process owns the console)
 *
 * 2. HKCU\Console
 *
 * When needed, a handle to the console window is obtained by one of
 * (in order of preference):
 *
 * 1. GetConsoleWindow
 *
 * 2. FindWindow using a temporary hopefully-unique title including
 * the process ID and (if available) the results of
 * QueryPerformanceCounter
 *
 * = Summary =
 *
 * This is large and brittle. I can see why almost no app does this.
 *
 */

/* feature guessing */
#ifndef MYMAN_GUESS_H_INCLUDED
#include "guess.h"
#endif

#ifndef MYMAN_RAWCURS_H_INCLUDED
#define MYMAN_RAWCURS_H_INCLUDED 1

#if defined(WIN32)
#ifndef USE_WINCONSOLE
#define USE_WINCONSOLE 1
#endif
#endif

#ifndef USE_WINCONSOLE
#define USE_WINCONSOLE 0
#endif

#ifdef macintosh
#ifdef TARGET_API_MAC_CARBON
#include <Carbon.h>
#else
#include <Errors.h>
#include <Files.h>
#include <MacTypes.h>
#include <Timer.h>
#endif
#include <ioctl.h>
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

#ifndef HAVE_TTYNAME
#define HAVE_TTYNAME 0
#endif

#ifdef __TINYC__
#ifndef HAVE_STRUCT_TIMEVAL
#define HAVE_STRUCT_TIMEVAL 0
#endif
#endif

#ifdef __BORLANDC__

/* Borland C++ supports Borland-style <conio.h>, but it is inferior to
 * their native Win32 console hooks because it doesn't support
 * non-standard terminal dimensions, so we disable it by default. */

#ifndef USE_CONIO
#define USE_CONIO 0
#endif

#endif /* defined(__BORLANDC__) */

#ifndef HAVE_WCWIDTH
#define HAVE_WCWIDTH 0
#endif

#else /* ! defined(WIN32) */

#if HAVE_SYS_H
#include <sys.h>
#else
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#endif

#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#if defined(__MSDOS__) || defined(CPM)

/* workarounds for DOS and CP/M */

#ifdef __MSDOS__
#include <dos.h>
#endif

#ifdef CPM
#include <cpm.h>

#ifndef USE_IOCTL
#define USE_IOCTL 0
#endif

#endif

#ifdef __DJGPP__

/* DJGPP supports termios and full Borland-style CONIO */

#ifndef USE_TERMIOS
#define USE_TERMIOS 1
#endif

#ifndef USE_CONIO
#define USE_CONIO 1
#endif

#ifndef HAVE_GETTIMEOFDAY
#define HAVE_GETTIMEOFDAY 1
#endif

#ifndef HAVE_USLEEP
#define HAVE_USLEEP 1
#endif

#ifndef HAVE_TTYNAME
#define HAVE_TTYNAME 1
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

#ifdef __TURBOC__

/* Turbo C supports full Borland-style CONIO */

#ifndef USE_CONIO
#define USE_CONIO 1
#endif

#endif /* defined(__TURBOC__) */

#ifndef USE_TERMIOS
#define USE_TERMIOS 0
#endif

#ifndef USE_CONIO
#define USE_CONIO 0
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

#ifndef HAVE_TTYNAME
#define HAVE_TTYNAME 0
#endif

/* other DOS environments typically support only Microsoft-style
 * CONIO */

#ifndef USE_CONIO_INPUT
#define USE_CONIO_INPUT 1
#endif

#endif /* defined(__MSDOS__) || defined(CPM) */

#ifdef __atarist__

#ifndef USE_TOSCONSOLE
#define USE_TOSCONSOLE 1
#endif

#endif /* defined(__atarist__) */

#if defined(__AROS__) || defined(AMIGA) || defined(MORPHOS)

#ifndef USE_AROSCONSOLE
#define USE_AROSCONSOLE 1
#endif

#endif /* defined(__AROS__) || defined(AMIGA) || defined(MORPHOS) */

#ifndef HAVE_WCWIDTH
#if defined(__MSDOS__) || defined(__atarist__) || defined(__AROS__) || defined(AMIGA) || defined(MORPHOS)
#define HAVE_WCWIDTH 0
#else
#define HAVE_WCWIDTH 1
#endif

#endif /* defined(__MSDOS__) || defined(__atarist__) || defined(__AROS__) || defined(AMIGA) || defined(MORPHOS) */

#ifndef USE_TERMIOS
#ifdef macintosh
#define USE_TERMIOS 0
#else
#define USE_TERMIOS 1
#endif
#endif

#endif /* ! defined(WIN32) */

#ifndef HAVE_WCWIDTH
#define HAVE_WCWIDTH 1
#endif

/* FIXME: this is a horrible hack! */
#if ! HAVE_WCWIDTH
#ifdef UNICODE
#define wcwidth(wc) (((((long) (wc)) >= 0x0020L) && (((long) (wc)) < 0x3000L)) ? 1 : ((((long) (wc)) >= 0x3000L) && (((long) (wc)) <= 0x10fffdL)) ? 2 : -1)
#else
#define wcwidth(wc) (((((long) (wc)) >= 0x20L) && (((long) (wc)) < 0xffL)) ? 1 : -1)
#endif
#endif

#ifndef HAVE_STRUCT_TIMEVAL
#ifdef macintosh
#define HAVE_STRUCT_TIMEVAL 0
#else
#define HAVE_STRUCT_TIMEVAL 1
#endif
#endif

#if ! HAVE_STRUCT_TIMEVAL

struct timeval
{
    long tv_sec;
    long tv_usec;
};

#endif /* ! HAVE_STRUCT_TIMEVAL */

#ifndef USE_TERMIOS
#define USE_TERMIOS 0
#endif

#ifndef USE_SGTTY
#define USE_SGTTY 0
#endif

#ifndef HAVE_GETTIMEOFDAY
#ifdef macintosh
#define HAVE_GETTIMEOFDAY 0
#else /* ! defined(macintosh) */
#define HAVE_GETTIMEOFDAY 1
#endif /* ! defined(macintosh) */
#endif /* ! defined(HAVE_GETTIMEOFDAY) */

#if ! HAVE_GETTIMEOFDAY

#undef gettimeofday
#define gettimeofday rawcurses_gettimeofday

static int rawcurses_gettimeofday(struct timeval *tv, void *tz)
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

#ifndef HAVE_ISATTY
#ifdef macintosh
#define HAVE_ISATTY 0
#else
#define HAVE_ISATTY 1
#endif
#endif

#if ! HAVE_ISATTY

#undef isatty
#define isatty(fd) rawcurses_isatty(fd)

int rawcurses_isatty(int fd)
{

#ifdef macintosh

    char namebuf[512];

    memset((void *) namebuf, 0, sizeof(namebuf));
#ifdef FIOFNAME
    if (ioctl(fd, FIOFNAME, (long *) (void *) namebuf) == -1)
    {
        return 0;
    }
#endif
    if ((! strcmp(namebuf, "dev:console"))
        ||
        (! strncmp(namebuf, "dev:tty", strlen("dev:tty")))
        ||
        (! strncmp(namebuf, "dev:pty", strlen("dev:pty"))))
    {
        return 1;
    }

#else /* ! defined(macintosh) */

#error no isatty(3) implementation for your platform, sorry

#endif /* ! defined(macintosh) */

    return 0;
}

#endif

#ifndef HAVE_USLEEP
#ifdef macintosh
#define HAVE_USLEEP 0
#else
#define HAVE_USLEEP 1
#endif
#endif

#if ! HAVE_USLEEP

#if defined(WIN32)

#undef usleep

/* originally from http://wyw.dcweb.cn/sleep.h.txt by Wu Yongwei */

#define usleep(t) Sleep((t) / 1000)

#else /* ! defined(WIN32) */

#undef usleep
#define usleep rawcurses_usleep

static int
rawcurses_usleep(unsigned long usecs)
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

#ifndef HAVE_TTYNAME
#define HAVE_TTYNAME 1
#endif

#if ! HAVE_TTYNAME
#undef ttyname
#define ttyname(fd) (isatty(fileno(stdin)) ? "con" : "stdin")
#endif

#if USE_TERMIOS
#include <termios.h>
#endif

#if USE_SGTTY
#include <sgtty.h>
#endif

#ifndef USE_CONIO
#define USE_CONIO 0
#endif

#ifndef USE_CONIO_INPUT
#define USE_CONIO_INPUT USE_CONIO
#endif

#ifndef USE_AROSCONSOLE
#define USE_AROSCONSOLE 0
#endif

#if USE_AROSCONSOLE
/* Workarounds for AROS (tested; seems to work) and AmigaOS (untested) */

#include <proto/dos.h>

#endif

#ifndef USE_TOSCONSOLE
#define USE_TOSCONSOLE 0
#endif

#if USE_TOSCONSOLE

#if defined(__GNUC__) && (__GNUC__ <= 2)

/* Workarounds for TOS; old interfaces */
#include <aesbind.h>
#include <keycodes.h>
#include <osbind.h>

#else /* ! defined(__GNUC__) && (__GNUC__ <= 2) */

/* Workarounds for TOS; new interfaces */
#include <gem.h>
#include <osbind.h>

/* once upon a time these were in <keycodes.h>; these days they seem
 * to be AWOL */
#define CURS_DN  0x50
#define CURS_LF  0x4b
#define CURS_RT  0x4d
#define CURS_UP  0x48
#define K_ESC    0x01
#define K_DEL    0x53
#define K_RET    0x1c
#define KP_0     0x70
#define KP_1     0x6d
#define KP_2     0x6e
#define KP_3     0x6f
#define KP_4     0x6a
#define KP_5     0x6b
#define KP_6     0x6c
#define KP_7     0x67
#define KP_8     0x68
#define KP_9     0x69
#define KP_DOT   0x71
#define KP_ENTER 0x72
#define KP_LP    0x63
#define KP_MINUS 0x4a
#define KP_PLUS  0x4e
#define KP_RP    0x64
#define KP_SLASH 0x65
#define KP_STAR  0x66

#endif /* ! defined(__GNUC__) && (__GNUC__ <= 2) */

#endif /* USE_TOSCONSOLE */

#if HAVE_WCHAR_H
#include <wchar.h>
#else
#if HAVE_STDINT_H
#include <stdint.h>
#endif
#endif

#ifndef HAVE_WCHAR_T
#if defined(wchar_t) || (! (defined(LSI_C) || defined(__TURBOC__)))
#define HAVE_WCHAR_T 1
#endif
#endif

#ifndef HAVE_WCHAR_T
#define HAVE_WCHAR_T 0
#endif

#if ! HAVE_WCHAR_T
#define wchar_t rawcurses_wchar_t
typedef int rawcurses_wchar_t;
#endif

#if USE_CONIO || USE_CONIO_INPUT

#ifdef RAWCURSES_CONIO_H
#include RAWCURSES_CONIO_H
#else
#include <conio.h>
#endif

#ifdef _INCLUDED_TURBOC_H
/* recover from serious braindamage in TurboC.h */
#undef short
#undef int
#undef unsigned
#undef long
#endif /* defined(_INCLUDED_TURBOC_H) */

#endif /* USE_CONIO || USE_CONIO_INPUT */

#ifndef USE_DIM_AND_BRIGHT
#define USE_DIM_AND_BRIGHT (! (rawcurses_stdio && has_colors() && rawcurses_stdio_nocolorbold && (COLORS == 8)))
#endif

#ifndef USE_ATTR
#define USE_ATTR 1
#endif

#ifndef USE_UNDERLINE
#define USE_UNDERLINE 1
#endif

#ifndef SWAPDOTS
#define SWAPDOTS (! (rawcurses_stdio && rawcurses_stdio_acs && (rawcurses_stdio_acs_h19 || rawcurses_stdio_cp437 || rawcurses_stdio_noswapdots)))
#endif

#ifndef USE_RAW_UCS
#ifdef UNICODE
#define USE_RAW_UCS 1
#else
#define USE_RAW_UCS 0
#endif
#endif

#if USE_WINCONSOLE

#ifndef USE_SIGWINCH
#define USE_SIGWINCH 0
#endif

#ifndef USE_IOCTL
#define USE_IOCTL 0
#endif

#else

#ifdef SIGWINCH
#ifndef USE_SIGWINCH
#define USE_SIGWINCH 1
#endif
#endif

#ifndef USE_SIGWINCH
#define USE_SIGWINCH 0
#endif

#ifndef USE_IOCTL
#define USE_IOCTL 1
#endif

#endif

#if USE_IOCTL
#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#else
#if HAVE_IOCTL_H
#include <ioctl.h>
#else
#if HAVE_IO_H
#include <io.h>
#endif
#endif
#endif
#endif

#ifndef HAVE_SELECT
#ifdef FD_SET
#define HAVE_SELECT 1
#else
#define HAVE_SELECT 0
#endif
#endif

#undef chtype
#define chtype rawcurses_chtype

typedef long chtype;

#undef attr_t
#define attr_t rawcurses_attr_t

typedef chtype attr_t;

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

#define ESCAPE(s) "\x1b" s

#define CSI(s) ESCAPE("[" s)

#define ST ESCAPE("\\")

#define OSC(s) ESCAPE("]" s)

#define cbreak()

#define noecho()

#define nonl()

#define nodelay(stdscr, x)

#define intrflush(stdscr, x)

#define immedok(stdscr, x)

#define idlok(stdscr, x)

#define clearok(stdscr, x)

#define leaveok(stdscr, x)

#undef getyx
#define getyx(stdscr, y, x) (rawcurses_getyx(&rawcurses_y, &rawcurses_x), (x) = rawcurses_x, (y) = rawcurses_y)

#undef wrefresh
#define wrefresh(stdscr) rawcurses_refresh()

#undef beep
#define beep() rawcurses_addch('\7')

#undef echo
#define echo()

#undef use_env
#define use_env(x)

#undef keypad
#define keypad(stdscr, x)

#define LINES rawcurses_h

#define COLS rawcurses_w

#define _PAIR_SHIFT 21

#undef COLOR_PAIR
#define COLOR_PAIR(p) (((chtype) p) << _PAIR_SHIFT)

#undef PAIR_NUMBER
#define PAIR_NUMBER(a) (((a) & RAWCURSES__A_COLOR) >> _PAIR_SHIFT)

#undef A_CHARTEXT
#define A_CHARTEXT ((chtype) 0x1fffffL)

#undef RAWCURSES__A_COLOR
#define RAWCURSES__A_COLOR 0x0fe00000L

#undef A_DIM
#define A_DIM ((chtype) 0x10000000UL)

#undef A_BOLD
#define A_BOLD ((chtype) 0x20000000UL)

#undef A_REVERSE
#define A_REVERSE ((chtype) 0x40000000UL)

#undef A_UNDERLINE
#define A_UNDERLINE ((chtype) 0x80000000UL)

#undef A_STANDOUT
#define A_STANDOUT A_REVERSE

#ifndef FOREGROUND_BLUE
#define FOREGROUND_BLUE 1
#endif

#ifndef FOREGROUND_GREEN
#define FOREGROUND_GREEN 2
#endif

#ifndef FOREGROUND_RED
#define FOREGROUND_RED 4
#endif

#ifndef FOREGROUND_INTENSITY
#define FOREGROUND_INTENSITY 8
#endif

#ifndef BACKGROUND_BLUE
#define BACKGROUND_BLUE 16
#endif

#ifndef BACKGROUND_GREEN
#define BACKGROUND_GREEN 32
#endif

#ifndef BACKGROUND_RED
#define BACKGROUND_RED 64
#endif

#ifndef BACKGROUND_INTENSITY
#define BACKGROUND_INTENSITY 128
#endif

#undef ACS_LRCORNER
#undef ACS_URCORNER
#undef ACS_ULCORNER
#undef ACS_LLCORNER
#undef ACS_PLUS
#undef ACS_HLINE
#undef ACS_LTEE
#undef ACS_RTEE
#undef ACS_BTEE
#undef ACS_TTEE
#undef ACS_VLINE
#undef ACS_BULLET
#undef ACS_BLOCK

#ifdef UNICODE

#define ACS_LRCORNER 0x2518U

#define ACS_URCORNER 0x2510U

#define ACS_ULCORNER 0x250cU

#define ACS_LLCORNER 0x2514U

#define ACS_PLUS 0x253cU

#define ACS_HLINE 0x2500U

#define ACS_LTEE 0x251cU

#define ACS_RTEE 0x2524U

#define ACS_BTEE 0x2534U

#define ACS_TTEE 0x252cU

#define ACS_VLINE 0x2502U

#define ACS_BULLET ((SWAPDOTS) ? 0x00b7U : 0x2022U)

#define ACS_BLOCK 0x2588U

#define RAWCURSES__ACS_THALF 0x2580U

#define RAWCURSES__ACS_BHALF 0x2584U

#define RAWCURSES__ACS_LHALF 0x258CU

#define RAWCURSES__ACS_RHALF 0x2590U

#else

#define ACS_LRCORNER 0xd9U

#define ACS_URCORNER 0xbfU

#define ACS_ULCORNER 0xdaU

#define ACS_LLCORNER 0xc0U

#define ACS_PLUS 0xc5U

#define ACS_HLINE 0xc4U

#define ACS_LTEE 0xc3U

#define ACS_RTEE 0xb4U

#define ACS_BTEE 0xc1U

#define ACS_TTEE 0xc2U

#define ACS_VLINE 0xb3U

#define ACS_BULLET ((SWAPDOTS) ? 0xf9U : 0xfeU)

#define ACS_BLOCK 0xdb

#define RAWCURSES__ACS_THALF 0xdf

#define RAWCURSES__ACS_BHALF 0xdc

#define RAWCURSES__ACS_LHALF 0xdd

#define RAWCURSES__ACS_RHALF 0xde

#endif

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

#undef COLORS
#define COLORS ((rawcurses_16color || (rawcurses_stdio && (rawcurses_stdio_88color || rawcurses_stdio_256color))) ? 16 : 8)

#undef COLOR_PAIRS
#define COLOR_PAIRS (16 * 16 / 2)

#undef COLOR_BLACK
#define COLOR_BLACK 0

#undef COLOR_BLUE
#define COLOR_BLUE 4

#undef COLOR_GREEN
#define COLOR_GREEN 2

#undef COLOR_CYAN
#define COLOR_CYAN (COLOR_BLUE | COLOR_GREEN)

#undef COLOR_RED
#define COLOR_RED 1

#undef COLOR_MAGENTA
#define COLOR_MAGENTA (COLOR_RED | COLOR_BLUE)

#undef COLOR_YELLOW
#define COLOR_YELLOW (COLOR_RED | COLOR_GREEN)

#undef COLOR_WHITE
#define COLOR_WHITE (COLOR_RED | COLOR_GREEN | COLOR_BLUE)

#define PEN_BRIGHT 8

#define INITSCR_WITH_HINTS

#define RAWCURSES_RESIZE_QUIESCE_USECS 250000L

#define RAWCURSES_PALETTE_QUIESCE_USECS 250000L

#define RAWCURSES_TITLE_QUIESCE_USECS 50000L

#ifndef RAWCURSES_GLOBAL
#define RAWCURSES_GLOBAL(dfn, init) static dfn init
#endif

#ifndef RAWCURSES_GLOBAL0
#define RAWCURSES_GLOBAL0(dfn) static dfn
#endif

RAWCURSES_GLOBAL0(int rawcurses_ungetch_buffer = ERR);
RAWCURSES_GLOBAL(int rawcurses_debug_utf8, = 0);
RAWCURSES_GLOBAL(int rawcurses_debug_utf8_y, = 0);
RAWCURSES_GLOBAL(int rawcurses_after_endwin, = 1);
RAWCURSES_GLOBAL(int rawcurses_winsize_pending, = 0);
RAWCURSES_GLOBAL(int rawcurses_got_winsize, = 0);
RAWCURSES_GLOBAL(int rawcurses_winsize_on_clear, = 0);
RAWCURSES_GLOBAL(int rawcurses_cursor_visibility, = 1);
RAWCURSES_GLOBAL(int rawcurses_w, = 0);
RAWCURSES_GLOBAL(int rawcurses_h, = 0);
RAWCURSES_GLOBAL(int rawcurses_nw, = 0);
RAWCURSES_GLOBAL(int rawcurses_nh, = 0);
RAWCURSES_GLOBAL(int rawcurses_x, = 0);
RAWCURSES_GLOBAL(int rawcurses_y, = 0);
RAWCURSES_GLOBAL(attr_t rawcurses_attr, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio, = -1);
RAWCURSES_GLOBAL(int rawcurses_raw, = 0);
RAWCURSES_GLOBAL(int rawcurses_builtin_wcwidth, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_utf8, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_iso2022, = 0);
RAWCURSES_GLOBAL(int rawcurses_civis, = -1);
RAWCURSES_GLOBAL(int rawcurses_transparent, = 1);
RAWCURSES_GLOBAL(chtype rawcurses_stdio_utf8_state, = 0UL);
RAWCURSES_GLOBAL(int rawcurses_stdio_utf8_remaining, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_acs, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_acs_h19, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_noswapdots, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_cp437, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_acs_nobullet, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_nocolorbold, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_vt52, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_st52, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_amiga, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_tw52, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_adm3a, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_relcup, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_256color, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_88color, = 0);
RAWCURSES_GLOBAL(int rawcurses_16color, = 1);
RAWCURSES_GLOBAL(int rawcurses_color, = 1);
RAWCURSES_GLOBAL(int rawcurses_fixedpal, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_ccc, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_ccc_linux, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_blink, = 0);
#if USE_TOSCONSOLE
RAWCURSES_GLOBAL(int rawcurses_stdio_stcon, = 0);
#endif
#if USE_AROSCONSOLE
RAWCURSES_GLOBAL(int rawcurses_stdio_aroscon, = 0);
RAWCURSES_GLOBAL(int rawcurses_stdio_aroscon_oldmode, = 0);
#endif
#if USE_CONIO
RAWCURSES_GLOBAL(int rawcurses_stdio_conio, = 0);
#endif
#if USE_CONIO_INPUT
RAWCURSES_GLOBAL(int rawcurses_stdio_conio_input, = 0);
#endif
#ifdef SIGTSTP
RAWCURSES_GLOBAL(volatile int rawcurses_got_sigtstp, = 0);
RAWCURSES_GLOBAL(void (*rawcurses_old_sigtstp_handler)(int), = NULL);
#endif
#ifdef SIGINT
RAWCURSES_GLOBAL(volatile int rawcurses_got_sigint, = 0);
RAWCURSES_GLOBAL(void (*rawcurses_old_sigint_handler)(int), = NULL);
#endif

#define RAWCURSES_INPUT_STATE_DEFAULT 0
#define RAWCURSES_INPUT_STATE_ESC (RAWCURSES_INPUT_STATE_DEFAULT + 1)
#define RAWCURSES_INPUT_STATE_ETERM_KEY (RAWCURSES_INPUT_STATE_ESC + 1)
#define RAWCURSES_INPUT_STATE_ET52_KEY (RAWCURSES_INPUT_STATE_ETERM_KEY + 1)
#define RAWCURSES_INPUT_STATE_IDENTITY_VT52 (RAWCURSES_INPUT_STATE_ET52_KEY + 1)
#define RAWCURSES_INPUT_STATE_CSI (RAWCURSES_INPUT_STATE_IDENTITY_VT52 + 1)
#define RAWCURSES_INPUT_STATE_CSI_DONE (RAWCURSES_INPUT_STATE_CSI + 1)
#define RAWCURSES_INPUT_STATE_SS3 (RAWCURSES_INPUT_STATE_CSI_DONE + 1)
#define RAWCURSES_INPUT_STATE_OSC_ESC (RAWCURSES_INPUT_STATE_SS3 + 1)
#define RAWCURSES_INPUT_STATE_OSC (RAWCURSES_INPUT_STATE_OSC_ESC + 1)
#define RAWCURSES_INPUT_STATE_IGNORE_CR (RAWCURSES_INPUT_STATE_OSC + 1)

RAWCURSES_GLOBAL(int rawcurses_input_state, = RAWCURSES_INPUT_STATE_DEFAULT);

/* should be big enough for even a batched full-palette response to OSC [ 4 ; 0 ; ? ; ... ; 15 ; ? BEL */
#define INPUT_PARAM_SIZE 4096

RAWCURSES_GLOBAL0(char rawcurses_input_param[INPUT_PARAM_SIZE]);
RAWCURSES_GLOBAL0(char rawcurses_input_intermed[INPUT_PARAM_SIZE]);
RAWCURSES_GLOBAL(attr_t rawcurses_old_attr_rgb, = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
RAWCURSES_GLOBAL0(char rawcurses_stdio_old_shortname_buf[INPUT_PARAM_SIZE]);
RAWCURSES_GLOBAL0(char rawcurses_stdio_old_title_buf[INPUT_PARAM_SIZE]);
RAWCURSES_GLOBAL(char *rawcurses_stdio_old_shortname, = NULL);
RAWCURSES_GLOBAL(char *rawcurses_stdio_old_title, = NULL);
RAWCURSES_GLOBAL(char *rawcurses_stdio_new_shortname, = NULL);
RAWCURSES_GLOBAL(char *rawcurses_stdio_new_title, = NULL);

#if USE_WINCONSOLE
RAWCURSES_GLOBAL0(HANDLE rawcurses_stdin);
RAWCURSES_GLOBAL0(HANDLE rawcurses_stdout);
RAWCURSES_GLOBAL0(HANDLE rawcurses_stderr);
RAWCURSES_GLOBAL0(DWORD rawcurses_old_mode);
RAWCURSES_GLOBAL(int rawcurses_old_x, = -1);
RAWCURSES_GLOBAL(int rawcurses_old_y, = -1);
RAWCURSES_GLOBAL(int rawcurses_old_mode_valid, = 0);
RAWCURSES_GLOBAL0(CONSOLE_CURSOR_INFO rawcurses_old_cursorinfo);
RAWCURSES_GLOBAL0(CONSOLE_CURSOR_INFO rawcurses_cursorinfo);
RAWCURSES_GLOBAL0(CONSOLE_SCREEN_BUFFER_INFO rawcurses_csbi);
RAWCURSES_GLOBAL0(SMALL_RECT rawcurses_old_size);
RAWCURSES_GLOBAL0(TCHAR rawcurses_old_title[MAX_PATH]);
RAWCURSES_GLOBAL(CHAR_INFO *rawcurses_old_screen, = NULL);
#endif
#if USE_TERMIOS
RAWCURSES_GLOBAL0(struct termios rawcurses_tty);
RAWCURSES_GLOBAL0(struct termios rawcurses_old_tty);
#endif
#if USE_SGTTY
RAWCURSES_GLOBAL0(struct sgttyb rawcurses_sgtty);
RAWCURSES_GLOBAL0(struct sgttyb rawcurses_old_sgtty);
#endif

RAWCURSES_GLOBAL0(chtype rawcurses_pairs[COLOR_PAIRS]);

typedef struct {
    short r, g, b;
} rawcurses_rgb_t;

RAWCURSES_GLOBAL0(rawcurses_rgb_t rawcurses_palette[16]);
RAWCURSES_GLOBAL0(rawcurses_rgb_t rawcurses_old_palette[16]);
RAWCURSES_GLOBAL0(char rawcurses_reset_palette_buf[INPUT_PARAM_SIZE]);
RAWCURSES_GLOBAL(size_t rawcurses_reset_palette_buflen, = 0);
RAWCURSES_GLOBAL(int rawcurses_reset_palette_seqs, = 0);
RAWCURSES_GLOBAL(int rawcurses_palette_changed, = 0);
RAWCURSES_GLOBAL(int rawcurses_palette_ever_changed, = 0);

/* list of terminals for which we send Atari ST-type color escape sequences */
/* NOTE: these are prefix matches */
static const char *RAWCURSES_ST52LIKE =
    "atari" "\0"
    "et52" "\0"
    "st52" "\0"
    "stv52" "\0"
    "tw52" "\0"
    "wterm" "\0"
    "\0\0";

/* list of terminals for which we send VT52-type escape sequences */
static const char *RAWCURSES_VT52LIKE =
    "addsviewpoint" "\0"
    "addsvp60" "\0"
    "aepro" "\0"
    "alto-h19" "\0"
    "alto-heath" "\0"
    "altoh19" "\0"
    "altoheath" "\0"
    "apple-videx3" "\0"
    "atari" "\0"
    "att4420" "\0"
    "cci" "\0"
    "cci1" "\0"
    "decpro" "\0"
    "dmterm" "\0"
    "dwk" "\0"
    "dwk-vt" "\0"
    "elks" "\0"
    "elks-vt52" "\0"
    "et52" "\0"
    "gator-52" "\0"
    "gator-52t" "\0"
    "h-100" "\0"
    "h-100bw" "\0"
    "h100" "\0"
    "h100bw" "\0"
    "h19" "\0"
    "h19-b" "\0"
    "h19-bs" "\0"
    "h19-g" "\0"
    "h19-u" "\0"
    "h19-us" "\0"
    "h19b" "\0"
    "h19g" "\0"
    "h19k" "\0"
    "h19kermit" "\0"
    "heath" "\0"
    "heathkit" "\0"
    "htx11" "\0"
    "hz1552" "\0"
    "hz1552-rv" "\0"
    "i3101" "\0"
    "ibm3101" "\0"
    "ibm3161" "\0"
    "ibm3161-C" "\0"
    "ibm3162" "\0"
    "ibm3163" "\0"
    "ibmaed" "\0"
    "intertube" "\0"
    "iris40" "\0"
    "kermit" "\0"
    "kermit-am" "\0"
    "mime2a" "\0"
    "mime2a-v" "\0"
    "modgraph2" "\0"
    "msk227" "\0"
    "msk22714" "\0"
    "msk227am" "\0"
    "mskermit227" "\0"
    "mskermit22714" "\0"
    "mskermit227am" "\0"
    "ncr260vppp" "\0"
    "ncr7901" "\0"
    "p19" "\0"
    "pc-coherent" "\0"
    "pc-venix" "\0"
    "pckermit" "\0"
    "pckermit12" "\0"
    "pckermit120" "\0"
    "pcz19" "\0"
    "pro350" "\0"
    "qnx" "\0"
    "qnx4" "\0"
    "rca" "\0"
    "regent20" "\0"
    "regent25" "\0"
    "regent40" "\0"
    "regent40+" "\0"
    "regent60" "\0"
    "sc410" "\0"
    "sc415" "\0"
    "scanset" "\0"
    "scr"/* hmm... */"ewpoint" "\0"
    "sibo" "\0"
    "st52" "\0"
    "stv52" "\0"
    "stv52pc" "\0"
    "superbrain" "\0"
    "t10" "\0"
    "t1061" "\0"
    "t1061f" "\0"
    "t3800" "\0"
    "tek4107" "\0"
    "tek4109" "\0"
    "tek4207-s" "\0"
    "teleray" "\0"
    "ti931" "\0"
    "trs16" "\0"
    "trs2" "\0"
    "trs80II" "\0"
    "trsII" "\0"
    "tty4420" "\0"
    "tw52" "\0"
    "tw52-color" "\0"
    "tw52-m" "\0"
    "vapple" "\0"
    "venix" "\0"
    "vi200" "\0"
    "vi50" "\0"
    "vi500" "\0"
    "vi55" "\0"
    "viewpoint" "\0"
    "viewpoint60" "\0"
    "viewpoint90" "\0"
    "vp60" "\0"
    "vp90" "\0"
    "vt-61" "\0"
    "vt50h" "\0"
    "vt52" "\0"
    "vt61" "\0"
    "wsiris" "\0"
    "wterm" "\0"
    "wy60-316X" "\0"
    "wyse-vp" "\0"
    "wyse60-316X" "\0"
    "xterm-vt52" "\0"
    "z-100" "\0"
    "z-100bw" "\0"
    "z100" "\0"
    "z100bw" "\0"
    "z110" "\0"
    "z110bw" "\0"
    "z19" "\0"
    "z29" "\0"
    "z29b" "\0"
    "z8001" "\0"
    "zen8001" "\0"
    "zenith" "\0"
    "zenith29" "\0"
    "zt-1" "\0"
    "ztx" "\0"
    "ztx-1-a" "\0"
    "ztx11" "\0"
    "\0\0";

/* list of terminals for which we assume an H19-style alternate
 * character set */
static const char *RAWCURSES_H19LIKE =
    "alto-h19" "\0"
    "alto-heath" "\0"
    "altoh19" "\0"
    "altoheath" "\0"
    "h-100" "\0"
    "h-100bw" "\0"
    "h100" "\0"
    "h100bw" "\0"
    "h19" "\0"
    "h19-a" "\0"
    "h19-b" "\0"
    "h19-bs" "\0"
    "h19-g" "\0"
    "h19-u" "\0"
    "h19-us" "\0"
    "h19a" "\0"
    "h19b" "\0"
    "h19g" "\0"
    "h19k" "\0"
    "h19kermit" "\0"
    "h29a-kc-bc" "\0"
    "h29a-kc-uc" "\0"
    "h29a-nkc-bc" "\0"
    "h29a-nkc-uc" "\0"
    "heath" "\0"
    "heath-ansi" "\0"
    "heathkit" "\0"
    "heathkit-a" "\0"
    "p19" "\0"
    "z-100" "\0"
    "z-100bw" "\0"
    "z100" "\0"
    "z100bw" "\0"
    "z110" "\0"
    "z110bw" "\0"
    "z19" "\0"
    "z29" "\0"
    "z29a" "\0"
    "z29a-kc-bc" "\0"
    "z29a-kc-uc" "\0"
    "z29a-nkc-bc" "\0"
    "z29a-nkc-uc" "\0"
    "z29b" "\0"
    "z39-a" "\0"
    "z39a" "\0"
    "zenith" "\0"
    "zenith29" "\0"
    "zenith39-a" "\0"
    "zenith39-ansi" "\0"
    "\0\0";

/* list of terminals for which we assume an CP437-style alternate
 * character set */
static const char *RAWCURSES_CP437LIKE =
    "aixterm-m-old" "\0"
    "ansi" "\0"
    "ansi-color-2-emx" "\0"
    "ansi-color-3-emx" "\0"
    "ansi-emx" "\0"
    "ansi-m" "\0"
    "ansi-mono" "\0"
    "ansi.sys" "\0"
    "ansi.sys-old" "\0"
    "ansi.sysk" "\0"
    "ansi43m" "\0"
    "ansi80x25" "\0"
    "ansi80x25-mono" "\0"
    "ansi80x30" "\0"
    "ansi80x30-mono" "\0"
    "ansi80x43" "\0"
    "ansi80x43-mono" "\0"
    "ansi80x50" "\0"
    "ansi80x50-mono" "\0"
    "ansi80x60" "\0"
    "ansi80x60-mono" "\0"
    "ansil" "\0"
    "ansil-mono" "\0"
    "ansis" "\0"
    "ansis-mono" "\0"
    "ansisysk" "\0"
    "att605-pc" "\0"
    "avatar" "\0"
    "avatar0" "\0"
    "avatar0+" "\0"
    "avatar1" "\0"
    "bsdos-pc" "\0"
    "bsdos-pc-m" "\0"
    "bsdos-pc-mono" "\0"
    "bsdos-pc-nobold" "\0"
    "bsdos-ppc" "\0"
    "cons25" "\0"
    "cons25-m" "\0"
    "cons30" "\0"
    "cons30-m" "\0"
    "cons43" "\0"
    "cons43-m" "\0"
    "cons50" "\0"
    "cons50-m" "\0"
    "cons60" "\0"
    "cons60-m" "\0"
    "cygwin" "\0"
    "cygwinB19" "\0"
    "cygwinDBG" "\0"
    "djgpp" "\0"
    "ecma+sgr" "\0"
    "emx-base" "\0"
    "hft-c-old" "\0"
    "hp700" "\0"
    "hpansi" "\0"
    "ibm5151" "\0"
    "ibm5154" "\0"
    "ibm6153" "\0"
    "ibm6153-40" "\0"
    "ibm6153-90" "\0"
    "ibm6154" "\0"
    "ibm6155" "\0"
    "ibmpc" "\0"
    "ibmpc3" "\0"
    "ibmpcx" "\0"
    "ibmx" "\0"
    "interix" "\0"
    "interix-nti" "\0"
    "klone+acs" "\0"
    "klone+sgr" "\0"
    "klone+sgr-dumb" "\0"
    "lft" "\0"
    "LFT-PC850" "\0"
    "lft-pc850" "\0"
    "linux" "\0"
    "linux-basic" "\0"
    "linux-c" "\0"
    "linux-c-nc" "\0"
    "linux-koi8r" "\0"
    "linux-m" "\0"
    "linux-nic" "\0"
    "ms-vt100" "\0"
    "ms-vt100+" "\0"
    "ms-vt100-color" "\0"
    "nansi.sys" "\0"
    "nansi.sysk" "\0"
    "nansisys" "\0"
    "nansisysk" "\0"
    "ntconsole" "\0"
    "ntconsole-100" "\0"
    "ntconsole-100-nti" "\0"
    "ntconsole-25" "\0"
    "ntconsole-25-nti" "\0"
    "ntconsole-25-w" "\0"
    "ntconsole-25-w-vt" "\0"
    "ntconsole-35" "\0"
    "ntconsole-35-nti" "\0"
    "ntconsole-35-w" "\0"
    "ntconsole-50" "\0"
    "ntconsole-50-nti" "\0"
    "ntconsole-50-w" "\0"
    "ntconsole-60" "\0"
    "ntconsole-60-nti" "\0"
    "ntconsole-60-w" "\0"
    "ntconsole-w" "\0"
    "ntconsole-w-vt" "\0"
    "opennt" "\0"
    "opennt-100" "\0"
    "opennt-100-nti" "\0"
    "opennt-25" "\0"
    "opennt-25-nti" "\0"
    "opennt-25-w" "\0"
    "opennt-25-w-vt" "\0"
    "opennt-35" "\0"
    "opennt-35-nti" "\0"
    "opennt-35-w" "\0"
    "opennt-50" "\0"
    "opennt-50-nti" "\0"
    "opennt-50-w" "\0"
    "opennt-60" "\0"
    "opennt-60-nti" "\0"
    "opennt-60-w" "\0"
    "opennt-nti" "\0"
    "opennt-w" "\0"
    "opennt-w-vt" "\0"
    "origibmpc3" "\0"
    "origpc3" "\0"
    "pc-minix" "\0"
    "pc3" "\0"
    "pc3-bold" "\0"
    "pcansi" "\0"
    "pcansi-25" "\0"
    "pcansi-25-m" "\0"
    "pcansi-33" "\0"
    "pcansi-33-m" "\0"
    "pcansi-43" "\0"
    "pcansi-43-m" "\0"
    "pcansi-m" "\0"
    "pcansi-mono" "\0"
    "pcansi25" "\0"
    "pcansi25m" "\0"
    "pcansi33" "\0"
    "pcansi33m" "\0"
    "pcansi43" "\0"
    "qnx" "\0"
    "qnx4" "\0"
    "qnxm" "\0"
    "qnxt" "\0"
    "qnxt2" "\0"
    "qnxt4" "\0"
    "qnxtmono" "\0"
    "qnxw" "\0"
    "rxvt-cygwin" "\0"
    "rxvt-cygwin-native" "\0"
    "screen.teraterm" "\0"
    "teraterm" "\0"
    "uwin" "\0"
    "vt100+" "\0"
    "vtnt" "\0"
    "wy60-AT" "\0"
    "wy60-PC" "\0"
    "wyse60-AT" "\0"
    "wyse60-PC" "\0"
    "xenix" "\0"
    "\0\0";

/* list of terminals for which we assume Amiga-style quirks */
static const char *RAWCURSES_AMIGALIKE =
    "amiga" "\0"
    "morphos" "\0"
    "\0\0";

/* list of terminals for which we send ADM3A-type escape sequences */
static const char *RAWCURSES_ADM3ALIKE =
    "1178" "\0"
    "a210" "\0"
    "a80" "\0"
    "abm85" "\0"
    "abm85e" "\0"
    "abm85h" "\0"
    "abm85h-old" "\0"
    "adm1" "\0"
    "adm11" "\0"
    "adm1178" "\0"
    "adm12" "\0"
    "adm1a" "\0"
    "adm2" "\0"
    "adm20" "\0"
    "adm21" "\0"
    "adm22" "\0"
    "adm31" "\0"
    "adm31-old" "\0"
    "adm3a" "\0"
    "adm3a+" "\0"
    "adm42" "\0"
    "adm42-ns" "\0"
    "adm5" "\0"
    "alt4" "\0"
    "alt7" "\0"
    "alt7pc" "\0"
    "altos-4" "\0"
    "altos4" "\0"
    "altos7" "\0"
    "altos7pc" "\0"
    "ampex-232" "\0"
    "ampex175" "\0"
    "ampex175-b" "\0"
    "ampex210" "\0"
    "ampex232" "\0"
    "ampex232w" "\0"
    "ampex80" "\0"
    "apple-soroc" "\0"
    "basis" "\0"
    "d80" "\0"
    "dialogue" "\0"
    "dialogue80" "\0"
    "dumb" "\0"
    "f100" "\0"
    "f100-rv" "\0"
    "f110" "\0"
    "f110-14" "\0"
    "f110-14w" "\0"
    "f110-w" "\0"
    "f200" "\0"
    "f200-w" "\0"
    "f200vi" "\0"
    "f200vi-w" "\0"
    "falco" "\0"
    "falco-p" "\0"
    "freedom" "\0"
    "freedom-rv" "\0"
    "freedom100" "\0"
    "freedom110" "\0"
    "freedom200" "\0"
    "hp700-wy" "\0"
    "icl6402" "\0"
    "icl6404" "\0"
    "icl6404-w" "\0"
    "ims950" "\0"
    "ims950-b" "\0"
    "ims950-rv" "\0"
    "iq120" "\0"
    "iq140" "\0"
    "kaypro" "\0"
    "kaypro2" "\0"
    "kds6402" "\0"
    "kds7372" "\0"
    "kds7372-w" "\0"
    "kt7" "\0"
    "kt7ix" "\0"
    "ktm" "\0"
    "mime2a-s" "\0"
    "mime340" "\0"
    "mime3a" "\0"
    "mime3ax" "\0"
    "mm340" "\0"
    "mt-70" "\0"
    "mt70" "\0"
    "ncr160wy50+pp" "\0"
    "ncr160wy50+wpp" "\0"
    "ncr160wy60pp" "\0"
    "ncr160wy60wpp" "\0"
    "ncr260wy325pp" "\0"
    "ncr260wy325wpp" "\0"
    "ncr260wy350pp" "\0"
    "ncr260wy350wpp" "\0"
    "ncr260wy50+pp" "\0"
    "ncr260wy50+wpp" "\0"
    "ncr260wy60pp" "\0"
    "ncr260wy60wpp" "\0"
    "northstar" "\0"
    "o85h" "\0"
    "oabm85h" "\0"
    "opus3n1+" "\0"
    "osborne" "\0"
    "osborne-w" "\0"
    "osborne1" "\0"
    "osborne1-w" "\0"
    "osexec" "\0"
    "qdss" "\0"
    "qdcons" "\0"
    "qvt101" "\0"
    "qvt101+" "\0"
    "qvt101p" "\0"
    "qvt102" "\0"
    "qvt119" "\0"
    "qvt119+" "\0"
    "qvt119+-25" "\0"
    "qvt119+-25-w" "\0"
    "qvt119+-w" "\0"
    "qvt119-25-w" "\0"
    "qvt119-w" "\0"
    "qvt119p" "\0"
    "qvt119p-25" "\0"
    "qvt119p-25-w" "\0"
    "qvt119p-w" "\0"
    "soroc" "\0"
    "soroc120" "\0"
    "soroc140" "\0"
    "synertek" "\0"
    "synertek380" "\0"
    "tandem6510" "\0"
    "ts-1" "\0"
    "ts-1p" "\0"
    "ts1" "\0"
    "ts1p" "\0"
    "tvi803" "\0"
    "tvi9065" "\0"
    "tvi910" "\0"
    "tvi910+" "\0"
    "tvi912" "\0"
    "tvi912b" "\0"
    "tvi912b-2p" "\0"
    "tvi912b-2p-mc" "\0"
    "tvi912b-2p-p" "\0"
    "tvi912b-2p-unk" "\0"
    "tvi912b-mc" "\0"
    "tvi912b-p" "\0"
    "tvi912b-unk" "\0"
    "tvi912b-vb" "\0"
    "tvi912b-vb-mc" "\0"
    "tvi912b-vb-p" "\0"
    "tvi912b-vb-unk" "\0"
    "tvi912c" "\0"
    "tvi912c-2p" "\0"
    "tvi912c-2p-mc" "\0"
    "tvi912c-2p-p" "\0"
    "tvi912c-2p-unk" "\0"
    "tvi912c-mc" "\0"
    "tvi912c-p" "\0"
    "tvi912c-unk" "\0"
    "tvi912c-vb" "\0"
    "tvi912c-vb-mc" "\0"
    "tvi912c-vb-p" "\0"
    "tvi912c-vb-unk" "\0"
    "tvi914" "\0"
    "tvi920" "\0"
    "tvi920b" "\0"
    "tvi920b-2p" "\0"
    "tvi920b-2p-mc" "\0"
    "tvi920b-2p-p" "\0"
    "tvi920b-2p-unk" "\0"
    "tvi920b-mc" "\0"
    "tvi920b-p" "\0"
    "tvi920b-unk" "\0"
    "tvi920b-vb" "\0"
    "tvi920b-vb-mc" "\0"
    "tvi920b-vb-p" "\0"
    "tvi920b-vb-unk" "\0"
    "tvi920c" "\0"
    "tvi920c-2p" "\0"
    "tvi920c-2p-mc" "\0"
    "tvi920c-2p-p" "\0"
    "tvi920c-2p-unk" "\0"
    "tvi920c-mc" "\0"
    "tvi920c-p" "\0"
    "tvi920c-unk" "\0"
    "tvi920c-vb" "\0"
    "tvi920c-vb-mc" "\0"
    "tvi920c-vb-p" "\0"
    "tvi920c-vb-unk" "\0"
    "tvi921" "\0"
    "tvi924" "\0"
    "tvi925" "\0"
    "tvi925-hi" "\0"
    "tvi92B" "\0"
    "tvi92D" "\0"
    "tvi950" "\0"
    "tvi950-2p" "\0"
    "tvi950-4p" "\0"
    "tvi950-rv" "\0"
    "tvi950-rv-2p" "\0"
    "tvi950-rv-4p" "\0"
    "tvipt" "\0"
    "vi50adm" "\0"
    "vp3a+" "\0"
    "wy100" "\0"
    "wy100q" "\0"
    "wy120" "\0"
    "wy120-25" "\0"
    "wy120-25-w" "\0"
    "wy120-vb" "\0"
    "wy120-w" "\0"
    "wy120-w-vb" "\0"
    "wy120-wvb" "\0"
    "wy150" "\0"
    "wy150-25" "\0"
    "wy150-25-w" "\0"
    "wy150-vb" "\0"
    "wy150-w" "\0"
    "wy150-w-vb" "\0"
    "wy150-wvb" "\0"
    "wy160" "\0"
    "wy160-25" "\0"
    "wy160-25-w" "\0"
    "wy160-42" "\0"
    "wy160-42-w" "\0"
    "wy160-43" "\0"
    "wy160-43-w" "\0"
    "wy160-vb" "\0"
    "wy160-w" "\0"
    "wy160-w-vb" "\0"
    "wy160-wvb" "\0"
    "wy30" "\0"
    "wy30-mc" "\0"
    "wy30-vb" "\0"
    "wy325" "\0"
    "wy325-25" "\0"
    "wy325-25w" "\0"
    "wy325-42" "\0"
    "wy325-42w" "\0"
    "wy325-42w-vb" "\0"
    "wy325-42wvb" "\0"
    "wy325-43" "\0"
    "wy325-43w" "\0"
    "wy325-43w-vb" "\0"
    "wy325-43wvb" "\0"
    "wy325-80" "\0"
    "wy325-vb" "\0"
    "wy325-w" "\0"
    "wy325-w-vb" "\0"
    "wy325-wvb" "\0"
    "wy325w-24" "\0"
    "wy350" "\0"
    "wy350-vb" "\0"
    "wy350-w" "\0"
    "wy350-wvb" "\0"
    "wy50" "\0"
    "wy50-mc" "\0"
    "wy50-vb" "\0"
    "wy50-w" "\0"
    "wy50-wvb" "\0"
    "wy60" "\0"
    "wy60-25" "\0"
    "wy60-25-w" "\0"
    "wy60-42" "\0"
    "wy60-42-w" "\0"
    "wy60-43" "\0"
    "wy60-43-w" "\0"
    "wy60-vb" "\0"
    "wy60-w" "\0"
    "wy60-w-vb" "\0"
    "wy60-wvb" "\0"
    "wy99f" "\0"
    "wy99fa" "\0"
    "wy99gt" "\0"
    "wy99gt-25" "\0"
    "wy99gt-25-w" "\0"
    "wy99gt-vb" "\0"
    "wy99gt-w" "\0"
    "wy99gt-w-vb" "\0"
    "wy99gt-wvb" "\0"
    "wyse-325" "\0"
    "wyse120" "\0"
    "wyse120-25" "\0"
    "wyse120-25-w" "\0"
    "wyse120-vb" "\0"
    "wyse120-w" "\0"
    "wyse120-w-vb" "\0"
    "wyse120-wvb" "\0"
    "wyse150" "\0"
    "wyse150-25" "\0"
    "wyse150-25-w" "\0"
    "wyse150-vb" "\0"
    "wyse150-w" "\0"
    "wyse150-w-vb" "\0"
    "wyse150-wvb" "\0"
    "wyse160" "\0"
    "wyse160-25" "\0"
    "wyse160-25-w" "\0"
    "wyse160-42" "\0"
    "wyse160-42-w" "\0"
    "wyse160-43" "\0"
    "wyse160-43-w" "\0"
    "wyse160-vb" "\0"
    "wyse160-w" "\0"
    "wyse160-w-vb" "\0"
    "wyse160-wvb" "\0"
    "wyse30" "\0"
    "wyse30-mc" "\0"
    "wyse30-vb" "\0"
    "wyse325" "\0"
    "wyse325-25" "\0"
    "wyse325-25w" "\0"
    "wyse325-42" "\0"
    "wyse325-42w" "\0"
    "wyse325-42w-vb" "\0"
    "wyse325-42wvb" "\0"
    "wyse325-43" "\0"
    "wyse325-43w" "\0"
    "wyse325-43w-vb" "\0"
    "wyse325-43wvb" "\0"
    "wyse325-80" "\0"
    "wyse325-vb" "\0"
    "wyse325-w" "\0"
    "wyse325-w-vb" "\0"
    "wyse325-wvb" "\0"
    "wyse325w-24" "\0"
    "wyse350" "\0"
    "wyse350-vb" "\0"
    "wyse350-w" "\0"
    "wyse350-wvb" "\0"
    "wyse50" "\0"
    "wyse50-mc" "\0"
    "wyse50-vb" "\0"
    "wyse50-w" "\0"
    "wyse50-wvb" "\0"
    "wyse60" "\0"
    "wyse60-25" "\0"
    "wyse60-25-w" "\0"
    "wyse60-42" "\0"
    "wyse60-42-w" "\0"
    "wyse60-43" "\0"
    "wyse60-43-w" "\0"
    "wyse60-vb" "\0"
    "wyse60-w" "\0"
    "wyse60-w-vb" "\0"
    "wyse60-wvb" "\0"
    "wyse99f" "\0"
    "wyse99fa" "\0"
    "wyse99gt" "\0"
    "wyse99gt-25" "\0"
    "wyse99gt-25-w" "\0"
    "wyse99gt-vb" "\0"
    "wyse99gt-w" "\0"
    "wyse99gt-w-vb" "\0"
    "wyse99gt-wvb" "\0"
    "x820" "\0"
    "xerox820" "\0"
    "z30" "\0"
    "z50" "\0"
    "zen30" "\0"
    "zen50" "\0"
    "\0\0";

/* list of ANSI/XTerm-like terminals for which we do not trust the
 * cursor-hiding capability */
static const char *RAWCURSES_NOCIVIS =
    "cxterm" "\0"
    "cygwin" "\0"
    "cygwinB19" "\0"
    "cygwinDBG" "\0"
    "darwin" "\0"
    "darwin-100x37" "\0"
    "darwin-100x37-m" "\0"
    "darwin-112x37" "\0"
    "darwin-112x37-m" "\0"
    "darwin-128x40" "\0"
    "darwin-128x40-m" "\0"
    "darwin-128x48" "\0"
    "darwin-128x48-m" "\0"
    "darwin-144x48" "\0"
    "darwin-144x48-m" "\0"
    "darwin-160x64" "\0"
    "darwin-160x64-m" "\0"
    "darwin-200x64" "\0"
    "darwin-200x64-m" "\0"
    "darwin-200x75" "\0"
    "darwin-200x75-m" "\0"
    "darwin-256x96" "\0"
    "darwin-256x96-m" "\0"
    "darwin-80x25" "\0"
    "darwin-80x25-m" "\0"
    "darwin-80x30" "\0"
    "darwin-80x30-m" "\0"
    "darwin-90x30" "\0"
    "darwin-90x30-m" "\0"
    "darwin-acs" "\0"
    "darwin-b" "\0"
    "darwin-b-acs" "\0"
    "darwin-f" "\0"
    "darwin-f-acs" "\0"
    "darwin-f2" "\0"
    "darwin-f2-acs" "\0"
    "darwin-m" "\0"
    "darwin-m-acs" "\0"
    "darwin-m-b" "\0"
    "darwin-m-b-acs" "\0"
    "darwin-m-f" "\0"
    "darwin-m-f-acs" "\0"
    "darwin-m-f2" "\0"
    "darwin-m-f2-acs" "\0"
    "eterm" "\0"
    "eterm-color" "\0"
    "kterm" "\0"
    "rxvt-cygwin" "\0"
    "rxvt-cygwin-native" "\0"
    "uuterm" "\0"
    "xnuppc" "\0"
    "xnuppc-100x37" "\0"
    "xnuppc-100x37-m" "\0"
    "xnuppc-112x37" "\0"
    "xnuppc-112x37-m" "\0"
    "xnuppc-128x40" "\0"
    "xnuppc-128x40-m" "\0"
    "xnuppc-128x48" "\0"
    "xnuppc-128x48-m" "\0"
    "xnuppc-144x48" "\0"
    "xnuppc-144x48-m" "\0"
    "xnuppc-160x64" "\0"
    "xnuppc-160x64-m" "\0"
    "xnuppc-200x64" "\0"
    "xnuppc-200x64-m" "\0"
    "xnuppc-200x75" "\0"
    "xnuppc-200x75-m" "\0"
    "xnuppc-256x96" "\0"
    "xnuppc-256x96-m" "\0"
    "xnuppc-80x25" "\0"
    "xnuppc-80x25-m" "\0"
    "xnuppc-80x30" "\0"
    "xnuppc-80x30-m" "\0"
    "xnuppc-90x30" "\0"
    "xnuppc-90x30-m" "\0"
    "xnuppc-acs" "\0"
    "xnuppc-b" "\0"
    "xnuppc-b-acs" "\0"
    "xnuppc-f" "\0"
    "xnuppc-f-acs" "\0"
    "xnuppc-f2" "\0"
    "xnuppc-f2-acs" "\0"
    "xnuppc-m" "\0"
    "xnuppc-m-acs" "\0"
    "xnuppc-m-b" "\0"
    "xnuppc-m-b-acs" "\0"
    "xnuppc-m-f" "\0"
    "xnuppc-m-f-acs" "\0"
    "xnuppc-m-f2" "\0"
    "xnuppc-m-f2-acs" "\0"
    "\0\0";

/* The rawcurses_fput_* family return >0 on success, <=0 otherwise;
 * they are somewhat analogous to terminfo but much less flexible. */

static int rawcurses_fput_ich(FILE *fh, int n)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        rawcurses_y = wherey() - 1;
        rawcurses_x = wherex() - 1;
#ifdef __TINYC__
        {
            int i;

            for (i = rawcurses_w - 1; i >= (rawcurses_x + n); i --)
            {
                char buf[4];

                _conio_gettext(i + 1 - n, rawcurses_y + 1,
                               i + 1 - n, rawcurses_y + 1,
                               buf);
                puttext(i + 1, rawcurses_y + 1,
                        i + 1, rawcurses_y + 1,
                        buf);
            }
        }
#else /* ! defined(__TINYC__) */
        movetext(rawcurses_x + 1 + n, rawcurses_y + 1,
                 rawcurses_w, rawcurses_y + 1,
                 rawcurses_x + 1, rawcurses_y + 1);
#endif /* ! defined(__TINYC__) */
        return 1;
    }
#endif /* USE_CONIO */
    if (rawcurses_stdio_adm3a || rawcurses_stdio_vt52) return 0;
    return (fprintf(fh, CSI("%d@"), n) > 0);
}

static int rawcurses_fput_request_palette(FILE *fh)
{
    int i;
    int ret = 1;

#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 0;
    }
#endif
    if (rawcurses_stdio_adm3a || rawcurses_stdio_vt52) return 0;
    if (! rawcurses_stdio_ccc) return 0;
    if (fputs(OSC("4"), fh) == EOF) ret = 0;
    for (i = 0; i < COLORS; i ++)
    {
        if (fprintf(fh, ";%d;?", i) <= 0) ret = 0;
    }
    if (fputs("\7", fh) == EOF) ret = 0;
    return ret;
}

static int rawcurses_fput_palette(FILE *fh, rawcurses_rgb_t *palette)
{
    int i;
    int ret = 1;

#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 0;
    }
#endif
    if (rawcurses_stdio_adm3a || rawcurses_stdio_vt52) return 0;
    if (! (rawcurses_stdio_ccc || rawcurses_stdio_ccc_linux)) return (rawcurses_stdio_256color || rawcurses_stdio_88color);
    if ((! rawcurses_stdio_ccc_linux) && (fputs(OSC("4"), fh) == EOF)) ret = 0;
    for (i = 0; i < COLORS; i ++)
    {
        if (fprintf(fh,
                    (rawcurses_stdio_ccc_linux ? (OSC("P%x%2.2x%2.2x%2.2x") ST) : ";%d;#%2.2x%2.2x%2.2x"),
                    i, 255 * palette[i].r / 1000, 255 * palette[i].g / 1000, 255 * palette[i].b / 1000) <= 0)
        {
            ret = 0;
        }
    }
    if ((! rawcurses_stdio_ccc_linux) && (fputs("\7", fh) == EOF)) ret = 0;
    return ret;
}

static int rawcurses_fput_palette_reset(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 0;
    }
#endif
    if (rawcurses_stdio_adm3a || rawcurses_stdio_vt52) return 0;
    if (! rawcurses_stdio_ccc_linux) return (rawcurses_stdio_256color || rawcurses_stdio_88color);
    return fputs(OSC("R") ST, fh) != EOF;
}

static int rawcurses_fput_clear(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        clrscr();
        gotoxy(1, 1);
        return 1;
    }
#endif
    if (rawcurses_stdio_adm3a) return fputs(ESCAPE(";") ESCAPE("*") ESCAPE("+") "\x1a", fh) != EOF;
    if (rawcurses_stdio_vt52) return fputs("\f" ESCAPE("H") ESCAPE("J"), fh) != EOF;
    if (rawcurses_stdio_amiga) return fputs(CSI("H") CSI("J"), fh) != EOF;
    return fputs(CSI("1;1H") CSI("2J"), fh) != EOF;
}

static int rawcurses_fput_civis(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
#ifdef _NOCURSOR
        _setcursortype(_NOCURSOR);
        return 1;
#else
        return 0;
#endif
    }
#endif
    if (rawcurses_stdio_st52) return fputs(ESCAPE("f"), fh) != EOF;
    if (rawcurses_stdio_vt52 || rawcurses_stdio_adm3a) return 0;
    return fputs(CSI("\?25l"), fh) != EOF;
}

static int rawcurses_fput_cnorm(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
#ifdef _NORMALCURSOR
        _setcursortype(_NORMALCURSOR);
        return 1;
#else
        return 0;
#endif
    }
#endif
    if (rawcurses_stdio_st52) return fputs(ESCAPE("e"), fh) != EOF;
    if (rawcurses_stdio_vt52 || rawcurses_stdio_adm3a || rawcurses_stdio_amiga) return 0;
    return fputs(CSI("\?25h"), fh) != EOF;
}

static int rawcurses_fput_smcup(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        clrscr();
        gotoxy(1, 1);
        return 1;
    }
#endif
    if (rawcurses_stdio_vt52 || rawcurses_stdio_adm3a || rawcurses_stdio_amiga) return rawcurses_fput_clear(fh);
    return (fprintf(fh, ESCAPE("7") CSI("s") "%s" CSI("\?7l") CSI("r"),
                    rawcurses_stdio_amiga ? "" : (CSI("\?47h"))) > 0);
}

static int rawcurses_fput_rmcup(FILE *fh, int lines)
{
    rawcurses_fput_clear(fh);
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 1;
    }
#endif
    if (rawcurses_stdio_vt52 || rawcurses_stdio_adm3a) return 1;
    return (fprintf(fh, CSI("%d;1H") CSI("39;49m") CSI("0m") CSI("\?7h") "%s" CSI("u") ESCAPE("8"),
                    lines,
                    rawcurses_stdio_amiga ? "" : (CSI("\?47l"))) > 0);
}

static int rawcurses_fput_enacs(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 1;
    }
#endif
    if (rawcurses_stdio_adm3a) return 0;
    if (rawcurses_stdio_vt52 || rawcurses_stdio_acs_h19 || rawcurses_stdio_cp437) return 1;
    if (! rawcurses_stdio_iso2022) return 1;
    return fputs(ESCAPE("(B") ESCAPE(")0"), fh) != EOF;
}

static int rawcurses_fput_smacs(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 1;
    }
#endif
    if (rawcurses_stdio_vt52) return fputs(ESCAPE("F"), fh) != EOF;
    if (rawcurses_stdio_cp437 && ! rawcurses_stdio_adm3a)
    {
        if (rawcurses_stdio_utf8) return 1;
        if (rawcurses_stdio_iso2022) fputs("\x0e" ESCAPE("(0"), fh);
        return (fputs(CSI("12m") CSI("11m"), fh) != EOF);
    }
    return fputc('\x0e', fh) != EOF;
}

static int rawcurses_fput_rmacs(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 1;
    }
#endif
    if (rawcurses_stdio_vt52) return fputs(ESCAPE("G"), fh) != EOF;
    if (rawcurses_stdio_cp437 && ! rawcurses_stdio_adm3a)
    {
        if (rawcurses_stdio_utf8) return 1;
        if (fputs(CSI("10m"), fh) != EOF)
        {
            if (rawcurses_stdio_iso2022) return fputs("\x0f" ESCAPE("(B"), fh) != EOF;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return fputc('\x0f', fh) != EOF;
}

#if 0 /* not used at the moment, but we do parse the responses in case you'd like to enable it */

static int rawcurses_fput_request_identity(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 0;
    }
#endif
    if (rawcurses_stdio_adm3a) return 0;
    return fputs(ESCAPE("Z"), fh) != EOF;
}

#endif

static int rawcurses_fput_request_winsize(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 0;
    }
#endif
    if (rawcurses_stdio_vt52 || rawcurses_stdio_adm3a) return 0;
    return fputs(CSI("999B") CSI("999C") CSI("999;999H") CSI("6n"), fh) != EOF;
}

static int rawcurses_fput_request_title(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 0;
    }
#endif
    if (rawcurses_stdio_vt52 || rawcurses_stdio_adm3a) return 0;
    return fputs(CSI("21t"), fh) != EOF;
}

static int rawcurses_fput_title(FILE *fh, const char *title)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 0;
    }
#endif
    if (rawcurses_stdio_vt52 && rawcurses_stdio_tw52)
    {
        return (fprintf(fh, ESCAPE("S%s\r"), (title)) > 0);
    }
    if (rawcurses_stdio_vt52 || rawcurses_stdio_adm3a) return 0;
    return (fprintf(fh, OSC("2;%s" ST), (title)) > 0);
}

static int rawcurses_fput_request_icon_name(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 0;
    }
#endif
    if (rawcurses_stdio_vt52 || rawcurses_stdio_adm3a) return 0;
    return fputs(CSI("20t"), fh) != EOF;
}

static int rawcurses_fput_icon_name(FILE *fh, const char *icon_name)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 0;
    }
#endif
    if (rawcurses_stdio_vt52 || rawcurses_stdio_adm3a) return 0;
    return (fprintf(fh, OSC("1;%s" ST), (icon_name)) > 0);
}

static int rawcurses_fput_bel(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        putch('\7');
        return 1;
    }
#endif
    return fputc('\7', fh) != EOF;
}

static int rawcurses_fput_clrtobot(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 0;
    }
#endif
    if (rawcurses_stdio_adm3a) return 0;
    if (rawcurses_stdio_vt52) return fputs(ESCAPE("J"), fh) != EOF;
    return fputs(CSI("0J"), fh) != EOF;
}

static int rawcurses_fput_clrtoeol(FILE *fh)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        clreol();
        return 1;
    }
#endif
    if (rawcurses_stdio_adm3a) return 0;
    if (rawcurses_stdio_vt52) return fputs(ESCAPE("K"), fh) != EOF;
    return fputs(CSI("0K"), fh) != EOF;
}

static int rawcurses_fput_relcup(FILE *fh, int dy, int dx)
{
    int ret;

#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        return 0;
    }
#endif
    ret = 1;
    if (rawcurses_stdio_vt52 || rawcurses_stdio_adm3a)
    {
        while (dy < 0)
        {
            if (rawcurses_stdio_adm3a) ret = ret && (fputc('\v', fh) != EOF);
            else ret = ret && (fputs(ESCAPE("A"), fh) != EOF);
            dy ++;
        }
        while (dx < 0)
        {
            if (rawcurses_stdio_adm3a) ret = ret && (fputc('\b', fh) != EOF);
            else ret = ret && (fputs(ESCAPE("D"), fh) != EOF);
            dx ++;
        }
        while (dy > 0)
        {
            if (rawcurses_stdio_adm3a) ret = ret && (fputc('\n', fh) != EOF);
            else ret = ret && (fputs(ESCAPE("B"), fh) != EOF);
            dy --;
        }
        while (dx > 0)
        {
            if (rawcurses_stdio_adm3a) ret = ret && (fputc('\f', fh) != EOF);
            else ret = ret && (fputs(ESCAPE("C"), fh) != EOF);
            dx --;
        }
    }
    else
    {
        if (dx < 0) ret = ret && (fprintf(fh, CSI("%dD"), -dx) > 0);
        else if (dx > 0) ret = ret && (fprintf(fh, CSI("%dC"), dx) > 0);
        if (dy < 0) ret = ret && (fprintf(fh, CSI("%dA"), -dy) > 0);
        else if (dy > 0) ret = ret && (fprintf(fh, CSI("%dB"), dy) > 0);
    }
    return ret;
}

static int rawcurses_fput_cup(FILE *fh, int y, int x)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        gotoxy(x + 1, y + 1);
        return 1;
    }
#endif
    if (rawcurses_stdio_vt52 || rawcurses_stdio_adm3a)
    {
        int yo, xo, ret;

        yo = 0;
        xo = 0;
        if (y > 0x5e)
        {
            yo = y - 0x5e;
            y = 0x5e;
        }
        if (x > 0x5e)
        {
            xo = x - 0x5e;
            x = 0x5e;
        }
        ret = (fprintf(fh, rawcurses_stdio_adm3a ? (ESCAPE("=%c%c")) : (ESCAPE("Y%c%c")), y + 0x20, x + 0x20) > 0);
        while (yo --)
        {
            if (rawcurses_stdio_adm3a) fputc('\n', fh);
            else fputs(ESCAPE("B"), fh);
        }
        while (xo --)
        {
            if (rawcurses_stdio_adm3a) fputc('\f', fh);
            else fputs(ESCAPE("C"), fh);
        }
        return ret;
    }
    return (fprintf(fh, CSI("%d;%dH"), (y) + 1, (x) + 1) > 0);
}

#undef can_change_color
#define can_change_color rawcurses_can_change_color
static int can_change_color(void);

static int rawcurses_fput_sgr(FILE *fh, attr_t a, attr_t a_rgb)
{
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        textcolor(a_rgb & (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY));
        textbackground((a_rgb & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)) / BACKGROUND_BLUE);
        return 1;
    }
#endif
    if (rawcurses_stdio_vt52)
    {
        if (rawcurses_stdio_tw52)
        {
            int toswinfg, toswinbg;
                
            toswinfg = (((a_rgb & FOREGROUND_RED) ? 1 : 0)
                        |
                        ((a_rgb & FOREGROUND_GREEN) ? 2 : 0)
                        |
                        ((a_rgb & FOREGROUND_BLUE) ? 4 : 0)
                        |
                        ((a_rgb & FOREGROUND_INTENSITY) ? 8 : 0));
            toswinbg = (((a_rgb & BACKGROUND_RED) ? 1 : 0)
                        |
                        ((a_rgb & BACKGROUND_GREEN) ? 2 : 0)
                        |
                        ((a_rgb & BACKGROUND_BLUE) ? 4 : 0)
                        |
                        ((a_rgb & BACKGROUND_INTENSITY) ? 8 : 0));
            fprintf(fh, ESCAPE("u") ESCAPE("z_"));
            if (a & A_UNDERLINE)
            {
                fprintf(fh, ESCAPE("yH"));
            }
            if (a & RAWCURSES__A_COLOR)
            {
                if (toswinbg > 7)
                {
                    int tmp;

                    tmp = toswinbg;
                    toswinbg = toswinfg;
                    toswinfg = tmp;
                    fprintf(fh, ESCAPE("p"));
                }
                fprintf(fh,
                        ESCAPE("3%c") ESCAPE("4%c"),
                        '0' + ((toswinfg == 8) ? 7 : (toswinfg % 8)),
                        '0' + ((toswinbg == 8) ? 7 : (toswinbg % 8)));
            }
            else
            {
                if (a & (A_STANDOUT | A_REVERSE))
                {
                    fprintf(fh, ESCAPE("p"));
                }
            }
            if ((toswinfg == 8) || (a & A_DIM))
            {
                fprintf(fh, ESCAPE("3~"));
            }
            if ((toswinfg > 8) || (a & A_BOLD))
            {
                fprintf(fh, ESCAPE("3}"));
            }
            return 1;
        }
        if (rawcurses_stdio_st52
            &&
            (! (a & RAWCURSES__A_COLOR)))
        {
            a_rgb = ((BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
                     |
                     ((a & A_UNDERLINE)
                      ? ((a & A_BOLD)
                         ? FOREGROUND_RED
                         : (FOREGROUND_RED | FOREGROUND_BLUE))
                      : ((a & A_BOLD)
                         ? (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
                         : 0)));
            if (a & (A_STANDOUT | A_REVERSE))
            {
                attr_t tmp;

                tmp = 0;
                if (a_rgb & FOREGROUND_RED) tmp |= BACKGROUND_RED;
                if (a_rgb & FOREGROUND_GREEN) tmp |= BACKGROUND_GREEN;
                if (a_rgb & FOREGROUND_BLUE) tmp |= BACKGROUND_BLUE;
                if (a_rgb & FOREGROUND_INTENSITY) tmp |= BACKGROUND_INTENSITY;
                if (a_rgb & BACKGROUND_RED) tmp |= FOREGROUND_RED;
                if (a_rgb & BACKGROUND_GREEN) tmp |= FOREGROUND_GREEN;
                if (a_rgb & BACKGROUND_BLUE) tmp |= FOREGROUND_BLUE;
                if (a_rgb & BACKGROUND_INTENSITY) tmp |= FOREGROUND_INTENSITY;
                a_rgb = tmp;
            }
        }
        /* FIXME: we should not assume that all VT52-like terminals
         * necessarily accept these H-19 sequences for standout (or
         * silently ignore them, which would be fine.) */
        if ((a & (A_STANDOUT | A_REVERSE))
            ||
            ((a_rgb & ((rawcurses_stdio_st52
                        ? 0
                        : BACKGROUND_INTENSITY)
                       |
                       BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE))
             !=
             (rawcurses_stdio_st52
              ? (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
              : 0)))
        {
            attr_t tmp;

            fputs(ESCAPE("p"), fh);
            tmp = 0;
            if (a_rgb & FOREGROUND_RED) tmp |= BACKGROUND_RED;
            if (a_rgb & FOREGROUND_GREEN) tmp |= BACKGROUND_GREEN;
            if (a_rgb & FOREGROUND_BLUE) tmp |= BACKGROUND_BLUE;
            if (a_rgb & FOREGROUND_INTENSITY) tmp |= BACKGROUND_INTENSITY;
            if (a_rgb & BACKGROUND_RED) tmp |= FOREGROUND_RED;
            if (a_rgb & BACKGROUND_GREEN) tmp |= FOREGROUND_GREEN;
            if (a_rgb & BACKGROUND_BLUE) tmp |= FOREGROUND_BLUE;
            if (a_rgb & BACKGROUND_INTENSITY) tmp |= FOREGROUND_INTENSITY;
            a_rgb = tmp;
        }
        else
        {
            fputs(ESCAPE("q"), fh);
        }
        if (rawcurses_stdio_st52)
        {
            int atarifg, ataribg;

            atarifg = (((a_rgb & FOREGROUND_RED) ? 1 : 0)
                      |
                      ((a_rgb & FOREGROUND_GREEN) ? 2 : 0)
                      |
                      ((a_rgb & FOREGROUND_BLUE) ? 4 : 0)
                      |
                      ((a_rgb & FOREGROUND_INTENSITY) ? 8 : 0));
            ataribg = (((a_rgb & BACKGROUND_RED) ? 1 : 0)
                      |
                      ((a_rgb & BACKGROUND_GREEN) ? 2 : 0)
                      |
                      ((a_rgb & BACKGROUND_BLUE) ? 4 : 0)
                      |
                      ((a_rgb & BACKGROUND_INTENSITY) ? 8 : 0));
            if (atarifg == 0) atarifg = 15;
            else if (atarifg == 15) atarifg = 0;
            if (ataribg == 0) ataribg = 15;
            else if (ataribg == 15) ataribg = 0;
            fprintf(fh,
                    ESCAPE("b%c") ESCAPE("c%c"),
                    '0' + atarifg,
                    '0' + ataribg);
        }
        return 1;
    }
    if (rawcurses_stdio_adm3a) return 0;
    fputs(CSI("0"), fh);
    if ((a_rgb & BACKGROUND_INTENSITY)
        &&
        (! (a & (A_REVERSE | A_STANDOUT)))
        &&
        (COLORS == 8)
        &&
        (! rawcurses_stdio_nocolorbold))
    {
        attr_t tmp;

        tmp = 0;
        if (a_rgb & FOREGROUND_RED) tmp |= BACKGROUND_RED;
        if (a_rgb & FOREGROUND_GREEN) tmp |= BACKGROUND_GREEN;
        if (a_rgb & FOREGROUND_BLUE) tmp |= BACKGROUND_BLUE;
        if (a_rgb & FOREGROUND_INTENSITY) tmp |= BACKGROUND_INTENSITY;
        if (a_rgb & BACKGROUND_RED) tmp |= FOREGROUND_RED;
        if (a_rgb & BACKGROUND_GREEN) tmp |= FOREGROUND_GREEN;
        if (a_rgb & BACKGROUND_BLUE) tmp |= FOREGROUND_BLUE;
        if (a_rgb & BACKGROUND_INTENSITY) tmp |= FOREGROUND_INTENSITY;
        a_rgb = tmp;
        fputs(";7", fh);
    }
    if ((a & A_BOLD) || ((a_rgb & FOREGROUND_INTENSITY) && ! rawcurses_stdio_nocolorbold)) fputs(";1", fh);
    if ((a_rgb & BACKGROUND_INTENSITY) && rawcurses_stdio_blink) fputs(";5", fh);
    if (a & A_DIM) fputs(";2", fh);
    if (a & A_UNDERLINE) fputs(";4", fh);
    if (a & RAWCURSES__A_COLOR)
    {
        int ansifg, ansibg;

        ansifg = (((a_rgb & FOREGROUND_RED) ? 1 : 0)
                  |
                  ((a_rgb & FOREGROUND_GREEN) ? 2 : 0)
                  |
                  ((a_rgb & FOREGROUND_BLUE) ? 4 : 0)
                  |
                  ((a_rgb & FOREGROUND_INTENSITY) ? 8 : 0));
        ansibg = (((a_rgb & BACKGROUND_RED) ? 1 : 0)
                  |
                  ((a_rgb & BACKGROUND_GREEN) ? 2 : 0)
                  |
                  ((a_rgb & BACKGROUND_BLUE) ? 4 : 0)
                  |
                  ((a_rgb & BACKGROUND_INTENSITY) ? 8 : 0));
        if ((rawcurses_stdio_256color || rawcurses_stdio_88color)
            &&
            (! (rawcurses_stdio_ccc || rawcurses_stdio_ccc_linux)))
        {
            int fg, bg;
            int rgbscale;
            int grayscale;
            rawcurses_rgb_t fg_rgb, bg_rgb;

            rgbscale = rawcurses_stdio_256color ? 6 : 4;
            grayscale = rawcurses_stdio_256color ? 26 : 10;
            fg_rgb.r = mille_to_scale(rawcurses_palette[ansifg].r, rgbscale);
            fg_rgb.g = mille_to_scale(rawcurses_palette[ansifg].g, rgbscale);
            fg_rgb.b = mille_to_scale(rawcurses_palette[ansifg].b, rgbscale);
            if (fg_rgb.r
                &&
                (fg_rgb.r < (rgbscale - 1))
                &&
                (fg_rgb.r
                 ==
                 fg_rgb.g)
                &&
                (fg_rgb.r
                 ==
                 fg_rgb.b)
                &&
                ((((long) fg_rgb.r) * (grayscale - 1) / (rgbscale - 1)) != (long) mille_to_scale(rawcurses_palette[ansifg].r, grayscale)))
            {
                fg = 16 + rgbscale * rgbscale * rgbscale + mille_to_scale(299L * rawcurses_palette[ansifg].r / 1000 + 587L * rawcurses_palette[ansifg].g / 1000 + 114L * rawcurses_palette[ansifg].b / 1000, grayscale) - 1;
            }
            else
            {
                fg = 16 + rgbscale * (rgbscale * fg_rgb.r + fg_rgb.g) + fg_rgb.b;
            }
            bg_rgb.r = mille_to_scale(rawcurses_palette[ansibg].r, rgbscale);
            bg_rgb.g = mille_to_scale(rawcurses_palette[ansibg].g, rgbscale);
            bg_rgb.b = mille_to_scale(rawcurses_palette[ansibg].b, rgbscale);
            if (bg_rgb.r
                &&
                (bg_rgb.r < (rgbscale - 1))
                &&
                (bg_rgb.r
                 ==
                 bg_rgb.g)
                &&
                (bg_rgb.r
                 ==
                 bg_rgb.b)
                &&
                ((((long) bg_rgb.r) * (grayscale - 1) / (rgbscale - 1)) != (long) mille_to_scale(rawcurses_palette[ansibg].r, grayscale)))
            {
                bg = 16 + rgbscale * rgbscale * rgbscale + mille_to_scale(299L * rawcurses_palette[ansibg].r / 1000 + 587L * rawcurses_palette[ansibg].g / 1000 + 114L * rawcurses_palette[ansibg].b / 1000, grayscale) - 1;
            }
            else
            {
                bg = 16 + rgbscale * (rgbscale * bg_rgb.r + bg_rgb.g) + bg_rgb.b;
            }
            fprintf(fh, "m" CSI("38;5;%dm") CSI("48;5;%d"), fg, bg);
        }
        else
        {
            fprintf(fh, ";%d;%d",
                    30 + ansifg % 8,
                    40 + ansibg % 8);
            if (rawcurses_16color)
            {
                if (a_rgb & FOREGROUND_INTENSITY)
                {
                    fprintf(fh, ";%d",
                            90 + ansifg % 8);
                }
                if (a_rgb & BACKGROUND_INTENSITY)
                {
                    fprintf(fh, ";%d",
                            100 + ansibg % 8);
                }
            }
        }
    }
    else
    {
        /*if (a & (A_REVERSE | A_STANDOUT)) fputs(";3", fh);*/
        if (a & (A_REVERSE | A_STANDOUT)) fputs(";7", fh);
    }
    return fputc('m', fh) != EOF;
}

/* only one attribute is allowed here for now: A_REVERSE, which is
 * XOR'ed with the A_REVERSE bit in the active attribute mask */
static chtype rawcurses_map_acs(chtype ch)
{
    if (rawcurses_stdio_cp437)
    {
        switch (ch)
        {
        case ACS_BLOCK:
            ch = 0xdb;
            break;
        case RAWCURSES__ACS_THALF:
            ch = 0xdf;
            break;
        case RAWCURSES__ACS_BHALF:
            ch = 0xdc;
            break;
        case RAWCURSES__ACS_LHALF:
            ch = 0xdd;
            break;
        case RAWCURSES__ACS_RHALF:
            ch = 0xde;
            break;
        case ACS_LRCORNER:
            ch = 217;
            break;
        case ACS_URCORNER:
            ch = 191;
            break;
        case ACS_ULCORNER:
            ch = 218;
            break;
        case ACS_LLCORNER:
            ch = 192;
            break;
        case ACS_PLUS:
            ch = 197;
            break;
        case ACS_HLINE:
            ch = 196;
            break;
        case ACS_LTEE:
            ch = 195;
            break;
        case ACS_RTEE:
            ch = 180;
            break;
        case ACS_BTEE:
            ch = 193;
            break;
        case ACS_TTEE:
            ch = 194;
            break;
        case ACS_VLINE:
            ch = 179;
            break;
        default:
            if (ch == (chtype) ACS_BULLET)
            {
                ch = 254;
                break;
            }
            ch = (chtype) ERR;
        }
        return ch;
    }
    if (rawcurses_stdio_acs_h19)
    {
        switch (ch)
        {
        case ACS_BLOCK:
            ch = ' ' | A_REVERSE;
            break;
        case RAWCURSES__ACS_THALF:
            ch = 'o';
            break;
        case RAWCURSES__ACS_BHALF:
            ch = 'o' | A_REVERSE;
            break;
        case RAWCURSES__ACS_LHALF:
            ch = 'p' | A_REVERSE;
            break;
        case RAWCURSES__ACS_RHALF:
            ch = 'p';
            break;
        case ACS_LRCORNER:
            ch = 'd';
            break;
        case ACS_URCORNER:
            ch = 'c';
            break;
        case ACS_ULCORNER:
            ch = 'f';
            break;
        case ACS_LLCORNER:
            ch = 'e';
            break;
        case ACS_PLUS:
            ch = 'b';
            break;
        case ACS_HLINE:
            ch = 'a';
            break;
        case ACS_LTEE:
            ch = 'v';
            break;
        case ACS_RTEE:
            ch = 't';
            break;
        case ACS_BTEE:
            ch = 'u';
            break;
        case ACS_TTEE:
            ch = 's';
            break;
        case ACS_VLINE:
            ch = '`';
            break;
        default:
            if ((ch == (chtype) ACS_BULLET) && ! (SWAPDOTS))
            {
                ch = '^';
                break;
            }
            ch = (chtype) ERR;
        }
        return ch;
    }
    switch (ch)
    {
    case ACS_LRCORNER:
        ch = 'j';
        break;
    case ACS_URCORNER:
        ch = 'k';
        break;
    case ACS_ULCORNER:
        ch = 'l';
        break;
    case ACS_LLCORNER:
        ch = 'm';
        break;
    case ACS_PLUS:
        ch = 'n';
        break;
    case ACS_HLINE:
        ch = 'q';
        break;
    case ACS_LTEE:
        ch = 't';
        break;
    case ACS_RTEE:
        ch = 'u';
        break;
    case ACS_BTEE:
        ch = 'v';
        break;
    case ACS_TTEE:
        ch = 'w';
        break;
    case ACS_VLINE:
        ch = 'x';
        break;
    default:
        if (ch == (chtype) ACS_BULLET)
        {
            ch = '~';
            break;
        }
        ch = (chtype) ERR;
    }
    return ch;
}

#if USE_WINCONSOLE

/* Information on this undocumented console configuration message is
 * from http://www.catch22.net/source/files/setconsoleinfo.c */

/* UNDOCUMENTED: this is called WM_SETCONSOLEINFO in the catch22.net
 * code */
#define RAWCURSES_WM_SETCONSOLEINFO (WM_USER + 201)

#ifdef __GNUC__
#ifndef PACKED
#define PACKED  __attribute__ ((packed))
#endif
#endif

#pragma pack(push, 1)

/* UNDOCUMENTED: this is called struct _CONSOLE_INFO in the
 * catch22.net code */
typedef struct
{
    ULONG cbSize;
    COORD dwScreenBufferSize;
    COORD dwWindowSize;
    DWORD dwWindowLeft;
    DWORD dwWindowTop;
    COORD dwFontSize;
    DWORD dwFontFamily;
    DWORD dwFontWeight;
    WCHAR szFaceName[LF_FACESIZE];
    DWORD dwCursorSize;
    BOOL bFullScreen;
    BOOL bQuickEdit;
    DWORD dwFlags;
    BOOL bInsertMode;
    WORD wScreenColors;
    WORD wPopupColors;
    BOOL bHistoryNoDup;
    DWORD dwHistoryBufferSize;
    DWORD dwNumberOfHistoryBuffers;
    COLORREF ColorTable[16];
    DWORD dwCodePageID;
    HWND hConsoleWindow;
    WCHAR uConsoleTitle[256];
}
#ifdef __GNUC__
 PACKED
#endif
 rawcurses_win32_console_info_t;

#pragma pack(pop)

#pragma pack(push, 1)

/* this is called CONSOLE_SCREEN_BUFFER_INFOEX in official docs */
typedef struct {
    ULONG cbSize;
    COORD dwSize;
    COORD dwCursorPosition;
    WORD wAttributes;
    SMALL_RECT srWindow;
    COORD dwMaximumWindowSize;
    WORD wPopupAttributes;
    BOOL bFullscreenSupported;
    COLORREF ColorTable[16];
}
#ifdef __GNUC__
 PACKED
#endif
 rawcurses_win32_console_screen_buffer_info_ex_t;

#pragma pack(pop)

/* typedefs for dynamically resolved functions */

/* from kernel32 */
typedef HANDLE WINAPI (*rawcurses_kernel32_CreateToolhelp32Snapshot_t)(DWORD, DWORD);
typedef HWND WINAPI (*rawcurses_kernel32_GetConsoleWindow_t)(void);
typedef BOOL WINAPI (*rawcurses_kernel32_GetConsoleScreenBufferInfoEx_t)(HANDLE, rawcurses_win32_console_screen_buffer_info_ex_t *);
typedef COORD WINAPI (*rawcurses_kernel32_GetConsoleFontSize_t)(HANDLE, DWORD);

#if ! (defined(__WINE__) || defined(__DMC__) || defined(__TINYC__) || defined(__BORLANDC__))
/* these do not exist at all in Wine */
typedef BOOL WINAPI (*rawcurses_kernel32_GetCurrentConsoleFont_t)(HANDLE, BOOL, PCONSOLE_FONT_INFO);
/* UNDOCUMENTED: this allows setting the console font index for a
 * given console window */
typedef BOOL WINAPI (*rawcurses_kernel32_SetConsoleFont_t)(HANDLE, DWORD);
#endif /* ! (defined(__WINE__) || defined(__DMC__) || defined(__TINYC__) || defined(__BORLANDC__)) */

typedef BOOL WINAPI (*rawcurses_kernel32_SetConsoleScreenBufferInfoEx_t)(HANDLE, rawcurses_win32_console_screen_buffer_info_ex_t *);
typedef BOOL WINAPI (*rawcurses_kernel32_QueryFullProcessImageNameW_t)(HANDLE, DWORD, LPWSTR, PDWORD);
typedef BOOL WINAPI (*rawcurses_kernel32_QueryFullProcessImageNameA_t)(HANDLE, DWORD, LPSTR, PDWORD);

/* from psapi */
typedef DWORD WINAPI (*rawcurses_psapi_GetModuleFileNameExW_t)(HANDLE, HMODULE, LPWSTR, DWORD);
typedef DWORD WINAPI (*rawcurses_psapi_GetModuleFileNameExA_t)(HANDLE, HMODULE, LPSTR, DWORD);

/* from shlwapi */
#ifndef WINSHLWAPI
#define WINSHLWAPI
#endif
typedef WINSHLWAPI BOOL WINAPI (*rawcurses_shlwapi_PathUnExpandEnvStringsW_t)(LPCWSTR, LPWSTR, UINT);
typedef WINSHLWAPI BOOL WINAPI (*rawcurses_shlwapi_PathUnExpandEnvStringsA_t)(LPCSTR, LPSTR, UINT);

/* helper function to find the console window handle so we can send a
 * message */
static HWND rawcurses_win32_get_console_window(void)
{
    HWND ret = 0;
    HMODULE kernel32;

    kernel32 = LoadLibraryA("kernel32");
    if (kernel32)
    {
        rawcurses_kernel32_GetConsoleWindow_t rawcurses_GetConsoleWindow;

        rawcurses_GetConsoleWindow = (rawcurses_kernel32_GetConsoleWindow_t)(GetProcAddress(kernel32, "GetConsoleWindow"));
        if (rawcurses_GetConsoleWindow)
        {
            ret = rawcurses_GetConsoleWindow();
        }
    }
    if (! ret)
    {
        TCHAR title[MAX_PATH + 1];

        memset((void *) title, 0, sizeof(title));
        if (GetConsoleTitle(title, sizeof(title) / sizeof(*title) - 1))
        {
            char newtitle[MAX_PATH + 1];
            LARGE_INTEGER count;

            sprintf(newtitle, "tty%lu", (unsigned long) GetCurrentProcessId());
            if (QueryPerformanceCounter(&count))
            {
                sprintf(newtitle + strlen(newtitle), "_%lu_%lu", (unsigned long) count.u.LowPart, (unsigned long) count.u.HighPart);
            }
            SetConsoleTitleA(newtitle);
            usleep(RAWCURSES_TITLE_QUIESCE_USECS);
            ret = FindWindowA(NULL, newtitle);
            SetConsoleTitle(title);
            usleep(RAWCURSES_TITLE_QUIESCE_USECS);
            if (! ret) ret = FindWindow(NULL, title);
        }
    }
    if (kernel32) FreeLibrary(kernel32);
    return ret;
}

#ifdef PROCESS_QUERY_LIMITED_INFORMATION
#define RAWCURSES_PROCESS_QUERY_LIMITED_INFORMATION PROCESS_QUERY_LIMITED_INFORMATION
#else
#define RAWCURSES_PROCESS_QUERY_LIMITED_INFORMATION 0x1000
#endif

static int rawcurses_win32_console_reg_copy(const WCHAR *name, DWORD type, void *out, DWORD *size)
{
    HKEY keys[2];
    int i;
    int read;
    HANDLE hConsoleWindow;
    TCHAR texenamebuf[MAX_PATH + 1];
    TCHAR *texename;
    TCHAR tpathbuf[MAX_PATH + 1];
    TCHAR *tpath;
    char *exename;
    char pathbuf[MAX_PATH + 1];
    char *path;
    HMODULE kernel32;
    HMODULE psapi;
    HMODULE shlwapi;
    rawcurses_shlwapi_PathUnExpandEnvStringsW_t rawcurses_PathUnExpandEnvStringsW;
    rawcurses_shlwapi_PathUnExpandEnvStringsA_t rawcurses_PathUnExpandEnvStringsA;

#ifdef UNICODE
#define rawcurses_PathUnExpandEnvStrings rawcurses_PathUnExpandEnvStringsW
#else
#define rawcurses_PathUnExpandEnvStrings rawcurses_PathUnExpandEnvStringsA
#endif

    kernel32 = LoadLibraryA("kernel32");
    psapi = LoadLibraryA("psapi");
    shlwapi = LoadLibraryA("shlwapi");
    rawcurses_PathUnExpandEnvStringsW = NULL;
    rawcurses_PathUnExpandEnvStringsA = NULL;
    if (shlwapi)
    {
        rawcurses_PathUnExpandEnvStringsW = (rawcurses_shlwapi_PathUnExpandEnvStringsW_t) GetProcAddress(shlwapi, "PathUnExpandEnvStringsW");
        rawcurses_PathUnExpandEnvStringsA = (rawcurses_shlwapi_PathUnExpandEnvStringsA_t) GetProcAddress(shlwapi, "PathUnExpandEnvStringsA");
    }
    hConsoleWindow = rawcurses_win32_get_console_window();
    read = 0;
    for (i = 0; i < sizeof(keys) / sizeof(*keys); i ++) keys[i] = INVALID_HANDLE_VALUE;
    path = pathbuf;
    tpath = tpathbuf;
    if (lstrlen(rawcurses_old_title))
    {
        lstrcpy(tpath, TEXT("Console\\"));
        if ((lstrlen(tpath) + lstrlen(rawcurses_old_title)) <= MAX_PATH)
        {
            TCHAR *ts;
            TCHAR *ts2;

            ts2 = rawcurses_old_title;
            for (ts = tpath + lstrlen(tpath); *ts2; (ts ++), (ts2 ++))
            {
                *ts = ((*ts2 == (TEXT("\\"))[0]) ? (TEXT("_"))[0] : *ts2);
            }
            *ts = (TEXT(""))[0];
            if (keys[0] == INVALID_HANDLE_VALUE)
            {
                if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, tpath, 0, KEY_READ, keys + 0))
                {
                    keys[0] = INVALID_HANDLE_VALUE;
                }
            }
        }
    }
    /* FIXME: need to figure out what character this should actually be */
#define RAWCURSES_WIN32_CAPTION_DELIMITER TEXT(" - ")
    texename = GetCommandLine();
    if (texename)
    {
        if (rawcurses_old_title
            &&
            lstrlen(rawcurses_old_title)
            &&
            (lstrlen(rawcurses_old_title) > (lstrlen(RAWCURSES_WIN32_CAPTION_DELIMITER) + lstrlen(texename)))
            &&
            (! lstrcmp(rawcurses_old_title + lstrlen(rawcurses_old_title) - lstrlen(texename), texename))
            &&
            (rawcurses_old_title[lstrlen(rawcurses_old_title) - lstrlen(texename) - 1] == (RAWCURSES_WIN32_CAPTION_DELIMITER)[lstrlen(RAWCURSES_WIN32_CAPTION_DELIMITER) - 1])
            &&
            (rawcurses_old_title[lstrlen(rawcurses_old_title) - lstrlen(texename) - lstrlen(RAWCURSES_WIN32_CAPTION_DELIMITER)] == (RAWCURSES_WIN32_CAPTION_DELIMITER)[0])
            &&
            tpath)
        {
            tpath[lstrlen(tpath) - lstrlen(texename) - lstrlen(RAWCURSES_WIN32_CAPTION_DELIMITER)] = (TEXT(""))[0];
            if (keys[0] == INVALID_HANDLE_VALUE)
            {
                if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, tpath, 0, KEY_READ, keys + 0))
                {
                    keys[0] = INVALID_HANDLE_VALUE;
                }
            }
        }
    }
    texename = NULL;
    exename = NULL;
    if ((hConsoleWindow != INVALID_HANDLE_VALUE)
        &&
        hConsoleWindow)
    {
        DWORD tty_pid;

        if (GetWindowThreadProcessId(hConsoleWindow, &tty_pid))
        {
            HANDLE hConsoleProcess;

            hConsoleProcess = OpenProcess(RAWCURSES_PROCESS_QUERY_LIMITED_INFORMATION, FALSE, tty_pid);
            if ((hConsoleProcess != INVALID_HANDLE_VALUE)
                &&
                hConsoleProcess)
            {
                if (kernel32)
                {
                    rawcurses_kernel32_QueryFullProcessImageNameW_t rawcurses_QueryFullProcessImageNameW;
                    rawcurses_kernel32_QueryFullProcessImageNameA_t rawcurses_QueryFullProcessImageNameA;
                    DWORD imagenamesize;

#ifdef UNICODE
#define rawcurses_QueryFullProcessImageName rawcurses_QueryFullProcessImageNameW
#else
#define rawcurses_QueryFullProcessImageName rawcurses_QueryFullProcessImageNameA
#endif

                    rawcurses_QueryFullProcessImageNameW = (rawcurses_kernel32_QueryFullProcessImageNameW_t) GetProcAddress(kernel32, "QueryFullProcessImageNameW");
                    rawcurses_QueryFullProcessImageNameA = (rawcurses_kernel32_QueryFullProcessImageNameA_t) GetProcAddress(kernel32, "QueryFullProcessImageNameA");
                    texename = texenamebuf;
                    imagenamesize = MAX_PATH;
                    if (rawcurses_QueryFullProcessImageName
                        &&
                        rawcurses_QueryFullProcessImageName(hConsoleProcess, 0, texename, &imagenamesize)
                        &&
                        imagenamesize)
                    {
                        texename[MAX_PATH] = (TEXT(""))[0];
                        if (! lstrlen(texename)) texename = NULL;
                    }
                    else
                    {
                        texename = NULL;
                    }
                }
                CloseHandle(hConsoleProcess);
            }
            hConsoleProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, tty_pid);
            if ((hConsoleProcess != INVALID_HANDLE_VALUE)
                &&
                hConsoleProcess)
            {
                if (psapi)
                {
                    rawcurses_psapi_GetModuleFileNameExW_t rawcurses_GetModuleFileNameExW;
                    rawcurses_psapi_GetModuleFileNameExA_t rawcurses_GetModuleFileNameExA;

#ifdef UNICODE
#define rawcurses_GetModuleFileNameEx rawcurses_GetModuleFileNameExW
#else
#define rawcurses_GetModuleFileNameEx rawcurses_GetModuleFileNameExA
#endif

                    rawcurses_GetModuleFileNameExW = (rawcurses_psapi_GetModuleFileNameExW_t) GetProcAddress(psapi, "GetModuleFileNameExW");
                    rawcurses_GetModuleFileNameExA = (rawcurses_psapi_GetModuleFileNameExA_t) GetProcAddress(psapi, "GetModuleFileNameExA");
                    if (rawcurses_GetModuleFileNameEx
                        &&
                        (! texename))
                    {
                        texename = texenamebuf;
                        if (rawcurses_GetModuleFileNameEx(hConsoleProcess, NULL, texename, MAX_PATH) > 0)
                        {
                            texename[MAX_PATH] = (TEXT(""))[0];
                            if (! lstrlen(texename)) texename = NULL;
                        }
                        else
                        {
                            texename = NULL;
                        }
                    }
                }
#ifndef __TINYC__
                if (kernel32)
                {
                    rawcurses_kernel32_CreateToolhelp32Snapshot_t rawcurses_CreateToolhelp32Snapshot;

                    rawcurses_CreateToolhelp32Snapshot = (rawcurses_kernel32_CreateToolhelp32Snapshot_t) GetProcAddress(kernel32, "CreateToolhelp32Snapshot");
                    if (rawcurses_CreateToolhelp32Snapshot
                        &&
                        (! texename))
                    {
                        HANDLE hSnapshot;

                        hSnapshot = rawcurses_CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, tty_pid);
                        if ((hSnapshot != INVALID_HANDLE_VALUE)
                            &&
                            hSnapshot)
                        {
                            MODULEENTRY32 me;

                            me.dwSize = sizeof(me);
                            if (Module32First(hSnapshot, &me))
                            {
                                texename = texenamebuf;
                                memcpy((void *) texename, (void *) me.szExePath,
                                       sizeof(me.szExePath) / sizeof(*(me.szExePath)));
                                texename[MAX_PATH] = (TEXT(""))[0];
                                if (! lstrlen(texename)) texename = NULL;
                            }
                            CloseHandle(hSnapshot);
                        }
                    }
                }
#endif /* ! defined(__TINYC__) */
                CloseHandle(hConsoleProcess);
            }
#if ! (defined(__WINE__) || defined(__WATCOMC__) || defined(__BORLANDC__))
            if (tty_pid == GetCurrentProcessId())
            {
                exename = _pgmptr;
            }
#endif /* ! (defined(__WINE__) || defined(__WATCOMC__) || defined(__BORLANDC__)) */
        }
    }
    if (texename && lstrlen(texename))
    {
        TCHAR texenamebuf2[MAX_PATH + 1];
        TCHAR *texename2;

        texename2 = texenamebuf2;
        *texename2 = 0;
        if (rawcurses_PathUnExpandEnvStrings
            &&
            rawcurses_PathUnExpandEnvStrings(texename, texename2, MAX_PATH))
        {
            texename = texename2;
        }
        lstrcpy(tpath, TEXT("Console\\"));
        if ((lstrlen(tpath) + lstrlen(texename)) <= MAX_PATH)
        {
            TCHAR *ts;
            TCHAR *ts2;

            ts2 = texename;
            for (ts = tpath + lstrlen(tpath); *ts2; (ts ++), (ts2 ++))
            {
                *ts = ((*ts2 == (TEXT("\\"))[0]) ? (TEXT("_"))[0] : *ts2);
            }
            *ts = (TEXT(""))[0];
            if (keys[0] == INVALID_HANDLE_VALUE)
            {
                if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, tpath, 0, KEY_READ, keys + 0))
                {
                    keys[0] = INVALID_HANDLE_VALUE;
                }
            }
        }
    }
    if (exename && strlen(exename))
    {
        char exenamebuf2[MAX_PATH + 1];
        char *exename2;

        exename2 = exenamebuf2;
        *exename2 = 0;
        if (rawcurses_PathUnExpandEnvStringsA
            &&
            rawcurses_PathUnExpandEnvStringsA(exename, exename2, MAX_PATH))
        {
            exename = exename2;
        }
        strcpy(path, "Console\\");
        if ((strlen(path) + strlen(exename)) <= MAX_PATH)
        {
            char *s;
            char *s2;

            s2 = exename;
            for (s = path + strlen(path); *s2; (s ++), (s2 ++))
            {
                *s = ((*s2 == '\\') ? '_' : *s2);
            }
            *s = '\0';
            if (keys[0] == INVALID_HANDLE_VALUE)
            {
                if (ERROR_SUCCESS != RegOpenKeyExA(HKEY_CURRENT_USER, path, 0, KEY_READ, keys + 0))
                {
                    keys[0] = INVALID_HANDLE_VALUE;
                }
            }
        }
    }
    strcpy(path, "Console");
    if (keys[1] == INVALID_HANDLE_VALUE)
    {
        if (ERROR_SUCCESS != RegOpenKeyExA(HKEY_CURRENT_USER, path, 0, KEY_READ, keys + 1))
        {
            keys[1] = INVALID_HANDLE_VALUE;
        }
    }
    for (i = 0; i < sizeof(keys) / sizeof(*keys); i ++)
    {
        if (keys[i] != INVALID_HANDLE_VALUE)
        {
            DWORD otype;
            DWORD osize;

            osize = *size;
            if ((RegQueryValueExW(keys[i], name, 0, &otype, NULL, &osize) == ERROR_SUCCESS)
                &&
                (otype == type)
                &&
                (osize >= 0)
                &&
                (osize <= *size)
                &&
                /* FIXME: race condition: if type changes here, we lose a bit... oh well! */
                (RegQueryValueExW(keys[i], name, 0, &otype, out, &osize) == ERROR_SUCCESS))
            {
                *size = osize;
                read = 1;
                break;
            }
        }
    }
    for (i = 0; i < sizeof(keys) / sizeof(*keys); i ++)
    {
        if (keys[i] != INVALID_HANDLE_VALUE) RegCloseKey(keys[i]);
    }
    if ((hConsoleWindow != INVALID_HANDLE_VALUE)
        &&
        hConsoleWindow)
    {
        CloseHandle(hConsoleWindow);
    }
    if (shlwapi)
    {
        FreeLibrary(shlwapi);
    }
    if (psapi)
    {
        FreeLibrary(psapi);
    }
    if (kernel32)
    {
        FreeLibrary(kernel32);
    }
    return read;
}

static DWORD rawcurses_win32_console_reg_get_dword(const WCHAR *name, DWORD defvalue)
{
    DWORD out;
    DWORD size;

    size = sizeof(out);
    if ((! rawcurses_win32_console_reg_copy(name, REG_DWORD, (void *) &out, &size))
        ||
        (size != sizeof(out)))
    {
        out = defvalue;
    }
    return out;
}

/* copy up to outlen WCHARs into out either from a REG_SZ console
 * registry entry corresponding to name or from defvalue; NULL
 * defvalue is equivalent to L""; returns out on success, NULL
 * otherwise */
static WCHAR *rawcurses_win32_console_reg_copy_sz(const WCHAR *name, const WCHAR *defvalue, WCHAR *out, size_t outlen)
{
    DWORD size;
    size_t deflen;

    deflen = defvalue ? wcslen(defvalue) : 0;
    if ((! out) || (! outlen)) return NULL;
    if (defvalue && (deflen > outlen)) return NULL;
    size = outlen * sizeof(*out);
    if (! rawcurses_win32_console_reg_copy(name, REG_SZ, (void *) out, &size))
    {
        if (out != defvalue)
        {
            memset((void *) out, 0, outlen * sizeof(*out));
            if (defvalue)
            {
                memmove((void *) out, (void *) defvalue, deflen * sizeof(*defvalue));
            }
        }
    }
    return out;
}

static int rawcurses_win32_get_palette(rawcurses_rgb_t *palette)
{
    int i;
    HMODULE kernel32;
    COLORREF ColorTable[16];

    kernel32 = LoadLibraryA("kernel32");
    for (i = 0; i < 16; i ++)
    {
        WCHAR buf[MAX_PATH];

        swprintf(buf,
#if ! (defined(__MSVCRT__) || defined(__BORLANDC__))
                 MAX_PATH - 1,
#endif /* ! (defined(__MSVCRT__) || defined(__BORLANDC__)) */
                 L"ColorTable%2.2d", i);
        if (i == 6) ColorTable[i] = RGB(126, 63, 0);
        else ColorTable[i] = RGB(((i & 4) ? 126 : 0) + ((i & 8) ? 63 : 0), \
                                 ((i & 2) ? 126 : 0) + ((i & 8) ? 63 : 0), \
                                 ((i & 1) ? 126 : 0) + ((i & 8) ? 63 : 0));
        ColorTable[i] = rawcurses_win32_console_reg_get_dword(buf, ColorTable[i]);
    }
    if (kernel32)
    {
        rawcurses_win32_console_screen_buffer_info_ex_t csbiex;
        rawcurses_kernel32_GetConsoleScreenBufferInfoEx_t rawcurses_GetConsoleScreenBufferInfoEx;

        csbiex.cbSize = sizeof(csbiex);
        rawcurses_GetConsoleScreenBufferInfoEx = (rawcurses_kernel32_GetConsoleScreenBufferInfoEx_t) GetProcAddress(kernel32, "GetConsoleScreenBufferInfoEx");
        if (rawcurses_GetConsoleScreenBufferInfoEx
            &&
            rawcurses_GetConsoleScreenBufferInfoEx(rawcurses_stdout, &csbiex))
        {
            memcpy((void *) ColorTable, (void *) &(csbiex.ColorTable), sizeof(ColorTable));
        }
    }
    for (i = 0; i < 16; i ++)
    {
        int ansicolor;

        ansicolor = (((i & FOREGROUND_RED) ? 1 : 0)
                     |
                     ((i & FOREGROUND_GREEN) ? 2 : 0)
                     |
                     ((i & FOREGROUND_BLUE) ? 4 : 0)
                     |
                     ((i & FOREGROUND_INTENSITY) ? 8 : 0));
        palette[ansicolor].r = (short) ((long) GetRValue(ColorTable[i]) * 1000 / 255);
        palette[ansicolor].g = (short) ((long) GetGValue(ColorTable[i]) * 1000 / 255);
        palette[ansicolor].b = (short) ((long) GetBValue(ColorTable[i]) * 1000 / 255);
    }
    if (kernel32) FreeLibrary(kernel32);
    return 1;
}

static int rawcurses_win32_set_palette(rawcurses_rgb_t *palette)
{
    HMODULE kernel32;
    int ret = 0;

    kernel32 = LoadLibraryA("kernel32");
    if (kernel32)
    {
        rawcurses_win32_console_screen_buffer_info_ex_t csbiex;
        rawcurses_kernel32_GetConsoleScreenBufferInfoEx_t rawcurses_GetConsoleScreenBufferInfoEx;
        rawcurses_kernel32_SetConsoleScreenBufferInfoEx_t rawcurses_SetConsoleScreenBufferInfoEx;

        csbiex.cbSize = sizeof(csbiex);
        rawcurses_GetConsoleScreenBufferInfoEx = (rawcurses_kernel32_GetConsoleScreenBufferInfoEx_t) GetProcAddress(kernel32, "GetConsoleScreenBufferInfoEx");
        rawcurses_SetConsoleScreenBufferInfoEx = (rawcurses_kernel32_SetConsoleScreenBufferInfoEx_t) GetProcAddress(kernel32, "SetConsoleScreenBufferInfoEx");
        if (rawcurses_GetConsoleScreenBufferInfoEx
            &&
            rawcurses_SetConsoleScreenBufferInfoEx
            &&
            rawcurses_GetConsoleScreenBufferInfoEx(rawcurses_stdout, &csbiex))
        {
            int i;

            for (i = 0; i < 16; i ++)
            {
                int ansicolor;

                ansicolor = (((i & FOREGROUND_RED) ? 1 : 0)
                             |
                             ((i & FOREGROUND_GREEN) ? 2 : 0)
                             |
                             ((i & FOREGROUND_BLUE) ? 4 : 0)
                             |
                             ((i & FOREGROUND_INTENSITY) ? 8 : 0));
                csbiex.ColorTable[i] = RGB((palette[ansicolor].r * 255 / 1000),
                                           (palette[ansicolor].g * 255 / 1000),
                                           (palette[ansicolor].b * 255 / 1000));
            }
            if (rawcurses_SetConsoleScreenBufferInfoEx(rawcurses_stdout, &csbiex))
            {
                ret = 1;
            }
        }
    }
    if (! ret)
    {
        rawcurses_win32_console_info_t cc;
#if ! (defined(__WINE__) || defined(__DMC__) || defined(__TINYC__) || defined(__BORLANDC__))
        CONSOLE_FONT_INFO cfi;

        cfi.nFont = (DWORD) -1;
        cfi.dwFontSize.X = 0;
        cfi.dwFontSize.Y = 0;
#endif /* ! (defined(__WINE__) || defined(__DMC__) || defined(__TINYC__) || defined(__BORLANDC__)) */
        if (GetConsoleScreenBufferInfo(rawcurses_stdout, &rawcurses_csbi))
        {
            int i;

            cc.cbSize = sizeof(cc);
            cc.dwScreenBufferSize = rawcurses_csbi.dwSize;
            cc.dwWindowSize.X = rawcurses_csbi.srWindow.Right - rawcurses_csbi.srWindow.Left + 1;
            cc.dwWindowSize.Y = rawcurses_csbi.srWindow.Bottom - rawcurses_csbi.srWindow.Top + 1;
            cc.dwWindowLeft = rawcurses_csbi.srWindow.Left;
            cc.dwWindowTop = rawcurses_csbi.srWindow.Top;
            cc.dwFontFamily = rawcurses_win32_console_reg_get_dword(L"FontFamily", FF_DONTCARE);
            cc.dwFontWeight = rawcurses_win32_console_reg_get_dword(L"FontWeight", FW_DONTCARE);
            *((DWORD *) (void *) &(cc.dwFontSize)) = rawcurses_win32_console_reg_get_dword(L"FontSize", MAKELONG(0, 0));
            rawcurses_win32_console_reg_copy_sz(L"FaceName", L"", cc.szFaceName, sizeof(cc.szFaceName) / sizeof(*(cc.szFaceName)));
            cc.dwCursorSize = rawcurses_cursorinfo.dwSize;
            cc.bFullScreen = rawcurses_win32_console_reg_get_dword(L"FullScreen", FALSE);
            cc.hConsoleWindow = rawcurses_win32_get_console_window();
            if ((cc.hConsoleWindow != INVALID_HANDLE_VALUE)
                &&
                cc.hConsoleWindow)
            {
                RECT geometry;

                if (GetWindowRect(cc.hConsoleWindow, &geometry))
                {
                    if (((geometry.top <= 0)
                         &&
                         (geometry.left <= 0)
                         &&
                         (geometry.bottom >= GetSystemMetrics(SM_CYSCREEN))
                         &&
                         (geometry.right >= GetSystemMetrics(SM_CXSCREEN)))
                        ||
                        /* who'd have thought it? fullscreen console
                         * apps have very strange geometries
                         * apparently far offscreen and the window
                         * dimensions are small and nonsensical (thay
                         * match neither pixel nor text dimensions
                         * known to other calls.) */
                        ((geometry.top == -32000L)
                         &&
                         (geometry.left == -32000L)))
                    {
                        cc.bFullScreen = TRUE;
                    }
                    else
                    {
                        cc.bFullScreen = FALSE;
                    }
                }
            }
            cc.bQuickEdit = rawcurses_win32_console_reg_get_dword(L"QuickEdit", TRUE);
            /* 0x10000 AUTO_POSITION (?) */
            cc.dwFlags = (rawcurses_win32_console_reg_get_dword(L"WindowPositon", (DWORD) -1) == (DWORD) -1) ? 0x10000 : 1;
            cc.bInsertMode = rawcurses_win32_console_reg_get_dword(L"InsertMode", TRUE);
            cc.wScreenColors = rawcurses_win32_console_reg_get_dword(L"ScreenColors", rawcurses_old_attr_rgb);
            cc.wPopupColors = rawcurses_win32_console_reg_get_dword(L"PopupColors", FOREGROUND_RED | FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
            cc.bHistoryNoDup = rawcurses_win32_console_reg_get_dword(L"HistoryNoDup", FALSE);
            cc.dwHistoryBufferSize = rawcurses_win32_console_reg_get_dword(L"HistoryBufferSize", 50);
            cc.dwNumberOfHistoryBuffers = rawcurses_win32_console_reg_get_dword(L"NumberOfHistoryBuffers", 4);
            for (i = 0; i < 16; i ++)
            {
                int ansicolor;

                ansicolor = (((i & FOREGROUND_RED) ? 1 : 0)
                             |
                             ((i & FOREGROUND_GREEN) ? 2 : 0)
                             |
                             ((i & FOREGROUND_BLUE) ? 4 : 0)
                             |
                             ((i & FOREGROUND_INTENSITY) ? 8 : 0));
                cc.ColorTable[i] = RGB((palette[ansicolor].r * 255 / 1000),
                                       (palette[ansicolor].g * 255 / 1000),
                                       (palette[ansicolor].b * 255 / 1000));
            }
            cc.dwCodePageID = GetConsoleOutputCP();
            memset((void *) cc.uConsoleTitle, 0, sizeof(cc.uConsoleTitle));
            if ((cc.hConsoleWindow != INVALID_HANDLE_VALUE)
                &&
                cc.hConsoleWindow)
            {
                DWORD tty_pid;

                if (GetWindowThreadProcessId(cc.hConsoleWindow, &tty_pid))
                {
                    HANDLE hConsoleProcess;
                    HKEY hkWine;

                    if (ERROR_SUCCESS == RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Wine", 0, KEY_READ, &hkWine))
                    {
                        /*

                          NOP in Wine, otherwise we would crash due to
                          unimplemented GetCurrentConsoleFont.

                        */
                        RegCloseKey(hkWine);
                    }
#if ! (defined(__WINE__) || defined(__DMC__) || defined(__TINYC__) || defined(__BORLANDC__))
                    else if (kernel32)
                    {
                        rawcurses_kernel32_GetCurrentConsoleFont_t rawcurses_GetCurrentConsoleFont;
                        rawcurses_kernel32_GetConsoleFontSize_t rawcurses_GetConsoleFontSize;

                        rawcurses_GetCurrentConsoleFont = (rawcurses_kernel32_GetCurrentConsoleFont_t) GetProcAddress(kernel32, "GetCurrentConsoleFont");
                        if (rawcurses_GetCurrentConsoleFont
                            &&
                            (rawcurses_GetCurrentConsoleFont(rawcurses_stdout, FALSE, &cfi)))
                        {
                            rawcurses_GetConsoleFontSize = (rawcurses_kernel32_GetConsoleFontSize_t) GetProcAddress(kernel32, "GetConsoleFontSize");
                            if (rawcurses_GetConsoleFontSize)
                            {
                                cfi.dwFontSize = rawcurses_GetConsoleFontSize(rawcurses_stdout, cfi.nFont);
                            }
                            cc.dwFontSize = cfi.dwFontSize;
                        }
                        if (cfi.dwFontSize.Y
                            &&
                            (cc.dwFontFamily & VARIABLE_PITCH))
                        {
                            cc.dwFontSize.X = 0;
                        }
                    }
#endif /* ! (defined(__WINE__) || defined(__DMC__) || defined(__TINYC__) || defined(__BORLANDC__)) */
                    hConsoleProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tty_pid);
                    if ((hConsoleProcess != INVALID_HANDLE_VALUE)
                        &&
                        hConsoleProcess)
                    {
                        HANDLE mapping;

                        mapping = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(cc), 0);
                        if ((mapping != INVALID_HANDLE_VALUE)
                            &&
                            mapping)
                        {
                            void *foreign_cc;

                            foreign_cc = MapViewOfFile(mapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(cc));
                            if (foreign_cc)
                            {
                                HANDLE remote_mapping;

                                memcpy(foreign_cc, (void *) &cc, sizeof(cc));
                                UnmapViewOfFile(foreign_cc);
                                if (DuplicateHandle(GetCurrentProcess(), mapping,
                                                    hConsoleProcess, &remote_mapping,
                                                    0,
                                                    FALSE,
                                                    DUPLICATE_SAME_ACCESS))
                                {
                                    HANDLE local_mapping;

                                    if (SendMessage(cc.hConsoleWindow, RAWCURSES_WM_SETCONSOLEINFO, (WPARAM) remote_mapping, 0))
                                    {
                                        ret = 1;
                                    }
                                    /* HACK: fullscreen mode seems to need this twice, not sure why... */
                                    if (cc.bFullScreen == TRUE)
                                    {
                                        SendMessage(cc.hConsoleWindow, RAWCURSES_WM_SETCONSOLEINFO, (WPARAM) remote_mapping, 0);
                                    }
#if ! (defined(__WINE__) || defined(__DMC__) || defined(__TINYC__) || defined(__BORLANDC__))
                                    if (kernel32)
                                    {
                                        rawcurses_kernel32_SetConsoleFont_t rawcurses_SetConsoleFont;

                                        rawcurses_SetConsoleFont = (rawcurses_kernel32_SetConsoleFont_t) GetProcAddress(kernel32, "SetConsoleFont");
                                        if (rawcurses_SetConsoleFont && (cfi.nFont != (DWORD) -1))
                                        {
                                            rawcurses_SetConsoleFont(rawcurses_stdout, cfi.nFont);
                                        }
                                    }
#endif /* ! (defined(__WINE__) || defined(__DMC__) || defined(__TINYC__) || defined(__BORLANDC__)) */
                                    if (DuplicateHandle(hConsoleProcess, remote_mapping,
                                                        GetCurrentProcess(), &local_mapping,
                                                        0,
                                                        FALSE,
                                                        DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS))
                                    {
                                        CloseHandle(local_mapping);
                                    }
                                    else
                                    {
                                        CloseHandle(remote_mapping);
                                    }
                                }
                            }
                            CloseHandle(mapping);
                        }
                        CloseHandle(hConsoleProcess);
                    }
                }
                CloseHandle(cc.hConsoleWindow);
            }
        }
    }
    if (kernel32) FreeLibrary(kernel32);
    return ret;
}

#endif

#undef attrset
#define attrset rawcurses_attrset
static int attrset(attr_t a);

#undef move
#define move rawcurses_move
static int move(int y, int x);

#undef addch
#define addch rawcurses_addch
static int addch(chtype ch);

#undef addstr
#define addstr rawcurses_addstr
static int addstr(const char *s);

static void rawcurses_getyx(int *y, int *x);

#undef erase
#define erase rawcurses_erase
static int erase(void);

#undef has_colors
#define has_colors() (rawcurses_color != 0)

#undef can_change_color
#define can_change_color rawcurses_can_change_color
static int can_change_color(void)
{
    return has_colors() && (! rawcurses_fixedpal) && (rawcurses_stdio ? ((rawcurses_stdio_ccc || rawcurses_stdio_ccc_linux) || rawcurses_stdio_256color || rawcurses_stdio_88color) : 1);
}

#undef start_color
#define start_color rawcurses_start_color
static int start_color(void)
{
    return OK;
}

#undef color_content
#define color_content rawcurses_color_content
static int color_content(short i, short *r, short *g, short *b)
{
    if (! can_change_color()) return ERR;
    if ((i < 0) || (i >= COLORS)) return ERR;
    *r = rawcurses_palette[i].r;
    *g = rawcurses_palette[i].g;
    *b = rawcurses_palette[i].b;
    return OK;
}

#undef init_color
#define init_color rawcurses_init_color
static int init_color(short i, short r, short g, short b)
{
    if (! can_change_color()) return ERR;
    if ((i < 0) || (i >= COLORS)) return ERR;
    if ((! rawcurses_palette_ever_changed)
        ||
        (rawcurses_palette[i].r != r)
        ||
        (rawcurses_palette[i].g != g)
        ||
        (rawcurses_palette[i].b != b))
    {
        rawcurses_palette[i].r = r;
        rawcurses_palette[i].g = g;
        rawcurses_palette[i].b = b;
        rawcurses_palette_changed = 1;
    }
    return OK;
}

#undef refresh
#define refresh rawcurses_refresh
static int refresh(void);

#undef curs_set
#define curs_set rawcurses_curs_set
static int curs_set(int visibility)
{
    int old_visibility;

    if ((visibility < 0) || (visibility > 1)) return ERR;
    old_visibility = rawcurses_cursor_visibility;
    rawcurses_cursor_visibility = visibility;
    if (rawcurses_civis)
    {
#if USE_WINCONSOLE
        if (! rawcurses_stdio)
        {
            rawcurses_cursorinfo.bVisible = rawcurses_cursor_visibility ? TRUE : FALSE;
            if (SetConsoleCursorInfo(rawcurses_stdout, &rawcurses_cursorinfo))
            {
                if (rawcurses_civis == -1) rawcurses_civis = 1;
            }
            else
            {
                if (rawcurses_civis == -1) rawcurses_civis = 0;
            }
        }
        else
#endif
        {
            int rv = rawcurses_cursor_visibility ? rawcurses_fput_cnorm(stdout) : rawcurses_fput_civis(stdout);
            if (rawcurses_civis == -1) rawcurses_civis = rv;
        }
    }
    return old_visibility;
}

#undef init_pair
#define init_pair rawcurses_init_pair
static int init_pair(short i, short fg, short bg)
{
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    rawcurses_pairs[i] = 0;
    if (fg & COLOR_RED) rawcurses_pairs[i] |= FOREGROUND_RED;
    if (fg & COLOR_GREEN) rawcurses_pairs[i] |= FOREGROUND_GREEN;
    if (fg & COLOR_BLUE) rawcurses_pairs[i] |= FOREGROUND_BLUE;
    if (fg & PEN_BRIGHT) rawcurses_pairs[i] |= FOREGROUND_INTENSITY;
    if (bg & COLOR_RED) rawcurses_pairs[i] |= BACKGROUND_RED;
    if (bg & COLOR_GREEN) rawcurses_pairs[i] |= BACKGROUND_GREEN;
    if (bg & COLOR_BLUE) rawcurses_pairs[i] |= BACKGROUND_BLUE;
    if (bg & PEN_BRIGHT) rawcurses_pairs[i] |= BACKGROUND_INTENSITY;
    return OK;
}

#undef pair_content
#define pair_content rawcurses_pair_content
static int pair_content(short i, short *fg, short *bg)
{
    if (! has_colors()) return ERR;
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    *fg = 0;
    if (rawcurses_pairs[i] & FOREGROUND_RED) *fg |= COLOR_RED;
    if (rawcurses_pairs[i] & FOREGROUND_GREEN) *fg |= COLOR_GREEN;
    if (rawcurses_pairs[i] & FOREGROUND_BLUE) *fg |= COLOR_BLUE;
    if (rawcurses_pairs[i] & FOREGROUND_INTENSITY) *fg |= PEN_BRIGHT;
    *bg = 0;
    if (rawcurses_pairs[i] & BACKGROUND_RED) *bg |= COLOR_RED;
    if (rawcurses_pairs[i] & BACKGROUND_GREEN) *bg |= COLOR_GREEN;
    if (rawcurses_pairs[i] & BACKGROUND_BLUE) *bg |= COLOR_BLUE;
    if (rawcurses_pairs[i] & BACKGROUND_INTENSITY) *bg |= PEN_BRIGHT;
    return OK;
}

static int rawcurses_getch(void);

#ifdef SIGTSTP

static void rawcurses_sigtstp_handler(int sig)
{
    if (sig == SIGTSTP)
    {
        signal(SIGTSTP, rawcurses_sigtstp_handler);
        rawcurses_got_sigtstp = 1;
    }
}

#endif

#ifdef SIGINT

static void rawcurses_sigint_handler(int sig)
{
    if (sig == SIGINT)
    {
        signal(SIGINT, rawcurses_sigint_handler);
        rawcurses_got_sigint = 1;
    }
}

#endif

static const char *rawcurses_getenv_boolean(const char *var)
{
    const char *val;

    val = getenv(var);
    if (val)
    {
        if (! strcmp(val, "0")) val = "";
    }
    return val;
}

static const char *rawcurses_term_type(void)
{
    const char *termType;
    const char *colorTermType;
    const char *x11display;
    const char *mlterm;
    const char *konsoleDcop;
    const char *termProgram;
    const char *termProgramVersion;
    const char *xEnvironment;

    termType = getenv("TERM");
    x11display = getenv("DISPLAY");
    colorTermType = getenv("COLORTERM");
    mlterm = getenv("MLTERM");
    konsoleDcop = getenv("KONSOLE_DCOP");
    termProgram = getenv("TERM_PROGRAM");
    termProgramVersion = getenv("TERM_PROGRAM_VERSION");
    xEnvironment = getenv("XENVIRONMENT");
    /* attempt to infer actual terminal type */
    if (termType
        &&
        (! strcmp(termType, "xterm"))
        &&
        (xEnvironment
         &&
         (strlen(xEnvironment) >= strlen("CXterm"))
         &&
         (! strcmp(xEnvironment + strlen(xEnvironment) - strlen("CXterm"), "CXterm"))))
    {
        termType = "cxterm";
    }
    else if (termType
             &&
             (! strcmp(termType, "rxvt"))
             &&
             colorTermType
             &&
             (! strcmp(colorTermType, "rxvt")))
    {
        termType = "aterm";
    }
    else if (termType
             &&
             (! strcmp(termType, "xterm"))
             &&
             colorTermType
             &&
             (! strcmp(colorTermType, ""))
             &&
             konsoleDcop)
    {
        termType = "konsole";
    }
    else if (termType
             &&
             (! strcmp(termType, "xterm"))
             &&
             colorTermType
             &&
             strcmp(colorTermType, "")
             &&
             strcmp(colorTermType, "1")
             &&
             strcmp(colorTermType, "0"))
    {
        termType = colorTermType;
    }
    else if (termType
             &&
             (! strcmp(termType, "xterm-color"))
             &&
             termProgram
             &&
             (! strcmp(termProgram, "iTerm.app")))
    {
        termType = "iTerm.app";
    }
    else if (termType
             &&
             ((! strcmp(termType, "vt100"))
              ||
              (! strcmp(termType, "xterm-color")))
             &&
             termProgram
             &&
             (! strcmp(termProgram, "A" /* Hmm... */ "pple_Terminal")))
    {
        if (termProgramVersion
            &&
            (atoi(termProgramVersion) <= 41))
        {
            termType = "nsterm";
        }
        else if (! strcmp(termType, "xterm-color"))
        {
            if (atoi(termProgramVersion) >= 100)
            {
                termType = "nsterm-16color";
            }
            else
            {
                termType = "nsterm-c-acs";
            }
        }
        else
        {
            termType = "nsterm-c-7";
        }
    }
    else if (termType
             &&
             (! strcmp(termType, "xterm"))
             &&
             mlterm)
    {
        termType = "mlterm";
    }
    else if (colorTermType
             &&
             (! x11display)
             &&
             (! strcmp(colorTermType, "rxvt-xpm"))
             &&
             termType
             &&
             ((! strcmp(termType, "xterm"))
              ||
              (! strcmp(termType, "rxvt"))))
    {
        termType = "rxvt-noacs";
    }
    else if (colorTermType
             &&
             strcmp("", colorTermType)
             &&
             strcmp("0", colorTermType)
             &&
             termType
             &&
             (! strcmp(termType, "tw52")))
    {
        termType = "tw52-color";
    }
#if USE_TOSCONSOLE
    if (! termType) termType = "st52";
#endif
#if defined(MORPHOS)
    if (! termType) termType = "morphos";
#endif
#if defined(__AROS__) || defined(AMIGA)
    if (! termType) termType = "amiga";
#endif
#ifdef __MSDOS__
    if (! termType) termType = "ansi.sys";
#endif
    /* this seems like a sane default... */
    if (! termType) termType = "vt100";
    return termType;
}

#ifndef NEED_LOCALE_IS_UTF8
#define NEED_LOCALE_IS_UTF8 1
#endif
static int locale_is_utf8(void);

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    const char *termType;
    const char *lines;
    const char *columns;

    termType = rawcurses_term_type();
    rawcurses_stdio_tw52 = 0;
    if (termType
        &&
        ((! strncmp(termType, "stv52", strlen("stv52")))
         ||
         (! strncmp(termType, "tw52", strlen("tw52")))))
    {
        /* internal flag for handling the ever-quirky TosWin2 in VT52
         * mode */
        rawcurses_stdio_tw52 = 1;
    }
    lines = getenv("LINES");
    columns = getenv("COLUMNS");
    if (rawcurses_getenv_boolean("RAWCURSES_LINES"))
    {
        lines = rawcurses_getenv_boolean("RAWCURSES_LINES");
    }
    if (rawcurses_getenv_boolean("RAWCURSES_COLUMNS"))
    {
        columns = rawcurses_getenv_boolean("RAWCURSES_COLUMNS");
    }
    if (rawcurses_getenv_boolean("RAWCURSES_16COLOR"))
    {
        rawcurses_16color = *(rawcurses_getenv_boolean("RAWCURSES_16COLOR")) ? 1 : 0;
    }
    else
    {
        rawcurses_16color = 1;
        if (strstr(termType, "-16color-")
            ||
            (strstr(termType, "-16color")
             &&
             (! strcmp(termType + strlen(termType) - strlen("-16color"), "-16color"))))
        {
            rawcurses_16color = 1;
        }
        else if ((! strcmp(termType, "kterm"))
                 ||
                 (! strcmp(termType, "cxterm"))
                 ||
                 (! strcmp(termType, "eterm"))
                 ||
                 (! strcmp(termType, "eterm-color"))
                 ||
                 ((! strncmp(termType, "nsterm", strlen("termType")))
                  &&
                  (! strstr(termType, "-c-"))
                  &&
                  ((! strstr(termType, "-c"))
                   ||
                   strcmp(termType + strlen(termType) - strlen("-c"), "-c")))
                 ||
                 (! strncmp(termType, "darwin", strlen("darwin")))
                 ||
                 (! strncmp(termType, "xnuppc", strlen("xnuppc"))))
        {
            rawcurses_16color = 0;
        }
    }
    rawcurses_palette_changed = 0;
    if (rawcurses_getenv_boolean("RAWCURSES_DEBUG_UTF8"))
    {
        rawcurses_debug_utf8 = *(rawcurses_getenv_boolean("RAWCURSES_DEBUG_UTF8")) ? 1 : 0;
    }
    else
    {
        rawcurses_debug_utf8 = 0;
    }
    rawcurses_debug_utf8_y = 0;
    if (rawcurses_getenv_boolean("RAWCURSES_256COLOR"))
    {
        rawcurses_stdio_256color = *(rawcurses_getenv_boolean("RAWCURSES_256COLOR")) ? 1 : 0;
    }
    else
    {
        rawcurses_stdio_256color = 0;
        if ((! strcmp(termType, "konsole"))
            ||
            (! strcmp(termType, "Eterm"))
            ||
            (! strcmp(termType, "Eterm-color"))
            ||
            (! strcmp(termType, "iTerm.app"))
            ||
            (! strcmp(termType, "Terminal"))
            ||
            (! strcmp(termType, "putty"))
            ||
            (! strcmp(termType, "rxvt-cygwin"))
            ||
            (! strcmp(termType, "rxvt-cygwin-native"))
            ||
            strstr(termType, "-256color-")
            ||
            (strstr(termType, "-256color")
             &&
             (! strcmp(termType + strlen(termType) - strlen("-256color"), "-256color"))))
        {
            rawcurses_stdio_256color = 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_88COLOR"))
    {
        rawcurses_stdio_88color = *(rawcurses_getenv_boolean("RAWCURSES_88COLOR")) ? 1 : 0;
    }
    else
    {
        rawcurses_stdio_88color = 0;
        if ((! strcmp(termType, "rxvt-unicode"))
            ||
            strstr(termType, "-88color-")
            ||
            (strstr(termType, "-88color")
             &&
             (! strcmp(termType + strlen(termType) - strlen("-88color"), "-88color"))))
        {
            rawcurses_stdio_88color = 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_LINUX"))
    {
        rawcurses_stdio_ccc_linux = *(rawcurses_getenv_boolean("RAWCURSES_LINUX")) ? 1 : 0;
    }
    else
    {
        rawcurses_stdio_ccc_linux = 0;
        if ((! strcmp(termType, "Eterm"))
            ||
            (! strcmp(termType, "Eterm-color"))
            ||
            (! strcmp(termType, "linux"))
            ||
            (! strcmp(termType, "linux-c"))
            ||
            (! strcmp(termType, "linux-c-nc"))
            ||
            (! strcmp(termType, "linux-nic"))
            ||
            (! strcmp(termType, "linux-koi8"))
            ||
            (! strcmp(termType, "linux-lat"))
            ||
            (! strcmp(termType, "linux-koi8r"))
            ||
            (! strcmp(termType, "linux-vt"))
            ||
            (! strcmp(termType, "putty")))
        {
            rawcurses_stdio_ccc_linux = 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_BLINK"))
    {
        rawcurses_stdio_blink = *(rawcurses_getenv_boolean("RAWCURSES_BLINK")) ? 1 : 0;
    }
    else
    {
        rawcurses_stdio_blink = 0;
        if ((! strcmp(termType, "Eterm"))
            ||
            (! strcmp(termType, "Eterm-color"))
            ||
            (! strcmp(termType, "uuterm"))
            ||
            (! strcmp(termType, "aterm"))
            ||
            (! strncmp(termType, "rxvt", strlen("rxvt")))
            ||
            (! strcmp(termType, "putty"))
            ||
            (! strncmp(termType, "linux", strlen("linux")))
            ||
            (! strncmp(termType, "cygwin", strlen("cygwin"))))
        {
            rawcurses_stdio_blink = 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_CCC"))
    {
        rawcurses_stdio_ccc = *(rawcurses_getenv_boolean("RAWCURSES_CCC")) ? 1 : 0;
    }
    else
    {
        rawcurses_stdio_ccc = 0;
        if ((! strcmp(termType, "Eterm"))
            ||
            (! strcmp(termType, "Eterm-color"))
            ||
            (! strcmp(termType, "aterm"))
            ||
            (! strcmp(termType, "rxvt"))
            ||
            (! strcmp(termType, "rxvt-16color"))
            ||
            (! strcmp(termType, "rxvt-256color"))
            ||
            (! strcmp(termType, "rxvt-color"))
            ||
            (! strcmp(termType, "rxvt-cygwin"))
            ||
            (! strcmp(termType, "rxvt-cygwin-native"))
            ||
            (! strcmp(termType, "rxvt-unicode"))
            ||
            (! strcmp(termType, "rxvt-xpm"))
            ||
            (! strcmp(termType, "xterm"))
            ||
            (! strcmp(termType, "xterm-88color"))
            ||
            (! strcmp(termType, "xterm-256color")))
        {
            rawcurses_stdio_ccc = 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_WCWIDTH"))
    {
        rawcurses_builtin_wcwidth = *(rawcurses_getenv_boolean("RAWCURSES_WCWIDTH")) ? -1 : 0;
        if (getenv("RAWCURSES_WCWIDTH") && (atoi(getenv("RAWCURSES_WCWIDTH")) > 0))
        {
            rawcurses_builtin_wcwidth = atoi(getenv("RAWCURSES_WCWIDTH"));
        }
    }
    else
    {
#ifdef __CYGWIN__
        rawcurses_builtin_wcwidth = -1;
#else
        rawcurses_builtin_wcwidth = 0;
#endif
    }
    rawcurses_raw = 0;
    rawcurses_stdio_utf8 = locale_is_utf8();
    rawcurses_winsize_on_clear = 1;
    if ((! strcmp(termType, "Eterm"))
        ||
        (! strcmp(termType, "Eterm-color"))
        ||
        (! strcmp(termType, "aterm"))
        ||
        (! strcmp(termType, "eterm"))
        ||
        (! strcmp(termType, "eterm-color"))
        ||
        ((! strncmp(termType, "rxvt", strlen("rxvt")))
         &&
         strcmp(termType, "rxvt-unicode")))
    {
        /* (m)rxvt is still living in the pre-UTF-8 dark ages. it gets
         * ASCII for now. likewise the Emacs "eterm"/"eterm-color". */
        rawcurses_stdio_utf8 = 0;
    }
    else if (! strncmp(termType, "cygwin", strlen("cygwin")))
    {
        /* severe problems reading the winsize response mean we only try to do it at startup */
        rawcurses_winsize_on_clear = 0;
    }
    if (rawcurses_getenv_boolean("RAWCURSES_UTF8"))
    {
        rawcurses_stdio_utf8 = *(rawcurses_getenv_boolean("RAWCURSES_UTF8")) ? 1 : 0;
    }
    if (rawcurses_getenv_boolean("RAWCURSES_VT52"))
    {
        rawcurses_stdio_vt52 = *(rawcurses_getenv_boolean("RAWCURSES_VT52")) ? 1 : 0;
    }
    else
    {
        const char *vt52like = RAWCURSES_VT52LIKE;

        rawcurses_stdio_vt52 = 0;
        while (strlen(vt52like))
        {
            if (! strcmp(termType, vt52like))
            {
                rawcurses_stdio_vt52 = 1;
                break;
            }
            vt52like += strlen(vt52like) + 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_AMIGA"))
    {
        rawcurses_stdio_amiga = *(rawcurses_getenv_boolean("RAWCURSES_AMIGA")) ? 1 : 0;
    }
    else
    {
        const char *amigalike = RAWCURSES_AMIGALIKE;

        rawcurses_stdio_amiga = 0;
        while (strlen(amigalike))
        {
            if (! strncmp(termType, amigalike, strlen(amigalike)))
            {
                rawcurses_stdio_amiga = 1;
                break;
            }
            amigalike += strlen(amigalike) + 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_ST52"))
    {
        rawcurses_stdio_st52 = *(rawcurses_getenv_boolean("RAWCURSES_ST52")) ? 1 : 0;
    }
    else
    {
        const char *st52like = RAWCURSES_ST52LIKE;

        rawcurses_stdio_st52 = 0;
        while (strlen(st52like))
        {
            if (! strncmp(termType, st52like, strlen(st52like)))
            {
                rawcurses_stdio_st52 = 1;
                break;
            }
            st52like += strlen(st52like) + 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_ADM3A"))
    {
        rawcurses_stdio_adm3a = *(rawcurses_getenv_boolean("RAWCURSES_ADM3A")) ? 1 : 0;
    }
    else
    {
        const char *adm3alike = RAWCURSES_ADM3ALIKE;

        rawcurses_stdio_adm3a = 0;
        while (strlen(adm3alike))
        {
            if (! strcmp(termType, adm3alike))
            {
                rawcurses_stdio_adm3a = 1;
                break;
            }
            adm3alike += strlen(adm3alike) + 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_ISO2022"))
    {
        rawcurses_stdio_iso2022 = *(rawcurses_getenv_boolean("RAWCURSES_ISO2022")) ? 1 : 0;
    }
    else
    {
        rawcurses_stdio_iso2022 = 1;
        if (rawcurses_stdio_adm3a
            ||
            rawcurses_stdio_vt52
            ||
            (! strncmp(termType, "pcansi", strlen("pcansi")))
            ||
            (! strncmp(termType, "ansi", strlen("ansi")))
            ||
            (! strncmp(termType, "ibmpc", strlen("ibmpc")))
            ||
            (! strncmp(termType, "origibmpc", strlen("origibmpc")))
            ||
            (! strncmp(termType, "nansi.sys", strlen("nansi.sys")))
            ||
            (! strncmp(termType, "cygwin", strlen("cygwin"))))
        {
            rawcurses_stdio_iso2022 = 0;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_ACS"))
    {
        rawcurses_stdio_acs = *(rawcurses_getenv_boolean("RAWCURSES_ACS")) ? 1 : 0;
    }
    else
    {
        rawcurses_stdio_acs = 0;
        if (((! strncmp(termType, "vt1", strlen("vt1")))
             ||
             (! strncmp(termType, "vt2", strlen("vt2")))
             ||
             (! strncmp(termType, "vt3", strlen("vt3")))
             ||
             (! strncmp(termType, "rxvt", strlen("rxvt")))
             ||
             (! strncmp(termType, "tw100", strlen("tw100")))
             ||
#if 0
             /* FIXME: this should be re-enabled once we find a way to
              * leave altcharset mode in TosWin II's VT52 mode */
             (! strncmp(termType, "tw52", strlen("tw52")))
             ||
#endif
             (! strcmp(termType, "iTerm.app"))
             ||
             (! strcmp(termType, "aterm"))
             ||
             (! strcmp(termType, "kterm"))
             ||
             (! strcmp(termType, "mlterm"))
             ||
             (! strcmp(termType, "Eterm"))
             ||
             (! strcmp(termType, "Eterm-color"))
             ||
             (! strcmp(termType, "arm100"))
             ||
             (! strcmp(termType, "arm100-am"))
             ||
             (! strcmp(termType, "konsole"))
             ||
             (! strcmp(termType, "gnome-terminal"))
             ||
             (! strcmp(termType, "gnome"))
             ||
             (! strncmp(termType, "gnome-", strlen("gnome-")))
             ||
             (! strcmp(termType, "Terminal"))
             ||
             (! strcmp(termType, "cxterm"))
             ||
             (! strcmp(termType, "nsterm-16color"))
             ||
             (! strncmp(termType, "xterm", strlen("xterm")))
             ||
             (strstr(termType, "-acs-")
              ||
              (strstr(termType, "-acs")
               &&
               (! strcmp(termType + strlen(termType) - strlen("-acs"), "-acs")))))
            &&
            ((! strstr(termType, "-noacs-"))
             &&
             ((! strstr(termType, "-noacs"))
              ||
              strcmp(termType + strlen(termType) - strlen("-noacs"), "-noacs")))
            &&
            strcmp(termType, "rxvt-cygwin")
            &&
            strcmp(termType, "rxvt-cygwin-native"))
        {
            /* HACK: avoid ACS in cygwin rxvt running in native Win32
             * GUI mode since the font lacks the ACS glyphs */
            rawcurses_stdio_acs = 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_NOBULLET"))
    {
        rawcurses_stdio_acs_nobullet = *(rawcurses_getenv_boolean("RAWCURSES_NOBULLET")) ? 1 : 0;
    }
    else
    {
        rawcurses_stdio_acs_nobullet = 0;
        /* HACK: avoid ACS bullet in nsterm since it is fullwidth in
         * the default setup */
        if (! strncmp(termType, "nsterm", strlen("nsterm")))
        {
            rawcurses_stdio_acs_nobullet = 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_NOCOLORBOLD"))
    {
        rawcurses_stdio_nocolorbold = *(rawcurses_getenv_boolean("RAWCURSES_NOCOLORBOLD")) ? 1 : 0;
    }
    else
    {
        rawcurses_stdio_nocolorbold = 0;
        if (((rawcurses_stdio_256color
              ||
              rawcurses_stdio_88color)
             &&
             (! rawcurses_stdio_ccc)
             &&
             (! rawcurses_stdio_ccc_linux))
            ||
            (rawcurses_16color
             &&
             ((! strcmp(termType, "konsole"))
              ||
              (! strcmp(termType, "gnome-terminal"))
              ||
              (! strcmp(termType, "gnome"))
              ||
              (! strncmp(termType, "gnome-", strlen("gnome-")))
              ||
              (! strcmp(termType, "Terminal"))
              ||
              (! strcmp(termType, "iTerm.app"))
              ||
              (! strncmp(termType, "nsterm-c", strlen("nsterm-c")))
              ||
              (! strcmp(termType, "nsterm-16color"))
              ||
              (! strncmp(termType, "xterm", strlen("xterm")))))
            ||
            (! strncmp(termType, "darwin", strlen("darwin")))
            ||
            (! strncmp(termType, "xnuppc", strlen("xnuppc"))))
        {
            rawcurses_stdio_nocolorbold = 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_H19"))
    {
        rawcurses_stdio_acs_h19 = *(rawcurses_getenv_boolean("RAWCURSES_H19")) ? 1 : 0;
    }
    else
    {
        const char *h19like = RAWCURSES_H19LIKE;

        rawcurses_stdio_acs_h19 = 0;
        while (strlen(h19like))
        {
            if (! strcmp(termType, h19like))
            {
                rawcurses_stdio_acs_h19 = 1;
                break;
            }
            h19like += strlen(h19like) + 1;
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_NOSWAPDOTS"))
    {
        rawcurses_stdio_noswapdots = *(rawcurses_getenv_boolean("RAWCURSES_NOSWAPDOTS")) ? 1 : 0;
    }
    else
    {
        rawcurses_stdio_noswapdots = 0;
        if ((! strncmp(termType, "tw100", strlen("tw100")))
            ||
            (! strncmp(termType, "tw52", strlen("tw52"))))
        {
            rawcurses_stdio_noswapdots = 1;
        }
    }
    if (rawcurses_stdio_acs_h19 && ! rawcurses_getenv_boolean("RAWCURSES_ACS"))
    {
        rawcurses_stdio_acs = 1;
    }
    if (rawcurses_getenv_boolean("RAWCURSES_CP437"))
    {
        rawcurses_stdio_cp437 = *(rawcurses_getenv_boolean("RAWCURSES_CP437")) ? 1 : 0;
    }
    else
    {
        const char *cp437like = RAWCURSES_CP437LIKE;

        rawcurses_stdio_cp437 = 0;
        while (strlen(cp437like))
        {
            if (! strcmp(termType, cp437like))
            {
                rawcurses_stdio_cp437 = 1;
                break;
            }
            cp437like += strlen(cp437like) + 1;
        }
    }
    if (rawcurses_stdio_cp437 && ! rawcurses_getenv_boolean("RAWCURSES_ACS"))
    {
        rawcurses_stdio_acs = 1;
    }
    if (rawcurses_getenv_boolean("RAWCURSES_RELCUP"))
    {
        rawcurses_stdio_relcup = *(rawcurses_getenv_boolean("RAWCURSES_RELCUP")) ? 1 : 0;
    }
    else
    {
        rawcurses_stdio_relcup = 0;
    }
    if (rawcurses_getenv_boolean("RAWCURSES_CIVIS"))
    {
        rawcurses_civis = *(rawcurses_getenv_boolean("RAWCURSES_CIVIS")) ? 1 : 0;
    }
    else
    {
        rawcurses_civis = -1;
    }
    if (rawcurses_getenv_boolean("RAWCURSES_TRANSPARENT"))
    {
        rawcurses_transparent = *(rawcurses_getenv_boolean("RAWCURSES_TRANSPARENT")) ? 1 : 0;
    }
    else
    {
        rawcurses_transparent = 1;
    }
    if (rawcurses_nw && rawcurses_nh)
    {
        rawcurses_w = rawcurses_nw;
        rawcurses_h = rawcurses_nh;
        w = rawcurses_w;
        h = rawcurses_h;
        rawcurses_nw = 0;
        rawcurses_nh = 0;
    }
    else
    {
        rawcurses_w = (columns && atoi(columns)) ? atoi(columns) : 0;
        rawcurses_h = (lines && atoi(lines)) ? atoi(lines) : 0;
    }
    rawcurses_x = -1;
    rawcurses_y = -1;
    rawcurses_attr = -1;
    rawcurses_old_attr_rgb = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
#if USE_WINCONSOLE
    if (rawcurses_getenv_boolean("RAWCURSES_STDIO"))
    {
        rawcurses_stdio = *(rawcurses_getenv_boolean("RAWCURSES_STDIO")) ? 1 : 0;
    }
    else if (rawcurses_getenv_boolean("RAWCURSES_CONIO"))
    {
        rawcurses_stdio = *(rawcurses_getenv_boolean("RAWCURSES_CONIO")) ? 1 : 0;
    }
    rawcurses_stdin = GetStdHandle(STD_INPUT_HANDLE);
    rawcurses_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    rawcurses_stderr = GetStdHandle(STD_ERROR_HANDLE);
    rawcurses_old_mode_valid = 0;
    if (rawcurses_old_screen)
    {
        free((void *) rawcurses_old_screen);
        rawcurses_old_screen = NULL;
    }
    if (rawcurses_stdio != 1)
    {
        SMALL_RECT newsize;

        if (rawcurses_stdio != 0) AllocConsole();
        rawcurses_stdin = GetStdHandle(STD_INPUT_HANDLE);
        rawcurses_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
        rawcurses_stderr = GetStdHandle(STD_ERROR_HANDLE);
        if (GetConsoleMode(rawcurses_stdin, &rawcurses_old_mode))
        {
            rawcurses_old_mode_valid = 1;
            SetConsoleMode(rawcurses_stdin, ENABLE_WINDOW_INPUT);
        }
        rawcurses_cursorinfo.bVisible = rawcurses_cursor_visibility ? TRUE : FALSE;
        rawcurses_cursorinfo.dwSize = 25;
        memcpy((void *) &rawcurses_old_cursorinfo, (void *) &rawcurses_cursorinfo, sizeof(rawcurses_old_cursorinfo));
        if (GetConsoleCursorInfo(rawcurses_stdout, &rawcurses_old_cursorinfo))
        {
            rawcurses_stdio = 0;
            rawcurses_cursorinfo.dwSize = rawcurses_old_cursorinfo.dwSize;
            if (rawcurses_civis)
            {
                rawcurses_cursorinfo.bVisible = rawcurses_cursor_visibility ? TRUE : FALSE;
                if (SetConsoleCursorInfo(rawcurses_stdout, &rawcurses_cursorinfo))
                {
                    if (rawcurses_civis == -1) rawcurses_civis = 1;
                }
                else
                {
                    if (rawcurses_civis == -1) rawcurses_civis = 0;
                }
            }
            else
            {
                rawcurses_cursorinfo.bVisible = rawcurses_old_cursorinfo.bVisible;
            }
        }
        rawcurses_old_title[0] = '\0';
        GetConsoleTitle(rawcurses_old_title, MAX_PATH);
        if (title || shortname)
        {
            SetConsoleTitleA(title ? title : shortname);
            usleep(RAWCURSES_TITLE_QUIESCE_USECS);
        }
        rawcurses_csbi.srWindow.Top = 0;
        rawcurses_csbi.srWindow.Left = 0;
        rawcurses_csbi.srWindow.Bottom = rawcurses_h - 1;
        rawcurses_csbi.srWindow.Right = rawcurses_w - 1;
        rawcurses_old_size.Top = 0;
        rawcurses_old_size.Left = 0;
        rawcurses_old_size.Right = 80 - 1;
        rawcurses_old_size.Bottom = 25 - 1;
        rawcurses_old_x = -1;
        rawcurses_old_y = -1;
        if (GetConsoleScreenBufferInfo(rawcurses_stdout, &rawcurses_csbi))
        {
            COORD old_screen_size;

            rawcurses_old_x = rawcurses_csbi.dwCursorPosition.X;
            rawcurses_old_y = rawcurses_csbi.dwCursorPosition.Y;
            rawcurses_old_attr_rgb = rawcurses_csbi.wAttributes;
            rawcurses_old_size.Right = rawcurses_csbi.srWindow.Right - rawcurses_csbi.srWindow.Left;
            rawcurses_old_size.Bottom = rawcurses_csbi.srWindow.Bottom - rawcurses_csbi.srWindow.Top;
            old_screen_size.X = rawcurses_old_size.Right + 1;
            old_screen_size.Y = rawcurses_old_size.Bottom + 1;
            rawcurses_old_screen = (CHAR_INFO *) malloc(old_screen_size.X * old_screen_size.Y * sizeof(CHAR_INFO));
            if (rawcurses_old_screen)
            {
                SMALL_RECT read_region;
                COORD upper_left;

                upper_left.X = 0;
                upper_left.Y = 0;
                read_region = rawcurses_old_size;
                if ((! ReadConsoleOutput(rawcurses_stdout,
                                         rawcurses_old_screen,
                                         old_screen_size,
                                         upper_left,
                                         & read_region))
                    ||
                    memcmp((void *) &read_region, (void *) &rawcurses_old_size, sizeof(read_region)))
                {
                    free((void *) rawcurses_old_screen);
                    rawcurses_old_screen = NULL;
                }
            }
            if (rawcurses_w > rawcurses_csbi.dwMaximumWindowSize.X) rawcurses_w = rawcurses_csbi.dwMaximumWindowSize.X;
            if (rawcurses_h > rawcurses_csbi.dwMaximumWindowSize.Y) rawcurses_h = rawcurses_csbi.dwMaximumWindowSize.Y;
            if (! rawcurses_w) rawcurses_w = rawcurses_old_size.Right + 1;
            if (! rawcurses_h) rawcurses_h = rawcurses_old_size.Bottom + 1;
        }
        newsize.Top = 0;
        newsize.Left = 0;
        newsize.Right = (w ? w : rawcurses_w ? rawcurses_w : 80) - 1;
        newsize.Bottom = (h ? h : rawcurses_h ? rawcurses_h : 25) - 1;
        if (newsize.Right < rawcurses_old_size.Right)
        {
            newsize.Right = rawcurses_old_size.Right;
        }
        if (newsize.Bottom < rawcurses_old_size.Bottom)
        {
            newsize.Bottom = rawcurses_old_size.Bottom;
        }
        SetConsoleWindowInfo(rawcurses_stdout, TRUE, &newsize);
        if (GetConsoleScreenBufferInfo(rawcurses_stdout, &rawcurses_csbi))
        {
            rawcurses_w = rawcurses_csbi.srWindow.Right + 1 - rawcurses_csbi.srWindow.Left;
            rawcurses_h = rawcurses_csbi.srWindow.Bottom + 1 - rawcurses_csbi.srWindow.Top;
        }
    }
    {
        DWORD pipemode = PIPE_READMODE_BYTE | PIPE_NOWAIT;

        SetNamedPipeHandleState(rawcurses_stdin, &pipemode, NULL, NULL);
    }
#else /* ! USE_WINCONSOLE */
#if USE_TOSCONSOLE
    {
        char *stdindev;

        stdindev = ttyname(fileno(stdin));
        if (stdindev
            &&
            (! strcmp(stdindev, "/dev/con")))
        {
            rawcurses_stdio_stcon = 1;
        }
        else
        {
            rawcurses_stdio_stcon = 0;
        }
    }
#endif /* USE_TOSCONSOLE */
#if USE_AROSCONSOLE
    {
        if (IsInteractive(Input()))
        {
            rawcurses_stdio_aroscon = 1;
        }
        else
        {
            rawcurses_stdio_aroscon = 0;
        }
    }
#endif /* USE_AROSCONSOLE */
#if USE_IOCTL
    if (! (rawcurses_w && rawcurses_h)) {
#ifdef TIOCGWINSZ
        struct winsize wsz;

        if (! ioctl(fileno(stdout), TIOCGWINSZ, &wsz))
        {
            if (! rawcurses_h) rawcurses_h = wsz.ws_row;
            if (! rawcurses_w) rawcurses_w = wsz.ws_col;
        }
#else
#ifdef TIOCGSIZE
        struct ttysize tsz;

        if (! ioctl(fileno(stdout), TIOCGSIZE, &tsz))
        {
            if (! rawcurses_h) rawcurses_h = tsz.ts_lines;
            if (! rawcurses_w) rawcurses_w = tsz.ts_cols;
        }
#endif
#endif
    }
#endif
#endif /* ! USE_WINCONSOLE */
#if USE_AROSCONSOLE
    if (rawcurses_stdio_aroscon)
    {
        if (SetMode(Input(), 1) != DOSTRUE)
        {
            rawcurses_stdio_aroscon_oldmode = 1;
        }
        else
        {
            rawcurses_stdio_aroscon_oldmode = 0;
        }
    }
#endif
#if USE_TERMIOS
    {
        fflush(stdout);
        tcgetattr(fileno(stdin), &rawcurses_old_tty);
        rawcurses_tty = rawcurses_old_tty;
        rawcurses_tty.c_lflag &= ~ICANON;
        rawcurses_tty.c_lflag &= ~ECHO;
#ifdef ONLCR
        rawcurses_tty.c_oflag &= ~ONLCR;
#else
#ifdef CRMOD
        rawcurses_tty.c_oflag &= ~CRMOD;
#endif
#endif
        tcsetattr(fileno(stdin), TCSANOW, &rawcurses_tty);
        setbuf(stdin, NULL);
    }
#endif /* USE_TERMIOS */
#if USE_SGTTY
    {
        fflush(stdout);
        ioctl(fileno(stdin), TIOCGETP, &rawcurses_old_sgtty);
        rawcurses_sgtty = rawcurses_old_sgtty;
        rawcurses_sgtty.sg_flags |= CBREAK;
        rawcurses_sgtty.sg_flags &= ~ECHO;
#ifdef CRMOD
        rawcurses_sgtty.sg_flags &= ~CRMOD;
#endif
#ifdef NODELAY
        rawcurses_sgtty.sg_flags |= NODELAY;
#endif
        ioctl(fileno(stdin), TIOCSETP, &rawcurses_sgtty);
        setbuf(stdin, NULL);
    }
#endif /* USE_SGTTY */
#if USE_CONIO || USE_CONIO_INPUT
#if USE_CONIO
    rawcurses_stdio_conio = -1;
#endif
#if USE_CONIO_INPUT
    rawcurses_stdio_conio_input = -1;
#endif
#if USE_CONIO
    if (rawcurses_getenv_boolean("RAWCURSES_CONIO"))
    {
        rawcurses_stdio_conio = *(rawcurses_getenv_boolean("RAWCURSES_CONIO")) ? 1 : 0;
#if USE_CONIO_INPUT
        rawcurses_stdio_conio_input = *(rawcurses_getenv_boolean("RAWCURSES_CONIO")) ? 1 : 0;
#endif
    }
#endif /* USE_CONIO */
#if USE_CONIO_INPUT
    if (rawcurses_getenv_boolean("RAWCURSES_CONIO_INPUT"))
    {
        rawcurses_stdio_conio_input = *(rawcurses_getenv_boolean("RAWCURSES_CONIO_INPUT")) ? 1 : 0;
    }
#endif
#if USE_CONIO
    if (rawcurses_stdio_conio == -1)
    {
        if (rawcurses_getenv_boolean("RAWCURSES_STDIO"))
        {
            rawcurses_stdio_conio = ! (*(rawcurses_getenv_boolean("RAWCURSES_STDIO")) ? 1 : 0);
#if USE_CONIO_INPUT
            if (rawcurses_stdio_conio_input == -1)
            {
                rawcurses_stdio_conio_input = rawcurses_stdio_conio;
            }
#endif /* USE_CONIO_INPUT */
        }
    }
#endif /* USE_CONIO */
    {
        char *stdindev;

        stdindev = ttyname(fileno(stdin));
#if USE_CONIO_INPUT
        if ((rawcurses_stdio_conio_input == 1)
            ||
            (stdindev
             &&
             (! strcmp(stdindev, "con"))
             &&
             (rawcurses_stdio_conio_input != 0)))
        {
            rawcurses_stdio_conio_input = 1;
        }
        else
        {
            rawcurses_stdio_conio_input = 0;
        }
#endif /* USE_CONIO_INPUT */
#if USE_CONIO
        if ((rawcurses_stdio_conio == 1)
            ||
            (stdindev
             &&
             (! strcmp(stdindev, "con"))
             &&
             (rawcurses_stdio_conio != 0)))
        {
#ifndef __TINYC__
            struct text_info screensize_info;
#endif /* ! defined(__TINYC__) */

            rawcurses_stdio_conio = 1;
#ifndef __TINYC__
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
                rawcurses_w = 40;
                rawcurses_h = 25;
                break;
            case BW80:
            case C80:
                rawcurses_w = 80;
                rawcurses_h = 25;
                break;
            case MONO:
                break;
#ifndef __TURBOC__
            case C4350:
                rawcurses_w = 80;
                rawcurses_h = 43;
                break;
#endif
#if defined(__BORLANDC__) && defined(WIN32)
            case C40X14:
            case BW40X14:
                rawcurses_w = 40;
                rawcurses_h = 14;
                break;
            case C40X21:
            case BW40X21:
                rawcurses_w = 40;
                rawcurses_h = 21;
                break;
            case C40X28:
            case BW40X28:
                rawcurses_w = 40;
                rawcurses_h = 28;
                break;
            case C40X43:
            case BW40X43:
                rawcurses_w = 40;
                rawcurses_h = 43;
                break;
            case C40X50:
            case BW40X50:
                rawcurses_w = 40;
                rawcurses_h = 50;
                break;
            case C40X60:
            case BW40X60:
                rawcurses_w = 40;
                rawcurses_h = 60;
                break;
            case C80X14:
            case BW80X14:
            case MONO14:
                rawcurses_w = 80;
                rawcurses_h = 14;
                break;
            case C80X21:
            case BW80X21:
            case MONO21:
                rawcurses_w = 80;
                rawcurses_h = 21;
                break;
            case C80X28:
            case BW80X28:
            case MONO28:
                rawcurses_w = 80;
                rawcurses_h = 28;
                break;
            case C80X43:
            case BW80X43:
            case MONO43:
                rawcurses_w = 80;
                rawcurses_h = 43;
                break;
            case C80X50:
            case BW80X50:
            case MONO50:
                rawcurses_w = 80;
                rawcurses_h = 50;
                break;
            case C80X60:
            case BW80X60:
            case MONO60:
                rawcurses_w = 80;
                rawcurses_h = 60;
                break;
#endif /* defined(__BORLANDC__) && defined(WIN32) */
            }
#endif /* ! defined(_CONIO2_H_) */
            if (screensize_info.screenwidth)
            {
                rawcurses_w = screensize_info.screenwidth;
            }
            if (screensize_info.screenheight)
            {
                rawcurses_h = screensize_info.screenheight;
            }
#ifdef _NOCURSOR
            if (rawcurses_civis == -1) rawcurses_civis = 1;
#endif
            if (rawcurses_civis == -1) rawcurses_civis = 0;
#endif /* ! defined(__TINYC__) */
        }
        else
        {
            rawcurses_stdio_conio = 0;
        }
#endif /* USE_CONIO */
    }
#endif /* USE_CONIO || USE_CONIO_INPUT */
    if (rawcurses_stdio)
    {
        rawcurses_stdio = 1;
#ifdef SIGTSTP
        rawcurses_got_sigtstp = 0;
        rawcurses_old_sigtstp_handler = signal(SIGTSTP, rawcurses_sigtstp_handler);
#endif
#ifdef SIGINT
        rawcurses_got_sigint = 0;
        rawcurses_old_sigint_handler = signal(SIGINT, rawcurses_sigint_handler);
#endif
        if (rawcurses_civis == -1)
        {
            const char *nocivis = RAWCURSES_NOCIVIS;

            while (strlen(nocivis))
            {
                if (! strcmp(termType, nocivis))
                {
                        rawcurses_civis = 0;
                        break;
                }
                nocivis += strlen(nocivis) + 1;
            }
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_RAW"))
    {
        rawcurses_raw = *rawcurses_getenv_boolean("RAWCURSES_RAW") ? 1 : 0;
    }
    else if (rawcurses_stdio)
    {
        if (rawcurses_stdio_utf8) rawcurses_raw = 1;
#ifndef UNICODE
        if (rawcurses_stdio_cp437) rawcurses_raw = 1;
#endif
    }
    {
        int got_winsize;

        got_winsize = 1;
        if (rawcurses_w < 1)
        {
            rawcurses_w = 80;
            got_winsize = 0;
        }
        if (rawcurses_h < 1)
        {
            rawcurses_h = 24;
            got_winsize = 0;
        }
        rawcurses_stdio_new_shortname = NULL;
        rawcurses_stdio_new_title = NULL;
        rawcurses_winsize_pending = 0;
        if (rawcurses_stdio)
        {
            if (! shortname) shortname = title;
            if (shortname) rawcurses_stdio_new_shortname = strdup(shortname);
            if (title) rawcurses_stdio_new_title = strdup(title);
            rawcurses_fput_smcup(stdout);
            if (rawcurses_civis)
            {
                rawcurses_cursor_visibility ? rawcurses_fput_cnorm(stdout) : rawcurses_fput_civis(stdout);
            }
            if (rawcurses_stdio_acs) rawcurses_fput_enacs(stdout);
            if (got_winsize && ! rawcurses_got_winsize)
            {
                if (rawcurses_stdio_new_shortname && ! rawcurses_stdio_old_shortname)
                {
                    rawcurses_fput_request_icon_name(stdout);
                    fflush(stdout);
                }
                else if (rawcurses_stdio_new_title && ! rawcurses_stdio_old_title)
                {
                    rawcurses_fput_request_title(stdout);
                    fflush(stdout);
                }
            }
            else
            {
                rawcurses_fput_request_winsize(stdout);
                rawcurses_winsize_pending = 1;
            }
            fflush(stdout);
        }
    }
    attrset(0);
    erase();
    if (rawcurses_getenv_boolean("RAWCURSES_COLOR"))
    {
        rawcurses_color = *(rawcurses_getenv_boolean("RAWCURSES_COLOR")) ? 1 : 0;
    }
    else
    {
        /* true to a first approximation... */
        if (strstr(termType, "-mono-")
            ||
            strstr(termType, "-m-")
            ||
            (! strcmp(termType, "et52"))
            ||
            (strstr(termType, "-m")
             &&
             (! strcmp(termType + strlen(termType) - strlen("-m"), "-m")))
            ||
            (strstr(termType, "-mono")
             &&
             (! strcmp(termType + strlen(termType) - strlen("-mono"), "-mono"))))
        {
            rawcurses_color = 0;
        }
        else
        {
            rawcurses_color = rawcurses_stdio ? (! ((rawcurses_stdio_vt52 && ! rawcurses_stdio_st52) || rawcurses_stdio_adm3a)) : 1;
#if USE_CONIO
#if ! defined(_CONIO2_H_)
            if (rawcurses_stdio_conio)
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
                    rawcurses_color = 0;
                    break;
                default:
                    rawcurses_color = 1;
                }
            }
#endif /* ! defined(_CONIO2_H_) */
#endif /* USE_CONIO */
#if USE_TOSCONSOLE
            if (rawcurses_stdio_stcon)
            {
                int vmode;

                /* very very deprecated, but this lets us avoid linking with GEM libraries */
                vmode = Getrez();
                if ((vmode == 1) /* 640x200x2 */
                    ||
                    (vmode == 2) /* 640x400x1 */
                    ||
                    (vmode == 6) /* 1280x960x1 (TT030) */
                    )
                {
                    rawcurses_color = 0;
                }
            }
#endif /* USE_TOSCONSOLE */
        }
    }
    if (rawcurses_getenv_boolean("RAWCURSES_FIXEDPAL"))
    {
        rawcurses_fixedpal = *(rawcurses_getenv_boolean("RAWCURSES_FIXEDPAL")) ? 1 : 0;
    }
    else
    {
        rawcurses_fixedpal = 0;
    }
    {
        int i;

        for (i = 0; i < 16; i ++)
        {
            int j;

            for (j = 0; j < 16; j ++)
            {
                int p;

                p = j * 16 + i;
                if (p >= COLOR_PAIRS) continue;
                if (! p)
                {
                    init_pair(p, COLOR_WHITE, COLOR_BLACK);
                }
                else
                {
                    init_pair(p,
                              ((i & 1) ? COLOR_RED : 0)
                              |
                              ((i & 2) ? COLOR_GREEN : 0)
                              |
                              ((i & 4) ? COLOR_BLUE : 0)
                              |
                              ((i & 8) ? PEN_BRIGHT : 0),
                              ((j & 1) ? COLOR_RED : 0)
                              |
                              ((j & 2) ? COLOR_GREEN : 0)
                              |
                              ((j & 4) ? COLOR_BLUE : 0)
                              |
                              ((j & 8) ? PEN_BRIGHT : 0));
                }
            }
        }
        for (i = 0; i < COLORS; i ++)
        {
            rawcurses_palette[i].r = ((i & 1) ? ((i & 8) ? 1000 : 750) : ((i & 8) ? 500 : 0));
            rawcurses_palette[i].g = ((i & 2) ? ((i & 8) ? 1000 : 750) : ((i & 8) ? 500 : 0));
            rawcurses_palette[i].b = ((i & 4) ? ((i & 8) ? 1000 : 750) : ((i & 8) ? 500 : 0));
        }
        if (rawcurses_palette_ever_changed)
        {
            rawcurses_palette_changed = 1;
            memcpy((void *) rawcurses_palette, (void *) rawcurses_old_palette, sizeof(rawcurses_palette));
        }
        else
        {
#if USE_WINCONSOLE
            if (! rawcurses_stdio)
            {
                rawcurses_win32_get_palette(rawcurses_palette);
            }
#endif
            memcpy((void *) rawcurses_old_palette, (void *) rawcurses_palette, sizeof(rawcurses_old_palette));
        }
    }
    refresh();
    rawcurses_after_endwin = 0;
}

static void rawcurses_palette_snoop(void)
{
#if USE_WINCONSOLE
    if ((! rawcurses_stdio) && rawcurses_palette_changed)
    {
        if (! rawcurses_palette_ever_changed)
        {
            rawcurses_palette_ever_changed = 1;
        }
        rawcurses_win32_set_palette(rawcurses_palette);
        rawcurses_palette_changed = 0;
    }
#endif
    if (rawcurses_stdio && rawcurses_palette_changed)
    {
        if (! rawcurses_palette_ever_changed)
        {
            rawcurses_palette_ever_changed = 1;
            fflush(stdout);
            if (rawcurses_fput_request_palette(stdout) > 0)
            {
                int ch;
                int i;
                int got_key_resize;

                got_key_resize = 0;
                fflush(stdout);
                for (i = 0; i < 10; i ++)
                {
                    ch = rawcurses_getch();
                    fflush(stdout);
                    if (ch == KEY_RESIZE)
                    {
                        got_key_resize = 1;
                    }
                    else if (ch != ERR)
                    {
                        rawcurses_ungetch_buffer = ch;
                        break;
                    }
                    else if (rawcurses_reset_palette_seqs >= COLORS)
                    {
                        break;
                    }
                    usleep(RAWCURSES_PALETTE_QUIESCE_USECS / 10);
                }
                if (got_key_resize)
                {
                    /* FIXME: this may discard input */
                    rawcurses_ungetch_buffer = KEY_RESIZE;
                }
            }
        }
        rawcurses_fput_palette(stdout, rawcurses_palette);
        rawcurses_palette_changed = 0;
    }
}

static void rawcurses_palette_reset(void)
{
#if USE_WINCONSOLE
    if ((! rawcurses_stdio) && rawcurses_palette_ever_changed)
    {
        if (memcmp((void *) rawcurses_old_palette, (void *) rawcurses_palette, sizeof(rawcurses_palette)))
        {
            rawcurses_win32_set_palette(rawcurses_old_palette);
        }
    }
#endif
    if (rawcurses_stdio && rawcurses_palette_ever_changed)
    {
        if (memcmp((void *) rawcurses_old_palette, (void *) rawcurses_palette, sizeof(rawcurses_palette)))
        {
            rawcurses_fput_palette(stdout, rawcurses_old_palette);
        }
        rawcurses_fput_palette_reset(stdout);
        if (rawcurses_reset_palette_buflen)
        {
            fputs(rawcurses_reset_palette_buf, stdout);
        }
    }
}

#undef endwin
#define endwin rawcurses_endwin
static void endwin(void)
{
    rawcurses_winsize_on_clear = 0;
    rawcurses_after_endwin = 1;
    rawcurses_attr = -1;
    refresh();
    if (rawcurses_nw && rawcurses_nh)
    {
        rawcurses_w = rawcurses_nw;
        rawcurses_h = rawcurses_nh;
    }
    rawcurses_palette_reset();
    attrset(0);
    if (rawcurses_stdio)
    {
#ifdef SIGTSTP
        rawcurses_got_sigtstp = 0;
        if (rawcurses_old_sigtstp_handler != SIG_ERR)
        {
            signal(SIGTSTP, rawcurses_old_sigtstp_handler);
        }
#endif
#ifdef SIGINT
        rawcurses_got_sigint = 0;
        if (rawcurses_old_sigint_handler != SIG_ERR)
        {
            signal(SIGINT, rawcurses_old_sigint_handler);
        }
#endif
        if (rawcurses_stdio_old_shortname) rawcurses_fput_icon_name(stdout, rawcurses_stdio_old_shortname);
        if (rawcurses_stdio_old_title) rawcurses_fput_title(stdout, rawcurses_stdio_old_title);
        if (rawcurses_civis) rawcurses_fput_cnorm(stdout);
        rawcurses_fput_rmcup(stdout, rawcurses_h);
        fflush(stdout);
        if (rawcurses_stdio_new_shortname)
        {
            free((void *) rawcurses_stdio_new_shortname);
            rawcurses_stdio_new_shortname = NULL;
        }
        if (rawcurses_stdio_new_title)
        {
            free((void *) rawcurses_stdio_new_title);
            rawcurses_stdio_new_title = NULL;
        }
        if (rawcurses_winsize_pending && rawcurses_got_winsize)
        {
            int ch;

            usleep(RAWCURSES_RESIZE_QUIESCE_USECS);
            while ((ch = rawcurses_getch()) != ERR)
            {
                if (ch != KEY_RESIZE)
                {
                    rawcurses_ungetch_buffer = ch;
                    break;
                }
            }
        }
    }
#if USE_WINCONSOLE
    if (! rawcurses_stdio)
    {
        SetConsoleWindowInfo(rawcurses_stdout, TRUE, &rawcurses_old_size);
        if (rawcurses_civis) SetConsoleCursorInfo(rawcurses_stdout, &rawcurses_old_cursorinfo);
        SetConsoleTitle(rawcurses_old_title);
        usleep(RAWCURSES_TITLE_QUIESCE_USECS);
        if (rawcurses_old_screen)
        {
            COORD upper_left;
            SMALL_RECT write_region;
            COORD old_screen_size;

            old_screen_size.X = rawcurses_old_size.Right + 1;
            old_screen_size.Y = rawcurses_old_size.Bottom + 1;
            upper_left.X = 0;
            upper_left.Y = 0;
            write_region = rawcurses_old_size;
            WriteConsoleOutput(rawcurses_stdout,
                               rawcurses_old_screen,
                               old_screen_size,
                               upper_left,
                               &write_region);
            free((void *) rawcurses_old_screen);
            rawcurses_old_screen = NULL;
        }
        if ((rawcurses_old_x >= 0)
            &&
            (rawcurses_old_y >= 0))
        {
            COORD coord;

            coord.X = rawcurses_old_x;
            coord.Y = rawcurses_old_y;
            SetConsoleCursorPosition(rawcurses_stdout, coord);
            rawcurses_old_x = -1;
            rawcurses_old_y = -1;
        }
    }
    if (rawcurses_old_mode_valid)
    {
        SetConsoleMode(rawcurses_stdin, rawcurses_old_mode);
    }
#endif /* USE_WINCONSOLE */
#if USE_SGTTY
    ioctl(fileno(stdin), TIOCSETP, &rawcurses_old_sgtty);
#endif
#if USE_TERMIOS
    tcsetattr(fileno(stdin), TCSANOW, &rawcurses_old_tty);
#endif
#if USE_AROSCONSOLE
    if (rawcurses_stdio_aroscon)
    {
        SetMode(Input(), rawcurses_stdio_aroscon_oldmode);
    }
#endif
}

static int refresh(void)
{
    rawcurses_palette_snoop();
    if (rawcurses_stdio)
    {
        int x, y;

        x = rawcurses_x;
        y = rawcurses_y;
        if ((! rawcurses_civis)
            &&
            (! rawcurses_cursor_visibility)
            &&
            (! rawcurses_after_endwin))
        {
            getyx(stdscr, y, x);
            move(rawcurses_h - 1, rawcurses_w - 1);
        }
        fflush(stdout);
        if ((! rawcurses_civis)
            &&
            (! rawcurses_cursor_visibility)
            &&
            (! rawcurses_after_endwin))
        {
            move(y, x);
        }
    }
    return OK;
}

static int rawcurses_getch(void)
{
    int ret = ERR;
    int soft_ERR;

#ifdef SIGTSTP
    if (rawcurses_stdio && rawcurses_got_sigtstp)
    {
        attr_t old_attr;

        old_attr = rawcurses_attr;
        signal(SIGTSTP, rawcurses_old_sigtstp_handler);
        rawcurses_got_sigtstp = 0;
        if (rawcurses_civis) rawcurses_fput_cnorm(stdout);
        rawcurses_palette_reset();
        attrset(0);
        rawcurses_fput_rmcup(stdout, rawcurses_h);
        fflush(stdout);
#if USE_WINCONSOLE
        if (rawcurses_old_mode_valid)
        {
            SetConsoleMode(rawcurses_stdin, rawcurses_old_mode);
        }
#endif /* USE_WINCONSOLE */
#if USE_SGTTY
        ioctl(fileno(stdin), TIOCSETP, &rawcurses_old_sgtty);
#endif
#if USE_TERMIOS
        tcsetattr(fileno(stdin), TCSANOW, &rawcurses_old_tty);
#endif
#if USE_AROSCONSOLE
        if (rawcurses_stdio_aroscon)
        {
            SetMode(Input(), rawcurses_stdio_aroscon_oldmode);
        }
#endif
        raise(SIGTSTP);
#if USE_WINCONSOLE
        if (rawcurses_old_mode_valid)
        {
            SetConsoleMode(rawcurses_stdin, ENABLE_WINDOW_INPUT);
        }
#endif /* USE_WINCONSOLE */
#if USE_AROSCONSOLE
        if (rawcurses_stdio_aroscon)
        {
            if (SetMode(Input(), 1) != DOSTRUE)
            {
                rawcurses_stdio_aroscon_oldmode = 1;
            }
            else
            {
                rawcurses_stdio_aroscon_oldmode = 0;
            }
        }
#endif
#if USE_TERMIOS
        tcsetattr(fileno(stdin), TCSANOW, &rawcurses_tty);
#endif
#if USE_SGTTY
        ioctl(fileno(stdin), TIOCSETP, &rawcurses_sgtty);
#endif
        signal(SIGTSTP, rawcurses_sigtstp_handler);
        if (rawcurses_palette_ever_changed) rawcurses_fput_palette(stdout, rawcurses_palette);
        rawcurses_fput_smcup(stdout);
        if (rawcurses_civis)
        {
            rawcurses_cursor_visibility ? rawcurses_fput_cnorm(stdout) : rawcurses_fput_civis(stdout);
        }
        attrset(old_attr);
        fflush(stdout);
        return KEY_RESIZE;
    }
#endif
#ifdef SIGINT
    if (rawcurses_stdio && rawcurses_got_sigint)
    {
        attr_t old_attr;

        old_attr = rawcurses_attr;
        signal(SIGINT, rawcurses_old_sigint_handler);
        rawcurses_got_sigint = 0;
        if (rawcurses_civis) rawcurses_fput_cnorm(stdout);
        rawcurses_palette_reset();
        attrset(0);
        rawcurses_fput_rmcup(stdout, rawcurses_h);
        fflush(stdout);
#if USE_WINCONSOLE
        if (rawcurses_old_mode_valid)
        {
            SetConsoleMode(rawcurses_stdin, rawcurses_old_mode);
        }
#endif /* USE_WINCONSOLE */
#if USE_SGTTY
        ioctl(fileno(stdin), TIOCSETP, &rawcurses_old_sgtty);
#endif
#if USE_TERMIOS
        tcsetattr(fileno(stdin), TCSANOW, &rawcurses_old_tty);
#endif
#if USE_AROSCONSOLE
        if (rawcurses_stdio_aroscon)
        {
            SetMode(Input(), rawcurses_stdio_aroscon_oldmode);
        }
#endif
        raise(SIGINT);
#if USE_WINCONSOLE
        if (rawcurses_old_mode_valid)
        {
            SetConsoleMode(rawcurses_stdin, ENABLE_WINDOW_INPUT);
        }
#endif /* USE_WINCONSOLE */
#if USE_AROSCONSOLE
        if (rawcurses_stdio_aroscon)
        {
            if (SetMode(Input(), 1) != DOSTRUE)
            {
                rawcurses_stdio_aroscon_oldmode = 1;
            }
            else
            {
                rawcurses_stdio_aroscon_oldmode = 0;
            }
        }
#endif
#if USE_TERMIOS
        tcsetattr(fileno(stdin), TCSANOW, &rawcurses_tty);
#endif
#if USE_SGTTY
        ioctl(fileno(stdin), TIOCSETP, &rawcurses_sgtty);
#endif
        signal(SIGINT, rawcurses_sigint_handler);
        if (rawcurses_palette_ever_changed) rawcurses_fput_palette(stdout, rawcurses_palette);
        rawcurses_fput_smcup(stdout);
        if (rawcurses_civis)
        {
            rawcurses_cursor_visibility ? rawcurses_fput_cnorm(stdout) : rawcurses_fput_civis(stdout);
        }
        attrset(old_attr);
        fflush(stdout);
        return KEY_RESIZE;
    }
#endif
    soft_ERR = 0;
    do
    {
        if (rawcurses_ungetch_buffer != ERR)
        {
            ret = rawcurses_ungetch_buffer;
            rawcurses_ungetch_buffer = ERR;
            return ret;
        }
#if USE_WINCONSOLE
        {
            DWORD nevents;

            if (GetNumberOfConsoleInputEvents(rawcurses_stdin, &nevents))
            {
                if (nevents)
                {
                    INPUT_RECORD irec;

                    ReadConsoleInput(rawcurses_stdin, &irec, 1, &nevents);
                    if (nevents)
                    {
                        switch (irec.EventType)
                        {
                        case KEY_EVENT:
                            if (irec.Event.KeyEvent.bKeyDown)
                            {
                                switch (irec.Event.KeyEvent.wVirtualKeyCode)
                                {
                                case VK_LEFT:  return KEY_LEFT;
                                case VK_RIGHT: return KEY_RIGHT;
                                case VK_UP:    return KEY_UP;
                                case VK_DOWN:  return KEY_DOWN;
                                case VK_SHIFT:
                                case VK_CONTROL:
                                case VK_MENU:
                                case VK_LSHIFT:
                                case VK_LCONTROL:
                                case VK_LMENU:
                                case VK_RSHIFT:
                                case VK_RCONTROL:
                                case VK_RMENU:
                                    return ERR;
                                }
#ifdef UNICODE
                                ret = (unsigned short) irec.Event.KeyEvent.uChar.UnicodeChar;
#else
                                ret = (unsigned char) irec.Event.KeyEvent.uChar.AsciiChar;
#endif
                            }
                            break;
                        case WINDOW_BUFFER_SIZE_EVENT:
                            if (1)
                            {
                                int w, h;

                                w = irec.Event.WindowBufferSizeEvent.dwSize.X;
                                h = irec.Event.WindowBufferSizeEvent.dwSize.Y;
                                if ((w >= 1)
                                    &&
                                    (h >= 1)
                                    &&
                                    ((w != rawcurses_w)
                                     ||
                                     (h != rawcurses_h))
                                    &&
                                    ! (rawcurses_nw && rawcurses_nh))
                                {
                                    rawcurses_nw = w;
                                    rawcurses_nh = h;
                                    return KEY_RESIZE;
                                }
                                return ERR;
                            }
                            break;
                        default:
                            return ERR;
                        }
                    }
                }
            }
            else
            {
                unsigned char buf[2];
                DWORD nbytes;
                struct stat st;

                st.st_size = 0;
                if ((! rawcurses_stdio) || (fstat(fileno(stdin), &st) || st.st_size))
                {
                    if (ReadFile(rawcurses_stdin, (LPVOID) buf, 1, &nbytes, NULL))
                    {
                        if (nbytes) ret = buf[0];
                        else ret = ERR;
                    }
                }
            }
        }
#else
        {
            unsigned char buf[2];
            int avail;

            avail = -1;
#if USE_CONIO_INPUT
            if ((avail == -1) && rawcurses_stdio_conio_input)
            {
                if (kbhit())
                {
                    avail = 1;
                }
                else
                {
                    avail = 0;
                }
            }
#endif /* USE_CONIO_INPUT */
#if USE_TOSCONSOLE
            if ((avail == -1) && rawcurses_stdio_stcon)
            {
                if (Cconis())
                {
                    avail = 1;
                }
                else
                {
                    avail = 0;
                }
            }
#endif
#if USE_AROSCONSOLE
            if ((avail == -1) && rawcurses_stdio_aroscon)
            {
                if (WaitForChar(Input(), 1))
                {
                    avail = 1;
                }
                else
                {
                    avail = 0;
                }
            }
#endif
#if HAVE_SELECT
            if (avail == -1)
            {
                fd_set readfds, writefds, exceptfds;
                struct timeval timeout;
                int rv;

                FD_ZERO(&readfds);
                FD_ZERO(&writefds);
                FD_ZERO(&exceptfds);
                FD_SET(fileno(stdin), &readfds);
                memset((void *) &timeout, 0, sizeof(timeout));
                timeout.tv_sec = 0;
                timeout.tv_usec = 0;
                rv = select(fileno(stdin) + 1, &readfds, &writefds, &exceptfds, &timeout);
                if (rv > 0)
                {
                    avail = 1;
                }
                else if (rv == 0)
                {
                    avail = 0;
                }
            }
#endif
#if USE_CONIO_INPUT
            if (rawcurses_stdio_conio_input)
            {
                if (avail)
                {
                    ret = getch();
                    if (ret == 0)
                    {
                        ret = getch();
                        switch (ret)
                        {
                        case 0x00:
                        case 0x03:
                            return 0;
                        case 0x48:
                            return KEY_UP;
                        case 0x4b:
                            return KEY_LEFT;
                        case 0x4d:
                            return KEY_RIGHT;
                        case 0x50:
                            return KEY_DOWN;
                        default:
                            ret = ERR;
                        }
                    }
                }
            }
            else
#endif /* USE_CONIO_INPUT */
#if USE_TOSCONSOLE
            if (rawcurses_stdio_stcon)
            {
                if (avail)
                {
                    unsigned long kcode;

                    kcode = (unsigned long) Crawcin();
                    if (kcode & 0xffU)
                    {
                        ret = (int) (kcode & 0xffU);
                    }
                    else
                    {
                        /* FIXME: modifiers are ignored right now. They should be a bitmask of:
                         * right shift (1)
                         * left shift (2)
                         * control (4)
                         * alternate (8)
                         * caps lock (16)
                         */
                        unsigned char kmod;

                        kcode >>= 16;
                        kmod = kcode >> 8;
                        kcode = kcode & 0xff;
                        switch (kcode)
                        {
                        case K_ESC:
                            return (unsigned char) '\x1b';
                        case K_RET:
                            return (unsigned char) '\r';
                        case K_DEL:
                            return (unsigned char) '\x7f';
                        case CURS_UP:
                            return KEY_UP;
                        case CURS_DN:
                            return KEY_DOWN;
                        case CURS_RT:
                            return KEY_RIGHT;
                        case CURS_LF:
                            return KEY_LEFT;
                        case KP_MINUS:
                            return (unsigned char) '-';
                        case KP_PLUS:
                            return (unsigned char) '+';
                        case KP_LP:
                            return (unsigned char) '(';
                        case KP_RP:
                            return (unsigned char) ')';
                        case KP_SLASH:
                            return (unsigned char) '/';
                        case KP_STAR:
                            return (unsigned char) '*';
                        case KP_0:
                            return (unsigned char) '0';
                        case KP_1:
                            return (unsigned char) '1';
                        case KP_2:
                            return (unsigned char) '2';
                        case KP_3:
                            return (unsigned char) '3';
                        case KP_4:
                            return (unsigned char) '4';
                        case KP_5:
                            return (unsigned char) '5';
                        case KP_6:
                            return (unsigned char) '6';
                        case KP_7:
                            return (unsigned char) '7';
                        case KP_8:
                            return (unsigned char) '8';
                        case KP_9:
                            return (unsigned char) '9';
                        case KP_DOT:
                            return (unsigned char) '.';
                        case KP_ENTER:
                            return (unsigned char) '\n';
                        default:
                            ret = ERR;
                            soft_ERR = 1;
                            continue;
                        }
                    }
                }
            }
            else
#endif /* USE_TOSCONSOLE */
            {
#ifdef FIONREAD
                if (! avail) ioctl(fileno(stdin), FIONREAD, &avail);
#endif /* defined(FIONREAD) */
                if (avail && (read(fileno(stdin), (void *) buf, 1) > 0))
                {
                    ret = buf[0];
                }
            }
        }
#endif
        soft_ERR = (ret == ERR) ? 0 : 1;
        if (rawcurses_stdio && rawcurses_stdio_utf8 && (ret != ERR))
        {
            int looks_like_st;

            looks_like_st = (ret == 0x9c);
            /* decode UTF-8 input; for now illegal sequences map to
             * U+FFFD REPLACEMENT CHARACTER */

            /* FIXME: we should probably generate one U+FFFD
             * REPLACEMENT CHARACTER per byte in an illegal
             * sequence rather than one per sequence */
            if ((ret <= 0x7f) || (ret >= 0x100))
            {
                if (rawcurses_stdio_utf8_remaining)
                {
                    rawcurses_ungetch_buffer = ret;
                    rawcurses_stdio_utf8_state = 0UL;
                    rawcurses_stdio_utf8_remaining = 0;
                    ret = 0xfffd;
                }
            }
            else if ((ret >= 0xc2) && (ret <= 0xf4))
            {
                int mask;
                int remaining;

                mask = 0x80;
                remaining = -1;
                while (ret & mask)
                {
                    ret &= ~mask;
                    mask >>= 1;
                    remaining ++;
                }
                if (rawcurses_stdio_utf8_remaining)
                {
                    rawcurses_stdio_utf8_state = ret;
                    ret = 0xfffd;
                }
                else
                {
                    rawcurses_stdio_utf8_state = ret;
                    ret = ERR;
                }
                rawcurses_stdio_utf8_remaining = remaining;
            }
            else if ((ret >= 0x80) && (ret <= 0xbf) && rawcurses_stdio_utf8_remaining)
            {
                rawcurses_stdio_utf8_state <<= 6;
                rawcurses_stdio_utf8_state |= ret & 0x3f;
                rawcurses_stdio_utf8_remaining --;
                ret = ERR;
                if (! rawcurses_stdio_utf8_remaining)
                {
                    ret = rawcurses_stdio_utf8_state;
                    rawcurses_stdio_utf8_state = 0;
                    if (((ret & 0xfffeL) == 0xfffeL)
                        ||
                        (ret > 0x10fffdL)
                        ||
                        ((ret >= 0xd800) && (ret <= 0xdfff)))
                    {
                        ret = 0xfffd;
                    }
                }
            }
            else
            {
                rawcurses_stdio_utf8_state = 0UL;
                rawcurses_stdio_utf8_remaining = 0UL;
                ret = 0xfffd;
            }
            if (ret == ERR)
            {
                continue;
            }
            /* an otherwise invalid 0x9c might be a mangled ST */
            if (looks_like_st
                &&
                ((rawcurses_input_state == RAWCURSES_INPUT_STATE_OSC)
                 ||
                 (rawcurses_input_state == RAWCURSES_INPUT_STATE_OSC_ESC))
                &&
                (ret == 0xfffd))
            {
                ret = 0x9c;
            }
            /* interactive debugging of the UTF-8 decoder */
            if (rawcurses_debug_utf8)
            {
                int y, x;
                attr_t a;
                static const char *hex = "0123456789ABCDEF";

                a = rawcurses_attr;
                rawcurses_getyx(&y, &x);
                attrset(0);
                rawcurses_debug_utf8_y %= LINES;
                move(rawcurses_debug_utf8_y, 0);
                rawcurses_debug_utf8_y ++;
                if (ret < 0x100000L) addch(' ');
                if (ret < 0x10000L) addch(' ');
                addstr("U+");
                if (ret >= 0x100000L) addch(hex[(ret >> 20) & 0xf]);
                if (ret >= 0x10000L) addch(hex[(ret >> 16) & 0xf]);
                addch(hex[(ret >> 12) & 0xf]);
                addch(hex[(ret >> 8) & 0xf]);
                addch(hex[(ret >> 4) & 0xf]);
                addch(hex[ret & 0xf]);
                addch(' ');
                if (((ret >= 0x20) && (ret <= 0x7e))
                    ||
                    ((ret >= 0xa0) && (ret <= 0x10fffdL)))
                {
                    addch(ret);
                }
                rawcurses_debug_utf8_y %= LINES;
                move(rawcurses_debug_utf8_y, 0);
                addstr("           ");
                move(y, x);
                attrset(a);
            }
        }
        if ((ret == 0x7f)
            &&
            (rawcurses_input_state != RAWCURSES_INPUT_STATE_DEFAULT)
            &&
            (rawcurses_input_state != RAWCURSES_INPUT_STATE_IGNORE_CR))
        {
            /* ignore RUBout inside an ESCape sequence */
            ret = ERR;
        }
        if (rawcurses_stdio_adm3a)
        {
            switch (ret)
            {
            case '\b': /* ADM3A */
                return KEY_LEFT;
            case '\n': /* ADM3A */
                return KEY_DOWN;
            case '\v': /* ADM3A */
                return KEY_UP;
            case '\f': /* ADM3A */
                return KEY_RIGHT;
            }
        }
        if ((ret >= 0x20) || (ret == 0x1b) || (ret == ERR)
            ||
            ((rawcurses_input_state == RAWCURSES_INPUT_STATE_IGNORE_CR)
             &&
             ((ret == '\r')
              ||
              (ret == '\n')))
            ||
            (rawcurses_stdio_st52 && (ret == '\x1e'))
            ||
            (((rawcurses_input_state == RAWCURSES_INPUT_STATE_OSC)
              ||
              (rawcurses_input_state == RAWCURSES_INPUT_STATE_OSC_ESC))
             &&
             (ret == '\7')))
        {
            switch (rawcurses_input_state)
            {
            case RAWCURSES_INPUT_STATE_ET52_KEY:
                rawcurses_input_state = RAWCURSES_INPUT_STATE_DEFAULT;
                switch (ret)
                {
                case 'A': /* ET52 */
                    ret = KEY_UP;
                    break;
                case 'B': /* ET52 */
                    ret = KEY_DOWN;
                    break;
                case 'C': /* ET52 */
                    ret = KEY_RIGHT;
                    break;
                case 'D': /* ET52 */
                    ret = KEY_LEFT;
                    break;
                default:
                    if (rawcurses_ungetch_buffer == ERR)
                    {
                        rawcurses_ungetch_buffer = ret;
                    }
                    ret = 0x1e;
                }
            case RAWCURSES_INPUT_STATE_IDENTITY_VT52:
                if (rawcurses_input_state == RAWCURSES_INPUT_STATE_IDENTITY_VT52)
                {
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_DEFAULT;
                    switch (ret)
                    {
                    case ERR:
                        rawcurses_input_state = RAWCURSES_INPUT_STATE_IDENTITY_VT52;
                        break;
                    default:
                        if ((ret >= 'A') && (ret <= '_'))
                        {
                            /* "I am a VT52" or some such
                             * nonsense. Probably an emulator lying to us,
                             * but we pretend to care... */
                            rawcurses_stdio_vt52 = 1;
                            rawcurses_stdio_adm3a = 0;
                            ret = ERR;
                        }
                    }
                }
                if (rawcurses_input_state != RAWCURSES_INPUT_STATE_DEFAULT)
                {
                    break;
                }
            case RAWCURSES_INPUT_STATE_IGNORE_CR:
                if (rawcurses_input_state == RAWCURSES_INPUT_STATE_IGNORE_CR)
                {
                    /* ignore the erroneous post-position-report CR
                     * from ANSI.SYS, which an input cooker might (or
                     * might not) have turned into LF somewhere along
                     * the way. */
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_DEFAULT;
                    if ((ret == '\r') || (ret == '\n')) ret = ERR;
                }
            case RAWCURSES_INPUT_STATE_ETERM_KEY:
                if (rawcurses_input_state == RAWCURSES_INPUT_STATE_ETERM_KEY)
                {
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_DEFAULT;
                    switch (ret)
                    {
                    case 'A': /* eterm (Emacs terminal emulator) */
                        ret = KEY_UP;
                        break;
                    case 'B': /* eterm (Emacs terminal emulator) */
                        ret = KEY_DOWN;
                        break;
                    case 'C': /* eterm (Emacs terminal emulator) */
                        ret = KEY_RIGHT;
                        break;
                    case 'D': /* eterm (Emacs terminal emulator) */
                        ret = KEY_LEFT;
                        break;
                    default:
                        /* FIXME: should support other keys too */
                        if ((ret >= ' ') && (ret <= '~'))
                        {
                            ret = ERR;
                        }
                    }
                }
            case RAWCURSES_INPUT_STATE_DEFAULT:
                switch (ret)
                {
                case 0x1e:
                    if (rawcurses_stdio_st52)
                    {
                        rawcurses_input_state = RAWCURSES_INPUT_STATE_ET52_KEY;
                        ret = ERR;
                    }
                    break;
                case 0x1b:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_ESC;
                    ret = ERR;
                    break;
                case 0x8f:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_SS3;
                    ret = ERR;
                    break;
                case 0x9b:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_CSI;
                    rawcurses_input_param[0] = '\0';
                    ret = ERR;
                    break;
                case 0x9d:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_OSC;
                    rawcurses_input_param[0] = '\0';
                    ret = ERR;
                    break;
                }
                break;
            case RAWCURSES_INPUT_STATE_ESC:
                rawcurses_input_state = RAWCURSES_INPUT_STATE_DEFAULT;
                switch (ret)
                {
                case 0x1b:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_ESC;
                    ret = ERR;
                    break;
                case 0x8f:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_SS3;
                    ret = ERR;
                    break;
                case 0x9b:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_CSI;
                    rawcurses_input_param[0] = '\0';
                    ret = ERR;
                    break;
                case 0x9d:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_OSC;
                    rawcurses_input_param[0] = '\0';
                    ret = ERR;
                    break;
                case '[':
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_CSI;
                    rawcurses_input_param[0] = '\0';
                    ret = ERR;
                    break;
                case 'O':
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_SS3;
                    ret = ERR;
                    break;
                case ']':
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_OSC;
                    rawcurses_input_param[0] = '\0';
                    ret = ERR;
                    break;
                case 'o':
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_ETERM_KEY;
                    ret = ERR;
                    break;
                case 'A': /* VT52 */
                    ret = KEY_UP;
                    break;
                case 'B': /* VT52 */
                    ret = KEY_DOWN;
                    break;
                case 'C': /* VT52 */
                    ret = KEY_RIGHT;
                    break;
                case 'D': /* VT52 */
                    ret = KEY_LEFT;
                    break;
                case '/':
                    ret = ERR;
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_IDENTITY_VT52;
                    break;
                case ERR:
                    ret = 0x1b;
                    break;
                case 0x1e:
                    if (rawcurses_stdio_st52)
                    {
                        rawcurses_input_state = RAWCURSES_INPUT_STATE_ET52_KEY;
                        ret = 0x1b;
                    }
                default:
                    ret = ERR;
                }
                break;
            case RAWCURSES_INPUT_STATE_CSI:
                rawcurses_input_state = RAWCURSES_INPUT_STATE_CSI_DONE;
                rawcurses_input_intermed[0] = '\0';
                switch (ret)
                {
                case 0x1b:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_ESC;
                    ret = ERR;
                    break;
                case 0x8f:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_SS3;
                    ret = ERR;
                    break;
                case 0x9b:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_CSI;
                    rawcurses_input_param[0] = '\0';
                    ret = ERR;
                    break;
                case 0x9d:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_OSC;
                    rawcurses_input_param[0] = '\0';
                    ret = ERR;
                    break;
                    /* parameter bytes: */
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
                case ';':
                case ':':
                case '<':
                case '=':
                case '>':
                case '?':
                    if ((strlen(rawcurses_input_param) + 1) < sizeof(rawcurses_input_param))
                    {
                        rawcurses_input_param[strlen(rawcurses_input_param) + 1] = '\0';
                        rawcurses_input_param[strlen(rawcurses_input_param)] = (char) ret;
                    }
                case ERR:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_CSI;
                    ret = ERR;
                    break;
                }
                if (rawcurses_input_state != RAWCURSES_INPUT_STATE_CSI_DONE)
                {
                    break;
                }
            case RAWCURSES_INPUT_STATE_CSI_DONE:
                rawcurses_input_state = RAWCURSES_INPUT_STATE_DEFAULT;
                switch (ret)
                {
                    /* intermediate bytes: */
                case ' ':
                case '!':
                case '\"':
                case '#':
                case '$':
                case '%':
                case '&':
                case '\'':
                case '(':
                case ')':
                case '*':
                case '+':
                case ',':
                case '-':
                case '.':
                case '/':
                    if ((strlen(rawcurses_input_intermed) + 1) < sizeof(rawcurses_input_intermed))
                    {
                        rawcurses_input_intermed[strlen(rawcurses_input_intermed) + 1] = '\0';
                        rawcurses_input_intermed[strlen(rawcurses_input_intermed)] = (char) ret;
                    }
                    ret = ERR;
                case ERR:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_CSI_DONE;
                    break;
                case 0x1b:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_ESC;
                    ret = ERR;
                    break;
                case 0x8f:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_SS3;
                    ret = ERR;
                    break;
                case 0x9b:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_CSI;
                    rawcurses_input_param[0] = '\0';
                    ret = ERR;
                    break;
                case 0x9d:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_OSC;
                    rawcurses_input_param[0] = '\0';
                    ret = ERR;
                    break;
                case 'A':
                    ret = ERR;
                    if (! strlen(rawcurses_input_intermed))
                        ret = KEY_UP;
                    break;
                case 'B':
                    ret = ERR;
                    if (! strlen(rawcurses_input_intermed))
                        ret = KEY_DOWN;
                    break;
                case 'C':
                    ret = ERR;
                    if (! strlen(rawcurses_input_intermed))
                        ret = KEY_RIGHT;
                    break;
                case 'D':
                    ret = ERR;
                    if (! strlen(rawcurses_input_intermed))
                        ret = KEY_LEFT;
                    break;
                case 'R':
                    ret = ERR;
                    if (! strlen(rawcurses_input_intermed))
                    {
                        int w, h;
                        size_t i;
                        int scanstate;

                        /* cursor position report */
                        rawcurses_input_state = RAWCURSES_INPUT_STATE_IGNORE_CR;
                        rawcurses_winsize_pending = 0;
                        rawcurses_got_winsize = 1;
                        w = 0;
                        h = 0;
                        scanstate = 0;
                        for (i = 0; (i < sizeof(rawcurses_input_param)) && rawcurses_input_param[i]; i ++)
                        {
                            if ((rawcurses_input_param[i] >= '0')
                                &&
                                (rawcurses_input_param[i] <= '9'))
                            {
                                if (scanstate == 0)
                                {
                                    h *= 10;
                                    h += (rawcurses_input_param[i] - '0');
                                }
                                else if (scanstate == 1)
                                {
                                    w *= 10;
                                    w += (rawcurses_input_param[i] - '0');
                                }
                            }
                            else if (rawcurses_input_param[i] == ';')
                            {
                                scanstate ++;
                            }
                            else
                            {
                                break;
                            }
                        }
                        if ((w >= 1)
                            &&
                            (h >= 1)
                            &&
                            ((w > 1) || (h > 1))
                            &&
                            ((w != rawcurses_w)
                             ||
                             (h != rawcurses_h))
                            &&
                            ! (rawcurses_nw && rawcurses_nh))
                        {
                            rawcurses_nw = w;
                            rawcurses_nh = h;
                            ret = KEY_RESIZE;
                        }
                        else
                        {
                            ret = ERR;
                            if (rawcurses_stdio_new_shortname && ! rawcurses_stdio_old_shortname)
                            {
                                rawcurses_fput_request_icon_name(stdout);
                                fflush(stdout);
                            }
                            else if (rawcurses_stdio_new_title && ! rawcurses_stdio_old_title)
                            {
                                rawcurses_fput_request_title(stdout);
                                fflush(stdout);
                            }
                        }
                        break;
                    }
                case 'c':
                    ret = ERR;
                    if (! strlen(rawcurses_input_intermed))
                    {
                        /* "I am a VTxxx" or some such
                         * nonsense. Probably an emulator lying to us,
                         * but we pretend to care... */
                        rawcurses_stdio_vt52 = 0;
                        rawcurses_stdio_adm3a = 0;
                    }
                default:
                    ret = ERR;
                }
                break;
            case RAWCURSES_INPUT_STATE_SS3:
                rawcurses_input_state = RAWCURSES_INPUT_STATE_DEFAULT;
                switch (ret)
                {
                case ERR:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_SS3;
                    break;
                case 0x1b:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_ESC;
                    ret = ERR;
                    break;
                case 0x8f:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_SS3;
                    ret = ERR;
                    break;
                case 0x9b:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_CSI;
                    rawcurses_input_param[0] = '\0';
                    ret = ERR;
                    break;
                case 0x9d:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_OSC;
                    rawcurses_input_param[0] = '\0';
                    ret = ERR;
                    break;
                case 'A':
                    ret = KEY_UP;
                    break;
                case 'B':
                    ret = KEY_DOWN;
                    break;
                case 'C':
                    ret = KEY_RIGHT;
                    break;
                case 'D':
                    ret = KEY_LEFT;
                    break;
                default:
                    ret = ERR;
                }
                break;
            case RAWCURSES_INPUT_STATE_OSC_ESC:
                if (rawcurses_input_state == RAWCURSES_INPUT_STATE_OSC_ESC)
                {
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_OSC;
                    switch (ret)
                    {
                    case ERR:
                        rawcurses_input_state = RAWCURSES_INPUT_STATE_OSC_ESC;
                        break;
                    case '\\':
                        ret = 0x9c;
                        break;
                    default:
                        if ((strlen(rawcurses_input_param) + 1) < sizeof(rawcurses_input_param))
                        {
                            rawcurses_input_param[strlen(rawcurses_input_param) + 1] = '\0';
                            rawcurses_input_param[strlen(rawcurses_input_param)] = '\x1b';
                        }
                    }
                }
                if (rawcurses_input_state != RAWCURSES_INPUT_STATE_OSC) break;
            case RAWCURSES_INPUT_STATE_OSC:
                switch (ret)
                {
                case ERR:
                    break;
                case '\7':
                case 0x9c:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_DEFAULT;
                    switch (rawcurses_input_param[0])
                    {
                    case 'l':
                        if (1)
                        {
                            if (! rawcurses_stdio_old_shortname)
                            {
                                memcpy((void *) rawcurses_stdio_old_shortname_buf, (void *) rawcurses_input_param, sizeof(rawcurses_input_param));
                                rawcurses_stdio_old_shortname = rawcurses_stdio_old_shortname_buf + 1;
                            }
                            if (rawcurses_stdio_new_shortname)
                            {
                                rawcurses_fput_icon_name(stdout, rawcurses_stdio_new_shortname);
                                free((void *) rawcurses_stdio_new_shortname);
                                rawcurses_stdio_new_shortname = NULL;
                                fflush(stdout);
                            }
                            if (rawcurses_stdio_new_title && ! rawcurses_stdio_old_title)
                            {
                                rawcurses_fput_request_title(stdout);
                                fflush(stdout);
                            }
                        }
                        break;
                    case 'L':
                        if (1)
                        {
                            if (! rawcurses_stdio_old_title)
                            {
                                memcpy((void *) rawcurses_stdio_old_title_buf, (void *) rawcurses_input_param, sizeof(rawcurses_input_param));
                                rawcurses_stdio_old_title = rawcurses_stdio_old_title_buf + 1;
                            }
                            if (rawcurses_stdio_new_title)
                            {
                                rawcurses_fput_title(stdout, rawcurses_stdio_new_title);
                                free((void *) rawcurses_stdio_new_title);
                                rawcurses_stdio_new_title = NULL;
                                fflush(stdout);
                            }
                        }
                        break;
                    case '4':
                        if (1)
                        {
                            if (rawcurses_reset_palette_seqs < COLORS)
                            {
                                rawcurses_reset_palette_seqs ++;
                                if ((rawcurses_reset_palette_buflen + strlen(OSC("4")) + strlen(rawcurses_input_param + 1) + strlen("\7") + 1)
                                    <
                                    sizeof(rawcurses_reset_palette_buf))
                                {
                                    if (! rawcurses_reset_palette_buflen)
                                    {
                                        *rawcurses_reset_palette_buf = '\0';
                                    }
                                    if ((strlen(rawcurses_reset_palette_buf) < strlen("\7"))
                                        ||
                                        memcmp((void *) (rawcurses_reset_palette_buf + strlen(rawcurses_reset_palette_buf) - strlen("\7")),
                                               (void *) ("\7"),
                                               strlen("\7")))
                                    {
                                        memcpy((void *) (rawcurses_reset_palette_buf + strlen(rawcurses_reset_palette_buf)),
                                               (void *) (OSC("4")),
                                               strlen(OSC("4")) + 1);
                                    }
                                    else
                                    {
                                        memcpy((void *) (rawcurses_reset_palette_buf
                                                         +
                                                         strlen(rawcurses_reset_palette_buf)
                                                         -
                                                         strlen("\7")),
                                               (void *) (""),
                                               strlen("") + 1);
                                    }
                                    memcpy((void *) (rawcurses_reset_palette_buf + strlen(rawcurses_reset_palette_buf)),
                                           (void *) (rawcurses_input_param + 1),
                                           strlen(rawcurses_input_param + 1) + 1);
                                    memcpy((void *) (rawcurses_reset_palette_buf + strlen(rawcurses_reset_palette_buf)),
                                           (void *) ("\7"),
                                           strlen("\7") + 1);
                                    rawcurses_reset_palette_buflen = strlen(rawcurses_reset_palette_buf);
                                }
                            }
                        }
                        break;
                    }
                    ret = ERR;
                    break;
                case 0x1b:
                    rawcurses_input_state = RAWCURSES_INPUT_STATE_OSC_ESC;
                    ret = ERR;
                    break;
                default:
                    if ((strlen(rawcurses_input_param) + 1) < sizeof(rawcurses_input_param))
                    {
                        rawcurses_input_param[strlen(rawcurses_input_param) + 1] = '\0';
                        rawcurses_input_param[strlen(rawcurses_input_param)] = (char) ret;
                    }
                    ret = ERR;
                    break;
                }
                break;
            }
        }
    }
    while (soft_ERR && (ret == ERR));
    return ret;
}

static int attrset(attr_t a)
{
    attr_t a_rgb;
    attr_t a_rgb_old;

    rawcurses_palette_snoop();
    a_rgb_old = rawcurses_old_attr_rgb;
    if ((! rawcurses_transparent)
        &&
        (! rawcurses_after_endwin))
    {
        a_rgb_old = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
    }
    if (a & RAWCURSES__A_COLOR)
    {
        a_rgb = rawcurses_pairs[PAIR_NUMBER(a)];
    }
    else
    {
        if (a & A_UNDERLINE)
        {
            if ((a_rgb_old & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)) == BACKGROUND_BLUE)
            {
                a_rgb = a_rgb_old;
            }
            else
            {
                a_rgb = FOREGROUND_BLUE | (a_rgb_old & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY));
            }
        }
        else if (a & A_DIM)
        {
            if ((a_rgb_old & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)) == BACKGROUND_INTENSITY)
            {
                a_rgb = a_rgb_old;
            }
            else
            {
                a_rgb = FOREGROUND_INTENSITY | (a_rgb_old & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY));
            }
        }
        else if (a & A_BOLD)
        {
            if ((a_rgb_old & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)) == (BACKGROUND_RED | BACKGROUND_GREEN))
            {
                a_rgb = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | (a_rgb_old & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY));
            }
            else
            {
                a_rgb = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | (a_rgb_old & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY));
            }
        }
        else
        {
            a_rgb = a_rgb_old;
        }
    }
    if (a & A_BOLD)
    {
        if ((a_rgb
             &
             (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY))
            !=
            (((a_rgb & FOREGROUND_RED) ? BACKGROUND_RED : 0)
             |
             ((a_rgb & FOREGROUND_GREEN) ? BACKGROUND_GREEN : 0)
             |
             ((a_rgb & FOREGROUND_BLUE) ? BACKGROUND_BLUE : 0)
             |
             BACKGROUND_INTENSITY))
        {
            a_rgb |= FOREGROUND_INTENSITY;
        }
    }
    if (a & (A_REVERSE | A_STANDOUT))
    {
        a_rgb = (
            ((a_rgb & FOREGROUND_RED) ? BACKGROUND_RED : 0)
            |
            ((a_rgb & FOREGROUND_GREEN) ? BACKGROUND_GREEN : 0)
            |
            ((a_rgb & FOREGROUND_BLUE) ? BACKGROUND_BLUE : 0)
            |
            ((a_rgb & FOREGROUND_INTENSITY) ? BACKGROUND_INTENSITY : 0)
            |
            ((a_rgb & BACKGROUND_RED) ? FOREGROUND_RED : 0)
            |
            ((a_rgb & BACKGROUND_GREEN) ? FOREGROUND_GREEN : 0)
            |
            ((a_rgb & BACKGROUND_BLUE) ? FOREGROUND_BLUE : 0)
            |
            ((a_rgb & BACKGROUND_INTENSITY) ? FOREGROUND_INTENSITY : 0));
    }
    a_rgb &= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
    if ((! rawcurses_transparent)
        &&
        (! rawcurses_after_endwin))
    {
        a = a_rgb;
    }
    if (a != rawcurses_attr)
    {
        rawcurses_attr = a;
#if USE_WINCONSOLE
        if (! rawcurses_stdio)
        {
            if (SetConsoleTextAttribute(rawcurses_stdout, a_rgb))
            {
                return OK;
            }
            return ERR;
        }
#endif
        if ((! rawcurses_transparent)
            &&
            (! rawcurses_after_endwin))
        {
            a |= RAWCURSES__A_COLOR;
        }
        rawcurses_fput_sgr(stdout, a, a_rgb);
    }
    return OK;
}

#undef standout
#define standout rawcurses_standout
static void standout(void)
{
    attrset(rawcurses_attr | A_STANDOUT);
}

#undef standend
#define standend rawcurses_standend
static void standend(void)
{
    attrset(0);
}

static int move(int y, int x) {
    if ((rawcurses_x != x)
        ||
        (rawcurses_y != y))
    {
#if USE_WINCONSOLE
        if (! rawcurses_stdio)
        {
            COORD coord;

            coord.X = x + rawcurses_csbi.srWindow.Left;
            coord.Y = y + rawcurses_csbi.srWindow.Top;
            if (SetConsoleCursorPosition(rawcurses_stdout, coord))
            {
                rawcurses_x = x;
                rawcurses_y = y;
                return OK;
            }
            return ERR;
        }
#endif
        if (rawcurses_stdio_relcup
            &&
            (rawcurses_fput_relcup(stdout, y - rawcurses_y, x - rawcurses_x) > 0))
        {
        }
        else
        {
            rawcurses_fput_cup(stdout, y, x);
        }
        rawcurses_x = x;
        rawcurses_y = y;
    }
    return OK;
}

#ifndef RAWCURSES_ASCII_REPLACEMENT_CHARACTER
#define RAWCURSES_ASCII_REPLACEMENT_CHARACTER '*'
#endif

static int wctoa(chtype ch)
{
    if (rawcurses_raw)
    {
#ifdef UNICODE
        if ((! rawcurses_stdio)
            ||
            rawcurses_stdio_utf8
            ||
            (ch == (chtype) (0xff & (unsigned char) ch)))
#endif
        {
            return ch;
        }
    }
    if (rawcurses_stdio && rawcurses_stdio_acs && (rawcurses_map_acs(ch) != (chtype) ERR))
    {
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
        case ACS_BLOCK:
        case RAWCURSES__ACS_THALF:
        case RAWCURSES__ACS_BHALF:
        case RAWCURSES__ACS_LHALF:
        case RAWCURSES__ACS_RHALF:
            return ch;
        }
        if ((ch == (chtype) ACS_BULLET)
            &&
            ! rawcurses_stdio_acs_nobullet)
        {
            return ch;
        }
    }
#ifdef UNICODE
#if USE_WINCONSOLE
    if (! rawcurses_stdio) return ch;
#endif
#else /* ! defined(UNICODE) */
    ch = (unsigned char) (char) ch;
#if USE_WINCONSOLE
    if ((! rawcurses_stdio) && (GetConsoleOutputCP() == 437)) return ch;
#endif
#endif /* ! defined(UNICODE) */
    switch (ch)
    {
    case ACS_BLOCK:
        ch = '#';
        break;
    case RAWCURSES__ACS_THALF:
        ch = '\"';
        break;
    case RAWCURSES__ACS_BHALF:
        ch = ',';
        break;
    case RAWCURSES__ACS_LHALF:
    case RAWCURSES__ACS_RHALF:
        ch = '#';
        break;
    case ACS_ULCORNER:
    case ACS_LLCORNER:
    case ACS_PLUS:
    case ACS_LTEE:
    case ACS_BTEE:
    case ACS_TTEE:
    case ACS_LRCORNER:
    case ACS_URCORNER:
    case ACS_RTEE:
        ch = '+';
        break;
    case ACS_HLINE:
        ch = '-';
        break;
    case ACS_VLINE:
        ch = '|';
        break;
#ifdef UNICODE
    case 0x2022:
    case 0x25a0:
    case 0x25cb:
    case 0x25cf:
    case 0x25d8:
        ch = 'o';
        break;
    case 0x30fb:
    case 0x00b7:
        ch = '.';
        break;
    case 0x3000:
        ch = ' ';
        break;
    case 0x2018:
    case 0x2019:
        ch = '\'';
        break;
    case 0x201c:
    case 0x201d:
        ch = '\"';
        break;
#endif
    default:
        if (ch == (chtype) ACS_BULLET)
        {
            ch = ((SWAPDOTS) ? '.' : 'o');
            break;
        }
#ifdef UNICODE
        if ((ch >= 0xff01)
            &&
            (ch <= 0xff5e))
        {
            ch = ch - 0xff00 + ' ';
        }
#endif
    }
    return ((ch == (ch & 0x7f)) ? ch : RAWCURSES_ASCII_REPLACEMENT_CHARACTER);
}

static int rawcurses_wcwidth(wchar_t wc)
{
    int defwidth = -1;

    /* U+3000 IDEOGRAPHIC SPACE gets special handling */
    if (wc == 0x3000) return 2;
    switch (wc)
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
    case ACS_BLOCK:
        defwidth = 1;
        break;
    case RAWCURSES__ACS_THALF:
    case RAWCURSES__ACS_BHALF:
    case RAWCURSES__ACS_LHALF:
    case RAWCURSES__ACS_RHALF:
        if (rawcurses_stdio && rawcurses_stdio_acs && (rawcurses_map_acs((chtype) wc) != (chtype) ERR))
        {
            defwidth = 1;
        }
        break;
    }
    if (((chtype) wc) == (chtype) ACS_BULLET)
    {
        defwidth = 1;
    }
    if ((defwidth > 0)
        &&
        rawcurses_stdio
        &&
        rawcurses_stdio_acs)
    {
        return defwidth;
    }
    if (! wc) return 0;
    if ((wctoa(wc) == RAWCURSES_ASCII_REPLACEMENT_CHARACTER) && (wc != RAWCURSES_ASCII_REPLACEMENT_CHARACTER)) return defwidth;
    if ((wc) == (wc & 0x1f)) return defwidth;
#if USE_CONIO
    if (rawcurses_stdio_conio)
    {
        if ((wc) != (wc & 0xff))
        {
            return defwidth;
        }
    }
#endif /* USE_CONIO */
    if ((rawcurses_builtin_wcwidth > 0)
        &&
        (wc >= 0x0080))
    {
        return rawcurses_builtin_wcwidth;
    }
#ifdef UNICODE
#if USE_WINCONSOLE
    {
        UINT codepage;

        codepage = rawcurses_stdio ? CP_ACP : GetConsoleOutputCP();
        if (codepage == 54936)
        {
            /* handle GB-18030 as if it were GB-2312 */
            codepage = 936;
        }
        if ((codepage != CP_UTF7)
            &&
            (codepage != CP_UTF8))
        {
            WCHAR wch;
            int len;
            BOOL failed;

            wch = wc;
            failed = FALSE;
            len = WideCharToMultiByte(
                codepage,
                0,
                &wch,
                1,
                NULL,
                0,
                NULL,
                &failed);
            if ((! failed)
                &&
                (len > 0))
            {
                return len;
            }
            if (! rawcurses_builtin_wcwidth)
            {
                return defwidth;
            }
        }
    }
#else /* ! USE_WINCONSOLE */
    if (! rawcurses_builtin_wcwidth)
    {
        int ret;

#if USE_CONIO
        if (rawcurses_stdio_conio)
        {
            ret = ((wc < 0x20) || (wc > 0xff)) ? -1 : 1;
        }
        else
#endif /* USE_CONIO */
        {
            ret = wcwidth(wc);
        }
        if (ret > 0) return ret;
        return defwidth;
    }
#endif /* ! USE_WINCONSOLE */
/* FIXME: this is a horrible hack! */
    if ((wc >= 0x0020) && (wc < 0x3000)) return 1;
    return 2;
#else /* ! defined(unicode) */
    return 1;
#endif /* ! defined(unicode) */
}

#undef wcwidth
#define wcwidth(wc) rawcurses_wcwidth(wc)

static int addch(chtype ch) {
    int wcw;

    rawcurses_palette_snoop();
    if (ch != (ch & A_CHARTEXT))
    {
        int ret;

        ch &= ~(rawcurses_attr & ch & ~A_CHARTEXT);
        attrset(rawcurses_attr | (ch & ~A_CHARTEXT));
        ret = addch(ch & A_CHARTEXT);
        attrset(rawcurses_attr & ~(ch & ~A_CHARTEXT));
        return ret;
    }
    if (ch == 0x3000)
    {
        /* U+3000 IDEOGRAPHIC SPACE gets special handling */
        addch(' ');
        ch = ' ';
    }
    else if (ch == '\n')
    {
        rawcurses_getyx(&rawcurses_y, &rawcurses_x);
        move(rawcurses_y + 1, 0);
        return OK;
    }
    else if (ch == '\r')
    {
        rawcurses_getyx(&rawcurses_y, &rawcurses_x);
        move(rawcurses_y, 0);
        return OK;
    }
    else if (ch == '\t')
    {
        rawcurses_getyx(&rawcurses_y, &rawcurses_x);
        while (rawcurses_x % 8)
        {
            addch(' ');
            rawcurses_getyx(&rawcurses_y, &rawcurses_x);
        }
        return OK;
    }
    else if (ch == '\7')
    {
        rawcurses_fput_bel(stdout);
        fflush(stdout);
        return OK;
    }
    wcw = wcwidth(ch);
    if (wcw < 1) return ERR;
    if ((rawcurses_x + wcw + (rawcurses_stdio && ((rawcurses_y + 1) == rawcurses_h))) > rawcurses_w)
    {
        return ERR;
    }
    rawcurses_x += wcw;
#if USE_WINCONSOLE
    if (! rawcurses_stdio)
    {
        DWORD buflen;
        TCHAR buf[2];
        chtype ch2 = ' ';

        switch (ch)
        {
        case ACS_ULCORNER:
        case ACS_LLCORNER:
        case ACS_PLUS:
        case ACS_LTEE:
        case ACS_BTEE:
        case ACS_TTEE:
            ch2 = ACS_HLINE;
            break;
        case '-':
        case ACS_HLINE:
        case 0xff0d:
            ch2 = ch;
            break;
        }
        buflen = 1;
        buf[0] = wctoa(ch);
        buf[1] = wctoa(ch2);
#ifndef UNICODE
        if (wcw > 1)
        {
            buflen = 2;
        }
#endif
        if (((rawcurses_x + (rawcurses_y == (rawcurses_h - 1))) > rawcurses_w)
            &&
            ((rawcurses_w * rawcurses_h) > 1))
        {
            return OK;
        }
        if ((rawcurses_x >= rawcurses_w)
            ||
            (rawcurses_y >= rawcurses_h))
        {
            return ERR;
        }
        if (WriteConsole(rawcurses_stdout, buf, buflen, &buflen, NULL))
        {
            if (buflen)
            {
                int ox, oy;
                int nx, ny;

                ox = rawcurses_x;
                oy = rawcurses_y;
                rawcurses_getyx(&ny, &nx);
                if ((nx % rawcurses_w) != (ox % rawcurses_w))
                {
                    while (-- wcw)
                    {
                        move(oy, ox - wcw);
                        WriteConsole(rawcurses_stdout, buf + 1, 1, &buflen, NULL);
                    }
                    move(oy, ox);
                }
                return OK;
            }
            return ERR;
        }
        return ERR;
    }
#endif
    {
        int wcw;
        int x, y;

        wcw = wcwidth((wchar_t) ch);
        if (wcw < 1)
        {
            wcw = 1;
            if ((ch < ' ')
                ||
                (ch > '~'))
            {
                ch = wctoa(ch);
                wcw = wcwidth((wchar_t) ch);
                if (wcw < 1)
                {
                    wcw = 1;
                    ch = RAWCURSES_ASCII_REPLACEMENT_CHARACTER;
                }
            }
        }
        if (wcw >= 2)
        {
            rawcurses_getyx(&y, &x);
            if (((x + wcw - 1 + (y == (rawcurses_h - 1))) <= rawcurses_w)
                ||
                ((rawcurses_w * rawcurses_h) <= 1))
            {
                int i;

                for (i = 0; i < wcw; i ++)
                {
#if USE_CONIO
                    if (rawcurses_stdio_conio)
                    {
                        putch(' ');
                    }
                    else
#endif
                    {
                        fputc(' ', stdout);
                    }
                }
                if (rawcurses_fput_relcup(stdout, 0, -wcw) <= 0)
                {
                    rawcurses_fput_cup(stdout, y, x);
                }
            }
        }
        while (wcw > 0)
        {
            chtype ch2 = ' ';
            int switch_to_acs = 0;
            int switch_to_reverse = 0;

            switch (ch)
            {
            case ACS_ULCORNER:
            case ACS_LLCORNER:
            case ACS_PLUS:
            case ACS_LTEE:
            case ACS_BTEE:
            case ACS_TTEE:
                ch2 = ACS_HLINE;
                break;
            case '-':
            case ACS_HLINE:
            case 0xff0d:
                ch2 = ch;
                break;
            }
            ch = wctoa(ch);
            rawcurses_getyx(&y, &x);
            if ((((x + (y == (rawcurses_h - 1))) > rawcurses_w)
                 ||
                 (y >= rawcurses_h))
                &&
                ((rawcurses_w * rawcurses_h) > 1))
            {
                break;
            }
            if (rawcurses_stdio && rawcurses_stdio_acs)
            {
                chtype acsch = rawcurses_map_acs(ch);
                if (acsch != (chtype) ERR)
                {
                    switch_to_acs = 1;
                    ch = acsch & ~A_REVERSE;
                    switch_to_reverse = (acsch & A_REVERSE) ? 1 : 0;
                }
            }
            if (switch_to_reverse) attrset(rawcurses_attr ^ A_REVERSE);
            if (switch_to_acs)
            {
                rawcurses_fput_smacs(stdout);
            }
#if USE_CONIO
            if (rawcurses_stdio_conio)
            {
                if (ch != (chtype) (0xff & (unsigned char) ch))
                {
                    ch = RAWCURSES_ASCII_REPLACEMENT_CHARACTER;
                }
                putch((int) (unsigned char) ch);
                wcw --;
            }
            else
#endif
            if ((rawcurses_raw) && (ch != (chtype) (0x7f & (unsigned char) ch)))
            {
#ifdef UNICODE
                if (rawcurses_stdio_utf8 && ! (switch_to_acs && rawcurses_stdio_cp437))
                {
                    fputc_utf8(ch, stdout);
                    if (wcw > 1)
                    {
                        if (! rawcurses_stdio_relcup) rawcurses_x = -1;
                        move(y, x);
                    }
                    wcw = 0;
                }
                else
#endif
                {
                    if (rawcurses_stdio_utf8)
                    {
                        fputc_utf8_cp437(0x100 | (int) (unsigned char) ch, stdout);
                        wcw --;
                    }
                    else
                    {
                        if (ch != (chtype) (0xff & (unsigned char) ch))
                        {
                            ch = RAWCURSES_ASCII_REPLACEMENT_CHARACTER;
                        }
                        fputc((int) (unsigned char) ch, stdout);
                        wcw --;
                    }
                }
            }
            else
            {
                fputc((int) (unsigned char) ch, stdout);
                wcw --;
            }
            if (switch_to_acs) rawcurses_fput_rmacs(stdout);
            if (switch_to_reverse) attrset(rawcurses_attr);
            ch = ch2;
        }
        if (rawcurses_x >= rawcurses_w)
        {
            rawcurses_x = rawcurses_w - 1;
            if (rawcurses_stdio_relcup
                &&
                (rawcurses_fput_relcup(stdout, -999, -999) > 0)
                &&
                (rawcurses_fput_relcup(stdout, rawcurses_y, rawcurses_h) > 0))
            {
            }
            else
            {
                rawcurses_fput_cup(stdout, rawcurses_y, rawcurses_x);
            }
        }
    }
    return OK;
}

#undef insch
#define insch rawcurses_insch
static int insch(chtype ch)
{
    /* FIXME: should implement this for WIN32 too */
    /* FIXME: this may fail silently -- do not trust it */
    if (rawcurses_stdio)
    {
        if (rawcurses_fput_ich(stdout, wcwidth(ch)) > 0)
        {
            int ret;
            int oy, ox;

            rawcurses_getyx(&oy, &ox);
            ret = addch(ch);
            move(oy, ox);
            return ret;
        }
    }
    return ERR;
}

#undef clrtoeol
#define clrtoeol rawcurses_clrtoeol
static int clrtoeol(void)
{
    if (rawcurses_stdio)
    {
        if ((rawcurses_fput_clrtoeol(stdout) > 0)
            &&
            rawcurses_transparent)
        {
            return OK;
        }
    }
    {
        int x;
        int ox, oy;

        rawcurses_getyx(&oy, &ox);
        for (x = ox; x < COLS; x ++)
        {
            move(oy, x);
            if (((oy < (LINES - 1)) || (x < (COLS - 1))) || ! rawcurses_stdio) addch(' ');
        }
        if (rawcurses_stdio
            &&
            (ox < (COLS - 1))
            &&
            (oy == (LINES - 1)))
        {
            move(oy, COLS - 2);
            insch(' ');
        }
        move(oy, ox);
    }
    return OK;
}

#undef clrtobot
#define clrtobot rawcurses_clrtobot
static int clrtobot(void) {
    if (rawcurses_stdio)
    {
        if ((rawcurses_fput_clrtobot(stdout) > 0)
            &&
            rawcurses_transparent)
        {
            return OK;
        }
    }
    {
        int y;
        int ox, oy;

        rawcurses_getyx(&oy, &ox);
        clrtoeol();
        for (y = oy + 1; y < LINES; y ++)
        {
            move(y, 0);
            clrtoeol();
        }
        move(oy, ox);
    }
    return OK;
}

static int addstr(const char *s) {
    while (*s)
    {
        addch((unsigned char) *s++);
    }
    return OK;
}

#undef mvprintw
#define mvprintw rawcurses_mvprintw
static int mvprintw(int y, int x, const char *s) {
    move(y, x);
    return addstr(s);
}

#undef resizeterm
#define resizeterm rawcurses_resizeterm
static int resizeterm(int y, int x)
{
    if (x && y
        &&
        (! (rawcurses_nw || rawcurses_nh)))
    {
        rawcurses_nw = x;
        rawcurses_nh = y;
    }
    if (rawcurses_nh)
    {
        y = rawcurses_nh;
    }
    if (rawcurses_nw)
    {
        x = rawcurses_nw;
    }
    if (! y) y = rawcurses_h;
    if (! x) x = rawcurses_w;
    if (x < 1) x = 80;
    if (y < 1) y = 24;
    rawcurses_h = y;
    rawcurses_w = x;
    return OK;
}

static void rawcurses_getyx(int *y, int *x)
{
#if USE_WINCONSOLE
    if (! rawcurses_stdio)
    {
        int old_left;
        int old_top;

        old_left = rawcurses_csbi.srWindow.Left;
        old_top = rawcurses_csbi.srWindow.Top;
        if (GetConsoleScreenBufferInfo(rawcurses_stdout, &rawcurses_csbi))
        {
            int w, h;

            w = rawcurses_csbi.srWindow.Right + 1 - rawcurses_csbi.srWindow.Left;
            h = rawcurses_csbi.srWindow.Bottom + 1 - rawcurses_csbi.srWindow.Top;
            if ((w >= 1)
                &&
                (h >= 1)
                &&
                ((((w != rawcurses_w)
                   ||
                   (h != rawcurses_h))
                  &&
                  ! (rawcurses_nw && rawcurses_nh))
                 ||
                 ((old_left != rawcurses_csbi.srWindow.Left)
                  ||
                  (old_top != rawcurses_csbi.srWindow.Top))))
            {
                rawcurses_nw = w;
                rawcurses_nh = h;
                if (rawcurses_ungetch_buffer == ERR)
                {
                    rawcurses_ungetch_buffer = KEY_RESIZE;
                }
            }
            rawcurses_x = rawcurses_csbi.dwCursorPosition.X - rawcurses_csbi.srWindow.Left;
            rawcurses_y = rawcurses_csbi.dwCursorPosition.Y - rawcurses_csbi.srWindow.Top;
        }
    }
#endif
    *y = rawcurses_y;
    *x = rawcurses_x;
}

#undef getch
#define getch rawcurses_getch

static int erase(void)
{
#if USE_WINCONSOLE
    if (! rawcurses_stdio)
    {
        COORD home;
        DWORD written;
        attr_t a_rgb_old;

        a_rgb_old = rawcurses_old_attr_rgb;
        if ((! rawcurses_transparent)
            &&
            (! rawcurses_after_endwin))
        {
            a_rgb_old = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
        }
        home.X = rawcurses_csbi.srWindow.Left;
        for (home.Y = rawcurses_csbi.srWindow.Top; home.Y <= rawcurses_csbi.srWindow.Bottom; home.Y ++)
        {
            FillConsoleOutputAttribute(rawcurses_stdout, a_rgb_old, rawcurses_csbi.srWindow.Right + 1 - rawcurses_csbi.srWindow.Left, home, &written);
            if (! FillConsoleOutputCharacter(rawcurses_stdout, ' ', rawcurses_csbi.srWindow.Right + 1 - rawcurses_csbi.srWindow.Left, home, &written) || ! written) break;
        }
        if (home.Y >= rawcurses_csbi.srWindow.Bottom) return OK;
        return ERR;
    }
#endif
    move(0, 0);
    if (rawcurses_stdio)
    {
        if ((rawcurses_fput_clear(stdout) > 0)
            &&
            rawcurses_transparent)
        {
            return OK;
        }
    }
    clrtobot();
    return OK;
}

#undef clear
#define clear rawcurses_clear
static int clear(void) {
    if (rawcurses_stdio && (! rawcurses_after_endwin) && (! rawcurses_winsize_pending) && rawcurses_got_winsize && rawcurses_winsize_on_clear)
    {
        rawcurses_fput_request_winsize(stdout);
        fflush(stdout);
        rawcurses_winsize_pending = 1;
    }
    return erase();
}

#endif /* MYMAN_RAWCURS_H_INCLUDED */
