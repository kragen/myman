#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <termios.h>

#define TILEFILE "tile5x2.txt"
#define TILEWIDTH 5
#define TILEHEIGHT 2

char tile[256 * TILEHEIGHT * TILEWIDTH];
int tile_used[256] = {0};

#define STILEFILE "stile7x3.txt"
#define STILEWIDTH 7
#define STILEHEIGHT 3

char stile[256 * STILEHEIGHT * STILEWIDTH];
int stile_used[256] = {0};

#define MAZEFILE "spear.txt"
#define WIDTH 21
#define HEIGHT 21
#define DOTS 164
#define DOTS_FRUIT1 35
#define DOTS_FRUIT2 70
#define MSGLEN 7
#define XMSG ((WIDTH - MSGLEN) / 2)
#define YMSG 11
#define READY    " READY!"
#define GAMEOVER " OVER! "
#define XSTART (WIDTH * TILEWIDTH / 2)
#define YSTART (31 * TILEHEIGHT / 2)
#define XFRUIT XSTART
#define YFRUIT (23 * TILEHEIGHT / 2)
#define YGHOST (YFRUIT - 2 * TILEHEIGHT)
#define YTOP (YFRUIT - 4 * TILEHEIGHT)
#define PAUSE " - PAUSED - "

unsigned char maze[HEIGHT][WIDTH + 1];
unsigned char blank_maze[HEIGHT][WIDTH + 1];
unsigned char msgbuf[MSGLEN];

#define SPRITES 32

unsigned char sprite[SPRITES], sprite_frame[SPRITES] = {'\0'};
int sprite_x[SPRITES], sprite_y[SPRITES];
int sprite_used[SPRITES] = {0};
int sprite_timer[SPRITES] = {0};

#define GHOSTS 4

int ghost_dir[GHOSTS], ghost_mem[GHOSTS], ghost_man[GHOSTS], ghost_timer[GHOSTS];

#define INKY 0
#define BLINKY 1
#define PINKY 2
#define CLYDE 3
#define MEAN(ghost) ((ghost) + 4)
#define MYMAN 8
#define BLUE(ghost) ((ghost) + 9)
#define FRUIT 13
#define SCORE1600 14
#define SCORE800 15
#define SCORE400 16
#define SCORE200 17
#define SCORE100 18

#define STILE_MEAN 8
#define STILE_EYES 10
#define STILE_BLUE 14
#define STILE_MYMAN 16
#define STILE_100 48
#define STILE_200 49
#define STILE_400 50
#define STILE_800 51
#define STILE_1600 52
#define STILE_WHITE 53

#define XTILE(x) ((x) / TILEWIDTH)
#define YTILE(y) ((y) / TILEHEIGHT)

#define PIXWIDTH ((WIDTH + 1) * TILEWIDTH)
#define PIXHEIGHT (HEIGHT * TILEHEIGHT)

#define XPIXWRAP(x) (((x) + PIXWIDTH) % PIXWIDTH)
#define YPIXWRAP(y) (((y) + PIXHEIGHT) % PIXHEIGHT)

#define XWRAP(x) (((x) + WIDTH + 1) % (WIDTH + 1))
#define YWRAP(y) (((y) + HEIGHT) % HEIGHT)

#define ISDOT(c) (((c) == 249) || ((c) == '.'))
#define ISPELLET(c) (((c) == 254) || ((c) == 'o'))
#define ISOPEN(c) (((c) == ' ') || ISDOT(c) || ISPELLET(c))
#define ISDOOR(c) ((c) == 240)

#define NOTTOP(y) (y >= (TILEHEIGHT / 2))
#define NOTBOTTOM(y) (y <= (TILEHEIGHT / 2))
#define NOTLEFT(x) (x >= (TILEWIDTH / 2))
#define NOTRIGHT(x) (x <= (TILEWIDTH / 2))

#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4

#define DIRWRAP(dir) (((dir) + 3) % 4 + 1)

#define XDIR(dir) (((dir) == RIGHT) - ((dir) == LEFT))
#define YDIR(dir) (((dir) == DOWN) - ((dir) == UP))

#define YLEAVING(dir, y) ( -(((dir) == UP) && ! NOTTOP(y)) + \
			   (((dir) == DOWN) && ! NOTBOTTOM(y)))
#define XLEAVING(dir, x) ( -(((dir) == LEFT) && ! NOTLEFT(x)) + \
			   (((dir) == RIGHT) && ! NOTRIGHT(x)))

#define BONUS(level) \
     (((level) < 4) ? (level - 1) : (((level) < 15) ? (level + 1) / 2 : 7))

#define DELAY 25000
#define TWOSECS 80
#define FRUITLIFE 300
#define DEATHSHIFT 3
#define DEATHDELAY (1 << (DEATHSHIFT + 2))
#define MEMDELAY (3 * TWOSECS)

int readfont(char *fontfile, int width, int height, char *font, int *used) {
  FILE *infile;
  int c, i, j, k;

  if (! (infile = fopen(fontfile, "rb"))) {
    perror(fontfile);
    return 1;
  }
  while (! feof(infile)) {
    if (fscanf(infile, "%x", &i) != 1) {
      if (! feof(infile)) {
	perror(fontfile);
	return 1;
      }
      continue;
    }
    if ((i < 0) || (i > 255)) {
      fprintf(stderr, "%s: invalid index %0.2X ignored\n", fontfile, i);
      continue;
    }
    if (used[i])
      fprintf(stderr, "%s: duplicate definition for %0.2X\n", fontfile, i);
    used[i] = 1;
    for (j = 0; j < height; j ++)
      for (k = 0; k < width; k ++)
	font[(i * height + j) * width + k] = ' ';
    for (j = 0; j < height; j ++) {
      while ((c = fgetc(infile)) != ':')
	if (c == EOF) {
	  if (! feof(infile))
	    perror(fontfile);
	  else
	    fprintf(stderr, "%s: premature EOF in definition for index %0.2X\n",
		    fontfile, i);
	  return 1;
	}
      if ((c = fgetc(infile)) == EOF)
	if (! feof(infile)) {
	  perror(fontfile);
	  return 1;
	}
      for (k = 0;
	   (k < width) &&
	     (c != '\v') && (c != '\f') &&
	     (c != '\n') && (c != '\r') && ! feof(infile);
	   k ++) {
	font[(i * height + j) * width + k] = c;
	if ((c = fgetc(infile)) == EOF) {
	  if (feof(infile))
	    continue;
	  perror(fontfile);
	  return 1;
	}
      }
      while ((c != '\v') && (c != '\f') &&
	     (c != '\n') && (c != '\r') && ! feof(infile)) {
	if (c == EOF) {
	  if (feof(infile))
	    continue;
	  perror(fontfile);
	  return 1;
	}
	c = fgetc(infile);
      }
    }
  }
  fclose(infile);
  return 0;
}

#define collide(s1, s2) ((XTILE(sprite_x[s1] - STILEWIDTH / 2) == XTILE(sprite_x[s2] - STILEWIDTH / 2)) \
			 && (YTILE(sprite_y[s1] - STILEHEIGHT / 2) == YTILE(sprite_y[s2] - STILEHEIGHT / 2)))

int main(int argc, char **argv) {
  FILE *infile;
  int i, j, k, kk = ERR, c;
  int dir = 0;
  int debug = 0;
  unsigned char *buffer;
  int
    level = 1,
    cycles = 0,
    score = 0,
    dots = 0,
    points = 0,
    points_sprite = SCORE200,
    pellet_timer = 0,
    lives = 3,
    earned = 0,
    dying = 0, winning = 0,
    dead = YSTART - YGHOST;

  sprite[FRUIT] = BONUS(level);
  sprite_x[FRUIT] = XFRUIT;
  sprite_y[FRUIT] = YFRUIT;

  sprite[SCORE100] = STILE_100;
  sprite_x[SCORE100] = XSTART;
  sprite_y[SCORE100] = YFRUIT;

  sprite[SCORE200] = STILE_200;
  sprite[SCORE400] = STILE_400;
  sprite[SCORE800] = STILE_800;
  sprite[SCORE1600] = STILE_1600;

  sprite[MYMAN] = STILE_MYMAN;
  sprite_x[MYMAN] = XSTART;
  sprite_y[MYMAN] = YSTART;
  sprite_used[MYMAN] = 1;

  for (i = 0; i < GHOSTS; i++) {
    int mean, blue;
    
    mean = MEAN(i);
    blue = BLUE(i);
    sprite_used[i] = 1;
    sprite_used[mean] = 1;
    sprite_used[blue] = 0;
    sprite[i] = STILE_EYES;
    sprite[mean] = STILE_MEAN;
    sprite_frame[i] = (ghost_dir[i] = i + 1) - 1;
    ghost_mem[i] = 0;
    ghost_timer[i] = TWOSECS;
    ghost_man[i] = 0;
    sprite_x[mean] =
      (sprite_x[i] = XSTART + 2 * TILEWIDTH * ((i == CLYDE) - (i == INKY)));
    sprite_y[mean] =
      (sprite_y[i] = YGHOST - 3 * TILEHEIGHT * (i == BLINKY));
  }

  if (argc != 1) {
    fprintf(stderr, "Usage: %s\n", *argv);
    return 2;
  }
  if (! (buffer = (unsigned char *)malloc(COLS * sizeof(unsigned char)))) {
    perror("malloc");
    return 1;
  }
  if (readfont(TILEFILE, TILEWIDTH, TILEHEIGHT, tile, tile_used) ||
      readfont(STILEFILE, STILEWIDTH, STILEHEIGHT, stile, stile_used))
    return 1;
  if (! (infile = fopen(MAZEFILE, "rb"))) {
    perror(MAZEFILE);
    return 1;
  }
  for (i = 0; i < HEIGHT; i ++) {
    for (j = 0; j < WIDTH; j ++)
      if ((c = fgetc(infile)) == EOF) {
	if (feof(infile))
	  fprintf(stderr, "%s: premature EOF\n", MAZEFILE);
	else
	  perror(MAZEFILE);
	return 1;
      } else if ((c == '\r') || (c == '\n') || (c == '\f') || (c == '\v'))
	j --;
      else
	blank_maze[i][j] = (maze[i][j] = c);
    blank_maze[i][WIDTH] = (maze[i][WIDTH] = c);
  }
  fclose(infile);

  initscr();
  clear();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  idlok(stdscr, TRUE);

  memcpy(msgbuf, maze[YMSG] + XMSG, MSGLEN);
  memcpy(maze[YMSG] + XMSG, READY, MSGLEN);

  while ((lives || dead || dying) &&
	 ((k = getch()) != 'q') && (k != 'Q'))
  {
    int s, x1, y1, line, col, xtile, ytile, xoff, yoff;

    usleep(DELAY);
    if ((k == 'r') || (k == 'R')) {
      clearok(curscr, TRUE);
      wrefresh(stdscr);
      continue;
    }
    if ((k == 'p') || (k == 'P') || (k == 27)) {
      mvprintw(LINES / 2, (COLS - strlen(PAUSE)) / 2, PAUSE);
      wrefresh(stdscr);
      while (getch() == ERR)
	usleep(DELAY);
      continue;
    }
    if (k == ERR)
      k = kk;
    else
      kk = k;
    xoff = sprite_x[MYMAN] % TILEWIDTH;
    yoff = sprite_y[MYMAN] % TILEHEIGHT;
    xtile = XTILE(sprite_x[MYMAN]);
    ytile = YTILE(sprite_y[MYMAN]);
    if (winning || dying) {
      int reset = 0;
      if (dying && ! -- dying) {
	reset = 1;
	-- lives;
      } else if (winning && ! -- winning) {
	memcpy((void *)maze,
	       (void *)blank_maze,
	       (WIDTH + 1) * HEIGHT * sizeof(unsigned char));
	dots = 0;
	pellet_timer = 0;
	sprite_used[FRUIT] = 0;
	reset = 1;
	++ level;
	sprite[FRUIT] = BONUS(level);
      }
      if (reset) {
	for (i = 0; i < GHOSTS; i++) {
	  int mean, blue;

	  mean = MEAN(i);
	  blue = BLUE(i);
	  sprite_used[i] = lives ? 1 : 0;
	  sprite_used[mean] = lives ? 1 : 0;
	  sprite_used[blue] = 0;
	  sprite_frame[i] = (ghost_dir[i] = i + 1) - 1;
	  ghost_mem[i] = 0;
	  ghost_timer[i] = TWOSECS;
	  ghost_man[i] = 0;
	  sprite_x[mean] =
	    (sprite_x[i] = XSTART + 2 * TILEWIDTH * ((i == CLYDE) - (i == INKY)));
	  sprite_y[mean] =
	    (sprite_y[i] = YGHOST - 3 * TILEHEIGHT * (i == BLINKY));
	}
	cycles = 0;
	sprite_x[MYMAN] = XSTART;
	sprite_y[MYMAN] = YSTART;
	sprite_frame[MYMAN] = 0;
	sprite_used[SCORE100] = 0;
	dead = YSTART - YGHOST;
	memcpy(msgbuf, maze[YMSG] + XMSG, MSGLEN);
	memcpy(maze[YMSG] + XMSG, lives ? READY : GAMEOVER, MSGLEN);
      }
      if (dying)
	sprite_frame[MYMAN] = ((DEATHDELAY - dying) >> DEATHSHIFT) % 4;
    } else if (dead) {
      if (! -- dead) {
	memcpy(maze[YMSG] + XMSG, msgbuf, MSGLEN);
	sprite[MYMAN] = STILE_MYMAN + 4;
	dir = LEFT;
      }
    } else {
      if (((score >= 10000) && ! earned) ||
	  ((score >= 50000) && (earned == 1))) {
	earned ++;
	lives ++;
      }
      if (ISPELLET((c = maze[ytile][xtile])) || ISDOT(c)) {
	maze[ytile][xtile] = ' ';
	sprite_frame[MYMAN] = 0;
	score += 10 + 40 * ISPELLET(c);
	if (ISPELLET(c)) {
	  points = 200;
	  points_sprite = SCORE200;
	  pellet_timer = 3 * TWOSECS;
	  for (s = 0; s < GHOSTS; s++) {
	    int mean, blue;

	    blue = BLUE(s);
	    mean = MEAN(s);
	    sprite_frame[blue] = 0;
	    if (sprite_used[mean]) {
	      sprite_used[s] = 0;
	      sprite_used[mean] = 0;
	      sprite_used[blue] = 1;
	      sprite[blue] = STILE_BLUE;
	      sprite_x[blue] = sprite_x[s];
	      sprite_y[blue] = sprite_y[s];
	    }
	  }
	}
	if ((++ dots == DOTS_FRUIT1) || (dots == DOTS_FRUIT2)) {
	  sprite_used[FRUIT] = 1;
	  sprite_timer[FRUIT] = FRUITLIFE;
	} else if (dots == DOTS) {
	  winning = 40;
	  sprite_frame[MYMAN] = 0;
	}
      } else if (sprite_used[FRUIT] &&
		 collide(MYMAN, FRUIT)) {
	sprite_used[FRUIT] = 0;
	sprite_used[SCORE100] = 1;
	sprite_timer[SCORE100] = TWOSECS;
	score += 100;
	sprite_frame[MYMAN] = 0;
      }
      if ((k == 'w') || (k == 'W')) {
	dots = DOTS - 1;
	kk = ERR;
      } else if ((k == 'd') || (k == 'D')) {
	debug = 1 - debug;
	kk = ERR;
      } else if (((k == 'h') || (k == 'H') || (k == '4') || (k == KEY_LEFT))
	  && ISOPEN(maze[ytile][XWRAP(xtile - NOTRIGHT(xoff))])) {
	dir = LEFT;
	sprite[MYMAN] = STILE_MYMAN + 4;
      } else if (((k == 'l') || (k == 'L') || (k == '6') || (k == KEY_RIGHT))
	       && ISOPEN(maze[ytile][XWRAP(xtile + NOTLEFT(xoff))])) {
	dir = RIGHT;
	sprite[MYMAN] = STILE_MYMAN + 12;
      } else if (((k == 'k') || (k == 'K') || (k == '8') || (k == KEY_UP))
	       && ISOPEN(maze[YWRAP(ytile - NOTBOTTOM(yoff))][xtile])) {
	dir = UP;
	sprite[MYMAN] = STILE_MYMAN;
      } else if (((k == 'j') || (k == 'J') || (k == '2') || (k == KEY_DOWN))
		 && ISOPEN(maze[YWRAP(ytile + NOTTOP(yoff))][xtile])) {
	dir = DOWN;
	sprite[MYMAN] = STILE_MYMAN + 16;
      }
      if (XDIR(dir))
	sprite_y[MYMAN] = (1 + 2 * ytile) * TILEHEIGHT / 2;
      else if (dir)
	sprite_x[MYMAN] = (1 + 2 * xtile) * TILEWIDTH / 2;
      if (ISOPEN(maze
		 [YWRAP(ytile + YLEAVING(dir, yoff + YDIR(dir)))]
		 [XWRAP(xtile + XLEAVING(dir, xoff + XDIR(dir)))])) {
	sprite_x[MYMAN] = XPIXWRAP(sprite_x[MYMAN] + XDIR(dir));
	sprite_y[MYMAN] = YPIXWRAP(sprite_y[MYMAN] + YDIR(dir) * (cycles & 1));
	sprite_frame[MYMAN] = (sprite_frame[MYMAN] + 1) % 4;
      }
      for (s = 0; s < GHOSTS; s ++) {
	int blue, mean, scared;
	
	blue = BLUE(s);
	mean = MEAN(s);
	scared = 0;
	sprite_frame[blue] =
	  (sprite_frame[mean] = (cycles & 4) ? 1 : 0);
	if (dying || winning) {
	  sprite_used[s] =
	    (sprite_used[blue] =
	     (sprite_used[mean] = 0));
	} else if (sprite_used[mean] && collide(mean, MYMAN)) {
	  dying = DEATHDELAY;
	  sprite[MYMAN] = STILE_MYMAN + 8;
	  sprite_frame[MYMAN] = 0;
	  s = -1;
	  continue;
	} else if (sprite_used[blue] && collide(blue, MYMAN)) {
	  score += points;
	  points *= 2;
	  sprite_timer[points_sprite] = TWOSECS;
	  sprite_used[points_sprite] = 1;
	  sprite_x[points_sprite] = sprite_x[blue];
	  sprite_y[points_sprite] = sprite_y[blue];
	  points_sprite --;
	  sprite_used[s] = 1;
	  sprite_x[s] = sprite_x[blue];
	  sprite_y[s] = sprite_y[blue];
	  sprite_used[blue] = 0;
	} else {
	  int dir0, dir1, dir2, o0, o1, o2, x, y, i1, j1, i, j;

	  dir1 = ghost_dir[s];
	  dir0 = DIRWRAP(dir1 + (s != PINKY) - (s == PINKY));
	  dir2 = DIRWRAP(dir1 - (s != PINKY) + (s == PINKY));
	  if (sprite_used[blue]) {
	    /* runnin' scared */
	    if (cycles & 1) {
	      x = sprite_x[blue];
	      y = sprite_y[blue];
	      i1 = XTILE(x);
	      j1 = YTILE(y);
	      o0 = ISOPEN(maze[YWRAP(j1 + YDIR(dir0))][XWRAP(i1 + XDIR(dir0))]);
	      o1 = ISOPEN(maze[YWRAP(j1 + YDIR(dir1))][XWRAP(i1 + XDIR(dir1))]);
	      o2 = ISOPEN(maze[YWRAP(j1 + YDIR(dir2))][XWRAP(i1 + XDIR(dir2))]);
	      if (((TILEWIDTH / 2 == x % TILEWIDTH) && XDIR(dir1))
		  || ((TILEHEIGHT / 2 == y % TILEHEIGHT) && YDIR(dir1))) {
		if (o0 || o2) {
		  if (! ghost_mem[s]) {
		    if (o2)
		      dir1 = dir2;
		    else if (! o1)
		      dir1 = dir0;
		  } else {
		    if (o0 && (dir0 != ghost_mem[s]))
		      dir1 = dir0;
		    else if (o2 && (dir2 != ghost_mem[s]))
		      dir1 = dir2;
		    else if (o1 && (dir1 != ghost_mem[s]))
			;
		    else if (o0)
		      dir1 = dir0;
		    else
		      dir1 = dir2;
		  }
		} else if (! o1)
		  dir1 = DIRWRAP(dir1 + 2);
	      }
	      if (XDIR(dir1))
		y = (1 + 2 * j1) * TILEHEIGHT / 2;
	      else if (dir1)
		x = (1 + 2 * i1) * TILEWIDTH / 2;
	      sprite_x[blue] = XPIXWRAP(x + XDIR(dir1));
	      if (cycles & 2)
		sprite_y[blue] = YPIXWRAP(y + YDIR(dir1));
	    }
	  } else if (sprite_used[mean]) {
	    /* out huntin' */
	    int door, d0, d1, d2;

	    x = sprite_x[mean];
	    y = sprite_y[mean];
	    i1 = XTILE(x);
	    j1 = YTILE(y);
	    o0 = ISOPEN(maze[YWRAP(j1 + YDIR(dir0))]
			[XWRAP(i1 + XDIR(dir0))]);
	    o1 = ISOPEN(maze[YWRAP(j1 + YDIR(dir1))]
			[XWRAP(i1 + XDIR(dir1))]);
	    o2 = ISOPEN(maze[YWRAP(j1 + YDIR(dir2))][XWRAP(i1 + XDIR(dir2))]);
	    door = ISDOOR(maze[YWRAP(j1 - 1)][i1]);
	    d0 = (dir0 == UP) && door;
	    d1 = (dir1 == UP) && door;
	    d2 = (dir2 == UP) && door;
	    if (((TILEWIDTH / 2 == x % TILEWIDTH) && XDIR(dir1))
		|| ((TILEHEIGHT / 2 == y % TILEHEIGHT) && YDIR(dir1))) {
	      if (d2 || (o2 && (dir2 == ghost_mem[s])))
		dir1 = dir2;
	      else if (d1 || (o1 && (dir1 == ghost_mem[s])))
		  ;
	      else if (d0 || (o0 && (dir0 == ghost_mem[s])))
		dir1 = dir0;
	      else if (o1)
		  ;
	      else if (o2)
		dir1 = dir2;
	      else if (o0)
		dir1 = dir0;
	      else
		dir1 = DIRWRAP(dir1 + 2);
	    }
	    if (XDIR(dir1))
	      y = (1 + 2 * j1) * TILEHEIGHT / 2;
	    else if (dir1)
	      x = (1 + 2 * i1) * TILEWIDTH / 2;
	    sprite_x[mean] = (sprite_x[s] = XPIXWRAP(x + XDIR(dir1)));
	    if (cycles & 1)
	      sprite_y[mean] = (sprite_y[s] = YPIXWRAP(y + YDIR(dir1)));
	  } else if (sprite_used[s]) {
	    int dx, dy;

	    /* goin' home */
	    ghost_timer[s] = MEMDELAY;
	    x = sprite_x[s];
	    y = sprite_y[s];
	    dx = (XSTART - x) / TILEWIDTH;
	    dy = ((dx ? YTOP : YGHOST) - y) / TILEHEIGHT;
	    if (dx * dx > dy * dy)
	      if (dx > 0)
		ghost_mem[s] = RIGHT;
	      else
		ghost_mem[s] = LEFT;
	    else
	      if (dy < 0)
		ghost_mem[s] = UP;
	      else
		ghost_mem[s] = DOWN;
	    i1 = XTILE(x);
	    j1 = YTILE(y);
	    if (! (dx || dy)) {
	      sprite_used[mean] = 1;
	      sprite_x[mean] = x;
	      sprite_y[mean] = y;
	      sprite_frame[s] = (ghost_dir[s] = s + 1) - 1;
	      ghost_mem[s] = 0;
	      continue;
	    }
	    o0 = ISOPEN(maze[YWRAP(j1 + YDIR(dir0))][XWRAP(i1 + XDIR(dir0))])
	      || ISDOOR(maze[YWRAP(j1 + YDIR(dir0))][XWRAP(i1 + XDIR(dir0))]);
	    o1 = ISOPEN(maze[YWRAP(j1 + YDIR(dir1))][XWRAP(i1 + XDIR(dir1))])
	      || ISDOOR(maze[YWRAP(j1 + YDIR(dir1))][XWRAP(i1 + XDIR(dir1))]);
	    o2 = ISOPEN(maze[YWRAP(j1 + YDIR(dir2))][XWRAP(i1 + XDIR(dir2))])
	      || ISDOOR(maze[YWRAP(j1 + YDIR(dir2))][XWRAP(i1 + XDIR(dir2))]);
	    if (o2 && (dir2 == ghost_mem[s]))
	      dir1 = dir2;
	    else if (o1 && (dir1 == ghost_mem[s]))
		;
	    else if (o0 && (dir0 == ghost_mem[s]))
	      dir1 = dir0;
	    else if (o1)
		;
	    else if (o2)
	      dir1 = dir2;
	    else if (o0)
	      dir1 = dir0;
	    else
	      dir1 = DIRWRAP(dir1 + 2);
	    sprite_x[mean] = (sprite_x[s] = XPIXWRAP(x + XDIR(dir1)));
	    sprite_x[s] = XPIXWRAP(x + XDIR(dir1));
	    sprite_y[s] = YPIXWRAP(y + YDIR(dir1));
	  }
	  if (ghost_timer[s] && ! -- ghost_timer[s]) {
	    ghost_timer[s] = MEMDELAY;
	    if (ghost_mem[s]) {
	      ghost_mem[s] = 0;
	    } else
	      dir1 = DIRWRAP(ghost_dir[s] + 2);
	  }
	  ghost_dir[s] = (sprite_frame[s] = dir1 - 1) + 1;
	}
      }
      for (s = 0; s < SPRITES; s ++) {
	if (sprite_used[s] && sprite_timer[s])
	  if (! -- sprite_timer[s])
	    sprite_used[s] = 0;
      }
    }
    x1 = sprite_x[MYMAN] - COLS / 2;
    y1 = sprite_y[MYMAN] - LINES / 2 - dead;
    if (x1 + COLS > WIDTH * TILEWIDTH)
      x1 = WIDTH * TILEWIDTH - COLS;
    if (y1 + LINES > HEIGHT * TILEHEIGHT)
      y1 = HEIGHT * TILEHEIGHT - LINES;
    if (x1 < 0)
      x1 = 0;
    if (y1 < 0)
      y1 = 0;
    for (line = 0; line < LINES; line++) {
      for (col = 0; col < COLS; col++) {
	int s;

	c = 0;
	xtile = XTILE((i = col + x1));
	ytile = YTILE((j = line + y1));
	for (s = 0; s < SPRITES; s ++) {
	  int t, x, y;

	  if (debug
	      && sprite_used[s]
	      && ((x = sprite_x[s]) == i)
	      && ((y = sprite_y[s]) == j)) {
	    if (s < GHOSTS)
	      c = (".^<v>")[ghost_mem[s]];
	    else
	      c = '.';
	    break;
	  } else if (sprite_used[s]
	      && stile_used[(t = sprite[s] + sprite_frame[s])]
	      && ((x = sprite_x[s] - STILEWIDTH / 2) <= i)
	      && ((xoff = i - x) < STILEWIDTH)
	      && ((y = sprite_y[s] - STILEHEIGHT / 2) <= j)
	      && ((yoff = j - y) < STILEHEIGHT)
	      && (c = stile[(t * STILEHEIGHT + yoff) * STILEWIDTH + xoff]))
	    break;
	}
	if ((ytile < HEIGHT) && (xtile < WIDTH)) {
	  if (c)
	    buffer[col] = c;
	  else if (tile_used[(c = maze[ytile][xtile])])
	    if (debug)
	      buffer[col] = (ISOPEN(c) ? ' ' : (ISDOOR(c) ? 'X' : '@'));
	    else if ((ISPELLET(c) && (cycles & 8)) || (winning & 8))
	      buffer[col] = ' ';
	    else	      
	      buffer[col] = tile[(c * TILEHEIGHT
				  + j % TILEHEIGHT) * TILEWIDTH
				+ i % TILEWIDTH];
	  else
	    buffer[col] = ' ';
	} else
	  buffer[col] = ' ';
      }
      mvaddnstr(line, 0, buffer, COLS);
    }
    if (sprite_used[FRUIT]) {
      char *msg[] = {" <  <N>  > ",
		     "<  <ONU>  >",
		     "  <BONUS>  ",
		     " < BONUS > ",
		     "<  BONUS  >",
		     " > BONUS < ",
		     "  >BONUS<  ",
		     ">  >ONU<  <"};

      mvprintw(LINES - 1, 1, msg[(cycles >> 1) & 7]);
    }
    if (pellet_timer) {
      int s, blue, mean;

      if (! -- pellet_timer) {
	for (s = 0; s < GHOSTS; s ++)
	  if (sprite_used[(blue = BLUE(s))]) {
	    sprite_used[(mean = MEAN(s))] = 1;
	    sprite_used[blue] = 0;
	    sprite_used[s] = 1;
	    sprite_x[s] =
	      (sprite_x[mean] = sprite_x[blue]);
	    sprite_y[s] =
	      (sprite_y[mean] = sprite_y[blue]);
	    sprite_frame[s] = (ghost_dir[s] = s + 1) - 1;
	  }
      } else if (pellet_timer <= TWOSECS)
	for (s = 0; s < GHOSTS; s ++)
	  sprite[BLUE(s)] = ((pellet_timer / 10) & 1) ? STILE_BLUE : STILE_WHITE;
    }
    mvprintw(LINES - 1, COLS - 46, " Level: %-10u Lives: %d Score: %-10u ",
	     level, lives, score);
    refresh();
    cycles ++;
  }
  endwin();
  fprintf(stderr, "%s: scored %d points, reached level %d\n",
	  argv[0], score, level);
  return 0;
}
