/*
 * eficurs.h - EFI (Extensible Firmware Interface) console driver for the MyMan video game
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

/* work-arounds for EFI (Extensible Firmware Interface) */
#include <efi.h>
#include <efilib.h>

#include <wchar.h>

#undef timeval
#define timeval eficurses_timeval
struct timeval
{
    long tv_sec;
    long tv_usec;
};

#undef gettimeofday
#define gettimeofday eficurses_gettimeofday
static int gettimeofday(struct timeval *tv, void *tz)
{
    if (tv)
    {
        EFI_TIME now;
        UINT16 year;
        UINT8 month;

        if (EFI_ERROR(RT->GetTime(&now, NULL)))
        {
            return 1;
        }
        tv->tv_usec = (long) (((unsigned long) now.Nanosecond) / 1000UL);
        tv->tv_sec =
            ((long) (unsigned) now.Second) +
            60 * (((long) (unsigned) now.Minute) +
                  ((now.TimeZone == EFI_UNSPECIFIED_TIMEZONE)
                   ? 0L
                   : ((long) (unsigned) now.TimeZone)) +
                  60 * (((long) (unsigned) now.Hour) +
                        60 * ((long) (unsigned) now.Day)));
#define IS_LEAP_YEAR(year) ((! ((year) % 4)) && (((year) % 100) || ! ((year) % 400)))
        for (year = 1970; year < now.Year; year ++)
        {
            tv->tv_sec += 60L * 60L * 24L * (365L + IS_LEAP_YEAR(year) ? 1L : 0L);
        }
        for (month = 1; month < now.Month; month ++)
        {
            tv->tv_sec += 60L * 60L * 24L * ((month == 2)
                                             ? (IS_LEAP_YEAR(now.Year) ? 29L : 28L)
                                             : ((month % 2) ? 31L : 30L));
        }
    }
    return 0;
}

#undef usleep
#define usleep eficurses_usleep
static int usleep(unsigned long usecs)
{
    if (EFI_ERROR(BS->Stall(usecs)))
    {
        return 1;
    }
    return 0;
}

#ifndef access

#define access(fn,type) eficurses_access(fn)

static int eficurses_access(const char *fn)
{
    FILE *fp;

    fp = fopen(fn, "rb");
    if (fp)
    {
        fclose(fp);
        return 0;
    }
    return -1;
}

#endif

#ifndef isatty
#define isatty(fd) 0
#endif

#ifndef USE_ATTR
#define USE_ATTR 1
#endif

#ifndef USE_UNDERLINE
#define USE_UNDERLINE 1
#endif

#ifndef SWAPDOTS
#define SWAPDOTS 1
#endif

#ifndef USE_ACS
#define USE_ACS 1
#endif

#ifndef USE_RAW_UCS
#define USE_RAW_UCS 1
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
#define wcwidth(ch) eficurses_wcwidth(ch)

static int eficurses_ready = 0;

typedef unsigned long int eficurses_chtype;

#undef chtype
#define chtype eficurses_chtype

typedef eficurses_chtype eficurses_attr_t;

#undef attr_t
#define attr_t eficurses_attr_t

static UINTN eficurses_w = 0, eficurses_h = 0;

static UINTN eficurses_x = 0, eficurses_y = 0;

static eficurses_attr_t eficurses_attr = 0;

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

#define getyx(stdscr, y, x) ((y) = eficurses_y), ((x) = eficurses_x)

#define wrefresh(stdscr) refresh()

#define beep() eficurses_addch('\a')

#define use_env(x)

#define echo()

#define can_change_color() (0)

#define keypad(stdscr, x)

#define LINES (eficurses_h)

#define COLS (eficurses_w)

#undef KEY_RESIZE
#define KEY_RESIZE 0xef00

#undef KEY_UP
#define KEY_UP 0xf700

#undef KEY_DOWN
#define KEY_DOWN 0xf701

#undef KEY_LEFT
#define KEY_LEFT 0xf702

#undef KEY_RIGHT
#define KEY_RIGHT 0xf703

#define A_DIM 0x800000UL

#define A_UNDERLINE A_DIM

#define A_BOLD 0x400000UL

#define A_STANDOUT 0x200000UL

#define A_CHARTEXT 0x1fffffUL

#define A_REVERSE A_STANDOUT

#define EFICURSES___A_COLOR 0xff000000UL

#define COLOR_PAIRS 256

#define COLORS 16

#define COLOR_BLACK EFI_BLACK

#define COLOR_BLUE EFI_BLUE

#define COLOR_GREEN EFI_GREEN

#define COLOR_CYAN (COLOR_BLUE | COLOR_GREEN)

#define COLOR_RED EFI_RED

#define COLOR_MAGENTA (COLOR_RED | COLOR_BLUE)

#define COLOR_YELLOW (COLOR_RED | COLOR_GREEN)

#define COLOR_WHITE (COLOR_RED | COLOR_GREEN | COLOR_BLUE)

#define PEN_BRIGHT EFI_BRIGHT

#define _PAIR_SHIFT 24

#define COLOR_PAIR(p) (((eficurses_attr_t) p) << _PAIR_SHIFT)

#define PAIR_NUMBER(a) (((a) & EFICURSES___A_COLOR) >> _PAIR_SHIFT)

#define ACS_LRCORNER BOXDRAW_UP_LEFT

#define ACS_URCORNER BOXDRAW_DOWN_LEFT

#define ACS_ULCORNER BOXDRAW_DOWN_RIGHT

#define ACS_LLCORNER BOXDRAW_UP_RIGHT

#define ACS_PLUS BOXDRAW_VERTICAL_HORIZONTAL

#define ACS_HLINE BOXDRAW_HORIZONTAL

#define ACS_LTEE BOXDRAW_VERTICAL_RIGHT

#define ACS_RTEE BOXDRAW_VERTICAL_LEFT

#define ACS_BTEE BOXDRAW_UP_HORIZONTAL

#define ACS_TTEE BOXDRAW_DOWN_HORIZONTAL

#define ACS_VLINE BOXDRAW_VERTICAL

#define ACS_BULLET 0x00b7

#define ACS_BLOCK BLOCKELEMENT_FULL_BLOCK

static struct { short fg, bg; } eficurses_pairs[COLOR_PAIRS];

static int eficurses_wcwidth(eficurses_chtype ch)
{
    CHAR16 buf[3];

    ch &= A_CHARTEXT;
    if (ch > 0x10FFFFL) return -1;
    if (ch > 0xFFFFL)
    {
        buf[0] = (CHAR16) 0xD800L | ((ch - 0x10000L) >> 10);
        buf[1] = (CHAR16) 0xDC00L | ((ch - 0x10000L) & 0x3FFL);
        buf[2] = L'\0';
    }
    else
    {
        buf[0] = (CHAR16) ch;
        buf[1] = L'\0';
    }
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
    if (buf[1] == L'\0')
    {
        if (IsValidEfiCntlChar(buf[0]))
        {
            return 0;
        }
        if (ch < ' ')
        {
            return -1;
        }
        if (IsValidAscii(buf[0]))
        {
            return 1;
        }
        if (LibIsValidTextGraphics(buf[0], NULL, NULL))
        {
            return 1;
        }
    }
    if (! EFI_ERROR(ST->ConOut->TestString(ST->ConOut, buf)))
    {
        return 1;
    }
    return 0;
}

static int curs_set(x)
{
    if (! eficurses_ready) return ERR;
    ST->ConOut->EnableCursor(ST->ConOut, x ? TRUE : FALSE);
    return OK;
}

static int attrset(eficurses_attr_t a)
{
    if (! eficurses_ready) return ERR;
    eficurses_attr = a;
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
    if (! eficurses_ready) return ERR;
    return 1;
}

static int start_color(void)
{
    if (! eficurses_ready) return ERR;
    if (! has_colors()) return ERR;
    return OK;
}

static int init_pair(short i, short fg, short bg);

#define INITSCR_WITH_HINTS

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    int i;
    char *ignored = NULL;

    if (! w) w = 80;
    if (! h) h = 25;
    ST->ConOut->QueryMode(ST->ConOut,
                          ST->ConOut->Mode->Mode,
                          &eficurses_w,
                          &eficurses_h);
    eficurses_w = eficurses_w ? eficurses_w : w;
    eficurses_h = eficurses_h ? eficurses_h : h;
    eficurses_ready = 1;
    eficurses_attr = -1;
    for (i = 0; i < COLOR_PAIRS; i ++)
    {
        eficurses_pairs[i].fg = i ? (i % COLORS) : COLOR_WHITE;
        eficurses_pairs[i].bg = i / COLORS;
    }
    ST->ConOut->SetAttribute(ST->ConOut, EFI_TEXT_ATTR(EFI_LIGHTGRAY, EFI_BLACK));
    attrset(0);
}

static void endwin(void)
{
    eficurses_ready = 0;
    ST->ConOut->SetAttribute(ST->ConOut, EFI_TEXT_ATTR(EFI_LIGHTGRAY, EFI_BLACK));
}

static int move(int y, int x);

static int eficurses_addch(eficurses_chtype ch);

static int erase(void)
{
    if (! eficurses_ready) return ERR;
    attrset(0);
    move(0, 0);
    ST->ConOut->SetAttribute(ST->ConOut, EFI_TEXT_ATTR(EFI_LIGHTGRAY, EFI_BLACK));
    ST->ConOut->ClearScreen(ST->ConOut);
    ST->ConOut->SetCursorPosition(ST->ConOut, eficurses_x, eficurses_y);
    return OK;
}

static int resizeterm(int y, int x)
{
    if (! eficurses_ready) return ERR;
    ST->ConOut->QueryMode(ST->ConOut,
                          ST->ConOut->Mode->Mode,
                          &eficurses_w,
                          &eficurses_h);
    eficurses_w = eficurses_w ? eficurses_w : x;
    eficurses_h = eficurses_h ? eficurses_h : y;
    return OK;
}

static int eficurses_getch(void) {
    int ret = ERR;
    UINTN i, j;

    if (! eficurses_ready) return (eficurses_chtype) ERR;
    ST->ConOut->QueryMode(ST->ConOut,
                          ST->ConOut->Mode->Mode,
                          &i,
                          &j);
    if ((i != eficurses_w) || (j != eficurses_h))
    {
        return KEY_RESIZE;
    }
    if (! EFI_ERROR(WaitForSingleEvent(ST->ConIn->WaitForKey, 1ULL)))
    {
        EFI_INPUT_KEY keystroke;

        if (! EFI_ERROR(ST->ConIn->ReadKeyStroke(ST->ConIn, &keystroke)))
        {
            if (keystroke.UnicodeChar)
            {
                ret = keystroke.UnicodeChar;
            }
            else if (keystroke.ScanCode == SCAN_ESC)
            {
                ret = 0x1b;
            }
            else if (keystroke.ScanCode == SCAN_UP)
            {
                ret = KEY_UP;
            }
            else if (keystroke.ScanCode == SCAN_DOWN)
            {
                ret = KEY_DOWN;
            }
            else if (keystroke.ScanCode == SCAN_LEFT)
            {
                ret = KEY_LEFT;
            }
            else if (keystroke.ScanCode == SCAN_RIGHT)
            {
                ret = KEY_RIGHT;
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
#define getch eficurses_getch

#undef addch
#define addch eficurses_addch

static int refresh(void)
{
    if (! eficurses_ready) return OK;
    ST->ConOut->SetCursorPosition(ST->ConOut, eficurses_x, eficurses_y);
    return OK;
}

static int move(int y, int x)
{
    if (! eficurses_ready) return ERR;
    if (y < 0) return ERR;
    if (y > eficurses_h) return ERR;
    if (x < 0) return ERR;
    if (x > eficurses_w) return ERR;
    eficurses_y = y;
    eficurses_x = x;
    return OK;
}

static int insch(const eficurses_chtype ch)
{
    return ERR;
}

static int delch(void)
{
    return ERR;
}

static int init_pair(short i, short fg, short bg)
{
    if (! eficurses_ready) return ERR;
    if (! has_colors()) return ERR;
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    if ((fg < 0) || (fg > COLORS)) return ERR;
    if ((bg < 0) || (bg > COLORS)) return ERR;
    eficurses_pairs[i].fg = fg;
    eficurses_pairs[i].bg = bg;
    return OK;
}

static int pair_content(short i, short *fg, short *bg)
{
    if (! eficurses_ready) return ERR;
    if (! has_colors()) return ERR;
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    *fg = eficurses_pairs[i].fg;
    *bg = eficurses_pairs[i].bg;
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

static int eficurses_addch(eficurses_chtype ch)
{
    short fg, bg;

    if (! eficurses_ready) return ERR;
    if (ch & ~A_CHARTEXT)
    {
        int ret;
        eficurses_attr_t a;

        a = eficurses_attr;
        attrset((PAIR_NUMBER(ch & ~A_CHARTEXT) ? (a & ~COLOR_PAIR(PAIR_NUMBER(a))) : a) | (ch & ~A_CHARTEXT));
        ret = eficurses_addch(ch & A_CHARTEXT);
        attrset(a);
        return ret;
    }
    if (ch == (eficurses_chtype) (unsigned long) (unsigned char) '\a')
    {
        return ERR;
    }
    else if (ch == (eficurses_chtype) (unsigned long) (unsigned char) '\b')
    {
        if (eficurses_y && ! eficurses_x)
        {
            move(eficurses_y - 1, eficurses_w - 1);
        }
        else if (eficurses_x)
        {
            move(eficurses_y, eficurses_x - 1);
        }
        return OK;
    }
    else if (ch == (eficurses_chtype) (unsigned long) (unsigned char) '\n')
    {
        if ((eficurses_y + 1) < eficurses_h)
        {
            move(eficurses_y + 1, 0);
        }
        else
        {
            move(eficurses_y, 0);
        }
        return OK;
    }
    else if (ch == (eficurses_chtype) (unsigned long) (unsigned char) '\t')
    {
        do
        {
            eficurses_addch((eficurses_chtype) (unsigned long) (unsigned char) ' ');
        }
        while (eficurses_x % 8);
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
        pair_content(PAIR_NUMBER(eficurses_attr), &fg, &bg);
    }
    if (eficurses_attr & A_UNDERLINE)
    {
        fg = COLOR_BLUE;
        bg = COLOR_BLACK;
    }
    else if ((eficurses_attr & A_STANDOUT) && has_colors())
    {
        fg = COLOR_MAGENTA;
        bg = COLOR_BLACK;
    }
    else if ((eficurses_attr & A_BOLD) && has_colors())
    {
        fg = COLOR_YELLOW | PEN_BRIGHT;
        bg = COLOR_BLACK;
    }
    if (has_colors() && PAIR_NUMBER(eficurses_attr))
    {
        pair_content(PAIR_NUMBER(eficurses_attr), &fg, &bg);
    }
    if (eficurses_attr & A_BOLD)
    {
        fg |= PEN_BRIGHT;
    }
    if (eficurses_attr & (A_STANDOUT | A_REVERSE))
    {
        short tmp;

        tmp = bg;
        bg = fg;
        fg = tmp;
    }
    if (eficurses_x < 0) eficurses_x = 0;
    if (eficurses_x >= eficurses_w)
    {
        eficurses_x = 0;
        eficurses_y ++;
    }
    if (eficurses_y < 0) eficurses_y = 0;
    if (eficurses_y >= eficurses_h)
    {
        eficurses_y = eficurses_h - 1;
    }
    if (ch == ACS_BLOCK)
    {
        bg = COLOR_BLACK;
    }
    if (fg == bg)
    {
        fg = (bg != COLOR_BLACK) ? COLOR_BLACK : COLOR_WHITE;
    }
    /* EFI Console supports only 8 background colors */
    if (bg == (EFI_BRIGHT | COLOR_BLACK)) bg = COLOR_WHITE;
    bg = bg & ~EFI_BRIGHT;
    if (fg == bg)
    {
        fg = COLOR_BLACK;
    }
    /* EFI Console scrolls when the last character cell is updated */
    if ((eficurses_y < (eficurses_h - 1))
        ||
        (eficurses_x < (eficurses_w - 1)))
    {
        CHAR16 buf[3];

        if (ch > 0xFFFFL)
        {
            buf[0] = (CHAR16) 0xD800L | ((ch - 0x10000L) >> 10);
            buf[1] = (CHAR16) 0xDC00L | ((ch - 0x10000L) & 0x3FFL);
            buf[2] = L'\0';
        }
        else
        {
            buf[0] = (CHAR16) ch;
            buf[1] = L'\0';
        }
        if (((buf[1] != L'\0')
             ||
             ((! IsValidAscii(buf[0]))
              &&
              (! LibIsValidTextGraphics(buf[0], NULL, NULL))))
            &&
            EFI_ERROR(ST->ConOut->TestString(ST->ConOut, buf)))
        {
            switch (ch)
            {
            case ACS_LRCORNER:
            case ACS_URCORNER:
            case ACS_ULCORNER:
            case ACS_LLCORNER:
            case ACS_PLUS:
            case ACS_LTEE:
            case ACS_RTEE:
            case ACS_BTEE:
            case ACS_TTEE:
                buf[0] = L'+';
                buf[1] = L'\0';
                break;
            case ACS_HLINE:
                buf[0] = L'-';
                buf[1] = L'\0';
                break;
            case ACS_VLINE:
                buf[0] = L'|';
                buf[1] = L'\0';
                break;
            case ACS_BULLET:
                buf[0] = L'.';
                buf[1] = L'\0';
                break;
            case ACS_BLOCK:
                buf[0] = L'#';
                buf[1] = L'\0';
                break;
            default:
                buf[0] = L'*';
                buf[1] = L'\0';
            }
        }
        ST->ConOut->SetCursorPosition(ST->ConOut, eficurses_x, eficurses_y);
        ST->ConOut->SetAttribute(ST->ConOut, EFI_TEXT_ATTR(fg, bg));
        ST->ConOut->OutputString(ST->ConOut, buf);
        eficurses_x ++;
        return OK;
    }
    else
    {
        return ERR;
    }
}

static int clrtoeol(void)
{
    if (! eficurses_ready) return ERR;
    {
        int x;
        int ox, oy;
        attr_t oa;

        oa = eficurses_attr;
        attrset(0);
        getyx(stdscr, oy, ox);
        clrtoeol();
        for (x = ox + 1; x < COLS; x ++)
        {
            move(oy, x);
            addch(' ');
        }
        move(oy, ox);
        attrset(oa);
    }
    return OK;
}

static int clrtobot(void)
{
    if (! eficurses_ready) return ERR;
    {
        int y;
        int ox, oy;
        attr_t oa;

        oa = eficurses_attr;
        attrset(0);
        getyx(stdscr, oy, ox);
        clrtoeol();
        for (y = oy + 1; y < LINES; y ++)
        {
            move(y, 0);
            clrtoeol();
        }
        move(oy, ox);
        attrset(oa);
    }
    return OK;
}

static int addstr(const char *s)
{
    if (! eficurses_ready) return ERR;
    while (*s)
    {
        eficurses_addch(*s++);
    }
    return OK;
}

static int mvprintw(int y, int x, const char *s)
{
    if (! eficurses_ready) return ERR;
    move(y, x);
    return addstr(s);
}
