
static INLINE void global_protect_move(struct state *s, struct protect *p, struct protect *parent) {
  p->h_score = s->h_score;
  p->g_score = s->g_score;
  p->t_score = s->t_score;
  p->parent = parent;
  p->n_towers = s->n_towers;
  p->hash = s->hash;
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
  if (p->from > -1) {
    p->last_bottom = s->tower_bottoms[p->t_from];
  } else {
    p->last_top = s->tower_tops[s->n_towers - 1];
    p->last_bottom = s->tower_bottoms[s->n_towers - 1];
  }
}

static INLINE void backout_move(struct state *s, struct protect *p) {
  s->n_towers = p->n_towers;
  s->tower_tops[p->t_from] = p->block;
  if (p->t_to > -1) {
    s->tower_tops[p->t_to] = p->to;
    if (s->tower_bottoms[p->t_to] == p->block)
      s->tower_bottoms[p->t_to] = p->to;
  }
  if (p->from > -1) {
    s->tower_bottoms[p->t_from] = p->last_bottom;
  } else {
    s->tower_tops[s->n_towers - 1] = p->last_top;
    s->tower_bottoms[s->n_towers - 1] = p->last_bottom;
  }
  s->blocks[p->block].on = p->from;
  s->h_score = p->h_score;
  s->g_score = p->g_score;
  s->t_score = p->t_score;
  s->hash = p->hash;
}

