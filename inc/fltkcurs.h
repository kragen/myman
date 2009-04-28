/*
 * fltkcurs.h - FLTK driver for the MyMan video game
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

#ifndef __cplusplus
#error you must compile this as C++ using e.g. make CC=g++ clean all
#endif

#ifdef FLTK2CURSES

#include <fltk/compat/FL/Fl.H>
#include <fltk/compat/FL/Fl_Text_Buffer.H>
#include <fltk/compat/FL/Fl_Text_Editor.H>
#include <fltk/compat/FL/Fl_Window.H>
#include <fltk/compat/FL/fl_ask.H>
#include <fltk/compat/FL/fl_draw.H>
typedef fltk::Font *Fl_Font;
typedef fltk::TextDisplay Fl_Text_Display;
#ifndef fl_beep
#define fl_beep (fltk::beep)
#endif

#else

#include <FL/Fl.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Window.H>
#include <FL/fl_ask.H>

#endif

/* work-arounds for the Fast Light Tool Kit (FLTK) */

#ifndef FLTKCURSES_FONT
#define FLTKCURSES_FONT "fixed"
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

#ifndef USE_SIGWINCH
#define USE_SIGWINCH 0
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
#define wcwidth(ch) fltkcurses_wcwidth(ch)

static int fltkcurses_argc;
static char *fltkcurses_argv[2] = { NULL, NULL };
static char *fltkcurses_title = NULL;
static int fltkcurses_ready = 0;

typedef unsigned long int chtype;

typedef chtype attr_t;

static int fltkcurses_x = 0, fltkcurses_y = 0, fltkcurses_w = 0, fltkcurses_h = 0;
static double fltkcurses_charw = 0.0, fltkcurses_charh = 0.0;
static int fltkcurses_pixw = 0, fltkcurses_pixh = 0;
static int fltkcurses_fullscreen = 0;
static int fltkcurses_oldpixw = 0, fltkcurses_oldpixh = 0;
static int fltkcurses_oldleft = 0, fltkcurses_oldtop = 0;

static attr_t fltkcurses_attr = 0;
static char fltkcurses_curattr = 'A';

class Fltkcurses_Text_Display : public Fl_Text_Display
{
  public:
    inline Fltkcurses_Text_Display(int x, int y, int w, int h) : Fl_Text_Display(x, y, w, h)
        {
        }
    inline void xy_to_rowcol(int x, int y, int* row, int* column,
                             int PosType = CHARACTER_POS)
        {
            return Fl_Text_Display::xy_to_rowcol(x, y, row, column, PosType);
        }
    inline int x() { return Fl_Text_Display::x(); }
    inline int y() { return Fl_Text_Display::y(); }
    inline int w() { return Fl_Text_Display::w(); }
    inline int h() { return Fl_Text_Display::h(); }
};

class Fltkcurses_Window : public Fl_Window
{
  private:
#ifdef FLTK2CURSES
    int cached_w, cached_h, cached_x, cached_y;
#endif
  public:
    inline Fltkcurses_Window(int w, int h, const char *title) : Fl_Window(w, h, title)
        {
#ifdef FLTK2CURSES
            this->cached_w = this->w();
            this->cached_h = this->h();
            this->cached_x = this->x();
            this->cached_y = this->y();
            this->resize(this->x(), this->y(), this->w(), this->h());
#endif
        };
    inline int x() { return Fl_Window::x(); }
    inline int y() { return Fl_Window::y(); }
    inline int w() { return Fl_Window::w(); }
    inline int h() { return Fl_Window::h(); }
    int handle(int event);
    void resize(int x, int y, int w, int h);
};

static Fltkcurses_Window *fltkcurses_window = NULL;
static Fl_Text_Buffer *fltkcurses_textBuffer = NULL;
static Fl_Text_Buffer *fltkcurses_styleBuffer = NULL;
static Fltkcurses_Text_Display *fltkcurses_textDisplay = NULL;
static chtype fltkcurses_getch_buffer[] = { ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR };

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

#define getyx(stdscr, y, x) ((y) = fltkcurses_y), ((x) = fltkcurses_x)

#define wrefresh(stdscr) refresh()

#define beep() addch('\a')

#define use_env(x)

#define echo()

#define has_colors() (1)

#define can_change_color() (1)

#define curs_set(x)

#define keypad(stdscr, x)

#define LINES (fltkcurses_h)

#define COLS (fltkcurses_w)

#define KEY_UP FL_Up

#define KEY_DOWN FL_Down

#define KEY_LEFT FL_Left

#define KEY_RIGHT FL_Right

#define KEY_RESIZE 0x10fffdUL

#define A_DIM 0x800000UL

#define A_UNDERLINE A_DIM

#define A_BOLD 0x400000UL

#define A_STANDOUT 0x200000UL

#define A_CHARTEXT 0x1fffffUL

#define A_REVERSE A_STANDOUT

#define FLTKCURSES__A_ALTCHARSET 0x1f0000L

#define FLTKCURSES__A_COLOR 0xff000000UL

#define COLOR_PAIRS 256

#define COLORS FL_NUM_FREE_COLOR

#define COLOR_BLACK 0

#define COLOR_BLUE 4

#define COLOR_GREEN 2

#define COLOR_CYAN (COLOR_BLUE | COLOR_GREEN)

#define COLOR_RED 1

#define COLOR_MAGENTA (COLOR_RED | COLOR_BLUE)

#define COLOR_YELLOW (COLOR_RED | COLOR_GREEN)

#define COLOR_WHITE (COLOR_RED | COLOR_GREEN | COLOR_BLUE)

#define PEN_BRIGHT 8

#define _PAIR_SHIFT 24

#define COLOR_PAIR(p) ((p) << _PAIR_SHIFT)

#define PAIR_NUMBER(a) (((a) & FLTKCURSES__A_COLOR) >> _PAIR_SHIFT)

#define ACS_LRCORNER (FLTKCURSES__A_ALTCHARSET | 0x2518L)

#define ACS_URCORNER (FLTKCURSES__A_ALTCHARSET | 0x2510L)

#define ACS_ULCORNER (FLTKCURSES__A_ALTCHARSET | 0x250cL)

#define ACS_LLCORNER (FLTKCURSES__A_ALTCHARSET | 0x2514L)

#define ACS_PLUS (FLTKCURSES__A_ALTCHARSET | 0x253cL)

#define ACS_HLINE (FLTKCURSES__A_ALTCHARSET | 0x2500L)

#define ACS_LTEE (FLTKCURSES__A_ALTCHARSET | 0x251cL)

#define ACS_RTEE (FLTKCURSES__A_ALTCHARSET | 0x2524L)

#define ACS_BTEE (FLTKCURSES__A_ALTCHARSET | 0x2534L)

#define ACS_TTEE (FLTKCURSES__A_ALTCHARSET | 0x252cL)

#define ACS_VLINE (FLTKCURSES__A_ALTCHARSET | 0x2502L)

#define ACS_BULLET (FLTKCURSES__A_ALTCHARSET | 0x00b7L)

#define ACS_BLOCK (FLTKCURSES__A_ALTCHARSET | 0x2588L)

#define FLTKCURSES__ACS_THALF 0x2580L

#define FLTKCURSES__ACS_BHALF 0x2584L

#define FLTKCURSES__ACS_LHALF 0x258cL

#define FLTKCURSES__ACS_RHALF 0x2590L

#define INITSCR_WITH_HINTS

static struct { short fg, bg; } fltkcurses_pairs[COLOR_PAIRS];
static struct { uchar red, green, blue; } fltkcurses_colors[COLORS];
static Fl_Text_Display::Style_Table_Entry fltkcurses_palette[COLORS];

static int fltkcurses_wcwidth(int wc);

static size_t fltkcurses_chtypetombs(chtype ch, char *buf)
{
    char *buf0;

    buf0 = buf;
    *buf0 = 0;
    ch &= A_CHARTEXT;
    if ((ch & FLTKCURSES__A_ALTCHARSET) == FLTKCURSES__A_ALTCHARSET)
    {
        if (fltkcurses_wcwidth(ch & ~FLTKCURSES__A_ALTCHARSET) == 1)
        {
            ch &= ~FLTKCURSES__A_ALTCHARSET;
        }
        else
        {
            return 0;
        }
    }
    if (ch < 0) return 0;
    if (ch > 0x10fffdL) return 0;
    if ((ch & 0xfffeL) == 0xfffeL) return 0;
    if (ch <= 0x7fL)
    {
        *(buf++) = ch;
    }
#ifndef FLTK2CURSES
    /* FLTK1 always uses ISO 8859-1 (bleh) */
    else if (ch <= 0xffL)
    {
        *(buf++) = ch;
    }
#else
    /* FLTK2 always uses UTF-8 (yay!) */
    else if (ch <= 0x7ffL)
    {
        *(buf++) = 0xc0 | (ch >> 6);
        *(buf++) = 0x80 | (ch & 0x3f);
    }
    else if (ch <= 0xffffL)
    {
        *(buf++) = 0xe0 | (ch >> 12);
        *(buf++) = 0x80 | ((ch >> 6) & 0x3f);
        *(buf++) = 0x80 | (ch & 0x3f);
    }
    else if (ch <= 0x10ffffL)
    {
        *(buf++) = 0xf0 | (ch >> 18);
        *(buf++) = 0x80 | ((ch >> 12) & 0x3f);
        *(buf++) = 0x80 | ((ch >> 6) & 0x3f);
        *(buf++) = 0x80 | (ch & 0x3f);
    }
#endif
    *buf = 0;
    return strlen(buf0);
}

#ifndef FLTKCURSES_MB_LEN_MAX
#define FLTKCURSES_MB_LEN_MAX 6
#endif

#ifndef FLTKCURSES_WFUDGE
#ifndef FLTK2CURSES
#define FLTKCURSES_WFUDGE 1
#else
#define FLTKCURSES_WFUDGE 1
#endif
#endif

static struct { chtype ch; int wcw; } fltkcurses_wcwidth_cache[] = {
    { ERR, -1 },
    { ERR, -1 },
    { ERR, -1 },
    { ERR, -1 },
    { ERR, -1 }
};
static int fltkcurses_wcwidth_cache_offset = 0;

static int fltkcurses_wcwidth(int wc)
{
    int x;
    int i;
    char buf[FLTKCURSES_MB_LEN_MAX];
    chtype ch = (chtype) wc;
    int ret = -1;

    ch &= A_CHARTEXT;
    for (i = 0; i < (int) (sizeof(fltkcurses_wcwidth_cache)/sizeof(*fltkcurses_wcwidth_cache)); i ++)
    {
        if (ch == fltkcurses_wcwidth_cache[i].ch)
        {
            return fltkcurses_wcwidth_cache[i].wcw;
        }
    }
    if (ch == (chtype) ERR)
    {
        ret = -1;
        goto done;
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
        ret = 1;
        goto done;
    }
    if (! fltkcurses_chtypetombs(ch, buf))
    {
        ret = 0;
        goto done;
    }
    fl_font(fltkcurses_palette[0].font, fltkcurses_palette[0].size);
    x = fl_width(buf);
    ret = (ch >= (chtype) (unsigned long) (unsigned char) ' ') ?
        ((int) ((x + fltkcurses_charw - 1) / fltkcurses_charw))
        :
        0;
  done:
    fltkcurses_wcwidth_cache[fltkcurses_wcwidth_cache_offset].ch = ch;
    fltkcurses_wcwidth_cache[fltkcurses_wcwidth_cache_offset].wcw = ret;
    fltkcurses_wcwidth_cache_offset ++;
    fltkcurses_wcwidth_cache_offset %= sizeof(fltkcurses_wcwidth_cache) / sizeof(*fltkcurses_wcwidth_cache);
    return ret;
}

static void fltkcurses_ungetch(chtype ch)
{
    int i;

    for (i = 0; i < (int) (sizeof(fltkcurses_getch_buffer)/sizeof(*fltkcurses_getch_buffer)); i ++)
    {
        if (fltkcurses_getch_buffer[i] == (chtype) ERR)
        {
            fltkcurses_getch_buffer[i] = ch;
            return;
        }
    }
    fl_beep();
}

int Fltkcurses_Window::handle(int event)
{
#ifdef FLTK2CURSES
    if ((this->w() != this->cached_w)
        ||
        (this->h() != this->cached_h)
        ||
        (this->x() != this->cached_x)
        ||
        (this->y() != this->cached_y))
    {
        this->cached_w = this->w();
        this->cached_h = this->h();
        this->cached_x = this->x();
        this->cached_y = this->y();
        this->resize(this->x(), this->y(), this->w(), this->h());
    }
#endif
    switch(event)
    {
    case FL_FOCUS:
    case FL_UNFOCUS:
    case FL_KEYUP:
        return 1;
    case FL_KEYDOWN:
    {
        int key;
        const char *text;
        int length;

        key = Fl::event_key();
        text = Fl::event_text();
        length = Fl::event_length();
        switch(key)
        {
        case KEY_UP:
        case KEY_DOWN:
        case KEY_LEFT:
        case KEY_RIGHT:
            fltkcurses_ungetch(key);
            return 1;
        case FL_BackSpace:
            fltkcurses_ungetch('\b');
            return 1;
        case FL_Tab:
            fltkcurses_ungetch('\t');
            return 1;
        case FL_Enter:
            if (Fl::event_state() & (FL_ALT | FL_META | FL_CTRL))
            {
                fltkcurses_fullscreen = ! fltkcurses_fullscreen;
                if (fltkcurses_fullscreen)
                {
                    fltkcurses_window->fullscreen();
                }
                else
                {
                    fltkcurses_window->fullscreen_off(fltkcurses_oldleft,
                                                      fltkcurses_oldtop,
                                                      fltkcurses_oldpixw,
                                                      fltkcurses_oldpixh);
                }
            }
            else
            {
                fltkcurses_ungetch('\r');
            }
            return 1;
        case FL_Escape:
            fltkcurses_ungetch('\x1b');
            return 1;
        case FL_Delete:
            fltkcurses_ungetch('\133');
            return 1;
        default:
            if (length == 1)
            {
                if (((*text == '\r') || (*text == '\n')) && (Fl::event_state() & (FL_ALT | FL_META | FL_CTRL)))
                {
                    fltkcurses_fullscreen = ! fltkcurses_fullscreen;
                    if (fltkcurses_fullscreen)
                    {
                        fltkcurses_window->fullscreen();
                    }
                    else
                    {
                        fltkcurses_window->fullscreen_off(fltkcurses_oldleft,
                                                          fltkcurses_oldtop,
                                                          fltkcurses_oldpixw,
                                                          fltkcurses_oldpixh);
                    }
                }
                else
                {
                    fltkcurses_ungetch((chtype) (unsigned long) (unsigned char) *text);
                }
                return 1;
            }
        }
        return 0;
    }
    default:
        return Fl_Window::handle(event);
    }
}

static int fltkcurses_pad = 4, fltkcurses_pad2 = 10;

void Fltkcurses_Window::resize(int x, int y, int w, int h)
{
    if (! fltkcurses_fullscreen)
    {
        fltkcurses_oldpixw = w;
        fltkcurses_oldpixh = h;
        fltkcurses_oldleft = x;
        fltkcurses_oldtop = y;
    }
    if (fltkcurses_ready
        &&
        ((fltkcurses_w != ((int) ((w - fltkcurses_pad) / fltkcurses_charw)))
         ||
         (fltkcurses_h != ((int) ((h - fltkcurses_pad) / fltkcurses_charh)))))
    {
        int rows, cols;
        fltkcurses_textDisplay->xy_to_rowcol(w,
                                             h,
                                             &rows,
                                             &cols,
                                             Fltkcurses_Text_Display::CHARACTER_POS);
        fltkcurses_textDisplay->resize(0, 0,
                                       ((int) ((w - fltkcurses_pad) / fltkcurses_charw + 1)) * FLTKCURSES_WFUDGE * fltkcurses_charw + fltkcurses_pad,
                                       ((int) ((h - fltkcurses_pad) / fltkcurses_charh + 1)) * fltkcurses_charh + fltkcurses_pad);
        if ((fltkcurses_w != cols)
            ||
            (fltkcurses_h != ((int) ((h - fltkcurses_pad) / fltkcurses_charh))))
        {
            fltkcurses_pixw = w;
            fltkcurses_pixh = h;
            fltkcurses_ungetch(KEY_RESIZE);
            fltkcurses_ready = 0;
        }
    }
#ifndef FLTK2CURSES
    Fl_Window::resize(x, y, w, h);
#endif
}

static void fltkcurses_styleUnfinished(int pos, void *cbarg)
{
}

static int init_color(short i, short r, short g, short b);

static int init_pair(short i, short fg, short bg);

static int pair_content(short i, short *fg, short *bg);

static void fltkcurses_updateStyle(int pos, int nInserted, int nDeleted, int nRestyled, const char *deletedText, void *cbarg)
{
    if ((! nInserted) && (! nDeleted))
    {
        fltkcurses_styleBuffer->unselect();
        return;
    }
    if (nInserted)
    {
        char *newStyle = new char[nInserted + 1];
        memset((void *) newStyle, fltkcurses_curattr, nInserted);
        newStyle[nInserted] = '\0';
        fltkcurses_styleBuffer->replace(pos, pos + nDeleted, newStyle);
        delete[] newStyle;
    }
    else
    {
        fltkcurses_styleBuffer->remove(pos, pos + nDeleted);
    }
    fltkcurses_styleBuffer->select(pos, pos + nInserted - nDeleted);
}

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    int i;
    int fontsize;
    int resizing = 0;
    int xchrome = 8, ychrome = 20;
    Fl_Font face = FL_COURIER;

    if (fltkcurses_pixw || fltkcurses_pixh)
    {
        fltkcurses_w = (int) ((fltkcurses_pixw - fltkcurses_pad) / fltkcurses_charw);
        fltkcurses_h = (int) ((fltkcurses_pixh - fltkcurses_pad) / fltkcurses_charh);
        fltkcurses_pixw = 0;
        fltkcurses_pixh = 0;
        resizing = 1;
        goto done;
    }
    fltkcurses_argc = 1;
    if (fltkcurses_title) free((void *) fltkcurses_title);
    fltkcurses_title = strdup(title);
    if (fltkcurses_argv[0]) free((void *) fltkcurses_argv[0]);
    fltkcurses_argv[0] = strdup(shortname);
    fltkcurses_argv[1] = 0;
    for (i = 0; i < (int) (sizeof(fltkcurses_getch_buffer)/sizeof(*fltkcurses_getch_buffer)); i ++)
    {
        fltkcurses_getch_buffer[i] = ERR;
    }
    fltkcurses_x = 0;
    fltkcurses_y = 0;
    fontsize = FL_NORMAL_SIZE;
#ifdef FLTKCURSES_FONT
    if (FLTKCURSES_FONT && *(FLTKCURSES_FONT))
    {
#ifdef FLTK2CURSES
        Fl_Font oldface = face;

        face = fltk::font(FLTKCURSES_FONT);
        if (! face) face = oldface;
#else
        Fl::set_font(face, FLTKCURSES_FONT);
#endif
    }
#endif
    if (myman_getenv("FLTKCURSES_FONT") && *(myman_getenv("FLTKCURSES_FONT")))
    {
#ifdef FLTK2CURSES
        Fl_Font oldface = face;

        face = fltk::font(myman_getenv("FLTKCURSES_FONT"));
        if (! face) face = oldface;
#else
        Fl::set_font(face, myman_getenv("FLTKCURSES_FONT"));
#endif
    }
    if (myman_getenv("FLTKCURSES_FONTSIZE") && atoi(myman_getenv("FLTKCURSES_FONTSIZE")))
    {
        fontsize = atoi(myman_getenv("FLTKCURSES_FONTSIZE"));
    }
    fl_font(face, fontsize);
    fltkcurses_charh = fl_height();
    if (fontsize > fltkcurses_charh) fltkcurses_charh = fontsize;
    fltkcurses_charw = fl_width(' ');
    for (i = ' '; i <= '~'; i ++)
    {
        int x, y;
        char buf[FLTKCURSES_MB_LEN_MAX];
        float wid;

        memset((void *) buf, '\0', sizeof(buf));
        buf[0] = i;
        fl_font(face, fontsize);
        wid = fl_width(buf);
        if (wid > fltkcurses_charw) fltkcurses_charw = wid;
        fl_measure(buf, x, y);
        if (x > fltkcurses_charw) fltkcurses_charw = x;
        if (y > fltkcurses_charh) fltkcurses_charh = y;
    }
    if (fltkcurses_charw <= 0.0)
    {
        fltkcurses_charw = fltkcurses_charh / 2.0;
    }
    if (fltkcurses_charw <= 0.0)
    {
        fltkcurses_charw = 1.0;
    }
    if (! h) h = 25;
    if (! w) w = 80;
#ifdef FLTK2CURSES
    {
        fltk::Rectangle chrome;
        fltk::Window *tmpwin = new fltk::Window(-100, -100, 1, 1);
        tmpwin->borders(&chrome);
        delete tmpwin;
        tmpwin = 0;
        xchrome = chrome.w() - chrome.x();
        ychrome = chrome.h() - chrome.y();
    }
#endif
    if ((w * fltkcurses_charw + fltkcurses_pad2 + xchrome) > Fl::w()) w = (Fl::w() - fltkcurses_pad2 - xchrome) / fltkcurses_charw;
    if ((h * fltkcurses_charh + fltkcurses_pad2 + ychrome) > Fl::h()) h = (Fl::h() - fltkcurses_pad2 - ychrome) / fltkcurses_charh;
    if (w < 1) w = 1;
    if (h < 1) h = 1;
    fltkcurses_w = w;
    fltkcurses_h = h;
    fltkcurses_window = new Fltkcurses_Window((int) (fltkcurses_w * fltkcurses_charw + fltkcurses_pad2),
                                              (int) (fltkcurses_h * fltkcurses_charh + fltkcurses_pad2),
                                              fltkcurses_title);
    fltkcurses_window->size_range((int) (fltkcurses_charw + fltkcurses_pad2),
                                  (int) (fltkcurses_charh + fltkcurses_pad2),
                                  0, 0,
                                  (int) fltkcurses_charw, (int) fltkcurses_charw);
    fltkcurses_window->cursor(FL_CURSOR_NONE);
    fltkcurses_textBuffer = new Fl_Text_Buffer();
    fltkcurses_styleBuffer = new Fl_Text_Buffer();
    fltkcurses_textDisplay = new Fltkcurses_Text_Display(
        0, 0,
        (int) ((fltkcurses_w + 1) * FLTKCURSES_WFUDGE * fltkcurses_charw + fltkcurses_pad),
        (int) ((fltkcurses_h + 1) * fltkcurses_charh + fltkcurses_pad));
    fltkcurses_textDisplay->box(FL_NO_BOX);
    fltkcurses_textDisplay->scrollbar_width(1);
    fltkcurses_textDisplay->scrollbar_align(FL_ALIGN_RIGHT_BOTTOM);
    fltkcurses_textDisplay->buffer(fltkcurses_textBuffer);
    for (i = 0; i < (int) (sizeof(fltkcurses_palette)/sizeof(*fltkcurses_palette)); i ++)
    {
        fltkcurses_palette[i].font = face;
        fltkcurses_palette[i].size = fontsize;
        fltkcurses_palette[i].attr = 0;
    }
    fltkcurses_textDisplay->highlight_data(fltkcurses_styleBuffer,
                                           fltkcurses_palette, sizeof(fltkcurses_palette)/sizeof(*fltkcurses_palette),
                                           'A', fltkcurses_styleUnfinished, 0);
    fltkcurses_textBuffer->add_modify_callback(fltkcurses_updateStyle, 0);
    fltkcurses_textDisplay->textfont(face);
    fltkcurses_textDisplay->textsize(fontsize);
    fltkcurses_textDisplay->hide_cursor();
    fltkcurses_textDisplay->color(FL_BLACK);
    fltkcurses_textDisplay->textcolor(FL_WHITE);
    fltkcurses_textDisplay->wrap_mode(0, 0);
    fltkcurses_window->end();
    fltkcurses_window->show(fltkcurses_argc, fltkcurses_argv);
    for (i = 0; i < (int) (sizeof(fltkcurses_palette)/sizeof(*fltkcurses_palette)); i ++)
    {
        init_color(i,
                   1000 * ((i == PEN_BRIGHT) ? 0x7777U : (i == COLOR_WHITE) ? 0xaaaaU : (i & COLOR_RED) ? ((i & PEN_BRIGHT) ? 0xffffU : 0x8888U) : 0) / 0xffffU,
                   1000 * ((i == PEN_BRIGHT) ? 0x7777U : (i == COLOR_WHITE) ? 0xaaaaU : (i & COLOR_GREEN) ? ((i & PEN_BRIGHT) ? 0xffffU : 0x8888U) : 0) / 0xffffU,
                   1000 * ((i == PEN_BRIGHT) ? 0x7777U : (i == COLOR_WHITE) ? 0xaaaaU : (i & COLOR_BLUE) ? ((i & PEN_BRIGHT) ? 0xffffU : 0x8888U) : 0) / 0xffffU);
    }
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
  done:
    fltkcurses_attr = 0;
    fltkcurses_curattr = 'A';
    fltkcurses_textDisplay->insert_position(0);
    fltkcurses_textBuffer->text("");
    fltkcurses_styleBuffer->text("");
    {
        char *blankScreen = new char[(fltkcurses_w + 1) * fltkcurses_h + 1];
        for (i = 0; i < fltkcurses_h; i ++)
        {
            memset((void *) (blankScreen + i * (fltkcurses_w + 1)), ' ', fltkcurses_w);
            blankScreen[fltkcurses_w + i * (fltkcurses_w + 1)] = '\n';
        }
        blankScreen[(fltkcurses_w + 1) * fltkcurses_h + 1] = '\0';
        fltkcurses_textDisplay->insert(blankScreen);
        delete[] blankScreen;
    }
    int cols, rows;
    /* this seems to work better for calculating character height, but
     * worse for width */
    fltkcurses_textDisplay->xy_to_rowcol(fltkcurses_window->w(),
                                         fltkcurses_window->h(),
                                         &rows,
                                         &cols,
                                         Fltkcurses_Text_Display::CHARACTER_POS);
    if (fltkcurses_h < rows)
    {
        fltkcurses_charh = 1.0 * fltkcurses_window->h() / rows;
        if (resizing)
        {
            for (; fltkcurses_h < rows; fltkcurses_h ++)
            {
                Fl::check();
                char *blankLine = new char[fltkcurses_w + 2];
                memset((void *) blankLine, ' ', fltkcurses_w);
                blankLine[fltkcurses_w] = '\n';
                blankLine[fltkcurses_w + 1] = '\0';
                fltkcurses_textDisplay->insert(blankLine);
                delete[] blankLine;
            }
        }
        else
        {
            fltkcurses_window->resize(
                fltkcurses_window->x(),
                fltkcurses_window->y(),
                (int) (fltkcurses_w * fltkcurses_charw + fltkcurses_pad2),
                (int) (fltkcurses_h * fltkcurses_charh + fltkcurses_pad2));
            fltkcurses_window->size_range((int) (fltkcurses_charw + fltkcurses_pad2),
                                          (int) (fltkcurses_charh + fltkcurses_pad2),
                                          0, 0,
                                          (int) fltkcurses_charw, (int) fltkcurses_charw);
        }
    }
    fltkcurses_ready = 1;
}

static void endwin(void)
{
    if (fltkcurses_pixw || fltkcurses_pixh)
    {
        fltkcurses_w = (int) ((fltkcurses_pixw - fltkcurses_pad) / fltkcurses_charw);
        fltkcurses_h = (int) ((fltkcurses_pixh - fltkcurses_pad) / fltkcurses_charh);
        return;
    }
    if (fltkcurses_ready)
    {
        fltkcurses_ready = 0;
        fltkcurses_textBuffer = NULL;
        fltkcurses_styleBuffer = NULL;
        fltkcurses_textDisplay = NULL;
        delete fltkcurses_window;
        fltkcurses_window = NULL;
    }
}

static int move(int y, int x);

static int addch(const chtype ch);

static int erase(void)
{
    int i;

    if (! fltkcurses_ready)
    {
        return ERR;
    }
    fltkcurses_attr = 0;
    fltkcurses_curattr = 'A';
    fltkcurses_textDisplay->insert_position(0);
    fltkcurses_textBuffer->text("");
    fltkcurses_styleBuffer->text("");
    {
        char *blankScreen = new char[(fltkcurses_w + 1) * fltkcurses_h + 1];
        for (i = 0; i < fltkcurses_h; i ++)
        {
            memset((void *) (blankScreen + i * (fltkcurses_w + 1)), ' ', fltkcurses_w);
            blankScreen[fltkcurses_w + i * (fltkcurses_w + 1)] = '\n';
        }
        blankScreen[(fltkcurses_w + 1) * fltkcurses_h + 1] = '\0';
        fltkcurses_textDisplay->insert(blankScreen);
        delete[] blankScreen;
    }
    move(0, 0);
    return OK;
}

static int resizeterm(int y, int x)
{
    return OK;
}

static chtype getch(void) {
    chtype ret = ERR;
    int i, j;

    j = -1;
    for (i = 0; i < (int) (sizeof(fltkcurses_getch_buffer)/sizeof(*fltkcurses_getch_buffer)); i ++)
    {
        if (fltkcurses_getch_buffer[i] != (chtype) ERR)
        {
            j = i;
        }
    }
    if (j != -1)
    {
        ret = fltkcurses_getch_buffer[j];
        fltkcurses_getch_buffer[j] = (chtype) ERR;
    }
    else
    {
        if (! Fl::check())
        {
#ifdef FLTK2CURSES
            if (! fltk::Window::first())
#endif
            {
                fltkcurses_ready = 0;
                return (chtype) (unsigned long) (unsigned char) 'q';
            }
        }
        for (i = 0; i < (int) (sizeof(fltkcurses_getch_buffer)/sizeof(*fltkcurses_getch_buffer)); i ++)
        {
            if (fltkcurses_getch_buffer[i] != (chtype) ERR)
            {
                j = i;
            }
        }
        if (j != -1)
        {
            ret = fltkcurses_getch_buffer[j];
            fltkcurses_getch_buffer[j] = (chtype) ERR;
        }
    }
    return ret;
}

static int refresh(void)
{
    if (! fltkcurses_ready) return OK;
    fltkcurses_ungetch(getch());
    return OK;
}

static void standout()
{
    fltkcurses_attr = A_STANDOUT;
}

static void standend()
{
    fltkcurses_attr = 0;
}

static int attrset(int a)
{
    fltkcurses_attr = a;
    return OK;
}

static int move(int y, int x)
{
    fltkcurses_y = y;
    fltkcurses_x = x;
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

static int init_pair(short i, short fg, short bg)
{
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    fltkcurses_pairs[i].fg = fg;
    fltkcurses_pairs[i].bg = bg;
    return OK;
}

static int pair_content(short i, short *fg, short *bg)
{
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    *fg = fltkcurses_pairs[i].fg;
    *bg = fltkcurses_pairs[i].bg;
    return OK;
}

static int start_color(void)
{
    return OK;
}

static int init_color(short i, short r, short g, short b)
{
    if ((i < 0) || (i > COLORS)) return ERR;
    if (r < 0) r = 0;
    if (r > 1000) r = 1000;
    fltkcurses_colors[i].red = 255 * r / 1000;
    if (g < 0) g = 0;
    if (g > 1000) g = 1000;
    fltkcurses_colors[i].green = 255 * g / 1000;
    if (b < 0) b = 0;
    if (b > 1000) b = 1000;
    fltkcurses_colors[i].blue = 255 * b / 1000;
    fltkcurses_palette[i].color = Fl_Color(FL_FREE_COLOR + i);
    Fl::set_color(fltkcurses_palette[i].color,
                  fltkcurses_colors[i].red,
                  fltkcurses_colors[i].green,
                  fltkcurses_colors[i].blue);
    while ((Fl::get_color(fltkcurses_palette[i].color)
            !=
            Fl::get_color(FL_WHITE))
           &&
           (Fl::get_color(fl_contrast(fltkcurses_palette[i].color, FL_BLACK))
            ==
            Fl::get_color(FL_WHITE)))
    {
        fltkcurses_palette[i].color = fl_lighter(fltkcurses_palette[i].color);
    }

    return OK;
}

static int color_content(short i, short *r, short *g, short *b)
{
    if ((i < 0) || (i > COLORS)) return ERR;
    *r = 1000 * fltkcurses_colors[i].red / 255;
    *g = 1000 * fltkcurses_colors[i].green / 255;
    *b = 1000 * fltkcurses_colors[i].blue / 255;
    return OK;
}

static int addch(chtype ch)
{
    char buf[FLTKCURSES_MB_LEN_MAX];
    attr_t attr;
    short fg, bg;
    short fgg, bgg;
    int wcw;

    wcw = wcwidth(ch);
    attr = (fltkcurses_attr & ~(PAIR_NUMBER(ch) ? PAIR_NUMBER(fltkcurses_attr) : 0)) | (ch & ~A_CHARTEXT);
    fg =
        (attr & A_BOLD) ? COLOR_YELLOW :
        (attr & A_UNDERLINE) ? COLOR_BLUE :
        (attr & A_STANDOUT) ? COLOR_MAGENTA :
        COLOR_WHITE;
    bg = COLOR_BLACK;
    if (PAIR_NUMBER(attr))
    {
        pair_content(PAIR_NUMBER(attr), &fg, &bg);
    }
    if (attr & A_BOLD)
    {
        fg |= PEN_BRIGHT;
    }
    if (attr & (A_REVERSE | A_STANDOUT))
    {
        short tmp;

        tmp = fg;
        fg = bg;
        bg = tmp;
    }
    fgg = 299L * fltkcurses_colors[fg].red / 255 + 587L * fltkcurses_colors[fg].green / 255 + 114L * fltkcurses_colors[fg].blue / 255;
    bgg = 299L * fltkcurses_colors[bg].red / 255 + 587L * fltkcurses_colors[bg].green / 255 + 114L * fltkcurses_colors[bg].blue / 255;
    if ((fg == bg) && (wcw == 1))
    {
        ch = ACS_BLOCK;
        bg = COLOR_BLACK;
        bgg = 299L * fltkcurses_colors[bg].red / 255 + 587L * fltkcurses_colors[bg].green / 255 + 114L * fltkcurses_colors[bg].blue / 255;
    }
    if ((! fgg) && bgg && (wcw == 1))
    {
        switch (ch)
        {
        case FLTKCURSES__ACS_THALF:
            fg = bg;
            bg = COLOR_BLACK;
            ch = FLTKCURSES__ACS_BHALF;
            break;
        case FLTKCURSES__ACS_BHALF:
            fg = bg;
            bg = COLOR_BLACK;
            ch = FLTKCURSES__ACS_THALF;
            break;
        case FLTKCURSES__ACS_LHALF:
            fg = bg;
            bg = COLOR_BLACK;
            ch = FLTKCURSES__ACS_RHALF;
            break;
        case FLTKCURSES__ACS_RHALF:
            fg = bg;
            bg = COLOR_BLACK;
            ch = FLTKCURSES__ACS_LHALF;
            break;
        }
        fgg = 299L * fltkcurses_colors[fg].red / 255 + 587L * fltkcurses_colors[fg].green / 255 + 114L * fltkcurses_colors[fg].blue / 255;
        bgg = 299L * fltkcurses_colors[bg].red / 255 + 587L * fltkcurses_colors[bg].green / 255 + 114L * fltkcurses_colors[bg].blue / 255;
    }
    if ((((fltkcurses_x ^ fltkcurses_y) & 1) ? (fgg <= bgg) : (fgg > bgg)) && bgg && (wcw == 1))
    {
        switch (ch)
        {
        case FLTKCURSES__ACS_THALF:
        case FLTKCURSES__ACS_BHALF:
        case FLTKCURSES__ACS_LHALF:
        case FLTKCURSES__ACS_RHALF:
            fg = bg;
            bg = COLOR_BLACK;
            ch = ACS_BLOCK;
            fgg = 299L * fltkcurses_colors[fg].red / 255 + 587L * fltkcurses_colors[fg].green / 255 + 114L * fltkcurses_colors[fg].blue / 255;
            bgg = 299L * fltkcurses_colors[bg].red / 255 + 587L * fltkcurses_colors[bg].green / 255 + 114L * fltkcurses_colors[bg].blue / 255;
            break;
        }
    }
    if (fgg && bgg && (wcw == 1))
    {
        switch (ch)
        {
        case FLTKCURSES__ACS_THALF:
        case FLTKCURSES__ACS_BHALF:
        case FLTKCURSES__ACS_LHALF:
        case FLTKCURSES__ACS_RHALF:
            bg = COLOR_BLACK;
            ch = ACS_BLOCK;
            bgg = 299L * fltkcurses_colors[bg].red / 255 + 587L * fltkcurses_colors[bg].green / 255 + 114L * fltkcurses_colors[bg].blue / 255;
            break;
        }
    }
    if ((bg != COLOR_BLACK) && (wcw == 1))
    {
        if ((ch != ACS_BLOCK)
            &&
            (ch != 0x2588L)
            &&
            (ch != 0x25a0L))
        {
            if ((ch == ' ')
                ||
                (ch == 0)
                ||
                (ch == 0x00a0L)
                ||
                (ch == 0x3000L)
                ||
                (ch == 0x2008L))
            {
                ch = ACS_BLOCK;
            }
            fg = bg;
        }
        bg = COLOR_BLACK;
        fgg = 299L * fltkcurses_colors[fg].red / 255 + 587L * fltkcurses_colors[fg].green / 255 + 114L * fltkcurses_colors[fg].blue / 255;
        bgg = 299L * fltkcurses_colors[bg].red / 255 + 587L * fltkcurses_colors[bg].green / 255 + 114L * fltkcurses_colors[bg].blue / 255;
    }
    fltkcurses_curattr = 'A' + (fg);
    ch &= A_CHARTEXT;
    if (! fltkcurses_ready) return ERR;
    if (ch == (chtype) (unsigned long) (unsigned char) '\a')
    {
        fl_beep();
        return OK;
    }
    else if (ch == (chtype) (unsigned long) (unsigned char) '\b')
    {
        if (fltkcurses_y && ! fltkcurses_x)
        {
            fltkcurses_y --;
        }
        fltkcurses_x += fltkcurses_w - 1;
        fltkcurses_x %= fltkcurses_w;
        return OK;
    }
    else if (ch == (chtype) (unsigned long) (unsigned char) '\n')
    {
        fltkcurses_y ++;
        fltkcurses_x = 0;
        return OK;
    }
    else if (ch == (chtype) (unsigned long) (unsigned char) '\t')
    {
        do
        {
            if (addch((chtype) (unsigned long) (unsigned char) ' ') == ERR)
            {
                return ERR;
            }
        }
        while (fltkcurses_x % 8);
        return OK;
    }
    else if (wcw < 1)
    {
        return ERR;
    }
    if (fltkcurses_x < 0) return ERR;
    if (fltkcurses_y < 0) return ERR;
    if (fltkcurses_x >= fltkcurses_w) return ERR;
    if (fltkcurses_y >= fltkcurses_h) return ERR;
    if (! fltkcurses_chtypetombs(ch, buf))
    {
        switch((ch & FLTKCURSES__A_ALTCHARSET) ? ch : (ch | FLTKCURSES__A_ALTCHARSET))
        {
        case ACS_LRCORNER:
        case ACS_URCORNER:
        case ACS_ULCORNER:
        case ACS_LLCORNER:
        case ACS_LTEE:
        case ACS_RTEE:
        case ACS_BTEE:
        case ACS_TTEE:
        case ACS_PLUS:
            fltkcurses_chtypetombs('+', buf);
            break;
        case ACS_HLINE:
            fltkcurses_chtypetombs('-', buf);
            break;
        case ACS_VLINE:
            fltkcurses_chtypetombs('|', buf);
            break;
        case ACS_BULLET:
            fltkcurses_chtypetombs('.', buf);
            break;
        case ACS_BLOCK:
            fltkcurses_chtypetombs('#', buf);
            break;
        default:
            fltkcurses_chtypetombs('*', buf);
        }
    }
    if (strlen(buf))
    {
        fltkcurses_textDisplay->insert_position(0);
        for (int voff = fltkcurses_y; voff; voff --)
        {
            fltkcurses_textDisplay->move_down();
        }
        for (int hoff = fltkcurses_x; hoff; hoff --)
        {
            fltkcurses_textDisplay->move_right();
        }
        int pos = fltkcurses_textDisplay->insert_position();
        char *tmp = fltkcurses_textBuffer->text_range(pos, pos + strlen(buf));
        if (strcmp(tmp, buf)
            ||
            (fltkcurses_curattr != fltkcurses_styleBuffer->character(pos)))
        {
            fltkcurses_textDisplay->overstrike(buf);
        }
        free((void *) tmp);
    }
    fltkcurses_x ++;
    return OK;
}

static int clrtobot(void)
{
    int x, y;

    for (x = fltkcurses_x; x < COLS; x ++)
    {
        move(fltkcurses_y, x);
        addch(' ');
    }
    for (y = fltkcurses_y + 1; y < LINES; y ++)
        for (x = 0; x < COLS; x ++)
        {
            move(y, x);
            addch(' ');
        }
    return OK;
}

static int clrtoeol(void)
{
    int x;

    for (x = fltkcurses_x; x < COLS; x ++)
    {
        move(fltkcurses_y, x);
        addch(' ');
    }
    return OK;
}

static int addstr(const char *s)
{
    while (*s)
    {
        addch((chtype) (unsigned long) (unsigned char) *s++);
    }
    return OK;
}

static int mvprintw(int y, int x, const char *s)
{
    move(y, x);
    return addstr(s);
}

