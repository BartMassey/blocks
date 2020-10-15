#include <stdio.h>

extern long random( void );
extern int atoi( char * );
extern void printf( const char *, ... );


int map[200];
int n;

int randn(int n) {
  return (random() >> 3) % n;
}

void shuffle(int a[], int n) {
  int i, j, t;
  
  for (i = 0; i < n; i++)
    a[i] = i;
  for (i = 0; i < n - 1; i++) {
    j = randn(n - i) + i;
    t = a[i];
    a[i] = a[j];
    a[j] = t;
  }
}

int top[200];
int on[200];

void printstate(int n_piles) {
  int i, j;

  shuffle(map, n);
  for (i = 0; i < n_piles; i++)
    top[i] = -1;
  for (i = 0; i < n; i++) {
    j = i % n_piles;
    on[map[i]] = top[j];
    top[j] = map[i];
  }
  for (i = 0; i < n; i++)
    printf("%d\n", on[i]);
}

int main(int argc, char **argv) {
  int n_start, n_goal;
  
  if (argc != 4)
    abort();
  n = atoi(argv[1]);
  if (n <= 0)
    abort();
  n_start = atoi(argv[2]);
  if (n_start <= 0)
    abort();
  n_goal = atoi(argv[3]);
  if (n_goal <= 0)
    abort();
  srandom(getpid());
  printf("%d\n\n", n);
  printstate(n_start);
  printf("\n");
  printstate(n_goal);
  return 0;
}
