#include<stdio.h>
#include<stdlib.h>
#define FILE_IN "val1.txt"


void init_file(int** arr, int* dim){
    FILE *fin;
    if ((fin = fopen(FILE_IN, "r+")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", FILE_IN);
        exit(-1);
    }
    
    fscanf(fin, " %d ", dim);
    *arr = malloc((*dim) * sizeof(int));
    for(int i = 0; i < *dim; i++)
        fscanf(fin, " %d ", &(*arr)[i]);
    
    fclose(fin);
}

void stampa_array(int* arr, int dim){
    for(int i = 0; i <dim ; i++)
        printf("%5d\t", arr[i]);
    printf("\n");
}

void stampa_antenne(int* indixs, int* antenne, int i){
    if(indixs[i] == -1){
        if(antenne[i])
            printf("%4d\t", i);
        return;
    }
    
    stampa_antenne(indixs, antenne, indixs[i]);
    if(antenne[i])
        printf("%4d\t", i);
}

void calcola(int* arr, const int dim){

    int opt[dim+1], indixs[dim+1], antenne[dim+1], i, tmp;

    opt[0] = 0;
    opt[1] = arr[0];
    indixs[0] = -1;
    indixs[1] = -1;
    antenne[0] = 1;
    antenne[1] = 1;

    for(i = 2; i < dim+1; i++){
        if((tmp = arr[i-1] + opt[i-2]) > opt[i-1]){ // insierisco la città in i
            opt[i] = tmp;
            indixs[i] = i-2;
            antenne[i] = 1;
        }else{
            opt[i] = opt[i-1];
            indixs[i] = i-1;
            antenne[i] = 0;
        }
    }

    printf("\t");
    stampa_array(arr, dim);
    stampa_array(opt, dim+1);
    stampa_array(indixs, dim+1);
    stampa_array(antenne, dim+1);

    printf("\n");
    stampa_antenne(indixs, antenne, dim);

}

int main(int argc, char* argv[]){
    
    int* arr, dim;
    init_file(&arr, &dim);

    calcola(arr, dim);

    free(arr);

    return 0;
}