#ifndef __REVEAL_H__
#define __REVEAL_H__

int alphasort(const struct dirent **a, const struct dirent **b);

int checkFlags(char *arg, int *showhidden, int *showlong);
void printInfo(char *path, const char *filename, int showlong);
void listFiles(int argCount, char **args);

#endif