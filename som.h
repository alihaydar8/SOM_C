#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include <time.h>


typedef struct vector_data {
double *x;
double norm;
char *id;
}vector_data;

typedef struct node {
double *w;
double activate;
char *etiquet;
}node;

typedef struct bmu{
    int i;
    int j;
    struct bmu * next;
}bmu;

typedef struct reseau {
    int nb_node;
    int nb_l;
    int nb_c;
    bmu **bmu ;
    node **map;
}reseau;

int Rand_int(int min,int max);
float Rand_float(float min,float max);
double distance_vector(double *x,double *w);
float *vector_moyenne(vector_data *v);
reseau *remplire_node(vector_data *v);
void vector_norm(vector_data *v);
void display_node(reseau *r);
void display_node_carre(reseau *r);
void display_vect(vector_data *v);
void normalize_vect(vector_data *v);
void shuffert(vector_data *v);
int * shuf(int * tab);
vector_data * recover(char *nom_fichier);
bmu *recuperer_BMU(reseau *r , vector_data v);
bmu * bmu_aleatoir(bmu * bu);
void affichage_bmu(bmu *bmu);
void apprentisage(reseau *r,vector_data *v);