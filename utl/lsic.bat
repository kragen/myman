@echo off
rem lsic.bat - compile MyMan using LSIC-86
rem
rem NOTE: this does not work yet -- the compiler runs out of memory and croaks

set CC=lcc -D__MSDOS__=1 -O -v

if not exist obj mkdir obj

%CC% -DRAWCURSES=1 -Iinc -Imygetopt -o myman.exe mygetopt\mygetopt.c src\utils.c src\logic.c src\myman.c

