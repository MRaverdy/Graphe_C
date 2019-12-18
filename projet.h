//projet.h
#include <stdio.h>
#include <stdlib.h>

struct data{
    double **graf;
    int *q_demand;
    int nb_ligne;
    int nb_client;
    int capacite_vehicule;
};


extern void ouverture_fichier(int , char *argv[], struct data *);
extern void clear(struct data *);
