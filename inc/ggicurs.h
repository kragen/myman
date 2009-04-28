/*
 * ggicurs.h - LibGGI/LibGII driver for the MyMan video game
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

#ifndef MYMAN_GGICURS_H_INCLUDED
#define MYMAN_GGICURS_H_INCLUDED 1

#include <ggi/gii.h>
#include <ggi/ggi.h>

#ifndef USE_GGIWMH
#define USE_GGIWMH 0
#endif

#if USE_GGIWMH
#include <ggi/wmh.h>
#endif

/* for CHAR_BIT */
#include <limits.h>

/* for sqrt */
#include <math.h>

/* for struct timeval */
#include <sys/time.h>

/* for gettimeofday */
#include <time.h>

/* work-arounds for LibGGI/LibGII */

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
#define SWAPDOTS 0
#endif

static int ggicurses_ready = 0;

static const char *ggicurses_title;

static double ggicurses_need_resize;

static int ggicurses_unmapped;

/* time in seconds to wait before handling a resize */
#define GGICURSES_RESIZE_INHIBIT 1.0

static int ggicurses_wmh;

static char *ggicurses_bitmap;

static char *ggicurses_swaprb;

static char *ggicurses_ascii;

static char *ggicurses_flushall;

static char *ggicurses_fixedpal;

static char *ggicurses_bitmap_double;

static char *ggicurses_bitmap_doubleheight;

struct ggicurses_cell {
    unsigned long ch;
    unsigned long attr;
};

typedef struct ggicurses_cell ggicurses_cell_t;

static int ggicurses_clearok;

static ggicurses_cell_t *ggicurses_backing;

typedef unsigned long int chtype;

typedef chtype attr_t;

static ggi_visual_t ggicurses_visual;

static ggi_mode ggicurses_mode;

static int ggicurses_direct;
static unsigned int ggicurses_reverse_endian;
static int ggicurses_bpp;
static int ggicurses_frame;
static int ggicurses_frames;
static const ggi_pixelformat *ggicurses_format;
static const ggi_directbuffer *ggicurses_buffer[2] = {0, 0};

static unsigned long ggicurses_x, ggicurses_y, ggicurses_w, ggicurses_h;

static int ggicurses_charheight, ggicurses_charwidth;

static unsigned long ggicurses_attr, ggicurses_realattr;

static ggi_pixel ggicurses_pixelmap[16], ggicurses_halfpixelmap[16][16];
static ggi_color ggicurses_colormap[16], ggicurses_halfcolormap[16][16];

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

#define ACS_LRCORNER 0xd9

#define ACS_URCORNER 0xbf

#define ACS_ULCORNER 0xda

#define ACS_LLCORNER 0xc0

#define ACS_PLUS 0xc5

#define ACS_HLINE 0xc4

#define ACS_LTEE 0xc3

#define ACS_RTEE 0xb4

#define ACS_BTEE 0xc1

#define ACS_TTEE 0xc2

#define ACS_VLINE 0xb3

/* #define ACS_BULLET 0xf9 */

#define ACS_BULLET 0xfe

#define cbreak()

#define noecho()

#define nonl()

#define nodelay(stdscr, x)

#define intrflush(stdscr, x)

#define immedok(stdscr, x)

#define idlok(stdscr, x)

#define clearok(stdscr, x) (x ? (ggicurses_clearok = 1) : 0)

#define leaveok(stdscr, x)

#define getyx(stdscr, y, x) ((y) = ggicurses_y), ((x) = ggicurses_x)

#define wrefresh(stdscr) refresh()

#define beep() addch('\7')

#define use_env(x)

#define echo()

#define curs_set(x)

#define keypad(stdscr, x)

#define can_change_color() (ggicurses_fixedpal ? 0 : 1)

#define has_colors() 1

#define start_color()

#define GGICURSES_LINES (ggicurses_mode.visible.y / ggicurses_charheight)

#define GGICURSES_COLS (ggicurses_mode.visible.x / ggicurses_charwidth)

#define LINES ggicurses_h

#define COLS ggicurses_w

#define COLORS 16

#define COLOR_PAIRS 256

static unsigned char ggicurses_pairs[COLOR_PAIRS];

#define COLOR_PAIR(i) (A_NORMAL | ggicurses_pairs[(i)])

#define COLOR_BLACK 0

#define COLOR_BLUE 1

#define COLOR_GREEN 2

#define COLOR_CYAN 3

#define COLOR_RED 4

#define COLOR_MAGENTA 5

#define COLOR_YELLOW 6

#define COLOR_WHITE 7

#define PEN_BRIGHT 8

#define KEY_UP GIIK_Up

#define KEY_DOWN GIIK_Down

#define KEY_LEFT GIIK_Left

#define KEY_RIGHT GIIK_Right

#define A_UNDERLINE 0x10000000

#define A_BOLD 0x20000000

#define A_STANDOUT 0x40000000

#define A_REVERSE A_STANDOUT

#define A_NORMAL 0x80000000

#define INITSCR_WITH_HINTS

static int insch(const chtype ch)
{
    return ERR;
}

static int delch(void)
{
    return ERR;
}

static int addch(const chtype ch) {
    if ((! ggicurses_clearok)
        &&
        (! ggicurses_backing[ggicurses_y * ggicurses_w + ggicurses_x].attr)
        &&
        (ggicurses_backing[ggicurses_y * ggicurses_w + ggicurses_x + ggicurses_w * ggicurses_h].ch == ch)
        &&
        (ggicurses_backing[ggicurses_y * ggicurses_w + ggicurses_x + ggicurses_w * ggicurses_h].attr == (ggicurses_attr ? ggicurses_attr : A_NORMAL)))
    {
        ggicurses_x ++;
        return OK;
    }
    ggicurses_backing[ggicurses_y * ggicurses_w + ggicurses_x].ch = ch;
    ggicurses_backing[ggicurses_y * ggicurses_w + ggicurses_x].attr = ggicurses_attr ? ggicurses_attr : A_NORMAL;
    if (! ggicurses_backing[ggicurses_y * ggicurses_w].attr)
    {
        ggicurses_backing[ggicurses_y * ggicurses_w].attr = A_NORMAL;
        ggicurses_backing[ggicurses_y * ggicurses_w].ch = 0;
    }
    ggicurses_x ++;
    return OK;
}

#define NEED_CP437_TO_ASCII
static chtype cp437_to_ascii(unsigned char ch);

static int ggicurses_addch(unsigned long ch, unsigned long attr, unsigned long x, unsigned long y) {
    int fg, bg;
    ggi_pixel fgpixel, bgpixel, halfpixel;

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
    if (attr & A_STANDOUT)
    {
        int tmp;

        tmp = fg;
        fg = bg;
        bg = tmp;
    }
    fgpixel = ggicurses_pixelmap[fg];
    bgpixel = ggicurses_pixelmap[bg];
    halfpixel = ggicurses_halfpixelmap[fg][bg];
    if (halfpixel == bgpixel) halfpixel = fgpixel;
    if (ggicurses_bitmap
        ||
        ((ggicurses_charwidth == 1)
         &&
         (ggicurses_charheight == 1)
         &&
         (! (attr & A_STANDOUT))
         &&
         ((ch == ' ')
          ||
          (ch == '\0')
          ||
          (ch == 0xfe)
          ||
          (ch == 0xff))))
    {
        ggi_pixel outpixel, outpixel2, outpixel3, outpixel4;

        outpixel =
            ((ch == ' ') || (ch == '\0') || (ch == 0xff))
            ?
            bgpixel
            :
            ((ch == 0xdf) || (ch == 0xdc) || (ch == 0xdd) || (ch == 0xde))
            ?
            halfpixel
            :
            fgpixel;
        outpixel2 = outpixel;
        if (ggicurses_bitmap_doubleheight)
        {
            if (ch == 0xdf)
            {
                outpixel = fgpixel;
                outpixel2 = bgpixel;
            }
            else if (ch == 0xdc)
            {
                outpixel = bgpixel;
                outpixel2 = fgpixel;
            }
        }
        outpixel3 = outpixel;
        outpixel4 = outpixel2;
        if (ggicurses_bitmap_double)
        {
            if (ch == 0xdd)
            {
                outpixel = (outpixel2 = fgpixel);
                outpixel3 = (outpixel4 = bgpixel);
            }
            else if (ch == 0xde)
            {
                outpixel = (outpixel2 = bgpixel);
                outpixel3 = (outpixel4 = fgpixel);
            }
        }
        if (ggicurses_bitmap_double || ggicurses_bitmap_doubleheight)
        {
            ggicurses_direct = 0;
        }
        if (ggicurses_direct)
        {
            unsigned char *pp;

            pp = (unsigned char *)(ggicurses_buffer[ggicurses_frame]->write)
                + y * ggicurses_charheight * ggicurses_buffer[ggicurses_frame]->buffer.plb.stride
                + x * ggicurses_charwidth * (ggicurses_bpp / CHAR_BIT);
            switch (ggicurses_bpp
                    *
                    ((((ggicurses_format->flags & GGI_PF_REVERSE_ENDIAN)
                       ?
                       1
                       :
                       0)
                      ^
                      ggicurses_reverse_endian)
                     ?
                     -1
                     :
                     1))
            {
            case -8:
            case 8:
                pp[0] = outpixel;
                break;
            case 16:
                pp[0] = outpixel >> 8;
                pp[1] = outpixel & 0xff;
                break;
            case -16:
                pp[1] = outpixel >> 8;
                pp[0] = outpixel & 0xff;
                break;
            case 24:
                pp[0] = outpixel >> 16;
                pp[1] = (outpixel >> 8) & 0xff;
                pp[2] = outpixel & 0xff;
                break;
            case -24:
                pp[2] = outpixel >> 16;
                pp[1] = (outpixel >> 8) & 0xff;
                pp[0] = outpixel & 0xff;
                break;
            case 32:
                pp[0] = outpixel >> 24;
                pp[1] = (outpixel >> 16) & 0xff;
                pp[2] = (outpixel >> 8) & 0xff;
                pp[3] = outpixel & 0xff;
                break;
            case -32:
                pp[3] = outpixel >> 24;
                pp[2] = (outpixel >> 16) & 0xff;
                pp[1] = (outpixel >> 8) & 0xff;
                pp[0] = outpixel & 0xff;
                break;
            default:
                ggicurses_direct = 0;
            }
        }
        if (! ggicurses_direct)
        {
            ggiPutPixel(ggicurses_visual, x * ggicurses_charwidth, y * ggicurses_charheight, outpixel);
            if (ggicurses_bitmap_doubleheight)
            {
                ggiPutPixel(ggicurses_visual, x * ggicurses_charwidth, y * ggicurses_charheight + (ggicurses_bitmap_double ? 2 : 1), outpixel2);
            }
            if (ggicurses_bitmap_double)
            {
                ggiPutPixel(ggicurses_visual, x * ggicurses_charwidth + 1, y * ggicurses_charheight, outpixel3);
                if (ggicurses_bitmap_doubleheight)
                {
                    ggiPutPixel(ggicurses_visual, x * ggicurses_charwidth + 1, y * ggicurses_charheight + (ggicurses_bitmap_double ? 2 : 1), outpixel4);
                }
            }
        }
    }
    else
    {
        if (attr != ggicurses_realattr)
        {
            ggicurses_realattr = attr;
            ggiSetGCForeground(ggicurses_visual, fgpixel);
            ggiSetGCBackground(ggicurses_visual, bgpixel);
        }
        if (ggicurses_ascii) ch = cp437_to_ascii(ch & 0xff);
        if (ggiPutc(ggicurses_visual, x * ggicurses_charwidth, y * ggicurses_charheight, ch))
        {
            if (ggicurses_ascii
                ||
                ggiPutc(ggicurses_visual, x * ggicurses_charwidth, y * ggicurses_charheight, cp437_to_ascii(ch & 0xff)))
            {
                ggiDrawBox(ggicurses_visual, x * ggicurses_charwidth, y * ggicurses_charheight, ggicurses_charwidth, ggicurses_charheight);
            }
        }
    }
    return OK;
}

static int move(y, x) {
    ggicurses_y = y;
    ggicurses_x = x;
    return OK;
}

static int clear(void) {
    memset((void *) (ggicurses_backing + ggicurses_w * ggicurses_h), 0, ggicurses_w * ggicurses_h * sizeof(ggicurses_cell_t));
    ggicurses_attr = A_NORMAL;
    move(0, 0);
    clearok(stdscr, TRUE);
    return OK;
}

static int attrset(int a)
{
    ggicurses_attr = a ? a : A_NORMAL;
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

    for (x = ggicurses_x; x < COLS; x ++)
    {
        move(ggicurses_y, x);
        addch(' ');
    }
    for (y = ggicurses_y + 1; y < LINES; y ++)
        for (x = 0; x < COLS; x ++)
        {
            move(y, x);
            addch(' ');
        }
    return OK;
}

static int clrtoeol(void) {
    unsigned int x;

    for (x = ggicurses_x; x < COLS; x ++)
    {
        move(ggicurses_y, x);
        addch(' ');
    }
    return OK;
}

static void endwin(void);

static int refresh(void);

static void ggicurses_resize(void)
{
    int fg, bg;

    /* FIXME: a resize should not disable direct buffering */
    ggicurses_direct = 0;
    ggiCheckMode(ggicurses_visual, &ggicurses_mode);
    ggiSetMode(ggicurses_visual, &ggicurses_mode);
    ggiGetMode(ggicurses_visual, &ggicurses_mode);
    if (ggicurses_bitmap || ggiGetCharSize(ggicurses_visual, &ggicurses_charwidth, &ggicurses_charheight))
    {
        ggicurses_charwidth = ggicurses_bitmap_double ? 2 : 1;
        ggicurses_charheight = ggicurses_bitmap_double ? 2 : 1;
        if (ggicurses_bitmap_doubleheight) ggicurses_charheight *= 2;
    }
    {
        int old_w;
        int old_h;
        int y;

        old_w = ggicurses_w;
        old_h = ggicurses_h;
        ggicurses_h = GGICURSES_LINES;
        ggicurses_w = GGICURSES_COLS;
        memmove((void *) ggicurses_backing, (void *) (ggicurses_backing + old_w * old_h), old_w * old_h * sizeof(ggicurses_cell_t));
        if ((old_w != ggicurses_w)
            ||
            (old_h != ggicurses_h))
        {
            if (ggicurses_w < old_w)
            {
                for (y = 0; (y < ggicurses_h) && (y < old_h); y ++)
                {
                    memmove((void *) (ggicurses_backing + y * ggicurses_w), (void *) (ggicurses_backing + y * old_w), ggicurses_w * sizeof(ggicurses_cell_t));
                }
            }
            if (ggicurses_w * ggicurses_h != old_w * old_h)
            {
                if (! (ggicurses_backing = (ggicurses_cell_t *) realloc((void *) ggicurses_backing, 2 * ggicurses_w * ggicurses_h * sizeof(ggicurses_cell_t))))
                {
                    perror("realloc");
                    fflush(stderr);
                    endwin();
                    exit(1);
                }
            }
            if (ggicurses_w > old_w)
            {
                for (y = ((ggicurses_h > old_h) ? old_h : ggicurses_h); y --;)
                {
                    memmove((void *) (ggicurses_backing + y * ggicurses_w), (void *) (ggicurses_backing + y * old_w), old_w * sizeof(ggicurses_cell_t));
                }
            }
            if ((ggicurses_w * ggicurses_h) > (old_w * old_h))
            {
                memset((void *) (ggicurses_backing + old_w * old_h), 0, (ggicurses_w * ggicurses_h - old_w * old_h) * sizeof(ggicurses_cell_t));
            }
        }
        memset((void *) (ggicurses_backing + ggicurses_w * ggicurses_h), 0, ggicurses_w * ggicurses_h * sizeof(ggicurses_cell_t));
    }
    ggiSetColorfulPalette(ggicurses_visual);
    for (fg = COLOR_BLACK; fg <= COLOR_WHITE; fg ++)
    {
        ggicurses_pixelmap[fg] = ggiMapColor(ggicurses_visual, &(ggicurses_colormap[fg]));
        ggicurses_pixelmap[fg | PEN_BRIGHT] = ggiMapColor(ggicurses_visual, &(ggicurses_colormap[fg | PEN_BRIGHT]));
    }
    for (bg = COLOR_BLACK; bg <= (COLOR_WHITE | PEN_BRIGHT); bg ++)
    {
        for (fg = COLOR_BLACK; fg <= bg; fg ++)
        {
            ggicurses_halfpixelmap[fg][bg] = ggiMapColor(ggicurses_visual, &(ggicurses_halfcolormap[fg][bg]));
            ggicurses_halfpixelmap[bg][fg] = ggicurses_halfpixelmap[fg][bg];
        }
    }
#if USE_GGIWMH
    if (ggicurses_wmh)
    {
	if (ggicurses_title) ggiWmhSetTitle(ggicurses_visual, ggicurses_title);
	ggiWmhAllowResize(ggicurses_visual,
                          /* minimum dimensions */
                          ggicurses_charwidth, ggicurses_charheight,
                          /* maximum dimensions */
                          32767 / ggicurses_charwidth * ggicurses_charwidth, 32767 / ggicurses_charheight * ggicurses_charheight,
                          /* step */
                          ggicurses_charwidth, ggicurses_charheight);
    }
#endif
}

static double ggicurses_now(void)
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
    int minx, miny, maxx, maxy;

    if (! ggicurses_ready) return OK;
    minx = ggicurses_w;
    miny = ggicurses_h;
    maxx = 0;
    maxy = 0;
    if (ggicurses_clearok)
    {
        ggicurses_realattr = 0;
        ggiSetGCForeground(ggicurses_visual, ggicurses_pixelmap[0]);
        ggiDrawBox(ggicurses_visual, 0, 0, ggicurses_mode.visible.x, ggicurses_mode.visible.y);
        ggiFlush(ggicurses_visual);
    }
    for (y = 0; y < ggicurses_h; y ++)
    {
        if ((ggicurses_direct && (ggicurses_frames > 1)) || ggicurses_clearok || ggicurses_backing[y * ggicurses_w].attr)
        {
            for (x = 0; x < ggicurses_w; x ++)
            {
                unsigned long ch;
                unsigned long attr;
                int pixdiff;
                
                attr = ggicurses_backing[y * ggicurses_w + x].attr;
                ch = ggicurses_backing[y * ggicurses_w + x].ch;
                pixdiff = ggicurses_clearok;
                if (ggicurses_clearok
                    ||
                    ((attr || (ggicurses_direct && (ggicurses_frames > 1)))
                     &&
                     (pixdiff = ((attr
                                  !=
                                  ggicurses_backing[y * ggicurses_w + x + ggicurses_w * ggicurses_h].attr)
                                 ||
                                 (ch
                                  !=
                                  ggicurses_backing[y * ggicurses_w + x + ggicurses_w * ggicurses_h].ch))))
                    ||
                    (ggicurses_direct && (ggicurses_frames > 1)))
                {
                    if (! ch)
                    {
                        attr = ggicurses_backing[y * ggicurses_w + x + ggicurses_w * ggicurses_h].attr;
                        ch = ggicurses_backing[y * ggicurses_w + x + ggicurses_w * ggicurses_h].ch;
                    }
                    ggicurses_backing[y * ggicurses_w + x + ggicurses_w * ggicurses_h].attr = attr;
                    ggicurses_backing[y * ggicurses_w + x + ggicurses_w * ggicurses_h].ch = ch;
                    ggicurses_backing[y * ggicurses_w + x].attr = 0;
                    ggicurses_backing[y * ggicurses_w + x].ch = 0;
                    if (pixdiff || ! (ggicurses_direct && (ggicurses_frames > 1)))
                    {
                        if (x < minx) minx = x;
                        if (y < miny) miny = y;
                        if (x >= maxx) maxx = x + 1;
                        if (y >= maxy) maxy = y + 1;
                    }
                    ggicurses_addch(ch ? ch : ' ', attr, x, y);
                }
            }
            ggicurses_backing[y * ggicurses_w].attr = 0;
        }
    }
    ggicurses_clearok = 0;
    if (ggicurses_direct)
    {
        ggiResourceRelease(ggicurses_buffer[ggicurses_frame]->resource);
        if ((ggicurses_mode.frames > 1)
            &&
            (ggiGetDisplayFrame(ggicurses_visual) != ggicurses_buffer[ggicurses_frame]->frame)
            &&
            ggiSetDisplayFrame(ggicurses_visual, ggicurses_buffer[ggicurses_frame]->frame))
        {
            endwin();
            fprintf(stderr, "ggicurses: ggiSetDisplayFrame failed\n");
            fflush(stderr);
            exit(1);
        }
    }
    if ((maxy > 0) && (maxx > 0))
    {
        if (ggicurses_flushall)
        {
            ggiFlush(ggicurses_visual);
        }
        else
        {
            long x, y, w, h;

            x = minx * ggicurses_charwidth;
            y = miny * ggicurses_charheight;
            w = (maxx - minx) * ggicurses_charwidth;
            h = (maxy - miny) * ggicurses_charheight;
            if (x < 0)
            {
                w += x;
                x = 0;
            }
            if (y < 0)
            {
                h += y;
                y = 0;
            }
            if ((x + w) > ggicurses_mode.visible.x)
            {
                w = ggicurses_mode.visible.x - x;
            }
            if ((y + h) > ggicurses_mode.visible.y)
            {
                h = ggicurses_mode.visible.y - y;
            }
            if ((w != ggicurses_mode.visible.x)
                ||
                (h != ggicurses_mode.visible.y))
            {
                ggiFlushRegion(ggicurses_visual,
                               x, y, w, h);
            }
            else
            {
                ggiFlush(ggicurses_visual);
            }
        }
    }
    if (ggicurses_direct)
    {
        ggicurses_frame = (ggicurses_frame + 1) % ggicurses_frames;
        if (ggiResourceAcquire(ggicurses_buffer[ggicurses_frame]->resource,
                               GGI_ACTYPE_READ | GGI_ACTYPE_WRITE))
        {
            endwin();
            fprintf(stderr, "ggicurses: failed to acquire directbuffer\n");
            fflush(stderr);
            exit(1);
        }
        ggicurses_format = ggicurses_buffer[ggicurses_frame]->buffer.plb.pixelformat;
        ggicurses_bpp = ggicurses_format->size;
        if (! ggicurses_bpp)
        {
            ggicurses_bpp = GT_SIZE(ggicurses_mode.graphtype);
        }
        if ((ggicurses_mode.frames > 1)
            &&
            (ggiGetWriteFrame(ggicurses_visual) != ggicurses_buffer[ggicurses_frame]->frame)
            &&
            ggiSetWriteFrame(ggicurses_visual, ggicurses_buffer[ggicurses_frame]->frame))
        {
            endwin();
            fprintf(stderr, "ggicurses: ggiSetWriteFrame failed\n");
            fflush(stderr);
            exit(1);
        }
        if ((ggicurses_mode.frames > 1)
            &&
            (ggiGetReadFrame(ggicurses_visual) != ggicurses_buffer[ggicurses_frame]->frame)
            &&
            ggiSetReadFrame(ggicurses_visual, ggicurses_buffer[ggicurses_frame]->frame))
        {
            endwin();
            fprintf(stderr, "ggicurses: ggiSetReadFrame failed\n");
            fflush(stderr);
            exit(1);
        }
    }
    {
        int newx;
        int newy;

        newx = ggicurses_mode.visible.x;
        newy = ggicurses_mode.visible.y;
#if USE_GGIWMH
        if (ggicurses_wmh)
        {
            if (! ggiWmhGetSize(ggicurses_visual, &newx, &newy))
            {
                ggicurses_unmapped = 0;
                if ((newx > 0)
                    &&
                    (newy > 0)
                    &&
                    ((ggicurses_mode.visible.x != newx)
                     ||
                     (ggicurses_mode.visible.y != newy)))
                {
                    ggicurses_mode.visible.x = newx;
                    ggicurses_mode.visible.y = newy;
                    ggicurses_need_resize = ggicurses_now();
                }
                else if ((newx <= 0)
                         ||
                         (newy <= 0))
                {
                    ggicurses_unmapped = 1;
                }
            }
        }
#endif
        if ((ggicurses_need_resize != 0.0)
            &&
            ((ggicurses_now() - ggicurses_need_resize) >= GGICURSES_RESIZE_INHIBIT))
        {
            ggicurses_resize();
            ggicurses_need_resize = 0.0;
        }
    }
    return OK;
}

static chtype getch(void)
{
    struct timeval tv = { 0, 0 };
    int gotevent;

    while ((gotevent = ggiEventPoll(ggicurses_visual, emKeyPress | emExpose | emCommand | (ggicurses_unmapped ? ~0UL : 0), &tv))
           ||
           ggicurses_unmapped)
    {
        ggi_event ev;

        ev.any.type = evNothing;
        if (gotevent)
        {
            ggiEventRead(ggicurses_visual, &ev, emKeyPress | emExpose | emCommand | (ggicurses_unmapped ? ~0UL : 0));
        }
        if (ev.any.type == evKeyPress)
        {
            ggicurses_unmapped = 0;
            if (GII_KTYP(ev.key.sym) != GII_KT_MOD)
            {
                return ev.key.sym;
            }
        }
        else if (ev.any.type == evExpose)
        {
            int x;
            int y;
            int px;
            int py;

            for (py = ev.expose.y; py < ev.expose.y + ev.expose.h; py ++)
            {
                y = py / ggicurses_charheight;
                if (y >= ggicurses_h) continue;
                ggicurses_backing[y * ggicurses_w].attr = ggicurses_backing[y * ggicurses_w + ggicurses_w * ggicurses_h].attr;
                ggicurses_backing[y * ggicurses_w].ch = ggicurses_backing[y * ggicurses_w + ggicurses_w * ggicurses_h].ch;
                if (! ggicurses_backing[y * ggicurses_w].attr)
                {
                    ggicurses_backing[y * ggicurses_w].attr = A_NORMAL;
                    ggicurses_backing[y * ggicurses_w].ch = 0;
                }
                for (px = ev.expose.x; px < ev.expose.x + ev.expose.w; px ++)
                {
                    x = px / ggicurses_charwidth;
                    if (x >= ggicurses_w) continue;
                    ggicurses_backing[y * ggicurses_w + x].attr = ggicurses_backing[y * ggicurses_w + x + ggicurses_w * ggicurses_h].attr;
                    ggicurses_backing[y * ggicurses_w + x].ch = ggicurses_backing[y * ggicurses_w + x + ggicurses_w * ggicurses_h].ch;
                    if (! ggicurses_backing[y * ggicurses_w + x].attr)
                    {
                        ggicurses_backing[y * ggicurses_w + x].attr = A_NORMAL;
                        ggicurses_backing[y * ggicurses_w + x].ch = 0;
                    }
                }
            }
            refresh();
        }
        else if (ev.any.type == evCommand)
        {
            if ((ev.cmd.code == GGICMD_REQUEST_SWITCH)
                &&
                (((ggi_cmddata_switchrequest *) &(ev.cmd.data))->request & GGI_REQSW_MODE))
            {
                if (memcmp((void *) &ggicurses_mode,
                           (void *) &(((ggi_cmddata_switchrequest *) &(ev.cmd.data))->mode),
                           sizeof(ggi_mode))
                    &&
                    ((ggi_cmddata_switchrequest *) &(ev.cmd.data))->mode.visible.x
                    &&
                    ((ggi_cmddata_switchrequest *) &(ev.cmd.data))->mode.visible.y)
                {
                    memcpy((void *) &ggicurses_mode,
                           (void *) &(((ggi_cmddata_switchrequest *) &(ev.cmd.data))->mode),
                           sizeof(ggi_mode));
                    ggicurses_need_resize = ggicurses_now();
                }
#if USE_GGIWMH
                if (ggicurses_wmh)
                {
                    int newx;
                    int newy;

                    newx = ggicurses_mode.visible.x;
                    newy = ggicurses_mode.visible.y;
                    if (! ggiWmhGetSize(ggicurses_visual, &newx, &newy))
                    {
                        ggicurses_unmapped = 0;
                        if (((newx != ggicurses_mode.visible.x)
                             ||
                             (newy != ggicurses_mode.visible.y))
                            &&
                            (newx > 0)
                            &&
                            (newy > 0))
                        {
                            ggicurses_mode.visible.x = newx;
                            ggicurses_mode.visible.y = newy;
                            ggicurses_need_resize = ggicurses_now();
                        }
                        else if ((newx <= 0)
                                 ||
                                 (newy <= 0))
                        {
                            ggicurses_unmapped = 1;
                        }
                    }
                }
#endif
            }
        }
        if (ggicurses_unmapped)
        {
            refresh();
        }
        if (ggicurses_unmapped)
        {
            tv.tv_sec = 1;
        }
        else
        {
            tv.tv_sec = 0;
        }
    }
    ggicurses_unmapped = 0;
    return ERR;
}

static void standout() {
    ggicurses_attr = A_STANDOUT;
}

static void standend() {
    ggicurses_attr = 0;
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

static unsigned int ggicurses_mix(unsigned int a, unsigned int b)
{
    unsigned long r;

    r = 0xffffUL * sqrt(1.0 * a / 0xffffUL * a / 0xffffUL + 1.0 * b / 0xffffUL * b / 0xffffUL) / M_SQRT2;
    if (r > 0xffffUL)
    {
        r = 0xffffUL;
    }
    return (unsigned int) r;
}

static int init_color(int i, short r, short g, short b) {
    int bg;

    ggicurses_colormap[i].r = 0xffff * (long) (ggicurses_swaprb ? b : r) / 1000;
    ggicurses_colormap[i].g = 0xffff * (long) g / 1000;
    ggicurses_colormap[i].b = 0xffff * (long) (ggicurses_swaprb ? r : b) / 1000;
    ggicurses_colormap[i].a = 0x0000;
    ggicurses_pixelmap[i] = ggiMapColor(ggicurses_visual, &(ggicurses_colormap[i]));
    for (bg = COLOR_BLACK; bg <= (COLOR_WHITE | PEN_BRIGHT); bg ++)
    {
        ggicurses_halfcolormap[i][bg].r = ggicurses_mix(ggicurses_colormap[i].r, ggicurses_colormap[bg].r);
        ggicurses_halfcolormap[i][bg].g = ggicurses_mix(ggicurses_colormap[i].g, ggicurses_colormap[bg].g);
        ggicurses_halfcolormap[i][bg].b = ggicurses_mix(ggicurses_colormap[i].b, ggicurses_colormap[bg].b);
        ggicurses_halfcolormap[i][bg].a = ggicurses_mix(ggicurses_colormap[i].a, ggicurses_colormap[bg].a);
        ggicurses_halfpixelmap[i][bg] = ggiMapColor(ggicurses_visual, &(ggicurses_halfcolormap[i][bg]));
        ggicurses_halfcolormap[bg][i] = ggicurses_halfcolormap[i][bg];
        ggicurses_halfpixelmap[bg][i] = ggicurses_halfpixelmap[i][bg];
    }
    return OK;
}

static int color_content(int i, short *r, short *g, short *b) {
    *(ggicurses_swaprb ? b : r) = (long) ggicurses_colormap[i].r * 1000 / 0xffff;
    *g = (long) ggicurses_colormap[i].g * 1000 / 0xffff;
    *(ggicurses_swaprb ? r : b) = (long) ggicurses_colormap[i].b * 1000 / 0xffff;
    return OK;
}

static int init_pair(int i, short fg, short bg) {
    ggicurses_pairs[i] = (bg << 4) | fg;
    return OK;
}

static int pair_content(int i, short *fg, short *bg) {
    *fg = ggicurses_pairs[i] & 0xf;
    *bg = ggicurses_pairs[i] >> 4;
    return OK;
}

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    int fg, bg;
    int auto_x, auto_y;

    ggicurses_title = title;
    ggicurses_bitmap = myman_getenv("GGICURSES_BITMAP");
    if (ggicurses_bitmap && ! *ggicurses_bitmap) ggicurses_bitmap = NULL;
    ggicurses_swaprb = myman_getenv("GGICURSES_SWAPRB");
    if (ggicurses_swaprb && ! *ggicurses_swaprb) ggicurses_swaprb = NULL;
    ggicurses_ascii = myman_getenv("GGICURSES_ASCII");
    if (ggicurses_ascii && ! *ggicurses_ascii) ggicurses_ascii = NULL;
    ggicurses_flushall = myman_getenv("GGICURSES_FLUSHALL");
    if (ggicurses_flushall && ! *ggicurses_flushall) ggicurses_flushall = NULL;
    ggicurses_fixedpal = myman_getenv("GGICURSES_FIXEDPAL");
    if (ggicurses_fixedpal && ! *ggicurses_fixedpal) ggicurses_fixedpal = NULL;
    ggicurses_bitmap_double = myman_getenv("GGICURSES_BITMAP_DOUBLE");
    if (ggicurses_bitmap_double && ! *ggicurses_bitmap_double) ggicurses_bitmap_double = NULL;
    ggicurses_bitmap_doubleheight = myman_getenv("GGICURSES_BITMAP_DOUBLEHEIGHT");
    if (ggicurses_bitmap_doubleheight && ! *ggicurses_bitmap_doubleheight) ggicurses_bitmap_doubleheight = NULL;
    if (! ggicurses_bitmap)
    {
        if (w > 128) w = 128;
        if (h > 80) h = 80;
    }
    if (ggiInit())
    {
        fprintf(stderr, "ggiInit failed\n");
        fflush(stderr);
        exit(1);
    }
    ggicurses_wmh = 0;
#if USE_GGIWMH
    if (! ggiWmhInit())
    {
        ggicurses_wmh = 1;
    }
#endif
    if (! (ggicurses_visual = ggiOpen(NULL)))
    {
#if USE_GGIWMH
        if (ggicurses_wmh)
        {
            ggiWmhExit();
        }
#endif
        ggiExit();
        fprintf(stderr, "ggiOpen failed\n");
        fflush(stderr);
        exit(1);
    }
    ggiParseMode("", &ggicurses_mode);
    auto_x = ggicurses_mode.visible.x == GGI_AUTO;
    auto_y = ggicurses_mode.visible.y == GGI_AUTO;
    ggicurses_mode.frames = 2;
    ggiAddFlags(ggicurses_visual, GGIFLAG_ASYNC);
    ggiCheckMode(ggicurses_visual, &ggicurses_mode);
    if (ggiSetMode(ggicurses_visual, &ggicurses_mode) < 0)
    {
        ggiParseMode("", &ggicurses_mode);
        ggiCheckMode(ggicurses_visual, &ggicurses_mode);
        if (ggiSetMode(ggicurses_visual, &ggicurses_mode) < 0)
        {
            fprintf(stderr, "ggiSetMode failed\n");
            fflush(stderr);
            ggiClose(ggicurses_visual);
#if USE_GGIWMH
            if (ggicurses_wmh)
            {
                ggiWmhExit();
            }
#endif
            ggiExit();
            exit(1);
        }
    }
    ggiGetMode(ggicurses_visual, &ggicurses_mode);
#if USE_GGIWMH
    if (ggicurses_wmh && (ggiWmhAttach(ggicurses_visual) < 0))
    {
        ggiWmhExit();
        ggicurses_wmh = 0;
    }
#endif
    if (ggicurses_bitmap || ggiGetCharSize(ggicurses_visual, &ggicurses_charwidth, &ggicurses_charheight))
    {
        ggicurses_charwidth = ggicurses_bitmap_double ? 2 : 1;
        ggicurses_charheight = ggicurses_bitmap_double ? 2 : 1;
        if (ggicurses_bitmap_doubleheight) ggicurses_charheight *= 2;
    }
    if (auto_x
        &&
        ((ggicurses_mode.visible.x == GGI_AUTO)
         ||
         (ggicurses_mode.visible.x > (w * ggicurses_charwidth))))
    {
        ggicurses_mode.visible.x = w * ggicurses_charwidth;
    }
    if (auto_y
        &&
        ((ggicurses_mode.visible.y == GGI_AUTO)
         ||
         (ggicurses_mode.visible.y > (h * ggicurses_charheight))))
    {
        ggicurses_mode.visible.y = h * ggicurses_charheight;
    }
    if (auto_x || auto_y)
    {
        ggiCheckMode(ggicurses_visual, &ggicurses_mode);
#if USE_GGIWMH
        if (ggicurses_wmh)
        {
            ggiWmhDetach(ggicurses_visual);
        }
#endif
        ggiSetMode(ggicurses_visual, &ggicurses_mode);
#if USE_GGIWMH
        if (ggicurses_wmh && (ggiWmhAttach(ggicurses_visual) < 0))
        {
            ggiWmhExit();
            ggicurses_wmh = 0;
        }
#endif
    }
    ggiGetMode(ggicurses_visual, &ggicurses_mode);
    ggiSetColorfulPalette(ggicurses_visual);
    for (fg = COLOR_BLACK; fg <= COLOR_WHITE; fg ++)
    {
        if (fg)
        {
            ggicurses_colormap[fg].r = (fg & (ggicurses_swaprb ? COLOR_BLUE : COLOR_RED)) ? 0x8888 : 0x0000;
            ggicurses_colormap[fg].g = (fg & COLOR_GREEN) ? 0x8888 : 0x0000;
            ggicurses_colormap[fg].b = (fg & (ggicurses_swaprb ? COLOR_RED : COLOR_BLUE)) ? 0x8888 : 0x0000;
            ggicurses_colormap[fg].a = 0x0000;
            ggicurses_colormap[fg | PEN_BRIGHT].r = ggicurses_colormap[fg].r ? 0xffff : 0x8888;
            ggicurses_colormap[fg | PEN_BRIGHT].g = ggicurses_colormap[fg].g ? 0xffff : 0x8888;
            ggicurses_colormap[fg | PEN_BRIGHT].b = ggicurses_colormap[fg].b ? 0xffff : 0x8888;
            ggicurses_colormap[fg | PEN_BRIGHT].a = ggicurses_colormap[fg].a;
        }
        else
        {
            ggicurses_colormap[fg].r = 0x0000;
            ggicurses_colormap[fg].g = 0x0000;
            ggicurses_colormap[fg].b = 0x0000;
            ggicurses_colormap[fg].a = 0x0000;
            ggicurses_colormap[fg | PEN_BRIGHT].r = 0x6666;
            ggicurses_colormap[fg | PEN_BRIGHT].g = 0x6666;
            ggicurses_colormap[fg | PEN_BRIGHT].b = 0x6666;
            ggicurses_colormap[fg | PEN_BRIGHT].a = 0x0000;
        }
        ggicurses_pixelmap[fg] = ggiMapColor(ggicurses_visual, &(ggicurses_colormap[fg]));
        ggicurses_pixelmap[fg | PEN_BRIGHT] = ggiMapColor(ggicurses_visual, &(ggicurses_colormap[fg | PEN_BRIGHT]));
    }
    for (bg = COLOR_BLACK; bg <= (COLOR_WHITE | PEN_BRIGHT); bg ++)
    {
        for (fg = COLOR_BLACK; fg <= bg; fg ++)
        {
            ggicurses_halfcolormap[fg][bg].r = ggicurses_mix(ggicurses_colormap[fg].r, ggicurses_colormap[bg].r);
            ggicurses_halfcolormap[fg][bg].g = ggicurses_mix(ggicurses_colormap[fg].g, ggicurses_colormap[bg].g);
            ggicurses_halfcolormap[fg][bg].b = ggicurses_mix(ggicurses_colormap[fg].b, ggicurses_colormap[bg].b);
            ggicurses_halfcolormap[fg][bg].a = ggicurses_mix(ggicurses_colormap[fg].a, ggicurses_colormap[bg].a);
            ggicurses_halfpixelmap[fg][bg] = ggiMapColor(ggicurses_visual, &(ggicurses_halfcolormap[fg][bg]));
            ggicurses_halfcolormap[bg][fg] = ggicurses_halfcolormap[fg][bg];
            ggicurses_halfpixelmap[bg][fg] = ggicurses_halfpixelmap[fg][bg];
        }
    }
    ggicurses_h = GGICURSES_LINES;
    ggicurses_w = GGICURSES_COLS;
    if (! (ggicurses_backing = (ggicurses_cell_t *) malloc(2 * ggicurses_w * ggicurses_h * sizeof(ggicurses_cell_t))))
    {
        perror("malloc");
        fflush(stderr);
#if USE_GGIWMH
        if (ggicurses_wmh)
        {
            ggiWmhDetach(ggicurses_visual);
        }
#endif
        ggiClose(ggicurses_visual);
#if USE_GGIWMH
        if (ggicurses_wmh)
        {
            ggiWmhExit();
        }
#endif
        ggiExit();
        exit(1);
    }
    memset((void *) ggicurses_backing, 0, 2 * ggicurses_w * ggicurses_h * sizeof(ggicurses_cell_t));
    ggicurses_need_resize = 0.0;
    ggicurses_unmapped = 0;
    clearok(stdscr, TRUE);
    ggicurses_attr = A_NORMAL;
    ggicurses_realattr = 0;
    ggicurses_frame = 0;
    ggicurses_frames = ggiDBGetNumBuffers(ggicurses_visual);
    if (ggicurses_frames > 2)
    {
        ggicurses_frames = 2;
    }
    ggicurses_direct = 0;
    if (ggicurses_bitmap)
    {
        int i;

        for (i = 0; i < ggicurses_frames; i ++)
        {
            ggicurses_buffer[ggicurses_frame] = ggiDBGetBuffer(ggicurses_visual, i);
            if (ggicurses_buffer[ggicurses_frame]->type & GGI_DB_SIMPLE_PLB)
            {
                ggicurses_frame ++;
            }
        }
        ggicurses_frames = ggicurses_frame;
        ggicurses_frame = 0;
        if (! ggicurses_frames)
        {
            /* no directbuffers */
        }
        else if (ggiResourceAcquire(ggicurses_buffer[ggicurses_frame]->resource,
                               GGI_ACTYPE_READ | GGI_ACTYPE_WRITE))
        {
            /* could not actually acquire a directbuffer */
        }
        else
        {
            if (ggicurses_buffer[ggicurses_frame]->write
                &&
                ggicurses_buffer[ggicurses_frame]->read)
            {
                ggicurses_format = ggicurses_buffer[ggicurses_frame]->buffer.plb.pixelformat;
                ggicurses_bpp = ggicurses_format->size;
                if (! ggicurses_bpp)
                {
                    ggicurses_bpp = GT_SIZE(ggicurses_mode.graphtype);
                }
                if ((CHAR_BIT == 8)
                    &&
                    ((ggicurses_bpp == 8)
                     ||
                     (ggicurses_bpp == 16)
                     ||
                     (ggicurses_bpp == 24)
                     ||
                     (ggicurses_bpp == 32)))
                {
                    ggicurses_reverse_endian = 0x12;
                    ggicurses_reverse_endian = (*((unsigned char *)(void *)(& ggicurses_reverse_endian)) == ggicurses_reverse_endian);
                    if (myman_getenv("GGICURSES_DIRECT")
                        &&
                        *(myman_getenv("GGICURSES_DIRECT")))
                    {
                        ggicurses_direct = 1;
                    }
                }
            }
            if (! ggicurses_direct)
            {
                ggiResourceRelease(ggicurses_buffer[ggicurses_frame]->resource);
            }
        }
    }
    if ((ggicurses_mode.frames > 1)
        &&
        (ggiGetWriteFrame(ggicurses_visual) !=
         (ggicurses_direct
          ?
          ggicurses_buffer[ggicurses_frame]->frame
          :
          ggicurses_frame))
        &&
        ggiSetWriteFrame(ggicurses_visual,
                         (ggicurses_direct
                          ?
                          ggicurses_buffer[ggicurses_frame]->frame
                          :
                          ggicurses_frame)))
    {
        fprintf(stderr,
                "ggicurses: ggiSetWriteFrame failed for frame %d\n",
                (ggicurses_direct
                 ?
                 ggicurses_buffer[ggicurses_frame]->frame
                 :
                 ggicurses_frame));
        fflush(stderr);
        endwin();
        exit(1);
    }
    if ((ggicurses_mode.frames > 1)
        &&
        (ggiGetReadFrame(ggicurses_visual) !=
         (ggicurses_direct
          ?
          ggicurses_buffer[ggicurses_frame]->frame
          :
          ggicurses_frame))
        &&
        ggiSetReadFrame(ggicurses_visual,
                         (ggicurses_direct
                          ?
                          ggicurses_buffer[ggicurses_frame]->frame
                          :
                          ggicurses_frame)))
    {
        fprintf(stderr,
                "ggicurses: ggiSetReadFrame failed for frame %d\n",
                (ggicurses_direct
                 ?
                 ggicurses_buffer[ggicurses_frame]->frame
                 :
                 ggicurses_frame));
        fflush(stderr);
        endwin();
        exit(1);
    }
    if ((! ggicurses_direct)
        &&
        (ggicurses_mode.frames > 1)
        &&
        (ggiGetDisplayFrame(ggicurses_visual) != ggicurses_frame)
        &&
        ggiSetDisplayFrame(ggicurses_visual, ggicurses_frame))
    {
        fprintf(stderr,
                "ggicurses: ggiSetDisplayFrame failed for frame %d\n",
                (ggicurses_direct
                 ?
                 ggicurses_buffer[ggicurses_frame]->frame
                 :
                 ggicurses_frame));
        fflush(stderr);
        endwin();
        exit(1);
    }
#if USE_GGIWMH
    if (ggicurses_wmh)
    {
	if (ggicurses_title) ggiWmhSetTitle(ggicurses_visual, ggicurses_title);
	ggiWmhAllowResize(ggicurses_visual,
                          /* minimum dimensions */
                          ggicurses_charwidth, ggicurses_charheight,
                          /* maximum dimensions */
                          32767 / ggicurses_charwidth * ggicurses_charwidth, 32767 / ggicurses_charheight * ggicurses_charheight,
                          /* step */
                          ggicurses_charwidth, ggicurses_charheight);
    }
#endif
    ggicurses_ready = 1;
}

static void endwin(void) {
    free(ggicurses_backing);
#if USE_GGIWMH
    if (ggicurses_wmh)
    {
        ggiWmhDetach(ggicurses_visual);
    }
#endif
    ggiClose(ggicurses_visual);
#if USE_GGIWMH
    if (ggicurses_wmh)
    {
        ggiWmhExit();
    }
#endif
    ggiExit();
    ggicurses_ready = 0;
}

static int ggicurses_wcwidth(const chtype ch) {
    return (ch < 0x20) ? 0 : 1;
}

#undef wcwidth
#define wcwidth ggicurses_wcwidth

#endif /* MYMAN_GGICURS_H_INCLUDED */
