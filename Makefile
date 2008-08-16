# Makefile - build script for the MyMan video game
# Copyright 1997-2006, Benjamin Sittler <bsittler@gmail.com>
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

SHELL = /bin/sh

# NOTE: you will need a fairly modern version of GNU Make (or a
# compatible program, if such a thing exists) to run this Makefile;
# the following version is known to work:
gnumake_ok_tested_version := GNU Make 3.80

gnumake_too_old_get_3_80 = $(if true,true,$(gnumake_too_old_get_3_80))

$(START_STRING_QUOTING_LIBRARY)


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
char_newline := $(subst $(char_tab),,$(char_newline))
char_tab := $(subst :,,$(char_tab))
export char_tab
export char_newline
nil :=
char_space := $(nil) $(nil)
char_dollar_sign := $$
char_reverse_solidus := \$(nil)
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
$(charclass_ascii_space) \
$(charclass_ascii_punct) \
$(charclass_ascii_digit) \
$(charclass_ascii_upper) \
$(charclass_ascii_lower)

charclass_ascii_id := \
$(charclass_ascii_lower) \
$(charclass_ascii_upper) \
$(charclass_ascii_digit) \
char_low_line


## case-folding functions

# ucase: convert lower case -> upper case
$(eval \
ucase $(char_equals_sign) $(subst $(char_comma)$(char_space),$(char_comma),$(foreach charname,$(strip $(charclass_ascii_lower)),$$$(char_left_parenthesis)subst $$(value $(charname))$(char_comma)$$(value $(subst char_latin_small_letter_,char_latin_capital_letter_,$(charname)))$(char_comma)))$(char_dollar_sign)1$(subst $(char_space),,$(patsubst %,$(char_right_parenthesis),$(strip $(charclass_ascii_lower)))))


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
$(charclass_ascii_id)

# characters that need simple backslashing for the shell
charclass_q_unsafe := \
$(filter-out char_reverse_solidus $(charclass_q_safe),$(charclass_all)) \
char_reverse_solidus

# q: re-quote $1 for interpretation as a shell word
$(eval \
q $(char_equals_sign) $(char_dollar_sign)$(char_left_parenthesis)subst $(char_dollar_sign)(char_newline),"$(char_dollar_sign)$(char_dollar_sign){char_newline}",$(subst $(char_comma)$(char_space),$(char_comma),$(foreach charname,$(strip $(charclass_q_unsafe)),$$$(char_left_parenthesis)subst $$(value $(charname))$(char_comma)$$(char_reverse_solidus)$$(value $(charname))$(char_comma)))$(char_dollar_sign)1$(subst $(char_space),,$(patsubst %,$(char_right_parenthesis),$(strip $(charclass_q_unsafe))))$(char_right_parenthesis))

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
cq $(char_equals_sign) $(char_dollar_sign)$(char_left_parenthesis)subst $(char_dollar_sign)(char_tab)$(char_comma)$(char_reverse_solidus)t$(char_comma)$(char_dollar_sign)$(char_left_parenthesis)subst $(char_dollar_sign)(char_newline)$(char_comma)$(char_reverse_solidus)n$(char_comma)$(subst $(char_comma)$(char_space),$(char_comma),$(foreach charname,$(strip $(charclass_cq_unsafe)),$$$(char_left_parenthesis)subst $$(value $(charname))$(char_comma)$$(char_reverse_solidus)$$(value $(charname))$(char_comma)))$(char_dollar_sign)1$(subst $(char_space),,$(patsubst %,$(char_right_parenthesis),$(strip $(charclass_cq_unsafe))))$(char_right_parenthesis)$(char_right_parenthesis))

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

mw = $(if $(findstring $(char_reverse_solidus)$(char_reverse_solidus)$(char_space),$(subst $(char_tab),$(char_space),$(subst $(char_vertical_line),$(char_space),$1))),$(error two reverse solidus characters ($(char_reverse_solidus)$(char_reverse_solidus)) followed by a space character ($(char_space))$(char_comma) a tab character ($(char_tab))$(char_comma) or a vertical line character ($(char_vertical_line)) are not yet supported in GNU make targets or dependencies; such a pattern occurs at least once in the string $(char_grave_accent)$1$(char_apostrophe)),$(if $(findstring $(char_colon),$1)$(findstring $(char_semicolon),$1)$(findstring $(char_dollar_sign)$(char_commercial_at),$1),$(error the colon character ($(char_colon)), semicolon character ($(char_semicolon)) and dollar sign-commercial at character sequence ($(char_dollar_sign)$(char_commercial_at)) are not yet supported in GNU Make targets or dependencies; such a character or sequence occurs at least once in the string $(char_grave_accent)$1$(char_apostrophe)),$(subst $(char_reverse_solidus)_open_reverse_solidus,$(char_reverse_solidus),$(subst $(char_reverse_solidus)_close_reverse_solidus,,$(subst $(char_reverse_solidus)_close_reverse_solidus$(char_reverse_solidus),$(char_reverse_solidus)$(char_reverse_solidus),$(subst $(char_reverse_solidus)_close_reverse_solidus$(char_reverse_solidus)_open_reverse_solidus,$(char_reverse_solidus)_open_reverse_solidus,$(subst $(char_vertical_line),$(char_reverse_solidus)$(char_vertical_line),$(subst $(char_colon),$(char_reverse_solidus)$(char_colon),$(subst $(char_tab),$(char_reverse_solidus)$(char_tab),$(subst $(char_space),$(char_reverse_solidus)$(char_space),$(subst $(char_reverse_solidus),$(char_reverse_solidus)_open_reverse_solidus$(char_reverse_solidus)_close_reverse_solidus,$1)))))))))))

# mq: quote dollar signs (useful when passing make variable
# settings through the command-line to a recursive invocation of make)
mq = $(subst $(char_dollar_sign),$(char_dollar_sign)$(char_dollar_sign),$1)

# sh: run shell commands with arguments possibly containing
# strings quoted by $(call q,...)
sh = $(shell char_newline=`echo ;echo :`; char_newline="$${char_newline%:}"; export char_newline; $(SHELL) -c $(call q,$1))

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

xq = $(subst %,_@,$(subst $(char_tab),_=,$(subst $(char_space),_-,$(subst $(char_solidus),_|,$(subst _,__,$1)))))
xu = $(subst __.,_,$(subst _|,$(char_solidus),$(subst _-,$(char_space),$(subst _=,$(char_tab),$(subst _@,%,$(subst __,__.,$1))))))

# s: like patsubst, but makes only a single replacement; $1 is source
# pattern (treated as a single word,) $2 is replacement pattern
# (treated as a signle word,) and $3 is the string to act upon (also
# treated as a single word); note that you need to quote the non-%
# parts of $1 and $2 using $(call qx,...)
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

$(END_STRING_QUOTING_LIBRARY)


# source directory for package
srcdir ?= .

# build date
ifeq ($(origin date),undefined)
date := $(call sh,$(SHELL) $(call q,$(srcdir))/configure --dump=date)
endif

## build: type of system to build the program on
ifeq ($(origin build),undefined)
build := $(call sh,$(SHELL) $(call q,$(srcdir))/configure --dump=build)
endif

## host: type of system to build the program for

# this must contain only characters that will be interpreted literally
# by both $(MAKE) and $(SHELL) in the following contexts: bare word,
# single-quoted string, double-quoted string, globbing pattern, suffix
# pattern
ifeq ($(origin host),undefined)
host := $(call sh,$(SHELL) $(call q,$(srcdir))/configure --dump=host)
endif

ifneq ($(host),$(build))
hostprefix ?= $(host)-
else
hostprefix ?=
endif

## MYMAN: base name for package (without version number or suffix)

# this must be a simple identifier (an ascii letter or ascii low line
# [_] followed by zero or more of: ascii letter, ascii digit or ascii
# low line)
MYMAN ?= myman

## VERSION: package version number

# this must contain only characters that will be interpreted literally
# by both $(MAKE) and $(SHELL) in the following contexts: bare word,
# single-quoted string, double-quoted string, globbing pattern, suffix
# pattern

# additionally, ascii hyphen-minus [-] is discouraged here -- at least
# for portable versions of MyMan -- since some manual-page processing
# tools (namely Solaris makewhatis) cannot parse a .TH line in a
# manual page with more than one ascii hyphen-minus
ifeq ($(origin VERSION),undefined)
VERSION := $(call sh,cat $(call q,$(srcdir))/VERSION)
endif

## DIST: full, versioned package name (without suffix)

# this must contain only characters that will be interpreted literally
# by both $(MAKE) and $(SHELL) in the following contexts: bare word,
# single-quoted string, double-quoted string, globbing pattern, suffix
# pattern
DIST ?= $(MYMAN)-$(VERSION)

## BINDIST: binary package name (without suffix)

# this must contain only characters that will be interpreted literally
# by both $(MAKE) and $(SHELL) in the following contexts: bare word,
# single-quoted string, double-quoted string, globbing pattern, suffix
# pattern
BINDIST ?= $(DIST)-$(host)

## directories

# distroot: subdirectory inside package that corresponds to the root
# directory on the machine where the package is to be installed
distroot = dist

# DESTDIR: installed filename prefix string

# if set, DESTDIR is prefixed to all directory names during `make
# install' (useful for creating installation snapshot directories)

# e.g. DESTDIR = ./$(BINDIST)/$(distroot)
DESTDIR ?=

# Common prefix for installation directories.
# NOTE: This directory must exist when you start the install.
prefix ?= /usr/local
exec_prefix ?= $(prefix)
# Where to put the executable for the command `myman'
bindir ?= $(exec_prefix)/bin
# Where to put the directory of architecture-dependent helper executables
libexecdir ?= $(exec_prefix)/libexec
# Root directory for architecture-independent data files
datarootdir ?= $(prefix)/share
# Where to put the directory of architecture-independent data files
datadir ?= $(datarootdir)
# Where to put the plain-text documentation
docdir ?= $(datarootdir)/doc
# The top-level directory for installing the man pages for this package
mandir ?= $(datarootdir)/man
# The extension for installing man pages
manext ?= .1

# The directory for installing section 6 man pages
man6dir ?= $(mandir)/man6
# The file name extension for installed section 6 man pages
man6ext ?= $(subst 1,6,$(manext))

# Directory of individual tile/sprite size and maze variant binaries
privatelibexecdir ?= $(libexecdir)/$(BINDIST)
# Directory of maze, tile and sprite datafiles
privatedatadir ?= $(datadir)/$(DIST)
# Directory of documentation
privatedocdir ?= $(docdir)/$(DIST)

# variable names for shared installation directories
public_dir_vars = \
prefix \
exec_prefix \
bindir \
libexecdir \
datadir \
docdir \
mandir \
man6dir

# variable names for private installation directories used only by
# this distribution of MyMan

$(DIST)_dir_vars = \
privatelibexecdir \
privatedatadir \
privatedocdir

# variable names for all installation directories
dir_vars = $(public_dir_vars) $($(DIST)_dir_vars)

# How to find this makefile
MAKEFILE ?= $(srcdir)/Makefile

## MAKELOOP: loopback recursive $(MAKE)

# names of makefile variables to pass to recursive $(MAKE); these
# variables are computed by external commands during interpretation of
# the makefile, so we'd like to avoid expensive and redundant
# re-computations
makeloop_vars = \
SHELL \
date \
build \
host \
VERSION \
xcurses_libraries \
xcurses_includes

# flags for loopback recursive $(MAKE)
MAKELOOP = \
    -f $(call q,$(MAKEFILE)) \
    --no-print-directory \
    $(foreach var,$(makeloop_vars),$(var)=$(call qmq,$($(var))))

##
## Configuration for the included my_getopt distribution
##

# Where to find the my_getopt files relative to $(srcdir)
MY_GETOPTDIR=my_getopt

# MY_GETOPT_H is the argument to #include in order to include the
# 'getopt.h' file from $(MY_GETOPTDIR) from a source file inside the
# $(srcdir) directory.
MY_GETOPTCPPFLAGS=-DMY_GETOPT_H=\"$(call qcq,$(MY_GETOPTDIR)/getopt.h)\"


##
## Toolchain Configuration
##

## Conventional filename suffices

# Option 1: GNU, BSD, Unix, etc.
ifeq ($(subst -cygwin32-,,$(subst -cygwin-,--,$(subst -msdosdjgpp-,--,-$(OSTYPE)-))),-$(OSTYPE)-)
OBJ_SUFFIX=.o
LIB_SUFFIX=.a
EXE_SUFFIX=
BAK_SUFFIX=~
HTM_SUFFIX=.html
TAR_SUFFIX=.tar
TGZ_SUFFIX=.tar.gz

else

# Option 2: Cygwin, EMX, DJGPP, etc. [hybrid between GNU and FreeDOS]
OBJ_SUFFIX=.o
LIB_SUFFIX=.a
EXE_SUFFIX=.exe
BAK_SUFFIX=~
HTM_SUFFIX=.html
TAR_SUFFIX=.tar
TGZ_SUFFIX=.tar.gz
endif

# Option 3: FreeDOS, OpenDOS, etc. [does anyone use them now?]
#OBJ_SUFFIX=.obj
#LIB_SUFFIX=.lib
#EXE_SUFFIX=.exe
#BAK_SUFFIX=.bak
#HTM_SUFFIX=.htm
#TAR_SUFFIX=.tar
#TGZ_SUFFIX=.tgz

# Convenient abbreviated versions of the common suffices
o = $(OBJ_SUFFIX)
a = $(LIB_SUFFIX)
x = $(EXE_SUFFIX)
bak = $(BAK_SUFFIX)
htm = $(HTM_SUFFIX)
tar = $(TAR_SUFFIX)
tgz = $(TGZ_SUFFIX)

# file removal
RM = rm
RMFLAGS = -f
REMOVE = $(RM) $(RMFLAGS) $(EXTRARMFLAGS)

# directory removal (empty directories only!)
RMDIR = rmdir
RMDIRFLAGS = --ignore-fail-on-non-empty
REMOVE_DIR = $(RMDIR) $(RMDIRFLAGS) $(EXTRARMDIRFLAGS)

# file and directory installation
#INSTALL = /usr/bin/install
INSTALL = install
INSTALLFLAGS =
STRIPINSTALLFLAGS =
INSTALL_PROGRAM = $(INSTALL) $(INSTALLFLAGS) $(EXTRAINSTALLFLAGS)
INSTALL_DATA = $(INSTALL) -m 644
INSTALL_DIR = $(INSTALL) -d
INSTALL_MANLINK = install_manlink

## non-backslash-interpreting "echo"-equivalent used during file generation
ECHOLINE = $(SHELL) $(call q,$(srcdir))/configure --dump=non_option_arguments --

## like $(ECHOLINE), but does C string-style quoting (apostrophe is not quoted)
ECHOLINEX = $(SHELL) $(call q,$(srcdir))/configure --dump=non_option_arguments --dump-escaped --

# special human-readable variable used to distinguish source from
# substituted results
this_file_undergoes_variable_substitution = \
automatically generated $(date) from $< for $(MYMAN) $(VERSION)

## variable substitution

# names of variables to subtitute
substitute_vars = \
MYMAN \
VERSION \
host \
date \
DIST \
BINDIST \
SIZES \
VARIANTS \
CURSES_FLAVOR \
CURSES_FLAVOR_TARGET \
SHELL \
DESTDIR \
$(dir_vars) \
HTM_SUFFIX \
EXE_SUFFIX \
SHELL \
this_file_undergoes_variable_substitution

# finalize values of all substituted variables
$(eval \
$(foreach substitute_var,$(substitute_vars),$(substitute_var) $(char_colon)$(char_equals_sign) $(char_dollar_sign)($(substitute_var))$(char_newline)))

# The substitution language is as follows for troff/groff manpages:

# @VARIABLE@ -> value of substituted variable $(VARIABLE)
#     \ in value of variable -> \e
#     . in value of variable -> \.
#     " in value of variable -> \"
#     - in value of variable -> \-
# @@ or unpaired @ alone on a line -> literal @
# @something else@ -> @something else

# substitution rule for troff/groff manpages
TROFF_SUBSTITUTE = \
$(ECHOLINEX) creating $(call q,$@) from $(call q,$<) && \
    sed -e 's|@\([^@]*\)@|@(\1)@|g;$(foreach var,$(substitute_vars),s|@($(var))@|'$(call q,$(subst &,\&,$(subst |,\|,$(subst $(char_space),\\$(char_space),$(subst -,\\-,$(subst $(char_quotation_mark),\\$(char_left_parenthesis)dq,$(subst .,\.,$(subst \,\\e,$(call cq,$($(var)))))))))))'|g;)s|@(\([^@]*\))@|@\1|g; ' < $(call q,$<) > $(call q,$@) || \
    ( \
        $(REMOVE) $(call q,$@) && \
            exit 1 \
    )

## Tools for updating the "whatis" database

# newer mandb style
MANDB = mandb
MANDBFLAGS =

# older makewhatis style (used if newer mandb style fails)
MAKEWHATIS = makewhatis
MAKEWHATISFLAGS =

## C preprocessing flags
CPPFLAGS = $(MY_GETOPTCPPFLAGS) $(EXTRACPPFLAGS)
HOSTCPPFLAGS = $(CPPFLAGS)

## ANSI C/C++ compiler (choose one)

# NOTE: MyMan is written in ANSI C, but pains have been taken to make
# sure it is also valid ANSI C++. Compiling using a C++ compiler
# rather than a C compiler may improve optimization and help find
# subtle type incompatibilities which would go unnoticed by a plain C
# compiler.

# Compiler for build machine

# Option 1: GNU C or C++ compiler (g++ or gcc)
#CC = g++ -g -Wall
CC = gcc

# Option 2: Old C or C++ compiler (c++, cc or c89)
#CC = c++
#CC = cc
#CC = c89

ifneq (,$(findstring gcc,$(subst g++,gcc,$(subst gxx,g++,$(CC)))))

# looks like GCC, let's generate debugging information and warnings
CFLAGS = $(EXTRACFLAGS) -g -Wall

else

# other compiler, at least try to generate debugging information
CFLAGS = $(EXTRACFLAGS) -g

endif


# Compiler for host machine

ifeq (,$(hostprefix))
HOSTCC = $(CC)
else
HOSTCC = $(hostprefix)gcc
endif

ifneq (,$(findstring gcc,$(subst g++,gcc,$(subst gxx,g++,$(HOSTCC)))))

# looks like GCC, let's optimize
HOSTCFLAGS = $(CFLAGS) $(EXTRAHOSTCFLAGS) -O3

else

# other compiler, don't try to optimize sizne that may conflict with
# debugging flags
HOSTCFLAGS = $(CFLAGS) $(EXTRAHOSTCFLAGS)

endif

# Complete preprocessing and compilation action for option 1 or 2
COMPILE = $(CC) $(CFLAGS) $(CPPFLAGS) -o $(call q,$@) -c $(call q,$<)
HOSTCOMPILE = $(HOSTCC) $(HOSTCFLAGS) $(HOSTCPPFLAGS) -o $(call q,$@) -c $(call q,$<)

## Libraries for all executables
LIBS = $(EXTRALIBS)
HOSTLIBS = $(LIBS) $(EXTRAHOSTLIBS)

## Linker (choose one)

# Option 1: Link using C/C++ compiler (specified above)
LD = $(CC)
LDFLAGS = $(CFLAGS) $(EXTRALDFLAGS)

HOSTLD = $(HOSTCC)
HOSTLDFLAGS = $(HOSTCFLAGS) $(EXTRAHOSTLDFLAGS)

# Option 2: Link using system linker
#LD = ld
#LDFLAGS = $(EXTRALDFLAGS)

# Complete linking action for option 1 or 2
LINK = $(LD) $(LDFLAGS) -o $(call q,$@) $(call q,$<)
HOSTLINK = $(HOSTLD) $(HOSTLDFLAGS) -o $(call q,$@) $(call q,$<)


##
## Game configuration
##

## create a single, monolithic executable
enable_monolithic ?= yes

## game sizes and variants supported by source
all_variants = myman catchum chomp gobble munchkin pacgal pacmanic portable spear pacman89 quackman tiny pacmania
all_sizes = small big huge square quack

## game sizes and variants to actually build
enable_variants ?= yes
enable_variant ?= yes

VARIANTS := $(patsubst ,$(firstword $(all_variants)),$(strip $(foreach variant,$(all_variants),$(firstword $(patsubst no,,$(patsubst ,$(filter $(variant),$(patsubst yes,$(variant),$(enable_variants))),$(patsubst ,$(filter $(variant),$(enable_variant)),$(filter $(variant) no,$(patsubst yes,$(variant),$(enable_variant_$(variant)))))))))))
VARIANT := $(firstword $(patsubst ,(none),$(filter $(all_variants),$(patsubst yes,$(firstword $(VARIANTS)),$(patsubst no,,$(enable_variant))))))

enable_sizes ?= yes
enable_size ?= yes

SIZES := $(patsubst ,$(firstword $(all_sizes)),$(strip $(foreach size,$(all_sizes),$(firstword $(patsubst no,,$(patsubst ,$(filter $(size),$(patsubst yes,$(size),$(enable_sizes))),$(patsubst ,$(filter $(size),$(enable_size)),$(filter $(size) no,$(patsubst yes,$(size),$(enable_size_$(size)))))))))))
SIZE := $(firstword $(patsubst ,(none),$(filter $(all_sizes),$(patsubst yes,$(firstword $(SIZES)),$(patsubst no,,$(enable_size))))))

# final version basename
GAME = v$(call idx,$(VARIANT),$(VARIANTS))s$(call idx,$(SIZE),$(SIZES))$(VARIANT)-$(SIZE)
# initial bootstrap version basename
BOOTSTRAP = s1$(GAME)
# intermediate builtin data file basename
DATA = s2$(GAME)

## for builtin data
ifeq ($(enable_monolithic),yes)
DATADEFS = -DBUILTIN_MAZE -DBUILTIN_TILE -DBUILTIN_STILE -Dbuiltin_mazefile=builtin_mazefile_$(VARIANT)_$(SIZE) -Dmaze_data=maze_data_$(VARIANT)_$(SIZE) -Dbuiltin_tilefile=builtin_tilefile_$(VARIANT)_$(SIZE) -Dtile=tile_$(VARIANT)_$(SIZE) -Dtile_used=tile_used_$(VARIANT)_$(SIZE) -Dtile_color=tile_color_$(VARIANT)_$(SIZE) -Dbuiltin_stilefile=builtin_stilefile_$(VARIANT)_$(SIZE) -Dstile=stile_$(VARIANT)_$(SIZE) -Dstile_used=stile_used_$(VARIANT)_$(SIZE) -Dstile_color=stile_color_$(VARIANT)_$(SIZE) -Dmain=main_$(VARIANT)_$(SIZE)
else
DATADEFS = -DBUILTIN_MAZE -DBUILTIN_TILE -DBUILTIN_STILE
endif

## curses options

# curses library

# ncurses under Linux, improperly installed in /usr
#CURSESLIBS = -L/usr/lib -lncurses
#CURSESINCLUDE = -I/usr/include/ncurses
# same, in /usr/local
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
# Use CP-437 graphics characters by default: (PC-console font)
#CURSOPTS = -DUSE_RAW=1
# Disable most character attributes:
#CURSOPTS = -DUSE_ATTR=0
# Exclude color support:
#CURSOPTS = -DUSE_COLOR=0
# Compile in color support, but leave it disabled by default:
#CURSOPTS = -DUSE_COLOR=1 -DCOLORIZE=0

CURSES_FLAVOR_TARGET = a text terminal or terminal emulator

ifeq (yes,$(with_slang_utf8))

CURSES_FLAVOR = slang-utf8

# UTF-8 S/Lang curses emulation (a.k.a. slcurses)
CURSESLIBS += -lslang
CURSESINCLUDE += $(call q,-DMY_CURSES_H=<slcurses.h>)

# use UCS/Unicode characters for internal representation
CURSOPTS += -DUSE_RAW_UCS=1

# enable slcurses compatibility hacks in myman.c
CURSOPTS += -DSLCURSES

else
ifeq (yes,$(with_slang))

CURSES_FLAVOR = slang

# S/Lang curses emulation (a.k.a. slcurses)
CURSESLIBS += -lslang
CURSESINCLUDE += $(call q,-DMY_CURSES_H=<slcurses.h>)

# enable slcurses compatibility hacks in myman.c
CURSOPTS += -DSLCURSES

else
ifeq (yes,$(with_xcurses))

CURSES_FLAVOR = xcurses
CURSES_FLAVOR_TARGET = an X Window System display

# PDCurses for X (a.k.a. XCurses)
XCURSES_CONFIG = xcurses-config
xcurses_libraries := $(call sh,$(SHELL) -c $(call q,$(XCURSES_CONFIG) --libs) </dev/null 2>/dev/null)
xcurses_includes := $(call sh,$(SHELL) -c $(call q,$(XCURSES_CONFIG) --cflags) </dev/null 2>/dev/null)
CURSESLIBS += $(xcurses_libraries)
CURSESINCLUDE += $(xcurses_includes)

EXTRAWRAPPERDEFS += -DXCURSES

else
ifeq (yes,$(with_ncursesw))

CURSES_FLAVOR = ncursesw

# ncurses (wide character version)
CURSESLIBS += -lncursesw
CURSESINCLUDE += $(call q,-DMY_CURSES_H=<ncursesw/curses.h>)

# use X/OPEN wide character curses API when in raw mode
CURSOPTS += -D_XOPEN_SOURCE_EXTENDED -D_XOPEN_SOURCE=500

else
ifeq (yes,$(with_ncurses))

CURSES_FLAVOR = ncurses

# ncurses in the usual place, but called "ncurses" instead of "curses"
CURSESLIBS += -lncurses
CURSESINCLUDE += $(call q,-DMY_CURSES_H=<ncurses.h>)

else
ifeq (yes,$(with_pdcurses))

# PDCurses
CURSES_FLAVOR = pdcurses
CURSES_FLAVOR_TARGET = a text window

# pdcurses in the usual place
CURSESLIBS += -lpdcurses

else

CURSES_FLAVOR = curses

# ncurses or SysV curses in the usual place
CURSESLIBS += -lcurses

endif
endif
endif
endif
endif
endif

## Tiles

# Big tiles using ASCII-art
TILEFILE_big = tile5x2.txt

# Huge tiles using ASCII-art
TILEFILE_huge = tile5x3.txt
TILEDEFS_huge = -DTILE_W=5 -DTILE_H=3

# Single-character tiles using altcharset translations (VT100-style
# graphics) or raw tile characters (CP437 or UCS/Unicode character
# graphics)
TILEFILE_square = tile1x1.txt
TILEDEFS_square = -DTILE_W=1 -DTILE_H=1

# Small tiles using altcharset translations (VT100-style graphics) or
# raw tile characters (CP437 or UCS/Unicode character graphics)
TILEFILE_small = tile2x1.txt
TILEDEFS_small = -DTILE_W=2 -DTILE_H=1

# Like small with a duck and bunnies
TILEFILE_quack = tile2x1.txt
TILEDEFS_quack = -DTILE_W=2 -DTILE_H=1

# Tile definitions for the selected size
TILEFILE = $(srcdir)/$(TILEFILE_$(SIZE))
TILEDEFS = -DTILEFILE=\"$(call qcq,$(TILEFILE))\" $(TILEDEFS_$(SIZE))

## Sprites

# Big sprites
STILEFILE_big = stile7x3.txt

# Huge sprites
STILEFILE_huge = stile10x6.txt
STILEDEFS_huge = -DSTILE_W=10 -DSTILE_H=6

# Square sprites
STILEFILE_square = stile1x1.txt
STILEDEFS_square = -DSTILE_W=1 -DSTILE_H=1 -DVISIBLE_EYES=0

# Small sprites
STILEFILE_small = stile2x1.txt
STILEDEFS_small = -DSTILE_W=2 -DSTILE_H=1 -DVISIBLE_EYES=0

# Small sprites with ducks and bunnies
STILEFILE_quack = quack2x1.txt
STILEDEFS_quack = -DSTILE_W=2 -DSTILE_H=1 -DVISIBLE_EYES=0

# Sprite definitions for the selected size
STILEFILE = $(srcdir)/$(STILEFILE_$(SIZE))
STILEDEFS = -DSTILEFILE=\"$(call qcq,$(STILEFILE))\" $(STILEDEFS_$(SIZE))

## Mazes

# The default maze, inspired by Namco's Pac-Man
MAZEFILE_myman = maze.txt

# A maze inspired by Yahoo Software's 'Catchum'
MAZEFILE_catchum = catchum.txt
MAZEDEFS_catchum = \
         -DMAZE_W=25 -DMAZE_H=23 \
         -DDOTS=203 -DDOTS_FRUIT1=45 -DDOTS_FRUIT2=90 \
         -DMSGLEN=9 -DRMSG=13 -DCMSG=8 -DRMSG2=9 -DCMSG2=8\
         -DREADY='"  READY! "' -DGAMEOVER='"GAME OVER"' \
         -DPLAYER1='"PLAYER  1"' -DPLAYER2='"PLAYER  2"' \
         -DRHERO=17.5 -DRFRUIT=13.5 -DRGHOST=11.5 -DRTOP=10.5 -DROGHOST=2

# A maze inspired by 'Chomp'

# FIXME: ghost eyes get stuck inside the bullpen while trying to
# respawn

MAZEFILE_chomp = chomp.txt
MAZEDEFS_chomp = \
         -DMAZE_W=21 -DMAZE_H=16 -DMAZE_N=6 \
         -DDOTS=152 -DDOTS_FRUIT1=45 -DDOTS_FRUIT2=90 \
         -DMSGLEN=9 -DRMSG=5 -DCMSG=6 -DRMSG2=5 -DCMSG2=6 \
         -DREADY='"  READY! "' -DGAMEOVER='"GAME OVER"' \
         -DPLAYER1='"PLAYER  1"' -DPLAYER2='"PLAYER  2"' \
         -DRHERO=10.5 -DRFRUIT=RHERO -DRGHOST=8.5 -DRTOP=5.5

# A maze inspired by 'Gobble Man'

# FIXME: ghosts get stuck along the left-hand side

MAZEFILE_gobble = gobble.txt
MAZEDEFS_gobble = \
         -DMAZE_W=41 -DMAZE_H=24 \
         -DDOTS=241 -DDOTS_FRUIT1=70 -DDOTS_FRUIT2=141 \
         -DMSGLEN=11 -DRMSG=7 -DCMSG=15 -DRMSG2=7 -DCMSG2=15 \
         -DREADY='"   READY!  "' -DGAMEOVER='" GAME OVER "' \
         -DPLAYER1='"PLAYER  ONE"' -DPLAYER2='"PLAYER  TWO"' \
         -DRHERO=15.5 -DRFRUIT=10.5 -DRGHOST=RFRUIT -DRTOP=RGHOST \
        -DCFRUIT=15.5

# A maze inspired by 'Munchkin' (a text-mode game for HDOS)

# FIXME: ghosts get stuck near the upper-right corner

MAZEFILE_munchkin = munchkin.txt
MAZEDEFS_munchkin = \
         -DMAZE_W=27 -DMAZE_H=15 \
         -DDOTS=91 -DDOTS_FRUIT1=31 -DDOTS_FRUIT2=61 \
         -DMSGLEN=7 -DRMSG=5 -DCMSG=10 -DRMSG2=5 -DCMSG2=10 \
         -DREADY='" READY!"' -DGAMEOVER='" GONE  "' \
         -DPLAYER1='" P ONE "' -DPLAYER2='" P TWO "' \
         -DRHERO=9.5 -DRFRUIT=7.5 -DRGHOST=RFRUIT -DRTOP=5.5

# A maze inspired by 'Pac-Gal' (a text-mode game for DOS)

# FIXME: ghosts get stuck in the bottom corners

MAZEFILE_pacgal = pac-gal.txt
MAZEDEFS_pacgal = \
         -DMAZE_W=40 -DMAZE_H=24 \
         -DDOTS=468 -DDOTS_FRUIT1=100 -DDOTS_FRUIT2=200 \
         -DRMSG=16 -DCMSG=15 -DRMSG2=16 -DCMSG2=15 \
         -DRHERO=18.5 -DRFRUIT=16.5 -DRGHOST=13.5 -DRTOP=10.5

# A maze inspired by 'Pac-Manic' (a game for DOS)
MAZEFILE_pacmanic = pacmanic.txt
MAZEDEFS_pacmanic = \
         -DMAZE_W=29 -DMAZE_H=29 \
         -DDOTS=248 -DDOTS_FRUIT1=72 -DDOTS_FRUIT2=144 \
         -DMSGLEN=11 -DRMSG=15 -DCMSG=9 -DRMSG2=15 -DCMSG2=9 \
         -DRHERO=21.5 -DRFRUIT=15.5 -DRGHOST=13.5 -DRTOP=11.5 \
         -DROGHOST=1

# A maze inspired by 'Coleco Portable Pac-Man' (a portable game machine)
MAZEFILE_portable = portable.txt
MAZEDEFS_portable = \
         -DMAZE_W=17 -DMAZE_H=19 \
         -DDOTS=68 -DDOTS_FRUIT1=0 -DDOTS_FRUIT2=0 \
         -DMSGLEN=7 -DRMSG=11 -DCMSG=5 -DRMSG2=11 -DCMSG2=5 \
         -DREADY='" READY!"' -DGAMEOVER='" GONE  "' \
         -DPLAYER1='" P ONE "' -DPLAYER2='" P TWO "' \
         -DRHERO=13.5 -DRFRUIT=11.5 -DRGHOST=9.5 -DRTOP=7.5 \
         -DROGHOST=2 -DCOGHOST=1

# A maze inspired by 'Spear-man' (a text-mode game)
MAZEFILE_spear = spear.txt
MAZEDEFS_spear = \
         -DMAZE_W=21 -DMAZE_H=21 \
         -DDOTS=164 -DDOTS_FRUIT1=35 -DDOTS_FRUIT2=70 \
         -DMSGLEN=7 -DRMSG=11 -DCMSG=7 -DRMSG2=11 -DCMSG2=7 \
         -DREADY='" READY!"' -DGAMEOVER='" GONE  "' \
         -DPLAYER1='" P ONE "' -DPLAYER2='" P TWO "' \
         -DRHERO=15.5 -DRFRUIT=11.5 -DRGHOST=9.5 -DRTOP=7.5 \
         -DROGHOST=2 -DCOGHOST=1

# A maze inspired by 'PACMAN89' (a text-terminal game for CP/M)
MAZEFILE_pacman89 = pacman89.txt
MAZEDEFS_pacman89 = \
         -DMAZE_W=20 -DMAZE_H=23 \
         -DDOTS=190 -DDOTS_FRUIT1=40 -DDOTS_FRUIT2=80 \
         -DMSGLEN=8 -DRMSG=13 -DCMSG=6 -DRMSG2=13 -DCMSG2=6 \
         -DREADY='" READY! "' -DGAMEOVER='"ALL GONE"' \
         -DPLAYER1='"PLAYER 1"' -DPLAYER2='"PLAYER 2"' \
         -DRHERO=17.5 -DRFRUIT=13.5 -DRGHOST=10.5 -DRTOP=7.5

# A maze inspired by YumPop.com's 'Quackman'

# FIXME: This maze is supposed to have a secret tunnel connecting the
# two dead-ends inside the "M"; the tunnel is only supposed to be
# usable by Quackman. At the moment this tunnel is visible and the
# ghosts know how to use it. The alternate mazefile quackman.txt has
# no such tunnel.

MAZEFILE_quackman = quackmn2.txt
MAZEDEFS_quackman = \
         -DMAZE_W=30 -DMAZE_H=33 \
         -DDOTS=263 -DDOTS_FRUIT1=72 -DDOTS_FRUIT2=144 \
         -DMSGLEN=10 -DRMSG=18 -DCMSG=10 -DRMSG2=11 -DCMSG2=10 \
         -DREADY='"  READY!  "' -DGAMEOVER='"GAME  OVER"' \
         -DPLAYER1='"PLAYER ONE"' -DPLAYER2='"PLAYER TWO"' \
         -DRHERO=21.5 -DRFRUIT=18.5

# A tiny maze for tiny screens

MAZEFILE_tiny = tiny.txt
MAZEDEFS_tiny = \
         -DMAZE_W=20 -DMAZE_H=12 \
         -DDOTS=101 -DDOTS_FRUIT1=32 -DDOTS_FRUIT2=64 \
         -DMSGLEN=10 -DRMSG=10 -DCMSG=5 -DRMSG2=10 -DCMSG2=5 \
         -DREADY='"  READY!  "' -DGAMEOVER='"GAME  OVER"' \
         -DPLAYER1='"PLAYER ONE"' -DPLAYER2='"PLAYER TWO"' \
         -DRHERO=6 -DCHERO=9.5 -DRFRUIT=10.5 -DRGHOST=4 -DROGHOST=1

# Inspired by Alawar's Pacmania in Advanced mode, first level

MAZEFILE_pacmania = pacmania.txt
MAZEDEFS_pacmania = \
         -DMAZE_W=17 -DMAZE_H=17 \
         -DDOTS=138 -DDOTS_FRUIT1=32 -DDOTS_FRUIT2=64 \
         -DMSGLEN=7 -DRMSG=8 -DCMSG=5 -DRMSG2=8 -DCMSG2=5 \
         -DREADY='" READY!"' -DGAMEOVER='"GOODBYE"' \
         -DPLAYER1='" P ONE "' -DPLAYER2='" P TWO "' \
         -DRHERO=13.5 -DCHERO=3.5 -DRFRUIT=8.5 -DCFRUIT=8.5 -DRGHOST=8.5 -DROGHOST=1.5 -DCGHOST=8.5 -DCOGHOST=2.5

# Maze definitions for the selected variant
MAZEFILE = $(srcdir)/$(MAZEFILE_$(VARIANT))
MAZEDEFS = -DMAZEFILE=\"$(call qcq,$(MAZEFILE))\" $(MAZEDEFS_$(VARIANT))

# Definitions for compiling the selected variant at the selected size
GAMEDEFS = $(TILEDEFS) $(STILEDEFS) $(MAZEDEFS) $(EXTRAGAMEDEFS)

# C preprocessor options for wrapper executable
WRAPPERDEFS = \
         $(foreach dir_var,$(dir_vars),-D$(call ucase,$(dir_var))=\"$(call qcq,$($(dir_var)))\") \
         -DDIST=\"$(call qcq,$(DIST))\" \
         -DMYMAN=\"$(call qcq,$(MYMAN))\" \
         -DVERSION=\"$(call qcq,$(VERSION))\" \
         -DSIZES=\"$(call qcq,$(SIZES))\" \
         -DSIZE=\"$(call qcq,$(SIZE))\" \
         -DVARIANTS=\"$(call qcq,$(VARIANTS))\" \
         -DVARIANT=\"$(call qcq,$(VARIANT))\" \
         -DHTM_SUFFIX=\"$(call qcq,$(HTM_SUFFIX))\"

WRAPPERDEFS += $(EXTRAWRAPPERDEFS)

# C preprocessor options for built-in games
ifeq ($(enable_monolithic),yes)
BUILTINDEFS = \
         -DBUILTIN_GAMES=$(call q,$(foreach variant,$(VARIANTS),$(foreach size,$(SIZES),GAME(main_$(variant)_$(size)))))
else
BUILTINDEFS =
endif

BUILTINDEFS += $(EXTRABUILTINDEFS)

##
## Targets
##

# run other sanity checks except during script generation

ifeq (,$(script_target))

all install uninstall clean bindist:: gnumake.ok configure.ok

all install:: compiler.ok

all uninstall:: coreutils.ok

endif

# the coreutils sanity check should be included in the installation
# script, since the installation script uses $(INSTALL)

install:: coreutils.ok

all clean install uninstall:: $(call mw,$(MAKEFILE))
	@(for VARIANT in $(VARIANTS); do \
             $(MAKE) $(MAKELOOP) \
                 VARIANT="$$VARIANT" \
                 $@-variant-"$$VARIANT" \
                     || exit $$?; \
         done)
	@(for SIZE in $(SIZES); do \
             $(MAKE) $(MAKELOOP) \
                 SIZE="$$SIZE" \
                 $@-size-"$$SIZE" \
                     || exit $$?; \
         done)
	@(for VARIANT in $(VARIANTS); do \
             for SIZE in $(SIZES); do \
                 $(MAKE) $(MAKELOOP) \
                     SIZE="$$SIZE" \
                     VARIANT="$$VARIANT" \
                     $@-"$$VARIANT"-"$$SIZE" \
                         || exit $$?; \
             done; \
         done)

.SUFFIXES:

.PHONY: coreutils.ok

# this rule is depended upon by the principal targets to verify that
# $(INSTALL) and $(RMDIR) work; its value is not cached; the "exit 1"
# here is unparenthesized intentionally, so that it will terminate an
# installation script
coreutils.ok: $(call mw,$(MAKEFILE))
	$(POST_UNPACK)
	@export installflag rmdirflag bogusflag; \
        installflag='strip'; \
        rmdirflag='ignore-fail-on-non-empty'; \
        bogusflag='not-a-valid-option'; \
        if $(SHELL) -c $(call q,$(INSTALL))' --$$installflag --help $$bogusflag | fgrep -v $$bogusflag | fgrep $$installflag' </dev/null >/dev/null && \
           $(SHELL) -c $(call q,$(RMDIR))' --$$rmdirflag --help $$bogusflag | fgrep -v $$bogusflag | fgrep $$rmdirflag' </dev/null >/dev/null; \
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
	$(NORMAL_UNPACK)

.PHONY: gnumake.ok

# test 1: in some older GNU Make versions, ?= doesn't work correctly
gnumake_ok_test_1_indirect := failed
gnumake_ok_test_1 ?= $(gnumake_ok_test_1_indirect)
gnumake_ok_test_1_indirect := ok

# test 2: in some older GNU Make versions, $(call ...) is not implemented
gnumake_ok_test_2_function = $1
gnumake_ok_test_2 := $(call gnumake_ok_test_2_function,ok)

# test 3: in some older GNU Make versions, $(call q,...) does not work
gnumake_ok_test_3 := $(patsubst \$$test,ok,$(call q,$$test))

# this rule is depended upon by the principal targets to perform a
# sanity check of GNU Make; its value is not cached
gnumake.ok: $(call mw,$(MAKEFILE))
ifneq ($(gnumake_ok_test_1),ok)
	@echo 'test 1: recursive variable expansion with ?= failed -- please upgrade your Make to e.g. $(gnumake_ok_tested_version)' >&2; \
            echo '** if you want to build anyway, see simple.mk' >&2; \
            exit 1
endif
ifneq ($(gnumake_ok_test_2),ok)
	@echo 'test 2: $$(call ...) failed -- please upgrade your Make to e.g. $(gnumake_ok_tested_version)' >&2; \
            echo '** if you want to build anyway, see simple.mk' >&2; \
            exit 1
endif
ifneq ($(gnumake_ok_test_3),ok)
	@echo 'test 3: $$(call q,...) failed -- please upgrade your Make to e.g. $(gnumake_ok_tested_version)' >&2; \
            echo '** if you want to build anyway, see simple.mk' >&2; \
            exit 1
endif
	@:

.PHONY: configure.ok

# this rule is depended upon by the principal targets to perform a
# sanity check of the configure system; its value is not cached
configure.ok: $(call mw,$(MAKEFILE))
	@(if test :$(call qs,$(call xq,unknown),$(nil),$(date)) != :$(nil); \
        then \
            :; \
        else \
            echo $@: configure check failed >&2; \
            $(ECHOLINEX) '** ' $(call q,$(SHELL) $(srcdir)/configure --dump=date) failed, >&2; \
            echo '** ' your shell might not work\; see INSTALL for hints >&2; \
            exit 1; \
        fi)

# this rule is depended upon by the principal targets to perform a
# sanity check of the compiler and linker for the build machine
compiler.ok: $(call mw,$(MAKEFILE))
	@$(MAKE) $(MAKELOOP) \
            sanity_return=0 \
            _sanity0$x
	@ ./_sanity0
	@$(REMOVE) _sanity0.c _sanity0 _sanity0$o
	@$(MAKE) $(MAKELOOP) \
            sanity_return=1 \
            _sanity1$x
	@( ./_sanity1 && exit 1 || : )
	@$(REMOVE) _sanity1.c _sanity1 _sanity1$o
	@$(ECHOLINEX) your compiler appears to work, so creating $(call q,$@)
	@$(ECHOLINEX) You may safely remove this file with \`$(call q,$(REMOVE) $@)\'. > $(call q,$@)

ifneq (,$(sanity_return))

_sanity$(sanity_return).c: $(call mw,$(MAKEFILE))
	@($(ECHOLINEX) '/* '$(call q,$@)': compiler sanity check */'; \
        echo '/* you may safely delete this file */'; \
        echo 'int main(int argc, char **argv) { return '$(call q,$(sanity_return))'; }') > $(call q,$@)

_sanity$(sanity_return)$o: _sanity$(sanity_return).c
	@$(COMPILE)

_sanity$(sanity_return)$x: _sanity$(sanity_return).c
	@$(LINK) $(LIBS)

endif

.PHONY: all clean install uninstall
.PHONY: all-variant-$(VARIANT) clean-variant-$(VARIANT) install-variant-$(VARIANT) uninstall-variant-$(VARIANT)
.PHONY: all-size-$(SIZE) clean-size-$(SIZE) install-size-$(SIZE) uninstall-size-$(SIZE)
.PHONY: all-$(VARIANT)-$(SIZE) clean-$(VARIANT)-$(SIZE) install-$(VARIANT)-$(SIZE) uninstall-$(VARIANT)-$(SIZE)
.PHONY: installdirs uninstalldirs
.PHONY: install-files uninstall-files
.PHONY: install-strip

install-strip: $(call mw,$(MAKEFILE))
	$(MAKE) $(MAKELOOP) \
            STRIPINSTALLFLAGS=$(call qmq,$(STRIPINSTALLFLAGS) -s) \
            install

.PHONY: distclean mostlyclean maintainer-clean

distclean::
	-$(REMOVE) compiler.ok

distclean mostlyclean:: clean

maintainer-clean:: distclean
	@echo 'This command is intended for maintainers to use; it'
	@echo 'deletes files that may need special tools to rebuild.'

doc_files = \
ChangeLog \
INSTALL \
LICENSE \
README \
NEWS \
ONEWS \
VERSION

dist_program_files = \
configure \
xmyman \
xmyman2 \
xmyman3 \
xmyman4 \
xbigman \
xhugeman \
xquackman \
myman.ct

dist_data_files = \
$(doc_files) \
Makefile \
simple.mk \
catchum.asc \
catchum.txt \
chomp.asc \
chomp.txt \
gobble.asc \
gobble.txt \
maze.asc \
maze.txt \
munchkin.asc \
munchkin.txt \
myman.man \
myman.c \
tile1x1.asc \
tile1x1.txt \
tile2x1.asc \
tile2x1.txt \
pac-gal.asc \
pac-gal.txt \
pacman89.asc \
pacman89.txt \
quackman.asc \
quackman.txt \
quackmn2.asc \
quackmn2.txt \
tiny.asc \
tiny.txt \
pacmania.asc \
pacmania.txt \
pacmanic.asc \
pacmanic.txt \
portable.asc \
portable.txt \
spear.asc \
spear.txt \
stile1x1.txt \
stile2x1.txt \
stile7x3.txt \
stile10x6.txt \
quack1x1.txt \
quack2x1.txt \
tile5x2.txt \
tile5x3.txt \
my_getopt/ChangeLog \
my_getopt/getopt.3 \
my_getopt/getopt.h \
my_getopt/getopt.txt \
my_getopt/LICENSE \
my_getopt/main.c \
my_getopt/Makefile \
my_getopt/my_getopt.c \
my_getopt/my_getopt.h \
my_getopt/README \
main.c

dist_dirs = \
my_getopt

dist_files = \
$(dist_program_files) \
$(dist_data_files)

.PHONY: fill-dir-$(DIST)

fill-dir-$(DIST):: $(call mw,$(MAKEFILE)) $(addprefix $(call mw,$(srcdir))/,$(dist_files)) empty-dir-xq-$(call mwxq,$(DIST))
	@$(MAKE) $(MAKELOOP) \
            $(foreach dir,$(dist_dirs),install-dir-xq-$(call qxq,$(DIST)/$(dir)))
	@($(foreach file,$(dist_data_files), \
            $(MAKE) $(MAKELOOP) \
                install-data-xq-$(call qxq,$(DIST)/$(file)) \
                data_file=$(call qmq,$(srcdir)/$(file)) \
                || exit $$?; \
        ))
	@($(foreach file,$(dist_program_files), \
            $(MAKE) $(MAKELOOP) \
                install-program-xq-$(call qxq,$(DIST)/$(file)) \
                program_file=$(call qmq,$(srcdir)/$(file)) \
                || exit $$?; \
        ))

.PHONY: dist

dist:: $(call mw,$(MAKEFILE))
	@$(MAKE) $(MAKELOOP) \
            compressed-tarball-xq-$(call qxq,$(DIST))
	-$(REMOVE) $(DIST)$(tar)
	@$(MAKE) $(MAKELOOP) \
            wipe-dir-xq-$(call qxq,$(DIST))

ifneq ($script_target,)

ifneq ($script_stage,)

.PHONY: genscript-$(script_stage)_$(script_target)

genscript-$(script_stage)_$(script_target): $(call mw,$(srcdir))/configure $(call mw,$(MAKEFILE))
	@echo $(call q,$(POST_UNPACK))
	@echo "## $(script_stage)_$(script_target) start"
	@$(ECHOLINE) $(call q,$(firstword $(MAKE)))'() { : ; }'
	@sed -n -e '/START_ECHOLINE_LIBRARY/,/END_ECHOLINE_LIBRARY/ p' < \
            $(call q,$<)
	@echo $(call q,$(NORMAL_UNPACK))
	@$(MAKE) $(MAKELOOP) -n -s \
            $(script_target) \
            ECHOLINE=echoline \
            ECHOLINEX=echolinex \
            PRE_$(call ucase,$(script_target))=pre_$(script_target) \
            POST_$(call ucase,$(script_target))=post_$(script_target) \
            NORMAL_$(call ucase,$(script_target))=normal_$(script_target) \
            | \
                sed -n -e '1 {;x;s/^$$/normal_$(script_target)/;x;};/^\(normal\|pre\|post\)_$(script_target)$$/ {;h;b;};G;s/\(.*\)'$(call q,$(char_reverse_solidus)$(char_newline))'$(script_stage)_$(script_target)$$/\1/p'
	@echo $(call q,$(POST_UNPACK))
	@echo "## $(script_stage)_$(script_target) end"
	@echo $(call q,$(NORMAL_UNPACK))

endif

.PHONY: genscript-$(script_target)-header

genscript-$(script_target)-header: $(call mw,$(srcdir))/configure $(call mw,$(MAKEFILE))
	@$(ECHOLINE) '#'\!$(call q,$(SHELL))' --'
	@echo '# $(script_target).sh - binary-$(script_target) script for the MyMan video game'
	@echo
	@echo '# This file was generated automatically.'
	@echo '# Do not edit this file by hand.'
	@echo
	@echo '# See INSTALL for instructions.'
	@echo
	@echo '# make sure some standard directories are in our search path'
	@echo 'PATH="$${PATH:-}:/usr/bin:/bin"'
	@echo 'export PATH'
	@echo
	@sed -n -e '/START_ECHOLINE_LIBRARY/,/END_ECHOLINE_LIBRARY/ p' < \
            $(call q,$<)
	@echo
	@$(foreach var,$(filter-out this_file_undergoes_variable_substitution,$(substitute_vars)),$(ECHOLINE) $(var)=$(call q,$(call q,$($(var))));)
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
	@echo '    echoline "$(script_target).sh ($${MYMAN}) $${VERSION}"'
	@echo '    exit 0'
	@echo 'fi'
	@echo
	@echo 'if test :"$$1" = :"--help" -o :"$$1" = :"-help" -o :"$$1" = :"-h"'
	@echo 'then'
	@$(ECHOLINE) '    echolinex "Usage: $${0} [options]"'
	@$(ECHOLINE) '    echoline "-y or"'
	@$(ECHOLINE) '    echoline "--yes                   do binary-$(script_target) without prompting for confirmation"'
	@$(ECHOLINE) '    echoline "-n or"'
	@$(ECHOLINE) '    echoline "--no                    don'$(call q,$(char_apostrophe))'t do binary-$(script_target)"'
	@$(ECHOLINE) '    echoline "-h or"'
	@$(ECHOLINE) '    echoline "--help                  show this message and exit"'
	@$(ECHOLINE) '    echoline "-V or"'
	@$(ECHOLINE) '    echoline "--version               print $(script_target).sh version and exit"'
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
	@echo '    echolinex "$$0: unrecognized parameter "'$(call q,$(call q,$(char_grave_accent)))'"$$1"'$(call q,$(call q,$(char_apostrophe)))' >&2'
	@echo '    echolinex "usage: $$0 [--help] [--version] [options...]" >&2'
	@echo '    echo >&2'
	@echo '    echolinex "see INSTALL in the $${MYMAN} distribution directory for instructions" >&2'
	@echo '    echo >&2'
	@echo '    echo Aborting. >&2'
	@echo '    exit 2'
	@echo 'fi'
	@echo
	@echo echo parameters for binary-$(script_target):
	@echo echo
	@$(foreach var,$(filter-out this_file_undergoes_variable_substitution,$(substitute_vars)),echo 'echolinex "    $(var): $${$(var)}"';)
	@echo echo
	@echo '# If stdin is a tty, make sure this is what you meant to do (use -y to override).'
	@echo '# Non-interactive invocations skip this check.'
	@echo 'while test -t 0 -a :"$${override}" != :"yes" -a :"$${override}" != :"y"'
	@echo 'do'
	@echo '    echolinex "About to do a binary-$(script_target) of $${BINDIST}." >&2'
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
	@echo 'echolinex "doing binary-$(script_target) of $${BINDIST}"'
	@$(ECHOLINE) $(MYMAN)'_found='
	@echo '$(script_target)_dirname=`dirname "$$0" 2>/dev/null`'
	@echo '$(script_target)_dirname="$${$(script_target)_dirname:-$${0##*/}}"'
	@echo '$(script_target)_dirname="$${$(script_target)_dirname:-.}"'
	@echo 'for $(script_target)_dir in . "$${$(script_target)_dirname}"'
	@echo 'do'
	@echo '    if test -f "$${$(script_target)_dir}"/VERSION'
	@echo '    then'
	@echo '        $(script_target)_dir_version=`cat "$${$(script_target)_dir}"/VERSION`'
	@echo '        if test :"$${VERSION}" = :"$${$(script_target)_dir_version}"'
	@echo '        then'
	@$(ECHOLINE) '            '$(MYMAN)'_found=t'
	@echo '            break'
	@echo '        fi'
	@echo '    fi'
	@echo 'done'
	@echo
	@$(ECHOLINE) 'if test :$${'$(MYMAN)'_found} != :t'
	@echo 'then'
	@$(ECHOLINE) '    echolinex unable to find VERSION file from "$${BINDIST}" >&2'
	@echo '    echo >&2'
	@echo '    echo Aborting. >&2'
	@echo '    exit 2'
	@echo 'fi'
	@echo
	@echo 'cd "$${$(script_target)_dir}" || exit 1'

.PHONY: genscript-$(script_target)

genscript-$(script_target): $(call mw,$(MAKEFILE))
	@$(MAKE) $(MAKELOOP) \
            genscript-$(script_target)-header
ifneq (uninstall,$(script_target))
	@$(ECHOLINE) '(cd '$(call qq,$(distroot))') || exit 1'
endif
ifeq (clean,$(script_target))
	@$(ECHOLINE) 'exec rm -rf install.sh '$(call qq,$(distroot))' clean.sh'
else
ifeq (install,$(script_target))
	@$(ECHOLINE) 'bindist_install_program() { '$(call q,$(INSTALL_PROGRAM)) $(call qq,$(distroot))'"$$2" "$$2"; }'
	@$(ECHOLINE) 'bindist_install_data() { '$(call q,$(INSTALL_DATA)) $(call qq,$(distroot))'"$$2" "$$2"; }'
	@$(ECHOLINE) 'bindist_install_dir() { '$(call q,$(INSTALL_DIR))' "$$1"; }'
endif
	@$(MAKE) $(MAKELOOP) \
            genscript-pre_$(script_target) \
            script_stage=pre
ifeq (install,$(script_target))
	@$(MAKE) $(MAKELOOP) \
            genscript-normal_$(script_target) \
            script_stage=normal \
            INSTALL_PROGRAM=bindist_install_program \
            INSTALL_DATA=bindist_install_data \
            INSTALL_DIR=bindist_install_dir \
            INSTALL_MANLINK=bindist_install_data \
            PRE_UNPACK=pre_unpack \
            NORMAL_UNPACK=normal_unpack \
            POST_UNPACK=post_unpack \
            | \
                sed -n -e '1 {;x;s/^$$/normal_unpack/;x;};/^\(normal\|pre\|post\)_unpack$$/ {;h;b;};G;s/\(.*\)'$(call q,$(char_reverse_solidus)$(char_newline))'post_unpack$$/\1/p'
else
	@$(MAKE) $(MAKELOOP) \
            genscript-normal_$(script_target) \
            script_stage=normal
endif
	@$(MAKE) $(MAKELOOP) \
            genscript-post_$(script_target) \
            script_stage=post
endif

endif

.PHONY: fill-dir-$(BINDIST)

fill-dir-$(BINDIST):: $(call mw,$(MAKEFILE)) all $(addprefix $(call mw,$(srcdir))/,$(doc_files)) empty-dir-xq-$(call mwxq,$(BINDIST))
	@-$(MAKE) $(MAKELOOP) \
             install-data-xq-$(call qxq,$(BINDIST)/$(MYMAN).txt) \
             data_file=$(MYMAN).txt
	@-$(MAKE) $(MAKELOOP) \
             install-data-xq-$(call qxq,$(BINDIST)/$(MYMAN).ps) \
             data_file=$(MYMAN).ps
	@-$(MAKE) $(MAKELOOP) \
             install-data-xq-$(call qxq,$(BINDIST)/$(MYMAN).dvi) \
             data_file=$(MYMAN).dvi
	@-$(MAKE) $(MAKELOOP) \
             install-data-xq-$(call qxq,$(BINDIST)/$(MYMAN)$(htm)) \
             data_file=$(MYMAN)$(htm)
	@($(foreach file,$(doc_files), \
            $(MAKE) $(MAKELOOP) \
                install-data-xq-$(call qxq,$(BINDIST)/$(file)) \
                data_file=$(call qmq,$(srcdir)/$(file)) \
                || exit $$?; \
        ))
	$(MAKE) $(MAKELOOP) \
            genscript-clean \
            script_target=clean \
            > $(BINDIST)/clean.sh
	-chmod 755 $(BINDIST)/clean.sh
	$(MAKE) $(MAKELOOP) \
            genscript-uninstall \
            script_target=uninstall \
            > $(BINDIST)/uninstall.sh
	-chmod 755 $(BINDIST)/uninstall.sh
	$(MAKE) $(MAKELOOP) \
            genscript-normal_install \
            script_stage=normal \
            script_target=install \
            DESTDIR=./$(BINDIST)/$(distroot) \
            | $(SHELL)
	$(MAKE) $(MAKELOOP) \
            genscript-install \
            script_target=install \
            > $(BINDIST)/install.sh
	-chmod 755 $(BINDIST)/install.sh

.PHONY: bindist

bindist:: $(call mw,$(MAKEFILE))
	@$(MAKE) $(MAKELOOP) \
            compressed-tarball-xq-$(call qxq,$(BINDIST))
	-$(REMOVE) $(BINDIST)$(tar)
	@$(MAKE) $(MAKELOOP) \
            wipe-dir-xq-$(call qxq,$(BINDIST))

.PHONY: info check installcheck

info check installcheck:
	@$(ECHOLINEX) $(call q,$(MAKEFILE)): warning: target $@ not implemented
	@-(exit 1)

TAGS:: $(addprefix $(call mw,$(srcdir))/,$(dist_files))
	etags -o $@ $(addprefix $(call q,$(srcdir))/,$(dist_files))

all:: $(call mw,$(MAKEFILE)) $(MYMAN)$x docs

.PHONY: docs

docs: $(MYMAN)$(man6ext) $(MYMAN).txt $(MYMAN).ps dvi $(MYMAN)$(htm) $(addprefix $(call mw,$(srcdir))/,$(doc_files))

.PHONY: dvi

dvi: $(MYMAN).dvi

clean:: wipe-dir-xq-$(call mwxq,$(DIST)) wipe-dir-xq-$(call mwxq,$(BINDIST))
	-$(REMOVE) $(DIST)$(tgz) $(DIST)$(tar)
	-$(REMOVE) $(BINDIST)$(tgz) $(BINDIST)$(tar)
	-$(REMOVE) *$(bak) *$o *$a $(MYMAN)$x TAGS
	-$(REMOVE) _sanity?$x _sanity?.c
	-$(REMOVE) $(MYMAN)$(man6ext) $(MYMAN).txt $(MYMAN).ps $(MYMAN).dvi $(MYMAN)$(htm)

install:: install-files

uninstall:: $(call mw,$(MAKEFILE))
	@$(MAKE) $(MAKELOOP) \
            uninstalldirs

install-files: $(call mw,$(MAKEFILE)) installdirs $(MYMAN)$x $(call mw,$(srcdir))/xmyman $(call mw,$(srcdir))/xmyman2 $(call mw,$(srcdir))/xmyman3 $(call mw,$(srcdir))/xmyman4 $(call mw,$(srcdir))/xbigman $(call mw,$(srcdir))/xhugeman $(call mw,$(srcdir))/xquackman $(call mw,$(srcdir))/myman.ct docs
	@$(MAKE) $(MAKELOOP) \
             install-program-xq-$(call qxq,$(DESTDIR)$(bindir)/$(MYMAN)$x) \
             program_file=$(MYMAN)$x \
             INSTALLFLAGS=$(call qmq,$(INSTALLFLAGS) $(STRIPINSTALLFLAGS))
	@$(MAKE) $(MAKELOOP) \
             install-program-xq-$(call qxq,$(DESTDIR)$(bindir)/$(DIST)$x) \
             program_file=$(MYMAN)$x \
             INSTALLFLAGS=$(call qmq,$(INSTALLFLAGS) $(STRIPINSTALLFLAGS))
	@$(MAKE) $(MAKELOOP) \
             install-program-xq-$(call qxq,$(DESTDIR)$(bindir)/xmyman) \
             program_file=$(call qmq,$(srcdir))/xmyman
	@$(MAKE) $(MAKELOOP) \
             install-program-xq-$(call qxq,$(DESTDIR)$(bindir)/xmyman2) \
             program_file=$(call qmq,$(srcdir))/xmyman2
	@$(MAKE) $(MAKELOOP) \
             install-program-xq-$(call qxq,$(DESTDIR)$(bindir)/xmyman3) \
             program_file=$(call qmq,$(srcdir))/xmyman3
	@$(MAKE) $(MAKELOOP) \
             install-program-xq-$(call qxq,$(DESTDIR)$(bindir)/xmyman4) \
             program_file=$(call qmq,$(srcdir))/xmyman4
	@$(MAKE) $(MAKELOOP) \
             install-program-xq-$(call qxq,$(DESTDIR)$(bindir)/xbigman) \
             program_file=$(call qmq,$(srcdir))/xbigman
	@$(MAKE) $(MAKELOOP) \
             install-program-xq-$(call qxq,$(DESTDIR)$(bindir)/xhugeman) \
             program_file=$(call qmq,$(srcdir))/xhugeman
	@$(MAKE) $(MAKELOOP) \
             install-program-xq-$(call qxq,$(DESTDIR)$(bindir)/xquackman) \
             program_file=$(call qmq,$(srcdir))/xquackman
	@$(MAKE) $(MAKELOOP) \
             install-program-xq-$(call qxq,$(DESTDIR)$(bindir)/myman.ct) \
             program_file=$(call qmq,$(srcdir))/myman.ct
	@($(foreach file,$(doc_files), \
            $(MAKE) $(MAKELOOP) \
                install-data-xq-$(call qxq,$(DESTDIR)$(privatedocdir)/$(file)) \
                data_file=$(call qmq,$(srcdir)/$(file)) \
                || exit $$?; \
        ))
	@-$(MAKE) $(MAKELOOP) \
             install-data-xq-$(call qxq,$(DESTDIR)$(privatedocdir)/$(MYMAN).txt) \
             data_file=$(MYMAN).txt
	@-$(MAKE) $(MAKELOOP) \
             install-data-xq-$(call qxq,$(DESTDIR)$(privatedocdir)/$(MYMAN).ps) \
             data_file=$(MYMAN).ps
	@-$(MAKE) $(MAKELOOP) \
             install-data-xq-$(call qxq,$(DESTDIR)$(privatedocdir)/$(MYMAN).dvi) \
             data_file=$(MYMAN).dvi
	@-$(MAKE) $(MAKELOOP) \
             install-data-xq-$(call qxq,$(DESTDIR)$(privatedocdir)/$(MYMAN)$(htm)) \
             data_file=$(MYMAN)$(htm)
	@-$(MAKE) $(MAKELOOP) \
             install-data-xq-$(call qxq,$(DESTDIR)$(man6dir)/$(DIST)$(man6ext)) \
             data_file=$(MYMAN)$(man6ext)
	@-$(MAKE) $(MAKELOOP) \
             install-manlink-xq-$(call qxq,$(DESTDIR)$(man6dir)/$(MYMAN)$(man6ext)) \
             manlink_file=$(call qmqs,$(call xq,$(mandir)/)%,%,$(man6dir)/$(DIST)$(man6ext))
	@-$(MAKE) $(MAKELOOP) \
             install-manlink-xq-$(call qxq,$(DESTDIR)$(man6dir)/xmyman$(man6ext)) \
             manlink_file=$(call qmqs,$(call xq,$(mandir)/)%,%,$(man6dir)/$(DIST)$(man6ext))
	@-$(MAKE) $(MAKELOOP) \
             install-manlink-xq-$(call qxq,$(DESTDIR)$(man6dir)/xmyman2$(man6ext)) \
             manlink_file=$(call qmqs,$(call xq,$(mandir)/)%,%,$(man6dir)/$(DIST)$(man6ext))
	@-$(MAKE) $(MAKELOOP) \
             install-manlink-xq-$(call qxq,$(DESTDIR)$(man6dir)/xmyman3$(man6ext)) \
             manlink_file=$(call qmqs,$(call xq,$(mandir)/)%,%,$(man6dir)/$(DIST)$(man6ext))
	@-$(MAKE) $(MAKELOOP) \
             install-manlink-xq-$(call qxq,$(DESTDIR)$(man6dir)/xmyman4$(man6ext)) \
             manlink_file=$(call qmqs,$(call xq,$(mandir)/)%,%,$(man6dir)/$(DIST)$(man6ext))
	@-$(MAKE) $(MAKELOOP) \
             install-manlink-xq-$(call qxq,$(DESTDIR)$(man6dir)/xbigman$(man6ext)) \
             manlink_file=$(call qmqs,$(call xq,$(mandir)/)%,%,$(man6dir)/$(DIST)$(man6ext))
	@-$(MAKE) $(MAKELOOP) \
             install-manlink-xq-$(call qxq,$(DESTDIR)$(man6dir)/xhugeman$(man6ext)) \
             manlink_file=$(call qmqs,$(call xq,$(mandir)/)%,%,$(man6dir)/$(DIST)$(man6ext))
	@-$(MAKE) $(MAKELOOP) \
             install-manlink-xq-$(call qxq,$(DESTDIR)$(man6dir)/xquackman$(man6ext)) \
             manlink_file=$(call qmqs,$(call xq,$(mandir)/)%,%,$(man6dir)/$(DIST)$(man6ext))
	@-$(MAKE) $(MAKELOOP) \
             install-manlink-xq-$(call qxq,$(DESTDIR)$(man6dir)/myman.ct$(man6ext)) \
             manlink_file=$(call qmqs,$(call xq,$(mandir)/)%,%,$(man6dir)/$(DIST)$(man6ext))
	$(POST_INSTALL)
	@$(MAKE) $(MAKELOOP) \
            update-whatis
	$(NORMAL_INSTALL)

.PHONY: update-whatis

update-whatis:
	@-(dir=$(call q,$(DESTDIR)$(mandir)); \
        export dir; \
        if $(SHELL) -c $(call q,$(MANDB) $(MANDBFLAGS) $(EXTRAMANDBFLAGS) "$${dir}") </dev/null >/dev/null 2>&1 || \
            $(SHELL) -c $(call q,$(MAKEWHATIS) $(MAKEWHATISFLAGS) $(EXTRAMAKEWHATISFLAGS) "$${dir}") </dev/null; \
        then \
            $(ECHOLINEX) updated whatis database for "$${dir}"; \
        else \
            echo $@: failed to update whatis database >&2; \
            $(ECHOLINEX) '** ' you should update the whatis database for "$${dir}" >&2; \
            echo '** ' if you would like the \`apropos\' and \`whatis\' programs to >&2; \
            $(ECHOLINEX) '** ' find $(MYMAN)\; if you have the \`mandb\' and/or \`makewhatis\' >&2; \
            echo '** ' programs, add their directories to your PATH and re-run this command >&2; \
            exit 1; \
        fi)

uninstall-files: $(call mw,$(MAKEFILE))
	@(for file in \
            $(call qxq,$(DESTDIR)$(bindir)/$(MYMAN)$x) \
            $(call qxq,$(DESTDIR)$(bindir)/$(DIST)$x) \
            $(call qxq,$(DESTDIR)$(bindir)/xmyman) \
            $(call qxq,$(DESTDIR)$(bindir)/xmyman2) \
            $(call qxq,$(DESTDIR)$(bindir)/xmyman3) \
            $(call qxq,$(DESTDIR)$(bindir)/xmyman4) \
            $(call qxq,$(DESTDIR)$(bindir)/xbigman) \
            $(call qxq,$(DESTDIR)$(bindir)/xhugeman) \
            $(call qxq,$(DESTDIR)$(bindir)/xquackman) \
            $(call qxq,$(DESTDIR)$(bindir)/myman.ct) \
            $(call qxq,$(DESTDIR)$(privatedocdir)/ChangeLog) \
            $(call qxq,$(DESTDIR)$(privatedocdir)/INSTALL) \
            $(call qxq,$(DESTDIR)$(privatedocdir)/LICENSE) \
            $(call qxq,$(DESTDIR)$(privatedocdir)/README) \
            $(call qxq,$(DESTDIR)$(privatedocdir)/NEWS) \
            $(call qxq,$(DESTDIR)$(privatedocdir)/ONEWS) \
            $(call qxq,$(DESTDIR)$(privatedocdir)/VERSION) \
            $(call qxq,$(DESTDIR)$(privatedocdir)/$(MYMAN).txt) \
            $(call qxq,$(DESTDIR)$(privatedocdir)/$(MYMAN).ps) \
            $(call qxq,$(DESTDIR)$(privatedocdir)/$(MYMAN).dvi) \
            $(call qxq,$(DESTDIR)$(privatedocdir)/$(MYMAN)$(htm)) \
            $(call qxq,$(DESTDIR)$(man6dir)/$(MYMAN)$(man6ext)) \
            $(call qxq,$(DESTDIR)$(man6dir)/myman.ct$(man6ext)) \
            $(call qxq,$(DESTDIR)$(man6dir)/xquackman$(man6ext)) \
            $(call qxq,$(DESTDIR)$(man6dir)/xhugeman$(man6ext)) \
            $(call qxq,$(DESTDIR)$(man6dir)/xbigman$(man6ext)) \
            $(call qxq,$(DESTDIR)$(man6dir)/xmyman4$(man6ext)) \
            $(call qxq,$(DESTDIR)$(man6dir)/xmyman3$(man6ext)) \
            $(call qxq,$(DESTDIR)$(man6dir)/xmyman2$(man6ext)) \
            $(call qxq,$(DESTDIR)$(man6dir)/xmyman$(man6ext)) \
            $(call qxq,$(DESTDIR)$(man6dir)/$(DIST)$(man6ext)) \
            ; \
        do \
            $(MAKE) $(MAKELOOP) \
                uninstall-file-xq-"$$file" \
                    || exit $$?; \
        done)
	$(POST_UNINSTALL)
	@$(MAKE) $(MAKELOOP) \
            update-whatis
	$(NORMAL_UNINSTALL)

uninstalldirs: $(call mw,$(MAKEFILE)) uninstall-files
	@(for dir in $(foreach dir_var,$($(DIST)_dir_vars),$(call qxq,$($(dir_var)))); \
        do \
            $(MAKE) $(MAKELOOP) \
                uninstall-dir-xq-$(call qxq,$(DESTDIR))"$$dir" \
                    || exit $$?; \
        done)
	@(for dir in $(foreach dir_var,$(public_dir_vars),$(call qxq,$($(dir_var)))); \
        do \
            $(MAKE) $(MAKELOOP) \
                uninstall-optional-dir-xq-$(call qxq,$(DESTDIR))"$$dir" \
                    || exit $$?; \
        done)

installdirs: $(call mw,$(MAKEFILE)) $(foreach dir_var,$(public_dir_vars) $($(DIST)_dir_vars),install-dir-xq-$(call mwxq,$(DESTDIR)$($(dir_var))))

ifneq ($(manlink_file),)

install-manlink-xq-%:
	$(POST_UNPACK)
	@install_manlink() { $(ECHOLINE) .so "$$1" > "$$2" || ( $(REMOVE) "$$2"; exit 1 ); return $?; }; \
            $(ECHOLINEX) installing link to manpage $(call q,$(manlink_file)) as $(call qsxu,install-manlink-xq-%,%,$@); \
                $(INSTALL_MANLINK) $(call q,$(manlink_file)) $(call qsxu,install-manlink-xq-%,%,$@)
	$(NORMAL_UNPACK)

endif

ifneq ($(program_file),)

install-program-xq-%:
	$(POST_UNPACK)
	@$(ECHOLINEX) installing program file $(call qsxu,install-program-xq-%,%,$@); \
            $(INSTALL_PROGRAM) $(call q,$(program_file)) $(call qsxu,install-program-xq-%,%,$@)
	$(NORMAL_UNPACK)

endif

ifneq ($(data_file),)

install-data-xq-%:
	$(POST_UNPACK)
	@$(ECHOLINEX) installing data file $(call qsxu,install-data-xq-%,%,$@); \
            $(INSTALL_DATA) $(call q,$(data_file)) $(call qsxu,install-data-xq-%,%,$@)
	$(NORMAL_UNPACK)

endif

all-size-$(SIZE) all-variant-$(VARIANT) clean-size-$(SIZE) clean-variant-$(VARIANT):
	@:

ifeq ($(enable_monolithic),yes)
all-$(VARIANT)-$(SIZE):
	@:
else
all-$(VARIANT)-$(SIZE): $(hostprefix)$(GAME)$x
endif

clean-$(VARIANT)-$(SIZE):
	-$(REMOVE) $(hostprefix)$(GAME)$x $(BOOTSTRAP)$x $(DATA).c

install-variant-$(VARIANT):: installdirs $(call mw,$(MAKEFILE)) all-variant-$(VARIANT) $(call mw,$(MAZEFILE))
	@($(foreach filevar,MAZEFILE, \
            $(MAKE) $(MAKELOOP) \
                install-data-xq-$(call qxq,$(DESTDIR)$(privatedatadir)/$(call s,$(call xq,$(srcdir)/)%,%,$($(filevar)))) \
                data_file=$(call qmq,$($(filevar))) \
                || exit $$?; \
        ))

install-size-$(SIZE): $(call mw,$(MAKEFILE)) all-size-$(SIZE) $(call mw,$(TILEFILE)) $(call mw,$(STILEFILE))
	@($(foreach filevar,TILEFILE STILEFILE, \
            $(MAKE) $(MAKELOOP) \
                install-data-xq-$(call qxq,$(DESTDIR)$(privatedatadir)/$(call s,$(call xq,$(srcdir)/)%,%,$($(filevar)))) \
                data_file=$(call qmq,$($(filevar))) \
                || exit $$?; \
        ))

install-$(VARIANT)-$(SIZE): $(call mw,$(MAKEFILE)) all-$(VARIANT)-$(SIZE)
ifeq ($(enable_monolithic),yes)
	@:
else
	@$(MAKE) $(MAKELOOP) \
            install-program-xq-$(call qxq,$(DESTDIR)$(privatelibexecdir)/$(VARIANT)-$(SIZE)$x) \
            program_file=$(hostprefix)$(GAME)$x \
            INSTALLFLAGS=$(call qmq,$(INSTALLFLAGS) $(STRIPINSTALLFLAGS))
endif

uninstall-size-$(SIZE): $(call mw,$(MAKEFILE))
	@(for datafile in $(call qxqs,$(call xq,$(srcdir)/)%,%,$(TILEFILE)) $(call qxqs,$(call xq,$(srcdir)/)%,%,$(STILEFILE)); \
        do \
            $(MAKE) $(MAKELOOP) \
                uninstall-file-xq-$(call qxq,$(DESTDIR)$(privatedatadir)/)"$$datafile" \
                    || exit $$?; \
        done)

uninstall-variant-$(VARIANT): $(call mw,$(MAKEFILE))
	@(for datafile in $(call qxqs,$(call xq,$(srcdir)/)%,%,$(MAZEFILE)); \
        do \
            $(MAKE) $(MAKELOOP) \
                uninstall-file-xq-$(call qxq,$(DESTDIR)$(privatedatadir)/)"$$datafile" \
                    || exit $$?; \
        done)

uninstall-$(VARIANT)-$(SIZE): $(call mw,$(MAKEFILE))
ifeq ($(enable_monolithic),yes)
	@:
else
	@$(MAKE) $(MAKELOOP) \
            uninstall-file-xq-$(call qxq,$(DESTDIR)$(privatelibexecdir)/$(VARIANT)-$(SIZE)$x)
endif

my_getopt$o: $(call mw,$(srcdir))/$(MY_GETOPTDIR)/my_getopt.c $(call mw,$(srcdir))/$(MY_GETOPTDIR)/my_getopt.h
	$(COMPILE)

$(host)-my_getopt$o: $(call mw,$(srcdir))/$(MY_GETOPTDIR)/my_getopt.c $(call mw,$(srcdir))/$(MY_GETOPTDIR)/my_getopt.h
	$(HOSTCOMPILE)

$(MYMAN)$o: $(call mw,$(srcdir))/main.c $(call mw,$(srcdir))/$(MY_GETOPTDIR)/getopt.h $(call mw,$(srcdir))/$(MY_GETOPTDIR)/my_getopt.h $(call mw,$(srcdir))/VERSION
	$(HOSTCOMPILE) $(WRAPPERDEFS) $(BUILTINDEFS)

ifeq ($(enable_monolithic),yes)

$(MYMAN)$x: $(foreach variant,$(VARIANTS),$(call mw,$(srcdir)/$(MAZEFILE_$(variant)))) $(foreach size,$(SIZES),$(call mw,$(srcdir)/$(TILEFILE_$(size))) $(call mw,$(srcdir)/$(STILEFILE_$(size)))) $(call mw,$(srcdir))/myman.c $(call mw,$(srcdir))/VERSION

endif

$(MYMAN)$x: $(MYMAN)$o $(hostprefix)my_getopt$o
ifeq ($(enable_monolithic),yes)
	@(i=0; \
	for VARIANT in $(VARIANTS); do \
	     i=`expr $$i + 1`; \
	     j=0; \
             for SIZE in $(SIZES); do \
	         j=`expr $$j + 1`; \
                 $(MAKE) $(MAKELOOP) \
                     SIZE="$$SIZE" \
                     VARIANT="$$VARIANT" \
                     s2v"$$i"s"$$j""$$VARIANT"-"$$SIZE"$o \
                     $(hostprefix)v"$$i"s"$$j""$$VARIANT"-"$$SIZE"$o \
                         || exit $$?; \
             done; \
         done)
	$(HOSTLINK) $(foreach variant,$(VARIANTS),$(foreach size,$(SIZES),$(call q,$(hostprefix)v$(call idx,$(variant),$(VARIANTS))s$(call idx,$(size),$(SIZES))$(variant)-$(size)$o) $(call q,s2v$(call idx,$(variant),$(VARIANTS))s$(call idx,$(size),$(SIZES))$(variant)-$(size)$o))) $(CURSESLIBS) $(hostprefix)my_getopt$o $(LIBS)
else
	$(HOSTLINK) $(hostprefix)my_getopt$o $(LIBS)
endif

$(MYMAN)$(man6ext): $(call mw,$(srcdir))/myman.man $(call mw,$(MAKEFILE)) $(call mw,$(srcdir))/VERSION
	@$(TROFF_SUBSTITUTE)

$(MYMAN).txt: $(MYMAN)$(man6ext)
	-($(SHELL) -c 'groff -man -Tascii | col -xb' < $(call q,$<) > $(call q,$@) || \
            ($(REMOVE) $(call q,$@); exit 1))

$(MYMAN).ps: $(MYMAN)$(man6ext)
	-($(SHELL) -c 'groff -man -Tps' < $(call q,$<) > $(call q,$@) || \
            ($(REMOVE) $(call q,$@); exit 1))

$(MYMAN).dvi: $(MYMAN)$(man6ext)
	-($(SHELL) -c 'groff -man -Tdvi' < $(call q,$<) > $(call q,$@) || \
            ($(REMOVE) $(call q,$@); exit 1))

$(MYMAN)$(htm): $(MYMAN)$(man6ext)
	-($(SHELL) -c 'groff -man -Thtml' < $(call q,$<) > $(call q,$@)$(bak) && \
            sed -e $(call q, \
                s/&minus;/\&$(char_number_sign)45;/g; \
                s/&lsquo;/\&$(char_number_sign)39;/g; \
                s/&rsquo;/\&$(char_number_sign)39;/g; \
                s/&ldquo;/\&$(char_number_sign)34;/g; \
                s/&rdquo;/\&$(char_number_sign)34;/g; \
            ) < $(call q,$@)$(bak) > $(call q,$@) || \
                ($(REMOVE) $(call q,$@); exit 1))
	-$(REMOVE) $(call q,$@)$(bak)

DATAOBJS = $(DATA)$o

.PHONY: deps-$(BOOTSTRAP)

deps-$(BOOTSTRAP): $(call mw,$(srcdir))/myman.c

$(host)-$(GAME)$o $(GAME)$o deps-$(BOOTSTRAP): $(call mw,$(srcdir))/$(MY_GETOPTDIR)/getopt.h $(call mw,$(srcdir))/$(MY_GETOPTDIR)/my_getopt.h

$(GAME)$o: $(call mw,$(srcdir))/myman.c $(call mw,$(srcdir))/VERSION
	$(COMPILE) $(CURSOPTS) $(EXTRACURSOPTS) $(CURSESINCLUDE) $(GAMEDEFS) $(WRAPPERDEFS) $(DATADEFS)

$(host)-$(GAME)$o: $(call mw,$(srcdir))/myman.c $(call mw,$(srcdir))/VERSION
	$(HOSTCOMPILE) $(CURSOPTS) $(EXTRACURSOPTS) $(CURSESINCLUDE) $(GAMEDEFS) $(WRAPPERDEFS) $(DATADEFS)

$(GAME)$x deps-$(BOOTSTRAP): my_getopt$o

$(host)-$(GAME)$x deps-$(BOOTSTRAP): $(hostprefix)my_getopt$o

$(GAME)$x: $(GAME)$o $(DATAOBJS)
	$(LINK) $(CURSESLIBS) my_getopt$o $(LIBS) $(DATAOBJS)

$(host)-$(GAME)$x: $(hostprefix)$(GAME)$o $(DATAOBJS)
	$(HOSTLINK) $(CURSESLIBS) $(hostprefix)my_getopt$o $(LIBS) $(DATAOBJS)

ifneq ($(GAME),$(BOOTSTRAP))

$(DATA)$o: $(DATA).c
	$(HOSTCOMPILE) $(DATADEFS)

$(BOOTSTRAP)$x: $(call mw,$(MAKEFILE)) deps-$(BOOTSTRAP)
	@$(MAKE) $(MAKELOOP) \
            $(BOOTSTRAP)$x \
            DATAOBJS= \
            DATADEFS= \
            GAME=$(BOOTSTRAP)

$(DATA).c: $(BOOTSTRAP)$x $(call mw,$(TILEFILE)) $(call mw,$(STILEFILE)) $(call mw,$(MAZEFILE))
	./$(BOOTSTRAP) -STMF $(call q,$@)

endif

$(call mw,$(MAKEFILE)):: $(call mw,$(srcdir))/VERSION
	@touch $(call q,$@)

# use these to help diagnose mis-set makefile variables; replace '%'
# with a Make variable name
dumpx-%:
	@$(ECHOLINEX) $(call q,$(value $(call s,dumpx-%,%,$@)))
dump-%:
	@$(ECHOLINE) $(call q,$(value $(call s,dump-%,%,$@)))

# these all use 'xq'-quoting for the '%' part in order to allow
# solidus (/) characters and other strange stuff to appear in the
# argument

install-dir-xq-%:
	$(POST_UNPACK)
	@test -d $(call qsxu,install-dir-xq-%,%,$@) || \
            ($(ECHOLINEX) creating directory $(call qsxu,install-dir-xq-%,%,$@) && \
                $(INSTALL_DIR) $(call qsxu,install-dir-xq-%,%,$@))
	$(NORMAL_UNPACK)

uninstall-optional-dir-xq-%:
	@-test ! -d $(call qsxu,uninstall-optional-dir-xq-%,%,$@) || \
            ($(REMOVE_DIR) $(call qsxu,uninstall-optional-dir-xq-%,%,$@) && \
                (test -d $(call qsxu,uninstall-optional-dir-xq-%,%,$@) || \
                    $(ECHOLINEX) removed directory $(call qsxu,uninstall-optional-dir-xq-%,%,$@)))

uninstall-dir-xq-%:
	@test ! -d $(call qsxu,uninstall-dir-xq-%,%,$@) || \
            ($(ECHOLINEX) removing directory $(call qsxu,uninstall-dir-xq-%,%,$@) && \
                $(REMOVE_DIR) $(call qsxu,uninstall-dir-xq-%,%,$@))

uninstall-file-xq-%:
	@test ! -f $(call qsxu,uninstall-file-xq-%,%,$@) -o -L $(call qsxu,uninstall-file-xq-%,%,$@) || \
            ($(ECHOLINEX) removing file $(call qsxu,uninstall-file-xq-%,%,$@) && \
                 $(REMOVE) $(call qsxu,uninstall-file-xq-%,%,$@))

wipe-dir-xq-%:
	test ! -d $(call qsxu,wipe-dir-xq-%,%,$@) || \
            rm -rf $(call qsxu,wipe-dir-xq-%,%,$@)

empty-dir-xq-%: wipe-dir-xq-%
	@$(MAKE) $(MAKELOOP) \
            $(call qs,empty-dir-xq-%,install-dir-xq-%,$@)

tarball-xq-%:
	@$(MAKE) $(MAKELOOP) \
            $(call qsxu,tarball-xq-%,fill-dir-%,$@)
	test ! -f $(call qsxu,tarball-xq-%,%$(call xq,$(tar)),$@) || \
            $(REMOVE) $(call qsxu,tarball-xq-%,%$(call xq,$(tar)),$@)
	tar -cf $(call qsxu,tarball-xq-%,%$(call xq,$(tar)),$@) $(call qsxu,tarball-xq-%,%,$@)

compressed-tarball-xq-%: tarball-xq-%
	test ! -f $(call qsxu,compressed-tarball-xq-%,%$(call xq,$(tgz)),$@) || \
            $(REMOVE) $(call qsxu,compressed-tarball-xq-%,%$(call xq,$(tgz)),$@)
	gzip -c -9 < $(call qsxu,tarball-xq-%,%$(call xq,$(tar)),$<) > $(call qsxu,compressed-tarball-xq-%,%$(call xq,$(tgz)),$@) || \
        ( \
            $(REMOVE) $(call qsxu,compressed-tarball-xq-%,%$(call xq,$(tgz)),$@); \
            exit 1 \
        )

# LocalWords:  myman
