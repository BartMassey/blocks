#include "blocks.h"

static char **aig;   /* above in goal */

void init_closure(void) {
    int n = n_blocks;
    int i, j, k;

    aig = (char **) malloc(n * sizeof(aig[0]));
    if (!aig) {
	perror("aig: malloc");
	exit(-1);
    }
    for (i = 0; i < n; i++) {
	aig[i] = (char *) malloc(n);
        if (!aig[i]) {
	    perror("aig: malloc");
	    exit(-1);
        }
	for (j = 0; j < n; j++)
	    aig[i][j] = (goal->blocks[i].on == j);
    }
    for (k = 0; k < n; k++)
	for (i = 0; i < n; i++)
	    if (aig[i][k])
		for (j = 0; j < n; j++)
		    if (aig[k][j] && !aig[i][j])
			aig[i][j] = 1;
    if (verbose > 7) {
        printf("aig:\n");
	printf("  ");
	for (i = 0; i < n_blocks; i++)
	    printf(" %02d", i);
	printf("\n");
	for (i = 0; i < n_blocks; i++) {
	    printf("%02d", i);
	    for (j = 0; j < n_blocks; j++)
		printf(" %01d ", aig[j][i]);
	    printf("\n");
	}
    }
}

void score_state(struct state *s) {
  int i, b, c;
  int h = 0;
  
  for (i = 0; i < s->n_towers; i++)
    for (b = s->tower_tops[i]; b != s->tower_bottoms[i]; b = s->blocks[b].on) {
      h++;
      for (c = b; c != -1; c = s->blocks[c].on)
	if (aig[b][c]) {
	  h++;
	  break;
	}
    }
  s->h_score = h;
}

#if 0
#define MARK_DEAD 1
#define MARK_STACKED 2
#define MARK_DONE 4
void deluxe_score_state(struct state *s) {
  int n = n_blocks;
  static int *marks = 0;

  if (!marks) {
    marks = malloc(n * sizeof(marks[0]));
    if (!marks) {
	perror("malloc: deluxe_score_state: marks");
	exit(-1);
    }
  }
  for (i = 0; i < n; i++)
    marks[i] = 0;
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
#else
void deluxe_score_state(struct state *s) {
  score_state(s);
}
#endif

void dumb_score_state(struct state *s) {
  int i;
  int t = n_blocks;

  for(i = 0; i < n_blocks; i++)
    t -= (s->blocks[i].on == goal->blocks[i].on);
  s->h_score = t;
}

int score_towertop(struct state *s, int t) {
  int b;
  int bt = s->tower_tops[t];
  
  if (bt == s->tower_bottoms[t])
    return 0;
  for (b = s->blocks[bt].on; b != -1; b = s->blocks[b].on)
    if (aig[bt][b])
      return 2;
  return 1;
}

void move(struct state *s, int t_from, int t_to) {
  int block, from, to;
  int old_tt_score;

  /* debugging, checks, and stats */
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
  /* get some invariants, check some assertions */
  block = s->tower_tops[t_from];
  from = s->blocks[block].on;
  if (t_from == -1)
    abort();
  if (from == -1 && t_to == -1)
    abort();
  if (t_to == t_from)
    abort();
  if (t_to > -1)
    to = s->tower_tops[t_to];
  else
    to = -1;
  /* more debugging, some scoring */
  if (verbose > 8) {
    printf("move block %d from tower %d onto block %d on tower %d\n",
	   block, t_from, to, t_to);
    write_picture(s);
  }
  if (dumb_heuristic)
    s->h_score += (from == goal->blocks[block].on);
  else if (fast_heuristic)
    old_tt_score = score_towertop(s, t_from);
  /* fix up source tower */
  if (from > -1) {
    if (s->tower_bottoms[t_from] == block)
      s->tower_bottoms[t_from] = from;
    s->tower_tops[t_from] = from;
  } else {
    --s->n_towers;
    s->tower_tops[t_from] = s->tower_tops[s->n_towers];
    s->tower_bottoms[t_from] = s->tower_bottoms[s->n_towers];
    if (t_to == s->n_towers)
      t_to = t_from;
  }
  /* fix up destination tower */
  if (t_to == -1) {
    t_to = s->n_towers++;
    s->tower_bottoms[t_to] = -1;
  }
  if (goal->blocks[block].on == to && s->tower_bottoms[t_to] == to)
    s->tower_bottoms[t_to] = block;
  s->tower_tops[t_to] = block;
  /* finish the move */
  s->hash += HASH(block, to) - HASH(block, s->blocks[block].on);
  s->blocks[block].on = to;
  /* more scoring, statistics */
  if (dumb_heuristic)
    s->h_score -= (to == goal->blocks[block].on);
  else if (fast_heuristic)
    s->h_score += score_towertop(s, t_to) - old_tt_score;
  else
    deluxe_score_state(s);
  s->g_score++;
  s->t_score = s->g_score + s->h_score;
  s->moved_block = block;
  s->moved_to = to;
  /* diagnostics */
  if (verbose > 8) {
    printf("\n");
    write_picture(s);
    printf("moved block %d from tower %d onto block %d on tower %d\n",
	   block, t_from, to, t_to);
  }
}

