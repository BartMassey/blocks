#include "blocks.h"

/*
 * binary search tree
 * keyed on state hash value
 * Bart Massey 2/96
 */

struct stateht *stateht_new(void) {
  return 0;
}

static INLINE void insert_val(struct stateht *t, struct state *s) {
  struct stateht_val *v;
  
  v = malloc(sizeof(*v));
  if (!v) {
    perror("malloc (new_val)");
    exit(-1);
  }
  v->s = s;
  v->next = t->v;
  t->v = v;
}

struct stateht *stateht_insert(struct stateht *t, struct state *s) {
  struct stateht *t0, *t1;
  int h = s->hash;

  t0 = 0;
  t1 = t;
  while (t1) {
    t0 = t1;
    if (h < t1->h) {
      t1 = t1->l;
      continue;
    }
    if (h > t1->h) {
      t1 = t1->r;
      continue;
    }
    insert_val(t, s);
    return t;
  }
  t1 = malloc(sizeof(*t1));
  if (!t1) {
    perror("malloc (stateht_insert)");
    exit(-1);
  }
  t1->h = h;
  t1->l = 0;
  t1->r = 0;
  t1->v = 0;
  insert_val(t1, s);
  if (!t0)
    return t1;
  if (h < t0->h)
    t0->l = t1;
  else
    t0->r = t1;
  return t;
}

struct state *stateht_match(struct stateht *t, struct state *s) {
  struct stateht_val *v;
  int h = s->hash;

  while (t) {
    if (h < t->h) {
      t = t->l;
      continue;
    }
    if (h > t->h) {
      t = t->r;
      continue;
    }
    for (v = t->v; v; v = v->next)
      if (same_state(v->s, s))
	return v->s;
    return 0;
  }
  return 0;
}
