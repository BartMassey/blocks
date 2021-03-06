#include "blocks.h"

static INLINE int hash(int i0, int i1) {
  return ((i0 ^ i1) << 16) | (i0 << 8) | i1;
}

void hash_state(struct state *s) {
  int h = 0;
  int i;
  
  for (i = 0; i < n_blocks; i++)
    h += hash(i, s->blocks[i].on);
  s->hash = h;
}

/*
 * Returns true only if b is above some block bmatch
 * in both s and goal .
 * i.e., it would be permissible for this function to
 * return false where b is above a correct block
 * However, the current version is both accurate
 * and quadratic -- uggh.
 */
int above_correct(struct state *s, int b) {
  int gb, sb;
  
  for (gb = goal->blocks[b].on; gb > -1; gb = goal->blocks[gb].on)
    for (sb = s->blocks[b].on; sb > -1; sb = s->blocks[sb].on)
      if (gb == sb)
	return 1;
  return 0;
}

void score_state(struct state *s) {
  int h, i, btop, badb, b;
  
  h = 0;
  for (i = 0; i < s->n_towers; i++) {
    btop = s->tower_tops[i];
    badb = -1;
    for (b = btop; b > -1; b = s->blocks[b].on)
      if (!ON_CORRECT(s, b))
	badb = b;
    if (badb == -1)
      continue;
    for (b = btop; b != s->blocks[badb].on; b = s->blocks[b].on)
      h += 1 + above_correct(s, b);
  }
  s->h_score = h;
}

void deluxe_score_state(struct state *s) {
  static int hasgraph = 0;
  static char *up_marks;
  static t_block *up_edges, *down_edges, *tower_tops;
  int n_tops;
  int n = n_blocks;
  int h, i, j;

  /* initialize */
  if (!hasgraph) {
    up_edges = malloc(n * sizeof(up_edges[0]));
    down_edges = malloc(n * sizeof(down_edges[0]));
  }
  /* Add all possible up and down edges. */
  for (i = 0; i < n; i++)
    up_edges[i] = -1;
  for (i = 0; i < n; i++)
    if (s->blocks[i].on != -1)
      up_edges[s->blocks[i].on] = s->blocks[i];
  for (i = 0; i < n; i++)
      down_edges[goal->blocks[i]] = goal->blocks[i].on;
  /*
   * Delete bogus up and down edges.
   * Count must-move blocks.
   */
  h = 0;
  for (i = 0; i < s->n_towers; i++) {
    t_block b, bb;
    t_block badb = -1;
    
    for (b = s->tower_tops[i]; b > -1; b = s->blocks[b].on)
      if (!ON_CORRECT(s, b))
	badb = b;
    if (badb == -1)
      continue;
    bb = s->blocks[badb].on;
    for (b = s->tower_tops[i]; b != bb; b = s->blocks[b].on)
      h++;
    while (bb > -1) {
      down_edges[bb] = -1;
      b = s->blocks[bb].on;
      if (b > -1)
        up_edges[b] = -1;
      bb = b;
    }
  }
  /* computer tower_tops[] hint */
  for (i = 0; i < s->n_towers; i++) {
    t_block b = s->tower_tops[i];
    t_block bb = s->blocks[b].on;
    
    if (bb > -1 && up_edges[bb])
      tower_tops[n_tops++] = b;
  }
  /* set the score */
  s->h_score = h + score_graph(n, up_edges, down_edges, n_tops, tower_tops);
}

int score_towertop(struct state *s, int t) {
  int btop, b;
  
  btop = s->tower_tops[t];
  for (b = btop; b > -1; b = s->blocks[b].on)
    if (!ON_CORRECT(s, b))
      break;
  if (b == -1)
    return 0;
  return 1 + above_correct(s, btop);
}

void move(struct state *s, int t_from, int t_to) {
  int block, from, to;
  int old_tt_score, new_tt_score;

  stat_nodes++;
  if (node_limit && stat_nodes > node_limit) {
    if (verbose > 0)
      printf("Node limit %d exceeded after %g secs\n", node_limit, cpu_secs());
    else
      printf("%g %d\n", cpu_secs(), -1);
    exit(-1);
  }
  if (timer_expired) {
    if (verbose > 0)
      printf("Time limit exceeded after %g secs\n", cpu_secs());
    else
      printf("%g %d\n", cpu_secs(), -1);
    exit(-1);
  }
  /* get some invariants */
  if (t_from == -1)
    abort();
  block = s->tower_tops[t_from];
  from = s->blocks[block].on;
  if (from == -1 && t_to == -1)
    abort();
  to = -1;
  if (t_to > -1)
    to = s->tower_tops[t_to];
  if (fast_heuristic)
    old_tt_score = score_towertop(s, t_from);
  /* fix up destination */
  s->hash += hash(block, to) - hash(block, s->blocks[block].on);
  s->blocks[block].on = to;
  if (t_to > -1) {
    s->tower_tops[t_to] = block;
    if (fast_heuristic)
      new_tt_score = score_towertop(s, t_to);
  } else {
    s->tower_tops[s->n_towers++] = block;
    if (fast_heuristic)
      new_tt_score = score_towertop(s, s->n_towers - 1);
  }
  /* fix up source */
  if (from > -1)
    s->tower_tops[t_from] = from;
  else
    s->tower_tops[t_from] = s->tower_tops[--s->n_towers];
  /* finish the move */
  if (fast_heuristic)
    s->h_score += new_tt_score - old_tt_score;
  else
    deluxe_score_state(s);
  s->g_score++;
  s->t_score = s->g_score + s->h_score;
  s->moved_block = block;
  s->moved_to = to;
}

struct state *read_state(void) {
  struct state *new = alloc_state();
  int on, i, j;

  /* read the blocks in */
  for (i = 0; i < n_blocks; i++) {
    on = getint();
    if (on < -1 || on >= n_blocks)
      abort();
    new->blocks[i].on = on;
  }
  /* calculate the tower tops -- quadratic */
  new->n_towers = 0;
  for (i = 0; i < n_blocks; i++) {
    for (j = 0; j < n_blocks; j++)
      if (new->blocks[j].on == i)
	break;
    if (j >= n_blocks)
      new->tower_tops[new->n_towers++] = i;
  }
  /* rest will be filled in by caller */
  return new;
}

void read_problem(void) {
  /* get number of blocks */
  n_blocks = getint();
  if (getchar() != '\n')
    abort();
  start = read_state();
  if (getchar() != '\n')
    abort();
  goal = read_state();
  if (getchar() != EOF)
    abort();
}

