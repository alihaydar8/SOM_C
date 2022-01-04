#include<stdio.h>
#include<stdlib.h>
#include <string.h> 
#include <math.h>
#include <time.h>
#include "som.h"

//////// fonction random qui prend deux int et renvoie un int
int Rand_int(int min,int max)
{
    int nRand ;
    nRand= min + (int)((float)rand() * (max-min+1) / (RAND_MAX-1)) ;
    return nRand;
}

/////// fonction random qui prend deux double et renvoie un double 
double Rand_float(double min,double max)
{
    double nRand = (double)rand() / (double) RAND_MAX; 
    return min + nRand * ( max - min );      
}


int main (){
///créer la base de donnée et l'allouer 
    base_vector_data *base = (base_vector_data*)malloc(sizeof(base_vector_data));
    if(base == NULL)
    {
        printf("base de donnée non allouée");
        exit(EXIT_FAILURE);
    }
///allouer la memoire nécessaire pour la base de donnée et l'initialiser
    initialise_base(base,150,4);
///mettre les données des fleurs depuis iris data dans la base et les traiter 
    recover(base,"iris.data");
    // display_vect(v);


///créer le réseau et l'allouer
    reseau *r = (reseau *)malloc(sizeof(reseau));
    if(r == NULL)
    {
        printf("reseau de donnée non alloué");
        exit(EXIT_FAILURE);
    }
//////allouer la memoire nécessaire pour le réseau et l'initialiser
    initialise_reseau(r,6,10,base->col,base->len,0.8,2000);
//////remplire la mattrice alèatoire
    remplir_node(r,base);
/////faire l'apprentissage a la matrice 
    apprentissage(r,base);

    display_node_carre(r); 
    display_info(r);
    free_reseau(r);
    free_base(base);
    
 return 0;
}


