#!/usr/bin/env python
# -*- coding: utf-8 -*-
#  asc2txt.py - maze converter for the MyMan video game
#  Copyright 2006 - 2007, Benjamin C. Wiley Sittler <bsittler@gmail.com>
# 
#   Permission is hereby granted, free of charge, to any person
#   obtaining a copy of this software and associated documentation
#   files (the "Software"), to deal in the Software without
#   restriction, including without limitation the rights to use, copy,
#   modify, merge, publish, distribute, sublicense, and/or sell copies
#   of the Software, and to permit persons to whom the Software is
#   furnished to do so, subject to the following conditions:
# 
#   The above copyright notice and this permission notice shall be
#   included in all copies or substantial portions of the Software.
# 
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#   NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
#   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
#   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.

# TODO: detect double walls enclosing no tiles and entirely enclosed
# by but not connected to single walls and convert to single walls

import os, sys, getopt

boxmap_double = [
    u'\N{BULLET}',
    u'\N{BOX DRAWINGS VERTICAL SINGLE AND LEFT DOUBLE}',
    u'\N{BOX DRAWINGS VERTICAL SINGLE AND RIGHT DOUBLE}',
    u'\N{BOX DRAWINGS VERTICAL SINGLE AND HORIZONTAL DOUBLE}',
    u'\N{BOX DRAWINGS DOWN DOUBLE AND HORIZONTAL SINGLE}',
    u'\N{BOX DRAWINGS DOUBLE DOWN AND LEFT}',
    u'\N{BOX DRAWINGS DOUBLE DOWN AND RIGHT}',
    u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}',
    u'\N{BOX DRAWINGS UP DOUBLE AND HORIZONTAL SINGLE}',
    u'\N{BOX DRAWINGS DOUBLE UP AND LEFT}',
    u'\N{BOX DRAWINGS DOUBLE UP AND RIGHT}',
    u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}',
    u'\N{BOX DRAWINGS VERTICAL DOUBLE AND HORIZONTAL SINGLE}',
    u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}',
    u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}',
    u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}',
    u'\N{BOX DRAWINGS DOUBLE HORIZONTAL}',
    u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}',
    u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}',
    u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}',
    u'\N{BOX DRAWINGS DOUBLE VERTICAL}',
    u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}',
    u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}',
    u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}',
    ]

boxmap_single = [
    u'\N{BULLET}',
    u'\N{BOX DRAWINGS LIGHT HORIZONTAL}',
    u'\N{BOX DRAWINGS LIGHT HORIZONTAL}',
    u'\N{BOX DRAWINGS LIGHT HORIZONTAL}',
    u'\N{BOX DRAWINGS LIGHT VERTICAL}',
    u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}',
    u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}',
    u'\N{BOX DRAWINGS LIGHT DOWN AND HORIZONTAL}',
    u'\N{BOX DRAWINGS LIGHT VERTICAL}',
    u'\N{BOX DRAWINGS LIGHT UP AND LEFT}',
    u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}',
    u'\N{BOX DRAWINGS LIGHT UP AND HORIZONTAL}',
    u'\N{BOX DRAWINGS LIGHT VERTICAL}',
    u'\N{BOX DRAWINGS LIGHT VERTICAL AND LEFT}',
    u'\N{BOX DRAWINGS LIGHT VERTICAL AND RIGHT}',
    u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}',
    u'\N{BOX DRAWINGS LIGHT HORIZONTAL}',
    u'\N{BOX DRAWINGS LIGHT DOWN AND HORIZONTAL}',
    u'\N{BOX DRAWINGS LIGHT UP AND HORIZONTAL}',
    u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}',
    u'\N{BOX DRAWINGS LIGHT VERTICAL}',
    u'\N{BOX DRAWINGS LIGHT VERTICAL AND LEFT}',
    u'\N{BOX DRAWINGS LIGHT VERTICAL AND RIGHT}',
    u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}',
    ]

fixup_single = {
    '|': {
    u'\N{BULLET}': (0, u'\N{BOX DRAWINGS LIGHT VERTICAL}'),
    u'\N{BOX DRAWINGS LIGHT HORIZONTAL}': (0, u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}': (8, u'\N{BOX DRAWINGS LIGHT VERTICAL AND LEFT}'),
    u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}': (8, u'\N{BOX DRAWINGS LIGHT VERTICAL AND RIGHT}'),
    u'\N{BOX DRAWINGS LIGHT DOWN AND HORIZONTAL}': (8, u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT UP AND LEFT}': (4, u'\N{BOX DRAWINGS LIGHT VERTICAL AND LEFT}'),
    u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}': (4, u'\N{BOX DRAWINGS LIGHT VERTICAL AND RIGHT}'),
    u'\N{BOX DRAWINGS LIGHT UP AND HORIZONTAL}': (4, u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}'),
    },
    '-': {
    u'\N{BULLET}': (0, u'\N{BOX DRAWINGS LIGHT HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}': (2, u'\N{BOX DRAWINGS LIGHT DOWN AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}': (1, u'\N{BOX DRAWINGS LIGHT DOWN AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT UP AND LEFT}': (2, u'\N{BOX DRAWINGS LIGHT UP AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}': (1, u'\N{BOX DRAWINGS LIGHT UP AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT VERTICAL}': (0, u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT VERTICAL AND LEFT}': (2, u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT VERTICAL AND RIGHT}': (1, u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}'),
    },
    }

fixup_double = {
    '|': {
    u'\N{BULLET}': (0, u'\N{BOX DRAWINGS DOUBLE VERTICAL}'),
    u'\N{BOX DRAWINGS DOUBLE HORIZONTAL}': (0, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE DOWN AND LEFT}': (8, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}'),
    u'\N{BOX DRAWINGS DOUBLE DOWN AND RIGHT}': (8, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}'),
    u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}': (8, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE UP AND LEFT}': (4, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}'),
    u'\N{BOX DRAWINGS DOUBLE UP AND RIGHT}': (4, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}'),
    u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}': (4, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}'),
    },
    '-': {
    u'\N{BULLET}': (0, u'\N{BOX DRAWINGS DOUBLE HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE DOWN AND LEFT}': (2, u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE DOWN AND RIGHT}': (1, u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE UP AND LEFT}': (2, u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE UP AND RIGHT}': (1, u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE VERTICAL}': (0, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}': (2, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}': (1, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}'),
    },
    }

fixup_both = {
    '|': {
    u'\N{BULLET}': (0, u'\N{BOX DRAWINGS DOUBLE VERTICAL}'),
    u'\N{BOX DRAWINGS LIGHT HORIZONTAL}': (0, u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}': (8, u'\N{BOX DRAWINGS LIGHT VERTICAL AND LEFT}'),
    u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}': (8, u'\N{BOX DRAWINGS LIGHT VERTICAL AND RIGHT}'),
    u'\N{BOX DRAWINGS LIGHT DOWN AND HORIZONTAL}': (8, u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT UP AND LEFT}': (4, u'\N{BOX DRAWINGS LIGHT VERTICAL AND LEFT}'),
    u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}': (4, u'\N{BOX DRAWINGS LIGHT VERTICAL AND RIGHT}'),
    u'\N{BOX DRAWINGS LIGHT UP AND HORIZONTAL}': (4, u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE HORIZONTAL}': (0, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE DOWN AND LEFT}': (8, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}'),
    u'\N{BOX DRAWINGS DOUBLE DOWN AND RIGHT}': (8, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}'),
    u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}': (8, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE UP AND LEFT}': (4, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}'),
    u'\N{BOX DRAWINGS DOUBLE UP AND RIGHT}': (4, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}'),
    u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}': (4, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}'),
    },
    '-': {
    u'\N{BULLET}': (0, u'\N{BOX DRAWINGS DOUBLE HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}': (2, u'\N{BOX DRAWINGS LIGHT DOWN AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}': (1, u'\N{BOX DRAWINGS LIGHT DOWN AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT UP AND LEFT}': (2, u'\N{BOX DRAWINGS LIGHT UP AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}': (1, u'\N{BOX DRAWINGS LIGHT UP AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT VERTICAL}': (0, u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT VERTICAL AND LEFT}': (2, u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS LIGHT VERTICAL AND RIGHT}': (1, u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE DOWN AND LEFT}': (2, u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE DOWN AND RIGHT}': (1, u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE UP AND LEFT}': (2, u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE UP AND RIGHT}': (1, u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE VERTICAL}': (0, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}': (2, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}'),
    u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}': (1, u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}'),
    },
    }

boxmap = {
    1: boxmap_single,
    2: boxmap_double,
    3: boxmap_double,
    }

fixup = {
    1: fixup_single,
    2: fixup_double,
    3: fixup_both,
    }

rewrite_ternary = {
    1: {
        4: {
            u'\N{BOX DRAWINGS LIGHT DOWN AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT HORIZONTAL}',
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT UP AND HORIZONTAL}',
        },
        10: {
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND RIGHT}': u'\N{BOX DRAWINGS LIGHT VERTICAL}',
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT VERTICAL AND LEFT}',
        },
        13: {
            u'\N{BOX DRAWINGS LIGHT UP AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT UP AND LEFT}',
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND LEFT}': u'\N{BOX DRAWINGS LIGHT UP AND LEFT}',
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT UP AND LEFT}',
        },
        30: {
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND LEFT}': u'\N{BOX DRAWINGS LIGHT VERTICAL}',
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT VERTICAL AND RIGHT}',
        },
        31: {
            u'\N{BOX DRAWINGS LIGHT UP AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}',
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND RIGHT}': u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}',
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}',
        },
        36: {
            u'\N{BOX DRAWINGS LIGHT UP AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT HORIZONTAL}',
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT DOWN AND HORIZONTAL}',
        },
        37: {
            u'\N{BOX DRAWINGS LIGHT DOWN AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}',
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND LEFT}': u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}',
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}',
        },
        39: {
            u'\N{BOX DRAWINGS LIGHT DOWN AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}',
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND RIGHT}': u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}',
            u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}',
        },
        },
    2: {
        4: {
            u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE HORIZONTAL}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}',
        },
        10: {
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}': u'\N{BOX DRAWINGS DOUBLE VERTICAL}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}',
        },
        13: {
            u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE UP AND LEFT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}': u'\N{BOX DRAWINGS DOUBLE UP AND LEFT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE UP AND LEFT}',
        },
        30: {
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}': u'\N{BOX DRAWINGS DOUBLE VERTICAL}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}',
        },
        31: {
            u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE UP AND RIGHT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}': u'\N{BOX DRAWINGS DOUBLE UP AND RIGHT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE UP AND RIGHT}',
        },
        36: {
            u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE HORIZONTAL}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}',
        },
        37: {
            u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE DOWN AND LEFT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}': u'\N{BOX DRAWINGS DOUBLE DOWN AND LEFT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE DOWN AND LEFT}',
        },
        39: {
            u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE DOWN AND RIGHT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}': u'\N{BOX DRAWINGS DOUBLE DOWN AND RIGHT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS DOUBLE DOWN AND RIGHT}',
        },
        },
    3: {
        1: {
            u'\N{BOX DRAWINGS DOUBLE DOWN AND RIGHT}': u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}',
            u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}': u'\N{BOX DRAWINGS DOWN SINGLE AND LEFT DOUBLE}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}': u'\N{BOX DRAWINGS UP DOUBLE AND RIGHT SINGLE}',
        },
        3: {
            u'\N{BOX DRAWINGS DOUBLE DOWN AND LEFT}': u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}',
            u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}': u'\N{BOX DRAWINGS DOWN SINGLE AND RIGHT DOUBLE}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}': u'\N{BOX DRAWINGS UP DOUBLE AND LEFT SINGLE}',
        },
        4: {
            u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT HORIZONTAL}',
            u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}': u'\N{BOX DRAWINGS UP DOUBLE AND HORIZONTAL SINGLE}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS UP DOUBLE AND HORIZONTAL SINGLE}',
            u'\N{BOX DRAWINGS DOUBLE HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT HORIZONTAL}',
        },
        9: {
            u'\N{BOX DRAWINGS DOUBLE UP AND RIGHT}': u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}',
            u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}': u'\N{BOX DRAWINGS UP SINGLE AND LEFT DOUBLE}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}': u'\N{BOX DRAWINGS DOWN DOUBLE AND RIGHT SINGLE}',
        },
        10: {
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}': u'\N{BOX DRAWINGS VERTICAL SINGLE AND LEFT DOUBLE}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}': u'\N{BOX DRAWINGS LIGHT VERTICAL}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS VERTICAL SINGLE AND LEFT DOUBLE}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL}': u'\N{BOX DRAWINGS LIGHT VERTICAL}',
        },
        12: {
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}',
        },
        13: {
            u'\N{BOX DRAWINGS DOUBLE UP AND LEFT}': u'\N{BOX DRAWINGS LIGHT UP AND LEFT}',
            u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT UP AND LEFT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}': u'\N{BOX DRAWINGS LIGHT UP AND LEFT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT UP AND LEFT}',
        },
        27: {
            u'\N{BOX DRAWINGS DOUBLE UP AND LEFT}': u'\N{BOX DRAWINGS LIGHT UP AND LEFT}',
            u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}': u'\N{BOX DRAWINGS UP SINGLE AND RIGHT DOUBLE}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}': u'\N{BOX DRAWINGS DOWN DOUBLE AND LEFT SINGLE}',
        },
        28: {
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL}',
        },
        30: {
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}': u'\N{BOX DRAWINGS LIGHT VERTICAL}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}': u'\N{BOX DRAWINGS VERTICAL SINGLE AND RIGHT DOUBLE}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS VERTICAL SINGLE AND RIGHT DOUBLE}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL}': u'\N{BOX DRAWINGS LIGHT VERTICAL}',
        },
        31: {
            u'\N{BOX DRAWINGS DOUBLE UP AND RIGHT}': u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}',
            u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}': u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT UP AND RIGHT}',
        },
        36: {
            u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}': u'\N{BOX DRAWINGS DOWN DOUBLE AND HORIZONTAL SINGLE}',
            u'\N{BOX DRAWINGS DOUBLE UP AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT HORIZONTAL}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS DOWN DOUBLE AND HORIZONTAL SINGLE}',
            u'\N{BOX DRAWINGS DOUBLE HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT HORIZONTAL}',
        },
        37: {
            u'\N{BOX DRAWINGS DOUBLE DOWN AND LEFT}': u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}',
            u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND LEFT}': u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT DOWN AND LEFT}',
        },
        39: {
            u'\N{BOX DRAWINGS DOUBLE DOWN AND RIGHT}': u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}',
            u'\N{BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND RIGHT}': u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}',
            u'\N{BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL}': u'\N{BOX DRAWINGS LIGHT DOWN AND RIGHT}',
        },
        },
    }

def trit_fill_1(tritmap, row, col, traditional):
    if traditional and not row:
        raise row
    trit = tritmap[row][col]
    if trit == -1:
        raise trit
    elif trit == 1:
        return
    tritmap[row][col] = 1
    trit_fill_1(tritmap, (row - 1 + len(tritmap)) % len(tritmap), col, traditional)
    trit_fill_1(tritmap, (row + 1) % len(tritmap), col, traditional)
    trit_fill_1(tritmap, row, (col - 1 + len(tritmap[row])) % len(tritmap[row]), traditional)
    trit_fill_1(tritmap, row, (col + 1) % len(tritmap[row]), traditional)

def find_dots(dots, row, col):
    ch = dots[row][col]
    if ch in u'o.':
        return True
    elif ch in u' ':
        dots[row] = dots[row][:col] + 'X' + dots[row][(col + 1):]
        north = find_dots(dots, (row - 1 + len(dots)) % len(dots), col)
        south = find_dots(dots, (row + 1) % len(dots), col)
        east = find_dots(dots, row, (col + 1) % len(dots[row]))
        west = find_dots(dots, row, (col - 1 + len(dots[row])) % len(dots[row]))
        if north or south or east or west:
            dots[row] = dots[row][:col] + '.' + dots[row][(col + 1):]
            return True
    return False

def main(argv, envp, stdin, stdout, stderr):
    opts, args = getopt.gnu_getopt(argv[1:], '3d2fho:1svtm', ['both', 'debug', 'double', 'fast', 'help', 'output=', 'single', 'slow', 'verbose', 'traditional', 'modern'])
    assert len(args) < 2
    outfn = '-'
    debug_dots = False
    debug_tritmap = False
    use_fill = True
    thickness = 3
    traditional = True
    for k, v in opts:
        if k in ('-3', '--both'):
            thickness = 3
        elif k in ('-d', '--debug'):
            debug_tritmap = True
        elif k in ('-2', '--double'):
            thickness = 2
        elif k in ('-f', '--fast'):
            use_fill = False
        elif k in ('-t', '--traditional'):
            traditional = True
        elif k in ('-m', '--modern'):
            traditional = False
        elif k in ('-h', '--help'):
            print >> stdout, 'Usage: %(progname)s [ options ... ] [ -- ] [ INFILE ]' % {
                'progname': argv[0]
                }
            print >> stdout, 'Options:'
            print >> stdout, '-3, --both      Use both single- and double-thickness lines (default)'
            print >> stdout, '-d, --debug     Write the tritmap to stderr'
            print >> stdout, '-2, --double    Use double-thickness lines exclusively'
            print >> stdout, '-f, --fast      Skip the expensive filling heuristic'
            print >> stdout, '-h, --help      Show this message and exit'
            print >> stdout, '-oFILE,'
            print >> stdout, '--output=FILE   Send output to FILE (default: stdout)'
            print >> stdout, '-1, --single    Use single-thickness lines exclusively'
            print >> stdout, '-s, --slow      Use the expensive filling heuristic (default)'
            print >> stdout, '-t, --traditional Use double outer maze walls'
            print >> stdout, '-m, --modern    Use single outer maze walls'
            print >> stdout, '-v, --verbose   Write the dotmap to stderr'
            print >> stdout
            print >> stdout, 'Read a plain-ASCII MyMan maze from stdin or a specified INFILE,'
            print >> stdout, 'convert ASCII-art to Unicode, and write the result to stdout.'
            return
        elif k in ('-o', '--output'):
            outfn = v
        elif k in ('-1', '--single'):
            thickness = 1
        elif k in ('-s', '--slow'):
            use_fill = True
        elif k in ('-v', '--verbose'):
            debug_dots = True
    if not args:
        infn = '-'
    else:
        infn = args[0]
    if infn == '-':
        infile = sys.stdin
    else:
        infile = file(infn, 'rb')
    if outfn == '-':
        outfile = sys.stdout
    else:
        outfile = file(outfn, 'wb')
    line = infile.readline().decode('utf-8').rstrip(u'\r\n').lstrip(u'\ufeff')
    while ("".join(line.split(u"\\\\")))[-1:] == u"\\":
        line = line + u'\r\n' + infile.readline().decode('utf-8').rstrip(u'\r\n')
    n, wxh = line.split(' ',1)
    opts = ''
    flags = '0'
    if len(wxh.split()) > 1: wxh, opts = wxh.split(None, 1)
    if '~' in wxh: wxh, flags = wxh.split('~',1)
    flags = int(flags, 0)
    w, h = wxh.split('x', 1)
    n, w, h = int(n), int(w), int(h)
    outfile.write(((u'\ufeff%s %sx%s') % (n, w, h)).encode('utf-8'))
    if flags: outfile.write((u'~%d' % flags).encode('utf-8'))
    if opts: outfile.write((u' %s' % opts).encode('utf-8'))
    outfile.write(u'\r\n'.encode('utf-8'))
    outfile.flush()
    for maze in xrange(n):
        rows = [ infile.readline().decode('utf-8').rstrip('\r\n') + u'X' for row in xrange(h) ] + [ u''.join(u'X' for col in xrange(w + 1)) ]
        dots = [ row for row in rows ]
        for row in xrange(0, h):
            ch1 = rows[row][w - 1]
            ch2 = rows[row][0]
            if (row in xrange(1, h - 1)) and (ch1 in u' ' and ch2 in u' '):
                northwest = rows[row - 1][w - 1] in u'+-~'
                northeast = rows[row - 1][0] in u'+-~'
                southwest = rows[row + 1][w - 1] in u'+-~'
                southeast = rows[row + 1][0] in u'+-~'
                if northeast and northwest and southeast and southwest:
                    if find_dots(dots, row, w - 1) and find_dots(dots, row, 0):
                        rows[row - 1] = rows[row - 1][:w] + u'-'
                        rows[row] = rows[row][:w] + u' '
                        rows[row + 1] = rows[row + 1][:w] + u'-'
            elif ch1 in u'-~' or ch2 in u'-~':
                rows[row] = rows[row][:w] + u'-'
        for col in xrange(0, w):
            ch1 = rows[h - 1][col]
            ch2 = rows[0][col]
            if (col in xrange(1, w - 1)) and (ch1 in u' ' and ch2 in u' '):
                northwest = rows[h - 1][col - 1] in u'+|l'
                northeast = rows[h - 1][col + 1] in u'+|l'
                southwest = rows[0][col - 1] in u'+|l'
                southeast = rows[0][col + 1] in u'+|l'
                if northeast and northwest and southeast and southwest:
                    if find_dots(dots, h - 1, col) and find_dots(dots, 0, col):
                        rows[h] = rows[h][:col][:-1] + u'| |' + rows[h][(col + 2):]
            elif ch1 in u'|l' or ch2 in u'|l':
                rows[h] = rows[h][:col] + u'|' + rows[h][col + 1:]
        tritmap = []
        orows = []
        if debug_dots:
            stderr.write((u'\r\n'.join(dots) + u'\r\n').encode('utf-8'))
            stderr.flush()
            stderr.write((u'\r\n'.join(rows) + u'\r\n').encode('utf-8'))
            stderr.flush()
        for row in xrange(h + 1):
            orow = u''
            line1 = []
            line2 = []
            line3 = []
            for col in xrange(w + 1):
                ch = rows[row][col]
                trits = [ [ -1, -1, -1 ], [ -1, -1, -1 ], [ -1, -1, -1 ] ]
                if ch in u'+':
                    north = rows[(row - 1 + len(rows)) % len(rows)][col] in u'|l+'
                    south = rows[(row + 1) % len(rows)][col] in u'|l+'
                    east = rows[row][(col + 1) % len(rows[row])] in u'-~=+'
                    west = rows[row][(col - 1 + len(rows[row])) % len(rows[row])] in u'-~=+'
                    trits = [ [ 0, north, 0 ], [ west, 1, east ], [ 0, south, 0 ] ]
                    ch = boxmap[thickness][8 * north
                                +
                                4 * south
                                +
                                2 * east
                                +
                                west]
                elif ch in u' ' or (ch >= u'A' and ch <= u'Z'):
                    trits = [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ]
                elif ch in u'-':
                    north = rows[(row - 1 + len(rows)) % len(rows)][col] in u'|l'
                    south = rows[(row + 1) % len(rows)][col] in u'|l'
                    ch = boxmap[thickness][16 + 2 * north + south]
                    trits = [ [ 0, north, 0 ], [ 1, 1, 1 ], [ 0, south, 0 ] ]
                elif ch in u'~':
                    north = rows[(row - 1 + len(rows)) % len(rows)][col] in u'|l'
                    south = rows[(row + 1) % len(rows)][col] in u'|l'
                    trits = [ [ 0, north, 0 ], [ 1, 1, 1 ], [ 0, south, 0 ] ]
                elif ch in u'|':
                    east = rows[row][(col + 1) % len(rows[row])] in u'-~='
                    west = rows[row][(col - 1 + len(rows[row])) % len(rows[row])] in u'-~='
                    ch = boxmap[thickness][20 + 2 * east + west]
                    trits = [ [ 0, 1, 0 ], [ west, 1, east ], [ 0, 1, 0 ] ]
                elif ch in u'l':
                    east = rows[row][(col + 1) % len(rows[row])] in u'-~='
                    west = rows[row][(col - 1 + len(rows[row])) % len(rows[row])] in u'-~='
                    trits = [ [ 0, 1, 0 ], [ west, 1, east ], [ 0, 1, 0 ] ]
                elif ch in u'=':
                    ch = u'\u2261'
                elif ch in u':':
                    ch = u'\u00A0'
                elif ch in u'.':
                    ch = u'\u00B7'
                elif ch in u'o':
                    ch = u'\u25a0'
                elif ch in u'\0':
                    ch = u'\u2008'
                elif ch in u'\7':
                    ch = u'\u2022'
                elif ch in u'<':
                    ch = u'\u00AB'
                elif ch in u'>':
                    ch = u'\u00BB'
                orow += ch
                line1 += trits[0]
                line2 += trits[1]
                line3 += trits[2]
            tritmap += [ line1, line2, line3 ]
            orows.append(orow)
        if use_fill:
            for row in xrange(len(tritmap)):
                line = tritmap[row]
                for col in xrange(len(line)):
                    ntritmap = [ [ trit for trit in line ] for line in tritmap ]
                    try:
                        trit_fill_1(ntritmap, row, col, traditional)
                        tritmap = ntritmap
                    except:
                        pass
        if debug_tritmap:
            for line in tritmap:
                for trit in line:
                    ch = { -1: '.', 0: ' ', 1: '#' }[trit]
                    stderr.write(ch.encode('utf-8'))
                stderr.write(u'\r\n'.encode('utf-8'))
                stderr.flush()
        for row in xrange(h):
            for col in xrange(w):
                ich = rows[row][col]
                ch = orows[row][col]
                code = 27 * tritmap[row * 3][col * 3] + 9 * tritmap[row * 3][col * 3 + 2] + 3 * tritmap[row * 3 + 2][col * 3] + tritmap[row * 3 + 2][col * 3 + 2]
                ch = rewrite_ternary[thickness].get(code, {}).get(ch, ch)
                mask, och = fixup[thickness].get(ich, {}).get(ch, (0, ch))
                north = orows[(row - 1 + len(orows)) % len(orows)][col] in u'|l'
                south = orows[(row + 1) % len(orows)][col] in u'|l'
                east = orows[row][(col + 1) % len(orows[row])] in u'-~='
                west = orows[row][(col - 1 + len(orows[row])) % len(orows[row])] in u'-~='
                if (mask & (8 * north + 4 * south + 2 * east + west)) == mask:
                    ch = och
                outfile.write(ch.encode('utf-8'))
            outfile.write(u'\r\n'.encode('utf-8'))
        outfile.flush()
    outfile.close()
    infile.close()

if __name__ == '__main__':
    main(sys.argv, os.environ, sys.stdin, sys.stdout, sys.stderr)
