#include "som.h"

double distance_vector(double *x,double *w){
    double distance = 0.0;
    for (int i = 0; i < 4;i++){
        distance = distance + (x[i] - w[i]) * (x[i] - w[i]);
    }
    distance = sqrt(distance);
    return distance;
}


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

// void shuffert(vector_data *v){
//     int size_row = 150;
//     vector_data c ;
//     int rand=0;
//     for(int i=0;i<size_row;i++){
//         rand =Rand_int(0,149);
//         c = v[i];
//         v[i]=v[rand];
//         v[rand]=c;
//     }
// }

int * shuf(int * tab){
// int *tab = (int*)malloc(150*sizeof(int));
int size_row = 150;
int rand = 0;
int c;
srand(time(NULL));
for(int i=0;i<size_row;i++){
    tab[i]=i;
}
for(int i=0;i<size_row;i++){
    rand =Rand_int(0,149);
    c = tab[i];
    tab[i] = tab[rand];
    tab[rand]=c;
}
return tab;
}

vector_data * recover(char *nom_fichier){
    int i = 0;
    int j=0; 
    char string[60];
    int size_row = 150;
    FILE *file= fopen( nom_fichier, "r" );
    vector_data *v;
    if(!file){
        printf("fichier n'existe pas");
    }
    v = (vector_data *)calloc(150, sizeof(vector_data));
    if(v==NULL){
        printf("null");
    }
    fseek(file,0,SEEK_SET);
	while((fgets(string,60,file)!=NULL) && (i<size_row) ){
		if(size_row > 1){
			j=0;
            v[i].x = (double *)calloc(4, sizeof(double));
            v[i].x[j] = atof(strtok(string,","));
			for(j=1;j<4;j++){
				v[i].x[j]= atof(strtok(NULL,","));
    		}
			v[i].id= strdup(strtok(NULL,"\n"));
		}		 
  		i++;	 
	}
    fclose(file);
    vector_norm(v);
    return v;
}