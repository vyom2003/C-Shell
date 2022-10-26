#include "head.h"
void prompt()
{
    char a[5000]="<";
    char username[1000];
    getlogin_r(username,1000);
    char hostname[1000];
    gethostname(hostname,1000);
    strcat(a,username);
    strcat(a,"@");
    strcat(a,hostname);
    strcat(a,":");
    char dir[1000];
    getcwd(dir,1000);
    if(strncmp(dir,HOME_DIR,strlen(HOME_DIR))==0)
    {
        char path[2000]="~";
        if(strlen(dir)!=strlen(HOME_DIR))
        strcat(path,&dir[strlen(HOME_DIR)]);
        strcat(a,path);
    }        
    else
    {
        strcat(a,dir);
    }
    printf("\033[1;32m");
    printf("%s> ",a);
    if(flag==1 && total>=1)
    {
        printf("\b\b took %lds> ",total);
        total=0;
        flag=0;
    }
    else if(flag==1)
    {
        flag=0;
        total=0;
    }
    printf("\033[0m");
}