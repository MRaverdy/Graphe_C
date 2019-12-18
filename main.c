#include <stdio.h>
#include "livraison.h"


int main(int argc, char *argv[]){
    struct data init; //graphe initial correspondant à la matrice de distance
    ouverture_fichier(argc,argv,&init);
    int ligne,colonne;
    //On imprime les informations principales :
        printf("nb_client=%d\ncapacité véhicule=%d\nnb de lignes allouées=%d\n",init.nb_client,init.capacite_vehicule,init.nb_ligne);
        printf("quantité demandée par chaque client : ");
        for(ligne=0;ligne<init.nb_client;ligne++){
                printf("%d ",init.q_demand[ligne]);
        }
        printf("\n");
        for(ligne=0;ligne<init.nb_ligne;ligne++){
            for(colonne=0;colonne<init.nb_ligne;colonne++){
                printf("%lf ",init.graf[ligne][colonne]);
            }
            printf("\n\n");
        }
        
        
    //Tour géant :
    printf("Vecteur du tour géant : ");
    int i,j;
    int tour[init.nb_client];
    tour_geant(init, tour, 1);
    for (i=0;i<init.nb_client;i++){
        printf("%d ",tour[i]);}
    printf("\n\n");
    
    //Procédure split :
    struct liste sous_graphe_aux[init.nb_client];
    split(tour, init, sous_graphe_aux);
    for (i=0;i<init.nb_client;i++){
        printf("client %d : ",i);
        imprimer_liste(&sous_graphe_aux[i]);
    }
    printf("\n\n");
    
    //Algorithme de Dijkstra :
    int pere[init.nb_ligne];
    double pi[init.nb_ligne];
    dijkstra(init.nb_ligne, sous_graphe_aux, pere, pi);
    
    printf("Vecteur père : ");
    for (i=0;i<init.nb_ligne;i++){
        printf("%d ",pere[i]);}
    printf("\n");
    
    printf("Vecteur pi : ");
    for (i=0;i<init.nb_ligne;i++){
        printf("%lf ",pi[i]);}
    printf("\n\n");
    
    
    printf("Coût total : %lf\n",pi[init.nb_client]);
    
    
    int longueur_pcc=0;
    i=init.nb_client;
    int pcc[i];
    double cout[i];
    
    while (i!=0){//On récupère ici la longueur du plus court chemin et les couts à l'aide des vecteurs père et pi
        pcc[longueur_pcc]=i;
        cout[longueur_pcc]=pi[i];
        i=pere[i];
        longueur_pcc++;}
        
    pcc[longueur_pcc]=0;
    cout[longueur_pcc]=0;
    
    //On imprime les résultats finaux : les trajets de chaque véhicule avec le coût de chaque trajet.
    for (i=0;i<longueur_pcc;i++){
        printf("Véhicule %d : 0", i+1);
        for (j=pcc[longueur_pcc-i];j<pcc[longueur_pcc-i-1];j++){
            printf("->%d",tour[j]);}
        printf("->0, ");
        printf("Cout : %lf\n", cout[longueur_pcc-i-1]-cout[longueur_pcc-i]);
    }
        
    clear(&init);
return 0;
}


