#include "blocks.h"

struct statepq *a_open;
struct stateht *a_seen;

static int (*soln)[2];
static int n_soln = 0;

void a_push_state(struct state *s) {
  struct state **sp;
  
  /* deal with duplicates */
  sp = stateht_match(a_seen, s);
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
    statepq_insert(s, a_open);
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
  a_open = statepq_insert(s, a_open);
  a_seen = stateht_insert(a_seen, s);
}

static void save_path(struct state *sp) {
  if (!sp->parent)
    return;
  save_path(sp->parent);
  soln[n_soln][0] = sp->moved_block;
  soln[n_soln][1] = sp->moved_to;
  n_soln++;
}

static int length(struct state *g) {
  int n = 0;
  struct state *sp;

  for (sp = g; sp->parent; sp = sp->parent)
    n++;
  return n;
}

void a_answer(struct state *g) {
  int i;
  int n = length(g);
  
  /* print stats */
  if (g->g_score != n)
    abort();
  soln = malloc(n * sizeof(soln[0]));
  if (!soln) {
    perror("malloc: soln");
    exit(1);
  }
  save_path(g);
  if (verbose > 0) {
    printf("solution:\n");
    for(i = 0; i < n_soln; i++)
      printf("move block %d to block %d\n", soln[i][0], soln[i][1]);
  }
  check_soln(n_soln, soln);
  if (verbose > 0) {
    printf("%g secs\n", cpu_secs());
    printf("%d nodes, %d cached, %d max open\n",
	   stat_nodes, stat_cached, stat_max_open);
    printf("%d heuristic, %d actual\n\n", start->h_score, g->g_score);
  } else {
    printf("%g %d %d %d %d %d\n",
	   cpu_secs(),
	   stat_nodes, stat_cached, stat_max_open,
	   start->h_score, g->g_score);
  }
}
