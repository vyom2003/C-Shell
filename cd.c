#include "head.h"
int cd_type1()
{
    char dir[1000];
    getcwd(dir, 1000);
    strcpy(PREV_DIR, dir);
    if (chdir(HOME_DIR) != 0)
    {
        perror("Error");
        return -1;
    }
    strcpy(CURR_DIR, HOME_DIR);
    return 0;
}
int cd_type2()
{
    int last = 0;
    for (int i = strlen(CURR_DIR) - 1; i >= 0; i--)
    {
        if (CURR_DIR[i] == '/')
        {
            last = i;
            break;
        }
    }
    if (last == 0)
    {
        strcpy(PREV_DIR, CURR_DIR);
        strcpy(CURR_DIR, "/");
    }
    else
    {
        strcpy(PREV_DIR, CURR_DIR);
        char *a = (char *)malloc(1000 * sizeof(char));
        strcpy(a, CURR_DIR);
        strcpy(CURR_DIR, "");
        strncat(CURR_DIR, a, last);
    }
    if (chdir(CURR_DIR) != 0)
    {
        perror("Error");
        return -1;
    }
    return 0;
}
int cd_type3()
{
    char *a = (char *)malloc(1000 * sizeof(char));
    strcpy(a, CURR_DIR);
    strcpy(CURR_DIR, PREV_DIR);
    strcpy(PREV_DIR, a);
    if (chdir(CURR_DIR) != 0)
    {
        perror("Error");
        return -1;
    }
    else
    {
        printf("%s\n", CURR_DIR);
    }
    return 0;
}
int cd_type4(char *a)
{
    if (a[0] == '.')
    {
        if (chdir(a) != 0)
        {
            perror("Error");
            return -1;
        }
        strcpy(PREV_DIR, CURR_DIR);
        getcwd(CURR_DIR, 1000);
    }
    else if (a[0] == '~')
    {
        char *b = (char *)malloc(1000 * sizeof(char));
        strcpy(b, HOME_DIR);
        strcat(b, &a[1]);
        if (chdir(b) != 0)
        {
            perror("Error");
            return -1;
        }
        strcpy(PREV_DIR, CURR_DIR);
        strcpy(CURR_DIR, b);
    }
    else if (strcmp(a, "/") == 0)
    {
        
        if (chdir(a) != 0)
        {
            perror("Error");
            return -1;
        }
        else
        {
            strcpy(PREV_DIR, CURR_DIR);
            getcwd(CURR_DIR,1000);
        }
    }
    else
    {
        char *b = (char *)malloc(1000 * sizeof(char));
        strcpy(b, CURR_DIR);
        if (strcmp(CURR_DIR, "/") == 0)
        {
            strcat(CURR_DIR, a);
        }
        else
        {
            strcat(CURR_DIR, "/");
            strcat(CURR_DIR, a);
        }
        if (chdir(CURR_DIR) != 0)
        {
            if (chdir(a) != 0)
            {
                perror("Error");
                strcpy(CURR_DIR, b);
                return -1;
            }
            else
            {
                getcwd(CURR_DIR,1000);
                strcpy(PREV_DIR, b);
            }
        }
        else
        {
            strcpy(PREV_DIR, b);
        }
        getcwd(CURR_DIR,1000);
    }
    return 0;
}