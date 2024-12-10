#ifndef __COMMAND_H__
#define __COMMAND_H__

char *getRelativePath(char *pwd);
void handleMultiple(char *command);
void executeCommand(char *command, char **args, int argCount, int bg);
void tokenise(char *command, int bg);
void takeinput(char *input);
void initialize_aliases();


void pipes(char **subpipe, int bg);
int redirection(char **args, int argCount);
void restore(int original_stdin, int original_stdout);

#endif