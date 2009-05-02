/* logic.c - various game logic for the MyMan video game
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

/* configuration information */
#ifdef HAVE_CONFIG_H
#ifndef MYMAN_CONFIG_H_INCLUDED
#include "config.h"
#endif
#endif

/* feature guessing */
#ifndef MYMAN_GUESS_H_INCLUDED
#include "guess.h"
#endif

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef EINVAL
#ifdef EARG
#define EINVAL EARG
#else
#define EINVAL EDOM
#endif
#endif

#ifdef macintosh
#if TARGET_API_MAC_CARBON
/* Case 1: flat Carbon headers */
#include <Carbon.h>
#else /* ! defined(TARGET_API_MAC_CARBON) */
/* Case 2: Toolbox */
#include <ConditionalMacros.h>
#if defined(UNIVERSAL_INTERFACES_VERSION) && (UNIVERSAL_INTERFACES_VERSION >= 0x0300)
#include <MacTypes.h>
#else
#include <Types.h>
#endif
#include <Files.h>
#endif /* ! defined(TARGET_API_MAC_CARBON) */
#endif /* ! defined(macintosh) */

#ifdef VMS
#include <unixlib.h>
#endif /* defined(VMS) */

/* MyMan utilities; also defines cruft like __MSDOS__ under some circumstances */
#ifndef MYMAN_UTILS_H_INCLUDED
#include "utils.h"
#endif

/* command-line argument parser */
#ifndef MYGETOPT_H
#ifdef MYGETOPT
#define MYGETOPT_H "getopt.h"
#endif
#endif

#ifdef MYGETOPT_H
#include MYGETOPT_H
#else
#include <getopt.h>
#endif

#ifndef HUGE_VAL
#define HUGE_VAL 1e500
#endif

static void
ghost_eaten_timer_expired(void)
{
    sprite_register_used[GHOST_SCORE] = 0;
    sprite_register_frame[GHOST_SCORE] ++;
    if ((((unsigned) sprite_register[GHOST_SCORE]) + sprite_register_frame[GHOST_SCORE]) > SPRITE_1600) sprite_register_frame[GHOST_SCORE] = SPRITE_1600 - ((unsigned) sprite_register[GHOST_SCORE]);
    if (sprite_register_used[HERO])
    {
        memcpy((void *) (maze + (maze_level * maze_h + rmsg) * (maze_w + 1) + cmsg),
               (void *) (blank_maze + (maze_level * maze_h + rmsg) * (maze_w + 1) + cmsg),
               MIN(msglen, maze_w - cmsg));
        memcpy((void *) (maze_color + (maze_level * maze_h + rmsg) * (maze_w + 1) + cmsg),
               (void *) (blank_maze_color + (maze_level * maze_h + rmsg) * (maze_w + 1) + cmsg),
               MIN(msglen, maze_w - cmsg));
        {
            int dirty_i;
            for (dirty_i = 0; dirty_i < msglen; dirty_i ++)
            {
                mark_cell(cmsg + dirty_i, rmsg);
            }
        }
        myman_sfx |= myman_sfx_siren0_up;
    }
    else
    {
        sprite_register_used[HERO] = NET_LIVES ? 1 : 0;
        sprite_register_used[munched] = (NET_LIVES || (munched != HERO)) ? 1 : 0;
    }
}

static int
check_level_transition(void)
{
    int reset = 0;
    static int init = 0;
    int i;

    if (! need_reset)
    {
        if (dying && ! -- dying) {
            reset = 1;
            if (! myman_demo) ++ lives_used;
        } else if ((! ghost_eaten_timer) && winning && ! -- winning) {
            need_reset = 1;
            if (INTERMISSION(level)
                &&
                (intermission < INTERMISSION_N))
            {
                intermission_running = (1 + INTERMISSION_TIME);
                key_buffer = key_buffer_ERR;
                return 1;
            }
        }
    }
    if (need_reset)
    {
        myman_sfx |= myman_sfx_start;
        need_reset = 0;
        reset = 1;
        memcpy((void *)maze,
               (void *)blank_maze,
               (maze_w + 1) * maze_h * maze_n * sizeof(unsigned char));
        memcpy((void *)maze_color,
               (void *)blank_maze_color,
               (maze_w + 1) * maze_h * maze_n * sizeof(unsigned char));
        DIRTY_ALL();
        ignore_delay = 1;
        frameskip = 0;
        dots = 0;
        pellet_timer = 0;
        pellet_time -= PELLET_ADJUST(7 * ONESEC);
        if (level && (FLIP_ALWAYS ||
                      INTERMISSION(level)))
        {
            ++ maze_level;
            maze_level %= maze_n;
            if (! maze_level)
            {
                maze_level = flip_to % maze_n;
            }
            if (FLIP_LOCK && ! maze_level)
            {
                maze_level = maze_n - 1;
            }
        }
        if (INTERMISSION(level) &&
            (intermission < INTERMISSION_N))
        {
            ++ intermission_shown;
            if (intermission_shown >= INTERMISSION_REPEAT(intermission))
            {
                ++ intermission;
                intermission_shown = 0;
            }
        }
        ++ level;
        sprite_register_frame[FRUIT] =
            sprite_register_frame[FRUIT_SCORE] = BONUS(level);
        pellet_time += PELLET_ADJUST(7 * ONESEC);
        if (pellet_time > PELLET_ADJUST(ONESEC))
            pellet_time -= PELLET_ADJUST(ONESEC);
        else
            pellet_time = 0;
        if (! myman_demo)
        {
            if ((mymandelay > SPEEDUP)
                &&
                (mymandelay >= (mindelay + SPEEDUP)))
            {
                mymandelay -= SPEEDUP;
            }
        }
    }
    if ((! init) && (winning || reset)) {
        init = 1;
        if (sprite_register_used[FRUIT])
        {
            DIRTY_ALL();
            ignore_delay = 1;
            frameskip = 0;
        }
        sprite_register_used[FRUIT] =
            sprite_register_used[FRUIT_SCORE] =
            sprite_register_used[GHOST_SCORE] = 0;
        for (i = 0; i < ghosts; i++) {
            int eyes, mean, blue;

            eyes = GHOSTEYES(i);
            mean = MEANGHOST(i);
            blue = BLUEGHOST(i);
            sprite_register_used[eyes] =
                sprite_register_used[mean] =
                sprite_register_used[blue] = 0;
        }
        sprite_register_frame[HERO] = 0;
    }
    if (reset) {
        memset((void *)home_dir, 0, sizeof(home_dir));
        for (i = 0; i < ghosts; i++) {
            int eyes, mean, blue;

            eyes = GHOSTEYES(i);
            mean = MEANGHOST(i);
            blue = BLUEGHOST(i);
            sprite_register_used[eyes] =
                sprite_register_used[mean] =
                sprite_register_used[blue] = 0;
            sprite_register_frame[eyes] = (ghost_dir[i] = DIRWRAP(i + 1)) - 1;
            ghost_mem[i] = 0;
            ghost_timer[i] = TWOSECS;
            ghost_man[i] = 0;
            sprite_register_x[mean] =
                (sprite_register_x[eyes] = (int) (XGHOST + COGHOST * gfx_w *
                                                  ((i == GHOST3) - (i == GHOST0))));
            sprite_register_y[mean] =
                (sprite_register_y[eyes] = (int) (YGHOST - ROGHOST * gfx_h *
                                                  (i == GHOST1)));
        }
        key_buffer = key_buffer_ERR;
        init = 0;
        cycles = 0;
        sprite_register_used[HERO] = 0;
        sprite_register_frame[HERO] = 0;
        sprite_register_x[HERO] = (int) XHERO;
        sprite_register_y[HERO] = (int) YHERO;
        dead =
            deadpan = (int) (YHERO - YGHOST);
        if (dead < 0)
            dead = -dead;
        if (dead < 2)
            dead = 2;
        maze_putsn_nonblank(rmsg, cmsg, MSG_COLOR, (NET_LIVES && ! myman_demo) ? msg_READY : msg_GAMEOVER, msglen);
        if (player != oldplayer)
        {
            oldplayer = player;
            ghost_eaten_timer = myman_demo ? 0 : ONESEC;
            munched = HERO;
            if (! myman_demo)
            {
                maze_putsn_nonblank(rmsg2, cmsg2, MSG2_COLOR, PLAYER(player), msglen);
            }
        }
    }
    if (dying)
        sprite_register_frame[HERO] = ((DEATHDELAY - dying) >> DEATHSHIFT) % 4;
    return 0;
}

static void
reset_hero(void)
{
    int i;

    if (deadpan)
        deadpan += (YHERO < YGHOST) - (YHERO > YGHOST);
    if (! -- dead) {
        for (i = 0; i < ghosts; i ++) {
            int eyes, mean, blue;

            eyes = GHOSTEYES(i);
            mean = MEANGHOST(i);
            blue = BLUEGHOST(i);
            sprite_register_used[eyes] = NET_LIVES ? VISIBLE_EYES : 0;
            sprite_register_used[mean] = NET_LIVES ? 1 : 0;
            sprite_register_used[blue] = 0;
        }
        sprite_register_used[HERO] = NET_LIVES ? 1 : 0;
        ghost_eaten_timer = ONESEC;
        munched = HERO;
        if (! myman_demo)
        {
            memcpy((void *) (maze + (maze_level * maze_h + rmsg2) * (maze_w + 1) + cmsg2),
                   (void *) (blank_maze + (maze_level * maze_h + rmsg2) * (maze_w + 1) + cmsg2),
                   MIN(msglen, maze_w - cmsg2));
            memcpy((void *) (maze_color + (maze_level * maze_h + rmsg2) * (maze_w + 1) + cmsg2),
                   (void *) (blank_maze_color + (maze_level * maze_h + rmsg2) * (maze_w + 1) + cmsg2),
                   MIN(msglen, maze_w - cmsg2));
            {
                int dirty_i;
                for (dirty_i = 0; dirty_i < msglen; dirty_i ++)
                {
                    mark_cell(cmsg2 + dirty_i, rmsg2);
                }
            }
        }
        hero_dir = dirhero;
        sprite_register[HERO] = SPRITE_HERO + ((hero_dir == MYMAN_LEFT) ? 4 : (hero_dir == MYMAN_RIGHT) ? 12 : (hero_dir == MYMAN_DOWN) ? 16 : 0);
        if (! use_color)
            if (use_underline)
            {
                my_clear();
                my_clearok(1);
                DIRTY_ALL();
                ignore_delay = 1;
                frameskip = 0;
                visible_frame = 1;
            }
    }
}

int
gamelogic(void)
{
    int xtile, ytile;
    int x_off, y_off;
    long c = 0;
    int i, j;
    int s;
    int collision_type = 0;

    xtile = XTILE(sprite_register_x[HERO]);
    ytile = YTILE(sprite_register_y[HERO]);
    x_off = sprite_register_x[HERO] % gfx_w;
    y_off = sprite_register_y[HERO] % gfx_h;
    if (ghost_eaten_timer && ! -- ghost_eaten_timer)
    {
        ghost_eaten_timer_expired();
    }
    if (winning || dying || need_reset)
    {
        return check_level_transition();
    }
    else if (dead && ! ghost_eaten_timer)
    {
        reset_hero();
    }
    else
    {
        if ((BONUSHERO && (score >= BONUSHERO) && ! earned) ||
            ((score >= BONUSHERO2) && (earned == (BONUSHERO ? 1 : 0)))) {
            if (! myman_demo)
            {
                earned ++;
            }
        }
        if (ISPELLET((c = (long) (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile]))
            || ISDOT(c)) {
            maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile] = ' ';
            sprite_register_frame[HERO] = 0;
            if (! myman_demo) score += 10 + 40 * ISPELLET(c);
            if (ISPELLET(c)) {
                myman_sfx |= myman_sfx_pellet;
                points = 200;
                sprite_register_frame[GHOST_SCORE] = 0;
                pellet_timer = pellet_time + 1;
                for (s = 0; s < ghosts; s++) {
                    int eyes, mean, blue;

                    eyes = GHOSTEYES(s);
                    blue = BLUEGHOST(s);
                    mean = MEANGHOST(s);
                    sprite_register_frame[blue] = 0;
                    if (sprite_register_used[mean] || sprite_register_used[blue]) {
                        if (sprite_register_used[mean]) {
                            sprite_register_x[blue] = sprite_register_x[eyes];
                            sprite_register_y[blue] = sprite_register_y[eyes];
                        }
                        sprite_register_used[eyes] = 0;
                        sprite_register_used[mean] = 0;
                        sprite_register_used[blue] = 1;
                        sprite_register[blue] = SPRITE_BLUE;
                        ghost_dir[s] = DIRWRAP(ghost_dir[s] + 2);
                    }
                }
            }
            else
            {
                myman_sfx |= myman_sfx_dot;
            }
#ifndef DOTS_FRUIT1
#define DOTS_FRUIT1 (70 * total_dots[maze_level] / 244)
#endif
#ifndef DOTS_FRUIT2
#define DOTS_FRUIT2 (170 * total_dots[maze_level] / 244)
#endif
            if ((++ dots == DOTS_FRUIT1) || (dots == DOTS_FRUIT2))
            {
                sprite_register_x[FRUIT] =
                    sprite_register_x[FRUIT_SCORE] = (int) XFRUIT;
                sprite_register_y[FRUIT] =
                    sprite_register_y[FRUIT_SCORE] = (int) YFRUIT;
                sprite_register_used[FRUIT] = 1;
                sprite_register_timer[FRUIT] = FRUITLIFE;
                myman_sfx |= myman_sfx_bonus;
            } else if (dots == total_dots[maze_level]) {
                munched = HERO;
                winning = 2 * TWOSECS;
                myman_sfx |= myman_sfx_level;
            }
        } else if (sprite_register_used[FRUIT] &&
                   collide(HERO, FRUIT))
        {
            DIRTY_ALL();
            ignore_delay = 1;
            frameskip = 0;
            sprite_register_used[FRUIT] = 0;
            sprite_register_used[FRUIT_SCORE] = 1;
            sprite_register_timer[FRUIT_SCORE] = TWOSECS;
            if (! myman_demo) score += bonus_score[sprite_register_frame[FRUIT]];
            sprite_register_frame[HERO] = 0;
            myman_sfx |= myman_sfx_fruit;
        }
        if (! ghost_eaten_timer)
        {
            if (XDIR(hero_dir))
                sprite_register_y[HERO] = (1 + 2 * ytile) * gfx_h / 2;
            else if (hero_dir)
                sprite_register_x[HERO] = (1 + 2 * xtile) * gfx_w / 2;
            if (ISZAPLEFT((unsigned) (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile])
                &&
                (! YDIR(hero_dir))
                &&
                (hero_dir == MYMAN_LEFT)) {
                int ii;

                for (ii = 1; ii < maze_w; ii ++)
                {
                    if (ISZAPRIGHT((unsigned) (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile - ii)]))
                    {
                        break;
                    }
                }
                sprite_register_x[HERO] = (1 + 2 * XWRAP(xtile - ii)) * gfx_w / 2;
            }
            else if (ISZAPRIGHT((unsigned) (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile])
                     &&
                     (! YDIR(hero_dir))
                     &&
                     (hero_dir == MYMAN_RIGHT)) {
                int ii;

                for (ii = 1; ii < maze_w; ii ++)
                {
                    if (ISZAPLEFT((unsigned) (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile + ii)]))
                    {
                        break;
                    }
                }
                sprite_register_x[HERO] = (1 + 2 * XWRAP(xtile + ii)) * gfx_w / 2;
            }
            else if (ISZAPUP((unsigned) (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile])
                     &&
                     (! XDIR(hero_dir))
                     &&
                     (hero_dir == MYMAN_UP)) {
                int ii;

                for (ii = 1; ii < maze_h; ii ++)
                {
                    if (ISZAPDOWN((unsigned) (unsigned char) maze[(maze_level*maze_h+YWRAP(ytile - ii)) * (maze_w + 1)+xtile]))
                    {
                        break;
                    }
                }
                sprite_register_y[HERO] = (1 + 2 * YWRAP(ytile - ii)) * gfx_h / 2;
            }
            else if (ISZAPDOWN((unsigned) (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+xtile])
                     &&
                     (! XDIR(hero_dir))
                     &&
                     (hero_dir == MYMAN_DOWN)) {
                int ii;

                for (ii = 1; ii < maze_h; ii ++)
                {
                    if (ISZAPUP((unsigned) (unsigned char) maze[(maze_level*maze_h+YWRAP(ytile + ii)) * (maze_w + 1)+xtile]))
                    {
                        break;
                    }
                }
                sprite_register_y[HERO] = (1 + 2 * YWRAP(ytile + ii)) * gfx_h / 2;
            }
            else
            {
                unsigned char m3;
                int x3, y3;

                x3 = xtile + XLEAVING(hero_dir, x_off + XDIR(hero_dir));
                y3 = ytile + YLEAVING(hero_dir, y_off + YDIR(hero_dir));
                m3 = (unsigned char)
                    maze[(maze_level * maze_h
                          +
                          YWRAP(y3))
                         *
                         (maze_w + 1)
                         +
                         XWRAP(x3)];
                if (ISOPEN((unsigned) m3))
                {
                    sprite_register_x[HERO] = XPIXWRAP(sprite_register_x[HERO] + XDIR(hero_dir) * ((MYMANSQUARE || gfx_reflect) ? (cycles & 1) : 1));
                    sprite_register_y[HERO] = YPIXWRAP(sprite_register_y[HERO] + YDIR(hero_dir) * ((gfx_reflect && ! MYMANSQUARE) ? 1 : (cycles & 1)));
                    sprite_register_frame[HERO] = (cycles / (1 + (MYMANFIFTH / 2))) % 4;
                }
            }
        }
        for (s = 0; s < ghosts; s ++) {
            int eyes, blue, mean;

            eyes = GHOSTEYES(s);
            blue = BLUEGHOST(s);
            mean = MEANGHOST(s);
            if ((! ghost_eaten_timer) || ((munched != eyes) && (munched != HERO)))
                sprite_register_frame[blue] =
                    (sprite_register_frame[mean] = ((cycles / MYMANFIFTH) & 1) ? 1 : 0);
            if (dying || winning) {
                sprite_register_used[eyes] =
                    (sprite_register_used[blue] =
                     (sprite_register_used[mean] = 0));
            }
            else if ((collision_type = check_collision(eyes, mean, blue)) != 0)
            {
                if (collision_type == COLLISION_TYPE_HERO)
                {
                    s = -1;
                    continue;
                }
            }
            else
            {
                int dir0, dir1, dir2, o0, o1, o2, x, y, i1, j1, i2, j2;
                unsigned char mcell;

                dir1 = DIRWRAP(ghost_dir[s]);
                dir0 = DIRWRAP(dir1 + (s != GHOST2) - (s == GHOST2));
                dir2 = DIRWRAP(dir1 - (s != GHOST2) + (s == GHOST2));
                if (sprite_register_used[blue] && ! ghost_eaten_timer)
                {
                    /* runnin' scared */
                    if (cycles & 1)
                    {
                        x = sprite_register_x[blue];
                        y = sprite_register_y[blue];
                        i1 = XTILE(x);
                        j1 = YTILE(y);
                        i2 = XTILE(sprite_register_x[HERO]);
                        j2 = YTILE(sprite_register_y[HERO]);
                        if ((i1 == i2) && (j1 <= j2)) {
                            for (j = j1; j <= j2; j ++)
                            {
                                mcell = (unsigned char) maze[(maze_level*maze_h+j) * (maze_w + 1)+i1];
                                if (! ISOPEN((unsigned) mcell))
                                    break;
                            }
                            if (j > j2) {
                                ghost_mem[s] = hero_dir;
                                ghost_timer[s] = (int) MEMDELAY(s);
                            }
                        } else if ((i1 == i2) && (j1 >= j2)) {
                            for (j = j2; j <= j1; j ++)
                            {
                                mcell = (unsigned char) maze[(maze_level*maze_h+j) * (maze_w + 1)+i1];
                                if (! ISOPEN((unsigned) mcell))
                                    break;
                            }
                            if (j > j1) {
                                ghost_mem[s] = hero_dir;
                                ghost_timer[s] = (int) MEMDELAY(s);
                            }
                        } else if ((j1 == j2) && (i1 <= i2)) {
                            for (i = i1; i <= i2; i ++)
                            {
                                mcell = (unsigned char) maze[(maze_level*maze_h+j1) * (maze_w + 1)+i];
                                if (! ISOPEN((unsigned) mcell))
                                    break;
                            }
                            if (i > i2) {
                                ghost_mem[s] = hero_dir;
                                ghost_timer[s] = (int) MEMDELAY(s);
                            }
                        } else if ((j1 == j2) && (i1 >= i2)) {
                            for (i = i2; i <= i1; i ++)
                            {
                                mcell = (unsigned char) maze[(maze_level*maze_h+j1) * (maze_w + 1)+i];
                                if (! ISOPEN((unsigned) mcell))
                                    break;
                            }
                            if (i > i1) {
                                ghost_mem[s] = hero_dir;
                                ghost_timer[s] = (int) MEMDELAY(s);
                            }
                        }
                        mcell = (unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir0))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir0))];
                        o0 = ISOPEN((unsigned) mcell);
                        mcell = (unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir1))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir1))];
                        o1 = ISOPEN((unsigned) mcell);
                        mcell = (unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir2))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir2))];
                        o2 = ISOPEN((unsigned) mcell);
                        if (((gfx_w / 2 == x % gfx_w) && XDIR(dir1))
                            || ((gfx_h / 2 == y % gfx_h) && YDIR(dir1))) {
                            if (o0 || o2) {
                                if (! ghost_mem[s]) {
                                    if (o2)
                                        dir1 = dir2;
                                    else if (! o1)
                                        dir1 = dir0;
                                }
                                else
                                {
                                    if (o0 && (dir0 != ghost_mem[s]))
                                        dir1 = dir0;
                                    else if (o2 && (dir2 != ghost_mem[s]))
                                        dir1 = dir2;
                                    else if (o1 && (dir1 != ghost_mem[s]))
                                        ;
                                    else if (o0)
                                        dir1 = dir0;
                                    else
                                        dir1 = dir2;
                                }
                            } else if (! o1)
                                dir1 = DIRWRAP(dir1 + 2);
                        }
                        if (XDIR(dir1))
                            y = (1 + 2 * j1) * gfx_h / 2;
                        else if (dir1)
                            x = (1 + 2 * i1) * gfx_w / 2;
                        if ((MYMANSQUARE || gfx_reflect) ? (cycles & 2) : 1)
                            sprite_register_x[blue] = XPIXWRAP(x + XDIR(dir1));
                        if ((gfx_reflect && ! MYMANSQUARE) ? 1 : (cycles & 2))
                            sprite_register_y[blue] = YPIXWRAP(y + YDIR(dir1));
                        if (! (unsigned) home_dir[(s % ghosts*maze_h+YTILE(sprite_register_y[blue]))*(maze_w+1)+XTILE(sprite_register_x[blue])])
                            home_dir[(s % ghosts*maze_h+YTILE(sprite_register_y[blue]))*(maze_w+1)+XTILE(sprite_register_x[blue])] =
                                DIRWRAP(dir1 + 2);
                    }
                }
                else if (sprite_register_used[mean] && ! ghost_eaten_timer)
                {
                    /* out huntin' */
                    int d0, d1, d2;

                    x = sprite_register_x[mean];
                    y = sprite_register_y[mean];
                    i1 = XTILE(x);
                    j1 = YTILE(y);
                    i2 = XTILE(sprite_register_x[HERO]);
                    j2 = YTILE(sprite_register_y[HERO]);
                    if ((i1 == i2) && (j1 <= j2)) {
                        for (j = j1; j <= j2; j ++)
                        {
                            mcell = (unsigned char) maze[(maze_level*maze_h+j) * (maze_w + 1)+i1];
                            if (! ISOPEN((unsigned) mcell))
                                break;
                        }
                        if (j > j2) {
                            ghost_mem[s] = hero_dir;
                            ghost_timer[s] = (int) MEMDELAY(s);
                        }
                    } else if ((i1 == i2) && (j1 >= j2)) {
                        for (j = j2; j <= j1; j ++)
                        {
                            mcell = (unsigned char) maze[(maze_level*maze_h+j) * (maze_w + 1)+i1];
                            if (! ISOPEN((unsigned) mcell))
                                break;
                        }
                        if (j > j1) {
                            ghost_mem[s] = hero_dir;
                            ghost_timer[s] = (int) MEMDELAY(s);
                        }
                    } else if ((j1 == j2) && (i1 <= i2)) {
                        for (i = i1; i <= i2; i ++)
                        {
                            mcell = (unsigned char) maze[(maze_level*maze_h+j1) * (maze_w + 1)+i];
                            if (! ISOPEN((unsigned) mcell))
                                break;
                        }
                        if (i > i2) {
                            ghost_mem[s] = hero_dir;
                            ghost_timer[s] = (int) MEMDELAY(s);
                        }
                    } else if ((j1 == j2) && (i1 >= i2)) {
                        for (i = i2; i <= i1; i ++)
                        {
                            mcell = (unsigned char) maze[(maze_level*maze_h+j1) * (maze_w + 1)+i];
                            if (! ISOPEN((unsigned) mcell))
                                break;
                        }
                        if (i > i1) {
                            ghost_mem[s] = hero_dir;
                            ghost_timer[s] = (int) MEMDELAY(s);
                        }
                    }
                    mcell = (unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir0))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir0))];
                    o0 = ISOPEN((unsigned) mcell);
                    mcell = (unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir1))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir1))];
                    o1 = ISOPEN((unsigned) mcell);
                    mcell = (unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir2))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir2))];
                    o2 = ISOPEN((unsigned) mcell);
                    d0 = d2 = 0;
                    mcell = (unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir1))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir1))];
                    d1 = ISDOOR((unsigned) mcell);
                    mcell = (unsigned char) maze[(maze_level*maze_h+j1) * (maze_w + 1)+i1];
                    if (! ISDOOR((unsigned) mcell))
                    {
                        mcell = (unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir0))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir0))];
                        d0 = ISDOOR((unsigned) mcell);
                        mcell = (unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir2))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir2))];
                        d2 = ISDOOR((unsigned) mcell);
                    }
                    d0 = d0 && (dir0 !=
                                find_home_dir(s,
                                              YWRAP(j1 + YDIR(dir0)),
                                              XWRAP(i1 + XDIR(dir0))))
                        && dir0 != MYMAN_DOWN
                        && dir0 != MYMAN_LEFT;
                    d2 = d2 && (dir2 !=
                                find_home_dir(s,
                                              YWRAP(j1 + YDIR(dir2)),
                                              XWRAP(i1 + XDIR(dir2))))
                        && dir2 != MYMAN_DOWN
                        && dir2 != MYMAN_LEFT;
                    d1 = d1 && (dir1 !=
                                find_home_dir(s,
                                              YWRAP(j1 + YDIR(dir1)),
                                              XWRAP(i1 + XDIR(dir1))))
                        && dir1 != MYMAN_DOWN
                        && dir1 != MYMAN_LEFT;
                    if (((gfx_w / 2 == x % gfx_w) && XDIR(dir1))
                        || ((gfx_h / 2 == y % gfx_h) && YDIR(dir1))) {
                        if (d2 || (o2 && (dir2 == ghost_mem[s])))
                            dir1 = dir2;
                        else if (d1 || (o1 && (dir1 == ghost_mem[s])))
                            ;
                        else if (d0 || (o0 && (dir0 == ghost_mem[s])))
                            dir1 = dir0;
                        else if (o1)
                            ;
                        else if (o2)
                            dir1 = dir2;
                        else if (o0)
                            dir1 = dir0;
                        else
                            dir1 = DIRWRAP(dir1 + 2);
                    }
                    if (XDIR(dir1))
                        y = (1 + 2 * j1) * gfx_h / 2;
                    else if (dir1)
                        x = (1 + 2 * i1) * gfx_w / 2;
                    if ((MYMANSQUARE || gfx_reflect) ? (cycles & 1) : 1)
                        sprite_register_x[mean] = (sprite_register_x[eyes] = XPIXWRAP(x + XDIR(dir1)));
                    if ((gfx_reflect && ! MYMANSQUARE) ? 1 : (cycles & 1))
                        sprite_register_y[mean] = (sprite_register_y[eyes] = YPIXWRAP(y + YDIR(dir1)));
                    if (! (unsigned) home_dir[(s % ghosts*maze_h+YTILE(sprite_register_y[mean]))*(maze_w+1)+XTILE(sprite_register_x[mean])])
                        home_dir[(s % ghosts*maze_h+YTILE(sprite_register_y[mean]))*(maze_w+1)+XTILE(sprite_register_x[mean])] =
                            DIRWRAP(dir1 + 2);
                } else if (sprite_register_used[eyes]
                           &&
                           ((munched != eyes) || (! ghost_eaten_timer))
                           &&
                           ((! ghost_eaten_timer) || ! sprite_register_used[MEANGHOST(s)])) {
                    int dx, dy;
                    unsigned char d, d1 = 0;

                    /* goin' home */
                    ghost_timer[s] = (int) MEMDELAY(s);
                    x = sprite_register_x[eyes];
                    y = sprite_register_y[eyes];
                    i1 = XTILE(x);
                    j1 = YTILE(y);
                    dx = (int) ((XGHOST - x) / gfx_w);
                    dy = (int) (((dx ? YTOP : YGHOST) - y) / gfx_h);
                    d = find_home_dir(s, j1, i1);
                    if ((unsigned) d)
                        d1 = (unsigned) DIRWRAP(find_home_dir(s, YWRAP(j1 + YDIR((unsigned) d)), XWRAP(i1 + XDIR((unsigned) d))) + 2);
                    if (((unsigned) d) &&
                        (! (((unsigned) d1) &&
                            (((unsigned) d1) == (unsigned) d))))
                        ghost_mem[s] = (unsigned) d;
                    else {
                        if (dx * dx > dy * dy) {
                            if (dx > 0)
                                ghost_mem[s] = MYMAN_RIGHT;
                            else
                                ghost_mem[s] = MYMAN_LEFT;
                        }
                        else
                        {
                            if (dy < 0)
                                ghost_mem[s] = MYMAN_UP;
                            else
                                ghost_mem[s] = MYMAN_DOWN;
                        }
                    }
                    if (! (dx || dy)) {
                        sprite_register_used[eyes] = NET_LIVES ? VISIBLE_EYES : 0;
                        sprite_register_used[mean] = 1;
                        sprite_register_x[mean] = x;
                        sprite_register_y[mean] = y;
                        sprite_register_frame[eyes] = (ghost_dir[s] = DIRWRAP(s + 1)) - 1;
                        ghost_mem[s] = 0;
                        continue;
                    }
                    mcell = (unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir0))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir0))];
                    o0 = ISOPEN((unsigned) mcell)
                        || ISDOOR((unsigned) mcell);
                    mcell = (unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir1))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir1))];
                    o1 = ISOPEN((unsigned) mcell)
                        || ISDOOR((unsigned) mcell);
                    mcell = (unsigned char) maze[(maze_level*maze_h+YWRAP(j1 + YDIR(dir2))) * (maze_w + 1)+XWRAP(i1 + XDIR(dir2))];
                    o2 = ISOPEN((unsigned) mcell)
                        || ISDOOR((unsigned) mcell);
                    if (o2 && (dir2 == ghost_mem[s]))
                        dir1 = dir2;
                    else if (o1 && (dir1 == ghost_mem[s]))
                        ;
                    else if (o0 && (dir0 == ghost_mem[s]))
                        dir1 = dir0;
                    else if (o1)
                        ;
                    else if (o2)
                        dir1 = dir2;
                    else if (o0)
                        dir1 = dir0;
                    else
                        dir1 = DIRWRAP(dir1 + 2);
                    sprite_register_x[mean] = (sprite_register_x[eyes] = XPIXWRAP(x + XDIR(dir1)));
                    sprite_register_x[eyes] = XPIXWRAP(x + XDIR(dir1));
                    sprite_register_y[eyes] = YPIXWRAP(y + YDIR(dir1));
                    if (! (unsigned) home_dir[(s % ghosts*maze_h+YTILE(sprite_register_y[eyes]))*(maze_w+1)+XTILE(sprite_register_x[eyes])])
                        home_dir[(s % ghosts*maze_h+YTILE(sprite_register_y[eyes]))*(maze_w+1)+XTILE(sprite_register_x[eyes])] =
                            DIRWRAP(dir1 + 2);
                }
                if ((! ghost_eaten_timer) && ghost_timer[s] && ! -- ghost_timer[s]) {
                    ghost_timer[s] = (int) MEMDELAY(s);
                    if (ghost_mem[s]) {
                        ghost_mem[s] = 0;
                    }
                    else
                        dir1 = DIRWRAP(ghost_dir[s] + 2);
                }
                ghost_dir[s] = (sprite_register_frame[eyes] = dir1 - 1) + 1;
                collision_type = check_collision(eyes, mean, blue);
                if (collision_type)
                {
                    if (collision_type == COLLISION_TYPE_HERO)
                    {
                        s = -1;
                        continue;
                    }
                }
            }
        }
        if (! ghost_eaten_timer)
            for (s = 0; s < SPRITE_REGISTERS; s ++) {
                if (sprite_register_used[s] && sprite_register_timer[s])
                    if (! -- sprite_register_timer[s])
                    {
                        sprite_register_used[s] = 0;
                        if (s == FRUIT)
                        {
                            DIRTY_ALL();
                            ignore_delay = 1;
                            frameskip = 0;
                        }
                    }
            }
    }
    return 0;
}
