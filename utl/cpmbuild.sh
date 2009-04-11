#!/bin/bash --
# cpmbuild.sh - compile MyMan for CP/M using z88dk or zxcc
#
# NOTE: this does not work yet -- the compiler runs out of memory and croaks

# we use z88dk by default; override this with use_z88dk=no in the environment
use_z88dk="${use_z88dk:-yes}"

# HACK HACK HACK... missing headers
test -d cpmfrotz && rm -rf cpmfrotz
mkdir -p cpmfrotz/sys
touch cpmfrotz/errno.h
# touch cpmfrotz/fcntl.h
# touch cpmfrotz/gcntl.h
# touch cpmfrotz/langinfo.h
# touch cpmfrotz/locale.h
touch cpmfrotz/signal.h
# touch cpmfrotz/sys/ioctl.h
# touch cpmfrotz/sys/socket.h
# touch cpmfrotz/sys/time.h
# touch cpmfrotz/termios.h
# touch cpmfrotz/unistd.h
# touch cpmfrotz/wchar.h

if [ :"$use_z88dk" = :yes ]
then
    # z88dk gets a bit further at the moment...
    zcc +cpm -Imygetopt -DRAWCURSES -Iinc -Icpmfrotz -o smallman.com src/myman.c 2>&1 | sed 's/^\([a-z0-9]\+\):"\([^"]\+\)" L:\([0-9]\+\) \(Warning\|Error\):/\4: \2:\3: /;s/^Warning: /warning: /;s/^Error: //'
else
    # poor old Hi-Tech C does not fare so well...
    zxc -I$(pwd)/mygetopt/ -DRAWCURSES -I$(pwd)/inc/ -I$(pwd)/cpmfrotz/ -I$(pwd)/cpmfrotz/sys/ -o smallman.com src/myman.c
fi

