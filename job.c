#include "head.h"
void add_back_job(char *name, int id)
{
    job *new = (job *)malloc(sizeof(job));
    new->name = (char *)malloc(1000 * sizeof(char));
    strcpy(new->name, "");
    strcpy(new->name, name);
    new->id = id;
    new->num = ctr;
    new->next = NULL;
    if (back_jobs == NULL)
    {
        back_jobs = new;
    }
    else
    {
        job *last = back_jobs;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = new;
    }
    num_job++;
    ctr++;
    return;
}
void add_term_job(char *name, int id, int status)
{
    terminate *new = (terminate *)malloc(sizeof(terminate));
    new->name = (char *)malloc(1000 * sizeof(char));
    strcpy(new->name, name);
    new->id = id;
    new->status = status;
    new->next = NULL;
    if (term == NULL)
    {
        term = new;
    }
    else
    {
        terminate *last = term;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = new;
    }
}
void print_term()
{
    terminate *prev;
    while (term != NULL)
    {
        if (term->status == 1)
        {
            printf("%s with pid %d terminated normally\n", term->name, term->id);
        }
        else
        {
            printf("%s with pid %d terminated normally\n", term->name, term->id);
        }
        prev = term;
        term = term->next;
        free(prev);
    }
}
void check_back()
{
    int orig = num_job;
    int status;
    job *head = back_jobs, *prev = NULL;
    while (head != NULL)
    {
        if (head->id == ignore)
        {
            break;
        }
        if (waitpid(head->id, &status, WNOHANG | WUNTRACED) != 0)
        {
            if (WEXITSTATUS(status))
            {
                add_term_job(head->name, head->id, 0);
            }
            else
            {
                add_term_job(head->name, head->id, 1);
            }
            free(head->name);
            if (!WIFSTOPPED(status))
            {
                if (prev == NULL)
                {
                    back_jobs = head->next;
                    head = head->next;
                    num_job--;
                    continue;
                }
                else
                {
                    prev->next = head->next;
                    head = head->next;
                    num_job--;
                    continue;
                }
            }
        }
        prev = head;
        head = head->next;
    }
    if (num_job == 0)
        ctr = 1;
    fflush(stdout);
    return;
}
void run_proc(char *proc, char *type)
{
    char **tokens = (char **)malloc(256 * sizeof(char *));
    char *str = (char *)(1000 * sizeof(char *));
    char *job_tbd = (char *)malloc(1000 * sizeof(char));
    strcpy(job_tbd, "");
    strcpy(job_tbd, proc);
    str = strtok(proc, " \t\n");
    int x = 0;
    while (str != NULL)
    {
        tokens[x] = (char *)malloc(1000 * sizeof(char));
        strcpy(tokens[x], str);
        x++;
        str = strtok(NULL, " \t\n");
    }
    pid_t id = fork();
    if (id == -1)
        perror("Error");
    else if (id == 0)
    {
        setpgid(0, 0);
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        int val = execvp(tokens[0], tokens);
        if (val < 0)
        {
            perror("Error");
            exit(1);
        }
        exit(0);
    }
    else
    {
        if (strcmp(type, "f") == 0)
        {
            time_t start = time(NULL);
            int status;
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(0, id);
            waitpid(id, &status, WUNTRACED);
            if (WIFSTOPPED(status))
            {
                char *name = strtok(job_tbd, "\n");
                add_back_job(name, id);
                printf("\n[%d] Stopped\t\t%s\n", ctr - 1, name);
            }
            signal(SIGCHLD, check_back);
            tcsetpgrp(0, getpgrp());

            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
            flag = 1;
            time_t end = time(NULL);
            time_t used_time = end - start;
            total += used_time;
            return;
        }
        else if (strcmp(type, "b") == 0)
        {
            int status;
            add_back_job(job_tbd, id);
            printf("[%d] %d\n", num_job, id);
        }
    }
}
void new_job(char *a)
{
    char *proc = (char *)malloc(1000 * sizeof(char));
    char *prev = (char *)malloc(1000 * sizeof(char));
    char **b = (char **)malloc(10 * sizeof(char *));
    for (int i = 0; i < 10; i++)
    {
        b[i] = (char *)malloc(1000 * sizeof(char));
    }
    char *f = (char *)malloc(1000 * sizeof(char *));
    int num_b = 0;
    proc = strtok(a, "&");
    while (proc != NULL)
    {
        strcpy(prev, proc);
        proc = strtok(NULL, "&");
        if (proc == NULL)
        {
            strcpy(f, prev);
        }
        else
        {
            strcpy(b[num_b++], prev);
        }
    }
    for (int i = 0; i < num_b; i++)
    {
        run_proc(b[i], "b");
    }
    for (int i = 0; i < strlen(f); i++)
    {
        if (f[i] == '<' || f[i] == '>' || f[i] == '|')
        {
            redir(f);
            return;
        }
    }
    run_proc(f, "f");
}