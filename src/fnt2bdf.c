/*BINFMTC:
 * fnt2bdf.c - font converter for the MyMan video game
 * Copyright 1998-2007, Benjamin C. Wiley Sittler <bsittler@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy,
 *  modify, merge, publish, distribute, sublicense, and/or sell copies
 *  of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int
main(int argc, char **argv) {
  FILE *infile;
  char *shortname;
  const char *filename;
  int i;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s fntfile\nConverts an 8x8 fnt console font file to a bdf file.\n", argv[0]);
    return 2;
  }
  filename = argv[1];
  if ((argv[1][0] == '-') && ! argv[1][1]) {
    infile = stdin;
    filename = "stdin";
  } else if (! (infile = fopen(argv[1], "rb"))) {
    perror(filename);
    return 1;
  }
  if (! (shortname = strdup(filename))) {
    perror("strdup");
    exit (1);
  } else {
    int len = strlen(shortname);

    if ((len > 4) && ! strcasecmp(shortname + len - 4, ".fnt"))
      shortname[len - 4] = '\0';
  }
  if (strrchr(shortname, '/')
      &&
      (strrchr(shortname, '/'))[1])
  {
      shortname = strrchr(shortname, '/') + 1;
  }
  printf("STARTFONT 2.1\n");
  printf("FONT %s\n", shortname);
  printf("SIZE 8 75 75\n");
  printf("FONTBOUNDINGBOX 8 8 0 -1\n");
  printf("STARTPROPERTIES 3\n");
  printf("FONT_DESCENT 1\n");
  printf("FONT_ASCENT 7\n");
  printf("DEFAULT_CHAR 0\n");
  printf("ENDPROPERTIES\n");
  printf("CHARS 256\n");
  for (i = 0; i < 256; i++) {
    int j;

    printf("STARTCHAR C%4.4x\n", i);
    printf("ENCODING %d\n", i);
    printf("SWIDTH 666 0\n");
    printf("DWIDTH 8 0\n");
    printf("BBX 8 8 0 -1\n");
    printf("BITMAP\n");
    for (j = 0; j < 8; j++) {
      int c;

      if ((c = fgetc(infile)) == EOF) {
	if (feof(infile))
	  fprintf(stderr, "%s: premature EOF\n", filename);
	else
	  perror(filename);
	return 1;
      }
      printf("%2.2x\n", c);
    }
    printf("ENDCHAR\n");
  }
  printf("ENDFONT\n");
  if (infile != stdin)
    fclose(infile);
  return 0;
}
