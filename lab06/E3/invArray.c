#include "invArray.h"

/* ADT 1 tipo */
struct invArray_s {
    inv_t *vettInv;
    int nInv;      // spazio occupato
    int maxInv;        // spazio allocato
};

/* creatore e disruttore */
invArray_t invArray_init(){
    invArray_t invArray = malloc(sizeof(struct invArray_s));
    invArray->vettInv = malloc(sizeof(inv_t));
    invArray->nInv = 0;
    invArray->maxInv = 1;
    return invArray;
}

void invArray_free(invArray_t invArray){
    free(invArray->vettInv);
    free(invArray);
}

/* lettura e scrittura su file */
void invArray_read(FILE *fp, invArray_t invArray){
    int i;
    fscanf(fp, " %d ", &i);
    while(!feof(fp)){
        if(invArray->nInv == invArray->maxInv){ // se non c'è spazio rialloca
            invArray->maxInv *= 2; // riallocazione logaritmica
            invArray->vettInv = realloc(invArray->vettInv, invArray->maxInv * sizeof(inv_t));
        }
        inv_read(fp, &invArray->vettInv[invArray->nInv++]);
    }
}

void invArray_print(FILE *fp, invArray_t invArray){
    for(int i = 0; i < invArray->nInv; i++){
        inv_print(fp, &invArray->vettInv[i]);
        printf("\n");
    }
}

/* stampa un unico oggetto selezionato da indice (nel vettore) */
void invArray_printByIndex(FILE *fp, invArray_t invArray, int index){
    inv_print(fp, &invArray->vettInv[index]);
}

/* ritorna puntatore a oggetto selezionato da indice (nel vettore) */
inv_t* invArray_getByIndex(invArray_t invArray, int index){
    
    return &invArray->vettInv[index];
}

/* ritorna indice (nel vettore) a oggetto selezionato da nome */
int invArray_searchByName(invArray_t invArray, char *name){
    for(int i = 0; i < invArray->nInv; i++){
        if(strcmp(invArray->vettInv[i].nome, name) == 0){
            return i;
        }
    }
    return -1;
}