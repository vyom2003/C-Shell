#include "head.h"
int select_command(char *command, FILE *fp)
{
    char* a=(char*)malloc(1000*sizeof(char));
    strcpy(a, command);
    char *str = (char *)malloc(1000 * sizeof(char));
    str = strtok(command, " \t\n");
    if (str == NULL)
    {
        return 0;
    }
    if (strcmp(str,"exit") == 0)
    {
        return 1;
    }
    for (int i = 0; i < strlen(a); i++)
    {
        if (a[i] == '<' || a[i] == '>' || a[i] == '|')
        {
            new_job(a);
            return 0;
        }
    }
    if (strcmp(str, "pwd") == 0)
    {
        com_pwd();
    }
    else if (strcmp(str, "echo") == 0)
    {
        com_echo(a);
    }
    else if (strcmp(str, "cd") == 0)
    {
        str = strtok(NULL, " \t\n");
        char *a = (char *)malloc(1000 * sizeof(char));
        if (str == NULL)
        {
            return cd_type1();
            return 0;
        }
        strcpy(a, str);
        a = strtok(NULL, " \t\n");
        if (a != NULL)
        {
            printf("cd : Too many arguments\n");
            return 0;
        }
        else if (strcmp(str, "~") == 0)
        {
            return cd_type1();
        }
        else if (strcmp(str, ".") == 0)
        {
            strcpy(PREV_DIR, CURR_DIR);
        }
        else if (strcmp(str, "..") == 0)
        {
            return cd_type2();
        }
        else if (strcmp(str, "-") == 0)
        {
            return cd_type3();
        }
        else
        {
            return cd_type4(str);
        }
        free(a);
    }
    else if (strcmp("history", str) == 0)
    {
        history(fp);
    }
    else if (strcmp("jobs", str) == 0)
    {
        jobs(a);
    }
    else if (strcmp("sig", str) == 0)
    {
        sig(a);
    }
    else if (strcmp("bg", str) == 0)
    {
        bg(a);
    }
    else if(strcmp(str,"fg")==0)
    {
        fg(a);
    }
    else if (strcmp("ls", str) == 0)
    {
        com_ls(a);
    }
    else if (strcmp("discover", str) == 0)
    {
        com_disc(a);
    }
    else if (strcmp("pinfo", str) == 0)
    {
        com_pinfo(a);
    }
    else
    {
        new_job(a);
    }
    return 0;
}