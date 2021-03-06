#include "som.h"

///////initialiser le réseau
void initialise_reseau(reseau *r,int nb_c,int nb_l,int col,int len,double alpha_initial,int iteration ){
    r->nb_c=nb_c;
    r->nb_l=nb_l;
    r->col=col;
    r->iteration=iteration;
    r->alpha_initial = alpha_initial;
////allouer les ligens de la map
    r->map = (node **)malloc(r->nb_l*sizeof(node*));
    if(r->map == NULL ){
        printf("map non alloué");
        exit(EXIT_FAILURE);
    }
////allouer le tableau de 150 bmu
    r->bmu=(bmu**)malloc(len*sizeof(bmu*));
    if(r->bmu == NULL ){
        printf("tableau bmu non alloué");
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<r->nb_l;i++){
////allouer les colonnes de la map      
        r->map[i] =(node *)malloc(r->nb_c*sizeof(node));
        if(r->map[i] == NULL ){
            printf("map[%d] non alloué",i);
            exit(EXIT_FAILURE);
        }
        for(int j=0;j<r->nb_c;j++){
/////allouer les 4 élements de chaque node             
            r->map[i][j].w = (double *)calloc(r->col, sizeof(double));
            if(  r->map[i][j].w == NULL ){
                printf("map[%d][%d] non alloué",i,j);
                exit(EXIT_FAILURE);
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
    printf("free bmu memoire\n");
    free(r->map);/// free les lignes de la map
    free(r);/// free le réseau
    printf("free réseau memoire\n");
}

//////remplir la map du réseau selon la moyenne calculée depuis les vecteurs de la base
void remplir_node(reseau *r , base_vector_data *base){
    double *tab=(double*)malloc(base->len *sizeof(double));
    if(tab == NULL){
        printf("errer au niveau de calcule du moyenne");
        exit(EXIT_FAILURE);
    }
    vector_moyenne(base,tab);
    for(int i=0;i<r->nb_l;i++){
        for(int j=0;j<r->nb_c;j++){
            for(int k=0;k<r->col ;k++){
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
            distance = distance_vector(v.x,r->map[l][c].w,r->col);
            if(distance <= min){
                if(distance < min){
                    bmu =temp ;      
                    bmu->i=l;
                    bmu->j=c;
                }
                else{
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
    return temp;
    
}

////////choisir un seul bmu de la lsite chainée
bmu * bmu_aleatoir(bmu * bu){
    int i = -1;
    bmu *temp = bu;
    while(bu != NULL){
        i++;
        bu = bu->next;
    }
    for(int j = 1;j<Rand_int(0,i);j++){
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

int calcule_rayon(reseau *r){
    int rayon = 0;
    int node_2 = r->nb_c * r->nb_l /2;
    for(rayon=0;node_2>0;rayon++)
    {
        node_2=node_2-rayon*8;
    }
    return rayon-1;
}

//////////////apprentissage
void apprentissage(reseau *r,base_vector_data *base){
    int *shuffle = (int*)malloc(base->len *sizeof(int));
    int phase_1 = r->iteration/4;
    int phase_2 = 3*phase_1;
    double alpha=0;
    int t =0;
    int t_total = phase_1;
    int rayon = calcule_rayon(r);
    int partie = t_total/rayon;
    remplire_tableau(shuffle,base->len);
    for(int iteration= 1; iteration<=r->iteration; iteration++){
        alpha = r->alpha_initial*(1-(double)(t/t_total));
        shuff(shuffle,base->len);/////melnager les indices
        for(int i=0;i<base->len;i++){
            r->bmu[i] = bmu_aleatoir(recuperer_BMU(r,base->v[shuffle[i]]));///sauvegarder le bmu choisi dans dans le réseau
            r->map[r->bmu[i]->i][r->bmu[i]->j].activate = distance_vector(base->v[shuffle[i]].x , r->map[r->bmu[i]->i][r->bmu[i]->j].w,base->col);//mettre la plus petite distance dans l'activation
            r->map[r->bmu[i]->i][r->bmu[i]->j].etiquet = base->v[shuffle[i]].id[8];///mise a jour de l'étiquet de bmu selon l'id du vecteur déclenchant
///// initialiser la nouvelle map pour changer le voisinage 
           for(int l=(int)fmax(0,r->bmu[i]->i-rayon);l<=(int)fmin(r->nb_l-1,r->bmu[i]->i+rayon);l++){
                for (int c = (int)fmax(0,r->bmu[i]->j-rayon); c <=(int)fmin(r->nb_c-1,r->bmu[i]->j+rayon); c++){
                    for(int k=0;k<r->col ;k++){
                        r->map[l][c].w[k] = r->map[l][c].w[k] + alpha * (base->v[shuffle[i]].x[k] - r->map[l][c].w[k]);
                    }
                } 
            }
        }
        t++;
////dimininuer le rayon
        if(t % partie == 0 && rayon>1){
            rayon--;
        }  
/////changer de phase      
        if(t == phase_1){
            r->alpha_initial=r->alpha_initial/10;
            t_total =phase_2;
            t=1;
        }
    }
    free(shuffle);//free le tableau de shuffer 
}