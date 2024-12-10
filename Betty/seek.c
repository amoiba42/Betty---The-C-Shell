#include "headers.h"

int findingnemo(char *nemo, char *reef, int dflag, int eflag, int fflag, char *found)
{
    struct dirent **contents;
    int count = scandir(reef, &contents, NULL, alphasort);
    if (count < 0)
    {
        return 0;
    }

    int found_count = 0;

    if (strstr(reef, nemo) != NULL)
    {
        struct stat stupid;
        if (stat(reef, &stupid) == 0)
        {
            if (S_ISDIR(stupid.st_mode) && !fflag)
            {
                printf(blue ".%s\n" reset_color, getRelativePath(reef));
            }
            else if ((S_ISDIR(stupid.st_mode) == 0) && !dflag)
            {
                printf(green ".%s\n" reset_color, getRelativePath(reef));
            }
            strcpy(found, reef);
            found_count++;
        }
    }

    for (int i = 0; i < count; i++)
    {
        char *name = contents[i]->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        {
            continue;
        }

        char path[MAX];
        snprintf(path, MAX, "%s/%s", reef, name);
        // printf("path is %s\n", path);
        struct stat st;

        if (access(path, R_OK) == -1)
        {
            free(contents[i]);
            continue;
        }

        if (stat(path, &st) != 0)
        {
            free(contents[i]);
            continue;
        }

        if (!fflag && !dflag && strncmp(name, nemo, strlen(nemo)) == 0)
        {
            found_count++;
            if (S_ISDIR(st.st_mode) && !fflag)
            {
                printf(blue ".%s\n" reset_color, getRelativePath(path));
                int subcount = findingnemo(nemo, path, dflag, eflag, fflag, found);
                found_count += subcount;
            }
            else if ((S_ISDIR(st.st_mode) == 0) && !dflag)
            {
                printf(green ".%s\n" reset_color, getRelativePath(path));
            }
            strcpy(found, path);
        }

        else if (S_ISDIR(st.st_mode) != 0)
        {
            if (strncmp(name, nemo, strlen(nemo)) == 0 && !fflag)
            {
                printf(blue ".%s\n" reset_color, getRelativePath(path));
                found_count++;
                strcpy(found, path);
            }
            int subcount = findingnemo(nemo, path, dflag, eflag, fflag, found);
            found_count += subcount;
        }

        else if (strncmp(name, nemo, strlen(nemo)) == 0 && (S_ISDIR(st.st_mode) == 0))
        {
            printf(green ".%s\n" reset_color, getRelativePath(path));
            found_count++;
            int subcount = findingnemo(nemo, path, dflag, eflag, fflag, found);
            strcpy(found, path);
            found_count += subcount;
        }
        free(contents[i]);
    }

    free(contents);
    return found_count;
}

int verifyFlags(char *arg, int *dflag, int *eflag, int *fflag)
{
    int valid_flags = 1;
    for (size_t i = 1; i < strlen(arg); i++)
    {
        if (arg[i] == 'd')
        {
            *dflag = 1;
        }
        else if (arg[i] == 'e')
        {
            *eflag = 1;
        }
        else if (arg[i] == 'f')
        {
            *fflag = 1;
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

void seekout(int argCount, char **args)
{
    int dflag = 0;
    int eflag = 0;
    int fflag = 0;
    int bro = 0;
    char *nemo = malloc(sizeof(char) * MAX);
    char *reef = malloc(sizeof(char) * MAX);

    for (int i = 0; i < argCount; i++)
    {
        if (strncmp(args[i], "-", 1) == 0)
        {
            if (verifyFlags(args[i], &dflag, &eflag, &fflag) == 1)
            {
                return;
            }
        }
        else
        {
            if (i < argCount && bro == 0)
            {
                bro = 1;

                strcpy(nemo, args[i]);
            }
            else if (i < argCount && bro != 0)
            {
                strcpy(reef, args[i]);
            }
            else if (i >= argCount)
            {
                printf(red "Error : Missing Arguements\n" reset_color);
                return;
            }
            else
            {
                printf(red "Error : Invalid Arguements\n" reset_color);
                return;
            }
            if (strlen(reef) == 0)
            {
                getcwd(reef, MAX);
            }
            /////////////////////////////////
            if (dflag == 1 && fflag == 1)
            {
                printf(red "Error : Invalid Arguements\n" reset_color);
                return;
            }
            char *found = malloc(sizeof(char) * MAX);
            int count = findingnemo(nemo, reef, dflag, eflag, fflag, found);

            // if (count == 0)
            // {
            //     printf(yellow "No matches found\n" reset_color);
            //     return;
            // }
            /////////////////////////////////
            if (eflag == 1 && count == 1)
            {

                struct stat st;
                if (stat(found, &st) == 0)
                {
                    if (S_ISDIR(st.st_mode) && !fflag)
                    {
                        if ((st.st_mode & S_IXUSR) == 0)
                        {
                            printf("Missing permissions for task!");
                            return;
                        }
                        else
                        {
                            strcpy(prevDir, currDir);
                            chdir(found);
                            strcpy(currDir, found);
                        }
                    }
                    else if (!dflag)
                    {
                        FILE *file = fopen(found, "r");
                        if (file == NULL)
                        {
                            perror(red "Error opening file" reset_color);
                            return;
                        }
                        int c;
                        while ((c = fgetc(file)) != EOF)
                        {
                            printf("%c", c);
                        }
                        fclose(file);
                    }
                }
            }
        }
    }
}
