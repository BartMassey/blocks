#include "blocks.h"

static struct statepq *open;
static struct stateht *seen;

static void push_state(struct state *s) {
  struct state **sp;
  
  /* deal with duplicates */
  sp = stateht_match(seen, s);
  if (sp) {
    struct state *s0 = *sp;
    
    if (s->g_score >= s0->g_score) {
      free_state (s);
      return;
    }
    if (s0->q) {
      statepq_delete(s0->q);
    } else {
      stat_open++;
      if (stat_open > stat_max_open)
	stat_max_open=stat_open;
    }
    statepq_insert(s, open);
    *sp = s;
    free_state(s0);
    return;
  }
  /* update stats */
  stat_cached++;
  stat_open++;
  if (stat_open > stat_max_open)
    stat_max_open=stat_open;
  /* insert in appropriate places */
  open = statepq_insert(s, open);
  seen = stateht_insert(seen, s);
}

static void ra_star_write_path(struct state *sp) {
  if (!sp->parent)
    return;
  ra_star_write_path(sp->parent);
  printf("%d %d\n", sp->moved_block, sp->moved_to);
}

static int ra_star_length(struct state *g) {
  int n = 0;
  struct state *sp;

  for (sp = g; sp->parent; sp = sp->parent)
    n++;
  return n;
}


static void ra_star_answer(struct state *g) {
  /* print stats */
  if (g->g_score != ra_star_length(g))
    abort();
  if (verbose > 0) {
    printf("%g secs\n", cpu_secs());
    printf("%d nodes, %d cached, %d max open\n",
	   stat_nodes, stat_cached, stat_max_open);
    printf("%d heuristic, %d actual\n\n", start->h_score, g->g_score);
    ra_star_write_path(g);
  } else {
    printf("%g %d %d %d %d %d\n",
	   cpu_secs(),
	   stat_nodes, stat_cached, stat_max_open,
	   start->h_score, g->g_score);
  }
}

int ra_star(void) {
  struct state *s, *s0;
  int i, j;

  open = statepq_new();
  seen = stateht_new();
  push_state(start);
  while (1) {
    open = statepq_delmin(open, &s);
    if (!s)
      break;
    if (verbose > 8) {
      printf("consider state\n");
      write_picture(s);
      if (verbose > 9)
        printf("heuristic distance %d\n", s->h_score);
    }
    if (same_state(s, goal)) {
      ra_star_answer(s);
      return 0;
    }
    --stat_open;
    if (verbose > 1 && s->g_score > stat_max_g) {
      stat_max_g = s->g_score;
      printf("g max now %d\n", stat_max_g);
    }
    if (verbose > 1 && s->h_score < stat_min_h) {
      stat_min_h = s->h_score;
      printf("h min now %d\n", stat_min_h);
    }
    /* move any block not on the table which should be */
    for (i = 0; i < s->n_towers; i++) {
      int i0 = s->tower_tops[i];
      
      if (s->blocks[i0].on != -1 && goal->blocks[i0].on == -1) {
	  s0 = copy_state(s);
	  move(s0, i, -1);
	  push_state(s0);
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
        push_state(s0);
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
	  push_state(s0);
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
      push_state(s0);
    }
next_state:;
  }
  printf("no solution!\n");
  return -1;
}
