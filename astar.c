#include "blocks.h"

int a_star(void) {
  struct state *s, *s0;
  int i, j;

  a_open = statepq_new();
  a_seen = stateht_new();
  a_push_state(start);
  while (1) {
    a_open = statepq_delmin(a_open, &s);
    if (!s)
      break;
    if (verbose > 8) {
      printf("consider state\n");
      write_picture(s);
      if (verbose > 9)
        printf("heuristic distance %d\n", s->h_score);
    }
    if (same_state(s, goal)) {
      a_answer(s);
      return 0;
    }
    --stat_open;
    if (verbose > 5 && s->g_score > stat_max_g) {
      stat_max_g = s->g_score;
      printf("g max now %d\n", stat_max_g);
    }
    if (verbose > 5 && s->h_score < stat_min_h) {
      stat_min_h = s->h_score;
      printf("h min now %d\n", stat_min_h);
    }
    /* try tabletop moves first */
    for (i = 0; i < s->n_towers; i++)
      if (s->blocks[s->tower_tops[i]].on > -1) {
	s0 = copy_state(s);
	move(s0, i, -1);
	a_push_state(s0);
      }
    /* then try tower-to-tower */
    for (i = 0; i < s->n_towers; i++)
      for (j = 0; j < s->n_towers; j++)
	if (i != j) {
	  s0 = copy_state(s);
	  move(s0, i, j);
	  a_push_state(s0);
	}
  }
  printf("no solution!\n");
  return -1;
}
  
