#include "blocks.h"

/*
 * Leftist Priority Queue
 * Bart Massey 5/95
 * C version 2/96, 7/96
 *
 * Ala Tarjan.
 */


static struct statepq *mesh(struct statepq *t1, struct statepq *t2);
static struct statepq *fixmesh(struct statepq *t);
/* Merge the queues. */
#define MELD(T1,T2) (fixmesh(mesh((T1),(T2))))

/* Create a new pq. */
struct statepq *statepq_new(void) {
  return 0;
}

/*
 * Perform the lazy deletions, and return the queue
 * with the first element undeleted
 */
static INLINE struct statepq *first(struct statepq *t) {
  struct statepq *t0;
  
  while (t && t->deleted) {
    t0 = t;
    t = MELD(t->l, t->r);
    free(t0);
  }
  return t;
}

static INLINE int score_lt(struct statepq *t1, struct statepq *t2) {
  struct state *s1;
  struct state *s2;
  
  if (!t1 || !t2)
    abort();
  s1 = t1->state;
  s2 = t2->state;
  if (s1->t_score < s2->t_score)
    return 1;
  if (s1->t_score == s2->t_score && s1->g_score > s2->g_score)
    return 1;
  return 0;
}

/* Merge the "right spines" of two pqs in order. */
static struct statepq *mesh(struct statepq *t1, struct statepq *t2) {
  t1 = first(t1);
  t2 = first(t2);
  if (!t1)
    return t2;
  if (!t2)
    return t1;
  if (score_lt(t1, t2)) {
    t1->r = mesh(t1->r, t2);
    return t1;
  }
  t2->r = mesh(t2->r, t1);
  return t2;
}

static INLINE int rank(struct statepq *t) {
  if (!t)
    return 0;
  return t->rank;
}

/*
 * Swap left and right children along the
 * right spine as necessary to restore
 * leftist property.
 */
static struct statepq *fixmesh(struct statepq *t) {
  int rank_l, rank_r;
  struct statepq *l, *r;
  
  t = first(t);
  if (!t)
    return 0;
  l = first(t->l);
  r = fixmesh(t->r);
  rank_l = rank(l);
  rank_r = rank(r);
  if (rank_l < rank_r) {
    t->rank = rank_l + 1;
    t->r = l;
    t->l = r;
    return t;
  }
  t->rank = rank_r + 1;
  t->l = l;
  t->r = r;
  return t;
}

/*
 * Insert into the queue by
 * making a new heap and merging.
 */
struct statepq *statepq_insert(struct state *s, struct statepq *t) {
  struct statepq *t0 = malloc(sizeof(*t0));

  if (!t0) {
    perror("malloc (statepq_insert)");
    exit(-1);
  }
  t0->state = s;
  t0->rank = 1;
  t0->l = 0;
  t0->r = 0;
  t0->deleted = 0;
  s->q = t0;
  return MELD(t, t0);
}

/* lazy deletion */
void statepq_delete(struct statepq *q) {
  q->deleted = 1;
  q->state->q = 0;
}

/*
 * Do any lazy deletions necessary,
 * delete the minimum element,
 * merge its left and right children,
 * save the state associated with the minimum element,
 * and return the altered pq.
 * Caller must dispose of state itself.
 */
struct statepq *statepq_delmin(struct statepq *t, struct state **sp) {
  struct statepq *t0;
  
  t = first(t);
  if (!t) {
    (*sp) = 0;
    return 0;
  }
  t0 = t;
  t = MELD(t->l, t->r);
  t0->state->q = 0;
  *sp = t0->state;
  free(t0);
  return t;
}

/* Merge the pqs. */
struct statepq *statepq_merge(struct statepq *t1, struct statepq *t2) {
  return MELD(t1, t2);
}
