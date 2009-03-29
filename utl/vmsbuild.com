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
$ write sys$output "$! to run myman, first define a logical:"
$ myman :== "$''f$environment("default")'myman.exe"
$ write sys$output "$ myman :== ''myman'"
