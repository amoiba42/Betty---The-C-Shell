#ifndef __HEADERS_H_
#define __HEADERS_H_
#define MAX 4096

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pwd.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include <signal.h>
#include <termios.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>

extern char home[MAX];
extern char prevDir[MAX];
extern char currDir[MAX];
extern char og[MAX];
extern char historyfile[MAX];
extern char tempfile[MAX];
extern char longcommand[MAX] ;
extern char lastcommand[MAX];
extern char supremecommand[MAX];
extern double total;
extern pid_t current_fg;
#define MAX_HISTORY 15
#define MAX_PROCESSES 4096

typedef struct
{
    char command[256];
    pid_t pid;
    int state; 
} ProcessInfo;

extern ProcessInfo processes[MAX_PROCESSES];
extern int process_count;


#include "color.h"
#include "prompt.h"
#include "command.h"
#include "setup.h"
#include "hop.h"
#include "proclore.h"
#include "reveal.h"
#include "seek.h"
#include "log.h"
#include "fgbg.h"
#include "manpages.h"
#include "neonatex.h"
#include "signals.h"

#endif