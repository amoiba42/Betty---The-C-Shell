#include "headers.h"

int alphasort(const struct dirent **a, const struct dirent **b)
{
    return strcmp((*a)->d_name, (*b)->d_name);
}

int checkFlags(char *arg, int *showhidden, int *showlong)
{
    int valid_flags = 1;

    for (size_t i = 1; i < strlen(arg); i++)
    {
        if (arg[i] == 'a')
        {
            *showhidden = 1;
        }
        else if (arg[i] == 'l')
        {
            *showlong = 1;
        }
        else
        {
            valid_flags = 0; 
            break;
        }
    }
    if (!valid_flags)
    {
        printf(red "Error: Invalid flag\n Try rechecking your command\n" reset_color);
        return 1;
    }
    return 0;
}

void printInfo(char *path, const char *filename, int showlong)
{
    char fullPath[MAX];
    struct stat fileStat;
    struct tm *timeinfo;
    char buffer[80];
    if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0)
    {
        return;
    }
    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, filename);
    if (stat(fullPath, &fileStat) == 0)
    {
        if (showlong)
        {
            printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
            printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
            printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
            printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
            printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
            printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
            printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
            printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
            printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
            printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

            timeinfo = localtime(&fileStat.st_mtime);
            strftime(buffer, 80, "%b %d \t %H:%M:%S", timeinfo);
            printf("\t %s\t", buffer);
            printf("%ld bytes\t", fileStat.st_size);
        }

        if (S_ISDIR(fileStat.st_mode))
        {
            printf(blue "%s" reset_color, filename);
        }

        else if (S_IXUSR & fileStat.st_mode)
        {
            printf(green "%s" reset_color, filename);
        }

        else if (S_ISREG(fileStat.st_mode))
        {
            printf(white "%s" reset_color, filename);
        }

        else
        {
            printf(yellow "%s" reset_color, filename);
        }
        printf("\n");
    }
    else
    {
        perror(red "stat" reset_color);
    }
}

void listFiles(int argCount, char **args)
{
    char path[MAX];
    int showhidden = 0;
    int showlong = 0;

    for (int i = 0; i < argCount; i++)
    {
        if (strncmp(args[i], "-", 1) == 0)
        {
            if (checkFlags(args[i], &showhidden, &showlong) == 1)
            {
                return;
            }
        }
    }

    if (argCount == 0)
    {
        strcpy(path, currDir);
    }
    else if (strncmp(args[0], "..", 2) == 0)
    {
        strcpy(path, args[0]);
    }
    else if (strlen(args[0]) == 1 && args[0][0] == '-')
    {
        strcpy(path, prevDir);
    }
    else if (args[0][0] == '.' || (args[0][0] == '-' && (showhidden || showlong)))
    {
        strcpy(path, currDir);
    }
    else if (args[0][0] == '~')
    {
        strcpy(path, og);
    }
    else if (argCount > 0 && args[0] != NULL)
    {
        strcpy(path, args[0]);
    }
    else
    {
        printf(red "Error: Invalid arguments\n" reset_color);
    }

    struct stat fileStat;
    struct dirent **namelist;
    int n = scandir(path, &namelist, NULL, alphasort);
    int y = 0;

    if (n < 0)
    {
        perror(red "scandir" reset_color);
    }
    else
    {
        while (y < n - 1)
        {
            y++;
            if (showhidden == 0 && namelist[y]->d_name[0] == '.')
            {
                continue;
            }

            printInfo(path, namelist[y]->d_name, showlong);
            free(namelist[y]);
        }
        free(namelist);
        return;
    }
}