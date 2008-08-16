#include <stdio.h>

#define TILEWIDTH 5
#define TILEHEIGHT 2

char font[256][TILEHEIGHT][TILEWIDTH];
int used[256] = {0};

int main(int argc, char **argv) {
  FILE *infile;
  int i, j, k, c;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s font-file text-file\nReads an ASCII font file and uses it to render a text file\n",
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
    for (j = 0; j < TILEHEIGHT; j ++)
      for (k = 0; k < TILEWIDTH; k ++)
	font[i][j][k] = ' ';
    for (j = 0; j < TILEHEIGHT; j ++) {
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
	   (k < TILEWIDTH) &&
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
  argv ++;
  if (infile != stdin)
    fclose(infile);
  if ((*argv[1] == '-') && ! argv[1][1]) {
    infile = stdin;
    argv[1] = "stdin";
  } else if (! (infile = fopen(argv[1], "rb"))) {
    perror(argv[1]);
    return 1;
  }
  while (! feof(infile)) {
    unsigned char buffer[81];

    if (! fgets(buffer, 81, infile)) {
      if (feof(infile))
	break;
      perror(argv[1]);
      return 1;
    }
    for (i = 0; i < TILEHEIGHT; i ++) {
      for (j = 0; (c = buffer[j]); j ++)
	if ((c != '\n') && (c != '\r') && (c != '\v') && (c != '\f'))
	  for (k = 0; k < TILEWIDTH; k++)
	    putchar(used[c] ? font[c][i][k] : c);
      putchar('\n');
    }
  }
  if (infile != stdin)
    fclose(infile);
  return 0;
}
