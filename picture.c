#include <stdio.h>
#include "extern.h"

int getint(void) {
  char buf[128];

  gets(buf);
  return atoi(buf);
}

int n_blocks;

struct block {
  int on;
  int covered;
};

struct state {
  struct block *blocks;
  int *tower_tops;
  int n_towers;
};

/* global state */
static struct state *it;

static struct state *alloc_state(void) {
  struct state *new = malloc(sizeof(struct state));

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

static void write_picture(void) {
  int n = it->n_towers;
  int *tower_heights = malloc(n * sizeof(int));
  int b, h, i, j, k;
  int max_tower;

  /* get tower heights and check for cycles */
  for (i = 0; i < n; i++) {
    tower_heights[i] = 0;
    for (j = it->tower_tops[i], k = 0; j > -1 && k < n_blocks; j = it->blocks[j].on, k++)
      tower_heights[i]++;
    if (k >= n_blocks) {
      fprintf(stderr, "tower %d cycles\n", i);
      exit(-1);
    }
  }
  /* check for two blocks on same block */
  for (i = 0; i < n; i++)
    it->blocks[i].covered = 0;
  for (i = 0; i < it->n_towers; i++)
    for (b = it->blocks[it->tower_tops[i]].on; b > -1; b = it->blocks[b].on) {
      if (it->blocks[b].covered) {
	fprintf(stderr, "block %d covered twice\n", b);
	exit(-1);
      }
      it->blocks[b].covered = 1;
    }
  /* find the tallest tower */
  max_tower = 0;
  for (i = 0; i < n; i++)
    if (tower_heights[i] > max_tower)
      max_tower = tower_heights[i];
  /* actually print the towers */
  for (i = 0; i < max_tower; i++) {
    h = max_tower - i;
    for (j = 0; j < n; j++)
      if (tower_heights[j] < h)
	printf("   ");
      else {
	b = it->tower_tops[j];
	for (k = 0; k < tower_heights[j] - h; k++)
	  b = it->blocks[b].on;
	printf(" %02d", b);
      }
    printf("\n");
  }
}

int main(void) {
  read_statedesc();
  write_picture();
  return 0;
}
