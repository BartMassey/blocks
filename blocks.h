#include <stdio.h>
#include "extern.h"

#ifdef __GNUC__
#define INLINE inline
#else
#define INLINE /*inline*/
#endif

#define ON_CORRECT(S,B) ((S)->blocks[(B)].on == goal->blocks[(B)].on)

typedef signed char t_block;

struct block {
  t_block on;
};

#define HASH(I0,I1) ((((I0) ^ (I1)) << 16) | ((I0) << 8) | (I1))

struct state {
  struct block *blocks;
  t_block *tower_tops;
  t_block *tower_bottoms;
  int n_towers;
  int h_score, g_score, t_score;   /* heuristic, real, total */
  int hash;
  /* used only by a_star() */
  struct state *parent;
  int moved_block, moved_to;
  struct statepq *q;
};

/* global state */
extern int n_blocks;
extern struct state *start, *goal;
/* statistics */
extern int stat_nodes;
extern int stat_cached;
extern int stat_open;
extern int stat_max_open;
extern int stat_max_g;
extern int stat_min_h;
extern int stat_decision_nodes;
/* control */
extern int verbose;
extern int node_limit;
extern int timer_expired;
extern int fast_heuristic;

/* misc.c */
extern double cpu_secs(void);
extern int getint(void);
extern void set_timer(int);
/* state.c */
extern struct state *alloc_state(void);
extern void free_state(struct state *);
#define same_state(S1,S2) \
  (((S1)->hash == (S2)->hash) && is_same_state((S1),(S2)))
extern int is_same_state(struct state *, struct state *);
extern struct state *copy_state(struct state *);
/* score.c */
extern void init_closure(void);
extern int score_towertop(struct state *s, int t);
extern void score_state(struct state *);
extern void deluxe_score_state(struct state *);
extern void move(struct state *, int, int);
/* infra.c */
extern void hash_state(struct state *);
extern struct state *read_state(void);
extern void read_problem(void);
extern void fix_bottoms(struct state *);
/* astar.c */
extern int a_star(void);
/* rastar.c */
extern int ra_star(void);
/* idastar.c */
extern int ida_star(void);
/* ridastar.c */
extern int rida_star(void);
/* statepq.c */
/* state structure */
struct statepq {
  struct state *state;
  int rank;  
  struct statepq *l, *r;
  int deleted;
};
/* Extract the value component of a node. */
#define statepq_val(Q) ((Q)->state)
/* Is the pq empty? */
#define statepq_isempty(Q) (!(Q))
extern struct statepq *statepq_new(void);
extern struct statepq *statepq_insert(struct state *, struct statepq *);
extern struct statepq *statepq_delmin(struct statepq *, struct state **);
extern void statepq_delete(struct statepq *);
extern struct statepq *statepq_merge(struct statepq *, struct statepq *);
/* stateht.c */
struct stateht_val {
  struct state *s;
  struct stateht_val *next;
};
struct stateht {
  int h;
  struct stateht_val *v;
  struct stateht *l, *r;
};
extern struct stateht *stateht_new(void);
extern struct stateht *stateht_insert(struct stateht *, struct state *);
extern struct state **stateht_match(struct stateht *t, struct state *s);
/* debug.c */
extern void write_picture(struct state *);
