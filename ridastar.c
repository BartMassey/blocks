#include "blocks.h"
#include "idamisc.c"

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
  /* move any tower top onto a correct tower */
  for (j = 0; j < s->n_towers; j++) {
    /* move only onto correct partial towers */
    if (score_towertop(s, j))
      continue;
    /* examine each towertop as from */
    for (i = 0; i < s->n_towers; i++)
      if (i != j) {
	int old_h = s->h_score;
	
	local_protect_move(s, &p, i, j);
	move(s, i, j);
	if (s->h_score > old_h - 1) {
	  /* this is not a strictly forward move */
	  backout_move(s, &p);
	  continue;
	}
	if (depth_first(s, c, &p))
	  return 1;
	backout_move(s, &p);
	/* a strictly forward move failed, so we're stuck */
	return 0;
      }
  }
  /* move any must-move-twice block onto the table */
  for (i = 0; i < s->n_towers; i++) {
    if (score_towertop(s, i) != 2)
      continue;
    if (s->blocks[s->tower_tops[i]].on > -1) {
      local_protect_move(s, &p, i, -1);
      move(s, i, -1);
      if (depth_first(s, c, &p))
	return 1;
      backout_move(s, &p);
      /* a strictly forward move failed, so we're stuck */
      return 0;
    }
  }
  /* otherwise, try moving something which must move to the table. */
  for (i = 0; i < s->n_towers; i++)
    if (s->blocks[s->tower_tops[i]].on > -1) {
      int old_h = s->h_score;
      
      local_protect_move(s, &p, i, -1);
      move(s, i, -1);
      if (s->h_score > old_h) {
        backout_move(s, &p);
	continue;
      }
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

  for (i = c; i < 2 * c + 2; i++) {
    struct state *s = copy_state(start);
    if (depth_first(s, i, 0))
      return 0;
    free_state(s);
  }
  printf("no solution!\n");
  return -1;
}
  

