/*
 * newtcurs.h - newt driver for the MyMan video game
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

#include <newt.h>

/* work-arounds for newt */

#undef usleep
#define usleep newtDelay

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
#define wcwidth(ch) newtcurses_wcwidth(ch)

static int newtcurses_ready = 0;

typedef unsigned long int chtype;

typedef chtype attr_t;

static unsigned long newtcurses_x = 0, newtcurses_y = 0, newtcurses_w = 0, newtcurses_h = 0;
static unsigned long newtcurses_maxw = 0, newtcurses_maxh = 0;

static attr_t newtcurses_attr = 0;

static newtComponent newtcurses_mainform = NULL;
static newtComponent *newtcurses_standouts = NULL;
static struct newtColors newtcurses_palette;
static chtype newtcurses_getch_buffer[] = { ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR };

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

#define getyx(stdscr, y, x) ((y) = newtcurses_y), ((x) = newtcurses_x)

#define wrefresh(stdscr) refresh()

#define beep() addch('\a')

#define use_env(x)

#define echo()

#define curs_set(x)

#define keypad(stdscr, x)

#define LINES (newtcurses_h)

#define COLS (newtcurses_w)

#define KEY_UP NEWT_KEY_UP

#define KEY_DOWN NEWT_KEY_DOWN

#define KEY_LEFT NEWT_KEY_LEFT

#define KEY_RIGHT NEWT_KEY_RIGHT

#define KEY_RESIZE NEWT_KEY_RESIZE

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

#define INITSCR_WITH_HINTS

#ifndef NEED_LOCALE_IS_UTF8
#define NEED_LOCALE_IS_UTF8 1
#endif
static int locale_is_utf8(void);

static int newtcurses_wcwidth(int ch)
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
    case ACS_BULLET:
        return 1;
    }
    if (locale_is_utf8() && (ch > 0x7f))
    {
        return 0;
    }
    return ((((ch) >= ' ') && ((ch) == (chtype) (unsigned long) (unsigned char) (char) ch)) ? 1 : 0);
}

static void newtcurses_suspend(void *d)
{
    newtSuspend();
    raise(SIGTSTP);
    newtResume();
}

static void newtcurses_ungetch(chtype ch)
{
    int i;

    for (i = 0; i < (sizeof(newtcurses_getch_buffer)/sizeof(*newtcurses_getch_buffer)); i ++)
    {
        if (newtcurses_getch_buffer[i] == ERR)
        {
            newtcurses_getch_buffer[i] = ch;
            return;
        }
    }
    newtBell();
}

static void newtcurses_help(newtComponent co, void *tag)
{
    newtcurses_ungetch((chtype) (unsigned long) (unsigned char) 'h');
}

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    int i;

    for (i = 0; i < (sizeof(newtcurses_getch_buffer)/sizeof(*newtcurses_getch_buffer)); i ++)
    {
        newtcurses_getch_buffer[i] = ERR;
    }
    if (w > 128) w = 128;
    if (h > 50) h = 50;
    newtInit();
    newtPushHelpLine(NULL);
    memcpy((void *) &newtcurses_palette, (void *) &newtDefaultColorPalette, sizeof(newtcurses_palette));
    newtcurses_palette.helpLineFg = (newtcurses_palette.rootFg = (newtcurses_palette.rootTextFg = "white"));
    newtcurses_palette.helpLineBg = (newtcurses_palette.rootBg = (newtcurses_palette.rootTextBg = "black"));
    newtcurses_palette.labelFg = "black";
    newtcurses_palette.labelBg = "lightgray";
    newtSetColors(newtcurses_palette);
    newtCls();
    newtGetScreenSize(&w, &h);
    newtcurses_w = w;
    newtcurses_h = h;
    newtcurses_x = 0;
    newtcurses_y = 0;
    newtcurses_attr = 0;
    newtSetSuspendCallback(newtcurses_suspend, NULL);
    newtSetHelpCallback(newtcurses_help);
    newtcurses_mainform = newtForm(NULL, "", 0);
    newtFormSetBackground(newtcurses_mainform, NEWT_COLORSET_ROOT);
    for (i = 0; i < 0x80; i ++)
    {
        newtFormAddHotKey(newtcurses_mainform, i);
    }
    newtFormAddHotKey(newtcurses_mainform, NEWT_KEY_UP);
    newtFormAddHotKey(newtcurses_mainform, NEWT_KEY_DOWN);
    newtFormAddHotKey(newtcurses_mainform, NEWT_KEY_LEFT);
    newtFormAddHotKey(newtcurses_mainform, NEWT_KEY_RIGHT);
    newtFormAddHotKey(newtcurses_mainform, NEWT_KEY_BKSPC);
    newtFormAddHotKey(newtcurses_mainform, NEWT_KEY_DELETE);
    newtFormAddHotKey(newtcurses_mainform, NEWT_KEY_F12);
    newtFormAddHotKey(newtcurses_mainform, NEWT_KEY_RESIZE);
    newtcurses_maxw = newtcurses_w;
    newtcurses_maxh = newtcurses_h;
    newtcurses_standouts = (newtComponent *) malloc(newtcurses_maxw * newtcurses_maxh * sizeof(newtComponent));
    if (newtcurses_standouts)
    {
        for (i = 0; i < (newtcurses_w * newtcurses_h); i ++)
        {
            newtcurses_standouts[i] = NULL;
        }
    }
    newtDrawForm(newtcurses_mainform);
    newtRefresh();
    newtFormSetTimer(newtcurses_mainform, 1);
    newtcurses_ready = 1;
}

static void endwin(void)
{
    newtcurses_ready = 0;
    newtFormDestroy(newtcurses_mainform);
    if (newtcurses_standouts)
    {
        free((void *) newtcurses_standouts);
        newtcurses_standouts = NULL;
    }
    newtcurses_mainform = NULL;
    newtPopHelpLine();
    newtFinished();
}

static int move(int y, int x);

static int addch(const chtype ch);

static int erase(void)
{
    int x, y;

    newtcurses_attr = 0;
    for (y = 0; y < LINES; y ++)
        for (x = 0; x < COLS; x ++)
        {
            move(y, x);
            addch((chtype) (unsigned long) (unsigned char) ' ');
        }
    move(0, 0);
    return OK;
}

static int resizeterm(int y, int x)
{
    newtResizeScreen(1);
    newtGetScreenSize(&x, &y);
    if (newtcurses_standouts
        &&
        ((y > newtcurses_maxh)
         ||
         (x > newtcurses_maxw)))
    {
        int i, j;
        newtComponent *newstandouts;

        newstandouts = (newtComponent *) realloc((void *) newtcurses_standouts,
                                        ((x > newtcurses_maxw) ? x : newtcurses_maxw)
                                        *
                                        ((y > newtcurses_maxh) ? y : newtcurses_maxh)
                                        *
                                        sizeof(newtComponent));
        if (newstandouts)
        {
            /* rearrange to support new geometry */
            newtcurses_standouts = newstandouts;
            for (i = ((y > newtcurses_maxh) ? y : newtcurses_maxh); i; i ++)
                for (j = ((x > newtcurses_maxw) ? x : newtcurses_maxw); j; j ++)
                {
                    newtComponent co;

                    co = NULL;
                    if ((i < newtcurses_maxh) && (j < newtcurses_maxw))
                    {
                        co = newtcurses_standouts[i * newtcurses_maxw + j];
                    }
                    newtcurses_standouts[i * ((x > newtcurses_maxw) ? x : newtcurses_maxw) + j] = co;
                }
        }
        else
        {
            /* screen got too big, stop supporting standouts */
            for (i = 0; i < (newtcurses_maxw * newtcurses_maxh); i ++)
            {
                if (newtcurses_standouts[i])
                {
                    newtLabelSetText(newtcurses_standouts[i], "");
                }
            }
            free((void *) newtcurses_standouts);
            newtcurses_standouts = NULL;
        }
    }
    newtcurses_w = x;
    newtcurses_h = y;
    if (newtcurses_w > newtcurses_maxw) newtcurses_maxw = newtcurses_w;
    if (newtcurses_h > newtcurses_maxh) newtcurses_maxh = newtcurses_h;
    return OK;
}

static chtype getch(void) {
    chtype ret = ERR;
    int i, j;

    newtGetScreenSize(&i, &j);
    if ((i != newtcurses_w) || (j != newtcurses_h))
    {
        newtResizeScreen(1);
        return KEY_RESIZE;
    }
    j = -1;
    for (i = 0; i < (sizeof(newtcurses_getch_buffer)/sizeof(*newtcurses_getch_buffer)); i ++)
    {
        if (newtcurses_getch_buffer[i] != ERR)
        {
            j = i;
        }
    }
    if (j != -1)
    {
        if (j == KEY_RESIZE)
        {
            newtResizeScreen(1);
        }
        ret = newtcurses_getch_buffer[j];
        newtcurses_getch_buffer[j] = ERR;
    }
    else
    {
        struct newtExitStruct ev;

        newtRefresh();
        newtFormRun(newtcurses_mainform, &ev);
        /*fprintf(stderr, "ev.reason: %d\n", ev.reason); fflush(stderr);*/
        if (ev.reason == NEWT_EXIT_HOTKEY)
        {
            /*fprintf(stderr, "ev.u.key: %x\n", ev.u.key); fflush(stderr);*/
            ret = ev.u.key;
            if (! ret)
            {
                ret = (chtype) ERR;
            }
            else if (ret == NEWT_KEY_F12)
            {
                ret = (int) (unsigned int) (unsigned char) 'q';
            }
            else if (ret == NEWT_KEY_BKSPC)
            {
                ret = (int) (unsigned int) (unsigned char) '\b';
            }
            else if (ret == NEWT_KEY_DELETE)
            {
                ret = (int) (unsigned int) (unsigned char) '\177';
            }
        }
        else if (ev.reason == NEWT_EXIT_COMPONENT)
        {
            ret = (chtype) (unsigned long) (unsigned char) 'q';
        }
    }
    return ret;
}

static int refresh(void)
{
    if (! newtcurses_ready) return OK;
    newtcurses_ungetch(getch());
    return OK;
}

static void standout()
{
    newtcurses_attr = A_STANDOUT;
}

static void standend()
{
    newtcurses_attr = 0;
}

static int attrset(int a)
{
    newtcurses_attr = a;
    return OK;
}

static int move(int y, int x)
{
    newtcurses_y = y;
    newtcurses_x = x;
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

static int init_color(short i, short r, short g, short b)
{
    return ERR;
}

static int addch(const chtype ch)
{
    char buf[2];

    memset((void *) buf, '\0', sizeof(buf));
    if (ch == (chtype) (unsigned long) (unsigned char) '\a')
    {
        newtBell();
        return OK;
    }
    else if (ch == (chtype) (unsigned long) (unsigned char) '\b')
    {
        if (newtcurses_y && ! newtcurses_x)
        {
            newtcurses_y --;
        }
        newtcurses_x += newtcurses_w - 1;
        newtcurses_x %= newtcurses_w;
        return OK;
    }
    else if (ch == (chtype) (unsigned long) (unsigned char) '\n')
    {
        newtcurses_y ++;
        newtcurses_x = 0;
        return OK;
    }
    else if (ch == (chtype) (unsigned long) (unsigned char) '\t')
    {
        do
        {
            addch((chtype) (unsigned long) (unsigned char) ' ');
        }
        while (newtcurses_x % 8);
        return OK;
    }
    else if (wcwidth(ch) < 1)
    {
        return ERR;
    }
    if (newtcurses_x < 0) return ERR;
    if (newtcurses_y < 0) return ERR;
    if (newtcurses_x >= newtcurses_w) return ERR;
    if (newtcurses_y >= newtcurses_h) return ERR;
    if (ch >= (locale_is_utf8() ? 0x80 : 0x100))
    {
        switch(ch)
        {
        case ACS_LRCORNER:
        case ACS_URCORNER:
        case ACS_ULCORNER:
        case ACS_LLCORNER:
        case ACS_PLUS:
            buf[0] = '+';
            break;
        case ACS_HLINE:
            buf[0] = '-';
            break;
        case ACS_LTEE:
        case ACS_RTEE:
        case ACS_BTEE:
        case ACS_TTEE:
            buf[0] = '+';
            break;
        case ACS_VLINE:
            buf[0] = '|';
            break;
        case ACS_BULLET:
            buf[0] = '.';
            break;
        default:
            buf[0] = '*';
        }
    }
    else
    {
        buf[0] = ch;
    }
    if (newtcurses_standouts
        &&
        newtcurses_standouts[newtcurses_y * newtcurses_maxw + newtcurses_x])
    {
        newtLabelSetText(newtcurses_standouts[newtcurses_y * newtcurses_maxw + newtcurses_x], "");
    }
    if ((newtcurses_attr & A_STANDOUT) && newtcurses_standouts)
    {
        if (! newtcurses_standouts[newtcurses_y * newtcurses_maxw + newtcurses_x])
        {
            newtComponent co;

            co = newtLabel(newtcurses_x, newtcurses_y, "");
            if (co)
            {
                newtcurses_standouts[newtcurses_y * newtcurses_maxw + newtcurses_x] = co;
                newtFormAddComponent(newtcurses_mainform, co);
            }
        }
        if (! newtcurses_standouts[newtcurses_y * newtcurses_maxw + newtcurses_x])
        {
            int i;

            /* too much standouting, remove all standouts */
            for (i = 0; i < (newtcurses_maxw * newtcurses_maxh); i ++)
            {
                if (newtcurses_standouts[i])
                {
                    newtLabelSetText(newtcurses_standouts[i], "");
                }
            }
            free((void *) newtcurses_standouts);
            newtcurses_standouts = NULL;
        }
        if (newtcurses_standouts && newtcurses_standouts[newtcurses_y * newtcurses_maxw + newtcurses_x])
        {
            newtLabelSetText(newtcurses_standouts[newtcurses_y * newtcurses_maxw + newtcurses_x], buf);
            newtcurses_x ++;
            return OK;
        }
    }
    newtDrawRootText(newtcurses_x, newtcurses_y, buf);
    newtcurses_x ++;
    return OK;
}

static int clrtobot(void)
{
    unsigned int x, y;

    for (x = newtcurses_x; x < COLS; x ++)
    {
        move(newtcurses_y, x);
        addch(' ');
    }
    for (y = newtcurses_y + 1; y < LINES; y ++)
        for (x = 0; x < COLS; x ++)
        {
            move(y, x);
            addch(' ');
        }
    return OK;
}

static int clrtoeol(void)
{
    unsigned int x;

    for (x = newtcurses_x; x < COLS; x ++)
    {
        move(newtcurses_y, x);
        addch(' ');
    }
    return OK;
}

static int addstr(const char *s)
{
    while (*s)
    {
        addch(*s++);
    }
    return OK;
}

static int mvprintw(int y, int x, const char *s)
{
    move(y, x);
    return addstr(s);
}

