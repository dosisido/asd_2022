#include "invArray.h"

/* ADT 1 tipo */
struct invArray_s {
    inv_t *array;
    int index;      // spazio occupato
    int max;        // spazio allocato
};

/* creatore e disruttore */
invArray_t invArray_init(){
    invArray_t invArray = malloc(sizeof(struct invArray_s));
    invArray->array = malloc(sizeof(inv_t));
    invArray->index = 0;
    invArray->max = 1;
    return invArray;
}

void invArray_free(invArray_t invArray){
    free(invArray->array);
    free(invArray);
}

/* lettura e scrittura su file */
void invArray_read(FILE *fp, invArray_t invArray){
    int i;
    fscanf(fp, " %d ", &i);
    while(!feof(fp)){
        if(invArray->index == invArray->max){ // se non c'è spazio rialloca
            invArray->max *= 2; // riallocazione logaritmica
            invArray->array = realloc(invArray->array, invArray->max * sizeof(inv_t));
        }
        inv_read(fp, &invArray->array[invArray->index++]);
    }
}

void invArray_print(FILE *fp, invArray_t invArray){
    for(int i = 0; i < invArray->index; i++){
        inv_print(fp, &invArray->array[i]);
        printf("\n");
    }
}

/* stampa un unico oggetto selezionato da indice (nel vettore) */
void invArray_printByIndex(FILE *fp, invArray_t invArray, int index){
    inv_print(fp, &invArray->array[index]);
}

/* ritorna puntatore a oggetto selezionato da indice (nel vettore) */
inv_t* invArray_getByIndex(invArray_t invArray, int index){
    
    return &invArray->array[index];
}

/* ritorna indice (nel vettore) a oggetto selezionato da nome */
int invArray_searchByName(invArray_t invArray, char *name){
    for(int i = 0; i < invArray->index; i++){
        if(strcmp(invArray->array[i].nome, name) == 0){
            return i;
        }
    }
    return -1;
}