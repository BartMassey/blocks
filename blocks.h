#include <stdio.h>
#include "extern.h"

#ifdef __GNUC__
#define INLINE inline
#else
#define INLINE /*inline*/
#endif

#define ON_CORRECT(S,B) ((S)->blocks[(B)].on == goal->blocks[(B)].on)

struct block {
  int on;
};

struct state {
  struct block *blocks;
  int *tower_tops;
  int n_towers;
  int h_score, g_score, t_score;   /* heuristic, real, total */
  /* used only by a_star() */
  struct state *parent;
  int moved_block, moved_to;
  struct state *q_next;  /* XXX will be superceded */
  int hash;
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
/* control */
extern int verbose;
extern int ida;
extern int node_limit;

/* misc.c */
extern double cpu_secs(void);
extern int getint(void);
/* state.c */
extern struct state *alloc_state(void);
extern void free_state(struct state *);
extern int same_state(struct state *, struct state *);
extern struct state *copy_state(struct state *);
/* infra.c */
extern void hash_state(struct state *);
extern int above_correct(struct state *, int);
extern void score_state(struct state *, int);
extern int score_towertop(struct state *s, int t);
extern void move(struct state *, int, int);
extern struct state *read_state(void);
extern void read_problem(void);
/* astar.c */
extern int a_star(void);
/* idastar.c */
extern int ida_star(void);
/* ridastar.c */
extern int rida_star(void);
/* statepq.c */
struct statepq {
  struct state *state;
  int rank;  
  struct statepq *l, *r;
};
extern struct statepq *statepq_new(void);
extern int statepq_isempty(struct statepq *);
extern struct state *statepq_val(struct statepq *);
extern struct statepq *statepq_insert(struct state *, struct statepq *);
extern struct state *statepq_min(struct statepq *);
extern struct statepq *statepq_delmin(struct statepq *);
extern struct statepq *statepq_merge(struct statepq *, struct statepq *);
