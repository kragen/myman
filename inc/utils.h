/* utils.h - various utilities for the MyMan video game
 * Copyright 1997-2009, Benjamin C. Wiley Sittler <bsittler@gmail.com>
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

#ifndef MYMAN_UTILS_H_INCLUDED
#define MYMAN_UTILS_H_INCLUDED 1

#if ! defined(WIN32)
#if defined(_WIN32)

/* some Win32 C compilers do not define WIN32 */
#define WIN32 1

#else /* ! defined(_WIN32) */
#if defined(DOS) || defined(_MSDOS) || defined(__DOS__) || defined(__TURBOC__)

/* some DOS C compilers do not define __MSDOS__ */
#ifndef __MSDOS__
#define __MSDOS__ 1
#endif

#endif /* defined(DOS) || defined(_MSDOS) || defined(__DOS__) || defined(__TURBOC__) */
#endif /* ! defined(_WIN32) */
#endif /* ! defined(WIN32) */

#undef DATADIR /* FIXME: conflicts with Win32 header files */

/* subdirectory for graphics datafiles */
#ifndef GFXDIR
#define GFXDIR "gfx"
#endif

/* PNG icon filename (only used by GTK+-2.0 driver so far) */
#ifndef MYMANICONPNG
#define MYMANICONPNG GFXDIR "/myman48.png"
#endif

/* notice to display on the intro screen */
#ifndef MYMANNOTICE
#define MYMANNOTICE "DEDICATED TO TORU"
#endif

/* used for HTML screenshots */
#ifndef HTM_SUFFIX
#define HTM_SUFFIX ".html"
#endif

/* used for text screenshots */
#ifndef TXT_SUFFIX
#define TXT_SUFFIX ".txt"
#endif 

extern const char *maze_ABOUT_prefix;

extern const char *maze_FIXME_prefix;

extern const char *maze_NOTE_prefix;

extern const char *tile_ABOUT_prefix;

extern const char *tile_FIXME_prefix;

extern const char *tile_NOTE_prefix;

extern const char *sprite_ABOUT_prefix;

extern const char *sprite_FIXME_prefix;

extern const char *sprite_NOTE_prefix;

extern const char *MYMANKEYS_prefix;

extern const char *MOREMESSAGE;

extern const char *DONEMESSAGE;

#define ISTEXT(c) (((c) == '!') || (((c) >= '0') && ((c) <= '9')) || ((c) == '@') || (((c) >= 'A') && ((c) <= 'Z')))
#define ISDOT(c) (((c) == 249) || ((c) == '.'))
#define ISPELLET(c) (((c) == 254) || ((c) == 'o'))
#define ISZAPLEFT(c) (((c) == '<') || (c == 174))
#define ISZAPRIGHT(c) (((c) == '>') || (c == 175))
#define ISZAPUP(c) ((c) == '^')
#define ISZAPDOWN(c) ((c) == 'v')
#define ISOPEN(c) (((c) == ' ') || ISDOT(c) || ISPELLET(c) || ((c) == '!') || (((c) >= 'A') && ((c) <= 'Z')) || ((c) == 'l') || ((c) == '~') || ISZAPRIGHT(c) || ISZAPLEFT(c) || ISZAPUP(c) || ISZAPDOWN(c))
#define ISDOOR(c) (((c) == '=') || (c == ':') || ((c) == 240) || (c == 255))

#define ISWALLCENTER(c) ((((unsigned) (unsigned char) (char) (c)) == 0x07) || (((unsigned) (unsigned char) (char) (c)) == 0x12) || (((unsigned) (unsigned char) (char) (c)) == 0x1d) || (((unsigned) (unsigned char) (char) (c)) == 0x3c) || (((unsigned) (unsigned char) (char) (c)) == 0x3e) || (((unsigned) (unsigned char) (char) (c)) == 0x5e) || (((unsigned) (unsigned char) (char) (c)) == 0x76) || (((unsigned) (unsigned char) (char) (c)) == 0xae) || (((unsigned) (unsigned char) (char) (c)) == 0xaf))
#define ISWALLUP(c) (((unsigned) udlr[(unsigned int) (unsigned char) (c)]) & 0xc0)
#define ISWALLDOWN(c) (((unsigned) udlr[(unsigned int) (unsigned char) (c)]) & 0x30)
#define ISWALLLEFT(c) (((unsigned) udlr[(unsigned int) (unsigned char) (c)]) & 0x0c)
#define ISWALLRIGHT(c) (((unsigned) udlr[(unsigned int) (unsigned char) (c)]) & 0x03)
#define ISWALL(c) (ISWALLUP(c) || ISWALLDOWN(c) || ISWALLLEFT(c) || ISWALLRIGHT(c) || ISWALLCENTER(c))
#define ISNONINVERTABLE(c) (ISPELLET(c) || ISDOT(c) || ISZAPLEFT(c) || ISZAPRIGHT(c) || ISZAPUP(c) || ISZAPDOWN(c) || ISDOOR(c))

#define NPENS 256

/* base name for package (without version number or suffix) */
#ifndef MYMAN
#define MYMAN "myman"
#endif

/* copyright information for the program as a whole */
#ifndef MYMANCOPYRIGHT
#define MYMANCOPYRIGHT "Copyright 1997-2009, Benjamin C. Wiley Sittler <bsittler@gmail.com>"
#endif

/* package version number */
#ifndef MYMANVERSION
#define MYMANVERSION "devel"
#endif

/* legal notice to display before starting */
extern const char *MYMANLEGALNOTICE;

extern const char *MYMANKEYS;

extern const char *short_options;
extern struct option *long_options;

/* UCS/Unicode mapping for CP437 (used in HTML snapshots) */

extern const unsigned long uni_cp437_halfwidth[256];

/* alternate UCS/Unicode mapping used in CJK fullwidth mode */

extern const unsigned long uni_cp437_fullwidth[256];

/* right-hand-size mapping used when approximating fullwidth with
 * halfwidth */

extern const unsigned char cp437_fullwidth_rhs[256];

/* mapping used to reflect sprites */

extern unsigned char reflect_sprite[256];

/* fallback mapping to map missing sprites to tiles */

extern unsigned char cp437_sprite[256];

/* box drawing Up-Down-Left-Right data for CP437 */

extern const unsigned char udlr[256];

/* fallback mapping for missing tiles */

extern unsigned char fallback_cp437[256];

/* mapping used to reflect tiles */

extern unsigned char reflect_cp437[256];

extern short mille_to_scale(short n, short scale);

extern const unsigned long *uni_cp437;

/* read a utf-8 sequence from stream, convert it to cp437, and return
 * it. unmappable sequences are silently converted to spaces. this
 * theoretically works with U+0000 .. U+D7FF and U+E000 .. U+10FFFF */

#if 0
extern int fputc_utf8(unsigned long u, FILE *stream);
#endif

extern int
fgetc_cp437_utf8(FILE *stream);

/* like fputc, but converts unicode to a utf-8 sequence. this
 * theoretically works with U+0000 .. U+D7FF and U+E000 .. U+10FFFF;
 * note that the return value is a bit different -- EOF for failure, 0
 * for success */

extern int fputc_utf8(unsigned long u, FILE *stream);

/* like fputc, but converts cp437 to a utf-8 sequence. this
 * theoretically works with U+0000 .. U+D7FF and U+E000 .. U+10FFFF;
 * adding 0x100 ignores CJK mode */

extern int fputc_utf8_cp437(int c, FILE *stream);

/* like ungetc, but converts cp437 to a utf-8 sequence and puts that
 * back one byte at a time. this theoretically works with U+0000
 * .. U+D7FF and U+E000 .. U+10FFFF */

extern int ungetc_cp437_utf8(int c, FILE *stream);

extern char *strword(const char *from, const char **endp, size_t *lenp);

extern long *strtollist(const char *from, const char **endp, size_t *lenp);

extern double *strtodlist(const char *from, const char **endp, size_t *lenp);

extern long *strtollist_word(const char *from, const char **endp, size_t *lenp);

extern double *strtodlist_word(const char *from, const char **endp, size_t *lenp);

extern void mymanescape(const char *s, int len);

extern int readfont(const char *fontfile,
                    int *w,
                    int *h,
                    const char **font,
                    int *used,
                    int *flags,
                    int *color,
                    const char **args);

extern void writefont(const char *file,
                      const char *prefix,
                      int w, int h,
                      const char **font,
                      int *used,
                      int flags,
                      int *color,
                      const char *args);

extern int parse_tile_args(const char *tilefile, const char *tile_args);

extern int parse_sprite_args(const char *spritefile, const char *sprite_args);

extern const char *progname;

extern int readmaze(const char *mazefile,
                    int *levels,
                    int *w,
                    int *h,
                    char **maze,
                    int *flags,
                    char **color,
                    const char **args);

extern void writemaze(const char *mazefile);

extern int parse_maze_args(const char *mazefile, const char *maze_args);

extern char *mystrdup(const char *s);

#undef strdup
#define strdup(s) mystrdup(s)

#ifndef MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) (((y) <= (x)) ? (x) : (y))
#endif

/* which level to show intermissions after */
#ifndef INTERMISSION
#define INTERMISSION(level) \
    (((level) == 2) || \
     (((level) >= 5) && (! (((level) - 1) % 4))))
#endif

/* intermission length */
#ifndef INTERMISSION_TIME
#define INTERMISSION_TIME (PIX_W * 3 + TWOSECS)
#endif

/* how many times to show a given intermission */
#ifndef INTERMISSION_REPEAT
#define INTERMISSION_REPEAT(intermission) \
    (((intermission) == 2) ? 3 : 1)
#endif

/* how many intermissions are there? */
#ifndef INTERMISSION_N
#define INTERMISSION_N 3
#endif

extern char *maze;
extern char *maze_color;
extern char *blank_maze;
extern char *blank_maze_color;
extern unsigned char *dirty_cell;
extern int all_dirty;

extern int nogame;

extern int maze_n;
extern int maze_w;
extern int maze_h;
extern int maze_flags;
extern int maze_level;
extern const char *maze_args;

extern int tile_w;
extern int tile_h;
extern int tile_flags;
extern const char *tile_args;
extern const char *tile[256];
extern int tile_used[256];
extern int tile_color[256];

extern int sprite_w;
extern int sprite_h;
extern int sprite_flags;
extern const char *sprite_args;
extern const char *sprite[256];
extern int sprite_used[256];
extern int sprite_color[256];

#ifndef VISIBLE_EYES
#define VISIBLE_EYES (! ((sprite_flags) & 1))
#endif

#ifndef SOLID_WALLS
#define SOLID_WALLS ((tile_flags) & 2)
#endif

#ifndef SOLID_WALLS_BGCOLOR
#define SOLID_WALLS_BGCOLOR ((tile_flags) & 4)
#endif

/* whether to always switch maze levels, rather than only during intermissions */
#ifndef FLIP_ALWAYS
#define FLIP_ALWAYS (! ((maze_flags) & 1))
#endif

/* whether to repeat the flip_to maze after showing the last one */
#ifndef FLIP_LOCK
#define FLIP_LOCK ((maze_flags) & 2)
#endif

/* if set, base the WALL_COLOR vs. MORTAR_COLOR choice on wall thickness rather than wall edge vs. wall fill*/
#ifndef BICOLOR_WALLS
#define BICOLOR_WALLS ((maze_flags) & 4)
#endif

#define XWRAP(x) (((x) + maze_w + 1) % (maze_w + 1))
#define YWRAP(y) (((y) + maze_h) % maze_h)

#define XWRAP2(x) (XWRAP(x) % maze_w)

#define CLEAN_ALL() do { memset((void *)dirty_cell, 0, maze_h * ((maze_w + 1 + 7) >> 3) * sizeof(*dirty_cell)); all_dirty = 0; } while (0)
#define DIRTY_ALL() do { all_dirty = 1; } while (0)
#define IS_CELL_DIRTY(x,y) (all_dirty || ((((long) (x)) >= 0) && (((long) (y)) >= 0) && ((x) <= maze_w) && ((y) < maze_h) && ((unsigned) dirty_cell[((y)) * ((maze_w+1+7) >> 3) + ((x)>>3)]&(1<<((x)&7)))))

extern void maze_erase(void);

extern void mark_cell(int x, int y);

extern void maze_puts(int y, int x, int color, const char *s);

extern void maze_putsn_nonblank(int y, int x, int color, const char *s, int n);

#ifndef LIVES
#define LIVES 3
#endif

#ifndef GHOSTS
#define GHOSTS (maze_GHOSTS_len ? maze_GHOSTS[maze_level % maze_GHOSTS_len] : 4)
#endif

#ifndef MAXGHOSTS
#define MAXGHOSTS 16
#endif

extern int ghost_dir[MAXGHOSTS], ghost_mem[MAXGHOSTS], ghost_man[MAXGHOSTS], ghost_timer[MAXGHOSTS];
extern unsigned char *home_dir;

/* sprite register numbers */
#define GHOSTEYES(ghost) ((ghost) * 2)
#define UNGHOSTEYES(sprite_register) ((sprite_register) / 2)
#define MEANGHOST(ghost) (GHOSTEYES(ghost) + 1)
#define HERO MEANGHOST(MAXGHOSTS)
#define BLUEGHOST(ghost) ((ghost) + HERO + 1)
#define FRUIT BLUEGHOST(MAXGHOSTS)
#define GHOST_SCORE (FRUIT + 1)
#define FRUIT_SCORE (GHOST_SCORE + 1)
/* large hero sprite register, split into four quadrants */
#define BIGHERO_UL (FRUIT_SCORE + 1)
#define BIGHERO_UR (BIGHERO_UL + 1)
#define BIGHERO_LL (BIGHERO_UR + 1)
#define BIGHERO_LR (BIGHERO_LL + 1)
/* total sprite register count */
#define SPRITE_REGISTERS (BIGHERO_LR + 1)

extern unsigned char sprite_register[SPRITE_REGISTERS];
extern int sprite_register_frame[SPRITE_REGISTERS];
extern int sprite_register_x[SPRITE_REGISTERS], sprite_register_y[SPRITE_REGISTERS];
extern int sprite_register_used[SPRITE_REGISTERS];
extern int sprite_register_timer[SPRITE_REGISTERS];
extern int sprite_register_color[SPRITE_REGISTERS];

extern void
mark_sprite_register(int s);

#define SPRITE_FRUIT 0x00
#define SPRITE_MEAN 0x08
#define SPRITE_EYES 0x0a
#define SPRITE_BLUE 0x0e
#define SPRITE_HERO 0x10
#define SPRITE_FRUIT_SCORE 0x29
#define SPRITE_200 0x31
#define SPRITE_400 0x32
#define SPRITE_800 0x33
#define SPRITE_1600 0x34
#define SPRITE_WHITE 0x35
#define SPRITE_LIFE 0x37
/* rightward-facing large hero sprite, split into four quadrants with four frames each */
#define SPRITE_BIGHERO_UL 0x38
#define SPRITE_BIGHERO_UR 0x3C
#define SPRITE_BIGHERO_LL 0x40
#define SPRITE_BIGHERO_LR 0x44

#define MYMAN_UP 1
#define MYMAN_LEFT 2
#define MYMAN_DOWN 3
#define MYMAN_RIGHT 4

#ifndef DIRHERO
#define DIRHERO MYMAN_LEFT
#endif

#define DIRWRAP(dir) (((dir) + 3) % 4 + 1)

#define XDIR(dir) ((((dir) == MYMAN_RIGHT) ? 1 : 0) - (((dir) == MYMAN_LEFT) ? 1 : 0))
#define YDIR(dir) ((((dir) == MYMAN_DOWN) ? 1 : 0) - (((dir) == MYMAN_UP) ? 1 : 0))

#define YLEAVING(dir, y) ( -((((dir) == MYMAN_UP) && ! NOTTOP(y)) ? 1 : 0) + \
((((dir) == MYMAN_DOWN) && ! NOTBOTTOM(y)) ? 1 : 0))
#define XLEAVING(dir, x) ( -((((dir) == MYMAN_LEFT) && ! NOTLEFT(x)) ? 1 : 0) + \
((((dir) == MYMAN_RIGHT) && ! NOTRIGHT(x)) ? 1 : 0))

extern unsigned char gfx2(unsigned char c);

extern size_t gfx1(const char **font, unsigned char c, int y, int x, int w);

extern unsigned char gfx0(unsigned char c, unsigned char *m);

#define gfx_w (gfx_reflect ? tile_h : tile_w)
#define gfx_h (gfx_reflect ? tile_w : tile_h)
#define gfx(c,y,x) ((unsigned long) (unsigned char) gfx2((unsigned long) (unsigned char) gfx1(tile, (unsigned long) (unsigned char) gfx0(((unsigned long) (unsigned char) (c)), (reflect_cp437)), (y) % gfx_h, (x) % gfx_w, tile_w)))

#define sgfx_w (gfx_reflect ? sprite_h : sprite_w)
#define sgfx_h (gfx_reflect ? sprite_w : sprite_h)
#define sgfx(c,y,x) ((unsigned long) (unsigned char) gfx2((unsigned long) (unsigned char) gfx1(sprite, (unsigned long) (unsigned char) gfx0(((unsigned long) (unsigned char) (c)), (reflect_sprite)), (y) % sgfx_h, (x) % sgfx_w, sprite_w)))

extern int reflect;
extern int gfx_reflect;
#define REFLECT_LARGE (reflect && (tile_w == tile_h) && (sprite_w == sprite_h))

#define XTILE(x) ((x) / gfx_w)
#define YTILE(y) ((y) / gfx_h)

#define collide(s1, s2) ((XTILE(sprite_register_x[s1] - sgfx_w / 2) == XTILE(sprite_register_x[s2] - sgfx_w / 2)) \
&& (YTILE(sprite_register_y[s1] - sgfx_h / 2) == YTILE(sprite_register_y[s2] - sgfx_h / 2)))

#define PIX_W ((maze_w + 1) * gfx_w)
#define PIX_H (maze_h * gfx_h)

#define XPIXWRAP(x) (((x) + PIX_W) % PIX_W)
#define YPIXWRAP(y) (((y) + PIX_H) % PIX_H)

#define NOTTOP(y) (y >= (gfx_h / 2))
#define NOTBOTTOM(y) (y <= (gfx_h / 2))
#define NOTLEFT(x) (x >= (gfx_w / 2))
#define NOTRIGHT(x) (x <= (gfx_w / 2))

#define XHERO (CHERO * gfx_w)
#define YHERO (RHERO * gfx_h)
#define XFRUIT (CFRUIT * gfx_w)
#define YFRUIT (RFRUIT * gfx_h)
#define YGHOST (RGHOST * gfx_h)
#define XGHOST (CGHOST * gfx_w)
#define YTOP (RTOP * gfx_h)

#define INSIDE_WALL_INVERTED 0x0001
#define INSIDE_WALL_NON_INVERTABLE 0x0002
#define INSIDE_WALL_FULLY_INVERTED 0x0004
#define INSIDE_WALL_FULLY_NON_INVERTED 0x0008
#define INSIDE_WALL_PROVISIONAL 0x0010
#define INSIDE_WALL_YES 0x0020
#define INSIDE_WALL_NO 0x0040
#define INSIDE_WALL_PHASE2 0x0080
#define INSIDE_WALL_PHASE3 0x0100
#define IS_INVERTED(x,y) (((unsigned) inside_wall[(maze_level*maze_h+(y)) * (maze_w + 1)+(x)]) & INSIDE_WALL_INVERTED)
#define IS_FULLY_INVERTED(x,y) (((unsigned) inside_wall[(maze_level*maze_h+(y)) * (maze_w + 1)+(x)]) & INSIDE_WALL_FULLY_INVERTED)
#define IS_FULLY_NON_INVERTED(x,y) (((unsigned) inside_wall[(maze_level*maze_h+(y)) * (maze_w + 1)+(x)]) & INSIDE_WALL_FULLY_NON_INVERTED)

extern double doubletime(void);
extern void my_usleep(long usecs);

extern void gameintro(void);
extern void gamedemo(void);
extern void gamestart(void);
extern void gameintermission(void);
extern void gamehelp(void);
extern void gameinfo(void);
extern int gamelogic(void);
extern void gamesfx(void);
extern void gamereset(void);
extern void gamerender(void);
extern int gameinput(void);
extern int gamecycle(int lines, int cols);

extern void creditscreen(void);
extern void paint_walls(int verbose);

extern unsigned short *inside_wall;
extern FILE *snapshot;
extern FILE *snapshot_txt;
extern int xoff_received;
extern double td;
extern const char * pager_notice;
extern const char * pager_remaining;
extern int pager_arrow_magic;
extern int reinit_requested;
extern long myman_intro;
extern unsigned long myman_start;
extern unsigned long myman_demo;
extern int munched;
extern int old_lines, old_cols, old_score, old_showlives, old_level;
extern int ignore_delay;
extern long frameskip, frameskip0, frameskip1;
extern long scrolling;
extern long frames;
extern long winning;
extern unsigned long mymandelay;
extern unsigned long mindelay;
extern int ghost_eaten_timer;
extern int paused;
extern long intermission_running;
extern unsigned long myman_demo_setup;
extern int need_reset;
extern int level,
    intermission,
    intermission_shown,
    cycles,
    score,
    dots,
    points,
    lives,
    lives_used,
    earned,
    dying,
    dead,
    deadpan,
    myman_lines,
    myman_columns,
    oldplayer,
    player;
extern int key_buffer;
extern int key_buffer_ERR;
extern long pellet_timer,
    pellet_time;

#define GHOST0 ((ghosts > 2) ? 0 : 2)
#define GHOST1 1
#define GHOST2 ((ghosts > 2) ? 2 : 0)
#define GHOST3 3

#ifndef NAME_HEADER
#define NAME_HEADER "CHARACTER /"
#endif

#ifndef NICK_HEADER
#define NICK_HEADER " NICKNAME"
#endif

#ifndef GHOST_NAMES
#define GHOST_NAMES GHOST_NAMES_XLT
#endif

#define GHOST_NAMES_JA 0

#define GHOST_NAMES_ALT 1

#define GHOST_NAMES_EN 2

#define GHOST_NAMES_XLT 3

#if GHOST_NAMES == GHOST_NAMES_JA

#ifndef GHOST0_NAME
#define GHOST0_NAME " KIMAGURE--"
#endif

#ifndef GHOST0_NICK
#define GHOST0_NICK "\"AOSUKE\""
#endif

#ifndef GHOST2_NAME
#define GHOST2_NAME " MACHIBUSE-"
#endif

#ifndef GHOST2_NICK
#define GHOST2_NICK "-\"PINKY\""
#endif

#ifndef GHOST1_NAME
#define GHOST1_NAME " OIKAKE----"
#endif

#ifndef GHOST1_NICK
#define GHOST1_NICK "\"AKABEI\""
#endif

#ifndef GHOST3_NAME
#define GHOST3_NAME " OTOBOKE---"
#endif

#ifndef GHOST3_NICK
#define GHOST3_NICK "\"GUZUTA\""
#endif

#endif

#if GHOST_NAMES == GHOST_NAMES_ALT

#ifndef GHOST0_NAME
#define GHOST0_NAME " STYLIST"
#endif

#ifndef GHOST0_NICK
#define GHOST0_NICK "\"MUCKY\""
#endif

#ifndef GHOST2_NAME
#define GHOST2_NAME " ROMP"
#endif

#ifndef GHOST2_NICK
#define GHOST2_NICK "\"MICKY\""
#endif

#ifndef GHOST1_NAME
#define GHOST1_NAME " URCHIN"
#endif

#ifndef GHOST1_NICK
#define GHOST1_NICK "\"MACKY\""
#endif

#ifndef GHOST3_NAME
#define GHOST3_NAME " CRYBABY"
#endif

#ifndef GHOST3_NICK
#define GHOST3_NICK "\"MOCKY\""
#endif

#endif

#if GHOST_NAMES == GHOST_NAMES_EN

#ifndef GHOST0_NAME
#define GHOST0_NAME "-BASHFUL"
#endif

#ifndef GHOST0_NICK
#define GHOST0_NICK "\"INKY\""
#endif

#ifndef GHOST2_NAME
#define GHOST2_NAME "-SPEEDY"
#endif

#ifndef GHOST2_NICK
#define GHOST2_NICK "\"PINKY\""
#endif

#ifndef GHOST1_NAME
#define GHOST1_NAME "-SHADOW"
#endif

#ifndef GHOST1_NICK
#define GHOST1_NICK "\"BLINKY\""
#endif

#ifndef GHOST3_NAME
#define GHOST3_NAME "-POKEY"
#endif

#ifndef GHOST3_NICK
#define GHOST3_NICK "\"CLYDE\""
#endif

#endif

#if GHOST_NAMES == GHOST_NAMES_XLT

#ifndef GHOST0_NAME
#define GHOST0_NAME " WHIMSICAL-"
#endif

#ifndef GHOST0_NICK
#define GHOST0_NICK "---\"CYAN\""
#endif

#ifndef GHOST2_NAME
#define GHOST2_NAME " AMBUSHER--"
#endif

#ifndef GHOST2_NICK
#define GHOST2_NICK "--\"MAUVE\""
#endif

#ifndef GHOST1_NAME
#define GHOST1_NAME " CHASER----"
#endif

#ifndef GHOST1_NICK
#define GHOST1_NICK "\"SCARLET\""
#endif

#ifndef GHOST3_NAME
#define GHOST3_NAME " CLOWN-----"
#endif

#ifndef GHOST3_NICK
#define GHOST3_NICK "--\"SLOTH\""
#endif

#endif

#define ghosts ((GHOSTS > MAXGHOSTS) ? MAXGHOSTS : GHOSTS)

#define WHOSE_HOME_DIR(r,c) (((unsigned) home_dir[(GHOST2 % ghosts*maze_h+(r))*(maze_w+1)+(c)]) ? GHOST2 \
                             : ((unsigned) home_dir[(GHOST0 % ghosts*maze_h+(r))*(maze_w+1)+(c)]) ? GHOST0 \
                             : ((unsigned) home_dir[(GHOST3 % ghosts*maze_h+(r))*(maze_w+1)+(c)]) ? GHOST3 \
                             : GHOST1)

#define rmsg (RMSG % maze_h)
#define cmsg (CMSG % maze_w)
#define rmsg2 (RMSG2 % maze_h)
#define cmsg2 (CMSG2 % maze_w)

#define COMPLEXITY_ADJUST(x) ((x) * (maze_h + maze_w) / (28 + 31))

#define TWOSECS    (10 * MYMANFIFTH)
#define ONESEC     (TWOSECS / 2)
#define FRUITLIFE  COMPLEXITY_ADJUST(TWOSECS * 15 / 4)
#define DEATHSHIFT 3
#define DEATHDELAY (1 << (DEATHSHIFT + 2))
#define MEMDELAY(ghost) (COMPLEXITY_ADJUST((3 + ((ghost) > 3) * (1.0 * ((ghost) - 3) / (ghosts - 3))) * TWOSECS))

#ifndef MAXFRAMESKIP
#define MAXFRAMESKIP (1 + tile_w)
#endif

#define MYMANSQUARE ((tile_h + tile_h) > tile_w)

#define MYMANFIFTH     (tile_w * (MYMANSQUARE ? 2 : 1))

#define PELLET_ADJUST(x) (COMPLEXITY_ADJUST(x * 4) / (pellets[maze_level] ? pellets[maze_level] : 4))

#ifndef MYMANDELAY
#define MYMANDELAY 166667
#endif

#ifndef SPEEDUP
#define SPEEDUP 1000
#endif

#define CJK_MODE (uni_cp437[0x20] == uni_cp437_fullwidth[0x20])

#ifndef CHERO
#define CHERO (maze_CHERO_len ? maze_CHERO[maze_level % maze_CHERO_len] : (maze_w * 0.5))
#endif

#ifndef RHERO
#define RHERO (maze_RHERO_len ? maze_RHERO[maze_level % maze_RHERO_len] : 23.5)
#endif

#ifndef CFRUIT
#define CFRUIT (maze_CFRUIT_len ? maze_CFRUIT[maze_level % maze_CFRUIT_len] : CHERO)
#endif

#ifndef RFRUIT
#define RFRUIT (maze_RFRUIT_len ? maze_RFRUIT[maze_level % maze_RFRUIT_len] : 17.5)
#endif

#ifndef RGHOST
#define RGHOST (maze_RGHOST_len ? maze_RGHOST[maze_level % maze_RGHOST_len] : (RFRUIT - 3.0))
#endif

#ifndef CGHOST
#define CGHOST (maze_CGHOST_len ? maze_CGHOST[maze_level % maze_CGHOST_len] : CHERO)
#endif

#ifndef COGHOST
#define COGHOST (maze_COGHOST_len ? maze_COGHOST[maze_level % maze_COGHOST_len] : 2)
#endif

#ifndef ROGHOST
#define ROGHOST (maze_ROGHOST_len ? maze_ROGHOST[maze_level % maze_ROGHOST_len] : 3)
#endif

#ifndef RTOP
#define RTOP (maze_RTOP_len ? maze_RTOP[maze_level % maze_RTOP_len] : (RFRUIT - 5.0))
#endif

#ifndef CMSG
#define CMSG (maze_CMSG_len ? maze_CMSG[maze_level % maze_CMSG_len] : 9)
#endif

#ifndef RMSG
#define RMSG (maze_RMSG_len ? maze_RMSG[maze_level % maze_RMSG_len] : 17)
#endif

#ifndef CMSG2
#define CMSG2 (maze_CMSG2_len ? maze_CMSG2[maze_level % maze_CMSG2_len] : 9)
#endif

#ifndef RMSG2
#define RMSG2 (maze_RMSG2_len ? maze_RMSG2[maze_level % maze_RMSG2_len] : 11)
#endif

#ifndef PLAYER1
#define PLAYER1  "PLAYER ONE"
#endif

#ifndef PLAYER2
#define PLAYER2  "PLAYER TWO"
#endif

#ifndef MAXPLAYERS
#define MAXPLAYERS 2
#endif

#define PLAYER(n) \
(((n) == 1) \
? msg_PLAYER1 \
: msg_PLAYER2)

#ifndef START
#define START "PUSH START BUTTON"
#endif

#ifndef CREDIT1
#define CREDIT1 "1 PLAYER ONLY"
#endif

#ifndef CREDIT2
#define CREDIT2 "1 OR 2 PLAYERS"
#endif

#define CREDIT(n) (((n) > 1) ? CREDIT1 : CREDIT1)

#ifndef READY
#define READY    "  READY!  "
#endif

#ifndef GAMEOVER
#define GAMEOVER "GAME  OVER"
#endif

#ifndef PAUSE
#define PAUSE " - PAUSED - "
#endif

#define BONUS(n) \
(((n) < 4) \
? ((n) - 1) \
: (((n) < 15) \
? ((n) + 1) / 2 \
: 7))

extern char *tmp_notice;
extern const char * maze_ABOUT;
extern const char * maze_FIXME;
extern const char * maze_NOTE;
extern const char * tile_ABOUT;
extern const char * tile_FIXME;
extern const char * tile_NOTE;
extern const char * sprite_ABOUT;
extern const char * sprite_FIXME;
extern const char * sprite_NOTE;
extern const char *msg_READY;
extern const char *msg_GAMEOVER;
extern const char *msg_PLAYER1;
extern const char *msg_PLAYER2;
extern const char *maze_WALL_COLORS;
extern size_t maze_WALL_COLORS_len;
extern const char *maze_DOT_COLORS;
extern size_t maze_DOT_COLORS_len;
extern const char *maze_PELLET_COLORS;
extern size_t maze_PELLET_COLORS_len;
extern const char *maze_MORTAR_COLORS;
extern size_t maze_MORTAR_COLORS_len;
extern long *maze_GHOSTS;
extern size_t maze_GHOSTS_len;
extern double *maze_RGHOST;
extern size_t maze_RGHOST_len;
extern double *maze_CGHOST;
extern size_t maze_CGHOST_len;
extern double *maze_ROGHOST;
extern size_t maze_ROGHOST_len;
extern double *maze_COGHOST;
extern size_t maze_COGHOST_len;
extern double *maze_RFRUIT;
extern size_t maze_RFRUIT_len;
extern double *maze_CFRUIT;
extern size_t maze_CFRUIT_len;
extern double *maze_RTOP;
extern size_t maze_RTOP_len;
extern double *maze_RHERO;
extern size_t maze_RHERO_len;
extern double *maze_CHERO;
extern size_t maze_CHERO_len;
extern long *maze_RMSG;
extern size_t maze_RMSG_len;
extern long *maze_CMSG;
extern size_t maze_CMSG_len;
extern long *maze_RMSG2;
extern size_t maze_RMSG2_len;
extern long *maze_CMSG2;
extern size_t maze_CMSG2_len;
extern int dirhero;
extern long scroll_offset_x0;
extern long scroll_offset_y0;
extern int msglen;
extern int hero_dir;
extern int *total_dots;
extern int *pellets;
extern long flip_to;
extern int debug;
extern int ghosts_p;
extern unsigned long myman_sfx;
extern int showlives;
extern int visible_frame;

#define myman_sfx_credit 0x1UL
#define myman_sfx_dot 0x2UL
#define myman_sfx_dying 0x4UL
#define myman_sfx_ghost 0x8UL
#define myman_sfx_intermission 0x10UL
#define myman_sfx_pellet 0x20UL
#define myman_sfx_siren0_down 0x40UL
#define myman_sfx_siren0_up 0x80UL
#define myman_sfx_siren1_down 0x100UL
#define myman_sfx_siren1_up 0x200UL
#define myman_sfx_siren2_down 0x400UL
#define myman_sfx_siren2_up 0x800UL
#define myman_sfx_start 0x1000UL
#define myman_sfx_fruit 0x2000UL
#define myman_sfx_life 0x4000UL
#define myman_sfx_level 0x8000UL
#define myman_sfx_bonus 0x10000UL
#define myman_sfx_nobeep_mask (myman_sfx_siren0_up|myman_sfx_siren0_down|myman_sfx_siren1_up|myman_sfx_siren1_down|myman_sfx_siren2_up|myman_sfx_siren2_down|myman_sfx_dot)

#ifndef WALL_COLORS
#define WALL_COLORS "\x00"
#endif

#ifndef DOT_COLORS
#define DOT_COLORS "\x07"
#endif

#ifndef PELLET_COLORS
#define PELLET_COLORS "\x00"
#endif

#ifndef MORTAR_COLORS
#define MORTAR_COLORS "\x09"
#endif

#ifndef WALL_COLOR
#define WALL_COLOR maze_WALL_COLORS[maze_level % maze_WALL_COLORS_len]
#endif

#ifndef DOT_COLOR
#define DOT_COLOR maze_DOT_COLORS[maze_level % maze_DOT_COLORS_len]
#endif

#ifndef PELLET_COLOR
#define PELLET_COLOR maze_PELLET_COLORS[maze_level % maze_PELLET_COLORS_len]
#endif

#ifndef MORTAR_COLOR
#define MORTAR_COLOR maze_MORTAR_COLORS[maze_level % maze_MORTAR_COLORS_len]
#endif

#define TRANSLATED_MORTAR_COLOR (((BICOLOR_WALLS) && (((unsigned) udlr[(unsigned) (unsigned char) (char) maze[(maze_level*maze_h+ytile)*(maze_w+1)+xtile]]) & 0xAA)) ? (WALL_COLOR) : (MORTAR_COLOR))

#define TRANSLATED_WALL_COLOR ((BICOLOR_WALLS) ? TRANSLATED_MORTAR_COLOR : (WALL_COLOR))

#define EFFECTIVE_MORTAR_COLOR (((TRANSLATED_WALL_COLOR) && ! (SOLID_WALLS || SOLID_WALLS_BGCOLOR)) ? (TRANSLATED_WALL_COLOR) : (TRANSLATED_MORTAR_COLOR))

#define NET_LIVES ((int) lives + (int) earned - (int) lives_used + (myman_demo ? 1 : 0))

#ifndef MSG_COLOR
#define MSG_COLOR ((NET_LIVES && ! myman_demo) ? 0xE : 0xC)
#endif

#ifndef MSG2_COLOR
#define MSG2_COLOR 0xB
#endif

#ifndef TEXT_COLOR
#define TEXT_COLOR 0xF
#endif

#ifndef PAUSE_COLOR
#define PAUSE_COLOR 0xF0
#endif

#ifndef EXTRA_GHOST_COLORS
#define EXTRA_GHOST_COLORS "\x0A\x05\x04\x03"
#endif

extern int my_clear(void);
extern void my_clearok(int ok);

#ifndef BONUSHERO
#define BONUSHERO 10000
#endif

#ifndef BONUSHERO2
#define BONUSHERO2 50000
#endif

#ifndef BONUSHEROTEXT
#if BONUSHERO
#define BONUSHEROTEXT "BONUS MYMAN FOR 10000 \x9es"
#else
#define BONUSHEROTEXT "BONUS MYMAN FOR 50000 \x9es"
#endif
#endif

extern int use_underline;
extern int use_color;
extern int bonus_score[8];

#define COLLISION_TYPE_HERO 1
#define COLLISION_TYPE_GHOST 2

extern int check_collision(int eyes, int mean, int blue);

extern int find_home_dir(int s, int r, int c);

/* heuristic for rewriting maze tiles */
extern long maze_visual(int n, int i, int j);

extern int myman_setenv(const char *name, const char *value);

extern char *myman_getenv(const char *name);

#endif /* ! defined(MYMAN_UTILS_H_INCLUDED) */
