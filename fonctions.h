
typedef struct Job Job;
typedef struct Machine Machine;
typedef struct Exec Exec;

struct Job //la structurre d'un job
{
    int id_job;
    int time_task1;
    int date_fin_task1;
    int time_task2;
    int date_fin_task2;
    int time_task3;
    int time_op;
};

struct Machine // Structure d'une machine
{
    int debut_indisp;
    int fin_indisp;
};

struct Exec  // Structure des tâches exécutées sur les machines
{
    int debut;
    char nom[20];
    int fin;
};


void lire_indisp();
void lire_jobs();

Machine lire_machine(int i);
int lire_nb_jobs();
Job lire_job(int i);
void heursitique1();
void heursitique2();
void heursitique3();
void heursitique4();
void execution_jobs();
void affichage();


