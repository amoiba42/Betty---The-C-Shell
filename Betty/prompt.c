#include "headers.h"
char longcommand[MAX];

void prompt()
{
    if (signal(SIGINT, handle_CtrlC) == SIG_ERR)
    {
        perror(red"Error setting SIGINT handler"reset_color);
        return;
    }
    if (signal(SIGTSTP, handle_CtrlZ) == SIG_ERR)
    {
        perror(red"Error setting SIGTSTP handler"reset_color);
        return;
    }
    char sysName[MAX] = "";
    if (gethostname(sysName, MAX) == -1)
    {
        printf(red "Error:Couldn't get hostname\n");
        return;
    }

    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL)
    {
        printf(red "Error:Couldn't get username\n"reset_color);
        return;
    }

    char command[256];
    printf(white "<");
    printf(yellow "%s", pw->pw_name);
    printf(white "@");
    printf(cyan "%s", sysName);
    printf(white ":");
    printf(purple "%s", getRelativePath(currDir));
    printf(light_red "%s", longcommand);
    printf(white ">" reset_color);
    fflush(stdout);
}