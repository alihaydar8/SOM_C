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

typedef struct base_vector_data {
    vector_data *v;
    int len;
    int col;
}base_vector_data;

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
    int nb_l;
    int nb_c;
    bmu **bmu ;
    node **map;
}reseau;

/////////initialisation
void initialise_base(base_vector_data *base);
void initialise_reseau(reseau *r);

/////////traville sur la base
int Rand_int(int min,int max);
float Rand_float(float min,float max);
double distance_vector(double *x,double *w);
void vector_norm(base_vector_data *base);
void normalize_vect(base_vector_data *base);
float *vector_moyenne(base_vector_data *base);
int * shuf(int * tab);

void recover(base_vector_data *base, char *nom_fichier);
void remplire_node(reseau *r,base_vector_data *base);

//////////// affichages
void display_node_carre(reseau *r);
void display_vect(base_vector_data *base);
void affichage_bmu(bmu *bmu);
void display_info(reseau *r);

//////////////trouver le bmu
bmu *recuperer_BMU(reseau *r , vector_data v);
bmu * bmu_aleatoir(bmu * bu);

///////////////apprentissage
void apprentissage(reseau *r,base_vector_data *base);

