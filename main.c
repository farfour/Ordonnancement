#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "fonctions.c"



int main(){
	
	
    int i,vrai = 1,choix;
    lire_indisp();
    for(i=0;i<3;i++){
        printf("\n\t La machine %d :  debut d'indisponiblite : %d , fin d'indisponiblite : %d \n",i+1,lire_machine(i).debut_indisp,lire_machine(i).fin_indisp);
    }
    lire_jobs();
    for(i=0;i<lire_nb_jobs();i++){
        printf("\n\n\t Le JOB %d :\n  - duree tache 1 : %d\n  - duree tache 2 : %d\n  - duree tache 3 : %d\n  - somme duree operatoire : %d\n",
               lire_job(i).id_job,lire_job(i).time_task1,lire_job(i).time_task2,lire_job(i).time_task3,lire_job(i).time_op);
    }
    getch();
    while(vrai){
        system("cls");
        printf("\n\n\t\t\t\t MENU \n");
        printf("\n\n\t 1- HEURISTIQUE 01 : FIFO (First In First Out)");
        printf("\n\n\t 2- HEURISTIQUE 02 : SPT (Shortest Processing Time)");
        printf("\n\n\t 3- HEURISTIQUE 03 : Priorité au job dont le temps d'opération est le plus grand ");
        printf("\n\n\t 4- HEURISTIQUE 04 : Priorité au job dont le temps d'execution de la premiere tache est le plus petit");
        printf("\n\n\n\t 5- QUITTER ");
        printf("\n\n\n\t Veuillez choisir l'une des heuristiques !  - ");
        scanf("%d",&choix);
        switch(choix)
        {
        case 1:
            heursitique1();
            break;
            
        case 2:
            heursitique2();
            break;
        case 3:
            heursitique3();
            break;
        case 4:
            heursitique4();
            break;
        case 5:
            exit(1);
            break;
        default:
            printf("\n\n Veuillez choisir un numero valide !");
            getch();
            break;
        }
    }

    return 0;
}
