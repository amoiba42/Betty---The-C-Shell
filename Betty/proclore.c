#include "headers.h"

ProcessInfo processes[MAX_PROCESSES];
int process_count = 0;

void printDetails(int argCount, char **args)
{
    pid_t mypid;
    char status_file[MAX];
    char exe_path[MAX];
    char buffer[MAX];

    if (argCount == 0)
    {
        mypid = getpid();
    }
    else
    {
        mypid = atoi(args[0]);
    }
    printf(green "PID\t\t:" reset_color);
    printf("    %d\n", mypid);

    ///////////////////////////////////

    pid_t pgroup = getpgid(mypid);
    if (pgroup == -1)
    {

        perror(red "Error getting process details" reset_color);
        return;
    }
    printf(green "Process group\t:" reset_color);
    printf("    %d\n", pgroup);

    ///////////////////////////////////

    snprintf(status_file, sizeof(status_file), "/proc/%d/status", mypid);
    FILE *file = fopen(status_file, "r");
    if (file == NULL)
    {
        perror("Error opening status file");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (strncmp(buffer, "State:", 6) == 0)
        {
            printf(green "Process Status\t:" reset_color);
            printf("    %s", buffer + 7);
            break;
        }
    }
    ///////////////////////////////////

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (strncmp(buffer, "VmSize:", 7) == 0)
        {
            printf(green "Virtual Memory\t:" reset_color);
            printf("%s", buffer + 8);
            break;
        }
    }

    fclose(file);
    ///////////////////////////////////

    snprintf(status_file, sizeof(status_file), "/proc/%d/exe", mypid);
    ssize_t len = readlink(status_file, exe_path, sizeof(exe_path) - 1);
    if (len == -1)
    {
        perror("Error reading executable link");
    }
    else
    {
        exe_path[len] = '\0';
        printf(green "Executable Path\t:" reset_color);
        printf("    %s\n", exe_path);
    }
}

void handleProcess(int sig)
{
    childsignal(sig);
    for (int i = 0; i < process_count; i++)
    {
        childsignal(sig);
        for (int i = 0; i < process_count; i++)
        {
            char status_file[256];
            snprintf(status_file, sizeof(status_file), "/proc/%d/status", processes[i].pid);

            FILE *file = fopen(status_file, "r");
            if (file == NULL)
            {
                perror("Error opening status file");
                processes[i].state = 0; 
                continue;
            }

            char buffer[256];
            int found_state = 0;

            while (fgets(buffer, sizeof(buffer), file))
            {
                if (strncmp(buffer, "State:", 6) == 0)
                {
                    char state_info[256];
                    sscanf(buffer, "State: %s", state_info);

                    if (strstr(state_info, "R") != NULL)
                    {
                        processes[i].state = 1;
                    }
                    else
                    {
                        processes[i].state = 0; 
                    }
                    found_state = 1;
                    break;
                }
            }
            fclose(file);
        }
    }
}

int compare_processes(const void *a, const void *b)
{
    ProcessInfo *proc1 = (ProcessInfo *)a;
    ProcessInfo *proc2 = (ProcessInfo *)b;
    return strcmp(proc1->command, proc2->command);
}

void checkbgjobs()
{
    for (int i = 0; i < process_count; i++)
    {
        int status;
        pid_t result = waitpid(processes[i].pid, &status, WNOHANG | WUNTRACED);

        if (result > 0)
        {
            if (WIFEXITED(status) || WIFSIGNALED(status))
            {
                for (int j = i; j < process_count - 1; j++)
                {
                    processes[j] = processes[j + 1];
                }
                process_count--;
                i--;
            }
            else if (result == -1)
            {
                perror("Error in waitpid");
            }
        }
    }
}

void printProcesses()
{
    checkbgjobs();
    qsort(processes, process_count, sizeof(ProcessInfo), compare_processes);
    for (int i = 0; i < process_count; i++)
    {
        printf("%d : %s - %s \n", processes[i].pid, processes[i].command, processes[i].state ? "Running" : "Stopped");
    }
}

void addProcess(const char *command, pid_t pid)
{
    checkbgjobs();
    if (process_count < MAX_PROCESSES)
    {
        strncpy(processes[process_count].command, command, 255);
        processes[process_count].command[255] = '\0';
        processes[process_count].pid = pid;
        processes[process_count].state = 1;
        process_count++;
    }
}

void removeProcess(pid_t pid)
{
    for (int i = 0; i < process_count; i++)
    {
        if (processes[i].pid == pid)
        {
            for (int j = i; j < process_count - 1; j++)
            {
                processes[j] = processes[j + 1];
            }
            process_count--;
            break;
        }
    }
}