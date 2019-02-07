#include "fonctions.h"
#include <string.h>
#include <conio.h>
#include<windows.h>
#include<math.h>
#include<stdio.h>
#include<time.h>

int nb_jobs = 3 ;

Job jobs[10];
Job jobs_init[10];

Exec exec_machine1[11];
Exec exec_machine2[11];
Exec exec_machine3[11];

Machine machines[3];

int somme_date_fin_exec = 0;

/* Lecture des durées d'indisponibilité des 3 machines */
void lire_indisp(){
    int i;
    int debut;
    int fin;
    printf("\n\n\t Veuillez introduire l'indisponibilites des machines \n");
    for (i=0;i<3;i++){
            system("cls");
            printf("\n\n\t Veuillez introduire l'indisponibilites des machines \n");
        printf("\n  - Veuillez introduire date debut d'indisponiblite de la machine %d :  - ",i+1);
        scanf("%d",&machines[i].debut_indisp);
        printf("\n  - Veuillez introduire date fin d'indisponiblite de la machine %d  :  - ",i+1);
        scanf("%d",&machines[i].fin_indisp);
    }
    getch();
}

/* Lecture des jobs */
void lire_jobs(){
    int i;
    getch();
    system("cls");
    printf("\n\n\t Veuillez introduire le nombre de Jobs : -  ");
    scanf("%d",&nb_jobs);

    for(i=0;i<nb_jobs;i++){
         system("cls");
        jobs_init[i].id_job=i+1;
        printf("\n\t\t  ### JOB %d ### \n\n",i+1);
        printf("\n  - Veuillez introduire la duree de la tache 1 : - ");
        scanf("%d",&jobs_init[i].time_task1);
        printf("\n  - Veuillez introduire la duree de la tache 2 : - ");
        scanf("%d",&jobs_init[i].time_task2);
        printf("\n  - Veuillez introduire la duree de la tache 3 : - ");
        scanf("%d",&jobs_init[i].time_task3);
        jobs_init[i].time_op  = jobs_init[i].time_task1 + jobs_init[i].time_task2 +jobs_init[i].time_task3;
        jobs_init[i].date_fin_task1=0;
        jobs_init[i].date_fin_task2=0;
    }
}

Machine lire_machine(int i){

    return machines[i];
}

int lire_nb_jobs(){
return nb_jobs;
}

Job lire_job(int i){
    return jobs_init[i];
}

/** Heuristique 01 - FIFO (First In First Out) **/

void heursitique1(){
    int i;
    /** Tri du tableau jobs : pas la peine parceque c'est deja trié (FIFO), on copie juste le tableau desjob "jobs_init" dans le tableau des jobs de travail "jobs"**/
    for(i=0;i<nb_jobs;i++){
        jobs[i]=jobs_init[i];
    }
    /** Initialiser le compteur **/
    somme_date_fin_exec=0;
    /** Construction des tableau exec **/
    execution_jobs();

    /** Affichage des resultats **/
    affichage();
}

/** Heuristique 02 - SPT (Shortest Processing Time) **/

void heursitique2(){
    int i,j;
    Job job;
    /** Copier les jobs initials "jobs_init" dans le tableau de travail "jobs" **/
    for(i=0;i<nb_jobs;i++){
        jobs[i]=jobs_init[i];
    }
    /** Initialiser le compteur **/
    somme_date_fin_exec=0;
    /** Tri du tableau jobs par ordre croissant des durees operatoire **/
    for(i=0;i<nb_jobs-1;i++){
        for(j=i+1;j<nb_jobs;j++){
            if ( jobs[i].time_op > jobs[j].time_op ) {
                job = jobs[i];
                jobs[i] = jobs[j];
                jobs[j] = job;
            }
        }
    }
    /** Construction des tableau exec **/
    execution_jobs();

    /** Affichage des resultats **/
    affichage();

}


/** Heuristique 03 .................................... **/

void heursitique3(){
	
	 int i,j;
    Job job;
    /** Copier les jobs initials "jobs_init" dans le tableau de travail "jobs" **/
    for(i=0;i<nb_jobs;i++){
        jobs[i]=jobs_init[i];
    }
    /** Initialiser le compteur **/
    somme_date_fin_exec=0;
    /** Tri du tableau jobs par ordre decroissant des durees operatoire **/
    for(i=0;i<nb_jobs-1;i++){
        for(j=i+1;j<nb_jobs;j++){
            if ( jobs[i].time_task1 + jobs[i].time_task2 > jobs[j].time_task1 + jobs[j].time_task2 ) {
                job = jobs[i];
                jobs[i] = jobs[j];
                jobs[j] = job;
            }
        }
    }
    /** Construction des tableau exec **/
    execution_jobs();

    /** Affichage des resultats **/
    affichage();
	

}

/** Heuristique 04 .................................... **/

void heursitique4(){
	
		 int i,j;
    Job job;
    /** Copier les jobs initials "jobs_init" dans le tableau de travail "jobs" **/
    for(i=0;i<nb_jobs;i++){
        jobs[i]=jobs_init[i];
    }
    /** Initialiser le compteur **/
    somme_date_fin_exec=0;
    /** Tri du tableau jobs par ordre croissant des durees d'execution de la premiere tache de chaque job **/
    for(i=0;i<nb_jobs-1;i++){
        for(j=i+1;j<nb_jobs;j++){
            if ( jobs[i].time_task1 > jobs[j].time_task1 ) {
                job = jobs[i];
                jobs[i] = jobs[j];
                jobs[j] = job;
            }
        }
    }
    /** Construction des tableau exec **/
    execution_jobs();

    /** Affichage des resultats **/
    affichage();
	
	

}


/** Procédure permettant l'execution des jobs et construction des tableaux exec **/
void execution_jobs(){
    int i,avancement, temps1=0,temps2=0,temps3=0,indisp1=0,indisp2=0,indisp3=0;
    char nom[4];

    /* Machine 01*/
    avancement=0;
    for(i=0;i<nb_jobs;i++){

        if(i==1) temps2 = temps1;
        if((temps1+jobs[i].time_task1 > machines[0].debut_indisp && temps1 <= machines[0].debut_indisp) || (temps1>=machines[0].fin_indisp && !indisp1) ){
           if( temps1 <= machines[0].fin_indisp ) temps1 = machines[0].fin_indisp;
           exec_machine1[avancement].debut = machines[0].debut_indisp;
           exec_machine1[avancement].fin = machines[0].fin_indisp;
           strcpy(exec_machine1[avancement].nom,"INDISP");
           avancement++;
           indisp1=1;
        }
        exec_machine1[avancement].debut = temps1;
        temps1 = temps1 + jobs[i].time_task1;
        jobs[i].date_fin_task1=temps1;
        exec_machine1[avancement].fin = temps1;
        sprintf(nom,"T1%d",jobs[i].id_job);
        strcpy(exec_machine1[avancement].nom,nom);
        avancement++;
    }

    /* Machine 02*/
    avancement=0;

    for(i=0;i<nb_jobs;i++){

        if(i==1) temps3 = temps2;
        if(temps2 < jobs[i].date_fin_task1) temps2 = jobs[i].date_fin_task1;
        if((temps2+jobs[i].time_task2 > machines[1].debut_indisp && temps2<= machines[1].debut_indisp) || (temps2>=machines[1].fin_indisp && !indisp2)){
           if( temps2<= machines[1].fin_indisp) temps2 = machines[1].fin_indisp;
           exec_machine2[avancement].debut = machines[1].debut_indisp;
           exec_machine2[avancement].fin = machines[1].fin_indisp;
           strcpy(exec_machine2[avancement].nom,"INDISP");
           avancement++;
           indisp2=1;
        }
        exec_machine2[avancement].debut = temps2;
        temps2 = temps2 + jobs[i].time_task2;
        jobs[i].date_fin_task2 = temps2;
        exec_machine2[avancement].fin = temps2;
        sprintf(nom,"T2%d",jobs[i].id_job);
        strcpy(exec_machine2[avancement].nom,nom);
        avancement++;
    }
      /* Machine 03*/
    avancement=0;
    for(i=0;i<nb_jobs;i++){
        if(temps3 < jobs[i].date_fin_task2) temps3 = jobs[i].date_fin_task2;
        if((temps3+jobs[i].time_task3 > machines[2].debut_indisp && temps3<= machines[2].debut_indisp)|| (temps3>=machines[2].fin_indisp && !indisp3)){
           if( temps3<= machines[2].fin_indisp) temps3 = machines[2].fin_indisp;
           exec_machine3[avancement].debut = machines[2].debut_indisp;
           exec_machine3[avancement].fin = machines[2].fin_indisp;
           strcpy(exec_machine3[avancement].nom,"INDISP");
           avancement++;
        }
        exec_machine3[avancement].debut = temps3;
        temps3 = temps3 + jobs[i].time_task3;
        somme_date_fin_exec = somme_date_fin_exec + temps3;
        exec_machine3[avancement].fin = temps3;
        sprintf(nom,"T3%d",jobs[i].id_job);
        strcpy(exec_machine3[avancement].nom,nom);
        avancement++;
        indisp3=1;
    }
}

void affichage(){
    system("cls");
    int i;
    printf("\n\t\t\t\t\t AFFICHAGE DES RESULTATS\n\n");
    /* Machine 01*/
    printf("\n------------------------------------------------------------------------------------------------------------------------\n\n  - M1: ");
    for(i=0;i<nb_jobs+1;i++){
        printf("| %d == %s ==> %d |",exec_machine1[i].debut,exec_machine1[i].nom,exec_machine1[i].fin);
    }

     /* Machine 02*/
    printf("\n\n------------------------------------------------------------------------------------------------------------------------\n\n  - M2: ");
    for(i=0;i<nb_jobs+1;i++){
        printf("| %d == %s ==> %d |",exec_machine2[i].debut,exec_machine2[i].nom,exec_machine2[i].fin);
    }

     /* Machine 03*/
    printf("\n\n------------------------------------------------------------------------------------------------------------------------\n\n  - M3: ");
    for(i=0;i<nb_jobs+1;i++){
        printf("| %d == %s ==> %d |",exec_machine3[i].debut,exec_machine3[i].nom,exec_machine3[i].fin);
    }
    printf("\n\n------------------------------------------------------------------------------------------------------------------------\n");
    printf("\n\n  La somme des dates de fins d'execution des jobs = %d \n",somme_date_fin_exec);
    getch();
}


