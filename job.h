void new_job(char *a);
void check_back();
void add_back_job(char * name, int id);
void run_proc(char *proc,char *type);
void add_term_job(char *name,int id,int status);
void print_term();
job *back_jobs;
int num_job;
int ctr;
typedef struct b{
    char *name;
    int id;
    int status;
    struct b* next;
}terminate;
terminate *term;