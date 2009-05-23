/* string.h - C library subset for the Extensible Firmware Interface
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

#ifndef EFILIBC_STRING_H_INC
#define EFILIBC_STRING_H_INC 1

#include <efi.h>
#include <efilib.h>

#include <sys/types.h>

#define strlen(s) strlena((CHAR8 *) s)
#define strcmp(s1,s2) strcmpa((CHAR8 *) (s1), (CHAR8 *) (s2))
#define strncmp(s1,s2,len) strncmpa((CHAR8 *) (s1), (CHAR8 *) (s2), (UINTN) (len))

extern char *strchr(const char *s, int c);

extern char *strrchr(const char *s, int c);

extern void *memcpy(void *dst, const void *src, size_t len);

extern void *memset(void *b, int c, size_t len);

extern char *strcpy(char *dst, const char *src);

extern char *strstr(const char *haystack, const char *needle);

#endif /* ! defined(EFILIBC_STRING_H_INC) */
