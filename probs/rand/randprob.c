#include <stdio.h>

extern long random( void );
extern int atoi( char * );
extern void printf( const char *, ... );


int map[200];
int n;
int hardmode;

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

void printstate(void) {
  int i, j, npiles = 0;

  shuffle(map, n);
  if (hardmode) {
    npiles = isqrt(n);
    if (npiles * npiles < n)
      npiles += randn(2);
    for (i = 0; i < npiles; i++)
      top[i] = -1;
    for (i = 0; i < n; i++) {
      j = i % npiles;
      on[map[i]] = top[j];
      top[j] = map[i];
    }
  } else {
    for (i = 0; i < n; i++) {
      j = (npiles > 0) ? randn(npiles + 1) - 1 : -1;
      if (j < 0) {
	on[map[i]] = -1;
	top[npiles++] = map[i];
      } else {
	on[map[i]] = top[j];
	top[j] = map[i];
      }
    }
  }
  for (i = 0; i < n; i++)
    printf("%d\n", on[i]);
}

int main(int argc, char **argv) {
  if (argc == 3) {
    if (strcmp(argv[1],"-h"))
      abort();
    hardmode = 1;
    argv++; argc--;
  }
  if (argc != 2)
    abort();
  n = atoi(argv[1]);
  if (n <= 0)
    abort();
  srandom(getpid());
  printf("%d\n\n", n);
  printstate();
  printf("\n");
  printstate();
  return 0;
}
