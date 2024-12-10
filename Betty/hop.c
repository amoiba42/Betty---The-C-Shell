#include "headers.h"

void changelocation(int argCount, char **args)
{
    int success = 1;
    if (argCount == 0)
    {
        strcpy(prevDir, currDir);
        chdir(og);
        getcwd(currDir, MAX);
    }
    else if (strcmp(args[0], "~") == 0)
    {
        strcpy(prevDir, currDir);
        chdir(og);
        getcwd(currDir, MAX);
    }
    else if (strcmp(args[0], "-") == 0)
    {
        char temp[MAX];
        strcpy(temp, currDir);
        chdir(prevDir);
        strcpy(prevDir, temp);
        getcwd(currDir, MAX);
    }
    else if (strcmp(args[0], ".") == 0)
    {
        strcpy(prevDir, currDir);
    }
    else if (strcmp(args[0], "..") == 0)
    {
        strcpy(prevDir, currDir);
        chdir("..");
        getcwd(currDir, MAX);
    }
    else
    {
        strcpy(prevDir, currDir);
        if (chdir(args[0]) != 0)
        {
            success = 0;
            printf(red "Error:Couldn't change directory\n" reset_color);
        }
        else
        {
            getcwd(currDir, MAX);
        }
    }
    if (success)
    {
        char mypath[MAX];
        getcwd(mypath, MAX);
        printf("%s\n", mypath);
    }
    return;
}