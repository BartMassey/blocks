extern void *malloc(unsigned);
extern void free(void *);
extern void perror(char *);
extern void printf(char *, ...);
extern void fprintf(FILE *, char *, ...);
#ifndef __GNUC__
extern int strcmp(char *, char *);
#endif
extern int atoi(char *);
