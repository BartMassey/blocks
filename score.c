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

int *marks = 0;

static int delete_cycle(int i,
			int nb,	struct state *s,
			int *nnb, struct state **ns) {
  for (i = 0; i < 
}

static int score_cycles(int nb, struct state *s) {
  int nt = s->n_towers;
  struct state *ns = 0;
  int nnb;
  
  if (nb <= 0)
    return 0;
  for (i = 0; i < s->n_towers; i++)
    if (delete_cycle(i, nb, s, &nnb, &ns))
      break;
  if (!ns)
    return 0;
  return 1 + score_cycles(nnb, ns);
}
			 
void deluxe_score_state(struct state *s) {
  int nb = n_blocks;
  int nt = s->n_towers;
  int h, i, j;

  if (!marks) {
    marks = malloc(nb * sizeof(marks[0]));
    if (!marks) {
	perror("malloc: deluxe_score_state: marks");
	exit(-1);
    }
  }
  for (i = 0; i < nb; i++)
    marks[i] = 0;
  h = 0;
  for (i = 0; i < nt; i++) {
    t_block b;
    t_block bottom = s->tower_bottoms[i];
    
    for (b = s->tower_tops[i]; b != bottom; b = s->blocks[b].on)
      h++;
  }
  for (i = 0; i < nt; i++) {
    t_block b;
    t_block bottom = s->tower_bottoms[i];
    
    for (b = s->tower_tops[i]; b != bottom; b = s->blocks[b].on)
      h++;
  }
  s->h_score = h + score_cycles(nb, clone_state(s));
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
  int old_tt_score = 0;  /* XXX init to hush gcc */

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

