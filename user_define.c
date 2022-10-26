#include "head.h"
int cmpfunc(const void *a, const void *b)
{
    job *node1 = *(job **)a;
    job *node2 = *(job **)b;
    return strcmp(node1->name, node2->name);
}
void jobs(char *a)
{
    int r_flag = 0, s_flag = 0;
    char *arg;
    arg = strtok(a, " \t\n");
    while (arg != NULL)
    {
        if (strcmp(arg, "-r") == 0)
            r_flag = 1;
        if (strcmp(arg, "-s") == 0)
            s_flag = 1;
        arg = strtok(NULL, " \t\n");
    }
    if (r_flag == 0 && s_flag == 0)
    {
        r_flag = 1;
        s_flag = 1;
    }
    job *head = back_jobs;
    job **arr_job = (job **)malloc(1024 * sizeof(job *));
    for (int i = 0; i < 1024; i++)
        arr_job[i] = (job *)malloc(1024 * sizeof(job));
    for (int i = 0; i < num_job; i++)
    {
        arr_job[i] = head;
        head = head->next;
    }
    qsort(arr_job, num_job, sizeof(job *), cmpfunc);
    for (int i = 0; i < num_job; i++)
    {
        char *path = (char *)malloc(1024 * sizeof(char));
        strcpy(path, "");
        sprintf(path, "/proc/%d/stat", arr_job[i]->id);
        FILE *fp = fopen(path, "r");
        if (fp == NULL)
        {
            perror("Error");
            return;
        }
        char *str2 = (char *)malloc(1024 * sizeof(char));
        char *type = (char *)malloc(20 * sizeof(char));
        strcpy(type, "");
        strcpy(str2, "");
        fgets(str2, 1024, fp);
        path = strtok(str2, " \t\n");
        int val = 1;
        while (path != NULL)
        {
            if (val == 3)
            {
                if (strcmp(path, "R") == 0 || strcmp(path, "S") == 0)
                {
                    strcpy(type, "Running");
                }
                else
                {
                    strcpy(type, "Stopped");
                }
            }
            path = strtok(NULL, " \t\n");
            val++;
        }
        if ((strcmp(type, "Running") == 0 && r_flag == 1) || (strcmp(type, "Stopped") == 0 && s_flag == 1))
            printf("[%d] %s %s [%d]\n", arr_job[i]->num, type, arr_job[i]->name, arr_job[i]->id);
    }
    if (num_job == 0)
    {
        printf("No background process\n");
    }
}
void sig(char *a)
{
    char *args = strtok(a, " \t\n");
    int val = 0;
    int proc_num, sig_num;
    while (args != NULL)
    {
        val++;
        if (val == 2)
        {
            proc_num = atoi(args);
        }
        if (val == 3)
        {
            sig_num = atoi(args);
        }
        args = strtok(NULL, " \t\n");
    }
    if (val != 3)
    {
        printf("sig:Incorrect number of arguments\n");
        return;
    }
    job *head = back_jobs;
    while (head != NULL)
    {
        if (head->num == proc_num)
        {
            if (kill(head->id, sig_num) == -1)
                perror("Error");
            return;
        }
        head = head->next;
    }
    printf("sig:No such process running\n");
    return;
}
void bg(char *a)
{
    char *args = strtok(a, " \t\n");
    int val = 0;
    int proc_num;
    while (args != NULL)
    {
        val++;
        if (val == 2)
        {
            proc_num = atoi(args);
        }
        args = strtok(NULL, " \t\n");
    }
    if (val != 2)
    {
        printf("bg:Incorrect number of arguments\n");
        return;
    }
    char *command = (char *)malloc(100 * sizeof(char));
    sprintf(command, "sig %d %d", proc_num, 18);
    select_command(command, NULL);
}
void fg(char *a)
{
    char *args = strtok(a, " \t\n");
    int val = 0;
    int proc_num;
    while (args != NULL)
    {
        val++;
        if (val == 2)
        {
            proc_num = atoi(args);
        }
        args = strtok(NULL, " \t\n");
    }
    if (val != 2)
    {
        printf("fg:Incorrect number of arguments\n");
        return;
    }
    job *head = back_jobs, *prev = NULL;
    while (head != NULL)
    {
        if (head->num == proc_num)
        {
            printf("%s\n", head->name);
            time_t start = time(NULL);
            if (kill(head->id, 18) == -1)
            {
                perror("Error");
                return;
            }
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(0, head->id);
            int status;
            ignore = head->id;
            waitpid(head->id, &status, WUNTRACED);
            if (!WIFSTOPPED(status))
            {
                if (prev == NULL)
                {
                    back_jobs = head->next;
                    num_job--;
                }
                else
                {
                    prev->next = head->next;
                    num_job--;
                }
                if (num_job == 0)
                {
                    ctr = 1;
                }
            }
            else
            {
                printf("\n[%d] Stopped\t\t%s\n", head->num, head->name);
            }
            tcsetpgrp(0, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
            flag = 1;
            time_t end = time(NULL);
            time_t used_time = end - start;
            total += used_time;
            ignore = 0;
            return;
        }
        prev = head;
        head = head->next;
    }
    printf("fg:No such Job\n");
    return;
}