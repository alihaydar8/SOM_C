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

double distance_vector(double *x,double *w){
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
    bmu **bmu ;
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

reseau *remplire_node(vector_data *v){
    reseau *r = (reseau *)malloc(sizeof(reseau));
    r->nb_node=60;
    r->nb_c=6;
    r->nb_l=10;
    int size_col = 4;
    float *tab = vector_moyenne(v);
    node **map = (node **)malloc(9*sizeof(node));

    for(int i=0;i<r->nb_l;i++){
        map[i] =(node *)malloc(7*sizeof(node));
        for(int j=0;j<r->nb_c;j++){
            map[i][j].w = (double *)calloc(4, sizeof(double));
            for(int k=0;k<size_col;k++){
                map[i][j].w[k]=Rand_float(tab[k]-0.02,tab[k]+0.02);
            }
            map[i][j].activate = 0;
            map[i][j].etiquet ="Iris_xxxxx";
        }
    }
    r->map = map;
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

void display_node(reseau *r){
	int size_col = 4;
    for(int i=0;i<r->nb_l;i++){
        printf("node[%d]:",i);
        for(int j=0;j<r->nb_c;j++){
            for(int k=0;k<size_col;k++){
                printf("%.3f ",r->map[i][j].w[k]);
            }
            printf("%c ",r->map[i][j].etiquet[5]);
        }
        printf("\n\n");
    }
}

void display_node_carre(reseau *r){
	int size_col = 4;
    for(int i=0;i<r->nb_l;i++){
        printf("node[%d]:  ",i);
        for(int j=0;j<r->nb_c;j++){
            if(r->map[i][j].etiquet[8] == 'x'){
                printf("\033[0;30m");
                printf("%c ",r->map[i][j].etiquet[8]); 
                printf("\033[0m");
            }
            if(r->map[i][j].etiquet[8] == 's'){
                printf("\x1B[31m");
                printf("%c ",r->map[i][j].etiquet[8]); 
                printf("\x1B[0m"); 
            }
            if(r->map[i][j].etiquet[8] == 'g'){
                printf("\x1B[34m");
                printf("%c ",r->map[i][j].etiquet[8]); 
                printf("\x1B[0m");
            }
            if(r->map[i][j].etiquet[8] == 'o'){
                printf("\x1B[32m");
                printf("%c ",r->map[i][j].etiquet[8]); 
                printf("\x1B[0m");
            }
        }
        printf("\n");
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
    vector_data c ;
    int rand=0;
    for(int i=0;i<size_row;i++){
        rand =Rand_int(0,149);
        c = v[i];
        v[i]=v[rand];
        v[rand]=c;
    }
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

bmu *recuperer_BMU(reseau *r , vector_data v){
    double distance=0.0;
    double min =100.00;
    bmu *temp;
    bmu *bmu = malloc(sizeof(*bmu));
    temp=bmu;
    for(int l=0;l<r->nb_l;l++){
        for(int c=0;c<r->nb_c;c++ ){
            distance = distance_vector(v.x,r->map[l][c].w);
            // printf("%d %d %f\n",l,c,distance);
            if(distance <= min){
                if(distance < min){
                    bmu =temp ;      
                    bmu->i=l;
                    bmu->j=c;
                }
                if ( distance == min ) {
                    bmu->next=malloc(sizeof(*bmu));
                    bmu = bmu->next;
                    bmu->i = l;
                    bmu->j = c; 
                }
                min=distance;
                bmu->next=NULL;
            }
        }
    }           
    // printf("%d\n",d1);
    // printf("%d \n",d2);
    return temp;
    
}

bmu * bmu_aleatoir(bmu * bu){
    int i = 0;
    int j = 0;
    bmu *temp = bu;
    while(bu != NULL){
        i++;
        bu = bu->next;
    }
    for(int j = 1;j<Rand_int(1,i);j++){
        temp = temp->next;
    }
    temp->next=NULL;
    return temp;
}

void affichage_bmu(bmu *bmu){
    while(bmu != NULL){
        printf("%d",bmu->i);
        printf("%d",bmu->j);
        printf("\t");
        bmu = bmu->next;
        }
}

int main (){
    double alpha_initial = Rand_float(0.7,0.9);
    double alpha=0;
    int t_total = 500;
    int t=1;
    vector_data *v;
    v = recover("iris.data");
    // double x =;
    // printf("%f",sizeof(*v));
    normalize_vect(v);
    shuffert(v);
    display_vect(v);
    reseau *r = remplire_node(v);
    printf("\n\n");
    display_node_carre(r);
    r->bmu=(bmu**)malloc(150*sizeof(bmu));
    
    // for(int i=0;i<150;i++){
    //     affichage_bmu(bmu_aleatoir(recuperer_BMU(r,v[i])));
    //     printf("\n");
    // }

    for(int iteration= 1; iteration<=2000 ; iteration++){
        alpha = alpha_initial*(1-(double)(t/t_total));
       for(int i=0;i<150;i++){
            r->bmu[i] = bmu_aleatoir(recuperer_BMU(r,v[i]));
            r->map[r->bmu[i]->i][r->bmu[i]->j].activate = distance_vector(v[i].x , r->map[r->bmu[i]->i][r->bmu[i]->j].w);//mettre la plus petite distance dans l'activation
            r->map[r->bmu[i]->i][r->bmu[i]->j].etiquet = v[i].id;
            for(int l=0;l<r->nb_l;l++){
                for (int c = 0; c < r->nb_c; c++){
                    if(((r->bmu[i]->i-3)<l<(r->bmu[i]->i+3)) && ((r->bmu[i]->j-3)<c<(r->bmu[i]->j+3))){
                        for(int k=0;k<4;k++){
                            r->map[l][c].w[k]=r->map[l][c].w[k]+ alpha*(v[i].x[k] - r->map[l][c].w[k]);
                        }
                    }
                } 
            }
        }
        t++;
        if(iteration==400){display_node_carre(r);
        printf("\n");}
        if(iteration==800){display_node_carre(r);
        printf("\n");}
        if(iteration==1200){display_node_carre(r);
        printf("\n");}
        if(iteration==1600){display_node_carre(r);
        printf("\n");}
        if(iteration==500){
            alpha=Rand_float(0.07,0.09);
            t_total =1500;
            t=1;
        }
    }
    display_node_carre(r);
     
 return 0;
}



