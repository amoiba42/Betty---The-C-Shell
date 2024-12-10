#include "headers.h"

void enter_raw_mode()
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void exit_raw_mode()
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag |= (ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int most_recent_pid()
{
    DIR *proc_dir;
    struct dirent *entry;
    pid_t max_pid = 0;
    proc_dir = opendir("/proc");

    if (proc_dir == NULL)
    {
        perror(red"Unable to open /proc directory"reset_color);
        exit(1);
    }

    while ((entry = readdir(proc_dir)) != NULL)
    {
        pid_t pid = atoi(entry->d_name);
        if (pid > max_pid)
        {
            max_pid = pid;
        }
    }
    closedir(proc_dir);
    return max_pid;
}

void printpresentpid(int delay)
{
    enter_raw_mode();
    while (1)
    {
        printf("%d\n", most_recent_pid());
        sleep(delay);
        char c;
        fd_set set;
        struct timeval timeout;

        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);

        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0)
        {
            if (read(STDIN_FILENO, &c, 1) > 0 && c == 'x')
            {
                break;
            }
        }
    }

    exit_raw_mode();
    return;
}