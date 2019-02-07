#include <stdio.h>
#include <stdlib.h>

int* randomchromosome();
void permuter(int* tab,int* tabr);
void indiceContrainte(int * tab,int * tabr,int v1,int v2,int v3);
int* generereChromosomeRealisable();
int compareChromosome(int* tab1, int* tab2);
void population(int** matrice);
void afficherChromosome(int* tab);
void trier(int*, int taille);
int estPresent(int* tab, int val, int taille);
int recherchePosition(int* chr,int taille, int val);
int ** ordonnancement(int * chromosome, int* dureeOp, int* tempIndis);
int fitness(int* chromosome, int* dureeOp, int* tempIndis);
void population(int** matrice);
void croisement(int* parent1, int* parent2, int* enfant1, int* enfant2);
void mutation(int* parent, int* mutant);
void selection(int** pop, int* dureeOp, int* tempIndis);
void algorithmeGenetique(int ** pop, int* dureeOp, int* tempIndis);
int main()
{
    int i;
    int * dureOperatoire;
    int * indisponibilite;
    int ** popula;
    int fit;
    int ** ordo;
    srand(time(NULL));
    //dure operatoire
    dureOperatoire = (int *)malloc(12 * sizeof(int));
    dureOperatoire[0] = 1;          dureOperatoire[1] = 9;              dureOperatoire[2] = 5;
    dureOperatoire[3] = 2;          dureOperatoire[4] = 4;              dureOperatoire[5] = 6;
    dureOperatoire[6] = 10;         dureOperatoire[7] = 3;              dureOperatoire[8]= 7;
    dureOperatoire[9] = 4;          dureOperatoire[10] = 1;             dureOperatoire[11] = 8;
    //indisponibilite
    indisponibilite = (int *)malloc(6 *sizeof(int));
    indisponibilite[0] = 3;         indisponibilite[1] = 5;             indisponibilite[2] = 8;
    indisponibilite[3] = 10;        indisponibilite[4] = 5;              indisponibilite[5] = 9;
    //population
    popula = (int**)malloc(25 * sizeof(int*));
    for(i=0; i < 25; i++)
        popula[i] =  (int*)malloc(12 * sizeof(int));
    //execution de l'algorithme genetique
    algorithmeGenetique(popula,dureOperatoire, indisponibilite);

    //affichage de meilleur resultat
    printf("le chromosome du meilleur resultat\n");
    afficherChromosome(popula[0]);
    fit = fitness(popula[0], dureOperatoire, indisponibilite);
    printf("la somme des dates de fin des jobs sont: %d \n", fit);
    ordo = (int**)malloc(6*sizeof(int*));
    for(i=0; i < 6; i++)
        ordo[i] = (int*)malloc(4*sizeof(int));
    ordo = ordonnancement(popula[0],dureOperatoire, indisponibilite);
    printf("Au niveau de la premiere machine\n");
    printf("Numero de tache ==> date de fin\n");
    for(i=0; i < 4; i++)
        printf("%d ==> %d \n",ordo[0][i], ordo[1][i]);
    printf("Au niveau de la deuxième machine\n");
    printf("Numero de tache ==> date de fin\n");
    for(i=0; i < 4; i++)
        printf("%d ==> %d\n",ordo[2][i], ordo[3][i]);
    printf("Au niveau de la troisieme machine\n");
    printf("Numero de tache ==> date de fin\n");
    for(i=0; i < 4; i++){
        printf("%d ==> %d\n",ordo[4][i], ordo[5][i]);
    }



    free(popula);
    free(indisponibilite);
    free(dureOperatoire);
    return 0;
}
void afficherChromosome(int* tab){
    int i=0;
    for(i = 0; i < 12; i++)
        printf(" %d", tab[i]);
    printf("\n");
}
void trier(int* tabr, int taille){
    int i,j, mini;
    int inter;
    for(i=0; i < taille-1 ; i++){
        mini = i;
        for(j=i + 1; j < taille; j++)
            if(tabr[j] < tabr[mini])
                mini = j;
        inter = tabr[i];
        tabr[i] = tabr[mini];
        tabr[mini] = inter;
    }
}
int recherchePosition(int* chr,int taille, int val){
    int re = -1;
    int i;
    for(i=0; i < taille; i++)
        if(chr[i] == val)
            return i;
    return re;
}
int* randomchromosome(){

	int * tab = (int *)malloc(12 * sizeof(int));
	int i = 1;
	int j = 0;
	int k, vrai;
	int val;
	while (i!= 0 )
	{
		vrai = 0;
		val = rand() % 12 + 1;
		k = 0;
		while(k <j && vrai == 0)
		{
			if( tab[k] == val )
				vrai = 1 ;
			k++;
		}
		if(vrai != 1)
		{
			tab[j] = val ;
			j++;
		}
		if(j == 12)
			i = 0;
	}
	return tab ;
}
void permuter(int* tab,int* tabr){
    int v1, v2, v3;
    if(tabr[0] > tabr[1] || tabr[1] > tabr[2] || tabr[0]>tabr[2]){
            v1 = tab[tabr[0]];
            v2 = tab[tabr[1]];
            v3 = tab[tabr[2]];
            trier(tabr,3);
            tab[tabr[0]] = v1;
            tab[tabr[1]] = v2;
            tab[tabr[2]] = v3;
    }
}
void indiceContrainte(int * tab,int * tabr,int v1,int v2,int v3){
    int i;
    for(i=0; i < 12; i++){
		if(tab[i] ==v1)
			tabr[0] = i;
		if(tab[i] == v2)
			tabr[1] = i;
		if(tab[i] == v3)
			tabr[2] = i;
	}
}
int* generereChromosomeRealisable(){

	int* tabr=(int*)malloc(3*sizeof(int));
    int* tab=(int*)malloc(12*sizeof(int));

    tab = randomchromosome();
	//contrainte 1 <2 <3
	//les positions de 1, 2 et 3
    indiceContrainte(tab,tabr, 1, 2, 3);
	if( tabr[0] > tabr[1] || tabr[1] > tabr[2] || tabr[0] > tab[2])
		permuter(tab, tabr);
    // contrainte 4 < 5 <6
    	//les positions de 4,5 et 6
    indiceContrainte(tab,tabr, 4, 5, 6);
	if( tabr[0] > tabr[1] || tabr[1] > tabr[2] || tabr[0] > tab[2])
		permuter(tab, tabr);
    //contrainte 7 < 8 < 9
    //les positions de 7, 8 et 9
	indiceContrainte(tab,tabr, 7,8,9);
	if( tabr[0] > tabr[1] || tabr[1] > tabr[2] || tabr[0] > tab[2])
		permuter(tab, tabr);
    //contrainte 10 < 11 <12
    //les positions de 10, 11 et 12
    indiceContrainte(tab,tabr, 10,11,12);
	if( tabr[0] > tabr[1] || tabr[1] > tabr[2] || tabr[0] > tab[2])
		permuter(tab, tabr);
    return tab;
}
int compareChromosome(int* tab1, int* tab2){
    int i = 0;
    int test = 0;
    while(test ==0 && i < 12){
        if(tab1[i]!=tab2[i])
            return test = 1;
        i++;
    }
    return test;
}
void population(int** matrice){
    int j, i = 0;
    int * tabInter ;
    int test;
    while(i < 25){
        test = 0;
        tabInter = (int *)malloc(12 * sizeof(int));
        tabInter = generereChromosomeRealisable();
        for(j=0; j < i; j++)
            if(compareChromosome(matrice[j],tabInter) == 0)
                test = 1;
        if(test == 0){
            for(j=0; j < 12; j++)
                matrice[i][j] = tabInter[j];
            i++;
        }
        tabInter = NULL;

    }
}
int estPresent(int* tab, int val, int taille){
    int i;
    int test = 0;
    for(i=0; i < taille ; i++){
        if(tab[i] == val)
            return test = 1;
    }
    return test;
}
int fitness(int* chromosome, int* dureeOp, int* tempIndis){
    int i, fit;
    int** ordo = (int**)malloc(6 * sizeof(int*));
    for(i=0; i < 6 ; i++){
        ordo[i] = (int*)malloc(4 * sizeof(int));
    }
    ordo = ordonnancement(chromosome, dureeOp, tempIndis);
    fit = 0;
    for(i=0; i < 4; i++){
        fit = fit + ordo[5][i];
    }
    free(ordo);
    return fit;
}
void croisement(int* parent1, int* parent2, int* enfant1, int* enfant2){
    int pointCoupure, i, j, k;
    //prendre une valeur compris entre 3 et 9
    pointCoupure = (rand() % 8) + 2;
    for(i = 0; i < pointCoupure; i++){
        enfant1[i] = parent1[i];
        enfant2[i] = parent2[i] ;
    }
    j = pointCoupure;
    k = pointCoupure;
    for(i=0; i < 12; i++){
        if(!estPresent(enfant1, parent2[i], j))
            enfant1[j++] = parent2[i];
        if(!estPresent(enfant2, parent1[i], k))
            enfant2[k++] = parent1[i];
    }
}
void mutation(int* parent, int* mutant){
    int pointMutant, i, inter, vrai;
    for(i=0; i < 12 ; i++){
        mutant[i] = parent[i];
    }
    vrai = 0;
    while(!vrai){
        //prendre une valeur compris entre 3 et 9
        pointMutant = (rand() % 8) + 2;
        if(mutant[pointMutant+1] != mutant[pointMutant] + 1 && mutant[pointMutant+1] != mutant[pointMutant] + 2){
            inter = mutant[pointMutant];
            mutant[pointMutant] = mutant[pointMutant+1];
            mutant[pointMutant+1] = inter;
            vrai = 1;
        }
    }
}
void selection(int** pop, int* dureeOp, int* tempIndis){
    int i,j,mini,itt;
    int * inter = (int*)malloc(12 * sizeof(int));
    int * fit = (int*)malloc(25 * sizeof(int));
    for(i=0; i < 25; i++)
        fit[i] = fitness(pop[i],dureeOp, tempIndis);
    for(i=0; i < 25 - 1; i++){
        mini = i;
        for(j=i+1; j < 25; j++)
            if(fit[j] < fit[mini])
                mini = j;
        inter = pop[i];
        pop[i] = pop[mini];
        pop[mini] = inter;
        itt = fit[i];
        fit[i] = fit[mini];
        fit[mini] = itt;
    }
    inter = NULL;
}
int ** ordonnancement(int * chr, int* dureeOp, int* tempIndis){
    int i, m1, m2, m3,v2;
    int** ordo = (int**)malloc(6 * sizeof(int*));
    for(i=0; i < 6 ; i++){
        ordo[i] = (int*)malloc(4 * sizeof(int));
    }
    m1 = 0;
    m2 = 0;
    m3 = 0;
    for(i=0; i< 12; i++)
    {
        if(chr[i] == 1 || chr[i] == 4 || chr[i] == 7 || chr[i] == 10)
        {
           ordo[0][m1] = chr[i];
           if(m1 ==0)
           {
                if(dureeOp[chr[i]-1] <= tempIndis[0])
                    ordo[1][m1++] = dureeOp[chr[i]-1];
                else
                    ordo[1][m1++] = tempIndis[1] + dureeOp[chr[i]-1];
           }
           else
           {
                if(tempIndis[0] >= ordo[1][m1-1] + dureeOp[chr[i]-1]){
                    ordo[1][m1] = ordo[1][m1-1] + dureeOp[chr[i]-1];
                    m1++;
                }
                else if(ordo[1][m1-1] <= tempIndis[0])
                    ordo[1][m1++] = tempIndis[1] + dureeOp[chr[i]-1];
                else{
                    ordo[1][m1] = ordo[1][m1-1] + dureeOp[chr[i]-1];
                    m1++;
                }
           }
        }
        else if(chr[i] == 2 || chr[i] == 5 || chr[i] == 8 || chr[i] == 11)
        {
            ordo[2][m2] = chr[i];
            if(m2 == 0)
            {
                v2 = recherchePosition(ordo[0], 4, chr[i]-1);
                if(ordo[1][v2] <= tempIndis[3])
                {
                    if(tempIndis[2] >= ordo[1][v2] + dureeOp[chr[i]-1])
                        ordo[3][m2++] = ordo[1][v2] + dureeOp[chr[i]-1];
                    else
                        ordo[3][m2++] = tempIndis[3] + dureeOp[chr[i]-1];
                }
                else
                    ordo[3][m2++] = ordo[1][v2] + dureeOp[chr[i]-1];
            }
            else
            {
                v2 = recherchePosition(ordo[0], 4, chr[i]-1);
                if(ordo[1][v2] <= ordo[3][m2 - 1])
                {
                    if(ordo[3][m2-1] <= tempIndis[2])
                    {
                        if(ordo[3][m2-1] + dureeOp[chr[i]-1] <= tempIndis[2]){
                            ordo[3][m2] = ordo[3][m2-1] + dureeOp[chr[i]-1];
                            m2++;
                        }
                        else
                            ordo[3][m2++] = dureeOp[chr[i]-1] + tempIndis[3];
                    }
                    else{
                       ordo[3][m2] = dureeOp[chr[i]-1] + ordo[3][m2-1];
                       m2++;
                    }
                }
                else if(ordo[3][m2-1] <= tempIndis[3])
                {
                        if(ordo[1][v2] + dureeOp[chr[i]-1] <= tempIndis[2])
                            ordo[3][m2++] = ordo[1][v2] + dureeOp[chr[i]-1];
                        else
                            ordo[3][m2++] =  dureeOp[chr[i]-1] + tempIndis[3];
                }
                else
                    ordo[3][m2++] = dureeOp[chr[i]-1] + ordo[1][v2];
            }
        }
        else{
                ordo[4][m3] = chr[i];
            if(m3 == 0)
            {
                v2 = recherchePosition(ordo[2], 4, chr[i]-1);
                if(ordo[3][v2] <= tempIndis[5])
                {
                    if(tempIndis[4] >= ordo[3][v2] + dureeOp[chr[i]-1])
                        ordo[5][m3++] = ordo[3][v2] + dureeOp[chr[i]-1];
                    else
                        ordo[5][m3++] = tempIndis[5] + dureeOp[chr[i]-1];
                }
                else
                    ordo[5][m3++] = ordo[3][v2] + dureeOp[chr[i]-1];
            }
            else
            {
                v2 = recherchePosition(ordo[2], 4, chr[i]-1);
                if(ordo[3][v2] <= ordo[5][m3-1])
                {
                    if(ordo[5][m3-1] <= tempIndis[4])
                    {
                        if(ordo[5][m3-1] + dureeOp[chr[i]-1] <= tempIndis[4]){
                            ordo[5][m3] = ordo[5][m3-1] + dureeOp[chr[i]-1];
                            m3++;
                        }
                        else
                            ordo[5][m3++] = dureeOp[chr[i]-1] + tempIndis[5];
                    }
                    else{
                       ordo[5][m3] = dureeOp[chr[i]-1] + ordo[5][m3-1];
                       m3++;
                    }
                }
                else if(ordo[5][m3-1] <= tempIndis[5])
                {
                        if(ordo[3][v2] + dureeOp[chr[i]-1] <= tempIndis[4])
                            ordo[5][m3++] = ordo[3][v2] + dureeOp[chr[i]-1];
                        else
                            ordo[5][m3++] =  dureeOp[chr[i]-1] + tempIndis[5];
                }
                else
                    ordo[5][m3++] = dureeOp[chr[i]-1] + ordo[3][v2];
            }

        }
    }
    return ordo;
}

void algorithmeGenetique(int ** pop, int* dureeOp, int* tempIndis){
    int i, j;
    population(pop);
    for(i=0; i < 20; i++){
        selection(pop, dureeOp, tempIndis);
        for(j=0; j< 10; j++){
            croisement(pop[j], pop[j+1], pop[10+j], pop[10+j+1]);
        }
        for(j=10; j < 15; j++){
            mutation(pop[j], pop[10+j]);
        }
    }
    selection(pop, dureeOp, tempIndis);
}
