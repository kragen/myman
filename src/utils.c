/* utils.c - various utilities for the MyMan video game
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
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
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

#if defined(WIN32)
#include <windows.h>
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

/* legal notice to display before starting */
const char *MYMANLEGALNOTICE =
"LEGAL NOTICE" "\n"
"\n"
"MyMan is an unofficial and unlicensed clone of the original Pac-Man" " "
"and Puckman games and of their sequels and imitators, and is not" " "
"endorsed by Namco Bandai Games Inc., owners of the Pac-Man and Puckman" " "
"registered trademarks and copyrights. Any trademarks used herein are" " "
"the property of their respective owners. No mention of a trademark," " "
"trademark owner, or other party shall be construed as an endorsement" " "
"of MyMan or any other product by any party. If you believe that by" " "
"using or posessing MyMan the rights of others would be infringed, you" " "
"are strongly encouraged to cease using MyMan and its derivatives and" " "
"delete all copies of the MyMan software and its derivatives, and to" " "
"inform the author or authors of MyMan and the distributor or" " "
"distributors from whom you obtained MyMan of the reasons for such" " "
"belief so that the infringing part may be removed or replaced. Any" " "
"imitation by MyMan of other software is intended to be purely for" " "
"purposes of humor and amusement." "\n"
"\n"
MYMAN " - The MyMan video game" "\n"
MYMANCOPYRIGHT "\n"
"\n"
"Permission is hereby granted, free of charge, to any person" " "
"obtaining a copy of this software and associated documentation" " "
"files (the \"Software\"), to deal in the Software without" " "
"restriction, including without limitation the rights to use, copy," " "
"modify, merge, publish, distribute, sublicense, and/or sell copies" " "
"of the Software, and to permit persons to whom the Software is" " "
"furnished to do so, subject to the following conditions:" "\n"
"\n"
"The above copyright notice and this permission notice shall be" " "
"included in all copies or substantial portions of the Software." "\n"
"\n"
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND," " "
"EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF" " "
"MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND" " "
"NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT" " "
"HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY," " "
"WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM," " "
"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER" " "
"DEALINGS IN THE SOFTWARE." "\n"
"\n"
"Author contact information:" "\n"
"\n"
"Benjamin C. W. Sittler <bsittler@gmail.com>" "\n"
"6214 Plumas Avenue" "\n"
"Richmond, California 94804" "\n"
"U.S.A." "\n"
"\n"
"Press \? or Ctrl-H during the game for help and additional notices." "\n"
"\0";

const char *MYMANKEYS =
"The following case-insensitive keystroke commands are recognized during" " "
"the game:" "\n"
"Q or Ctrl-C: quit the game" "\n"
"P or ESC: pause the game" "\n"
"Ctrl-Q: re-enable output (after Ctrl-S)" "\n"
"R, Ctrl-L or Ctrl-R: refresh (redraw) the screen" "\n"
"@: reset the display subsystem and refresh (redraw) the screen" "\n"
"Ctrl-S: inhibit output until Ctrl-Q is typed" "\n"
"S: toggle sound on/off" "\n"
"W: warp to the next level (after consuming the next dot)" "\n"
"C: toggle color on/off (if enabled at compile-time)" "\n"
"B: toggle use of dim and bold attributes for missing colors" "\n"
"U: toggle underlining of walls on/off (if enabled at compile-time)" "\n"
"D: toggle maze debugging on/off" "\n"
"T: save an HTML screenshot to the file snap####" HTM_SUFFIX ", where #### is" " "
"a four-digit sequence number, and flash the screen briefly; a" " "
"plain text version is saved as snap####" TXT_SUFFIX " using backspacing" " "
"to represent underlined and bold text" "\n"
"O or 0: toggle appearance of power pellets and dots" "\n"
"A: toggle between ASCII altcharset translations and your terminal\'s" " "
"altcharset translations" "\n"
"I: toggle between using your terminal\'s vertical scrolling capabilities" " "
"and redrawing the screen" "\n"
"X: toggle between raw tile characters (CP437 or UCS/Unicode character" " "
"graphics) and altcharset translations (VT100-style graphics)" "\n"
"E: toggle between UCS/Unicode and CP437 for internal representation" " "
"of raw tile characters" "\n"
"H, 4, Ctrl-B or LEFT: move left" "\n"
"J, 2, Ctrl-N or DOWN: move down" "\n"
"K, 8, Ctrl-P or UP: move up" "\n"
"L, 6, Ctrl-F or RIGHT: move right" "\n"
", (comma) or <: cancel pending vertical move" "\n"
". (full stop) or >: cancel pending horizontal move" "\n"
"/ or \\: toggle diagonal maze reflection" "\n"
"!: display information about the maze, tileset and spriteset" "\n"
"\? or Ctrl-H: display help screen" "\n"
"\n"
"The pager recognizes the following special commands:" "\n"
"SPACE: display next page or finish" "\n"
"ESC: finish immediately" "\n"
"Move left then right with no intervening keystrokes:" " "
"equivalent to ESC" "\n"
"\0";

const char *maze_ABOUT_prefix = "\n" "Maze: ";

const char *maze_FIXME_prefix = "\n" "FIXME (Maze): ";

const char *maze_NOTE_prefix = "\n" "NOTE (Maze): ";

const char *tile_ABOUT_prefix = "\n" "Tiles: ";

const char *tile_FIXME_prefix = "\n" "FIXME (Tiles): ";

const char *tile_NOTE_prefix = "\n" "NOTE (Tiles): ";

const char *sprite_ABOUT_prefix = "\n" "Sprites: ";

const char *sprite_FIXME_prefix = "\n" "FIXME (Sprites): ";

const char *sprite_NOTE_prefix = "\n" "NOTE (Sprites): ";

const char *MYMANKEYS_prefix = "Keyboard Map" "\n" "\n";

const char *MOREMESSAGE = "[Space: more, Q: quit]";

const char *DONEMESSAGE = "[Space: OK, Q: quit]";

const char *short_options = "Vv:z:bcd:D:g:l:Lhkm:noprqs:t:uUMRSTf:F:aAeEBNiI12xX";
static struct option long_options_static[] =
{
    {"version", 0, 0, 'V'},
    {"help", 0, 0, 'h'},
    {"keys", 0, 0, 'k'},
    {"legal", 0, 0, 'L'},
    {"variant", 1, 0, 'v'},
    {"size", 1, 0, 'z'},
    {0, 0, 0, 0}
};
struct option *long_options = long_options_static;

/* if the utf-8 stream starts with U+FEFF (BOM), read it off and
 * ignore it. returns 1 if BOM was stripped, 0 otherwise. while a BOM
 * is not recommended in utf-8 text, it is automatically inserted by
 * some tools (notepad, for instance.) */
static int
ignore_bom_utf8(FILE *stream)
{
    int c;

    c = fgetc(stream);
    if (c == 0xef)
    {
        int c1;

        c1 = fgetc(stream);
        if (c1 == 0xbb)
        {
            int c2;

            c2 = fgetc(stream);
            if (c2 == 0xbf)
            {
                return 1;
            }
            if (c2 != EOF)
            {
                ungetc(c2, stream);
            }
        }
        if (c1 != EOF)
        {
            ungetc(c1, stream);
        }
    }
    if (c != EOF)
    {
        ungetc(c, stream);
    }
    return 0;
}

/* UCS/Unicode mapping for CP437 (used in HTML snapshots) */

const unsigned long
uni_cp437_halfwidth[256] =
{
/* 0x00 */
    0x2008, 0x263A, 0x263B, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022,
    0x25D8, 0x25CB, 0x25D9, 0x2642, 0x2640, 0x266A, 0x266B, 0x263C,
/* 0x10 */
    0x25BA, 0x25C4, 0x2195, 0x203C, 0x00B6, 0x00A7, 0x25AC, 0x21A8,
    0x2191, 0x2193, 0x2192, 0x2190, 0x221F, 0x2194, 0x25B2, 0x25BC,
/* 0x20 */
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
    0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
/* 0x30 */
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
    0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
/* 0x40 */
    0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
    0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
/* 0x50 */
    0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
    0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
/* 0x60 */
    0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
    0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
/* 0x70 */
    0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
    0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x2302,
/* 0x80 */
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7,
    0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
/* 0x90 */
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9,
    0x00FF, 0x00D6, 0x00DC, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
/* 0xA0 */
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,
    0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
/* 0xB0 */
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
    0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
/* 0xC0 */
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F,
    0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
/* 0xD0 */
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B,
    0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
/* 0xE0 */
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4,
    0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
/* 0xF0 */
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248,
    0x00B0, 0x00B7, 0x2022, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
};

/* alternate UCS/Unicode mapping used in CJK fullwidth mode */

const unsigned long
uni_cp437_fullwidth[256] =
{
/* 0x00 */
    0x3000, 0x25CB, 0x25CF, 0x25CF, 0x25CF, 0x25A0, 0x25A0, 0x25CF,
    0x25A0, 0x25CB, 0x25A0, 0x2642, 0x2640, 0xFF03, 0xFF03, 0x2606,
/* 0x10 */
    0x2605, 0x2605, 0xFF29, 0xFF01, 0xFF30, 0x00A7, 0x25A0, 0xFF2C,
    0x2191, 0x2193, 0x2192, 0x2190, 0xFF2C, 0x2500, 0x25B2, 0x25BC,
/* 0x20 */
    0x3000, 0xFF01, 0x201D, 0xFF03, 0xFF04, 0xFF05, 0xFF06, 0x2019,
    0xFF08, 0xFF09, 0xFF0A, 0xFF0B, 0xFF0C, 0x2500, 0xFF0E, 0xFF0F,
/* 0x30 */
    0xFF10, 0xFF11, 0xFF12, 0xFF13, 0xFF14, 0xFF15, 0xFF16, 0xFF17,
    0xFF18, 0xFF19, 0xFF1A, 0xFF1B, 0xFF1C, 0xFF1D, 0xFF1E, 0xFF1F,
/* 0x40 */
    0xFF20, 0xFF21, 0xFF22, 0xFF23, 0xFF24, 0xFF25, 0xFF26, 0xFF27,
    0xFF28, 0xFF29, 0xFF2A, 0xFF2B, 0xFF2C, 0xFF2D, 0xFF2E, 0xFF2F,
/* 0x50 */
    0xFF30, 0xFF31, 0xFF32, 0xFF33, 0xFF34, 0xFF35, 0xFF36, 0xFF37,
    0xFF38, 0xFF39, 0xFF3A, 0xFF3B, 0xFF3C, 0xFF3D, 0xFF3E, 0xFF3F,
/* 0x60 */
    0x2018, 0xFF41, 0xFF42, 0xFF43, 0xFF44, 0xFF45, 0xFF46, 0xFF47,
    0xFF48, 0xFF49, 0xFF4A, 0xFF4B, 0xFF4C, 0xFF4D, 0xFF4E, 0xFF4F,
/* 0x70 */
    0xFF50, 0xFF51, 0xFF52, 0xFF53, 0xFF54, 0xFF55, 0xFF56, 0xFF57,
    0xFF58, 0xFF59, 0xFF5A, 0xFF5B, 0xFF5C, 0xFF5D, 0x301C, 0x0394,
/* 0x80 */
    0xFF23, 0xFF55, 0xFF45, 0xFF41, 0xFF41, 0xFF41, 0xFF41, 0xFF43,
    0xFF45, 0xFF45, 0xFF45, 0xFF49, 0xFF49, 0xFF49, 0xFF21, 0xFF21,
/* 0x90 */
    0xFF25, 0xFF45, 0xFF25, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF55, 0xFF55,
    0xFF59, 0xFF2F, 0xFF35, 0xFFE0, 0xFFE1, 0xFFE5, 0xFF50, 0xFF46,
/* 0xA0 */
    0xFF41, 0xFF49, 0xFF4F, 0xFF55, 0xFF4E, 0xFF2E, 0xFF41, 0xFF4F,
    0xFF1F, 0x250C, 0xFFE2, 0x00BD, 0x00BC, 0xFF01, 0x300A, 0x300B,
/* 0xB0 */
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2524, 0x2524, 0x2524,
    0x252C, 0x2524, 0x2502, 0x2510, 0x2518, 0x2524, 0x2534, 0x2510,
/* 0xC0 */
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x251C, 0x251C,
    0x2514, 0x250C, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x2534,
/* 0xD0 */
    0x2534, 0x252C, 0x252C, 0x251C, 0x2534, 0x252C, 0x251C, 0x253C,
    0x253C, 0x2518, 0x250C, 0x25A0, 0x25A0, 0x25A0, 0x25A0, 0x25A0,
/* 0xE0 */
    0x03B1, 0x03B2, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x03BC, 0x03C4,
    0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
/* 0xF0 */
    0x2261, 0x00B1, 0xFF1E, 0xFF1C, 0x222B, 0x222B, 0x00F7, 0xFF1D,
    0x00B0, 0x30FB, 0x30FB, 0x221A, 0xFF4E, 0xFF12, 0x25CF, 0x3000
};

const unsigned char cp437_fullwidth_rhs[256] =
{
/*00*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/*10*/
    0x20, 0xcd, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xc4, 0x20, 0x20, 0x20, 0x20, 0x20,
/*20*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2D, 0x20, 0x20,
/*30*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3D, 0x20, 0x20,
/*40*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/*50*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/*60*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/*70*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/*80*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/*90*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/*A0*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/*B0*/
    0xb0, 0xb1, 0xb2, 0x20, 0x20, 0x20, 0x20, 0x20, 0xc4, 0x20, 0x20, 0x20, 0x20, 0x20, 0xc4, 0x20,
/*C0*/
    0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xcd, 0xc4, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
/*D0*/
    0xc4, 0xcd, 0xc4, 0xc4, 0xcd, 0xcd, 0xc4, 0xc4, 0xcd, 0x20, 0xc4, 0xdb, 0xdc, 0x20, 0xde, 0xdf,
/*E0*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/*F0*/
    0xf0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20
};

const unsigned long *uni_cp437 = uni_cp437_halfwidth;

unsigned char reflect_sprite[256] = {
    /* 0x00 */
    /*%8*/0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, /*"^*/0x0b, /*"<*/0x0a, /*"v*/0x0d, /*">*/0x0c, /*m2*/0x0e, 0x0f,
    /* 0x10 */
    /*C<4*/0x14, 0x15, 0x16, 0x17, /*C^4*/0x10, 0x11, 0x12, 0x13,
    /*CX4*/0x18, 0x19, 0x1a, 0x1b, /*Cv4*/0x20, 0x21, 0x22, 0x23,
    /* 0x20 */
    /*C>4*/0x1c, 0x1d, 0x1e, 0x1f,
    /**/0x24, 0x25, 0x26, 0x27,
    /**/0x28, /*1*/0x29, /*3*/0x2a, /*5*/0x2b, /*7*/0x2c, /*10*/0x2d, /*20*/0x2e, /*30*/0x2f,
    /* 0x30 */
    /*50*/0x30, /*2*/0x31, /*4*/0x32, /*8*/0x33, /*16*/0x34, /*M2*/0x35, 0x36, /*C*/0x37,
    /*C(UL)*/0x38, 0x39, 0x3a, 0x3b,
    /*C(UR)*/0x40, 0x41, 0x42, 0x43,
    /* 0x40 */
    /*C(LL)*/0x3c, 0x3d, 0x3e, 0x3f,
    /*C(LR)*/0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    /* 0x50 */
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
    /* 0x60 */
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
    /* 0x70 */
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
    0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
    /* 0x80 */
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
    /* 0x90 */
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
    /* 0xa0 */
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
    /* 0xb0 */
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
    /* 0xc0 */
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    /* 0xd0 */
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
    /* 0xe0 */
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
    /* 0xf0 */
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};

unsigned char cp437_sprite[256] = {
    /* 0x00 */
    /*%8*/'%', 'v', '@', '@', '@', 'Y', 'A', 'F',
    'A', 'A', /*"^*/'\"', /*"<*/'\"', /*"v*/'\"', /*">*/'\"', /*m2*/'m', 'm',
    /* 0x10 */
    /*C<4*/'C', 'c', 'C', 'c', /*C^4*/'C', 'c', 'C', 'c',
    /*CX4*/'C', '(', '*', '\'', /*Cv4*/'C', 'c', 'C', 'c',
    /* 0x20 */
    /*C>4*/'C', 'c', 'C', 'c',
    /**/0x00, 0x00, 0x00, 0x00,
    /**/0x00,
    /*1*/'1', /*3*/'3', /*5*/'5', /*7*/'7', /*10*/'1', /*20*/'2', /*30*/'3',
    /* 0x30 */
    /*50*/'5', /*2*/'2', /*4*/'4', /*8*/'8', /*16*/'!', /*M2*/'M', 'M', /*C*/'C',
    /*C(UL)*/'/', '/', '/', '/',
    /*C(UR)*/'\\', '-', '/', '-',
    /* 0x40 */
    /*C(LL)*/'\\', '\\', '\\', '\\',
    /*C(LR)*/'/', '-', '\\', '-',
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0x50 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0x60 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0x70 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0x80 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0x90 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xa0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xb0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xc0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xd0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xe0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xf0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char
udlr[256] = {
/*00*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*10*/
    0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x40, 0x01, 0x04, 0x00, 0x00, 0x20, 0x80,
/*20*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x05, 0x00, 0x00,
/*30*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
/*40*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*50*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*60*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00,
/*70*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x05, 0x00,
/*80*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*90*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*A0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*B0*/
    0x00, 0x00, 0x00, 0x50, 0x54, 0x58, 0xa4, 0x64, 0x19, 0xa8, 0xa0, 0x28, 0x88, 0x94, 0x49, 0x14,
/*C0*/
    0x41, 0x45, 0x15, 0x51, 0x05, 0x55, 0x52, 0xa1, 0x82, 0x22, 0x8a, 0x2a, 0x82, 0x0a, 0xaa, 0x4a,
/*D0*/
    0x85, 0x1a, 0x25, 0x91, 0x46, 0x16, 0x61, 0xa5, 0x5a, 0x44, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00,
/*E0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*F0*/
    0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned char fallback_cp437[256] = {
/* 0x00 */
    0x00, '@', '@', '*', '+', '%', '&', 'o',
    '#', 'o', '#', 'd', 'q', 'l', 'H', '*',
/* 0x10 */
    0xcd, 0xcd, 0xb3, '!', 'P', '$', '#', 'L',
    0xb3, 0xb3, 0xc4, 0xc4, '!', 0xc4, 0xba, 0xba,
/* 0x20 */
    ' ', '!', '\"', '0', '$', '%', '&', '\'',
    '(', ')', '*', '+', ',', '-', '.', '/',
/* 0x30 */
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', ':', ';', '<', '-', '>', '\?',
/* 0x40 */
    'C', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
/* 0x50 */
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
/* 0x60 */
    '`', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
/* 0x70 */
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', '{', '|', '}', '-', 'D',
/* 0x80 */
    'C', 'u', 'e', 'a', 'a', 'a', 'a', 'c',
    'e', 'e', 'a', 'i', 'i', 'i', 'A', 'A',
/* 0x90 */
    'E', 'e', 'E', 'o', 'o', 'o', 'u', 'u',
    'y', 'O', 'U', 'c', 'L', 'Y', 'P', 'f',
/* 0xA0 */
    'a', 'i', 'o', 'u', 'n', 'N', 'a', 'o',
    '\?', '-', '-', '%', '%', '!', '<', '>',
/* 0xB0 */
    0xb1, '#', 0xb1, '|', 0xc5, 0xb4, 0xb4, 0xb6,
    0xd1, 0xb4, 0xb3, 0xbf, 0xd9, 0xb6, 0xcf, 0xc5,
/* 0xC0 */
    0xc5, 0xc5, 0xc5, 0xc5, '-', '+', 0xc3, 0xc3,
    0xc0, 0xda, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc1,
/* 0xD0 */
    0xc1, 0xc2, 0xc2, 0xc7, 0xcf, 0xd1, 0xc7, 0xc5,
    0xc5, 0xc5, 0xc5, '#', '_', 0xb1, 0xb1, '\"',
/* 0xE0 */
    'a', 'B', 'G', 'p', 'S', 's', 'm', 't',
    'p', 't', 'W', 'd', '8', 'f', 'e', '^',
/* 0xF0 */
    '=', '+', '>', '<', 's', 's', '%', '=',
    0x09, '.', 0x07, 'V', 'n', '2', 0xfa, ' '
};

unsigned char reflect_cp437[256] = {
/* 0x00 */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
/* 0x10 */
    0x1f, 0x1e, 0x1d, '=', 0x14, 0x15, 0x16, 0x17,
    0x1a, 0x1b, 0x18, 0x19, 0x1c, 0x12, 0x11, 0x10,
/* 0x20 */
    ' ', '!', '\"', '#', '$', '%', '&', ',',
    '^', '_', '*', '+', '\'', '|', '.', '/',
/* 0x30 */
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', ':', ';', '^', '!', 'v', '\?',
/* 0x40 */
    '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
/* 0x50 */
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', '-', '\\', '-', '<', '|',
/* 0x60 */
    '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', '~', 'm', 'n', 'o',
/* 0x70 */
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
    'x', 'y', 'z', '-', '-', '-', 'l', 0x7f,
/* 0x80 */
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
/* 0x90 */
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
/* 0xA0 */
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, '^', 'v',
/* 0xB0 */
    0xb0, 0xb1, 0xb2, 0xc4, 0xc1, 0xd0, 0xcf, 0xd4,
    0xd3, 0xca, 0xcd, 0xc8, 0xbc, 0xbe, 0xbd, 0xc0,
/* 0xC0 */
    0xbf, 0xb4, 0xc3, 0xc2, 0xb3, 0xc5, 0xd2, 0xd1,
    0xbb, 0xc9, 0xb9, 0xcc, 0xcb, 0xba, 0xce, 0xb6,
/* 0xD0 */
    0xb5, 0xc7, 0xc6, 0xb8, 0xb7, 0xd6, 0xd5, 0xd8,
    0xd7, 0xd9, 0xda, 0xdb, 0xde, 0xdf, 0xdc, 0xdd,
/* 0xE0 */
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xeb, '8', 0xed, 'm', 'c',
/* 0xF0 */
    0xba, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, '$',
    0xf8, 0xf9, 0xfa, '7', 0xfc, 0xfd, 0xfe, 0xff
};

static const char MYMAN_LINEARSCALE[] =
    "\x0\x0\x1\x1\x2\x2\x3\x3\x4\x4\x5\x5\x6\x6\x7\x7"
    "\x8\x8\x9\x9\xa\xa\xb\xb\xc\xc\xd\xd\xe\xe\xf\xf"
    "\x10\x10\x11\x11\x12\x12\x13\x13\x14\x14\x15\x15\x16\x16\x17\x17"
    "\x18\x18\x19\x19\x1a\x1a\x1b\x1b\x1c\x1c\x1d\x1d\x1e\x1e\x1f\x1f"
    "\x20\x20\x21\x21\x22\x22\x23\x23\x24\x24\x25\x25\x26\x26\x27\x27"
    "\x28\x28\x29\x29\x2a\x2a\x2b\x2b\x2c\x2c\x2d\x2d\x2e\x2e\x2f\x2f"
    "\x30\x30\x31\x31\x32\x32\x33\x33\x34\x34\x35\x35\x36\x36\x37\x37"
    "\x38\x38\x39\x39\x3a\x3a\x3b\x3b\x3c\x3c\x3d\x3d\x3e\x3e\x3f\x3f"
    "\x40\x40\x41\x41\x42\x42\x43\x43\x44\x44\x45\x45\x46\x46\x47\x47"
    "\x48\x48\x49\x49\x4a\x4a\x4b\x4b\x4c\x4c\x4d\x4d\x4e\x4e\x4f\x4f"
    "\x50\x50\x51\x51\x52\x52\x53\x53\x54\x54\x55\x55\x56\x56\x57\x57"
    "\x58\x58\x59\x59\x5a\x5a\x5b\x5b\x5c\x5c\x5d\x5d\x5e\x5e\x5f\x5f"
    "\x60\x60\x61\x61\x62\x62\x63\x63\x64\x64\x65\x65\x66\x66\x67\x67"
    "\x68\x68\x69\x69\x6a\x6a\x6b\x6b\x6c\x6c\x6d\x6d\x6e\x6e\x6f\x6f"
    "\x70\x70\x71\x71\x72\x72\x73\x73\x74\x74\x75\x75\x76\x76\x77\x77"
    "\x78\x78\x79\x79\x7a\x7a\x7b\x7b\x7c\x7c\x7d\x7d\x7e\x7e\x7f\x7f"
    "\x80\x80\x81\x81\x82\x82\x83\x83\x84\x84\x85\x85\x86\x86\x87\x87"
    "\x88\x88\x89\x89\x8a\x8a\x8b\x8b\x8c\x8c\x8d\x8d\x8e\x8e\x8f\x8f"
    "\x90\x90\x91\x91\x92\x92\x93\x93\x94\x94\x95\x95\x96\x96\x97\x97"
    "\x98\x98\x99\x99\x9a\x9a\x9b\x9b\x9c\x9c\x9d\x9d\x9e\x9e\x9f\x9f"
    "\xa0\xa0\xa1\xa1\xa2\xa2\xa3\xa3\xa4\xa4\xa5\xa5\xa6\xa6\xa7\xa7"
    "\xa8\xa8\xa9\xa9\xaa\xaa\xab\xab\xac\xac\xad\xad\xae\xae\xaf\xaf"
    "\xb0\xb0\xb1\xb1\xb2\xb2\xb3\xb3\xb4\xb4\xb5\xb5\xb6\xb6\xb7\xb7"
    "\xb8\xb8\xb9\xb9\xba\xba\xbb\xbb\xbc\xbc\xbd\xbd\xbe\xbe\xbf\xbf"
    "\xc0\xc0\xc1\xc1\xc2\xc2\xc3\xc3\xc4\xc4\xc5\xc5\xc6\xc6\xc7\xc7"
    "\xc8\xc8\xc9\xc9\xca\xca\xcb\xcb\xcc\xcc\xcd\xcd\xce\xce\xcf\xcf"
    "\xd0\xd0\xd1\xd1\xd2\xd2\xd3\xd3\xd4\xd4\xd5\xd5\xd6\xd6\xd7\xd7"
    "\xd8\xd8\xd9\xd9\xda\xda\xdb\xdb\xdc\xdc\xdd\xdd\xde\xde\xdf\xdf"
    "\xe0\xe0\xe1\xe1\xe2\xe2\xe3\xe3\xe4\xe4\xe5\xe5\xe6\xe6\xe7\xe7"
    "\xe8\xe8\xe9\xe9\xea\xea\xeb\xeb\xec\xec\xed\xed\xee\xee\xef\xef"
    "\xf0\xf0\xf1\xf1\xf2\xf2\xf3\xf3\xf4\xf4\xf5\xf5\xf6\xf6\xf7\xf7"
    "\xf8\xf8\xf9\xf9\xfa\xfa\xfb\xfb\xfc\xfc\xfd\xfd\xfe\xfe\xff\xff"
    "\0";

short mille_to_scale(short n, short scale)
{
    return (short) (((long) (scale - 1)) * ((long) (unsigned char) MYMAN_LINEARSCALE[n * 511L / 1000]) / 255);
}

/* read a utf-8 sequence from stream, convert it to cp437, and return
 * it. unmappable sequences are silently converted to spaces. this
 * theoretically works with U+0000 .. U+D7FF and U+E000 .. U+10FFFF */

#if 0
int fputc_utf8(unsigned long u, FILE *stream);
#endif

int
fgetc_cp437_utf8(FILE *stream)
{
    int c;

    c = fgetc(stream);
    if (c >= 0x80)
    {
        unsigned long u;
        int i;

        u = 0x20;
        if ((c >= 0xc2) && (c <= 0xdf))
        {
            int c1;

            c1 = fgetc(stream);
            if ((c1 >= 0x80) && (c1 <= 0xbf))
            {
                u = ((((unsigned long) c) & 0x1f) << 6) | (((unsigned long) c1) & 0x3f);
            }
            else if (c1 != EOF)
            {
                ungetc(c1, stream);
            }
        }
        else if ((c >= 0xe0) && (c <= 0xef))
        {
            int c1;

            c1 = fgetc(stream);
            if ((c1 >= ((c == 0xe0) ? 0xa0 : 0x80)) && (c1 <= ((c == 0xed) ? 0x9f : 0xbf)))
            {
                int c2;

                c2 = fgetc(stream);
                if ((c2 >= 0x80) && (c2 <= 0xbf))
                {
                    u = ((((unsigned long) c) & 0x0f) << 12) | ((((unsigned long) c1) & 0x3f) << 6) | (((unsigned long) c2) & 0x3f);
                }
                else if (c2 != EOF)
                {
                    ungetc(c2, stream);
                }
            }
            else if (c1 != EOF)
            {
                ungetc(c1, stream);
            }
        }
        else if ((c >= 0xf0) && (c <= 0xf4))
        {
            int c1;

            c1 = fgetc(stream);
            if ((c1 >= ((c == 0xf0) ? 0x90 : 0x80)) && (c1 <= ((c == 0xf4) ? 0x8f : 0xbf)))
            {
                int c2;

                c2 = fgetc(stream);
                if ((c2 >= 0x80) && (c2 <= 0xbf))
                {
                    int c3;

                    c3 = fgetc(stream);
                    if ((c3 >= 0x80) && (c3 <= 0xbf))
                    {
                        u = ((((unsigned long) c) & 0x07) << 18) | ((((unsigned long) c1) & 0x3f) << 12) | ((((unsigned long) c2) & 0x3f) << 6) | (((unsigned long) c3) & 0x3f);
                    }
                    else if (c3 != EOF)
                    {
                        ungetc(c3, stream);
                    }
                }
                else if (c2 != EOF)
                {
                    ungetc(c2, stream);
                }
            }
            else if (c1 != EOF)
            {
                ungetc(c1, stream);
            }
        }
        c = ' ';
        for (i = 0; i <= 0xff; i ++)
        {
            if (uni_cp437_halfwidth[i] == u)
            {
                c = i;
                break;
            }
        }
        if (i > 0xff)
        {
            for (i = 0x20; i <= 0x11f; i ++)
            {
                if (uni_cp437_fullwidth[i & 0xff] == u)
                {
                    c = i & 0xff;
                    break;
                }
            }
        }
        if (i > 0x11f)
        {
            if ((u >= 0xff01) && (u <= 0xff5f))
            {
                /* FULLWIDTH ASCII -> ASCII */
                c = (int) (u + 0x20 - 0xff00);
            }
            else if ((u == 0x85) ||
                     (u == 0x2028) ||
                     (u == 0x2029))
            {
                /* NEL, LINE SEPARATOR and PARAGRAPH SEPARATOR -> LINE
                 * FEED */
                c = '\n';
            }
        }
    }
#if 0
    fputc_utf8(((c == '\v') || (c == '\f') || (c == '\n') || (c == '\r')) ? c : uni_cp437[(unsigned int) (unsigned char) c], stderr);
#endif
    return c;
}

/* like fputc, but converts unicode to a utf-8 sequence. this
 * theoretically works with U+0000 .. U+D7FF and U+E000 .. U+10FFFF;
 * note that the return value is a bit different -- EOF for failure, 0
 * for success */

int fputc_utf8(unsigned long u, FILE *stream)
{
    if (u <= 0x7f)
    {
        if (fputc(u, stream) == EOF)
        {
            return EOF;
        }
    }
    else if (u <= 0x7ff)
    {
        if (fputc(0xc0 | (u >> 6), stream) == EOF)
        {
            return EOF;
        }
        else if (fputc(0x80 | (u & 0x3f), stream) == EOF)
        {
            return EOF;
        }
    }
    else if (u <= 0xffff)
    {
        if (fputc(0xe0 | (u >> 12), stream) == EOF)
        {
            return EOF;
        }
        else if (fputc(0x80 | ((u >> 6) & 0x3f), stream) == EOF)
        {
            return EOF;
        }
        else if (fputc(0x80 | (u & 0x3f), stream) == EOF)
        {
            return EOF;
        }
    }
    else if (u <= 0x10ffff)
    {
        if (fputc(0xf0 | (u >> 18), stream) == EOF)
        {
            return EOF;
        }
        else if (fputc(0x80 | ((u >> 12) & 0x3f), stream) == EOF)
        {
            return EOF;
        }
        else if (fputc(0x80 | ((u >> 6) & 0x3f), stream) == EOF)
        {
            return EOF;
        }
        else if (fputc(0x80 | (u & 0x3f), stream) == EOF)
        {
            return EOF;
        }
    }
    else
    {
        return EOF;
    }
    return 0;
}

/* like fputc, but converts cp437 to a utf-8 sequence. this
 * theoretically works with U+0000 .. U+D7FF and U+E000 .. U+10FFFF;
 * adding 0x100 ignores CJK mode */

int fputc_utf8_cp437(int c, FILE *stream)
{
    unsigned long u;

    u = 0;
    if ((c < 0) || (c > 0x1ff))
    {
        return EOF;
    }
    u = (c & 0x100) ? uni_cp437_halfwidth[((int) (unsigned char) c) & 0xff] : uni_cp437[c];
    if (fputc_utf8(u, stream) == EOF)
    {
        return EOF;
    }
    return c;
}

/* like ungetc, but converts cp437 to a utf-8 sequence and puts that
 * back one byte at a time. this theoretically works with U+0000
 * .. U+D7FF and U+E000 .. U+10FFFF */

int ungetc_cp437_utf8(int c, FILE *stream)
{
    unsigned long u;

    u = 0;
    if ((c < 0) || (c > 0xff))
    {
        return EOF;
    }
    u = uni_cp437[c];
    if (u <= 0x7f)
    {
        if (ungetc(u, stream) == EOF)
        {
            return EOF;
        }
    }
    else if (u <= 0x7ff)
    {
        if (ungetc(0x80 | (u & 0x3f), stream) == EOF)
        {
            return EOF;
        }
        else if (ungetc(0xc0 | (u >> 6), stream) == EOF)
        {
            fgetc(stream);
            return EOF;
        }
    }
    else if (u <= 0xffff)
    {
        if (ungetc(0x80 | (u & 0x3f), stream) == EOF)
        {
            return EOF;
        }
        else if (ungetc(0x80 | ((u >> 6) & 0x3f), stream) == EOF)
        {
            fgetc(stream);
            return EOF;
        }
        else if (ungetc(0xe0 | (u >> 12), stream) == EOF)
        {
            fgetc(stream);
            fgetc(stream);
            return EOF;
        }
    }
    else if (u <= 0x10ffff)
    {
        if (ungetc(0x80 | (u & 0x3f), stream) == EOF)
        {
            return EOF;
        }
        else if (ungetc(0x80 | ((u >> 6) & 0x3f), stream) == EOF)
        {
            fgetc(stream);
            return EOF;
        }
        else if (ungetc(0x80 | ((u >> 12) & 0x3f), stream) == EOF)
        {
            fgetc(stream);
            fgetc(stream);
            return EOF;
        }
        else if (ungetc(0xf0 | (u >> 18), stream) == EOF)
        {
            fgetc(stream);
            fgetc(stream);
            fgetc(stream);
            return EOF;
        }
    }
    else
    {
        return EOF;
    }
    return c;
}

char *
strword(const char *from, const char **endp, size_t *lenp)
{
    char *word = NULL;
    size_t wordlen = 0;
    int quotes = 0;
    int escape = 0;
    unsigned long numeric = 0;
    int numeric_len = 0;
    char c;

    word = strdup("");
    if (! word) return word;
    while (1)
    {
        char *newword;

        c = *from;
        if (! c) break;
        from ++;
        if ((escape == 0)
            &&
            (c == quotes))
        {
            quotes = 0;
            continue;
        }
        if ((escape == 0)
            &&
            (quotes == 0))
        {
            if ((c == '\t') || (c == ' ') || (c == '\n') || (c == '\t') || (c == '\v') || (c == '\f') || (c == '\r'))
            {
                from --;
                break;
            }
            if ((c == '\'') || (c == '\"'))
            {
                quotes = c;
                continue;
            }
        }
        if (escape == 0)
        {
            if (c == '\\')
            {
                escape = c;
                continue;
            }
        }
        if (escape == '\\')
        {
            switch(c)
            {
            case 'a':
                /* in ANSI C, but not handled by unprotoize */
                escape = 0;
                c = '\7';
                break;
            case 'b':
                escape = 0;
                c = '\b';
                break;
            case 't':
                escape = 0;
                c = '\t';
                break;
            case 'v':
                escape = 0;
                c = '\v';
                break;
            case 'f':
                escape = 0;
                c = '\f';
                break;
            case 'n':
                escape = 0;
                c = '\n';
                break;
            case 'r':
                escape = 0;
                c = '\r';
                break;
            case 'e':
                /* GNU extension */
                escape = 0;
                c = '\x1b';
                break;
            case 'u':
                escape = 'u';
                numeric = 0;
                numeric_len = 0;
                continue;
            case 'U':
                escape = 'U';
                numeric = 0;
                numeric_len = 0;
                continue;
            case 'x':
            case 'X':
                escape = 'x';
                numeric = 0;
                numeric_len = 0;
                continue;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                escape = '0';
                numeric = c - '0';
                numeric_len = 1;
                continue;
            case '\\':
            case '\"':
            case '\'':
            case '\?':
            default:
                escape = 0;
                break;
            }
        }
        if ((escape == 'u') || (escape == 'U'))
        {
            numeric *= 16;
            numeric_len ++;
            if ((c >= '0') && (c <= '9'))
            {
                numeric += c - '0';
            }
            else if ((c >= 'A') && (c <= 'F'))
            {
                numeric += c - 'A' + 10;
            }
            else if ((c >= 'a') && (c <= 'f'))
            {
                numeric += c - 'a' + 10;
            }
            else
            {
                break;
            }
            if (numeric_len == ((escape == 'U') ? 8 : 4))
            {
                int i;

                escape = 0;
                c = ' ';
                for (i = 0; i <= 0xff; i ++)
                {
                    if (uni_cp437_halfwidth[i] == numeric)
                    {
                        c = i;
                        break;
                    }
                }
                if (i > 0xff)
                {
                    for (i = 0x20; i <= 0x11f; i ++)
                    {
                        if (uni_cp437_fullwidth[i & 0xff] == numeric)
                        {
                            c = i & 0xff;
                            break;
                        }
                    }
                }
                if (i > 0x11f)
                {
                    if ((numeric >= 0xff01UL) && (numeric <= 0xff5fUL))
                    {
                        /* FULLWIDTH ASCII -> ASCII */
                        c = (int) (numeric + 0x20 - 0xff00UL);
                    }
                    else if ((numeric == 0x85) ||
                             (numeric == 0x2028UL) ||
                             (numeric == 0x2029UL))
                    {
                        /* NEL, LINE SEPARATOR and PARAGRAPH SEPARATOR -> LINE
                         * FEED */
                        c = '\n';
                    }
                }
            }
        }
        if (escape == 'x')
        {
            if ((c >= '0') && (c <= '9'))
            {
                numeric *= 16;
                numeric_len ++;
                numeric += c - '0';
            }
            else if ((c >= 'A') && (c <= 'F'))
            {
                numeric *= 16;
                numeric_len ++;
                numeric += c - 'A' + 10;
            }
            else if ((c >= 'a') && (c <= 'f'))
            {
                numeric *= 16;
                numeric_len ++;
                numeric += c - 'a' + 10;
            }
            else if (! numeric_len)
            {
                break;
            }
            else
            {
                from --;
                c = numeric;
                escape = 0;
            }
            if ((numeric_len == 2) && (escape == 'x'))
            {
                escape = 0;
                c = numeric;
            }
        }
        if (escape == '0')
        {
            if ((c >= '0') && (c <= '7'))
            {
                numeric *= 8;
                numeric_len ++;
                numeric += c - '0';
            }
            else if (! numeric_len)
            {
                break;
            }
            else
            {
                from --;
                c = numeric;
                escape = 0;
            }
            if ((numeric_len == 3) && (escape == '0'))
            {
                escape = 0;
                c = numeric;
            }
        }
        if (! escape)
        {
            newword = (char *) realloc((void *) word, wordlen + 2);
            if (! newword)
            {
                free((void *) word);
                word = NULL;
                break;
            }
            word = newword;
            word[wordlen] = c;
            wordlen ++;
            word[wordlen] = '\0';
        }
    }
    if (quotes || escape || (word && (wordlen != strlen(word)) && ! lenp))
    {
        errno = EINVAL;
        if (word)
        {
            free((void *) word);
            word = NULL;
        }
    }
    if (word && endp) *endp = from;
    if (word && lenp) *lenp = wordlen;
    return word;
}

long *strtollist(const char *from, const char **endp, size_t *lenp)
{
    long *list = NULL;
    size_t listlen = 0;

    list = (long *) malloc(sizeof(long));
    if (! list)
    {
        return list;
    }
    list[listlen] = 0;
    while (*from)
    {
        long tmp;
        char *endp_tmp;
        int errno_tmp;
        int errno_strtol;

        while (*from && isspace(*from))
        {
            from ++;
        }
        if (! *from)
        {
            break;
        }
        errno_tmp = errno;
        errno = 0;
        tmp = strtol(from, &endp_tmp, 0);
        errno_strtol = errno;
        if (! errno) errno = errno_tmp;
        if (((tmp == LONG_MAX) || (tmp == LONG_MIN)) && errno_strtol && (errno_strtol != EINVAL))
        {
            if (list) free((void *) list);
            list = NULL;
            break;
        }
        else if (endp_tmp == from)
        {
            break;
        }
        else
        {
            long *list_tmp;

            from = endp_tmp;
            list_tmp = (long *) realloc((void *) list, (listlen + 2) * sizeof(long));
            if (! list_tmp)
            {
                free((void *) list);
                list = NULL;
                break;
            }
            list = list_tmp;
            list[listlen] = tmp;
            listlen = listlen + 1;
            list[listlen] = 0;
            while (*from && isspace(*from))
            {
                from ++;
            }
            if (*from != ',')
            {
                break;
            }
            else
            {
                from ++;
            }
        }
    }
    if (list && *from && ! isspace(*from))
    {
        errno = EINVAL;
        if (list) free((void *) list);
        list = NULL;
        listlen = 0;
    }
    if (list && endp) *endp = from;
    if (list) *lenp = listlen;
    return list;
}

double *strtodlist(const char *from, const char **endp, size_t *lenp)
{
    double *list = NULL;
    size_t listlen = 0;

    list = (double *) malloc(sizeof(double));
    if (! list)
    {
        return list;
    }
    list[listlen] = 0;
    while (*from)
    {
        double tmp;
        char *endp_tmp;
        int errno_tmp;
        int errno_strtod;

        while (*from && isspace(*from))
        {
            from ++;
        }
        if (! *from)
        {
            break;
        }
        errno_tmp = errno;
        errno = 0;
        tmp = strtod(from, &endp_tmp);
        errno_strtod = errno;
        if (! errno) errno = errno_tmp;
        if (((tmp == HUGE_VAL) || (tmp == -HUGE_VAL)) && errno_strtod && (errno_strtod != EINVAL))
        {
            if (list) free(list);
            list = NULL;
            break;
        }
        else if (endp_tmp == from)
        {
            break;
        }
        else
        {
            double *list_tmp;

            from = endp_tmp;
            list_tmp = (double *) realloc((void *) list, (listlen + 2) * sizeof(double));
            if (! list_tmp)
            {
                free((void *) list);
                list = NULL;
                break;
            }
            list = list_tmp;
            list[listlen] = tmp;
            listlen = listlen + 1;
            list[listlen] = 0;
            while (*from && isspace(*from))
            {
                from ++;
            }
            if (*from != ',')
            {
                break;
            }
            else
            {
                from ++;
            }
        }
    }
    if (list && *from && ! isspace(*from))
    {
        errno = EINVAL;
        if (list) free((void *) list);
        list = NULL;
        listlen = 0;
    }
    if (list && endp) *endp = from;
    if (list) *lenp = listlen;
    return list;
}

long *strtollist_word(const char *from, const char **endp, size_t *lenp)
{
    long *list = NULL;
    char *word = NULL;
    size_t word_len = 0;

    *lenp = 0;
    word = strword(from, endp, &word_len);
    if (word)
    {
        const char *list_end;
        size_t i;

        for (i = 0; i < word_len; i ++)
        {
            /* convert NUL -> SP */
            if (! word[i]) word[i] = ' ';
        }
        list = strtollist(word, &list_end, lenp);
        if (*list_end)
        {
            free((void *) list);
            list = NULL;
            errno = EINVAL;
        }
        free((void *) word);
    }
    return list;
}

double *strtodlist_word(const char *from, const char **endp, size_t *lenp)
{
    double *list = NULL;
    char *word = NULL;
    size_t word_len = 0;

    *lenp = 0;
    word = strword(from, endp, &word_len);
    if (word)
    {
        const char *list_end;
        size_t i;

        for (i = 0; i < word_len; i ++)
        {
            /* convert NUL -> SP */
            if (! word[i]) word[i] = ' ';
        }
        list = strtodlist(word, &list_end, lenp);
        if (*list_end)
        {
            free((void *) list);
            list = NULL;
            errno = EINVAL;
        }
        free((void *) word);
    }
    return list;
}

#define MYMAN_ISPRINT(c) ((' ' == 0x20) ? (((c) >= 0x20) && ((c) <= 0x7e)) : isprint(c))

void
mymanescape(const char *s, int len)
{
    int c;
    int i;

    for (i = 0; i < len; i ++)
        if (MYMAN_ISPRINT((c = (int) (unsigned char) s[i]))) {
            if ((c == '\"') || (c == '\\') || (c == '\'') || (c == '\?'))
                putchar('\\');
            putchar(c);
        }
        else
        {
            if (((i + 1 == len) || ! isdigit((int) (unsigned char) s[i + 1])) && (c == '\0'))
                printf("\\0");
            else
                printf("\\%3.3o", c);
        }
}

const char *progname = NULL;

static FILE *fopen_datafile(const char *path, const char *mode)
{
    char *buf = NULL;
    FILE *ret = NULL;

    ret = fopen(path, mode);
#ifdef macintosh
    /* FIXME: we roll our own sleazy POSIX->mac filename conversion
     * hack that really only works for completely ASCII filenames. */
    if (strrchr(path, '/') && (! ret))
    {
        int i;

        i = 0;
        if ((! strncmp(path, "/Volumes/", strlen("/Volumes/")))
            &&
            strchr(path + strlen("/Volumes/"), '/'))
        {
            /* /Volumes/volName/foo -> volName:foo */
            buf = strdup(path + strlen("/Volumes/"));
        }
        else if (*path == '/')
        {
            Str255 volName;
            short vRefNum;
            long dirID;

            if (noErr == HGetVol(volName, &vRefNum, &dirID))
            {
                buf = (char *) malloc(volName[0] + strlen(path) + 1);
                if (buf)
                {
                    i = volName[0];
                    /* /foo -> volName:foo */
                    memcpy((void *) buf, (void *) (volName + 1), i);
                    memcpy((void *) (buf + i), (void *) path, strlen(path) + 1);
                }
            }
        }
        else if ((! strchr(path, ':'))
                 ||
                 (strchr(path, ':') > strchr(path, '/')))
        {
            /* foo/bar -> :foo:bar */
            buf = strdup(path);
            if (buf)
            {
                char *buf2;

                buf2 = (char *) realloc((void *) buf, strlen(buf) + 2);
                if (buf2)
                {
                    buf = buf2;
                    memmove((void *) (buf + 1), (void *) buf, strlen(buf) + 1);
                    *buf = '/';
                }
                else
                {
                    free((void *) buf);
                    buf = NULL;
                }
            }
        }
        if (buf)
        {
            /* '//' -> '/' */
            while (strstr(buf + i, "//"))
            {
                char *slashslash;

                slashslash = strstr(buf + i, "//");
                memmove(slashslash + strlen("/"), slashslash + strlen("//"), strlen(slashslash + strlen("//")) + 1);
            }
            /* '/./' -> '/' */
            while (strstr(buf + i, "/./"))
            {
                char *slashdotslash;

                slashdotslash = strstr(buf + i, "/./");
                memmove(slashdotslash + strlen("/"), slashdotslash + strlen("/./"), strlen(slashdotslash + strlen("/./")) + 1);
            }
            /* '/../' -> '//' */
            while (strstr(buf + i, "/../"))
            {
                char *dotdot;

                dotdot = strstr(buf + i, "/../");
                sprintf(dotdot, "//");
                memmove(dotdot + strlen("//"), dotdot + strlen("/../"), strlen(dotdot + strlen("/../")) + 1);
            }
            /* swap '/' and ':' */
            for (; buf[i]; i ++)
            {
                if (buf[i] == '/')
                {
                    buf[i] = ':';
                }
                else if (buf[i] == ':')
                {
                    buf[i] = '/';
                }
            }
            ret = fopen(buf, mode);
            free((void *) buf);
            buf = NULL;
        }
    }
#endif
    if (progname && *progname && (! ret))
    {
#ifdef VMS

#pragma __nostandard

        const char *progname_posix = NULL;

        progname_posix = decc$translate_vms(progname);
        if (! progname_posix) progname_posix = progname;

#undef progname
#define progname progname_posix

#pragma __standard

#endif /* defined(VMS) */
        buf = (char *) malloc(strlen(progname) + 1 + strlen(path) + 1);
        if (buf)
        {
            char *sep;

            strcpy(buf, progname);
            sep = strrchr(buf, '/');
            if (! sep)
            {
                sep = buf;
            }
#if defined(WIN32) || defined(__MSDOS__)
            if (strrchr(sep, '\\'))
            {
                sep = strrchr(sep, '\\');
            }
#endif
#ifdef macintosh
            if (strrchr(sep, ':'))
            {
                sep = strrchr(sep, ':');
            }
#endif
            if (sep && (sep != buf))
            {
                sep += 1;
                sep[0] = '\0';
                strcpy(sep, path);
                ret = fopen(buf, mode);
                if (! ret)
                {
                    strcpy(buf, progname);
#ifdef VMS
#undef progname
#endif /* defined(VMS) */
                    sep = strstr(buf, ".app/");
                    if (sep)
                    {
                        sep += strlen(".app");
                        sep[0] = '\0';
                    }
#if defined(WIN32) || defined(__MSDOS__)
                    if (strstr(buf, ".app\\"))
                    {
                        sep = strstr(buf, ".app\\");
                        sep += strlen(".app");
                        sep[0] = '\0';
                    }
#endif
#ifdef macintosh
                    if (strstr(buf, ".app:"))
                    {
                        sep = strstr(buf, ".app:");
                        sep += strlen(".app");
                        sep[0] = '\0';
                    }
#endif
                    if (sep)
                    {
                        sep = strrchr(buf, '/');
                        if (! sep)
                        {
                            sep = buf;
                        }
#if defined(WIN32) || defined(__MSDOS__)
                        if (strrchr(sep, '\\'))
                        {
                            sep = strrchr(sep, '\\');
                        }
#endif
#ifdef macintosh
                        if (strrchr(sep, ':'))
                        {
                            sep = strrchr(sep, ':');
                        }
#endif
                        if (sep && (sep != buf))
                        {
                            sep += 1;
                            sep[0] = '\0';
                            strcpy(sep, path);
                            ret = fopen(buf, mode);
                        }
                    }
                }
            }
            free((void *) buf);
            buf = NULL;
        }
    }
#ifdef PRIVATEDATADIR
    if ((! ret) && strncmp(path, PRIVATEDATADIR, strlen(PRIVATEDATADIR)))
    {
        buf = (char *) malloc(strlen(PRIVATEDATADIR) + 1 + strlen(path) + 1);
        if (buf)
        {
            sprintf(buf,
                    "%s/%s",
                    strlen(PRIVATEDATADIR) ? PRIVATEDATADIR : ".",
                    path);
            ret = fopen_datafile(buf, mode);
            free((void *) buf);
            buf = NULL;
        }
    }
#endif
    if ((! ret)
        &&
        ((strlen(path) < strlen(".txt"))
         ||
         (strcmp(path + strlen(path) - strlen(".txt"), ".txt"))))
    {
        buf = (char *) malloc(strlen(path) + strlen(".txt") + 1);
        if (buf)
        {
            sprintf(buf, "%s%s", path, ".txt");
            ret = fopen_datafile(buf, mode);
            free((void *) buf);
            buf = NULL;
        }
    }
    return ret;
}

int
readfont(const char *fontfile,
         int *w,
         int *h,
         const char **font,
         int *used,
         int *flags,
         int *color,
         const char **args)
{
    FILE *infile;
    int c, i, j, k;
    int rw, rh;
    char X;
    char *font_dynamic[256];

    *args = NULL;
    *flags = 0;
    for (i = 0; i < 256; i ++) {
        used[i] = 0;
        color[i] = 0;
    }
    infile = fopen_datafile(fontfile, "rb");
    if (! infile) {
        perror(fontfile);
        return 1;
    }
    ignore_bom_utf8(infile);
    if ((fscanf(infile, "%d%c%d", &rw, &X, &rh) != 3) ||
        (tolower(X) != 'x')) {
        fprintf(stderr, "%s: can't find a dimension specification WxH\n",
                fontfile);
        return 1;
    }
    *w = rw;
    *h = rh;
    for (i = 0; i <256; i ++)
    {
        font[i] = NULL;
    }
    for (i = 0; i <256; i ++)
    {
        font_dynamic[i] = (char *) malloc(rh * rw);
        if (! font_dynamic[i])
        {
            perror("malloc");
            for (j = 0; j < i; j ++)
            {
                free((void *) font_dynamic[j]);
            }
            return 1;
        }
    }
    memcpy((void *) font, (void *) font_dynamic, sizeof(font_dynamic));
    if (! feof(infile))
    {
        c = fgetc_cp437_utf8(infile);
        if (c == '~')
        {
            if (fscanf(infile, "%d", flags) != 1)
            {
                fprintf(stderr, "%s: can't find flags ~F after dimension specification %dx%d\n",
                        fontfile,
                        rw, rh);
                return 1;
            }
        }
        else
        {
            ungetc_cp437_utf8(c, infile);
        }
    }
    if (! feof(infile))
    {
        c = fgetc_cp437_utf8(infile);
        if ((c == ' ') || (c == '\t'))
        {
            char *args_tmp = NULL;
            size_t args_tmp_len = 0;
            int escaped = 0;

            args_tmp = strdup("");
            if (! args_tmp)
            {
                perror("strdup");
                return 1;
            }
            while (((c = fgetc_cp437_utf8(infile)) != EOF)
                   &&
                   (c != '\v') && (c != '\f') && (c != '\n') && (c != '\r'))
            {
                if (c == '\\')
                {
                    int c2;

                    escaped = ! escaped;
                    if (escaped)
                    {
                        c2 = fgetc_cp437_utf8(infile);
                        if (c2 == '\r')
                        {
                            int c3;

                            c3 = fgetc_cp437_utf8(infile);
                            if (c3 == '\n')
                            {
                                c2 = c3;
                            }
                            else
                            {
                                ungetc_cp437_utf8(c3, infile);
                            }
                        }
                        if ((c2 == '\v') || (c2 == '\f') || (c2 == '\n') || (c2 == '\r'))
                        {
                            escaped = 0;
                            continue;
                        }
                        ungetc_cp437_utf8(c2, infile);
                    }
                }
                args_tmp = (char *) realloc((void *) args_tmp, args_tmp_len + 1 + ((c == 0) ? 4 : 1));
                if (! args_tmp)
                {
                    perror("realloc");
                    return 1;
                }
                if (c == 0)
                {
                    if (! escaped)
                    {
                        args_tmp[args_tmp_len] = '\\';
                        args_tmp_len ++;
                    }
                    args_tmp[args_tmp_len] = 'x';
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '0';
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '0';
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '\0';
                }
                else
                {
                    args_tmp[args_tmp_len] = c;
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '\0';
                }
                if (escaped && (c != '\\'))
                {
                    escaped = 0;
                }
            }
            if (args_tmp_len)
            {
                *args = args_tmp;
            }
            else
            {
                free((void *) args_tmp);
                args_tmp = NULL;
            }
        }
        if ((c == '\n') || (c == '\r') || (c == '\v') || (c == '\f'))
        {
            ungetc(c, infile);
        }
        else
        {
            ungetc_cp437_utf8(c, infile);
        }
    }
    while (! feof(infile))
    {
        if (fscanf(infile, "%x", &i) != 1)
        {
            if (! feof(infile))
            {
                long offset;

                offset = ftell(infile);
                fprintf(stderr, "%s: can't find an index at byte %ld near \'\\u%4.4lx\' or equivalent\n",
                        fontfile, offset, uni_cp437[(unsigned int) (unsigned char) fgetc_cp437_utf8(infile)]);
                return 1;
            }
            continue;
        }
        if ((i < 0) || (i > 255))
        {
            fprintf(stderr, "%s: invalid index %2.2X ignored\n", fontfile, i);
            continue;
        }
        if (used[i])
            fprintf(stderr, "%s: duplicate definition for %2.2X\n", fontfile, i);
        used[i] = 1;
        if ((c = fgetc_cp437_utf8(infile)) == '~')
        {
            if (fscanf(infile, "%x", &c) != 1)
            {
                if (! feof(infile))
                {
                    fprintf(stderr, "%s: can't find a color for index %2.2X\n",
                            fontfile, i);
                    return 1;
                }
                continue;
            }
            if ((c < 0) || (c >= NPENS))
                fprintf(stderr, "%s: invalid color %2.2X ignored\n", fontfile, c);
            else
                color[i] = c;
        }
        else
            ungetc_cp437_utf8(c, infile);
        for (j = 0; j < rh; j ++)
            for (k = 0; k < rw; k ++)
                font_dynamic[i][j * rw + k] = ' ';
        for (j = 0; j < rh; j ++)
        {
            while ((c = fgetc_cp437_utf8(infile)) != ':')
                if (c == EOF)
                {
                    if (! feof(infile))
                        perror(fontfile);
                    else
                        fprintf(stderr, "%s: premature EOF in index %2.2X\n",
                                fontfile, i);
                    return 1;
                }
            if ((c = fgetc_cp437_utf8(infile)) == EOF)
                if (! feof(infile))
                {
                    perror(fontfile);
                    return 1;
                }
            for (k = 0;
                 (k < rw) &&
                     (c != '\v') && (c != '\f') &&
                     (c != '\n') && (c != '\r') && ! feof(infile);
                 k ++)
            {
                font_dynamic[i][j * rw + k] = c;
                if ((c = fgetc_cp437_utf8(infile)) == EOF) {
                    if (feof(infile))
                        continue;
                    perror(fontfile);
                    return 1;
                }
            }
            while ((c != '\v') && (c != '\f') &&
                   (c != '\n') && (c != '\r') && ! feof(infile)) {
                if (c == EOF) {
                    if (feof(infile))
                        continue;
                    perror(fontfile);
                    return 1;
                }
                c = fgetc_cp437_utf8(infile);
            }
        }
    }
    fclose(infile);
    return 0;
}

extern void
writefont(const char *file,
          const char *prefix,
          int w, int h,
          const char **font,
          int *used,
          int flags,
          int *color,
          const char *args)
{
    int c, i, j;

    printf("static const char builtin_%sfile_str[] = \"", prefix);
    mymanescape(file, strlen(file));
    printf("\";\n");
    printf("const char *builtin_%sfile = builtin_%sfile_str;\n", prefix, prefix);
    printf("int %s_flags = %d;\n", prefix, flags);
    printf("int %s_w = %d;\n", prefix, w);
    printf("int %s_h = %d;\n", prefix, h);
    printf("const char *%s_args = ", prefix);
    if (args)
    {
        printf("\"");
        mymanescape(args, strlen(args));
        printf("\"");
    }
    else
    {
        printf("0");
    }
    printf(";\n");
    for (c = 0; c < 256; c ++)
    {
        printf("static const char builtin_%s_data_%d[%d * %d] = {", prefix, c, h, w);
        for (i = 0; i < h; i ++)
        {
            printf("\n ");
            for (j = 0; j < w; j ++)
            {
                char k;

                printf(" \'");
                k = font[c][i * w + j];
                mymanescape(&k, 1);
                printf("\'");
                if (((i + 1) < h) || ((j + 1) < w))
                    printf(",");
            }
        }
        printf("};\n");
    }
    printf("const char *%s[256] = {", prefix);
    for (c = 0; c < 256; c ++)
    {
        printf("\n ");
        printf("builtin_%s_data_%d", prefix, c);
        if ((c + 1) < 256) printf(",");
    }
    printf("};\n");
    printf("int %s_used[256] = {\n", prefix);
    for (c = 0; c < 256; c ++) {
        if (c && ! (c & 3))
            printf(",\n");
        else if (c)
            printf(", ");
        printf("/* 0x%2.2X */ %d", c, used[c]);
    }
    printf("};\n");
    printf("int %s_color[256] = {\n", prefix);
    for (c = 0; c < 256; c ++) {
        if (c && ! (c & 3))
            printf(",\n");
        else if (c)
            printf(", ");
        printf("/* 0x%2.2X */ 0x%X", c, color[c]);
    }
    printf("};\n");
}

int
parse_tile_args(const char *tilefile, const char *tile_args)
{
    const char *argp = tile_args;
    int c;

    while (1)
    {
        const char *endp;

        c = *argp;
        if (! c) break;
        if (isspace(c))
        {
            argp ++;
            continue;
        }
        else if ((endp = strchr(argp, '=')) != 0)
        {
            if (! strncmp(argp, "ABOUT", endp - argp))
            {
                argp = endp + 1;
                tile_ABOUT = strword(argp, &endp, 0);
                if (! tile_ABOUT)
                {
                    perror("ABOUT");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "NOTE", endp - argp))
            {
                argp = endp + 1;
                tile_NOTE = strword(argp, &endp, 0);
                if (! tile_NOTE)
                {
                    perror("NOTE");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "FIXME", endp - argp))
            {
                argp = endp + 1;
                tile_FIXME = strword(argp, &endp, 0);
                if (! tile_FIXME)
                {
                    perror("FIXME");
                    return 1;
                }
                argp = endp;
            }
            else
            {
                fprintf(stderr, "%s: unrecognized tile argument: ",
                        tilefile);
                fflush(stderr);
                fwrite((void *) argp, 1, endp - argp, stderr);
                fflush(stderr);
                fprintf(stderr, "\n");
                fflush(stderr);
                return 1;
            }
        }
        else
        {
            fprintf(stderr, "%s: unrecognized tile arguments: %s\n",
                    tilefile,
                    argp);
            fflush(stderr);
            return 1;
        }
    }
    return 0;
}

int
parse_sprite_args(const char *spritefile, const char *sprite_args)
{
    const char *argp = sprite_args;
    int c;

    while (1)
    {
        const char *endp;

        c = *argp;
        if (! c) break;
        if (isspace(c))
        {
            argp ++;
            continue;
        }
        else if ((endp = strchr(argp, '=')) != 0)
        {
            if (! strncmp(argp, "ABOUT", endp - argp))
            {
                argp = endp + 1;
                sprite_ABOUT = strword(argp, &endp, 0);
                if (! sprite_ABOUT)
                {
                    perror("ABOUT");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "NOTE", endp - argp))
            {
                argp = endp + 1;
                sprite_NOTE = strword(argp, &endp, 0);
                if (! sprite_NOTE)
                {
                    perror("NOTE");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "FIXME", endp - argp))
            {
                argp = endp + 1;
                sprite_FIXME = strword(argp, &endp, 0);
                if (! sprite_FIXME)
                {
                    perror("FIXME");
                    return 1;
                }
                argp = endp;
            }
            else
            {
                fprintf(stderr, "%s: unrecognized sprite argument: ",
                        spritefile);
                fflush(stderr);
                fwrite((void *) argp, 1, endp - argp, stderr);
                fflush(stderr);
                fprintf(stderr, "\n");
                fflush(stderr);
                return 1;
            }
        }
        else
        {
            fprintf(stderr, "%s: unrecognized sprite arguments: %s\n",
                    spritefile,
                    argp);
            fflush(stderr);
            return 1;
        }
    }
    return 0;
}

int
readmaze(const char *mazefile,
         int *levels,
         int *w,
         int *h,
         char **maze,
         int *flags,
         char **color,
         const char **args)
{
    char X;
    int c = EOF, i, j;
    int n;
    FILE *infile = NULL;

    infile = fopen_datafile(mazefile, "rb");
    if (! infile) {
        perror(mazefile);
        return 1;
    }
    ignore_bom_utf8(infile);
    {
        int rn, rw, rh;

        if ((fscanf(infile, "%d%d%c%d", &rn, &rw, &X, &rh) != 4) ||
            (tolower(X) != 'x'))
        {
            fprintf(stderr, "%s: can't find a dimension specification N WxH\n",
                    mazefile);
            return 1;
        }
        if ((rw < 1) || (rh < 1) || (rn < 1)) {
            fprintf(stderr, "%s: dimension specification %d %dx%d is too small\n",
                    mazefile, rn, rw, rh);
            return 1;
        }
        *levels = rn;
        *h = rh;
        *w = rw;
    }
    *flags = 0;
    *args = NULL;
    if (! feof(infile))
    {
        c = fgetc_cp437_utf8(infile);
        if (c == '~')
        {
            if (fscanf(infile, "%d", flags) != 1)
            {
                fprintf(stderr, "%s: can't find flags ~F after dimension specification %d %dx%d\n",
                        mazefile,
                        *levels, *w, *h);
                return 1;
            }
        }
        else
        {
            ungetc_cp437_utf8(c, infile);
        }
        c = fgetc_cp437_utf8(infile);
        if ((c == ' ') || (c == '\t'))
        {
            char *args_tmp = NULL;
            size_t args_tmp_len = 0;
            int escaped = 0;

            args_tmp = strdup("");
            if (! args_tmp)
            {
                perror("strdup");
                return 1;
            }
            while (((c = fgetc_cp437_utf8(infile)) != EOF)
                   &&
                   (c != '\v') && (c != '\f') && (c != '\n') && (c != '\r'))
            {
                if (c == '\\')
                {
                    int c2;

                    escaped = ! escaped;
                    if (escaped)
                    {
                        c2 = fgetc_cp437_utf8(infile);
                        if (c2 == '\r')
                        {
                            int c3;

                            c3 = fgetc_cp437_utf8(infile);
                            if (c3 == '\n')
                            {
                                c2 = c3;
                            }
                            else
                            {
                                ungetc_cp437_utf8(c3, infile);
                            }
                        }
                        if ((c2 == '\v') || (c2 == '\f') || (c2 == '\n') || (c2 == '\r'))
                        {
                            escaped = 0;
                            continue;
                        }
                        ungetc_cp437_utf8(c2, infile);
                    }
                }
                args_tmp = (char *) realloc((void *) args_tmp, args_tmp_len + 1 + ((c == 0) ? 4 : 1));
                if (! args_tmp)
                {
                    perror("realloc");
                    return 1;
                }
                if (c == 0)
                {
                    if (! escaped)
                    {
                        args_tmp[args_tmp_len] = '\\';
                        args_tmp_len ++;
                    }
                    args_tmp[args_tmp_len] = 'x';
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '0';
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '0';
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '\0';
                }
                else
                {
                    args_tmp[args_tmp_len] = c;
                    args_tmp_len ++;
                    args_tmp[args_tmp_len] = '\0';
                }
                if (escaped && (c != '\\'))
                {
                    escaped = 0;
                }
            }
            if (args_tmp_len)
            {
                *args = args_tmp;
            }
            else
            {
                free((void *) args_tmp);
                args_tmp = NULL;
            }
        }
        if ((c == '\n') || (c == '\r') || (c == '\v') || (c == '\f'))
        {
            ungetc(c, infile);
        }
        else
        {
            ungetc_cp437_utf8(c, infile);
        }
    }
    if ((! feof(infile))
        &&
        (((X = fgetc_cp437_utf8(infile)) != '\n') &&
         (X != '\r') && (X != '\v') && (X != '\f')))
    {
        fprintf(stderr, "%s: garbage after dimension specification %d %dx%d~%d%s%s %X\n",
                mazefile,
                *levels, *w, *h, *flags, *args ? " " : "", *args ? *args : "", X);
        return 1;
    }
    *maze = (char *) malloc(*levels * *h * (*w + 1) * sizeof(**maze));
    if (! *maze)
    {
        perror("malloc");
        return 1;
    }
    memset((void *) *maze, 0, *levels * *h * (*w + 1) * sizeof(**maze));
    *color = (char *) malloc(*levels * *h * (*w + 1) * sizeof(**color));
    if (! *color)
    {
        perror("malloc");
        return 1;
    }
    memset((void *) *color, 0, *levels * *h * (*w + 1) * sizeof(**color));
    for (n = 0; n < *levels; n ++)
    {
        for (i = 0; i < *h; i ++)
        {
            for (j = 0; j < *w; j ++)
            {
                if ((c = fgetc_cp437_utf8(infile)) == EOF)
                {
                    if (feof(infile))
                        fprintf(stderr, "%s: premature EOF\n", mazefile);
                    else
                        perror(mazefile);
                    return 1;
                } else if ((c == '\r') || (c == '\n') || (c == '\f') || (c == '\v'))
                    j --;
                else
                    (*maze)[(n * *h+i) * (*w + 1)+j] = (char) (unsigned char) c;
            }
            if (ISPELLET(c) || ISDOT(c))
            {
                c = ' ';
            }
            (*maze)[(n * *h+i) * (*w + 1)+*w] = (char) (unsigned char) c;
        }
    }
    fclose(infile);
    return 0;
}

void
writemaze(const char *mazefile)
{
    int n;
    int i;

    printf("int maze_n = %d;\n", maze_n);
    printf("int maze_w = %d;\n", maze_w);
    printf("int maze_h = %d;\n", maze_h);
    printf("int maze_flags = %d;\n", maze_flags);
    printf("const char *maze_args = ");
    if (maze_args)
    {
        printf("\"");
        mymanescape(maze_args, strlen(maze_args));
        printf("\"");
    }
    else
    {
        printf("0");
    }
    printf(";\n");
    printf("static const char builtin_mazefile_str[] = \"");
    mymanescape(mazefile,
                strlen(mazefile));
    printf("\";\n");
    printf("const char *builtin_mazefile = builtin_mazefile_str;\n");
    printf("const char *maze_data = \n");
    for (n = 0; n < maze_n; n ++)
    {
        printf("/* 0x%X */\n", n);
        for (i = 0; i < maze_h; i ++) {
            printf("  \"");
            mymanescape(maze + ((n * maze_h) + i) * (maze_w + 1), maze_w + 1);
            printf("\"\n");
        }
    }
    printf(";\n");
    printf("const char *maze_color_data = \n");
    for (n = 0; n < maze_n; n ++)
    {
        printf("/* 0x%X */\n", n);
        for (i = 0; i < maze_h; i ++) {
            printf("  \"");
            mymanescape(maze_color + ((n * maze_h) + i) * (maze_w + 1), maze_w + 1);
            printf("\"\n");
        }
    }
    printf(";\n");
}

int
parse_maze_args(const char *mazefile, const char *maze_args)
{
    const char *argp = maze_args;
    int c;

    while (1)
    {
        const char *endp;

        c = *argp;
        if (! c) break;
        if (isspace(c))
        {
            argp ++;
            continue;
        }
        else if ((endp = strchr(argp, '=')) != 0)
        {
            if (! strncmp(argp, "FLIP_TO", endp - argp))
            {
                argp = endp + 1;
                flip_to = strtol(argp, (char **) &endp, 0);
                if (endp == argp)
                {
                    perror("strtol: FLIP_TO");
                    return 1;
                }
                if ((*endp) && ! isspace(*endp))
                {
                    fprintf(stderr, "%s: FLIP_TO: garbage after argument: %s\n",
                            mazefile,
                            endp);
                    fflush(stderr);
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "GHOSTS", endp - argp))
            {
                long * tmp_ghosts = NULL;
                size_t tmp_ghosts_len = 0;

                argp = endp + 1;
                tmp_ghosts = strtollist_word(argp, &endp, &tmp_ghosts_len);
                if (! tmp_ghosts)
                {
                    perror("GHOSTS");
                    return 1;
                }
                argp = endp;
                if (! ghosts_p)
                {
                    maze_GHOSTS = tmp_ghosts;
                    maze_GHOSTS_len = tmp_ghosts_len;
                }
                else
                {
                    free((void *) tmp_ghosts);
                    tmp_ghosts = NULL;
                    tmp_ghosts_len = 0;
                }
            }
            else if (! strncmp(argp, "RGHOST", endp - argp))
            {
                argp = endp + 1;
                maze_RGHOST = strtodlist_word(argp, &endp, &maze_RGHOST_len);
                if (! maze_RGHOST)
                {
                    perror("RGHOST");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "CGHOST", endp - argp))
            {
                argp = endp + 1;
                maze_CGHOST = strtodlist_word(argp, &endp, &maze_CGHOST_len);
                if (! maze_CGHOST)
                {
                    perror("CGHOST");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "ROGHOST", endp - argp))
            {
                argp = endp + 1;
                maze_ROGHOST = strtodlist_word(argp, &endp, &maze_ROGHOST_len);
                if (! maze_ROGHOST)
                {
                    perror("ROGHOST");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "COGHOST", endp - argp))
            {
                argp = endp + 1;
                maze_COGHOST = strtodlist_word(argp, &endp, &maze_COGHOST_len);
                if (! maze_COGHOST)
                {
                    perror("COGHOST");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "RFRUIT", endp - argp))
            {
                argp = endp + 1;
                maze_RFRUIT = strtodlist_word(argp, &endp, &maze_RFRUIT_len);
                if (! maze_RFRUIT)
                {
                    perror("RFRUIT");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "CFRUIT", endp - argp))
            {
                argp = endp + 1;
                maze_CFRUIT = strtodlist_word(argp, &endp, &maze_CFRUIT_len);
                if (! maze_CFRUIT)
                {
                    perror("CFRUIT");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "RTOP", endp - argp))
            {
                argp = endp + 1;
                maze_RTOP = strtodlist_word(argp, &endp, &maze_RTOP_len);
                if (! maze_RTOP)
                {
                    perror("RTOP");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "RHERO", endp - argp))
            {
                argp = endp + 1;
                maze_RHERO = strtodlist_word(argp, &endp, &maze_RHERO_len);
                if (! maze_RHERO)
                {
                    perror("RHERO");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "CHERO", endp - argp))
            {
                argp = endp + 1;
                maze_CHERO = strtodlist_word(argp, &endp, &maze_CHERO_len);
                if (! maze_CHERO)
                {
                    perror("CHERO");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "RMSG", endp - argp))
            {
                argp = endp + 1;
                maze_RMSG = strtollist_word(argp, &endp, &maze_RMSG_len);
                if (! maze_RMSG)
                {
                    perror("RMSG");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "CMSG", endp - argp))
            {
                argp = endp + 1;
                maze_CMSG = strtollist_word(argp, &endp, &maze_CMSG_len);
                if (! maze_CMSG)
                {
                    perror("CMSG");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "RMSG2", endp - argp))
            {
                argp = endp + 1;
                maze_RMSG2 = strtollist_word(argp, &endp, &maze_RMSG2_len);
                if (! maze_RMSG2)
                {
                    perror("RMSG2");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "CMSG2", endp - argp))
            {
                argp = endp + 1;
                maze_CMSG2 = strtollist_word(argp, &endp, &maze_CMSG2_len);
                if (! maze_CMSG2)
                {
                    perror("CMSG2");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "ABOUT", endp - argp))
            {
                argp = endp + 1;
                maze_ABOUT = strword(argp, &endp, 0);
                if (! maze_ABOUT)
                {
                    perror("ABOUT");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "NOTE", endp - argp))
            {
                argp = endp + 1;
                maze_NOTE = strword(argp, &endp, 0);
                if (! maze_NOTE)
                {
                    perror("NOTE");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "FIXME", endp - argp))
            {
                argp = endp + 1;
                maze_FIXME = strword(argp, &endp, 0);
                if (! maze_FIXME)
                {
                    perror("FIXME");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "DIRHERO", endp - argp))
            {
                char *dirhero_tmp = NULL;

                argp = endp + 1;
                dirhero_tmp = strword(argp, &endp, 0);
                if (! dirhero_tmp)
                {
                    perror("DIRHERO");
                    return 1;
                }
                argp = endp;
                if (! strcmp(dirhero_tmp, "UP"))
                {
                    dirhero = MYMAN_UP;
                }
                else if (! strcmp(dirhero_tmp, "DOWN"))
                {
                    dirhero = MYMAN_DOWN;
                }
                else if (! strcmp(dirhero_tmp, "LEFT"))
                {
                    dirhero = MYMAN_LEFT;
                }
                else if (! strcmp(dirhero_tmp, "RIGHT"))
                {
                    dirhero = MYMAN_RIGHT;
                }
                else
                {
                    fprintf(stderr, "%s: DIRHERO: must be one of UP, DOWN, LEFT or RIGHT; got \"%s\" instead\n",
                            mazefile,
                            dirhero_tmp);
                    fflush(stderr);
                    return 1;
                }
                free((void *) dirhero_tmp);
                dirhero_tmp = NULL;
            }
            else if (! strncmp(argp, "GAMEOVER", endp - argp))
            {
                argp = endp + 1;
                msg_GAMEOVER = strword(argp, &endp, 0);
                if (! msg_GAMEOVER)
                {
                    perror("GAMEOVER");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "PLAYER1", endp - argp))
            {
                argp = endp + 1;
                msg_PLAYER1 = strword(argp, &endp, 0);
                if (! msg_PLAYER1)
                {
                    perror("PLAYER1");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "PLAYER2", endp - argp))
            {
                argp = endp + 1;
                msg_PLAYER2 = strword(argp, &endp, 0);
                if (! msg_PLAYER2)
                {
                    perror("PLAYER2");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "READY", endp - argp))
            {
                argp = endp + 1;
                msg_READY = strword(argp, &endp, 0);
                if (! msg_READY)
                {
                    perror("READY");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "WALL_COLORS", endp - argp))
            {
                argp = endp + 1;
                maze_WALL_COLORS = strword(argp, &endp, &maze_WALL_COLORS_len);
                if (! maze_WALL_COLORS)
                {
                    perror("WALL_COLORS");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "DOT_COLORS", endp - argp))
            {
                argp = endp + 1;
                maze_DOT_COLORS = strword(argp, &endp, &maze_DOT_COLORS_len);
                if (! maze_MORTAR_COLORS)
                {
                    perror("DOT_COLORS");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "PELLET_COLORS", endp - argp))
            {
                argp = endp + 1;
                maze_PELLET_COLORS = strword(argp, &endp, &maze_PELLET_COLORS_len);
                if (! maze_MORTAR_COLORS)
                {
                    perror("PELLET_COLORS");
                    return 1;
                }
                argp = endp;
            }
            else if (! strncmp(argp, "MORTAR_COLORS", endp - argp))
            {
                argp = endp + 1;
                maze_MORTAR_COLORS = strword(argp, &endp, &maze_MORTAR_COLORS_len);
                if (! maze_MORTAR_COLORS)
                {
                    perror("MORTAR_COLORS");
                    return 1;
                }
                argp = endp;
            }
            else
            {
                fprintf(stderr, "%s: unrecognized maze argument: ",
                        mazefile);
                fflush(stderr);
                fwrite((void *) argp, 1, endp - argp, stderr);
                fflush(stderr);
                fprintf(stderr, "\n");
                fflush(stderr);
                return 1;
            }
        }
        else
        {
            fprintf(stderr, "%s: unrecognized maze arguments: %s\n",
                    mazefile,
                    argp);
            fflush(stderr);
            return 1;
        }
    }
    return 0;
}

char *mystrdup(const char *s)
{
    size_t len = 0;
    char *ret = 0;

    len = strlen(s);
    ret = (char *) malloc(len + 1);
    if (ret)
    {
        memcpy((void *) ret, (const void *) s, len + 1);
    }
    return ret;
}

char *maze = NULL;
char *maze_color = NULL;
char *blank_maze = NULL;
char *blank_maze_color = NULL;
unsigned char *dirty_cell = NULL;
int all_dirty;

int nogame = 0;

int maze_n;
int maze_w;
int maze_h;
int maze_flags;
int maze_level = 0;
const char *maze_args = NULL;

int tile_w;
int tile_h;
int tile_flags;
const char *tile_args = NULL;
const char *tile[256] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};
int tile_used[256];
int tile_color[256];

int sprite_w;
int sprite_h;
int sprite_flags;
const char *sprite_args = NULL;
const char *sprite[256] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};
int sprite_used[256];
int sprite_color[256];

void
maze_erase(void)
{
    memset((void *) (maze + maze_level * maze_h * (maze_w + 1)), 0, (maze_w + 1) * maze_h);
    memset((void *) (maze_color + maze_level * maze_h * (maze_w + 1)), 0, (maze_w + 1) * maze_h);
    DIRTY_ALL();
}

void
mark_cell(int x, int y)
{
    if ((! all_dirty) && ((((long) (x)) >= 0) && (((long) (y)) >= 0) && ((x) <= maze_w) && ((y) < maze_h)))
    {
        int idx;

        idx = ((y)) * ((maze_w+1+7) >> 3) + ((x)>>3);
        dirty_cell[idx] =
            (1U << ((x)&7))
            |
            (unsigned) (unsigned char) dirty_cell[idx];
    }
}

void
maze_puts(int y, int x, int color, const char *s)
{
    int i;
    unsigned char c;

    if (y < 0) return;
    if (y >= maze_h) return;
    for (i = 0; 0 != (int) (unsigned char) (c = (unsigned) (unsigned char) (s)[i]); i ++)
    {
        if (((x + i) >= 0) && ((x + i) < maze_w))
        {
            maze[(maze_level*maze_h+YWRAP(y)) * (maze_w + 1)+XWRAP(x + i)] = c;
            maze_color[(maze_level*maze_h+YWRAP(y)) * (maze_w + 1)+XWRAP(x + i)] = (char) (unsigned char) color;
            mark_cell(XWRAP(x + i), YWRAP(y));
        }
    }
}

void
maze_putsn_nonblank(int y, int x, int color, const char *s, int n)
{
    int i;
    unsigned char c;

    if (y < 0) return;
    if (y >= maze_h) return;
    for (i = 0; (i < (n)) && (0 != (int) (unsigned char) (c = (unsigned) (unsigned char) (s)[i])); i ++)
    {
        int cc;
                
        cc = color;
        if (((x + i) >= 0) && ((x + i) < maze_w))
        {
            if (' ' == (char) c)
            {
                c = blank_maze[(maze_level*maze_h+YWRAP(y)) * (maze_w + 1)+XWRAP(x + i)];
                cc = blank_maze_color[(maze_level*maze_h+YWRAP(y)) * (maze_w + 1)+XWRAP(x + i)];
            }
            maze[(maze_level*maze_h+YWRAP(y)) * (maze_w + 1)+XWRAP(x + i)] = c;
            maze_color[(maze_level*maze_h+YWRAP(y)) * (maze_w + 1)+XWRAP(x + i)] = (char) (unsigned char) cc;
            mark_cell(XWRAP(x + i), YWRAP(y));
        }
    }
}

unsigned char sprite_register[SPRITE_REGISTERS];
int sprite_register_frame[SPRITE_REGISTERS];
int sprite_register_x[SPRITE_REGISTERS], sprite_register_y[SPRITE_REGISTERS];
int sprite_register_used[SPRITE_REGISTERS];
int sprite_register_timer[SPRITE_REGISTERS];
int sprite_register_color[SPRITE_REGISTERS];

void
mark_sprite_register(int s)
{
    int _dirty_sprite_register_x, _dirty_sprite_register_y;

    for (_dirty_sprite_register_y = 0; _dirty_sprite_register_y < ((gfx_h > sgfx_h) ? gfx_h : sgfx_h); _dirty_sprite_register_y ++)
        for (_dirty_sprite_register_x = 0; _dirty_sprite_register_x < ((gfx_w > sgfx_w) ? gfx_w : sgfx_w); _dirty_sprite_register_x ++)
        {
            mark_cell(XTILE(sprite_register_x[s] + _dirty_sprite_register_x - ((gfx_w > sgfx_w) ? gfx_w : sgfx_w) / 2),
                       YTILE(sprite_register_y[s] + _dirty_sprite_register_y - ((gfx_h > sgfx_h) ? gfx_h : sgfx_h) / 2));
        }
}

int ghost_dir[MAXGHOSTS], ghost_mem[MAXGHOSTS], ghost_man[MAXGHOSTS], ghost_timer[MAXGHOSTS];
unsigned char *home_dir = NULL;

unsigned char
gfx2(unsigned char c)
{
    return (((reflect ^ gfx_reflect) && ! REFLECT_LARGE) ? ((unsigned) reflect_cp437[(unsigned long) (unsigned char) (c)]) : (unsigned) (c));
}

size_t
gfx1(const char **font, unsigned char c, int y, int x, int w)
{
    return (reflect ^ ((reflect ^ gfx_reflect) && ! REFLECT_LARGE)) ?
        font[(unsigned) (unsigned char) c][x * w + y] :
        font[(unsigned) (unsigned char) c][y * w + x];
}

unsigned char
gfx0(unsigned char c, unsigned char *m)
{
    return (REFLECT_LARGE | gfx_reflect) ? ((unsigned) (unsigned char) ((m)[(unsigned long) (unsigned char) (c)])) : ((unsigned) (unsigned char) (c));
}

int reflect = 0;
int gfx_reflect = 0;
long myman_intro = 1;
unsigned long myman_start = 0;
unsigned long myman_demo = 0;
int munched = HERO;
int old_lines, old_cols, old_score, old_showlives, old_level;
int ignore_delay = 0;
long frameskip = 0, frameskip0 = 0, frameskip1 = 0;
long scrolling = 0;
long frames = 0;
long winning = 1;
unsigned long mymandelay = MYMANDELAY;
unsigned long mindelay = MYMANDELAY / 2;
int ghost_eaten_timer = 0;
int paused = 0;
long intermission_running = 0;
unsigned long myman_demo_setup = 0;
int need_reset = 0;
int level = 0,
    intermission = 0,
    intermission_shown = 0,
    cycles = 0,
    score = 0,
    dots = 0,
    points = 0,
    lives = LIVES,
    lives_used = 0,
    earned = 0,
    dying = 0,
    dead = 0,
    deadpan = 0,
    myman_lines = 0,
    myman_columns = 0,
    oldplayer = 0,
    player = 1;
long pellet_timer = 0,
    pellet_time = 0;

void
gameinfo(void)
{
    if (maze_ABOUT || maze_FIXME || maze_NOTE
        || tile_ABOUT || tile_FIXME || tile_NOTE
        || sprite_ABOUT || sprite_FIXME || sprite_NOTE)
    {
        if (tmp_notice)
        {
            free((void *) tmp_notice);
            tmp_notice = 0;
        }
        tmp_notice = (char *) malloc(
            ((maze_ABOUT ? ((maze_ABOUT_prefix ? strlen(maze_ABOUT_prefix) : 0) + strlen(maze_ABOUT)) : 0) + (maze_FIXME ? ((maze_FIXME_prefix ? strlen(maze_FIXME_prefix) : 0) + strlen(maze_FIXME)) : 0) + (maze_NOTE ? ((maze_NOTE_prefix ? strlen(maze_NOTE_prefix) : 0) + strlen(maze_NOTE)) : 0)
             + (tile_ABOUT ? ((tile_ABOUT_prefix ? strlen(tile_ABOUT_prefix) : 0) + strlen(tile_ABOUT)) : 0) + (tile_FIXME ? ((tile_FIXME_prefix ? strlen(tile_FIXME_prefix) : 0) + strlen(tile_FIXME)) : 0) + (tile_NOTE ? ((tile_NOTE_prefix ? strlen(tile_NOTE_prefix) : 0) + strlen(tile_NOTE)) : 0)
             + (sprite_ABOUT ? ((sprite_ABOUT_prefix ? strlen(sprite_ABOUT_prefix) : 0) + strlen(sprite_ABOUT)) : 0) + (sprite_FIXME ? ((sprite_FIXME_prefix ? strlen(sprite_FIXME_prefix) : 0) + strlen(sprite_FIXME)) : 0) + (sprite_NOTE ? ((sprite_NOTE_prefix ? strlen(sprite_NOTE_prefix) : 0) + strlen(sprite_NOTE)) : 0))
            + 1
            + 1
            );
        if (tmp_notice)
        {
            *tmp_notice = '\0';
            if (maze_ABOUT) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (maze_ABOUT_prefix ? maze_ABOUT_prefix : ""), maze_ABOUT);
            if (maze_FIXME) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (maze_FIXME_prefix ? maze_FIXME_prefix : ""), maze_FIXME);
            if (maze_NOTE) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (maze_NOTE_prefix ? maze_NOTE_prefix : ""), maze_NOTE);
            if (tile_ABOUT) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (tile_ABOUT_prefix ? tile_ABOUT_prefix : ""), tile_ABOUT);
            if (tile_FIXME) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (tile_FIXME_prefix ? tile_FIXME_prefix : ""), tile_FIXME);
            if (tile_NOTE) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (tile_NOTE_prefix ? tile_NOTE_prefix : ""), tile_NOTE);
            if (sprite_ABOUT) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (sprite_ABOUT_prefix ? sprite_ABOUT_prefix : ""), sprite_ABOUT);
            if (sprite_FIXME) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (sprite_FIXME_prefix ? sprite_FIXME_prefix : ""), sprite_FIXME);
            if (sprite_NOTE) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (sprite_NOTE_prefix ? sprite_NOTE_prefix : ""), sprite_NOTE);
            sprintf(tmp_notice + strlen(tmp_notice), "\n");
            pager_notice = tmp_notice;
            reinit_requested = 1;
        }
    }
}

void
gamehelp(void)
{
    if (MYMANKEYS
        || maze_ABOUT || maze_FIXME || maze_NOTE
        || tile_ABOUT || tile_FIXME || tile_NOTE
        || sprite_ABOUT || sprite_FIXME || sprite_NOTE)
    {
        if (tmp_notice)
        {
            free((void *) tmp_notice);
            tmp_notice = 0;
        }
        tmp_notice = (char *) malloc(
            ((MYMANKEYS ? ((MYMANKEYS_prefix ? strlen(MYMANKEYS_prefix) : 0) + strlen(MYMANKEYS)) : 0)
             + (maze_ABOUT ? ((maze_ABOUT_prefix ? strlen(maze_ABOUT_prefix) : 0) + strlen(maze_ABOUT)) : 0) + (maze_FIXME ? ((maze_FIXME_prefix ? strlen(maze_FIXME_prefix) : 0) + strlen(maze_FIXME)) : 0) + (maze_NOTE ? ((maze_NOTE_prefix ? strlen(maze_NOTE_prefix) : 0) + strlen(maze_NOTE)) : 0)
             + (tile_ABOUT ? ((tile_ABOUT_prefix ? strlen(tile_ABOUT_prefix) : 0) + strlen(tile_ABOUT)) : 0) + (tile_FIXME ? ((tile_FIXME_prefix ? strlen(tile_FIXME_prefix) : 0) + strlen(tile_FIXME)) : 0) + (tile_NOTE ? ((tile_NOTE_prefix ? strlen(tile_NOTE_prefix) : 0) + strlen(tile_NOTE)) : 0)
             + (sprite_ABOUT ? ((sprite_ABOUT_prefix ? strlen(sprite_ABOUT_prefix) : 0) + strlen(sprite_ABOUT)) : 0) + (sprite_FIXME ? ((sprite_FIXME_prefix ? strlen(sprite_FIXME_prefix) : 0) + strlen(sprite_FIXME)) : 0) + (sprite_NOTE ? ((sprite_NOTE_prefix ? strlen(sprite_NOTE_prefix) : 0) + strlen(sprite_NOTE)) : 0))
            + 1
            + 1
            );
        if (tmp_notice)
        {
            *tmp_notice = '\0';
            if (MYMANKEYS) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (MYMANKEYS_prefix ? MYMANKEYS_prefix : ""), MYMANKEYS);
            if (maze_ABOUT) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (maze_ABOUT_prefix ? maze_ABOUT_prefix : ""), maze_ABOUT);
            if (maze_FIXME) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (maze_FIXME_prefix ? maze_FIXME_prefix : ""), maze_FIXME);
            if (maze_NOTE) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (maze_NOTE_prefix ? maze_NOTE_prefix : ""), maze_NOTE);
            if (tile_ABOUT) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (tile_ABOUT_prefix ? tile_ABOUT_prefix : ""), tile_ABOUT);
            if (tile_FIXME) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (tile_FIXME_prefix ? tile_FIXME_prefix : ""), tile_FIXME);
            if (tile_NOTE) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (tile_NOTE_prefix ? tile_NOTE_prefix : ""), tile_NOTE);
            if (sprite_ABOUT) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (sprite_ABOUT_prefix ? sprite_ABOUT_prefix : ""), sprite_ABOUT);
            if (sprite_FIXME) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (sprite_FIXME_prefix ? sprite_FIXME_prefix : ""), sprite_FIXME);
            if (sprite_NOTE) sprintf(tmp_notice + strlen(tmp_notice), "%s%s", (sprite_NOTE_prefix ? sprite_NOTE_prefix : ""), sprite_NOTE);
            sprintf(tmp_notice + strlen(tmp_notice), "\n");
            pager_notice = tmp_notice;
            reinit_requested = 1;
        }
    }
}

void
gameintro(void)
{
    int s;

    if (myman_intro == 1)
    {
        cycles = -1;
        for (s = 0; s < SPRITE_REGISTERS; s ++)
        {
            sprite_register_used[s] = 0;
            sprite_register_timer[s] = 0;
        }
        maze_erase();
        ghost_eaten_timer = 0;
    }
    for (s = 0; s < 4; s ++)
    {
        int eyes;
        int mean;
        int blue;
        int ghost;
        int nick_col, nick_row;
        int name_col, name_row;
        const char *ghost_name;
        const char *ghost_nick;

        ghost = (s == 0) ? GHOST1 : (s == 1) ? GHOST2 : (s == 2) ? GHOST0 : (s == 3) ? GHOST3 : s;
        ghost_nick = (ghost == GHOST0) ? GHOST0_NICK : (ghost == GHOST2) ? GHOST2_NICK : (ghost == GHOST1) ? GHOST1_NICK : (ghost == GHOST3) ? GHOST3_NICK : "";
        ghost_name = (ghost == GHOST0) ? GHOST0_NAME : (ghost == GHOST2) ? GHOST2_NAME : (ghost == GHOST1) ? GHOST1_NAME : (ghost == GHOST3) ? GHOST3_NAME : "-?????";
        name_row = 3 * (s + 1) + 1;
        if (name_row < 2) name_row = 2;
        name_col = 7 + (maze_w - 28) / 2;
        if (name_col < (sgfx_w + gfx_w - 1) / gfx_w) name_col = (sgfx_w + gfx_w - 1) / gfx_w;
        nick_row = name_row;
        nick_col = name_col + (int) strlen(NAME_HEADER);
        if (nick_col + (int) strlen(NICK_HEADER) > maze_w)
        {
            nick_col = name_col + 1;
            nick_row = name_row + 1;
        }
        if ((! s) && (myman_intro == 1))
        {
            maze_puts(name_row - 2, name_col, 0xF, NAME_HEADER);
            maze_puts(nick_row - 2, nick_col, 0xF, NICK_HEADER);
        }
        if (ghost >= ghosts) continue;
        eyes = GHOSTEYES(ghost);
        mean = MEANGHOST(ghost);
        blue = BLUEGHOST(ghost);
        if (myman_intro == (1 + s * 3 * TWOSECS / 4))
        {
            int hero_x;

            hero_x = (9 + maze_w - 28) * gfx_w / 2;
            if (hero_x < sgfx_w / 2)
            {
                hero_x = XPIXWRAP(sgfx_w / 2);
            }
            sprite_register_used[eyes] = VISIBLE_EYES;
            sprite_register_used[mean] = 1;
            sprite_register_used[blue] = 0;
            sprite_register_frame[mean] = 0;
            sprite_register_x[HERO] =
                sprite_register_x[eyes] =
                sprite_register_x[mean] = hero_x;
            sprite_register_y[HERO] =
                sprite_register_y[eyes] =
                sprite_register_y[mean] = (6 * s + 9) * gfx_h / 2;
            if (sprite_register_y[HERO] < sgfx_h / 2)
            {
                sprite_register_y[HERO] =
                    sprite_register_y[eyes] =
                    sprite_register_y[mean] = YPIXWRAP(sgfx_h / 2);
            }
            deadpan = 0;
            sprite_register_frame[eyes] = MYMAN_RIGHT - 1;
        }
        else if ((myman_intro > (1 + s * 3 * TWOSECS / 4))
                 &&
                 (myman_intro < (1 + (s * 3 + 1) * TWOSECS / 4)))
        {
            sprite_register_x[HERO] -= ((myman_intro - 1) - (1 + s * 3 * TWOSECS / 4)) * (name_col * gfx_w - sprite_register_x[mean]) / (1 + TWOSECS / 4);
            sprite_register_x[HERO] += (myman_intro - (1 + s * 3 * TWOSECS / 4)) * (name_col * gfx_w - sprite_register_x[mean]) / (1 + TWOSECS / 4);
        }
        if (myman_intro == (1 + (s * 3 + 1) * TWOSECS / 4))
        {
#define MEANCOLOR (use_color ? sprite_register_color[mean] : 0xF)
            sprite_register_x[HERO] = name_col * gfx_w + gfx_w / 2;
            maze_puts(name_row, name_col,
                      sprite_color[((unsigned) sprite_register[mean]) + sprite_register_frame[mean]]
                      ?
                      sprite_color[((unsigned) sprite_register[mean]) + sprite_register_frame[mean]]
                      :
                      MEANCOLOR
                      ,
                      ghost_name);
        }
        else if ((myman_intro > (1 + (s * 3 + 1) * TWOSECS / 4))
                 &&
                 (myman_intro < (1 + (s * 3 + 2) * TWOSECS / 4)))
        {
            sprite_register_x[HERO] -= ((myman_intro - 1) - (1 + (s * 3 + 1) * TWOSECS / 4)) * ((int) strlen(NAME_HEADER) * gfx_w) / (1 + TWOSECS / 4);
            sprite_register_x[HERO] += (myman_intro - (1 + (s * 3 + 1) * TWOSECS / 4)) * ((int) strlen(NAME_HEADER) * gfx_w) / (1 + TWOSECS / 4);
        }
        if (myman_intro == (1 + (s * 3 + 2) * TWOSECS / 4))
        {
            sprite_register_x[HERO] = nick_col * gfx_w + gfx_w / 2;
            maze_puts(nick_row, nick_col,
                      sprite_color[((unsigned) sprite_register[mean]) + sprite_register_frame[mean]]
                      ?
                      sprite_color[((unsigned) sprite_register[mean]) + sprite_register_frame[mean]]
                      :
                      MEANCOLOR
                      ,
                      ghost_nick);
        }
        else if ((myman_intro > (1 + (s * 3 + 2) * TWOSECS / 4))
                 &&
                 (myman_intro < (1 + ((s + 1) * 3) * TWOSECS / 4)))
        {
            sprite_register_x[HERO] -= ((myman_intro - 1) - (1 + (s * 3 + 2) * TWOSECS / 4)) * ((int) strlen(NICK_HEADER) * gfx_w) / (1 + TWOSECS / 4);
            sprite_register_x[HERO] += (myman_intro - (1 + (s * 3 + 2) * TWOSECS / 4)) * ((int) strlen(NICK_HEADER) * gfx_w) / (1 + TWOSECS / 4);
        }
        if (myman_intro > (4 * 3 * TWOSECS / 4))
        {
            sprite_register_frame[mean] = ((myman_intro / MYMANFIFTH) & 1) ? 1 : 0;
        }
    }
    if (myman_intro == (1 + 4 * 3 * TWOSECS / 4))
    {
        sprite_register_x[HERO] = gfx_w * (maze_w + 1) - sgfx_w;
        sprite_register_y[HERO] = ((17 * 2 + 1) * gfx_h) / 2;
        if ((YTILE(sprite_register_y[HERO]) + 6) >= maze_h)
        {
            maze_erase();
            for (s = 0; s < SPRITE_REGISTERS; s ++)
            {
                sprite_register_used[s] = 0;
                sprite_register_timer[s] = 0;
                sprite_register_frame[s] = 0;
            }
            sprite_register_y[HERO] = 3 * gfx_h / 2;
        }
        sprite_register_frame[HERO] = 0;
        sprite_register[HERO] = SPRITE_HERO + 4;
        sprite_register_used[HERO] = 1;
        if ((YTILE(sprite_register_y[HERO]) + 11) < maze_h)
        {
            maze_puts(YWRAP(YTILE(sprite_register_y[HERO]) + 11), ((int) maze_w - (int) strlen(MYMANNOTICE)) / 2, 0xD, MYMANNOTICE);
        }
        else
        {
            maze_puts(YWRAP(maze_h - 1), ((int) maze_w - (int) strlen(MYMANNOTICE)) / 2, 0xD, MYMANNOTICE);
        }
        maze_puts(YTILE(sprite_register_y[HERO]) + 6, 12 + (maze_w - 28) / 2, 0xF, "50 \x9es");
        maze_puts(YTILE(sprite_register_y[HERO]) + 4, 12 + (maze_w - 28) / 2, 0xF, "10 \x9es");
        maze_puts(YTILE(sprite_register_y[HERO]) + 6, 10 + (maze_w - 28) / 2, 0x7, "\xfe");
        maze_puts(YTILE(sprite_register_y[HERO]) + 4, 10 + (maze_w - 28) / 2, 0x7, "\xf9");
        maze_puts(YTILE(sprite_register_y[HERO]), 4 + (maze_w - 28) / 2, 0x7, "\xfe");
    }
    else if (sprite_register_used[HERO] || ghost_eaten_timer)
    {
        if (ghost_eaten_timer)
        {
            ghost_eaten_timer --;
            if (! ghost_eaten_timer)
            {
                sprite_register_used[HERO] = 1;
                sprite_register_used[GHOST_SCORE] = 0;
            }
        }
        else
        {
            sprite_register_frame[HERO] = (myman_intro / (1 + (MYMANFIFTH / 2))) % 4;
            if ((((unsigned) sprite_register[HERO]) == (SPRITE_HERO + 4))
                &&
                (sprite_register_x[HERO] == gfx_w * (4 + (maze_w - 28) / 2)))
            {
                maze_puts(YTILE(sprite_register_y[HERO]), 4 + (maze_w - 28) / 2, 0x7, " ");
                sprite_register[HERO] = SPRITE_HERO + 12;
                sprite_register_frame[HERO] = 0;
            }
            else if (((unsigned) sprite_register[HERO]) == (SPRITE_HERO + 4))
            {
                sprite_register_x[HERO] --;
            }
            else
            {
                sprite_register_x[HERO] ++;
            }
        }
        for (s = 0; s < 4; s ++)
        {
            int eyes;
            int mean;
            int blue;
            int ghost;
            const char *extra_ghost_colors = EXTRA_GHOST_COLORS;
                            
            ghost = MAXGHOSTS - 4 + ((s == 0) ? GHOST1 : (s == 1) ? GHOST2 : (s == 2) ? GHOST0 : (s == 3) ? GHOST3 : s);
            if ((ghost < ghosts) || (ghost >= MAXGHOSTS)) continue;
            eyes = GHOSTEYES(ghost);
            mean = MEANGHOST(ghost);
            blue = BLUEGHOST(ghost);
            sprite_register[eyes] = SPRITE_EYES;
            sprite_register[mean] = SPRITE_MEAN;
            sprite_register[blue] = SPRITE_BLUE;
            sprite_register_y[eyes] =
                sprite_register_y[mean] =
                sprite_register_y[blue] = sprite_register_y[HERO];
            sprite_register_frame[eyes] = MYMAN_RIGHT - 1;
            sprite_register_frame[blue] =
                (sprite_register_frame[mean] = ((myman_intro / MYMANFIFTH) & 1) ? 1 : 0);
            if ((((unsigned) sprite_register[HERO]) == (SPRITE_HERO + 4)) && ! ghost_eaten_timer)
            {
                sprite_register_frame[eyes] = MYMAN_LEFT - 1;
                sprite_register_used[eyes] = VISIBLE_EYES;
                sprite_register_used[mean] = 1;
                sprite_register_used[blue] = 0;
                sprite_register_x[eyes] =
                    sprite_register_x[mean] =
                    sprite_register_x[blue] = 3 * (sprite_register_x[HERO] - gfx_w * (3 + (maze_w - 28) / 2)) / 2 + sgfx_w * s + gfx_w * (4 + (maze_w - 28) / 2);
            }
            else if (sprite_register_used[mean] && ! ghost_eaten_timer)
            {
                sprite_register_used[eyes] =
                    sprite_register_used[mean] = 0;
                sprite_register_used[blue] = 1;
            }
            else if (sprite_register_used[eyes])
            {
                sprite_register_x[eyes] =
                    sprite_register_x[mean] =
                    sprite_register_x[blue] += 2;
            }
            else if (sprite_register_used[blue] && collide(blue, HERO) && ! ghost_eaten_timer)
            {
                sprite_register_used[blue] = 0;
                sprite_register_used[eyes] = 1;
                ghost_eaten_timer = ONESEC;
                sprite_register_frame[HERO] = 0;
                sprite_register_used[HERO] = 0;
                sprite_register_used[GHOST_SCORE] = 1;
                sprite_register_x[GHOST_SCORE] = sprite_register_x[blue];
                sprite_register_y[GHOST_SCORE] = sprite_register_y[blue];
                sprite_register_frame[GHOST_SCORE] = s;
            }
            else if (sprite_register_used[blue] && (myman_intro & 1) && ! ghost_eaten_timer)
            {
                sprite_register_x[eyes] =
                    sprite_register_x[mean] =
                    sprite_register_x[blue] ++;
            }
            sprite_register_color[eyes] = 0xF;
            sprite_register_color[blue] = 0x9;
            sprite_register_color[mean] = extra_ghost_colors[(s % strlen(extra_ghost_colors))];
            if (s == 2) sprite_register_color[mean] = 0xB;
            if (s == 0) sprite_register_color[mean] = 0xC;
            if (s == 1) sprite_register_color[mean] = 0xD;
            if (s == 3) sprite_register_color[mean] = 0x6;
        }
    }
    if (! ghost_eaten_timer)
        for (s = 0; s < SPRITE_REGISTERS; s ++) {
            if (sprite_register_used[s] && sprite_register_timer[s])
                if (! -- sprite_register_timer[s])
                    sprite_register_used[s] = 0;
        }
    myman_intro ++;
    cycles ++;
}

void
creditscreen(void)
{
    int s;
    int r_off;

    r_off = maze_h - 16;
    if (r_off < 0)
    {
        r_off = 0;
    }
    maze_puts(r_off + 12, ((int) maze_w - (int) strlen(MYMANNOTICE)) / 2, 0xD, MYMANNOTICE);
    maze_puts(r_off + 8, ((int) maze_w - (int) strlen(BONUSHEROTEXT)) / 2, 0x7, BONUSHEROTEXT);
    maze_puts(r_off + 4, ((int) maze_w - (int) strlen(CREDIT(1))) / 2, MSG2_COLOR, CREDIT(1));
    maze_puts(r_off, ((int) maze_w - (int) strlen(START)) / 2, 0x6, START);
    if (debug)
    {
        for (s = 0; s < 256; s ++)
        {
            if ((s / 16) < maze_h)
                if ((s % 16) < maze_w)
                    maze_puts(maze_h - 1 - (s / 16), s % 16, s, "*");
        }
    }
    sprite_register_x[HERO] = maze_w * gfx_w / 2;
    sprite_register_y[HERO] = r_off * gfx_h;
}

void
gamedemo(void)
{
    int s;
    int xtile, ytile;
    int x_off, y_off;

    xtile = XTILE(sprite_register_x[HERO]);
    ytile = YTILE(sprite_register_y[HERO]);
    x_off = sprite_register_x[HERO] % gfx_w;
    y_off = sprite_register_y[HERO] % gfx_h;
    if ((myman_demo == 1) && (! myman_demo_setup))
    {
        level = 0;
        maze_level = 0;
        intermission = 0;
        intermission_shown = 0;
        for (s = 0; s < SPRITE_REGISTERS; s ++)
        {
            sprite_register_used[s] = 0;
            sprite_register_timer[s] = 0;
            sprite_register_frame[s] = 0;
        }
        maze_erase();
        ghost_eaten_timer = 0;
        winning = 1;
        oldplayer = 0;
        player = 1;
        pellet_timer = 0;
        pellet_time = PELLET_ADJUST(7 * ONESEC);
        for (s = 0; s < frames % 8; s ++)
        {
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
            ++ level;
            sprite_register_frame[FRUIT] =
                sprite_register_frame[FRUIT_SCORE] = BONUS(level);
            pellet_time += PELLET_ADJUST(7 * ONESEC);
            if (pellet_time > PELLET_ADJUST(ONESEC))
                pellet_time -= PELLET_ADJUST(ONESEC);
            else
                pellet_time = 0;
        }
        cycles = 0;
        dots = 0;
        dead = 0;
        deadpan = 0;
        dying = 0;
        myman_demo_setup = 1 + (15UL * (maze_h * maze_w) * TWOSECS / (28 * 31)) / 2;
    }
    if (! (winning || dying || (dead && ! ghost_eaten_timer)))
    {
        if (! (frames % ((TWOSECS / 20) + 1)))
        {
            unsigned char mleft, mdown, mright, mup;
            mleft = (unsigned) (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile - NOTRIGHT(x_off))];
            mdown = (unsigned) (unsigned char) maze[(maze_level*maze_h+YWRAP(ytile + NOTTOP(y_off))) * (maze_w + 1)+xtile];
            mright = (unsigned) (unsigned char) maze[(maze_level*maze_h+ytile) * (maze_w + 1)+XWRAP(xtile + NOTLEFT(x_off))];
            mup = (unsigned) (unsigned char) maze[(maze_level*maze_h+YWRAP(ytile - NOTBOTTOM(y_off))) * (maze_w + 1)+xtile];
            if (ISOPEN((unsigned) mleft)
                &&
                ISPELLET((unsigned) mleft))
            {
                hero_dir = MYMAN_LEFT;
                sprite_register[HERO] = SPRITE_HERO + 4;
            }
            else if (ISOPEN((unsigned) mdown)
                     &&
                     ISPELLET((unsigned) mdown))
            {
                hero_dir = MYMAN_DOWN;
                sprite_register[HERO] = SPRITE_HERO + 16;
            }
            else if (ISOPEN((unsigned) mright)
                     &&
                     ISPELLET((unsigned) mright))
            {
                hero_dir = MYMAN_RIGHT;
                sprite_register[HERO] = SPRITE_HERO + 12;
            }
            else if (ISOPEN((unsigned) mup)
                     &&
                     ISPELLET((unsigned) mup))
            {
                hero_dir = MYMAN_UP;
                sprite_register[HERO] = SPRITE_HERO;
            }
            else if (ISOPEN((unsigned) mup)
                     &&
                     ISDOT((unsigned) mup))
            {
                hero_dir = MYMAN_UP;
                sprite_register[HERO] = SPRITE_HERO;
            }
            else if (ISOPEN((unsigned) mleft)
                     &&
                     ISDOT((unsigned) mleft))
            {
                hero_dir = MYMAN_LEFT;
                sprite_register[HERO] = SPRITE_HERO + 4;
            }
            else if (ISOPEN((unsigned) mdown)
                     &&
                     ISDOT((unsigned) mdown))
            {
                hero_dir = MYMAN_DOWN;
                sprite_register[HERO] = SPRITE_HERO + 16;
            }
            else if (ISOPEN((unsigned) mright)
                     &&
                     ISDOT((unsigned) mright))
            {
                hero_dir = MYMAN_RIGHT;
                sprite_register[HERO] = SPRITE_HERO + 12;
            }
            else if (ISOPEN((unsigned) mleft)
                     &&
                     (hero_dir != MYMAN_RIGHT))
            {
                hero_dir = MYMAN_LEFT;
                sprite_register[HERO] = SPRITE_HERO + 4;
            }
            else if (ISOPEN((unsigned) mup)
                     &&
                     (hero_dir != MYMAN_DOWN))
            {
                hero_dir = MYMAN_UP;
                sprite_register[HERO] = SPRITE_HERO;
            }
            else if (ISOPEN((unsigned) mright)
                     &&
                     (hero_dir != MYMAN_LEFT))
            {
                hero_dir = MYMAN_RIGHT;
                sprite_register[HERO] = SPRITE_HERO + 12;
            }
            else if (ISOPEN((unsigned) mdown)
                     &&
                     (hero_dir != MYMAN_UP))
            {
                hero_dir = MYMAN_DOWN;
                sprite_register[HERO] = SPRITE_HERO + 16;
            }
            else if (! (ISOPEN((unsigned) mleft)
                        ||
                        ISOPEN((unsigned) mright)
                        ||
                        ISOPEN((unsigned) mdown)))
            {
                hero_dir = MYMAN_UP;
                sprite_register[HERO] = SPRITE_HERO;
            }
            else if (! (ISOPEN((unsigned) mleft)
                        ||
                        ISOPEN((unsigned) mright)
                        ||
                        ISOPEN((unsigned) mup)))
            {
                hero_dir = MYMAN_DOWN;
                sprite_register[HERO] = SPRITE_HERO + 16;
            }
            else if (! (ISOPEN((unsigned) mright)
                        ||
                        ISOPEN((unsigned) mdown)
                        ||
                        ISOPEN((unsigned) mup)))
            {
                hero_dir = MYMAN_LEFT;
                sprite_register[HERO] = SPRITE_HERO + 4;
            }
            else if (! (ISOPEN((unsigned) mleft)
                        ||
                        ISOPEN((unsigned) mdown)
                        ||
                        ISOPEN((unsigned) mup)))
            {
                hero_dir = MYMAN_RIGHT;
                sprite_register[HERO] = SPRITE_HERO + 12;
            }
        }
    }
    if (myman_demo_setup)
    {
        myman_demo_setup --;
    }
    if (! myman_demo_setup)
    {
        myman_demo ++;
    }
    maze_puts(rmsg, cmsg, MSG_COLOR, msg_GAMEOVER);
}

void
gamestart(void)
{
    level = 0;
    maze_level = 0;
    intermission = 0;
    intermission_shown = 0;
    maze_erase();
    ghost_eaten_timer = 0;
    winning = 1;
    oldplayer = 0;
    player = 1;
    pellet_timer = 0;
    pellet_time = PELLET_ADJUST(7 * ONESEC);
    cycles = 0;
    score = 0;
    dots = 0;
    dead = 0;
    deadpan = 0;
    dying = 0;
    myman_start = 0;
}

void
gameintermission(void)
{
    int s;

    if (intermission_running == (1 + INTERMISSION_TIME))
    {
        for (s = 0; s < SPRITE_REGISTERS; s ++)
        {
            sprite_register_used[s] = 0;
            sprite_register_timer[s] = 0;
            sprite_register_frame[s] = 0;
        }
        maze_erase();
        myman_sfx |= myman_sfx_intermission;
    }
    intermission_running --;
    if (intermission == 0)
    {
        if ((INTERMISSION_TIME - intermission_running) <= PIX_W)
        {
            sprite_register_used[HERO] = 1;
            sprite_register_y[HERO] = (maze_h / 2 - 2) * gfx_h;
            sprite_register_x[HERO] = PIX_W - gfx_w / 2 - (INTERMISSION_TIME - intermission_running);
            sprite_register[HERO] = SPRITE_HERO + 4;
            sprite_register_frame[HERO] = ((INTERMISSION_TIME - intermission_running) / (1 + (MYMANFIFTH / 2))) % 4;
            if (GHOST1 < ghosts)
            {
                int mean;
                int eyes;
                int blue;

                mean = MEANGHOST(GHOST1);
                eyes = GHOSTEYES(GHOST1);
                blue = BLUEGHOST(GHOST1);
                sprite_register_x[mean] =
                    sprite_register_x[eyes] =
                    sprite_register_x[blue] = 2 * sprite_register_x[HERO] + sgfx_w;
                sprite_register_y[mean] =
                    sprite_register_y[eyes] =
                    sprite_register_y[blue] = sprite_register_y[HERO];
                sprite_register[mean] = SPRITE_MEAN;
                sprite_register[eyes] = SPRITE_EYES;
                sprite_register[blue] = SPRITE_BLUE;
                sprite_register_frame[eyes] = MYMAN_LEFT - 1;
                sprite_register_frame[blue] =
                    (sprite_register_frame[mean] = (((INTERMISSION_TIME - intermission_running) / MYMANFIFTH) & 1) ? 1 : 0);
                sprite_register_used[mean] = 1;
                sprite_register_used[eyes] = VISIBLE_EYES;
                sprite_register_used[blue] = 0;
            }
        }
        else if ((intermission_running <= (2 * PIX_W)) && intermission_running)
        {
            /* TODO: need a giant hero here */
            sprite_register_used[HERO] = 1;
            sprite_register_used[BIGHERO_UL] = 0;
            sprite_register_used[BIGHERO_UR] = 0;
            sprite_register_used[BIGHERO_LL] = 0;
            sprite_register_used[BIGHERO_LR] = 0;
            if (sprite_used[SPRITE_BIGHERO_UL]
                &&
                sprite_used[SPRITE_BIGHERO_UR]
                &&
                sprite_used[SPRITE_BIGHERO_LL]
                &&
                sprite_used[SPRITE_BIGHERO_LR])
            {
                sprite_register_used[HERO] = 0;
                sprite_register_used[BIGHERO_UL] = 1;
                sprite_register_used[BIGHERO_UR] = 1;
                sprite_register_used[BIGHERO_LL] = 1;
                sprite_register_used[BIGHERO_LR] = 1;
            }
            sprite_register_y[HERO] = (maze_h / 2 + 2) * gfx_h;
            sprite_register_x[HERO] = PIX_W - gfx_w / 2 - intermission_running;
            sprite_register_y[BIGHERO_UL] = sprite_register_y[HERO] - sgfx_h;
            sprite_register_y[BIGHERO_UR] = sprite_register_y[HERO] - sgfx_h;
            sprite_register_y[BIGHERO_LL] = sprite_register_y[HERO];
            sprite_register_y[BIGHERO_LR] = sprite_register_y[HERO];
            sprite_register_x[BIGHERO_UL] = sprite_register_x[HERO] - sgfx_w;
            sprite_register_x[BIGHERO_UR] = sprite_register_x[HERO];
            sprite_register_x[BIGHERO_LL] = sprite_register_x[HERO] - sgfx_w;
            sprite_register_x[BIGHERO_LR] = sprite_register_x[HERO];
            sprite_register[HERO] = SPRITE_HERO + 12;
            sprite_register[BIGHERO_UL] = SPRITE_BIGHERO_UL;
            sprite_register[BIGHERO_UR] = SPRITE_BIGHERO_UR;
            sprite_register[BIGHERO_LL] = SPRITE_BIGHERO_LL;
            sprite_register[BIGHERO_LR] = SPRITE_BIGHERO_LR;
            sprite_register_frame[HERO] = ((INTERMISSION_TIME - intermission_running) / (1 + (MYMANFIFTH / 2))) % 4;
            sprite_register_frame[BIGHERO_UL] = sprite_register_frame[HERO];
            sprite_register_frame[BIGHERO_UR] = sprite_register_frame[HERO];
            sprite_register_frame[BIGHERO_LL] = sprite_register_frame[HERO];
            sprite_register_frame[BIGHERO_LR] = sprite_register_frame[HERO];
            if (GHOST1 < ghosts)
            {
                int mean;
                int eyes;
                int blue;

                mean = MEANGHOST(GHOST1);
                eyes = GHOSTEYES(GHOST1);
                blue = BLUEGHOST(GHOST1);
                sprite_register_x[mean] =
                    sprite_register_x[eyes] =
                    sprite_register_x[blue] = PIX_W - gfx_w / 2 - (PIX_W - gfx_w / 2 - sprite_register_x[HERO]) / 2 + sgfx_w;
                sprite_register_y[mean] =
                    sprite_register_y[eyes] =
                    sprite_register_y[blue] = sprite_register_y[HERO];
                sprite_register[mean] = SPRITE_MEAN;
                sprite_register[eyes] = SPRITE_EYES;
                sprite_register[blue] = SPRITE_BLUE;
                sprite_register_frame[eyes] = MYMAN_LEFT - 1;
                sprite_register_frame[blue] =
                    (sprite_register_frame[mean] = (((INTERMISSION_TIME - intermission_running) / MYMANFIFTH) & 1) ? 1 : 0);
                sprite_register_used[mean] = 0;
                sprite_register_used[eyes] = 0;
                sprite_register_used[blue] = 1;
            }
        }
        else
        {
            for (s = 0; s < SPRITE_REGISTERS; s ++)
            {
                sprite_register_used[s] = 0;
                sprite_register_timer[s] = 0;
                sprite_register_frame[s] = 0;
            }
        }
    }
    else
    {
        maze_erase();
        sprite_register_y[HERO] = gfx_h * (maze_h * intermission_running / (1 + INTERMISSION_TIME));
        sprite_register_x[HERO] = gfx_w * maze_w / 2;
        maze_puts(YTILE(sprite_register_y[HERO]), (maze_w - (int) strlen("COFFEE BREAK")) / 2, 1 + 0xE * intermission_running / (1 + INTERMISSION_TIME), "COFFEE BREAK");
    }
}

void
gamereset(void)
{
    int i;

    pellet_time = PELLET_ADJUST(7 * ONESEC);
    sprite_register[FRUIT] = SPRITE_FRUIT;
    sprite_register[FRUIT_SCORE] = SPRITE_FRUIT_SCORE;
    sprite_register_frame[FRUIT] =
        sprite_register_frame[FRUIT_SCORE] = BONUS(level);
    sprite_register_x[FRUIT] =
        sprite_register_x[FRUIT_SCORE] = (int) XFRUIT;
    sprite_register_y[FRUIT] =
        sprite_register_y[FRUIT_SCORE] = (int) YFRUIT;

    sprite_register[GHOST_SCORE] = SPRITE_200;
    sprite_register_frame[GHOST_SCORE] = 0;

    hero_dir = dirhero;
    sprite_register[HERO] = SPRITE_HERO + ((hero_dir == MYMAN_LEFT) ? 4 : (hero_dir == MYMAN_RIGHT) ? 12 : (hero_dir == MYMAN_DOWN) ? 16 : 0);
    sprite_register_frame[HERO] = 0;
    sprite_register_x[HERO] = (int) XHERO;
    sprite_register_y[HERO] = (int) YHERO;
    sprite_register_used[HERO] = 0;

    sprite_register_color[HERO] = 0xE;
    sprite_register_color[BIGHERO_UL] = 0xE;
    sprite_register_color[BIGHERO_UR] = 0xE;
    sprite_register_color[BIGHERO_LL] = 0xE;
    sprite_register_color[BIGHERO_LR] = 0xE;

    for (i = 0; i < MAXGHOSTS; i++) {
        int eyes, mean, blue;
        const char *extra_ghost_colors = EXTRA_GHOST_COLORS;

        eyes = GHOSTEYES(i);
        mean = MEANGHOST(i);
        blue = BLUEGHOST(i);
        sprite_register[eyes] = SPRITE_EYES;
        sprite_register[mean] = SPRITE_MEAN;
        sprite_register[blue] = SPRITE_BLUE;
        sprite_register_used[eyes] =
            sprite_register_used[mean] =
            sprite_register_used[blue] = 0;
        sprite_register_frame[eyes] =
            sprite_register_frame[mean] =
            sprite_register_frame[blue] = 0;
        ghost_mem[i] = 0;
        ghost_timer[i] = TWOSECS;
        ghost_man[i] = 0;
        sprite_register_color[eyes] = 0xF;
        sprite_register_color[blue] = 0x9;
        sprite_register_color[mean] = extra_ghost_colors[(i % strlen(extra_ghost_colors))];
    }

    if (GHOST0 < MAXGHOSTS) sprite_register_color[MEANGHOST(GHOST0)] = 0xB;
    if (GHOST1 < MAXGHOSTS) sprite_register_color[MEANGHOST(GHOST1)] = 0xC;
    if (GHOST2 < MAXGHOSTS) sprite_register_color[MEANGHOST(GHOST2)] = 0xD;
    if (GHOST3 < MAXGHOSTS) sprite_register_color[MEANGHOST(GHOST3)] = 0x6;
}

char *tmp_notice = 0;
const char * maze_ABOUT = 0;
const char * maze_FIXME = 0;
const char * maze_NOTE = 0;
const char * tile_ABOUT = 0;
const char * tile_FIXME = 0;
const char * tile_NOTE = 0;
const char * sprite_ABOUT = 0;
const char * sprite_FIXME = 0;
const char * sprite_NOTE = 0;
const char *msg_READY = READY;
const char *msg_GAMEOVER = GAMEOVER;
const char *msg_PLAYER1 = PLAYER1;
const char *msg_PLAYER2 = PLAYER2;
const char *maze_WALL_COLORS = WALL_COLORS;
size_t maze_WALL_COLORS_len = sizeof(WALL_COLORS) - 1;
const char *maze_DOT_COLORS = DOT_COLORS;
size_t maze_DOT_COLORS_len = sizeof(DOT_COLORS) - 1;
const char *maze_PELLET_COLORS = PELLET_COLORS;
size_t maze_PELLET_COLORS_len = sizeof(PELLET_COLORS) - 1;
const char *maze_MORTAR_COLORS = MORTAR_COLORS;
size_t maze_MORTAR_COLORS_len = sizeof(MORTAR_COLORS) - 1;
long *maze_GHOSTS = NULL;
size_t maze_GHOSTS_len = 0;
double *maze_RGHOST = NULL;
size_t maze_RGHOST_len = 0;
double *maze_CGHOST = NULL;
size_t maze_CGHOST_len = 0;
double *maze_ROGHOST = NULL;
size_t maze_ROGHOST_len = 0;
double *maze_COGHOST = NULL;
size_t maze_COGHOST_len = 0;
double *maze_RFRUIT = NULL;
size_t maze_RFRUIT_len = 0;
double *maze_CFRUIT = NULL;
size_t maze_CFRUIT_len = 0;
double *maze_RTOP = NULL;
size_t maze_RTOP_len = 0;
double *maze_RHERO = NULL;
size_t maze_RHERO_len = 0;
double *maze_CHERO = NULL;
size_t maze_CHERO_len = 0;
long *maze_RMSG = NULL;
size_t maze_RMSG_len = 0;
long *maze_CMSG = NULL;
size_t maze_CMSG_len = 0;
long *maze_RMSG2 = NULL;
size_t maze_RMSG2_len = 0;
long *maze_CMSG2 = NULL;
size_t maze_CMSG2_len = 0;
int dirhero = DIRHERO;
long scroll_offset_x0 = 0;
long scroll_offset_y0 = 0;
int msglen = 0;
int hero_dir;
int *total_dots = NULL;
int *pellets = NULL;
long flip_to = 0;
int debug = 0;
int ghosts_p = 0;
unsigned long myman_sfx = 0UL;
int showlives;
int visible_frame;

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

int bonus_score[8] = {100, 300, 500, 700, 1000, 2000, 3000, 5000};

#define COLLISION_TYPE_HERO 1
#define COLLISION_TYPE_GHOST 2

int check_collision(int eyes, int mean, int blue)
{
    if (sprite_register_used[mean] && collide(mean, HERO) && ! ghost_eaten_timer) {
        myman_sfx |= myman_sfx_dying;
        dying = DEATHDELAY;
        sprite_register[HERO] = SPRITE_HERO + 8;
        sprite_register_frame[HERO] = 0;
        ghost_eaten_timer = ONESEC;
        munched = HERO;
        return COLLISION_TYPE_HERO;
    } else if (sprite_register_used[blue] && collide(blue, HERO) && ! ghost_eaten_timer) {
        myman_sfx |= myman_sfx_ghost;
        if (! myman_demo) score += points;
        points *= 2;
        if (points > 1600) points = 1600;
        ghost_eaten_timer = ONESEC;
        sprite_register_used[HERO] = 0;
        munched = eyes;
        sprite_register_used[GHOST_SCORE] = 1;
        sprite_register_x[GHOST_SCORE] = sprite_register_x[blue];
        sprite_register_y[GHOST_SCORE] = sprite_register_y[blue];
        sprite_register_x[eyes] = sprite_register_x[blue];
        sprite_register_y[eyes] = sprite_register_y[blue];
        sprite_register_used[blue] = 0;
        return COLLISION_TYPE_GHOST;
    }
    return 0;
}

int
find_home_dir(int s, int r, int c)
{
    return (((unsigned) home_dir[(WHOSE_HOME_DIR((r),(c)) % ghosts*maze_h+(r))*(maze_w+1)+(c)])
            ?
            ((unsigned) home_dir[(WHOSE_HOME_DIR((r),(c)) % ghosts*maze_h+(r))*(maze_w+1)+(c)])
            :
            ((unsigned) home_dir[((s)*maze_h+(r))*(maze_w+1)+(c)]));
}

/* heuristic for rewriting maze tiles */
long
maze_visual(int n, int i, int j)
{
    int c;

    c = (int) (unsigned char) maze[(n*maze_h+i) * (maze_w + 1)+j];
    switch (c)
    {
    case 0xb5:
        if ((! ISWALLUP(blank_maze[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]))
            ||
            (! ISWALLDOWN(blank_maze[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j])))
            c = 0x10;
        break;
    case 0xc6:
        if ((! ISWALLUP(blank_maze[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]))
            ||
            (! ISWALLDOWN(blank_maze[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j])))
            c = 0x11;
        break;
    case 'l':
    case 0xb3:
        if ((! ISWALLUP(blank_maze[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]))
            &&
            (! ISWALLDOWN(blank_maze[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j])))
            c = 0x12;
        else if (! ISWALLUP(blank_maze[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]))
            c = 0x19;
        else if (! ISWALLDOWN(blank_maze[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j]))
            c = 0x18;
        break;
    case 0xd0:
        if ((! ISWALLLEFT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j + 1)]))
            ||
            (! ISWALLRIGHT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j - 1)])))
            c = 0x1f;
        break;
    case 0xd2:
        if ((! ISWALLLEFT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j + 1)]))
            ||
            (! ISWALLRIGHT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j - 1)])))
            c = 0x1e;
        break;
    case '~':
    case 0xc4:
        if ((! ISWALLLEFT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j + 1)]))
            &&
            (! ISWALLRIGHT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j - 1)])))
            c = 0x1d;
        else if (! ISWALLLEFT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j + 1)]))
            c = 0x1b;
        else if (! ISWALLRIGHT(blank_maze[(n*maze_h+i) * (maze_w + 1)+XWRAP(j - 1)]))
            c = 0x1a;
        break;
    }
    return c;
}

int
gamecycle(int lines, int cols)
{
    int s;
    int ret;

    showlives = ((myman_intro || myman_start || myman_demo) ? 0 : NET_LIVES) - 1 + (((munched == HERO) && (! sprite_register_used[HERO])) ? 1 : 0);
    if ((old_lines != lines)
        ||
        (old_cols != cols)
        ||
        (old_score > score)
        ||
        (old_showlives != showlives)
        ||
        (old_level != level))
    {
        DIRTY_ALL();
        ignore_delay = 1;
        frameskip = 0;
        old_lines = lines;
        old_cols = cols;
        /* TODO: make some video memory for the status areas
         * in order to avoid unnecessary full refreshes */
        old_score = score;
        old_showlives = showlives;
        old_level = level;
    }
    gamesfx();
    if (! (winning || NET_LIVES || dead || dying || ghost_eaten_timer || myman_intro || myman_demo || myman_start || intermission_running || need_reset))
    {
        key_buffer = key_buffer_ERR;
        myman_intro = 1;
        myman_start = 0;
        myman_demo = 0;
        myman_demo_setup = 0;
        lives_used = 0;
        earned = 0;
        winning = 1;
        ghost_eaten_timer = 0;
        level = 0;
        maze_level = 0;
        intermission = 0;
        intermission_shown = 0;
        for (s = 0; s < SPRITE_REGISTERS; s ++)
        {
            sprite_register_used[s] = 0;
            sprite_register_timer[s] = 0;
            sprite_register_frame[s] = 0;
        }
        maze_erase();
        oldplayer = 0;
        player = 1;
        pellet_timer = 0;
        pellet_time = PELLET_ADJUST(7 * ONESEC);
        cycles = 0;
        dots = 0;
        dead = 0;
        deadpan = 0;
        dying = 0;
    }
#if MYMANDELAY
    if (mymandelay
        &&
        (! myman_demo_setup)
        &&
        ! ((frames + frameskip) % (frameskip ? frameskip : 1)))
    {
        double td2;
        unsigned long actual_delay;

        do
        {
            actual_delay = (myman_demo ? ((mymandelay + mindelay) / 2) : mymandelay) * (frameskip ? frameskip : 1);
            td2 = doubletime();
            if (td == -1.0L)
            {
                td = 0.0L;
                td2 = 0.0L;
                frameskip = 0;
            }
            if (td == 0.0L)
            {
                td = td2 - (actual_delay * 1e-6L);
            }
            if ((td2 != 0.0L)
                &&
                (td != 0.0L))
            {
                if (td2 > td)
                {
                    unsigned long delta;
                    double nframeskip;
                    size_t kfr;
                    int use_buffer;
                    static double onframeskip[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

                    nframeskip = onframeskip[0];
                    for (kfr = 1; kfr < sizeof(onframeskip) / sizeof(*onframeskip); kfr ++)
                    {
                        nframeskip += onframeskip[kfr];
                        onframeskip[kfr - 1] = onframeskip[kfr];
                    }
                    nframeskip /= kfr;
                    use_buffer = (frameskip == (nframeskip + 0.5));
                    delta = (unsigned long int) (1e6L * (td2 - td));
                    nframeskip = ((frameskip ? frameskip : 1) * delta) / (actual_delay ? actual_delay : 1);
                    if (nframeskip > MAXFRAMESKIP)
                    {
                        nframeskip = MAXFRAMESKIP;
                    }
                    onframeskip[kfr - 1] = nframeskip;
                    if (use_buffer) nframeskip = 0.0;
                    for (kfr = 0; kfr < sizeof(onframeskip) / sizeof(*onframeskip); kfr ++)
                    {
                        if (use_buffer)
                            nframeskip += onframeskip[kfr];
                        else
                            onframeskip[kfr] = nframeskip;
                    }
                    if (use_buffer) nframeskip /= kfr;
                    if (! ignore_delay)
                    {
                        frameskip = (unsigned long) (nframeskip + 0.5);
                    }
                    actual_delay = (myman_demo ? ((mymandelay + mindelay) / 2) : mymandelay) * (frameskip ? frameskip : 1);
                    if (delta <= actual_delay)
                    {
                        actual_delay -= delta;
                    }
                    else
                    {
                        actual_delay = 0;
                    }
                }
            }
            if (actual_delay)
            {
                unsigned long secs;

                secs = actual_delay / 999999L;
                while (secs --)
                {
                    my_usleep(999999UL);
                    actual_delay -= 999999UL;
                }
                if (actual_delay % 999999UL)
                {
                    my_usleep(actual_delay % 999999UL);
                }
                if (actual_delay == ((myman_demo ? ((mymandelay + mindelay) / 2) : mymandelay) * (frameskip ? frameskip : 1)))
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        while (1);
        ignore_delay = 0;
        td = doubletime();
        if (td == -1.0L)
        {
            ignore_delay = 1;
            frameskip = 0;
        }
    }
#endif
    ret = gameinput();
    if (ret >= 0)
    {
        return ret;
    }
    visible_frame = ! ((frames ++) % (frameskip ? frameskip : 1));
    if (myman_intro && ! (paused || snapshot || snapshot_txt))
    {
        gameintro();
        if (((! ghost_eaten_timer)
             &&
             ((sprite_register_frame[GHOST_SCORE] == 3)
              ||
              ((sprite_register_used[HERO])
               &&
               (((unsigned) sprite_register[HERO]) == (SPRITE_HERO + 12))
               &&
               (XTILE(sprite_register_x[HERO]) >= maze_w))))
            ||
            (ret != -1))
        {
            myman_intro = 0;
            myman_demo = 1;
        }
    }
    if (myman_demo && ! (paused || snapshot || snapshot_txt))
    {
        gamedemo();
        if ((myman_demo > (1 + (20UL * (maze_h * maze_w) * TWOSECS / (28 * 31)) / 2))
            ||
            (ret != -1))
        {
            ghost_eaten_timer = 0;
            myman_demo = 0;
            myman_demo_setup = 0;
            myman_intro = (ret == -1);
            myman_start = (ret != -1) ? (30 * ONESEC) : 0;
            if (myman_start)
            {
                myman_sfx |= myman_sfx_credit;
            }
            winning = 1;
            oldplayer = 0;
            player = 1;
            pellet_timer = 0;
            pellet_time = PELLET_ADJUST(7 * ONESEC);
            cycles = 0;
            dots = 0;
            dead = 0;
            deadpan = 0;
            dying = 0;
            level = 0;
            maze_level = 0;
            intermission = 0;
            intermission_shown = 0;
            for (s = 0; s < SPRITE_REGISTERS; s ++)
            {
                sprite_register_used[s] = 0;
                sprite_register_timer[s] = 0;
                sprite_register_frame[s] = 0;
            }
            maze_erase();
            if (myman_start)
            {
                creditscreen();
                ret = -1;
                key_buffer = key_buffer_ERR;
            }
        }
    }
    if (myman_start && ! (paused || snapshot || snapshot_txt))
    {
        myman_start --;
        if ((! myman_start)
            ||
            (ret != -1))
        {
            gamestart();
        }
    }
    if (intermission_running && ! (paused || snapshot || snapshot_txt))
    {
        gameintermission();
        if ((! intermission_running) || myman_demo || (ret != -1))
        {
            if (! myman_demo) ret = -1;
            intermission_running = 0;
            need_reset = 1;
            for (s = 0; s < SPRITE_REGISTERS; s ++)
            {
                sprite_register_used[s] = 0;
            }
        }
    }
    if (! (paused || snapshot || snapshot_txt || myman_intro || myman_start || intermission_running))
    {
        if (gamelogic())
        {
            return 1;
        }
    }
    if (visible_frame && ! (xoff_received || myman_demo_setup))
    {
        gamerender();
    }
    if (! (paused || snapshot || snapshot_txt))
    {
        if (pellet_timer && (! ghost_eaten_timer)) {
            int s, eyes, blue, mean;
                
            if (! -- pellet_timer) {
                for (s = 0; s < ghosts; s ++)
                    if (sprite_register_used[(blue = BLUEGHOST(s))]) {
                        sprite_register_used[(mean = MEANGHOST(s))] = 1;
                        sprite_register_used[blue] = 0;
                        sprite_register_used[(eyes = GHOSTEYES(s))] = VISIBLE_EYES;
                        sprite_register_x[eyes] =
                            (sprite_register_x[mean] = sprite_register_x[blue]);
                        sprite_register_y[eyes] =
                            (sprite_register_y[mean] = sprite_register_y[blue]);
                        sprite_register_frame[eyes] = (ghost_dir[s] = DIRWRAP(s + 1)) - 1;
                    }
            } else if (pellet_timer <= TWOSECS)
                for (s = 0; s < ghosts; s ++)
                    sprite_register[BLUEGHOST(s)] = ((2 * pellet_timer / ONESEC) & 1)
                        ? SPRITE_BLUE
                        : SPRITE_WHITE;
        }
        cycles ++;
    }
    return 1;
}

void
paint_walls(int verbose)
{
    int n;
    double tdt, tdt2 = 0.0L;
    int tdt_used = 0;

    memset((void *) inside_wall, '\0', sizeof(inside_wall));
    tdt = doubletime();
    tdt_used = 0;
    for (n = 0; n < maze_n; n ++)
    {
        int phase;

        total_dots[n] = 0;
        pellets[n] = 0;
        for (phase = 0; phase <= 4; phase ++)
        {
            int phase_done;

            do
            {
                int i;

                phase_done = 1;
                for (i = 0; i < maze_h; i ++)
                {
                    int j;

                    if ((! nogame) && verbose)
                    {
                        tdt2 = doubletime();
                        if ((tdt2 - tdt) >= 1.0)
                        {
                            tdt = tdt2;
                            tdt_used = 1;
                            fprintf(stderr, "%3d%%\r",
                                    (int) (((((float) n) * 5.0 + (float) phase) * maze_h + (float) i) * 100.0
                                           /
                                           (((float) maze_n) * 5.0 * ((float) maze_h))
                                           +
                                           0.5));
                            fflush(stderr);
                        }
                    }
                    for (j = 0; j <= maze_w; j ++)
                    {
                        long c;

                        c = maze_visual(n, i, j);
                        switch (phase)
                        {
                        case 0:
                            if (ISPELLET(c) || ISDOT(c))
                            {
                                total_dots[n] ++;
                                if (ISPELLET(c))
                                {
                                    pellets[n] ++;
                                }
                            }
                            if (ISNONINVERTABLE(c))
                            {
                                inside_wall[(n*maze_h+i) * (maze_w + 1)+j] = ((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) | INSIDE_WALL_NON_INVERTABLE;
                            }
                            break;
                        case 1:
                            if ((! (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) & INSIDE_WALL_NON_INVERTABLE))
                                &&
                                (! ((unsigned) udlr[c]))
                                &&
                                ((((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j]) & INSIDE_WALL_NON_INVERTABLE)
                                 ||
                                 (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]) & INSIDE_WALL_NON_INVERTABLE)
                                 ||
                                 (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                 ||
                                 (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE)))
                            {
                                inside_wall[(n*maze_h+i) * (maze_w + 1)+j] = ((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) | INSIDE_WALL_NON_INVERTABLE;
                                phase_done = 0;
                            }
                            break;
                        case 2:
                        case 3:
                            if ((! (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) & (INSIDE_WALL_NON_INVERTABLE | INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES | INSIDE_WALL_NO | ((phase == 2) ? INSIDE_WALL_PHASE2 : INSIDE_WALL_PHASE3))))
                                &&
                                ((phase == 3)
                                 ||
                                 ((((((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j]) & INSIDE_WALL_NON_INVERTABLE)
                                    ^
                                    (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]) & INSIDE_WALL_NON_INVERTABLE))
                                   &&
                                   ((((unsigned) udlr[c]) & 0x05) == 0x05))
                                  ||
                                  (((((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                    ^
                                    (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE))
                                   &&
                                   ((((unsigned) udlr[c]) & 0x50) == 0x50)))))
                            {
                                int painting_done;

                                inside_wall[(n*maze_h+i) * (maze_w + 1)+j] = ((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) | INSIDE_WALL_PROVISIONAL;
                                if ((((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j]) & INSIDE_WALL_NON_INVERTABLE)
                                    &&
                                    ((((unsigned) udlr[c]) & 0x05) == 0x05))
                                {
                                    inside_wall[(n*maze_h+i) * (maze_w + 1)+j] = ((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) | INSIDE_WALL_NO;
                                    if (! (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]) & (INSIDE_WALL_NON_INVERTABLE | INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES | INSIDE_WALL_NO)))
                                    {
                                        inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j] = ((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]) | (INSIDE_WALL_YES | INSIDE_WALL_PROVISIONAL);
                                    }
                                }
                                else if ((((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                         &&
                                         ((((unsigned) udlr[c]) & 0x50) == 0x50))
                                {
                                    inside_wall[(n*maze_h+i) * (maze_w + 1)+j] = ((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) | INSIDE_WALL_NO;
                                    if (! (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)]) & (INSIDE_WALL_NON_INVERTABLE | INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES | INSIDE_WALL_NO)))
                                    {
                                        inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)] = ((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)]) | (INSIDE_WALL_YES | INSIDE_WALL_PROVISIONAL);
                                    }
                                }
                                else
                                {
                                    inside_wall[(n*maze_h+i) * (maze_w + 1)+j] = ((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) | INSIDE_WALL_YES;
                                }
                                do
                                {
                                    int i2, j2;
                                    int undo = 0;

                                    painting_done = 1;
                                    for (i2 = 0; i2 < maze_h; i2 ++)
                                        for (j2 = 0; j2 <= maze_w; j2 ++)
                                        {
                                            long c2;

                                            c2 = maze_visual(n, i2, j2);
                                            if (! undo)
                                            {
                                                if (((! (((unsigned) udlr[c2]) & 0x04))
                                                     &&
                                                     (((unsigned) inside_wall[(n*maze_h+YWRAP(i2 + 1)) * (maze_w + 1)+j2]) & INSIDE_WALL_YES))
                                                    ||
                                                    ((! (((unsigned) udlr[c2]) & 0x40))
                                                     &&
                                                     (((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+XWRAP2(j2 + 1)]) & INSIDE_WALL_YES)))
                                                {
                                                    if (((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2]) & (INSIDE_WALL_NON_INVERTABLE | INSIDE_WALL_NO))
                                                    {
                                                        undo = 1;
                                                        i2 = 0;
                                                        j2 = 0;
                                                        painting_done = 1;
                                                    }
                                                    else
                                                    {
                                                        if (! (((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2]) & INSIDE_WALL_YES))
                                                        {
                                                            inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] = ((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2]) | INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES;
                                                            painting_done = 0;
                                                        }
                                                    }
                                                }
                                                else if (((((unsigned) udlr[c2]) & 0x04)
                                                          &&
                                                          ((((unsigned) inside_wall[(n*maze_h+YWRAP(i2 + 1)) * (maze_w + 1)+j2]) & (INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES)) == ((INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES) ^ (((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2]) & INSIDE_WALL_PROVISIONAL))))
                                                         ||
                                                         ((((unsigned) udlr[c2]) & 0x40)
                                                          &&
                                                          ((((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+XWRAP2(j2 + 1)]) & (INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES)) == ((INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES) ^ (((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2]) & INSIDE_WALL_PROVISIONAL)))))
                                                {
                                                    if ((((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2]) & INSIDE_WALL_YES) == INSIDE_WALL_YES)
                                                    {
                                                        undo = 1;
                                                        i2 = 0;
                                                        j2 = 0;
                                                        painting_done = 1;
                                                    }
                                                }
                                            }
                                            if (! undo)
                                            {
                                                if ((((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2]) & INSIDE_WALL_YES)
                                                    &&
                                                    (! (((unsigned) udlr[c2]) & 0x04)))
                                                {
                                                    if (((unsigned) inside_wall[(n*maze_h+YWRAP(i2 + 1)) * (maze_w + 1)+j2]) & (INSIDE_WALL_NON_INVERTABLE | INSIDE_WALL_NO))
                                                    {
                                                        undo = 1;
                                                        i2 = 0;
                                                        j2 = 0;
                                                        painting_done = 1;
                                                    }
                                                    else
                                                    {
                                                        if (! (((unsigned) inside_wall[(n*maze_h+YWRAP(i2 + 1)) * (maze_w + 1)+j2]) & INSIDE_WALL_YES))
                                                        {
                                                            inside_wall[(n*maze_h+YWRAP(i2 + 1)) * (maze_w + 1)+j2] = ((unsigned) inside_wall[(n*maze_h+YWRAP(i2 + 1)) * (maze_w + 1)+j2]) | INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES;
                                                            painting_done = 0;
                                                        }
                                                    }
                                                }
                                            }
                                            if (! undo)
                                            {
                                                if ((((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2]) & INSIDE_WALL_YES)
                                                    &&
                                                    (! (((unsigned) udlr[c2]) & 0x40)))
                                                {
                                                    if (((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+XWRAP2(j2 + 1)]) & (INSIDE_WALL_NON_INVERTABLE | INSIDE_WALL_NO))
                                                    {
                                                        undo = 1;
                                                        i2 = 0;
                                                        j2 = 0;
                                                        painting_done = 1;
                                                    }
                                                    else
                                                    {
                                                        if (! (((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+XWRAP2(j2 + 1)]) & INSIDE_WALL_YES))
                                                        {
                                                            inside_wall[(n*maze_h+i2) * (maze_w + 1)+XWRAP2(j2 + 1)] = ((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+XWRAP2(j2 + 1)]) | INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES;
                                                            painting_done = 0;
                                                        }
                                                    }
                                                }
                                            }
                                            if (undo)
                                            {
                                                if (((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2]) & INSIDE_WALL_PROVISIONAL)
                                                {
                                                    inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] = ((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2]) & ~(INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES | INSIDE_WALL_NO);
                                                }
                                            }
                                        }
                                }
                                while (! painting_done);
                                {
                                    int i2, j2;

                                    for (i2 = 0; i2 < maze_h; i2 ++)
                                        for (j2 = 0; j2 <= maze_w; j2 ++)
                                        {
                                            if (((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2]) & INSIDE_WALL_PROVISIONAL)
                                            {
                                                inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2] = ((unsigned) inside_wall[(n*maze_h+i2) * (maze_w + 1)+j2]) & ~INSIDE_WALL_PROVISIONAL;
                                            }
                                        }
                                }
                                phase_done = 0;
                                inside_wall[(n*maze_h+i) * (maze_w + 1)+j] = ((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) | ((phase == 2) ? INSIDE_WALL_PHASE2 : INSIDE_WALL_PHASE3);
                            }
                            break;
                        case 4:
                            if (! (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) & INSIDE_WALL_NON_INVERTABLE))
                            {
                                int ul, ll, ur, lr;

                                ul = !! (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) & INSIDE_WALL_YES);
                                ll = !! (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]) & INSIDE_WALL_YES);
                                ur = !! (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_YES);
                                lr = !! (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_YES);
                                if ((ul + ll + ur + lr) == 0)
                                {
                                    if ((((unsigned) udlr[c]) & 0x05) == 0x05)
                                    {
                                        if (((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ll = 1;
                                            lr = 1;
                                        }
                                        if (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ur = 1;
                                        }
                                    }
                                    if ((((unsigned) udlr[c]) & 0x50) == 0x50)
                                    {
                                        if (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ll = 1;
                                        }
                                        if (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ur = 1;
                                            lr = 1;
                                        }
                                    }
                                    if ((((unsigned) udlr[c]) & 0x55) == 0x44)
                                    {
                                        if (((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ur = 1;
                                            ll = 1;
                                            lr = 1;
                                        }
                                        if (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                        }
                                    }
                                    if ((((unsigned) udlr[c]) & 0x55) == 0x41)
                                    {
                                        if (((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ll = 1;
                                            lr = 1;
                                        }
                                        if (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ur = 1;
                                        }
                                    }
                                    if ((((unsigned) udlr[c]) & 0x55) == 0x14)
                                    {
                                        if (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ur = 1;
                                            lr = 1;
                                        }
                                        if (((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ll = 1;
                                        }
                                    }
                                    if ((((unsigned) udlr[c]) & 0x55) == 0x11)
                                    {
                                        if (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ur = 1;
                                            ll = 1;
                                        }
                                        if (((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            lr = 1;
                                        }
                                    }
                                    if ((((unsigned) udlr[c]) & 0x55) == 0x15)
                                    {
                                        if ((((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                            ||
                                            (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE))
                                        {
                                            ul = 1;
                                            ur = 1;
                                        }
                                        if (((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+j]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ll = 1;
                                            lr = 1;
                                        }
                                    }
                                    if ((((unsigned) udlr[c]) & 0x55) == 0x45)
                                    {
                                        if ((((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                            ||
                                            (((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE))
                                        {
                                            ll = 1;
                                            lr = 1;
                                        }
                                        if (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+j]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ur = 1;
                                        }
                                    }
                                    if ((((unsigned) udlr[c]) & 0x55) == 0x51)
                                    {
                                        if ((((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                            ||
                                            (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE))
                                        {
                                            ul = 1;
                                            ll = 1;
                                        }
                                        if (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ur = 1;
                                            lr = 1;
                                        }
                                    }
                                    if ((((unsigned) udlr[c]) & 0x55) == 0x54)
                                    {
                                        if ((((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                            ||
                                            (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE))
                                        {
                                            ur = 1;
                                            lr = 1;
                                        }
                                        if (((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                        {
                                            ul = 1;
                                            ll = 1;
                                        }
                                    }
                                    if ((((unsigned) udlr[c]) & 0x55) == 0x55)
                                    {
                                        if ((((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                            ||
                                            (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE))
                                        {
                                            ur = 1;
                                            ll = 1;
                                        }
                                        if ((((unsigned) inside_wall[(n*maze_h+YWRAP(i - 1)) * (maze_w + 1)+XWRAP2(j + 1)]) & INSIDE_WALL_NON_INVERTABLE)
                                            ||
                                            (((unsigned) inside_wall[(n*maze_h+YWRAP(i + 1)) * (maze_w + 1)+XWRAP2(j - 1)]) & INSIDE_WALL_NON_INVERTABLE))
                                        {
                                            ul = 1;
                                            lr = 1;
                                        }
                                    }
                                    if ((ul + ur + ll + lr) == 4)
                                    {
                                        ul = 0;
                                        ur = 0;
                                        ll = 0;
                                        lr = 0;
                                        inside_wall[(n*maze_h+i) * (maze_w + 1)+j] = ((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) | INSIDE_WALL_FULLY_NON_INVERTED;
                                    }
                                }
                                if (((ul + ll + ur + lr) > 2)
                                    ||
                                    (((ul + ll + ur + lr) == 2)
                                     &&
                                     ul))
                                {
                                    inside_wall[(n*maze_h+i) * (maze_w + 1)+j] = ((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) | INSIDE_WALL_INVERTED;
                                }
                                if (((ul + ll + ur + lr) == 4)
                                    &&
                                    (((!! (((unsigned) udlr[c]) & 0x40))
                                      +
                                      (!! (((unsigned) udlr[c]) & 0x10))
                                      +
                                      (!! (((unsigned) udlr[c]) & 0x04))
                                      +
                                      (!! (((unsigned) udlr[c]) & 0x01))) > 1))
                                {
                                    inside_wall[(n*maze_h+i) * (maze_w + 1)+j] = ((unsigned) inside_wall[(n*maze_h+i) * (maze_w + 1)+j]) | INSIDE_WALL_FULLY_INVERTED;
                                }
                            }
                            break;
                        }
                    }
                }
            }
            while (! phase_done);
        }
    }
    if (tdt_used)
    {
        fprintf(stderr, "    \r"); fflush(stderr);
        tdt_used = 0;
    }
}

static struct myman_environ_ent {
    const char *name;
    char *value;
    struct myman_environ_ent *next;
} *myman_environ = NULL;

int myman_setenv(const char *name, const char *value)
{
    int ret = 1;

#if HAVE_SETENV
#ifdef macintosh
    ret = setenv(name, value);
#else /* ! defined(macintosh) */
    ret = setenv(name, value, 1);
#endif /* ! defined(macintosh) */
#else /* ! HAVE_SETENV */
#if HAVE_PUTENV || defined(WIN32)
    {
        char *pair;
        size_t name_len, value_len;

        name_len = strlen(name);
        value_len = strlen(value);
        pair = (char *) malloc(name_len + 1 + value_len + 1);
        if (pair)
        {
            memcpy((void *) pair, (void *) name, name_len);
            pair[name_len] = '=';
            memcpy((void *) (pair + name_len + 1), (void *) value, value_len);
            pair[name_len + 1 + value_len] = '\0';
#if HAVE_PUTENV
            ret = putenv(pair);
#else /* ! HAVE_PUTENV */
            ret = _putenv(pair);
#endif /* ! HAVE_PUTENV */
            free((void *) pair);
        }
#ifdef WIN32
        ret = SetEnvironmentVariableA(name, value) ? ret : 1;
#endif /* ! defined(WIN32) */
    }
#endif /* HAVE_PUTENV || defined(WIN32) */
#endif /* ! HAVE_SETENV */
    if (! ret)
    {
        const char *value_check;

        value_check = getenv(name);
        if (value ? ((! value_check) || strcmp(value_check, value)) : (value_check && *value_check))
        {
            ret = 1;
        }
    }
    if (ret)
    {
        char *value_copy;

        value_copy = strdup(value);
        if (value_copy)
        {
            struct myman_environ_ent *ent;

            for (ent = myman_environ; ent; ent = ent->next)
            {
                if (! strcmp(name, ent->name))
                {
                    free((void *) ent->value);
                    ent->value = value_copy;
                    ret = 0;
                    break;
                }
            }
            if (! ent)
            {
                ent = (struct myman_environ_ent *) malloc(sizeof(struct myman_environ_ent));
                if (ent)
                {
                    ent->name = strdup(name);
                    if (ent->name)
                    {
                        ent->value = value_copy;
                        ent->next = myman_environ;
                        myman_environ = ent;
                        ret = 0;
                    }
                    if (ret)
                    {
                        free((void *) ent);
                    }
                }
            }
            if (ret)
            {
                free((void*) value_copy);
            }
        }
    }
    return ret;
}

char *myman_getenv(const char *name)
{
    char *ret;

    ret = getenv(name);
    if (! ret)
    {
        struct myman_environ_ent *ent;

        for (ent = myman_environ; ent; ent = ent->next)
        {
            if (! strcmp(name, ent->name))
            {
                ret = ent->value;
                break;
            }
        }
    }
    return ret;
}
