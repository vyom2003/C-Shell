#include "head.h"
void redir(char *a)
{
    int flag_pipe=0;
    int in = dup(STDIN_FILENO);
    int out=dup(STDOUT_FILENO);
    char *str, *savePtr = NULL;
    str = strtok_r(a, " \t\n", &savePtr);
    char *command =(char *)malloc(1000*sizeof(char*));
    strcpy(command,"");
    while(str!=NULL)
    {
        if(strcmp(str,"<")==0)
        {
            str=strtok_r(NULL," \t\n", &savePtr);
            int fd_in=open(str,O_RDONLY);
            if(fd_in==-1)
            {
                perror("Error");
                return;
            }
            dup2(fd_in,STDIN_FILENO);
        }
        else if(strcmp(str,">")==0)
        {
            str=strtok_r(NULL," \t\n", &savePtr);
            int fd_out=open(str,O_WRONLY | O_CREAT | O_TRUNC,0644);
            if(fd_out==-1)
            {
                perror("Error");
                return;
            }
            else
            {

                dup2(fd_out,STDOUT_FILENO);
            }
            flag_pipe=1;
        }
        else if(strcmp(str,">>")==0)
        {
            str=strtok_r(NULL," \t\n", &savePtr);
            int fd_out=open(str,O_WRONLY | O_CREAT | O_APPEND,0644);
            if(fd_out==-1)
            {
                perror("Error");
                return;
            }
            else
            {
                dup2(fd_out,STDOUT_FILENO);
            }
            flag_pipe=1;
        }
        else if(strcmp(str,"|")==0)
        {
            int p[2];
            if(pipe(p)==-1)
            {
                perror("Error");
                return;
            }
            if(flag_pipe==0&&dup2(p[1],STDOUT_FILENO)==-1)
            {
                perror("Error");
                return;
            }
            close(p[1]);
            select_command(command,NULL);
            strcpy(command,"");
            dup2(p[0],STDIN_FILENO);
            close(p[0]);
            dup2(out,STDOUT_FILENO);
        }
        else
        {
            strcat(command," ");
            strcat(command,str);
        }
        str=strtok_r(NULL," \t\n", &savePtr);
    }
    if(strcmp(command,"")!=0)
    {
        select_command(command,NULL);
    }
    dup2(out,STDOUT_FILENO);
    dup2(in,STDIN_FILENO);
    fflush(stdout);
    close(in);
    close(out);
}