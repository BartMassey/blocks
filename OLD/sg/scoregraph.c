#include "blocks.h"

static int timestamp;
static t_block *labels = 0;
static int *onstack = 0;
static t_block *up_edges, *down_edges;

static void set_min(t_block *edges, t_block cur) {
    t_block e = edges[cur];
    t_block le = labels[e];
	
    if (le != -1 && le < labels[cur] && onstack[le])
        labels[cur] = le;
}

static void scc1(int i) {
    if (labels[i] != -1) {
	set_min(up_edges, i);
	set_min(down_edges, i);
	return;
    }
    onstack[i] = 1;
    labels[i] = timestamp++;
    if (up_edges[i] != -1) {
	scc1(up_edges[i]);
	set_min(up_edges, i);
    }
    if (down_edges[i] != -1) {
	scc1(down_edges[i]);
	set_min(down_edges, i);
    }
    onstack[i] = 0;
    return;
}

/*
 * strongly connected components
 * compute into labels
 */
static void scc(int n) {
    int i;

    if (!labels)
	labels = (t_block *) malloc(n * sizeof(labels[0]));
    for (i = 0; i < n; i++)
	labels[i] = -1;
    if (!onstack)
	onstack = malloc(n * sizeof(onstack[0]));
    for (i = 0; i < n; i++)
	onstack[i] = 0;
    timestamp = 0;
    for (i = 0; i < n; i++)
	scc1(i);
}

/*
 * The "up" edge of a block is the block which is on it,
 * or -1 if it is under a correctly positioned block or on top
 * of a stack.
 * The "down" edge of a block is the block which it will be
 * on in the goal state, or -1 if it is already in the right
 * place or will be on the table in the goal state.
 * The algorithm tries to break every cycle in the graph
 * by removing all topmost nodes that are part of the cycle.
 * It counts one for each cycle that it breaks.
 */

int score_graph(int n, t_block *up_edges0, t_block *down_edges0,
		int n_tops, t_block *tower_tops) {
    int i;

    up_edges = up_edges0;
    down_edges = down_edges0;
    scc(n);
    for(i = 0; i < n_tops; i++) {
	/*HERE*/
    }
}
