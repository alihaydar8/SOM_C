#include "som.h"

////initialise la base 
void initialise_base(base_vector_data *base){
    base->len=150;
    base->col=4;
    base->v =  (vector_data *)calloc(base->len, sizeof(vector_data));
    if(base->v==NULL){
        printf("null");
    }
    for(int i=0;i<base->len;i++){
        base->v[i].x = (double *)calloc(base->col, sizeof(double));
    }

}

/////calculer la distance de chaque vecteur
double distance_vector(double *x,double *w){
    double distance = 0.0;
    for (int i = 0; i < 4;i++){
        distance = distance + (x[i] - w[i]) * (x[i] - w[i]);
    }
    distance = sqrt(distance);
    return distance;
}

//////Calculer les quatres moyennes
float *vector_moyenne(base_vector_data *base){
    float *tab =(float*)malloc(4*sizeof(float));
    for(int j=0;j<base->col;j++){
        for(int i = 0;i<base->len;i++){
            tab[j]= tab[j] + base->v[i].x[j];
        }
        tab[j]=tab[j]/base->len;
    }   
    return tab;
}

///////////calculer la norme des vecteurs de donnes 
void vector_norm(base_vector_data *base){
    for(int i=0;i<base->len;i++){
        for(int j=0;j<base->col;j++){
            base->v[i].norm=base->v[i].norm+ base->v[i].x[j]*base->v[i].x[j];
        }
        base->v[i].norm = sqrt(base->v[i].norm);
    }
}

///////////afficher les vecteurs 
void display_vect(base_vector_data *base){
    
    for(int i=0;i<base->len;i++){
        printf("vecteur %d  ",i);
        for(int j=0;j<base->col;j++){
           printf("%f ",base->v[i].x[j]);
        }
        printf("%s ",base->v[i].id);
        printf("%f",base->v[i].norm);
        printf("\n");
    }
}

//////////normaliser le vecteur
void normalize_vect(base_vector_data *base){
    for(int i=0;i<base->len;i++){
        for(int j=0;j<base->col;j++){
            base->v[i].x[j]=base->v[i].x[j]/base->v[i].norm;
        }
    }   
}

///////////melanger les indice du tableau des donner 
int * shuf(int * tab){
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

//////// sauvgarder les donnees dans la base
void recover(base_vector_data *base,char *nom_fichier){
    int i = 0;
    int j=0; 
    char string[60];
    FILE *file= fopen( nom_fichier, "r" );
    
    if(!file){
        printf("fichier n'existe pas");
    }
    
    fseek(file,0,SEEK_SET);
	while((fgets(string,60,file)!=NULL) && (i<base->len) ){
		if(base->len > 1){
			j=0;
            base->v[i].x[j] = atof(strtok(string,","));
			for(j=1;j<4;j++){
				base->v[i].x[j]= atof(strtok(NULL,","));
    		}
			base->v[i].id= strdup(strtok(NULL,"\n"));
		}		 
  		i++;	 
	}
    fclose(file);
    vector_norm(base);
    normalize_vect(base);
}