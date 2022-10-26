#include "head.h"
FILE* insert_into_hist(char* a,FILE* fp)
{
    fseek(fp,0,SEEK_SET);
    int num_lines=0;
    char str[1000];
    char prev[1000];
    while(fgets(str,1000,fp))
    {
        num_lines++;
        strcpy(prev,str);
    }
    if(strcmp(prev,a)==0)
    return fp;
    if(num_lines<20)
    {
        fwrite(a,sizeof(char),strlen(a),fp);
        fseek(fp,0,SEEK_SET);
        return fp;
    }
    else{
        fseek(fp,0,SEEK_SET);
        char path[1000];
        strcpy(path,HOME_DIR);
        strcat(path,"/.temp");
        FILE* fp2=fopen(path,"a+");
        fgets(str,1000,fp);
        while(fgets(str,1000,fp))
        {
            fwrite(str,sizeof(char),strlen(str),fp2);
        }
        fwrite(a,sizeof(char),strlen(a),fp2);
        char path2[1000];
        strcpy(path2,HOME_DIR);
        strcat(path2,"/.history");
        remove(path2);
        rename(path,path2);
        fseek(fp2,0,SEEK_SET);
        return fp2;
    }
}
void history(FILE* fp)
{
    int num_lines=0;
    char str[1000];
    while(fgets(str,1000,fp))
    {
        num_lines++;
    }
    if(num_lines<=10)
    {
        fseek(fp,0,SEEK_SET);
        while(fgets(str,1000,fp))
        {
            printf("%s",str);
        }
    }
    else
    {
        fseek(fp,0,SEEK_SET);
        for(int i=0;i<num_lines-10;i++)
        {
            fgets(str,1000,fp);
        }
        while(fgets(str,1000,fp))
        {
            printf("%s",str);
        }
    }
}