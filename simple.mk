# simple.mk - simple build script for the MyMan video game
# Copyright 2003, 2006, 2009 Benjamin Sittler <bsittler@gmail.com>
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
MYGETOPTDIR=mygetopt
MYGETOPTCPPFLAGS=-DMYGETOPT
MYGETOPTLIBS=${MYGETOPTDIR}/mygetopt.c

# utility functions
UTILSOBJS=src/utils.c

# game logic
LOGICOBJS=src/logic.c

CINCLUDES=-Iinc -I${MYGETOPTDIR}

all: hugeman bigman smallman squareman

clean:
	-rm hugeman
	-rm bigman
	-rm smallman
	-rm squareman

# "myman" is the default maze; see Makefile for more

# "big" is the default size (large ASCII-art pictures)
bigman=${big} ${myman}

bigman: src/myman.c ${UTILSOBJS} ${LOGICOBJS}
	${CC} ${CINCLUDES} ${CFLAGS} ${CURSOPTS} ${CURSESINCLUDE} ${MYGETOPTCPPFLAGS} ${bigman} -o $@ $< ${CURSESLIBS} ${MYGETOPTLIBS} ${UTILSOBJS} ${LOGICOBJS}

# "huge" is the largest size (huge ASCII-art pictures)
TILEFILE_huge=chr5x3.txt
hugetiles=-DTILEFILE=\"chr/${TILEFILE_huge}\"
SPRITEFILE_huge=spr10x6.txt
hugesprites=-DSPRITEFILE=\"spr/${SPRITEFILE_huge}\"
huge=-DMYMANSIZE=\"huge\" ${hugetiles} ${hugesprites}
hugeman=${huge} ${myman}

hugeman: src/myman.c ${UTILSOBJS} ${LOGICOBJS}
	${CC} ${CINCLUDES} ${CFLAGS} ${CURSOPTS} ${CURSESINCLUDE} ${MYGETOPTCPPFLAGS} ${hugeman} -o $@ $< ${CURSESLIBS} ${MYGETOPTLIBS} ${UTILSOBJS} ${LOGICOBJS}

# "small" uses a pair of character-cells to represent each tile and sprite
TILEFILE_small=khr2h.txt
smalltiles=-DTILEFILE=\"chr/${TILEFILE_small}\"
SPRITEFILE_small=spr2h.txt
smallsprites=-DSPRITEFILE=\"spr/${SPRITEFILE_small}\"
small=-DMYMANSIZE=\"small\" ${smalltiles} ${smallsprites}
smallman=${small} ${myman}

smallman: src/myman.c ${UTILSOBJS} ${LOGICOBJS}
	${CC} ${CINCLUDES} ${CFLAGS} ${CURSOPTS} ${CURSESINCLUDE} ${MYGETOPTCPPFLAGS} ${smallman} -o $@ $< ${CURSESLIBS} ${MYGETOPTLIBS} ${UTILSOBJS} ${LOGICOBJS}

# "square" uses one character-cell to represent each tile and sprite
TILEFILE_square=khr1.txt
squaretiles=-DTILEFILE=\"chr/${TILEFILE_square}\"
SPRITEFILE_square=spr1.txt
squaresprites=-DSPRITEFILE=\"spr/${SPRITEFILE_square}\"
square=-DMYMANSIZE=\"square\" ${squaretiles} ${squaresprites}
squareman=${square} ${myman}

squareman: src/myman.c ${UTILSOBJS} ${LOGICOBJS}
	${CC} ${CINCLUDES} ${CFLAGS} ${CURSOPTS} ${CURSESINCLUDE} ${MYGETOPTCPPFLAGS} ${squareman} -o $@ $< ${CURSESLIBS} ${MYGETOPTLIBS} ${UTILSOBJS} ${LOGICOBJS}

.PHONY: all clean

