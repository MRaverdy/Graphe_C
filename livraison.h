#if ! defined (LIVRAISON_H)
#define LIVRAISON_H 1


struct data{
    double **graf;
    int *q_demand;
    int nb_ligne;
    int nb_client;
    int capacite_vehicule;
};


struct liste{
    int nbelement;
    struct maillon *tete;};
    
struct maillon{
    int numclient;
    double distance;
    struct maillon *suivant;};
    

extern void ouverture_fichier(int , char *argv[], struct data *);
extern void clear(struct data *);

extern void lecture(double**, int*, int*, int*);
extern void dijkstra(int, struct liste*, int*, double*);
extern void split(int*, struct data, struct liste*);
extern void tour_geant(struct data, int*, int);
extern void ajouter_en_tete(struct liste*, int, double);
extern void imprimer_liste (struct liste *);
#endif
