#include <sys/time.h>
#include <sys/resource.h>
#include "blocks.h"
extern int getrusage(int, struct rusage *);

double cpu_secs(void) {
  struct rusage ru;
  
  /* get user cpu */
  if (getrusage(RUSAGE_SELF, &ru) < 0) {
    perror("getrusage");
    exit(-1);
  }
  return ru.ru_utime.tv_sec + ru.ru_utime.tv_usec / 1000000.0;
}

int getint(void) {
  char buf[128];

  gets(buf);
  return atoi(buf);
}
