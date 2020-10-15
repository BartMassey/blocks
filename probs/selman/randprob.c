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

/* uggh! -- should be ok for this purpose, though */
static unsigned
isqrt( n )
    unsigned n;
{
    unsigned k = n / 2;

    while( k * k > n )
	k--;
    return k;
}

void printstate(int npiles) {
  int i, j;

  shuffle(map, n);
  for (i = 0; i < npiles; i++)
    top[i] = -1;
  for (i = 0; i < n; i++) {
    j = i % npiles;
    on[map[i]] = top[j];
    top[j] = map[i];
  }
  for (i = 0; i < n; i++)
    printf("%d\n", on[i]);
}

int main(int argc, char **argv) {
  int npiles;
  if (argc != 2)
    abort();
  n = atoi(argv[1]);
  if (n <= 0)
    abort();
  srandom(getpid());
  npiles = isqrt(n);
  printf("%d\n\n", n);
  printstate(npiles);
  printf("\n");
  printstate(npiles - 1);
  return 0;
}
