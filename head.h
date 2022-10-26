#ifndef HEAD_H
#define HEAD_H
typedef struct a{
    int num;
    int id;
    char* name;
    struct a* next;
}job;
#define f(i,j,n) for(int i=j;i<n;i++)
#include<sys/wait.h>
#include <time.h>
#include "job.h"
#include "prompt.h"
#include<string.h>
#include<stdio.h>
#include <limits.h>
#include<unistd.h>
#include<netdb.h>
#include "pwd.h"
#include "echo.h"
#include<stdlib.h>
#include "cd.h"
#include "history.h"
#include<fcntl.h>
#include<sys/stat.h>
#include "ls.h"
#include "com_selector.h"
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<sys/types.h>
#include "discover.h"
#include "pinfo.h"
#include <signal.h>
#include <sys/syscall.h> 
#include "redir.h"
#include "user_define.h"
#include <termios.h>
#include <ctype.h>
#include "tabfunc.h"
#endif