/*
 * Riccardo Rosin 284211
 * Lab 2 Es 2
*/
#include<stdio.h>
#include<stdlib.h>
#define fileIn "mat.txt"

int** leggiMatrice(int *nr, int *nc){
    int i, j, **mat;
    FILE *fin;
    if ((fin = fopen(fileIn, "r")) == NULL){
        printf("Errore nell'apertura del file: \"fileIn\"");
        return NULL;
    }
    
    fscanf(fin, "%d%d", nr, nc);

    mat = (int**) malloc( *nr * sizeof(int*));

    for(i = 0; i < *nr; i++){
        mat[i] = malloc( *nc * sizeof(int));
        for(j = 0; j < *nc; j++)
            fscanf(fin, " %d ",  &mat[i][j]);    
    }

    fclose(fin);
    return mat;
}

void stampa_matrice(int *M[], int r, int c){
    int i, j;
    for(i=0; i<r; i++){
        for(j=0; j<c; j++)
            printf("%d	", M[i][j]);
        printf("\n");
    }
}

void separa(int *mat[], int nr, int nc, int **bianche, int **nere, int *dimVett){
    int i,j, u = 0, v = 0;
    *dimVett = (nr * nc / 2) + 1;
    *bianche = (int*) malloc(sizeof(int) * i);
    *nere = (int*) malloc(sizeof(int) * i);


    // se somma posizioni pari è bianca, altrimenti è nera
    for(i = 0; i < nr; i++){
        for(j = 0; j < nc; j++){
            if((i + j) %2 == 0)
                //bianca
                (*bianche)[u++] = mat[i][j];
            else
                // nera
                (*nere)[v++] = mat[i][j];
        }
    }
    if(u<v) (*bianche)[u] = 0;
    else (*nere)[v] = 0;
 
}

void stampaVet(int vett[], int dim){
    for (int i = 0; i < dim; i++) printf("%d ", vett[i]);
    printf("\n");
}

void freeMem(int **mat, int nr){
    for(int i=0; i<nr; i++) free(mat[i]);
    free(mat);
}


int main(int argc, char* argv){
    int nr, nc, *bianche, *nere, dimVett;
    int **mat = leggiMatrice(&nr, &nc);
    stampa_matrice(mat, nr, nc);
    printf("\n");
    
    separa(mat, nr, nc, &bianche, &nere, &dimVett);
    
    stampaVet(bianche, dimVett);
    stampaVet(nere, dimVett);

    freeMem(mat, nr);
    free(bianche);
    free(nere);
    
    return 0;
}