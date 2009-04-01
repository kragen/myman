$! VMSBUILD.COM
$! -*- dcl -*- coding: iso-8859-1 -*-
$! vmsbuild.com - OpenVMS DCL build script for the MyMan video game
$! Copyright 2009, Benjamin C. Wiley Sittler <bsittler@gmail.com>
$!
$! Permission is hereby granted, free of charge, to any person
$! obtaining a copy of this software and associated documentation
$! files (the "Software"), to deal in the Software without
$! restriction, including without limitation the rights to use, copy,
$! modify, merge, publish, distribute, sublicense, and/or sell copies
$! of the Software, and to permit persons to whom the Software is
$! furnished to do so, subject to the following conditions:
$!
$! The above copyright notice and this permission notice shall be
$! included in all copies or substantial portions of the Software.
$!
$! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
$! EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
$! MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
$! NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
$! HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
$! WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
$! OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
$! DEALINGS IN THE SOFTWARE.
$!
$!
$! Run it from the parent directory like this:
$! $ @[.utl]vmsbuild
$!
$! If you would like to build with "BSD 4.4" curses:
$! $ @[.utl]vmsbuild "/define=_BSD44_CURSES"
$!
$! If you would like to build with the built-in (color) TTY driver:
$! $ @[.utl]vmsbuild "/define=RAWCURSES"
$!
$ write sys$output "$! cleaning"
$ if f$search("myman.obj") .nes. "" then delete myman.obj;*
$ if f$search("mygetopt.obj") .nes. "" then delete mygetopt.obj;*
$ if f$search("utils.obj") .nes. "" then delete utils.obj;*
$ if f$search("myman.exe") .nes. "" then delete myman.exe;*
$ if f$search("myman_''f$getsyi("arch_name")'.exe") .nes. "" then delete myman_'f$getsyi("arch_name")'.exe;*
$ write sys$output "$! compiling"
$ cc -
/include_directory=([.inc],[.mygetopt])-
 'p1' 'p2' 'p3' 'p4' 'p5' 'p6' 'p7' 'p8' -
[.src]myman.c,-
[.mygetopt]mygetopt.c,-
[.src]utils.c
$ write sys$output "$! linking"
$ link myman.obj,utils.obj,mygetopt.obj
$ write sys$output "$! copying"
$ copy myman.exe myman_'f$getsyi("arch_name")'.exe
$ write sys$output "$! done"
$ write sys$output "$!"
$ write sys$output "$! Example using the undocumented mcr command to run myman"
$ write sys$output "$! with smaller tiles and sprites and a non-default maze:"
$ write sys$output "$!"
$ write sys$output "$! $ mcr []myman -s spr/spr2h -t chr/khr2h -m lvl/kpacman"
$ write sys$output "$!"
$ write sys$output "$! Once you define symbols and/or extend DCL$PATH to include"
$ write sys$output "$!    ''f$environment("default")'"
$ write sys$output "$! you can omit the ""mcr []"" prefix:"
$ write sys$output "$!"
$ write sys$output "$! $ myman -s spr/spr2h -t chr/khr2h -m lvl/kpacman"
$ write sys$output "$!"
$ write sys$output "$! To define the symbols:"
$ write sys$output "$!"
$ myman :== "$''f$environment("default")'myman_''f$getsyi("arch_name")'.exe"
$ bigman :== 'myman'
$ smallman :== 'myman' -s spr/spr2h -t chr/khr2h
$ squareman :== 'myman' -s spr/spr1 -t chr/khr1
$ write sys$output "$ myman :== ''myman'"
$ write sys$output "$ bigman :== ''bigman'"
$ write sys$output "$ smallman :== ''smallman'"
$ write sys$output "$ squareman :== ''squareman'"
