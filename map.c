#include "som.h"

reseau *remplire_node(vector_data *v){
    reseau *r = (reseau *)malloc(sizeof(reseau));
    r->nb_node=60;
    r->nb_c=7;
    r->nb_l=8;
    int size_col = 4;
    float *tab = vector_moyenne(v);
    node **map = (node **)malloc(r->nb_l*sizeof(node));

    for(int i=0;i<r->nb_l;i++){
        map[i] =(node *)malloc(r->nb_c*sizeof(node));
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
    for(int i=0;i<r->nb_l;i++){
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

void apprentisage(reseau *r,vector_data *v){
    int *sh = (int*)malloc(150*sizeof(int));
    double alpha_initial = 0.9;
    double alpha=0;
    int t_total = 500;
    int t=1;
    int rr=3;
    for(int iteration= 1; iteration<=2000; iteration++){
        alpha = alpha_initial*(1-(double)(t/t_total));
        // shuffert(v);
        sh = shuf(sh);
        // for(int i =0; i<150;i++){
        //     printf("%d:%d\n",i,sh[i]);
        // }
        for(int i=0;i<150;i++){
            r->bmu[i] = bmu_aleatoir(recuperer_BMU(r,v[sh[i]]));
            r->map[r->bmu[i]->i][r->bmu[i]->j].activate = distance_vector(v[sh[i]].x , r->map[r->bmu[i]->i][r->bmu[i]->j].w);//mettre la plus petite distance dans l'activation
            r->map[r->bmu[i]->i][r->bmu[i]->j].etiquet = v[sh[i]].id;
            for(int l=0;l<r->nb_l;l++){
                for (int c = 0; c < r->nb_c; c++){
                    if(  ((r->bmu[i]->i-rr)<=l)  && (l<=(r->bmu[i]->i+rr)) && ((r->bmu[i]->j-rr)<=c) && (c<=(r->bmu[i]->j+rr)) ){
                        for(int k=0;k<4;k++){
                            r->map[l][c].w[k] = r->map[l][c].w[k] + alpha * (v[sh[i]].x[k] - r->map[l][c].w[k]);
                        }
                    }
                } 
            }
        }
        if(iteration == 170){
            // rr = 2;
        }
        if(iteration == 320){
            // rr = 1;
        }
        // if(iteration==400){display_node_carre(r);
        // printf("\n");}
        // if(iteration==800){display_node_carre(r);
        // printf("\n");}
        // if(iteration==1200){display_node_carre(r);
        // printf("\n");}
        // if(iteration==1600){display_node_carre(r);
        // printf("\n");}
         
        t++;
        if(iteration==500){
            alpha_initial=0.07;
            t_total =1500;
            t=1;
            rr=1;
        }
    }
}