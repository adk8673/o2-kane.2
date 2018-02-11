#ifndef FUNCTION_LIBRARY_H
#define FUNCTION_LIBRARY_H

int checkNumber(const char *);
void writeError(const char *, const char *);
void createChildProcess(const char *, const char *);
int makeargv(const char *, const char *, char***);

#endif
