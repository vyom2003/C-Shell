#include "head.h" 
void com_echo(char * a)
{
    char* str=strtok(a," \t\n");
    str=strtok(NULL," \t\n");
    while(str!=NULL)
    {
        printf("%s ",str);
        str=strtok(NULL," \t\n");
    }
    printf("\n");
}