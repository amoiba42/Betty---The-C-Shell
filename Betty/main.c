#include "headers.h"

int main()
{
    setup();
    char *input = malloc(sizeof(char) * MAX);
    while (1)
    {
        prompt();
        signal(SIGCHLD, handleProcess);
        longcommand[0] = '\0';
        total = 0;
        takeinput(input);
    }
    return 0;
}