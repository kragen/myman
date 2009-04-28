/*
 * allegrocurses.h - Allegro driver for the MyMan video game
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

#ifndef MYMAN_ALLEGCUR_H_INCLUDED
#define MYMAN_ALLEGCUR_H_INCLUDED 1

#define ALLEGRO_NO_CLEAR_BITMAP_ALIAS

#define ALLEGRO_NO_KEY_DEFINES

#ifdef main
#define ALLEGRO_NO_MAGIC_MAIN
#endif

#include <allegro.h>

/* for CHAR_BIT */
#include <limits.h>

/* for sqrt */
#include <math.h>

/* for struct timeval */
#include <sys/time.h>

/* for gettimeofday */
#include <time.h>

/* work-arounds for Allegro */

#ifndef USE_ICONV
#define USE_ICONV 0
#endif

#ifndef USE_RAW_UCS
#define USE_RAW_UCS 1
#endif

#ifndef USE_PALETTE
#define USE_PALETTE 1
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

#ifndef USE_SIGWINCH
#define USE_SIGWINCH 0
#endif

#ifndef USE_IOCTL
#define USE_IOCTL 0
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
#define TRUE (1)
#endif

static int allegrocurses_joy_key = ERR;

static int allegrocurses_mouse_key = ERR;

static int allegrocurses_mouse_b = 0;

static int allegrocurses_mouse_enabled = 0;

static FONT *allegrocurses_font;

static int allegrocurses_ready = 0;

static int allegrocurses_dirtypal = 1;

static const char *allegrocurses_title;

static double allegrocurses_need_resize;

static int allegrocurses_unmapped;

/* time in seconds to wait before handling a resize */
#define ALLEGROCURSES_RESIZE_INHIBIT 1.0

static char *allegrocurses_bitmap;

static char *allegrocurses_swaprb;

static char *allegrocurses_fixedpal;

static char *allegrocurses_bitmap_double;

static char *allegrocurses_bitmap_doubleheight;

struct allegrocurses_cell {
    unsigned long ch;
    unsigned long attr;
};

typedef struct allegrocurses_cell allegrocurses_cell_t;

static int allegrocurses_clearok;

static allegrocurses_cell_t *allegrocurses_backing;

typedef unsigned long int chtype;

typedef chtype attr_t;

static unsigned long allegrocurses_x, allegrocurses_y, allegrocurses_w, allegrocurses_h;

static int allegrocurses_charheight, allegrocurses_charwidth;

static unsigned long allegrocurses_attr;

static int allegrocurses_pixelmap[16], allegrocurses_halfpixelmap[16][16];
static RGB allegrocurses_colormap[16];
static PALETTE allegrocurses_palette;

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

#define cbreak()

#define noecho()

#define nonl()

#define nodelay(stdscr, x)

#define intrflush(stdscr, x)

#define immedok(stdscr, x)

#define idlok(stdscr, x)

#define clearok(stdscr, x) (x ? (allegrocurses_clearok = 1) : 0)

#define leaveok(stdscr, x)

#define getyx(stdscr, y, x) ((y) = allegrocurses_y), ((x) = allegrocurses_x)

#define wrefresh(stdscr) refresh()

static size_t allegrocurses_beep_offset;

static unsigned char allegrocurses_beep_midi[] = {
    5, /* program change, 9th channel*/ 0xc8,
    /*    --- patch 80 / program 81 (GM lead 1, square wave) */ 0x50,
    /*    note on, 9th channel */ 0x98,
    /*    --- note (middle C) */ 0x3C,
    /*    --- velocity (96 / 127ths) */ 0x60,
    3, /* note on, 9th channel */ 0x98,
    /*    --- note (middle C) */ 0x3C,
    /*    --- off (velocity 0) */ 0x00,
    0
};

static void allegrocurses_beep_tick(void)
{
    int bytes;

    bytes = allegrocurses_beep_midi[allegrocurses_beep_offset] + 1;
    if ((allegrocurses_beep_offset + bytes) >= sizeof(allegrocurses_beep_midi))
    {
        remove_int(allegrocurses_beep_tick);
    }
    if (bytes)
    {
        midi_out(allegrocurses_beep_midi + allegrocurses_beep_offset + 1, bytes - 1);
    }
    allegrocurses_beep_offset += bytes;
}

END_OF_FUNCTION(allegrocurses_beep_tick)

static int beep(void)
{
    allegrocurses_beep_offset = 0;
    install_int(allegrocurses_beep_tick, 60 * 1000 / 240);
    allegrocurses_beep_tick();
    return OK;
}

#define use_env(x)

#define echo()

#define curs_set(x)

#define keypad(stdscr, x)

#define can_change_color() (allegrocurses_fixedpal ? 0 : 1)

#define has_colors() 1

#define start_color()

#define ALLEGROCURSES_LINES (SCREEN_H / allegrocurses_charheight)

#define ALLEGROCURSES_COLS (SCREEN_W / allegrocurses_charwidth)

#define LINES allegrocurses_h

#define COLS allegrocurses_w

#define COLORS 16

#define COLOR_PAIRS 256

static unsigned char allegrocurses_pairs[COLOR_PAIRS];

#define COLOR_PAIR(i) (A_NORMAL | allegrocurses_pairs[(i)])

#define COLOR_BLACK 0

#define COLOR_BLUE 1

#define COLOR_GREEN 2

#define COLOR_CYAN 3

#define COLOR_RED 4

#define COLOR_MAGENTA 5

#define COLOR_YELLOW 6

#define COLOR_WHITE 7

#define PEN_BRIGHT 8

#define A_REVERSE 0x8000000UL

#define A_UNDERLINE 0x10000000UL

#define A_BOLD 0x20000000UL

#define A_STANDOUT 0x40000000UL

#define A_NORMAL 0x80000000UL

#define INITSCR_WITH_HINTS

#define KEY_LEFT (__allegro_KEY_LEFT << 8)

#define KEY_RIGHT (__allegro_KEY_RIGHT << 8)

#define KEY_UP (__allegro_KEY_UP << 8)

#define KEY_DOWN (__allegro_KEY_DOWN << 8)

#define ALLEGROCURSES_DEADCELL 0xffffUL

static int insch(const chtype ch)
{
    return ERR;
}

static int delch(void)
{
    return ERR;
}

static int allegrocurses_wcwidth(const chtype ch) {
    return (ch < 0x20) ? 0 : 1;
}

#undef wcwidth
#define wcwidth allegrocurses_wcwidth

static int addch(const chtype ch)
{
    int newmem, onscreen, rowstart;

    rowstart = allegrocurses_y * allegrocurses_w;
    newmem = rowstart + allegrocurses_x;
    onscreen = newmem + allegrocurses_w * allegrocurses_h;
    if ((! allegrocurses_clearok)
        &&
        (! allegrocurses_backing[newmem].attr)
        &&
        (allegrocurses_backing[onscreen].ch == ch)
        &&
        (allegrocurses_backing[onscreen].attr == (allegrocurses_attr ? allegrocurses_attr : A_NORMAL)))
    {
        allegrocurses_x += wcwidth(ch);
        return OK;
    }
    if (((allegrocurses_x + 1) < allegrocurses_w)
        &&
        ((allegrocurses_backing[onscreen + 1].ch == ALLEGROCURSES_DEADCELL)
         ||
         (allegrocurses_backing[newmem + 1].ch == ALLEGROCURSES_DEADCELL)))
    {
        if ((! allegrocurses_backing[newmem + 1].ch)
            ||
            (allegrocurses_backing[newmem + 1].ch == ALLEGROCURSES_DEADCELL))
        {
            allegrocurses_backing[newmem + 1].ch = ' ';
        }
    }
    if (allegrocurses_x
        &&
        ((allegrocurses_backing[onscreen].ch == ALLEGROCURSES_DEADCELL)
         ||
         (allegrocurses_backing[newmem].ch == ALLEGROCURSES_DEADCELL)))
    {
        if ((! allegrocurses_backing[newmem - 1].ch)
            ||
            (allegrocurses_backing[newmem - 1].ch == ALLEGROCURSES_DEADCELL))
        {
            allegrocurses_backing[newmem - 1].ch = ' ';
        }
    }
    allegrocurses_backing[newmem].ch = ch;
    allegrocurses_backing[newmem].attr = allegrocurses_attr ? allegrocurses_attr : A_NORMAL;
    if (! allegrocurses_backing[rowstart].attr)
    {
        allegrocurses_backing[rowstart].attr = A_NORMAL;
        allegrocurses_backing[rowstart].ch = ALLEGROCURSES_DEADCELL;
    }
    if ((ch == (wchar_t) ch) && (wcwidth(ch) == 2) && ((allegrocurses_x + 1) < allegrocurses_w))
    {
        allegrocurses_backing[newmem + 1].ch = ALLEGROCURSES_DEADCELL;
        allegrocurses_backing[newmem + 1].attr = allegrocurses_attr ? allegrocurses_attr : A_NORMAL;
        allegrocurses_x ++;
    }
    allegrocurses_x ++;
    if (allegrocurses_x >= allegrocurses_w)
    {
        allegrocurses_x = 0;
        allegrocurses_y ++;
        if (allegrocurses_y >= allegrocurses_h)
        {
            allegrocurses_y = allegrocurses_h - 1;
        }
    }
    return OK;
}

static int allegrocurses_addch(unsigned long ch, unsigned long attr, unsigned long x, unsigned long y) {
    int fg, bg;
    int fgpixel, bgpixel, halfpixel;

    fg = COLOR_WHITE;
    bg = COLOR_BLACK;
    if (attr & A_UNDERLINE)
    {
        fg = COLOR_BLUE;
    }
    if (attr & A_BOLD)
    {
        fg |= PEN_BRIGHT;
    }
    if (attr & (PEN_BRIGHT | COLOR_WHITE))
    {
        fg = attr & (PEN_BRIGHT | COLOR_WHITE);
    }
    if (attr & ((PEN_BRIGHT | COLOR_WHITE) << 4))
    {
        bg = (attr & ((PEN_BRIGHT | COLOR_WHITE) << 4)) >> 4;
    }
    if (attr & (A_STANDOUT | A_REVERSE))
    {
        int tmp;

        tmp = fg;
        fg = bg;
        bg = tmp;
    }
    fgpixel = allegrocurses_pixelmap[fg];
    bgpixel = allegrocurses_pixelmap[bg];
    halfpixel = allegrocurses_halfpixelmap[fg][bg];
    if (halfpixel == bgpixel) halfpixel = fgpixel;
    if (allegrocurses_bitmap
        ||
        ((allegrocurses_charwidth == 1)
         &&
         (allegrocurses_charheight == 1)
         &&
         (! (attr & (A_STANDOUT | A_REVERSE)))
         &&
         ((ch == ' ')
          ||
          (ch == '\0')
          ||
          (ch == 0x25a0)
          ||
          (ch == 0x00a0)
          ||
          (ch == 0x3000)
          ||
          (ch == 0x25cf))))
    {
        int outpixel, outpixel2, outpixel3, outpixel4;
        int wcw = 1;

        if ((ch == (wchar_t) ch) && (wcwidth(ch) == 2))
        {
            wcw = 2;
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
        if (allegrocurses_bitmap_doubleheight)
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
        if (allegrocurses_bitmap_double)
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
        putpixel(screen, x * allegrocurses_charwidth, y * allegrocurses_charheight, outpixel);
        if (allegrocurses_bitmap_doubleheight)
        {
            putpixel(screen, x * allegrocurses_charwidth, y * allegrocurses_charheight + (allegrocurses_bitmap_double ? 2 : 1), outpixel2);
        }
        if (allegrocurses_bitmap_double)
        {
            putpixel(screen, x * allegrocurses_charwidth + 1, y * allegrocurses_charheight, (wcw == 2) ? outpixel : outpixel3);
            if (allegrocurses_bitmap_doubleheight)
            {
                putpixel(screen, x * allegrocurses_charwidth + 1, y * allegrocurses_charheight + (allegrocurses_bitmap_double ? 2 : 1), (wcw == 2) ? outpixel2 : outpixel4);
            }
        }
        if (wcw == 2)
        {
            putpixel(screen, (x + 1) * allegrocurses_charwidth, y * allegrocurses_charheight, outpixel);
            if (allegrocurses_bitmap_doubleheight)
            {
                putpixel(screen, (x + 1) * allegrocurses_charwidth, y * allegrocurses_charheight + (allegrocurses_bitmap_double ? 2 : 1), outpixel2);
            }
            if (allegrocurses_bitmap_double)
            {
                putpixel(screen, (x + 1) * allegrocurses_charwidth + 1, y * allegrocurses_charheight, outpixel3);
                if (allegrocurses_bitmap_doubleheight)
                {
                    putpixel(screen, (x + 1) * allegrocurses_charwidth + 1, y * allegrocurses_charheight + (allegrocurses_bitmap_double ? 2 : 1), outpixel4);
                }
            }
        }
    }
    else
    {
        int n;
        char s[5];
        int ch2 = ' ';
        int wcw = 1;

        s[0] = '?';
        s[1] = 0;
        wcw = (ch == (wchar_t) ch) ? wcwidth(ch) : 1;
        if (wcw < 1)
        {
            wcw = 1;
            if ((ch < ' ')
                ||
                (ch > '~'))
            {
                ch = '?';
            }
        }
        while (wcw > 0)
        {
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
            if ((ch < 0) || (ch > 0x10ffffUL) || ((ch & 0xfffeUL) == 0xfffeUL))
            {
                ch = 0xfffdUL;
            }
            for (n = 0; n < get_font_ranges(allegrocurses_font); n ++)
            {
                if ((ch >= get_font_range_begin(allegrocurses_font, n))
                    &&
                    (ch <= get_font_range_end(allegrocurses_font, n)))
                {
                    break;
                }
            }
            if (n == get_font_ranges(allegrocurses_font))
            {
                switch (ch)
                {
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
                case 0x2022:
                case 0x25a0:
                case 0x25cb:
                case 0x25cf:
                case 0x25d8:
                    ch = 'o';
                    break;
                case ACS_BULLET:
                case 0x30fb:
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
                default:
                    if ((ch >= 0xff01)
                        &&
                        (ch <= 0xff5e))
                    {
                        ch = ch - 0xff00 + ' ';
                    }
                }
            }
            if (ch <= 0x7fUL)
            {
                s[0] = ch;
                s[1] = 0;
            }
            else if (ch <= 0x07ffUL)
            {
                s[0] = 0xc0 | (ch >> 6);
                s[1] = 0x80 | (ch & 0x3f);
                s[2] = 0;
            }
            else if (ch <= 0xffffUL)
            {
                s[0] = 0xe0 | (ch >> 12);
                s[1] = 0x80 | ((ch >> 6) & 0x3f);
                s[2] = 0x80 | (ch & 0x3f);
                s[3] = 0;
            }
            else if (ch <= 0x10ffffUL)
            {
                s[0] = 0xf0 | (ch >> 18);
                s[1] = 0x80 | ((ch >> 12) & 0x3f);
                s[2] = 0x80 | ((ch >> 6) & 0x3f);
                s[3] = 0x80 | (ch & 0x3f);
                s[4] = 0;
            }
            if ((wcw == 2)
                &&
                (ch2 == ' '))
            {
                wcw = 1;
                rectfill(screen,
                         x * allegrocurses_charwidth, y * allegrocurses_charheight,
                         (x + 2) * allegrocurses_charwidth - 1, (y + 1) * allegrocurses_charheight - 1,
                         bgpixel);
                textout_ex(screen, allegrocurses_font, s, (2 * x + 1) * allegrocurses_charwidth / 2, y * allegrocurses_charheight, fgpixel, bgpixel);
            }
            else
            {
                textout_ex(screen, allegrocurses_font, s, x * allegrocurses_charwidth, y * allegrocurses_charheight, fgpixel, bgpixel);
            }
            wcw --;
            x ++;
            ch = ch2;
        }
    }
    return OK;
}

static int move(y, x)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= allegrocurses_w) x = allegrocurses_w - 1;
    if (y >= allegrocurses_h) x = allegrocurses_h - 1;
    allegrocurses_y = y;
    allegrocurses_x = x;
    return OK;
}

static int clear(void)
{
    memset((void *) (allegrocurses_backing + allegrocurses_w * allegrocurses_h), 0, (allegrocurses_w * allegrocurses_h * sizeof(allegrocurses_cell_t)));
    allegrocurses_attr = A_NORMAL;
    move(0, 0);
    clearok(stdscr, TRUE);
    return OK;
}

static int attrset(int a)
{
    allegrocurses_attr = a ? a : A_NORMAL;
    return OK;
}

static int erase(void) {
    unsigned int x, y;

    attrset(0);
    for (y = 0; y < LINES; y ++)
        for (x = 0; x < COLS; x ++)
        {
            move(y, x);
            addch(' ');
        }
    move(0, 0);
    clearok(stdscr, TRUE);
    return OK;
}

static int clrtobot(void) {
    unsigned int x, y;

    for (x = allegrocurses_x; x < COLS; x ++)
    {
        move(allegrocurses_y, x);
        addch(' ');
    }
    for (y = allegrocurses_y + 1; y < LINES; y ++)
        for (x = 0; x < COLS; x ++)
        {
            move(y, x);
            addch(' ');
        }
    return OK;
}

static int clrtoeol(void) {
    unsigned int x;

    for (x = allegrocurses_x; x < COLS; x ++)
    {
        move(allegrocurses_y, x);
        addch(' ');
    }
    return OK;
}

static void endwin(void);

static int refresh(void);

static void allegrocurses_resize(void)
{
    if (allegrocurses_bitmap)
    {
        allegrocurses_charwidth = allegrocurses_bitmap_double ? 2 : 1;
        allegrocurses_charheight = allegrocurses_bitmap_double ? 2 : 1;
        if (allegrocurses_bitmap_doubleheight) allegrocurses_charheight *= 2;
    }
    else
    {
        allegrocurses_charwidth = text_length(allegrocurses_font, "M");
        allegrocurses_charheight = text_height(allegrocurses_font);
    }
    {
        int old_w;
        int old_h;
        int y;

        old_w = allegrocurses_w;
        old_h = allegrocurses_h;
        allegrocurses_h = ALLEGROCURSES_LINES;
        allegrocurses_w = ALLEGROCURSES_COLS;
        memmove((void *) allegrocurses_backing, (void *) (allegrocurses_backing + old_w * old_h), old_w * old_h * sizeof(allegrocurses_cell_t));
        if ((old_w != allegrocurses_w)
            ||
            (old_h != allegrocurses_h))
        {
            if (allegrocurses_w < old_w)
            {
                for (y = 0; (y < allegrocurses_h) && (y < old_h); y ++)
                {
                    memmove((void *) (allegrocurses_backing + y * allegrocurses_w), (void *) (allegrocurses_backing + y * old_w), allegrocurses_w * sizeof(allegrocurses_cell_t));
                }
            }
            if (allegrocurses_w * allegrocurses_h != old_w * old_h)
            {
                if (! (allegrocurses_backing = (allegrocurses_cell_t *) realloc((void *) allegrocurses_backing, 2 * allegrocurses_w * allegrocurses_h * sizeof(allegrocurses_cell_t))))
                {
                    perror("realloc");
                    fflush(stderr);
                    endwin();
                    exit(1);
                }
            }
            if (allegrocurses_w > old_w)
            {
                for (y = ((allegrocurses_h > old_h) ? old_h : allegrocurses_h); y --;)
                {
                    memmove((void *) (allegrocurses_backing + y * allegrocurses_w), (void *) (allegrocurses_backing + y * old_w), old_w * sizeof(allegrocurses_cell_t));
                }
            }
            if ((allegrocurses_w * allegrocurses_h) > (old_w * old_h))
            {
                memset((void *) (allegrocurses_backing + old_w * old_h), 0, (allegrocurses_w * allegrocurses_h - old_w * old_h) * sizeof(allegrocurses_cell_t));
            }
        }
        memset((void *) (allegrocurses_backing + allegrocurses_w * allegrocurses_h), 0, allegrocurses_w * allegrocurses_h * sizeof(allegrocurses_cell_t));
    }
    allegrocurses_dirtypal = 1;
}

static double allegrocurses_now(void)
{
    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    myman_gettimeofday(&tv, 0);
    return 1.0L * tv.tv_sec + 1.0e-6 * tv.tv_usec;
}

static int refresh(void)
{
    int x;
    int y;
    int fg, bg;

    if (! allegrocurses_ready) return OK;
    acquire_screen();
    scare_mouse();
    if (allegrocurses_dirtypal)
    {
        allegrocurses_dirtypal = 0;
        if (! allegrocurses_fixedpal)
        {
            set_palette(allegrocurses_palette);
        }
        for (fg = COLOR_BLACK; fg <= COLOR_WHITE; fg ++)
        {
            allegrocurses_pixelmap[fg] = makecol(allegrocurses_colormap[fg].r, allegrocurses_colormap[fg].g, allegrocurses_colormap[fg].b);
            allegrocurses_pixelmap[fg | PEN_BRIGHT] = makecol(allegrocurses_colormap[fg | PEN_BRIGHT].r, allegrocurses_colormap[fg | PEN_BRIGHT].g, allegrocurses_colormap[fg | PEN_BRIGHT].b);
        }
        for (bg = COLOR_BLACK; bg <= (COLOR_WHITE | PEN_BRIGHT); bg ++)
        {
            for (fg = COLOR_BLACK; fg <= bg; fg ++)
            {
                allegrocurses_halfpixelmap[fg][bg] = makecol(allegrocurses_palette[16*(fg)+(bg)].r, allegrocurses_palette[16*(fg)+(bg)].g, allegrocurses_palette[16*(fg)+(bg)].b);
                allegrocurses_halfpixelmap[bg][fg] = allegrocurses_halfpixelmap[fg][bg];
            }
        }
    }
    if (allegrocurses_clearok)
    {
        rectfill(screen, 0, 0, SCREEN_W - 1, SCREEN_H - 1, makecol(allegrocurses_colormap[0].r, allegrocurses_colormap[0].g, allegrocurses_colormap[0].b));
    }
    for (y = 0; y < allegrocurses_h; y ++)
    {
        if (allegrocurses_clearok || allegrocurses_backing[y * allegrocurses_w].attr)
        {
            for (x = 0; x < allegrocurses_w; x ++)
            {
                unsigned long ch;
                unsigned long attr;
                int pixdiff;
                
                attr = allegrocurses_backing[y * allegrocurses_w + x].attr;
                ch = allegrocurses_backing[y * allegrocurses_w + x].ch;
                pixdiff = allegrocurses_clearok;
                if (allegrocurses_clearok
                    ||
                    (attr
                     &&
                     (pixdiff = ((attr
                                  !=
                                  allegrocurses_backing[y * allegrocurses_w + x + allegrocurses_w * allegrocurses_h].attr)
                                 ||
                                 (ch
                                  !=
                                  allegrocurses_backing[y * allegrocurses_w + x + allegrocurses_w * allegrocurses_h].ch)))))
                {
                    if (! ch)
                    {
                        attr = allegrocurses_backing[y * allegrocurses_w + x + allegrocurses_w * allegrocurses_h].attr;
                        ch = allegrocurses_backing[y * allegrocurses_w + x + allegrocurses_w * allegrocurses_h].ch;
                    }
                    allegrocurses_backing[y * allegrocurses_w + x + allegrocurses_w * allegrocurses_h].attr = attr;
                    allegrocurses_backing[y * allegrocurses_w + x + allegrocurses_w * allegrocurses_h].ch = ch;
                    allegrocurses_backing[y * allegrocurses_w + x].attr = 0;
                    allegrocurses_backing[y * allegrocurses_w + x].ch = ALLEGROCURSES_DEADCELL;
                    if (ch != ALLEGROCURSES_DEADCELL)
                    {
                        allegrocurses_addch(ch ? ch : ' ', attr, x, y);
                    }
                }
            }
            allegrocurses_backing[y * allegrocurses_w].attr = 0;
        }
    }
    allegrocurses_clearok = 0;
    unscare_mouse();
    release_screen();
    {
        if ((allegrocurses_need_resize != 0.0)
            &&
            ((allegrocurses_now() - allegrocurses_need_resize) >= ALLEGROCURSES_RESIZE_INHIBIT))
        {
            allegrocurses_resize();
            allegrocurses_need_resize = 0.0;
        }
    }
    return OK;
}

static volatile int allegrocurses_closed = 0;

static void allegrocurses_onclose(void)
{
    allegrocurses_closed = 1;
}

static void allegrocurses_onmap(void)
{
    allegrocurses_unmapped = 1;
}

static chtype getch(void)
{
    int gotevent;

    while ((gotevent = (keypressed() == TRUE))
           ||
           allegrocurses_closed
           ||
           allegrocurses_unmapped)
    {
        if (allegrocurses_mouse_enabled)
        {
            allegrocurses_mouse_enabled = 0;
            enable_hardware_cursor();
        }
        if (gotevent)
        {
            int val;

            val = readkey();
            switch (val & ~0xFF)
            {
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_UP:
            case KEY_DOWN:
                val &= ~0xFF;
            }
            if (val & 0xFF)
            {
                return (val & 0xFF);
            }
            else
            {
                return val;
            }
        }
        if (allegrocurses_closed)
        {
            allegrocurses_closed = 0;
            return 'Q';
        }
        if (allegrocurses_unmapped)
        {
            refresh();
            allegrocurses_unmapped = 0;
            allegrocurses_mouse_key = ERR;
            allegrocurses_joy_key = ERR;
        }
    }
    if ((! mouse_needs_poll())
        ||
        ! poll_mouse())
    {
        int mouse_key = ERR;
        int mickey_x = 0;
        int mickey_y = 0;
        int previously_enabled;

        previously_enabled = allegrocurses_mouse_enabled;
        if (allegrocurses_mouse_enabled)
        {
            get_mouse_mickeys(&mickey_x, &mickey_y);
            if (mickey_x || mickey_y)
            {
                position_mouse(SCREEN_W / 2, SCREEN_H / 2);
            }
        }
        if (allegrocurses_mouse_b != mouse_b)
        {
            if (mouse_b & 1)
            {
                allegrocurses_mouse_key = ERR;
                allegrocurses_mouse_enabled = 1;
                mouse_key = ' ';
            }
            if (mouse_b & 2)
            {
                allegrocurses_mouse_key = ERR;
                allegrocurses_mouse_enabled = 1;
                mouse_key = '\x1b';
            }
            allegrocurses_mouse_b = mouse_b;
            if (allegrocurses_mouse_enabled
                &&
                ! previously_enabled)
            {
                disable_hardware_cursor();
            }
        }
        else if ((mickey_y * mickey_y) > (mickey_x * mickey_x))
        {
            if (mickey_y < 0)
            {
                mouse_key = KEY_UP;
            }
            else if (mickey_y > 0)
            {
                mouse_key = KEY_DOWN;
            }
        }
        else if (mickey_x < 0)
        {
            mouse_key = KEY_LEFT;
        }
        else if (mickey_x > 0)
        {
            mouse_key = KEY_RIGHT;
        }
        if (mouse_key != allegrocurses_mouse_key)
        {
            if (mouse_key == ERR)
            {
                if ((allegrocurses_mouse_key == KEY_UP)
                    ||
                    (allegrocurses_mouse_key == KEY_DOWN))
                {
                    mouse_key = '.';
                }
                else if ((allegrocurses_mouse_key == KEY_LEFT)
                         ||
                         (allegrocurses_mouse_key == KEY_RIGHT))
                {
                    mouse_key = ',';
                }
                allegrocurses_mouse_key = ERR;
            }
            else
            {
                allegrocurses_mouse_key = mouse_key;
            }
            if (mouse_key != ERR)
            {
                return mouse_key;
            }
        }
    }
    if (! poll_joystick())
    {
        if (num_joysticks)
        {
            int joy_key = ERR;

            if (joy[0].num_sticks >= 1)
            {
                if (joy[0].stick[0].num_axis >= 1)
                {
                    if (joy[0].stick[0].axis[0].d1)
                    {
                        joy_key = KEY_LEFT;
                    }
                    else if (joy[0].stick[0].axis[0].d2)
                    {
                        joy_key = KEY_RIGHT;
                    }
                }
                if (joy[0].stick[0].num_axis >= 2)
                {
                    if (joy[0].stick[0].axis[1].d1)
                    {
                        joy_key = KEY_UP;
                    }
                    else if (joy[0].stick[0].axis[1].d2)
                    {
                        joy_key = KEY_DOWN;
                    }
                }
            }
            if (joy[0].num_buttons >= 1)
            {
                if (joy[0].button[0].b)
                {
                    joy_key = ' ';
                }
            }
            if (joy[0].num_buttons >= 2)
            {
                if (joy[0].button[1].b)
                {
                    joy_key = '\x1b';
                }
            }
            if (joy_key != allegrocurses_joy_key)
            {
                if (joy_key == ERR)
                {
                    if ((allegrocurses_joy_key == KEY_UP)
                        ||
                        (allegrocurses_joy_key == KEY_DOWN))
                    {
                        joy_key = '.';
                    }
                    else if ((allegrocurses_joy_key == KEY_LEFT)
                             ||
                             (allegrocurses_joy_key == KEY_RIGHT))
                    {
                        joy_key = ',';
                    }
                    allegrocurses_joy_key = ERR;
                }
                else
                {
                    allegrocurses_joy_key = joy_key;
                }
                if (joy_key != ERR)
                {
                    if (allegrocurses_mouse_enabled)
                    {
                        allegrocurses_mouse_enabled = 0;
                        enable_hardware_cursor();
                    }
                    return joy_key;
                }
            }
        }
    }
    allegrocurses_unmapped = 0;
    return ERR;
}

static void standout() {
    allegrocurses_attr = A_STANDOUT;
}

static void standend() {
    allegrocurses_attr = 0;
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

static unsigned int allegrocurses_mix(unsigned int a, unsigned int b)
{
    unsigned long r;

    r = 0xff * sqrt(1.0 * a / 0xff * a / 0xff + 1.0 * b / 0xff * b / 0xff) / M_SQRT2;
    if (r > 0xff)
    {
        r = 0xff;
    }
    return (unsigned int) r;
}

static int init_color(int i, short r, short g, short b)
{
    int bg;

    allegrocurses_colormap[i].r = 0xff * (long) (allegrocurses_swaprb ? b : r) / 1000;
    allegrocurses_colormap[i].g = 0xff * (long) g / 1000;
    allegrocurses_colormap[i].b = 0xff * (long) (allegrocurses_swaprb ? r : b) / 1000;
    for (bg = COLOR_BLACK; bg <= (COLOR_WHITE | PEN_BRIGHT); bg ++)
    {
        allegrocurses_palette[16*(i)+(bg)].r = allegrocurses_mix(allegrocurses_colormap[i].r, allegrocurses_colormap[bg].r);
        allegrocurses_palette[16*(i)+(bg)].g = allegrocurses_mix(allegrocurses_colormap[i].g, allegrocurses_colormap[bg].g);
        allegrocurses_palette[16*(i)+(bg)].b = allegrocurses_mix(allegrocurses_colormap[i].b, allegrocurses_colormap[bg].b);
        allegrocurses_palette[16*(bg)+(i)] = allegrocurses_palette[16*(i)+(bg)];
    }
    allegrocurses_dirtypal = 1;
    return OK;
}

static int color_content(int i, short *r, short *g, short *b) {
    *(allegrocurses_swaprb ? b : r) = (long) allegrocurses_colormap[i].r * 1000 / 0xff;
    *g = (long) allegrocurses_colormap[i].g * 1000 / 0xff;
    *(allegrocurses_swaprb ? r : b) = (long) allegrocurses_colormap[i].b * 1000 / 0xff;
    return OK;
}

static int init_pair(int i, short fg, short bg) {
    allegrocurses_pairs[i] = (bg << 4) | fg;
    return OK;
}

static int pair_content(int i, short *fg, short *bg) {
    *fg = allegrocurses_pairs[i] & 0xf;
    *bg = allegrocurses_pairs[i] >> 4;
    return OK;
}

static int allegrocurses_init_done = 0;

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    int fg, bg, depth, maxw, maxh;

    allegrocurses_title = title;
    allegrocurses_bitmap = myman_getenv("ALLEGROCURSES_BITMAP");
    if (allegrocurses_bitmap && ! *allegrocurses_bitmap) allegrocurses_bitmap = NULL;
    allegrocurses_swaprb = myman_getenv("ALLEGROCURSES_SWAPRB");
    if (allegrocurses_swaprb && ! *allegrocurses_swaprb) allegrocurses_swaprb = NULL;
    allegrocurses_fixedpal = myman_getenv("ALLEGROCURSES_FIXEDPAL");
    if (allegrocurses_fixedpal && ! *allegrocurses_fixedpal) allegrocurses_fixedpal = NULL;
    allegrocurses_bitmap_double = myman_getenv("ALLEGROCURSES_BITMAP_DOUBLE");
    if (allegrocurses_bitmap_double && ! *allegrocurses_bitmap_double) allegrocurses_bitmap_double = NULL;
    allegrocurses_bitmap_doubleheight = myman_getenv("ALLEGROCURSES_BITMAP_DOUBLEHEIGHT");
    if (allegrocurses_bitmap_doubleheight && ! *allegrocurses_bitmap_doubleheight) allegrocurses_bitmap_doubleheight = NULL;
    if (! allegrocurses_init_done)
    {
        allegrocurses_init_done = 1;
        set_uformat(U_UTF8);
        if (allegro_init())
        {
            allegro_message("allegro_init failed: %s\n", allegro_error);
            exit(1);
        }
        allegro_404_char = '*';
    }
    depth = desktop_color_depth();
    if (depth)
    {
        set_color_depth(depth);
    }
    allegrocurses_font = font;
    if (myman_getenv("ALLEGROCURSES_TXT_FONT") && *(myman_getenv("ALLEGROCURSES_TXT_FONT")))
    {
        allegrocurses_font = load_txt_font(myman_getenv("ALLEGROCURSES_TXT_FONT"), NULL, NULL);
        if (! allegrocurses_font)
        {
            allegro_message("%s: load_txt_font failed: %s\n", myman_getenv("ALLEGROCURSES_TXT_FONT"), allegro_error);
            exit(1);
        }
    }
    else if (myman_getenv("ALLEGROCURSES_FONT") && *(myman_getenv("ALLEGROCURSES_FONT")))
    {
        allegrocurses_font = load_font(myman_getenv("ALLEGROCURSES_FONT"), NULL, NULL);
        if (! allegrocurses_font)
        {
            allegro_message("%s: load_font failed: %s\n", myman_getenv("ALLEGROCURSES_FONT"), allegro_error);
            exit(1);
        }
    }
    if (allegrocurses_bitmap)
    {
        allegrocurses_charwidth = allegrocurses_bitmap_double ? 2 : 1;
        allegrocurses_charheight = allegrocurses_bitmap_double ? 2 : 1;
        if (allegrocurses_bitmap_doubleheight) allegrocurses_charheight *= 2;
    }
    else
    {
        allegrocurses_charwidth = text_length(allegrocurses_font, "M");
        allegrocurses_charheight = text_height(allegrocurses_font);
    }
    if (! get_desktop_resolution(&maxw, &maxh))
    {
        if (maxw > 40) maxw -= 40;
        if (maxh > 40) maxh -= 40;
        maxw /= allegrocurses_charwidth;
        maxh /= allegrocurses_charheight;
        if (maxw && (maxw < w)) w = maxw;
        if (maxh && (maxh < h)) h = maxh;
    }
    if ((install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) == -1)
        &&
        (install_sound(DIGI_AUTODETECT, MIDI_DIGMID, NULL) == -1)
        &&
        (install_sound(DIGI_NONE, MIDI_AUTODETECT, NULL) == -1)
        &&
        (install_sound(DIGI_NONE, MIDI_NONE, NULL) == -1))
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("install_sound failed: %s\n", allegro_error);
        exit(1);
    }
    if (set_gfx_mode(GFX_SAFE, w * allegrocurses_charwidth, h * allegrocurses_charheight, 0, 0))
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("set_gfx_mode failed: %s\n", allegro_error);
        exit(1);
    }
    if (install_keyboard())
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("install_keyboard failed: %s\n", allegro_error);
        exit(1);
    }
    if (install_timer() < 0)
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("install_timer failed: %s\n", allegro_error);
        exit(1);
    }
    if (install_mouse() == -1)
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("install_mouse failed: %s\n", allegro_error);
        exit(1);
    }
    if (install_joystick(JOY_TYPE_AUTODETECT)
        &&
        install_joystick(JOY_TYPE_NONE))
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("install_joystick failed: %s\n", allegro_error);
        exit(1);
    }
    set_window_title(allegrocurses_title);
    allegrocurses_closed = 0;
    set_close_button_callback(allegrocurses_onclose);
    set_display_switch_callback(SWITCH_IN, allegrocurses_onmap);
    for (fg = COLOR_BLACK; fg <= COLOR_WHITE; fg ++)
    {
        if (fg)
        {
            allegrocurses_colormap[fg].r = ((fg & (allegrocurses_swaprb ? COLOR_BLUE : COLOR_RED)) ? 0x8888 : 0x0000) >> 8;
            allegrocurses_colormap[fg].g = ((fg & COLOR_GREEN) ? 0x8888 : 0x0000) >> 8;
            allegrocurses_colormap[fg].b = ((fg & (allegrocurses_swaprb ? COLOR_RED : COLOR_BLUE)) ? 0x8888 : 0x0000) >> 8;
            allegrocurses_colormap[fg | PEN_BRIGHT].r = (allegrocurses_colormap[fg].r ? 0xffff : 0x8888) >> 8;
            allegrocurses_colormap[fg | PEN_BRIGHT].g = (allegrocurses_colormap[fg].g ? 0xffff : 0x8888) >> 8;
            allegrocurses_colormap[fg | PEN_BRIGHT].b = (allegrocurses_colormap[fg].b ? 0xffff : 0x8888) >> 8;
        }
        else
        {
            allegrocurses_colormap[fg].r = 0x0000;
            allegrocurses_colormap[fg].g = 0x0000;
            allegrocurses_colormap[fg].b = 0x0000;
            allegrocurses_colormap[fg | PEN_BRIGHT].r = 0x6666 >> 8;
            allegrocurses_colormap[fg | PEN_BRIGHT].g = 0x6666 >> 8;
            allegrocurses_colormap[fg | PEN_BRIGHT].b = 0x6666 >> 8;
        }
    }
    for (bg = COLOR_BLACK; bg <= (COLOR_WHITE | PEN_BRIGHT); bg ++)
    {
        for (fg = COLOR_BLACK; fg <= bg; fg ++)
        {
            allegrocurses_palette[16*(fg)+(bg)].r = allegrocurses_mix(allegrocurses_colormap[fg].r, allegrocurses_colormap[bg].r);
            allegrocurses_palette[16*(fg)+(bg)].g = allegrocurses_mix(allegrocurses_colormap[fg].g, allegrocurses_colormap[bg].g);
            allegrocurses_palette[16*(fg)+(bg)].b = allegrocurses_mix(allegrocurses_colormap[fg].b, allegrocurses_colormap[bg].b);
            allegrocurses_palette[16*(bg)+(fg)] = allegrocurses_palette[16*(fg)+(bg)];
        }
    }
    allegrocurses_h = ALLEGROCURSES_LINES;
    allegrocurses_w = ALLEGROCURSES_COLS;
    if (! (allegrocurses_backing = (allegrocurses_cell_t *) malloc(2 * allegrocurses_w * allegrocurses_h * sizeof(allegrocurses_cell_t))))
    {
        remove_joystick();
        remove_mouse();
        remove_keyboard();
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        perror("malloc");
        fflush(stderr);
        exit(1);
    }
    memset((void *) allegrocurses_backing, 0, 2 * allegrocurses_w * allegrocurses_h * sizeof(allegrocurses_cell_t));
    allegrocurses_need_resize = 0.0;
    allegrocurses_unmapped = 0;
    allegrocurses_clearok = 1;
    allegrocurses_attr = A_NORMAL;
    allegrocurses_ready = 1;
    allegrocurses_dirtypal = 1;
    allegrocurses_mouse_enabled = 0;
    enable_hardware_cursor();
    LOCK_VARIABLE(allegrocurses_beep_offset);
    LOCK_VARIABLE(allegrocurses_beep_midi);
    LOCK_FUNCTION(allegrocurses_beep_tick);
}

static void endwin(void) {
    if (allegrocurses_mouse_enabled)
    {
        enable_hardware_cursor();
    }
    remove_timer();
    free(allegrocurses_backing);
    allegrocurses_ready = 0;
    remove_joystick();
    remove_mouse();
    remove_keyboard();
    remove_sound();
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    if (allegrocurses_font != font)
    {
        destroy_font(allegrocurses_font);
    }
}

#endif /* MYMAN_ALLEGCUR_H_INCLUDED */
