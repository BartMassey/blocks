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
/* control */
int verbose = 0;
int ida = 0;
int rida = 0;
int node_limit = 0;

int main(int argc, char **argv) {
  argv++; argc--;
  while (argc > 0)
    if (argv[0][0] != '-')
      abort();
    else
      switch (argv[0][1]) {
      case 'v':
	if (argc < 2)
	  abort();
	verbose = atoi(argv[1]);
	argv += 2; argc -= 2;
	break;
      case 'l':
	if (argc < 2)
	  abort();
	node_limit = atoi(argv[1]);
	argv += 2; argc -= 2;
	break;
      case 'a':
	if (argc < 2)
	  abort();
	if (!strcmp(argv[1], "ida"))
	  ida = 1;
	else if (!strcmp(argv[1], "rida"))
	  rida = 1;
	else
	  abort();
	argv += 2; argc -= 2;
	break;
      }
  read_problem();
  score_state(start, 0);
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
  return a_star();
}
