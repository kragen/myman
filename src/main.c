/*
 * main.c - variant/size-selecting wrapper for starting the MyMan video game
 * Copyright 1997-2006, Benjamin C. Wiley Sittler <bsittler@gmail.com>
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
#include <string.h>
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif
#include <stdio.h>
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <time.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdlib.h>

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

/* Allegro sometimes redefines main */
#ifdef ALLEGROCURSES
#ifndef main
#include <allegro.h>
#endif
#endif

/* SDL sometimes redefines main */
#ifdef USE_SDL
#if USE_SDL
#ifdef main
#define _SDL_main_h
#else
#define MAIN_NO_ENVP
#endif
#include <SDL.h>
#endif
#endif

/* MyMan utilities; also defines cruft like __MSDOS__ under some circumstances */
#include "utils.h"

#ifdef XCURSES
#define XCURSES_USAGE \
" [ -- toolkit-options]"
#else
#define XCURSES_USAGE
#endif

#define SUMMARY(progname) \
"Summary: %s [--help] [--version] [options]" \
XCURSES_USAGE \
"\n", \
progname

#define USAGE(progname, variant, size) "" \
"Usage: %s [options]" \
XCURSES_USAGE \
"\n" \
"%s" \
"Defaults: -v %s -z %s\n" \
"", \
progname, \
"-v VARIANT or\n" \
"--variant=VARIANT       select game variant [" MYMANVARIANT "]\n" \
"-z SIZE or\n" \
"--size=SIZE             select tile and sprite size [" MYMANSIZE "]\n" \
"-h or\n" \
"--help                  show this message and exit\n" \
"-k or\n" \
"--keys                  show keybindings and exit\n" \
"-L or\n" \
"--legal                 show legal notice and exit\n" \
"-V or\n" \
"--version               print " MYMAN " version and exit\n" \
"Variants: " MYMANVARIANTS "\n" \
"Sizes:    " MYMANSIZES "\n" \
"" \
, \
defvariant \
, \
defsize

#define MYMANVERSION_INFO \
"" MYMAN " " MYMANVERSION "\n" \
MYMANCOPYRIGHT "\n"

#ifdef BUILTIN_SIZES
const char *builtin_size;
const char *builtin_tilefile;
const char *builtin_spritefile;
#endif

#ifdef BUILTIN_VARIANTS
const char *builtin_variant;
const char *builtin_mazefile;
const char *maze_data;
const char *maze_color_data;
extern int main_builtin(int argc, char *argv[], char *envp[]);
#endif

int
main(int argc, char *argv[]
#ifndef MAIN_NO_ENVP
     , char *envp[]
#endif
    )
{
    const char *progname = "";
#ifdef MAIN_NO_ENVP
    extern char **environ;
    char **envp = environ;
#endif
    int i, option_index;
    static char MYMANVARIANT_str[] = MYMANVARIANT;
    const char *defvariant = MYMANVARIANT_str;
    static char MYMANSIZE_str[] = MYMANSIZE;
    const char *defsize = MYMANSIZE_str;
    char *variant = NULL;
    char *size = NULL;
    char variants[] = MYMANVARIANTS "\0\0";
    char sizes[] = MYMANSIZES "\0\0";
#ifdef PRIVATELIBEXECDIR
    char scratch[] = PRIVATELIBEXECDIR "/\0" MYMANVARIANTS "-" MYMANSIZES;
#endif

    if (argc >= 1)
    {
        progname = argv[0];
    }
    while ((variant = strrchr(variants, ' ')) != NULL)
    {
        *variant = '\0';
    }
    while ((size = strrchr(sizes, ' ')) != NULL)
    {
        *size = '\0';
    }

#if defined(__APPLE__) || defined(macintosh)
    /* when launched as a native application under Mac OS X, there may
     * be a bogus process serial number parameter. */
    if ((argc == 2) && (! strncmp(argv[1], "-psn_", strlen("-psn_"))))
    {
        argc = 1;
    }
#endif
    while((i = getopt_long(argc, argv, short_options,
                           long_options, &option_index))
          != -1)
        switch(i)
        {
	case 'i':
	case 'I':
        case 'a':
        case 'A':
        case 'e':
        case 'E':
        case 'N':
        case 'B':
        case 'b':
        case 'c':
        case '2':
        case '1':
        case 'd':
        case 'D':
        case 'g':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 'R':
        case 's':
        case 't':
        case 'u':
        case 'U':
        case 'M':
        case 'S':
        case 'T':
        case 'f':
        case 'F':
        case 'k':
        case 'L':
        case 'x':
        case 'X':
            break;
        case 'z':
            defsize = optarg;
            break;
        case 'v':
            defvariant = optarg;
            break;
        case 'h':
            printf(USAGE(progname, defvariant, defsize));
            printf("Variant ");
            goto nomoreargs;
        case 'V':
            printf(MYMANVERSION_INFO);
            goto nomoreargs;
        default:
            fprintf(stderr, SUMMARY(progname));
            fflush(stderr), exit(2);
        }
  nomoreargs:
    for (size = sizes;
         *size != '\0';
         size += strlen(size) + 1)
    {
        if (! strcmp(size, defsize))
        {
            break;
        }
    }
    if (*size == '\0')
    {
        fprintf(stderr, "%s: unrecognized size\n", defsize);
        fprintf(stderr, SUMMARY(progname));
        fflush(stderr), exit(2);
    }
    for (variant = variants;
         *variant != '\0';
         variant += strlen(variant) + 1)
    {
        if (! strcmp(variant, defvariant))
        {
            break;
        }
    }
    if (*variant == '\0')
    {
        fprintf(stderr, "%s: unrecognized variant\n", defvariant);
        fprintf(stderr, SUMMARY(progname));
        fflush(stderr), exit(2);
    }
#ifdef PRIVATELIBEXECDIR
    strncat(scratch, defvariant, strlen(defvariant));
    strncat(scratch, "-", 1);
    strncat(scratch, defsize, strlen(defsize));
#endif
    fflush(stdout);
    fflush(stderr);
#ifdef BUILTIN_SIZES
    builtin_size = 0;
#define BUILTIN_SIZE(size) \
    do \
    { \
        if (! strcmp(defsize, # size)) \
        { \
            extern char *builtin_tilefile_ ## size; \
            extern char *tile_ ## size [256]; \
            extern int tile_used_ ## size [256]; \
            extern int tile_color_ ## size [256]; \
            extern int tile_w_ ## size; \
            extern int tile_h_ ## size; \
            extern int tile_flags_ ## size; \
            extern const char *tile_args_ ## size; \
            extern char *builtin_spritefile_ ## size; \
            extern char *sprite_ ## size [256]; \
            extern int sprite_used_ ## size [256]; \
            extern int sprite_color_ ## size [256]; \
            extern int sprite_w_ ## size; \
            extern int sprite_h_ ## size; \
            extern int sprite_flags_ ## size; \
            extern const char *sprite_args_ ## size; \
            builtin_tilefile = builtin_tilefile_ ## size; \
            memcpy((void *) tile, (void *)tile_ ## size, sizeof(tile)); \
            memcpy((void *) tile_used, (void *) tile_used_ ## size, sizeof(tile_used)); \
            memcpy((void *) tile_color, (void *) tile_color_ ## size, sizeof(tile_color)); \
            tile_w = tile_w_ ## size; \
            tile_h = tile_h_ ## size; \
            tile_flags = tile_flags_ ## size; \
            tile_args = tile_args_ ## size; \
            builtin_spritefile = builtin_spritefile_ ## size; \
            memcpy((void *) sprite, (void *)sprite_ ## size, sizeof(sprite)); \
            memcpy((void *) sprite_used, (void *) sprite_used_ ## size, sizeof(sprite_used)); \
            memcpy((void *) sprite_color, (void *) sprite_color_ ## size, sizeof(sprite_color)); \
            sprite_w = sprite_w_ ## size; \
            sprite_h = sprite_h_ ## size; \
            sprite_flags = sprite_flags_ ## size; \
            sprite_flags = sprite_flags_ ## size; \
            sprite_args = sprite_args_ ## size; \
            builtin_size = # size; \
        } \
    } while(0);
    if (1) { BUILTIN_SIZES }
#endif
#ifdef BUILTIN_VARIANTS
#ifdef MYGETOPT_H
    mygetopt_reset();
#else
    optind = 1;
    opterr = 1;
    optopt = 0;
    optarg = 0;
#endif
    builtin_variant = 0;
#ifdef BUILTIN_SIZES
#define BUILTIN_VARIANT(variant) \
    do \
    { \
        if (builtin_size) \
        { \
            if (! strcmp(defvariant, # variant)) \
            { \
                extern char *builtin_mazefile_ ## variant; \
                extern const char *maze_data_ ## variant; \
                extern const char *maze_color_data_ ## variant; \
                extern int maze_n_ ## variant; \
                extern int maze_w_ ## variant; \
                extern int maze_h_ ## variant; \
                extern int maze_flags_ ## variant; \
                extern const char *maze_args_ ## variant; \
                builtin_mazefile = builtin_mazefile_ ## variant; \
                maze_data = maze_data_ ## variant; \
                maze_color_data = maze_color_data_ ## variant; \
                maze_n = maze_n_ ## variant; \
                maze_w = maze_w_ ## variant; \
                maze_h = maze_h_ ## variant; \
                maze_flags = maze_flags_ ## variant; \
                maze_args = maze_args_ ## variant; \
                builtin_variant = # variant; \
                return main_builtin(argc,argv,envp); \
            } \
        } \
    } while(0);
    if (1) { BUILTIN_VARIANTS }
#endif
#endif
#ifdef PRIVATELIBEXECDIR
    execve(scratch, argv, envp);
    perror(scratch);
#else
    fprintf(stderr, "%s: internal error: PRIVATELIBEXECDIR not defined and selected variant not built in\n", argv[0]);
#endif
    fflush(stderr);
    return 1;
}
#ifdef END_OF_MAIN
END_OF_MAIN()
#endif
