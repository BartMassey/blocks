#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include "blocks.h"
extern int getrusage(int, struct rusage *);
extern int setitimer(int, struct itimerval *, struct itimerval *);

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

/*ARGSUSED*/
static void expired(int sig, int code, struct sigcontext *scp, void *addr) {
  timer_expired = 1;
}

void set_timer(int sec) {
  struct itimerval it, oit;

  it.it_interval.tv_sec = 0;
  it.it_interval.tv_usec = 0;
  it.it_value.tv_sec = sec;
  it.it_value.tv_usec = 0;
  if (signal(SIGVTALRM, expired) == SIG_ERR) {
    perror("signal");
    exit(-1);
  }
  if (setitimer(ITIMER_VIRTUAL, &it, &oit) == -1) {
    perror("setitimer");
    exit(-1);
  }
}
