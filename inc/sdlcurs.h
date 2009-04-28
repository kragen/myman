/*
 * sdlcurs.h - SDL driver for the MyMan video game
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

#error SDL driver is not yet finished -- use pdcursessdl for now

#ifndef MYMAN_SDLCURS_H_INCLUDED
#define MYMAN_SDLCURS_H_INCLUDED 1

#include <SDL.h>

/* for CHAR_BIT */
#include <limits.h>

/* for sqrt */
#include <math.h>

/* for struct timeval */
#include <sys/time.h>

/* for gettimeofday */
#include <time.h>

/* work-arounds for SDL */

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

static int sdlcurses_ready = 0;

static const char *sdlcurses_title;

static double sdlcurses_need_resize;

static int sdlcurses_unmapped;

/* time in seconds to wait before handling a resize */
#define SDLCURSES_RESIZE_INHIBIT 1.0

static char *sdlcurses_bitmap;

static char *sdlcurses_swaprb;

static char *sdlcurses_ascii;

static char *sdlcurses_flushall;

static char *sdlcurses_fixedpal;

static char *sdlcurses_bitmap_double;

static char *sdlcurses_bitmap_doubleheight;

struct sdlcurses_cell {
    unsigned long ch;
    unsigned long attr;
};

typedef struct sdlcurses_cell sdlcurses_cell_t;

static int sdlcurses_clearok;

static sdlcurses_cell_t *sdlcurses_backing;

typedef unsigned long int chtype;

typedef chtype attr_t;

static SDL_Surface *sdlcurses_screen;

static sdl_mode sdlcurses_mode;

static int sdlcurses_direct;
static unsigned int sdlcurses_reverse_endian;
static int sdlcurses_bpp;
static int sdlcurses_frame;
static int sdlcurses_frames;
static const SDL_PixelFormat *sdlcurses_format;
static const sdl_directbuffer *sdlcurses_buffer[2] = {0, 0};

static unsigned long sdlcurses_x, sdlcurses_y, sdlcurses_w, sdlcurses_h;

static int sdlcurses_charheight, sdlcurses_charwidth;

static unsigned long sdlcurses_attr, sdlcurses_realattr;

static Uint32 sdlcurses_pixelmap[16], sdlcurses_halfpixelmap[16][16];
static SDL_Color sdlcurses_colormap[16], sdlcurses_halfcolormap[16][16];

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

#define clearok(stdscr, x) (x ? (sdlcurses_clearok = 1) : 0)

#define leaveok(stdscr, x)

#define getyx(stdscr, y, x) ((y) = sdlcurses_y), ((x) = sdlcurses_x)

#define wrefresh(stdscr) refresh()

#define beep() addch('\7')

#define use_env(x)

#define echo()

#define curs_set(x)

#define keypad(stdscr, x)

#define can_change_color() (sdlcurses_fixedpal ? 0 : 1)

#define has_colors() 1

#define start_color()

#define SDLCURSES_LINES (sdlcurses_mode.visible.y / sdlcurses_charheight)

#define SDLCURSES_COLS (sdlcurses_mode.visible.x / sdlcurses_charwidth)

#define LINES sdlcurses_h

#define COLS sdlcurses_w

#define COLORS 16

#define COLOR_PAIRS 256

static unsigned char sdlcurses_pairs[COLOR_PAIRS];

#define COLOR_PAIR(i) (A_NORMAL | sdlcurses_pairs[(i)])

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

static int addch(const chtype ch)
{
    if ((! sdlcurses_clearok)
        &&
        (! sdlcurses_backing[sdlcurses_y * sdlcurses_w + sdlcurses_x].attr)
        &&
        (sdlcurses_backing[sdlcurses_y * sdlcurses_w + sdlcurses_x + sdlcurses_w * sdlcurses_h].ch == ch)
        &&
        (sdlcurses_backing[sdlcurses_y * sdlcurses_w + sdlcurses_x + sdlcurses_w * sdlcurses_h].attr == (sdlcurses_attr ? sdlcurses_attr : A_NORMAL)))
    {
        sdlcurses_x ++;
        return OK;
    }
    sdlcurses_backing[sdlcurses_y * sdlcurses_w + sdlcurses_x].ch = ch;
    sdlcurses_backing[sdlcurses_y * sdlcurses_w + sdlcurses_x].attr = sdlcurses_attr ? sdlcurses_attr : A_NORMAL;
    if (! sdlcurses_backing[sdlcurses_y * sdlcurses_w].attr)
    {
        sdlcurses_backing[sdlcurses_y * sdlcurses_w].attr = A_NORMAL;
        sdlcurses_backing[sdlcurses_y * sdlcurses_w].ch = 0;
    }
    sdlcurses_x ++;
    return OK;
}

static int sdlcurses_addch(unsigned long ch, unsigned long attr, unsigned long x, unsigned long y) {
    int fg, bg;
    Uint32 fgpixel, bgpixel, halfpixel;

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
    fgpixel = sdlcurses_pixelmap[fg];
    bgpixel = sdlcurses_pixelmap[bg];
    halfpixel = sdlcurses_halfpixelmap[fg][bg];
    if (halfpixel == bgpixel) halfpixel = fgpixel;
    if (sdlcurses_bitmap
        ||
        ((sdlcurses_charwidth == 1)
         &&
         (sdlcurses_charheight == 1)
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
        Uint32 outpixel, outpixel2, outpixel3, outpixel4;

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
        if (sdlcurses_bitmap_doubleheight)
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
        if (sdlcurses_bitmap_double)
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
        if (sdlcurses_bitmap_double || sdlcurses_bitmap_doubleheight)
        {
            sdlcurses_direct = 0;
        }
        if (sdlcurses_direct)
        {
            unsigned char *pp;

            pp = (unsigned char *)(sdlcurses_buffer[sdlcurses_frame]->write)
                + y * sdlcurses_charheight * sdlcurses_buffer[sdlcurses_frame]->buffer.plb.stride
                + x * sdlcurses_charwidth * (sdlcurses_bpp / CHAR_BIT);
            switch (sdlcurses_bpp
                    *
                    ((((sdlcurses_format->flags & SDL_PF_REVERSE_ENDIAN)
                       ?
                       1
                       :
                       0)
                      ^
                      sdlcurses_reverse_endian)
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
                sdlcurses_direct = 0;
            }
        }
        if (! sdlcurses_direct)
        {
            sdlPutPixel(sdlcurses_screen, x * sdlcurses_charwidth, y * sdlcurses_charheight, outpixel);
            if (sdlcurses_bitmap_doubleheight)
            {
                sdlPutPixel(sdlcurses_screen, x * sdlcurses_charwidth, y * sdlcurses_charheight + (sdlcurses_bitmap_double ? 2 : 1), outpixel2);
            }
            if (sdlcurses_bitmap_double)
            {
                sdlPutPixel(sdlcurses_screen, x * sdlcurses_charwidth + 1, y * sdlcurses_charheight, outpixel3);
                if (sdlcurses_bitmap_doubleheight)
                {
                    sdlPutPixel(sdlcurses_screen, x * sdlcurses_charwidth + 1, y * sdlcurses_charheight + (sdlcurses_bitmap_double ? 2 : 1), outpixel4);
                }
            }
        }
    }
    else
    {
        if (attr != sdlcurses_realattr)
        {
            sdlcurses_realattr = attr;
            sdlSetGCForeground(sdlcurses_screen, fgpixel);
            sdlSetGCBackground(sdlcurses_screen, bgpixel);
        }
        if (sdlcurses_ascii) ch = cp437_to_ascii(ch & 0xff);
        if (sdlPutc(sdlcurses_screen, x * sdlcurses_charwidth, y * sdlcurses_charheight, ch))
        {
            if (sdlcurses_ascii
                ||
                sdlPutc(sdlcurses_screen, x * sdlcurses_charwidth, y * sdlcurses_charheight, cp437_to_ascii(ch & 0xff)))
            {
                sdlDrawBox(sdlcurses_screen, x * sdlcurses_charwidth, y * sdlcurses_charheight, sdlcurses_charwidth, sdlcurses_charheight);
            }
        }
    }
    return OK;
}

static int move(y, x) {
    sdlcurses_y = y;
    sdlcurses_x = x;
    return OK;
}

static int clear(void) {
    memset((void *) (sdlcurses_backing + sdlcurses_w * sdlcurses_h), 0, sdlcurses_w * sdlcurses_h * sizeof(sdlcurses_cell_t));
    sdlcurses_attr = A_NORMAL;
    move(0, 0);
    clearok(stdscr, TRUE);
    return OK;
}

static int attrset(int a)
{
    sdlcurses_attr = a ? a : A_NORMAL;
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

    for (x = sdlcurses_x; x < COLS; x ++)
    {
        move(sdlcurses_y, x);
        addch(' ');
    }
    for (y = sdlcurses_y + 1; y < LINES; y ++)
        for (x = 0; x < COLS; x ++)
        {
            move(y, x);
            addch(' ');
        }
    return OK;
}

static int clrtoeol(void) {
    unsigned int x;

    for (x = sdlcurses_x; x < COLS; x ++)
    {
        move(sdlcurses_y, x);
        addch(' ');
    }
    return OK;
}

static void endwin(void);

static int refresh(void);

static void sdlcurses_resize(void)
{
    int fg, bg;

    /* FIXME: a resize should not disable direct buffering */
    sdlcurses_direct = 0;
    sdlCheckMode(sdlcurses_screen, &sdlcurses_mode);
    sdlSetMode(sdlcurses_screen, &sdlcurses_mode);
    sdlGetMode(sdlcurses_screen, &sdlcurses_mode);
    if (sdlcurses_bitmap || sdlGetCharSize(sdlcurses_screen, &sdlcurses_charwidth, &sdlcurses_charheight))
    {
        sdlcurses_charwidth = sdlcurses_bitmap_double ? 2 : 1;
        sdlcurses_charheight = sdlcurses_bitmap_double ? 2 : 1;
        if (sdlcurses_bitmap_doubleheight) sdlcurses_charheight *= 2;
    }
    {
        int old_w;
        int old_h;
        int y;

        old_w = sdlcurses_w;
        old_h = sdlcurses_h;
        sdlcurses_h = SDLCURSES_LINES;
        sdlcurses_w = SDLCURSES_COLS;
        memmove((void *) sdlcurses_backing, (void *) (sdlcurses_backing + old_w * old_h), old_w * old_h * sizeof(sdlcurses_cell_t));
        if ((old_w != sdlcurses_w)
            ||
            (old_h != sdlcurses_h))
        {
            if (sdlcurses_w < old_w)
            {
                for (y = 0; (y < sdlcurses_h) && (y < old_h); y ++)
                {
                    memmove((void *) (sdlcurses_backing + y * sdlcurses_w), (void *) (sdlcurses_backing + y * old_w), sdlcurses_w * sizeof(sdlcurses_cell_t));
                }
            }
            if (sdlcurses_w * sdlcurses_h != old_w * old_h)
            {
                if (! (sdlcurses_backing = (sdlcurses_cell_t *) realloc((void *) sdlcurses_backing, 2 * sdlcurses_w * sdlcurses_h * sizeof(sdlcurses_cell_t))))
                {
                    perror("realloc");
                    fflush(stderr);
                    endwin();
                    exit(1);
                }
            }
            if (sdlcurses_w > old_w)
            {
                for (y = ((sdlcurses_h > old_h) ? old_h : sdlcurses_h); y --;)
                {
                    memmove((void *) (sdlcurses_backing + y * sdlcurses_w), (void *) (sdlcurses_backing + y * old_w), old_w * sizeof(sdlcurses_cell_t));
                }
            }
            if ((sdlcurses_w * sdlcurses_h) > (old_w * old_h))
            {
                memset((void *) (sdlcurses_backing + old_w * old_h), 0, (sdlcurses_w * sdlcurses_h - old_w * old_h) * sizeof(sdlcurses_cell_t));
            }
        }
        memset((void *) (sdlcurses_backing + sdlcurses_w * sdlcurses_h), 0, sdlcurses_w * sdlcurses_h * sizeof(sdlcurses_cell_t));
    }
    sdlSetColorfulPalette(sdlcurses_screen);
    for (fg = COLOR_BLACK; fg <= COLOR_WHITE; fg ++)
    {
        sdlcurses_pixelmap[fg] = SDL_MapRGB(sdlcurses_format, sdlcurses_colormap[fg].r, sdlcurses_colormap[fg].g, sdlcurses_colormap[fg].b);
        sdlcurses_pixelmap[fg | PEN_BRIGHT] = SDL_MapRGB(sdlcurses_format, sdlcurses_colormap[fg | PEN_BRIGHT].r, sdlcurses_colormap[fg | PEN_BRIGHT].g, sdlcurses_colormap[fg | PEN_BRIGHT].b);
    }
    for (bg = COLOR_BLACK; bg <= (COLOR_WHITE | PEN_BRIGHT); bg ++)
    {
        for (fg = COLOR_BLACK; fg <= bg; fg ++)
        {
            sdlcurses_halfpixelmap[fg][bg] = SDL_MapRGB(sdlcurses_format, sdlcurses_halfcolormap[fg][bg].r, sdlcurses_halfcolormap[fg][bg].g, sdlcurses_halfcolormap[fg][bg].b);
            sdlcurses_halfpixelmap[bg][fg] = sdlcurses_halfpixelmap[fg][bg];
        }
    }
    if (sdlcurses_title) sdlWmhSetTitle(sdlcurses_screen, sdlcurses_title);
    sdlWmhAllowResize(sdlcurses_screen,
                      /* minimum dimensions */
                      sdlcurses_charwidth, sdlcurses_charheight,
                      /* maximum dimensions */
                      32767 / sdlcurses_charwidth * sdlcurses_charwidth, 32767 / sdlcurses_charheight * sdlcurses_charheight,
                      /* step */
                      sdlcurses_charwidth, sdlcurses_charheight);
}

static double sdlcurses_now(void)
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

    if (! sdlcurses_ready) return OK;
    minx = sdlcurses_w;
    miny = sdlcurses_h;
    maxx = 0;
    maxy = 0;
    if (sdlcurses_clearok)
    {
        sdlcurses_realattr = 0;
        sdlSetGCForeground(sdlcurses_screen, sdlcurses_pixelmap[0]);
        sdlDrawBox(sdlcurses_screen, 0, 0, sdlcurses_mode.visible.x, sdlcurses_mode.visible.y);
        sdlFlush(sdlcurses_screen);
    }
    for (y = 0; y < sdlcurses_h; y ++)
    {
        if ((sdlcurses_direct && (sdlcurses_frames > 1)) || sdlcurses_clearok || sdlcurses_backing[y * sdlcurses_w].attr)
        {
            for (x = 0; x < sdlcurses_w; x ++)
            {
                unsigned long ch;
                unsigned long attr;
                int pixdiff;
                
                attr = sdlcurses_backing[y * sdlcurses_w + x].attr;
                ch = sdlcurses_backing[y * sdlcurses_w + x].ch;
                pixdiff = sdlcurses_clearok;
                if (sdlcurses_clearok
                    ||
                    ((attr || (sdlcurses_direct && (sdlcurses_frames > 1)))
                     &&
                     (pixdiff = ((attr
                                  !=
                                  sdlcurses_backing[y * sdlcurses_w + x + sdlcurses_w * sdlcurses_h].attr)
                                 ||
                                 (ch
                                  !=
                                  sdlcurses_backing[y * sdlcurses_w + x + sdlcurses_w * sdlcurses_h].ch))))
                    ||
                    (sdlcurses_direct && (sdlcurses_frames > 1)))
                {
                    if (! ch)
                    {
                        attr = sdlcurses_backing[y * sdlcurses_w + x + sdlcurses_w * sdlcurses_h].attr;
                        ch = sdlcurses_backing[y * sdlcurses_w + x + sdlcurses_w * sdlcurses_h].ch;
                    }
                    sdlcurses_backing[y * sdlcurses_w + x + sdlcurses_w * sdlcurses_h].attr = attr;
                    sdlcurses_backing[y * sdlcurses_w + x + sdlcurses_w * sdlcurses_h].ch = ch;
                    sdlcurses_backing[y * sdlcurses_w + x].attr = 0;
                    sdlcurses_backing[y * sdlcurses_w + x].ch = 0;
                    if (pixdiff || ! (sdlcurses_direct && (sdlcurses_frames > 1)))
                    {
                        if (x < minx) minx = x;
                        if (y < miny) miny = y;
                        if (x >= maxx) maxx = x + 1;
                        if (y >= maxy) maxy = y + 1;
                    }
                    sdlcurses_addch(ch ? ch : ' ', attr, x, y);
                }
            }
            sdlcurses_backing[y * sdlcurses_w].attr = 0;
        }
    }
    sdlcurses_clearok = 0;
    if (sdlcurses_direct)
    {
        sdlResourceRelease(sdlcurses_buffer[sdlcurses_frame]->resource);
        if ((sdlcurses_mode.frames > 1)
            &&
            (sdlGetDisplayFrame(sdlcurses_screen) != sdlcurses_buffer[sdlcurses_frame]->frame)
            &&
            sdlSetDisplayFrame(sdlcurses_screen, sdlcurses_buffer[sdlcurses_frame]->frame))
        {
            endwin();
            fprintf(stderr, "sdlcurses: sdlSetDisplayFrame failed\n");
            fflush(stderr);
            exit(1);
        }
    }
    if ((maxy > 0) && (maxx > 0))
    {
        if (sdlcurses_flushall)
        {
            sdlFlush(sdlcurses_screen);
        }
        else
        {
            long x, y, w, h;

            x = minx * sdlcurses_charwidth;
            y = miny * sdlcurses_charheight;
            w = (maxx - minx) * sdlcurses_charwidth;
            h = (maxy - miny) * sdlcurses_charheight;
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
            if ((x + w) > sdlcurses_mode.visible.x)
            {
                w = sdlcurses_mode.visible.x - x;
            }
            if ((y + h) > sdlcurses_mode.visible.y)
            {
                h = sdlcurses_mode.visible.y - y;
            }
            if ((w != sdlcurses_mode.visible.x)
                ||
                (h != sdlcurses_mode.visible.y))
            {
                sdlFlushRegion(sdlcurses_screen,
                               x, y, w, h);
            }
            else
            {
                sdlFlush(sdlcurses_screen);
            }
        }
    }
    if (sdlcurses_direct)
    {
        sdlcurses_frame = (sdlcurses_frame + 1) % sdlcurses_frames;
        if (sdlResourceAcquire(sdlcurses_buffer[sdlcurses_frame]->resource,
                               SDL_ACTYPE_READ | SDL_ACTYPE_WRITE))
        {
            endwin();
            fprintf(stderr, "sdlcurses: failed to acquire directbuffer\n");
            fflush(stderr);
            exit(1);
        }
        sdlcurses_format = sdlcurses_buffer[sdlcurses_frame]->buffer.plb.pixelformat;
        sdlcurses_bpp = sdlcurses_format->size;
        if (! sdlcurses_bpp)
        {
            sdlcurses_bpp = GT_SIZE(sdlcurses_mode.graphtype);
        }
        if ((sdlcurses_mode.frames > 1)
            &&
            (sdlGetWriteFrame(sdlcurses_screen) != sdlcurses_buffer[sdlcurses_frame]->frame)
            &&
            sdlSetWriteFrame(sdlcurses_screen, sdlcurses_buffer[sdlcurses_frame]->frame))
        {
            endwin();
            fprintf(stderr, "sdlcurses: sdlSetWriteFrame failed\n");
            fflush(stderr);
            exit(1);
        }
        if ((sdlcurses_mode.frames > 1)
            &&
            (sdlGetReadFrame(sdlcurses_screen) != sdlcurses_buffer[sdlcurses_frame]->frame)
            &&
            sdlSetReadFrame(sdlcurses_screen, sdlcurses_buffer[sdlcurses_frame]->frame))
        {
            endwin();
            fprintf(stderr, "sdlcurses: sdlSetReadFrame failed\n");
            fflush(stderr);
            exit(1);
        }
    }
    {
        int newx;
        int newy;

        newx = sdlcurses_mode.visible.x;
        newy = sdlcurses_mode.visible.y;
        if (! sdlWmhGetSize(sdlcurses_screen, &newx, &newy))
        {
            sdlcurses_unmapped = 0;
            if ((newx > 0)
                &&
                (newy > 0)
                &&
                ((sdlcurses_mode.visible.x != newx)
                 ||
                 (sdlcurses_mode.visible.y != newy)))
            {
                sdlcurses_mode.visible.x = newx;
                sdlcurses_mode.visible.y = newy;
                sdlcurses_need_resize = sdlcurses_now();
            }
            else if ((newx <= 0)
                     ||
                     (newy <= 0))
            {
                sdlcurses_unmapped = 1;
            }
        }
        if ((sdlcurses_need_resize != 0.0)
            &&
            ((sdlcurses_now() - sdlcurses_need_resize) >= SDLCURSES_RESIZE_INHIBIT))
        {
            sdlcurses_resize();
            sdlcurses_need_resize = 0.0;
        }
    }
    return OK;
}

static chtype getch(void)
{
    struct timeval tv = { 0, 0 };
    int gotevent;

    while ((gotevent = sdlEventPoll(sdlcurses_screen, emKeyPress | emExpose | emCommand | (sdlcurses_unmapped ? ~0UL : 0), &tv))
           ||
           sdlcurses_unmapped)
    {
        sdl_event ev;

        ev.any.type = evNothing;
        if (gotevent)
        {
            sdlEventRead(sdlcurses_screen, &ev, emKeyPress | emExpose | emCommand | (sdlcurses_unmapped ? ~0UL : 0));
        }
        if (ev.any.type == evKeyPress)
        {
            sdlcurses_unmapped = 0;
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
                y = py / sdlcurses_charheight;
                if (y >= sdlcurses_h) continue;
                sdlcurses_backing[y * sdlcurses_w].attr = sdlcurses_backing[y * sdlcurses_w + sdlcurses_w * sdlcurses_h].attr;
                sdlcurses_backing[y * sdlcurses_w].ch = sdlcurses_backing[y * sdlcurses_w + sdlcurses_w * sdlcurses_h].ch;
                if (! sdlcurses_backing[y * sdlcurses_w].attr)
                {
                    sdlcurses_backing[y * sdlcurses_w].attr = A_NORMAL;
                    sdlcurses_backing[y * sdlcurses_w].ch = 0;
                }
                for (px = ev.expose.x; px < ev.expose.x + ev.expose.w; px ++)
                {
                    x = px / sdlcurses_charwidth;
                    if (x >= sdlcurses_w) continue;
                    sdlcurses_backing[y * sdlcurses_w + x].attr = sdlcurses_backing[y * sdlcurses_w + x + sdlcurses_w * sdlcurses_h].attr;
                    sdlcurses_backing[y * sdlcurses_w + x].ch = sdlcurses_backing[y * sdlcurses_w + x + sdlcurses_w * sdlcurses_h].ch;
                    if (! sdlcurses_backing[y * sdlcurses_w + x].attr)
                    {
                        sdlcurses_backing[y * sdlcurses_w + x].attr = A_NORMAL;
                        sdlcurses_backing[y * sdlcurses_w + x].ch = 0;
                    }
                }
            }
            refresh();
        }
        else if (ev.any.type == evCommand)
        {
            if ((ev.cmd.code == SDLCMD_REQUEST_SWITCH)
                &&
                (((sdl_cmddata_switchrequest *) &(ev.cmd.data))->request & SDL_REQSW_MODE))
            {
                int newx;
                int newy;

                if (memcmp((void *) &sdlcurses_mode,
                           (void *) &(((sdl_cmddata_switchrequest *) &(ev.cmd.data))->mode),
                           sizeof(sdl_mode))
                    &&
                    ((sdl_cmddata_switchrequest *) &(ev.cmd.data))->mode.visible.x
                    &&
                    ((sdl_cmddata_switchrequest *) &(ev.cmd.data))->mode.visible.y)
                {
                    memcpy((void *) &sdlcurses_mode,
                           (void *) &(((sdl_cmddata_switchrequest *) &(ev.cmd.data))->mode),
                           sizeof(sdl_mode));
                    sdlcurses_need_resize = sdlcurses_now();
                }
                newx = sdlcurses_mode.visible.x;
                newy = sdlcurses_mode.visible.y;
                if (! sdlWmhGetSize(sdlcurses_screen, &newx, &newy))
                {
                    sdlcurses_unmapped = 0;
                    if (((newx != sdlcurses_mode.visible.x)
                         ||
                         (newy != sdlcurses_mode.visible.y))
                        &&
                        (newx > 0)
                        &&
                        (newy > 0))
                    {
                        sdlcurses_mode.visible.x = newx;
                        sdlcurses_mode.visible.y = newy;
                        sdlcurses_need_resize = sdlcurses_now();
                    }
                    else if ((newx <= 0)
                             ||
                             (newy <= 0))
                    {
                        sdlcurses_unmapped = 1;
                    }
                }
            }
        }
        if (sdlcurses_unmapped)
        {
            refresh();
        }
        if (sdlcurses_unmapped)
        {
            tv.tv_sec = 1;
        }
        else
        {
            tv.tv_sec = 0;
        }
    }
    sdlcurses_unmapped = 0;
    return ERR;
}

static void standout() {
    sdlcurses_attr = A_STANDOUT;
}

static void standend() {
    sdlcurses_attr = 0;
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

static unsigned int sdlcurses_mix(unsigned int a, unsigned int b)
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

    sdlcurses_colormap[i].r = 0xffff * (long) (sdlcurses_swaprb ? b : r) / 1000;
    sdlcurses_colormap[i].g = 0xffff * (long) g / 1000;
    sdlcurses_colormap[i].b = 0xffff * (long) (sdlcurses_swaprb ? r : b) / 1000;
    sdlcurses_colormap[i].a = 0x0000;
    sdlcurses_pixelmap[i] = SDL_MapRGB(sdlcurses_format, sdlcurses_colormap[i].r,, sdlcurses_colormap[i].g, sdlcurses_colormap[i].b);
    for (bg = COLOR_BLACK; bg <= (COLOR_WHITE | PEN_BRIGHT); bg ++)
    {
        sdlcurses_halfcolormap[i][bg].r = sdlcurses_mix(sdlcurses_colormap[i].r, sdlcurses_colormap[bg].r);
        sdlcurses_halfcolormap[i][bg].g = sdlcurses_mix(sdlcurses_colormap[i].g, sdlcurses_colormap[bg].g);
        sdlcurses_halfcolormap[i][bg].b = sdlcurses_mix(sdlcurses_colormap[i].b, sdlcurses_colormap[bg].b);
        sdlcurses_halfcolormap[i][bg].a = sdlcurses_mix(sdlcurses_colormap[i].a, sdlcurses_colormap[bg].a);
        sdlcurses_halfpixelmap[i][bg] = SDL_MapRGB(sdlcurses_format, sdlcurses_halfcolormap[i][bg].r, sdlcurses_halfcolormap[i][bg].g, sdlcurses_halfcolormap[i][bg].b);
        sdlcurses_halfcolormap[bg][i] = sdlcurses_halfcolormap[i][bg];
        sdlcurses_halfpixelmap[bg][i] = sdlcurses_halfpixelmap[i][bg];
    }
    return OK;
}

static int color_content(int i, short *r, short *g, short *b) {
    *(sdlcurses_swaprb ? b : r) = (long) sdlcurses_colormap[i].r * 1000 / 0xffff;
    *g = (long) sdlcurses_colormap[i].g * 1000 / 0xffff;
    *(sdlcurses_swaprb ? r : b) = (long) sdlcurses_colormap[i].b * 1000 / 0xffff;
    return OK;
}

static int init_pair(int i, short fg, short bg) {
    sdlcurses_pairs[i] = (bg << 4) | fg;
    return OK;
}

static int pair_content(int i, short *fg, short *bg) {
    *fg = sdlcurses_pairs[i] & 0xf;
    *bg = sdlcurses_pairs[i] >> 4;
    return OK;
}

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    int fg, bg;
    int auto_x, auto_y;

    sdlcurses_title = title;
    sdlcurses_bitmap = myman_getenv("SDLCURSES_BITMAP");
    if (sdlcurses_bitmap && ! *sdlcurses_bitmap) sdlcurses_bitmap = NULL;
    sdlcurses_swaprb = myman_getenv("SDLCURSES_SWAPRB");
    if (sdlcurses_swaprb && ! *sdlcurses_swaprb) sdlcurses_swaprb = NULL;
    sdlcurses_ascii = myman_getenv("SDLCURSES_ASCII");
    if (sdlcurses_ascii && ! *sdlcurses_ascii) sdlcurses_ascii = NULL;
    sdlcurses_flushall = myman_getenv("SDLCURSES_FLUSHALL");
    if (sdlcurses_flushall && ! *sdlcurses_flushall) sdlcurses_flushall = NULL;
    sdlcurses_fixedpal = myman_getenv("SDLCURSES_FIXEDPAL");
    if (sdlcurses_fixedpal && ! *sdlcurses_fixedpal) sdlcurses_fixedpal = NULL;
    sdlcurses_bitmap_double = myman_getenv("SDLCURSES_BITMAP_DOUBLE");
    if (sdlcurses_bitmap_double && ! *sdlcurses_bitmap_double) sdlcurses_bitmap_double = NULL;
    sdlcurses_bitmap_doubleheight = myman_getenv("SDLCURSES_BITMAP_DOUBLEHEIGHT");
    if (sdlcurses_bitmap_doubleheight && ! *sdlcurses_bitmap_doubleheight) sdlcurses_bitmap_doubleheight = NULL;
    if (! sdlcurses_bitmap)
    {
        if (w > 128) w = 128;
        if (h > 80) h = 80;
    }
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        fprintf(stderr, "SDL_Init failed\n");
        fflush(stderr);
        exit(1);
    }
    if (! (sdlcurses_screen = SDL_SetVideoMode(w * sdlcurses_charwidth, h * sdlcurses_charheight, 0, SDL_SWSURFACE)))
    {
        if (! (sdlcurses_screen = SDL_SetVideoMode(0, 0, 0, SDL_SWSURFACE)))
        {
            SDL_Quit();
            fprintf(stderr, "SDL_SetVideoMode failed\n");
            fflush(stderr);
            exit(1);
        }
    }
    sdlParseMode("", &sdlcurses_mode);
    auto_x = sdlcurses_mode.visible.x == SDL_AUTO;
    auto_y = sdlcurses_mode.visible.y == SDL_AUTO;
    sdlcurses_mode.frames = 2;
    sdlAddFlags(sdlcurses_screen, SDLFLAG_ASYNC);
    sdlCheckMode(sdlcurses_screen, &sdlcurses_mode);
    if (sdlSetMode(sdlcurses_screen, &sdlcurses_mode) < 0)
    {
        sdlParseMode("", &sdlcurses_mode);
        sdlCheckMode(sdlcurses_screen, &sdlcurses_mode);
        if (sdlSetMode(sdlcurses_screen, &sdlcurses_mode) < 0)
        {
            fprintf(stderr, "sdlSetMode failed\n");
            fflush(stderr);
            SDL_Quit();
            exit(1);
        }
    }
    sdlGetMode(sdlcurses_screen, &sdlcurses_mode);
    if (sdlcurses_bitmap || sdlGetCharSize(sdlcurses_screen, &sdlcurses_charwidth, &sdlcurses_charheight))
    {
        sdlcurses_charwidth = sdlcurses_bitmap_double ? 2 : 1;
        sdlcurses_charheight = sdlcurses_bitmap_double ? 2 : 1;
        if (sdlcurses_bitmap_doubleheight) sdlcurses_charheight *= 2;
    }
    if (auto_x
        &&
        ((sdlcurses_mode.visible.x == SDL_AUTO)
         ||
         (sdlcurses_mode.visible.x > (w * sdlcurses_charwidth))))
    {
        sdlcurses_mode.visible.x = w * sdlcurses_charwidth;
    }
    if (auto_y
        &&
        ((sdlcurses_mode.visible.y == SDL_AUTO)
         ||
         (sdlcurses_mode.visible.y > (h * sdlcurses_charheight))))
    {
        sdlcurses_mode.visible.y = h * sdlcurses_charheight;
    }
    if (auto_x || auto_y)
    {
        sdlCheckMode(sdlcurses_screen, &sdlcurses_mode);
        sdlSetMode(sdlcurses_screen, &sdlcurses_mode);
    }
    sdlGetMode(sdlcurses_screen, &sdlcurses_mode);
    sdlSetColorfulPalette(sdlcurses_screen);
    for (fg = COLOR_BLACK; fg <= COLOR_WHITE; fg ++)
    {
        if (fg)
        {
            sdlcurses_colormap[fg].r = (fg & (sdlcurses_swaprb ? COLOR_BLUE : COLOR_RED)) ? 0x8888 : 0x0000;
            sdlcurses_colormap[fg].g = (fg & COLOR_GREEN) ? 0x8888 : 0x0000;
            sdlcurses_colormap[fg].b = (fg & (sdlcurses_swaprb ? COLOR_RED : COLOR_BLUE)) ? 0x8888 : 0x0000;
            sdlcurses_colormap[fg].a = 0x0000;
            sdlcurses_colormap[fg | PEN_BRIGHT].r = sdlcurses_colormap[fg].r ? 0xffff : 0x8888;
            sdlcurses_colormap[fg | PEN_BRIGHT].g = sdlcurses_colormap[fg].g ? 0xffff : 0x8888;
            sdlcurses_colormap[fg | PEN_BRIGHT].b = sdlcurses_colormap[fg].b ? 0xffff : 0x8888;
            sdlcurses_colormap[fg | PEN_BRIGHT].a = sdlcurses_colormap[fg].a;
        }
        else
        {
            sdlcurses_colormap[fg].r = 0x0000;
            sdlcurses_colormap[fg].g = 0x0000;
            sdlcurses_colormap[fg].b = 0x0000;
            sdlcurses_colormap[fg].a = 0x0000;
            sdlcurses_colormap[fg | PEN_BRIGHT].r = 0x6666;
            sdlcurses_colormap[fg | PEN_BRIGHT].g = 0x6666;
            sdlcurses_colormap[fg | PEN_BRIGHT].b = 0x6666;
            sdlcurses_colormap[fg | PEN_BRIGHT].a = 0x0000;
        }
        sdlcurses_pixelmap[fg] = SDL_MapRGB(sdlcurses_format, sdlcurses_colormap[fg].r, sdlcurses_colormap[fg].g, sdlcurses_colormap[fg].b);
        sdlcurses_pixelmap[fg | PEN_BRIGHT] = SDL_MapRGB(sdlcurses_format, sdlcurses_colormap[fg | PEN_BRIGHT].r, sdlcurses_colormap[fg | PEN_BRIGHT].g, sdlcurses_colormap[fg | PEN_BRIGHT].b);
    }
    for (bg = COLOR_BLACK; bg <= (COLOR_WHITE | PEN_BRIGHT); bg ++)
    {
        for (fg = COLOR_BLACK; fg <= bg; fg ++)
        {
            sdlcurses_halfcolormap[fg][bg].r = sdlcurses_mix(sdlcurses_colormap[fg].r, sdlcurses_colormap[bg].r);
            sdlcurses_halfcolormap[fg][bg].g = sdlcurses_mix(sdlcurses_colormap[fg].g, sdlcurses_colormap[bg].g);
            sdlcurses_halfcolormap[fg][bg].b = sdlcurses_mix(sdlcurses_colormap[fg].b, sdlcurses_colormap[bg].b);
            sdlcurses_halfcolormap[fg][bg].a = sdlcurses_mix(sdlcurses_colormap[fg].a, sdlcurses_colormap[bg].a);
            sdlcurses_halfpixelmap[fg][bg] = SDL_MapRGB(sdlcurses_format, sdlcurses_halfcolormap[fg][bg].r, sdlcurses_halfcolormap[fg][bg].g, sdlcurses_halfcolormap[fg][bg].b);
            sdlcurses_halfcolormap[bg][fg] = sdlcurses_halfcolormap[fg][bg];
            sdlcurses_halfpixelmap[bg][fg] = sdlcurses_halfpixelmap[fg][bg];
        }
    }
    sdlcurses_h = SDLCURSES_LINES;
    sdlcurses_w = SDLCURSES_COLS;
    if (! (sdlcurses_backing = (sdlcurses_cell_t *) malloc(2 * sdlcurses_w * sdlcurses_h * sizeof(sdlcurses_cell_t))))
    {
        perror("malloc");
        fflush(stderr);
        SDL_Quit();
        exit(1);
    }
    memset((void *) sdlcurses_backing, 0, 2 * sdlcurses_w * sdlcurses_h * sizeof(sdlcurses_cell_t));
    sdlcurses_need_resize = 0.0;
    sdlcurses_unmapped = 0;
    clearok(stdscr, TRUE);
    sdlcurses_attr = A_NORMAL;
    sdlcurses_realattr = 0;
    sdlcurses_frame = 0;
    sdlcurses_frames = sdlDBGetNumBuffers(sdlcurses_screen);
    if (sdlcurses_frames > 2)
    {
        sdlcurses_frames = 2;
    }
    sdlcurses_direct = 0;
    if (sdlcurses_bitmap)
    {
        int i;

        for (i = 0; i < sdlcurses_frames; i ++)
        {
            sdlcurses_buffer[sdlcurses_frame] = sdlDBGetBuffer(sdlcurses_screen, i);
            if (sdlcurses_buffer[sdlcurses_frame]->type & SDL_DB_SIMPLE_PLB)
            {
                sdlcurses_frame ++;
            }
        }
        sdlcurses_frames = sdlcurses_frame;
        sdlcurses_frame = 0;
        if (! sdlcurses_frames)
        {
            /* no directbuffers */
        }
        else if (sdlResourceAcquire(sdlcurses_buffer[sdlcurses_frame]->resource,
                               SDL_ACTYPE_READ | SDL_ACTYPE_WRITE))
        {
            /* could not actually acquire a directbuffer */
        }
        else
        {
            if (sdlcurses_buffer[sdlcurses_frame]->write
                &&
                sdlcurses_buffer[sdlcurses_frame]->read)
            {
                sdlcurses_format = sdlcurses_buffer[sdlcurses_frame]->buffer.plb.pixelformat;
                sdlcurses_bpp = sdlcurses_format->size;
                if (! sdlcurses_bpp)
                {
                    sdlcurses_bpp = GT_SIZE(sdlcurses_mode.graphtype);
                }
                if ((CHAR_BIT == 8)
                    &&
                    ((sdlcurses_bpp == 8)
                     ||
                     (sdlcurses_bpp == 16)
                     ||
                     (sdlcurses_bpp == 24)
                     ||
                     (sdlcurses_bpp == 32)))
                {
                    sdlcurses_reverse_endian = 0x12;
                    sdlcurses_reverse_endian = (*((unsigned char *)(void *)(& sdlcurses_reverse_endian)) == sdlcurses_reverse_endian);
                    if (myman_getenv("SDLCURSES_DIRECT")
                        &&
                        *(myman_getenv("SDLCURSES_DIRECT")))
                    {
                        sdlcurses_direct = 1;
                    }
                }
            }
            if (! sdlcurses_direct)
            {
                sdlResourceRelease(sdlcurses_buffer[sdlcurses_frame]->resource);
            }
        }
    }
    if ((sdlcurses_mode.frames > 1)
        &&
        (sdlGetWriteFrame(sdlcurses_screen) !=
         (sdlcurses_direct
          ?
          sdlcurses_buffer[sdlcurses_frame]->frame
          :
          sdlcurses_frame))
        &&
        sdlSetWriteFrame(sdlcurses_screen,
                         (sdlcurses_direct
                          ?
                          sdlcurses_buffer[sdlcurses_frame]->frame
                          :
                          sdlcurses_frame)))
    {
        fprintf(stderr,
                "sdlcurses: sdlSetWriteFrame failed for frame %d\n",
                (sdlcurses_direct
                 ?
                 sdlcurses_buffer[sdlcurses_frame]->frame
                 :
                 sdlcurses_frame));
        fflush(stderr);
        endwin();
        exit(1);
    }
    if ((sdlcurses_mode.frames > 1)
        &&
        (sdlGetReadFrame(sdlcurses_screen) !=
         (sdlcurses_direct
          ?
          sdlcurses_buffer[sdlcurses_frame]->frame
          :
          sdlcurses_frame))
        &&
        sdlSetReadFrame(sdlcurses_screen,
                         (sdlcurses_direct
                          ?
                          sdlcurses_buffer[sdlcurses_frame]->frame
                          :
                          sdlcurses_frame)))
    {
        fprintf(stderr,
                "sdlcurses: sdlSetReadFrame failed for frame %d\n",
                (sdlcurses_direct
                 ?
                 sdlcurses_buffer[sdlcurses_frame]->frame
                 :
                 sdlcurses_frame));
        fflush(stderr);
        endwin();
        exit(1);
    }
    if ((! sdlcurses_direct)
        &&
        (sdlcurses_mode.frames > 1)
        &&
        (sdlGetDisplayFrame(sdlcurses_screen) != sdlcurses_frame)
        &&
        sdlSetDisplayFrame(sdlcurses_screen, sdlcurses_frame))
    {
        fprintf(stderr,
                "sdlcurses: sdlSetDisplayFrame failed for frame %d\n",
                (sdlcurses_direct
                 ?
                 sdlcurses_buffer[sdlcurses_frame]->frame
                 :
                 sdlcurses_frame));
        fflush(stderr);
        endwin();
        exit(1);
    }
    if (sdlcurses_title) sdlWmhSetTitle(sdlcurses_screen, sdlcurses_title);
    sdlWmhAllowResize(sdlcurses_screen,
                      /* minimum dimensions */
                      sdlcurses_charwidth, sdlcurses_charheight,
                      /* maximum dimensions */
                      32767 / sdlcurses_charwidth * sdlcurses_charwidth, 32767 / sdlcurses_charheight * sdlcurses_charheight,
                      /* step */
                      sdlcurses_charwidth, sdlcurses_charheight);
    sdlcurses_ready = 1;
}

static void endwin(void) {
    free(sdlcurses_backing);
    SDL_Quit();
    sdlcurses_ready = 0;
}

static int sdlcurses_wcwidth(const chtype ch) {
    return (ch < 0x20) ? 0 : 1;
}

#undef wcwidth
#define wcwidth sdlcurses_wcwidth

#endif /* MYMAN_SDLCURS_H_INCLUDED */
