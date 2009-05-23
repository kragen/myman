/* efilibc.c - C library subset for the Extensible Firmware Interface
 * Copyright 2009, Benjamin C. Wiley Sittler <bsittler@gmail.com>
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

/* EFI headers */
#ifdef __EFI__
#include <efi.h>
#include <efilib.h>
#endif /* defined(__EFI__) */

/* GCC headers */
#include <stdarg.h>
#include <limits.h>

/* efilibc headers */
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <wchar.h>

#define UTF8SIZE(wch) (((wch) < 0x80L) \
                       ? 1 \
                       : (((wch) < 0x800L) \
                          ? 2 \
                          : (((wch) > 0xFFFFL) \
                             ? 4 \
                             : 3)))

#define UTF8SEQLEN(leadbyte) ((((leadbyte) & 0xF8) == 0xF0) \
                              ? 4 \
                              : ((((leadbyte) & 0xF0) == 0xE0) \
                                 ? 3 \
                                 : ((((leadbyte) & 0xE0) == 0xC0) \
                                    ? 2 \
                                    : 1)))

#ifdef __EFI__
static FILE efilibc_FILEs[3 + FOPEN_MAX];

FILE *stdin = efilibc_FILEs + 0;
FILE *stdout = efilibc_FILEs + 1;
FILE *stderr = efilibc_FILEs + 2;

int errno = 0;

typedef struct {
    size_t sz;
    void *next;
    void *prev;
} efilibc_heap_entry_t;

static void *efilibc_heap = NULL;

#undef main
extern int main(int argc, char *argv[], char *envp[]);

static EFI_HANDLE efilibc_image_handle;
static EFI_LOADED_IMAGE *efilibc_loaded_image;
static char **efilibc_argv = NULL;
static char *efilibc_argv_contents = NULL;

static CHAR16 *efilibc_utf8to16(const char *utf8)
{
    CHAR16 *ret = NULL;
    int counting;

    for (counting = -1; counting < 1; counting ++)
    {
        size_t i;
        long c, c2, c3, c4;
        long wc;
        size_t sz = 0;

        for (i = 0; (c = (unsigned char) utf8[i]) != '\0'; i ++)
        {
            if (c < 0x80L)
            {
                wc = c;
            }
            else if (((c & 0xE0L) == 0xC0L)
                     &&
                     ((c2 = (unsigned char) utf8[i + 1]) != '\0')
                     &&
                     ((c2 & 0xC0L) == 0x80L)
                     &&
                     ((wc = (((c & 0x1FL) << 6) | (c2 & 0x3FL))) >= 0x80L))
            {
                i ++;
            }
            else if (((c & 0xF0L) == 0xE0L)
                     &&
                     ((c2 = (unsigned char) utf8[i + 1]) != '\0')
                     &&
                     ((c2 & 0xC0L) == 0x80L)
                     &&
                     ((c3 = (unsigned char) utf8[i + 2]) != '\0')
                     &&
                     ((c3 & 0xC0L) == 0x80L)
                     &&
                     ((wc = (((c & 0x0FL) << 12) | ((c2 & 0x3FL) << 6) | (c3 & 0x3FL))) >= 0x800L))
            {
                i += 2;
            }
            else if (((c & 0xF8L) == 0xF0L)
                     &&
                     ((c2 = (unsigned char) utf8[i + 1]) != '\0')
                     &&
                     ((c2 & 0xC0L) == 0x80L)
                     &&
                     ((c3 = (unsigned char) utf8[i + 2]) != '\0')
                     &&
                     ((c3 & 0xC0L) == 0x80L)
                     &&
                     ((c4 = (unsigned char) utf8[i + 3]) != '\0')
                     &&
                     ((c4 & 0xC0L) == 0x80L)
                     &&
                     ((wc = (((c & 0x0FL) << 18) | ((c2 & 0x3FL) << 12) | ((c3 & 0x3FL) << 6) | (c4 & 0x3FL))) > 0xFFFFL))
            {
                i += 3;
            }
            else
            {
                wc = 0xFFFDL;
            }
            if (wc > 0xFFFFL)
            {
                if (ret)
                {
                    ret[sz] = (CHAR16) (0xD800L | (((wc - 0x10000L) >> 10) & 0x3FFL));
                    ret[sz + 1] = (CHAR16) (0xDC00L | ((wc - 0x10000L) & 0x3FFL));
                }
                sz += 2;
            }
            else
            {
                if (ret)
                {
                    ret[sz] = (CHAR16) wc;
                }
                sz ++;
            }
        }
        if (! ret)
        {
            ret = (CHAR16 *) malloc((sz + 1) * sizeof(CHAR16));
        }
        else
        {
            ret[sz] = L'\0';
        }
    }
    return ret;
}

// This owes much to http://osxbook.com/book/bonus/chapter4/efiprogramming/
EFI_STATUS efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab)
{
    int argc;
    char *dummy_argv[2];
    char **argv;
    int i;

    for (i = 0; i < (3 + FOPEN_MAX); i ++)
    {
        size_t j;

        efilibc_FILEs[i].fd = -1;
        for (j = 0; j < sizeof(efilibc_FILEs[i].ungetc_buf) / sizeof(*(efilibc_FILEs[i].ungetc_buf)); j ++)
        {
            efilibc_FILEs[i].ungetc_buf[j] = EOF;
        }
        for (j = 0; j < sizeof(efilibc_FILEs[i].utf8out_buf) / sizeof(*(efilibc_FILEs[i].utf8out_buf)); j ++)
        {
            efilibc_FILEs[i].utf8out_buf[j] = '\0';
        }
        efilibc_FILEs[i].at_eof = 0;
        efilibc_FILEs[i].offset = 0;
        efilibc_FILEs[i].data = NULL;
    }
    efilibc_FILEs[0].fd = 0;
    efilibc_FILEs[1].fd = 1;
    efilibc_FILEs[2].fd = 2;
    efilibc_image_handle = image_handle;
    dummy_argv[0] = (char *) "";
    dummy_argv[1] = NULL;
    argv = dummy_argv;
    argc = 1;
    InitializeLib(image_handle, systab);
    if (! EFI_ERROR(BS->HandleProtocol(image_handle, &LoadedImageProtocol, (VOID *) &efilibc_loaded_image)))
    {
        INTN i;
        int efilibc_argc = 0;
        int efilibc_argv_contents_sz = 0;

        for (i = 0; i < (efilibc_loaded_image->LoadOptionsSize / sizeof(CHAR16)); i ++)
        {
            long wch;

            wch = ((CHAR16 *) efilibc_loaded_image->LoadOptions)[i];
            if ((wch >= 0xD800L) && (wch <= 0xDBFFL)
                &&
                ((i + 1) < (efilibc_loaded_image->LoadOptionsSize / sizeof(CHAR16)))
                &&
                (((CHAR16 *) efilibc_loaded_image->LoadOptions)[i + 1] >= 0xDC00L)
                &&
                (((CHAR16 *) efilibc_loaded_image->LoadOptions)[i + 1] <= 0xDFFFL))
            {
                i ++;
                wch = ((((wch & 0x3FFL) << 10)
                        |
                        (((CHAR16 *) efilibc_loaded_image->LoadOptions)[i] & 0x3FFL))
                       +
                       0x10000L);
            }
            if ((wch < 0) || (wch > 0x10FFFFL))
            {
                wch = 0xFFFD;
            }
            efilibc_argv_contents_sz += UTF8SIZE(wch);
            if ((wch == L'\0') || (wch == L' '))
            {
                efilibc_argc ++;
                while ((wch == L' ')
                       &&
                       ((i + 1) < (efilibc_loaded_image->LoadOptionsSize / sizeof(CHAR16)))
                       &&
                       ((((CHAR16 *) efilibc_loaded_image->LoadOptions)[i + 1] == L' ')
                        ||
                        (((CHAR16 *) efilibc_loaded_image->LoadOptions)[i + 1] == L'\0')))
                {
                    i ++;
                    wch = ((CHAR16 *) efilibc_loaded_image->LoadOptions)[i];
                }
            }
            if (wch == L'\0')
            {
                break;
            }
        }
        efilibc_argv_contents = (char *) malloc(efilibc_argv_contents_sz);
        if (efilibc_argv_contents)
        {
            efilibc_argv = (char **) malloc((efilibc_argc + 1) * sizeof(char *));
            if (efilibc_argv)
            {
                efilibc_argv_contents_sz = 0;
                efilibc_argc = 0;
                efilibc_argv[efilibc_argc] = efilibc_argv_contents;
                for (i = 0; i < (efilibc_loaded_image->LoadOptionsSize / sizeof(CHAR16)); i ++)
                {
                    long wch;

                    /* UTF-16 -> UTF-8 */
                    wch = ((CHAR16 *) efilibc_loaded_image->LoadOptions)[i];
                    if ((wch >= 0xD800L) && (wch <= 0xDBFFL)
                        &&
                        ((i + 1) < (efilibc_loaded_image->LoadOptionsSize / sizeof(CHAR16)))
                        &&
                        (((CHAR16 *) efilibc_loaded_image->LoadOptions)[i + 1] >= 0xDC00L)
                        &&
                        (((CHAR16 *) efilibc_loaded_image->LoadOptions)[i + 1] <= 0xDFFFL))
                    {
                        i ++;
                        wch = ((((wch & 0x3FFL) << 10)
                                |
                                (((CHAR16 *) efilibc_loaded_image->LoadOptions)[i] & 0x3FFL))
                               +
                               0x10000L);
                    }
                    if ((wch < 0) || (wch > 0x10FFFFL))
                    {
                        wch = 0xFFFDL;
                    }
                    if (wch < 0x80L)
                    {
                        efilibc_argv_contents[efilibc_argv_contents_sz] = wch;
                    }
                    else if (wch < 0x800L)
                    {
                        efilibc_argv_contents[efilibc_argv_contents_sz] = 0xC0 | (wch >> 6);
                        efilibc_argv_contents[efilibc_argv_contents_sz + 1] = 0x80 | (wch & 0x3F);
                    }
                    else if (wch > 0xFFFFL)
                    {
                        efilibc_argv_contents[efilibc_argv_contents_sz] = 0xF0 | (wch >> 18);
                        efilibc_argv_contents[efilibc_argv_contents_sz + 1] = 0x80 | ((wch >> 12) & 0x3F);
                        efilibc_argv_contents[efilibc_argv_contents_sz + 2] = 0x80 | ((wch >> 6) & 0x3F);
                        efilibc_argv_contents[efilibc_argv_contents_sz + 3] = 0x80 | (wch & 0x3F);
                    }
                    else
                    {
                        efilibc_argv_contents[efilibc_argv_contents_sz] = 0xE0 | (wch >> 12);
                        efilibc_argv_contents[efilibc_argv_contents_sz + 1] = 0x80 | ((wch >> 6) & 0x3F);
                        efilibc_argv_contents[efilibc_argv_contents_sz + 2] = 0x80 | (wch & 0x3F);
                    }
                    efilibc_argv_contents_sz += UTF8SIZE(wch);
                    if ((wch == L'\0') || (wch == L' '))
                    {
                        efilibc_argv_contents[efilibc_argv_contents_sz - 1] = '\0';
                        efilibc_argc ++;
                        efilibc_argv[efilibc_argc] = &(efilibc_argv_contents[efilibc_argv_contents_sz]);
                        while ((wch == L' ')
                               &&
                               ((i + 1) < (efilibc_loaded_image->LoadOptionsSize / sizeof(CHAR16)))
                               &&
                               ((((CHAR16 *) efilibc_loaded_image->LoadOptions)[i + 1] == L' ')
                                ||
                                (((CHAR16 *) efilibc_loaded_image->LoadOptions)[i + 1] == L'\0')))
                        {
                            i ++;
                            wch = ((CHAR16 *) efilibc_loaded_image->LoadOptions)[i];
                        }
                    }
                    if (wch == L'\0')
                    {
                        break;
                    }
                }
                efilibc_argv[efilibc_argc] = NULL;
                argv = efilibc_argv;
                argc = efilibc_argc;
            }
        }
    }
    else
    {
        efilibc_loaded_image = NULL;
    }
    exit(main(argc, argv, NULL));
    return EFI_SUCCESS;
}

void exit(int status)
{
    size_t i;

    while (efilibc_heap != NULL)
    {
        free(efilibc_heap);
    }
    for (i = 0; i < (3 + FOPEN_MAX); i ++)
    {
        if (efilibc_FILEs[i].fd != -1)
        {
            fflush(efilibc_FILEs + i);
            fclose(efilibc_FILEs + i);
        }
    }
    BS->Exit(efilibc_image_handle, status ? (! EFI_SUCCESS) : EFI_SUCCESS, 0, NULL);
    ST->StdErr->OutputString(ST->StdErr, L"BS->Exit(...) failed, hanging.\r\n");
    while (1);
}

void free(void *ptr)
{
    if (ptr)
    {
        efilibc_heap_entry_t *heap_entry;

        heap_entry = ((efilibc_heap_entry_t *) efilibc_heap) - 1;
        if (heap_entry->prev)
        {
            ((efilibc_heap_entry_t *) heap_entry->prev)[-1].next = heap_entry->next;
        }
        else
        {
            efilibc_heap = heap_entry->next;
        }
        if (heap_entry->next)
        {
            ((efilibc_heap_entry_t *) heap_entry->next)[-1].prev = heap_entry->prev;
        }
        BS->FreePool((VOID *) heap_entry);
    }
}

void *malloc(size_t sz)
{
    void *ret = NULL;

    if (EFI_ERROR(BS->AllocatePool(EfiLoaderData, sizeof(efilibc_heap_entry_t) + sz, (VOID *) &ret)))
    {
        return NULL;
    }
    if (ret)
    {
        efilibc_heap_entry_t *heap_entry;

        heap_entry = (efilibc_heap_entry_t *) ret;
        ret = (void *) (heap_entry + 1);
        heap_entry->sz = sz;
        heap_entry->prev = NULL;
        heap_entry->next = efilibc_heap;
        if (heap_entry->next)
        {
            ((efilibc_heap_entry_t *) heap_entry->next)[-1].prev = ret;
        }
        efilibc_heap = ret;
    }
    return ret;
}

void *realloc(void *ptr, size_t sz)
{
    void *ret;

    if (! ptr) return ptr;
    ret = malloc(sz);
    if (ret)
    {
        if (((efilibc_heap_entry_t *) ptr)[-1].sz < sz)
        {
            sz = ((efilibc_heap_entry_t *) ptr)[-1].sz;
        }
        memcpy(ret, ptr, sz);
        free(ptr);
    }
    return ret;
}
#endif /* defined(__EFI__) */

char *strchr(const char *s, int c)
{
    char *ret = NULL;
    int c2;

    while (s && (c2 = *s) && (! ret))
    {
        if (c2 == c) ret = (char *) s;
        s ++;
    }
    if (! c) ret = (char *) s;
    return ret;
}

char *strrchr(const char *s, int c)
{
    char *ret = NULL;
    int c2;

    while (s && (c2 = *s))
    {
        if (c2 == c) ret = (char *) s;
        s ++;
    }
    if (! c) ret = (char *) s;
    return ret;
}

#undef printf
int printf(const char *fmt, ...)
{
    int ret;
    va_list va;

    va_start(va, fmt);
    ret = vfprintf(stdout, fmt, va);
    va_end(va);
    return ret;
}

int fprintf(FILE *stream, const char *fmt, ...)
{
    int ret;
    va_list va;

    va_start(va, fmt);
    ret = vfprintf(stream, fmt, va);
    va_end(va);
    return ret;
}

static int vfnprintf(FILE *stream,
                     int (*fputc)(int c, FILE *stream),
                     size_t (*fwrite)(const void *ptr, size_t sz, size_t nmem, FILE *stream),
                     int (*fprintf)(FILE *stream, const char *fmt, ...),
                     const char *fmt, va_list va)
{
    size_t i;
    int c;
    int nwritten = 0;

    for (i = 0; fmt && ((c = (unsigned char) fmt[i]) != '\0'); i ++)
    {
        int c2;

        if ((c == '%')
            &&
            ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
        {
            int pound = 0;
            int zero = 0;
            int negfield = 0;
            int space = 0;
            int plus = 0;
            int thousands = 0;
            int fieldwidth = 0;
            int precision = 0;
            int lengthmod = 0;

            i ++;
            c = c2;
          vfprintf_flag:
            if ((c == '#')
                &&
                ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
            {
                i ++;
                pound = 1;
                c = c2;
                goto vfprintf_flag;
            }
            if ((c == '0')
                &&
                ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
            {
                i ++;
                zero = 1;
                c = c2;
                goto vfprintf_flag;
            }
            if ((c == '-')
                &&
                ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
            {
                i ++;
                negfield = 1;
                c = c2;
                goto vfprintf_flag;
            }
            if ((c == ' ')
                &&
                ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
            {
                i ++;
                space = 1;
                c = c2;
                goto vfprintf_flag;
            }
            if ((c == '+')
                &&
                ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
            {
                i ++;
                plus = 1;
                c = c2;
                goto vfprintf_flag;
            }
            if ((c == '\'')
                &&
                ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
            {
                i ++;
                thousands = 1;
                c = c2;
                goto vfprintf_flag;
            }
          vfprintf_fieldwidth:
            if (isdigit(c)
                &&
                ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
            {
                i ++;
                fieldwidth *= 10;
                fieldwidth += (c - '0');
                c = c2;
                goto vfprintf_fieldwidth;
            }
            if ((c == '.')
                &&
                ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
            {
                i ++;
                c = c2;
              vfprintf_precision:
                if (isdigit(c)
                    &&
                    ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
                {
                    i ++;
                    precision *= 10;
                    precision += (c - '0');
                    c = c2;
                    goto vfprintf_precision;
                }
            }
            if ((c == 'h')
                &&
                ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
            {
                i ++;
                lengthmod = -1;
                c = c2;
                if ((c == 'h')
                    &&
                    ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
                {
                    i ++;
                    lengthmod = -2;
                    c = c2;
                }
            }
            else if ((c == 'l')
                &&
                ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
            {
                i ++;
                lengthmod = 1;
                c = c2;
                if ((c == 'l')
                    &&
                    ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
                {
                    i ++;
                    lengthmod = 2;
                    c = c2;
                }
            }
            if (c == '%')
            {
                fputc(c, stream);
                nwritten ++;
            }
            else if (c == 's')
            {
                if (lengthmod == 2)
                {
                    const long *str2ww;
                    size_t sz;
                    long wwc;

                    str2ww = va_arg(va, const long *);
                    for (sz = 0; (wwc = str2ww[sz]) != (long) L'\0'; sz ++)
                    {
                        nwritten += fprintf(stream, "%llc", wwc);
                    }
                }
                else if (lengthmod == 1)
                {
                    const wchar_t *str2w;
                    size_t sz;
                    long wc, wc2;

                    str2w = va_arg(va, const wchar_t *);
                    for (sz = 0; (wc = (long) str2w[sz]) != L'\0'; sz ++)
                    {
                        if ((wc >= 0xD800L)
                            &&
                            (wc <= 0xDBFFL)
                            &&
                            ((wc2 = str2w[sz + 1]) != L'\0')
                            &&
                            (wc2 >= 0xDC00L)
                            &&
                            (wc2 <= 0xDFFFL))
                        {
                            sz ++;
                            wc = (((wc & 0x3FFL) << 10) | (wc2 & 0x3FFL)) + 0x10000L;
                        }
                        nwritten += fprintf(stream, "%llc", wc);
                    }
                }
                else
                {
                    const char *str2;
                    size_t sz;

                    str2 = va_arg(va, const char *);
                    sz = strlen(str2);
                    fwrite(str2, 1, sz, stream);
                    nwritten += sz;
                }
            }
            else if (c == 'c')
            {
                if (lengthmod > 0)
                {
                    long wch;

                    if (lengthmod == 2)
                    {
                        wch = va_arg(va, long);
                    }
                    else
                    {
                        wch = va_arg(va, int);
                    }
                    if ((wch < 0) || (wch > 0x10FFFFL))
                    {
                        wch = 0xFFFDL;
                    }
                    if (wch < 0x80L)
                    {
                        fputc(wch, stream);
                    }
                    else if (wch < 0x800L)
                    {
                        fputc(0xC0 | (wch >> 6), stream);
                        fputc(0x80 | (wch & 0x3F), stream);
                    }
                    else if (wch > 0xFFFFL)
                    {
                        fputc(0xF0 | (wch >> 18), stream);
                        fputc(0x80 | ((wch >> 12) & 0x3F), stream);
                        fputc(0x80 | ((wch >> 6) & 0x3F), stream);
                        fputc(0x80 | (wch & 0x3F), stream);
                    }
                    else
                    {
                        fputc(0xF0 | (wch >> 12), stream);
                        fputc(0x80 | ((wch >> 6) & 0x3F), stream);
                        fputc(0x80 | (wch & 0x3F), stream);
                    }
                    nwritten += UTF8SIZE(wch);
                }
                else
                {
                    int ch;

                    ch = va_arg(va, int);
                    fputc(ch, stream);
                    nwritten ++;
                }
            }
            else if ((c == 'd') || (c == 'i') || (c == 'u') || (c == 'x') || (c == 'X') || (c == 'o') || (c == 'p'))
            {
                unsigned long long num;
                unsigned long long num2;
                int place;
                int base;
                int neg = 0;
                static const char *digits = "0123456789ABCDEF";

                if ((c == 'd') || (c == 'i'))
                {
                    long long snum = 0;

                    if (lengthmod == 2)
                    {
                        snum = va_arg(va, long long);
                    }
                    else if (lengthmod == 1)
                    {
                        snum = va_arg(va, long);
                    }
                    else
                    {
                        snum = va_arg(va, int);
                    }
                    if (snum < 0)
                    {
                        num = (unsigned long long) -snum;
                        neg = 1;
                    }
                    else
                    {
                        num = (unsigned long long) snum;
                    }
                }
                else if (c == 'p')
                {
                    num = (unsigned long) va_arg(va, void *);
                }
                else
                {
                    if (lengthmod == 2)
                    {
                        num = va_arg(va, unsigned long long);
                    }
                    else if (lengthmod == 1)
                    {
                        num = va_arg(va, unsigned long);
                    }
                    else
                    {
                        num = va_arg(va, unsigned);
                    }
                }
                if ((c == 'x') || (c == 'X') || (c == 'p'))
                {
                    if (pound || (c == 'p'))
                    {
                        fputc('0', stream);
                        nwritten ++;
                        fputc(c, stream);
                        nwritten ++;
                    }
                    base = 16;
                }
                else if (c == 'o')
                {
                    if (pound && num)
                    {
                        fputc('0', stream);
                        nwritten ++;
                    }
                    base = 8;
                }
                else
                {
                    base = 10;
                }
                if (neg)
                {
                    fputc('-', stream);
                    nwritten ++;
                }
                place = 1;
                for (num2 = num; num2 >= base; num2 /= base)
                {
                    place *= base;
                }
                while (place)
                {
                    int digit;

                    digit = digits[(num / place) % base];
                    if ((c == 'x') || (c == 'p')) digit = tolower(digit);
                    fputc(digit, stream);
                    nwritten ++;
                    place /= base;
                }
            }
            else
            {
                fputc('%', stream);
                nwritten ++;
                if (pound)
                {
                    fputc('#', stream);
                    nwritten ++;
                }
                if (zero)
                {
                    fputc('0', stream);
                    nwritten ++;
                }
                if (negfield)
                {
                    fputc('-', stream);
                    nwritten ++;
                }
                if (space)
                {
                    fputc(' ', stream);
                    nwritten ++;
                }
                if (plus)
                {
                    fputc('+', stream);
                    nwritten ++;
                }
                if (thousands)
                {
                    fputc('\'', stream);
                    nwritten ++;
                }
                if (fieldwidth)
                {
                    nwritten += fprintf(stream, "%d", fieldwidth);
                }
                if (precision)
                {
                    nwritten += fprintf(stream, ".%d", precision);
                }
                nwritten += fprintf(stream, "%s",
                                    (lengthmod == 2) ? "ll"
                                    : (lengthmod == 1) ? "l"
                                    : (lengthmod == -1) ? "h"
                                    : (lengthmod == -2) ? "hh"
                                    : "");
                fputc(c, stream);
                nwritten ++;
            }
        }
        else
        {
            fputc(c, stream);
        }
    }
    return nwritten;
}

#ifdef __EFI__
int fflush(FILE *stream)
{
    if (stream && (stream->fd != -1))
    {
        return 0;
    }
    return EOF;
}

void *memcpy(void *dst, const void *src, size_t len)
{
    CopyMem(dst, (void *) src, len);
    return dst;
}

void *memset(void *b, int c, size_t len)
{
    SetMem(b, len, c);
    return b;
}

FILE *fopen(const char *path, const char *mode)
{
    FILE *ret = NULL;
    int i;

    if (strcmp(mode, "r")
        &&
        strcmp(mode, "rb"))
    {
        return ret;
    }
    for (i = 0; i < (3 + FOPEN_MAX); i ++)
    {
        if (efilibc_FILEs[i].fd == -1)
        {
            int fd;
            int j;
            CHAR16 *name16 = NULL;

            fd = 3;
          fopen_next_fd:
            for (j = 0; j < (3 + FOPEN_MAX); j ++)
            {
                if (efilibc_FILEs[j].fd == fd)
                {
                    fd ++;
                    goto fopen_next_fd;
                }
            }
            name16 = (CHAR16 *) efilibc_utf8to16(path);
            if (name16)
            {
                EFI_DEVICE_PATH *efi_device_path;

                efi_device_path = FileDevicePath(efilibc_loaded_image ? efilibc_loaded_image->DeviceHandle : NULL, name16);
                if (efi_device_path)
                {
                    if (! EFI_ERROR(OpenSimpleReadFile(FALSE,
                                                       NULL, 0,
                                                       &efi_device_path,
                                                       &(efilibc_FILEs[i].efi_device_handle),
                                                       &(efilibc_FILEs[i].simple_read_handle))))
                    {
                        size_t k;

                        ret = efilibc_FILEs + i;
                        ret->fd = fd;
                        for (k = 0; k < sizeof(ret->ungetc_buf) / sizeof(*(ret->ungetc_buf)); k ++)
                        {
                            ret->ungetc_buf[k] = EOF;
                        }
                        for (k = 0; k < sizeof(ret->utf8out_buf) / sizeof(*(ret->utf8out_buf)); k ++)
                        {
                            ret->utf8out_buf[k] = '\0';
                        }
                        ret->offset = 0;
                        ret->at_eof = 0;
                        ret->at_eof = 0;
                        ret->data = NULL;
                    }
                    BS->FreePool((VOID *) efi_device_path);
                }
                free((void *) name16);
                name16 = NULL;
            }
        }
    }
    return ret;
}

int fclose(FILE *stream)
{
    if ((! stream) || (stream->fd == -1))
    {
        return EOF;
    }
    if (stream->fd > 2)
    {
        CloseSimpleReadFile(stream->simple_read_handle);
    }
    stream->fd = -1;
    return 0;
}
#endif /* defined(__EFI__) */

int fputs(const char *str, FILE *stream)
{
    size_t i;
    int c;

    if (! str) return EOF;
    for (i = 0; (c = (unsigned char) str[i]) != '\0'; i ++)
    {
        if (fputc(c, stream) != c)
        {
            return EOF;
        }
    }
    return 0;
}

int puts(const char *str)
{
    int ret;

    ret = fputs(str, stdout);
    if (ret != EOF)
    {
        ret = fputc('\n', stdout);
    }
    return ret;
}

#ifdef __EFI__
int fputc(int c, FILE *stream)
{
    if (c == EOF) return c;
    c = (int) (unsigned char) c;
    c = c & 0xFF;
    if (stream
        &&
        ((stream->fd == 1)
         ||
         (stream->fd == 2)))
    {
        SIMPLE_TEXT_OUTPUT_INTERFACE *toif = NULL;
        CHAR16 buf[2];
        int c2;

        if (stream->fd == 2) toif = ST->StdErr;
        if (toif == NULL) toif = ST->ConOut;
        if ((c & 0xC0) != 0x80)
        {
            size_t i;

            for (i = 0; i < sizeof(stream->utf8out_buf)/sizeof(*(stream->utf8out_buf)); i ++)
            {
                if (stream->utf8out_buf[i] != '\0')
                {
                    buf[0] = (CHAR16) 0xFFFD;
                    buf[1] = L'\0';
                    toif->OutputString(toif, buf);
                    stream->utf8out_buf[i] = '\0';
                }
            }
            if (UTF8SEQLEN(c) > 1)
            {
                stream->utf8out_buf[0] = c;
                return c;
            }
            else if (c & 0x80)
            {
                buf[0] = (CHAR16) 0xFFFD;
                buf[1] = L'\0';
                toif->OutputString(toif, buf);
                return c;
            }
            /* otherwise, fall through... */
        }
        else if ((c2 = (int) (unsigned char) stream->utf8out_buf[0]) != '\0')
        {
            size_t seqlen;

            seqlen = strlen(stream->utf8out_buf);
            stream->utf8out_buf[seqlen] = c;
            seqlen ++;
            stream->utf8out_buf[seqlen] = '\0';
            if (UTF8SEQLEN(c2) <= seqlen)
            {
                CHAR16 *seqbuf;

                seqbuf = efilibc_utf8to16(stream->utf8out_buf);
                if (seqbuf != NULL)
                {
                    toif->OutputString(toif, seqbuf);
                    free((void *) seqbuf);
                }
                else
                {
                    buf[0] = (CHAR16) 0xFFFD;
                    buf[1] = L'\0';
                    toif->OutputString(toif, buf);
                }
                memset((void *) stream->utf8out_buf, '\0', sizeof(stream->utf8out_buf));
            }
            return c;
        }
        else
        {
            buf[0] = (CHAR16) 0xFFFD;
            buf[1] = L'\0';
            toif->OutputString(toif, buf);
            return c;
        }
        buf[0] = c;
        buf[1] = L'\0';
        if (c == '\n')
        {
            toif->OutputString(toif, L"\r");
        }
        if (c == '\t')
        {
            do
            {
                toif->OutputString(toif, L" ");
            }
            while (toif->Mode->CursorColumn % 8);
        }
        else
        {
            toif->OutputString(toif, buf);
        }
        return c;
    }
    return EOF;
}
#endif /* defined(__EFI__) */

#undef putchar
int putchar(int c)
{
    return fputc(c, stdout);
}

int sprintf(char *str, const char *fmt, ...)
{
    int ret;
    va_list va;

    va_start(va, fmt);
    ret = vsprintf(str, fmt, va);
    va_end(va);
    return ret;
}

typedef struct {
    char *str;
    size_t offset;
} ostrFILE;

static int osfputc(int c, FILE *ostrfile_as_stream)
{
    ostrFILE *ostrfile;

    ostrfile = (ostrFILE *) (void *) ostrfile_as_stream;
    ostrfile->str[ostrfile->offset] = c;
    ostrfile->offset ++;
    return c;
}

static size_t osfwrite(const void *ptr, size_t sz, size_t nmem, FILE *ostrfile_as_stream)
{
    ostrFILE *ostrfile;

    ostrfile = (ostrFILE *) (void *) ostrfile_as_stream;
    memcpy((void *) (ostrfile->str + ostrfile->offset), ptr, sz * nmem);
    ostrfile->offset += sz * nmem;
    return nmem;
}

static int osfprintf(FILE *ostrfile_as_stream, const char *fmt, ...)
{
    va_list va;
    int ret;
    ostrFILE *ostrfile;

    ostrfile = (ostrFILE *) (void *) ostrfile_as_stream;
    va_start(va, fmt);
    ret = vsprintf(ostrfile->str + ostrfile->offset, fmt, va);
    ostrfile->offset += ret;
    va_end(va);
    return ret;
}

int vsprintf(char *str, const char *fmt, va_list va)
{
    ostrFILE ostrfile[1];
    int ret;

    ostrfile->str = str;
    ostrfile->offset = 0;
    ret = vfnprintf((FILE *) (void *) ostrfile, osfputc, osfwrite, osfprintf, fmt, va);
    str[ostrfile->offset] = '\0';
    return ret;
}

int vfprintf(FILE *stream, const char *fmt, va_list va)
{
    return vfnprintf(stream, fputc, fwrite, fprintf, fmt, va);
}

#ifdef __EFI__
void perror(const char *str)
{
    fprintf(stderr, "%s: Unknown Error\n", str); fflush(stderr);
}

FILE *freopen(const char *path, const char *mode, FILE *stream)
{
    return NULL;
}
#endif /* defined(__EFI__) */

#undef atoi
int atoi(const char *nptr)
{
    return (int) strtol(nptr, (char **)NULL, 10);
}

#ifdef __EFI__
int fileno(FILE *stream)
{
    if (stream)
    {
        return stream->fd;
    }
    return -1;
}

char *getenv(const char *name)
{
    return NULL;
}

int fgetc(FILE *stream)
{
    int ret = EOF;

    if (stream && (stream->fd != -1))
    {
        if (stream->ungetc_buf[0] != EOF)
        {
            size_t i;

            for (i = 0; i < sizeof(stream->ungetc_buf) / sizeof(*(stream->ungetc_buf)); i ++)
            {
                if (stream->ungetc_buf[i] == EOF)
                {
                    break;
                }
            }
            ret = stream->ungetc_buf[i - 1];
            stream->ungetc_buf[i - 1] = EOF;
        }
        else if (stream->fd > 2)
        {
            UINTN read_size;
            char ch;

            read_size = 1;
            if (! EFI_ERROR(ReadSimpleReadFile(stream->simple_read_handle,
                                               stream->offset,
                                               &read_size,
                                               (void *) &ch)))
            {
                if (read_size == 1)
                {
                    ret = (unsigned char) ch;
                    stream->offset += read_size;
                }
                else
                {
                    stream->at_eof = 1;
                }
            }
        }
    }
    return ret;
}

int ungetc(int c, FILE *stream)
{
    if (stream && (stream->fd != -1) && (c != EOF) && (stream->ungetc_buf[sizeof(stream->ungetc_buf) / sizeof(*(stream->ungetc_buf)) - 1] == EOF))
    {
        size_t i;

        for (i = 0; i < sizeof(stream->ungetc_buf) / sizeof(*(stream->ungetc_buf)); i ++)
        {
            if (stream->ungetc_buf[i] == EOF)
            {
                stream->ungetc_buf[i] = c;
                break;
            }
        }
        return c;
    }
    return EOF;
}
#endif /* defined(__EFI__) */

#undef isprint
int isprint(int c)
{
    return (c >= ' ') && (c <= '~');
}

#undef isspace
int isspace(int c)
{
    return (c == ' ') || (c == '\v') || (c == '\t') || (c == '\n') || (c == '\f') || (c == '\r');
}

#undef isdigit
int isdigit(int c)
{
    return (c >= '0') && (c <= '9');
}

#undef isxdigit
int isxdigit(int c)
{
    return ((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f'));
}

long strtol(const char *nptr, char **endptr, int base)
{
    long ret = 0L;
    long sgn = 1L;
    int c;
    size_t i = 0;

    if (! nptr)
    {
        if (endptr)
        {
            *endptr = (char *) nptr;
        }
        return ret;
    }
    while (isspace(nptr[i]))
    {
        i ++;
    }
    if (nptr[i] == '-')
    {
        sgn = -sgn;
        i ++;
    }
    if (((! strncmp(nptr + i, "0x", strlen("0x")))
         ||
         (! strncmp(nptr + i, "0X", strlen("0X"))))
        &&
        ((base == 0)
         ||
         (base == 16)))
    {
        base = 16;
        i += strlen("0x");
    }
    else if ((! strncmp(nptr + i, "0", strlen("0")))
             &&
             (base == 0))
    {
        base = 8;
    }
    else if (base == 0)
    {
        base = 10;
    }
    while ((c = nptr[i]) != '\0')
    {
        if (! c) break;
        c = tolower(c);
        if ((c >= '0') && (c <= '9') && ((c - '0') < base))
        {
            c -= '0';
        }
        else if ((c >= 'a') && (c <= 'z') && (base > 10) && ((c - 'a' + 10) < base))
        {
            c -= 'a';
            c += 10;
        }
        else
        {
            break;
        }
        ret *= base;
        ret += c;
        i ++;
    }
    if (endptr)
    {
        *endptr = ((char *) nptr) + i;
    }
    return sgn * ret;
}

double strtod(const char *nptr, char **endptr)
{
    double ret = 0.0;
    double sgn = 1.0;
    double mul = 1.0;
    size_t i = 0;
    int c;

    if (! nptr)
    {
        if (endptr)
        {
            *endptr = (char *) nptr;
        }
        return ret;
    }
    if (nptr[i] == '-')
    {
        sgn = -sgn;
        i ++;
    }
    for (; (c = nptr[i]) != '\0'; i ++)
    {
        if (! c) break;
        if ((c >= '0') && (c <= '9'))
        {
            c -= '0';
        }
        else if ((c == '.') && (mul == 1.0))
        {
            mul = 0.1;
            continue;
        }
        else
        {
            break;
        }
        if (mul == 1.0)
        {
            ret *= 10.0;
            ret += 1.0 * c;
        }
        else
        {
            ret += mul * c;
            mul /= 10.0;
        }
    }
    if (endptr)
    {
        *endptr = ((char *) nptr) + i;
    }
    return ret;
}

char *strcpy(char *dst, const char *src)
{
    if (! dst) return NULL;
    if (! src) return NULL;
    return memcpy(dst, src, strlen(src) + 1);
}

char *strstr(const char *haystack, const char *needle)
{
    size_t needle_sz;

    if (! needle) return NULL;
    if (! haystack) return NULL;
    if (! *needle) return (char *) haystack;
    needle_sz = strlen(needle);
    while (*haystack)
    {
        if (! strncmp(haystack, needle, needle_sz)) return (char *) haystack;
        haystack ++;
    }
    return NULL;
}

int tolower(int c)
{
    if ((c >= 'A') && (c <= 'Z')) return c + 'a' - 'A';
    return c;
}

int vfnscanf(FILE *stream,
             int (*fgetc)(FILE *stream),
             int (*ungetc)(int c, FILE *stream),
             const char *fmt, va_list va)
{
    size_t i;
    int c, c2, c3, c4;
    int ret;
    int ok;

    ret = 0;
    for (i = 0; (c = (unsigned char) fmt[i]) != '\0'; i ++)
    {
        if ((c == '%') && ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
        {
            int lengthmod = 0;

            i ++;
            c = c2;
            if ((c == 'h')
                &&
                ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
            {
                i ++;
                lengthmod = -1;
                c = c2;
                if ((c == 'h')
                    &&
                    ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
                {
                    i ++;
                    lengthmod = -2;
                    c = c2;
                }
            }
            else if ((c == 'l')
                &&
                ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
            {
                i ++;
                lengthmod = 1;
                c = c2;
                if ((c == 'l')
                    &&
                    ((c2 = (unsigned char) fmt[i + 1]) != '\0'))
                {
                    i ++;
                    lengthmod = 2;
                    c = c2;
                }
            }
            if ((c != 'c') && (c != '%'))
            {
                while (((c2 = fgetc(stream)) != EOF)
                       &&
                       isspace(c2))
                {
                    c2 = EOF;
                }
                if (c2 != EOF)
                {
                    ungetc(c2, stream);
                }
            }
            if (c == 'c')
            {
                char *cp;

                cp = va_arg(va, char *);
                if ((c2 = fgetc(stream)) != EOF)
                {
                    *cp = c2;
                    ret ++;
                }
                else
                {
                    break;
                }
            }
            else if ((c == 'd') || (c == 'i'))
            {
                long long snum = 0LL;
                int base;

                ok = 0;
                base = 10;
                if (c == 'i')
                {
                    if (((c2 = fgetc(stream)) != EOF)
                        &&
                        (c2 == '0'))
                    {
                        ok = 1;
                        snum = 0LL;
                        ret ++;
                        base = 8;
                        if (((c3 = fgetc(stream)) != EOF)
                            &&
                            (tolower(c3) == 'x')
                            &&
                            ((c4 = fgetc(stream)) != EOF)
                            &&
                            isxdigit(c4))
                        {
                            base = 16;
                            ungetc(c4, stream);
                        }
                        if ((c3 != EOF) && (tolower(c3) != 'x'))
                        {
                            ungetc(c3, stream);
                        }
                        else if ((c3 != EOF)
                                 &&
                                 (tolower(c3) == 'x')
                                 &&
                                 (c4 != EOF)
                                 &&
                                 ! isxdigit(c4))
                        {
                            ungetc(c4, stream);
                            ungetc(c3, stream);
                        }
                    }
                    if ((c2 != EOF) && (c2 != '0'))
                    {
                        ungetc(c2, stream);
                    }
                }
                while (((c2 = fgetc(stream)) != EOF)
                       &&
                       ((base == 16) ? isxdigit(c2) : (isdigit(c2)
                                                       &&
                                                       ((c2 - '0') < base))))
                {
                    if (! ok)
                    {
                        ok = 1;
                        snum = 0LL;
                        ret ++;
                    }
                    snum *= 10LL;
                    snum += c2 - '0';
                    c2 = EOF;
                }
                if (c2 != EOF)
                {
                    ungetc(c2, stream);
                }
                if (! ok) break;
                if (lengthmod == 2)
                {
                    long long *llip;

                    llip = va_arg(va, long long *);
                    *llip = snum;
                }
                else if (lengthmod == 1)
                {
                    long *lip;

                    lip = va_arg(va, long *);
                    *lip = (long) snum;
                }
                else if (lengthmod == -1)
                {
                    short *hip;

                    hip = va_arg(va, short *);
                    *hip = (short) snum;
                }
                else if (lengthmod == -2)
                {
                    char *hhip;

                    hhip = va_arg(va, char *);
                    *hhip = (char) snum;
                }
                else
                {
                    int *ip;

                    ip = va_arg(va, int *);
                    *ip = (int) snum;
                }
            }
            else if ((c == 'o') || (c == 'u') || (c == 'x'))
            {
                unsigned long long num = 0ULL;

                ok = 0;
                while (((c2 = fgetc(stream)) != EOF)
                       &&
                       ((c == 'u') ? isdigit(c2) : isxdigit(c2)))
                {
                    if ((c == 'o') && (c >= '8')) break;
                    if (! ok)
                    {
                        ok = 1;
                        num = 0ULL;
                        ret ++;
                    }
                    num *= (c == 'u') ? 10ULL : (c == 'o') ? 8ULL : 16ULL;
                    c2 = tolower(c2);
                    num += c2 - (isdigit(c2) ? '0' : ('a' - 10));
                    c2 = EOF;
                }
                if (c2 != EOF)
                {
                    ungetc(c2, stream);
                }
                if (! ok) break;
                if (lengthmod == 2)
                {
                    unsigned long long *llup;

                    llup = va_arg(va, unsigned long long *);
                    *llup = num;
                }
                else if (lengthmod == 1)
                {
                    unsigned long *lup;

                    lup = va_arg(va, unsigned long *);
                    *lup = (unsigned long) num;
                }
                else if (lengthmod == -1)
                {
                    unsigned short *hup;

                    hup = va_arg(va, unsigned short *);
                    *hup = (unsigned short) num;
                }
                else if (lengthmod == -2)
                {
                    unsigned char *hhup;

                    hhup = va_arg(va, unsigned char *);
                    *hhup = (unsigned char) num;
                }
                else
                {
                    unsigned *up;

                    up = va_arg(va, unsigned *);
                    *up = (unsigned) num;
                }
            }
            else if (c == '%')
            {
                c2 = fgetc(stream);
                if (c2 == EOF) break;
                if (c2 != c) break;
                {
                    ungetc(c2, stream);
                    break;
                }
            }
            else
            {
                fprintf(stderr, "%s:%d: unsupported scanf conversion specifier \'%%%c\'\n", __FILE__, __LINE__, c); fflush(stderr);
            }
        }
        else if (isspace(c))
        {
            ok = 0;
            while (((c2 = fgetc(stream)) != EOF)
                   &&
                   isspace(c2))
            {
                ok = 1;
            }
            if (c2 != EOF)
            {
                ungetc(c2, stream);
            }
            if (! ok) break;
        }
        else
        {
            c2 = fgetc(stream);
            if (c2 == EOF) break;
            if (c2 != c)
            {
                ungetc(c2, stream);
                break;
            }
        }
    }
    if (ret == 0)
    {
        c = fgetc(stream);
        if (c != EOF)
        {
            ungetc(c, stream);
        }
        else
        {
            ret = EOF;
        }
    }
    return ret;
}

#define ISFUNGETC_MAX 16

typedef struct {
    const char *str;
    size_t offset;
    int ungetc_buf[ISFUNGETC_MAX];
} istrFILE;

static int isfgetc(FILE *istrfile_as_stream)
{
    int ret;
    istrFILE *istrfile;

    istrfile = (istrFILE *) (void *) istrfile_as_stream;
    ret = EOF;
    if (istrfile->ungetc_buf[0] != EOF)
    {
        size_t i;

        for (i = 0; i < sizeof(istrfile->ungetc_buf) / sizeof(*(istrfile->ungetc_buf)); i ++)
        {
            if (istrfile->ungetc_buf[i] == EOF)
            {
                break;
            }
        }
        ret = istrfile->ungetc_buf[i - 1];
        istrfile->ungetc_buf[i - 1] = EOF;
    }
    else if (istrfile->str[istrfile->offset] != '\0')
    {
        ret = istrfile->str[istrfile->offset];
        istrfile->offset ++;
    }
    return ret;
}

static int isfungetc(int c, FILE *istrfile_as_stream)
{
    istrFILE *istrfile;

    istrfile = (istrFILE *) (void *) istrfile_as_stream;
    if ((c != EOF) && (istrfile->ungetc_buf[sizeof(istrfile->ungetc_buf) / sizeof(*(istrfile->ungetc_buf)) - 1] == EOF))
    {
        size_t i;

        for (i = 0; i < sizeof(istrfile->ungetc_buf) / sizeof(*(istrfile->ungetc_buf)); i ++)
        {
            if (istrfile->ungetc_buf[i] == EOF)
            {
                istrfile->ungetc_buf[i] = c;
                break;
            }
        }
        return c;
    }
    return EOF;
}

int vsscanf(const char *str, const char *fmt, va_list va)
{
    int ret;
    istrFILE istrfile[1];
    size_t i;

    istrfile->str = str;
    istrfile->offset = 0;
    for (i = 0; i < sizeof(istrfile->ungetc_buf) / sizeof(*(istrfile->ungetc_buf)); i ++)
    {
        istrfile->ungetc_buf[i] = EOF;
    }
    ret = vfnscanf((FILE *) (void *) istrfile, isfgetc, isfungetc, fmt, va);
    return ret;
}

int vfscanf(FILE *stream, const char *fmt, va_list va)
{
    int ret;

    ret = vfnscanf(stream, fgetc, ungetc, fmt, va);
    return ret;
}

int sscanf(const char *str, const char *fmt, ...)
{
    int ret;
    va_list va;

    va_start(va, fmt);
    ret = vsscanf(str, fmt, va);
    va_end(va);
    return ret;
}

int fscanf(FILE *stream, const char *fmt, ...)
{
    int ret;
    va_list va;

    va_start(va, fmt);
    ret = vfscanf(stream, fmt, va);
    va_end(va);
    return ret;
}

#ifdef __EFI__
int feof(FILE *stream)
{
    if (stream && (stream->fd != -1))
    {
        return stream->at_eof;
    }
    return 1;
}

long ftell(FILE *stream)
{
    if (stream && (stream->fd != -1) && (stream->fd > 2))
    {
        return stream->offset;
    }
    return -1;
}
#endif /* defined(__EFI__) */

size_t fwrite(const void *ptr, size_t sz, size_t nmem, FILE *stream)
{
    size_t nwritten;

    for (nwritten = 0; nwritten < nmem; nwritten ++)
    {
        size_t i;

        for (i = 0; i < sz; i ++)
        {
            int c = (unsigned char) ((const char *) ptr)[nwritten * sz + i];
            if (fputc(c, stream) != c)
            {
                break;
            }
        }
        if (i < sz)
        {
            break;
        }
    }
    return nwritten;
}
