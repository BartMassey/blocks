#include "blocks.h"

static struct statepq *open;
static struct state *closed = 0;

static struct statepq *stack[100000];
static int duplicate_in_open(struct statepq *q, struct state *s) {
  int sp = 0;
  struct state *s0;

  stack[sp++] = q;
  while (sp) {
    if (sp >= 100000)
      abort();
    q = stack[--sp];
    while (q) {
      if (same_state(s, (s0 = statepq_val(q)))) {
	if (s->g_score < s0->g_score) {
	  statepq_delete(q);
	  free(s0);
	  statepq_insert(s, open);
	}
        return 1;
      }
      stack[sp++] = q->r;
      q = q->l;
    }
  }
  return 0;
}

static void push_state(struct state *s) {
  struct state *qp;

  /* deal with duplicates */
  for (qp = closed; qp; qp = qp->q_next)
    if (same_state(s, qp)) {
      free_state (s);
      return;
    }
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
  while (!statepq_isempty(open)) {
    open = statepq_delmin(open, &s);
    if (same_state(s, goal)) {
      a_star_answer(s);
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
  
