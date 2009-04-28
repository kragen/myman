/*
 * optcurs.h - Redraw optimization driver for the MyMan video game
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

#ifndef MYMAN_OPTCURS_H_INCLUDED
#define MYMAN_OPTCURS_H_INCLUDED 1

#ifdef WACS_ULCORNER
#error "optcurs.h" does not yet work with a wide-character curses implementation
#endif

#ifndef wcwidth
#include <wchar.h>
#endif

typedef struct {
    chtype ch;
    attr_t attr;
} optcurses_cell_t;

#ifndef OPTCURSES_GLOBAL
#define OPTCURSES_GLOBAL(dfn, init) static dfn init
#endif

OPTCURSES_GLOBAL(int optcurses_after_endwin, = 1);
OPTCURSES_GLOBAL(int optcurses_w, = 0);
OPTCURSES_GLOBAL(int optcurses_h, = 0);
OPTCURSES_GLOBAL(int optcurses_x, = -1);
OPTCURSES_GLOBAL(int optcurses_y, = -1);
OPTCURSES_GLOBAL(attr_t optcurses_attr, = 0);
OPTCURSES_GLOBAL(int optcurses_clearok_flag, = 0);
OPTCURSES_GLOBAL(const char *optcurses_direct, = NULL);
OPTCURSES_GLOBAL(const char *optcurses_interlace, = NULL);
OPTCURSES_GLOBAL(const char *optcurses_bitmap, = NULL);
OPTCURSES_GLOBAL(const char *optcurses_bitmap_force, = NULL);
OPTCURSES_GLOBAL(const char *optcurses_bitmap_braille, = NULL);

/*
 * NOTE: this is actually an array of optcurses_cell_t of
 * dimensionality [2][optcurses_h][optcurses_w] followed by an array
 * of unsigned char char of dimensionality [optcurses_h]; the first
 * array is an application buffer at [0] followed by a screen buffer
 * at [1], and the second array is a clean-list for the application
 * buffer where a 0xFF entry indicates no change to the
 * corresponding screen line since the last refresh.
 */
OPTCURSES_GLOBAL(optcurses_cell_t *optcurses_backing, = NULL);

static int OPTCURSES_IS_ROW_DIRTY(int y)
{
    return (0xFFU
            !=
            (unsigned) ((unsigned char *)(optcurses_backing + 2 * optcurses_w * optcurses_h))[(y / ((optcurses_bitmap && *optcurses_bitmap) ? 2 : 1))]);
}

static int OPTCURSES_DIRTY_ROW(int y)
{
    return (((unsigned char *)(optcurses_backing + 2 * optcurses_w * optcurses_h))[(y / ((optcurses_bitmap && *optcurses_bitmap) ? 2 : 1))] = 0U);
}

static int OPTCURSES_CLEAN_ROW(int y)
{
    return (((unsigned char *)(optcurses_backing + 2 * optcurses_w * optcurses_h))[(y / ((optcurses_bitmap && *optcurses_bitmap) ? 2 : 1))] = 0xFFU);
}

#define OPTCURSES__LINES (LINES * ((optcurses_bitmap && *optcurses_bitmap) ? 2 : 1))

#ifndef OPTCURSES_UNICODE
#ifdef USE_RAW_UCS
#if USE_RAW_UCS
#define OPTCURSES_UNICODE 1
#endif
#endif
#endif

static void optcurses_init(void) {
    if ((optcurses_x == -1) || (optcurses_y == -1)) getyx(stdscr, optcurses_y, optcurses_x);
    if ((COLS != optcurses_w)
        ||
        (OPTCURSES__LINES != optcurses_h))
    {
        if (! optcurses_direct)
        {
            optcurses_direct = myman_getenv("OPTCURSES_DIRECT");
            if (! optcurses_direct) optcurses_direct = "";
            if (! strcmp(optcurses_direct, "0")) optcurses_direct = "";
        }
        if (! optcurses_interlace)
        {
            optcurses_interlace = myman_getenv("OPTCURSES_INTERLACE");
            if (! optcurses_interlace) optcurses_interlace = "";
            if (! strcmp(optcurses_interlace, "0")) optcurses_interlace = "";
        }
        if (! optcurses_bitmap)
        {
            optcurses_bitmap = myman_getenv("OPTCURSES_BITMAP");
            if (! optcurses_bitmap) optcurses_bitmap = "";
            if (! strcmp(optcurses_bitmap, "0")) optcurses_bitmap = "";
        }
        if (! optcurses_bitmap_force)
        {
            optcurses_bitmap_force = myman_getenv("OPTCURSES_BITMAP_FORCE");
            if (! optcurses_bitmap_force) optcurses_bitmap_force = "";
            if (! strcmp(optcurses_bitmap_force, "0")) optcurses_bitmap_force = "";
        }
        if (! optcurses_bitmap_braille)
        {
            optcurses_bitmap_braille = myman_getenv("OPTCURSES_BITMAP_BRAILLE");
            if (! optcurses_bitmap_braille) optcurses_bitmap_braille = "";
            if (! strcmp(optcurses_bitmap_braille, "0")) optcurses_bitmap_braille = "";
        }
        if ((*optcurses_direct)
            ||
            ((COLS * OPTCURSES__LINES) <= 1))
        {
            /* no backing */
            if (optcurses_backing)
            {
                free((void *) optcurses_backing);
                optcurses_backing = NULL;
            }
            optcurses_bitmap = 0;
        }
        else if (! optcurses_backing)
        {
            optcurses_backing = (optcurses_cell_t *) malloc(2 * COLS * OPTCURSES__LINES * sizeof(optcurses_cell_t) + OPTCURSES__LINES);
            if (optcurses_backing)
            {
                memset((void *) optcurses_backing, 0, 2 * COLS * OPTCURSES__LINES * sizeof(optcurses_cell_t) + OPTCURSES__LINES);
            }
            else
            {
                optcurses_bitmap = 0;
            }
        }
        else
        {
            /* screen grew, enlarge backing buffer */
            if ((COLS * OPTCURSES__LINES) > (optcurses_w * optcurses_h))
            {
                optcurses_cell_t *new_backing;

                new_backing = (optcurses_cell_t *) realloc((void *) optcurses_backing, 2 * COLS * OPTCURSES__LINES * sizeof(optcurses_cell_t) + OPTCURSES__LINES);
                if (! new_backing)
                {
                    free((void *) optcurses_backing);
                }
                optcurses_backing = new_backing;
            }
            if (! optcurses_backing) optcurses_bitmap = 0;
            if (optcurses_backing)
            {
                int y;

                /* resize application backing buffer */
                if (optcurses_w < COLS)
                {
                    /* scanlines got longer, so move starting at the end */
                    for (y = OPTCURSES__LINES; y --; )
                    {
                        if (y < optcurses_h)
                        {
                            memmove((void *) (optcurses_backing + y * COLS),
                                   (void *) (optcurses_backing + y * optcurses_w),
                                   optcurses_w * sizeof(optcurses_cell_t));
                            memset((void *) (optcurses_backing + y * COLS + optcurses_w),
                                   0,
                                   (COLS - optcurses_w) * sizeof(optcurses_cell_t));
                        }
                        else
                        {
                            memset((void *) (optcurses_backing + y * COLS),
                                   0,
                                   COLS * sizeof(optcurses_cell_t));
                        }
                    }
                }
                else
                {
                    /* scanlines got shorter or remained the same length, so move starting at the beginning */
                    for (y = 0; y < OPTCURSES__LINES; y ++)
                    {
                        if (y < optcurses_h)
                        {
                            memmove((void *) (optcurses_backing + y * COLS),
                                    (void *) (optcurses_backing + y * optcurses_w),
                                    optcurses_w * sizeof(optcurses_cell_t));
                        }
                        else
                        {
                            memset((void *) (optcurses_backing + y * COLS),
                                   0,
                                   COLS * sizeof(optcurses_cell_t));
                        }
                    }
                }
                /* invalidate screen backing buffer */
                memset((void *) (optcurses_backing + OPTCURSES__LINES * COLS),
                       0,
                       OPTCURSES__LINES * COLS * sizeof(optcurses_cell_t) + OPTCURSES__LINES);
                /* screen shrunk, shrink backing buffer */
                if ((COLS * OPTCURSES__LINES) < (optcurses_w * optcurses_h))
                {
                    optcurses_cell_t *new_backing;
                    
                    new_backing = (optcurses_cell_t *) realloc((void *) optcurses_backing, 2 * COLS * OPTCURSES__LINES * sizeof(optcurses_cell_t) + OPTCURSES__LINES);
                    if (! new_backing)
                    {
                        free((void *) optcurses_backing);
                    }
                    optcurses_backing = new_backing;
                }
            }
        }
        optcurses_w = COLS;
        optcurses_h = OPTCURSES__LINES;
    }
    if (optcurses_x < 0) optcurses_x = 0;
    if (optcurses_y < 0) optcurses_y = 0;
    if (optcurses_x >= optcurses_w)
    {
        optcurses_x = 0;
        optcurses_y ++;
    }
    if (optcurses_y >= optcurses_h)
    {
        optcurses_y = optcurses_h - 1;
    }
}

static int optcurses_refresh(void);

static void optcurses_endwin(void)
{
    optcurses_refresh();
    optcurses_after_endwin = 1;
    optcurses_w = 0;
    optcurses_h = 0;
    optcurses_x = -1;
    optcurses_y = -1;
    optcurses_attr = 0;
    optcurses_clearok_flag = 0;
    optcurses_direct = NULL;
    if (optcurses_backing)
    {
        free((void *) optcurses_backing);
        optcurses_backing = NULL;
    }
    endwin();
}

static int optcurses_beep(void) {
    return beep();
}

static int optcurses_move(int y, int x)
{
    optcurses_y = y;
    optcurses_x = x;
    optcurses_init(); /* clamps coordinates */
    if (! optcurses_backing)
    {
        return move(y, x);
    }
    return OK;
}

static void optcurses_getyx(int *y, int *x)
{
    optcurses_init();
    if (! optcurses_backing)
    {
        getyx(stdscr, optcurses_y, optcurses_x);
    }
    *y = optcurses_y;
    *x = optcurses_x;
}

static int optcurses_wcwidth(wchar_t wc) {
    return wcwidth(wc);
}

static int optcurses_addch(chtype ch) {
    optcurses_init();
    if (optcurses_backing)
    {
        if (ch == '\n')
        {
            optcurses_move(optcurses_y + 1, 0);
        }
        else if (ch == '\r')
        {
            optcurses_move(optcurses_y, 0);
        }
        else if (ch == '\t')
        {
            while (optcurses_x % 8) optcurses_addch(' ');
        }
        else if (ch == '\a')
        {
            optcurses_beep();
        }
        else
        {
            wchar_t wc;
            int wcw;

#ifdef A_CHARTEXT
            wc = ch & A_CHARTEXT;
#else
            wc = ch;
#endif
            wcw = wc ? optcurses_wcwidth(wc) : 0;
            if (wcw < 1) return ERR;
            if (optcurses_x && (optcurses_x + wcw > optcurses_w)) optcurses_addch('\n');
            optcurses_x += wcw;
            do
            {
                OPTCURSES_DIRTY_ROW(optcurses_y);
                optcurses_backing[optcurses_y * optcurses_w + optcurses_x - wcw].ch = ch;
                optcurses_backing[optcurses_y * optcurses_w + optcurses_x - wcw].attr = optcurses_attr;
                ch = (chtype) ERR;
            }
            while (-- wcw);
        }
        return OK;
    }
    return addch(ch);
}

static int optcurses_attrset(attr_t a)
{
    optcurses_init();
    optcurses_attr = a;
    if (! optcurses_backing)
    {
        return attrset(a);
    }
    return OK;
}

static int optcurses_clearok(int flag)
{
    optcurses_init();
    optcurses_clearok_flag = flag;
    if (! optcurses_backing)
    {
        clearok(stdscr, flag ? TRUE : FALSE);
    }
    return OK;
}

static int optcurses_clrtoeol(void)
{
    optcurses_init();
    if (optcurses_backing)
    {
        int x;
        int ox, oy;
        attr_t oa;

        oa = optcurses_attr;
        optcurses_attrset(0);
        optcurses_getyx(&oy, &ox);
        for (x = ox; x < COLS; x ++)
        {
            optcurses_move(oy, x);
            optcurses_addch(' ');
        }
        optcurses_move(oy, ox);
        optcurses_attrset(oa);
    }
    else
    {
        clrtoeol();
    }
    return OK;
}

static int optcurses_clrtobot(void) {
    optcurses_init();
    if (optcurses_backing)
    {
        int y;
        int ox, oy;
        attr_t oa;

        oa = optcurses_attr;
        optcurses_attrset(0);
        optcurses_getyx(&oy, &ox);
        optcurses_clrtoeol();
        for (y = oy + 1; y < OPTCURSES__LINES; y ++)
        {
            optcurses_move(y, 0);
            optcurses_clrtoeol();
        }
        optcurses_move(oy, ox);
        optcurses_attrset(oa);
    }
    else
    {
        clrtobot();
    }
    return OK;
}

static int optcurses_erase(void)
{
    optcurses_init();
    if (optcurses_backing)
    {
        int ox, oy;
        attr_t oa;

        oa = optcurses_attr;
        optcurses_attrset(0);
        optcurses_getyx(&oy, &ox);
        optcurses_move(0, 0);
        optcurses_clrtobot();
        optcurses_move(oy, ox);
        optcurses_attrset(oa);
    }
    else
    {
        erase();
    }
    return OK;
}

static int optcurses_clear(void)
{
    optcurses_init();
    if (optcurses_backing)
    {
        optcurses_clearok(1);
        return optcurses_erase();
    }
    return clear();
}

static int optcurses_addstr(const char *s) {
    optcurses_init();
    if (optcurses_backing)
    {
        while (*s)
        {
            optcurses_addch((unsigned char) *s++);
        }
        return OK;
    }
    return addstr(s);
}

static int optcurses_mvprintw(int y, int x, const char *s) {
    optcurses_init();
    if (optcurses_backing)
    {
        optcurses_move(y, x);
        return optcurses_addstr(s);
    }
    return mvprintw(y, x, s);
}

#ifdef INITSCR_WITH_HINTS

static void optcurses_initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    optcurses_after_endwin = 0;
    optcurses_w = 0;
    optcurses_h = 0;
    optcurses_x = -1;
    optcurses_y = -1;
    optcurses_attr = 0;
    optcurses_clearok_flag = 0;
    optcurses_direct = NULL;
    if (optcurses_backing)
    {
        free((void *) optcurses_backing);
        optcurses_backing = NULL;
    }
    if (! optcurses_direct)
    {
        optcurses_direct = myman_getenv("OPTCURSES_DIRECT");
        if (! optcurses_direct) optcurses_direct = "";
        if (! strcmp(optcurses_direct, "0")) optcurses_direct = "";
    }
    optcurses_bitmap = NULL;
    if (! (optcurses_direct && *optcurses_direct))
    {
        if (! optcurses_bitmap)
        {
            optcurses_bitmap = myman_getenv("OPTCURSES_BITMAP");
            if (! optcurses_bitmap) optcurses_bitmap = "";
            if (! strcmp(optcurses_bitmap, "0")) optcurses_bitmap = "";
        }
    }
    initscrWithHints(((optcurses_bitmap && *optcurses_bitmap) ? ((h + 1) / 2) : h), w, title, shortname);
    optcurses_init();
    optcurses_refresh();
}

#else

static WINDOW *optcurses_initscr(void)
{
    WINDOW *stdscr = NULL;

    optcurses_after_endwin = 0;
    optcurses_w = 0;
    optcurses_h = 0;
    optcurses_x = -1;
    optcurses_y = -1;
    optcurses_attr = 0;
    optcurses_clearok_flag = 0;
    optcurses_direct = NULL;
    if (optcurses_backing)
    {
        free((void *) optcurses_backing);
        optcurses_backing = NULL;
    }
    if (! optcurses_direct)
    {
        optcurses_direct = myman_getenv("OPTCURSES_DIRECT");
        if (! optcurses_direct) optcurses_direct = "";
        if (! strcmp(optcurses_direct, "0")) optcurses_direct = "";
    }
    optcurses_bitmap = NULL;
    if (! (optcurses_direct && *optcurses_direct))
    {
        if (! optcurses_bitmap)
        {
            optcurses_bitmap = myman_getenv("OPTCURSES_BITMAP");
            if (! optcurses_bitmap) optcurses_bitmap = "";
            if (! strcmp(optcurses_bitmap, "0")) optcurses_bitmap = "";
        }
    }
    stdscr = initscr();
    optcurses_init();
    optcurses_refresh();
    return stdscr;
}

#endif

static int optcurses__combine_bitmap_attr_ok(attr_t fgattr, attr_t bgattr)
{
    /* FIXME: we should be able to combine in other color models too,
     * provided we set up appropriate color pairs */
    if (! (optcurses_bitmap_braille && *optcurses_bitmap_braille))
    {
#ifdef PAIR_NUMBER
        if (has_colors())
        {
            short fg = 0, bg = 0, dontcare;
            int fgpair, bgpair;
            attr_t revmask, rev;

            revmask = 0;
            rev = 0;
#ifdef A_STANDOUT
            revmask |= A_STANDOUT;
            rev = A_STANDOUT;
#endif
#ifdef A_REVERSE
            revmask |= A_REVERSE;
            rev = A_REVERSE;
#endif
            fgpair = PAIR_NUMBER(fgattr);
            bgpair = PAIR_NUMBER(bgattr);
            if ((fgpair > 0)
                &&
                (fgpair < COLOR_PAIRS)
                &&
                (bgpair > 0)
                &&
                (bgpair < COLOR_PAIRS)
                &&
                ((fgattr & revmask)
                 ?
                 (pair_content(fgpair, &dontcare, &fg) != ERR)
                 :
                 (pair_content(fgpair, &fg, &dontcare) != ERR))
                &&
                (fg >= 0)
                &&
                (fg < COLORS)
                &&
                ((bgattr & revmask)
                 ?
                 (pair_content(bgpair, &dontcare, &bg) != ERR)
                 :
                 (pair_content(bgpair, &bg, &dontcare) != ERR))
                &&
                (bg >= 0)
                &&
                (bg < COLORS))
            {
                int needrev = 0;
                int fgbold = 0;
                int bgbold = 0;

#ifdef A_BOLD
                fgbold = (fgattr & A_BOLD) && (! (fgattr & revmask));
                bgbold = (bgattr & A_BOLD) && (! (bgattr & revmask));
#endif
                if (bgbold && revmask)
                {
                    dontcare = fg;
                    fg = bg;
                    bg = dontcare;
                    needrev = 1;
                }
                if (fg == bg)
                {
                    fg = COLOR_BLACK;
                }
                if (fg == bg)
                {
                    fg = COLOR_WHITE;
                }
                for (fgpair = 1; fgpair < COLOR_PAIRS; fgpair ++)
                {
                    short fg2, bg2;

                    if (pair_content(fgpair, &fg2, &bg2) != ERR)
                    {
                        if ((fg2 == fg)
                            &&
                            (bg2 == bg))
                        {
                            fgattr = COLOR_PAIR(fgpair);
                            if (needrev && rev)
                            {
                                fgattr |= rev;
                                if (bgbold)
                                {
#ifdef A_BOLD
                                    fgattr |= A_BOLD;
#endif
                                }
                            }
                            else
                            {
                                if (fgbold)
                                {
#ifdef A_BOLD
                                    fgattr |= A_BOLD;
#endif
                                }
                            }
                            return 1;
                        }
                        else if (rev
                                 &&
                                 (fg2 == bg)
                                 &&
                                 (bg2 == fg)
                                 &&
                                 (! (needrev))
                                 &&
                                 (! fgbold))
                        {
                            fgattr = COLOR_PAIR(fgpair);
                            fgattr |= rev;
                            if (bgbold)
                            {
#ifdef A_BOLD
                                fgattr |= A_BOLD;
#endif
                            }
                            return 1;
                        }
                    }
                }
            }
        }
#else
#ifdef MYMAN_CACACURS_H_INCLUDED
#ifdef CACA_API_VERSION_1
        if (has_colors())
        {
            return (fgattr | bgattr) & ~(A_BOLD | A_STANDOUT | A_UNDERLINE);
        }
#else
        return fgattr && bgattr;
#endif
#else
#ifdef MYMAN_ALLEGCUR_H_INCLUDED
        return (fgattr & 0xf) || (bgattr & 0xf0);
#else
#ifdef MYMAN_GGICURS_H_INCLUDED
        return (fgattr & 0xf) || (bgattr & 0xf0);
#endif
#endif
#endif
#endif
    }
    return 0;
}

static int optcurses__cmp_bitmap_attr(attr_t fgattr, attr_t bgattr)
{
    if (fgattr < bgattr) return -1;
    if (fgattr > bgattr) return 1;
    return 0;
}

static attr_t optcurses__mix_bitmap_attr(attr_t fgattr, attr_t bgattr)
{
    if (optcurses__cmp_bitmap_attr(fgattr, bgattr) > 0)
    {
        attr_t tmp;

        tmp = fgattr;
        fgattr = bgattr;
        bgattr = tmp;
    }
    fgattr |= (bgattr & A_UNDERLINE);
    fgattr &= ((bgattr & A_BOLD) | ~A_BOLD);
    return fgattr;
}

static attr_t optcurses__combine_bitmap_attr(attr_t fgattr, attr_t bgattr)
{
    /* FIXME: we should be able to combine in other color models too,
     * provided we set up appropriate color pairs */
#ifdef PAIR_NUMBER
    if (has_colors())
    {
        short fg = 0, bg = 0, dontcare;
        int fgpair, bgpair;
        attr_t revmask, rev;
        attr_t boldbit = 0;

        revmask = 0;
        rev = 0;
#ifdef A_STANDOUT
        revmask |= A_STANDOUT;
        rev = A_STANDOUT;
#endif
#ifdef A_REVERSE
        revmask |= A_REVERSE;
        rev = A_REVERSE;
#endif
#ifdef A_BOLD
        boldbit = A_BOLD;
#endif
        fgpair = PAIR_NUMBER(fgattr);
        bgpair = PAIR_NUMBER(bgattr);
        if ((fgpair > 0)
            &&
            (fgpair < COLOR_PAIRS)
            &&
            (bgpair > 0)
            &&
            (bgpair < COLOR_PAIRS)
            &&
            ((fgattr & revmask)
             ?
             (pair_content(fgpair, &dontcare, &fg) != ERR)
             :
             (pair_content(fgpair, &fg, &dontcare) != ERR))
            &&
            (fg >= 0)
            &&
            (fg < COLORS)
            &&
            ((bgattr & revmask)
             ?
             (pair_content(bgpair, &dontcare, &bg) != ERR)
             :
             (pair_content(bgpair, &bg, &dontcare) != ERR))
            &&
            (bg >= 0)
            &&
            (bg < COLORS))
        {
            int needrev = 0;
            int fgbold = 0;
            int bgbold = 0;

            fgbold = (fgattr & boldbit) && (! (fgattr & revmask));
            bgbold = (bgattr & boldbit) && (! (bgattr & revmask));
            if (bgbold && revmask)
            {
                dontcare = fg;
                fg = bg;
                bg = dontcare;
                needrev = 1;
            }
            if (fg == bg)
            {
                fg = COLOR_BLACK;
            }
            if (fg == bg)
            {
                fg = COLOR_WHITE;
            }
            for (fgpair = 1; fgpair < COLOR_PAIRS; fgpair ++)
            {
                short fg2, bg2;

                if (pair_content(fgpair, &fg2, &bg2) != ERR)
                {
                    if ((fg2 == fg)
                        &&
                        (bg2 == bg))
                    {
                        fgattr = COLOR_PAIR(fgpair);
                        if (needrev && rev)
                        {
                            fgattr |= rev;
                            if (bgbold)
                            {
                                fgattr |= boldbit;
                            }
                        }
                        else
                        {
                            if (fgbold)
                            {
                                fgattr |= boldbit;
                            }
                        }
                        return fgattr;
                    }
                    else if (rev
                             &&
                             (fg2 == bg)
                             &&
                             (bg2 == fg)
                             &&
                             (! (needrev))
                             &&
                             (! fgbold))
                    {
                        fgattr = COLOR_PAIR(fgpair);
                        fgattr |= rev;
                        if (bgbold)
                        {
                            fgattr |= boldbit;
                        }
                        return fgattr;
                    }
                }
            }
        }
    }
#else
#ifdef MYMAN_CACACURS_H_INCLUDED
#ifdef CACA_API_VERSION_1
    if (has_colors())
    {
        unsigned char fg, bg;
        unsigned long oattr;

        fg = cucul_attr_to_ansi_fg(fgattr) | ((fgattr & A_BOLD) ? 8 : 0);
        bg = cucul_attr_to_ansi_fg(bgattr) | ((bgattr & A_BOLD) ? 8 : 0);
        oattr = cucul_get_attr(cv, -1, -1);
        cucul_set_attr(cv, fgattr);
        cucul_set_color_ansi(cv, fg, bg);
        if (can_change_color())
        {
            cucul_set_color_argb(cv,
                                 0xf000u | cucul_attr_to_rgb12_fg(fgattr),
                                 0xf000u | cucul_attr_to_rgb12_fg(bgattr));
        }
        fgattr = cucul_get_attr(cv, -1, -1);
        cucul_set_attr(cv, oattr);
    }
#else
    fgattr = (fgattr & ~0xf) | ((bgattr & 0xf0) >> 4) | ((bgattr & A_BOLD) ? 0x8 : 0);
#endif
#else
#ifdef MYMAN_ALLEGCUR_H_INCLUDED
    fgattr = (fgattr & ~0xf0) | ((bgattr & 0xf) << 4);
#else
#ifdef MYMAN_GGICURS_H_INCLUDED
    fgattr = (fgattr & ~0xf0) | ((bgattr & 0xf) << 4);
#endif
#endif
#endif
#endif
    return fgattr;
}

static void optcurses__combine_bitmap(chtype *chout, attr_t *attrout, chtype ch, attr_t attr, chtype ch1, attr_t attr1)
{
    attr_t revmask, rev;

    revmask = A_STANDOUT;
    rev = A_STANDOUT;
#ifdef A_REVERSE
    revmask |= A_REVERSE;
    rev = A_REVERSE;
#endif
#ifdef PAIR_NUMBER
    if (has_colors())
    {
        short pn, fg, bg;

        pn = PAIR_NUMBER(attr);
        if ((pn > 0) && (pn < COLOR_PAIRS)
            &&
            (pair_content(pn, &fg, &bg) != ERR)
            &&
            (fg >= 0)
            &&
            (fg < COLORS)
            &&
            (bg >= 0)
            &&
            (bg < COLORS))
        {
            short r, g, b;

            if (attr & revmask)
            {
                short tmp;

                tmp = fg;
                fg = bg;
                bg = tmp;
            }
            if ((ch == ACS_BLOCK)
#ifdef OPTCURSES_UNICODE
                ||
                (ch == 0x2588u)
                ||
                (ch == 0x25A0u)
#else
                ||
                (ch == 0xdb)
#endif
                ||
                (ch == '#'))
            {
                short tmp;

                tmp = fg;
                fg = bg;
                bg = tmp;
            }
            if ((can_change_color()
                 &&
                 (color_content(bg, &r, &g, &b) != ERR))
                ?
                (r || g || b)
                :
                (bg != COLOR_BLACK))
            {
                int i;

                for (i = 1; i < COLOR_PAIRS; i ++)
                {
                    short fg2, bg2;

                    if ((pair_content(i, &fg2, &bg2) != ERR)
                        &&
                        (fg2 == bg)
                        &&
                        (bg2 >= 0)
                        &&
                        (bg2 < COLORS)
                        &&
                        ((can_change_color()
                          &&
                          (color_content(bg2, &r, &g, &b) != ERR))
                         ?
                         ((! r) && (! g) && (! b))
                         :
                         (bg2 == COLOR_BLACK)))
                    {
                        attr &= ~revmask;
                        attr &= ~COLOR_PAIR(pn);
                        attr |= COLOR_PAIR(i);
                        ch = '#';
                        break;
                    }
                }
            }
        }
        pn = PAIR_NUMBER(attr1);
        if ((pn > 0) && (pn < COLOR_PAIRS)
            &&
            (pair_content(pn, &fg, &bg) != ERR)
            &&
            (fg >= 0)
            &&
            (fg < COLORS)
            &&
            (bg >= 0)
            &&
            (bg < COLORS))
        {
            short r, g, b;

            if (attr1 & revmask)
            {
                short tmp;

                tmp = fg;
                fg = bg;
                bg = tmp;
            }
            if ((ch1 == ACS_BLOCK)
#ifdef OPTCURSES_UNICODE
                ||
                (ch1 == 0x2588u)
                ||
                (ch1 == 0x25A0u)
#else
                ||
                (ch1 == 0xdb)
#endif
                ||
                (ch1 == '#'))
            {
                short tmp;

                tmp = fg;
                fg = bg;
                bg = tmp;
            }
            if ((can_change_color()
                 &&
                 (color_content(bg, &r, &g, &b) != ERR))
                ?
                (r || g || b)
                :
                (bg != COLOR_BLACK))
            {
                int i;

                for (i = 1; i < COLOR_PAIRS; i ++)
                {
                    short fg2, bg2;

                    if ((pair_content(i, &fg2, &bg2) != ERR)
                        &&
                        (fg2 == bg)
                        &&
                        (bg2 >= 0)
                        &&
                        (bg2 < COLORS)
                        &&
                        ((can_change_color()
                          &&
                          (color_content(bg2, &r, &g, &b) != ERR))
                         ?
                         ((! r) && (! g) && (! b))
                         :
                         (bg2 == COLOR_BLACK)))
                    {
                        attr1 &= ~revmask;
                        attr1 &= ~COLOR_PAIR(pn);
                        attr1 |= COLOR_PAIR(i);
                        ch1 = '#';
                        break;
                    }
                }
            }
        }
    }
#endif
    if (attr & revmask)
    {
        attr &= ~revmask;
        if ((attr1 & revmask)
            &&
            (! (optcurses_bitmap_braille && *optcurses_bitmap_braille))
#ifdef OPTCURSES_UNICODE
            &&
            (wcwidth(0x2580u) == 1)
#else
            &&
            (wcwidth(0xdf) == 1)
#endif
            )
        {
            attr1 &= ~revmask;
            optcurses__combine_bitmap(chout, attrout, ch, attr, ch1, attr1);
            *attrout |= rev;
            return;
        }
        if ((ch == (chtype) ERR)
             ||
             (ch == ' ')
             ||
             (ch == '\0')
#ifdef OPTCURSES_UNICODE
             ||
             (ch == 0x00a0u)
             ||
             (ch == 0x3000u)
             ||
             (ch == 0x2008u)
#else
             ||
             (ch == 0xff)
#endif
            )
        {
            ch = '#';
        }
        else
        {
            ch = ' ';
        }
    }
    if (attr1 & revmask)
    {
        attr1 &= ~revmask;
        if ((ch1 == (chtype) ERR)
             ||
             (ch1 == ' ')
             ||
             (ch1 == '\0')
#ifdef OPTCURSES_UNICODE
             ||
             (ch1 == 0x00a0u)
             ||
             (ch1 == 0x3000u)
             ||
             (ch1 == 0x2008u)
#else
             ||
             (ch1 == 0xff)
#endif
            )
        {
            ch1 = '#';
        }
        else
        {
            ch1 = ' ';
        }
    }
    if ((ch == (chtype) ERR)
        &&
        (ch1 == (chtype) ERR))
    {
        /* NOP */
    }
    else if ((ch == (chtype) ERR)
             ||
             (ch == ' ')
             ||
             (ch == '\0')
#ifdef OPTCURSES_UNICODE
             ||
             (ch == 0x00a0u)
             ||
             (ch == 0x3000u)
             ||
             (ch == 0x2008u)
#else
             ||
             (ch == 0xff)
#endif
        )
    {
        if ((ch1 == (chtype) ERR)
            ||
            (ch1 == ' ')
            ||
            (ch1 == '\0')
#ifdef OPTCURSES_UNICODE
            ||
            (ch1 == 0x00a0u)
            ||
            (ch1 == 0x3000u)
            ||
            (ch1 == 0x2008u)
#else
            ||
            (ch1 == 0xff)
#endif
            )
        {
            ch = ' ';
        }
        else
        {
#ifdef OPTCURSES_UNICODE
            ch = (optcurses_bitmap_braille && *optcurses_bitmap_braille) ? 0x28e4u : 0x2584u;
#else
            ch = 0xdc;
#endif
            if (wcwidth(ch) != 1)
            {
                ch = ',';
            }
            attr = attr1;
        }
    }
    else
    {
        if ((ch1 == (chtype) ERR)
            ||
            (ch1 == ' ')
            ||
            (ch1 == '\0')
#ifdef OPTCURSES_UNICODE
            ||
            (ch1 == 0x00a0u)
            ||
            (ch1 == 0x3000u)
            ||
            (ch1 == 0x2008u)
#else
            ||
            (ch1 == 0xff)
#endif
            )
        {
#ifdef OPTCURSES_UNICODE
            ch = (optcurses_bitmap_braille && *optcurses_bitmap_braille) ? 0x281bu : 0x2580u;
#else
            ch = 0xdf;
#endif
            if (wcwidth(ch) != 1)
            {
                ch = '\"';
            }
        }
        else
        {
            if ((optcurses__cmp_bitmap_attr(attr, attr1) < 0) && optcurses__combine_bitmap_attr_ok(attr, attr1))
            {
#ifdef OPTCURSES_UNICODE
                ch = (optcurses_bitmap_braille && *optcurses_bitmap_braille) ? 0x28e4u : 0x2584u;
#else
                ch = 0xdc;
#endif
                if (wcwidth(ch) != 1)
                {
                    ch = (optcurses_bitmap_force && *optcurses_bitmap_force) ? ',' : '#';
                    attr = (optcurses_bitmap_force && *optcurses_bitmap_force) ? optcurses__combine_bitmap_attr(attr1, attr) : optcurses__mix_bitmap_attr(attr1, attr);
                }
                else
                {
                    attr = optcurses__combine_bitmap_attr(attr1, attr);
                }
            }
            else if ((optcurses__cmp_bitmap_attr(attr, attr1) > 0) && optcurses__combine_bitmap_attr_ok(attr, attr1))
            {
#ifdef OPTCURSES_UNICODE
                ch = (optcurses_bitmap_braille && *optcurses_bitmap_braille) ? 0x281bu : 0x2580u;
#else
                ch = 0xdf;
#endif
                if (wcwidth(ch) != 1)
                {
                    ch = (optcurses_bitmap_force && *optcurses_bitmap_force) ? '\"' : '#';
                    attr = (optcurses_bitmap_force && *optcurses_bitmap_force) ? optcurses__combine_bitmap_attr(attr, attr1) : optcurses__mix_bitmap_attr(attr, attr1);
                }
                else
                {
                    attr = optcurses__combine_bitmap_attr(attr, attr1);
                }
            }
            else
            {
#ifdef ACS_BLOCK
                ch = ACS_BLOCK;
                if (optcurses_bitmap_braille && *optcurses_bitmap_braille)
#endif
                {
#ifdef OPTCURSES_UNICODE
                    ch = (optcurses_bitmap_braille && *optcurses_bitmap_braille) ? 0x28ffu : 0x2588u;
#else
                    ch = 0xdb;
#endif
                }
                if (wcwidth(ch) != 1)
                {
                    ch = '#';
                }
                if (optcurses__cmp_bitmap_attr(attr, attr1) < 0)
                {
                    attr = optcurses__mix_bitmap_attr(attr1, attr);
                }
                else
                {
                    attr = optcurses__mix_bitmap_attr(attr, attr1);
                }
            }
        }
    }
    *chout = ch;
    *attrout = attr;
}

static int optcurses_refresh(void)
{
    if (! optcurses_after_endwin)
    {
        optcurses_init();
        if (optcurses_backing)
        {
            int x, y, y_permute;
            int ox, oy;
            attr_t a;
            int changed;

            changed = 0;
            a = 0;
            ox = -1;
            oy = -1;
            if (optcurses_clearok_flag)
            {
                changed = 1;
                optcurses_clearok_flag = 0;
                attrset(0);
                clear();
                memset((void *) (optcurses_backing + optcurses_h * optcurses_w),
                       0,
                       optcurses_h * optcurses_w * sizeof(optcurses_cell_t) + optcurses_h);
            }
            for (y_permute = 0; y_permute < optcurses_h; y_permute ++)
            {
                y = (optcurses_interlace && *optcurses_interlace) ? (2 * (y_permute % ((optcurses_h + 1) / 2)) + (y_permute / ((optcurses_h + 1) / 2))) : y_permute;
                if ((y < OPTCURSES__LINES) && OPTCURSES_IS_ROW_DIRTY(y))
                {
                    for (x = 0; (x < optcurses_w) && (x < COLS); x ++)
                    {
                        attr_t attr, attr2;
                        chtype ch, ch2;

                        attr = optcurses_backing[y * optcurses_w + x].attr;
                        ch = optcurses_backing[y * optcurses_w + x].ch;
                        attr2 = optcurses_backing[(optcurses_h + y) * optcurses_w + x].attr;
                        ch2 = optcurses_backing[(optcurses_h + y) * optcurses_w + x].ch;
                        if (optcurses_bitmap && *optcurses_bitmap)
                        {
                            attr_t attr1, attr3;
                            chtype ch1, ch3;

                            attr1 = attr;
                            ch1 = ERR;
                            attr3 = attr2;
                            ch3 = ERR;
                            if ((y + 1) < OPTCURSES__LINES)
                            {
                                attr1 = optcurses_backing[(y + 1) * optcurses_w + x].attr;
                                ch1 = optcurses_backing[(y + 1) * optcurses_w + x].ch;
                                attr3 = optcurses_backing[(optcurses_h + y + 1) * optcurses_w + x].attr;
                                ch3 = optcurses_backing[(optcurses_h + y + 1) * optcurses_w + x].ch;
                            }
                            optcurses__combine_bitmap(&ch, &attr, ch, attr, ch1, attr1);
                            optcurses__combine_bitmap(&ch2, &attr2, ch2, attr2, ch3, attr3);
                        }
                        if (! ch2) ch2 = ' ';
                        if ((ch
                             !=
                             ch2)
                            ||
                            (attr
                             !=
                             attr2))
                        {
                            optcurses_backing[(optcurses_h + y) * optcurses_w + x] =
                                optcurses_backing[y * optcurses_w + x];
                            if (optcurses_bitmap && *optcurses_bitmap)
                            {
                                optcurses_backing[(optcurses_h + y + 1) * optcurses_w + x] =
                                    optcurses_backing[(y + 1) * optcurses_w + x];
                            }
                            if (ch
                                &&
                                (ch != (chtype) ERR))
                            {
                                int nx, ny;
                                int wcw;

                                changed = 1;
                                if ((ox != x) || (oy != y)) move(y / ((optcurses_bitmap && *optcurses_bitmap) ? 2 : 1), x);
                                if (a != attr)
                                {
                                    if (! attr) standend();
                                    else if ((! a) && (attr == A_STANDOUT)) standout();
                                    else attrset(attr);
                                    a = attr;
                                }
                                wcw = optcurses_wcwidth(ch);
                                if (wcw < 1) wcw = 1;
                                if ((x > 0) && ((x + wcw) == optcurses_w) && ((y + ((optcurses_bitmap && *optcurses_bitmap) ? 1 : 0)) >= (OPTCURSES__LINES - 1)))
                                {
                                    int i;
                                    chtype chprev;
                                    attr_t attrprev;
                                    int wcwprev;

                                    chprev = ERR;
                                    for (i = 1;
                                         ((chprev = optcurses_backing[(optcurses_h + y) * optcurses_w + x - i].ch) == (chtype) ERR) && ((x - i) > 0);
                                         i ++)
                                    {
                                    }
                                    if (chprev != (chtype) ERR)
                                    {
                                        attrprev = optcurses_backing[(optcurses_h + y) * optcurses_w + x - i].attr;
                                        if (optcurses_bitmap && *optcurses_bitmap)
                                        {
                                            attr_t attrprev1;
                                            chtype chprev1;

                                            attrprev1 = attrprev;
                                            chprev1 = ((chtype) ERR);
                                            if ((y + 1) < OPTCURSES__LINES)
                                            {
                                                attrprev1 = optcurses_backing[(optcurses_h + y + 1) * optcurses_w + x].attr;
                                                chprev1 = optcurses_backing[(optcurses_h + y + 1) * optcurses_w + x].ch;
                                            }
                                            optcurses__combine_bitmap(&chprev, &attrprev, chprev, attrprev, chprev1, attrprev1);
                                        }
                                        move(y / ((optcurses_bitmap && *optcurses_bitmap) ? 2 : 1), x - i);
                                        delch();
                                        delch();
                                        if (insch(ch) != ERR)
                                        {
                                            if (attrprev != attr)
                                            {
                                                if (! attrprev) standend();
                                                else if ((! a) && (attrprev == A_STANDOUT)) standout();
                                                else attrset(attrprev);
                                                a = attrprev;
                                            }
                                            wcwprev = optcurses_wcwidth(chprev);
                                            insch(chprev);
                                            if (a != attr)
                                            {
                                                if (! attr) standend();
                                                else if ((! a) && (attr == A_STANDOUT)) standout();
                                                else attrset(attr);
                                                a = attr;
                                            }
                                        }
                                    }
                                }
                                move(y / ((optcurses_bitmap && *optcurses_bitmap) ? 2 : 1), x);
                                addch(ch);
                                getyx(stdscr, ny, nx);
                                if ((nx % COLS) != ((x + wcw) % COLS))
                                {
                                    int i;

                                    for (i = 0; i < wcw; i ++)
                                    {
                                        move(y / ((optcurses_bitmap && *optcurses_bitmap) ? 2 : 1), x + i);
                                        addch(' ');
                                    }
                                    move(y / ((optcurses_bitmap && *optcurses_bitmap) ? 2 : 1), x);
                                    addch(ch);
                                    getyx(stdscr, ny, nx);
                                }
                                ox = x + wcw;
                                oy = y;
                                if ((nx % COLS) != (ox % COLS))
                                {
                                    ox = -1;
                                }
                                while (-- wcw)
                                {
                                    x ++;
                                    optcurses_backing[(optcurses_h + y) * optcurses_w + x] =
                                        optcurses_backing[y * optcurses_w + x];
                                }
                            }
                        }
                    }
                    OPTCURSES_CLEAN_ROW(y);
                }
            }
            if (a)
            {
                changed = 1;
                attrset(0);
            }
            if (! changed) return OK;
        }
    }
    /* reinitialize in case a geometry change was detected during rendering */
    optcurses_init();
    return refresh();
}

static int optcurses_init_color(short i, short r, short g, short b)
{
    optcurses_init();
    if (optcurses_backing)
    {
        optcurses_clearok(1);
    }
    return init_color(i, r, g, b);
}

#undef init_color
#define init_color optcurses_init_color

#undef wcwidth
#define wcwidth optcurses_wcwidth

#undef initscrWithHints
#define initscrWithHints optcurses_initscrWithHints

#undef initscr
#define initscr optcurses_initscr

#undef endwin
#define endwin optcurses_endwin

#undef LINES
#define LINES optcurses_h

#undef COLS
#define COLS optcurses_w

#undef refresh
#define refresh optcurses_refresh

#undef move
#define move optcurses_move

#undef getyx
#define getyx(stdscr, y, x) (optcurses_getyx(&optcurses_y, &optcurses_x), ((x) = optcurses_x), ((y) = optcurses_y))

#undef attrset
#define attrset optcurses_attrset

#undef standout
#define standout() optcurses_attrset(optcurses_attr | A_STANDOUT)

#undef standend
#define standend() optcurses_attrset(0)

#undef addch
#define addch optcurses_addch

#undef erase
#define erase optcurses_erase

#undef clearok
#define clearok(stdscr, flag) optcurses_clearok(((flag) == FALSE) ? 0 : 1)

#undef clear
#define clear optcurses_clear

#undef clrtoeol
#define clrtoeol optcurses_clrtoeol

#undef clrtobot
#define clrtobot optcurses_clrtobot

#undef addstr
#define addstr optcurses_addstr

#undef mvprintw
#define mvprintw optcurses_mvprintw

#endif /* MYMAN_OPTCURS_H_INCLUDED */
