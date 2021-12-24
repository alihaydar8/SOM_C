#include<stdio.h>
#include<stdlib.h>
#include <string.h> 
#include <math.h>
#include <time.h>
#include "som.h"

int Rand_int(int min,int max)
{
    int nRand ;
    // srand(time(NULL));
    nRand= min + (int)((float)rand() * (max-min+1) / (RAND_MAX-1)) ;
    return nRand;
}

float Rand_float(float min,float max)
{
    // srand(time(NULL));
    float nRand = (float)rand() / (float) RAND_MAX; 
    return min + nRand * ( max - min );      
}

int main (){
    
    vector_data *v;
    v = recover("iris.data");
    // double x =;
    //printf("%f",sizeof(*v));
    normalize_vect(v);
    // shuffert(v);
    // display_vect(v);
    reseau *r = remplire_node(v);
    // printf("\n\n");
    // display_node_carre(r);
    r->bmu=(bmu**)malloc(150*sizeof(bmu));
    apprentisage(r,v);
    
    // for(int i=0;i<150;i++){
    //     affichage_bmu(bmu_aleatoir(recuperer_BMU(r,v[i])));
    //     printf("\n");
    // }

    display_node_carre(r);
    printf("\n");
    
    printf("\x1B[34m");
    printf("g pour Iris-virginica  |");
    printf("\x1B[0m");

    printf("\x1B[31m");
    printf("|  S pour Iris-versicolor  |"); 
    printf("\x1B[0m");

    printf("\x1B[32m");
    printf("|  o pour Iris-setosa ");
    printf("\x1B[0m");
    
    printf("\n");
    
     
 return 0;
}



