#include "blocks.h"
#include "idaprotect.c"

static void write_path(struct state *s, struct protect *p) {
  int moved_block, moved_to;
  
  if (!p)
    return;
  backout_move(s, p);
  moved_block = p->block;
  moved_to = p->to;
  write_path(s, p->parent);
  printf("%d %d\n", moved_block, moved_to);
}

static int length(struct protect *p) {
  int n = 0;

  while(p) {
    p = p->parent;
    n++;
  }
  return n;
}

void ida_answer(struct state *g, struct protect *p) {
  g = copy_state(g);
  /* print stats */
  if (g->g_score != length(p))
    abort();
  if (verbose > 0) {
    printf("%g secs\n", cpu_secs());
    printf("%d nodes\n",
	   stat_nodes);
    if (stat_decision_nodes >= 0)
      printf("%d decision nodes\n", stat_decision_nodes);
    printf("%d heuristic, %d actual\n\n", start->h_score, g->g_score);
    write_path(g, p);
  } else {
    if (stat_decision_nodes >= 0)
      printf("%g %d %d %d %d\n",
	     cpu_secs(), stat_nodes, stat_decision_nodes,
	     start->h_score, g->g_score);
    else
      printf("%g %d %d %d\n",
	     cpu_secs(), stat_nodes, 
	     start->h_score, g->g_score);
  }
}
