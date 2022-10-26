#include "head.h"
void com_disc(char *a)
{
    char *str = (char *)malloc(1000 * sizeof(char));
    str = strtok(a, " \t\n");
    int flag1 = 0, flag2 = 0;
    char *directory = (char *)malloc(500 * sizeof(char));
    char *file = (char *)malloc(500 * sizeof(char));
    strcpy(directory, ".");
    strcpy(file,"");
    while (str != NULL)
    {
        if (strcmp("discover", str) == 0)
        {
            ;
        }
        else if (strcmp("-d", str) == 0)
        {
            flag1 = 1;
        }
        else if (strcmp("-f", str) == 0)
        {
            flag2 = 1;
        }
        else if (str[0] == '"')
        {
            strncpy(file, &str[1],strlen(str)-2);
        }
        else
        {
            strcpy(directory, str);
        }
        str = strtok(NULL, " \t\n");
    }
    if(directory[0]=='~')
    {
        char *a=(char*)malloc(1000*sizeof(char));
        strcpy(a,HOME_DIR);
        if(strlen(directory)>1)
        strcat(a,&directory[1]);
        if(chdir(a)<0)
        {
            perror("Error");
            return;
        }
    }
    else if(chdir(directory)<0)
    {
        perror("Error");
        return;
    }
    char *home_path=(char*)malloc(1000*sizeof(char));
    getcwd(home_path,1000);
    if (flag1 == 0 && flag2 == 0)
    {
        flag1 = 1;
        flag2 = 1;
    }
    if(flag1==1 && strcmp(file,"")==0)
    {
        printf("%s\n",directory);
    }
    disc_type1(flag1, flag2,directory,strlen(home_path),file);

    chdir(CURR_DIR);

}
void disc_type1(int f1, int f2,char *home,int len,char *file)
{
    struct dirent **list_files;
    int num = scandir(".", &list_files, NULL, alphasort);
    if (num == 0)
        return;
    for (int i = 0; i < num; i++)
    {
        struct stat file_stat;
        stat(list_files[i]->d_name, &file_stat);
        if(strcmp(list_files[i]->d_name,".")==0 || strcmp(list_files[i]->d_name,"..")==0)
        {
            continue;
        }
        if(S_ISREG(file_stat.st_mode)==1)
        {
            if(strcmp(file,"")==0 && f2==1)
            {
                char *path=(char*)malloc(1000*sizeof(char));
                getcwd(path,1000);
                char *a=(char*)malloc(1000*sizeof(char));
                strcpy(a,home);
                strcat(a,&path[len]);
                if(strcmp(a,"/")!=0) strcat(a,"/");
                strcat(a,list_files[i]->d_name);
                printf("%s\n",a);
            }
            else if(f2==1)
            {
                if(strcmp(file,list_files[i]->d_name)==0)
                {
                    char *path=(char*)malloc(1000*sizeof(char));
                    getcwd(path,1000);
                    char *a=(char*)malloc(1000*sizeof(char));
                    strcpy(a,home);
                    strcat(a,&path[len]);
                    if(strcmp(a,"/")!=0) strcat(a,"/");
                    strcat(a,list_files[i]->d_name);
                    printf("%s\n",a);
                }
            }
        }
        else
        {
            if(strcmp(file,"")==0 && f1==1)
            {
                char *path=(char*)malloc(1000*sizeof(char));
                getcwd(path,1000);
                char *a=(char*)malloc(1000*sizeof(char));
                strcpy(a,home);
                strcat(a,&path[len]);
                if(strcmp(a,"/")!=0) strcat(a,"/");
                strcat(a,list_files[i]->d_name);
                printf("%s\n",a);
            }
            else if(f1==1)
            {
                if(strcmp(file,list_files[i]->d_name)==0)
                {
                    char *path=(char*)malloc(1000*sizeof(char));
                    getcwd(path,1000);
                    char *a=(char*)malloc(1000*sizeof(char));
                    strcpy(a,home);
                    strcat(a,&path[len]);
                    if(strcmp(a,"/")!=0) strcat(a,"/");
                    strcat(a,list_files[i]->d_name);
                    printf("%s\n",a);
                }
            }
            char *path=(char*)malloc(1000*sizeof(char));
            getcwd(path,1000);
            if(chdir(list_files[i]->d_name)>=0)
            {
                disc_type1(f1,f2,home,len,file);
            }
            else continue;
            chdir(path);
            free(path);
        }
    }
}
