#include "headers.h"
pid_t current_fg;
double total;
char supremecommand[MAX];
typedef struct
{
    char *name;
    char *value;
} Alias;

Alias aliases[MAX];
int alias_count = 0;

void add_alias(const char *name, const char *value)
{
    if (alias_count >= MAX)
    {
        fprintf(stderr, "Alias limit reached\n");
        return;
    }

    aliases[alias_count].name = strdup(name);
    aliases[alias_count].value = strdup(value);
    alias_count++;
}

void initialize_aliases()
{
    FILE *file = fopen(".myshrc", "r");
    if (file == NULL)
    {
        perror(red "Unable to open .myshrc" reset_color);
        return;
    }

    char line[MAX];
    while (fgets(line, sizeof(line), file))
    {
        char *delimiter = strchr(line, '=');
        if (delimiter != NULL)
        {
            *delimiter = '\0';
            char *name = line;
            char *value = delimiter + 1;

            size_t len = strlen(value);
            if (len > 0 && value[len - 1] == '\n')
            {
                value[len - 1] = '\0';
            }

            add_alias(name, value);
        }
    }
    fclose(file);
}

char *expand_aliases(const char *input)
{
    char *new_input = strdup(input);
    if (new_input == NULL)
    {
        perror(red "Memory allocation error" reset_color);
        return NULL;
    }

    size_t len = strlen(new_input);
    if (len > 0 && (new_input[len - 1] == '\n' || new_input[len - 1] == '\r'))
    {
        new_input[len - 1] = '\0';
    }

    char *expanded = NULL;
    for (int i = 0; i < alias_count; i++)
    {
        size_t alias_len = strlen(aliases[i].name);
        if (strncmp(new_input, aliases[i].name, alias_len) == 0 &&
            (new_input[alias_len] == '\0' || new_input[alias_len] == ' '))
        {

            size_t new_len = strlen(aliases[i].value) + (strlen(new_input) - alias_len) + 1;
            expanded = malloc(new_len);
            if (expanded == NULL)
            {
                perror(red "Memory allocation error" reset_color);
                free(new_input);
                return NULL;
            }

            strcpy(expanded, aliases[i].value);
            strcat(expanded, new_input + alias_len);
            free(new_input);
            return expanded;
        }
    }

    free(new_input);
    return strdup(input);
}

int redirection(char **args, int argCount)
{
    int flag = 1;
    int value_to_return = argCount;
    int in = 0, app = 0, out = 0;
    char *inputFile = NULL;
    char *outputFile = NULL;

    for (int i = 0; i < argCount; i++)
    {
        if (strcmp(args[i], "<") == 0)
        {
            in = 1;
            inputFile = strdup(args[i + 1]);
            args[i] = NULL;
            value_to_return = i;
        }
        else if (strcmp(args[i], ">>") == 0)
        {
            app = 1;
            outputFile = strdup(args[i + 1]);
            args[i] = NULL;
            value_to_return = i; // value_to_return = i;
        }
        else if (strcmp(args[i], ">") == 0)
        {
            out = 1;
            outputFile = strdup(args[i + 1]);
            args[i] = NULL;
            value_to_return = i;
        }
    }

    if (in == 1)
    {
        int fd = open(inputFile, O_RDONLY, 0644);
        char file_contents[MAX];

        if (fd < 0)
        {
            return value_to_return;
        }

        dup2(fd, STDIN_FILENO);

        clearerr(stdin);
        close(fd);
    }

    if (out == 1 || app == 1)
    {
        int flags = O_WRONLY | O_CREAT;
        if (app == 1)
        {
            flags |= O_APPEND;
        }
        else if (out == 1)
        {
            flags |= O_TRUNC;
        }
        int fd2 = open(outputFile, flags, 0644);
        if (fd2 < 0)
        {
            return value_to_return;
        }
        dup2(fd2, STDOUT_FILENO);
        close(fd2);
    }
    free(inputFile);
    free(outputFile);
    return value_to_return;
}

void restore(int original_stdin, int original_stdout)
{
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
    return;
}

void executeCommand(char *command, char **args, int argCount, int bg)
{
    char *expanded_command = expand_aliases(command);
    strcpy(command, expanded_command);

    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    strcpy(supremecommand, command);
    int flag = 1;
    if (!bg)
    {

        if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0)
        {
            flag = 0;
            printf(yellow "Thanks for using Betty\n");
            exit(0);
        }
        else if (strcmp(command, "pwd") == 0)
        {
            char *relpath = getRelativePath(currDir);
            printf("%s\n", relpath);
        }
        else if (strcmp(command, "hop") == 0)
        {
            flag = 0;
            changelocation(argCount, args);
        }
        else if (strcmp(command, "reveal") == 0)
        {
            listFiles(argCount, args);
        }
        else if (strcmp(command, "log") == 0)
        {
            myhistory(argCount, args);
        }
        else if (strcmp(command, "seek") == 0)
        {
            seekout(argCount, args);
        }
        else if (strcmp(command, "proclore") == 0)
        {
            printDetails(argCount, args);
        }
        else if (strcmp(command, "activities") == 0)
        {
            printProcesses();
        }
        else if (strcmp(command, "ping") == 0)
        {
            int pid = atoi(args[0]);
            int signal = atoi(args[1]) % 32;
            sendsignal(pid, signal);
        }
        else if (strcmp(command, "neonate") == 0)
        {
            if (argCount == 2)
            {
                if (strcmp(args[0], "-n") == 0)
                {
                    int delay = atoi(args[1]);
                    printpresentpid(delay);
                }
                else
                {
                    printf(red "Invalid command\n" reset_color);
                }
            }
            else
            {
                printf(red "Invalid command\n" reset_color);
            }
        }
        else if (strcmp(command, "fg") == 0)
        {
            if (argCount != 1)
            {
                printf(red "Invalid command\n" reset_color);
                return;
            }
            else
            {
                run_in_fg(args);
            }
        }
        else if (strcmp(command, "bg") == 0)
        {
            if (argCount != 1)
            {
                printf(red "Invalid command\n" reset_color);
                return;
            }
            else
            {
                run_in_bg(args);
            }
        }
        else if (strcmp(command, "iMan") == 0)
        {
            if (argCount != 1)
            {
                printf(red "Invalid command\n" reset_color);
                return;
            }
            else
            {
                showpages(args);
            }
        }
        else
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                perror(red "Fork failed" reset_color);
                return;
            }
            else if (pid == 0)
            {
                int trim_command = 0;
                trim_command = redirection(args, argCount);
                char *argos[MAX];
                argos[0] = command;
                if (argCount != 0)
                {
                    for (int i = 0; i < trim_command; i++)
                    {
                        argos[i + 1] = args[i];
                    }
                }
                argos[trim_command + 1] = NULL;
                // printf("entered this\n");
                setpgid(0, 0);
                execvp(command, argos);
                printf(red "Error: '%s' is not a valid command \n" reset_color, command);
                exit(1);
            }
            else
            {
                current_fg = pid;
                time_t start, end;
                time(&start);
                waitpid(pid, NULL, WUNTRACED);
                restore(original_stdin, original_stdout);
                time(&end);
                double elapsed = difftime(end, start);
                total += elapsed;
                if (total > 2.0)
                {
                    char *pass = malloc(sizeof(char) * MAX);
                    snprintf(pass, MAX, " %s %d", command, (int)total);
                    strcat(longcommand, pass);
                    free(pass);
                }
                return;
            }
        }
    }
    else
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror(red "Fork failed" reset_color);
            return;
        }
        else if (pid == 0)
        {
            int trim_command = redirection(args, argCount);
            char *argos[MAX];
            if (argCount != 0)
            {
                argos[0] = command;
                for (int i = 0; i < trim_command; i++)
                {
                    argos[i + 1] = args[i];
                }
                argos[trim_command + 1] = NULL;
            }
            setpgid(0, 0);
            // printf("entered that\n");
            execvp(command, argos);
            restore(original_stdin, original_stdout);
            printf("Background process completed successfully\n");
            exit(1);
        }
        else
        {
            printf("PID : %d\n", pid);
            addProcess(command, pid);
            printf("--\n");
            return;
        }
    }

    if (flag == 1)
    {
        printf("--\n");
    }
    return;
}

void tokenise(char *command, int bg)
{
    char *lite;
    char spaceDelimiters[5] = " \n\t\0";
    char *token = strtok_r(command, spaceDelimiters, &lite);
    if (token == NULL)
    {
        return;
    }

    int argCount = 0;
    char *args[MAX];
    char *function = malloc(sizeof(char) * MAX);
    strcpy(function, token);
    function[strlen(token)] = '\0';

    while ((token = strtok_r(NULL, spaceDelimiters, &lite)) != NULL)
    {
        args[argCount] = malloc(strlen(token) + 1);
        strcpy(args[argCount], token);
        argCount = argCount + 1;
    }
    executeCommand(function, args, argCount, bg);
    for (int i = 0; i < argCount; i++)
    {
        free(args[i]);
    }

    free(function);
}

void pipes(char **subpipe, int bg)
{
    int i = 0, in_fd = 0, fd[2];
    pid_t pid;
    int save_in, save_out;

    pid_t pids[MAX];
    int numPids = 0;

    save_in = dup(STDIN_FILENO);
    save_out = dup(STDOUT_FILENO);

    while (subpipe[i] != NULL)
    {
        if (subpipe[i + 1] != NULL)
        {
            if (pipe(fd) == -1)
            {
                perror("Pipe failed");
                exit(1);
            }
        }

        pid = fork();
        if (pid < 0)
        {
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0)
        {
            if (in_fd != 0)
            {
                if (dup2(in_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2 failed for stdin");
                    exit(1);
                }
                close(in_fd);
            }

            if (subpipe[i + 1] != NULL)
            {
                if (dup2(fd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 failed for stdout");
                    exit(1);
                }
                close(fd[1]);
            }

            close(fd[0]);
            tokenise(subpipe[i], bg);
            exit(0);
        }
        else
        {
            pids[numPids++] = pid;

            if (subpipe[i + 1] == NULL)
            {
                wait(NULL);
            }

            close(fd[1]);
            in_fd = fd[0];
            i++;
        }
    }

    dup2(save_in, STDIN_FILENO);
    close(save_in);
    dup2(save_out, STDOUT_FILENO);
    close(save_out);

    for (int j = 0; j < numPids; j++)
    {
        waitpid(pids[j], NULL, 0);
    }
    return;
}

int isValidPipe(char *command)
{
    int len = strlen(command);

    if (len == 0 || command[0] == '|' || command[len - 1] == '|')
    {
        return 0;
    }

    for (int i = 1; i < len; i++)
    {
        if (command[i] == '|' && command[i - 1] == '|')
        {
            return 0;
        }
    }

    return 1;
}

void breaktopipe(char *command, int bg)
{

    if (!isValidPipe(command))
    {
        printf("Invalid pipe usage.\n");
        return;
    }

    char *subpipes[MAX];
    int subpipeCount = 0;
    if (strstr(command, "|") == NULL)
    {
        tokenise(command, bg);
    }
    else
    {
        char *token = strtok(command, "|");
        while (token != NULL)
        {
            subpipes[subpipeCount++] = token;
            token = strtok(NULL, "|");
        }
        pipes(subpipes, bg);
    }
}

void handleMultiple(char *input)
{
    char *lite;
    char *token = strtok_r(input, ";", &lite);

    while (token != NULL)
    {
        char *lite2 = malloc(sizeof(char) * MAX);
        strcpy(lite2, token);
        char *commandPart = strtok_r(lite2, "&", &lite2);

        if (strcmp(token, commandPart) == 0)
        {
            breaktopipe(commandPart, 0);
        }
        else
        {
            while (commandPart != NULL)
            {
                char *nextPart = strtok_r(NULL, "&", &lite2);

                if (nextPart == NULL && commandPart[strlen(commandPart) - 1] != '&')
                {
                    breaktopipe(commandPart, 0);
                    break;
                }
                else if (nextPart == NULL && commandPart[strlen(commandPart) - 1] == '&')
                {
                    breaktopipe(commandPart, 1);
                    break;
                }
                else
                {
                    breaktopipe(commandPart, 1);
                }
                commandPart = nextPart;
            }
        }
        token = strtok_r(NULL, ";", &lite);
    }
    return;
}

void takeinput(char *input)
{
    char ch;
    if (fgets(input, MAX, stdin) == NULL)
    {
        handle_CtrlD();
    }
    else
    {
        addtoHistory(input);
        handleMultiple(input);
    }
    return;
}
