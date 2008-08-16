# simple.mk - simple build script for the MyMan video game
# Copyright 2003, 2006 Benjamin Sittler <bsittler@gmail.com>
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.


# See README and INSTALL for standard build instructions

# NOTE: It is recommended that you install a recent version of GNU
# Make and follow the standard build procedure, but this simpler build
# script has been provided in case that is not possible. Edit it to
# suit your configuration, and see the main Makefile for examples.


# Usage: edit to suit, then run

#   make -f simple.mk

# This is a sample makefile to build two MyMan instances, each with
# only a single size/variant binary. No data files are compiled into
# the resulting executables, so they must be run from a directory with
# the data files, or the data filenames must be given using the
# -m/-s/-t command line parameters.

# ansi C compiler
CC=cc
#CC=gcc

# curses library
CURSESLIBS=-lcurses
#CURSESLIBS=-lpdcurses
CURSESINCLUDE=
CURSOPTS=

# getopt library
MY_GETOPTDIR=my_getopt
MY_GETOPTCPPFLAGS=-DMY_GETOPT_H=\"$(MY_GETOPTDIR)/getopt.h\"
MY_GETOPTLIBS=$(MY_GETOPTDIR)/my_getopt.c

all: hugeman bigman smallman squareman

clean:
	-rm hugeman
	-rm bigman
	-rm smallman
	-rm squareman

# "myman" is the default maze; see Makefile for more;
# "big" is the default size (large ASCII-art pictures)
bigman=$(big) $(myman)

bigman: myman.c
	$(CC) $(CFLAGS) $(CURSOPTS) $(CURSESINCLUDE) $(MY_GETOPTCPPFLAGS) $(bigman) -o $@ $< $(CURSESLIBS) $(MY_GETOPTLIBS)

# "myman" is the default maze; see Makefile for more;
# "huge" is the largest size (huge ASCII-art pictures)
TILEFILE_huge=tile5x3.txt
TILEDEFS_huge=-DTILE_W=5 -DTILE_H=3
hugetiles=-DTILEFILE=\"$(TILEFILE_huge)\" $(TILEDEFS_huge)
STILEFILE_huge=stile10x6.txt
STILEDEFS_huge=-DSTILE_W=10 -DSTILE_H=6
hugesprites=-DSTILEFILE=\"$(STILEFILE_huge)\" $(STILEDEFS_huge)
huge=-DSIZE=\"huge\" $(hugetiles) $(hugesprites)
hugeman=$(huge) $(myman)

hugeman: myman.c
	$(CC) $(CFLAGS) $(CURSOPTS) $(CURSESINCLUDE) $(MY_GETOPTCPPFLAGS) $(hugeman) -o $@ $< $(CURSESLIBS) $(MY_GETOPTLIBS)

# "small" uses a pair of character-cells to represent each tile and sprite
TILEFILE_small=tile2x1.txt
TILEDEFS_small=-DTILE_W=2 -DTILE_H=1
smalltiles=-DTILEFILE=\"$(TILEFILE_small)\" $(TILEDEFS_small)
STILEFILE_small=stile2x1.txt
STILEDEFS_small=-DSTILE_W=2 -DSTILE_H=1 -DVISIBLE_EYES=0
smallsprites=-DSTILEFILE=\"$(STILEFILE_small)\" $(STILEDEFS_small)
small=-DSIZE=\"small\" $(smalltiles) $(smallsprites)
smallman=$(small) $(myman)

smallman: myman.c
	$(CC) $(CFLAGS) $(CURSOPTS) $(CURSESINCLUDE) $(MY_GETOPTCPPFLAGS) $(smallman) -o $@ $< $(CURSESLIBS) $(MY_GETOPTLIBS)

# "square" uses one character-cell to represent each tile and sprite
TILEFILE_square=tile1x1.txt
TILEDEFS_square=-DTILE_W=1 -DTILE_H=1
squaretiles=-DTILEFILE=\"$(TILEFILE_square)\" $(TILEDEFS_square)
STILEFILE_square=stile1x1.txt
STILEDEFS_square=-DSTILE_W=1 -DSTILE_H=1 -DVISIBLE_EYES=0
squaresprites=-DSTILEFILE=\"$(STILEFILE_square)\" $(STILEDEFS_square)
square=-DSIZE=\"square\" $(squaretiles) $(squaresprites)
squareman=$(square) $(myman)

squareman: myman.c
	$(CC) $(CFLAGS) $(CURSOPTS) $(CURSESINCLUDE) $(MY_GETOPTCPPFLAGS) $(squareman) -o $@ $< $(CURSESLIBS) $(MY_GETOPTLIBS)

.PHONY: all clean

