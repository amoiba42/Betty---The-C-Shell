#include "headers.h"

#define MAX_HISTORY 15
char shellpath[MAX];
char lastcommand[MAX];

void addtoHistory(const char *command)
{
    if (strcmp(command, lastcommand) == 0)
    {
        return;
    }
    char history[MAX_HISTORY][MAX];
    int count = 0;

    // if (strstr(command, "log") != NULL)
    // {
    //     return;
    // }

    FILE *fp = fopen(historyfile, "r");
    if (fp != NULL)
    {
        while (fgets(history[count], MAX, fp) != NULL && count < MAX_HISTORY)
        {
            count++;
        }
        fclose(fp);
    }

    if (count == MAX_HISTORY)
    {
        for (int i = 0; i < MAX_HISTORY - 1; i++)
        {
            strcpy(history[i], history[i + 1]);
        }
        count--;
    }

    strncpy(history[count], command, MAX - 1);
    history[count][MAX - 1] = '\0';
    count++;

    fp = fopen(historyfile, "w");
    if (fp == NULL)
    {
        perror(red "Error in writing to history file" reset_color);
        return;
    }
    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%s", history[i]);
    }
    fclose(fp);
    strcpy(lastcommand, command);
}


void clearHistory()
{
    FILE *fp = fopen(historyfile, "w");
    if (fp == NULL)
    {
        perror(red"Error in clearing history"reset_color);
        return;
    }
    fclose(fp);
}

void myhistory(int argCount, char **args)
{
    if (argCount == 0)
    {
        FILE *fp = fopen(historyfile, "r");
        if (fp == NULL)
        {
            printf("No commands in history\n");
            return;
        }
        char *line = NULL;
        size_t len = 0;
        while (getline(&line, &len, fp) != -1)
        {
            printf("%s", line);
        }
        fclose(fp);
    }

    else if (argCount == 1 && strcmp(args[0], "purge") == 0)
    {
        clearHistory();
    }

    else if (argCount > 0 && strcmp(args[0], "execute") == 0)
    {
        char *command = malloc(sizeof(char) * MAX);
        char *function;

        FILE *fp = fopen(historyfile, "r");
        if (fp == NULL)
        {
            printf("No commands in history\n");
            return;
        }
        else
        {
            if (argCount < 2)
            {
                printf(red "No command number given\n" reset_color);
                return;
            }
            else
            {
                int commandNumber = atoi(args[1]);
                int count = 0;
                char history[MAX_HISTORY][MAX];

                while (fgets(history[count], MAX, fp) != NULL && count < MAX_HISTORY)
                {
                    count++;
                }

                if (commandNumber >= count)
                {
                    printf(red "Invalid command number\n" reset_color);
                    return;
                }
                else
                {
                    strcpy(command, history[count - commandNumber]);
                    handleMultiple(command);
                    return;
                }
                printf(red "Invalid command number\n" reset_color);
            }
        }
        fclose(fp);
    }
    else
    {
        printf(red "Invalid arguments\n" reset_color);
    }
}