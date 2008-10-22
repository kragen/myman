@echo off
@rem # aztecc.bat - compile MyMan using Aztec C
@rem #
@rem # NOTE: this does not work yet -- the compiler runs out of memory and croaks

set CC=c +l

if "%CLIB%" == "" goto setup

goto ready

:setup

if "%AZTECC%" == "" set AZTECC=c:\az8652a
set CLIB=%AZTECC%\lib\;
set CBIN=%AZTECC%\bin\
set INCLUDE=%AZTECC%\include\;
set PATH=%CBIN%;%PATH%

:ready

if not exist as.exe copy %cbin%as.exe .
if not exist cgen.exe copy %cbin%cgen.exe .

if not exist obj mkdir obj

%CC% -D RAWCURSES=1 -i mygetopt -i inc -o myman.exe mygetopt\mygetopt.c src\utils.c src\myman.c

if errorlevel 1 goto croaked
set ret=0
goto ok
:croaked
set ret=1
:ok

%comspec% /c exit %ret%
