#include<stdio.h>
#include<stdlib.h>
#include <string.h> 
#include <math.h>

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

double distance(double *x,double *w){
    double distance = 0.0;
    for (int i = 0; i < 4;i++){
        distance = distance + (x[i] - w[i]) * (x[i] - w[i]);
    }
    distance = sqrt(distance);
    return distance;
}

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
    bmu bmu ;
    node **map;
}reseau;

float *vector_moyenne(vector_data *v){
    int size_col = 4;
    int size_row = 150;
    float *tab =(float*)malloc(4*sizeof(float));
    for(int j=0;j<size_col;j++){
        for(int i = 0;i<size_row;i++){
            tab[j]= tab[j] + v[i].x[j];
        }
        tab[j]=tab[j]/size_row;
    }   
    return tab;
}

reseau remplire_node(vector_data *v){
    reseau r;
    r.nb_node=63;
    r.nb_c=7;
    r.nb_l=9;
    int size_col = 4;
    float *tab = vector_moyenne(v);
    node **map = (node **)malloc(9*sizeof(node));

    for(int i=0;i<r.nb_l;i++){
        map[i] =(node *)malloc(7*sizeof(node));
        for(int j=0;j<r.nb_c;j++){
            map[i][j].w = (double *)calloc(4, sizeof(double));
            for(int k=0;k<size_col;k++){
                map[i][j].w[k]=Rand_float(tab[k]-0.02,tab[k]+0.02);
            }
        }
    }
    r.map = map;
    return r;
}

void vector_norm(vector_data *v){
    int size_row = 150;
	int size_col = 4;
    for(int i=0;i<size_row;i++){
        for(int j=0;j<size_col;j++){
            v[i].norm=v[i].norm+ v[i].x[j]*v[i].x[j];
        }
        v[i].norm = sqrt(v[i].norm);
    }
}

void display_node(reseau r){
	int size_col = 4;
    for(int i=0;i<r.nb_l;i++){
        printf("node[%d]:",i);
        for(int j=0;j<r.nb_c;j++){
            for(int k=0;k<size_col;k++){
                printf("%.3f ",r.map[i][j].w[k]);
            }
            printf(" ");
        }
        printf("\n\n");
    }
}

void display_vect(vector_data *v){
    int size_row = 150;
	int size_col = 4;
    for(int i=0;i<size_row;i++){
        printf("vecteur %d  ",i);
        for(int j=0;j<size_col;j++){
           printf("%f ",v[i].x[j]);
        }
        printf("%s ",v[i].id);
        printf("%f",v[i].norm);
        printf("\n");
    }
}

void normalize_vect(vector_data *v){
    int size_row = 150;
	int size_col = 4;
    for(int i=0;i<size_row;i++){
        for(int j=0;j<size_col;j++){
            v[i].x[j]=v[i].x[j]/v[i].norm;
        }
    }   
}

void shuffert(vector_data *v){
    int size_row = 150;
	int size_col = 4;
    vector_data c ;
    int rand=0;
    for(int i=0;i<size_row;i++){
        for(int j=0;j<size_col;j++){
        rand =Rand_int(0,149);
        c = v[i];
        v[i]=v[rand];
        v[rand]=c;
        }
    }
}

vector_data * recover(char *string){
    int i = 0;
    int j=0; 
    char tab[60];
    char *tab1[150];
    int size_row = 150;
    FILE *f= fopen( string, "r" );
    vector_data *v;
    if(!f){
        printf("fichier n'existe pas");
    }
    v = (vector_data *)calloc(150, sizeof(vector_data));
    if(v==NULL){
        printf("null");
    }
    fseek(f,0,SEEK_SET);
	while((fgets(tab,60,f)!=NULL) && (i<size_row) ){
		if(size_row > 1){
			j=0;
            v[i].x = (double *)calloc(4, sizeof(double));
            v[i].x[j] = atof(strtok(tab,","));
			for(j=1;j<4;j++){
				v[i].x[j]= atof(strtok(NULL,","));
    		}
			v[i].id= strdup(strtok(NULL,"\n"));
		}		 
  		i++;	 
	}
    fclose(f);
    vector_norm(v);
    return v;

}

int main (){
    vector_data *v; 
    v = recover("iris.data");
    // double x =;
    // printf("%f",sizeof(*v));
    normalize_vect(v);
    shuffert(v);
    display_vect(v);
    reseau r = remplire_node(v);
    printf("\n\n");
    display_node(r);


 return 0;
}