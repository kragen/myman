/*
 * aacurses.h - aalib driver for the MyMan video game
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

#include <aalib.h>

/* work-arounds for aalib */

#ifndef USE_ATTR
#define USE_ATTR 1
#endif

#ifndef USE_UNDERLINE
#define USE_UNDERLINE 1
#endif

#ifndef SWAPDOTS
#define SWAPDOTS (aacurses_swapdots)
#endif

#ifndef USE_ACS
#define USE_ACS (aacurses_use_acs)
#endif

#ifndef USE_SIGWINCH
#define USE_SIGWINCH 0
#endif

#ifndef USE_IOCTL
#define USE_IOCTL 0
#endif

static int aacurses_ready = 0;

static char *aacurses_bitmap;

typedef unsigned long int chtype;

static aa_context *ctx;

static struct aa_hardware_params aacurses_hwparams;

static struct aa_renderparams aacurses_renderparams;

static unsigned long aacurses_x, aacurses_y;

static enum aa_attribute aacurses_attr;

#ifndef OK
#define OK (0)
#endif

#ifndef ERR
#define ERR (-1)
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

#define getyx(stdscr, y, x) ((y) = aacurses_y), ((x) = aacurses_x)

#define wrefresh(stdscr) refresh()

#define beep() addch('\7')

#define use_env(x)

#define echo()

#define curs_set(x)

#define keypad(stdscr, x)

#define LINES (aacurses_bitmap ? aa_imgheight(ctx) : aa_scrheight(ctx))

#define COLS (aacurses_bitmap ? aa_imgwidth(ctx) : aa_scrwidth(ctx))

#define KEY_UP AA_UP

#define KEY_DOWN AA_DOWN

#define KEY_LEFT AA_LEFT

#define KEY_RIGHT AA_RIGHT

#define A_DIM 0x10000000

#define A_UNDERLINE A_DIM

#define A_BOLD 0x20000000

#define A_STANDOUT 0x40000000

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

static int aacurses_use_acs = 1;

static int aacurses_swapdots = 1;

#define INITSCR_WITH_HINTS

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    aacurses_bitmap = myman_getenv("AACURSES_BITMAP");
    if (aacurses_bitmap && ! *aacurses_bitmap) aacurses_bitmap = NULL;
    if (aacurses_bitmap)
    {
        h = (h + 1) / 2;
        w = (w + 1) / 2;
    }
    if (w > 128) w = 128;
    if (h > 50) h = 50;
    memcpy((void *) &aacurses_renderparams, (void *) &aa_defrenderparams, sizeof(aa_renderparams));
    aa_parseoptions(& aacurses_hwparams, & aacurses_renderparams, NULL, NULL);
    if (aacurses_hwparams.maxwidth && (w > aacurses_hwparams.maxwidth)) w = aacurses_hwparams.maxwidth;
    if (w < aacurses_hwparams.minwidth) w = aacurses_hwparams.maxwidth;
    if (aacurses_hwparams.recwidth) w = aacurses_hwparams.recwidth;
    if (aacurses_hwparams.minheight && (h > aacurses_hwparams.maxheight)) h = aacurses_hwparams.maxheight;
    if (h < aacurses_hwparams.minheight) h = aacurses_hwparams.minheight;
    if (aacurses_hwparams.recheight) h = aacurses_hwparams.recheight;
    if (myman_getenv("DISPLAY") && *myman_getenv("DISPLAY") && (! myman_getenv("AAOPTS")))
    {
        if (! aacurses_bitmap)
        {
            aacurses_use_acs = 1;
            aacurses_swapdots = 1;
        }
        aacurses_hwparams.width = w;
        aacurses_hwparams.height = h;
    }
    else
    {
        if (! aacurses_bitmap)
        {
            aacurses_use_acs = 0;
            aacurses_swapdots = 0;
        }
    }
    aacurses_hwparams.supported = AA_NORMAL_MASK | AA_DIM_MASK | AA_BOLD_MASK | AA_BOLDFONT_MASK | AA_REVERSE_MASK | AA_EXTENDED | AA_EIGHT;
    ctx = aa_autoinit(& aacurses_hwparams);
    if (! ctx)
    {
        fprintf(stderr, "aa_autoinit failed\n");
        fflush(stderr);
        exit(1);
    }
    aa_autoinitkbd(ctx, 0);
    aa_hidecursor(ctx);
    aacurses_x = 0;
    aacurses_y = 0;
    aacurses_attr = AA_NORMAL;
    aacurses_ready = 1;
}

static void endwin(void) {
    aacurses_ready = 0;
    aa_showcursor(ctx);
    aa_flush(ctx);
    aa_uninitkbd(ctx);
    aa_close(ctx);
}

static int erase(void) {
    aacurses_attr = AA_NORMAL;
    for (aacurses_y = 0; aacurses_y < LINES; aacurses_y ++)
        for (aacurses_x = 0; aacurses_x < COLS; aacurses_x ++)
        {
            if (aacurses_bitmap)
            {
                aa_putpixel(ctx, aacurses_x, aacurses_y, 0);
            }
            else
            {
                aa_puts(ctx, aacurses_x, aacurses_y, aacurses_attr, " ");
            }
        }
    aacurses_x = 0;
    aacurses_y = 0;
    return OK;
}

static int refresh(void) {
    if (! aacurses_ready) return OK;
    if (aacurses_bitmap)
    {
        aa_render(ctx, &aacurses_renderparams, 0, 0, aa_scrwidth(ctx), aa_scrheight(ctx));
    }
    aa_flush(ctx);
    return OK;
}

static chtype getch(void) {
    int ch;

    ch = aa_getevent(ctx, 0);
    if (ch)
    {
        if (ch == AA_RESIZE)
        {
            aa_resize(ctx);
        }
        else if (ch == AA_BACKSPACE)
        {
            return 0x08;
        }
        else if (ch == AA_ESC)
        {
            return 0x1b;
        }
        else
        {
            return ch;
        }
    }
    return ERR;
}

static void standout() {
    aacurses_attr = AA_SPECIAL;
}

static void standend() {
    aacurses_attr = AA_NORMAL;
}

static int attrset(int a)
{
    aacurses_attr = (a & A_BOLD) ? ((aacurses_hwparams.supported & AA_BOLD_MASK) ? AA_BOLD : AA_BOLDFONT) : (a & A_DIM) ? AA_DIM : (a & A_STANDOUT) ? AA_SPECIAL : AA_NORMAL;
    return OK;
}

static void move(int y, int x) {
    aacurses_y = y;
    aacurses_x = x;
}

static int addch(const chtype ch) {
    if (aacurses_bitmap)
    {
        int pixel;

        pixel = 192;
        if ((ch == ' ')
            ||
            (ch == '\0')
            ||
            (ch == '\xff'))
        {
            pixel = 0;
        }
        else if ((ch == '\xdf')
                 ||
                 (ch == '\xdc')
                 ||
                 (ch == '\xdd')
                 ||
                 (ch == '\xde'))
        {
            pixel /= 2;
        }
        if ((aacurses_attr == AA_BOLD) | (aacurses_attr == AA_BOLDFONT))
        {
            pixel = pixel * 255 / 192;
        }
        else if (aacurses_attr == AA_DIM)
        {
            pixel /= 2;
        }
        else if (aacurses_attr == AA_SPECIAL)
        {
            pixel = 255 - pixel;
        }
        aa_putpixel(ctx, aacurses_x, aacurses_y, pixel);
    }
    else
    {
        char s[2];

        s[0] = (char) ch;
        s[1] = '\0';
        aa_puts(ctx, aacurses_x, aacurses_y, aacurses_attr & aacurses_hwparams.supported, s);
    }
    aacurses_x ++;
    return OK;
}

static int clrtobot(void) {
    unsigned int x, y;

    for (x = aacurses_x; x < COLS; x ++)
    {
        move(aacurses_y, x);
        addch(' ');
    }
    for (y = aacurses_y + 1; y < LINES; y ++)
        for (x = 0; x < COLS; x ++)
        {
            move(y, x);
            addch(' ');
        }
    return OK;
}

static int clrtoeol(void) {
    unsigned int x;

    for (x = aacurses_x; x < COLS; x ++)
    {
        move(aacurses_y, x);
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

