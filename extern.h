extern void *malloc(unsigned);
extern void free(void *);
#ifdef __GNUC__
extern void perror(char *);
extern void printf(char *, ...);
extern void fprintf(FILE *, char *, ...);
#else
extern int strcmp(char *, char *);
#endif
extern int atoi(char *);
