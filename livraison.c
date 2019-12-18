#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include"livraison.h"

void ouverture_fichier(int argc, char *argv[], struct data *init){
    FILE * fic;
    int ligne,colonne;
    fic = fopen(argv[1],"r");
    if (fic==NULL) printf("le fichier %s est inaccessible",argv[1]);
	else { 
        fscanf(fic,"%d",&init->nb_client);//lecture du premier nombre du fichier qui est égal au nombre de client
        fscanf(fic,"%d",&init->capacite_vehicule);//lecture du second nombre du fichier qui est égal à la capacite_vehiculecité de chaque véhicule
        init->nb_ligne=init->nb_client+1; //nb ligne=nb_col <=> nombre de client +1 car il faut ajouter le dépot

        init->q_demand=(int*)malloc(init->nb_client*sizeof(int));//vecteur du nombre d'unités à livrer
        
        for(ligne=0;ligne<init->nb_client;ligne++){
            fscanf(fic,"%d",&init->q_demand[ligne]);
        }
        //initialisation de la matrice
        init->graf=(double **)malloc(init->nb_ligne*sizeof(double **));
        for(ligne=0;ligne<init->nb_ligne;ligne++){
            init->graf[ligne]=(double *)malloc(init->nb_ligne*sizeof(double));
        }
        for(ligne=0;ligne<init->nb_ligne;ligne++){
            for(colonne=0;colonne<init->nb_ligne;colonne++){
                fscanf(fic,"%lf",&init->graf[ligne][colonne]);
            }
        }
		fclose(fic);
        
    }
}


void clear(struct data *init){//libération des éléments de la structure alloués dynamiquement
    free(init->q_demand); 
    free(init->graf);
}


void tour_geant(struct data data, int *tour, int premier_client){
    bool mark[data.nb_ligne]; //Tableau qui indique pour chaque sommet si il est deja dans le tableau tour.
    double a[data.nb_ligne], dist;//a permet de traiter la matrice de distance par ligne,
    //dist stocke les distances au fur et a mesure pour les comparer.
    mark[0] = true;//Le dépôt est déjà exploré
    int j;
    tour[0]=premier_client;//Initialisation de la première valeur du vecteur tour
    //Initialisation du vecteur mark à false :
    for (int m = 1; m < data.nb_client; m++) mark[m] = false;
    mark[premier_client]=true;
    int k = premier_client; //dernier sommet exploré, initialisé au premier client
    
    for (int i = 1; i < data.nb_client; i++){
        for (j = 0; j <= data.nb_client; j++){//on traite par ligne de la matrice de distance (graf)
            a[j] = data.graf[k][j]; //stockage de la ligne k dans a, k étant le dernier sommet exploré
        }
        dist = (int)INFINITY;//Initialisation de la distance à l'infini
        //boucle qui recherche la plus petite distance à partir du client j :
        for (j = 1; j < data.nb_ligne; j++){
            if ((!mark[j]) && (a[j] < dist)){//Le client ne doit pas être marqué.
                dist = a[j];
                k = j;
            }
        }
        mark[k] = true;
        tour[i] = k;
    }
}
        
        

void ajouter_en_tete(struct liste *sous_graphe_aux, int numclient, double distance){
    struct maillon *nouveau;
    nouveau = (struct maillon*)malloc (sizeof (struct maillon));
    //Affectation des valeurs :
    nouveau->numclient = numclient;
    nouveau->distance = distance;
    nouveau->suivant = sous_graphe_aux->tete;
    //Ajout en tete :
    sous_graphe_aux->tete = nouveau;
    sous_graphe_aux->nbelement += 1;
}
    
    
    
        
void split(int *tour, struct data data, struct liste *sous_graphe_aux){
    int j, load;
    double cost;
    for (int i = 0; i < data.nb_client; i++){
        j = i;
        load = 0; //Un nouveau véhicule est affrété. 
        sous_graphe_aux[i].tete = (struct maillon*)0;
        sous_graphe_aux[i].nbelement = 0;
        do{
            load = load + data.q_demand[tour[j]-1];//-1 parce que le client i correspond à l'indice i-1
            if (i == j){
                //tour[j] est le premier client qui sera livré par le véhicule courant.
                cost = data.graf[0][tour[i]] + data.graf[tour[i]][0];
            }
            else {
                //tour[j] est inséré à la fin de la livraison effectuée par le véhicule courant.
                cost = cost - data.graf[tour[j-1]][0] + data.graf[tour[j-1]][tour[j]] + data.graf[tour[j]][0];
            }
            if (load <= data.capacite_vehicule) {
                //La livraison de tour[j] peut être assurée par le véhicule courant.
                ajouter_en_tete(&sous_graphe_aux[i], j+1, cost);}
            j++;
        }
        while ((j < data.nb_client) && (load < data.capacite_vehicule)); //La livraison de tous les clients a été planifiée -ou- la livraison du client tour[j] entraîne une violation de la contrainte de capacité.
    }
}
        
        
        
void imprimer_liste (struct liste *L)
{   struct maillon *M;
    int i;

    printf ("[");
    M = L->tete;//M sert a parcourir la liste chainée
    for (i = 0; i < L->nbelement; i++)
    {   if (i == 0)
            printf ("client %d : %lf", M->numclient, M->distance);
        else
            printf (", client %d : %lf", M->numclient, M->distance);
        M = M->suivant;
    }
    printf ("]\n");
}

        
void dijkstra(int nb_ligne, struct liste *sous_graphe_aux, int *pere, double *pi){
    bool mark[nb_ligne]; //sommets fixés
    bool fini;//marqueur de fin
    int x;//x est le sommet actuel
    int j;
    struct maillon *y;//y permet d'accéder à la liste chainée du sommet actuel
    mark[0] = false;
    pere[0] = 0;//Le vecteur père correspond aux pères de chaque sommet, prédecesseurs
    pi[0] = 0; //Le vecteur pi correspond au coût du plus court chemin pour chaque sommet
    
    fini = false;
    for (int i = 1; i < nb_ligne; i++){//initialisation de l'ensemble des valeurs des vecteurs
        mark[i] = false;
        pere[i] = 0;
        pi[i] = INFINITY;
    }
    do{
        fini = true;
        x = 0;
        while ((x < nb_ligne-1) && (mark[x] == true)){//boucle pour choisir x tel que Mark[x]=faux
            x++;
        }
        if (mark[x] == false){ //sert uniquement pour le dernier tour quand tous les sommets sont marqués. Si cette condition n'est pas vérifiée, fini reste égal à true et la boucle s'arrête.
            for (j = x + 1; j < nb_ligne-1; j++){//boucle pour essayer de trouver x tel que pi[x] minimum (avec Mark[x]=faux toujours vérifié).
                if ((mark[j] == false) && (pi[j] < pi[x])) x = j;
            }
            fini = false;//Un sommet est toujours non marqué, la boucle ne s'arrête pas.
            mark[x] = true;//On marque le sommet actuel.
            y = sous_graphe_aux[x].tete;//y accède à la liste chainée du sommet actuel
            for (int k = 0; k < sous_graphe_aux[x].nbelement; k++){//boucle qui parcourt tous les maillons de la liste chainée du sommet actuel.
                if (pi[y->numclient] > pi[x] + y->distance){//si on trouve un potentiel plus petit, on remplace
                    pi[y->numclient] = pi[x] + y->distance;
                    pere[y->numclient] = x;
                }
                y = y->suivant;
            }
        }
    }
    while (!fini);
}
        
        
