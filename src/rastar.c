#include "blocks.h"

int ra_star(void) {
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
    /* move any block not on the table which should be */
    for (i = 0; i < s->n_towers; i++) {
      int i0 = s->tower_tops[i];
      
      if (s->blocks[i0].on != -1 && goal->blocks[i0].on == -1) {
	  s0 = copy_state(s);
	  move(s0, i, -1);
	  a_push_state(s0);
	  goto next_state;
      }
    }
    /* move any other must-move block into position */
    for (j = 0; j < s->n_towers; j++) {
      int n = score_towertop(s, j);
      int j0;
      
      /* must-move-twice blocks go to the table */
      if (n == 2) {
        s0 = copy_state(s);
        move(s0, j, -1);
        a_push_state(s0);
        goto next_state;
      }
      /* move only onto correct partial towers */
      if (n)
	continue;
      j0 = s->tower_tops[j];
      /* examine each towertop as from */
      for (i = 0; i < s->n_towers; i++)
	if (i != j && j0 == goal->blocks[s->tower_tops[i]].on) {
	  s0 = copy_state(s);
	  move(s0, i, j);
	  a_push_state(s0);
	  goto next_state;
	}
    }
    /*
     * Otherwise, try moving something which must move to the table.
     * Note that any tower top not on the table must move, here.
     */
    for (i = 0; i < s->n_towers; i++) {
      if (s->blocks[s->tower_tops[i]].on == -1)
	continue;
      s0 = copy_state(s);
      move(s0, i, -1);
      a_push_state(s0);
    }
next_state:;
  }
  printf("no solution!\n");
  return -1;
}
