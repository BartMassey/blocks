#include "blocks.h"

static struct statepq *open;
static struct state *closed = 0;

static int duplicate_state(struct state *s, struct state *qs) {
  if (same_state(s, qs)) {
    if (s->g_score < qs->g_score) {
      /* copy relevant state for new path */
      qs->g_score = s->g_score;  /* XXX reprioritize */
      qs->t_score = s->t_score;
      qs->moved_block = s->moved_block;
      qs->moved_to = s->moved_to;
      qs->parent = s->parent;
    }
    free_state (s);
    return 1;
  }
  return 0;
}

/* XXX temporary hack */
static int duplicate_in_open(struct statepq *q, struct state *s) {
  return q && (duplicate_state(s, statepq_val(q))
	       || duplicate_in_open(q->l, s)
	       || duplicate_in_open(q->r, s));
}

static void push_state(struct state *s) {
  struct state *qp;

  /* deal with duplicates */
  for (qp = closed; qp; qp = qp->q_next)
    if (duplicate_state(s, qp))
      return;
  if (duplicate_in_open(open, s))
    return;
  /* update stats */
  stat_cached++;
  stat_open++;
  if (stat_open > stat_max_open)
    stat_max_open=stat_open;
  /* insert in appropriate place */
  open = statepq_insert(s, open);
}

static void a_star_write_path(struct state *sp) {
  if (!sp->parent)
    return;
  a_star_write_path(sp->parent);
  printf("%d %d\n", sp->moved_block, sp->moved_to);
}

static int a_star_length(struct state *g) {
  int n = 0;
  struct state *sp;

  for (sp = g; sp->parent; sp = sp->parent)
    n++;
  return n;
}


static void a_star_answer(struct state *g) {
  /* print stats */
  if (g->g_score != a_star_length(g))
    abort();
  if (verbose > 0) {
    printf("%g secs\n", cpu_secs());
    printf("%d nodes, %d cached, %d max open\n",
	   stat_nodes, stat_cached, stat_max_open);
    printf("%d heuristic, %d actual\n\n", start->h_score, g->g_score);
    a_star_write_path(g);
  } else {
    printf("%g %d %d %d %d %d\n",
	   cpu_secs(),
	   stat_nodes, stat_cached, stat_max_open,
	   start->h_score, g->g_score);
  }
}

int a_star(void) {
  struct state *s, *s0;
  int i, j;

  open = statepq_new();
  push_state(start);
  while ((s = statepq_min(open))) {
    if (same_state(s, goal)) {
      a_star_answer(s);
      return 0;
    }
    open = statepq_delmin(open);
    --stat_open;
    if (verbose > 1 && s->g_score > stat_max_g) {
      stat_max_g = s->g_score;
      printf("g max now %d\n", stat_max_g);
    }
    if (verbose > 1 && s->h_score < stat_min_h) {
      stat_min_h = s->h_score;
      printf("h min now %d\n", stat_min_h);
    }
    s->q_next = closed;
    closed = s;
    /* try tabletop moves first */
    for (i = 0; i < s->n_towers; i++)
      if (s->blocks[s->tower_tops[i]].on > -1) {
	s0 = copy_state(s);
	move(s0, i, -1);
	push_state(s0);
      }
    /* then try tower-to-tower */
    for (i = 0; i < s->n_towers; i++)
      for (j = 0; j < s->n_towers; j++)
	if (i != j) {
	  s0 = copy_state(s);
	  move(s0, i, j);
	  push_state(s0);
	}
  }
  printf("no solution!\n");
  return -1;
}
  
