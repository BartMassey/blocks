#include <stdio.h>

extern long random( void );
extern int atoi( char * );
extern void printf( const char *, ... );


int map[200];
int top[200];
int n;
int npiles;

int randn(int n) {
  return (random() >> 3) % n;
}

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

void printgoal(void) {
  int i, j;
  
  for(i = 0; i < npiles; i++)
    top[i] = -1;
  for(i = 0; i < n; i++) {
    j = i % npiles;
    map[i] = top[j];
    top[j] = i;
  }
  for(i = 0; i < n; i++)
    printf("%d\n", map[i]);
}

void printstart(void) {
  int i, j, l;
  
  for(i = 0; i < npiles; i++)
    top[i] = -1;
  l = 0;
  for(i = 0; i < n; i++) {
    j = (i + l) % npiles;
    if (i % npiles == npiles - 1)
      l++;
    map[i] = top[j];
    top[j] = i;
  }
  for(i = 0; i < n; i++)
    printf("%d\n", map[i]);
}

int main(int argc, char **argv) {
  if (argc != 2)
    abort();
  n = atoi(argv[1]);
  if (n <= 0)
    abort();
  srandom(getpid());
  npiles = isqrt(n);
  if (npiles * npiles < n)
    npiles++;
  printf("%d\n\n", n);
  printstart();
  printf("\n");
  printgoal();
  return 0;
}
