#include "blocks.h"

/* for do-undo move buffering (for depth_first()) */
struct protect {
  int t_from, t_to;
  int n_towers;
  int block, from, to;
  int h_score, g_score, t_score;
  struct protect *parent;
  int last_top;
};

static INLINE void global_protect_move(struct state *s, struct protect *p, struct protect *parent) {
  p->h_score = s->h_score;
  p->g_score = s->g_score;
  p->t_score = s->t_score;
  p->parent = parent;
  p->n_towers = s->n_towers;
}

static INLINE void local_protect_move(struct state *s, struct protect *p, int t_from, int t_to) {
  p->t_from = t_from;
  p->t_to = t_to;
  p->block = s->tower_tops[t_from];
  if (t_to > -1)
    p->to = s->tower_tops[t_to];
  else
    p->to = -1;
  p->from = s->blocks[p->block].on;
  if (p->from == -1)
    p->last_top = s->tower_tops[s->n_towers - 1];
}

static INLINE void backout_move(struct state *s, struct protect *p) {
  s->n_towers = p->n_towers;
  s->tower_tops[p->t_from] = p->block;
  if (p->t_to > -1)
    s->tower_tops[p->t_to] = p->to;
  if (p->from == -1)
    s->tower_tops[s->n_towers - 1] = p->last_top;
  s->blocks[p->block].on = p->from;
  s->h_score = p->h_score;
  s->g_score = p->g_score;
  s->t_score = p->t_score;
}

static void ida_write_path(struct state *s, struct protect *p) {
  int moved_block, moved_to;
  
  if (!p)
    return;
  backout_move(s, p);
  moved_block = p->block;
  moved_to = p->to;
  ida_write_path(s, p->parent);
  printf("%d %d\n", moved_block, moved_to);
}

static int ida_length(struct protect *p) {
  int n = 0;

  while(p) {
    p = p->parent;
    n++;
  }
  return n;
}

static void ida_answer(struct state *g, struct protect *p) {
  g = copy_state(g);
  /* print stats */
  if (g->g_score != ida_length(p))
    abort();
  if (verbose > 0) {
    printf("%g secs\n", cpu_secs());
    printf("%d nodes\n",
	   stat_nodes);
    printf("%d heuristic, %d actual\n\n", start->h_score, g->g_score);
    ida_write_path(g, p);
  } else {
    printf("%g %d %d %d\n",
	   cpu_secs(), stat_nodes, 
	   start->h_score, g->g_score);
  }
}

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
  /* try tabletop moves first */
  for (i = 0; i < s->n_towers; i++)
    if (s->blocks[s->tower_tops[i]].on > -1) {
      local_protect_move(s, &p, i, -1);
      move(s, i, -1);
      if (depth_first(s, c, &p))
	return 1;
      backout_move(s, &p);
    }
  /* then try tower-to-tower */
  for (i = 0; i < s->n_towers; i++)
    for (j = 0; j < s->n_towers; j++)
      if (i != j) {
	local_protect_move(s, &p, i, j);
	move(s, i, j);
	if (depth_first(s, c, &p))
	  return 1;
	backout_move(s, &p);
      }
  return 0;
}

int ida_star(void) {
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
  

