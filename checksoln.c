#include "blocks.h"

static int cheap_move(struct state *s, int block, int to) {
  int i;
  int t_from, t_to, from;

  for (i = 0; i < s->n_towers; i++)
    if (s->tower_tops[i] == block)
      break;
  if (i >= s->n_towers)
    return 0;
  t_from = i;
  if (to > -1) {
    for (i = 0; i < s->n_towers; i++)
      if (s->tower_tops[i] == to)
	break;
    if (i >= s->n_towers)
      return 0;
    t_to = i;
  } else
    t_to = -1;
  from = s->blocks[block].on;
  if (from > -1)
    s->tower_tops[t_from] = from;
  else {
    s->tower_tops[t_from] = s->tower_tops[--s->n_towers];
    if (t_to == s->n_towers)
      t_to = t_from;
  }
  if (t_to == -1)
    t_to = s->n_towers++;
  s->tower_tops[t_to] = block;
  s->blocks[block].on = to;
  if (verbose > 2) {
    printf("moved %d to %d\n", block, to);
    write_picture(s);
  }
  return 1;
}

void check_soln(int n_soln, int (*soln)[2]) {
  int i;
  struct state *s = copy_state(saved_start);
  
  for (i = 0; i < n_soln; i++)
    if (!cheap_move(s, soln[i][0], soln[i][1])) {
      printf("move %d to %d invalid!\n", soln[i][0], soln[i][1]);
      write_picture(s);
      exit(1);
    }
  hash_state(s);
  if (!same_state(s, goal)) {
    printf("solution does not achieve goal!\n");
    write_picture(s);
    printf("\n");
    write_picture(goal);
    exit(1);
  }
}
