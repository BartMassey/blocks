#include <stdio.h>
#include "extern.h"

int getint(void) {
  char buf[128];

  fgets(buf, 128, stdin);
  return atoi(buf);
}

int n_blocks;

struct block {
  int on;
};

struct state {
  struct block *blocks;
  int *tower_tops;
  int *tower_bottoms;
  int n_towers;
};

/* global state */
static struct state *it;

static struct state *alloc_state(void) {
  struct state *new = malloc(sizeof(struct state));
  int i;

  if (!new) {
    perror("malloc (new)");
    exit(-1);
  }
  new->blocks = malloc(sizeof(struct block) * n_blocks);
  if (!new->blocks) {
    perror("malloc (new->blocks)");
    exit(-1);
  }
  /*
   * We should be careful and only allocate n_towers
   * new tower_tops, but that might be one too few
   * for move(), which may want to start a new tower...
   * Punt for now.
   */
  new->tower_tops = malloc(sizeof(int) * n_blocks);
  if (!new->tower_tops) {
    perror("malloc (new->tower_tops)");
    exit(-1);
  }
  /* this next is just to keep write_picture() happy. */
  new->tower_bottoms = malloc(sizeof(int) * n_blocks);
  if (!new->tower_bottoms) {
    perror("malloc (new->tower_bottoms)");
    exit(-1);
  }
  for(i = 0; i < n_blocks; i++)
    new->tower_bottoms[i] = -1;
  return new;
}

static struct state *read_state(void) {
  struct state *new = alloc_state();
  int on, i, j;

  /* read the blocks in */
  for (i = 0; i < n_blocks; i++) {
    on = getint();
    if (on < -1 || on >= n_blocks) {
      fprintf(stderr, "bad block %d on %d\n", i, on);
      exit(-1);
    }
    new->blocks[i].on = on;
  }
  /* calculate the tower tops -- quadratic */
  new->n_towers = 0;
  for (i = 0; i < n_blocks; i++) {
    for (j = 0; j < n_blocks; j++)
      if (new->blocks[j].on == i)
	break;
    if (j >= n_blocks)
      new->tower_tops[new->n_towers++] = i;
  }
  /* rest will be filled in by caller */
  return new;
}

static void read_statedesc(void) {
  /* get number of blocks */
  n_blocks = getint();
  if (getchar() != '\n') {
    fprintf(stderr, "no separator\n");
    exit(-1);
  }
  /* get state */
  it = read_state();
  if (getchar() != EOF) {
    fprintf(stderr, "expected EOF\n");
    exit(-1);
  }
}

#include "writepicture.c"

int main(void) {
  read_statedesc();
  write_picture(it);
  return 0;
}
