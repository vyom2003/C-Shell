#include "head.h"
extern int ptr;
void tabfunc(char *a)
{
    char *copy = (char *)malloc(1000 * sizeof(char));
    strcpy(copy, a);
    char *str = strtok(copy, " \t\n");
    char *prev = (char *)malloc(1000 * sizeof(char));
    strcpy(prev, "");
    while (str != NULL)
    {
        strcpy(prev, str);
        str = strtok(NULL, " \t\n");
    }
    char *file_path = (char *)malloc(100 * sizeof(char));
    strcpy(file_path, "");
    for (int i = 0; i < strlen(prev); i++)
    {
        if (prev[i] == '/')
        {
            strcpy(file_path,"./");
            strncat(file_path, prev, i);
            if(chdir(file_path)==-1)
            {
                chdir(CURR_DIR);
                return;
            }
            prev=&prev[i+1];
            i=0;
        }
    }
    if(a[strlen(a)-1]==' ')
    {
        strcpy(prev,"");
    }
    char **accepted = (char **)malloc(100 * sizeof(char *));
    for (int i = 0; i < 100; i++)
        accepted[i] = (char *)malloc(1000 * sizeof(char));
    struct dirent **list_files;
    int ctr_tab = 0;
    int num = scandir(".", &list_files, NULL, alphasort);
    if (num < 0)
    {
        chdir(CURR_DIR);
        return;
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            if (strncmp(prev, list_files[i]->d_name, strlen(prev)) == 0)
            {
                strcpy(accepted[ctr_tab], list_files[i]->d_name);
                ctr_tab++;
            }
        }
    }
    if (ctr_tab == 0)
    {
        chdir(CURR_DIR);
        return;
    }
    if (ctr_tab > 1)
    {
        printf("\n");
        for (int i = 0; i < ctr_tab; i++)
        {
            struct stat st;
            stat(accepted[i], &st);
            printf("%s", accepted[i]);
            if (S_ISREG(st.st_mode) == 1)
            {
                printf("\n");
            }
            else
                printf("/\n");
        }
    }
    char *pres=(char*)malloc(100*sizeof(char));
    pres=getcwd(pres,100);
    chdir(CURR_DIR);
    printf("\r");
    prompt();
    chdir(pres);
    char *base = accepted[0];
    int i = strlen(prev);
    for (; i < strlen(base); i++)
    {
        int flag = 0;
        for (int j = 1; j < ctr_tab; j++)
        {
            if (accepted[j][i] != base[i])
            {
                flag = 1;
                chdir(CURR_DIR);
                break;
            }
        }
        if (flag)
        {
            chdir(CURR_DIR);
            break;
        }
        else
        {
            a[ptr] = base[i];
            ptr++;
        }
    }
    if (i == strlen(base) && ctr_tab == 1)
    {
        struct stat st;
        stat(base, &st);
        if (S_ISREG(st.st_mode) == 1)
        {
            a[ptr] = ' ';
            ptr++;
        }
        else
        {
            a[ptr] = '/';
            ptr++;
        }
    }
    printf("%s", a);
    chdir(CURR_DIR);
}