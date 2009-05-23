/* guess.h - feature guessing for the MyMan video game
 * Copyright 2008, Benjamin C. Wiley Sittler <bsittler@gmail.com>
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

#ifndef MYMAN_GUESS_H_INCLUDED
#define MYMAN_GUESS_H_INCLUDED 1

/* some Win32 C compilers do not define WIN32 */
#if ! defined(WIN32)
#ifdef _WIN32
#define WIN32 _WIN32
#endif
#endif

#if ! defined(WIN32)
#ifdef _WIN32_
#define WIN32 _WIN32_
#endif
#endif

#if ! defined(WIN32)
#ifdef _WIN32_
#define WIN32 _WIN32_
#endif
#endif

#if ! defined(WIN32)
#ifdef __WIN32__
#define WIN32 __WIN32__
#endif
#endif

/* We use WIN16 as a marker for 16-bit windows */
#ifndef WIN32
#ifndef WIN16
#ifdef __WINDOWS__
#define WIN16 __WINDOWS__
#else
#ifdef _WINDOWS_
#define WIN16 _WINDOWS_
#ifdef WINDOWS
#define WIN16 WINDOWS
#endif
#endif
#endif
#endif
#endif

#if ! defined(WIN32)
#ifndef WIN16
#if defined(DOS) || defined(__TURBOC__) || defined(_DOS) || defined(MSDOS) || defined(__DOS__)

/* some DOS C compilers do not define __MSDOS__ */
#ifndef __MSDOS__
#define __MSDOS__ 1
#endif

#endif /* defined(DOS) || defined(__TURBOC__) || defined(_DOS) || defined(MSDOS) || defined(__DOS__) */
#endif /* ! defined(WIN16) */
#endif /* ! defined(WIN32) */

/* Do we have <winsock.h>? */
#ifndef HAVE_WINSOCK_H
#if defined(WIN16) || defined(WIN32)
#define HAVE_WINSOCK_H 1
#endif
#endif

#ifndef HAVE_WINSOCK_H
#define HAVE_WINSOCK_H 0
#endif


/* HAVE_FCNTL_H: do we have <fcntl.h>? */

#ifdef __EFI__
#ifndef HAVE_FCNTL_H
#define HAVE_FCNTL_H 0
#endif
#endif

#ifndef HAVE_FCNTL_H
#define HAVE_FCNTL_H 1
#endif

/* HAVE_IO_H: do we have <io.h>? */

#ifndef HAVE_IO_H
#ifdef __TURBOC__
#define HAVE_IO_H 1
#endif
#endif

#ifndef HAVE_IO_H
#ifdef __BORLANDC__
#define HAVE_IO_H 1
#endif
#endif

#ifndef HAVE_IO_H
#define HAVE_IO_H 0
#endif

/* HAVE_IOCTL_H: do we have <ioctl.h>? */

#ifndef HAVE_IOCTL_H
#if defined(__PACIFIC__) || defined(HI_TECH_C) || defined(macintosh)
#define HAVE_IOCTL_H 1
#endif
#endif

#ifndef HAVE_IOCTL_H
#define HAVE_IOCTL_H 0
#endif

/* HAVE_LANGINFO_H: do we have <langinfo.h>? */

#ifndef HAVE_LANGINFO_H
#if ! (defined(WIN32) || defined(CPM) || defined(macintosh) || defined(__MSDOS__) || defined(atarist) || defined(__atarist__) || defined(__DMC__) || defined(__HAIKU__) || defined(__WATCOMC__) || defined(__EFI__))
#define HAVE_LANGINFO_H 1
#endif
#endif

#ifndef HAVE_LANGINFO_H
#define HAVE_LANGINFO_H 0
#endif

/* HAVE_LOCALE_H: do we have <locale.h>? */

#ifndef HAVE_LOCALE_H
#if ! (defined(LSI_C) || defined(__PACIFIC__) || defined(HI_TECH_C) || defined(SMALL_C) || defined(__TURBOC__) || (defined(__BCC__) && defined(__MSDOS__)) || defined(__EFI__))
#define HAVE_LOCALE_H 1
#endif
#endif

#ifndef HAVE_LOCALE_H
#define HAVE_LOCALE_H 0
#endif

/* HAVE_PSAPI_H: do we have <psapi.h>? */

#ifndef HAVE_PSAPI_H
#ifdef WIN32
#if ! (defined(__DMC__) || defined(__TINYC__))
#define HAVE_PSAPI_H 1
#endif
#endif
#endif

#ifndef HAVE_PSAPI_H
#define HAVE_PSAPI_H 0
#endif

/* HAVE_SHLWAPI_H: do we have <shlwapi.h>? */

#ifndef HAVE_SHLWAPI_H
#ifdef WIN32
#if ! (defined(__DMC__) || defined(__TINYC__))
#define HAVE_SHLWAPI_H 1
#endif
#endif
#endif

#ifndef HAVE_SHLWAPI_H
#define HAVE_SHLWAPI_H 0
#endif

/* HAVE_STDINT_H: do we have <stdint.h>? */

#ifndef HAVE_STDINT_H
#if ! (defined(macintosh) || defined(AZTEC) || defined(LSI_C) || defined(__PACIFIC__) || defined(HI_TECH_C) || defined(SMALL_C) || defined(__TURBOC__) || (defined(__BCC__) && defined(__MSDOS__)) || defined(__EFI__))
#ifdef __atarist__
#if ((! defined(__GNUC__)) || (__GNUC__ > 2))
#define HAVE_STDINT_H 1
#else /* ! ((! defined(__GNUC__)) || (__GNUC__ > 2)) */
#define HAVE_STDINT_H 0
#endif /* ! ((! defined(__GNUC__)) || (__GNUC__ > 2)) */
#else /* ! defined(__atarist__) */
#define HAVE_STDINT_H 1
#endif /* ! defined(__atarist__) */
#endif /* ! (defined(macintosh) || defined(LSI_C) || defined(__PACIFIC__) || defined(HI_TECH_C) || defined(SMALL_C) || defined(__TURBOC__) || (defined(__BCC__) && defined(__MSDOS__)) || defined(__EFI__)) */
#endif

#ifndef HAVE_STDINT_H
#define HAVE_STDINT_H 0
#endif

/* HAVE_SYS_H: do we have <sys.h>? */

#ifndef HAVE_SYS_H
#if defined(__PACIFIC__) || defined(HI_TECH_C)
#define HAVE_SYS_H 1
#endif
#endif

#ifndef HAVE_SYS_H
#define HAVE_SYS_H 0
#endif

/* HAVE_SYS_IOCTL_H: do we have <sys/ioctl.h>? */

#ifndef HAVE_SYS_IOCTL_H
#if ! (defined(AZTEC) || defined(__PACIFIC__) || defined(HI_TECH_C) || defined(macintosh) || defined(__TURBOC__) || defined(LSI_C) || defined(__BCC__) || defined(__DMC__) || defined(__WATCOMC__) || defined(__TINYC__) || defined(__EFI__))
#define HAVE_SYS_IOCTL_H 1
#endif
#endif

#ifndef HAVE_SYS_IOCTL_H
#define HAVE_SYS_IOCTL_H 0
#endif

/* HAVE_SYS_SOCKET_H: do we have <sys/socket.h>? */

#ifndef HAVE_SYS_SOCKET_H
#if ! (defined(__MSDOS__) || defined(CPM) || defined(macintosh) || defined(__DMC__) || defined(__WATCOMC__) || defined(__EFI__))
#if ! (defined(__atarist__) && defined(__GNUC__) && (__GNUC__ == 2))
#define HAVE_SYS_SOCKET_H 1
#endif /* ! (defined(__atarist__) && defined(__GNUC__) && (__GNUC__ == 2)) */
#endif /* ! (defined(__MSDOS__) || defined(CPM) || defined(macintosh) || defined(__DMC__) || defined(__WATCOMC__) || defined(__EFI__)) */
#endif

#ifndef HAVE_SYS_SOCKET_H
#define HAVE_SYS_SOCKET_H 0
#endif

/* HAVE_SYS_STAT_H: do we have <sys/stat.h>? */

#ifndef HAVE_SYS_STAT_H
#if ! (defined(__MSDOS__) || defined(CPM) || defined(macintosh) || defined(__EFI__))
#define HAVE_SYS_STAT_H 1
#endif
#endif

#ifndef HAVE_SYS_STAT_H
#define HAVE_SYS_STAT_H 0
#endif

/* HAVE_SYS_TIME_H: do we have <sys/time.h>? */

#ifndef HAVE_SYS_TIME_H
#if ! (defined(AZTEC) || defined(LSI_C) || defined(macintosh) || defined(__PACIFIC__) || defined(HI_TECH_C) || defined(SMALL_C) || defined(__TURBOC__) || defined(__WATCOMC__) || defined(__POCC__) || defined(__EFI__))
#define HAVE_SYS_TIME_H 1
#endif
#endif

#ifndef HAVE_SYS_TIME_H
#define HAVE_SYS_TIME_H 0
#endif

/* HAVE_SYS_TYPES_H: do we have <sys/types.h>? */

#ifndef HAVE_SYS_TYPES_H
#ifndef macintosh
#ifndef __PACIFIC__
#ifndef AZTEC
#define HAVE_SYS_TYPES_H 1
#endif
#endif
#endif
#endif

#ifndef HAVE_SYS_TYPES_H
#define HAVE_SYS_TYPES_H 0
#endif

/* HAVE_TLHELP32_H: do we have <tlhelp32.h>? */

#ifndef HAVE_TLHELP32_H
#ifdef WIN32
#ifndef __TINYC__
#define HAVE_TLHELP32_H 1
#endif
#endif
#endif

#ifndef HAVE_TLHELP32_H
#define HAVE_TLHELP32_H 0
#endif

/* HAVE_UNISTD_H: do we have <unistd.h>? */

#ifndef HAVE_UNISTD_H
#if ! (defined(AZTEC) || defined(LSI_C) || defined(macintosh) || defined(__PACIFIC__) || defined(HI_TECH_C) || defined(SMALL_C) || defined(__TURBOC__) || (defined(WIN32) && defined(__DMC__)) || defined(__EFI__))
#define HAVE_UNISTD_H 1
#endif
#endif

#ifndef HAVE_UNISTD_H
#define HAVE_UNISTD_H 0
#endif

/* HAVE_UNIXIO_H: do we have <unixio.h>? */

#ifndef HAVE_UNIXIO_H
#if defined(__PACIFIC__) || defined(HI_TECH_C)
#define HAVE_UNIXIO_H 1
#endif
#endif

#ifndef HAVE_UNIXIO_H
#define HAVE_UNIXIO_H 0
#endif

/* HAVE_WCHAR_H: do we have <wchar.h>? */

#ifndef HAVE_WCHAR_H
#if ! (defined(macintosh) || defined(AZTEC) || defined(LSI_C) || defined(__PACIFIC__) || defined(HI_TECH_C) || defined(SMALL_C) || defined(__TURBOC__) || (defined(__BCC__) && defined(__MSDOS__)) || defined(__atarist__))
#define HAVE_WCHAR_H 1
#endif
#endif

#ifndef HAVE_WCHAR_H
#define HAVE_WCHAR_H 0
#endif

/* HAVE_OLD_GETTIMEOFDAY: do we have a one-argument version of gettimeofday(2)? */
#ifndef HAVE_OLD_GETTIMEOFDAY
#define HAVE_OLD_GETTIMEOFDAY 0
#endif

#if HAVE_OLD_GETTIMEOFDAY
#define myman_gettimeofday(tv,tz) gettimeofday((tv))
#else
#define myman_gettimeofday(tv,tz) gettimeofday((tv),(tz))
#endif

/* HAVE_PUTENV: do we have a putenv() call? */

#if defined(__MSDOS__) || defined(__BORLANDC__)
#ifndef HAVE_PUTENV
#define HAVE_PUTENV 1
#endif
#endif

#if defined(WIN32) || defined(macintosh) || defined(__EFI__)
#ifndef HAVE_PUTENV
#define HAVE_PUTENV 0
#endif
#endif

#ifndef HAVE_PUTENV
#define HAVE_PUTENV 1
#endif

/* HAVE_SETENV: do we have a setenv() call? */

#ifdef macintosh
#if defined (__useAppleExts__) || ((defined (applec) && ! defined (__STDC__)) || (defined (__PPCC__) && __STDC__ == 0))
#if CALL_NOT_IN_CARBON || __MPWINTERNAL__
#ifndef HAVE_SETENV
#define HAVE_SETENV 1
#endif
#endif
#endif
#endif /* defined(macintosh) */

#if defined(WIN32) || defined(__MSDOS__) || defined(__DMC__) || defined(macintosh) || defined(__EFI__)
#ifndef HAVE_SETENV
#define HAVE_SETENV 0
#endif
#endif

#ifndef HAVE_SETENV
#define HAVE_SETENV 1
#endif

/* HAVE__PGMPTR: do we have _pgmptr ? */

#ifdef WIN32
#if ! (defined(__WINE__) || defined(__WATCOMC__) || defined(__BORLANDC__) || defined(__POCC__))
#ifndef HAVE__PGMPTR
#define HAVE__PGMPTR 1
#endif /* ! defined(HAVE__PGMPTR) */
#endif /* ! (defined(__WINE__) || defined(__WATCOMC__) || defined(__BORLANDC__) || defined(__POCC__)) */
#endif /* defined(WIN32) */

#ifndef HAVE__PGMPTR
#define HAVE__PGMPTR 0
#endif /* ! defined(HAVE__PGMPTR) */

/* MAIN_NO_ENVP: define only if main() should not accept a third argument */

#ifndef MAIN_NO_ENVP
#if defined(__POCC__)
#define MAIN_NO_ENVP 1
#endif
#endif

#endif /* ! defined(MYMAN_GUESS_H_INCLUDED) */
