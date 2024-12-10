#ifndef __PROCLORE_H__
#define __PROCLORE_H__

void printDetails(int argCount, char **args);

int compare_processes(const void *a, const void *b);

void handleProcess(int sig);

void printProcesses();

void addProcess(const char *command, pid_t pid);

void removeProcess(pid_t pid);
void checkbgjobs();
#endif