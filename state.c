#include "blocks.h"

INLINE struct state *alloc_state(void) {
  struct state *new = malloc(sizeof(*new));

  if (!new) {
    perror("malloc (new)");
    exit(-1);
  }
  new->blocks = malloc(sizeof(new->blocks[0]) * n_blocks);
  if (!new->blocks) {
    perror("malloc (new->blocks)");
    exit(-1);
  }
  /*
   * We should be careful and only allocate n_towers
   * new tower_tops, but that might be one too few
   * for move(), which may want to start a new tower...
   * Punt for now.
   */
  new->tower_tops = malloc(sizeof(new->tower_tops[0]) * n_blocks);
  if (!new->tower_tops) {
    perror("malloc (new->tower_tops)");
    exit(-1);
  }
  new->tower_bottoms = malloc(sizeof(new->tower_bottoms[0]) * n_blocks);
  if (!new->tower_bottoms) {
    perror("malloc (new->tower_bottoms)");
    exit(-1);
  }
  return new;
}

void free_state(struct state *s) {
  free(s->blocks);
  free(s->tower_tops);
  free(s->tower_bottoms);
  free(s);
}

int is_same_state(struct state *s1, struct state *s2) {
  int i;

  /*
   * XXX This first check is redundant --
   * performed by same_state() macro.
   * We'll repeat for safety.
   */
  if (s1->hash != s2->hash)
    return 0;
  if (s1->n_towers != s2->n_towers)
    return 0;
  for (i = 0; i < n_blocks; i++)
    if (s1->blocks[i].on != s2->blocks[i].on)
      return 0;
  return 1;
}

INLINE struct state *clone_state(struct state *old) {
  struct state *new = alloc_state();
#ifdef NO_MEMCPY
  int i, n;
  
  n = n_blocks;
  for (i = 0; i < n; i++)
    new->blocks[i] = old->blocks[i];
  n = old->n_towers;
  for (i = 0; i < n; i++)
    new->tower_tops[i] = old->tower_tops[i];
  for (i = 0; i < n; i++)
    new->tower_bottoms[i] = old->tower_bottoms[i];
#else
  memcpy(new->blocks, old->blocks,
	 n_blocks * sizeof(old->blocks[0]));
  memcpy(new->tower_tops, old->tower_tops,
	 old->n_towers * sizeof(old->tower_tops[0]));
  memcpy(new->tower_bottoms, old->tower_bottoms,
	 old->n_towers * sizeof(old->tower_bottoms[0]));
#endif
  new->n_towers = old->n_towers;
  new->g_score = old->g_score;
  new->h_score = old->h_score;
  new->t_score = old->t_score;
  new->hash = old->hash;
  new->parent = old->parent;
  return new;
}
    
struct state *copy_state(struct state *parent) {
  struct state *new = clone_state(parent);
  
  new->parent = parent;
  return new;
}
