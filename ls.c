#include "head.h"
void com_ls(char *a)
{
    char *str = (char *)malloc(1000 * sizeof(char));
    str = strtok(a, " \t\n");
    int flag1 = 0, flag2 = 0;
    char **list_files = (char **)malloc(10 * sizeof(char *));
    for (int i = 0; i < 10; i++)
    {
        list_files[i] = (char *)malloc(1000 * sizeof(char));
    }
    int num_files = 0;
    while (str != NULL)
    {
        if (strcmp("ls", str) == 0)
        {
            ;
        }
        else if (strcmp("-a", str) == 0)
        {
            flag1 = 1;
        }
        else if (strcmp("-l", str) == 0)
        {
            flag2 = 1;
        }
        else if (strcmp("-al", str) == 0 || strcmp("-la", str) == 0)
        {
            flag1 = 1;
            flag2 = 1;
        }
        else
        {
            strcpy(list_files[num_files++], str);
        }
        str = strtok(NULL, " \t\n");
    }
    if (num_files == 0)
    {
        char curr[1000];
        char prev[1000];
        strcpy(curr, CURR_DIR);
        strcpy(prev, PREV_DIR);
        char *a = (char *)malloc(1000 * sizeof(char));
        strcpy(a, "  .");
        ls_curr(a, flag1, flag2);
        strcpy(CURR_DIR, curr);
        strcpy(PREV_DIR, prev);
    }
    else
    {
        for (int i = 0; i < num_files; i++)
        {
            if (num_files != 1 && i != 0)
            {
                printf("\n%s:\n", list_files[i]);
            }
            else if (num_files != 1)
            {
                printf("%s:\n", list_files[i]);
            }

            char curr[1000];
            char prev[1000];
            strcpy(curr, CURR_DIR);
            strcpy(prev, PREV_DIR);
            ls_curr(list_files[i], flag1, flag2);
            strcpy(CURR_DIR, curr);
            strcpy(PREV_DIR, prev);
            char command[1000] = "cd ";
            strcat(command, CURR_DIR);
            select_command(command, NULL);
            strcpy(CURR_DIR, curr);
            strcpy(PREV_DIR, prev);
        }
    }
    free(str);
    for (int i = 0; i < 10; i++)
    {
        free(list_files[i]);
    }
    free(list_files);
}
void ls_curr(char *a, int f1, int f2)
{
    char command[1000] = "cd ";
    strcat(command, a);
    struct stat file_stat;
    stat(a,&file_stat);
    if(S_ISREG(file_stat.st_mode)==1)
    {
        print_for_file(a,f2);
        return;
    }
    if(select_command(command, NULL)==-1)
    {
        return;
    }
    struct dirent **list_files;
    int num = scandir(".", &list_files, NULL, alphasort);
    if (num < 0)
        return;
    else
    {
        if (f2 == 1)
        {
            long long total = 0;
            for (int i = 0; i < num; i++)
            {
                struct stat file_stat;
                stat(list_files[i]->d_name, &file_stat);
                if (f1==0&&((list_files[i]->d_name)[0]) != '.')
                {
                    total+=file_stat.st_blocks/2;
                }
                else if(f1==1)
                {
                    total+=file_stat.st_blocks/2;
                }
            }
            printf("total %lld\n", total);
        }
        for (int i = 0; i < num; i++)
        {
            if (f1 == 1)
            {
                print_for_file(list_files[i]->d_name, f2);
            }
            else
            {
                if (((list_files[i]->d_name)[0]) != '.')
                {
                    print_for_file(list_files[i]->d_name, f2);
                }
            }
        }
    }
}
void print_for_file(char *file, int flag)
{
    if (flag == 0)
    {
        struct stat file_stat;
        stat(file, &file_stat);
        if(S_ISREG(file_stat.st_mode)==0)
        {
            printf("\033[0;34m");
            printf("%s\n",file);
            printf("\033[0m");
        }
        else if((file_stat.st_mode& S_IXUSR))
        {
            printf("\033[0;32m");
            printf("%s\n", file);
            printf("\033[0m");
        }
        else
        {
            printf("%s\n", file);
        }
    }
    else
    {
        char *permissions = (char *)malloc(100 * sizeof(char));
        strcpy(permissions, "");
        struct stat file_stat;
        stat(file, &file_stat);
        strcat(permissions, S_ISREG(file_stat.st_mode) ? "-" : "d");
        strcat(permissions, (file_stat.st_mode & S_IRUSR) ? "r" : "-");
        strcat(permissions, (file_stat.st_mode & S_IWUSR) ? "w" : "-");
        strcat(permissions, (file_stat.st_mode & S_IXUSR) ? "x" : "-");
        strcat(permissions, (file_stat.st_mode & S_IRGRP) ? "r" : "-");
        strcat(permissions, (file_stat.st_mode & S_IWGRP) ? "w" : "-");
        strcat(permissions, (file_stat.st_mode & S_IXGRP) ? "x" : "-");
        strcat(permissions, (file_stat.st_mode & S_IROTH) ? "r" : "-");
        strcat(permissions, (file_stat.st_mode & S_IWOTH) ? "w" : "-");
        strcat(permissions, (file_stat.st_mode & S_IXOTH) ? "x" : "-");
        struct passwd *pwd;
        struct group *grid;
        grid = getgrgid(file_stat.st_gid);
        pwd = getpwuid(file_stat.st_uid);
        struct tm *time = localtime(&(file_stat.st_mtime));
        char *buffer = (char *)malloc(30 * sizeof(char));
        strftime(buffer, 30, "%b %d %H:%M", time);
        printf("%s  %3ld %s %s  %10ld %.12s  ", permissions, file_stat.st_nlink, pwd->pw_name, grid->gr_name, file_stat.st_size, buffer);
        if(permissions[0]=='d')
        {
            printf("\033[0;34m");
            printf("%s\n",file);
            printf("\033[0m");
        }
        else if(file_stat.st_mode& S_IXUSR)
        {
            printf("\033[0;32m");
            printf("%s\n", file);
            printf("\033[0m");
        }
        else
        {
            printf("%s\n",file);
        }
        free(permissions);
    }
}