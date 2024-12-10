#include "headers.h"
char home[MAX];
char prevDir[MAX];
char currDir[MAX];
char historyfile[MAX];
char tempfile[MAX];
char og[MAX];



char *getRelativePath(char *pwd)
{
    char *relPath = malloc(sizeof(char) * 4096);
    if (strcmp(pwd, og) == 0)
    {
        strcpy(relPath, "~");
    }
    else if (strncmp(pwd, og, strlen(og)) == 0) // comparing if part of the path matches og and then copying the remaining part as path
    {
        strcpy(relPath, "~");
        strcpy(relPath + 1, pwd + strlen(og));
    }
    else
    {
        strcpy(relPath, pwd);
    }
    return relPath;
}

void play_animation(char *frames[], int frame_count, int delay, int repeat)
{
    do
    {
        for (int i = 0; i < frame_count; i++)
        {
            printf(yellow "%s" reset_color, frames[i]);
            usleep(delay * 1000);
        }
    } while (repeat);
}

void setup()
{
    char *frames[] = {
        "                                   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣆⢀⣶⡶⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "                                   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣿⢸⠟⣠⣶⡷⠀⠀⠀⠀⠀⠀⠀⠀⠀\n",
        "                                   ⠀⠀⠀⠀⠀⠀⢀⣀⠀⢀⣠⠴⠴⠶⠚⠿⠿⠾⠭⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "                                   ⠀⠀⠀⠀⢀⠴⢋⡽⠚⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠢⣀⠀⠀⠀⠀⠀⠀\n",
        "                                   ⠀⠀⢀⡔⠁⡰⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⠚⠛⣖⠀⠀⠀⠀\n"
        "                                   ⠀⢀⡏⠀⡼⢡⠚⡛⠒⣄⠀⠀⠀⠀⣠⠖⠛⠛⠲⡄⠐⢯⠁⠀⠀⠹⡧⠀⠀⠀\n",
        "                                   ⠀⣸⠀⠀⡇⠘⠦⣭⡤⢟⡤⠤⣀⠀⠣⣀⡉⢁⣀⠟⠀⠀⢷⠀⠀⠀⠙⣗⠀⠀\n"
        "                                   ⠁⢻⠀⠀⢷⢀⡔⠉⢻⡅⣀⣤⡈⠙⠒⠺⠯⡍⠁⠀⠀⠀⢸⡆⠀⠀⠀⠘⡶⠄\n",
        "                                   ⠀⣈⣧⠴⠚⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣰⠃⠀⠀⠀⠀⣸⡇⠀⠀⠀⠀⠸⣔\n"
        "                                   ⣾⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣧⣤⡤⠴⠖⠋⢹⠃⠀⠀⠀⠀⠀⣷\n",
        "                                   ⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⣻⠁⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⣼\n"
        "                                   ⠙⠑⣤⣀⠀⠀⠀⠀⠀⢀⠀⠀⢄⣐⠴⠋⠀⠀⠀⠀⠀⠀⠘⢆⠀⠀⠀⠀⣰⠟\n",
        "                                   ⠀⠀⠀⣑⡟⠛⠛⠛⠛⠛⠛⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢴⡾⠋⠀\n"
        "                                   ⠀⠀⠀⡾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡇⠀⠀\n",
        "                                   ⠀⠀⣰⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀\n"
        "                                   ⠀⠀⠸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⠃⠀⠃\n",
    };

    int frame_count = sizeof(frames) / sizeof(frames[0]);
    int delay = 200;
    int repeat = 0;

    play_animation(frames, frame_count, delay, repeat);
    initialize_aliases();
    strcpy(home, getenv("HOME"));
    getcwd(currDir, MAX);
    strcpy(og, currDir);
    strcpy(prevDir, currDir);

    const char *filename = "history.txt";
    size_t filelength = strlen(filename) + strlen(og);
    snprintf(historyfile, filelength + 2, "%s/%s", og, filename);
    printf(yellow "Welcome to Betty,the friendly interactive shell\n" reset_color);

    return;
}