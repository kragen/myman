$! MyMan build script for OpenVMS
$!
$! Run it from the parent directory like this:
$! $ @[.utl]vmsbuild
$!
$! If you would like to build with the "BSD 4.4" curses:
$! $ @[.utl]vmsbuild "/define=_BSD44_CURSES"
$!
$! Once it's built, launch easily using (undocumented) mcr:
$! $ mcr []myman
$!
$ write sys$output "$! cleaning"
$ delete myman.obj;*
$ delete mygetopt.obj;*
$ delete utils.obj;*
$ delete myman.exe;*
$ write sys$output "$! compiling"
$ cc -
/include_directory=([.inc],[.mygetopt])-
 'p1' 'p2' 'p3' 'p4' 'p5' 'p6' 'p7' 'p8' -
[.src]myman.c,-
[.mygetopt]mygetopt.c,-
[.src]utils.c
$ write sys$output "$! linking"
$ link myman.obj,utils.obj,mygetopt.obj
$ write sys$output "$! done"
$ write sys$output "$!"
$ write sys$output "$! Example using the undocumented mcr command to run myman"
$ write sys$output "$! with smaller tiles and sprites and a non-default maze:"
$ write sys$output "$!"
$ write sys$output "$! $ mcr []myman -s spr/spr2h -t chr/khr2h -m lvl/kpacman"
$ write sys$output "$!"
$ write sys$output "$! To run myman without ""mcr []"", define a symbol:"
$ myman :== "$''f$environment("default")'myman.exe"
$ write sys$output "$ myman :== ''myman'"
