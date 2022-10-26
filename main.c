#include "head.h"
int ptr;
char *command;
void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}
void c_handle()
{
    printf("^C\n");
    prompt();
    ptr=0;
    memset(command,'\0',1000);
}
void z_handle()
{
    return;
}
int main()
{
    ctr=1;
    num_job=0;
    ignore=0;
    back_jobs=NULL;
    term=NULL;
    flag=0;
    total=0;
    HOME_DIR = (char *)malloc(1000 * sizeof(char));
    PREV_DIR = (char *)malloc(1000 * sizeof(char));
    CURR_DIR = (char *)malloc(1000 * sizeof(char));
    getcwd(HOME_DIR, 1000);
    strcpy(PREV_DIR, HOME_DIR);
    strcpy(CURR_DIR, HOME_DIR);
    char path[1000];
    strcpy(path, HOME_DIR);
    strcat(path, "/.history");
    FILE *fp = fopen(path, "a+");
    char ch;
    while (1)
    {
        setbuf(stdout,NULL);
        signal(SIGCHLD,check_back);
        signal(SIGINT,c_handle);
        signal(SIGTSTP,z_handle);
        command = (char *)malloc(1000 * sizeof(char));
        memset(command,'\0',1000);
        ptr=0;
        enableRawMode();
        print_term();
        prompt();
        while(read(STDIN_FILENO,&ch,1)==1)
        {
            if(iscntrl(ch))
            {
                if(ch==10)
                {
                    command[ptr]='\n';
                    printf("\n");
                    break;
                }
                else if(ch ==127)
                {
                    if(ptr>0)
                    {
                        printf("\b \b");
                        ptr--;
                        command[ptr]='\0';
                    }
                }
                else if(ch==4)
                {
                    printf("\n");
                    exit(0);
                }
                else if(ch ==3)
                {
                    printf("^C\n");
                    prompt();
                    ptr=0;
                    memset(command,'\0',1000);
                }
                else if(ch==9)
                {
                    tabfunc(command);
                }
                else
                {
                    printf("%c",ch);
                    command[ptr]=ch;
                    ptr++;
                }
            }
            else
            {
                command[ptr]=ch;
                printf("%c",ch);
                ptr++;
            }
        }
        disableRawMode();
        print_term();
        if(strcmp(command,"\n"))
        fp = insert_into_hist(command, fp);
        char *a = (char *)malloc(1000 * sizeof(char));
        a = strtok(command, ";");
        int flag_ex = 0;
        char **b = (char **)malloc(sizeof(char *) * 1000);
        for (int i = 0; i < 1000; i++)
        {
            b[i] = (char *)malloc(1000 * sizeof(char));
        }
        int num=0;
        while (a != NULL)
        {
            strcpy(b[num++], a);
            a = strtok(NULL, ";");
        }
        for(int i=0;i<num;i++)
        {
            flag_ex=select_command(b[i], fp);
        }
        if (flag_ex==1)
            break;
    }
}