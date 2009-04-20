@echo off
@rem # aztecc.bat - compile MyMan using Aztec C
@rem #
@rem # NOTE: this does not work yet -- the compiler runs out of memory and croaks

if "%CLIB%" == "" goto setup

goto ready

:setup

if "%AZTECC%" == "" set AZTECC=c:\az8652a
set CLIB=%AZTECC%\lib\;
set CBIN=%AZTECC%\bin\
set INCLUDE=%AZTECC%\include\;
set PATH=%CBIN%;%PATH%

:ready

if not exist as.exe copy %CBIN%as.exe .
if not exist cgen.exe copy %CBIN%cgen.exe .

if not exist obj mkdir obj

cc -Z4096 +l -B -DAZTEC -D__MSDOS__ -DRAWCURSES=1 -Imygetopt -Iinc -o obj\mygetopt.o mygetopt\mygetopt.c
cc -Z4096 +l -B -DAZTEC -D__MSDOS__ -DRAWCURSES=1 -Imygetopt -Iinc -o obj\utils.o src\utils.c
cc -Z4096 +l -B -DAZTEC -D__MSDOS__ -DRAWCURSES=1 -Imygetopt -Iinc -o obj\logic.o src\logic.c
cc -Z4096 +l -B -DAZTEC -D__MSDOS__ -DRAWCURSES=1 -Imygetopt -Iinc -o obj\myman.o src\myman.c
c +l -B -o myman.exe obj\mygetopt.o obj\utils.o obj\logic.o obj\myman.o

if errorlevel 1 goto croaked
set ret=0
goto ok
:croaked
set ret=1
:ok

%comspec% /c exit %ret%
