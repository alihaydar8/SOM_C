#include "som.h"

////initialiser la base de donnée
void initialise_base(base_vector_data *base,int len,int col){
    base->len=len;
    base->col=col;
/////allouer 150 vecteurs dans la base
    base->v =  (vector_data *)calloc(base->len, sizeof(vector_data));
    if(base->v==NULL){
        printf("tableau de vecteur non alloué");
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<base->len;i++){
/////allouer les 4 elements dans chaque vecteur  
        base->v[i].x = (double *)calloc(base->col, sizeof(double));
        if(base->v[i].x == NULL){
            printf("vecteur[%d] non alloué",i);
            exit(EXIT_FAILURE);
        }
/////allouer l'id dans chaque vecteur
        base->v[i].id = (char*)malloc(20*sizeof(char));
        if(base->v[i].id == NULL){
            printf("vecteur[%d].id non alloué",i);
            exit(EXIT_FAILURE);
        }
    }
}

////free base
void free_base(base_vector_data *base){
    for(int i=0;i<base->len;i++){
        free(base->v[i].id);///free id
        free(base->v[i].x);///free 4 élements 
    }
    free(base->v);///free 150 vecteurs 
    free(base);////free base 
    printf("free base memoire\n");
}

/////calculer la distance de chaque vecteur
//// une fonction qui prends deux tableaux et renvoie la distance eucludienne 
double distance_vector(double *x,double *w,int col){
    double distance = 0.0;
    for (int i = 0; i <col;i++){
        distance = distance + pow((x[i] - w[i]),2);
    }
    distance = sqrt(distance);
    return distance;
}

//////Calculer les quatre moyennes
/////fonction qui recoit la base et renvoie les quatres moyennes 
void vector_moyenne(base_vector_data *base,double *tab){
    for(int j=0;j<base->col;j++){
        for(int i = 0;i<base->len;i++){
            tab[j]= tab[j] + base->v[i].x[j];
        }
        tab[j]=tab[j]/base->len;
    }  
}

///////////calculer la norme des vecteurs de données et la sauvgarder dans la base 
void vector_norm(base_vector_data *base){
    for(int i=0;i<base->len;i++){
        for(int j=0;j<base->col;j++){
            base->v[i].norm=base->v[i].norm+ pow(base->v[i].x[j],2);
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

//////////fonction qui change la valeur des élemenets par leur valeur normalisée  
void normalize_vect(base_vector_data *base){
    for(int i=0;i<base->len;i++){
        for(int j=0;j<base->col;j++){
            base->v[i].x[j]=base->v[i].x[j]/base->v[i].norm;
        }
    }   
}


void remplire_tableau(int * tab,int size_row){
    for(int i=0;i<size_row;i++){////////modifier
        tab[i]=i;
    }
}

///////////fonction qui melange les indices du tableau, elle prend un tableau des indices et rend le tableau melangé
void shuff(int * tab,int size_row){
    int rand = 0;
    int c;
    srand(time(NULL));
    for(int i=0;i<size_row;i++){
        rand =Rand_int(0,size_row-1);
        c = tab[i];
        tab[i] = tab[rand];
        tab[rand]=c;
    }
}

//////// sauvgarder les données dans la base
void recover(base_vector_data *base,char *nom_fichier){
    int i = 0;
    char string[60];
    FILE *file= fopen( nom_fichier, "r" );
    if(!file){
        printf("fichier n'existe pas");
        exit(EXIT_FAILURE);
    }
	while((fgets(string,60,file)!=NULL) && (i<base->len) ){
            base->v[i].x[0] = strtod(strtok(string,","),NULL);/////la de double srtod
			for(int j=1;j<base->col;j++){
				base->v[i].x[j]= strtod(strtok(NULL,","),NULL);
    		}
			base->v[i].id= strdup(strtok(NULL,"\n"));
  		i++;	 
	}
    fclose(file);
    vector_norm(base);
    normalize_vect(base);
}