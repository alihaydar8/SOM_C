#include "som.h"
int COL =4;
int ROW =150;

///////initialiser le réseau
void initialise_reseau(reseau *r){
    r->nb_c=6;
    r->nb_l=10;
////allouer les ligens de la map
    r->map = (node **)malloc(r->nb_l*sizeof(node));
    if(r->map == NULL ){
        printf("map non alloué");
    }
////allouer le tableau de 150 bmu
    r->bmu=(bmu**)malloc(ROW*sizeof(bmu));
    if(r->bmu == NULL ){
        printf("tableau bmu non alloué");
    }
    for(int i=0;i<r->nb_l;i++){
////allouer les colonnes de la map      
        r->map[i] =(node *)malloc(r->nb_c*sizeof(node));
        if(r->map[i] == NULL ){
            printf("map[%d] non alloué",i);
        }
        for(int j=0;j<r->nb_c;j++){
/////allouer les 4 élements de chaque node             
            r->map[i][j].w = (double *)calloc(COL, sizeof(double));
            if(  r->map[i][j].w == NULL ){
                printf("map[%d][%d] non alloué",i,j);
            }
        }
    }
}
/////free réseau
void free_reseau(reseau *r){
    for(int i=0;i<r->nb_l;i++){
        for(int j=0;j<r->nb_c;j++){          
            free(r->map[i][j].w);///free les 4 élements
        }
        free(r->map[i]);/// free les colonnes de la map
    }
    free(r->bmu);/// free les 150 bmu
    free(r->map);/// free les lignes de la map
    free(r);/// free le réseau
    printf("réseau free memoire\n");
}

//////remplir la map du réseau selon la moyenne calculée depuis les vecteurs de la base
void remplire_node(reseau *r , base_vector_data *base){
    double *tab=(double*)malloc(COL*sizeof(double));
    if(tab == NULL){
        printf("errer au niveau de calcule du moyenne");
    }
    vector_moyenne(base,tab);
    for(int i=0;i<r->nb_l;i++){
        for(int j=0;j<r->nb_c;j++){
            for(int k=0;k<COL;k++){
                r->map[i][j].w[k]=Rand_float(tab[k]-0.02,tab[k]+0.02);
            }
            r->map[i][j].activate = 0;
            r->map[i][j].etiquet ='x';
        }
    }
    free(tab);
}

///afficher la map model
void display_node_carre(reseau *r){
    for(int i=0;i<r->nb_l;i++){
        // printf("%d ",i );
        for(int j=0;j<r->nb_c;j++){
            if(r->map[i][j].etiquet == 'x'){
                printf("\033[0;30m");
                printf("%c ",r->map[i][j].etiquet); 
                printf("\033[0m");
            }
            if(r->map[i][j].etiquet == 's'){
                printf("\x1B[31m");
                printf("%c ",r->map[i][j].etiquet); 
                printf("\x1B[0m"); 
            }
            if(r->map[i][j].etiquet == 'g'){
                printf("\x1B[34m");
                printf("%c ",r->map[i][j].etiquet); 
                printf("\x1B[0m");
            }
            if(r->map[i][j].etiquet == 'o'){
                printf("\x1B[35m");
                printf("%c ",r->map[i][j].etiquet); 
                printf("\x1B[0m");
            }
        }
        printf("\n");
    }
}

////afficher le nombre de chaque fleur calculée par le model 
void display_info(reseau *r){
    int g=0;
    int s=0;
    int o=0;
    for(int i=0;i<r->nb_l;i++){
        for(int j=0;j<r->nb_c;j++){
            if(r->map[i][j].etiquet == 's'){
                s++;
            }
            if(r->map[i][j].etiquet == 'g'){
                g++;
            }
            if(r->map[i][j].etiquet == 'o'){
                o++;
            }
        }
    }
    printf("\n");
    
    printf("\x1B[34m");
    printf("g pour Iris-virginica(%d)  |",g);
    printf("\x1B[0m");

    printf("\x1B[31m");
    printf("|  S pour Iris-versicolor(%d)  |",s); 
    printf("\x1B[0m");

    printf("\x1B[35m");
    printf("|  o pour Iris-setosa(%d)  |",o);
    printf("\x1B[0m");
    
    printf("\n");
}

//////trouver tous les bmu qui sont associés au meme vecteur et les sauvegarder dans  une liste chainée
bmu *recuperer_BMU(reseau *r , vector_data v){
    double distance=0.0;
    double min =100.00;
    bmu *temp;
    bmu *bmu = malloc(sizeof(*bmu));
    temp=bmu;
    for(int l=0;l<r->nb_l;l++){
        for(int c=0;c<r->nb_c;c++ ){
            distance = distance_vector(v.x,r->map[l][c].w);
            // printf("[%d][%d]: %f\n",l,c,distance);
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

////////choisir un seul bmu de la lsite chainée
bmu * bmu_aleatoir(bmu * bu){
    int i = 0;
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

//////afficher la liste chainée des bmu
void affichage_bmu(bmu *bmu){
    while(bmu != NULL){
        printf("%d",bmu->i);
        printf("%d",bmu->j);
        printf("\t");
        bmu = bmu->next;
        }
}

//////////////apprentissage
void apprentissage(reseau *r,base_vector_data *base){
    int *shuffle = (int*)malloc(base->len *sizeof(int));
    int iteartion_total = 2000;
    double alpha_initial = 0.8;
    double alpha=0;
    int t_total = iteartion_total/4;
    int t=1;
    int rayon=3;
    for(int iteration= 1; iteration<=2000; iteration++){
        alpha = alpha_initial*(1-(double)(t/t_total));
        shuff(shuffle);/////melnager les indices
        for(int i=0;i<base->len;i++){
            r->bmu[i] = bmu_aleatoir(recuperer_BMU(r,base->v[shuffle[i]]));///sauvegarder le bmu choisi dans dans le réseau
            r->map[r->bmu[i]->i][r->bmu[i]->j].activate = distance_vector(base->v[shuffle[i]].x , r->map[r->bmu[i]->i][r->bmu[i]->j].w);//mettre la plus petite distance dans l'activation
            r->map[r->bmu[i]->i][r->bmu[i]->j].etiquet = base->v[shuffle[i]].id[8];///mise a jour de l'étiquet de bmu selon l'id du vecteur déclenchant
///// initialiser la nouvelle map pour changer le voisinage 
           for(int l=(int)fmax(0,r->bmu[i]->i-rayon);l<=(int)fmin(r->nb_l-1,r->bmu[i]->i+rayon);l++){
                for (int c = (int)fmax(0,r->bmu[i]->j-rayon); c <=(int)fmin(r->nb_c-1,r->bmu[i]->j+rayon); c++){
                    for(int k=0;k<4;k++){
                         r->map[l][c].etiquet = base->v[shuffle[i]].id[8];
                        r->map[l][c].w[k] = r->map[l][c].w[k] + alpha * (base->v[shuffle[i]].x[k] - r->map[l][c].w[k]);
                    }
                } 
            }
        }
////dimininuer le rayon
        if(iteration == (int)iteartion_total/(4*3) || iteration == (int)iteartion_total/(2*3)){
            rayon--;
        }  
        t++;
/////changer de phase      
        if(iteration == iteartion_total/4){
            alpha_initial=alpha_initial/10;
            t_total =3*iteartion_total/4;
            t=1;
        }
    }
    free(shuffle);//free le tableau de shuffer 
}