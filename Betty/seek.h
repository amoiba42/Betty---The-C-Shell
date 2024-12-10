#ifndef  __SEEK_H__
#define  __SEEK_H__

int findingdory(char *dory, char *reef, int dflag, int eflag, int fflag);
int verifyFlags(char *arg, int *dflag, int *eflag, int *fflag);
void seekout(int argCount, char **args);

#endif