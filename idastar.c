#include "blocks.h"
#include "idaprotect.c"

static int depth_first(struct state *s, int c, struct protect *pp) {
  struct protect p;
  int i, j;
  
  if (s->t_score > c)
    return 0;
  if (same_state(s, goal)) {
    ida_answer(s, pp);
    return 1;
  }
  global_protect_move(s, &p, pp);
  /* try tabletop moves first */
  for (i = 0; i < s->n_towers; i++)
    if (s->blocks[s->tower_tops[i]].on > -1) {
      local_protect_move(s, &p, i, -1);
      move(s, i, -1);
      if (depth_first(s, c, &p))
	return 1;
      backout_move(s, &p);
    }
  /* then try tower-to-tower */
  for (i = 0; i < s->n_towers; i++)
    for (j = 0; j < s->n_towers; j++)
      if (i != j) {
	local_protect_move(s, &p, i, j);
	move(s, i, j);
	if (depth_first(s, c, &p))
	  return 1;
	backout_move(s, &p);
      }
  return 0;
}

int ida_star(void) {
  int c = start->t_score;
  int i;

  for (i = c; i < 2 * c + 2; i++) {
    struct state *s = copy_state(start);
    if (depth_first(s, i, 0))
      return 0;
    free_state(s);
  }
  printf("no solution!\n");
  return -1;
}
  

