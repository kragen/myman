/* stdio.h - C library subset for the Extensible Firmware Interface
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

#ifndef EFILIBC_STDIO_H_INC
#define EFILIBC_STDIO_H_INC 1

#include <efi.h>
#include <efilib.h>

#include <stdarg.h>

#include <sys/types.h>

#define EOF -1
#define FOPEN_MAX 20
#define UNGETC_MAX 16

typedef struct {
    int fd;
    int ungetc_buf[UNGETC_MAX];
    char utf8out_buf[5];
    int at_eof;
    EFI_HANDLE efi_device_handle;
    SIMPLE_READ_FILE simple_read_handle;
    UINTN offset;
    void *data;
} FILE;

extern FILE *stdin;

extern FILE *stdout;

extern FILE *stderr;

extern int vfprintf(FILE *stream, const char *fmt, va_list va);

extern int printf(const char *fmt, ...);

extern int fprintf(FILE *stream, const char *fmt, ...);

extern int fflush(FILE *stream);

extern FILE *fopen(const char *path, const char *mode);

extern int fclose(FILE *stream);

extern int fputs(const char *str, FILE *stream);

extern int puts(const char *str);

extern int fputc(int c, FILE *stream);

extern int putchar(int c);

extern int fgetc(FILE *stream);

extern int ungetc(int c, FILE *stream);

extern int sprintf(char *str, const char *fmt, ...);

extern int vsprintf(char *str, const char *fmt, va_list va);

extern void perror(const char *str);

extern FILE *freopen(const char *path, const char *mode, FILE *stream);

extern int fileno(FILE *stream);

extern int vfscanf(FILE *stream, const char *fmt, va_list va);

extern int vsscanf(const char *str, const char *fmt, va_list va);

extern int fscanf(FILE *stream, const char *fmt, ...);

extern int sscanf(const char *str, const char *fmt, ...);

extern int feof(FILE *stream);

extern long ftell(FILE *stream);

extern size_t fwrite(const void *ptr, size_t sz, size_t nmem, FILE *stream);

extern int putchar(int c);

#endif /* ! defined(EFILIBC_STDIO_H_INC) */
