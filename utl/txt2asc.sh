#!/bin/bash --
# -*- coding: utf-8 -*-
#  asc2txt.py - bitmap tile and sprite to ASCII-art converter
#  Copyright 2008, Benjamin C. Wiley Sittler <bsittler@gmail.com>
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

# NOTE: this is an incomplete ASCII graphical approximation of the
# CP437 repertoire with a few modifications for maze file
# transformation

exec sed '
s/¡\|‼/!/g;
s/▀/"/g;
s/█\|▌\|▐\|░\|▒/#/g;
s/¼\|½/%/g;
s/○\|☺\|☻\|♥\|♦\|♣\|♠\|°\|☼/*/g;
s/├\|┤\|┬\|•\|┼\|┌\|┐\|└\|┴\|┘\|╞\|╡\|╒\|╤\|╕\|╘\|╧\|╛\|╪\|╟\|╢\|╓\|╥\|╖\|╙\|╨\|╜\|╫\|╠\|╣\|╔\|╦\|╗\|╚\|╩\|╝\|╬\|±\|÷/+/g;
s/▄/,/g;
s/─\|⌐\|═\|¬/-/g;
s/·/./g;
s/Φ/0/g;
s/²/2/g;
s/≤\|«/</g;
s/≈\|≡/=/g;
s/≥\|»/>/g;
s/¿/?/g;
s/Å\|Ä\|∩\|♫/A/g;
s/Θ\|ß/B/g;
s/Ç/C/g;
s/⌂/D/g;
s/Æ\|É\|£\|Σ/E/g;
s/⌡/J/g;
s/Ñ/N/g;
s/Ö/O/g;
s/₧/P/g;
s/Ω/Q/g;
s/§/S/g;
s/Γ/T/g;
s/Ü/U/g;
s/¥/Y/g;
s/á\|å\|à\|ä\|â\|ª\|α/a/g;
s/¢\|ç/c/g;
s/δ/d/g;
s/æ\|è\|ë\|ê\|é\|ε/e/g;
s/ƒ\|⌠/f/g;
s/í\|ì\|î\|ï/i/g;
s/π\|ñ\|ⁿ/n/g;
s/σ\|◘\|■\|ó\|ò\|ö\|ô\|º/o/g;
s/τ/t/g;
s/ú\|ù\|û\|ü/u/g;
s/√/v/g;
s/∞/w/g;
s/φ\|µ\|ÿ/y/g;
s/│\|║/|/g;
' "$@"
exit $?
