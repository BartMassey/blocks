#include "blocks.h"
#include "idaprotect.c"

static int depth_first(struct state *s, int c, struct protect *pp) {
  struct protect p;
  int i, j;
  
  if (same_state(s, goal)) {
    ida_answer(s, pp);
    return 1;
  }
  if (s->t_score > c)
    return 0;
  global_protect_move(s, &p, pp);
  /* move any block not on the table which should be */
  for (i = 0; i < s->n_towers; i++) {
    int i0 = s->tower_tops[i];

    if (s->blocks[i0].on != -1 && goal->blocks[i0].on == -1) {
	local_protect_move(s, &p, i, -1);
	move(s, i, -1);
	if (depth_first(s, c, &p))
	  return 1;
	backout_move(s, &p);
	return 0;
    }
  }
  /* move any other must-move block into position */
  for (j = 0; j < s->n_towers; j++) {
    int n = score_towertop(s, j);
    int j0;

    /* must-move-twice blocks go to the table */
    if (n == 2) {
      local_protect_move(s, &p, j, -1);
      move(s, j, -1);
      if (depth_first(s, c, &p))
	return 1;
      backout_move(s, &p);
      return 0;
    }
    /* move only onto correct partial towers */
    if (n)
      continue;
    j0 = s->tower_tops[j];
    /* examine each towertop as from */
    for (i = 0; i < s->n_towers; i++)
      if (i != j && j0 == goal->blocks[s->tower_tops[i]].on) {
	local_protect_move(s, &p, i, j);
	move(s, i, j);
	if (depth_first(s, c, &p))
	  return 1;
	backout_move(s, &p);
	return 0;
      }
  }
  /*
   * Otherwise, try moving something which must move to the table.
   * Note that any tower top not on the table must move, here.
   */
  stat_decision_nodes++;
  for (i = 0; i < s->n_towers; i++) {
    if (s->blocks[s->tower_tops[i]].on == -1)
      continue;
    local_protect_move(s, &p, i, -1);
    move(s, i, -1);
    if (depth_first(s, c, &p))
      return 1;
    backout_move(s, &p);
  }
  /* We've tried all the moves that could possibly help.  We're stuck. */
  return 0;
}

int rida_star(void) {
  int c = start->t_score;
  int i;

  stat_decision_nodes = 0;
  for (i = c; i < 2 * c + 2; i++) {
    struct state *s = copy_state(start);
    if (depth_first(s, i, 0))
      return 0;
    free_state(s);
  }
  printf("no solution!\n");
  return -1;
}
  

