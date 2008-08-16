/*BINFMTC:-lcurses
 * utils.h - UTF-8 and (modified) CP-437 utilities for the MyMan video game
 * Copyright 1997-2008, Benjamin C. Wiley Sittler <bsittler@gmail.com>
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

#ifndef UTILS_H_INC
#define UTILS_H_INC 1

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

static const unsigned long
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

static const unsigned long
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

static const unsigned char cp437_fullwidth_rhs[256] =
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

static const unsigned long *uni_cp437 = uni_cp437_halfwidth;

/* read a utf-8 sequence from stream, convert it to cp437, and return
 * it. unmappable sequences are silently converted to spaces. this
 * theoretically works with U+0000 .. U+D7FF and U+E000 .. U+10FFFF */

#if 0
static int fputc_utf8(unsigned long u, FILE *stream);
#endif

static int
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
                u = ((c & 0x1f) << 6) | (c1 & 0x3f);
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
                    u = ((c & 0x0f) << 12) | ((c1 & 0x3f) << 6) | (c2 & 0x3f);
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
                        u = ((c & 0x07) << 18) | ((c1 & 0x3f) << 12) | ((c2 & 0x3f) << 6) | (c3 & 0x3f);
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

static int fputc_utf8(unsigned long u, FILE *stream)
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

#ifdef NEED_FPUTC_UTF8_CP437
static int fputc_utf8_cp437(int c, FILE *stream)
{
    unsigned long u;

    u = 0;
    if ((c < 0) || (c > 0x1ff))
    {
        return EOF;
    }
    u = (c & 0x100) ? uni_cp437_halfwidth[((unsigned char) c) & 0xff] : uni_cp437[c];
    if (fputc_utf8(u, stream) == EOF)
    {
        return EOF;
    }
    return c;
}
#endif

/* like ungetc, but converts cp437 to a utf-8 sequence and puts that
 * back one byte at a time. this theoretically works with U+0000
 * .. U+D7FF and U+E000 .. U+10FFFF */

static int ungetc_cp437_utf8(int c, FILE *stream)
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

static char *
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
    while ((c = *from))
    {
        char *newword;

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
                escape = 0;
                c = '\a';
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

static long *strtollist(const char *from, const char **endp, size_t *lenp)
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

        errno_tmp = errno;
        errno = 0;
        tmp = strtol(from, &endp_tmp, 0);
        errno_strtol = errno;
        if (! errno) errno = errno_tmp;
        if (((tmp == LONG_MAX) || (tmp == LONG_MIN)) && (errno_strtol == ERANGE))
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

static double *strtodlist(const char *from, const char **endp, size_t *lenp)
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

        errno_tmp = errno;
        errno = 0;
        tmp = strtod(from, &endp_tmp);
        errno_strtod = errno;
        if (! errno) errno = errno_tmp;
        if (((tmp == HUGE_VAL) || (tmp == -HUGE_VAL)) && (errno_strtod == ERANGE))
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

#endif /* ! defined(UTILS_H_INC) */
