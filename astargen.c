#include "blocks.h"

struct statepq *a_open;
struct stateht *a_seen;

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

static void write_path(struct state *sp) {
  if (!sp->parent)
    return;
  write_path(sp->parent);
  printf("%d %d\n", sp->moved_block, sp->moved_to);
}

static int length(struct state *g) {
  int n = 0;
  struct state *sp;

  for (sp = g; sp->parent; sp = sp->parent)
    n++;
  return n;
}

void a_answer(struct state *g) {
  /* print stats */
  if (g->g_score != length(g))
    abort();
  if (verbose > 0) {
    printf("%g secs\n", cpu_secs());
    printf("%d nodes, %d cached, %d max open\n",
	   stat_nodes, stat_cached, stat_max_open);
    printf("%d heuristic, %d actual\n\n", start->h_score, g->g_score);
    write_path(g);
  } else {
    printf("%g %d %d %d %d %d\n",
	   cpu_secs(),
	   stat_nodes, stat_cached, stat_max_open,
	   start->h_score, g->g_score);
  }
}
