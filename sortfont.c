#include <stdio.h>

#define TILE_HEIGHT 2

char font[256][TILE_HEIGHT][5];
int used[256] = {0};

int main(int argc, char **argv) {
  FILE *infile;
  int i, j, k, c;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s font-file\nReads an ASCII font file, sorts it, and prints it\n",
	    argv[0]);
    return 2;
  }
  if ((*argv[1] == '-') && ! argv[1][1]) {
    infile = stdin;
    argv[1] = "stdin";
  } else if (! (infile = fopen(argv[1], "rb"))) {
    perror(argv[1]);
    return 1;
  }
  while (! feof(infile)) {
    if (fscanf(infile, "%x", &i) != 1) {
      if (! feof(infile)) {
	perror(argv[1]);
	return 1;
      }
      break;
    }
    if ((i < 0) || (i > 255)) {
      fprintf(stderr, "%s: invalid index %0.2X ignored\n", argv[1], i);
      break;
    }
    if (used[i])
      fprintf(stderr, "%s: duplicate definition for %0.2X\n", argv[1], i);
    used[i] = 1;
    for (j = 0; j < TILE_HEIGHT; j ++)
      for (k = 0; k < 5; k ++)
	font[i][j][k] = ' ';
    for (j = 0; j < TILE_HEIGHT; j ++) {
      while ((c = fgetc(infile)) != ':')
	if (c == EOF) {
	  if (! feof(infile))
	    perror(argv[1]);
	  else
	    fprintf(stderr, "%s: premature EOF in definition for index %0.2X\n",
		    argv[1], i);
	  return 1;
	}
      if ((c = fgetc(infile)) == EOF)
	if (! feof(infile)) {
	  perror(argv[1]);
	  return 1;
	}
      for (k = 0;
	   (k < 5) &&
	     (c != '\v') && (c != '\f') &&
	     (c != '\n') && (c != '\r') && ! feof(infile);
	   k ++) {
	font[i][j][k] = c;
	if ((c = fgetc(infile)) == EOF) {
	  if (feof(infile))
	    break;
	  perror(argv[1]);
	  return 1;
	}
      }
      while ((c != '\v') && (c != '\f') &&
	     (c != '\n') && (c != '\r') && ! feof(infile)) {
	if (c == EOF) {
	  if (feof(infile))
	    break;
	  perror(argv[1]);
	  return 1;
	}
	c = fgetc(infile);
      }
    }
  }
  if (infile != stdin)
    fclose(infile);
  for (i = 0; i < 256; i ++)
    if (used[i]) {
      printf("%0.2X\n", i);
      for (j = 0; j < TILE_HEIGHT; j ++) {
	putchar(':');
	for (k = 0; k < 5; k ++)
	  putchar(font[i][j][k]);
	if (! j)
	  printf(" '%c' %u", i, i);
	putchar('\n');
      }
      putchar('\n');
    }
  return 0;
}
