#include "blocks.h"

/*
 * Leftist Priority Queue
 * Bart Massey 5/95
 * C version 2/96
 *
 * Ala Tarjan.
 */

/* Create a new pq. */
struct statepq *statepq_new(void) {
  return 0;
}

/* Is the pq empty? */
int statepq_isempty(struct statepq *t) {
  return !t;
}

/* Extract the value component of a node. */
struct state *statepq_val(struct statepq *t) {
  if (!t)
    abort();
  return t->state;
}

static INLINE int score_lt(struct statepq *t1, struct statepq *t2) {
  struct state *s1 = t1->state;
  struct state *s2 = t2->state;
  
  if (s1->t_score < s2->t_score)
    return 1;
  if (s1->t_score == s2->t_score && s1->g_score > s2->g_score)
    return 1;
  return 0;
}

/* Merge the "right spines" of two pqs in order. */
static struct statepq *mesh(struct statepq *t1, struct statepq *t2) {
  struct statepq *tmp;
  
  if (!t1)
    return t2;
  if (!t2)
    return t1;
  if (score_lt(t2, t1)) {
    tmp = t1;
    t1 = t2;
    t2 = tmp;
  }
  t1->r = mesh(t1->r, t2);
  return t1;
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
  struct statepq *r;
  
  if (!t)
    return 0;
  rank_l = rank(t->l);
  r = fixmesh(t->r);
  rank_r = rank(r);
  if (rank_l < rank_r) {
    t->rank = rank_l + 1;
    t->r = t->l;
    t->l = r;
    return t;
  }
  t->rank = rank_r + 1;
  t->r = r;
  return t;
}

/* Merge the queues. */
static INLINE struct statepq *meld(struct statepq *t1, struct statepq *t2) {
  return fixmesh(mesh(t1, t2));
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
  return meld(t, t0);
}

/* The minimum element of the heap */
struct state *statepq_min(struct statepq *t) {
  if (!t)
    return 0;
  return t->state;
}

/*
 * Delete the minimum element and
 * merge its left and right children.
 * Caller must dispose of state itself.
 */
struct statepq *statepq_delmin(struct statepq *t) {
  struct statepq *t0;
  
  if (!t)
    abort();
  t0 = t;
  t = meld(t->l, t->r);
  free(t0);
  return t;
}

/* Merge the pqs. */
struct statepq *statepq_merge(struct statepq *t1, struct statepq *t2) {
  return meld(t1, t2);
}
