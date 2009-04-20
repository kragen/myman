@echo off
rem pacificc.bat - compile MyMan using Pacific C
rem
rem NOTE: this does not work yet -- the compiler runs out of memory and croaks

set CC=pacc -q -c -Bl -X -v -2
%CC% -DRAWCURSES -Iinc -Imygetopt -omyman.exe src\utils.c src\logic.c mygetopt\mygetopt.c src\myman.c %1 %2 %3 %4 %5 %6 %7 %8 %9
