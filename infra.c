#include "blocks.h"

void hash_state(struct state *s) {
  int h = 0;
  int i;
  
  for (i = 0; i < n_blocks; i++)
    h += HASH(i, s->blocks[i].on);
  s->hash = h;
}

struct state *read_state(void) {
  struct state *new = alloc_state();
  int on, i, j;

  /* read the blocks in */
  for (i = 0; i < n_blocks; i++) {
    on = getint();
    if (on < -1 || on >= n_blocks)
      abort();
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

void read_problem(void) {
  /* get number of blocks */
  n_blocks = getint();
  if (getchar() != '\n')
    abort();
  start = read_state();
  if (getchar() != '\n')
    abort();
  goal = read_state();
  if (getchar() != EOF)
    abort();
}

void fix_bottoms(struct state *s) {
  int i;
  
  for (i = 0; i < s->n_towers; i++) {
    t_block b;
    
    for (b = s->tower_tops[i]; b != -1; b = s->blocks[b].on)
      if (!ON_CORRECT(s, b))
	break;
    if (b == -1) {
      s->tower_bottoms[i] = s->tower_tops[i];
      continue;
    }
    s->tower_bottoms[i] = s->blocks[b].on;
  }
}

