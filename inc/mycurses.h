/*
 * mycurses.h - quick and dirty replacement for curses.h
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

#ifndef MYMAN
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static int fputc_utf8_cp437(int ch, FILE *fh) { return fputc(ch & 0xff, fh); }
static int locale_is_utf8(void) { return 0; }
#endif
#ifdef BUILDING_MYCURSES
#define RAWCURSES_GLOBAL(dfn, init) dfn init
#else
#define RAWCURSES_GLOBAL(dfn, init) extern dfn
#endif
#define OPTCURSES_GLOBAL RAWCURSES_GLOBAL
#include "rawcurs.h"
#include "optcurs.h"
/* additional macros used by rogue985 */
#define initscr() initscrWithHints(24, 80, 0, 0)
#define mvaddch(y, x, ch) (move(y, x), addch(ch))
#define inch() (optcurses_backing ? (optcurses_backing[optcurses_y * optcurses_w + optcurses_x].ch ? optcurses_backing[optcurses_y * optcurses_w + optcurses_x].ch : optcurses_backing[(optcurses_h + optcurses_y) * optcurses_w + optcurses_x].ch) : (chtype) ERR)
#define mvinch(y, x) (move(y, x), inch())
#define mvaddstr(y, x, s) (move(y, x), addstr(s))
#define crmode()
