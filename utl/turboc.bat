@echo off
@rem # turboc.bat - compile MyMan using Turbo C
@rem #
@rem # NOTE: this does not work yet -- the compiler runs out of memory and croaks

set CC=tcc -mh -O -Z -1

if not exist obj mkdir obj

@rem #
@rem # create directory hierarchy for converted files
@rem #

if not exist obj\crlf mkdir obj\crlf
if not exist obj\crlf\mygetopt mkdir obj\crlf\mygetopt
if not exist obj\crlf\inc mkdir obj\crlf\inc
if not exist obj\crlf\src mkdir obj\crlf\src


@rem #
@rem # echox - bootstrap executable for creating multi-line c source with CR/LF pairs
@rem #
@rem # NOTE: this is only created if it does not already exist

if exist obj\echox.exe goto echox

cd obj
%comspec% /c "echo main(i,s)char**s;{while(*++s)puts(*s);} > echox.c"
%CC% -eechox.exe echox.c
cd ..

:echox

@rem #
@rem # u2d - lightweight line-ending converter
@rem #
@rem # NOTE: this is only created if it does not already exist

if exist obj\u2d.exe goto u2d

cd obj
echox "#include <stdio.h>" "main(i){while((i=getchar())!=-1)putchar(i);}" > u2d.c
%CC% -eu2d.exe u2d.c
cd ..

:u2d

%comspec% /c "obj\u2d < mygetopt\getopt.h > obj\crlf\mygetopt\getopt.h"
%comspec% /c "obj\u2d < mygetopt\mygetopt.h > obj\crlf\mygetopt\mygetopt.h"
%comspec% /c "obj\u2d < inc\guess.h > obj\crlf\inc\guess.h"
%comspec% /c "obj\u2d < inc\utils.h > obj\crlf\inc\utils.h"
%comspec% /c "obj\u2d < inc\coniocur.h > obj\crlf\inc\coniocur.h"
%comspec% /c "obj\u2d < inc\rawcurs.h > obj\crlf\inc\rawcurs.h"
%comspec% /c "obj\u2d < inc\optcurs.h > obj\crlf\inc\optcurs.h"

%comspec% /c "obj\u2d < mygetopt\mygetopt.c > obj\crlf\mygetopt\mygetopt.c"
%comspec% /c "obj\u2d < src\utils.c > obj\crlf\src\utils.c"
%comspec% /c "obj\u2d < src\logic.c > obj\crlf\src\logic.c"
%comspec% /c "obj\u2d < src\myman.c > obj\crlf\src\myman.c"
%comspec% /c "obj\u2d < src\main.c > obj\crlf\src\main.c"

cd obj\crlf

rem %CC% -DCONIOCURSES=1 -Imygetopt -Iinc -n..\..\obj -e..\myman.exe src\utils.c src\logic.c src\myman.c mygetopt\*.c
%CC% -DMY_CURSES_H="coniocur.h" -Imygetopt -Iinc -n..\..\obj -e..\myman src\utils src\logic src\myman mygetopt\*.c

if errorlevel 1 goto croaked
set ret=0
goto ok
:croaked
set ret=1
:ok

cd ..\..

@rem #
@rem # remove converted files
@rem #

del obj\crlf\mygetopt\*.h
del obj\crlf\inc\*.h
del obj\crlf\mygetopt\*.c
del obj\crlf\src\*.c

@rem #
@rem # remove directory hierarchy for converted files
@rem #

rmdir obj\crlf\mygetopt
rmdir obj\crlf\inc
rmdir obj\crlf\src
rmdir obj\crlf

%comspec% /c exit %ret%
