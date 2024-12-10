#include "headers.h"

struct SignalInfo
{
    int number;
    const char *name;
};

struct SignalInfo signals[] = {
    {SIGHUP, "SIGHUP"},
    {SIGINT, "SIGINT"},
    {SIGQUIT, "SIGQUIT"},
    {SIGILL, "SIGILL"},
    {SIGTRAP, "SIGTRAP"},
    {SIGABRT, "SIGABRT"},
    {SIGBUS, "SIGBUS"},
    {SIGFPE, "SIGFPE"},
    {SIGKILL, "SIGKILL"},
    {SIGUSR1, "SIGUSR1"},
    {SIGSEGV, "SIGSEGV"},
    {SIGUSR2, "SIGUSR2"},
    {SIGPIPE, "SIGPIPE"},
    {SIGALRM, "SIGALRM"},
    {SIGTERM, "SIGTERM"},
    {SIGCHLD, "SIGCHLD"},
    {SIGCONT, "SIGCONT"},
    {SIGSTOP, "SIGSTOP"},
    {SIGTSTP, "SIGTSTP"},
    {SIGTTIN, "SIGTTIN"},
    {SIGTTOU, "SIGTTOU"},
    {SIGURG, "SIGURG"},
    {SIGXCPU, "SIGXCPU"},
    {SIGXFSZ, "SIGXFSZ"},
    {SIGVTALRM, "SIGVTALRM"},
    {SIGPROF, "SIGPROF"},
    {SIGWINCH, "SIGWINCH"},
    {SIGIO, "SIGIO"},
    {SIGPWR, "SIGPWR"},
    {SIGSYS, "SIGSYS"}
};

void handle_CtrlD()
{
    for (int i = 0; i < process_count; i++)
    {
        if (kill(processes[i].pid, 0) == 0)
        {
            kill(processes[i].pid, SIGKILL);
            int status;
            waitpid(processes[i].pid, &status, 0);

            if (WIFEXITED(status) || WIFSIGNALED(status))
            {
                continue;
            }
        }
    }
    printf("\nExiting shell by ^D.\n");
    exit(0);
}

void handle_CtrlC(int sig)
{
    if (current_fg > 0)
    {
        kill(current_fg, SIGINT);
        printf("\nAbrupt Exit due to signal SIGINT (2) from process with id = %d\n", current_fg);
        current_fg = -1;
        return;
    }
    return;
}

void handle_CtrlZ(int sig)
{
    if (current_fg > 0)
    {
        setpgid(current_fg, current_fg);
        kill(current_fg, SIGSTOP);
        addProcess(supremecommand, current_fg);
        printProcesses();
        printf("\nStopped process with id = %d by signal SIGSTP (20) and pushed to bg\n", current_fg);
        current_fg = -1;
        return;
    }
}

void sendsignal(int pid, int sig)
{
    if (kill(pid, sig) == -1)
    {
        if (errno == ESRCH)
        {
            printf(red "Error: No such process found\n" reset_color);
        }
        else
        {
            perror(red "Could not send signal" reset_color);
        }
    }
    else
    {
        printf("Sent %s (%d) to process with pid = %d\n", signals[sig-1].name, signals[sig-1].number, pid);
    }
    checkbgjobs();
    return;
}

void childsignal(int sig)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Background process with pid = %d exited normally \n", pid);
        }
        else if (WIFSIGNALED(status))
        {
            printf("Background process with pid = %d exited abnormally \n", pid);
        }
        removeProcess(pid);
    }
    checkbgjobs();
    return;
}
