#include "blocks.h"
#include "idaprotect.c"

static int (*soln)[2];
static int n_soln = 0;

static void save_path(struct state *s, struct protect *p) {
  int moved_block, moved_to;
  
  if (!p)
    return;
  backout_move(s, p);
  moved_block = p->block;
  moved_to = p->to;
  save_path(s, p->parent);
  soln[n_soln][0] = moved_block;
  soln[n_soln][1] = moved_to;
  n_soln++;
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
  int i;
  int n = length(p);
  struct state *g0;
  
  /* print stats */
  if (g->g_score != n)
    abort();
  soln = malloc(n * sizeof(soln[0]));
  if (!soln) {
    perror("malloc: soln");
    exit(1);
  }
  g0 = clone_state(g);
  save_path(g0, p);
  if (verbose > 0) {
    printf("solution:\n");
    for(i = 0; i < n_soln; i++)
      printf("move block %d to block %d\n", soln[i][0], soln[i][1]);
  }
  check_soln(n_soln, soln);
  if (verbose > 0) {
    printf("%g secs\n", cpu_secs());
    printf("%d nodes\n",
	   stat_nodes);
    if (stat_decision_nodes >= 0)
      printf("%d decision nodes\n", stat_decision_nodes);
    printf("%d heuristic, %d actual\n\n", start->h_score, g->g_score);
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
