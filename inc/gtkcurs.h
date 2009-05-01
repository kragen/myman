/*
 * gtkcurs.h - GTK+ driver for the MyMan video game
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

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

/* work-arounds for GTK+ */

#if GTK_MAJOR_VERSION >= 2

/* GTK+ 2.0 and up */
#define gtkcurses_loadIconFile(ic) gtk_window_set_default_icon_from_file((ic), NULL)
#define gtkcurses_connect(o,e,cb,d) g_signal_connect_object(G_OBJECT(o),(e),G_CALLBACK(cb),(d),0)
#define gtkcurses_idleAdd(cb,d) g_idle_add(cb,d)

/* GTK+ 2.0 and up use Pango by default; you may however override this
 * for the older ASCII-only GTK+ 1.x behavior */
#ifndef USE_PANGO
#define USE_PANGO 1
#endif

#else

/* GTK+ 1.x */
#define gtkcurses_loadIconFile(ic)
#define gtkcurses_connect(o,e,cb,d) gtk_signal_connect_object(GTK_OBJECT(o),(e),GTK_SIGNAL_FUNC(cb),(d))
#define gtkcurses_idleAdd(cb,d) gtk_idle_add(cb,d)

#endif

#ifndef USE_PANGO
#define USE_PANGO 0
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

#if defined(WIN32)

#include <windows.h>

#ifndef HAVE_GETTIMEOFDAY
#define HAVE_GETTIMEOFDAY 0
#endif

#ifndef HAVE_USLEEP
#define HAVE_USLEEP 0
#endif

#endif

#ifndef HAVE_GETTIMEOFDAY
#define HAVE_GETTIMEOFDAY 1
#endif

#ifndef HAVE_USLEEP
#define HAVE_USLEEP 1
#endif

#if ! HAVE_GETTIMEOFDAY

#undef gettimeofday
#define gettimeofday gtkcurses_gettimeofday

static int gtkcurses_gettimeofday(struct timeval *tv, void *tz)
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

#error no gettimeofday(2) implementation for your platform, sorry
    return 1;

#endif /* ! defined(__MSDOS__) */

#endif /* ! defined(WIN32) */

    return 0;
}

#endif /* ! HAVE_GETTIMEOFDAY */

#if ! HAVE_USLEEP

#if defined(WIN32)

#undef usleep

/* originally from http://wyw.dcweb.cn/sleep.h.txt by Wu Yongwei */

#define usleep(t) Sleep((t) / 1000)

#else /* ! defined(WIN32) */

#undef usleep
#define usleep gtkcurses_usleep

static int
gtkcurses_usleep(unsigned long usecs)
{
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
    return 0;
}

#endif /* ! defined(WIN32) */

#endif /* ! HAVE_USLEEP */

#if USE_ICONV
#include <iconv.h>
/* for uint32_t */
#include <stdint.h>

static iconv_t gtkcurses_cd_to_wchar = (iconv_t) -1;

static iconv_t gtkcurses_cd_to_uni = (iconv_t) -1;

static wchar_t gtkcurses_ucs_to_wchar(unsigned long ucs)
{
    uint32_t ucsbuf[2];
    wchar_t wcbuf[2];
    const char *ibuf;
    char *obuf;
    size_t ibl;
    size_t obl;
    const char *my_locale;

    if ((! (my_locale = setlocale(LC_CTYPE, "")))
        ||
        (! *my_locale)
        ||
        (! strcmp(my_locale, "C"))
        ||
        (! strcmp(my_locale, "POSIX")))
    {
        wcbuf[0] = 0;
        goto done;
    }
    if ((gtkcurses_cd_to_wchar == (iconv_t) -1)
        &&
        ((gtkcurses_cd_to_wchar = iconv_open("wchar_t//IGNORE", "UCS-4-INTERNAL")) == (iconv_t) -1))
    {
        wcbuf[0] = 0;
        goto done;
    }
    ucsbuf[0] = ucs;
    ucsbuf[1] = 0;
    wcbuf[0] = 0;
    wcbuf[1] = 0;
    ibuf = (char *) (void *) ucsbuf;
    obuf = (char *) (void *) wcbuf;
    ibl = sizeof(ucsbuf);
    obl = sizeof(wcbuf);
    if ((! iconv(gtkcurses_cd_to_wchar, &ibuf, &ibl, &obuf, &obl))
        ||
        wcbuf[1]
        ||
        (! wcbuf[0]))
    {
        wcbuf[0] = 0;
        goto done;
    }
    if (gtkcurses_cd_to_uni == (iconv_t) -1)
    {
        gtkcurses_cd_to_uni = iconv_open("UCS-4-INTERNAL//IGNORE", "wchar_t");
    }
    ucsbuf[0] = 0;
    ibuf = (char *) (void *) wcbuf;
    obuf = (char *) (void *) ucsbuf;
    ibl = sizeof(wcbuf);
    obl = sizeof(ucsbuf);
    if ((gtkcurses_cd_to_uni != (iconv_t) -1)
        &&
        (iconv(gtkcurses_cd_to_uni, &ibuf, &ibl, &obuf, &obl))
        &&
        (ucsbuf[0] != ucs))
    {
        /* does not round-trip, probably a broken character */
        wcbuf[0] = 0;
        goto done;
    }
  done:
    if (my_locale)
    {
        setlocale(LC_CTYPE, my_locale);
    }
    return wcbuf[0] ? wcbuf[0] : (((ucs >= 0x20) && (ucs <= 0x7e)) ? ((wchar_t) ucs) : 0);
}

#else

#define gtkcurses_ucs_to_wchar(ucs) ((((unsigned long) (wchar_t) (unsigned long) (ucs)) == (unsigned long) (ucs)) ? ((wchar_t) (unsigned long) (ucs)) : ((wchar_t) 0))

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

#if ! USE_PANGO
#include <wchar.h>
static int gtkcurses_native_wcwidth(wchar_t wc)
{
    return wcwidth(wc);
}
#endif

#undef wcwidth
#define wcwidth(ch) gtkcurses_wcwidth(ch)

static int gtkcurses_init_done = 0;
static int gtkcurses_ready = 0;
static int gtkcurses_resize_pending = 0;
static int gtkcurses_mapped = 0;
static int gtkcurses_has_focus = 0;
static int gtkcurses_bitmap = 0;

typedef unsigned long int chtype;

typedef chtype attr_t;

static unsigned long gtkcurses_x = 0, gtkcurses_y = 0, gtkcurses_w = 0, gtkcurses_h = 0;
static unsigned long gtkcurses_charw = 0, gtkcurses_charh = 0;
static unsigned long gtkcurses_neww = 0, gtkcurses_newh = 0;
static unsigned long gtkcurses_pixwidth = 0, gtkcurses_pixheight = 0;

static attr_t gtkcurses_attr = 0;

static chtype gtkcurses_getch_buffer[] = { ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR };

#define clear erase

#define has_colors() (1)

#define can_change_color() (1)

#define cbreak()

#define noecho()

#define nonl()

#define nodelay(stdscr, x)

#define intrflush(stdscr, x)

#define immedok(stdscr, x)

#define idlok(stdscr, x)

#define clearok(stdscr, x)

#define leaveok(stdscr, x)

#define getyx(stdscr, y, x) ((y) = gtkcurses_y), ((x) = gtkcurses_x)

#define wrefresh(stdscr) refresh()

#define beep() addch('\a')

#define use_env(x)

#define echo()

#define curs_set(x)

#define keypad(stdscr, x)

#define LINES (gtkcurses_h)

#define COLS (gtkcurses_w)

#define KEY_UP GDK_Up

#define KEY_DOWN GDK_Down

#define KEY_LEFT GDK_Left

#define KEY_RIGHT GDK_Right

#define KEY_RESIZE 0x10fffdL

#define A_DIM 0x00200000

#define A_UNDERLINE A_DIM

#define A_BOLD 0x00400000

#define A_STANDOUT 0x00800000

#define A_REVERSE A_STANDOUT

#define GTKCURSES__A_COLOR 0xff000000

#define COLOR_PAIRS 256

#define COLORS 16

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

#define PAIR_NUMBER(a) (((a) & GTKCURSES__A_COLOR) >> _PAIR_SHIFT)

#define A_CHARTEXT ((chtype) 0x1fffffL)

#define GTKCURSES__A_ALTCHARSET 0x1f0000L

#define ACS_LRCORNER (GTKCURSES__A_ALTCHARSET | 0x2518L)

#define ACS_URCORNER (GTKCURSES__A_ALTCHARSET | 0x2510L)

#define ACS_ULCORNER (GTKCURSES__A_ALTCHARSET | 0x250cL)

#define ACS_LLCORNER (GTKCURSES__A_ALTCHARSET | 0x2514L)

#define ACS_PLUS (GTKCURSES__A_ALTCHARSET | 0x253cL)

#define ACS_HLINE (GTKCURSES__A_ALTCHARSET | 0x2500L)

#define ACS_LTEE (GTKCURSES__A_ALTCHARSET | 0x251cL)

#define ACS_RTEE (GTKCURSES__A_ALTCHARSET | 0x2524L)

#define ACS_BTEE (GTKCURSES__A_ALTCHARSET | 0x2534L)

#define ACS_TTEE (GTKCURSES__A_ALTCHARSET | 0x252cL)

#define ACS_VLINE (GTKCURSES__A_ALTCHARSET | 0x2502L)

#define ACS_BULLET (GTKCURSES__A_ALTCHARSET | 0x00b7L)

#define ACS_BLOCK (GTKCURSES__A_ALTCHARSET | 0x2588L)

#define INITSCR_WITH_HINTS

#ifndef GTKCURSES_MB_LEN_MAX
#define GTKCURSES_MB_LEN_MAX 6
#endif

static GtkWidget *gtkcurses_window = NULL;
static GdkWindow *gtkcurses_gdkwindow = NULL;
static GtkWidget *gtkcurses_dummy = NULL;
#if USE_PANGO
static PangoLayout *gtkcurses_layout = NULL;
static PangoFontDescription *gtkcurses_fontDescription = NULL;
#else
static GdkFont *gtkcurses_font = NULL;
#endif
static GdkColor gtkcurses_colors[COLORS];
static GdkColormap *gtkcurses_colormap;
static GdkGC *gtkcurses_palette[COLORS * COLORS] = { NULL };
static struct { short fg, bg; } gtkcurses_pairs[COLOR_PAIRS];

static int init_pair(short i, short fg, short bg)
{
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    gtkcurses_pairs[i].fg = fg;
    gtkcurses_pairs[i].bg = bg;
    return OK;
}

static int pair_content(short i, short *fg, short *bg)
{
    if ((i < 0) || (i > COLOR_PAIRS)) return ERR;
    *fg = gtkcurses_pairs[i].fg;
    *bg = gtkcurses_pairs[i].bg;
    return OK;
}

static int start_color(void)
{
    return OK;
}

static int init_color(short i, short r, short g, short b)
{
    short j;

    if ((i < 0) || (i > COLORS)) return ERR;
    if (r < 0) r = 0;
    if (r > 1000) r = 1000;
    gtkcurses_colors[i].red = 0xffffU * r / 1000;
    if (g < 0) g = 0;
    if (g > 1000) g = 1000;
    gtkcurses_colors[i].green = 0xffffU * g / 1000;
    if (b < 0) b = 0;
    if (b > 1000) b = 1000;
    gtkcurses_colors[i].blue = 0xffffU * b / 1000;
    gdk_colormap_alloc_color(gtkcurses_colormap, gtkcurses_colors + i, 0, 1);
    for (j = 0; j < COLORS; j ++)
    {
        if (! gtkcurses_palette[i * COLORS + j])
        {
            gtkcurses_palette[i * COLORS + j] = gdk_gc_new(gtkcurses_gdkwindow);
#if ! USE_PANGO
            gdk_gc_set_font(gtkcurses_palette[i * COLORS + j], gtkcurses_font);
#endif
            gdk_gc_set_fill(gtkcurses_palette[i * COLORS + j], GDK_SOLID);
            gdk_gc_set_foreground(gtkcurses_palette[i * COLORS + j], gtkcurses_colors + 15);
        }
        if (! gtkcurses_palette[j * COLORS + i])
        {
            gtkcurses_palette[j * COLORS + i] = gdk_gc_new(gtkcurses_gdkwindow);
#if ! USE_PANGO
            gdk_gc_set_font(gtkcurses_palette[j * COLORS + i], gtkcurses_font);
#endif
            gdk_gc_set_fill(gtkcurses_palette[j * COLORS + i], GDK_SOLID);
            gdk_gc_set_background(gtkcurses_palette[j * COLORS + i], gtkcurses_colors + 0);
        }
        gdk_gc_set_background(gtkcurses_palette[i * COLORS + j], gtkcurses_colors + i);
        gdk_gc_set_foreground(gtkcurses_palette[j * COLORS + i], gtkcurses_colors + i);
    }
    return OK;
}

static int color_content(short i, short *r, short *g, short *b)
{
    if ((i < 0) || (i > COLORS)) return ERR;
    *r = 1000 * gtkcurses_colors[i].red / 0xffffU;
    *g = 1000 * gtkcurses_colors[i].green / 0xffffU;
    *b = 1000 * gtkcurses_colors[i].blue / 0xffffU;
    return OK;
}

static size_t gtkcurses_chtypetombs(chtype ch, char *buf)
{
    char *buf0;

    buf0 = buf;
    *buf0 = 0;
    if (gtkcurses_bitmap) return 0;
    ch &= A_CHARTEXT;
    if (ch < 0) return 0;
    if (ch > 0x10fffdL) return 0;
    if ((ch & 0xfffeL) == 0xfffeL) return 0;
    if ((ch & GTKCURSES__A_ALTCHARSET) == GTKCURSES__A_ALTCHARSET)
    {
        ch &= ~GTKCURSES__A_ALTCHARSET;
    }
    if (ch < ' ')
    {
        *(buf ++) = ch;
        *buf = 0;
        return strlen(buf0);
    }
#if ! USE_PANGO
    {
        char *buf2;
        GdkWChar wbuf[2];

        wbuf[0] = gtkcurses_ucs_to_wchar(ch);
        wbuf[1] = 0;
        if (gtkcurses_native_wcwidth(wbuf[0]) < 1)
        {
            *buf0 = 0;
            return 0;
        }
        buf2 = gdk_wcstombs(wbuf);
        if (buf2)
        {
            if (strlen(buf2) < ((gtkcurses_font->type == GDK_FONT_FONTSET) ? GTKCURSES_MB_LEN_MAX : (1 + 1)))
            {
                memcpy(buf, buf2, strlen(buf2) + 1);
            }
            free((void *) buf2);
        }
        buf += strlen(buf);
    }
#else
    /* Yay! Pango always uses UTF-8. */
    if (ch <= 0x7fL)
    {
        *(buf++) = ch;
    }
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

static int gtkcurses_charpixwidth(char *buf)
{
    if (gtkcurses_bitmap)
    {
        return 1;
    }
    else
    {
#if USE_PANGO
        int w, h;

        pango_layout_set_text(gtkcurses_layout, buf, strlen(buf));
        pango_layout_get_pixel_size(gtkcurses_layout, &w, &h);
        return w;
#else
        return gdk_text_width(gtkcurses_font, buf, strlen(buf));
#endif
    }
}

static int gtkcurses_wcwidth(int ch)
{
    char buf[GTKCURSES_MB_LEN_MAX];
    int wcw;

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
    if ((ch < 0) || (ch > 0x10fffdU)) return 0;
    if (gtkcurses_bitmap) return ((ch >= ' ') && (ch != '\x7f'));
    gtkcurses_chtypetombs(ch, buf);
    wcw = (gtkcurses_charpixwidth(buf) + gtkcurses_charw - 1) / gtkcurses_charw;
    if (wcw > 2) wcw = 2;
    return wcw;
}

static void gtkcurses_ungetch(chtype ch)
{
    int i;

    if (ch == (chtype) ERR) return;
    for (i = 0; i < (sizeof(gtkcurses_getch_buffer)/sizeof(*gtkcurses_getch_buffer)); i ++)
    {
        if ((gtkcurses_getch_buffer[i] == ch)
            &&
            (ch == KEY_RESIZE))
        {
            return;
        }
        if (gtkcurses_getch_buffer[i] == (chtype) ERR)
        {
            gtkcurses_getch_buffer[i] = ch;
            return;
        }
    }
}

static gint gtkcurses_onIdle(gpointer unused)
{
    if (gtkcurses_ready && gtkcurses_mapped && gtkcurses_has_focus) gtk_main_quit();
    return TRUE;
}

static gint gtkcurses_onKeyPress(GtkWidget *grab_widget, GdkEventKey *event, gpointer unused)
{
    chtype ch;

    switch (event->keyval)
    {
    case ' ':
        ch = (event->state & GDK_CONTROL_MASK) ? 0 : ' ';
        break;
    case GDK_VoidSymbol:
        ch = (chtype) ERR;
        break;
    case GDK_BackSpace:
        ch = '\b';
        break;
    case GDK_Tab:
        ch = '\t';
        break;
    case GDK_Linefeed:
        ch = '\n';
        break;
    case GDK_Return:
        ch = '\r';
        break;
    case GDK_Pause:
        ch = '\x1b';
        break;
    case GDK_Escape:
        ch = '\x1b';
        break;
    case GDK_Delete:
        ch = '\x7f';
        break;
    case GDK_Break:
        ch = '\3';
        break;
    case GDK_KP_Tab:
        ch = '\t';
        break;
    case GDK_KP_Space:
        ch = ' ';
        break;
    case GDK_KP_Enter:
        ch = '\r';
        break;
    case GDK_Left:
    case GDK_KP_Left:
        ch = KEY_LEFT;
        break;
    case GDK_Up:
    case GDK_KP_Up:
        ch = KEY_UP;
        break;
    case GDK_Right:
    case GDK_KP_Right:
        ch = KEY_RIGHT;
        break;
    case GDK_Down:
    case GDK_KP_Down:
        ch = KEY_DOWN;
        break;
    case GDK_KP_Delete:
        ch = '\x7f';
        break;
    case GDK_KP_Equal:
        ch = '=';
        break;
    case GDK_KP_Multiply:
        ch = '*';
        break;
    case GDK_KP_Add:
        ch = '+';
        break;
    case GDK_KP_Separator:
        ch = ',';
        break;
    case GDK_KP_Subtract:
        ch = '-';
        break;
    case GDK_KP_Decimal:
        ch = '.';
        break;
    case GDK_KP_Divide:
        ch = '/';
        break;
    case GDK_KP_0:
        ch = '0';
        break;
    case GDK_KP_1:
        ch = '1';
        break;
    case GDK_KP_2:
        ch = '2';
        break;
    case GDK_KP_3:
        ch = '3';
        break;
    case GDK_KP_4:
        ch = '4';
        break;
    case GDK_KP_5:
        ch = '5';
        break;
    case GDK_KP_6:
        ch = '6';
        break;
    case GDK_KP_7:
        ch = '7';
        break;
    case GDK_KP_8:
        ch = '8';
        break;
    case GDK_KP_9:
        ch = '9';
        break;
    default:
        ch = (strlen(event->string) == 1) ? event->string[0] : ((event->keyval >= 0x00) && (event->keyval <= 0x7f) ? event->keyval : (chtype) ERR);
    }
    if (ch != (chtype) ERR)
    {
        gtkcurses_ungetch(ch);
        if (gtkcurses_ready) gtk_main_quit();
        return TRUE;
    }
    return FALSE;
}

static int move(int y, int x);

static int erase(void)
{
    short fg, bg;

    if (! gtkcurses_ready) return ERR;
    gtkcurses_attr = 0;
    move(0, 0);
    pair_content(PAIR_NUMBER(gtkcurses_attr), &fg, &bg);
    gdk_draw_rectangle(gtkcurses_gdkwindow,
                       gtkcurses_palette[COLORS * fg + bg],
                       1,
                       0,
                       0,
                       gtkcurses_pixwidth,
                       gtkcurses_pixheight);
    return OK;
}

static gboolean gtkcurses_onExpose(GtkWidget *widget, GdkEventExpose *event, gpointer ignored)
{
    if (gtkcurses_ready && ! event->count)
    {
        gtkcurses_resize_pending = 1;
        gtkcurses_ungetch(KEY_RESIZE);
    }
    return TRUE;
}

static gboolean gtkcurses_onConfigure(GtkWidget *widget, GdkEventConfigure *event, gpointer ignored)
{
    unsigned long w, h;

    w = event->width / gtkcurses_charw;
    h = event->height / gtkcurses_charh;
    gtkcurses_pixwidth = event->width;
    gtkcurses_pixheight = event->height;
    if (gtkcurses_ready)
    {
        gtkcurses_neww = w;
        gtkcurses_newh = h;
        gtkcurses_resize_pending = 1;
        gtkcurses_ungetch(KEY_RESIZE);
    }
    return TRUE;
}

static gint gtkcurses_onDestroy(GtkWidget * widget, GdkEventAny *event, gpointer ignored)
{
    if (gtkcurses_ready)
    {
        gtkcurses_ready = 0;
        gtkcurses_window = NULL;
        gtkcurses_gdkwindow = NULL;
        gtk_main_quit();
    }
    gtkcurses_ungetch('q');
    return TRUE;
}

static gint gtkcurses_onMap(GtkWidget * widget, GdkEventAny *event, gpointer ignored)
{
    gtkcurses_mapped = 1;
    if (gtkcurses_ready && gtkcurses_has_focus)
    {
        gtk_main_quit();
    }
    return TRUE;
}

static gint gtkcurses_onUnmap(GtkWidget * widget, GdkEventAny *event, gpointer ignored)
{
    gtkcurses_mapped = 0;
    return TRUE;
}

static gint gtkcurses_onFocusIn(GtkWidget * widget, GdkEventFocus *event, gpointer ignored)
{
    gtkcurses_has_focus = 1;
    if (gtkcurses_ready && gtkcurses_mapped)
    {
        gtk_main_quit();
    }
    return TRUE;
}

static gint gtkcurses_onFocusOut(GtkWidget * widget, GdkEventFocus *event, gpointer ignored)
{
    gtkcurses_has_focus = 0;
    return TRUE;
}

static int resizeterm(int y, int x)
{
    return OK;
}

static void initscrWithHints(int h, int w, const char *title, const char *shortname)
{
    int i, j;
    const char *font;
    const char *icon;

    if (gtkcurses_resize_pending)
    {
        goto gtkcurses_handle_resize;
    }
    icon = myman_getenv("GTKCURSES_ICON");
    if (icon && ! *icon) icon = NULL;
    font = myman_getenv("GTKCURSES_FONT");
    if (font && ! *font) font = NULL;
    if (font && ! *font) font = NULL;
    gtkcurses_bitmap = (myman_getenv("GTKCURSES_BITMAP") && *myman_getenv("GTKCURSES_BITMAP")) ? (strcmp("0", myman_getenv("GTKCURSES_BITMAP"))) : 0;
    if (! h) h = 25;
    if (! w) w = 80;
    for (i = 0; i < (sizeof(gtkcurses_getch_buffer)/sizeof(*gtkcurses_getch_buffer)); i ++)
    {
        gtkcurses_getch_buffer[i] = ERR;
    }
    if (! gtkcurses_init_done)
    {
        char *locale;
        int fake_argc;
        const char *fake_argv_data[2];
        const char **fake_argv;

        fake_argc = 1;
        fake_argv = fake_argv_data;
        fake_argv[0] = shortname;
        fake_argv[1] = NULL;
        /* FIXME: this loses constness; really we should malloc or
         * alloca our own copies here, but cleanup is a nightmare */
        gtk_init(&fake_argc, (char ***) &fake_argv);
        locale = gdk_set_locale();
        gtkcurses_idleAdd(gtkcurses_onIdle, NULL);
#if USE_PANGO
        gtkcurses_fontDescription = pango_font_description_from_string(font ? font :
#if defined(WIN32)
                                                                       "Courier"
#else
                                                                       "fixed"
#endif
            );
        if (! pango_font_description_get_size(gtkcurses_fontDescription))
        {
            pango_font_description_set_size(gtkcurses_fontDescription, 12);
        }
#else
        if (font)
        {
            gtkcurses_font = gdk_fontset_load(font);
            if (! gtkcurses_font)
            {
                gtkcurses_font = gdk_font_load(font);
            }
        }
        else
        {
            gtkcurses_font = NULL;
        }
        if (! gtkcurses_font)
        {
            gtkcurses_font = gdk_font_load("fixed");
        }
#endif
        gtkcurses_init_done = 1;
    }
    gtkcurses_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
#if USE_PANGO
    gtkcurses_layout = gtk_widget_create_pango_layout(gtkcurses_window, "");
    pango_layout_set_font_description(gtkcurses_layout, gtkcurses_fontDescription);
#endif
    gtk_window_set_title(GTK_WINDOW(gtkcurses_window), title);
    if (icon && ! access(icon, R_OK))
    {
        gtkcurses_loadIconFile(icon);
    }
    gtkcurses_connect(gtkcurses_window, "configure-event", gtkcurses_onConfigure, NULL);
    gtkcurses_connect(gtkcurses_window, "expose-event", gtkcurses_onExpose, NULL);
    gtkcurses_connect(gtkcurses_window, "delete-event", gtkcurses_onDestroy, NULL);
    gtkcurses_connect(gtkcurses_window, "destroy-event", gtkcurses_onDestroy, NULL);
    gtkcurses_connect(gtkcurses_window, "map-event", gtkcurses_onMap, NULL);
    gtkcurses_connect(gtkcurses_window, "unmap-event", gtkcurses_onUnmap, NULL);
    gtkcurses_connect(gtkcurses_window, "focus-in-event", gtkcurses_onFocusIn, NULL);
    gtkcurses_connect(gtkcurses_window, "focus-out-event", gtkcurses_onFocusOut, NULL);
    gtkcurses_connect(gtkcurses_window, "key-press-event", gtkcurses_onKeyPress, NULL);
    gtkcurses_dummy = gtk_label_new("");
    if (gtkcurses_bitmap)
    {
        gtkcurses_charw = gtkcurses_charh = 1;
    }
    else
    {
        gtkcurses_charh = 0;
        gtkcurses_charw = 0;
#if USE_PANGO
        for (i = ' '; i <= '~'; i ++)
        {
            char buf[GTKCURSES_MB_LEN_MAX];
            int width, height;

            gtkcurses_chtypetombs(i, buf);
            pango_layout_set_text(gtkcurses_layout, buf, strlen(buf));
            pango_layout_get_pixel_size(gtkcurses_layout, &width, &height);
            if (width > gtkcurses_charw) gtkcurses_charw = width;
            if (height > gtkcurses_charh) gtkcurses_charh = height;
        }
#else
        {
            int maxascent, maxdescent;

            maxascent = gtkcurses_font->ascent;
            maxdescent = gtkcurses_font->descent;
            for (i = ('j' - '_'); i <= '~'; i ++)
            {
                char buf[GTKCURSES_MB_LEN_MAX];
                int lbearing, rbearing, width, ascent, descent;

                gtkcurses_chtypetombs(i, buf);
                gdk_text_extents(gtkcurses_font, buf, 1, &lbearing, &rbearing, &width, &ascent, &descent);
                if (ascent > maxascent) maxascent = ascent;
                if (descent > maxdescent) maxdescent = descent;
                if ((maxascent + maxdescent) > gtkcurses_charh) gtkcurses_charh = maxascent + maxdescent;
                if (width > gtkcurses_charw) gtkcurses_charw = width;
            }
        }
#endif
        if (! gtkcurses_charh) gtkcurses_charh = 16;
        if (! gtkcurses_charw) gtkcurses_charw = 8;
    }
    if ((w * gtkcurses_charw) > gdk_screen_width()) w = gdk_screen_width() / gtkcurses_charw;
    if ((h * gtkcurses_charh) > gdk_screen_height()) h = gdk_screen_height() / gtkcurses_charh;
    if (w < 1) w = 1;
    if (h < 1) h = 1;
    gtkcurses_w = w;
    gtkcurses_h = h;
    gtkcurses_x = 0;
    gtkcurses_y = 0;
    gtkcurses_attr = 0;
    gtkcurses_pixwidth = w * gtkcurses_charw;
    gtkcurses_pixheight = h * gtkcurses_charh;
    gtk_window_set_default_size(GTK_WINDOW(gtkcurses_window), gtkcurses_pixwidth, gtkcurses_pixheight);
    gtk_container_add(GTK_CONTAINER(gtkcurses_window), gtkcurses_dummy);
    gtk_widget_show_all(gtkcurses_window);
    gtkcurses_gdkwindow = gtk_widget_get_parent_window(gtkcurses_dummy);
    gtkcurses_colormap = gtk_widget_get_default_colormap();
    memset((void *) gtkcurses_palette, 0, sizeof(gtkcurses_palette));
    for (i = 0; i < COLORS; i ++)
    {
        init_color(i,
                   1000 * ((i == PEN_BRIGHT) ? 0x7777U : (i == COLOR_WHITE) ? 0xaaaaU : (i & COLOR_RED) ? ((i & PEN_BRIGHT) ? 0xffffU : 0x8888U) : 0) / 0xffffU,
                   1000 * ((i == PEN_BRIGHT) ? 0x7777U : (i == COLOR_WHITE) ? 0xaaaaU : (i & COLOR_GREEN) ? ((i & PEN_BRIGHT) ? 0xffffU : 0x8888U) : 0) / 0xffffU,
                   1000 * ((i == PEN_BRIGHT) ? 0x7777U : (i == COLOR_WHITE) ? 0xaaaaU : (i & COLOR_BLUE) ? ((i & PEN_BRIGHT) ? 0xffffU : 0x8888U) : 0) / 0xffffU);
    }
    for (i = 0; i < COLORS; i ++)
    {
        for (j = 0; j < COLORS; j ++)
        {
            init_pair(i * COLORS + j, j, i);
        }
    }
    gtkcurses_ready = 1;
    gtk_main();
  gtkcurses_handle_resize:
    if (gtkcurses_resize_pending)
    {
        gtkcurses_w = gtkcurses_neww ? gtkcurses_neww : gtkcurses_w;
        gtkcurses_h = gtkcurses_newh ? gtkcurses_newh : gtkcurses_h;
        gtkcurses_neww = 0;
        gtkcurses_newh = 0;
        gtkcurses_resize_pending = 0;
        for (i = 0; i < (sizeof(gtkcurses_getch_buffer)/sizeof(*gtkcurses_getch_buffer)); i ++)
        {
            while (gtkcurses_getch_buffer[i] == KEY_RESIZE)
            {
                for (j = i + 1; j < (sizeof(gtkcurses_getch_buffer)/sizeof(*gtkcurses_getch_buffer)); j ++)
                {
                    gtkcurses_getch_buffer[j - 1] = gtkcurses_getch_buffer[j];
                }
                gtkcurses_getch_buffer[j - 1] = (chtype) ERR;
            }
        }
    }
}

static void endwin(void)
{
    if (gtkcurses_ready && ! gtkcurses_resize_pending)
    {
#if USE_ICONV
        if (gtkcurses_cd_to_wchar != (iconv_t) -1)
        {
            iconv_close(gtkcurses_cd_to_wchar);
            gtkcurses_cd_to_wchar = (iconv_t) -1;
        }
        if (gtkcurses_cd_to_uni != (iconv_t) -1)
        {
            iconv_close(gtkcurses_cd_to_uni);
            gtkcurses_cd_to_uni = (iconv_t) -1;
        }
#endif
        gtkcurses_ready = 0;
        if (gtkcurses_window) gtk_widget_destroy(gtkcurses_window);
        gtkcurses_window = NULL;
    }
}

static int addch(const chtype ch);

static chtype getch(void)
{
    chtype ret = ERR;
    int i, j;

    if (! gtkcurses_ready) return ERR;
    j = -1;
    for (i = 0; i < (sizeof(gtkcurses_getch_buffer)/sizeof(*gtkcurses_getch_buffer)); i ++)
    {
        if (gtkcurses_getch_buffer[i] != ERR)
        {
            j = i;
        }
    }
    if (j != -1)
    {
        ret = gtkcurses_getch_buffer[j];
        gtkcurses_getch_buffer[j] = ERR;
    }
    else
    {
        do
        {
            gtk_main();
            for (i = 0; i < (sizeof(gtkcurses_getch_buffer)/sizeof(*gtkcurses_getch_buffer)); i ++)
            {
                if (gtkcurses_getch_buffer[i] != ERR)
                {
                    j = i;
                }
            }
            if (j != -1)
            {
                ret = gtkcurses_getch_buffer[j];
                gtkcurses_getch_buffer[j] = ERR;
            }
        }
        while ((ret == (chtype) ERR) && (! (gtkcurses_mapped && gtkcurses_has_focus)));
    }
    return ret;
}

static int refresh(void)
{
    if (! gtkcurses_ready) return OK;
    gtkcurses_ungetch(getch());
    return OK;
}

static void standout()
{
    gtkcurses_attr = A_STANDOUT;
}

static void standend()
{
    gtkcurses_attr = 0;
}

static int attrset(int a)
{
    gtkcurses_attr = a;
    return OK;
}

static int move(int y, int x)
{
    gtkcurses_y = y;
    gtkcurses_x = x;
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

static int addch(chtype ch)
{
    char buf[GTKCURSES_MB_LEN_MAX];
    short fg, bg;
    attr_t a;
    int wcw;

    a = (gtkcurses_attr & ~(PAIR_NUMBER(ch) ? PAIR_NUMBER(gtkcurses_attr) : 0)) | (ch & ~A_CHARTEXT);
    ch &= A_CHARTEXT;
    if (! gtkcurses_ready) return ERR;
    wcw = wcwidth(ch);
    if (ch == (chtype) (unsigned long) (unsigned char) '\a')
    {
        return OK;
    }
    else if (ch == (chtype) (unsigned long) (unsigned char) '\b')
    {
        if (gtkcurses_y && ! gtkcurses_x)
        {
            gtkcurses_y --;
        }
        gtkcurses_x += gtkcurses_w - 1;
        gtkcurses_x %= gtkcurses_w;
        return OK;
    }
    else if (ch == (chtype) (unsigned long) (unsigned char) '\n')
    {
        gtkcurses_y ++;
        gtkcurses_x = 0;
        return OK;
    }
    else if (ch == (chtype) (unsigned long) (unsigned char) '\t')
    {
        do
        {
            addch((chtype) (unsigned long) (unsigned char) ' ');
        }
        while (gtkcurses_x % 8);
        return OK;
    }
    else if (wcw < 1)
    {
        return ERR;
    }
    if (gtkcurses_x < 0) return ERR;
    if (gtkcurses_y < 0) return ERR;
    if (gtkcurses_x >= gtkcurses_w) return ERR;
    if (gtkcurses_y >= gtkcurses_h) return ERR;
#if USE_PANGO
    if ((ch & GTKCURSES__A_ALTCHARSET) == GTKCURSES__A_ALTCHARSET)
    {
        int width;

        gtkcurses_chtypetombs(ch & ~GTKCURSES__A_ALTCHARSET, buf);
        width = gtkcurses_charpixwidth(buf);
        if ((width > 0) && (width <= gtkcurses_charw))
        {
            ch &= ~GTKCURSES__A_ALTCHARSET;
        }
    }
#else
    /* surprisingly many X11 fonts support the XTerm convention for
     * overlaying C0 with the VT100 altcharset */
    switch(ch)
    {
    case ACS_LRCORNER:
        gtkcurses_chtypetombs(('j' - '_'), buf);
        if (gtkcurses_charpixwidth(buf) > 0)
        {
            ch = ('j' - '_');
        }
        break;
    case ACS_URCORNER:
        gtkcurses_chtypetombs(('k' - '_'), buf);
        if (gtkcurses_charpixwidth(buf) > 0)
        {
            ch = ('k' - '_');
        }
        break;
    case ACS_ULCORNER:
        gtkcurses_chtypetombs(('l' - '_'), buf);
        if (gtkcurses_charpixwidth(buf) > 0)
        {
            ch = ('l' - '_');
        }
        break;
    case ACS_LLCORNER:
        gtkcurses_chtypetombs(('m' - '_'), buf);
        if (gtkcurses_charpixwidth(buf) > 0)
        {
            ch = ('m' - '_');
        }
        break;
    case ACS_PLUS:
        gtkcurses_chtypetombs(('n' - '_'), buf);
        if (gtkcurses_charpixwidth(buf) > 0)
        {
            ch = ('n' - '_');
        }
        break;
    case ACS_HLINE:
        gtkcurses_chtypetombs(('q' - '_'), buf);
        if (gtkcurses_charpixwidth(buf) > 0)
        {
            ch = ('q' - '_');
        }
        break;
    case ACS_LTEE:
        gtkcurses_chtypetombs(('t' - '_'), buf);
        if (gtkcurses_charpixwidth(buf) > 0)
        {
            ch = ('t' - '_');
        }
        break;
    case ACS_RTEE:
        gtkcurses_chtypetombs(('u' - '_'), buf);
        if (gtkcurses_charpixwidth(buf) > 0)
        {
            ch = ('u' - '_');
        }
        break;
    case ACS_BTEE:
        gtkcurses_chtypetombs(('v' - '_'), buf);
        if (gtkcurses_charpixwidth(buf) > 0)
        {
            ch = ('v' - '_');
        }
        break;
    case ACS_TTEE:
        gtkcurses_chtypetombs(('w' - '_'), buf);
        if (gtkcurses_charpixwidth(buf) > 0)
        {
            ch = ('w' - '_');
        }
        break;
    case ACS_VLINE:
        gtkcurses_chtypetombs(('x' - '_'), buf);
        if (gtkcurses_charpixwidth(buf) > 0)
        {
            ch = ('x' - '_');
        }
        break;
    case ACS_BULLET:
        gtkcurses_chtypetombs(('~' - '_'), buf);
        if (gtkcurses_charpixwidth(buf) > 0)
        {
            ch = ('~' - '_');
        }
        break;
    }
#endif
    switch(ch)
    {
    case ACS_LRCORNER:
    case ACS_URCORNER:
    case ACS_ULCORNER:
    case ACS_LLCORNER:
    case ACS_PLUS:
        gtkcurses_chtypetombs('+', buf);
        break;
    case ACS_HLINE:
        gtkcurses_chtypetombs('-', buf);
        break;
    case ACS_LTEE:
    case ACS_RTEE:
    case ACS_BTEE:
    case ACS_TTEE:
        gtkcurses_chtypetombs('+', buf);
        break;
    case ACS_VLINE:
        gtkcurses_chtypetombs('|', buf);
        break;
    case ACS_BULLET:
        gtkcurses_chtypetombs('.', buf);
        break;
    case ACS_BLOCK:
        gtkcurses_chtypetombs('#', buf);
        break;
    default:
        gtkcurses_chtypetombs(ch, buf);
        if (gtkcurses_charpixwidth(buf) <= 0)
        {
            gtkcurses_chtypetombs('*', buf);
        }
    }
    fg = COLOR_WHITE | PEN_BRIGHT;
    bg = COLOR_BLACK;
    if (a & A_BOLD)
    {
        fg = COLOR_YELLOW | PEN_BRIGHT;
    }
    else if (a & A_UNDERLINE)
    {
        fg = COLOR_BLUE;
    }
    if (a & GTKCURSES__A_COLOR)
    {
        pair_content(PAIR_NUMBER(a), &fg, &bg);
        if (a & A_BOLD)
        {
            fg |= PEN_BRIGHT;
        }
    }
    if (gtkcurses_bitmap)
    {
        if ((ch != ' ')
            &&
            (ch != '\0')
            &&
            (ch != 0x00a0U)
            &&
            (ch != 0x3000U)
            &&
            (ch != 0x2008U))
        {
            ch = ACS_BLOCK;
        }
        else
        {
            ch = ' ';
        }
    }
    if (((a & A_STANDOUT) ? 1 : 0) ^ (ch == ACS_BLOCK))
    {
        int tmp;

        tmp = fg;
        fg = bg;
        bg = tmp;
    }
    gdk_draw_rectangle(gtkcurses_gdkwindow,
                       gtkcurses_palette[COLORS * fg + bg],
                       1,
                       gtkcurses_x * gtkcurses_charw,
                       gtkcurses_y * gtkcurses_charh,
                       gtkcurses_charw * wcw,
                       gtkcurses_charh);
    if ((ch != ' ') && (ch != ACS_BLOCK) && ! gtkcurses_bitmap)
    {
#if USE_PANGO
        int w, h;

        pango_layout_set_text(gtkcurses_layout, buf, strlen(buf));
        pango_layout_get_pixel_size(gtkcurses_layout, &w, &h);
        gdk_draw_layout(gtkcurses_gdkwindow,
                        gtkcurses_palette[COLORS * bg + fg],
                        gtkcurses_x * gtkcurses_charw + ((gtkcurses_charw * wcw) - w) / 2,
                        gtkcurses_y * gtkcurses_charh,
                        gtkcurses_layout);
#else
        gdk_draw_text(gtkcurses_gdkwindow,
                      gtkcurses_font,
                      gtkcurses_palette[COLORS * bg + fg],
                      gtkcurses_x * gtkcurses_charw + ((gtkcurses_charw * wcw) - gtkcurses_charpixwidth(buf)) / 2,
                      gtkcurses_y * gtkcurses_charh + gtkcurses_font->ascent,
                      buf,
                      strlen(buf));
#endif
    }
    gtkcurses_x += wcw;
    return OK;
}

static int clrtobot(void)
{
    unsigned int x, y;

    if (! gtkcurses_ready) return ERR;
    for (x = gtkcurses_x; x < COLS; x ++)
    {
        move(gtkcurses_y, x);
        addch(' ');
    }
    for (y = gtkcurses_y + 1; y < LINES; y ++)
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

    if (! gtkcurses_ready) return ERR;
    for (x = gtkcurses_x; x < COLS; x ++)
    {
        move(gtkcurses_y, x);
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

/* we do iconv internally, so do not handle this in the caller */
#undef USE_ICONV
#define USE_ICONV 0
