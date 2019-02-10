#include "Ordonnancement.h"

ILOSTLBEGIN //remplace dans les .cpp usign namespace std;

/////////////////////////////////
//// CONSTRUCTEUR PAR DEFAUT ////
/////////////////////////////////

Ordonnancement::Ordonnancement() {
	cout << endl << " // Constructeur par defaut " << endl;

	nbrJob = 4;
	nbrMachine = 3;
	maxVal = 1000;
	// les debut et fin d'indisponibilité sur les machines
	Tij = IloArray<IloNumArray>(env, nbrMachine);
	for (int i = 0; i < nbrMachine; i++) {
		Tij[i] = IloNumArray(env, 2);
	}
	Tij[0][0] = 3;		Tij[0][1] = 5;  //machine 1
	Tij[1][0] = 8;		Tij[1][1] = 10;	 //machine 2
	Tij[2][0] = 5;		Tij[2][1] = 9;  //machine 3

	//les durees operatoires taches sur chaque job
	Pij = IloArray<IloNumArray>(env, nbrJob);
	for (int i = 0; i < nbrJob; i++) {
		Pij[i] = IloNumArray(env, nbrMachine);
	}
	Pij[0][0] = 1;	Pij[0][1] = 9;	Pij[0][2] = 5;
	Pij[1][0] = 2;	Pij[1][1] = 4;	Pij[1][2] = 6;
	Pij[2][0] = 10;	Pij[2][1] = 3;	Pij[2][2] = 7;
	Pij[3][0] = 4;	Pij[3][1] = 1;	Pij[3][2] = 8;


	//fichier LP
	nfLP = "lp/pb.lp";

	cpuMax = 20;  //60 sec. par défaut, temps max de résolution.
	affichageTraceMIP = 2; // 0 pour désactiver

	cout << " // FIN Constructeur par defaut." << endl;
}

////////////////////////////////////////////
//// CONSTRUCTEUR A PARTIR D'UN FICHIER ////
////////////////////////////////////////////

Ordonnancement::Ordonnancement(std::string nFichier) {

	cout << endl << " // Constructeur avec Fichier  .dat ... " << endl;

	//parametre par defaut
	maxVal = IloInfinity;
	nfData = "data/nFichier.dat";
	nfLP = "lp/pb.lp";
	cpuMax = 20;  //60 sec. par défaut, temps max de résolution.
	affichageTraceMIP = 2; // 0 pour désactiver

	ifstream ifData(nFichier, ios::in); //Ouverture du fichier donné en argument

	if (!ifData) {
		cout << endl << "--!! ERREUR ouverture fichier : " << nFichier << " !! " << endl << endl;
	}
	else {
		if (nFichier.find(".dat") == std::string::npos) {
			//si ce n'est pas fichier de type data  alors ERREUR
			cout << endl << "--!! ERREUR type de fichier !! non data !! (" << nFichier << ")" << endl << endl;
		}
		else
		{
			ifData >> nbrJob;
			ifData >> nbrMachine;

			//indisponibilite
			Tij = IloArray<IloNumArray>(env, nbrMachine);
			for (int i = 0; i < nbrMachine; i++) 
				Tij[i] = IloNumArray(env, 2);

			for (int i = 0; i < nbrMachine; i++)
				ifData >> Tij[i][0] >> Tij[i][1];
			
			//duree operatoire des taches
			Pij = IloArray<IloNumArray>(env, nbrJob);
			for (int i = 0; i < nbrJob; i++) 
				Pij[i] = IloNumArray(env, nbrMachine);

			for (int i = 0; i < nbrJob; i++) 
				ifData >> Pij[i][0] >> Pij[i][1] >> Pij[i][2];

			ifData.close();
		}
	}
	cout << " // FIN Constructeur avec nomFichier." << endl;
}

////////////////////////////////////////////////
////DESTRUCTEUR DE LA CLASSE ORDONNANCEMENT ////
////////////////////////////////////////////////

Ordonnancement::~Ordonnancement() {
	//Destructeur
}

//////////////////////////////////////////////////
////METHODES POUR LA CONSTRUCTION DES MODELES ////
//////////////////////////////////////////////////

void Ordonnancement::Construire_Modele() {

	cout << endl << " >>>>> Construction modèle <<<<<<  " << endl;
	try {
		///modele de cplex
		mPb = IloModel(env); //pointeur vers l'objet contenant le modèle 
		iPb = IloCplex(mPb); //associe le modèle à une insance à résoudre
		bestVal = IloInfinity; //valeur optimale initiale 

		///VARIABLES...
		varXij1 = IloArray<IloBoolVarArray>(env, nbrJob);
		varXij2 = IloArray<IloBoolVarArray>(env, nbrJob);
		varXij3 = IloArray<IloBoolVarArray>(env, nbrJob);
		for (int i = 0; i < nbrJob ; i++)
		{
			varXij1[i] = IloBoolVarArray(env, nbrJob);
			varXij2[i] = IloBoolVarArray(env, nbrJob);
			varXij3[i] = IloBoolVarArray(env, nbrJob);
		}
		varYjk = IloArray<IloBoolVarArray>(env, nbrJob);
		for (int j = 0; j < nbrJob; j++)
		{
			varYjk[j] = IloBoolVarArray(env, nbrMachine);
		}
		varCij = IloArray<IloNumVarArray>(env, nbrJob);
		for (int i = 0; i < nbrJob; i++)
		{
			varCij[i] = IloNumVarArray(env, nbrMachine, 0, IloInfinity, ILOINT);
		}

		///CONTRAINTES...
		ctrOrdo = IloRangeArray(env, 77);
		IloExpr expression;
		//Contraintes de début d'indisponibilité
		for (int k = 0; k < nbrMachine; k++) {
			expression = varYjk[0][k] * Pij[0][k];
			for (int j = 1; j < nbrJob; j++)
				expression += varYjk[j][k] * Pij[j][k];
			ctrOrdo[k] = IloRange(env, 0, expression, Tij[k][0]);
			expression.end();
		}
		
		int ctr = nbrMachine;
		//Contrainte de fin d'indisponibilite
		for (int k = 0; k < nbrMachine; k++)
		{
			for (int j = 0; j < nbrJob; j++)
				ctrOrdo[ctr++] = IloRange(env,0 , varCij[j][k] - Tij[k][1] - Pij[j][k] + varYjk[j][k] * maxVal, IloInfinity);
		}
		
		//Contrainte de precedence
		//Dans le premier programme nous avons laissé cplex gerer les parentheses avec le maxVal 
		//dans ce programme nous avons modifier la variable Xij qui est maintenant egal à 1 si i > j pour détourner la contrainte (Xij - 1) * MaxVal
		//Ainsi la contrainte devient -Xij * MaxVal --> la conséquence est que le résultat monte de 82 à 101 qui est vérifié par la génétique. 
		for (int k = 0; k < nbrMachine; k++)
		{
			for (int i = 0; i < nbrJob; i++) {
				for (int j = 0; j < nbrJob; j++) {
					if (i != j) {
						if (k == 0)
							ctrOrdo[ctr++] = IloRange(env, 0, varCij[j][k] - varCij[i][k] - Pij[j][k] + maxVal * varXij1[i][j]  , IloInfinity);
						else if (k == 0)
							ctrOrdo[ctr++] = IloRange(env, 0, varCij[j][k] - varCij[i][k] - Pij[j][k] + maxVal * varXij2[i][j]  , IloInfinity);
						else
							ctrOrdo[ctr++] = IloRange(env, 0, varCij[j][k] - varCij[i][k] - Pij[j][k] + maxVal * varXij3[i][j]  , IloInfinity);
					}
				}
			}
		}
	//Contrainte de precedenes des taches d'un job
		for (int k = 1; k < nbrMachine; k++)
		{
			for (int j = 0; j < nbrJob; j++)
				ctrOrdo[ctr++] = IloRange(env, 0, varCij[j][k] - varCij[j][k - 1] - Pij[j][k], IloInfinity);
		}
		//Contraintes d’exclusivite des variables sur la position des taches d'une machine
		for (int i = 0; i < nbrJob - 1; i++) {
			for (int j = i+1; j < nbrJob; j++) {
				ctrOrdo[ctr++] = IloRange(env, 1, varXij1[i][j] + varXij1[j][i], 1);
				ctrOrdo[ctr++] = IloRange(env, 1, varXij2[i][j] + varXij2[j][i], 1);
				ctrOrdo[ctr++] = IloRange(env, 1, varXij3[i][j] + varXij3[j][i], 1);
			}
		}

		mPb.add(ctrOrdo);
		///OBJECTIF
		///Minimiser la date de fin des taches sur la derniere machine
		expression = varCij[0][nbrMachine - 1];
		for (int i = 1; i < nbrJob; i++)
			expression += varCij[i][nbrMachine - 1];
		obj = IloObjective(env, expression, IloObjective::Minimize);
		mPb.add(obj);

		//export du modele dans un fichier au fomat lp
		iPb.exportModel(nfLP.c_str());
	}
	catch (IloException & e) {
		cerr << "[Construction modele] > ERREUR : exception = " << e << endl;
	}
	cout << "  >>>>> Fin Construction modele <<<<<" << endl;
}

//////////////////////////////////////////////////
////METHODES POUR LA RESOLUTION DES MODELES   ////
//////////////////////////////////////////////////

void Ordonnancement::Resoudre_Modele() {

	cout << endl << " >>>>>> Resoltution Modele <<<<<<<< " << endl;
	//// RESOLUTION... 
	try { //pour capter et afficher une erreur d'execution

		iPb.setParam(IloCplex::TiLim, cpuMax); //cpu max en secondes
		iPb.setParam(IloCplex::MIPDisplay, affichageTraceMIP); // = 0 pour désavtiver
		iPb.solve(); //Résoudre...
	}
	catch (IloException & e) {
		cerr << "[Resoudre_Modele_General()] > ERREUR : exception = " << e << endl;
	}
	cout << " >>>>>> FIN Resolution Modele <<<<<<" << endl;

}