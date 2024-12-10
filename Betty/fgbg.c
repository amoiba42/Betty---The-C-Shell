#include "headers.h"

void run_in_fg(char **args)
{
    int given_pid = atoi(args[0]);
    if (kill(given_pid, 0) == -1)
    {
        perror(red "No such process" reset_color);
        return;
    }
    else
    {
        pid_t shell_pgid = getpgrp();
        time_t start, end;
        time(&start);
        tcsetpgrp(STDIN_FILENO, given_pid);
        kill(given_pid, SIGCONT);
        int status;
        waitpid(given_pid, &status, WUNTRACED);
        time(&end);
        double elapsed = difftime(end, start);
        total += elapsed;
        if (total > 2.0)
        {
            char *pass = malloc(sizeof(char) * MAX);
            snprintf(pass, MAX + 200, " %s %d", supremecommand, (int)total);
            strcat(longcommand, pass);
            free(pass);
        }
        printf("\nProcess with pid %d exited with status %d\n", given_pid, WEXITSTATUS(status));
        signal(SIGTTOU, SIG_IGN);
        tcsetpgrp(STDIN_FILENO, shell_pgid);
        signal(SIGTTOU, SIG_DFL);
        removeProcess(given_pid);
    }
}

void run_in_bg(char **args)
{
    int given_pid = atoi(args[0]);
    if (kill(given_pid, 0) == -1)
    {
        perror(red "No such process" reset_color);
        return;
    }
    else
    {
        kill(given_pid, SIGCONT);
    }
}
