/*
 * main.c - variant/size-selecting wrapper for starting the MyMan video game
 * Copyright 1997-2006, Benjamin Sittler <bsittler@gmail.com>
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

#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

/* command-line argument parser */
#ifdef MY_GETOPT_H
#include MY_GETOPT_H
#else
#include <getopt.h>
#endif

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
"--variant=VARIANT       select game variant [" VARIANT "]\n" \
"-z SIZE or\n" \
"--size=SIZE             select tile and sprite size [" SIZE "]\n" \
"-h or\n" \
"--help                  show this message and exit\n" \
"-V or\n" \
"--version               print " MYMAN " version and exit\n" \
"Variants: " VARIANTS "\n" \
"Sizes:    " SIZES "\n" \
"" \
, \
defvariant \
, \
defsize

#define VERSION_INFO \
"" MYMAN " " VERSION "\n"

int main(int argc, char *argv[], char *envp[])
{
    int i, option_index;
    static char VARIANT_str[] = VARIANT;
    const char *defvariant = VARIANT_str;
    static char SIZE_str[] = SIZE;
    const char *defsize = SIZE_str;
    char *variant = NULL;
    char *size = NULL;
    char variants[] = VARIANTS "\0\0";
    char sizes[] = SIZES "\0\0";
    char scratch[] = PRIVATELIBEXECDIR "/\0" VARIANTS "-" SIZES;
    static struct option long_options[] =
        {
            {"version", 0, 0, 'V'},
            {"help", 0, 0, 'h'},
            {"variant", 1, 0, 'v'},
            {"size", 1, 0, 'z'},
            {0, 0, 0, 0}
        };

    while ((variant = strrchr(variants, ' ')) != NULL)
    {
        *variant = '\0';
    }
    while ((size = strrchr(sizes, ' ')) != NULL)
    {
        *size = '\0';
    }

    while((i = getopt_long(argc, argv, "Vv:z:bcd:hm:noprqs:t:uUMRSTf:F:aAeEBNiI12",
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
            break;
        case 'z':
            defsize = optarg;
            break;
        case 'v':
            defvariant = optarg;
            break;
        case 'h':
            printf(USAGE(argv[0], defvariant, defsize));
            printf("Variant ");
            goto nomoreargs;
        case 'V':
            printf(VERSION_INFO);
            goto nomoreargs;
        default:
            fprintf(stderr, SUMMARY(argv[0]));
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
        fprintf(stderr, SUMMARY(argv[0]));
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
        fprintf(stderr, SUMMARY(argv[0]));
        fflush(stderr), exit(2);
    }
    strncat(scratch, defvariant, strlen(defvariant));
    strncat(scratch, "-", 1);
    strncat(scratch, defsize, strlen(defsize));
    fflush(stdout);
    fflush(stderr);
#ifdef BUILTIN_GAMES
#ifdef MY_GETOPT_H
    my_getopt_reset();
#else
    optind = 1;
    opterr = 1;
    optopt = 0;
    optarg = 0;
#endif

#define GAME(main) \
    if (1) \
    { \
        char s[] = "main_\0" VARIANTS "_" SIZES; \
        extern int main(int argc, char *argv[], char *envp[]); \
        strncat(s, defvariant, strlen(defvariant)); \
        strncat(s, "_", 1); \
        strncat(s, defsize, strlen(defsize)); \
        if (! strcmp(s, # main) ) return main(argc,argv,envp); \
    }
    BUILTIN_GAMES
#endif
    execve(scratch, argv, envp);
    perror(scratch);
    fflush(stderr);
    return 1;
}
