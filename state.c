#include "blocks.h"

struct state *alloc_state(void) {
  struct state *new = malloc(sizeof(struct state));

  if (!new) {
    perror("malloc (new)");
    exit(-1);
  }
  new->blocks = malloc(sizeof(struct block) * n_blocks);
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
  new->tower_tops = malloc(sizeof(int) * n_blocks);
  if (!new->tower_tops) {
    perror("malloc (new->tower_tops)");
    exit(-1);
  }
  return new;
}

void free_state(struct state *s) {
  free(s->blocks);
  free(s->tower_tops);
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
  if (s1->n_towers != s2->n_towers || s1->h_score != s2->h_score)
    return 0;
  for (i = 0; i < n_blocks; i++)
    if (s1->blocks[i].on != s2->blocks[i].on)
      return 0;
  return 1;
}

struct state *copy_state(struct state *parent) {
  struct state *new = alloc_state();
  int i;
  
  for (i = 0; i < n_blocks; i++)
    new->blocks[i] = parent->blocks[i];
  new->n_towers = parent->n_towers;
  for (i = 0; i < parent->n_towers; i++)
    new->tower_tops[i] = parent->tower_tops[i];
  new->g_score = parent->g_score;
  new->h_score = parent->h_score;
  new->t_score = parent->t_score;
  new->hash = parent->hash;
  new->parent = parent;
  return new;
}
    
