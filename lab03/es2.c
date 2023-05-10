/*
 * Riccardo Rosin 284211
 * Lab 3 Es 2
*/
#include<stdio.h>
#include<stdlib.h>
#define fileIn "brani.txt"
#define lenStr (255+1)

typedef struct dato{
    int n;
    char **canzoni;
} dato;

int init(dato** elementi, int* n, int** arr){
    // inizializza tutti i vettori e legge dal file i valori
    int i, j;
    char tmp[lenStr];
    FILE *fin;
    if ((fin = fopen(fileIn, "r")) == NULL){
        printf("Errore nell'apertura del file: \"fileIn\"");
        return -1;
    }
    
    if(fscanf(fin, " %d ", n) != 1) return -1;
    if(((*elementi) = (dato*) malloc(*n * sizeof(dato))) == NULL) return -1;    // alloca vettore di struct
    if(((*arr) = (int*) malloc(*n * sizeof(int))) == NULL) return -1;

    for(i=0; i<*n; i++){ // per il numero di amici
        fscanf(fin, " %d ", &(*elementi)[i].n); // leggo numero canzoni e alloco memoria in riga successiva
        if(((*elementi)[i].canzoni = (char**) malloc((*elementi)[i].n * sizeof(char*))) == NULL) return -1;
        for(j=0; j<(*elementi)[i].n; j++){ // per il numero di canzoni per l'amico selezionato
            if(((*elementi)[i].canzoni[j] = (char*) malloc(lenStr * sizeof(char))) == NULL) return -1;
            fscanf(fin, " %s ", (*elementi)[i].canzoni[j]);
        }
    }

    fclose(fin);
    return 0;
}

void libera(dato** elementi, int n, int** arr){
    // libero tutti gli array dinamici
    int i,j;
    for(i=0;i<n;i++){
        for(j=0; j<(*elementi)[i].n; j++)
            free((*elementi)[i].canzoni[j]);
        free((*elementi)[i].canzoni);
    }
    free(*elementi);
    free(*arr);
}

void stampa(FILE* stream, dato* elementi, int* arr, int n){
    // stampo la combinazione trovata
    for(int i=0; i<n; i++)
        fprintf(stream,"%s ", elementi[i].canzoni[arr[i]]);
    fprintf(stream,"\n");
}

// a -> numero amico
void rico(FILE* stream, dato* elementi, int* arr, int a, int n){
    // ricorro sugli amici e sulle canzoni per troare le combinazioni
    if(a == n){stampa(stream, elementi, arr, n); return;}
    int i;
    for(i=0; i<elementi[a].n; i++){
        arr[a] = i;
        rico(stream, elementi, arr,  a+1, n);
    }
}

void stampaTutto(dato *elementi, int n){
    // stampo i dati di input in un formato simile a quello del file d'ingresso
    int i, j;
    printf("%d\n", n);
    for(i=0; i<n; i++){
        printf("\t%d\n", elementi[i].n);
        for(j=0; j<elementi[i].n; j++)
            printf("\t\t%s\n", elementi[i].canzoni[j]);
    }
    printf("\n");
}

int main(int argc, char* argv){
    dato* elementi;
    int n, *arr;
    if(init(&elementi, &n, &arr) == -1) return -1;
    // stampaTutto(elementi, n);
    rico(stdout, elementi, arr, 0, n);
    libera(&elementi, n, &arr);
    return 0;
}