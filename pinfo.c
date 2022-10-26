#include "head.h"
void com_pinfo(char *str)
{
    char *a = (char *)malloc(1000 * sizeof(char));
    a = strtok(str, " \t\n");
    a = strtok(NULL, " \t\n");
    int id;
    if (a == NULL)
    {
        id = getpid();
    }
    else
    {
        id = atoi(a);
    }
    char *path = (char *)malloc(1024 * sizeof(char));
    strcpy(path, "");
    printf("pid : %d\n", id);
    sprintf(path, "/proc/%d/stat", id);
    FILE *fp = fopen(path, "r");
    if(fp==NULL)
    {
        perror("Error");
        return;
    }
    char *str2 = (char *)malloc(1024 * sizeof(char));
    strcpy(str2, "");
    fgets(str2, 1024, fp);
    path = strtok(str2, " \t\n");
    int val = 1;
    int pgrp, tgpid;
    while (path != NULL)
    {
        if (val == 3)
        {
            printf("process status : %s", path);
        }
        else if (val == 5)
        {
            pgrp = atoi(path);
        }
        else if (val == 8)
        {
            tgpid = atoi(path);
            if (pgrp == tgpid)
            {
                printf("+");
            }
            printf("\n");
        }
        else if (val == 23)
        {
            printf("memory : %s\n", path);
        }
        val++;
        path = strtok(NULL, " \t\n");
    }
    path = (char *)malloc(sizeof(char) * 1024);
    strcpy(path, "");
    sprintf(path, "/proc/%d/exe", id);
    int size = readlink(path, str2, 1024);
    if(size==-1)
    {
        perror("Error");
        return;
    }
    str2[size] = '\0';
    char *str3 = (char *)malloc(sizeof(char) * 1024);
    if (strncmp(str2, HOME_DIR, strlen(HOME_DIR)) == 0)
    {
        strcpy(str3, "~");
        strcat(str3, &str2[strlen(HOME_DIR)]);
        str2=str3;
    }
    printf("executable path : %s\n", str2);
    free(str2);
    free(path);
}