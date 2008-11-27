# Makefile - build script for the MyMan video game
# Copyright 1997-2008, Benjamin C. Wiley Sittler <bsittler@gmail.com>
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


# See README and INSTALL for more information.

ifeq ($(origin _SHELL),undefined)
SHELL = /bin/sh
else
SHELL = ${_SHELL}
endif

# NOTE: you will need a fairly modern version of GNU Make (or a
# compatible program, if such a thing exists) to run this Makefile;
# the following version is known to work:
gnumake_ok_tested_version := GNU Make 3.80

gnumake_too_old_get_3_80 = $(if true,true,${gnumake_too_old_get_3_80})

${START_STRING_QUOTING_LIBRARY}


## supported characters (tab + newline + printable ASCII)

# these constant strings are used during string escaping; the
# quotation mark, apostrophe and grave accent are each repeated twice
# to fool broken syntax-highlighting editors (emacs makefile-mode in
# particular)

# NOTE: char_tab and char_newline are special -- to overcome
# shortcomings in Makefile syntax, they must be exported through the
# environment to sub-processes, and their values must be set in all
# generated shell scripts

define char_tab
	:
endef
define char_newline
	:
	:
endef
char_newline := $(subst ${char_tab},,${char_newline})
char_tab := $(subst :,,${char_tab})
export char_tab
export char_newline
nil :=
char_space := ${nil} ${nil}
char_dollar_sign := $$
char_reverse_solidus := \${nil}
define char_number_sign
#
endef
char_quotation_mark := "
char_quotation_mark := "
char_apostrophe := '
char_apostrophe := '
char_grave_accent := `
char_grave_accent := `
char_exclamation_mark := !
char_percent_sign := %
char_ampersand := &
char_left_parenthesis := (
char_right_parenthesis := )
char_asterisk := *
char_plus_sign := +
char_comma := ,
char_hyphen_minus := -
char_full_stop := .
char_solidus := /
char_digit_zero := 0
char_digit_one := 1
char_digit_two := 2
char_digit_three := 3
char_digit_four := 4
char_digit_five := 5
char_digit_six := 6
char_digit_seven := 7
char_digit_eight := 8
char_digit_nine := 9
char_colon := :
char_semicolon := ;
char_less_than_sign := <
char_equals_sign := =
char_greater_than_sign := >
char_question_mark := ?
char_commercial_at := @
char_latin_capital_letter_a := A
char_latin_capital_letter_b := B
char_latin_capital_letter_c := C
char_latin_capital_letter_d := D
char_latin_capital_letter_e := E
char_latin_capital_letter_f := F
char_latin_capital_letter_g := G
char_latin_capital_letter_h := H
char_latin_capital_letter_i := I
char_latin_capital_letter_j := J
char_latin_capital_letter_k := K
char_latin_capital_letter_l := L
char_latin_capital_letter_m := M
char_latin_capital_letter_n := N
char_latin_capital_letter_o := O
char_latin_capital_letter_p := P
char_latin_capital_letter_q := Q
char_latin_capital_letter_r := R
char_latin_capital_letter_s := S
char_latin_capital_letter_t := T
char_latin_capital_letter_u := U
char_latin_capital_letter_v := V
char_latin_capital_letter_w := W
char_latin_capital_letter_x := X
char_latin_capital_letter_y := Y
char_latin_capital_letter_z := Z
char_left_square_bracket := [
char_right_square_bracket := ]
char_circumflex_accent := ^
char_low_line := _
char_latin_small_letter_a := a
char_latin_small_letter_b := b
char_latin_small_letter_c := c
char_latin_small_letter_d := d
char_latin_small_letter_e := e
char_latin_small_letter_f := f
char_latin_small_letter_g := g
char_latin_small_letter_h := h
char_latin_small_letter_i := i
char_latin_small_letter_j := j
char_latin_small_letter_k := k
char_latin_small_letter_l := l
char_latin_small_letter_m := m
char_latin_small_letter_n := n
char_latin_small_letter_o := o
char_latin_small_letter_p := p
char_latin_small_letter_q := q
char_latin_small_letter_r := r
char_latin_small_letter_s := s
char_latin_small_letter_t := t
char_latin_small_letter_u := u
char_latin_small_letter_v := v
char_latin_small_letter_w := w
char_latin_small_letter_x := x
char_latin_small_letter_y := y
char_latin_small_letter_z := z
char_left_curly_bracket := {
char_vertical_line := |
char_right_curly_bracket := }
char_tilde := ~


## character classes

charclass_ascii_space := \
char_tab \
char_newline \
char_space

charclass_ascii_punct := \
char_exclamation_mark \
char_quotation_mark \
char_number_sign \
char_dollar_sign \
char_percent_sign \
char_ampersand \
char_apostrophe \
char_left_parenthesis \
char_right_parenthesis \
char_asterisk \
char_plus_sign \
char_comma \
char_hyphen_minus \
char_full_stop \
char_solidus \
char_colon \
char_semicolon \
char_less_than_sign \
char_equals_sign \
char_greater_than_sign \
char_question_mark \
char_commercial_at \
char_left_square_bracket \
char_reverse_solidus \
char_right_square_bracket \
char_circumflex_accent \
char_low_line \
char_grave_accent \
char_left_curly_bracket \
char_vertical_line \
char_right_curly_bracket \
char_tilde

charclass_ascii_digit := \
char_digit_zero \
char_digit_one \
char_digit_two \
char_digit_three \
char_digit_four \
char_digit_five \
char_digit_six \
char_digit_seven \
char_digit_eight \
char_digit_nine

charclass_ascii_upper := \
char_latin_capital_letter_a \
char_latin_capital_letter_b \
char_latin_capital_letter_c \
char_latin_capital_letter_d \
char_latin_capital_letter_e \
char_latin_capital_letter_f \
char_latin_capital_letter_g \
char_latin_capital_letter_h \
char_latin_capital_letter_i \
char_latin_capital_letter_j \
char_latin_capital_letter_k \
char_latin_capital_letter_l \
char_latin_capital_letter_m \
char_latin_capital_letter_n \
char_latin_capital_letter_o \
char_latin_capital_letter_p \
char_latin_capital_letter_q \
char_latin_capital_letter_r \
char_latin_capital_letter_s \
char_latin_capital_letter_t \
char_latin_capital_letter_u \
char_latin_capital_letter_v \
char_latin_capital_letter_w \
char_latin_capital_letter_x \
char_latin_capital_letter_y \
char_latin_capital_letter_z

charclass_ascii_lower := \
char_latin_small_letter_a \
char_latin_small_letter_b \
char_latin_small_letter_c \
char_latin_small_letter_d \
char_latin_small_letter_e \
char_latin_small_letter_f \
char_latin_small_letter_g \
char_latin_small_letter_h \
char_latin_small_letter_i \
char_latin_small_letter_j \
char_latin_small_letter_k \
char_latin_small_letter_l \
char_latin_small_letter_m \
char_latin_small_letter_n \
char_latin_small_letter_o \
char_latin_small_letter_p \
char_latin_small_letter_q \
char_latin_small_letter_r \
char_latin_small_letter_s \
char_latin_small_letter_t \
char_latin_small_letter_u \
char_latin_small_letter_v \
char_latin_small_letter_w \
char_latin_small_letter_x \
char_latin_small_letter_y \
char_latin_small_letter_z

charclass_all := \
${charclass_ascii_space} \
${charclass_ascii_punct} \
${charclass_ascii_digit} \
${charclass_ascii_upper} \
${charclass_ascii_lower}

charclass_ascii_id := \
${charclass_ascii_lower} \
${charclass_ascii_upper} \
${charclass_ascii_digit} \
char_low_line


## case-folding functions

# ucase: convert lower case -> upper case
$(eval \
ucase ${char_equals_sign} $(subst ${char_comma}${char_space},${char_comma},$(foreach charname,$(strip ${charclass_ascii_lower}),$$${char_left_parenthesis}subst $$(value ${charname})${char_comma}$$(value $(subst char_latin_small_letter_,char_latin_capital_letter_,${charname}))${char_comma}))${char_dollar_sign}1$(subst ${char_space},,$(patsubst %,${char_right_parenthesis},$(strip ${charclass_ascii_lower}))))


## string quoting functions

charclass_q_safe := \
char_newline \
char_hyphen_minus \
char_equals_sign \
char_plus_sign \
char_full_stop \
char_comma \
char_colon \
char_solidus \
${charclass_ascii_id}

# characters that need simple backslashing for the shell
charclass_q_unsafe := \
$(filter-out char_reverse_solidus ${charclass_q_safe},${charclass_all}) \
char_reverse_solidus

# q: re-quote $1 for interpretation as a shell word
$(eval \
q ${char_equals_sign} ${char_dollar_sign}${char_left_parenthesis}subst ${char_dollar_sign}{char_newline},"${char_dollar_sign}${char_dollar_sign}{char_newline}",$(subst ${char_comma}${char_space},${char_comma},$(foreach charname,$(strip ${charclass_q_unsafe}),$$${char_left_parenthesis}subst $$(value ${charname})${char_comma}$${char_reverse_solidus}$$(value ${charname})${char_comma}))${char_dollar_sign}1$(subst ${char_space},,$(patsubst %,${char_right_parenthesis},$(strip ${charclass_q_unsafe})))${char_right_parenthesis})

# characters that need simple backslashing for the C compiler (we pass
# apostrophe unmodified at the moment; this works fine for
# double-quoted string initializers)
charclass_cq_unsafe := \
char_question_mark \
char_quotation_mark \
char_reverse_solidus

# other C quoting we handle:
#     <TAB> -> \t
#     <NEWLINE> -> \n

# cq: re-quote $1 for inclusion in a literal C string
$(eval \
cq ${char_equals_sign} ${char_dollar_sign}${char_left_parenthesis}subst ${char_dollar_sign}{char_tab}${char_comma}${char_reverse_solidus}t${char_comma}${char_dollar_sign}${char_left_parenthesis}subst ${char_dollar_sign}{char_newline}${char_comma}${char_reverse_solidus}n${char_comma}$(subst ${char_comma}${char_space},${char_comma},$(foreach charname,$(strip ${charclass_cq_unsafe}),$$${char_left_parenthesis}subst $$(value ${charname})${char_comma}$${char_reverse_solidus}$$(value ${charname})${char_comma}))${char_dollar_sign}1$(subst ${char_space},,$(patsubst %,${char_right_parenthesis},$(strip ${charclass_cq_unsafe})))${char_right_parenthesis}${char_right_parenthesis})

# ccq: re-quote $1 for inclusion in a C or C++ comment (the /* ... */ syntax or the // ... syntax )

# comment delimiters
#     */ -> *//*
#     <NEWLINE> -> <NEWLINE>//*/<SPACE>/*
ccq = $(subst ${char_newline},${char_newline}${char_solidus}${char_solidus}${char_asterisk}${char_solidus}${char_space}${char_solidus}${char_asterisk},$(subst ${char_asterisk}${char_solidus},${char_asterisk}${char_solidus}${char_solidus}${char_asterisk},$1))

# mw: quoting for treatment as a make target or dependency

# very bizarre backslashing rules
#     \\<SPACE> -> error
#     \\<TAB> -> error
#     \\| -> error
#     <SPACE> -> \<SPACE>
#     <TAB> -> \<TAB>
#     | -> \|
#     : -> error

# NOTE: the actual handling of reverse solidus (\) and colon (:) is
# too complicated and silly to go into; basically i regard the GNU
# Make handling of these characters to be a bug

# FIXME: once the GNU Make developers decide on a reasonable solution
# to this problem, mw will need to be rewritten

mw = $(if $(findstring ${char_reverse_solidus}${char_reverse_solidus}${char_space},$(subst ${char_tab},${char_space},$(subst ${char_vertical_line},${char_space},$1))),$(error two reverse solidus characters (${char_reverse_solidus}${char_reverse_solidus}) followed by a space character (${char_space})${char_comma} a tab character (${char_tab})${char_comma} or a vertical line character (${char_vertical_line}) are not yet supported in GNU make targets or dependencies; such a pattern occurs at least once in the string ${char_grave_accent}$1${char_apostrophe}),$(if $(findstring ${char_colon},$1)$(findstring ${char_semicolon},$1)$(findstring ${char_dollar_sign}${char_commercial_at},$1),$(error the colon character (${char_colon}), semicolon character (${char_semicolon}) and dollar sign-commercial at character sequence (${char_dollar_sign}${char_commercial_at}) are not yet supported in GNU Make targets or dependencies; such a character or sequence occurs at least once in the string ${char_grave_accent}$1${char_apostrophe}),$(subst ${char_reverse_solidus}_open_reverse_solidus,${char_reverse_solidus},$(subst ${char_reverse_solidus}_close_reverse_solidus,,$(subst ${char_reverse_solidus}_close_reverse_solidus${char_reverse_solidus},${char_reverse_solidus}${char_reverse_solidus},$(subst ${char_reverse_solidus}_close_reverse_solidus${char_reverse_solidus}_open_reverse_solidus,${char_reverse_solidus}_open_reverse_solidus,$(subst ${char_vertical_line},${char_reverse_solidus}${char_vertical_line},$(subst ${char_colon},${char_reverse_solidus}${char_colon},$(subst ${char_tab},${char_reverse_solidus}${char_tab},$(subst ${char_space},${char_reverse_solidus}${char_space},$(subst ${char_reverse_solidus},${char_reverse_solidus}_open_reverse_solidus${char_reverse_solidus}_close_reverse_solidus,$1)))))))))))

# mq: quote dollar signs (useful when passing make variable
# settings through the command-line to a recursive invocation of make)
mq = $(subst ${char_dollar_sign},${char_dollar_sign}${char_dollar_sign},$1)

# shell: run shell commands with arguments possibly containing
# strings quoted by $(call q,...)
shell = $(shell char_newline=`echo ;echo :`; char_newline="$${char_newline%:}"; export char_newline; ${SHELL} -c $(call q,$1))

# xq(xu): primitive (un-)quoting for s; this
# allows us to use built-in functions expecting space-separated lists
# when we have single-item lists possibly containing embedded spaces,
# and allows us to treat solidus (/) as a normal word character in
# s patterns and inputs.

#     _ <-> __
#     / <-> _|
#     <SPACE> <-> _-
#     <TAB> <-> _=
#     % <-> _@

xq = $(subst %,_@,$(subst ${char_tab},_=,$(subst ${char_space},_-,$(subst ${char_solidus},_|,$(subst _,__,$1)))))
xu = $(subst __.,_,$(subst _|,${char_solidus},$(subst _-,${char_space},$(subst _=,${char_tab},$(subst _@,%,$(subst __,__.,$1))))))

# s: like patsubst, but makes only a single replacement; $1 is source
# pattern (treated as a single word,) $2 is replacement pattern
# (treated as a single word,) and $3 is the string to act upon (also
# treated as a single word); note that you need to quote the non-%
# parts of $1 and $2 using $(call xq,...)
s = $(call xu,$(patsubst $1,$2,$(call xq,$3)))

# qq: composition of q and q
qq = $(call q,$(call q,$1))

# qs: composition of q and s
qs = $(call q,$(call s,$1,$2,$3))

# qcq: composition of q and cq
qcq = $(call q,$(call cq,$1))

# qmq: composition of q and mq
qmq = $(call q,$(call mq,$1))

# qmqs: composition of qmq and s
qmqs = $(call qmq,$(call s,$1,$2,$3))

# qxq: composition of q and xq
qxq = $(call q,$(call xq,$1))

# qxqs: composition of qxq and s
qxqs = $(call qxq,$(call s,$1,$2,$3))

# qsxu: qs with $(call xu,$3) for $3
qsxu = $(call qs,$1,$2,$(call xu,$3))

# mwxq: composition of mw and xq
mwxq = $(call mw,$(call xq,$1))

# idx: index of $1 in $2
lastword = $(word $(words $1),$1)
idx = $(if $1,$(strip $(if $2,$(call idx,$1,$(filter-out $(call lastword,$2),$2)) $(if $(subst $(call lastword,$2),,$1),,$(words $2)))))

${END_STRING_QUOTING_LIBRARY}


# source directory for package
ifeq ($(subst default,undefined,$(origin srcdir)),undefined)
srcdir = .
endif

# source file directory prefix (i.e. with the trailing slash)
ifeq ($(subst default,undefined,$(origin src)),undefined)
ifeq (${srcdir},.)
src =
else
src = ${srcdir}/
endif
endif

# object file directory for package
ifeq ($(subst default,undefined,$(origin objdir)),undefined)
objdir = obj
endif

# object file directory prefix (i.e. with the trailing slash)
ifeq ($(subst default,undefined,$(origin obj)),undefined)
ifeq (${objdir},.)
obj =
else
obj = ${objdir}/
endif
endif

# build date
ifeq ($(origin date),undefined)
date := $(call shell,${SHELL} $(call q,${src})configure --dump=date)
endif

# build date in ISO 8601 format
ifeq ($(origin isodate),undefined)
isodate := $(call shell,${SHELL} $(call q,${src})configure --dump=isodate)
endif

## build: type of system to build the program on
ifeq ($(origin build),undefined)
build := $(call shell,${SHELL} $(call q,${src})configure --dump=build)
endif

## host: type of system to build the program for

# this must contain only characters that will be interpreted literally
# by both ${MAKE} and ${SHELL} in the following contexts: bare word,
# single-quoted string, double-quoted string, globbing pattern, suffix
# pattern
ifeq ($(origin host),undefined)
host := $(call shell,${SHELL} $(call q,${src})configure --dump=host)
endif

ifneq (${host},${build})
ifeq ($(subst default,undefined,$(origin hostprefix)),undefined)
hostprefix = ${host}-
endif
else
ifeq ($(subst default,undefined,$(origin hostprefix)),undefined)
hostprefix = 
endif
endif

## MYMAN: base name for package (without version number or suffix)

# this must be a simple identifier (an ascii letter or ascii low line
# [_] followed by zero or more of: ascii letter, ascii digit or ascii
# low line)
ifeq ($(subst default,undefined,$(origin MYMAN)),undefined)
MYMAN = myman
endif

# CREATOR: four-character Macintosh cretor code
ifeq ($(subst default,undefined,$(origin CREATOR)),undefined)
CREATOR = MyMa
endif

# TYPE: four-character Macintosh type code
ifeq ($(subst default,undefined,$(origin TYPE)),undefined)
TYPE = APPL
endif

# base names for other executables
ifeq ($(subst default,undefined,$(origin XMYMAN)),undefined)
XMYMAN = x${MYMAN}
endif
ifeq ($(subst default,undefined,$(origin XMYMAN2)),undefined)
XMYMAN2 = ${XMYMAN}2
endif
ifeq ($(subst default,undefined,$(origin XMYMAN3)),undefined)
XMYMAN3 = ${XMYMAN}3
endif
ifeq ($(subst default,undefined,$(origin XMYMAN4)),undefined)
XMYMAN4 = ${XMYMAN}4
endif
ifeq ($(subst default,undefined,$(origin XBIGMAN)),undefined)
XBIGMAN = xbig$(call s,$(call xq,my)%,%,${MYMAN})
endif
ifeq ($(subst default,undefined,$(origin XHUGEMAN)),undefined)
XHUGEMAN = xhuge$(call s,$(call xq,my)%,%,${MYMAN})
endif
ifeq ($(subst default,undefined,$(origin XBITMAN)),undefined)
XBITMAN = xbit$(call s,$(call xq,my)%,%,${MYMAN})
endif
ifeq ($(subst default,undefined,$(origin XBITMAN2)),undefined)
XBITMAN2 = xbit$(call s,$(call xq,my)%,%,${MYMAN})2
endif
ifeq ($(subst default,undefined,$(origin XBITMAN3)),undefined)
XBITMAN3 = xbit$(call s,$(call xq,my)%,%,${MYMAN})3
endif
ifeq ($(subst default,undefined,$(origin XBITMAN4)),undefined)
XBITMAN4 = xbit$(call s,$(call xq,my)%,%,${MYMAN})4
endif
ifeq ($(subst default,undefined,$(origin XBITMAN5)),undefined)
XBITMAN5 = xbit$(call s,$(call xq,my)%,%,${MYMAN})5
endif
ifeq ($(subst default,undefined,$(origin XBITMAN6)),undefined)
XBITMAN6 = xbit$(call s,$(call xq,my)%,%,${MYMAN})6
endif
ifeq ($(subst default,undefined,$(origin XQUACKMAN)),undefined)
XQUACKMAN = xquack$(call s,$(call xq,my)%,%,${MYMAN})
endif
ifeq ($(subst default,undefined,$(origin MYMANCOMMAND)),undefined)
MYMANCOMMAND = ${MYMAN}.command
endif
ifeq ($(subst default,undefined,$(origin MYMAN_CT)),undefined)
MYMAN_CT = ${MYMAN}.ct
endif

# EXTRAPROGS: extra programs to install into ${bindir}
ifeq ($(subst default,undefined,$(origin EXTRAPROGS)),undefined)
EXTRAPROGS =
endif

## MYMANVERSION: package version number

# NOTE: The following format must be adhered to exactly when windows
# icon and version information are enabled using with_win_icon=yes

# for maximum portability, this should be a sequence of three integers
# separated by '.', in the form MAJOR.MINOR.RELEASE, identifying the
# major version, minor version, and release build iteration, chosen
# such that successive versions sort correctly both numerically and in
# ASCII, and versions shall not be indistinguishable when any or all
# components are stripped of trailing zeros (this means 0.9 is
# followed by 0.91, for instance.) Conventionally, pre-release major
# versions are numbered starting at the previous major version number
# with a minor version number of 9 or above, starting with 9, and
# correctly sortable both in ASCII order and numerically (so the
# sequence leading up to 1.0 might be 0.9.x, 0.91.x, 0.92.x,
# ... 0.99.x, 0.991.x, 0.992.x, etc.) The same principle applies to
# the pre-releases leading up to a new minor version, so e.g. 0.7.0
# might be preceded by 0.6.9, 0.6.91, 0.6.92, ... 0.6.99, 0.6.991,
# 0.6.992, etc.

# this must contain only characters that will be interpreted literally
# by both ${MAKE} and ${SHELL} in the following contexts: bare word,
# single-quoted string, double-quoted string, globbing pattern, suffix
# pattern

# additionally, ascii hyphen-minus [-] is discouraged here -- at least
# for portable versions of MyMan -- since some manual-page processing
# tools (namely Solaris makewhatis) cannot parse a .TH line in a
# manual page with more than one ascii hyphen-minus
ifeq ($(origin MYMANVERSION),undefined)
MYMANVERSION := $(call shell,cat $(call q,${src})VERSION)
endif

## MYMANCOPYRIGHT: human-readable copyright information
ifeq ($(origin MYMANCOPYRIGHT),undefined)
MYMANCOPYRIGHT := $(call shell,cat $(call q,${src})COPYRIGHT)
endif

## DIST: full, versioned package name (without suffix)

# this must contain only characters that will be interpreted literally
# by both ${MAKE} and ${SHELL} in the following contexts: bare word,
# single-quoted string, double-quoted string, globbing pattern, suffix
# pattern
ifeq ($(subst default,undefined,$(origin DIST)),undefined)
DIST = ${MYMAN}-${MYMANVERSION}
endif

## BINDIST: binary package name (without suffix)

# this must contain only characters that will be interpreted literally
# by both ${MAKE} and ${SHELL} in the following contexts: bare word,
# single-quoted string, double-quoted string, globbing pattern, suffix
# pattern
ifeq ($(subst default,undefined,$(origin BINDIST)),undefined)
BINDIST = ${DIST}-${host}
endif

## CVSDUMP: CVS repository dump package name (without suffix)

ifeq ($(subst default,undefined,$(origin CVSDUMP)),undefined)
CVSDUMP = ${MYMAN}-cvs
endif

## CVSDIST: CVS source snapshot package name (without suffix)

ifeq ($(subst default,undefined,$(origin CVSDIST)),undefined)
CVSDIST = ${MYMAN}-wip
endif

## directories

# distroot: subdirectory inside package that corresponds to the root
# directory on the machine where the package is to be installed
ifeq ($(subst default,undefined,$(origin distroot)),undefined)
distroot = dist
endif

# DESTDIR: installed filename prefix string

# if set, DESTDIR is prefixed to all directory names during 'make
# install' (useful for creating installation snapshot directories)

# e.g. DESTDIR = ./${BINDIST}/${distroot}
ifeq ($(subst default,undefined,$(origin DESTDIR)),undefined)
DESTDIR = 
endif

# Common prefix for installation directories.
# NOTE: This directory must exist when you start the install.
ifeq ($(subst default,undefined,$(origin prefix)),undefined)
prefix = /usr/local
endif
ifeq ($(subst default,undefined,$(origin exec_prefix)),undefined)
exec_prefix = ${prefix}
endif
# Where to put the executable for the command 'myman'
ifeq ($(subst default,undefined,$(origin bindir)),undefined)
bindir = ${exec_prefix}/bin
endif
# Where to put the bundle for 'myman.app' (only used when building for Carbon)
ifeq ($(subst default,undefined,$(origin appdir)),undefined)
appdir = /Applications
endif
# Root directory for architecture-independent data files
ifeq ($(subst default,undefined,$(origin datarootdir)),undefined)
datarootdir = ${prefix}/share
endif
# Where to put the directory of architecture-independent data files
ifeq ($(subst default,undefined,$(origin datadir)),undefined)
datadir = ${datarootdir}
endif
# Where to put the plain-text documentation
ifeq ($(subst default,undefined,$(origin docdir)),undefined)
docdir = ${datarootdir}/doc
endif
# The top-level directory for installing the man pages for this package
ifeq ($(subst default,undefined,$(origin mandir)),undefined)
mandir = ${datarootdir}/man
endif
# The extension for installing man pages
ifeq ($(subst default,undefined,$(origin manext)),undefined)
manext = .1
endif

# The directory for installing section 6 man pages
ifeq ($(subst default,undefined,$(origin man6dir)),undefined)
man6dir = ${mandir}/man6
endif
# The file name extension for installed section 6 man pages
ifeq ($(subst default,undefined,$(origin man6ext)),undefined)
man6ext = $(subst 1,6,${manext})
endif

# Directory of maze, tile, sprite and graphics datafiles
ifeq ($(subst default,undefined,$(origin privatedatadir)),undefined)
privatedatadir = ${datadir}/${DIST}
endif
# Directory of maze datafiles
ifeq ($(subst default,undefined,$(origin mazedir)),undefined)
mazedir = ${privatedatadir}/lvl
endif
# Directory of tile datafiles
ifeq ($(subst default,undefined,$(origin tiledir)),undefined)
tiledir = ${privatedatadir}/chr
endif
# Directory of sprite datafiles
ifeq ($(subst default,undefined,$(origin spritedir)),undefined)
spritedir = ${privatedatadir}/spr
endif
# Directory of graphics datafiles
ifeq ($(subst default,undefined,$(origin gfxdir)),undefined)
gfxdir = ${privatedatadir}/gfx
endif
# Directory of documentation
ifeq ($(subst default,undefined,$(origin privatedocdir)),undefined)
privatedocdir = ${docdir}/${DIST}
endif

# variable names for shared installation directories
public_dir_vars = \
prefix \
exec_prefix \
bindir \
datadir \
docdir \
mandir \
man6dir

# variable names for private installation directories used only by
# this distribution of MyMan

${DIST}_dir_vars = \
privatedatadir \
privatedocdir

# variable names for private installation subdirectories used only by
# this distribution of MyMan
${DIST}_subdir_vars = \
mazedir \
tiledir \
spritedir \
gfxdir

# variable names for all installation directories
dir_vars = ${public_dir_vars} $(${DIST}_dir_vars) $(${DIST}_subdir_vars)

# How to find this makefile
ifeq ($(subst default,undefined,$(origin MAKEFILE)),undefined)
MAKEFILE = ${src}Makefile
endif

## MAKELOOP: loopback recursive ${MAKE}

# names of makefile variables to pass to recursive ${MAKE}; these
# variables are computed by external commands during interpretation of
# the makefile, so we'd like to avoid expensive and redundant
# re-computations
makeloop_vars = \
AALIBCFLAGS \
AALIBLIBS \
ALLEGROCFLAGS \
ALLEGROLIBS \
build \
CACACFLAGS \
CACALIBS \
date \
isodate \
DIST_EXE \
FLTK_CONFIG \
FLTKCFLAGS \
FLTKLIBS \
GGICFLAGS \
GGILIBS \
GTK_CONFIG \
GTKCFLAGS \
GTKLIBS \
host \
MYMAN_CT_EXE \
MYMAN_EXE \
MYMANCOMMAND_EXE \
MYMANCOPYRIGHT \
MYMANVERSION \
MYMANWEBSITE \
NCURSES_CONFIG \
NCURSESCFLAGS \
NCURSESLIBS \
NCURSESW_CONFIG \
NCURSESWCFLAGS \
NCURSESWLIBS \
PKG_CONFIG \
SDL_CONFIG \
SHELL \
with_dmg \
with_gtk2 \
with_fltk2 \
with_mac \
with_mac_icon \
with_win_icon \
with_zip \
XBIGMAN_EXE \
XBITMAN2_EXE \
XBITMAN3_EXE \
XBITMAN4_EXE \
XBITMAN5_EXE \
XBITMAN6_EXE \
XBITMAN_EXE \
XCURSES_CONFIG \
xcurses_includes \
xcurses_libraries \
XHUGEMAN_EXE \
XMYMAN2_EXE \
XMYMAN3_EXE \
XMYMAN4_EXE \
XMYMAN_EXE \
XQUACKMAN_EXE

# flags for loopback recursive ${MAKE}
ifeq ($(subst default,undefined,$(origin MAKELOOP)),undefined)
MAKELOOP = \
    -f $(call q,${MAKEFILE}) \
    --no-print-directory \
    $(foreach var,${makeloop_vars},${var}=$(call qmq,$(${var})))
endif

##
## Configuration for the included mygetopt distribution
##

# Where to find the mygetopt files relative to ${srcdir}
ifeq ($(subst default,undefined,$(origin MYGETOPTDIR)),undefined)
MYGETOPTDIR = mygetopt
endif

# MYGETOPT_H is the argument to #include in order to include the
# 'getopt.h' file from ${MYGETOPTDIR} from a source file inside the
# ${src}src directory. MYGETOPTCPPFLAGS includes preprocessor options
# needed for this arrangement and MYGETOPTCINCLUDES adds the
# needed include path entry or entries.
ifeq ($(subst default,undefined,$(origin MYGETOPTCPPFLAGS)),undefined)
MYGETOPTCPPFLAGS =
endif
ifeq ($(subst default,undefined,$(origin MYGETOPTCINCLUDES)),undefined)
MYGETOPTCINCLUDES = -I${src}mygetopt
endif
ifeq ($(subst default,undefined,$(origin MYGETOPT_H)),undefined)
MYGETOPT_H = ${char_quotation_mark}getopt.h${char_quotation_mark}
endif

##
## Toolchain Configuration
##

## Conventional filename suffices (build)

# Option 1: GNU, BSD, Unix, MiNT, etc.
ifeq ($(subst -pe-,--,$(subst -msys-,--,$(subst -os2-,--,$(subst -cygwin32-,--,$(subst -mingw32msvc-,--,$(subst -mingw32-,--,$(subst -cygwin-,--,$(subst -mingw-,--,$(subst -msdosdjgpp-,--,$(subst -msdos-,--,-${build}-)))))))))),-${build}-)
ifeq ($(subst default,undefined,$(origin BUILD_OBJ_SUFFIX)),undefined)
BUILD_OBJ_SUFFIX = .o
endif
ifeq ($(subst default,undefined,$(origin BUILD_LIB_SUFFIX)),undefined)
BUILD_LIB_SUFFIX = .a
endif
ifeq ($(subst default,undefined,$(origin BUILD_EXE_SUFFIX)),undefined)
ifeq ($(subst -mint-,--,-${build}-),-${build}-)
BUILD_EXE_SUFFIX = 
else
BUILD_EXE_SUFFIX = .prg
endif
endif
ifeq ($(subst default,undefined,$(origin BUILD_TMP_SUFFIX)),undefined)
BUILD_TMP_SUFFIX = .tmp
endif
ifeq ($(subst default,undefined,$(origin BUILD_HTM_SUFFIX)),undefined)
BUILD_HTM_SUFFIX = .html
endif
ifeq ($(subst default,undefined,$(origin BUILD_TXT_SUFFIX)),undefined)
BUILD_TXT_SUFFIX = .txt
endif
ifeq ($(subst default,undefined,$(origin BUILD_TAR_SUFFIX)),undefined)
BUILD_TAR_SUFFIX = .tar
endif
ifeq ($(subst default,undefined,$(origin BUILD_TGZ_SUFFIX)),undefined)
BUILD_TGZ_SUFFIX = .tar.gz
endif
ifeq ($(subst default,undefined,$(origin BUILD_SH_SUFFIX)),undefined)
BUILD_SH_SUFFIX = 
endif

else

# Option 2: Cygwin, EMX, DJGPP, etc. [hybrid between GNU and FreeDOS]
ifeq ($(subst default,undefined,$(origin BUILD_OBJ_SUFFIX)),undefined)
BUILD_OBJ_SUFFIX = .o
endif
ifeq ($(subst default,undefined,$(origin BUILD_LIB_SUFFIX)),undefined)
BUILD_LIB_SUFFIX = .a
endif
ifeq ($(subst default,undefined,$(origin BUILD_EXE_SUFFIX)),undefined)
BUILD_EXE_SUFFIX = .exe
endif
ifeq ($(subst default,undefined,$(origin BUILD_TMP_SUFFIX)),undefined)
BUILD_TMP_SUFFIX = .tmp
endif
ifeq ($(subst default,undefined,$(origin BUILD_HTM_SUFFIX)),undefined)
BUILD_HTM_SUFFIX = .html
endif
ifeq ($(subst default,undefined,$(origin BUILD_TXT_SUFFIX)),undefined)
BUILD_TXT_SUFFIX = .txt
endif
ifeq ($(subst default,undefined,$(origin BUILD_TAR_SUFFIX)),undefined)
BUILD_TAR_SUFFIX = .tar
endif
ifeq ($(subst default,undefined,$(origin BUILD_TGZ_SUFFIX)),undefined)
BUILD_TGZ_SUFFIX = .tar.gz
endif
ifeq ($(subst default,undefined,$(origin BUILD_SH_SUFFIX)),undefined)
BUILD_SH_SUFFIX = .sh
endif
endif

# Option 3: FreeDOS, OpenDOS, etc. [does anyone use them now?]
#BUILD_OBJ_SUFFIX=.obj
#BUILD_LIB_SUFFIX=.lib
#BUILD_EXE_SUFFIX=.exe
#BUILD_TMP_SUFFIX=.tmp
#BUILD_HTM_SUFFIX=.htm
#BUILD_TXT_SUFFIX=.txt
#BUILD_TAR_SUFFIX=.tar
#BUILD_TGZ_SUFFIX=.tgz
#BUILD_SH_SUFFIX=.sh

# Convenient abbreviated versions of the common suffices
ifeq ($(subst default,undefined,$(origin o)),undefined)
O = ${BUILD_OBJ_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin a)),undefined)
A = ${BUILD_LIB_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin x)),undefined)
X = ${BUILD_EXE_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin tmp)),undefined)
Tmp = ${BUILD_TMP_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin htm)),undefined)
Htm = ${BUILD_HTM_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin txt)),undefined)
Txt = ${BUILD_TXT_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin tar)),undefined)
Tar = ${BUILD_TAR_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin tgz)),undefined)
Tgz = ${BUILD_TGZ_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin sh)),undefined)
Sh = ${BUILD_SH_SUFFIX}
endif

## Conventional filename suffices (host)

# Option 1: GNU, BSD, Unix, MiNT, etc.
ifeq ($(subst -pe-,--,$(subst -msys-,--,$(subst -os2-,--,$(subst -cygwin32-,--,$(subst -mingw32msvc-,--,$(subst -mingw32-,--,$(subst -cygwin-,--,$(subst -mingw-,--,$(subst -msdosdjgpp-,--,$(subst -msdos-,--,-${host}-)))))))))),-${host}-)
ifeq ($(subst default,undefined,$(origin OBJ_SUFFIX)),undefined)
OBJ_SUFFIX = .o
endif
ifeq ($(subst default,undefined,$(origin LIB_SUFFIX)),undefined)
LIB_SUFFIX = .a
endif
ifeq ($(subst default,undefined,$(origin EXE_SUFFIX)),undefined)
ifeq ($(subst -mint-,--,-${host}-),-${host}-)
EXE_SUFFIX = 
else
EXE_SUFFIX = .prg
endif
endif
ifeq ($(subst default,undefined,$(origin TMP_SUFFIX)),undefined)
TMP_SUFFIX = .tmp
endif
ifeq ($(subst default,undefined,$(origin HTM_SUFFIX)),undefined)
HTM_SUFFIX = .html
endif
ifeq ($(subst default,undefined,$(origin TXT_SUFFIX)),undefined)
TXT_SUFFIX = .txt
endif
ifeq ($(subst default,undefined,$(origin TAR_SUFFIX)),undefined)
TAR_SUFFIX = .tar
endif
ifeq ($(subst default,undefined,$(origin TGZ_SUFFIX)),undefined)
TGZ_SUFFIX = .tar.gz
endif
ifeq ($(subst default,undefined,$(origin SH_SUFFIX)),undefined)
SH_SUFFIX = 
endif

else

# Option 2: Cygwin, EMX, DJGPP, etc. [hybrid between GNU and FreeDOS]
ifeq ($(subst default,undefined,$(origin OBJ_SUFFIX)),undefined)
OBJ_SUFFIX = .o
endif
ifeq ($(subst default,undefined,$(origin LIB_SUFFIX)),undefined)
LIB_SUFFIX = .a
endif
ifeq ($(subst default,undefined,$(origin EXE_SUFFIX)),undefined)
EXE_SUFFIX = .exe
endif
ifeq ($(subst default,undefined,$(origin TMP_SUFFIX)),undefined)
TMP_SUFFIX = .tmp
endif
ifeq ($(subst default,undefined,$(origin HTM_SUFFIX)),undefined)
HTM_SUFFIX = .html
endif
ifeq ($(subst default,undefined,$(origin TXT_SUFFIX)),undefined)
TXT_SUFFIX = .txt
endif
ifeq ($(subst default,undefined,$(origin TAR_SUFFIX)),undefined)
TAR_SUFFIX = .tar
endif
ifeq ($(subst default,undefined,$(origin TGZ_SUFFIX)),undefined)
TGZ_SUFFIX = .tar.gz
endif
ifeq ($(subst default,undefined,$(origin SH_SUFFIX)),undefined)
SH_SUFFIX = .sh
endif
endif

# Option 3: FreeDOS, OpenDOS, etc. [does anyone use them now?]
#OBJ_SUFFIX=.obj
#LIB_SUFFIX=.lib
#EXE_SUFFIX=.exe
#TMP_SUFFIX=.tmp
#HTM_SUFFIX=.htm
#TXT_SUFFIX=.txt
#TAR_SUFFIX=.tar
#TGZ_SUFFIX=.tgz
#SH_SUFFIX=.sh

# Convenient abbreviated versions of the common suffices
ifeq ($(subst default,undefined,$(origin o)),undefined)
o = ${OBJ_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin a)),undefined)
a = ${LIB_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin x)),undefined)
x = ${EXE_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin tmp)),undefined)
tmp = ${TMP_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin htm)),undefined)
htm = ${HTM_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin txt)),undefined)
txt = ${TXT_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin tar)),undefined)
tar = ${TAR_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin tgz)),undefined)
tgz = ${TGZ_SUFFIX}
endif
ifeq ($(subst default,undefined,$(origin sh)),undefined)
sh = ${SH_SUFFIX}
endif

# file removal
ifeq ($(subst default,undefined,$(origin RM)),undefined)
RM = rm
endif
ifeq ($(subst default,undefined,$(origin RMFLAGS)),undefined)
RMFLAGS = -f
endif
ifeq ($(subst default,undefined,$(origin REMOVE)),undefined)
REMOVE = ${RM} ${RMFLAGS} ${EXTRARMFLAGS}
endif

# remote file synchronization
ifeq ($(subst default,undefined,$(origin RSYNC)),undefined)
RSYNC = rsync
endif
ifeq ($(subst default,undefined,$(origin RSYNCFLAGS)),undefined)
RSYNCFLAGS = -vP
endif

# version control system
ifeq ($(subst default,undefined,$(origin CVS)),undefined)
CVS = cvs
endif

# SourceForge project name
ifeq ($(subst default,undefined,$(origin SFGROUP)),undefined)
SFGROUP = myman
endif
# SourceForge user name to use with rsync-over-ssh
ifeq ($(subst default,undefined,$(origin SFUSER)),undefined)
ifeq ($(subst default,undefined,$(origin USER)),undefined)
SFUSER := $(shell id -un)
else
SFUSER := ${USER}
endif
endif
ifeq ($(subst default,undefined,$(origin SFPREFIX)),undefined)
ifneq (${SFUSER},)
SFPREFIX = ${SFUSER},${SFGROUP}@
else
SFPREFIX =
endif
endif
ifeq ($(subst default,undefined,$(origin MYMANWEBSITE)),undefined)
MYMANWEBSITE = http://${SFGROUP}.sf.net/
endif
ifeq ($(subst default,undefined,$(origin MYMANWEBSITERSYNC)),undefined)
MYMANWEBSITERSYNC = ${SFPREFIX}web.sf.net:/home/groups/$(call shell,${SHELL} -c $(call q,${ECHOLINE_internal} $(call q,${SFGROUP}) | ${SED} ${SEDFLAGS} -e $(call q,1 s|^..|&/&|; 1 s|^.|&/&|)))
endif
ifeq ($(subst default,undefined,$(origin UPLOADSRSYNC)),undefined)
UPLOADSRSYNC = ${SFPREFIX}frs.sf.net:uploads/
endif
ifeq ($(subst default,undefined,$(origin UPLOADSWEBSITE)),undefined)
UPLOADSWEBSITE = http://sf.net/project/admin/newrelease.php?package_id=288220&group_id=236995
endif
ifeq ($(subst default,undefined,$(origin MYMANCVSRSYNC)),undefined)
MYMANCVSRSYNC = rsync://${SFGROUP}.cvs.sf.net/cvsroot/${SFGROUP}/*
endif

# function to generate removal command for file $1
uninstall_file = test ! -f $(call q,$1) -o -L $(call q,$1) || \
            (${ECHOLINEX} removing file $(call q,$1) && \
                 ${REMOVE} $(call q,$1))

# symbolic link creation
ifeq ($(subst default,undefined,$(origin LN)),undefined)
LN = ln
endif
ifeq ($(subst default,undefined,$(origin LNFLAGS)),undefined)
LNFLAGS =
endif
ifeq ($(subst default,undefined,$(origin SYMLINK)),undefined)
SYMLINK = ${LN} ${LNFLAGS} ${EXTRALNFLAGS} -s
endif

# utility to create a .zip distribution file
ifeq ($(subst default,undefined,$(origin ZIP)),undefined)
ZIP = zip
endif

# mac utility to create a disk image distribution file
ifeq ($(subst default,undefined,$(origin HDIUTIL)),undefined)
HDIUTIL = hdiutil
endif

# mac utilities for icon manipulation
ifeq ($(subst default,undefined,$(origin TIFFUTIL)),undefined)
TIFFUTIL = tiffutil
endif
ifeq ($(subst default,undefined,$(origin TIFF2ICNS)),undefined)
TIFF2ICNS = tiff2icns
endif
ifeq ($(subst default,undefined,$(origin OSASCRIPT)),undefined)
OSASCRIPT = osascript
endif
ifeq ($(subst default,undefined,$(origin SETFILE)),undefined)
SETFILE = /Developer/Tools/SetFile
endif
ifeq ($(subst default,undefined,$(origin REZ)),undefined)
REZ = /Developer/Tools/Rez
endif
ifeq ($(subst default,undefined,$(origin PROD_FINDER)),undefined)
PROD_FINDER = prod_finder() { dir=`dirname "$$1"`; base=`basename "$$1"`; cd "$$dir" && ${OSASCRIPT} -e $(call q,on run argv${char_newline}tell application "Finder" to update item 1 of argv as POSIX file${char_newline}end run) "`pwd`/$$base"; }; prod_finder
endif
ifeq ($(subst default,undefined,$(origin set_mac_icon)),undefined)
set_mac_icon = set_mac_icon() { \
        test -r $(call q,$1) && \
            ${ECHOLINE} $(call q,read 'icns' ${char_left_parenthesis}-16455${char_right_parenthesis} "$(call cq,$1)";) | \
            ${REZ} -o "$$1" && \
            ${SETFILE} -a C "$$1" && \
            ( ${PROD_FINDER} "$$1" ); }; set_mac_icon
endif

# Win32 BMP creation for icons
ifeq ($(subst default,undefined,$(origin ANYTOPNM)),undefined)
ANYTOPNM = anytopnm
endif
ifeq ($(subst default,undefined,$(origin PPMTOBMP)),undefined)
PPMTOBMP = ppmtobmp
endif
ifeq ($(subst default,undefined,$(origin CONVERT_TO_BMP)),undefined)
CONVERT_TO_BMP = ${ECHOLINEX} creating $(call q,$@) from $(call q,$<) && \
        ${ANYTOPNM} $< | ${PPMTOBMP} -windows > $@ || \
            ( \
                ${REMOVE} $(call q,$@) ; \
                exit 1 \
            )
endif
# Win32 ICO creation for icons
ifeq ($(subst default,undefined,$(origin PPMTOWINICON)),undefined)
PPMTOWINICON = ppmtowinicon
endif
# (Win32) resource compiler
ifeq ($(subst default,undefined,$(origin RC)),undefined)
RC = windres
endif
ifeq ($(subst default,undefined,$(origin HOSTRC)),undefined)
ifneq (${host},${build})
HOSTRC = ${hostprefix}windres
else
HOSTRC = ${RC}
endif
endif
ifeq ($(subst default,undefined,$(origin HOSTCOMPILE_RESOURCE)),undefined)
ifneq (,$(findstring rcc,${HOSTRC}))
HOSTCOMPILE_RESOURCE = ${ECHOLINEX} creating $(call q,$@) from $(call q,$<) && \
            ${HOSTRC} -r -o$(call q,$@) $(call q,$<)
hostbind_resource = ${ECHOLINEX} binding $(call q,$1) to $(call q,$@) && \
            ${HOSTRC} -o$(call q,$@) $(call q,$1) || \
            (${ECHOLINEX} removing file $(call q,$@) && \
                 ${REMOVE} $(call q,$@))
else
ifeq (,$(findstring wrc,${HOSTRC})$(findstring windres,${HOSTRC}))
HOSTCOMPILE_RESOURCE = ${ECHOLINEX} creating $(call q,$@) from $(call q,$<) && \
            ${HOSTRC} -r -fo$(call q,$@) $(call q,$<)
hostbind_resource = ${ECHOLINEX} binding $(call q,$1) to $(call q,$@) && \
            ${HOSTRC} -fe$(call q,$@) $(call q,$1) || \
            (${ECHOLINEX} removing file $(call q,$@) && \
                 ${REMOVE} $(call q,$@))
else
HOSTCOMPILE_RESOURCE = ${ECHOLINEX} creating $(call q,$@) from $(call q,$<) && \
            ${HOSTRC} -r $(call q,$<) $(call q,$@)
hostbind_resource = ${ECHOLINEX} binding $(call q,$1) to $(call q,$@) && \
            ${HOSTRC} $(call q,$1) $(call q,$@) || \
            (${ECHOLINEX} removing file $(call q,$@) && \
                 ${REMOVE} $(call q,$@))
endif
endif
endif

# directory removal (empty directories only!)
ifeq ($(subst default,undefined,$(origin RMDIR)),undefined)
RMDIR = rmdir
endif
ifeq ($(subst default,undefined,$(origin RMDIRFLAGS)),undefined)
RMDIRFLAGS = --ignore-fail-on-non-empty
endif
ifeq ($(subst default,undefined,$(origin REMOVE_DIR)),undefined)
REMOVE_DIR = ${RMDIR} ${RMDIRFLAGS} ${EXTRARMDIRFLAGS}
endif

# stream editor
ifeq ($(subst default,undefined,$(origin SED)),undefined)
SED = sed
endif
ifeq ($(subst default,undefined,$(origin SEDFLAGS)),undefined)
SEDFLAGS = 
endif

# backspace remover
ifeq ($(subst default,undefined,$(origin COL)),undefined)
COL = col
endif
ifeq ($(subst -darwin,-,-${build}-),-${build}-)
ifeq ($(subst default,undefined,$(origin COLFLAGS)),undefined)
COLFLAGS = -xbp
endif
ifeq ($(subst default,undefined,$(origin TROFFTXTDRIVER)),undefined)
TROFFTXTDRIVER = -Tutf8
endif
else
ifeq ($(subst default,undefined,$(origin COLFLAGS)),undefined)
COLFLAGS = -xb
endif
ifeq ($(subst default,undefined,$(origin TROFFTXTDRIVER)),undefined)
TROFFTXTDRIVER = -Tascii
endif
endif
ifeq ($(subst default,undefined,$(origin FILTER_BACKSPACES)),undefined)
FILTER_BACKSPACES = ${COL} ${COLFLAGS}
endif

# manpage formatter
ifeq ($(subst default,undefined,$(origin TROFF)),undefined)
TROFF = groff
endif
ifeq ($(subst default,undefined,$(origin TROFFFLAGS)),undefined)
TROFFFLAGS = -man
endif
export GROFF_NO_SGR
GROFF_NO_SGR:=t

ifeq ($(subst default,undefined,$(origin TROFFTXT)),undefined)
TROFFTXT = ${TROFF}
endif
ifeq ($(subst default,undefined,$(origin TROFFTXTFLAGS)),undefined)
TROFFTXTFLAGS = ${TROFFFLAGS} ${TROFFTXTDRIVER}
endif

ifeq ($(subst default,undefined,$(origin TROFFPS)),undefined)
TROFFPS = ${TROFF}
endif
ifeq ($(subst default,undefined,$(origin TROFFPSFLAGS)),undefined)
TROFFPSFLAGS = ${TROFFFLAGS} -Tps
endif

ifeq ($(subst default,undefined,$(origin TROFFDVI)),undefined)
TROFFDVI = ${TROFF}
endif
ifeq ($(subst default,undefined,$(origin TROFFDVIFLAGS)),undefined)
TROFFDVIFLAGS = ${TROFFFLAGS} -Tdvi
endif

ifeq ($(subst default,undefined,$(origin TROFFHTML)),undefined)
TROFFHTML = ${TROFF}
endif
ifeq ($(subst default,undefined,$(origin TROFFHTMLFLAGS)),undefined)
TROFFHTMLFLAGS = ${TROFFFLAGS} -Thtml
endif

ifeq ($(subst default,undefined,$(origin FORMAT_MANPAGE_TXT)),undefined)
FORMAT_MANPAGE_TXT = ${TROFFTXT} ${TROFFTXTFLAGS}
endif
ifeq ($(subst default,undefined,$(origin FORMAT_MANPAGE_PS)),undefined)
FORMAT_MANPAGE_PS = ${TROFFPS} ${TROFFPSFLAGS}
endif
ifeq ($(subst default,undefined,$(origin FORMAT_MANPAGE_DVI)),undefined)
FORMAT_MANPAGE_DVI = ${TROFFDVI} ${TROFFDVIFLAGS}
endif
ifeq ($(subst default,undefined,$(origin FORMAT_MANPAGE_HTML)),undefined)
FORMAT_MANPAGE_HTML = ${TROFFHTML} ${TROFFHTMLFLAGS}
endif

# manpage conversions
ifeq ($(subst default,undefined,$(origin MANTOTXT)),undefined)
MANTOTXT = \
${ECHOLINEX} creating $(call q,$@) from $(call q,$<) && \
${SHELL} -c $(call q,${FORMAT_MANPAGE_TXT} | ${FILTER_BACKSPACES}) < $(call q,$<) > $(call q,$@) || \
    ( \
        ${REMOVE} $(call q,$@) ; \
        exit 1 \
    )
endif
ifeq ($(subst default,undefined,$(origin MANTOPS)),undefined)
MANTOPS = \
${ECHOLINEX} creating $(call q,$@) from $(call q,$<) && \
${SHELL} -c $(call q,${FORMAT_MANPAGE_PS}) < $(call q,$<) > $(call q,$@) || \
    ( \
        ${REMOVE} $(call q,$@) ; \
        exit 1 \
    )
endif
ifeq ($(subst default,undefined,$(origin MANTODVI)),undefined)
MANTODVI = \
${ECHOLINEX} creating $(call q,$@) from $(call q,$<) && \
${SHELL} -c $(call q,${FORMAT_MANPAGE_DVI}) < $(call q,$<) > $(call q,$@) || \
    ( \
        ${REMOVE} $(call q,$@) ; \
        exit 1 \
    )
endif
ifeq ($(subst default,undefined,$(origin MANTOHTML)),undefined)
MANTOHTML = \
${ECHOLINEX} creating $(call q,$@) from $(call q,$<) && \
(${SHELL} -c $(call q,${FORMAT_MANPAGE_HTML}) < $(call q,$<) > $(call q,$@)${tmp} && \
            ${SED} ${SEDFLAGS} -e $(call q, \
                s/&minus;/\&${char_number_sign}45;/g; \
                s/&lsquo;/\&${char_number_sign}39;/g; \
                s/&rsquo;/\&${char_number_sign}39;/g; \
                s/&ldquo;/\&${char_number_sign}34;/g; \
                s/&rdquo;/\&${char_number_sign}34;/g; \
            ) < $(call q,$@)${tmp} > $(call q,$@) && \
                ( \
                    ${REMOVE} $(call q,$@)${tmp} \
                ) || \
                ( \
                    ${REMOVE} $(call q,$@) ; \
                    ${REMOVE} $(call q,$@)${tmp} ; \
                    exit 1 \
                ) \
)
endif

# file and directory installation
#INSTALL = /usr/bin/install
ifeq ($(subst default,undefined,$(origin INSTALL)),undefined)
INSTALL = install
endif
ifeq ($(subst default,undefined,$(origin INSTALLFLAGS)),undefined)
INSTALLFLAGS = 
endif
ifeq ($(subst default,undefined,$(origin STRIPINSTALLFLAGS)),undefined)
STRIPINSTALLFLAGS = 
endif
ifeq ($(subst default,undefined,$(origin INSTALL_PROGRAM)),undefined)
INSTALL_PROGRAM = ${INSTALL} ${INSTALLFLAGS} ${EXTRAINSTALLFLAGS}
endif
ifeq ($(subst default,undefined,$(origin INSTALL_DATA)),undefined)
INSTALL_DATA = ${INSTALL} -m 644
endif
ifeq ($(subst default,undefined,$(origin INSTALL_DIR)),undefined)
INSTALL_DIR = ${INSTALL} -d
endif
ifeq ($(subst default,undefined,$(origin INSTALL_MANLINK)),undefined)
INSTALL_MANLINK = install_manlink
endif
ifeq ($(subst default,undefined,$(origin MKPARENTDIR)),undefined)
MKPARENTDIR = test -d $(call q,$(dir $@)) || \
            (${ECHOLINEX} creating directory $(call q,$(dir $@)) && \
                ${INSTALL_DIR} $(call q,$(dir $@)))
endif

## non-backslash-interpreting "echo"-equivalent used during file
## generation (internal version invoked directly by make subshell)
ifeq ($(subst default,undefined,$(origin ECHOLINE_internal)),undefined)
ECHOLINE_internal = ${SHELL} $(call q,${src})configure --dump=non_option_arguments --
endif

## non-backslash-interpreting "echo"-equivalent used during file generation
ifeq ($(subst default,undefined,$(origin ECHOLINE)),undefined)
ECHOLINE = ${ECHOLINE_internal}
endif

## like ${ECHOLINE}, but does C string-style quoting (apostrophe is not quoted)
ifeq ($(subst default,undefined,$(origin ECHOLINEX)),undefined)
ECHOLINEX = ${SHELL} $(call q,${src})configure --dump=non_option_arguments --dump-escaped --
endif

# special human-readable variable used to distinguish source from
# substituted results
this_file_undergoes_variable_substitution = \
automatically generated ${date} for ${MYMAN} ${MYMANVERSION}

## program names

ifeq ($(subst default,undefined,$(origin program_prefix)),undefined)
program_prefix = 
endif

ifeq ($(subst default,undefined,$(origin program_suffix)),undefined)
program_suffix = 
endif

ifeq ($(subst default,undefined,$(origin program_transform_name)),undefined)
program_transform_name = s,x,x,
endif

program = $(if $(call s,$(call qx,s${char_comma}x${char_comma}x${char_comma}),,${program_transform_name}),$(call shell,${SHELL} -c $(call q,${ECHOLINE_internal} $(call q,${program_prefix}$1${program_suffix}) | ${SED} ${SEDFLAGS} -e $(call q,${program_transform_name}))),${program_prefix}$1${program_suffix})

ifeq ($(subst default,undefined,$(origin MYMAN_EXE)),undefined)
MYMAN_EXE = $(call program,${MYMAN})
endif
ifeq ($(subst default,undefined,$(origin MYMAN_APP)),undefined)
MYMAN_APP = $(call program,${MYMAN}).app
endif
ifeq ($(subst default,undefined,$(origin MYMAN_ICNS)),undefined)
MYMAN_ICNS = $(call program,${MYMAN}).icns
endif
ifeq ($(subst default,undefined,$(origin PLATFORM)),undefined)
PLATFORM = MacOS
endif
ifeq ($(subst default,undefined,$(origin DIST_EXE)),undefined)
DIST_EXE = $(call program,${DIST})
endif
ifeq ($(subst default,undefined,$(origin XMYMAN_EXE)),undefined)
XMYMAN_EXE = $(call program,${XMYMAN})
endif
ifeq ($(subst default,undefined,$(origin XMYMAN2_EXE)),undefined)
XMYMAN2_EXE = $(call program,${XMYMAN2})
endif
ifeq ($(subst default,undefined,$(origin XMYMAN3_EXE)),undefined)
XMYMAN3_EXE = $(call program,${XMYMAN3})
endif
ifeq ($(subst default,undefined,$(origin XMYMAN4_EXE)),undefined)
XMYMAN4_EXE = $(call program,${XMYMAN4})
endif
ifeq ($(subst default,undefined,$(origin XBIGMAN_EXE)),undefined)
XBIGMAN_EXE = $(call program,${XBIGMAN})
endif
ifeq ($(subst default,undefined,$(origin XHUGEMAN_EXE)),undefined)
XHUGEMAN_EXE = $(call program,${XHUGEMAN})
endif
ifeq ($(subst default,undefined,$(origin XBITMAN_EXE)),undefined)
XBITMAN_EXE = $(call program,${XBITMAN})
endif
ifeq ($(subst default,undefined,$(origin XBITMAN2_EXE)),undefined)
XBITMAN2_EXE = $(call program,${XBITMAN2})
endif
ifeq ($(subst default,undefined,$(origin XBITMAN3_EXE)),undefined)
XBITMAN3_EXE = $(call program,${XBITMAN3})
endif
ifeq ($(subst default,undefined,$(origin XBITMAN4_EXE)),undefined)
XBITMAN4_EXE = $(call program,${XBITMAN4})
endif
ifeq ($(subst default,undefined,$(origin XBITMAN5_EXE)),undefined)
XBITMAN5_EXE = $(call program,${XBITMAN5})
endif
ifeq ($(subst default,undefined,$(origin XBITMAN6_EXE)),undefined)
XBITMAN6_EXE = $(call program,${XBITMAN6})
endif
ifeq ($(subst default,undefined,$(origin XQUACKMAN_EXE)),undefined)
XQUACKMAN_EXE = $(call program,${XQUACKMAN})
endif
ifeq ($(subst default,undefined,$(origin MYMANCOMMAND_EXE)),undefined)
MYMANCOMMAND_EXE = $(call program,${MYMANCOMMAND})
endif
ifeq ($(subst default,undefined,$(origin MYMAN_CT_EXE)),undefined)
MYMAN_CT_EXE = $(call program,${MYMAN_CT})
endif

## Tools for updating the "whatis" database

# newer mandb style
ifeq ($(subst default,undefined,$(origin MANDB)),undefined)
MANDB = mandb
endif
ifeq ($(subst default,undefined,$(origin MANDBFLAGS)),undefined)
MANDBFLAGS = 
endif

# older makewhatis style (used if newer mandb style fails)
ifeq ($(subst default,undefined,$(origin MAKEWHATIS)),undefined)
MAKEWHATIS = makewhatis
endif
ifeq ($(subst default,undefined,$(origin MAKEWHATISFLAGS)),undefined)
MAKEWHATISFLAGS = 
endif

## C preprocessing flags
ifeq ($(subst default,undefined,$(origin CPPFLAGS)),undefined)
CPPFLAGS = ${EXTRACPPFLAGS}
endif
ifeq ($(subst default,undefined,$(origin HOSTCPPFLAGS)),undefined)
HOSTCPPFLAGS = ${CPPFLAGS}
endif
ifeq ($(subst default,undefined,$(origin CINCLUDES)),undefined)
CINCLUDES = ${EXTRACINCLUDES}
endif
ifeq ($(subst default,undefined,$(origin HOSTCINCLUDES)),undefined)
HOSTCINCLUDES = ${CINCLUDES}
endif

## Extra C preprocessing flags for MyMan
ifeq ($(subst default,undefined,$(origin MYMANCPPFLAGS)),undefined)
MYMANCPPFLAGS = -DHAVE_CONFIG_H=1 ${MYGETOPTCPPFLAGS}
endif
ifeq ($(subst default,undefined,$(origin MYMANCINCLUDES)),undefined)
MYMANCINCLUDES = -I${src}inc -I. ${MYGETOPTCINCLUDES}
endif

## ANSI C/C++ compiler (choose one)

# NOTE: MyMan is written in ANSI C, but pains have been taken to make
# sure it is also valid ANSI C++. Compiling using a C++ compiler
# rather than a C compiler may improve optimization and help find
# subtle type incompatibilities which would go unnoticed by a plain C
# compiler.

# Compiler for build machine

# Option 1: GNU C or C++ compiler (g++ or gcc)
#CC = g++ -g -Wall
ifeq ($(subst default,undefined,$(origin CC)),undefined)
CC = gcc
endif

# Option 2: Old C or C++ compiler (c++, cc or c89)
#CC = c++
#CC = cc
#CC = c89

ifneq (,$(findstring gcc,$(subst g++,gcc,$(subst gxx,g++,${CC}))))

# looks like GCC, let's generate debugging information and warnings
ifeq ($(subst default,undefined,$(origin CFLAGS)),undefined)
CFLAGS = ${EXTRACFLAGS} -g -Wall
endif

else

# other compiler, at least try to generate debugging information
ifeq ($(subst default,undefined,$(origin CFLAGS)),undefined)
CFLAGS = ${EXTRACFLAGS} -g
endif

endif


# Compiler for host machine

ifeq (,${hostprefix})
ifeq ($(subst default,undefined,$(origin HOSTCC)),undefined)
HOSTCC = ${CC}
endif
else
ifeq ($(subst default,undefined,$(origin HOSTCC)),undefined)
HOSTCC = ${hostprefix}gcc
endif
endif

ifneq (,$(findstring gcc,$(subst g++,gcc,$(subst gxx,g++,${HOSTCC}))))

ifeq ($(subst default,undefined,$(origin HOSTCFLAGS)),undefined)
HOSTCFLAGS = ${CFLAGS} ${EXTRAHOSTCFLAGS}
endif

# looks like GCC, let's optimize unless there's already an
# opitimization flag
ifeq (,$(findstring ${char_space}-O,${HOSTCC} ${HOSTCFLAGS}))
HOSTCFLAGS += -O3
endif

#ifneq ($(subst -darwin,-,-${host}-),-${host}-)
#HOSTCFLAGS += -mlong-branch
#endif

else

# other compiler, don't try to optimize since that may conflict with
# debugging flags
ifeq ($(subst default,undefined,$(origin HOSTCFLAGS)),undefined)
HOSTCFLAGS = ${CFLAGS} ${EXTRAHOSTCFLAGS}
endif

endif

# Complete preprocessing and compilation action for option 1 or 2
ifeq ($(subst default,undefined,$(origin COMPILE)),undefined)
ifneq (,$(findstring dmc,${CC})$(findstring bcc32,${CC}))
COMPILE = compile() { ${ECHOLINEX} ${CC} ${CFLAGS} ${CPPFLAGS} ${MYMANCPPFLAGS} ${CINCLUDES} ${MYMANCINCLUDES} "$$@" -o$(call q,$@) -c $(call q,$<); ${CC} ${CFLAGS} ${CPPFLAGS} ${MYMANCPPFLAGS} ${CINCLUDES} ${MYMANCINCLUDES} "$$@" -o$(call q,$@) -c $(call q,$<); }; compile
else
COMPILE = compile() { ${ECHOLINEX} ${CC} ${CFLAGS} ${CPPFLAGS} ${MYMANCPPFLAGS} ${CINCLUDES} ${MYMANCINCLUDES} "$$@" -o $(call q,$@) -c $(call q,$<); ${CC} ${CFLAGS} ${CPPFLAGS} ${MYMANCPPFLAGS} ${CINCLUDES} ${MYMANCINCLUDES} "$$@" -o $(call q,$@) -c $(call q,$<); }; compile
endif
endif
ifeq ($(subst default,undefined,$(origin HOSTCOMPILE)),undefined)
ifneq (,$(findstring dmc,${HOSTCC})$(findstring bcc32,${HOSTCC}))
HOSTCOMPILE = hostcompile() { ${ECHOLINEX} ${HOSTCC} ${HOSTCFLAGS} ${HOSTCPPFLAGS} ${MYMANCPPFLAGS} ${HOSTCINCLUDES} ${MYMANCINCLUDES} "$$@" -o$(call q,$@) -c $(call q,$<); ${HOSTCC} ${HOSTCFLAGS} ${HOSTCPPFLAGS} ${MYMANCPPFLAGS} ${HOSTCINCLUDES} ${MYMANCINCLUDES} "$$@" -o$(call q,$@) -c $(call q,$<); }; hostcompile
else
HOSTCOMPILE = hostcompile() { ${ECHOLINEX} ${HOSTCC} ${HOSTCFLAGS} ${HOSTCPPFLAGS} ${MYMANCPPFLAGS} ${HOSTCINCLUDES} ${MYMANCPPFLAGS} "$$@" -o $(call q,$@) -c $(call q,$<); ${HOSTCC} ${HOSTCFLAGS} ${HOSTCPPFLAGS} ${MYMANCPPFLAGS} ${HOSTCINCLUDES} ${MYMANCINCLUDES} "$$@" -o $(call q,$@) -c $(call q,$<); }; hostcompile
endif
endif

## Libraries for all executables
ifeq ($(subst default,undefined,$(origin LIBS)),undefined)
LIBS = ${EXTRALIBS}
endif
ifeq ($(subst default,undefined,$(origin HOSTLIBS)),undefined)
HOSTLIBS = ${LIBS} ${EXTRAHOSTLIBS}
endif

## Linker (choose one)

# Option 1: Link using C/C++ compiler (specified above)
# Option 2: Link using linker (specified as [HOST]LD)
ifeq ($(subst default,undefined,$(origin LINK)),undefined)
ifeq ($(subst default,undefined,$(origin LD)),undefined)
ifneq (,$(findstring dmc,${CC}))
LINK = ${CC} ${CFLAGS} ${EXTRACFLAGS} -o$(call q,$@) $(call q,$<)
else
ifneq (,$(findstring bcc32,${CC}))
LINK = ${CC} ${CFLAGS} ${EXTRACFLAGS} -e$(call q,$@) $(call q,$<)
else
LINK = ${CC} ${CFLAGS} ${EXTRACFLAGS} -o $(call q,$@) $(call q,$<)
endif
endif
else
LINK = ${LD} ${LDFLAGS} ${EXTRALDFLAGS} -o $(call q,$@) $(call q,$<)
endif
endif
ifeq ($(subst default,undefined,$(origin HOSTLINK)),undefined)
ifeq ($(subst default,undefined,$(origin HOSTLD)),undefined)
ifneq (,$(findstring dmc,${HOSTCC}))
HOSTLINK = ${HOSTCC} ${HOSTCFLAGS} ${EXTRAHOSTCFLAGS} -o$(call q,$@) $(call q,$<)
else
ifneq (,$(findstring bcc32,${HOSTCC}))
HOSTLINK = ${HOSTCC} ${HOSTCFLAGS} ${EXTRAHOSTCFLAGS} -e$(call q,$@) $(call q,$<)
else
HOSTLINK = ${HOSTCC} ${HOSTCFLAGS} ${EXTRAHOSTCFLAGS} -o $(call q,$@) $(call q,$<)
endif
endif
else
HOSTLINK = ${HOSTLD} ${HOSTLDFLAGS} ${EXTRAHOSTLDFLAGS} -o $(call q,$@) $(call q,$<)
endif
endif


##
## Game configuration
##

## game sizes and variants included in distribution
all_variants = \
myman \
catchum \
chomp \
gobble \
munchkin \
pacgal \
pacmanic \
portable \
msport \
spear \
pacman89 \
quackman \
tinypac2 \
pacmania \
kpacman \
pacjr \
mspp \
jrpac \
jrpac64 \
jrpac26 \
kasper \
scandal \
hangly \
pesco \
ebipac \
hackem \
pac64 \
mspac26 \
pac2600 \
pac26 \
mrpac26 \
ipac \
nespac \
ghost \
dacman \
babypac \
paczx \
mspaczx \
pacmsx \
pac4a \
pac20 \
mspac78 \
mspaclnx \
mspac20 \
mspac4a \
mspacnes \
mspacgb \
mspaccdi \
jellymon \
gobbler \
ghostman \
demon \
nokiapac \
mspacgg \
snespac \
pacjs \
pman2 \
pman \
clsweep \
pcman \
zuckman \
paczx2 \
mazeman \
glooper \
ghunt \
spooky \
ohno \
pacgem \
munchman \
mmproto \
crush \
pacbox \
tinypac \
hasewolf \
pac700 \
pacmanvs \
smartpac \
bugman \
pacman2 \
pacman98 \
snapper \
peerless \
psppac \
pac6001 \
deluxe \
scarfman \
pac8bit \
mspacgt \
newpuc2 \
arrange \
asciiman \
pacpunk \
pactac \
mspaccpc \
spook \
cattrax \
jrpac52 \
pcmania4 \
nibble \
crazyg \
chicky \
monster \
tomyport \
packri \
puckpock \
blinky \
mania \
eatit \
greedy \
mschomp \
pacpsion \
pacxgs \
paccv \
muncher \
munchy \
xensrvng \
ultrapac \
fhmcpac \
mspacatk \
packman \
nascom \
pacnstuf \
pakkman \
msblitz \
mshack \
lives \
gobbler2 \
pacmanpc \
ababa \
puckmon \
yoyopac \
gobble2 \
mmpalm \
gobbler0 \
perqman \
mazewar \
3demon \
pacpdp11 \
apollo \
pacrt11 \
sidtool \
pacvms \
mspacln2 \
mspwlnx \
mspwlnx2 \
gobbler1 \
gobbler3 \
ghostma2 \
strange \
mini \
mstiny \
alien \
pacst \
pacmen \
sympac \
tituspac \
classmun \
ascpac \
minipac \
ruckman \
guimp \
hannah \
chompman \
javapac \
titanman \
hatman \
xfred \
pacgent \
hedges \
gnasher \
snail \
munchie \
revenge \
gobbleag \
thief \
plaque \
plaque2 \
sir \
pacmunch \
jrsmall \
jrtiny \
3dpac \
ackman \
paccdi \
gpac \
gsmall \
hackem2 \
hangly26 \
makman \
pacclone \
scheme \
pcsmall \
pcman2 \
qman \
tman \
toyman \
salomon \
byh \
ptmx \
smallpac \
mssmall \
baby2 \
pac \
pac2 \
small \
upacthin \
blobbo \
tiny \
mc10 \
ghost2 \
nopoint \
gobbler4 \
pacgalax \
kcm \
remake \
guppy \
pepperii \
p4c \
simplej \
3b1 \
k3 \
pacmes \
cupcake \
iitpac \
pacmanx \
pacex2 \
armpac2 \
deluxe2 \
zaurus \
vermin \
cgkit \
misspac \
misstiny \
pac49 \
pac188 \
small188
all_sizes = \
small \
big \
huge \
square \
quack \
bitmap \
bitmap2 \
bitmap3 \
bitmap4 \
bitmap0 \
bitmap1 \
bitmap5 \
bitmap6 \
bitmap7 \
bitmap8 \
bitmap9 \
bitmapa \
bitmapb \
perq \
perq2 \
perq3 \
perq4 \
perq5 \
kpac \
kpac2 \
kpac3 \
kpac4 \
otto \
otto2 \
otto3 \
otto4 \
custom \
bitmap3a \
kpac3a \
bitmap5a \
kpac5 \
bitmap6a \
kpac6 \
bitmap7a \
kpac7 \
bitmap8a \
kpac8 \
bitmap9a \
kpac9 \
otto3a \
rogue

## game sizes and variants to actually build
ifeq ($(subst default,undefined,$(origin enable_variants)),undefined)
enable_variants = yes
endif
ifeq ($(subst default,undefined,$(origin enable_variant)),undefined)
enable_variant = yes
endif

MYMANVARIANTS := $(patsubst ,$(firstword ${all_variants}),$(strip $(foreach variant,${all_variants},$(firstword $(patsubst no,,$(patsubst ,$(filter ${variant},$(patsubst yes,${variant},${enable_variants})),$(patsubst ,$(filter ${variant},${enable_variant}),$(filter ${variant} no,$(patsubst yes,${variant},$(enable_variant_${variant}))))))))))
MYMANVARIANT := $(firstword $(patsubst ,(none),$(filter ${all_variants},$(patsubst yes,$(firstword ${MYMANVARIANTS}),$(patsubst no,,${enable_variant})))))

ifeq ($(subst default,undefined,$(origin enable_sizes)),undefined)
enable_sizes = yes
endif
ifeq ($(subst default,undefined,$(origin enable_size)),undefined)
enable_size = yes
endif

MYMANSIZES := $(patsubst ,$(firstword ${all_sizes}),$(strip $(foreach size,${all_sizes},$(firstword $(patsubst no,,$(patsubst ,$(filter ${size},$(patsubst yes,${size},${enable_sizes})),$(patsubst ,$(filter ${size},${enable_size}),$(filter ${size} no,$(patsubst yes,${size},$(enable_size_${size}))))))))))
MYMANSIZE := $(firstword $(patsubst ,(none),$(filter ${all_sizes},$(patsubst yes,$(firstword ${MYMANSIZES}),$(patsubst no,,${enable_size})))))

# final version basename
ifeq ($(subst default,undefined,$(origin GAME)),undefined)
GAME = game
endif
# initial bootstrap version basename
ifeq ($(subst default,undefined,$(origin BOOTSTRAP)),undefined)
BOOTSTRAP = ${obj}s1${GAME}
endif
# bootstrap version basename as it is passed to the shell
ifeq ($(subst default,undefined,$(origin XBOOTSTRAP)),undefined)
ifeq (,${obj})
XBOOTSTRAP = ${objdir}/${BOOTSTRAP}
else
XBOOTSTRAP = ${BOOTSTRAP}
endif
endif
# intermediate builtin data file basenames
mymansize_data = ${obj}s2s$(call idx,$1,${all_sizes})
mymanvariant_data = ${obj}s2v$(call idx,$1,${all_variants})

## for builtin data
ifeq ($(subst default,undefined,$(origin DATADEFS)),undefined)
DATADEFS = -DBUILTIN_MAZE -DBUILTIN_TILE -DBUILTIN_SPRITE -DBUILTIN_SIZE -DBUILTIN_VARIANT -Dmain=main_builtin
endif
datafiledefs = -Dbuiltin_tilefile=builtin_tilefile_$2 -Dtile=tile_$2 -Dtile_used=tile_used_$2 -Dtile_color=tile_color_$2 -Dtile_w=tile_w_$2 -Dtile_h=tile_h_$2 -Dtile_flags=tile_flags_$2 -Dtile_args=tile_args_$2 -Dbuiltin_spritefile=builtin_spritefile_$2 -Dsprite=sprite_$2 -Dsprite_used=sprite_used_$2 -Dsprite_color=sprite_color_$2 -Dsprite_w=sprite_w_$2 -Dsprite_h=sprite_h_$2 -Dsprite_flags=sprite_flags_$2 -Dsprite_args=sprite_args_$2 -Dbuiltin_mazefile=builtin_mazefile_$1 -Dmaze_n=maze_n_$1 -Dmaze_w=maze_w_$1 -Dmaze_h=maze_h_$1 -Dmaze_flags=maze_flags_$1 -Dmaze_args=maze_args_$1 -Dmaze_color_data=maze_color_data_$1 -Dmaze_data=maze_data_$1

### libiconv options

ifeq (yes,${with_libiconv})

ifeq ($(subst default,undefined,$(origin ICONVLIBS)),undefined)
ICONVLIBS = -liconv
endif
ifeq ($(subst default,undefined,$(origin ICONVCFLAGS)),undefined)
ICONVCFLAGS = 
endif
ICONVOPTS += -DUSE_ICONV=1 ${ICONVCFLAGS}

endif

ifeq ($(subst default,undefined,$(origin ICONVLIBS)),undefined)
ICONVLIBS = 
endif
ifeq ($(subst default,undefined,$(origin ICONVCFLAGS)),undefined)
ICONVCFLAGS = 
endif
ifeq ($(subst default,undefined,$(origin ICONVOPTS)),undefined)
ICONVOPTS = 
endif

## curses options

# curses library

# ncurses under Linux, installed in /usr (not neccessarily as <curses.h> / -lcurses)
#CURSESLIBS = -L/usr/lib -lncurses
#CURSESINCLUDE = -I/usr/include/ncurses
# same, in /usr/local (not neccessarily as <curses.h> / -lcurses)
#CURSESLIBS = -L/usr/local/lib -lncurses
#CURSESINCLUDE = -I/usr/local/include/ncurses
# SysV curses in the SysV optional installation dirs, on a SunOS 4 system
#CURSESLIBS = -L/usr/5lib -lcurses
#CURSESINCLUDE = -I/usr/5include

# curses-related myman options

# Enable use of beep() by default:
#CURSOPTS = -DSOUND=1
# Use only ASCII characters by default:
#CURSOPTS = -DUSE_ACS=0
# Use CP437 graphics characters by default: (PC-console font)
#CURSOPTS = -DUSE_RAW=1
# Disable most character attributes:
#CURSOPTS = -DUSE_ATTR=0
# Exclude color support:
#CURSOPTS = -DUSE_COLOR=0
# Compile in color support, but leave it disabled by default:
#CURSOPTS = -DUSE_COLOR=1 -DCOLORIZE=0

ifeq (yes,${with_mac_unicode})

with_mac = yes

CURSOPTS += -DUNICODE

endif

ifeq (yes,${with_fltk2})

with_fltk = yes

CURSOPTS += -DFLTK2CURSES

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = FLTK 2
endif

# FLTK 2
ifeq ($(subst default,undefined,$(origin FLTK_CONFIG)),undefined)
FLTK_CONFIG = ${hostprefix}fltk2-config
endif

endif

ifeq (yes,${with_gtk2})

with_gtk = yes

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = GTK+ 2.0
endif

# GTK+ 2.0
ifeq ($(subst default,undefined,$(origin GTK_CONFIG)),undefined)
GTK_CONFIG = ${PKG_CONFIG} gtk+-2.0
endif
ifeq ($(subst default,undefined,$(origin PKG_CONFIG)),undefined)
PKG_CONFIG = ${hostprefix}pkg-config
endif

endif

ifeq (yes,${with_libcaca0})

with_libcaca = yes

CURSOPTS += -DCACACURSES0

endif

ifeq (yes,${with_xcursesw})

with_xcurses = yes

CURSOPTS += -DPDC_WIDE

endif

ifeq (yes,${with_pdcursesw})

with_pdcurses = yes

CURSOPTS += -DPDC_WIDE

endif

ifeq (yes,${with_pdcursessdl})

with_pdcurses = yes

ifeq ($(subst default,undefined,$(origin SDL_CONFIG)),undefined)
SDL_CONFIG = ${hostprefix}sdl-config
endif

ifeq ($(origin SDLLIBS),undefined)
SDLLIBS := $(call shell,${SDL_CONFIG} --libs)
endif
CURSESLIBS += ${SDLLIBS}

ifeq ($(origin SDLCFLAGS),undefined)
SDLCFLAGS := $(call shell,${SDL_CONFIG} --cflags)
endif
CURSOPTS += -DUSE_SDL ${SDLCFLAGS}

EXTRAWRAPPERDEFS += -DUSE_SDL ${SDLCFLAGS}

endif

ifeq (yes,${with_ggiwmh})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = libggi
endif

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR_TARGET)),undefined)
CURSES_FLAVOR_TARGET = a GGI display target
endif

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR_SECTION)),undefined)
CURSES_FLAVOR_SECTION = 7
endif

# LibGGI/LibGII with WindowManagerHints 
ifeq ($(subst default,undefined,$(origin GGILIBS)),undefined)
GGILIBS = -lggiwmh -lggi -lgii
endif
CURSESLIBS += ${GGILIBS}

ifeq ($(subst default,undefined,$(origin GGICFLAGS)),undefined)
GGICFLAGS = 
endif
CURSOPTS += -DUSE_GGIWMH=1 -DGGICURSES ${GGICFLAGS}

else
ifeq (yes,${with_ggi})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = libggi
endif

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR_TARGET)),undefined)
CURSES_FLAVOR_TARGET = a GGI display target
endif

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR_SECTION)),undefined)
CURSES_FLAVOR_SECTION = 7
endif

# LibGGI/LibGII
ifeq ($(subst default,undefined,$(origin GGILIBS)),undefined)
GGILIBS = -lggi -lgii
endif
CURSESLIBS += ${GGILIBS}

ifeq ($(subst default,undefined,$(origin GGICFLAGS)),undefined)
GGICFLAGS = 
endif
CURSOPTS += -DGGICURSES ${GGICFLAGS}

else
ifeq (yes,${with_sdl})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = SDL
endif

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR_TARGET)),undefined)
CURSES_FLAVOR_TARGET = an SDL display target
endif

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR_SECTION)),undefined)
CURSES_FLAVOR_SECTION = 3
endif

# SDL
ifeq ($(subst default,undefined,$(origin SDL_CONFIG)),undefined)
SDL_CONFIG = ${hostprefix}sdl-config
endif

ifeq ($(origin SDLLIBS),undefined)
SDLLIBS := $(call shell,${SDL_CONFIG} --libs)
endif
CURSESLIBS += ${SDLLIBS}

ifeq ($(origin SDLCFLAGS),undefined)
SDLCFLAGS := $(call shell,${SDL_CONFIG} --cflags)
endif
CURSOPTS += -DSDLCURSES -DUSE_SDL ${SDLCFLAGS}

EXTRAWRAPPERDEFS += -DUSE_SDL ${SDLCFLAGS}

else
ifeq (yes,${with_twin})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = twin
endif

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR_TARGET)),undefined)
CURSES_FLAVOR_TARGET = a Twin display
endif

# Twin
ifeq ($(subst default,undefined,$(origin TWINLIBS)),undefined)
TWINLIBS = -lTw
endif
CURSESLIBS += ${TWINLIBS}

ifeq ($(subst default,undefined,$(origin TWINCFLAGS)),undefined)
TWINCFLAGS = 
endif
CURSOPTS += -DTWCURSES ${TWINCFLAGS}

else
ifeq (yes,${with_aalib})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = aalib
endif

# aalib
ifeq ($(subst default,undefined,$(origin AALIB_CONFIG)),undefined)
AALIB_CONFIG = ${hostprefix}aalib-config
endif
ifeq ($(origin AALIBLIBS),undefined)
AALIBLIBS := $(call shell,${AALIB_CONFIG} --libs)
endif
CURSESLIBS += ${AALIBLIBS}

ifeq ($(origin AALIBFLAGS),undefined)
AALIBCFLAGS := $(call shell,${AALIB_CONFIG} --cflags)
endif
CURSOPTS += -DAACURSES ${AALIBCFLAGS}

else
ifeq (yes,${with_disp})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = disp
endif

# disp
CURSOPTS += -DDISPCURSES ${DISPCFLAGS}

else
ifeq (yes,${with_conio})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = conio
endif

# Borland-style CONIO
CURSOPTS += -DCONIOCURSES ${DISPCFLAGS}

else
ifeq (yes,${with_newt})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = newt
endif

# newt
ifeq ($(origin NEWTLIBS),undefined)
NEWTLIBS = -lnewt -lslang
endif
CURSESLIBS += ${NEWTLIBS}

CURSOPTS += -DNEWTCURSES ${NEWTCFLAGS}

else
ifeq (yes,${with_fltk})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = FLTK
endif

# FLTK requires our code to be compiled as C++
ifeq ($(subst default,undefined,$(origin CC)),undefined)
CC = g++
endif

# FLTK
ifeq ($(subst default,undefined,$(origin FLTK_CONFIG)),undefined)
FLTK_CONFIG = ${hostprefix}fltk-config
endif
ifeq ($(origin FLTKLIBS),undefined)
FLTKLIBS := $(call shell,${FLTK_CONFIG} --ldflags)
endif
CURSESLIBS += ${FLTKLIBS}

ifeq ($(origin FLTKCFLAGS),undefined)
FLTKCFLAGS := $(call shell,${FLTK_CONFIG} --cflags)
endif
CURSOPTS += -DFLTKCURSES ${FLTKCFLAGS}

else
ifeq (yes,${with_mac})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = Carbon
endif

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR_TARGET)),undefined)
CURSES_FLAVOR_TARGET = a Mac OS X display
endif

ifeq ($(subst default,undefined,$(origin SDKROOT)),undefined)
SDKROOT = /Developer/SDKs/MacOSX10.4u.sdk
endif

ifeq ($(subst default,undefined,$(origin ARCHS)),undefined)
ARCHS = ppc i386
endif

ifeq ($(subst default,undefined,$(origin CARBONCFLAGS)),undefined)
CARBONCFLAGS = -framework Carbon
endif

ifeq ($(subst default,undefined,$(origin CARBONLIBS)),undefined)
CARBONLIBS = ${CARBONCFLAGS}
endif

HOSTCFLAGS += -isysroot ${SDKROOT} $(foreach arch,${ARCHS},-arch ${arch})
HOSTLIBS += -isysroot ${SDKROOT} $(foreach arch,${ARCHS},-arch ${arch})

ifeq (,${hostprefix})
CFLAGS += -isysroot ${SDKROOT} $(foreach arch,${ARCHS},-arch ${arch})
LIBS += -isysroot ${SDKROOT} $(foreach arch,${ARCHS},-arch ${arch})
endif

# Carbon/Toolbox
CURSOPTS += -DMACCURSES ${CARBONCFLAGS}

CURSESLIBS += ${CARBONLIBS}

else
ifeq (yes,${with_raw})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = stdio
endif

CURSOPTS += -DRAWCURSES -DUNICODE

else
ifeq (yes,${with_raw_cp437})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = stdio
endif

CURSOPTS += -DRAWCURSES

else
ifeq (yes,${with_allegro})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = allegro
endif

# Allegro
ifeq ($(subst default,undefined,$(origin ALLEGRO_CONFIG)),undefined)
ALLEGRO_CONFIG = ${hostprefix}allegro-config
endif
ifeq ($(origin ALLEGROLIBS),undefined)
ALLEGROLIBS := $(call shell,${ALLEGRO_CONFIG} --libs)
endif
#CURSESLIBS += ${ALLEGROLIBS}
EXTRALIBS += ${ALLEGROLIBS}

ifeq ($(origin ALLEGROFLAGS),undefined)
ALLEGROCFLAGS := $(call shell,${ALLEGRO_CONFIG} --cflags)
endif
# use UCS/Unicode characters for internal representation
CURSOPTS += -DUSE_RAW_UCS=1 -DALLEGROCURSES ${ALLEGROCFLAGS}

EXTRAWRAPPERDEFS += -DALLEGROCURSES ${ALLEGROCFLAGS}

else
ifeq (yes,${with_libcaca})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = libcaca
endif

# libcaca
ifeq ($(subst default,undefined,$(origin CACA_CONFIG)),undefined)
CACA_CONFIG = ${hostprefix}caca-config
endif
ifeq ($(origin CACALIBS),undefined)
CACALIBS := $(call shell,${CACA_CONFIG} --libs)
endif
CURSESLIBS += ${CACALIBS}

ifeq ($(origin CACAFLAGS),undefined)
CACACFLAGS := $(call shell,${CACA_CONFIG} --cflags)
endif
# use UCS/Unicode characters for internal representation
CURSOPTS += -DUSE_RAW_UCS=1 -DCACACURSES ${CACACFLAGS}

else
ifeq (yes,${with_gtk})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = GTK+
endif

# GTK+
ifeq ($(subst default,undefined,$(origin GTK_CONFIG)),undefined)
GTK_CONFIG = ${hostprefix}gtk-config
endif
ifeq ($(origin GTKLIBS),undefined)
GTKLIBS := $(call shell,${GTK_CONFIG} --libs)
endif
CURSESLIBS += ${GTKLIBS}

ifeq ($(origin GTKFLAGS),undefined)
GTKCFLAGS := $(call shell,${GTK_CONFIG} --cflags)
endif
CURSOPTS += -DGTKCURSES ${GTKCFLAGS}

else
ifeq (yes,${with_slang_utf8})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = slang-utf8
endif

# UTF-8 S-Lang curses emulation (a.k.a. slcurses)
CURSESLIBS += -lslang
CURSESINCLUDE += $(call q,-DMY_CURSES_H=<slcurses.h>)

# use UCS/Unicode characters for internal representation
CURSOPTS += -DUSE_RAW_UCS=1

# enable slcurses compatibility hacks in myman.c
CURSOPTS += -DSLCURSES

else
ifeq (yes,${with_slang})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = slang
endif

# S-Lang curses emulation (a.k.a. slcurses)
CURSESLIBS += -lslang
CURSESINCLUDE += $(call q,-DMY_CURSES_H=<slcurses.h>)

# enable slcurses compatibility hacks in myman.c
CURSOPTS += -DSLCURSES

else
ifeq (yes,${with_xcurses})

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = xcurses
endif
ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR_TARGET)),undefined)
CURSES_FLAVOR_TARGET = an X Window System display
endif

# PDCurses for X (a.k.a. XCurses)
ifeq ($(subst default,undefined,$(origin XCURSES_CONFIG)),undefined)
XCURSES_CONFIG = ${hostprefix}xcurses-config
endif
xcurses_libraries := $(call shell,${SHELL} -c $(call q,${XCURSES_CONFIG} --libs) </dev/null 2>/dev/null)
xcurses_includes := $(call shell,${SHELL} -c $(call q,${XCURSES_CONFIG} --cflags) </dev/null 2>/dev/null)
CURSESLIBS += ${xcurses_libraries}
CURSESINCLUDE += ${xcurses_includes}

EXTRAWRAPPERDEFS += -DXCURSES

else
ifeq (yes,${with_ncursesw})

# ncurses 5.x (wide character version)

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = ncursesw
endif

ifeq ($(subst default,undefined,$(origin NCURSESW_CONFIG)),undefined)
NCURSESW_CONFIG = ${hostprefix}ncursesw5-config
endif
ifeq ($(origin NCURSESWLIBS),undefined)
NCURSESWLIBS := $(call shell,${NCURSESW_CONFIG} --libs)
endif
CURSESLIBS += ${NCURSESWLIBS}

ifeq ($(origin NCURSESWCFLAGS),undefined)
NCURSESWCFLAGS := $(call shell,${NCURSESW_CONFIG} --cflags)
endif
CURSOPTS += ${NCURSESWCFLAGS}

CURSESINCLUDE += $(call q,-DMY_CURSES_H=<ncurses.h>)

# use X/OPEN wide character API when in raw mode
CURSOPTS += -DNCURSES_XOPEN_HACK

else
ifeq (yes,${with_ncurses})

# ncurses 5.x in the usual place, but called "ncurses" instead of "curses"

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = ncurses
endif

ifeq ($(subst default,undefined,$(origin NCURSES_CONFIG)),undefined)
NCURSES_CONFIG = ${hostprefix}ncurses5-config
endif
ifeq ($(origin NCURSESLIBS),undefined)
NCURSESLIBS := $(call shell,${NCURSES_CONFIG} --libs)
endif
CURSESLIBS += ${NCURSESLIBS}

ifeq ($(origin NCURSESCFLAGS),undefined)
NCURSESCFLAGS := $(call shell,${NCURSES_CONFIG} --cflags)
endif
CURSOPTS += ${NCURSESCFLAGS}

CURSESINCLUDE += $(call q,-DMY_CURSES_H=<ncurses.h>)

else
ifeq (yes,${with_pdcurses})

# PDCurses
ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = pdcurses
endif
ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR_TARGET)),undefined)
CURSES_FLAVOR_TARGET = a text window
endif

# pdcurses in the usual place
CURSESLIBS += -lpdcurses

else

# ncurses or SysV curses in the usual place
CURSESLIBS += -lcurses

endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif

ifneq (${host},${build})

# The bootstrap executable is built with CP-437 rawcurses (like --with-raw-cp437) when cross-compiling

#BUILDCURSESLIBS =
#BUILDICONVLIBS =
BUILDCURSOPTS = -DRAWCURSES=1
#EXTRABUILDCURSOPTS =
#BUILDICONVOPTS =
#BUILDCURSESINCLUDE =
#EXTRABUILDICONVOPTS =

else

BUILDCURSESLIBS = ${CURSESLIBS}
BUILDICONVLIBS = ${ICONVLIBS}
BUILDCURSOPTS = ${CURSOPTS}
EXTRABUILDCURSOPTS = ${EXTRACURSOPTS}
BUILDICONVOPTS = ${ICONVOPTS}
BUILDCURSESINCLUDE = ${CURSESINCLUDE}
EXTRABUILDICONVOPTS = ${EXTRAICONVOPTS}

endif

ifeq (yes,${with_sdl_mixer})
SDLCFLAGS += -DUSE_SDL_MIXER=1
SDLLIBS += -lSDL_mixer
endif

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR)),undefined)
CURSES_FLAVOR = curses
endif

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR_TARGET)),undefined)
CURSES_FLAVOR_TARGET = a text terminal or terminal emulator
endif

ifeq ($(subst default,undefined,$(origin CURSES_FLAVOR_SECTION)),undefined)
CURSES_FLAVOR_SECTION = 3
endif

ifeq ($(subst default,undefined,$(origin with_mac_icon)),undefined)
ifeq (yes,${with_mac})
with_mac_icon = yes
else
ifeq ($(subst -apple-,--,-${host}-),-${host}-)
with_mac_icon =
else
with_mac_icon = yes
endif
endif
endif

ifeq ($(subst default,undefined,$(origin with_dmg)),undefined)
ifeq (yes,${with_mac})
with_dmg = yes
else
ifeq (yes,${with_mac_icon})
with_dmg = yes
else
ifeq ($(subst -apple-,--,-${host}-),-${host}-)
with_dmg =
else
with_dmg = yes
endif
endif
endif
endif

ifeq ($(subst default,undefined,$(origin with_win_icon)),undefined)
ifeq ($(subst -msys-,--,$(subst -mingw32msvc-,--,$(subst -mingw32-,--,$(subst -mingw-,--,$(subst -cygwin32-,--,$(subst -cygwin-,--,-${host}-)))))),-${host}-)
with_win_icon =
else
with_win_icon = yes
endif
endif

ifeq ($(subst default,undefined,$(origin with_zip)),undefined)
ifeq ($(subst -pe-,--,$(subst -msys-,--,$(subst -os2-,--,$(subst -cygwin32-,--,$(subst -mingw32msvc-,--,$(subst -mingw32-,--,$(subst -cygwin-,--,$(subst -mingw-,--,$(subst -msdosdjgpp-,--,$(subst -msdos-,--,-${host}-)))))))))),-${host}-)
with_zip =
else
with_zip = yes
endif
endif

## variable substitution

# names of variables to subtitute
substitute_vars = \
MYMAN \
MYMANVERSION \
MYMANWEBSITE \
MYMANCOPYRIGHT \
host \
date \
isodate \
DIST \
BINDIST \
MYMANSIZES \
MYMANVARIANTS \
CURSES_FLAVOR \
CURSES_FLAVOR_TARGET \
CURSES_FLAVOR_SECTION \
SHELL \
DESTDIR \
${dir_vars} \
HTM_SUFFIX \
TXT_SUFFIX \
EXE_SUFFIX \
SH_SUFFIX \
SHELL \
MYMAN_CT \
MYMAN_EXE \
DIST_EXE \
XMYMAN_EXE \
XMYMAN2_EXE \
XMYMAN3_EXE \
XMYMAN4_EXE \
XBIGMAN_EXE \
XHUGEMAN_EXE \
XBITMAN_EXE \
XBITMAN2_EXE \
XBITMAN3_EXE \
XBITMAN4_EXE \
XBITMAN5_EXE \
XBITMAN6_EXE \
XQUACKMAN_EXE \
MYMANCOMMAND_EXE \
MYMAN_CT_EXE \
this_file_undergoes_variable_substitution

# special support for NeXTSTEP-style application bundles

ifeq (yes,${with_mac})

public_dir_vars += \
appdir

appdir_MYMAN_APP = ${appdir}/${MYMAN_APP}
appdir_MYMAN_APP_CONTENTS = ${appdir_MYMAN_APP}/Contents
appdir_MYMAN_APP_CONTENTS_PLATFORM = ${appdir_MYMAN_APP_CONTENTS}/${PLATFORM}
appdir_MYMAN_APP_CONTENTS_RESOURCES = ${appdir_MYMAN_APP_CONTENTS}/Resources

${DIST}_dir_vars += \
appdir_MYMAN_APP \
appdir_MYMAN_APP_CONTENTS \
appdir_MYMAN_APP_CONTENTS_PLATFORM \
appdir_MYMAN_APP_CONTENTS_RESOURCES

makeloop_vars += \
MYMAN_APP \
PLATFORM

endif

# finalize values of all substituted variables
$(eval \
$(foreach substitute_var,${substitute_vars},${substitute_var} ${char_colon}${char_equals_sign} ${char_dollar_sign}{${substitute_var}}${char_newline}))

# The substitution language is as follows for troff/groff manpages:

# @VARIABLE@ -> value of substituted variable ${VARIABLE}
#    ' ' (space) in value of variable -> '\ ' (reverse solidus, space)
#    '"' in value of variable -> '\(dq'
#    '-' in value of variable -> '\-'
#    '.' in value of variable -> '\.'
#    '\' in value of variable -> '\e'
# @VARIABLE[*]@ -> value of substituted variable ${VARIABLE}
#    as above, except words of ${VARIABLE} are separated by unescaped spaces
# @@ or unpaired @ alone on a line -> literal @
# @something else@ -> @something else

# substitution rule for troff/groff manpages
ifeq ($(subst default,undefined,$(origin TROFF_SUBSTITUTE)),undefined)
TROFF_SUBSTITUTE = \
${ECHOLINEX} creating $(call q,$@) from $(call q,$<) && \
    ${SED} ${SEDFLAGS} -e 's|@\([^@]*\)@|@(\1)@|g;$(foreach var,${substitute_vars},s|@(${var})@|'$(call q,$(subst &,\&,$(subst |,\|,$(subst ${char_space},\\${char_space},$(subst -,\\-,$(subst ${char_quotation_mark},\\${char_left_parenthesis}dq,$(subst .,\.,$(subst \,\\e,$(call cq,$(${var}))))))))))'|g; s|@(${var}\[\*\])@|'$(call q,$(foreach word,$(${var}),$(subst &,\&,$(subst |,\|,$(subst ${char_space},\\${char_space},$(subst -,\\-,$(subst ${char_quotation_mark},\\${char_left_parenthesis}dq,$(subst .,\.,$(subst \,\\e,$(call cq,${word}))))))))))'|g;)s|@(\([^@]*\))@|@\1|g; ' < $(call q,$<) > $(call q,$@) || \
    ( \
        ${REMOVE} $(call q,$@) && \
            exit 1 \
    )
endif

# substitution rules for shell scripts
ifeq ($(subst default,undefined,$(origin SH_SUBSTITUTE)),undefined)
SH_SUBSTITUTE = \
${ECHOLINEX} creating $(call q,$@) from $(call q,$<) && \
    ${SED} ${SEDFLAGS} -e 's|@\([^@]*\)@|@(\1)@|g;$(foreach var,${substitute_vars},s|'$(call q,@(${var})@)'|'$(call q,$(subst &,\&,$(subst |,\|,$(subst \,\\,$(call q,$(${var}))))))'|g;)s|@(\([^@]*\))@|@\1|g; ' < $(call q,$<) > $(call q,$@) || \
    ( \
        ${REMOVE} $(call q,$@) && \
            exit 1 \
    )
endif

## Tiles and Sprites (a.k.a. "Sizes")
TILEFILE_big = chr5x2.txt
SPRITEFILE_big = spr7x3.txt
TILEFILE_perq = prq28.txt
SPRITEFILE_perq = pqs48.txt
TILEFILE_perq2 = prq28h.txt
SPRITEFILE_perq2 = pqs48h.txt
TILEFILE_perq3 = prq14.txt
SPRITEFILE_perq3 = pqs24.txt
TILEFILE_perq4 = prq14h.txt
SPRITEFILE_perq4 = pqs24h.txt
TILEFILE_perq5 = prq7.txt
SPRITEFILE_perq5 = pqs12.txt
TILEFILE_bitmap1 = chr32h.txt
SPRITEFILE_bitmap1 = spr64h.txt
TILEFILE_bitmap0 = chr32.txt
SPRITEFILE_bitmap0 = spr64.txt
TILEFILE_bitmap9 = chr2hb.txt
SPRITEFILE_bitmap9 = spr4h.txt
TILEFILE_bitmap9a = chr2hc.txt
SPRITEFILE_bitmap9a = spr4hb.txt
TILEFILE_bitmapa = chr16.txt
SPRITEFILE_bitmapa = spr32.txt
TILEFILE_bitmapb = chr16h.txt
SPRITEFILE_bitmapb = spr32h.txt
TILEFILE_bitmap8 = chr2.txt
SPRITEFILE_bitmap8 = spr4.txt
TILEFILE_bitmap8a = chr2b.txt
SPRITEFILE_bitmap8a = spr4b.txt
TILEFILE_bitmap7 = chr3.txt
SPRITEFILE_bitmap7 = spr6.txt
TILEFILE_bitmap7a = chr3b.txt
SPRITEFILE_bitmap7a = spr6b.txt
TILEFILE_bitmap6 = chr4h.txt
SPRITEFILE_bitmap6 = spr8h.txt
TILEFILE_bitmap6a = chr4hb.txt
SPRITEFILE_bitmap6a = spr8hb.txt
TILEFILE_bitmap5 = chr4.txt
SPRITEFILE_bitmap5 = spr8.txt
TILEFILE_bitmap5a = chr4b.txt
SPRITEFILE_bitmap5a = spr8b.txt
TILEFILE_bitmap4 = chr6h.txt
SPRITEFILE_bitmap4 = spr12h.txt
TILEFILE_bitmap3 = chr6.txt
SPRITEFILE_bitmap3 = spr12.txt
TILEFILE_bitmap3a = chr5.txt
SPRITEFILE_bitmap3a = spr10.txt
TILEFILE_bitmap2 = chr8h.txt
SPRITEFILE_bitmap2 = spr16h.txt
TILEFILE_bitmap = chr8.txt
SPRITEFILE_bitmap = spr16.txt
TILEFILE_kpac = khr8.txt
SPRITEFILE_kpac = spr16.txt
TILEFILE_kpac2 = khr8h.txt
SPRITEFILE_kpac2 = spr16h.txt
TILEFILE_kpac3 = khr6.txt
SPRITEFILE_kpac3 = spr12.txt
TILEFILE_kpac3a = khr5.txt
SPRITEFILE_kpac3a = spr10.txt
TILEFILE_kpac4 = khr6h.txt
SPRITEFILE_kpac4 = spr12h.txt
TILEFILE_kpac5 = khr4.txt
SPRITEFILE_kpac5 = spr8b.txt
TILEFILE_kpac6 = khr4h.txt
SPRITEFILE_kpac6 = spr8hb.txt
TILEFILE_kpac7 = khr3.txt
SPRITEFILE_kpac7 = spr6b.txt
TILEFILE_kpac8 = khr2.txt
SPRITEFILE_kpac8 = spr4b.txt
TILEFILE_kpac9 = khr2hb.txt
SPRITEFILE_kpac9 = spr4hb.txt
TILEFILE_otto = khr8.txt
SPRITEFILE_otto = ott16.txt
TILEFILE_otto2 = khr8h.txt
SPRITEFILE_otto2 = ott16h.txt
TILEFILE_otto3 = khr6.txt
SPRITEFILE_otto3 = ott12.txt
TILEFILE_otto3a = khr5.txt
SPRITEFILE_otto3a = ott10.txt
TILEFILE_otto4 = khr6h.txt
SPRITEFILE_otto4 = ott12h.txt
TILEFILE_huge = chr5x3.txt
SPRITEFILE_huge = spr10x6.txt
TILEFILE_square = khr1.txt
SPRITEFILE_square = spr1.txt
TILEFILE_small = khr2h.txt
SPRITEFILE_small = spr2h.txt
TILEFILE_quack = khr2h.txt
SPRITEFILE_quack = quack2h.txt
TILEFILE_custom = khr1b.txt
SPRITEFILE_custom = spr3.txt
TILEFILE_rogue = rogue.txt
SPRITEFILE_rogue = rogues.txt

# Tile and sprite definitions for the selected size
tilefile = ${src}chr/$(firstword $(TILEFILE_$1) $1.txt)
tiledefs = -DTILEFILE=\"$(call qcq,$(call tilefile,$1))\"

spritefile = ${src}spr/$(firstword $(SPRITEFILE_$1) $1.txt)
spritedefs = -DSPRITEFILE=\"$(call qcq,$(call spritefile,$1))\"

## Mazes
MAZEFILE_myman = maze.txt

# Maze definitions for the selected variant
mazefile = ${src}lvl/$(firstword $(MAZEFILE_$1) $1.txt)
mazedefs = -DMAZEFILE=\"$(call qcq,$(call mazefile,$1))\"

# Definitions for compiling the selected variant at the selected size
gamedefs = $(call tiledefs,$2) $(call spritedefs,$2) $(call mazedefs,$1) ${EXTRAGAMEDEFS}

# C preprocessor options for wrapper executable
ifeq ($(subst default,undefined,$(origin WRAPPERDEFS)),undefined)
WRAPPERDEFS =
endif

WRAPPERDEFS += ${EXTRAWRAPPERDEFS}

# C preprocessor options for built-in games
ifeq ($(subst default,undefined,$(origin BUILTIN_VARIANTS)),undefined)
BUILTIN_VARIANTS = $(subst ${char_space},${char_space}${char_reverse_solidus}${char_newline},$(foreach variant,${MYMANVARIANTS},BUILTIN_VARIANT(${variant})))
endif
ifeq ($(subst default,undefined,$(origin BUILTIN_SIZES)),undefined)
BUILTIN_SIZES = $(subst ${char_space},${char_space}${char_reverse_solidus}${char_newline},$(foreach size,${MYMANSIZES},BUILTIN_SIZE(${size})))
endif
ifeq ($(subst default,undefined,$(origin BUILTINDEFS)),undefined)
BUILTINDEFS =
endif

BUILTINDEFS += ${EXTRABUILTINDEFS}

# configuration variables to write to config.h
ifeq ($(subst default,undefined,$(origin configvars)),undefined)
configvars = BUILTIN_VARIANTS BUILTIN_SIZES
endif

##
## Targets
##

# run other sanity checks except during script generation

ifeq (,${script_target})

all install uninstall clean bindist:: gnumake.ok configure.ok

all install:: $(call mw,${obj}compiler.ok)

all uninstall:: coreutils.ok sed.ok

endif

# the coreutils sanity check should be included in the installation
# script, since the installation script uses ${INSTALL}

install:: coreutils.ok

.SUFFIXES:

.PHONY: coreutils.ok sed.ok

# FIXME: at the moment our use of makefile recursion is not
# parallel-safe

.NOTPARALLEL: all

# this rule is depended upon by the principal targets to verify that
# ${SED} works; its value is not cached
sed.ok: $(call mw,${MAKEFILE})
	@(export sedflag bogusflag; \
        sedflag='silent'; \
        bogusflag='not-a-valid-option'; \
        if ${SHELL} -c $(call q,${SED})' --$$sedflag --help $$bogusflag | grep -v $$bogusflag | grep $$sedflag' </dev/null >/dev/null; \
        then \
            : ; \
        else \
            echo $@: sed check failed >&2; \
            echo '** ' make sure GNU \`sed\' is installed, >&2; \
            echo '** ' and either put it first in your PATH or set the SED >&2; \
            echo '** ' makefile variable to contain its full path and >&2; \
            echo '** ' recompile >&2; \
            exit 1; \
        fi)

# this rule is depended upon by the principal targets to verify that
# ${INSTALL} and ${RMDIR} work; its value is not cached; the "exit 1"
# here is unparenthesized intentionally, so that it will terminate an
# installation script
coreutils.ok: $(call mw,${MAKEFILE})
	${POST_UNPACK}
	@export installflag rmdirflag bogusflag; \
        installflag='strip'; \
        rmdirflag='ignore-fail-on-non-empty'; \
        bogusflag='not-a-valid-option'; \
        if ${SHELL} -c $(call q,${INSTALL})' --$$installflag --help $$bogusflag | grep -v $$bogusflag | grep $$installflag' </dev/null >/dev/null && \
           ${SHELL} -c $(call q,${RMDIR})' --$$rmdirflag --help $$bogusflag | grep -v $$bogusflag | grep $$rmdirflag' </dev/null >/dev/null; \
        then \
            : ; \
        else \
            echo $@: coreutils check failed >&2; \
            echo '** ' make sure \`install\' and \`rmdir\' from GNU coreutils are installed, >&2; \
            echo '** ' and either put them first in your PATH or set the INSTALL and RMDIR >&2; \
            echo '** ' makefile variables, respectively, to contain their full paths and >&2; \
            echo '** ' recompile >&2; \
            exit 1; \
        fi
	${NORMAL_UNPACK}

.PHONY: gnumake.ok

# test 1: in some older GNU Make versions, ?= doesn't work correctly
gnumake_ok_test_1_indirect := failed
ifeq ($(subst default,undefined,$(origin gnumake_ok_test_1)),undefined)
gnumake_ok_test_1 = ${gnumake_ok_test_1_indirect}
endif
gnumake_ok_test_1_indirect := ok

# test 2: in some older GNU Make versions, $(call ...) is not implemented
gnumake_ok_test_2_function = $1
gnumake_ok_test_2 := $(call gnumake_ok_test_2_function,ok)

# test 3: in some older GNU Make versions, $(call q,...) does not work
gnumake_ok_test_3 := $(patsubst \$$test,ok,$(call q,$$test))

ifeq ($(origin NOP),undefined)
ifeq ($(subst default,undefined,$(origin NOP)),undefined)
NOP = @:
endif
endif

# this rule is depended upon by the principal targets to perform a
# sanity check of GNU Make; its value is not cached
gnumake.ok: $(call mw,${MAKEFILE})
ifneq (${gnumake_ok_test_1},ok)
	@echo 'test 1: recursive variable expansion with ?= failed -- please upgrade your Make to e.g. ${gnumake_ok_tested_version}' >&2; \
            echo '** if you want to build anyway, see simple.mk' >&2; \
            exit 1
endif
ifneq (${gnumake_ok_test_2},ok)
	@echo 'test 2: $$(call ...) failed -- please upgrade your Make to e.g. ${gnumake_ok_tested_version}' >&2; \
            echo '** if you want to build anyway, see simple.mk' >&2; \
            exit 1
endif
ifneq (${gnumake_ok_test_3},ok)
	@echo 'test 3: $$(call q,...) failed -- please upgrade your Make to e.g. ${gnumake_ok_tested_version}' >&2; \
            echo '** if you want to build anyway, see simple.mk' >&2; \
            exit 1
endif
	${NOP}

.PHONY: configure.ok

# this rule is depended upon by the principal targets to perform a
# sanity check of the configure system; its value is not cached
configure.ok: $(call mw,${MAKEFILE})
	@(if test :$(call qs,$(call xq,unknown),${nil},${date}) != :${nil}; \
        then \
            :; \
        else \
            echo $@: configure check failed >&2; \
            ${ECHOLINEX} '** ' $(call q,${SHELL} $(call q,${src})configure --dump=date) failed, >&2; \
            echo '** ' your shell might not work\; see INSTALL for hints >&2; \
            exit 1; \
        fi)

# this rule is depended upon by the principal targets to perform a
# sanity check of the compiler and linker for the build machine
${obj}compiler.ok: $(call mw,${MAKEFILE})
	@${MAKE} ${MAKELOOP} \
            sanity_return=0 \
            _sanity0$X
	@ ./_sanity0
	@${REMOVE} $(call q,${obj}_sanity0.c) _sanity0 $(call q,${obj}_sanity0$O)
	@${MAKE} ${MAKELOOP} \
            sanity_return=1 \
            _sanity1$X
	@( ./_sanity1 && exit 1 || : )
	@${REMOVE} $(call q,${obj}_sanity1.c) _sanity1 $(call q,${obj}_sanity1$O)
	@${ECHOLINEX} your compiler appears to work, so creating $(call q,$@)
	@${ECHOLINEX} You may safely remove this file with \`$(call q,${REMOVE} $@)\'. > $(call q,$@)

ifneq (,${sanity_return})

${obj}_sanity${sanity_return}.c: $(call mw,${MAKEFILE})
	@${MKPARENTDIR}
	@(${ECHOLINEX} '/* '$(call q,$(call ccq,$@))': compiler sanity check */'; \
        echo '/* you may safely delete this file */'; \
        echo 'int main(int argc, char **argv) { return '$(call q,${sanity_return})'; }') > $(call q,$@)

${obj}_sanity${sanity_return}$O: $(call mw,${obj}_sanity${sanity_return}.c)
	@${MKPARENTDIR}
	@${COMPILE}

_sanity${sanity_return}$X: $(call mw,${obj}_sanity${sanity_return}$O)
	@${LINK} ${LIBS}

endif

.PHONY: all clean install uninstall
.PHONY: installdirs uninstalldirs
.PHONY: install-files uninstall-files
.PHONY: install-strip

install-strip: $(call mw,${MAKEFILE})
	${MAKE} ${MAKELOOP} \
            STRIPINSTALLFLAGS=$(call qmq,${STRIPINSTALLFLAGS} -s) \
            install

.PHONY: distclean mostlyclean maintainer-clean

distclean::
	-${REMOVE} $(call q,${obj}compiler.ok)
	@${MAKE} ${MAKELOOP} \
            uninstall-optional-dir-xq-$(call qxq,${objdir})

distclean mostlyclean:: clean

maintainer-clean:: distclean wipe-dir-xq-$(call mwxq,${CVSDUMP})

pdcicon.bmp: ${src}gfx/myman.png
	@-${CONVERT_TO_BMP}

myman_ico_deps = ${src}gfx/myman64.png ${src}gfx/myman48.png ${src}gfx/myman32.png

${obj}myman.ico: $(foreach dep,${myman_ico_deps},$(call mw,${dep}))
	@-${ECHOLINEX} creating $(call q,$@) from $(call q,${myman_ico_deps}) && \
        ( $(foreach dep,${myman_ico_deps},${ANYTOPNM} $(call q,${dep});) ) | ${PPMTOWINICON} $(foreach dep,${myman_ico_deps},-) > $(call q,$@) || \
            ( \
                ${REMOVE} $(call q,$@) ; \
                exit 1 \
            )

doc_files = \
ChangeLog \
AUTHORS \
INSTALL \
LICENSE \
README \
THANKS \
TODO \
NEWS \
ONEWS \
VERSION \
COPYRIGHT

dist_program_files = \
configure \
src/fnt2bdf.c \
src/myman.c \
utl/asc2txt.py \
utl/bitmap2.py \
utl/bitmap4.py \
utl/cpmbuild.sh \
utl/lsic.sh \
utl/macbuild.txt \
utl/myman.ct \
utl/pacificc.sh \
utl/turboc.sh \
utl/aztecc.sh \
utl/txt2asc.sh \
debian/postinst \
debian/postrm \
debian/rules \
debian/myman.spec

dist_data_files = \
${doc_files} \
.cvsignore \
Makefile \
chr/chr1.asc \
chr/chr1.txt \
chr/chr16.asc \
chr/chr16.txt \
chr/chr16h.asc \
chr/chr16h.txt \
chr/chr1b.txt \
chr/chr2.asc \
chr/chr2.txt \
chr/chr2b.asc \
chr/chr2b.txt \
chr/chr2h.asc \
chr/chr2h.txt \
chr/chr2hb.asc \
chr/chr2hb.txt \
chr/chr2hc.asc \
chr/chr2hc.txt \
chr/chr3.asc \
chr/chr3.txt \
chr/chr32.asc \
chr/chr32.png \
chr/chr32.txt \
chr/chr32h.asc \
chr/chr32h.txt \
chr/chr3b.asc \
chr/chr3b.txt \
chr/chr4.asc \
chr/chr4.txt \
chr/chr4b.asc \
chr/chr4b.txt \
chr/chr4h.asc \
chr/chr4h.txt \
chr/chr4hb.asc \
chr/chr4hb.txt \
chr/chr5.asc \
chr/chr5.txt \
chr/chr5x2.txt \
chr/chr5x3.txt \
chr/chr6.asc \
chr/chr6.png \
chr/chr6.txt \
chr/chr6h.asc \
chr/chr6h.txt \
chr/chr8.afn \
chr/chr8.asc \
chr/chr8.bdf \
chr/chr8.fnt \
chr/chr8.png \
chr/chr8.txt \
chr/chr8h.asc \
chr/chr8h.txt \
chr/khr1.asc \
chr/khr1.txt \
chr/khr1b.txt \
chr/khr2.asc \
chr/khr2.txt \
chr/khr2h.asc \
chr/khr2h.txt \
chr/khr2hb.asc \
chr/khr2hb.txt \
chr/khr3.asc \
chr/khr3.txt \
chr/khr4.asc \
chr/khr4.txt \
chr/khr4h.asc \
chr/khr4h.txt \
chr/khr5.asc \
chr/khr5.txt \
chr/khr5x2.txt \
chr/khr6.asc \
chr/khr6.txt \
chr/khr6h.asc \
chr/khr6h.txt \
chr/khr8.asc \
chr/khr8.txt \
chr/khr8h.asc \
chr/khr8h.txt \
chr/notile.txt \
chr/prq14.asc \
chr/prq14.txt \
chr/prq14h.asc \
chr/prq14h.txt \
chr/prq28.asc \
chr/prq28.txt \
chr/prq28h.asc \
chr/prq28h.txt \
chr/prq7.asc \
chr/prq7.txt \
chr/rogue.txt \
game.html \
gfx/monster.gif \
gfx/monster.xcf \
gfx/monsterleft.gif \
gfx/myman.png \
gfx/myman.svg \
gfx/myman32.png \
gfx/myman48.png \
gfx/myman64.png \
gfx/otto.gif \
gfx/otto.xcf \
gfx/ottoup.gif \
inc/aacurses.h \
inc/allegcur.h \
inc/cacacurs.h \
inc/coniocur.h \
inc/dispcurs.h \
inc/fltkcurs.h \
inc/ggicurs.h \
inc/gtkcurs.h \
inc/guess.h \
inc/maccurs.h \
inc/mycurses.h \
inc/newtcurs.h \
inc/optcurs.h \
inc/rawcurs.h \
inc/sdlcurs.h \
inc/twcurses.h \
inc/utils.h \
lvl/3b1.asc \
lvl/3b1.txt \
lvl/3demon.asc \
lvl/3demon.txt \
lvl/3dpac.asc \
lvl/3dpac.txt \
lvl/ababa.asc \
lvl/ababa.txt \
lvl/ackman.asc \
lvl/ackman.txt \
lvl/alien.asc \
lvl/alien.txt \
lvl/apollo.asc \
lvl/apollo.txt \
lvl/armpac2.asc \
lvl/armpac2.txt \
lvl/arrange.asc \
lvl/arrange.txt \
lvl/asciiman.asc \
lvl/asciiman.txt \
lvl/ascpac.asc \
lvl/ascpac.txt \
lvl/baby2.asc \
lvl/baby2.txt \
lvl/babypac.asc \
lvl/babypac.txt \
lvl/blinky.asc \
lvl/blinky.txt \
lvl/blobbo.asc \
lvl/blobbo.txt \
lvl/bugman.asc \
lvl/bugman.txt \
lvl/byh.asc \
lvl/byh.txt \
lvl/catchum.asc \
lvl/catchum.txt \
lvl/cattrax.asc \
lvl/cattrax.txt \
lvl/cgkit.asc \
lvl/cgkit.txt \
lvl/chicky.asc \
lvl/chicky.txt \
lvl/chomp.asc \
lvl/chomp.txt \
lvl/chompman.asc \
lvl/chompman.txt \
lvl/classmun.asc \
lvl/classmun.txt \
lvl/clsweep.asc \
lvl/clsweep.txt \
lvl/crazyg.asc \
lvl/crazyg.txt \
lvl/crush.asc \
lvl/crush.txt \
lvl/cupcake.asc \
lvl/cupcake.txt \
lvl/dacman.asc \
lvl/dacman.txt \
lvl/deluxe.asc \
lvl/deluxe.txt \
lvl/deluxe2.asc \
lvl/deluxe2.txt \
lvl/demon.asc \
lvl/demon.txt \
lvl/eatit.asc \
lvl/eatit.txt \
lvl/ebipac.asc \
lvl/ebipac.txt \
lvl/fhmcpac.asc \
lvl/fhmcpac.txt \
lvl/ghost.asc \
lvl/ghost.txt \
lvl/ghost2.asc \
lvl/ghost2.txt \
lvl/ghostma2.asc \
lvl/ghostma2.txt \
lvl/ghostman.asc \
lvl/ghostman.txt \
lvl/ghunt.asc \
lvl/ghunt.txt \
lvl/glooper.asc \
lvl/glooper.txt \
lvl/gnasher.asc \
lvl/gnasher.txt \
lvl/gobble.asc \
lvl/gobble.txt \
lvl/gobble2.asc \
lvl/gobble2.txt \
lvl/gobbleag.asc \
lvl/gobbleag.txt \
lvl/gobbler.asc \
lvl/gobbler.txt \
lvl/gobbler0.asc \
lvl/gobbler0.txt \
lvl/gobbler1.asc \
lvl/gobbler1.txt \
lvl/gobbler2.asc \
lvl/gobbler2.txt \
lvl/gobbler3.asc \
lvl/gobbler3.txt \
lvl/gobbler4.asc \
lvl/gobbler4.txt \
lvl/gpac.asc \
lvl/gpac.txt \
lvl/greedy.asc \
lvl/greedy.txt \
lvl/gsmall.asc \
lvl/gsmall.txt \
lvl/guimp.asc \
lvl/guimp.txt \
lvl/guppy.asc \
lvl/guppy.txt \
lvl/hackem.asc \
lvl/hackem.txt \
lvl/hackem2.asc \
lvl/hackem2.txt \
lvl/hangly.asc \
lvl/hangly.txt \
lvl/hangly26.asc \
lvl/hangly26.txt \
lvl/hannah.asc \
lvl/hannah.txt \
lvl/hasewolf.asc \
lvl/hasewolf.txt \
lvl/hatman.asc \
lvl/hatman.txt \
lvl/hedges.asc \
lvl/hedges.txt \
lvl/iitpac.asc \
lvl/iitpac.txt \
lvl/ipac.asc \
lvl/ipac.txt \
lvl/javapac.asc \
lvl/javapac.txt \
lvl/jellymon.asc \
lvl/jellymon.txt \
lvl/jrpac.asc \
lvl/jrpac.txt \
lvl/jrpac26.asc \
lvl/jrpac26.txt \
lvl/jrpac52.asc \
lvl/jrpac52.txt \
lvl/jrpac64.asc \
lvl/jrpac64.txt \
lvl/jrsmall.asc \
lvl/jrsmall.txt \
lvl/jrtiny.asc \
lvl/jrtiny.txt \
lvl/k3.asc \
lvl/k3.txt \
lvl/kasper.asc \
lvl/kasper.txt \
lvl/kcm.asc \
lvl/kcm.txt \
lvl/kpacman.asc \
lvl/kpacman.txt \
lvl/lives.asc \
lvl/lives.txt \
lvl/makman.asc \
lvl/makman.txt \
lvl/mania.asc \
lvl/mania.txt \
lvl/maze.asc \
lvl/maze.txt \
lvl/mazeman.asc \
lvl/mazeman.txt \
lvl/mazewar.asc \
lvl/mazewar.txt \
lvl/mc10.asc \
lvl/mc10.txt \
lvl/mini.asc \
lvl/mini.txt \
lvl/minipac.asc \
lvl/minipac.txt \
lvl/misspac.asc \
lvl/misspac.txt \
lvl/misstiny.asc \
lvl/misstiny.txt \
lvl/mmpalm.asc \
lvl/mmpalm.txt \
lvl/mmproto.asc \
lvl/mmproto.txt \
lvl/monster.asc \
lvl/monster.txt \
lvl/mrpac26.asc \
lvl/mrpac26.txt \
lvl/msblitz.asc \
lvl/msblitz.txt \
lvl/mschomp.asc \
lvl/mschomp.txt \
lvl/mshack.asc \
lvl/mshack.txt \
lvl/mspac20.asc \
lvl/mspac20.txt \
lvl/mspac26.asc \
lvl/mspac26.txt \
lvl/mspac4a.asc \
lvl/mspac4a.txt \
lvl/mspac78.asc \
lvl/mspac78.txt \
lvl/mspacatk.asc \
lvl/mspacatk.txt \
lvl/mspaccdi.asc \
lvl/mspaccdi.txt \
lvl/mspaccpc.asc \
lvl/mspaccpc.txt \
lvl/mspacgb.asc \
lvl/mspacgb.txt \
lvl/mspacgg.asc \
lvl/mspacgg.txt \
lvl/mspacgt.asc \
lvl/mspacgt.txt \
lvl/mspacln2.asc \
lvl/mspacln2.txt \
lvl/mspaclnx.asc \
lvl/mspaclnx.txt \
lvl/mspacnes.asc \
lvl/mspacnes.txt \
lvl/mspaczx.asc \
lvl/mspaczx.txt \
lvl/msport.asc \
lvl/msport.txt \
lvl/mspp.asc \
lvl/mspp.txt \
lvl/mspp2.txt \
lvl/mspwlnx.asc \
lvl/mspwlnx.txt \
lvl/mspwlnx2.asc \
lvl/mspwlnx2.txt \
lvl/mssmall.asc \
lvl/mssmall.txt \
lvl/mstiny.asc \
lvl/mstiny.txt \
lvl/muncher.asc \
lvl/muncher.txt \
lvl/munchie.asc \
lvl/munchie.txt \
lvl/munchkin.asc \
lvl/munchkin.txt \
lvl/munchman.asc \
lvl/munchman.txt \
lvl/munchy.asc \
lvl/munchy.txt \
lvl/nascom.asc \
lvl/nascom.txt \
lvl/nespac.asc \
lvl/nespac.txt \
lvl/newpuc2.asc \
lvl/newpuc2.txt \
lvl/nibble.asc \
lvl/nibble.txt \
lvl/nokiapac.asc \
lvl/nokiapac.txt \
lvl/nopoint.asc \
lvl/nopoint.txt \
lvl/ohno.asc \
lvl/ohno.txt \
lvl/p4c.asc \
lvl/p4c.txt \
lvl/pac.asc \
lvl/pac.txt \
lvl/pac188.asc \
lvl/pac188.txt \
lvl/pac2.txt \
lvl/pac20.asc \
lvl/pac20.txt \
lvl/pac26.asc \
lvl/pac26.txt \
lvl/pac2600.asc \
lvl/pac2600.txt \
lvl/pac49.asc \
lvl/pac49.txt \
lvl/pac4a.asc \
lvl/pac4a.txt \
lvl/pac6001.asc \
lvl/pac6001.txt \
lvl/pac64.asc \
lvl/pac64.txt \
lvl/pac700.asc \
lvl/pac700.txt \
lvl/pac8bit.asc \
lvl/pac8bit.txt \
lvl/pacbox.asc \
lvl/pacbox.txt \
lvl/paccdi.asc \
lvl/paccdi.txt \
lvl/pacclone.asc \
lvl/pacclone.txt \
lvl/paccv.asc \
lvl/paccv.txt \
lvl/pacex2.asc \
lvl/pacex2.txt \
lvl/pacgal.asc \
lvl/pacgal.txt \
lvl/pacgalax.asc \
lvl/pacgalax.txt \
lvl/pacgem.asc \
lvl/pacgem.txt \
lvl/pacgent.asc \
lvl/pacgent.txt \
lvl/pacjr.asc \
lvl/pacjr.txt \
lvl/pacjs.asc \
lvl/pacjs.txt \
lvl/packman.asc \
lvl/packman.txt \
lvl/packri.asc \
lvl/packri.txt \
lvl/pacman2.asc \
lvl/pacman2.txt \
lvl/pacman89.asc \
lvl/pacman89.txt \
lvl/pacman98.asc \
lvl/pacman98.txt \
lvl/pacmania.asc \
lvl/pacmania.txt \
lvl/pacmanic.asc \
lvl/pacmanic.txt \
lvl/pacmanpc.asc \
lvl/pacmanpc.txt \
lvl/pacmanvs.asc \
lvl/pacmanvs.txt \
lvl/pacmanx.asc \
lvl/pacmanx.txt \
lvl/pacmen.asc \
lvl/pacmen.txt \
lvl/pacmes.asc \
lvl/pacmes.txt \
lvl/pacmsx.asc \
lvl/pacmsx.txt \
lvl/pacmunch.asc \
lvl/pacmunch.txt \
lvl/pacnstuf.asc \
lvl/pacnstuf.txt \
lvl/pacpdp11.asc \
lvl/pacpdp11.txt \
lvl/pacpsion.asc \
lvl/pacpsion.txt \
lvl/pacpunk.asc \
lvl/pacpunk.txt \
lvl/pacrt11.asc \
lvl/pacrt11.txt \
lvl/pacst.asc \
lvl/pacst.txt \
lvl/pactac.asc \
lvl/pactac.txt \
lvl/pacvms.asc \
lvl/pacvms.txt \
lvl/pacxgs.asc \
lvl/pacxgs.txt \
lvl/paczx.asc \
lvl/paczx.txt \
lvl/paczx2.asc \
lvl/paczx2.txt \
lvl/pakkman.asc \
lvl/pakkman.txt \
lvl/pcman.asc \
lvl/pcman.txt \
lvl/pcman2.asc \
lvl/pcman2.txt \
lvl/pcmania4.asc \
lvl/pcmania4.txt \
lvl/pcsmall.asc \
lvl/pcsmall.txt \
lvl/peerless.asc \
lvl/peerless.txt \
lvl/pepperii.asc \
lvl/pepperii.txt \
lvl/perqman.asc \
lvl/perqman.txt \
lvl/pesco.asc \
lvl/pesco.txt \
lvl/plaque.asc \
lvl/plaque.txt \
lvl/plaque2.asc \
lvl/plaque2.txt \
lvl/pman.asc \
lvl/pman.txt \
lvl/pman2.asc \
lvl/pman2.txt \
lvl/portable.asc \
lvl/portable.txt \
lvl/psppac.asc \
lvl/psppac.txt \
lvl/ptmx.asc \
lvl/ptmx.txt \
lvl/puckmon.asc \
lvl/puckmon.txt \
lvl/puckpock.asc \
lvl/puckpock.txt \
lvl/qman.asc \
lvl/qman.txt \
lvl/quackman.asc \
lvl/quackman.txt \
lvl/remake.asc \
lvl/remake.txt \
lvl/revenge.asc \
lvl/revenge.txt \
lvl/ruckman.asc \
lvl/ruckman.txt \
lvl/salomon.asc \
lvl/salomon.txt \
lvl/scandal.asc \
lvl/scandal.txt \
lvl/scarfman.asc \
lvl/scarfman.txt \
lvl/scheme.asc \
lvl/scheme.txt \
lvl/sidtool.asc \
lvl/sidtool.txt \
lvl/simplej.asc \
lvl/simplej.txt \
lvl/sir.asc \
lvl/sir.txt \
lvl/small.asc \
lvl/small.txt \
lvl/small188.asc \
lvl/small188.txt \
lvl/smallpac.asc \
lvl/smallpac.txt \
lvl/smartpac.asc \
lvl/smartpac.txt \
lvl/snail.asc \
lvl/snail.txt \
lvl/snapper.asc \
lvl/snapper.txt \
lvl/snespac.asc \
lvl/snespac.txt \
lvl/spear.asc \
lvl/spear.txt \
lvl/spook.asc \
lvl/spook.txt \
lvl/spooky.asc \
lvl/spooky.txt \
lvl/strange.asc \
lvl/strange.txt \
lvl/sympac.asc \
lvl/sympac.txt \
lvl/thief.asc \
lvl/thief.txt \
lvl/tiny.asc \
lvl/tiny.txt \
lvl/tinypac.asc \
lvl/tinypac.txt \
lvl/tinypac2.asc \
lvl/tinypac2.txt \
lvl/titanman.asc \
lvl/titanman.txt \
lvl/tituspac.asc \
lvl/tituspac.txt \
lvl/tituspc2.txt \
lvl/tman.asc \
lvl/tman.txt \
lvl/tomyport.asc \
lvl/tomyport.txt \
lvl/toyman.asc \
lvl/toyman.txt \
lvl/ultrapac.asc \
lvl/ultrapac.txt \
lvl/upacthin.txt \
lvl/vermin.asc \
lvl/vermin.txt \
lvl/xensrvng.asc \
lvl/xensrvng.txt \
lvl/xfred.asc \
lvl/xfred.txt \
lvl/yoyopac.asc \
lvl/yoyopac.txt \
lvl/zaurus.asc \
lvl/zaurus.txt \
lvl/zuckman.asc \
lvl/zuckman.txt \
mygetopt/ChangeLog \
mygetopt/getopt.3 \
mygetopt/getopt.h \
mygetopt/getopt.txt \
mygetopt/LICENSE \
mygetopt/main.c \
mygetopt/Makefile \
mygetopt/mygetopt.c \
mygetopt/mygetopt.h \
mygetopt/README \
pdcfont.bmp \
pdcfont0.bmp \
pdcfont1.bmp \
sfx/bonus.mid \
sfx/credit.mid \
sfx/dot.mid \
sfx/dot2.mid \
sfx/dying.mid \
sfx/fruit.mid \
sfx/ghost.mid \
sfx/intermission.mid \
sfx/level.mid \
sfx/life.mid \
sfx/pellet.mid \
sfx/siren0_down.mid \
sfx/siren0_up.mid \
sfx/siren1_down.mid \
sfx/siren1_up.mid \
sfx/siren2_down.mid \
sfx/siren2_up.mid \
sfx/start.mid \
sfx/start.xm \
simple.mk \
spr/nosprite.txt \
spr/ott10.asc \
spr/ott10.txt \
spr/ott12.asc \
spr/ott12.txt \
spr/ott12h.asc \
spr/ott12h.txt \
spr/ott16.asc \
spr/ott16.png \
spr/ott16.txt \
spr/ott16h.asc \
spr/ott16h.txt \
spr/pqs12.asc \
spr/pqs12.txt \
spr/pqs24.asc \
spr/pqs24.txt \
spr/pqs24h.asc \
spr/pqs24h.txt \
spr/pqs48.asc \
spr/pqs48.txt \
spr/pqs48h.asc \
spr/pqs48h.txt \
spr/quack1.txt \
spr/quack2h.txt \
spr/rogues.txt \
spr/spr1.txt \
spr/spr10.asc \
spr/spr10.txt \
spr/spr10x6.txt \
spr/spr12.asc \
spr/spr12.png \
spr/spr12.txt \
spr/spr12h.asc \
spr/spr12h.txt \
spr/spr16.asc \
spr/spr16.png \
spr/spr16.txt \
spr/spr16h.asc \
spr/spr16h.txt \
spr/spr2h.txt \
spr/spr3.txt \
spr/spr32.asc \
spr/spr32.txt \
spr/spr32h.asc \
spr/spr32h.txt \
spr/spr4.asc \
spr/spr4.txt \
spr/spr4b.asc \
spr/spr4b.txt \
spr/spr4h.asc \
spr/spr4h.txt \
spr/spr4hb.asc \
spr/spr4hb.txt \
spr/spr6.asc \
spr/spr6.txt \
spr/spr64.asc \
spr/spr64.png \
spr/spr64.txt \
spr/spr64h.asc \
spr/spr64h.txt \
spr/spr6b.asc \
spr/spr6b.txt \
spr/spr7x3.txt \
spr/spr8.asc \
spr/spr8.txt \
spr/spr8b.asc \
spr/spr8b.txt \
spr/spr8h.asc \
spr/spr8h.txt \
spr/spr8hb.asc \
spr/spr8hb.txt \
src/main.c \
src/mycurses.c \
src/myman.man \
src/utils.c \
utl/bigman.term \
utl/bitman.term \
utl/bitman2.term \
utl/bitman3.term \
utl/bitman3a.term \
utl/bitman4.term \
utl/bitman5.term \
utl/bitman6.term \
utl/bitman7.term \
utl/bitman8.term \
utl/hugeman.term \
utl/jrbit.term \
utl/jrbit2.term \
utl/jrbit3.term \
utl/jrbit3a.term \
utl/jrbit4.term \
utl/jrbit5.term \
utl/jrbit6.term \
utl/jrbit7.term \
utl/jrbit8.term \
utl/jrpac.term \
utl/jrpac2.term \
utl/kpacman.term \
utl/kpacman2.term \
utl/lsic.bat \
utl/msbit.term \
utl/msbit2.term \
utl/msbit3.term \
utl/msbit3a.term \
utl/msbit4.term \
utl/msbit5.term \
utl/msbit6.term \
utl/msbit7.term \
utl/msbit8.term \
utl/myman.term \
utl/myman2.term \
utl/mymancmd.in \
utl/pacificc.bat \
utl/quackman.term \
utl/turboc.bat \
utl/aztecc.bat \
utl/xbigman.in \
utl/xbitman.in \
utl/xbitman2.in \
utl/xbitman3.in \
utl/xbitman4.in \
utl/xbitman5.in \
utl/xbitman6.in \
utl/xhugeman.in \
utl/xmyman.in \
utl/xmyman2.in \
utl/xmyman3.in \
utl/xmyman4.in \
utl/xquack.in \
debian/changelog \
debian/control \
fink/myman.info \
${website_files}

website_files = \
website/htdocs/FAF34783A802D362.gpg \
website/htdocs/favicon.ico \
website/htdocs/index.html \
website/htdocs/interface.xsl \
website/htdocs/myman.png \
website/htdocs/myman.xml \
website/htdocs/myman-0.7.0.png \
website/htdocs/snap0000.html \
website/htdocs/snap0000.txt

dist_dirs = \
src \
inc \
mygetopt \
lvl \
chr \
spr \
utl \
sfx \
gfx \
debian \
fink \
${website_dirs}

website_dirs = \
website \
website/cgi-bin \
website/htdocs

dist_files = \
${dist_program_files} \
${dist_data_files}

ifeq ($(subst default,undefined,$(origin DRIVERS)),undefined)
DRIVERS = $(call mw,${src}inc/sdlcurs.h) $(call mw,${src}inc/ggicurs.h) $(call mw,${src}inc/twcurses.h) $(call mw,${src}inc/aacurses.h) $(call mw,${src}inc/allegcur.h) $(call mw,${src}inc/cacacurs.h) $(call mw,${src}inc/rawcurs.h) $(call mw,${src}inc/maccurs.h) $(call mw,${src}inc/newtcurs.h) $(call mw,${src}inc/dispcurs.h) $(call mw,${src}inc/coniocur.h) $(call mw,${src}inc/fltkcurs.h) $(call mw,${src}inc/gtkcurs.h) $(call mw,${src}inc/optcurs.h)
endif

ifeq ($(subst default,undefined,$(origin UTILS)),undefined)
UTILS = $(call mw,${src}inc/guess.h) $(call mw,${src}inc/utils.h) $(call mw,${src}src/utils.c)
endif

.PHONY: push-website

push-website: $(foreach file,${website_files},$(call mw,${src}${file}))
	$(foreach dir,${website_dirs},\
            test -d $(call q,${dir}) || \
            (${ECHOLINEX} creating directory $(call q,${dir}) && \
                ${INSTALL_DIR} $(call q,${dir})) || \
                exit $$?; \
            ${RSYNC} ${RSYNCFLAGS} -r -l -t --chmod=ugo=rwX --executability -essh --cvs-exclude $(call q,${src}${dir}/) $(call q,${MYMANWEBSITERSYNC}$(call s,$(call xq,website)%,%,${dir})/) || \
                exit $$?; \
            ${RSYNC} ${RSYNCFLAGS} -essh --cvs-exclude --times --dirs $(call q,${MYMANWEBSITERSYNC}$(call s,$(call xq,website)%,%,${dir})/) $(call q,${src}${dir}/) || \
                exit $$?;)
	${RSYNC} ${RSYNCFLAGS} -r -l -t --chmod=ugo=rwX --executability -essh --delete --cvs-exclude --delete-excluded $(call q,${src})website/ $(call q,${MYMANWEBSITERSYNC}/)
	@${ECHOLINE} $(call q,)
	@${ECHOLINE} $(call q,Now visit the website here:)
	@${ECHOLINE} $(call q,    ${MYMANWEBSITE})
	@${ECHOLINE} $(call q,And make sure it works.)

.PHONY: fill-dir-xq-$(call mwxq,${CVSDUMP})

fill-dir-xq-$(call xq,${CVSDUMP}):
	${RSYNC} ${RSYNCFLAGS} -a --delete $(call q,${MYMANCVSRSYNC}) $(call q,${CVSDUMP})

.PHONY: push-cvsdist

push-cvsdist: cvsdump cvsdist
	${RSYNC} ${RSYNCFLAGS} -aessh $(call q,${CVSDUMP})-${isodate}${tgz} $(call q,${CVSDIST})-${isodate}${tgz} $(call q,${UPLOADSRSYNC})
	@${ECHOLINE} $(call q,)
	@${ECHOLINE} $(call q,Now create a new file release called myman-cvs-${isodate} here:)
	@${ECHOLINE} $(call q,    ${UPLOADSWEBSITE})
	@${ECHOLINE} $(call q,And add the files ${CVSDUMP}-${isodate}${tgz} and ${CVSDIST}-${isodate}${tgz} to it.)
	@${ECHOLINE} $(call q,Set the type of each file to platform-independent source .gz)

.PHONY: cvsdist

cvsdist: fill-dir-xq-$(call xq,${CVSDUMP}) empty-dir-xq-$(call mwxq,${CVSDIST})
	${CVS} -d $(call q,$(shell pwd)/${CVSDUMP}) co -P -d $(call q,${CVSDIST}) myman
	${MAKE} -C $(call q,${CVSDIST}) dist DIST=$(call q,${CVSDIST})-${isodate}
	${INSTALL_DATA} $(call q,${CVSDIST}/${CVSDIST})-${isodate}${tgz} $(call q,${CVSDIST})-${isodate}${tgz}
	@${MAKE} ${MAKELOOP} \
            wipe-dir-xq-$(call qxq,${CVSDIST})

.PHONY: cvsdump

$(call mw,${CVSDUMP}${tgz}): cvsdump

cvsdump: compressed-tarball-xq-$(call mwxq,${CVSDUMP})
	-${REMOVE} $(call q,${CVSDUMP})${tar}
	${INSTALL_DATA} $(call q,${CVSDUMP})${tgz} $(call q,${CVSDUMP})-${isodate}${tgz}
	-${REMOVE} $(call q,${CVSDUMP})${tgz}

.PHONY: fill-dir-xq-$(call mwxq,${DIST})

fill-dir-xq-$(call xq,${DIST}):: $(call mw,${MAKEFILE}) $(addprefix $(call mw,${src}),${dist_files}) empty-dir-xq-$(call mwxq,${DIST}) $(foreach file,${dist_data_files} ${dist_program_files},$(call mw,${src}${file}))
	@${MAKE} ${MAKELOOP} \
            $(foreach dir,${dist_dirs},install-dir-xq-$(call qxq,${DIST}/${dir}))
	${POST_UNPACK}
	@${NOP}$(foreach file,${dist_data_files},${char_newline}${char_tab}@( \
            ${ECHOLINEX} installing data file $(call q,${DIST}/${file}); \
            ${INSTALL_DATA} $(call q,${src}${file}) $(call q,${DIST}/${file}) \
                || exit $$?; \
        ))
	@($(foreach file,${dist_program_files}, \
            ${ECHOLINEX} installing program file $(call q,${DIST}/${file}); \
            ${INSTALL_PROGRAM} $(call q,${src}${file}) $(call q,${DIST}/${file}) \
                || exit $$?; \
        ))
	${NORMAL_UNPACK}

.PHONY: dist

$(call mw,${DIST}${tgz}): dist

dist:: $(call mw,${MAKEFILE})
	@${MAKE} ${MAKELOOP} \
            compressed-tarball-xq-$(call qxq,${DIST})
	-${REMOVE} ${DIST}${tar}
	@${MAKE} ${MAKELOOP} \
            wipe-dir-xq-$(call qxq,${DIST})

ifneq ($script_target,)

ifneq ($script_stage,)

.PHONY: genscript-${script_stage}_${script_target}

genscript-${script_stage}_${script_target}: $(call mw,${src})configure $(call mw,${MAKEFILE})
	@echo $(call q,${POST_UNPACK})
	@echo "## ${script_stage}_${script_target} start"
	@${ECHOLINE} $(call q,$(firstword ${MAKE}))'() { : ; }'
	@${SED} ${SEDFLAGS} -n -e '/START_ECHOLINE_LIBRARY/,/END_ECHOLINE_LIBRARY/ p' < \
            $(call q,$<)
	@echo $(call q,${NORMAL_UNPACK})
	@${MAKE} ${MAKELOOP} -n -s \
            ${script_target} \
            ECHOLINE=echoline \
            ECHOLINEX=echolinex \
            PRE_$(call ucase,${script_target})=pre_${script_target} \
            POST_$(call ucase,${script_target})=post_${script_target} \
            NORMAL_$(call ucase,${script_target})=normal_${script_target} \
            | \
                ${SED} ${SEDFLAGS} -n -e '1 {;x;s/^$$/normal_${script_target}/;x;};/^\(normal\|pre\|post\)_${script_target}$$/ {;h;b;};G;s/\(.*\)'$(call q,${char_reverse_solidus}${char_newline})'${script_stage}_${script_target}$$/\1/p'
	@echo $(call q,${POST_UNPACK})
	@echo "## ${script_stage}_${script_target} end"
	@echo $(call q,${NORMAL_UNPACK})

endif

.PHONY: genscript-${script_target}-header

genscript-${script_target}-header: $(call mw,${src})configure $(call mw,${MAKEFILE})
	@${ECHOLINE} '#'\!$(call q,${SHELL})' --'
	@echo '# '$(call qq,${script_target}.sh - binary-${script_target} script for the MyMan video game)
	@echo
	@echo '# This file was generated automatically.'
	@echo '# Do not edit this file by hand.'
	@echo
	@echo '# See INSTALL for instructions.'
	@echo
	@echo '# make sure some standard directories are in our search path'
	@echo 'if test :"$${OSTYPE}" != :"msdosdjgpp"'
	@echo 'then'
	@echo '    PATH="$${PATH:-}:/usr/bin:/bin"'
	@echo '    export PATH'
	@echo 'fi'
	@echo
	@${SED} ${SEDFLAGS} -n -e '/START_ECHOLINE_LIBRARY/,/END_ECHOLINE_LIBRARY/ p' < \
            $(call q,$<)
	@echo
	@$(foreach var,$(filter-out this_file_undergoes_variable_substitution,${substitute_vars}),${ECHOLINE} ${var}=$(call q,$(call q,$(${var})));)
	@echo
	@echo override=
	@echo 'while test :"$$1" = :"--yes" -o :"$$1" = :"-yes" -o :"$$1" = :"-y"'
	@echo 'do'
	@echo '    override=yes'
	@echo '    shift'
	@echo 'done'
	@echo
	@echo 'if test :"$$1" = :"--no" -o :"$$1" = :"-no" -o :"$$1" = :"-n"'
	@echo 'then'
	@echo '    echo >&2'
	@echo '    echo Aborting. >&2'
	@echo '    exit 2'
	@echo 'fi'
	@echo
	@echo 'if test :"$$1" = :"--version" -o :"$$1" = :"-version" -o :"$$1" = :"-V"'
	@echo 'then'
	@echo '    echoline '$(call qq,${script_target}.sh)'" ($${MYMAN}) $${MYMANVERSION}"'
	@echo '    echoline "$${MYMANCOPYRIGHT}"'
	@echo '    exit 0'
	@echo 'fi'
	@echo
	@echo 'if test :"$$1" = :"--help" -o :"$$1" = :"-help" -o :"$$1" = :"-h"'
	@echo 'then'
	@${ECHOLINE} '    echolinex "Usage: $${0} [options]"'
	@${ECHOLINE} '    echoline "-y or"'
	@${ECHOLINE} '    echoline "--yes                   do binary-'$(call qq,${script_target})' without prompting for confirmation"'
	@${ECHOLINE} '    echoline "-n or"'
	@${ECHOLINE} '    echoline "--no                    don"'$(call qq,${char_apostrophe})'"t do binary-"'$(call qq,${script_target})
	@${ECHOLINE} '    echoline "-h or"'
	@${ECHOLINE} '    echoline "--help                  show this message and exit"'
	@${ECHOLINE} '    echoline "-V or"'
	@${ECHOLINE} '    echoline "--version               print "'$(call qq,${script_target}.sh)'" version and exit"'
	@echo '    exit 0'
	@echo 'fi'
	@echo
	@echo 'if test :"$$1" = :"--"'
	@echo 'then'
	@echo '    shift'
	@echo 'fi'
	@echo
	@echo 'if test $$# != 0'
	@echo 'then'
	@echo '    echolinex "$$0: unrecognized parameter "'$(call q,$(call q,${char_grave_accent}))'"$$1"'$(call q,$(call q,${char_apostrophe}))' >&2'
	@echo '    echolinex "usage: $$0 [--help] [--version] [options...]" >&2'
	@echo '    echo >&2'
	@echo '    echolinex "see INSTALL in the $${MYMAN} distribution directory for instructions" >&2'
	@echo '    echo >&2'
	@echo '    echo Aborting. >&2'
	@echo '    exit 2'
	@echo 'fi'
	@echo
	@echo echo parameters for binary-${script_target}:
	@echo echo
	@$(foreach var,$(filter-out this_file_undergoes_variable_substitution,${substitute_vars}),echo 'echolinex "    ${var}: $${${var}}"';)
	@echo echo
	@echo '# If stdin is a tty, make sure this is what you meant to do (use -y to override).'
	@echo '# Non-interactive invocations skip this check.'
	@echo 'while test -t 0 -a :"$${override}" != :"yes" -a :"$${override}" != :"y"'
	@echo 'do'
	@echo '    echolinex "About to do a binary-${script_target} of $${BINDIST}." >&2'
	@echo '    echoline "Are you sure? (yes/no)" >&2'
	@echo '    if read REPLY'
	@echo '    then'
	@echo '        override="$${REPLY}"'
	@echo '    else'
	@echo '        echolinex "$${0}: failed to read reply from standard input" >&2'
	@echo '        override=no'
	@echo '    fi'
	@echo '    if test :"$${override}" = :"no" -o :"$${override}" = :"n"'
	@echo '    then'
	@echo '        echo >&2'
	@echo '        echo Aborting. >&2'
	@echo '        exit 2'
	@echo '    fi'
	@echo 'done'
	@echo
	@echo 'echolinex "doing binary-${script_target} of $${BINDIST}"'
	@${ECHOLINE} ${MYMAN}'_found='
	@echo '${script_target}_dirname=`dirname "$$0" 2>/dev/null`'
	@echo '${script_target}_dirname="$${${script_target}_dirname:-$${0##*/}}"'
	@echo '${script_target}_dirname="$${${script_target}_dirname:-.}"'
	@echo 'for ${script_target}_dir in . "$${${script_target}_dirname}"'
	@echo 'do'
	@echo '    if test -f "$${${script_target}_dir}"/VERSION'
	@echo '    then'
	@echo '        ${script_target}_dir_version=`cat "$${${script_target}_dir}"/VERSION`'
	@echo '        if test :"$${MYMANVERSION}" = :"$${${script_target}_dir_version}"'
	@echo '        then'
	@${ECHOLINE} '            '${MYMAN}'_found=t'
	@echo '            break'
	@echo '        fi'
	@echo '    fi'
	@echo 'done'
	@echo
	@${ECHOLINE} 'if test :$${'${MYMAN}'_found} != :t'
	@echo 'then'
	@${ECHOLINE} '    echolinex unable to find VERSION file from "$${BINDIST}" >&2'
	@echo '    echo >&2'
	@echo '    echo Aborting. >&2'
	@echo '    exit 2'
	@echo 'fi'
	@echo
	@echo 'cd "$${${script_target}_dir}" || exit 1'

.PHONY: genscript-${script_target}

genscript-${script_target}: $(call mw,${MAKEFILE})
	@${MAKE} ${MAKELOOP} \
            genscript-${script_target}-header
ifneq (uninstall,${script_target})
	@${ECHOLINE} '(cd '$(call qq,${distroot})') || exit 1'
endif
ifeq (clean,${script_target})
	@${ECHOLINE} 'exec rm -rf '$(call qq,install.sh)' '$(call qq,${distroot})' '$(call qq,clean.sh)
else
ifeq (install,${script_target})
	@${ECHOLINE} 'bindist_install_program() { '$(call q,${INSTALL_PROGRAM}) $(call qq,${distroot})'"$$2" "$$2"; }'
	@${ECHOLINE} 'bindist_install_data() { '$(call q,${INSTALL_DATA}) $(call qq,${distroot})'"$$2" "$$2"; }'
	@${ECHOLINE} 'bindist_install_dir() { '$(call q,${INSTALL_DIR})' "$$1"; }'
endif
	@${MAKE} ${MAKELOOP} \
            genscript-pre_${script_target} \
            script_stage=pre
ifeq (install,${script_target})
	@${MAKE} ${MAKELOOP} \
            genscript-normal_${script_target} \
            script_stage=normal \
            INSTALL_PROGRAM=bindist_install_program \
            INSTALL_DATA=bindist_install_data \
            INSTALL_DIR=bindist_install_dir \
            INSTALL_MANLINK=bindist_install_data \
            PRE_UNPACK=pre_unpack \
            NORMAL_UNPACK=normal_unpack \
            POST_UNPACK=post_unpack \
            | \
                ${SED} ${SEDFLAGS} -n -e '1 {;x;s/^$$/normal_unpack/;x;};/^\(normal\|pre\|post\)_unpack$$/ {;h;b;};G;s/\(.*\)'$(call q,${char_reverse_solidus}${char_newline})'post_unpack$$/\1/p'
else
	@${MAKE} ${MAKELOOP} \
            genscript-normal_${script_target} \
            script_stage=normal
endif
	@${MAKE} ${MAKELOOP} \
            genscript-post_${script_target} \
            script_stage=post
endif

endif

.PHONY: fill-dir-xq-${BINDIST}

fill-dir-xq-${BINDIST}:: $(call mw,${MAKEFILE}) all $(addprefix $(call mw,${src}),${doc_files}) empty-dir-xq-$(call mwxq,${BINDIST})
	@-${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${BINDIST}/${MYMAN}${txt}) \
             data_file=${MYMAN}${txt}
	@-${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${BINDIST}/${MYMAN}.ps) \
             data_file=${MYMAN}.ps
	@-${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${BINDIST}/${MYMAN}.dvi) \
             data_file=${MYMAN}.dvi
	@-${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${BINDIST}/${MYMAN}${htm}) \
             data_file=${MYMAN}${htm}
	${POST_UNPACK}
	@($(foreach file,${doc_files}, \
            ${ECHOLINEX} installing data file $(call q,${BINDIST}/${file}); \
            ${INSTALL_DATA} $(call q,${src}${file}) $(call q,${BINDIST}/${file}) \
                || exit $$?; \
        ))
	${NORMAL_UNPACK}
	${MAKE} ${MAKELOOP} \
            genscript-clean \
            script_target=clean \
            > ${BINDIST}/$(call q,clean.sh)
	-chmod 755 ${BINDIST}/$(call q,clean.sh)
	${MAKE} ${MAKELOOP} \
            genscript-uninstall \
            script_target=uninstall \
            > ${BINDIST}/$(call q,uninstall.sh)
	-chmod 755 ${BINDIST}/$(call q,uninstall.sh)
	${MAKE} ${MAKELOOP} \
            genscript-normal_install \
            script_stage=normal \
            script_target=install \
            DESTDIR=./${BINDIST}/${distroot} \
            | ${SHELL}
	${MAKE} ${MAKELOOP} \
            genscript-install \
            script_target=install \
            > ${BINDIST}/$(call q,install.sh)
	-chmod 755 ${BINDIST}/$(call q,install.sh)
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,./${BINDIST}/${MYMANCOMMAND_EXE}) \
             program_file=$(call qmq,${MYMANCOMMAND})
	@-${SYMLINK} $(call q,${distroot}${bindir}/${MYMAN_EXE}$x) $(call q,./${BINDIST}/${MYMAN_EXE}$x) \
             || ${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,./${BINDIST}/${MYMAN_EXE}$x) \
             program_file=$(call qmq,${MYMAN}$x) \
             INSTALLFLAGS=$(call qmq,${INSTALLFLAGS} ${STRIPINSTALLFLAGS})
ifeq (yes,${with_mac})
	@-${SYMLINK} $(call q,${distroot}${appdir_MYMAN_APP}) $(call q,./${BINDIST}/${MYMAN_APP})
endif

.PHONY: bindist

$(call mw,${BINDIST}${tgz}): bindist

ifeq (yes,${with_zip})
$(call mw,${BINDIST}.zip): bindist
endif

ifeq (yes,${with_dmg})
$(call mw,${BINDIST}.dmg): bindist
endif

bindist:: $(call mw,${MAKEFILE})
	@${MAKE} ${MAKELOOP} \
            compressed-tarball-xq-$(call qxq,${BINDIST})
	-${REMOVE} $(call q,${BINDIST}${tar})
ifeq (yes,${with_zip})
	-${REMOVE} $(call q,${BINDIST}.zip)
	-${ZIP} -r -9 $(call q,${BINDIST}.zip) $(call q,${BINDIST}) || \
            ( \
                ${REMOVE} $(call q,${BINDIST}.zip) ; \
                exit 1 \
            )
endif
ifeq (yes,${with_dmg})
	-${HDIUTIL} create -volname $(call q,${BINDIST}) -srcfolder $(call q,${BINDIST}) $(call q,${BINDIST}.dmg) || \
            ( \
                ${REMOVE} $(call q,${BINDIST}.dmg) ; \
                exit 1 \
            )
endif
	@${MAKE} ${MAKELOOP} \
            wipe-dir-xq-$(call qxq,${BINDIST})

.PHONY: info check installcheck

info check installcheck:
	@${ECHOLINEX} $(call q,${MAKEFILE}): warning: target $@ not implemented
	@-(exit 1)

TAGS:: $(addprefix $(call mw,${src}),${dist_files})
	etags -o $@ $(addprefix $(call q,${src}),${dist_files})

all:: $(call mw,${MAKEFILE}) ${MYMAN}$x $(call mw,${MYMANCOMMAND})

ifeq (yes,${with_rxvt})
all install-files:: $(call mw,${XMYMAN2}${sh})
endif

ifeq (yes,${with_kterm})
all install-files:: $(call mw,${XMYMAN4}${sh})
endif

ifeq (yes,${with_xterm})
all install-files:: $(call mw,${XMYMAN}${sh}) $(call mw,${XMYMAN3}${sh}) $(call mw,${XQUACKMAN}${sh}) $(call mw,${XBIGMAN}${sh}) $(call mw,${XHUGEMAN}${sh}) $(call mw,${XBITMAN}${sh}) $(call mw,${XBITMAN2}${sh}) $(call mw,${XBITMAN3}${sh}) $(call mw,${XBITMAN4}${sh}) $(call mw,${XBITMAN5}${sh}) $(call mw,${XBITMAN6}${sh})
endif

ifeq (yes,${with_win_icon})

all:: $(call mw,${obj}winicon.res)

${obj}winicon.rc: $(call mw,${MAKEFILE})
	@${ECHOLINEX} creating $(call q,$@) && \
        ${ECHOLINE} $(call q,${char_number_sign}include <windows.h>${char_newline}\
IDI_APPLICATION ICON "$(call cq,${obj}myman.ico)"${char_newline}\
1 VERSIONINFO${char_newline}\
    FILEVERSION $(subst .,${char_comma},${MYMANVERSION}${char_comma}0)${char_newline}\
    PRODUCTVERSION $(subst .,${char_comma},${MYMANVERSION}${char_comma}0)${char_newline}\
    FILEFLAGSMASK 0x3f${char_newline}\
    FILEOS 0x4${char_newline}\
    FILETYPE 0x2${char_newline}\
BEGIN${char_newline}\
    BLOCK "StringFileInfo"${char_newline}\
    BEGIN${char_newline}\
        BLOCK "040904e4"${char_newline}\
        BEGIN${char_newline}\
            VALUE "CompanyName"${char_comma} "Benjamin C. Wiley Sittler"${char_newline}\
            VALUE "FileDescription"${char_comma} "The MyMan video game"${char_newline}\
            VALUE "FileVersion"${char_comma} "$(call cq,${MYMANVERSION})"${char_newline}\
            VALUE "Full Version"${char_comma} "$(call cq,${MYMANVERSION})"${char_newline}\
            VALUE "InternalName"${char_comma} "$(call cq,${MYMAN})"${char_newline}\
            VALUE "LegalCopyright"${char_comma} "$(call cq,${MYMANCOPYRIGHT})"${char_newline}\
            VALUE "OriginalFilename"${char_comma} "$(call cq,${MYMAN}$x)"${char_newline}\
            VALUE "ProductName"${char_comma} "$(call cq,${MYMAN})"${char_newline}\
            VALUE "ProductVersion"${char_comma} "$(call cq,${MYMANVERSION})"${char_newline}\
        END${char_newline}\
    END${char_newline}\
END) > $@ || \
            ( \
                ${REMOVE} $(call q,$@) ; \
                exit 1 \
            )

${obj}winicon.res: $(call mw,${obj}winicon.rc) $(call mw,${obj}myman.ico)
	@${HOSTCOMPILE_RESOURCE}

endif

ifeq (yes,${with_mac_icon})

all:: $(call mw,${obj}${MYMAN}.icns)

${obj}${MYMAN}.tiff: ${src}gfx/myman.png ${src}gfx/myman64.png ${src}gfx/myman48.png ${src}gfx/myman32.png
	-${TIFFUTIL} -cat ${src}gfx/myman.png ${src}gfx/myman64.png ${src}gfx/myman48.png ${src}gfx/myman32.png -out $(call q,$@) || \
            ( \
                ${REMOVE} $(call q,$@) ; \
                exit 1 \
            )

${obj}${MYMAN}.icns: $(call mw,${obj}${MYMAN}.tiff)
	-${TIFF2ICNS} $(call q,$<) $(call q,$@) || \
            ( \
                ${REMOVE} $(call q,$@) ; \
                exit 1 \
            )

endif

ifeq (yes,${with_mac})

all:: ${MYMAN}.app/Contents/Info.plist ${MYMAN}.app/Contents/PkgInfo ${MYMAN}.app/Contents/${PLATFORM}/${MYMAN}$x $(call mw,${obj}${MYMAN}.plist)

ifeq (yes,${with_mac_icon})
all:: ${MYMAN}.app/Contents/Resources/${MYMAN}.icns
endif

${obj}${MYMAN}.plist: $(call mw,${src})configure $(call mw,${MAKEFILE})
	@-${REMOVE} $(call q,$@)
	@${ECHOLINEX} creating $(call q,$@) && \
	(touch $(call q,$@) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) CFBundleSignature -string $(call q,${CREATOR})) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) CFBundlePackageType -string $(call q,${TYPE}) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) CFBundleExecutable -string $(call q,${MYMAN_EXE}$x) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) CFBundleVersion -string $(call q,${MYMANVERSION}) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) CFBundleShortVersionString -string $(call q,${MYMANVERSION}) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) NSHumanReadableCopyright -string $(call q,${MYMANCOPYRIGHT}) || \
            ( \
                ${REMOVE} $(call q,$@) ; \
                exit 1 \
            )
ifeq (yes,${with_mac_icon})
	@${ECHOLINEX} adding icon information to $(call q,$@) && \
        defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) CFBundleIconFile -string $(call q,${MYMAN_ICNS}) || \
            ( \
                ${REMOVE} $(call q,$@) ; \
                exit 1 \
            )
endif

${MYMAN}.app/Contents/PkgInfo: $(call mw,${src})configure $(call mw,${MAKEFILE})
	@${ECHOLINEX} creating $(call q,$@) && \
	( \
	echo_n= ; \
	echo_c= ; \
	 \
	if test :"`echo -n \"_\"`" != :"_" ; \
	then \
	    echo_c='\c' ; \
	else \
	    echo_n='-n' ; \
	fi ; \
	 \
	echo $${echo_n} $(call q,${TYPE})$(call q,${CREATOR})$${echo_c} > $(call q,$@)) || \
            ( \
                ${REMOVE} $(call q,$@) ; \
                exit 1 \
            )

${MYMAN}.app/Contents/Info.plist: $(call mw,${src})configure $(call mw,${MAKEFILE})
	@${MKPARENTDIR}
	@-${REMOVE} $(call q,$@)
	@${ECHOLINEX} creating $(call q,$@) && \
	(touch $(call q,$@) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) CFBundleSignature -string $(call q,${CREATOR})) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) CFBundlePackageType -string $(call q,${TYPE}) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) CFBundleExecutable -string $(call q,${MYMAN}$x) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) CFBundleVersion -string $(call q,${MYMANVERSION}) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) CFBundleShortVersionString -string $(call q,${MYMANVERSION}) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) NSHumanReadableCopyright -string $(call q,${MYMANCOPYRIGHT}) || \
            ( \
                ${REMOVE} $(call q,$@) ; \
                exit 1 \
            )
ifeq (yes,${with_mac_icon})
	@${ECHOLINEX} adding icon information to $(call q,$@) && \
	defaults write $(call q,${CURDIR}/$(call s,%$(call xq,.plist),%,$@)) CFBundleIconFile -string $(call q,${MYMAN}.icns) || \
            ( \
                ${REMOVE} $(call q,$@) ; \
                exit 1 \
            )
endif

${MYMAN}.app/Contents/Resources/${MYMAN}.icns: $(call mw,${obj}${MYMAN}.icns)
	@${MKPARENTDIR}
	@-${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,$@) \
             data_file=$(call qmq,$<)

ifeq (yes,${with_mac_icon})
${MYMAN}.app/Contents/${PLATFORM}/${MYMAN}$x: $(call mw,${MYMAN}.app/Contents/Resources/${MYMAN}.icns)
endif

${MYMAN}.app/Contents/${PLATFORM}/${MYMAN}$x: ${MYMAN}$x $(call mw,${MYMAN}.app/Contents/Info.plist) $(call mw,${MYMAN}.app/Contents/PkgInfo)
	@${MKPARENTDIR}
	@${MAKE} ${MAKELOOP} \
             install-dir-xq-$(call qxq,${MYMAN}.app/Contents/${PLATFORM}) \
             install-program-xq-$(call qxq,$@) \
             program_file=$(call qmq,$<)
	@-(${PROD_FINDER} $(call q,${MYMAN}.app))
endif

.PHONY: doc docs

doc docs: ${MYMAN}${man6ext} ${MYMAN}${txt} ${MYMAN}.ps dvi ${MYMAN}${htm} $(addprefix $(call mw,${src}),${doc_files})

.PHONY: dvi

dvi: ${MYMAN}.dvi

ifeq (yes,${with_mac})
clean:: wipe-dir-xq-$(call mwxq,${MYMAN}.app)
endif

clean:: wipe-dir-xq-$(call mwxq,${DIST}) wipe-dir-xq-$(call mwxq,${BINDIST}) wipe-dir-xq-$(call mwxq,${CVSDIST})
	-${REMOVE} config.h
	-${REMOVE} ${DIST}${tgz} ${DIST}${tar}
	-${REMOVE} ${BINDIST}${tgz} ${BINDIST}${tar}
	-${REMOVE} ${CVSDUMP}${tgz} ${CVSDUMP}${tar}
	-${REMOVE} ${CVSDUMP}-[0-9][0-9][0-9][0-9]*-[0-9][0-9]-[0-9][0-9]${tgz}
	-${REMOVE} ${CVSDUMP}-[0-9][0-9][0-9][0-9]*-[0-9][0-9]-[0-9][0-9]${tar}
	-${REMOVE} ${CVSDIST}-[0-9][0-9][0-9][0-9]*-[0-9][0-9]-[0-9][0-9]${tgz}
ifeq (yes,${with_zip})
	-${REMOVE} ${BINDIST}.zip
endif
ifeq (yes,${with_dmg})
	-${REMOVE} ${BINDIST}.dmg
endif
	-${REMOVE} ${MYMAN}${htm}${tmp} $(call q,${objdir})/*$o ${MYMAN}$x TAGS
	-${REMOVE} $(call q,${objdir})/*$O
	-${REMOVE} _sanity?$X $(call q,${obj}_sanity)?.c
	-${REMOVE} ${MYMAN}${man6ext} ${MYMAN}${txt} ${MYMAN}.ps ${MYMAN}.dvi ${MYMAN}${htm}
	-${REMOVE} ${XMYMAN}${sh} ${XMYMAN2}${sh} ${XMYMAN3}${sh} ${XMYMAN4}${sh} ${XBIGMAN}${sh} ${XHUGEMAN}${sh} ${XBITMAN}${sh} ${XBITMAN2}${sh} ${XBITMAN3}${sh} ${XBITMAN4}${sh} ${XBITMAN5}${sh} ${XBITMAN6}${sh} ${XQUACKMAN}${sh} ${MYMANCOMMAND}
	-${REMOVE} $(foreach variant,${MYMANVARIANTS},$(call q,$(call mymanvariant_data,${variant}).c))
	-${REMOVE} $(foreach size,${MYMANSIZES},$(call q,$(call mymansize_data,${size}).c))
	-${REMOVE} ${hostprefix}${GAME}$x $(call q,${BOOTSTRAP}$X)
	-${REMOVE} pdcicon.bmp ${obj}myman.ico
ifeq (yes,${with_win_icon})
	-${REMOVE} ${obj}winicon.rc ${obj}winicon.res ${obj}winicon.c
endif
ifeq (yes,${with_mac})
	-${REMOVE} $(call q,${obj}${MYMAN}.plist)
endif
ifeq (yes,${with_mac_icon})
	-${REMOVE} $(call q,${obj}${MYMAN}.tiff) $(call q,${obj}${MYMAN}.icns)
endif

install:: install-files

uninstall:: $(call mw,${MAKEFILE})
	@${MAKE} ${MAKELOOP} \
            uninstalldirs

ifeq (yes,${with_mac})
install-files:: $(call mw,${obj}${MYMAN}.plist) $(call mw,${MYMAN}.app/Contents/PkgInfo)
endif

ifeq (yes,${with_mac_icon})
install-files:: $(call mw,${obj}${MYMAN}.icns)
endif

all install-files:: docs

install-files:: $(call mw,${MAKEFILE}) installdirs ${MYMAN}$x $(call mw,${MYMANCOMMAND}) $(call mw,${src})utl/myman.ct $(foreach variant,${MYMANVARIANTS},$(call mw,$(call mazefile,${variant}))) $(foreach size,${MYMANSIZES},$(call mw,$(call tilefile,${size})) $(call mw,$(call spritefile,${size}))) ${src}gfx/myman.png ${src}gfx/myman64.png ${src}gfx/myman48.png ${src}gfx/myman32.png $(foreach extraprog,${EXTRAPROGS},$(call mw,${extraprog}))
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${MYMAN_EXE}$x) \
             program_file=$(call qmq,${MYMAN}$x) \
             INSTALLFLAGS=$(call qmq,${INSTALLFLAGS} ${STRIPINSTALLFLAGS})
	@${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${DESTDIR}${gfxdir}/myman.png) \
             data_file=$(call qmq,${src}gfx/myman.png)
	@${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${DESTDIR}${gfxdir}/myman64.png) \
             data_file=$(call qmq,${src}gfx/myman64.png)
	@${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${DESTDIR}${gfxdir}/myman48.png) \
             data_file=$(call qmq,${src}gfx/myman48.png)
	@${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${DESTDIR}${gfxdir}/myman32.png) \
             data_file=$(call qmq,${src}gfx/myman32.png)
ifeq (yes,${with_mac_icon})
	@-($(call set_mac_icon,${obj}${MYMAN}.icns) $(call q,${DESTDIR}/${bindir}/${MYMAN_EXE}$x))
endif
ifeq (yes,${with_mac})
	@${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${DESTDIR}${appdir_MYMAN_APP_CONTENTS}/Info.plist) \
             data_file=$(call qmq,${obj}${MYMAN}.plist)
	@${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${DESTDIR}${appdir_MYMAN_APP_CONTENTS}/PkgInfo) \
             data_file=$(call qmq,${MYMAN}.app/Contents/PkgInfo)
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${appdir_MYMAN_APP_CONTENTS_PLATFORM}/${MYMAN_EXE}$x) \
             program_file=$(call qmq,${MYMAN}$x) \
             INSTALLFLAGS=$(call qmq,${INSTALLFLAGS} ${STRIPINSTALLFLAGS})
ifeq (yes,${with_mac_icon})
	@-${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${DESTDIR}${appdir_MYMAN_APP_CONTENTS_RESOURCES}/${MYMAN_ICNS}) \
             data_file=$(call qmq,${obj}${MYMAN}.icns)
endif
	${POST_UNPACK}
	@-(${PROD_FINDER} $(call q,${DESTDIR}${appdir_MYMAN_APP}))
	${NORMAL_UNPACK}
endif
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${DIST_EXE}$x) \
             program_file=$(call qmq,${MYMAN}$x) \
             INSTALLFLAGS=$(call qmq,${INSTALLFLAGS} ${STRIPINSTALLFLAGS})
ifeq (yes,${with_rxvt})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XMYMAN2_EXE}${sh}) \
             program_file=$(call qmq,${XMYMAN2}${sh})
endif
ifeq (yes,${with_kterm})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XMYMAN4_EXE}${sh}) \
             program_file=$(call qmq,${XMYMAN4}${sh})
endif
ifeq (yes,${with_xterm})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XMYMAN_EXE}${sh}) \
             program_file=$(call qmq,${XMYMAN}${sh})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XMYMAN3_EXE}${sh}) \
             program_file=$(call qmq,${XMYMAN3}${sh})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XBIGMAN_EXE}${sh}) \
             program_file=$(call qmq,${XBIGMAN}${sh})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XHUGEMAN_EXE}${sh}) \
             program_file=$(call qmq,${XHUGEMAN}${sh})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XBITMAN_EXE}${sh}) \
             program_file=$(call qmq,${XBITMAN}${sh})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XBITMAN2_EXE}${sh}) \
             program_file=$(call qmq,${XBITMAN2}${sh})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XBITMAN3_EXE}${sh}) \
             program_file=$(call qmq,${XBITMAN3}${sh})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XBITMAN4_EXE}${sh}) \
             program_file=$(call qmq,${XBITMAN4}${sh})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XBITMAN5_EXE}${sh}) \
             program_file=$(call qmq,${XBITMAN5}${sh})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XBITMAN6_EXE}${sh}) \
             program_file=$(call qmq,${XBITMAN6}${sh})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${XQUACKMAN_EXE}${sh}) \
             program_file=$(call qmq,${XQUACKMAN}${sh})
endif

	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${MYMANCOMMAND_EXE}) \
             program_file=$(call qmq,${MYMANCOMMAND})
ifneq (,${EXTRAPROGS})
	@($(foreach extraprog,${EXTRAPROGS}, \
	${MAKE} ${MAKELOOP} \
            install-program-xq-$(call qxq,${DESTDIR}${bindir}/$(call program,${extraprog})) \
            program_file=$(call qmq,${extraprog}) || \
	    exit 1; \
	))
endif
ifeq (yes,${with_ctheme})
	@${MAKE} ${MAKELOOP} \
             install-program-xq-$(call qxq,${DESTDIR}${bindir}/${MYMAN_CT_EXE}) \
             program_file=$(call qmq,${src}utl/myman.ct)
endif
	${POST_UNPACK}
	@($(foreach file,${doc_files}, \
            ${ECHOLINEX} installing data file $(call q,${DESTDIR}${privatedocdir}/${file}); \
            ${INSTALL_DATA} $(call q,${src}${file}) $(call q,${DESTDIR}${privatedocdir}/${file}) \
                || exit $$?; \
        ))
	${NORMAL_UNPACK}
	@-${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${DESTDIR}${privatedocdir}/${MYMAN}${txt}) \
             data_file=${MYMAN}${txt}
	@-${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${DESTDIR}${privatedocdir}/${MYMAN}.ps) \
             data_file=${MYMAN}.ps
	@-${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${DESTDIR}${privatedocdir}/${MYMAN}.dvi) \
             data_file=${MYMAN}.dvi
	@-${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${DESTDIR}${privatedocdir}/${MYMAN}${htm}) \
             data_file=${MYMAN}${htm}
	@-${MAKE} ${MAKELOOP} \
             install-data-xq-$(call qxq,${DESTDIR}${man6dir}/${DIST_EXE}${man6ext}) \
             data_file=${MYMAN}${man6ext}
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${MYMAN_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
ifneq ($x,)
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${MYMAN_EXE}$x${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${DIST_EXE}$x${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
endif
ifeq (yes,${with_rxvt})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XMYMAN2_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
endif
ifeq (yes,${with_kterm})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XMYMAN4_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
endif
ifeq (yes,${with_xterm})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XMYMAN_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XMYMAN3_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBIGMAN_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XHUGEMAN_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBITMAN_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBITMAN2_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBITMAN3_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBITMAN4_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBITMAN5_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBITMAN6_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XQUACKMAN_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
endif
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${MYMANCOMMAND_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
ifneq (${sh},)
ifeq (yes,${with_rxvt})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XMYMAN2_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
endif
ifeq (yes,${with_kterm})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XMYMAN4_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
endif
ifeq (yes,${with_xterm})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XMYMAN_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XMYMAN3_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBIGMAN_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XHUGEMAN_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBITMAN_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBITMAN2_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBITMAN3_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBITMAN4_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBITMAN5_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XBITMAN6_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${XQUACKMAN_EXE}${sh}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
endif
endif
ifeq (yes,${with_xtheme})
	@-${MAKE} ${MAKELOOP} \
             install-manlink-xq-$(call qxq,${DESTDIR}${man6dir}/${MYMAN_CT_EXE}${man6ext}) \
             manlink_file=$(call qmqs,$(call xq,${mandir}/)%,%,${man6dir}/${DIST}${man6ext})
endif
	${POST_UNPACK}
	@($(foreach file,$(foreach variant,${MYMANVARIANTS},$(call mazefile,${variant})), \
            ${ECHOLINEX} installing maze file $(call q,${DESTDIR}${mazedir}/$(call s,$(call xq,${src}lvl/)%,%,${file})); \
            ${INSTALL_DATA} $(call q,${file}) $(call q,${DESTDIR}${mazedir}/$(call s,$(call xq,${src}lvl/)%,%,${file})) \
                || exit $$?; \
        ))
	@($(foreach file,$(foreach size,${MYMANSIZES},$(call tilefile,${size})), \
            ${ECHOLINEX} installing tile file $(call q,${DESTDIR}${tiledir}/$(call s,$(call xq,${src}chr/)%,%,${file})); \
            ${INSTALL_DATA} $(call q,${file}) $(call q,${DESTDIR}${tiledir}/$(call s,$(call xq,${src}chr/)%,%,${file})) \
                || exit $$?; \
        ))
	@($(foreach file,$(foreach size,${MYMANSIZES},$(call spritefile,${size})), \
            ${ECHOLINEX} installing sprite file $(call q,${DESTDIR}${spritedir}/$(call s,$(call xq,${src}spr/)%,%,${file})); \
            ${INSTALL_DATA} $(call q,${file}) $(call q,${DESTDIR}${spritedir}/$(call s,$(call xq,${src}spr/)%,%,${file})) \
                || exit $$?; \
        ))
	${NORMAL_UNPACK}
	${POST_INSTALL}
	@${MAKE} ${MAKELOOP} \
            update-whatis
	${NORMAL_INSTALL}

.PHONY: update-whatis

update-whatis:
	@-(dir=$(call q,${DESTDIR}${mandir}); \
        export dir; \
        if ${SHELL} -c $(call q,${MANDB} ${MANDBFLAGS} ${EXTRAMANDBFLAGS} "$${dir}") </dev/null >/dev/null 2>&1 || \
            ${SHELL} -c $(call q,${MAKEWHATIS} ${MAKEWHATISFLAGS} ${EXTRAMAKEWHATISFLAGS} "$${dir}") </dev/null; \
        then \
            ${ECHOLINEX} updated whatis database for "$${dir}"; \
        else \
            echo $@: failed to update whatis database >&2; \
            ${ECHOLINEX} '** ' you should update the whatis database for "$${dir}" >&2; \
            echo '** ' if you would like the \`apropos\' and \`whatis\' programs to >&2; \
            ${ECHOLINEX} '** ' find ${MYMAN}\; if you have the \`mandb\' and/or \`makewhatis\' >&2; \
            echo '** ' programs, add their directories to your PATH and re-run this command >&2; \
            exit 1; \
        fi)

uninstall-files: $(call mw,${MAKEFILE})
ifeq (yes,${with_mac})
	@($(call uninstall_file,${DESTDIR}${appdir_MYMAN_APP_CONTENTS}/Info.plist))
	@($(call uninstall_file,${DESTDIR}${appdir_MYMAN_APP_CONTENTS}/PkgInfo))
	@($(call uninstall_file,${DESTDIR}${appdir_MYMAN_APP_CONTENTS_PLATFORM}/${MYMAN_EXE}$x))
	@($(call uninstall_file,${DESTDIR}${appdir_MYMAN_APP_CONTENTS_RESOURCES}/${MYMAN_ICNS}))
endif
	@${NOP}$(foreach file,${DESTDIR}${bindir}/${MYMAN_EXE}$x ${DESTDIR}${bindir}/${DIST_EXE}$x ${DESTDIR}${bindir}/${XMYMAN_EXE}${sh} ${DESTDIR}${bindir}/${XMYMAN2_EXE}${sh} ${DESTDIR}${bindir}/${XMYMAN3_EXE}${sh} ${DESTDIR}${bindir}/${XMYMAN4_EXE}${sh} ${DESTDIR}${bindir}/${XBIGMAN_EXE}${sh} ${DESTDIR}${bindir}/${XHUGEMAN_EXE}${sh} ${DESTDIR}${bindir}/${XBITMAN_EXE}${sh} ${DESTDIR}${bindir}/${XBITMAN2_EXE}${sh} ${DESTDIR}${bindir}/${XBITMAN3_EXE}${sh} ${DESTDIR}${bindir}/${XBITMAN4_EXE}${sh} ${DESTDIR}${bindir}/${XBITMAN5_EXE}${sh} ${DESTDIR}${bindir}/${XBITMAN6_EXE}${sh} ${DESTDIR}${bindir}/${XQUACKMAN_EXE}${sh} ${DESTDIR}${bindir}/${MYMANCOMMAND_EXE} $(foreach extraprog,${EXTRAPROGS},${DESTDIR}${bindir}/$(call program,${extraprog})) ${DESTDIR}${bindir}/${MYMAN_CT_EXE} ${DESTDIR}${privatedocdir}/${MYMAN}${txt} ${DESTDIR}${privatedocdir}/${MYMAN}.ps ${DESTDIR}${privatedocdir}/${MYMAN}.dvi ${DESTDIR}${privatedocdir}/${MYMAN}${htm} ${DESTDIR}${man6dir}/${MYMAN_EXE}${man6ext} ${DESTDIR}${man6dir}/${MYMAN_CT_EXE}${man6ext} ${DESTDIR}${man6dir}/${XQUACKMAN_EXE}${man6ext} ${DESTDIR}${man6dir}/${MYMANCOMMAND_EXE}${man6ext} ${DESTDIR}${man6dir}/${XHUGEMAN_EXE}${man6ext} ${DESTDIR}${man6dir}/${XBITMAN_EXE}${man6ext} ${DESTDIR}${man6dir}/${XBITMAN2_EXE}${man6ext} ${DESTDIR}${man6dir}/${XBITMAN3_EXE}${man6ext} ${DESTDIR}${man6dir}/${XBITMAN4_EXE}${man6ext} ${DESTDIR}${man6dir}/${XBITMAN5_EXE}${man6ext} ${DESTDIR}${man6dir}/${XBITMAN6_EXE}${man6ext} ${DESTDIR}${man6dir}/${XBIGMAN_EXE}${man6ext} ${DESTDIR}${man6dir}/${XMYMAN4_EXE}${man6ext} ${DESTDIR}${man6dir}/${XMYMAN3_EXE}${man6ext} ${DESTDIR}${man6dir}/${XMYMAN2_EXE}${man6ext} ${DESTDIR}${man6dir}/${XMYMAN_EXE}${man6ext} ${DESTDIR}${man6dir}/${DIST_EXE}${man6ext} ${DESTDIR}${man6dir}/${MYMAN_EXE}$x${man6ext} ${DESTDIR}${man6dir}/${DIST_EXE}$x${man6ext} ${DESTDIR}${man6dir}/${XQUACKMAN_EXE}${sh}${man6ext} ${DESTDIR}${man6dir}/${XHUGEMAN_EXE}${sh}${man6ext} ${DESTDIR}${man6dir}/${XBITMAN_EXE}${sh}${man6ext} ${DESTDIR}${man6dir}/${XBITMAN2_EXE}${sh}${man6ext} ${DESTDIR}${man6dir}/${XBITMAN3_EXE}${sh}${man6ext} ${DESTDIR}${man6dir}/${XBITMAN4_EXE}${sh}${man6ext} ${DESTDIR}${man6dir}/${XBITMAN5_EXE}${sh}${man6ext} ${DESTDIR}${man6dir}/${XBITMAN6_EXE}${sh}${man6ext} ${DESTDIR}${man6dir}/${XBIGMAN_EXE}${sh}${man6ext} ${DESTDIR}${man6dir}/${XMYMAN4_EXE}${sh}${man6ext} ${DESTDIR}${man6dir}/${XMYMAN3_EXE}${sh}${man6ext} ${DESTDIR}${man6dir}/${XMYMAN2_EXE}${sh}${man6ext} ${DESTDIR}${man6dir}/${XMYMAN_EXE}${sh}${man6ext} ${DESTDIR}${gfxdir}/myman.png ${DESTDIR}${gfxdir}/myman64.png ${DESTDIR}${gfxdir}/myman48.png ${DESTDIR}${gfxdir}/myman32.png $(foreach file,${doc_files},${DESTDIR}${privatedocdir}/${file}) $(foreach variant,${MYMANVARIANTS},$(call s,$(call xq,${src}lvl/)%,$(call xq,${DESTDIR}${mazedir}/)%,$(call mazefile,${variant}))) $(foreach size,${MYMANSIZES},$(call s,$(call xq,${src}chr/)%,$(call xq,${DESTDIR}${tiledir}/)%,$(call tilefile,${size})) $(call s,$(call xq,${src}spr/)%,$(call xq,${DESTDIR}${spritedir}/)%,$(call spritefile,${size}))),${char_newline}${char_tab}@(\
            $(call uninstall_file,${file}) \
                || exit $$?; \
        ))
	@${MAKE} ${MAKELOOP} \
            update-whatis
	${NORMAL_UNINSTALL}

uninstalldirs: $(call mw,${MAKEFILE}) uninstall-files
	@(for dir in $(foreach dir_var,$(${DIST}_subdir_vars) $(${DIST}_dir_vars),$(call qxq,$(${dir_var}))); \
        do \
            ${MAKE} ${MAKELOOP} \
                uninstall-dir-xq-$(call qxq,${DESTDIR})"$$dir" \
                    || exit $$?; \
        done)
	@(for dir in $(foreach dir_var,${public_dir_vars},$(call qxq,$(${dir_var}))); \
        do \
            ${MAKE} ${MAKELOOP} \
                uninstall-optional-dir-xq-$(call qxq,${DESTDIR})"$$dir" \
                    || exit $$?; \
        done)

installdirs: $(call mw,${MAKEFILE}) $(foreach dir_var,${public_dir_vars} $(${DIST}_dir_vars) $(${DIST}_subdir_vars),install-dir-xq-$(call mwxq,${DESTDIR}$(${dir_var})))

ifneq (${manlink_file},)

install-manlink-xq-%:
	${POST_UNPACK}
	@install_manlink() { ${ECHOLINE} .so "$$1" > "$$2" || ( ${REMOVE} "$$2"; exit 1 ); return $?; }; \
            ${ECHOLINEX} installing link to manpage $(call q,${manlink_file}) as $(call qsxu,install-manlink-xq-%,%,$@); \
                ${INSTALL_MANLINK} $(call q,${manlink_file}) $(call qsxu,install-manlink-xq-%,%,$@)
	${NORMAL_UNPACK}

endif

ifneq (${program_file},)

install-program-xq-%:
	${POST_UNPACK}
	@${ECHOLINEX} installing program file $(call qsxu,install-program-xq-%,%,$@); \
            ${INSTALL_PROGRAM} $(call q,${program_file}) $(call qsxu,install-program-xq-%,%,$@)
	${NORMAL_UNPACK}

endif

ifneq (${data_file},)

install-data-xq-%:
	${POST_UNPACK}
	@${ECHOLINEX} installing data file $(call qsxu,install-data-xq-%,%,$@); \
            ${INSTALL_DATA} $(call q,${data_file}) $(call qsxu,install-data-xq-%,%,$@)
	${NORMAL_UNPACK}

endif

config.h: $(call mw,${MAKEFILE})
	@${ECHOLINEX} creating $(call q,$@) && \
        ${ECHOLINE} $(call q,/* $(call ccq,$@) - configuration information for the MyMan video game\
${char_newline} *\
${char_newline} * This file was generated automatically using $(call ccq,${MAKEFILE}).\
${char_newline} * Do not edit this file by hand.\
${char_newline} */\
${char_newline}${char_number_sign}ifndef MYMAN_CONFIG_H_INCLUDED\
${char_newline}${char_number_sign}define MYMAN_CONFIG_H_INCLUDED 1\
$(foreach var,${dir_vars} DIST MYMAN MYMAN_EXE MYMANVERSION MYMANCOPYRIGHT MYMANSIZES MYMANSIZE MYMANVARIANTS MYMANVARIANT HTM_SUFFIX TXT_SUFFIX,${char_newline}${char_number_sign}ifndef $(call ucase,${var})\
${char_newline}${char_number_sign}define $(call ucase,${var}) ${char_quotation_mark}$(call cq,$(${var}))${char_quotation_mark}\
${char_newline}${char_number_sign}endif)\
$(foreach var,${configvars} MYGETOPT_H,${char_newline}${char_number_sign}ifndef ${var}\
${char_newline}${char_number_sign}define ${var} $(${var})\
${char_newline}${char_number_sign}endif)\
${char_newline}${char_number_sign}endif /* ! defined${char_left_parenthesis}MYMAN_CONFIG_H_INCLUDED${char_right_parenthesis} */\
${char_newline}) | \
            ${SED} ${SEDFLAGS} -e $(call q,s/ $$//) > $(call q,$@) || \
            ( \
                ${REMOVE} $(call q,$@) ; \
                exit 1 \
            )

${obj}mygetopt$O: $(call mw,${src})${MYGETOPTDIR}/mygetopt.c $(call mw,${src})${MYGETOPTDIR}/mygetopt.h config.h
	@${MKPARENTDIR}
	@${COMPILE}

ifneq (${obj}mygetopt$O,${obj}${hostprefix}mygetopt$o)

${obj}${hostprefix}mygetopt$o: $(call mw,${src})${MYGETOPTDIR}/mygetopt.c $(call mw,${src})${MYGETOPTDIR}/mygetopt.h config.h
	@${MKPARENTDIR}
	@${HOSTCOMPILE}

endif

${obj}${MYMAN}$o: $(call mw,${src})src/main.c $(call mw,${src})${MYGETOPTDIR}/getopt.h $(call mw,${src})${MYGETOPTDIR}/mygetopt.h $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT $(call mw,${MAKEFILE}) config.h ${UTILS}
	@${MKPARENTDIR}
	@${HOSTCOMPILE} ${WRAPPERDEFS} ${BUILTINDEFS}

${obj}utils$O: $(call mw,${src})src/utils.c config.h $(call mw,${src})${MYGETOPTDIR}/getopt.h $(call mw,${src})${MYGETOPTDIR}/mygetopt.h ${UTILS}
	@${MKPARENTDIR}
	@${COMPILE} ${WRAPPERDEFS}

ifneq (${obj}utils$O,${obj}${hostprefix}utils$o)

${obj}${hostprefix}utils$o: $(call mw,${src})src/utils.c config.h $(call mw,${src})${MYGETOPTDIR}/getopt.h $(call mw,${src})${MYGETOPTDIR}/mygetopt.h ${UTILS}
	@${MKPARENTDIR}
	@${HOSTCOMPILE} ${WRAPPERDEFS}

endif

${MYMAN}$x: $(foreach variant,${MYMANVARIANTS},$(call mw,$(call mazefile,${variant}))) $(foreach size,${MYMANSIZES},$(call mw,$(call tilefile,${size})) $(call mw,$(call spritefile,${size}))) $(call mw,${src})src/myman.c $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT ${DRIVERS} ${UTILS}

ifneq (,$(findstring pdcurses,${CURSES_FLAVOR}))
${MYMAN}$x: $(call mw,pdcicon.bmp)
endif

MYMAN_objs = $(foreach size,${MYMANSIZES},$(call mymansize_data,${size})$o) $(foreach variant,${MYMANVARIANTS},$(call mymanvariant_data,${variant})$o) ${obj}${hostprefix}game$o ${obj}${hostprefix}mygetopt$o ${obj}${hostprefix}utils$o

ifeq (yes,${with_win_icon})

${MYMAN}$x: $(call mw,${obj}winicon.res)

ifneq (,$(findstring bcc32,${HOSTCC}))
MYMAN_objs += ${obj}winicon$o

${obj}winicon.c: $(call mw,${MAKEFILE})
	@${ECHOLINEX} creating $(call q,$@) && \
        ${ECHOLINE} $(call q,${char_number_sign}pragma resource "$(call cq,${obj}winicon.res)") > $(call q,$@)

${obj}winicon$o: $(call mw,${obj}winicon.c)
	@${HOSTCOMPILE}

else
ifneq (,$(findstring windres,${HOSTRC}))
MYMAN_objs += ${obj}winicon$o

${obj}winicon$o: $(call mw,${obj}winicon.res)
	@$(call hostbind_resource,$<)

else
ifeq (,$(findstring owcc,${HOSTLINK}))
MYMAN_objs += ${obj}winicon.res
endif
endif
endif

endif

ifeq (yes,${with_mac_icon})
${MYMAN}$x: $(call mw,${obj}${MYMAN}.icns)
endif

${MYMAN}$x: $(call mw,${obj}${MYMAN}$o) $(foreach object,${MYMAN_objs},$(call mw,${object}))
	${HOSTLINK} $(foreach object,${MYMAN_objs},$(call q,${object})) ${CURSESLIBS} ${ICONVLIBS} ${HOSTLIBS}
ifeq (yes,${with_win_icon})
ifneq (,$(findstring owcc,${HOSTLINK}))
	@$(call hostbind_resource,${obj}winicon.res)
endif
endif
ifeq (yes,${with_mac_icon})
	@-($(call set_mac_icon,${obj}${MYMAN}.icns) $(call q,$@))
endif

${XMYMAN}${sh}: $(call mw,${src})utl/xmyman.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${XMYMAN2}${sh}: $(call mw,${src})utl/xmyman2.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${XMYMAN3}${sh}: $(call mw,${src})utl/xmyman3.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${XMYMAN4}${sh}: $(call mw,${src})utl/xmyman4.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${XBIGMAN}${sh}: $(call mw,${src})utl/xbigman.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${XQUACKMAN}${sh}: $(call mw,${src})utl/xquack.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${MYMANCOMMAND}: $(call mw,${src})utl/mymancmd.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${XHUGEMAN}${sh}: $(call mw,${src})utl/xhugeman.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${XBITMAN}${sh}: $(call mw,${src})utl/xbitman.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${XBITMAN2}${sh}: $(call mw,${src})utl/xbitman2.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${XBITMAN3}${sh}: $(call mw,${src})utl/xbitman3.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${XBITMAN4}${sh}: $(call mw,${src})utl/xbitman4.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${XBITMAN5}${sh}: $(call mw,${src})utl/xbitman5.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${XBITMAN6}${sh}: $(call mw,${src})utl/xbitman6.in $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${SH_SUBSTITUTE}
	-chmod 755 $(call q,$@)

${MYMAN}${man6ext}: $(call mw,${src})src/myman.man $(call mw,${MAKEFILE}) $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@${TROFF_SUBSTITUTE}

${MYMAN}${txt}: ${MYMAN}${man6ext}
	@-${MANTOTXT}

${MYMAN}.ps: ${MYMAN}${man6ext}
	@-${MANTOPS}

${MYMAN}.dvi: ${MYMAN}${man6ext}
	@-${MANTODVI}

${MYMAN}${htm}: ${MYMAN}${man6ext}
	@-${MANTOHTML}

${obj}${hostprefix}${GAME}$o: $(call mw,${src})${MYGETOPTDIR}/getopt.h $(call mw,${src})${MYGETOPTDIR}/mygetopt.h ${DRIVERS} ${UTILS}

${obj}${hostprefix}${GAME}$o: $(call mw,${src})src/myman.c $(call mw,${src})${MYGETOPTDIR}/getopt.h $(call mw,${src})${MYGETOPTDIR}/mygetopt.h $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT ${DRIVERS} ${UTILS} config.h
	@${MKPARENTDIR}
	@${HOSTCOMPILE} ${CURSOPTS} ${EXTRACURSOPTS} ${ICONVOPTS} ${EXTRAICONVOPTS} ${CURSESINCLUDE} $(call gamedefs,${MYMANVARIANT},${MYMANSIZE}) ${WRAPPERDEFS} ${DATADEFS}

${GAME}$X: $(call mw,${obj}mygetopt$O) $(call mw,${obj}utils$O)

ifneq (${GAME}$X,${hostprefix}${GAME}$x)

${hostprefix}${GAME}$x: $(call mw,${obj}${hostprefix}mygetopt$O) $(call mw,${obj}${hostprefix}utils$O)

endif

${obj}s2v%$o: $(call mw,${obj}s2v)%.c
	@${MKPARENTDIR}
	@${HOSTCOMPILE} ${DATADEFS} $(call datafiledefs,$(word $(call s,$(call xq,${obj}s2v)%$(call xq,$o),%,$@),${all_variants}),${MYMANSIZE})

${obj}s2s%$o: $(call mw,${obj}s2s)%.c
	@${MKPARENTDIR}
	@${HOSTCOMPILE} ${DATADEFS} $(call datafiledefs,${MYMANVARIANT},$(word $(call s,$(call xq,${obj}s2s)%$(call xq,$o),%,$@),${all_sizes}))

${BOOTSTRAP}$X: $(call mw,${BOOTSTRAP}$O) $(call mw,${obj}mygetopt$O) $(call mw,${obj}utils$O)
	@${MKPARENTDIR}
	${LINK} ${BUILDCURSESLIBS} ${BUILDICONVLIBS} $(call q,${obj}mygetopt$O) $(call mw,${obj}utils$O) ${LIBS}

${BOOTSTRAP}$O: $(call mw,${src})src/myman.c $(call mw,${src})${MYGETOPTDIR}/getopt.h $(call mw,${src})${MYGETOPTDIR}/mygetopt.h ${DRIVERS} $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT ${DRIVERS} ${UTILS} config.h
	@${MKPARENTDIR}
	@${COMPILE} ${BUILDCURSOPTS} ${EXTRABUILDCURSOPTS} ${BUILDICONVOPTS} ${BUILDCURSESINCLUDE} ${EXTRABUILDICONVOPTS} $(call gamedefs,${MYMANVARIANT},${MYMANSIZE}) ${WRAPPERDEFS}

define variant_template
$$(call mymanvariant_data,$1).c: $$(call mw,$${BOOTSTRAP}$$X) $$(call mw,$$(call tilefile,$${MYMANSIZE})) $$(call mw,$$(call spritefile,$${MYMANSIZE})) $$(call mw,$$(call mazefile,$1))
	@${MKPARENTDIR}
	@($${ECHOLINEX} $$(call q,creating $$(call mymanvariant_data,$1).c from $$(call mazefile,$1) for --variant=$1); \
            $$(call q,$${XBOOTSTRAP}) -m $$(call q,$$(call mazefile,$1)) -MF $$(call q,$$(call mymanvariant_data,$1).c) \
            || exit $$$$?; \
        )
endef

$(eval $(foreach variant,${MYMANVARIANTS},$(call variant_template,${variant})${char_newline}))

define size_template
$$(call mymansize_data,$1).c: $$(call mw,$${BOOTSTRAP}$$X) $$(call mw,$$(call tilefile,$1)) $$(call mw,$$(call spritefile,$1)) $$(call mw,$$(call mazefile,${MYMANVARIANT}))
	@${MKPARENTDIR}
	@($${ECHOLINEX} $$(call q,creating $$(call mymansize_data,$1).c from $$(call tilefile,$1) and $$(call spritefile,$1) for --size=$1) ; \
            $$(call q,$${XBOOTSTRAP}) -t $$(call q,$$(call tilefile,$1)) -s $$(call q,$$(call spritefile,$1)) -STF $$(call q,$$(call mymansize_data,$1).c) \
            || exit $$$$?; \
        )
endef

$(eval $(foreach size,${MYMANSIZES},$(call size_template,${size})${char_newline}))

$(call mw,${MAKEFILE}):: $(call mw,${src})VERSION $(call mw,${src})COPYRIGHT
	@touch $(call q,$@)

# use these to help diagnose mis-set makefile variables; replace '%'
# with a Make variable name
dumpx-%:
	@${ECHOLINEX} $(call q,$(value $(call s,dumpx-%,%,$@)))
dump-%:
	@${ECHOLINE} $(call q,$(value $(call s,dump-%,%,$@)))

# these all use 'xq'-quoting for the '%' part in order to allow
# solidus (/) characters and other strange stuff to appear in the
# argument

install-dir-xq-%:
	${POST_UNPACK}
	@test -d $(call qsxu,install-dir-xq-%,%,$@) || \
            (${ECHOLINEX} creating directory $(call qsxu,install-dir-xq-%,%,$@) && \
                ${INSTALL_DIR} $(call qsxu,install-dir-xq-%,%,$@))
	${NORMAL_UNPACK}

uninstall-optional-dir-xq-%:
	@-test ! -d $(call qsxu,uninstall-optional-dir-xq-%,%,$@) || \
            (${REMOVE_DIR} $(call qsxu,uninstall-optional-dir-xq-%,%,$@) && \
                (test -d $(call qsxu,uninstall-optional-dir-xq-%,%,$@) || \
                    ${ECHOLINEX} removed directory $(call qsxu,uninstall-optional-dir-xq-%,%,$@)))

uninstall-dir-xq-%:
	@test ! -d $(call qsxu,uninstall-dir-xq-%,%,$@) || \
            (${ECHOLINEX} removing directory $(call qsxu,uninstall-dir-xq-%,%,$@) && \
                ${REMOVE_DIR} $(call qsxu,uninstall-dir-xq-%,%,$@))

wipe-dir-xq-%:
	test ! -d $(call qsxu,wipe-dir-xq-%,%,$@) || \
            rm -rf $(call qsxu,wipe-dir-xq-%,%,$@)

empty-dir-xq-%: wipe-dir-xq-%
	@${MAKE} ${MAKELOOP} \
            $(call qs,empty-dir-xq-%,install-dir-xq-%,$@)

.PHONY: tarball-xq-%

tarball-xq-%:
	@${MAKE} ${MAKELOOP} \
            $(call qsxu,tarball-xq-%,fill-dir-xq-%,$@)
	test ! -f $(call qsxu,tarball-xq-%,%$(call xq,${tar}),$@) || \
            ${REMOVE} $(call qsxu,tarball-xq-%,%$(call xq,${tar}),$@)
	tar -cf $(call qsxu,tarball-xq-%,%$(call xq,${tar}),$@) $(call qsxu,tarball-xq-%,%,$@)

.PHONY: compressed-tarball-xq-%

compressed-tarball-xq-%: tarball-xq-%
	test ! -f $(call qsxu,compressed-tarball-xq-%,%$(call xq,${tgz}),$@) || \
            ${REMOVE} $(call qsxu,compressed-tarball-xq-%,%$(call xq,${tgz}),$@)
	gzip -c -9 < $(call qsxu,tarball-xq-%,%$(call xq,${tar}),$<) > $(call qsxu,compressed-tarball-xq-%,%$(call xq,${tgz}),$@) || \
        ( \
            ${REMOVE} $(call qsxu,compressed-tarball-xq-%,%$(call xq,${tgz}),$@); \
            exit 1 \
        )

.PHONY: summary

summary:
	@${ECHOLINE} Configuration summary:
	@$(foreach var,$(filter-out this_file_undergoes_variable_substitution,${substitute_vars}),echo '    '${var}': '$(call q,$(${var}));)

# LocalWords:  myman

