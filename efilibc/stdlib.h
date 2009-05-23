/* stdlib.h - C library subset for the Extensible Firmware Interface
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

#ifndef EFILIBC_STDLIB_H_INC
#define EFILIBC_STDLIB_H_INC 1

#include <efi.h>
#include <efilib.h>

#include <sys/types.h>

extern int atoi(const char *nptr);

extern void free(void *ptr);

extern void *malloc(size_t sz);

extern void *realloc(void *ptr, size_t sz);

extern void exit(int status);

extern char *getenv(const char *name);

extern long strtol(const char *nptr, char **endptr, int base);

extern double strtod(const char *nptr, char **endptr);

#endif /* ! defined(EFILIBC_STDLIB_H_INC) */
