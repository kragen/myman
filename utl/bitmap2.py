#!/usr/bin/env python
# -*- coding: utf-8 -*-
#  bitmap2.py - bitmap tile compactor for the MyMan video game
#  Copyright 2007, Benjamin Sittler <bsittler@gmail.com>
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


import sys

BLACK = u'\N{FULL BLOCK}]\N{BLACK SQUARE}'

HALF = u'\N{LEFT HALF BLOCK}\N{RIGHT HALF BLOCK}\N{LOWER HALF BLOCK}\N{UPPER HALF BLOCK}'

def compact(a, b):
    better = lambda a, b: (a == u'\N{PUNCTUATION SPACE}') and b or (b == u'\N{PUNCTUATION SPACE}') and a or a.isspace() and b or a
    if a == u'\0': a = u'\N{PUNCTUATION SPACE}'
    if b == u'\0': b = u'\N{PUNCTUATION SPACE}'
    if a != b or a not in u'\N{LEFT HALF BLOCK}\N{RIGHT HALF BLOCK}':
        if a in HALF and not b.isspace():
            a = u'\N{FULL BLOCK}'
            pass
        if b in HALF and not a.isspace():
            b = u'\N{FULL BLOCK}'
            pass
        pass
    if a == b:
        return a
    return {
        False:
        {
        False: better(a, b),
        True: u'\N{LOWER HALF BLOCK}'
        },
        True:
        {
        False: u'\N{UPPER HALF BLOCK}',
        True: better(a, b)
        }
        }[a in BLACK][b in BLACK]

last = ''

def combine(a, b):
    while len(a) < len(b):
        a += b[len(a)]
        pass
    while len(a) > len(b):
        b += a[len(b)]
        pass
    c = []
    for i in xrange(len(a)):
        c.append(compact(a[i], b[i]))
        pass
    return ''.join(c)

first = True

newline = False

for line in sys.stdin.readlines():
    line = line.decode('utf-8')
    newline = False
    if line:
        if line != line.rstrip(u'\r\n'):
            line = line.rstrip(u'\r\n')
            newline = True
            pass
        pass
    if first:
        first = False
        if line[:1] == u'\ufeff': line = line[1:]
        sys.stdout.write(u'\ufeff'.encode('utf-8'))
        sys.stdout.flush()
        try:
            flags = 0
            if '~' in line:
                wxh, flags = [ x for x in line.rstrip().split('~', 1) ]
                flags = int(flags, 0)
                line = wxh
                pass
            w, h = [ int(x) for x in line.rstrip().split('x') ]
            if flags:
                line = u'%sx%s~%s\n' % (w, (h + 1) // 2, flags)
                pass
            else:
                line = u'%sx%s\n' % (w, (h + 1) // 2)
                pass
            pass
        except:
            pass
        pass
    if not last and line[:1] == ':':
        last = line
        continue
    elif last:
        if line[:1] != ':':
            sys.stdout.write(last.encode('utf-8'))
            pass
        else:
            line = combine(last, line)
            pass
        pass
    if newline: line += u'\n'
    newline = False
    sys.stdout.write(line.encode('utf-8'))
    sys.stdout.flush()
    last = ''
    pass

if newline: last += u'\n'

if last:
    sys.stdout.write(last.encode('utf-8'))
    sys.stdout.flush()
    pass
