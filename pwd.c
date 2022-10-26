#include "head.h"
void com_pwd(){
    char dir[1000];
    getcwd(dir,1000);
    printf("%s\n",dir);
}