#include "blocks.h"

/* global state */
int n_blocks;
struct state *start, *goal;
/* statistics */
int stat_nodes = 1;
int stat_cached = 1;
int stat_open = 1;
int stat_max_open = 1;
int stat_max_g = 0;
int stat_min_h;
int stat_decision_nodes = -1;
/* control */
int verbose = 0;
int node_limit = 0;
int timer_expired = 0;
int fast_heuristic = 1;
int dumb_heuristic = 0;

int main(int argc, char **argv) {
  int ida = 0;
  int rida = 0;
  int ra = 0;

  argv++; argc--;
  while (argc > 0)
    if (argv[0][0] != '-') {
      fprintf(stderr, "blocks: - expected\n");
      exit(1);
    } else
      switch (argv[0][1]) {
      case 'v':
	if (argc < 2) {
	  fprintf(stderr, "blocks: -v: argument expected\n");
	  exit(1);
	}
	verbose = atoi(argv[1]);
	argv += 2; argc -= 2;
	break;
      case 'l':
	if (argc < 2) {
	  fprintf(stderr, "blocks: -l: argument expected\n");
	  exit(1);
	}
	node_limit = atoi(argv[1]);
	argv += 2; argc -= 2;
	break;
      case 't':
	if (argc < 2) {
	  fprintf(stderr, "blocks: -t: argument expected\n");
	  exit(1);
	}
	set_timer(atoi(argv[1]));
	argv += 2; argc -= 2;
	break;
      case 'a':
	if (argc < 2) {
	  fprintf(stderr, "blocks: -a: argument expected\n");
	  exit(1);
	}
	if (!strcmp(argv[1], "ida"))
	  ida = 1;
	else if (!strcmp(argv[1], "rida"))
	  rida = 1;
	else if (!strcmp(argv[1], "ra"))
	  ra = 1;
	else {
	  fprintf(stderr, "blocks: -a: unknown algorithm %s\n", argv[1]);
	  exit(1);
	}
	argv += 2; argc -= 2;
	break;
      case 'h':
	if (argc < 2) {
	  fprintf(stderr, "blocks: -h: argument expected\n");
	  exit(1);
	}
	if (!strcmp(argv[1], "slow"))
	  fast_heuristic = 0;
	else if(!strcmp(argv[1], "dumb"))
	  dumb_heuristic = 1;
	else {
	  fprintf(stderr, "blocks: -h: unknown heuristic %s\n", argv[1]);
	  exit(1);
	}
	argv += 2; argc -= 2;
	break;
      }
  read_problem();
  init_closure();
  fix_bottoms(start);
  if (dumb_heuristic)
    dumb_score_state(start);
  else if (fast_heuristic)
    score_state(start);
  else
    deluxe_score_state(start);
  start->g_score = 0;
  start->t_score = start->h_score;
  hash_state(start);
  start->parent = 0;
  start->moved_block = -1;
  start->moved_to = -1;
  goal->h_score = 0;   /* better be! */
  hash_state(goal);
  stat_min_h = start->h_score;
  if (ida)
    return ida_star();
  if (rida)
    return rida_star();
  if (ra)
    return ra_star();
  return a_star();
}
