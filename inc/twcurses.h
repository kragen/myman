/*
 * twcurses.c - Twin driver for the MyMan video game
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

#include <Tw/Tw.h>
#include <Tw/Twkeys.h>

/* libTw 2.0.0+ */

#ifndef USE_ATTR
#define USE_ATTR 1
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

static int twcurses_ready = 0;

struct twcurses_cell {
    byte ch;
    hwcol attr;
};

typedef struct twcurses_cell twcurses_cell_t;

static int twcurses_clearok;

static twcurses_cell_t *twcurses_backing;

static hwcol twcurses_attr;

static tdisplay twcurses_display;

static twindow twcurses_window;

static tmenu twcurses_menu;

static tmsgport twcurses_msgport;

typedef unsigned long int chtype;

static int twcurses_x, twcurses_y, twcurses_w, twcurses_h;

#ifndef OK
#define OK (0)
#endif

#ifndef ERR
#define ERR (-1)
#endif

#define clear erase

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

#define ACS_BULLET 0xf9

#define cbreak()

#define noecho()

#define nonl()

#define nodelay(stdscr, x)

#define intrflush(stdscr, x)

#define immedok(stdscr, x)

#define idlok(stdscr, x)

#define clearok(stdscr, x) (x ? (twcurses_clearok = 1) : 0)

#define leaveok(stdscr, x)

#define getyx(stdscr, y, x) ((y) = twcurses_y), ((x) = twcurses_x)

#define wrefresh(stdscr) refresh()

#define beep() addch('\7')

#define use_env(x)

#define echo()

#define curs_set(x)

#define keypad(stdscr, x)

#define can_change_color() 1

#define has_colors() 1

#define start_color()

#define LINES (twcurses_h)

#define COLS (twcurses_w)

#define COLOR_BLACK BLACK

#define COLOR_BLUE BLUE

#define COLOR_GREEN GREEN

#define COLOR_CYAN CYAN

#define COLOR_RED RED

#define COLOR_MAGENTA MAGENTA

#define COLOR_YELLOW YELLOW

#define COLOR_WHITE WHITE

#define COLORS 16

#define COLOR_PAIRS (COLORS * COLORS)

static chtype twcurses_pairs[COLOR_PAIRS];

#define A_BOLD 0x800000UL

#define A_STANDOUT 0x1000000UL

#define A_REVERSE 0x2000000UL

#define A_UNDERLINE 0x400000UL

#define A_COLOR 0x200000UL

#define COLOR_PAIR(i) (A_COLOR | ((chtype) (i)))

#define KEY_LEFT TW_Left

#define KEY_RIGHT TW_Right

#define KEY_UP TW_Up

#define KEY_DOWN TW_Down

#define INITSCR_WITH_HINTS

static void standout() {
    twcurses_attr |= A_STANDOUT;
}

static void standend() {
    twcurses_attr &= ~A_STANDOUT;
}

static int attrset(int a)
{
    short fg, bg;

    fg = WHITE;
    bg = BLACK;
    if (a & A_UNDERLINE)
    {
        fg = BLUE;
    }
    if (a & A_STANDOUT)
    {
        fg = HIGH | WHITE;
    }
    if (a & A_COLOR)
    {
        fg = COLFG(twcurses_pairs[((unsigned long) a) & 0xffUL]);
        bg = COLBG(twcurses_pairs[((unsigned long) a) & 0xffUL]);
    }
    if (a & A_BOLD)
    {
        fg |= HIGH;
    }
    if (a & (A_STANDOUT | A_REVERSE))
    {
        short tmp;

        tmp = fg;
        fg = bg;
        bg = tmp;
    }
    twcurses_attr = COL(fg, bg);
    return OK;
}

static void move(y, x) {
    twcurses_y = y;
    twcurses_x = x;
}

static int addch(const chtype ch) {
    twcurses_backing[twcurses_y * twcurses_w + twcurses_x].ch = ch & (chtype) 0x1fffffUL;
    twcurses_backing[twcurses_y * twcurses_w + twcurses_x].attr = twcurses_attr | (ch & ~0x1fffffUL);
    twcurses_x ++;
    return OK;
}

static int clrtobot(void) {
    unsigned int x, y;

    for (x = twcurses_x; x < COLS; x ++)
    {
        move(twcurses_y, x);
        addch(' ');
    }
    for (y = twcurses_y + 1; y < LINES; y ++)
        for (x = 0; x < COLS; x ++)
        {
            move(y, x);
            addch(' ');
        }
    return OK;
}

static int clrtoeol(void) {
    unsigned int x;

    for (x = twcurses_x; x < COLS; x ++)
    {
        move(twcurses_y, x);
        addch(' ');
    }
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

#define TWCURSES_TITLE "TwCurses Window"

#define TWCURSES_PROGNAME "twcurses"

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    byte *name;
    byte name_def[sizeof(TWCURSES_TITLE)];
    byte *progname;
    byte progname_def[sizeof(TWCURSES_PROGNAME)];
    hwcol *namecolors;
    hwcol namecolors_def[sizeof(TWCURSES_TITLE)];
    int i;

    name = 0;
    namecolors = 0;
    if (title)
    {
        name = (byte *) malloc(strlen(title) * sizeof(byte));
        namecolors = (byte *) malloc(strlen(title) * sizeof(hwcol));
    }
    if ((! name)
        ||
        (! namecolors))
    {
        name = name_def;
        namecolors = namecolors_def;
        title = TWCURSES_TITLE;
    }
    progname = 0;
    if (shortname)
    {
        progname = (byte *) malloc(strlen(shortname) * sizeof(byte));
    }
    if (! progname)
    {
        progname = progname_def;
        shortname = TWCURSES_PROGNAME;
    }
    twcurses_attr = 0;
    for (i = 0; i < strlen(title); i ++)
    {
        name[i] = title[i];
        namecolors[i] = COL(HIGH | YELLOW, BLACK);
    }
    for (i = 0; i < strlen(shortname); i ++)
    {
        progname[i] = shortname[i];
    }
    if (! (twcurses_display = Tw_Open(NULL)))
    {
        fprintf(stderr, "TwOpen failed\n");
        fflush(stderr);
        exit(1);
    }
    if (w > Tw_GetDisplayWidth(twcurses_display)) w = Tw_GetDisplayWidth(twcurses_display);
    if (h >= Tw_GetDisplayHeight(twcurses_display)) h = Tw_GetDisplayHeight(twcurses_display) - 1;
    twcurses_w = w;
    twcurses_h = h;
    if (! (twcurses_backing = (twcurses_cell_t *) malloc(2 * w * h * sizeof(twcurses_cell_t))))
    {
        perror("malloc");
        fflush(stderr);
        exit(1);
    }
    if (TW_NOID == (twcurses_msgport = Tw_CreateMsgPort(twcurses_display, strlen(shortname), progname)))
    {
        Tw_Close(twcurses_display);
        free(twcurses_backing);
        fprintf(stderr, "Tw_CreateMsgPort failed\n");
        fflush(stderr);
        exit(1);
    }
    if (TW_NOID == (twcurses_menu = Tw_CreateMenu(twcurses_display, COL(WHITE, BLACK), COL(HIGH | YELLOW, BLACK), COL(HIGH | BLACK, BLACK), COL(BLACK, HIGH | BLACK), COL(BLUE, BLACK), COL(BLACK, BLUE), 0)))
    {
        Tw_DeleteMsgPort(twcurses_display, twcurses_msgport);
        Tw_Close(twcurses_display);
        free(twcurses_backing);
        fprintf(stderr, "Tw_CreateMenu failed\n");
        fflush(stderr);
        exit(1);
    }
    if (TW_NOID == (twcurses_window = Tw_CreateWindow(twcurses_display, strlen(title), name, namecolors, twcurses_menu, COL(WHITE, BLACK), TW_NOCURSOR, TW_WINDOW_WANT_KEYS | TW_WINDOW_WANT_CHANGES | TW_WINDOW_RESIZE | TW_WINDOW_CLOSE | TW_WINDOW_DRAG, TW_WINDOWFL_USEROWS, twcurses_w, twcurses_h, 0)))
    {
        Tw_DeleteMenu(twcurses_display, twcurses_menu);
        Tw_DeleteMsgPort(twcurses_display, twcurses_msgport);
        Tw_Close(twcurses_display);
        free(twcurses_backing);
        fprintf(stderr, "Tw_CreateWindow failed\n");
        fflush(stderr);
        exit(1);
    }
    Tw_MapWindow(twcurses_display, twcurses_window, Tw_FirstScreen(twcurses_display));
    erase();
    twcurses_clearok = 1;
    twcurses_ready = 1;
}

static void endwin(void) {
    twcurses_ready = 0;
    Tw_DeleteMenu(twcurses_display, twcurses_menu);
    Tw_DeleteWindow(twcurses_display, twcurses_window);
    Tw_DeleteMsgPort(twcurses_display, twcurses_msgport);
    Tw_Close(twcurses_display);
    free(twcurses_backing);
}

static int refresh(void) {
    int x;
    int y;

    if (! twcurses_ready) return OK;
    for (y = 0; y < twcurses_h; y ++)
    {
        for (x = 0; x < twcurses_w; x ++)
        {
            byte ch;
            hwcol attr;
            
            attr = twcurses_backing[y * twcurses_w + x].attr;
            ch = twcurses_backing[y * twcurses_w + x].ch;
            if (twcurses_clearok
                ||
                (attr
                 &&
                 ((attr
                   !=
                   twcurses_backing[y * twcurses_w + x + twcurses_w * twcurses_h].attr)
                  ||
                  (ch
                   !=
                   twcurses_backing[y * twcurses_w + x + twcurses_w * twcurses_h].ch))))
            {
                byte uch;

                twcurses_backing[y * twcurses_w + x].attr = 0;
                twcurses_backing[y * twcurses_w + x + twcurses_w * twcurses_h].attr = attr;
                twcurses_backing[y * twcurses_w + x + twcurses_w * twcurses_h].ch = ch;
                Tw_GotoXYWindow(twcurses_display, twcurses_window, x, y);
                Tw_SetColTextWindow(twcurses_display, twcurses_window, attr);
                uch = ch;
                Tw_WriteAsciiWindow(twcurses_display, twcurses_window, 1, &uch);
            }
        }
    }
    twcurses_clearok = 0;
    Tw_Sync(twcurses_display);
    return OK;
}

static chtype getch(void) {
    tmsg msg;

    msg = Tw_ReadMsg(twcurses_display, FALSE);
    if (msg != (tmsg) 0)
    {
        switch ((udat) msg->Type)
        {
        case TW_MSG_WIDGET_KEY:
            return msg->Event.EventKeyboard.Code;
        case TW_MSG_WIDGET_CHANGE:
            if (msg->Event.EventWidget.Code == TW_MSG_WIDGET_RESIZE)
            {
                twcurses_cell_t *twcurses_backing_new;

                if (! (twcurses_backing_new = (twcurses_cell_t *) realloc((void *) twcurses_backing, 2 * msg->Event.EventWidget.XWidth * msg->Event.EventWidget.YWidth * sizeof(twcurses_cell_t))))
                {
                    perror("realloc");
                    fflush(stderr);
                }
                else
                {
                    twcurses_backing = twcurses_backing_new;
                    twcurses_w = msg->Event.EventWidget.XWidth;
                    twcurses_h = msg->Event.EventWidget.YWidth;
                    erase();
                    twcurses_clearok = 1;
                }
            }
            break;
        case TW_MSG_WIDGET_GADGET:
            if ((msg->Event.EventGadget.W == twcurses_window)
                &&
                (msg->Event.EventGadget.Code == 0))
            {
                return 'Q';
            }
            break;
        }
    }
    return ERR;
}

static int init_color(int i, short r, short g, short b) {
    return ERR;
}

static int color_content(int i, short *r, short *g, short *b) {
    return ERR;
}

static int init_pair(int i, short fg, short bg) {
    if ((i < 0) || (i >= COLOR_PAIRS)) return ERR;
    twcurses_pairs[i] = COL(fg, bg);
    return OK;
}

static int pair_content(int i, short *fg, short *bg) {
    if ((i < 0) || (i >= COLOR_PAIRS)) return ERR;
    *fg = COLFG(twcurses_pairs[i]);
    *bg = COLBG(twcurses_pairs[i]);
    return OK;
}
