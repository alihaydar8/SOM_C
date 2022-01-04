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
    int len;//150
    int col;//4
}base_vector_data;

typedef struct node {
double *w;
double activate;
char etiquet;
}node;

typedef struct bmu{
    int i;
    int j;
    struct bmu * next;
}bmu;

typedef struct reseau {
    int nb_l;//10
    int nb_c;//6
    int col;//4
    int iteration;
    double alpha_initial;
    bmu **bmu ;
    node **map;
}reseau;

/////////initialisation
void initialise_base(base_vector_data *base,int len,int col);
void initialise_reseau(reseau *r,int nb_c,int nb_l,int col,int len,double alpha_initial,int iteration );

/////////traville sur la base
int Rand_int(int min,int max);
double Rand_float(double min,double max);
double distance_vector(double *x,double *w,int col);
void vector_norm(base_vector_data *base);
void normalize_vect(base_vector_data *base);
void vector_moyenne(base_vector_data *base, double *tab);
void remplire_tableau(int * tab,int size_row);
void shuff(int * tab,int size_row);

void recover(base_vector_data *base, char *nom_fichier);
void remplir_node(reseau *r,base_vector_data *base);

//////////// affichages
void display_node_carre(reseau *r);
void display_vect(base_vector_data *base);
void affichage_bmu(bmu *bmu);
void display_info(reseau *r);

//////////////trouver le bmu
bmu *recuperer_BMU(reseau *r , vector_data v);
bmu * bmu_aleatoir(bmu * bu);

///////////////apprentissage
int calcule_rayon(reseau *r);
void apprentissage(reseau *r,base_vector_data *base);

//////// free
void free_base(base_vector_data *base);
void free_reseau(reseau *r);