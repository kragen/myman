README - basic documentation for the MyMan video game
Copyright 1997-2009, Benjamin C. Wiley Sittler <bsittler@gmail.com>

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.

Website: http://myman.sourceforge.net/


        NOTE: !!! LICENSE CHANGE !!!

        MyMan is no longer in the public domain. It is now distributed
        under a BSD-style license (see LICENSE for details.)


Legal Notice
------------

MyMan is an unofficial and unlicensed clone of the original Pac-Man
and Puckman games and of their sequels and imitators, and is not
endorsed by Namco Bandai Games Inc., owners of the Pac-Man and Puckman
registered trademarks and copyrights. Any trademarks used herein are
the property of their respective owners. No mention of a trademark,
trademark owner, or other party shall be construed as an endorsement
of MyMan or any other product by any party. If you believe that by
using or posessing MyMan the rights of others would be infringed, you
are strongly encouraged to cease using MyMan and its derivatives and
delete all copies of the MyMan software and its derivatives, and to
inform the author or authors of MyMan and the distributor or
distributors from whom you obtained MyMan of the reasons for such
belief so that the infringing part may be removed or replaced. Any
imitation by MyMan of other software is intended to be purely for
purposes of humor and amusement.

Please see the file LICENSE for author contact information.


History
-------

MyMan was written on a dare on Sunday, 1998-06-28. The original
version had no color or attribute support and only supported large
tiles (equivalent to -z big.) It was 742 lines of ANSI C and used
three data files: a maze file (equivalent to -v pac), a tile file and
a sprite file. It worked with ncurses on Linux and cygwin. Several
variations using other maze layouts were created over the following
hours. Color came in December of that year, as did support for small
tiles (equivalent to -z small.) The mygetopt argument parser is a bit
older, having been started in 1997 as part of the SMOKE-16 project.


Acknowledgements
----------------

Toru Iwatani had the original idea for Namco's 'Pac-Man'/'Puck-Man',
which in turn served as inspiration for this game.
Innumerable others have written their own variants of the original
game which in turn inspired variants for this game, and I have tried
to credit them where authorship is known. Credits for each variant are
listed in the in-game help screen reachable by pressing ? or Ctrl-H.


Description
-----------

Basic premise of the MyMan video game:

  "Those scary ghosts are back, and this time they've spotted
   you! What's worse is that they've mistaken you for their old
   rival Pac, and they're out for (yellow) blood!"

MyMan displays (A) on a text terminal or terminal emulator (using
ncurses, slang, PDCurses, SysV curses, BSD curses, old VMS curses, or
the raw stdio terminal driver), (B) on an X Window System display
(using PDCurses for X, a.k.a. XCurses), (C) on any terminal or display
supported by PDCurses for SDL, libcaca, LibGGI/LibGII, Allegro, TWin,
aalib, Carbon/Toolbox, newt, GTK+, FLTK, <disp.h>, Borland-style
CONIO, or the Extensible Firmware Interface (EFI) or (D) in a Win32
command prompt window (using the raw Win32 terminal driver).  Since
MyMan is fairly fast-moving, you'll need a reasonably fast computer
and display.  It once ran acceptably fast on a 486-66 under Linux, and
may still.

Installation
------------

Installation instructions are in the file INSTALL.


License and Version
-------------------

The MyMan version number is recorded in the file VERSION inside the
source package. To determine the version number of an installed MyMan
executable, run e.g. 'myman -V'.

This version of MyMan has a BSD-like license; see the file LICENSE for
details. Earlier versions were placed in the public domain.

MyMan includes a bundled copy of the 'mygetopt' argument parser,
which is licensed under the same terms as MyMan; see the README file
in the 'mygetopt' subdirectory of the MyMan source distribution
for details.

Note that the contents of the MyMan datafiles (tiles, sprites and
mazes) are in the public domain.

The file 'start.xm' is by Raina / Titan and was originally created in
2007 for an AmigaOS 4.0 port of MyMan 0.6. See:

http://os4depot.net/index.php?function=showfile&file=game/action/pfp-myma.lha


To Do
-----

See TODO for notes on desired features and bugfixes.


A Note on Text Encoding and the B.O.M.
--------------------------------------

Note that many files in the MyMan distribution are distributed as
Unicode/UCS text in the UTF-8 encoding with an explicit "byte-order
mark" (B.O.M.) at the beginning. This three-byte sequence (0xEF, 0xBB,
0xBF in hexadecimal notation) allows many web browsers and text
editors on Windows, Mac OS X, and other operating systems to
automatically select the correct character encoding when displaying or
editing the files. Please try to preserve this sequence when further
redistributing the files or sending in patches so that others will be
able to read the files painlessly.


Environment Variables
---------------------

Some configurations of MyMan are influenced by the values of
environment variables, in addition to those behaviors documented for
the underlying display libraries:

 Display Library        Variable            Effect
-----------------      ----------          --------

 AA-Lib                 AACURSES_BITMAP     Nonempty value causes
                                            pseudo-bitmap rendering
                        AAOPTS              Presence of variable
                                            inhibits auto-sizing
                        DISPLAY             Absence or empty value
                                            inhibits auto-sizing

 libcaca 0.x
                        CACA_DRIVER         Absence or value other
                                            than "x11" (case-insensitive)
                                            inhibits auto-sizing to
                                            large geometries
                        CACA_FONT           Absence or value other
                                            than "nil2" (case-insensitive)
                                            inhibits auto-sizing to
                                            large geometries
                        CACA_GEOMETRY       Nonempty value inhibits
                                            auto-sizing
                        CACACURSES_ACS      Nonzero value forces use
                                            of font positions 0-31 for
                                            VT100 alternate character
                                            set (XTerm-style)
                        OPTCURSES_DIRECT    Nonzero value disables redraw
                                            optimization layer
                        OPTCURSES_INTERLACE Nonzero value renders by
                                            interleaving half-screens
                        OPTCURSES_BITMAP    Nonzero value causes use of
                                            pseudo-bitmap characters
                        OPTCURSES_BITMAP_FORCE
                                            Nonzero value allows use of
                                            inverse video when using
                                            ASCII pseudo-bitmap characters
                        OPTCURSES_BITMAP_BRAILLE
                                            Nonzero value forces use of
                                            Braille pseudo-bitmap characters
                                            on a Unicode display

 libcaca 1.x            CACACURSES_BITMAP   Nonempty value causes
                                            pseudo-bitmap rendering
                        CACACURSES_FIXEDPAL Nonempty value disables
                                            custom color palette
                        CACA_DRIVER         Absence or value other
                                            than "x11" (case-insensitive)
                                            inhibits auto-sizing to
                                            large geometries
                        CACA_FONT           Absence or value other
                                            than "nil2" (case-insensitive)
                                            inhibits auto-sizing to
                                            large geometries
                        CACACURSES_BITMAP_BRIGHTNESS
                                            Floating-point brightness for
                                            pseudo-bitmap rendering
                        CACACURSES_BITMAP_GAMMA
                                            Floating-point gamma value for
                                            pseudo-bitmap rendering
                        CACACURSES_BITMAP_CONTRAST
                                            Floating-point contrast for
                                            pseudo-bitmap rendering
                        CACACURSES_BITMAP_ANTIALIAS
                                            Antialiasing mode for
                                            pseudo-bitmap rendering;
                                            one of "none", "prefilter"
                                            or "default"
                        CACACURSES_BITMAP_COLOR
                                            Color mode for
                                            pseudo-bitmap rendering;
                                            one of "mono", "gray", "8", "16",
                                            "fullgray", "full8", "full16",
                                            or "default"
                        CACACURSES_BITMAP_CHARSET
                                            Character set for
                                            pseudo-bitmap rendering;
                                            one of "ascii", "shades",
                                            "blocks", or "default"
                        CACACURSES_BITMAP_ALGORITHM
                                            Error diffusion mode for
                                            pseudo-bitmap rendering;
                                            one of "none", "ordered2",
                                            "ordered4", "ordered8",
                                            "random", or "fstein"
                        CACACURSES_BITMAP_DOUBLE
                                            Nonempty value doubles
                                            bitmap width and height
                        CACACURSES_BITMAP_DOUBLEHEIGHT
                                            Nonempty value doubles
                                            bitmap height
                        OPTCURSES_DIRECT    Nonzero value disables redraw
                                            optimization layer
                        OPTCURSES_INTERLACE Nonzero value renders by
                                            interleaving half-screens
                        OPTCURSES_BITMAP    Nonzero value causes use of
                                            pseudo-bitmap characters
                        OPTCURSES_BITMAP_FORCE
                                            Nonzero value allows use of
                                            inverse video when using
                                            ASCII pseudo-bitmap characters
                        OPTCURSES_BITMAP_BRAILLE
                                            Nonzero value forces use of
                                            Braille pseudo-bitmap characters
                                            on a Unicode display

 LibGGI/LibGII          GGICURSES_BITMAP    Nonempty value causes bitmap
                                            rendering
                        GGICURSES_SWAPRB    Nonempty value interchanges
                                            red and blue color channels
                        GGICURSES_ASCII     Nonempty value forces ASCII-only
                                            rendering
                        GGICURSES_FLUSHALL  Nonempty value disables selective
                                            rectangular region updates in favor
                                            of complete refreshes
                        GGICURSES_FIXEDPAL  Nonempty value disables custom
                                            color palette
                        GGICURSES_DIRECT    Nonempty value enables use of
                                            directbuffer rendering
                        GGICURSES_BITMAP_DOUBLE
                                            Nonempty value doubles
                                            bitmap width and height
                                            by scanline simulation
                        GGICURSES_BITMAP_DOUBLEHEIGHT
                                            Nonempty value doubles
                                            bitmap height
                        OPTCURSES_DIRECT    Nonzero value disables redraw
                                            optimization layer
                        OPTCURSES_INTERLACE Nonzero value renders by
                                            interleaving half-screens
                        OPTCURSES_BITMAP    Nonzero value causes use of
                                            pseudo-bitmap characters
                        OPTCURSES_BITMAP_FORCE
                                            Nonzero value allows use of
                                            inverse video when using
                                            ASCII pseudo-bitmap characters
                        OPTCURSES_BITMAP_BRAILLE
                                            Nonzero value forces use of
                                            Braille pseudo-bitmap characters
                                            on a Unicode display

 Allegro                ALLEGROCURSES_BITMAP
                                            Nonempty value causes bitmap
                                            rendering
                        ALLEGROCURSES_BITMAP_DOUBLE
                                            Nonempty value doubles
                                            bitmap width and height
                                            by scanline simulation
                        ALLEGROCURSES_BITMAP_DOUBLEHEIGHT
                                            Nonempty value doubles
                                            bitmap height
                        ALLEGROCURSES_SWAPRB
                                            Nonempty value interchanges
                                            red and blue color channels
                        ALLEGROCURSES_FIXEDPAL
                                            Nonempty value disables custom
                                            color palette
                        ALLEGROCURSES_FONT
                                            Font for non-bitmap rendering;
                                            must be a format supported by
                                            load_font(3); chr8.fnt in
                                            the MyMan distribution works
                        ALLEGROCURSES_TXT_FONT
                                            Font for non-bitmap rendering;
                                            must be a format supported by
                                            load_txt_font(3); chr8.afn
                                            in the MyMan distribution works
                        OPTCURSES_DIRECT    Nonzero value disables redraw
                                            optimization layer
                        OPTCURSES_INTERLACE Nonzero value renders by
                                            interleaving half-screens
                        OPTCURSES_BITMAP    Nonzero value causes use of
                                            pseudo-bitmap characters
                        OPTCURSES_BITMAP_FORCE
                                            Nonzero value allows use of
                                            inverse video when using
                                            ASCII pseudo-bitmap characters
                        OPTCURSES_BITMAP_BRAILLE
                                            Nonzero value forces use of
                                            Braille pseudo-bitmap characters
                                            on a Unicode display

 Win32 Console/CONIO/TOS/AROS/VMS/*nix TTY
                        RAWCURSES_RAW       Nonzero value causes raw UTF-8
                                            character output (Unicode) or
                                            CP-437 byte output (CP-437)
                        RAWCURSES_WCWIDTH   Nonzero value causes builtin
                                            character width data to be used
                                            as a fallback for characters
                                            not handled by locale data;
                                            a positive integer value causes
                                            all non-ASCII characters to
                                            assume that width (in cells)
                        RAWCURSES_STDIO     Nonzero value forces use of stdio
                                            (Win32/CONIO)
                        RAWCURSES_CONIO     Nonzero value forces use of Borland-
                                            style CONIO
                        RAWCURSES_CONIO_INPUT
                                            Nonzero value forces use
                                            of CONIO input
                        RAWCURSES_UTF8      Nonzero value forces UTF-8 output
                                            when using stdio
                        RAWCURSES_DEBUG_UTF8
                                            Nonzero value forces UTF-8 debugging
                                            output when using stdio
                        RAWCURSES_ACS       Nonzero value forces use of the
                                            VT100 alternate character set when
                                            using stdio
                        RAWCURSES_CP437     Nonzero value forces use of CP-437
                                            alternate character set when using
                                            stdio (ESC[11m)
                        RAWCURSES_ISO2022   Zero value disables use of ISO 2022
                                            character set selection sequences
                        RAWCURSES_WIDE      Nonzero value forces use of the
                                            VT100 double-width characters when
                                            using stdio
                        RAWCURSES_H19       Nonzero value forces use of the
                                            H-19 alternate character set when
                                            using stdio
                        RAWCURSES_VT52      Nonzero value forces use of the
                                            VT52 escape sequences when
                                            using stdio
                        RAWCURSES_ST52      Nonzero value forces use of the
                                            ST52 escape sequences when
                                            using stdio
                        RAWCURSES_VWMTERM   Nonzero value forces use of the
                                            vwmterm escape sequences when
                                            using stdio
                        RAWCURSES_AMIGA     Nonzero value forces use of the
                                            Amiga console escape sequences when
                                            using stdio
                        RAWCURSES_DECTERM   Nonzero value forces use of the
                                            DECterm escape sequences when
                                            using stdio
                        RAWCURSES_ADM3A     Nonzero value forces use of the
                                            ADM3A escape sequences when
                                            using stdio
                        RAWCURSES_COLOR     Nonzero value enables color
                        RAWCURSES_16COLOR   Nonzero value enables use of the
                                            16 colors rather than 8
                        RAWCURSES_BLINK     Nonzero value forces use of the
                                            blink attribute for bright background
                                            colors when using stdio
                        RAWCURSES_88COLOR   Nonzero value enables use of the
                                            XTerm-style 88-color static
                                            palette (an XTerm compile-time
                                            option) when using stdio
                        RAWCURSES_256COLOR  Nonzero value enables use of the
                                            XTerm-style 256-color static
                                            palette (an XTerm compile-time
                                            option) when using stdio
                        RAWCURSES_CCC       Nonzero value forces use of the
                                            XTerm dynamic color palette when using
                                            stdio
                        RAWCURSES_LINUX     Nonzero value forces use of the
                                            Linux console dynamic color palette
                                            when using stdio
                        RAWCURSES_FIXEDPAL  Nonzero value disables custom color
                                            palette changes and use of more than 16
                                            colors
                        RAWCURSES_NOBULLET  Nonzero value disables use of the
                                            alternate character set bullet
                                            when using stdio
                        RAWCURSES_NOCOLORBOLD
                                            Nonzero value disables use of the
                                            bold attribute in conjunction with
                                            color when using stdio
                        RAWCURSES_CIVIS     Zero value disables the use of
                                            cursor-hiding escape sequences
                                            when using stdio
                        RAWCURSES_RELCUP    Nonzero value forces use of
                                            relative cursor addressing when
                                            using stdio
                        RAWCURSES_TRANSPARENT
                                            Zero value forces use of
                                            color for all attributes
                        RAWCURSES_LINES     Overrides LINES
                        RAWCURSES_COLUMNS   Overrides COLUMNS
                        OPTCURSES_DIRECT    Nonzero value disables redraw
                                            optimization layer
                        OPTCURSES_INTERLACE Nonzero value renders by
                                            interleaving half-screens
                        OPTCURSES_BITMAP    Nonzero value causes use of
                                            pseudo-bitmap characters
                        OPTCURSES_BITMAP_FORCE
                                            Nonzero value allows use of
                                            inverse video when using
                                            ASCII pseudo-bitmap characters
                        OPTCURSES_BITMAP_BRAILLE
                                            Nonzero value forces use of
                                            Braille pseudo-bitmap characters
                                            on a Unicode display
 Carbon/Toolbox         MACCURSES_FONT      Font name
                        MACCURSES_FONTSIZE  Initial font size in pixels
                        MACCURSES_FONTENCODING
                                            Font encoding name, possibly one of:
                                                CP437
                                                MacRoman
                                                MacJapanese
                                                MacVT100
                                                US-ASCII
                        OPTCURSES_DIRECT    Nonzero value disables redraw
                                            optimization layer
                        OPTCURSES_INTERLACE Nonzero value renders by
                                            interleaving half-screens
                        OPTCURSES_BITMAP    Nonzero value causes use of
                                            pseudo-bitmap characters
                        OPTCURSES_BITMAP_FORCE
                                            Nonzero value allows use of
                                            inverse video when using
                                            ASCII pseudo-bitmap characters
                        OPTCURSES_BITMAP_BRAILLE
                                            Nonzero value forces use of
                                            Braille pseudo-bitmap characters
                                            on a Unicode display
 GTK+                   GTKCURSES_FONT      Font name
                        GTKCURSES_ICON      Icon graphic
                        GTKCURSES_BITMAP    Nonzero value causes
                                            psuedo-bitmap rendering
                        OPTCURSES_DIRECT    Nonzero value disables redraw
                                            optimization layer
                        OPTCURSES_INTERLACE Nonzero value renders by
                                            interleaving half-screens
                        OPTCURSES_BITMAP    Nonzero value causes use of
                                            pseudo-bitmap characters
                        OPTCURSES_BITMAP_FORCE
                                            Nonzero value allows use of
                                            inverse video when using
                                            ASCII pseudo-bitmap characters
                        OPTCURSES_BITMAP_BRAILLE
                                            Nonzero value forces use of
                                            Braille pseudo-bitmap characters
                                            on a Unicode display
 FLTK                   FLTKCURSES_FONT     Font face
                        FLTKCURSES_FONTSIZE
                                            Font size
                        OPTCURSES_DIRECT    Nonzero value disables redraw
                                            optimization layer
                        OPTCURSES_INTERLACE Nonzero value renders by
                                            interleaving half-screens
                        OPTCURSES_BITMAP    Nonzero value causes use of
                                            pseudo-bitmap characters
                        OPTCURSES_BITMAP_FORCE
                                            Nonzero value allows use of
                                            inverse video when using
                                            ASCII pseudo-bitmap characters
                        OPTCURSES_BITMAP_BRAILLE
                                            Nonzero value forces use of
                                            Braille pseudo-bitmap characters
                                            on a Unicode display
 Open Watcom-style graph.h
                        GRAPHCURSES_BITMAP  Nonzero value causes bitmap
                                            rendering
                        GRAPHCURSES_MODE    Display mode number:
                                             -3 graphics, highest res
                                             -2 graphics, most colors
                                             -1 original mode
                                            or any MDA/CGA/HGC/EGA/VGA/MCGA/VESA
                                            mode number
                        GRAPHCURSES_ROWS    Display height in text rows or:
                                             -1 maximum height
                                              0 default height for mode
                        GRAPHCURSES_COLOR   Zero disables color
                        GRAPHCURSES_CCC     Zero disables EGA/VGA/MCGA/SVGA/VESA
                                            palette remapping
                        GRAPHCURSES_PALETTE Select palette for CGA mode 4/5:
                                             -1 current palette
                                              0 dark green/red/brown
                                              1 dark cyan/magenta/white
                                              2 light green/red/yellow
                                              3 light cyan/magenta/white
                        GRAPHCURSES_BRIGHTBG
                                            Zero disables use of bright background
                                            on CGA/EGA/VGA/MCGA/SVGA under MS-DOS
                        OPTCURSES_DIRECT    Nonzero value disables redraw
                                            optimization layer
                        OPTCURSES_INTERLACE Nonzero value renders by
                                            interleaving half-screens
                        OPTCURSES_BITMAP    Nonzero value causes use of
                                            pseudo-bitmap characters
                        OPTCURSES_BITMAP_FORCE
                                            Nonzero value allows use of
                                            inverse video when using
                                            ASCII pseudo-bitmap characters
                        OPTCURSES_BITMAP_BRAILLE
                                            Nonzero value forces use of
                                            Braille pseudo-bitmap characters
                                            on a Unicode display
 <disp.h>               DISPCURSES_USEBIOS  Nonzero value forces use of
                                            BIOS routines for display I/O
                                            (DOS)
                        OPTCURSES_DIRECT    Nonzero value disables redraw
                                            optimization layer
                        OPTCURSES_INTERLACE Nonzero value renders by
                                            interleaving half-screens
                        OPTCURSES_BITMAP    Nonzero value causes use of
                                            pseudo-bitmap characters
                        OPTCURSES_BITMAP_FORCE
                                            Nonzero value allows use of
                                            inverse video when using
                                            ASCII pseudo-bitmap characters
                        OPTCURSES_BITMAP_BRAILLE
                                            Nonzero value forces use of
                                            Braille pseudo-bitmap characters
                                            on a Unicode display
 newt
   or
 Borland-style CONIO    
   or
 Extensible Firmware Interface (EFI)
                        OPTCURSES_DIRECT    Nonzero value disables redraw
                                            optimization layer
                        OPTCURSES_INTERLACE Nonzero value renders by
                                            interleaving half-screens
                        OPTCURSES_BITMAP    Nonzero value causes use of
                                            pseudo-bitmap characters
                        OPTCURSES_BITMAP_FORCE
                                            Nonzero value allows use of
                                            inverse video when using
                                            ASCII pseudo-bitmap characters
                        OPTCURSES_BITMAP_BRAILLE
                                            Nonzero value forces use of
                                            Braille pseudo-bitmap characters
                                            on a Unicode display


Detailed Notes
--------------

For basic usage information, refer to the text version of the MyMan
manual page, myman.txt (generated from myman.man by 'make docs' using
groff.)


A. When Power Pellets Look Strange:

The compile-time option -DSWAPDOTS=1 or the equivalent command-line
option '-o' or in-game command 'O' or '0' is often necessary for a
decent display. This depends greatly on what font you use.


B. More Authentic Colors:

If you have a color X server and the XFree XTerm terminal emulator,
you can try the included xmyman script which should start MyMan in a
window of its own, with proper colors. If you have the rxvt terminal
emulator, you can try the xmyman2 script for a similar effect. And
finally, if you have XFree86 XTerm and a font for IBM codepage 437,
you can try the included xmyman3 script for more different types of
pseudo-graphic characters (single and double lines, dots, and power
pellets.)

Alternatively, you can get the "ctheme" package from

   http://sourceforge.net/project/?group_id=2640

and then run the included 'myman.ct' color theme script in your XTerm
or on the Linux console.

Changing your terminal's palette can greatly improve the experience;
recommended RGB values for each palette entry are listed at the end of
this file.

If you're using the Mac OS X Terminal application, you may install
TerminalColors to get customizable colors:

   http://www.culater.net/software/TerminalColors/TerminalColors.php

The myman.term file in the MyMan distribution takes advantage of
TerminalColors (if installed) to set the MyMan color palette.

If you're using the Darwin/Mac OS X console (this is a "text mode"
with no GUI or windowing system and is very rarely used,) you may
install utl/mach_mypal to switch your console to an appropriate
palette and utl/mach_acs to enable alternate character set (line
drawing) support. These have only been tested in Mac OS X 10.4 "Tiger"
so far.


C. Window Size:

A terminal height of 34 or more lines works quite nicely, but 24 or 25
(or even less) will work. Similarly, a width of 58 or more characters
is nice, but not required. If you're a real pro (or lacking screen
real-estate) you can play MyMan in a terminal window with 7 lines of
12 characters each... it's actually playable!


D. Keyboard Map:

       The following case-insensitive keystroke commands are recognized during
       the game:

       Q or Ctrl-C:
              quit the game

       P or ESC:
              pause the game

       Ctrl-Q:
              re-enable output (after Ctrl-S)

       R, Ctrl-L or Ctrl-R:
              refresh (redraw) the screen

       @:     reset the display subsystem and refresh (redraw) the screen

       Ctrl-S:
              inhibit output until Ctrl-Q is typed

       S:     toggle sound on/off

       W:     warp to the next level (after consuming the next dot)

       C:     toggle color on/off (if enabled at compile-time)

       B:     toggle use of dim and bold attributes for missing colors

       U:     toggle underlining of walls on/off (if enabled at compile-time)

       D:     toggle maze debugging on/off

       T:     save an HTML screenshot to the file snap####.html, where #### is
              a four-digit sequence number, and flash the screen briefly; a
              plain text version is saved as snap####.txt using backspacing
              to represent underlined and bold text

       O or 0:
              toggle appearance of power pellets and dots

       A:     toggle between ASCII altcharset translations and your terminal's
              altcharset translations

       I:     toggle between using your terminal's vertical scrolling capabil-
              ities and redrawing the screen

       X:     toggle between raw tile characters (CP437 or UCS/Unicode charac-
              ter graphics) and altcharset translations (VT100-style graphics)

       E:     toggle between UCS/Unicode and CP437 for internal representation
              of raw tile characters

       H, 4, Ctrl-B or LEFT:
              move left

       J, 2, Ctrl-N or DOWN:
              move down

       K, 8, Ctrl-P or UP:
              move up

       L, 6, Ctrl-F or RIGHT:
              move right

       , (comma) or <:
              cancel pending vertical move

       . (full stop) or >:
              cancel pending horizontal move

       / or \:
              toggle diagonal maze reflection

       !:     display information about the maze, tileset and
              spriteset

       ? or Ctrl-H:
              display help screen

       The pager recognizes the following special commands:

       SPACE:
              display next page or finish

       ESC:   finish immediately

       Move left then right with no intervening keystrokes:
              equivalent to ESC

E. Configuration at Start-Up:

Several of the game's settings can be changed at startup using
command-line parameters; use the '-h' parameter for more information.


F. Configuration at Compile-Time:

There are several properties of the game which may be altered using
compile-time options:

Compile-time option           Default value         Description
MYMANDELAY                    25000                 Delay is between updates.
                                                    (in microseconds)
SPEEDUP                       1000                  Amount delay decreases
                                                    after each level.

Values other than the default and 0 don't work particularly well. This
uses the usleep library function.

Boolean Compile-time option   Default value         Description
USE_ATTR                      (automatic)           Try to use attributes.
USE_COLOR                     (automatic)           Enable color support.
USE_PALETTE                   1 (except under       Enable support for
                                 slang)             color palette manipulation.
USE_KEYPAD                    (automatic)           Try to use the keypad.
USE_ACS                       1                     Use your terminal's
                                                    altcharset translations by
                                                    default.
USE_RAW                       0                     Output untranslated CP437
                                                    character graphics by
                                                    default.

The preceding options affect which curses features MyMan uses. MyMan
was developed using ncurses, primarily under GNU/Linux. It should be
fairly portable to other operating systems and other SysV-like curses
implementations. A partial port to old BSD curses has been completed,
and uses the FIONREAD ioctl to simulate nodelay(). A port to old VMS
curses has also been completed bypassing getch() and using raw QIO.

Options with automatic default values will be enabled if your curses
implementation seems to support them. Support is verified by checking
for relevant #defines; if your curses implementation doesn't provide
the relevant #defines, the default value will likely be wrong. This
guessing process is pessimistic, meaning that features are more likely
to be erroneously disabled than erroneously enabled.

Boolean Compile-time option   Default               Description
SWAPDOTS                      0                     Use 'o' for power pellets
                                                    and ACS_BULLET for dots.
                                                    (useful for vt100/latin-1
                                                     terminals and emulations)

This compile-time option can be modified on the command-line or at run-time.

Boolean Compile-time option   Default               Description
COLORIZE                      1                     Try to use color.
                                                    (somewhat slower than
                                                     monochrome)

This compile-time option can be modified on the command-line, and
toggled at run-time using 'c', provided USE_COLOR is non-zero.

Compile-time option           Default value       Description
TILEFILE                      "chr/chr5x2.txt"    File containing maze tiles.

A tile file contains a dimension specifier followed by some tiles. The
dimension specifier is of the form WxH, where W and H are decimal
integers corresponding to the width and height of the tiles. This may
optionally be followed by ~F, where F is a flag bitmap written as a
decimal integer. The flag bits defined so far are "2", which indicates
invertable bitmap tiles, and "4", which indicates that all
non-inverted wall parts should have their background color set. The
line may optionally end with tile file arguments ' arg1=value1
arg2=value2 ...'.

Tile Argument           Description
ABOUT                   Textual description of this tile file
NOTE                    Additional notes for this tile file
FIXME                   Known problems with this tile file

Each maze tile is described as follows:
XX~Y <- two-digit hex code; corresponds roughly to CP437 character code.
   ^___ optional: '~' followed by 1-digit hex code; corresponds to color.
:cccc...cc <- ':' followed by W characters; further characters ignored.
(repeated H times)
Blank lines are ignored.

Hex codes with special meaning:

20(' '),FF: walkable tile
2E('.'),F9: edible dot
6F('o'),FE: power pellet
3D('='),3A(':'),F0,FF: door (to ghost house)
7E('~'): secret door in horizontal wall
6C('l'): secret door in vertical wall
3C('<'),AE: zap left to matching right zap
3E('>'),AF: zap right to matching left zap
76('v'): zap down to matching up zap
5E('^'): zap up to matching down zap
10,11,18-1B,1E,1F,2B('+'),2D('-'),7C('|'),B3-DA,F0: walls

Color codes:

0: white (def.) 8: dim white
1: blue         9: bold blue
2: green        A: bold green
3: cyan         B: bold cyan
4: red          C: bold red
5: magenta      D: bold magenta
6: yellow       E: bold yellow
7: white        F: bold white

Compile-time option       Default value          Description
SPRITEFILE                "spr/spr7x3.txt"       File containing sprites.

A sprite file contains a dimension specifier followed by some
frames. The dimension specifier is of the form WxH, where W and H are
decimal integers corresponding to the width and height of sprite
frames.  This may optionally be followed by ~F, where F is a flag
bitmap written as a decimal integer. The only flag bit currently
defined is "1", which indicates that ghost eyes are invisible. The
line may optionally end with sprite file arguments ' arg1=value1
arg2=value2 ...'.

Note that ghost eyes are always visible while a ghost is returning
home (after being eaten.) Setting the "1" flag bit is a good idea if
your ghost eye sprite totally obsures your "mean" ghost sprite.

Sprite Argument         Description
ABOUT                   Textual description of this sprite file
NOTE                    Additional notes for this sprite file
FIXME                   Known problems with this sprite file

The sprite frame format is similar to the maze tile format, with the
following differences: a literal NUL (ASCII 0) in the description is
treated as a transparent character, and the hex codes are as follows:

00-07: level indicators (fruit, keys, etc.)
08-09: "mean" ghost body animation
0a: ghost eyes looking up
0b: ghost eyes looking left
0c: ghost eyes looking down
0d: ghost eyes looking right
0e-0f: "scared" ghost animation
10-13: MyMan walking up animation
14-17: MyMan walking left animation
18-1b: MyMan dying animation
1c-1f: MyMan walking right animation
20-23: MyMan walking down animation
24-28: (unused)
29: 100 points
2A: 300 points
2B: 500 points
2C: 700 points
2D: 1000 points
2E: 2000 points
2F: 3000 points
30: 5000 points
31: 200 points
32: 400 points
33: 800 points
34: 1600 points
35-36: "white/scared" ghost animation
37: life counter
38-3B: large MyMan walking right animation (upper-left quadrant)
3C-3F: large MyMan walking right animation (upper-right quadrant)
40-43: large MyMan walking right animation (lower-left quadrant)
44-47: large MyMan walking right animation (lower-right quadrant)
48-FF: (unused)

Compile-time option           Default value         Description
MAZEFILE                      "lvl/maze.txt"        Maze description file.

The maze description file starts with a line containing 'N WxH' or 'N
WxH~F' and optional maze arguments ' arg1=value1 arg2=value2 ...', and
the remaining lines are an ASCII picture of the maze. If present, F is
a flag bitmap written as a decimal integer. The flag bits defined so
far are "1", which indicates that maze changes only occur at game
intermissions, "2", which indicates whether to repeat the FLIP_TO maze
after showing the last maze, and "4", which indicates that wall color
choice should be based on wall thickness, rather than whether a wall
edge or a filled wall section is being drawn.

NOTE: The Python program scripts/asc2txt.py can be used to
      automatically convert plain-ASCII ".asc" mazes to UTF-8 ".txt"
      mazes. The output of scripts/asc2txt.py often needs some manual
      cleanup.

Maze argument                 Default value         Description
ABOUT                         (none)                Textual description of this maze file
NOTE                          (none)                Additional notes for this maze file
FIXME                         (none)                Known problems with this maze file
CHERO                         (W * 0.5)             MyMan's start column.
RHERO                         23.5                  MyMan's start row.
CFRUIT                        CHERO                 Prize column.
RFRUIT                        17.5                  Prize row.
RGHOST                        (RFRUIT - 3.0)        Ghost start row.
ROGHOST                       3                     Ghost offset rows.
COGHOST                       2                     Ghost offset columns.
RTOP                          (RFRUIT - 5.0)        Ghost door row.
CMSG                          9                     Message column.
RMSG                          17                    Message row.
CMSG2                         9                     Player Message column.
RMSG2                         11                    Player Message row.
PLAYER1                       "PLAYER ONE"
PLAYER2                       "PLAYER TWO"
READY                         "  READY!  "          Ready message.
GAMEOVER                      "GAME  OVER"          Game over message.
PAUSE                         " - PAUSED - "        Pause string.
GHOSTS                        4                     Number of ghosts.

Set this palette if you can (Red/Grn/Blu values are on a scale of
0-255; colors listed in VGA order):

Red Grn Blu   Hex#    R%  G%  B%  MyMan color   VGA color         ANSI color
===-===-===--======--===-===-===--============-================--===============
  0   0   0  000000    0   0   0  Black         0 Black           0 Black
  0   0 221  0000DD    0   0  86  Blue          1 Blue            4 Blue
  0 183   0  00B700    0  71   0  Green         2 Green           2 Green
  0 221 221  00DDDD    0  86  86  Cyan          3 Cyan            6 Cyan
221 150  70  DD9646   86  58  27  Brown         4 Red             1 Red
255  32 150  FF2096  100  12  58  Magenta       5 Magenta         5 Magenta
255 183  70  FFB746  100  71  27  Salmon        6 Brown           3 Yellow
255 183 150  FFB796  100  71  58  Light Yellow  7 Light Grey      7 White
102 102 102  666666   40  40  40  Grey          8 Dark Grey       8 Bold Black
 32  32 221  2020DD   12  12  86  Light Blue    9 Light Blue     12 Bold Blue
150 255   0  96FF00   58 100   0  Light Green  10 Light Green    10 Bold Green
  0 255 221  00FFDD    0 100  86  Light Cyan   11 Light Cyan     14 Bold Cyan
255   0   0  FF0000  100   0   0  Light Red    12 Light Red       9 Bold Red
249 178 215  F9B2D7   98  70  84  Pink         13 Light Magenta  13 Bold Magenta
255 255   0  FFFF00  100 100   0  Yellow       14 Yellow         11 Bold Yellow
221 221 221  DDDDDD   86  86  86  Light Grey   15 White          15 Bold White

If your terminal only supports eight distinct colors, set this palette
instead (colors listed in ANSI order):

Red Grn Blu   Hex#    R%  G%  B%  MyMan color(s)             ANSI color
===-===-===--======--===-===-===--=========================--==========
  0   0   0  000000    0   0   0  Black                      0 Black   
238  75  35  EE4B23   93  29  13  Brown + Light Red          1 Red     
 75 219   0  4BDB00   29  85   0  Green + Light Green        2 Green   
255 219  35  FFDB23  100  85  13  Salmon + Yellow            3 Yellow  
 16  16 221  1010DD    6   6  86  Blue + Light Blue          4 Blue    
252 105 182  FC69B6   98  41  71  Magenta + Pink             5 Magenta 
  0 238 221  00EEDD    0  93  86  Cyan + Light Cyan          6 Cyan    
238 202 185  EECAB9   93  79  72  Light Yellow + Light Grey  7 White   
