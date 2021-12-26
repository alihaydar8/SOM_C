#include<stdio.h>
#include<stdlib.h>
#include <string.h> 
#include <math.h>
#include <time.h>
#include "som.h"

int Rand_int(int min,int max)
{
    int nRand ;
    nRand= min + (int)((float)rand() * (max-min+1) / (RAND_MAX-1)) ;
    return nRand;
}

float Rand_float(float min,float max)
{
    float nRand = (float)rand() / (float) RAND_MAX; 
    return min + nRand * ( max - min );      
}


int main (){
    
    base_vector_data *base = (base_vector_data*)malloc(sizeof(base_vector_data));
    initialise_base(base);
    recover(base,"iris.data");
    // display_vect(v);

    reseau *r = (reseau *)malloc(sizeof(reseau));
    initialise_reseau(r);
    remplire_node(r,base);
    
    apprentissage(r,base);

    display_node_carre(r); 
    display_info(r);
 return 0;
}


